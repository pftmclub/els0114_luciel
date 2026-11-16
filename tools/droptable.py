#!/usr/bin/env python3
"""Offline field / dungeon rewards (plan field-rewards, OFFLINE_FIELD_REWARDS_PHASES.md).

  python tools/droptable.py census [--out FILE] [--gaps] [--quiet]
  python tools/droptable.py explain <dungeon id | field id>        one key's rows, roles and marks
  python tools/droptable.py summary                                reads the census file: the in-scope groups, roles, none / box NPCs

census (R0, read-only): one row per (dungeon id incl. difficulty, or field id) x stage x sub-stage x spawned NPC with the NPC's level,
spawn count, role and the script marks that decided it, and the reward-row state against the server tables
(`specific` = a row under the key, `wildcard` = only the key-0 row, `none`). Writes tools/data/drop_census.tsv. The scripts are walked by
june2017_re/dungeon_npc_dump.lua (`marks` mode) and june2017_re/lua_registry_dump.lua (`deep` mode), both run in the KOG luajit; the
registry and enum reads reuse june2017_re/dungeon_registry_census.py. Roles (a script mark decides, never the NPC's name):

  dungeon  box         the NPC's templet class is NCT_THING_BOX
           none        NO_DROP on every entry, or a templet class NCT_THING_* other than a box (gate, trap, device, checker ...)
           final-boss  HAVE_BOSS_GAGE / SHOW_BOSS_NAME / MONSTER_GRADE boss in the last stage, or a CLEAR_COND kill target of its last sub-stage
           stage-boss  the same marks in an earlier stage
           mob-minor   a mob whose sub-stage NPC_REWARD_RATE row is below 1
           mob         every other spawned NPC
           unknown     no templet row, or an NPC_ID the walker could not resolve (listed for R1, never guessed)
  field    field-boss  its spawn group has IS_BOSS_MONSTER_GROUP, or is named by the field's BATTLE_FIELD_MIDDLE_BOSS_INFO
           box / none  as above, and DROP = FALSE is none
           field-mob   DROP = TRUE
Later phases add: pools, gen, check, simulate.
"""
import argparse
import collections
import os
import re
import subprocess
import sys

TRUNK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(TRUNK)                       # june2017_re/dungeon_registry_census.py reads os.getcwd() at import
sys.path.insert(0, os.path.join(TRUNK, "tools"))
sys.path.insert(0, os.path.join(TRUNK, "june2017_re"))

LJ_DIR = os.path.join(TRUNK, "elstools", "vendor", "luajit")
NPC_DUMP = os.path.join(TRUNK, "june2017_re", "dungeon_npc_dump.lua")
REG_DUMP = os.path.join(TRUNK, "june2017_re", "lua_registry_dump.lua")
DATA = os.path.join(TRUNK, "tools", "data")
OUT_DEFAULT = os.path.join(DATA, "drop_census.tsv")
SERVER_DIR = os.path.join(TRUNK, "KncWX2Server", "ServerResource", "US")

# The plan's in-scope keys (OFFLINE_FIELD_REWARDS_PHASES.md "Measurement"): phase -> (label, dungeon keys, field keys).
SCOPE = [
    ("R3", "Lanox dungeons", list(range(33270, 33323)), []),
    ("R3", "Lanox fields", [], [40029, 40030, 40031, 40032]),
    ("R4", "Elysion dungeons", list(range(33330, 33353)) + list(range(33870, 33903)), []),
    ("R4", "Elysion fields", [], [40035, 40036, 40037]),
    ("R5", "Secret dungeons", [33260, 33261, 41270, 41271], []),
]

COLUMNS = ["scope", "key", "key_name", "diff", "script", "stage", "sub", "npc_id", "npc", "level", "count", "form", "role", "marks",
           "state", "gap"]
BOSS_GRADES = ("MG_BOSS_NPC", "MG_MIDDLE_BOSS_NPC")


# --- luajit ---------------------------------------------------------------------------------------------------------------------
def lj(script, *args):
    r = subprocess.run([os.path.join(LJ_DIR, "luajit.exe"), script] + [str(a) for a in args], capture_output=True,
                       stdin=subprocess.DEVNULL, timeout=600, cwd=LJ_DIR)
    return r.stdout.decode("utf-8", "replace").splitlines()


def kv(parts):
    d = {}
    for p in parts:
        k, _, v = p.partition("=")
        d[k] = v
    return d


def calls(path, mode="calls"):
    """-> [(method, {k: v}, [(key, index, {k: v})])]: every g_pX:Method( { } ) call of a registry file; `deep` adds its table rows."""
    out = []
    for ln in lj(REG_DUMP, mode, path):
        p = ln.split("\t")
        if p[0] == "E":
            raise SystemExit("dump %s: %s" % (path, ln))
        if p[0] == "C":
            out.append((p[1], kv(p[2:]), []))
        elif p[0] == "I" and out:
            out[-1][2].append((p[1], p[2], kv(p[3:])))
    return out


def enum_names(table, path):
    import dungeon_registry_census as drc
    return drc.enum(path, table, optional=True)


def nui(v):
    return v[4:] if v and v.startswith("NUI:") else None


def kc(kom_dir, name):
    import kom
    return os.path.join(kom.KOM_ROOT, kom_dir + "_kom", name)


# --- server rows ----------------------------------------------------------------------------------------------------------------
def server_rows():
    """-> (monster rows {(dungeon id, npc id): row}, field rows {(field id, npc id): row}, static keys {(kind, id)}). Read from the
    staged data036 files, the ones the loader mounts; the loose KncWX2Server copies must hold the same keys (checked by `census`)."""
    return rows_of(kc("data036", "DropTable.lua"), kc("data036", "StaticDropTable.lua"))


def rows_of(drop_path, static_path):
    mon, fld, static = {}, {}, set()
    for meth, f, _ in calls(drop_path):
        if meth == "AddMonsterDropInfo":
            mon[(int(f["DungeonID"]), int(f["MonsterID"]))] = f
        elif meth == "AddBattleFieldNpcDropInfo":
            fld[(int(f["BattleFieldID"]), int(f["MonsterID"]))] = f
    for meth, f, _ in calls(static_path):
        if meth == "AddStaticDropInfo":
            if "BattleFieldID" in f:
                static.add(("field", int(f["BattleFieldID"])))
            else:
                static.add(("dungeon", int(f["DungeonID"])))
    return mon, fld, static


def state_of(rows, key, npc):
    if (key, npc) in rows:
        return "specific"
    if (0, npc) in rows:
        return "wildcard"
    return "none"


# --- registries -----------------------------------------------------------------------------------------------------------------
class Reg:
    def __init__(self):
        import dungeon_registry_census as drc
        import kom
        self.kom = kom
        enum = kc("data036", "Enum.lua")
        self.npc = drc.enum(enum, "NPC_UNIT_ID")              # NUI_X -> id
        self.npc_by_id = {}
        for n, v in self.npc.items():
            self.npc_by_id.setdefault(v, n)
        self.village = drc.enum(enum, "VILLAGE_MAP_ID")
        self.village_by_id = {v: n for n, v in self.village.items()}
        de = kc("data036", "DungeonEnum.lua")
        self.dungeon_enum = drc.enum(de, "DUNGEON_ID")
        self.unused_enum = drc.enum(de, "UNUSED_DUNGEON_ID", optional=True)
        self.dungeon_name = {v: n for n, v in self.dungeon_enum.items()}
        for n, v in self.unused_enum.items():
            self.dungeon_name.setdefault(v, "UNUSED:" + n)
        self.dd, self.dd_unres, _ = drc.registry(kc("data035", "DungeonData.lua"), self.dungeon_enum, self.unused_enum)
        self.staged = kom.staged_names()
        self.templets = {}
        for meth, f, _ in calls(kc("data037", "NPCTemplet.lua")):
            n = nui(f.get("NPC_ID"))
            if meth == "AddNPCTemplet" and n and n not in self.templets:     # the first row for an id wins
                self.templets[n] = f

    def templet(self, name):
        t = self.templets.get(name)
        if t is None and name in self.npc:
            for alt, v in self.npc.items():           # AddNPCTemplet_LUA is keyed by value: another name with the same id
                if v == self.npc[name] and alt in self.templets:
                    return self.templets[alt]
        return t

    def script_path(self, name):
        hit = self.staged.get(name.lower())
        if not hit:
            return None
        stem, spelling = sorted(hit.items())[0]
        return os.path.join(self.kom.KOM_ROOT, stem + "_kom", spelling)


def tclass(t):
    c = (t or {}).get("CLASS_TYPE", "")
    return c.split(".", 1)[1] if c.startswith("E:NPC_CLASS_TYPE.") else ""


def flag(v):
    return v is not None and v not in ("FALSE", "false", "0", "nil")


# --- dungeons -------------------------------------------------------------------------------------------------------------------
def walk_script(path):
    """-> dict: entries [(stage, sub, form, group, index, subnpc, npc token, marks)], kills {(stage, sub): [npc token]},
    rewards {(stage, sub, npc token): rate}, subs [(stage, sub)], errors."""
    ent, marks, kills, rewards, subs, errs = [], {}, collections.defaultdict(list), {}, [], []
    for ln in lj(NPC_DUMP, path, "marks"):
        q = ln.split("\t")
        if q[0] == "N":
            ent.append(tuple(q[1:]))
        elif q[0] == "M":
            marks[tuple(q[1:7])] = kv(q[7:])
        elif q[0] == "K":
            kills[(int(q[1]), int(q[2]))].append(q[5])
        elif q[0] == "W":
            rewards[(int(q[1]), int(q[2]), q[3])] = q[4]
        elif q[0] == "B":
            subs.append((int(q[1]), int(q[2])))
        elif q[0] == "E":
            errs.append(q[1])
    return {"entries": ent, "marks": marks, "kills": kills, "rewards": rewards, "subs": subs, "errors": errs}


def grade_of(m):
    g = m.get("MONSTER_GRADE", "")
    return g.rsplit(".", 1)[-1] if g else ""


def classify_dungeon(reg, tok, ms, kill):
    """-> (kind, marks): kind is box / none / unknown / boss / mob. `ms` = the mark dicts of the entries that spawn this NPC in one
    sub-stage; `kill` = the NPC is a CLEAR_COND kill target of that sub-stage."""
    n = nui(tok)
    if n is None:
        return "unknown", ["NPC_ID=" + tok]
    t = reg.templet(n)
    if t is None:
        return "unknown", ["no templet"]
    cls = tclass(t)
    if cls == "NCT_THING_BOX":
        return "box", ["class=" + cls]
    if cls.startswith("NCT_THING_"):
        return "none", ["class=" + cls]
    if all(flag(m.get("NO_DROP")) for m in ms):
        return "none", ["NO_DROP"]
    marks, boss = [], False
    if any(flag(m.get("HAVE_BOSS_GAGE")) for m in ms):
        marks.append("HAVE_BOSS_GAGE"); boss = True
    if any(flag(m.get("SHOW_BOSS_NAME")) for m in ms):
        marks.append("SHOW_BOSS_NAME"); boss = True
    gr = sorted({grade_of(m) for m in ms if grade_of(m)} - {"MG_NORMAL_NPC"})
    if gr:
        marks.append("MONSTER_GRADE=" + "/".join(gr))
        boss = boss or any(g in BOSS_GRADES for g in gr)
    if any(flag(m.get("NO_DROP")) for m in ms):
        marks.append("NO_DROP(some)")
    if kill:
        marks.append("CLEAR_COND")
    return ("boss" if boss else "mob"), marks


def dungeon_rows(reg, mon, keys=None):
    """All census rows for the dungeons the live registry registers."""
    rows, per_script = [], {}
    for did in sorted(reg.dd):
        if keys is not None and did not in keys:
            continue
        _, f = reg.dd[did]
        script = f.get("dataFileName", "").strip('"')
        diff = f.get("difficulty", "").strip('"').replace("@DIFFICULTY_LEVEL.DL_", "")
        kname = reg.dungeon_name.get(did, "?")
        npc_level = int(float(f.get("m_NPCLevel", "0") or 0)) if f.get("m_NPCLevel", "0").lstrip("-").replace(".", "").isdigit() else 0
        band = int(f.get("m_MaxLevel", "0")) if f.get("m_MaxLevel", "0").isdigit() else 0
        if band <= 0 and f.get("m_MinLevel", "0").isdigit():
            band = int(f["m_MinLevel"])
        path = reg.script_path(script)
        if path is None:
            rows.append(dict(scope="dungeon", key=did, key_name=kname, diff=diff, script=script, stage="-", sub="-", npc_id="-",
                             npc="-", level="-", count="0", form="-", role="unknown", marks="no script in any kom", state="-", gap=""))
            continue
        if script not in per_script:
            per_script[script] = walk_script(path)
        w = per_script[script]
        last_stage = max((st_ for st_, _ in w["subs"]), default=0)
        last_sub = max((b for st_, b in w["subs"] if st_ == last_stage), default=0)
        by_sub = collections.OrderedDict()
        for e in w["entries"]:
            st, sb, form, g, idx, sub, tok = int(e[0]), int(e[1]), e[2], e[3], e[4], e[5], e[6]
            by_sub.setdefault((st, sb), []).append((form, g, idx, sub, tok, w["marks"].get((e[0], e[1], form, g, idx, sub), {})))
        for (st, sb), es in by_sub.items():
            groups = collections.OrderedDict()
            for form, g, idx, sub, tok, m in es:
                groups.setdefault((form, g), []).append((idx, sub, tok, m))
            toks = collections.OrderedDict()
            for (form, g), items in groups.items():
                cnt = collections.Counter()
                for idx, sub, tok, m in items:
                    rate = float(m.get("SUB_NPC_RATE", "100")) / 100.0 if sub != "-" else 1.0
                    cnt[tok] += rate
                for tok, c in cnt.items():
                    d = toks.setdefault(tok, {"count": 0.0, "marks": [], "form": form})
                    d["count"] = d["count"] + c if form == "plain" else max(d["count"], c)   # one rate-group spawns
                    d["marks"] += [m for idx, sub, t2, m in items if t2 == tok]
            kills = w["kills"].get((st, sb), [])
            for tok, d in toks.items():
                kill = tok in kills
                is_last = st == last_stage and sb == last_sub
                rr = w["rewards"].get((st, sb, tok))
                role, marks = classify_dungeon(reg, tok, d["marks"], kill)
                if rr is not None:
                    marks.append("REWARD_RATE=" + rr)
                if role == "boss":
                    role = "final-boss" if is_last else "stage-boss"
                elif role == "mob":
                    if is_last and kill:
                        role = "final-boss"
                    elif _num(rr) is not None and _num(rr) < 1:
                        role = "mob-minor"
                n = nui(tok)
                npc_id = reg.npc.get(n) if n else None
                lv = [int(m["LEVEL"]) for m in d["marks"] if m.get("LEVEL", "").lstrip("-").isdigit() and int(m["LEVEL"]) > 0]
                if lv:
                    level, lsrc = max(lv), ""
                elif npc_level > 0:
                    level, lsrc = npc_level, ""
                else:
                    level, lsrc = band, "band"
                if flag(f.get("m_bRelativeMonsterLevel")):
                    lsrc = (lsrc + " relative").strip()
                nm = (reg.templet(n) or {}).get("NAME", "") if n else ""
                nm = nm or (n[4:] if n else tok)
                state = state_of(mon, did, npc_id) if npc_id is not None else "-"
                if lsrc:
                    marks.append("level=" + lsrc)         # band = no LEVEL / m_NPCLevel (the dungeon's level band), relative = party-scaled
                rows.append(dict(scope="dungeon", key=did, key_name=kname, diff=diff, script=script, stage=st, sub=sb,
                                 npc_id=npc_id if npc_id is not None else "-", npc=nm,
                                 level=str(level),
                                 count=_fmt(d["count"]), form=d["form"], role=role, marks=";".join(marks), state=state, gap=""))
    return rows, per_script


def _num(s):
    try:
        return float(s)
    except (TypeError, ValueError):
        return None


def _fmt(x):
    return ("%d" % x) if abs(x - round(x)) < 1e-9 else ("%.2f" % x)


# --- fields ---------------------------------------------------------------------------------------------------------------------
def field_rows(reg, fld, keys=None):
    rows = []
    for meth, f, sub in calls(kc("data036", "BattleFieldData.lua"), "deep"):
        if meth != "AddBattleFieldData":
            continue
        name = f.get("BATTLE_FIELD_ID", "").replace("E:VILLAGE_MAP_ID.", "")
        fid = reg.village.get(name)
        if fid is None or (keys is not None and fid not in keys):
            continue
        tab = f.get("NPC_TABLE_FILE_NAME", "")
        base = int(f.get("STANDARD_MONSTER_LEVEL", "0") or 0)
        boss_groups = set()
        for key, idx, row in sub:
            if key == "BATTLE_FIELD_MIDDLE_BOSS_INFO":
                boss_groups |= {int(x) for x in row.get("SPAWN_GROUP_ID", "").split(",") if x.isdigit()}
        path = reg.script_path(tab)
        if path is None:
            rows.append(dict(scope="field", key=fid, key_name=name, diff="-", script=tab, stage="-", sub="-", npc_id="-", npc="-",
                             level="-", count="0", form="-", role="unknown", marks="no script in any kom", state="-", gap=""))
            continue
        for meth2, g, ents in calls(path, "deep"):
            if meth2 != "AddSpawnMonsterGroup":
                continue
            gid = int(g.get("SPAWN_GROUP_ID", "-1"))
            is_boss = flag(g.get("IS_BOSS_MONSTER_GROUP")) or gid in boss_groups
            for key, idx, e in ents:
                if key != "#" or "MONSTER_ID" not in e:
                    continue
                n = nui(e["MONSTER_ID"])
                npc_id = reg.npc.get(n) if n else None
                t = reg.templet(n) if n else None
                cls = tclass(t)
                marks = []
                if t is None:
                    role = "unknown"; marks.append("no templet")
                elif cls == "NCT_THING_BOX":
                    role = "box"; marks.append("class=" + cls)
                elif cls.startswith("NCT_THING_"):
                    role = "none"; marks.append("class=" + cls)
                elif not flag(e.get("DROP", "TRUE")):
                    role = "none"; marks.append("DROP=FALSE")
                elif is_boss:
                    role = "field-boss"
                    marks.append("IS_BOSS_MONSTER_GROUP" if flag(g.get("IS_BOSS_MONSTER_GROUP")) else "MIDDLE_BOSS_INFO")
                else:
                    role = "field-mob"; marks.append("DROP")
                lvl = base + int(e.get("ADD_LEVEL", "0") or 0)
                rows.append(dict(scope="field", key=fid, key_name=name, diff="-", script=tab, stage=gid, sub="-",
                                 npc_id=npc_id if npc_id is not None else "-",
                                 npc=((t or {}).get("NAME", "") or (n[4:] if n else e["MONSTER_ID"])), level=str(lvl),
                                 count=e.get("MAX_NUMBER_OF_THIS_MONSTER", "1"), form="rate%s" % e.get("RATE", ""),
                                 role=role, marks=";".join(marks),
                                 state=state_of(fld, fid, npc_id) if npc_id is not None else "-", gap=""))
    return rows


# --- census ---------------------------------------------------------------------------------------------------------------------
def build(keys=None):
    reg = Reg()
    mon, fld, static = server_rows()
    drows, per_script = dungeon_rows(reg, mon, keys)
    frows = field_rows(reg, fld, keys)
    rowed = {k for k, _ in mon if k != 0}
    live = {r["key"] for r in drows if r["state"] == "specific"}     # a row under the key for an NPC the script spawns
    for r in drows:
        if r["key"] in live and r["state"] == "none" and r["role"] not in ("none", "box", "unknown"):
            r["gap"] = "R6"
    return reg, mon, fld, static, drows + frows, rowed


def write_tsv(rows, path):
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\t".join(COLUMNS) + "\n")
        for r in rows:
            fh.write("\t".join(str(r.get(c, "")).replace("\t", " ") for c in COLUMNS) + "\n")


def crosscheck_loose(mon, fld, static):
    """The loose KncWX2Server copies and the staged files must hold the same keys (else the census reads the wrong table)."""
    m2, f2, s2 = rows_of(os.path.join(SERVER_DIR, "DropTable.lua"), os.path.join(SERVER_DIR, "StaticDropTable.lua"))
    return set(mon) == set(m2) and set(fld) == set(f2) and static == s2, (len(m2), len(f2), len(s2))


def cmd_census(a):
    reg, mon, fld, static, rows, rowed = build()
    write_tsv(rows, a.out)
    ok, loose = crosscheck_loose(mon, fld, static)
    d = [r for r in rows if r["scope"] == "dungeon"]
    f = [r for r in rows if r["scope"] == "field"]
    reg_keys = sorted(reg.dd)
    have = [k for k in reg_keys if k in rowed]
    out = [
        "rows %d (dungeon %d, field %d) -> %s" % (len(rows), len(d), len(f), a.out if os.path.splitdrive(a.out)[0].lower() != os.path.splitdrive(TRUNK)[0].lower() else os.path.relpath(a.out, TRUNK)),
        "registered dungeons %d, with a server row under their own key %d, without %d" % (len(reg_keys), len(have), len(reg_keys) - len(have)),
        "server rows: %d monster, %d field-NPC, %d static keys; staged == loose KncWX2Server copy: %s %s" % (
            len(mon), len(fld), len(static), "YES" if ok else "NO", loose),
        "roles " + " ".join("%s=%d" % kv_ for kv_ in sorted(collections.Counter(r["role"] for r in rows).items())),
    ]
    fields_all = sorted({r["key"] for r in f})
    nofld = [k for k in fields_all if not any(kk == k for kk, _ in fld)]
    out.append("fields %d, without any field-NPC row of their own %s" % (
        len(fields_all), " ".join("%d" % k for k in nofld)))
    if not a.quiet:
        print("\n".join(out))
    if a.gaps:
        gap = [r for r in rows if r["gap"]]
        print("GAP (state none inside a dungeon that has rows): %d rows" % len(gap))
        for r in gap:
            print("%s\t%s\t%s\t%s\tst%s.%s\t%s\t%s\tlv %s\t%s" % (r["key"], r["key_name"], r["diff"], r["script"], r["stage"], r["sub"],
                                                                 r["npc_id"], r["npc"], r["level"], r["role"]))
    return 0


def cmd_summary(a):
    import csv
    with open(a.file, encoding="utf-8", newline="") as fh:
        rows = list(csv.DictReader(fh, delimiter="	"))
    for phase, label, dkeys, fkeys in SCOPE:
        keys = set(dkeys) | set(fkeys)
        rs = [r for r in rows if int(r["key"]) in keys]
        present = sorted({int(r["key"]) for r in rs})
        print("%s %s: %d keys, %d rows" % (phase, label, len(present), len(rs)))
        print("  roles " + " ".join("%s=%d" % kv_ for kv_ in sorted(collections.Counter(r["role"] for r in rs).items())))
        print("  state " + " ".join("%s=%d" % kv_ for kv_ in sorted(collections.Counter(r["state"] for r in rs).items())))
        for role in ("none", "box", "unknown"):
            names = collections.OrderedDict()
            for r in rs:
                if r["role"] == role:
                    names.setdefault((r["npc_id"], r["npc"], r["marks"]), set()).add(int(r["key"]))
            for (nid, nm, mk), ks in names.items():
                print("  %-7s %-6s %-34s %-28s keys %d" % (role, nid, nm, mk, len(ks)))
    return 0


def cmd_explain(a):
    reg, mon, fld, static, rows, rowed = build({a.key})
    for r in rows:
        print("\t".join(str(r.get(c, "")) for c in COLUMNS))
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    c = sub.add_parser("census")
    c.add_argument("--out", default=OUT_DEFAULT)
    c.add_argument("--gaps", action="store_true")
    c.add_argument("--quiet", action="store_true")
    c.set_defaults(fn=cmd_census)
    m = sub.add_parser("summary")
    m.add_argument("--file", default=OUT_DEFAULT)
    m.set_defaults(fn=cmd_summary)
    e = sub.add_parser("explain")
    e.add_argument("key", type=int)
    e.set_defaults(fn=cmd_explain)
    a = ap.parse_args()
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")      # NPC names carry non-ASCII text
    return a.fn(a)


if __name__ == "__main__":
    sys.exit(main())
