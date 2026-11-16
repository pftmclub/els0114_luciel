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

  python tools/droptable.py pools [--check]                         item pools -> tools/data/drop_item_pools.tsv (R1)
  python tools/droptable.py rates [--check]                         rate sheet -> tools/data/drop_rate_sheet.tsv (R1)
  python tools/droptable.py simulate <key> [<key> ...]              expected items per clear and per role kill against the studio baseline (R1)

pools / rates / simulate (R1) write and read the two `gen` inputs in its columns (see above `gen_blocks`); every rate line is copied from the
studio row its source names (`rates --check` re-reads each one), the pools come from item.kim and each dungeon's BOSS_DROP_ITEM.

  python tools/droptable.py gen   [--only lanox|elysion|secret|all] [--keys 33270,33271] [--dry-run] [--out-dir DIR]
  python tools/droptable.py check [--drop-file F --static-file F]

gen (R2) builds the stand-in block of KncWX2Server/ServerResource/US/DropTable.lua and StaticDropTable.lua from the census, the item pools
and the rate sheet (the input columns are described above `gen_blocks`): one row per (key, NPC) for a scoped key, no row for a role the
sheet has no `param` line for, never a (key, MonsterID) pair or a static key the studio rows already hold, group ids from 20000-20999.
`--dry-run` prints the counts and writes nothing; with `--out-dir` it also writes the two files it would produce there. Without
--dry-run it replaces (or appends) the block in the two server files, BOM and CRLF kept. check (R2): the block equals a fresh gen, no
pair / static key / group id shared with the studio rows, group ids inside the reserved range, every item in item.kim, BOM and CRLF intact,
the block ASCII, a row's outer probabilities sum to 100 or less.
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


# --- gen / check (R2) -------------------------------------------------------------------------------------------------------------
# The generated stand-in block (OFFLINE_FIELD_REWARDS_PHASES.md "Design: the stand-in"). Inputs, all under tools/data:
#   drop_census.tsv        R0: key x stage x sub x NPC, role per NPC (`none` / `unknown` never get a row)
#   drop_item_pools.tsv    R1: pool region band key npc item_id name type grade source
#                          band = `lo-hi` dungeon level or `*`; key = a dungeon / field id (difficulty-less base id or the exact
#                          key) or `*`; npc = an NPC id or `*` (monster cards)
#   drop_rate_sheet.tsv    R1: kind region role diff pool exp ed ed_property prob inner source
#                          kind `param`  (region, role, diff) -> exp / ed / ed_property of the row; no param line = no row for the role
#                          kind `item`   every item of the pool joins DropItemList at `prob` each
#                          kind `group`  the pool becomes one group (inner `inner` each, blank = 100 / items) named in DropGroupList at `prob`
#                          kind `static` every item of the pool joins the key's static row at `prob` each (role and exp / ed unused)
#                          region = lanox / elysion / secret / `*`; diff = NORMAL / HARD / EXPERT / FIELD / `*`; role `mob-card` adds
#                          to a mob / mob-minor whose pool rows name the NPC in `npc`
BLOCK_DATE = "2026-11-16"
BLOCK_BEGIN = "--{{ Iruha : %s // field-rewards stand-in, generated by tools/droptable.py (OFFLINE_FIELD_REWARDS_PHASES.md) - do not hand-edit" % BLOCK_DATE
BLOCK_BEGIN_RE = re.compile(r"^--\{\{ Iruha : [0-9-]+ // field-rewards stand-in, generated by tools/droptable\.py", re.M)
BLOCK_END = "--}}"
POOLS_DEFAULT = os.path.join(DATA, "drop_item_pools.tsv")
SHEET_DEFAULT = os.path.join(DATA, "drop_rate_sheet.tsv")
GROUP_FIRST, GROUP_LAST = 20000, 20999          # the reserved range (Rules 3); X2OfflineDropTable.cpp holds the same pair
DIFF_STEP = {"NORMAL": 0, "HARD": 1, "EXPERT": 2}
ROLE_PRIORITY = ["final-boss", "stage-boss", "field-boss", "mob", "field-mob", "mob-minor", "box"]   # one row per (key, NPC): first wins
NO_ROW_ROLES = ("none", "unknown")
KEEP_WILDCARD_REGIONS = ("secret",)     # the studio's key-0 rows (CHEST 20, CHEST_MONSTER 29) already answer these dungeons: no row of their own
GEN_REGIONS = ("lanox", "elysion", "secret")
POOL_COLS = ["pool", "region", "band", "key", "npc", "item_id", "name", "type", "grade", "source"]
SHEET_COLS = ["kind", "region", "role", "diff", "pool", "exp", "ed", "ed_property", "prob", "inner", "source"]


def read_tsv(path, cols, what):
    """-> list of dicts, [] when the file does not exist yet (R1 writes the pools and the sheet). Header must hold `cols`."""
    if not os.path.isfile(path):
        return []
    with open(path, encoding="utf-8", newline="") as fh:
        lines = [ln.rstrip("\r\n") for ln in fh if ln.strip() and not ln.startswith("#")]
    if not lines:
        return []
    head = lines[0].split("\t")
    miss = [c for c in cols if c not in head]
    if miss:
        raise SystemExit("%s: %s lacks column(s) %s (has %s)" % (what, path, ", ".join(miss), ", ".join(head)))
    out = []
    for ln in lines[1:]:
        parts = ln.split("\t")
        parts += [""] * (len(head) - len(parts))
        out.append(dict(zip(head, parts)))
    return out


def region_of(label):
    return label.split()[0].lower()


def scope_keys(only, keys):
    """-> list of (region, kind, key) in the plan's scope for --only / --keys."""
    out = []
    for _, label, dkeys, fkeys in SCOPE:
        reg = region_of(label)
        if only not in (None, "all", reg):
            continue
        out += [(reg, "dungeon", k) for k in dkeys] + [(reg, "field", k) for k in fkeys]
    if keys:
        out = [t for t in out if t[2] in keys]
    return out


def band_has(band, level):
    if band in ("", "*"):
        return True
    m = re.match(r"^(\d+)-(\d+)$", band)
    return bool(m) and int(m.group(1)) <= level <= int(m.group(2))


def fmt_prob(x):
    t = "%.4f" % x
    return t.rstrip("0").rstrip(".")


def fmt_list(entries):
    """`{ { id, prob },{ id, prob },}` as the studio writes it; an empty list is `{ }`."""
    return "{ " + "".join("{ %d, %s }," % e for e in entries) + "}"


def eff_role(region, role, marks):
    """The role a census NPC is paid as: a secret dungeon's stage boss below MONSTER_GRADE=1 is a mob (the studio's secret rows pay only
    the grade-1 boss; Elder 33030 x 534, its stage boss, carries a mob-sized row)."""
    if region == "secret" and role == "stage-boss" and "MONSTER_GRADE=1" not in marks.split(";"):
        return "mob"
    return role


def pool_items(pools, name, region, level, key, base, npc):
    """The items of pool `name` that apply to this dungeon / field and NPC, in file order, an id once."""
    if region == "secret":
        base = key                                  # a hell dungeon's entry list is its own, never the common list of key - 1
    seen, out = set(), []
    for p in pools:
        if p["pool"] != name or p["region"] not in (region, "*") or not band_has(p["band"], level):
            continue
        if p["key"] not in ("*", "", str(key), str(base)) or p["npc"] not in ("*", "", str(npc)):
            continue
        iid = int(p["item_id"])
        if iid not in seen:
            seen.add(iid)
            out.append(iid)
    return out


def pick(sheet, kind, region, role, diff):
    """Sheet lines of `kind` for (region, role, diff); the exact diff beats `*` for one (pool) line."""
    got = [r for r in sheet if r["kind"] == kind and r["region"] in (region, "*") and r["role"] in (role, "*") and r["diff"] in (diff, "*")]
    return got


def studio_pairs(text):
    """The studio rows in a server file's text (any stand-in block already cut out): -> (monster pairs, field pairs, static keys, groups)."""
    mon, fld, static = set(), set(), set()
    for m in re.finditer(r"^DropTable:AddMonsterDropInfo\s*\{(.*?)^\}", text, re.S | re.M):
        d, n = re.search(r"DungeonID\s*=\s*(\d+)", m.group(1)), re.search(r"MonsterID\s*=\s*(\d+)", m.group(1))
        if d and n:
            mon.add((int(d.group(1)), int(n.group(1))))
    for m in re.finditer(r"^DropTable:AddBattleFieldNpcDropInfo\s*\{(.*?)^\}", text, re.S | re.M):
        d, n = re.search(r"BattleFieldID\s*=\s*(\d+)", m.group(1)), re.search(r"MonsterID\s*=\s*(\d+)", m.group(1))
        if d and n:
            fld.add((int(d.group(1)), int(n.group(1))))
    for m in re.finditer(r"^DropTable:AddStaticDropInfo\s*\{(.*?)^\}", text, re.S | re.M):
        d = re.search(r"DungeonID\s*=\s*(\d+)", m.group(1))
        if d:
            static.add(("dungeon", int(d.group(1))))
    for m in re.finditer(r"^DropTable:AddBattleFieldStaticDropInfo\s*\{(.*?)^\}", text, re.S | re.M):
        d = re.search(r"BattleFieldID\s*=\s*(\d+)", m.group(1))
        if d:
            static.add(("field", int(d.group(1))))
    groups = {int(g) for g in re.findall(r"^DropTable:AddToGroup\(\s*(\d+)\s*,", text, re.M)}
    return mon, fld, static, groups


def split_block(raw):
    """-> (before, block text or None, after) for a server file's decoded text; the block runs from its begin line to the `--}}` line."""
    m = BLOCK_BEGIN_RE.search(raw)
    if not m:
        return raw, None, ""
    e = raw.find(BLOCK_END, m.start())
    if e < 0:
        raise SystemExit("stand-in block has no closing %s line" % BLOCK_END)
    e = raw.find("\n", e)
    e = len(raw) if e < 0 else e + 1
    return raw[:m.start()], raw[m.start():e], raw[e:]


def read_server_text(path):
    with open(path, "rb") as fh:
        b = fh.read()
    if b[:3] != b"\xef\xbb\xbf":
        raise SystemExit("%s: no UTF-8 BOM" % path)
    if b.count(b"\n") != b.count(b"\r\n"):
        raise SystemExit("%s: bare LF line(s); gen keeps the file's bytes and needs CRLF throughout" % path)
    return b[3:].decode("utf-8").replace("\r\n", "\n")          # the text is LF inside; write_server puts CRLF back


def gen_blocks(only, keys, census, pools, sheet, drop_text, static_text):
    """-> (drop block text or None, static block text or None, stats dict). Text uses \\n; callers convert to CRLF."""
    st = collections.OrderedDict(dungeon_rows=0, dungeon_keys=set(), field_rows=0, field_keys=set(), static_rows=0, groups=0,
                                 skipped_studio=0, mixed_roles=0, no_sheet=collections.Counter(), warn=[])
    smon, sfld, sstatic, sgroups = studio_pairs(drop_text)
    _, _, sstatic2, _ = studio_pairs(static_text)
    sstatic = sstatic | sstatic2
    scope = scope_keys(only, keys)
    by_key = collections.defaultdict(list)
    for r in census:
        by_key[(r["scope"], int(r["key"]))].append(r)

    group_ids, group_text, next_gid = {}, [], [GROUP_FIRST]

    def group_for(items):
        sig = tuple(items)
        if sig not in group_ids:
            if next_gid[0] > GROUP_LAST:
                raise SystemExit("gen needs more than the reserved group ids %d-%d" % (GROUP_FIRST, GROUP_LAST))
            group_ids[sig] = next_gid[0]
            for iid, inner in items:
                group_text.append("DropTable:AddToGroup( %d, %d, %s )" % (next_gid[0], iid, inner))
            next_gid[0] += 1
        return group_ids[sig]

    drow_text, frow_text, static_rows = [], [], []
    for region, kind, key in scope:
        rows = by_key.get((kind, key), [])
        if not rows:
            continue
        diff = rows[0]["diff"] if kind == "dungeon" else "FIELD"
        base = key - DIFF_STEP.get(diff, 0) if kind == "dungeon" else key
        level = max([int(r["level"]) for r in rows if r["level"].isdigit() and r["role"] not in NO_ROW_ROLES + ("box",)] or [0])
        npcs, wild = collections.OrderedDict(), set()
        for r in rows:
            npcs.setdefault(int(r["npc_id"]), []).append(eff_role(region, r["role"], r["marks"]))
            if r["state"] == "wildcard" and region in KEEP_WILDCARD_REGIONS:
                wild.add(int(r["npc_id"]))
        studio = smon if kind == "dungeon" else sfld
        made = 0
        for npc in sorted(npcs):
            if npc in wild:
                continue
            roles = set(npcs[npc])
            if len(roles - set(NO_ROW_ROLES)) > 1:
                st["mixed_roles"] += 1
            role = next((x for x in ROLE_PRIORITY if x in roles), None)
            if role is None:
                continue
            if (key, npc) in studio:
                st["skipped_studio"] += 1                   # Rules 3: never repeat a studio (key, MonsterID) pair
                continue
            par = [r for r in pick(sheet, "param", region, role, diff)]
            par.sort(key=lambda r: (r["diff"] == "*", r["region"] == "*"))
            if not par:
                st["no_sheet"][(region, role, diff)] += 1
                continue
            p = par[0]
            items, groups, seen = [], [], set()
            lines = pick(sheet, "item", region, role, diff) + pick(sheet, "group", region, role, diff)
            if role in ("mob", "mob-minor"):
                lines += [r for r in sheet if r["kind"] in ("item", "group") and r["role"] == "mob-card"
                          and r["region"] in (region, "*") and r["diff"] in (diff, "*")]
            for ln in lines:
                its = pool_items(pools, ln["pool"], region, level, key, base, npc)
                if ln["kind"] == "item":
                    for iid in its:
                        if iid not in seen:
                            seen.add(iid)
                            items.append((iid, ln["prob"]))
                elif its:
                    inner = ln["inner"] if ln["inner"] else fmt_prob(100.0 / len(its))
                    groups.append((group_for([(iid, inner) for iid in its]), ln["prob"]))
            tot = sum(float(x[1]) for x in items) + sum(float(x[1]) for x in groups)
            if tot > 100.0 + 1e-6:
                st["warn"].append("%s key %d npc %d: outer probabilities sum to %s" % (kind, key, npc, fmt_prob(tot)))
            fkey = "DungeonID" if kind == "dungeon" else "BattleFieldID"
            meth = "AddMonsterDropInfo" if kind == "dungeon" else "AddBattleFieldNpcDropInfo"
            txt = ["DropTable:%s" % meth, "{", "\t%s = %d," % (fkey, key), "\tMonsterID = %d," % npc, "\tStandIn = 1,",
                   "\tExp = %s," % p["exp"], "\tED = %s," % p["ed"], "\tEDProperty = %s," % p["ed_property"],
                   "\tDropItemList = %s," % fmt_list(items), "\tDropGroupList = %s," % fmt_list(groups), "}", ""]
            (drow_text if kind == "dungeon" else frow_text).append("\n".join(txt))
            made += 1
        if made:
            st["dungeon_rows" if kind == "dungeon" else "field_rows"] += made
            st["dungeon_keys" if kind == "dungeon" else "field_keys"].add(key)
        # the key's own static row, from the sheet's `static` lines
        sl = pick(sheet, "static", region, "*", diff)
        sitems, seen = [], set()
        for ln in sl:
            for iid in pool_items(pools, ln["pool"], region, level, key, base, "*"):
                if iid not in seen:
                    seen.add(iid)
                    sitems.append((iid, ln["prob"]))
        if sitems and (kind, key) not in sstatic:
            meth = "AddStaticDropInfo" if kind == "dungeon" else "AddBattleFieldStaticDropInfo"
            fkey = "DungeonID" if kind == "dungeon" else "BattleFieldID"
            static_rows.append("\n".join(["DropTable:%s" % meth, "{", "\t%s = %d," % (fkey, key), "\tEnable = True,", "\tStandIn = 1,",
                                          "\tDropItemList = %s," % fmt_list(sitems), "}", ""]))
            st["static_rows"] += 1
        elif sitems:
            st["warn"].append("%s key %d: the studio already has a static row, not repeated" % (kind, key))
    st["groups"] = len(group_ids)

    def wrap(body):
        head = [BLOCK_BEGIN, "-- scope: %s" % (only or "all") + ("; keys: %s" % ",".join(str(k) for k in sorted(keys)) if keys else ""),
                "if DROP_STANDIN then"]
        return "\n".join(head + body + ["end", BLOCK_END, ""])

    drop_body = group_text + drow_text + frow_text          # every AddToGroup precedes the rows that name it (the loader needs that)
    dblock = wrap(drop_body) if (drow_text or frow_text) else None
    sblock = wrap(static_rows) if static_rows else None
    return dblock, sblock, st


def with_block(raw_text, block):
    """The server file's text (LF inside) with its stand-in block replaced by `block`, or removed when None."""
    before, old, after = split_block(raw_text)
    if block is None:
        return before + after
    if before and not before.endswith("\n"):
        before += "\n"
    return before + block + after


def write_server(path, text):
    with open(path, "wb") as fh:
        fh.write(b"\xef\xbb\xbf" + text.replace("\n", "\r\n").encode("utf-8"))


def gen_inputs(a):
    census = [r for r in read_tsv(a.census, COLUMNS, "census")]
    return census, read_tsv(a.pools, POOL_COLS, "pools"), read_tsv(a.sheet, SHEET_COLS, "rate sheet")


def parse_keys(text):
    return {int(x) for x in text.split(",") if x.strip()} if text else None


def cmd_gen(a):
    census, pools, sheet = gen_inputs(a)
    if a.only not in (None, "all") + GEN_REGIONS:
        raise SystemExit("--only takes %s" % ", ".join(("all",) + GEN_REGIONS))
    keys = parse_keys(a.keys)
    dtext = read_server_text(a.drop_file)
    stext = read_server_text(a.static_file)
    dstudio = split_block(dtext)
    sstudio = split_block(stext)
    dblock, sblock, st = gen_blocks(a.only, keys, census, pools, sheet, dstudio[0] + dstudio[2], sstudio[0] + sstudio[2])
    new_d = with_block(dtext, dblock)
    new_s = with_block(stext, sblock)
    print("GEN scope=%s: %d dungeon row(s) over %d key(s), %d field row(s) over %d field(s), %d static row(s), %d group(s)" % (
        a.only or "all", st["dungeon_rows"], len(st["dungeon_keys"]), st["field_rows"], len(st["field_keys"]), st["static_rows"],
        st["groups"]))
    print("     pools %d item row(s), rate sheet %d line(s); studio pairs left alone %d; NPCs with mixed roles %d" % (
        len(pools), len(sheet), st["skipped_studio"], st["mixed_roles"]))
    if not pools or not sheet:
        print("     NOTE: %s%s%s missing or empty - R1 writes it; no row can be generated" % (
            "" if pools else "drop_item_pools.tsv", "" if pools or sheet else " and ", "" if sheet else "drop_rate_sheet.tsv"))
    for (reg, role, diff), n in sorted(st["no_sheet"].items()) if sheet else []:
        print("     WARNING no param line for %s / %s / %s: %d NPC(s) get no row" % (reg, role, diff, n))
    for w in st["warn"]:
        print("     WARNING " + w)
    if a.out_dir:
        os.makedirs(a.out_dir, exist_ok=True)
        write_server(os.path.join(a.out_dir, "DropTable.lua"), new_d)
        write_server(os.path.join(a.out_dir, "StaticDropTable.lua"), new_s)
        print("     wrote DropTable.lua + StaticDropTable.lua under %s (the server files are untouched)" % a.out_dir)
    if a.dry_run:
        print("DRY RUN: nothing written to the server Lua")
        return 0
    write_server(a.drop_file, new_d)
    write_server(a.static_file, new_s)
    print("WROTE %s and %s" % (a.drop_file, a.static_file))
    return 0


def parse_block_rows(blk):
    """-> [(method, key, npc id or 0 for a static row, [(item, prob)], [(group, prob)])] for the rows of a block."""
    pair = re.compile(r"\{\s*(\d+)\s*,\s*([0-9.]+)\s*\}")
    out = []
    for m in re.finditer(r"^DropTable:(Add\w+)\n\{\n(.*?)^\}", blk, re.S | re.M):
        body = m.group(2)
        key = re.search(r"(?:DungeonID|BattleFieldID)\s*=\s*(\d+)", body)
        npc = re.search(r"MonsterID\s*=\s*(\d+)", body)
        il = re.search(r"^\tDropItemList = (.*),$", body, re.M)
        gl = re.search(r"^\tDropGroupList = (.*),$", body, re.M)
        out.append((m.group(1), int(key.group(1)) if key else 0, int(npc.group(1)) if npc else 0,
                    [(int(x), p) for x, p in pair.findall(il.group(1))] if il else [],
                    [(int(x), p) for x, p in pair.findall(gl.group(1))] if gl else []))
    return out


def parse_block_groups(blk):
    return [(int(g), int(i), p) for g, i, p in re.findall(r"^DropTable:AddToGroup\(\s*(\d+)\s*,\s*(\d+)\s*,\s*([0-9.]+)\s*\)", blk, re.M)]


def block_scope(block):
    m = re.search(r"^-- scope: (\w+)(?:; keys: ([0-9,]+))?$", block, re.M)
    return (m.group(1), frozenset(parse_keys(m.group(2)) or ())) if m else (None, None)


def cmd_check(a):
    census, pools, sheet = gen_inputs(a)
    import kim as kimmod
    index = kimmod.Kim(a.kim).index()
    fails, notes = [], []
    studio_texts, blocks, stats = {}, {}, {}
    for name, path in (("DropTable.lua", a.drop_file), ("StaticDropTable.lua", a.static_file)):
        b = open(path, "rb").read()
        if b[:3] != b"\xef\xbb\xbf":
            fails.append("%s: BOM missing" % name)
        if b.count(b"\n") != b.count(b"\r\n"):
            fails.append("%s: %d bare LF line(s) (CRLF expected throughout)" % (name, b.count(b"\n") - b.count(b"\r\n")))
        text = (b[3:] if b[:3] == b"\xef\xbb\xbf" else b).decode("utf-8", "replace").replace("\r\n", "\n")
        before, blk, after = split_block(text)
        studio_texts[name], blocks[name] = before + after, blk
        if blk is not None:
            try:
                blk.encode("ascii")
            except UnicodeEncodeError:
                fails.append("%s: the block holds non-ASCII text" % name)
    # a fresh gen with the scope the block names must reproduce the block byte for byte
    scopes = {block_scope(b) for b in blocks.values() if b is not None}
    if len(scopes) > 1:
        fails.append("the two files' blocks name different scopes: %s" % sorted(map(str, scopes)))
    if scopes:
        only, keys = next(iter(scopes))
        only = None if only == "all" else only
        db, sb, st = gen_blocks(only, keys, census, pools, sheet, studio_texts["DropTable.lua"], studio_texts["StaticDropTable.lua"])
        for name, fresh in (("DropTable.lua", db), ("StaticDropTable.lua", sb)):
            got = blocks[name]
            if (got or "") != (fresh or ""):
                fails.append("%s: the block differs from a fresh gen (hand edit, or the census / pools / sheet changed)" % name)
    else:
        notes.append("no stand-in block in either file (nothing generated yet)")
    mon, fld, static, groups = studio_pairs(studio_texts["DropTable.lua"])
    _, _, static2, _ = studio_pairs(studio_texts["StaticDropTable.lua"])
    static |= static2
    seen_pairs, seen_static, n_rows, n_static = set(), set(), 0, 0
    for name, blk in blocks.items():
        if blk is None:
            continue
        bm, bf, bs, bg = studio_pairs(blk)
        for kind, pairs, studio in (("dungeon", bm, mon), ("field", bf, fld)):
            for p in sorted(pairs):
                n_rows += 1
                if p in studio:
                    fails.append("%s: %s pair %s repeats a studio row" % (name, kind, p))
                if (kind, p) in seen_pairs:
                    fails.append("%s: %s pair %s appears twice" % (name, kind, p))
                seen_pairs.add((kind, p))
        for k in sorted(bs):
            n_static += 1
            if k in static:
                fails.append("%s: static key %s repeats a studio row" % (name, k))
            if k in seen_static:
                fails.append("%s: static key %s appears twice" % (name, k))
            seen_static.add(k)
        for g in sorted(bg):
            if g in groups:
                fails.append("%s: group id %d is also a studio group" % (name, g))
            if not GROUP_FIRST <= g <= GROUP_LAST:
                fails.append("%s: group id %d outside %d-%d" % (name, g, GROUP_FIRST, GROUP_LAST))
        defined = {g for g, _, _ in parse_block_groups(blk)}
        for meth, key, npc, items, grps in parse_block_rows(blk):
            tot = sum(float(p) for _, p in items) + sum(float(p) for _, p in grps)
            if tot > 100.0 + 1e-6:
                fails.append("%s: %s key %d npc %d: outer probabilities sum to %s (over 100)" % (name, meth, key, npc, fmt_prob(tot)))
            for iid, _ in items:
                if iid not in index:
                    fails.append("%s: item %d is not in item.kim" % (name, iid))
            for g, _ in grps:
                if g not in defined:
                    fails.append("%s: key %d npc %d names group %d the block never defines" % (name, key, npc, g))
        for g, iid, _ in parse_block_groups(blk):
            if iid not in index:
                fails.append("%s: group %d item %d is not in item.kim" % (name, g, iid))
    print("CHECK rows %d, static rows %d, groups in block %d; item.kim = %s (%d items)" % (
        n_rows, n_static, sum(len(studio_pairs(b)[3]) for b in blocks.values() if b), os.path.basename(os.path.dirname(a.kim)) or a.kim,
        len(index)))
    for n in notes:
        print("     NOTE " + n)
    for f in fails:
        print("     FAIL " + f)
    print("RESULT: %s" % ("FAIL (%d)" % len(fails) if fails else "PASS"))
    return 1 if fails else 0


def add_gen_paths(p):
    p.add_argument("--census", default=OUT_DEFAULT)
    p.add_argument("--pools", default=POOLS_DEFAULT)
    p.add_argument("--sheet", default=SHEET_DEFAULT)
    p.add_argument("--drop-file", default=os.path.join(SERVER_DIR, "DropTable.lua"))
    p.add_argument("--static-file", default=os.path.join(SERVER_DIR, "StaticDropTable.lua"))


# --- item pools, rate sheet, simulate (R1) ---------------------------------------------------------------------------------------
# R1 writes the two inputs `gen` reads, in the columns described above `gen_blocks` (POOL_COLS / SHEET_COLS). A studio group whose inner
# weights differ is written as one `item` line per member at outer x weight / 100 (one lottery picks one case, so the odds are the same);
# a group of equal weights stays a `group` line. Every value is read from the studio row its `source` names; `rates --check` re-reads them.
R1_POOLS = os.path.join(DATA, "drop_item_pools.tsv")
R1_SHEET = os.path.join(DATA, "drop_rate_sheet.tsv")
R1_POOL_COLS = ["pool", "region", "band", "key", "npc", "item_id", "name", "type", "grade", "source"]
R1_SHEET_COLS = ["kind", "region", "role", "diff", "pool", "exp", "ed", "ed_property", "prob", "inner", "source"]

# Region gear ladder (item.kim 10LL....): (tier, first id, last id, NPC-level band); a tier is the items in the range at that use_level.
# Band: the tier nearest the key's NPC level, a tie to the higher tier (Lanox 70-73 -> L72, 74-76 -> L76, as the plan's Design table).
# Sets: the region's two RARE sets (the studio's Sander bosses carry two RARE sets, 2810 + 2814).
REGION_DATA = collections.OrderedDict([
    ("lanox", dict(tiers=[(72, 10720000, 10721999, "0-73"), (76, 10760000, 10761999, "74-99")], sets=[3900, 4015])),
    ("elysion", dict(tiers=[(80, 10800000, 10801999, "0-80"), (82, 10801000, 10821999, "81-84"), (87, 10851000, 10871999, "85-99")],
                     sets=[4521, 5212])),
])
R1_GRADES = [("normal", "IG_NORMAL"), ("rare", "IG_RARE"), ("elite", "IG_ELITE")]
R1_ARMOUR = [("top", "EP_DEFENCE_BODY"), ("bottom", "EP_DEFENCE_LEG"), ("gloves", "EP_DEFENCE_HAND"), ("shoes", "EP_DEFENCE_FOOT")]
STATIC_COMMON = [99809, 99810, 99811, 99812]          # the four cooking goods of every studio static row
STATIC_REGION_SANDER = [99823, 99824]                 # Sander's pair; item.kim has no Lanox / Elysion 'Cooking Good' pair
BOSS_LIST_ORDER = ["COMMON", "ELSWORD", "AISHA", "RENA", "RAVEN", "EVE", "CHUNG", "ARA", "ELESIS", "ADD", "LUCIEL", "ROSE", "AIN"]
EXCLUDE_TEXT = ("(Cannot use)", "Cannot be used", "(Event)", "[Event]")
DIFF3 = collections.OrderedDict([("NORMAL", 33250), ("HARD", 33251), ("EXPERT", 33252)])


class Items:
    """item.kim (the staged June kim); strings decoded on demand."""
    F = ["item_type", "item_grade", "use_level", "unit_class", "eqip_position", "set_id", "fashion", "shop_price_type", "is_pc_bang"]

    def __init__(self):
        import kim
        self.m = kim
        self.k = kim.Kim("staged")
        self.era = self.k.L["era"]
        self.rows = {}
        for i in self.k.ids():
            r = self.k.raw(i)
            self.rows[i] = (r, {f: kim.field_values(self.k, r, f, decode_strings=False)[0] for f in self.F})

    def has(self, i):
        return i in self.rows

    def get(self, i, f):
        r, d = self.rows[i]
        return d[f] if f in d else self.m.field_values(self.k, r, f)[0]

    def label(self, i, f):
        v = self.get(i, f)
        return self.m.enum_label(f, v, self.era) or str(v)

    def bad(self, i):
        """-> why an item may not enter a pool (not in item.kim, fashion, cash, PC-bang, 'Cannot use' / event text), or ''."""
        if not self.has(i):
            return "not in item.kim"
        if self.get(i, "fashion"):
            return "fashion"
        if self.label(i, "shop_price_type") == "SPT_CASH":
            return "cash"
        if self.get(i, "is_pc_bang"):
            return "pc-bang"
        text = (self.get(i, "name") + " " + self.get(i, "desc")).lower()
        hit = [t for t in EXCLUDE_TEXT if t.lower() in text]
        return "text '%s'" % hit[0] if hit else ""

    def find(self, pred):
        return [i for i, (r, d) in sorted(self.rows.items()) if pred(i, d)]

    def by_name(self, name):
        return [i for i in self.find(lambda i, d: not d["fashion"]) if self.get(i, "name") == name]


def ascii_text(s):
    return s.encode("ascii", "replace").decode("ascii")


def stone_levels(items):
    """-> {stone level: (weapon id, armour id, lo, hi)} from the stones' own item.kim text ('Levels 61-70', 'level 71~81')."""
    out = {}
    for lv in range(1, 10):
        w, a = items.by_name("Weapon Enhancement Stone Lv.%d" % lv), items.by_name("Armor Enhancement Stone Lv.%d" % lv)
        m = re.search(r"[Ll]evels?\s+(\d+)\s*[-~]\s*(\d+)", items.get(w[0], "desc")) if len(w) == 1 and len(a) == 1 else None
        if m:
            out[lv] = (w[0], a[0], int(m.group(1)), int(m.group(2)))
    return out


def boss_lists(reg, key):
    """-> [(list name, [item ids])] of the dungeon's BOSS_DROP_ITEM: COMMON first, then the per-character lists."""
    _, f = reg.dd[key]
    found = dict((n, [int(x) for x in re.findall(r"\d+=(\d+)", v)])
                 for n, v in re.findall(r"([A-Z_]+)=\{([^{}]*)\}", f.get("BOSS_DROP_ITEM", "")))
    return [(n, found[n]) for n in BOSS_LIST_ORDER if found.get(n)] + \
           [(n, v) for n, v in sorted(found.items()) if n not in BOSS_LIST_ORDER and v]


def card_items(items):
    """-> {lower-case monster name: card id} for every usable 'Monster Card' item."""
    out = {}
    for i in items.find(lambda i, d: not d["fashion"]):
        nm = items.get(i, "name")
        if nm.startswith("Monster Card") and not items.bad(i):
            m = re.match(r"Monster Card\s*[-:]?\s*['‘’\"“”]?(.*?)['‘’\"“”]?\s*$", nm)
            out.setdefault(m.group(1).strip().lower(), i)
    return out


def region_keys():
    """-> {region: (dungeon keys, field keys)} from SCOPE."""
    out = collections.OrderedDict()
    for _, label, dk, fk in SCOPE:
        d, f = out.setdefault(region_of(label), ([], []))
        d += dk
        f += fk
    return out


# --- studio rows ----------------------------------------------------------------------------------------------------------------
_STUDIO = {}


def studio():
    """-> groups {id: [(item, weight)]}, mon / fld {(key, npc): (fields, [(item, p)], [(group, p)])}, static {(kind, key): [(item, p)]}.
    Rows from the staged data036 files (census proves them equal to the loose copies); AddToGroup's positional args from the loose text."""
    if _STUDIO:
        return _STUDIO

    def lst(sub, key):
        return [(int(r["1"]), float(r["2"])) for k, i, r in sub if k == key and "1" in r]
    mon, fld, static = {}, {}, {}
    for meth, f, sub in calls(kc("data036", "DropTable.lua"), "deep"):
        if meth == "AddMonsterDropInfo":
            mon[(int(f["DungeonID"]), int(f["MonsterID"]))] = (f, lst(sub, "DropItemList"), lst(sub, "DropGroupList"))
        elif meth == "AddBattleFieldNpcDropInfo":
            fld[(int(f["BattleFieldID"]), int(f["MonsterID"]))] = (f, lst(sub, "DropItemList"), lst(sub, "DropGroupList"))
    for meth, f, sub in calls(kc("data036", "StaticDropTable.lua"), "deep"):
        if meth in ("AddStaticDropInfo", "AddBattleFieldStaticDropInfo"):
            static[("field", int(f["BattleFieldID"])) if "BattleFieldID" in f else ("dungeon", int(f["DungeonID"]))] = lst(sub, "DropItemList")
    groups = collections.defaultdict(list)
    with open(os.path.join(SERVER_DIR, "DropTable.lua"), encoding="utf-8-sig", errors="replace") as fh:
        for m in re.finditer(r"^\s*DropTable:AddToGroup\(\s*(\d+)\s*,\s*(\d+)\s*,\s*([\d.]+)\s*\)", fh.read(), re.M):
            groups[int(m.group(1))].append((int(m.group(2)), float(m.group(3))))
    _STUDIO.update(groups=groups, mon=mon, fld=fld, static=static)
    return _STUDIO


def studio_row(tab, key, npc):
    """-> (exp, ed, ed_property, {'i<id>' | 'g<id>': p}) of one studio row."""
    f, il, gl = studio()[tab][(key, npc)]
    cases = collections.OrderedDict([("i%d" % i, p) for i, p in il] + [("g%d" % g, p) for g, p in gl])
    return f.get("Exp"), f.get("ED"), f.get("EDProperty"), cases


def equal_weights(gid):
    ws = {w for _, w in studio()["groups"].get(gid, [])}
    return len(ws) == 1 and abs(sum(w for _, w in studio()["groups"][gid]) - 100) < 1e-6


# --- pools ----------------------------------------------------------------------------------------------------------------------
def build_pools(reg, items, census):
    rows, left_out = [], []

    def add(pool, region, band, key, npc, i, source):
        why = items.bad(i)
        if why:
            left_out.append("%s %s %s %s %s: %s" % (pool, region, band, key, i, why))
            return
        rows.append(dict(pool=pool, region=region, band=band, key=key, npc=npc, item_id=i, name=ascii_text(items.get(i, "name")),
                         type=items.label(i, "item_type"), grade=items.label(i, "item_grade"), source=source))

    levels = stone_levels(items)
    keys = region_keys()
    for region, R in REGION_DATA.items():
        for tier, lo, hi, band in R["tiers"]:
            ids = items.find(lambda i, d: lo <= i <= hi and d["use_level"] == tier and not d["fashion"])
            src = "item.kim %d-%d use_level %d" % (lo, hi, tier)
            for gname, glabel in R1_GRADES:
                for i in ids:
                    if items.label(i, "item_grade") == glabel and items.label(i, "eqip_position") == "EP_WEAPON_HAND":
                        add("gear-weapon-%s" % gname, region, band, "*", "*", i, src + ", one per class")
                for aname, apos in R1_ARMOUR:
                    for i in ids:
                        if items.label(i, "item_grade") == glabel and items.label(i, "eqip_position") == apos:
                            add("gear-armour-normal" if gname == "normal" else "gear-%s-%s" % (aname, gname), region, band, "*", "*", i, src)
            slv = [lv for lv, (_, _, a, b) in sorted(levels.items()) if a <= tier <= b]
            if not slv:
                left_out.append("%s L%d: no enhancement stone covers the tier" % (region, tier))
                continue
            w, a, slo, shi = levels[slv[0]]
            for i, why in ((w, "weapon"), (a, "armour")):
                add("stone-enh-%s" % why, region, band, "*", "*", i, "item.kim: Lv.%d is for lv %d-%d (studio group 4005 holds Lv.6)" % (slv[0], slo, shi))
        for n, sid in enumerate(R["sets"], 1):
            members = items.find(lambda i, d: d["set_id"] == sid and not d["fashion"])
            for i in members:
                pos = items.label(i, "eqip_position")
                if pos == "EP_WEAPON_HAND":
                    add("set-weapon-%d" % n, region, "*", "*", "*", i, "item.kim set %d" % sid)
                for aname, apos in R1_ARMOUR:
                    if pos == apos:
                        add("set-%s" % aname, region, "*", "*", "*", i, "item.kim set %d" % sid)
        for key in keys[region][0]:
            boss_pools(reg, key, region, add)
    for key in keys.get("secret", ([], []))[0]:
        boss_pools(reg, key, "secret", add)
    add("secret-cube", "secret", "*", "*", "*", 111610, "DropTable 33190 x 948 i111610")
    for i, wt in studio()["groups"][3280]:
        add("secret-supply.%d" % i, "secret", "*", "*", "*", i, "studio group 3280 weight %s" % _fmt(wt))
    # flattened studio groups (unequal weights) and the equal-weight ones kept whole
    for gid, pool in ((4015, "stone-magic"), (4090, "stone-bundle"), (6002, "box-food"), (6004, "box-fruit")):
        for i, wt in studio()["groups"][gid]:
            if gid == 4090 and i in [levels[6][0], levels[6][1]]:
                continue                                        # the bundle's Lv.6 stones become the tier's own (below)
            add("%s.%d" % (pool, i), "*", "*", "*", "*", i, "studio group %d weight %s" % (gid, _fmt(wt)))
    for region, R in REGION_DATA.items():
        for tier, lo, hi, band in R["tiers"]:
            slv = [lv for lv, (_, _, a, b) in sorted(levels.items()) if a <= tier <= b][0]
            for kind, idx in (("weapon", 0), ("armour", 1)):
                add("stone-bundle.enh-%s" % kind, region, band, "*", "*", levels[slv][idx],
                    "studio group 4090's Lv.6 %s stone at the tier's level (Lv.%d)" % (kind, slv))
    for gid, pool in ((6005, "box-buff"), (6006, "box-potion"), (6008, "box-field-potion")):
        for i, wt in studio()["groups"][gid]:
            add(pool, "*", "*", "*", "*", i, "studio group %d (equal weights)" % gid)
    cards = card_items(items)
    scope = {int(k) for _, _, dk, fk in SCOPE for k in dk + fk}
    seen = set()
    for r in census:
        if int(r["key"]) not in scope or r["role"] in NO_ROW_ROLES + ("box",) or r["npc_id"] in seen:
            continue
        c = cards.get(r["npc"].strip().lower())
        if c:
            seen.add(r["npc_id"])
            add("card", "*", "*", "*", r["npc_id"], c, "item.kim card named for NPC %s's templet NAME" % r["npc_id"])
    add("jewel-box", "*", "*", "*", "*", 113276, "DropTable 33250 x 1411 i113276")
    for i in STATIC_COMMON:
        add("static-common", "*", "*", "*", "*", i, "StaticDropTable 33250 / 40024")
    for i in STATIC_REGION_SANDER:
        add("static-region", "*", "*", "*", "*", i, "StaticDropTable 33250 / 40024 (Sander's pair; no Lanox / Elysion pair in item.kim)")
    add("static-box", "*", "*", "*", "*", 111069, "StaticDropTable 33250 i111069")
    add("static-secret-box", "*", "*", "*", "*", 111068, "StaticDropTable 33190 / 33191 i111068")
    return rows, left_out


def boss_pools(reg, key, region, add):
    if key not in reg.dd:
        return
    for name, ids in boss_lists(reg, key):
        for n, i in enumerate(ids, 1):
            if name == "COMMON":
                add("boss-common-%d" % n, region, "*", key, "*", i, "DungeonData %d BOSS_DROP_ITEM.COMMON[%d]" % (key, n))
            else:
                add("boss-class", region, "*", key, "*", i, "DungeonData %d BOSS_DROP_ITEM.%s" % (key, name))


# --- rate sheet -----------------------------------------------------------------------------------------------------------------
# role -> [(table, {diff: studio key}, studio NPC, {case: (kind, pool)})]. Each case is copied as written: a `group` case keeps its outer
# probability; an `item` case of a one-item studio group or a fixed item keeps its value; `flat` spreads an unequal-weight studio group
# over its pool members (pool prefix + '.' + item id) at outer x weight / 100.
GEAR_MOB = [("g181", "group", "gear-weapon-normal"), ("g182", "group", "gear-weapon-rare"), ("g184", "group", "gear-armour-normal"),
            ("g185", "group", "gear-top-rare"), ("g187", "group", "gear-bottom-rare"), ("g189", "group", "gear-gloves-rare"),
            ("g191", "group", "gear-shoes-rare"), ("g4005", "enh", "stone-enh"), ("g4015", "flat", "stone-magic")]
GEAR_BOSS = [("g182", "group", "gear-weapon-rare"), ("g183", "group", "gear-weapon-elite"),
             ("g185", "group", "gear-top-rare"), ("g186", "group", "gear-top-elite"), ("g187", "group", "gear-bottom-rare"),
             ("g188", "group", "gear-bottom-elite"), ("g189", "group", "gear-gloves-rare"), ("g190", "group", "gear-gloves-elite"),
             ("g191", "group", "gear-shoes-rare"), ("g192", "group", "gear-shoes-elite"), ("g4090", "flat", "stone-bundle")]
RATE_TEMPLATES = [
    ("mob", "mon", DIFF3, 1382, GEAR_MOB),
    ("mob-minor", "mon", DIFF3, 1415, GEAR_MOB),
    ("mob-card", "mon", DIFF3, 1378, [("i78776", "item", "card")]),
    ("stage-boss", "mon", DIFF3, 1387, GEAR_BOSS + [("i78777", "item", "card"), ("g1162", "item", "set-bottom"), ("g1164", "item", "set-shoes")]),
    # boss-class: the studio spreads the 8 character weapons over 4 stage bosses at 0.5 each (1387-1389, 1409); one line on the final boss
    # gives the same total per clear, since a role line reaches every NPC of the role
    ("final-boss", "mon", DIFF3, 1411, GEAR_BOSS + [("i113276", "item", "jewel-box"), ("g2127", "item", "boss-common-1"),
                                                    ("g2128", "item", "boss-common-2"), ("g2129", "item", "boss-common-3"),
                                                    ("g1160", "group", "set-weapon-1"), ("g1170", "group", "set-weapon-2")]),
    ("final-boss", "mon", DIFF3, 1387, [("g2130", "item", "boss-class")]),
    # user 2026-11-16 (R1 decision 6): stage bosses also drop each set's top and gloves at Karuso Village's stage-boss values
    ("stage-boss", "mon", collections.OrderedDict([("NORMAL", 33230), ("HARD", 33231), ("EXPERT", 33232)]), 1215,
     [("g1161", "item", "set-top"), ("g1163", "item", "set-gloves")]),
    ("box", "mon", DIFF3, 20, [("g6002", "flat", "box-food"), ("g6004", "flat", "box-fruit"), ("g6005", "group", "box-buff"),
                               ("g6006", "group", "box-potion")]),
    ("field-mob", "fld", {"FIELD": 40024}, 5143, GEAR_MOB),
    ("field-boss", "fld", {"FIELD": 40024}, 5124, []),
    ("box", "fld", {"FIELD": 40024}, 20, [("g6002", "flat", "box-food"), ("g6008", "group", "box-field-potion")]),
]
STATIC_TEMPLATES = [
    ("dungeon", DIFF3, [("i%d" % i, "static-common") for i in STATIC_COMMON] + [("i%d" % i, "static-region") for i in STATIC_REGION_SANDER] +
     [("i111069", "static-box")]),
    ("field", {"FIELD": 40024}, [("i%d" % i, "static-common") for i in STATIC_COMMON] + [("i%d" % i, "static-region") for i in STATIC_REGION_SANDER]),
]
# Studio cases of a template row left out on purpose: 1387's boss weapons go to the final boss (one `boss-class` line), its second set's
# pieces are covered by the same `set-bottom` / `set-shoes` lines (the pools hold both sets); the 1387 row read for the final boss's
# `boss-class` line copies only that case; 1378 (a carded mob) gives only its card, its other cases equal 1382's.
NOT_COPIED = {("stage-boss", 1387): {"g2130", "g2137", "g1172", "g1174"}, ("final-boss", 1387): "*", ("mob-card", 1378): "*",
              ("stage-boss", 1215): "*"}
USER_TEMPLATES = {("stage-boss", 1215): "user 2026-11-16 (R1 decision 6: add set tops / gloves): "}
# The phase's decisions, as `decision` lines gen ignores (OFFLINE_FIELD_REWARDS_PHASES.md "Phase R1 - Done").
DECISIONS = [
    ("boss-class", "user 2026-11-16 (R1 decision 1): class-blind; every listed character weapon can drop for anyone, no emulator filter"),
    ("stone-level", "item.kim text (R1 decision 2, no choice left): Lv.7 for L72 / L76 / L80 (71-81), Lv.8 for L82 / L87 (81-90)"),
    ("field-boss", "user 2026-11-16 (R1 decision 3): EXP + ED only, as DropTable field 40024 x 5124"),
    ("henir-defence", "default kept (R1 decision 4): Henir 38100 and Defence 38300-38302 get no stand-in rows"),
    ("set-pools", "user 2026-11-16 (R1 decision 5): both region sets on every boss, as Behemoth Heart's two sets"),
    ("set-top-gloves", "user 2026-11-16 (R1 decision 6): stage bosses add set tops / gloves at DropTable 33230-33232 x 1215 values"),
]
SECRET_TEMPLATES = [("mob", {"NORMAL": 459, "HARD": 459}), ("mob-minor", {"NORMAL": 944, "HARD": 980}),
                    ("stage-boss", {"NORMAL": 948, "HARD": 984}), ("final-boss", {"NORMAL": 948, "HARD": 984}),
                    ("mob-card", {"NORMAL": 460, "HARD": 460})]
# Velder case -> (kind, pool). The entry list of Velder's DungeonData is COMMON = [111926, 133535, 117392, 117393] (hell: the Hamel / Ranox list),
# so each Velder-only item takes the target's entry item at the same position: 111926 = COMMON[1], the dungeon set's gloves / shoes
# (g3220 / g3230) = COMMON[3] / [4]; hell's four set groups (g3240, g3250, g3260, g3210, in the row's order) = COMMON[2..5]. Velder's two crafting
# materials, 78331 (alchemy item) and 111700 (Green Bethbite), take the list's materials COMMON[1] / [2] (hell: its only material, COMMON[1], which
# then carries 111926 + 111700). A row's slots that land on one entry item add up, so each role keeps the studio row's total.
SECRET_CASES = {
    "NORMAL": {"i111926": ("item", "boss-common-1"), "i111700": ("item", "boss-common-2"), "i78331": ("item", "boss-common-1"), "i111610": ("item", "secret-cube"),
               "g3280": ("flat", "secret-supply"), "g3220": ("item", "boss-common-3"), "g3230": ("item", "boss-common-4")},
    "HARD": {"i111926": ("item", "boss-common-1"), "i111700": ("item", "boss-common-1"), "i111610": ("item", "secret-cube"),
             "g3280": ("flat", "secret-supply"), "g3240": ("item", "boss-common-2"), "g3250": ("item", "boss-common-3"),
             "g3260": ("item", "boss-common-4"), "g3210": ("item", "boss-common-5")},
}
SECRET_CARD_CASES = {"i78671": ("item", "card")}
SECRET_BASE = {"NORMAL": 33190, "HARD": 33191}      # the studio secret dungeon whose rows are the template (common / hell = the census diffs)
REGION_ROWS = ("lanox", "elysion")


def build_sheet():
    """-> (lines, problems)."""
    st = studio()
    lines, problems, covered = [], [], collections.defaultdict(set)

    def line(kind, region, role, diff, pool, prob="", inner="", source="", exp="", ed="", edp=""):
        lines.append(dict(kind=kind, region=region, role=role, diff=diff, pool=pool, exp=exp, ed=ed, ed_property=edp,
                          prob=prob, inner=inner, source=source))

    for role, tab, diffs, npc, cases in RATE_TEMPLATES:
        tname = "DropTable" if tab == "mon" else "DropTable field"
        for diff, key in diffs.items():
            if (key, npc) not in st[tab]:
                problems.append("%s: no studio row %d x %d" % (role, key, npc))
                continue
            exp, ed, edp, have = studio_row(tab, key, npc)
            covered[(role, tab, key)].add(npc)
            for region in REGION_ROWS:
                if role not in ("mob-card",) and not any(l["kind"] == "param" and l["role"] == role and l["diff"] == diff and l["region"] == region
                                                         for l in lines):
                    line("param", region, role, diff, "-", exp=exp, ed=ed, edp=edp, source="%s %d x %d Exp / ED / EDProperty" % (tname, key, npc))
                for c, kind, pool in cases:
                    if c not in have:
                        problems.append("%s %s: studio row %d x %d has no case %s" % (role, diff, key, npc, c))
                        continue
                    p = have[c]
                    src = USER_TEMPLATES.get((role, npc), "") + "%s %d x %d %s %s" % (tname, key, npc, c, _fmt(p))
                    if kind in ("group", "item"):
                        line(kind, region, role, diff, pool, prob=fmt_prob(p), source=src)
                    elif kind == "enh":                     # group 4005: Lv.6 weapon 35 / armour 65 -> the tier's stones, same weights
                        g = dict(st["groups"][int(c[1:])])
                        for name, wt in (("weapon", g[130082]), ("armour", g[130091])):
                            line("item", region, role, diff, "%s-%s" % (pool, name), prob=fmt_prob(p * wt / 100.0),
                                 source="%s x group %s weight %s" % (src, c[1:], _fmt(wt)))
                    elif kind == "flat":
                        for i, wt in st["groups"][int(c[1:])]:
                            sub = "%s.%d" % (pool, i)
                            if c == "g4090" and i in (130082, 130091):
                                sub = "%s.enh-%s" % (pool, "weapon" if i == 130082 else "armour")
                            line("item", region, role, diff, sub, prob=fmt_prob(p * wt / 100.0),
                                 source="%s x group %s weight %s" % (src, c[1:], _fmt(wt)))
            for c in have:
                skip = NOT_COPIED.get((role, npc), set())
                if c not in [x[0] for x in cases] and skip != "*" and c not in skip:
                    problems.append("%s %s: studio case %d x %d %s not copied" % (role, diff, key, npc, c))
    for kind, diffs, cases in STATIC_TEMPLATES:
        for diff, key in diffs.items():
            have = dict(("i%d" % i, p) for i, p in st["static"].get((kind, key), []))
            for region in REGION_ROWS:
                for c, pool in cases:
                    if c not in have:
                        problems.append("static %s %d has no %s" % (kind, key, c))
                        continue
                    if any(l["kind"] == "static" and l["pool"] == pool and l["diff"] == diff and l["region"] == region for l in lines):
                        continue
                    line("static", region, "*", diff, pool, prob=fmt_prob(have[c]), source="StaticDropTable %s %d %s %s" % (kind, key, c, _fmt(have[c])))
    for diff, key in SECRET_BASE.items():                   # the secret dungeons' static row: the four cooking goods + the secret box
        have = dict(("i%d" % i, p) for i, p in st["static"].get(("dungeon", key), []))
        for c, pool in [("i%d" % i, "static-common") for i in STATIC_COMMON] + [("i111068", "static-secret-box")]:
            if c not in have:
                problems.append("static dungeon %d has no %s" % (key, c))
            elif not any(l["kind"] == "static" and l["pool"] == pool and l["diff"] == diff and l["region"] == "secret" for l in lines):
                line("static", "secret", "*", diff, pool, prob=fmt_prob(have[c]), source="StaticDropTable dungeon %d %s %s" % (key, c, _fmt(have[c])))
    # secret dungeons: the studio's Velder secret rows, each Velder-only item replaced by the entry-list item at the same position
    for role, npcs in SECRET_TEMPLATES:
        for diff, npc in npcs.items():
            key = SECRET_BASE[diff]
            if (key, npc) not in st["mon"]:
                problems.append("secret %s: no studio row %d x %d" % (role, key, npc))
                continue
            exp, ed, edp, have = studio_row("mon", key, npc)
            if role != "mob-card":
                line("param", "secret", role, diff, "-", exp=exp, ed=ed, edp=edp, source="DropTable %d x %d Exp / ED / EDProperty" % (key, npc))
            want = SECRET_CARD_CASES if role == "mob-card" else SECRET_CASES[diff]
            acc = collections.OrderedDict()                     # pool -> [probability, sources]: slots that land on one entry item add up
            for c, p in have.items():
                if role == "mob-card" and c not in want:
                    continue                                    # 460's other cases equal 459's
                if c not in want:
                    problems.append("secret %s %s: studio case %d x %d %s has no mapping" % (role, diff, key, npc, c))
                    continue
                src = "DropTable %d x %d %s %s" % (key, npc, c, _fmt(p))
                if want[c][0] == "item":
                    slot = acc.setdefault(want[c][1], [0.0, []])
                    slot[0] += p
                    slot[1].append(src)
                else:
                    for i, wt in st["groups"][int(c[1:])]:
                        slot = acc.setdefault("%s.%d" % (want[c][1], i), [0.0, []])
                        slot[0] += p * wt / 100.0
                        slot[1].append("%s x group %s weight %s" % (src, c[1:], _fmt(wt)))
            for pool, (p, srcs) in acc.items():
                line("item", "secret", role, diff, pool, prob=fmt_prob(p), source=" + ".join(srcs))
    for topic, why in DECISIONS:
        line("decision", "*", "-", "-", topic, source=why)
    return lines, problems


def write_sheet(rows, cols, path, note):
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("# %s\n" % note)
        fh.write("\t".join(cols) + "\n")
        for r in rows:
            fh.write("\t".join(str(r.get(c, "")).replace("\t", " ") for c in cols) + "\n")


def line_key(r):
    return (r["kind"], r["region"], r["role"], r["diff"], r["pool"])


def cmd_pools(a):
    reg, items = Reg(), Items()
    rows, left_out = build_pools(reg, items, read_tsv(a.census, COLUMNS, "census"))
    note = "field-rewards item pools, written by tools/droptable.py pools (OFFLINE_FIELD_REWARDS_PHASES.md R1) - do not hand-edit"
    if a.check:
        old = read_tsv(a.out, R1_POOL_COLS, "pools")
        same = [[str(r[c]) for c in R1_POOL_COLS] for r in rows] == [[r[c] for c in R1_POOL_COLS] for r in old]
        print("POOLS %d row(s); file == fresh pools: %s" % (len(rows), "YES" if same else "NO"))
    else:
        write_sheet(rows, R1_POOL_COLS, a.out, note)
        print("POOLS %d row(s), %d distinct item(s) -> %s" % (len(rows), len({r["item_id"] for r in rows}), os.path.relpath(a.out, TRUNK)))
    cnt = collections.Counter((re.split(r"[-.]", r["pool"])[0], r["region"]) for r in rows)
    print("      " + " ".join("%s/%s=%d" % (p, rg, n) for (p, rg), n in sorted(cnt.items())))
    for p in left_out:
        print("      LEFT OUT " + p)
    return 0 if not a.check or same else 1


def cmd_rates(a):
    lines, problems = build_sheet()
    fresh_keys = {line_key(l) for l in lines}
    user = [r for r in read_tsv(a.out, R1_SHEET_COLS, "rate sheet") if r["source"].startswith("user ") and line_key(r) not in fresh_keys]
    for u in user:                                        # a user decision replaces the generated line it names, or adds one
        lines = [l for l in lines if line_key(l) != line_key(u)] + [u]
    note = ("field-rewards rate sheet (percent per kill; columns: see tools/droptable.py above gen_blocks), written by droptable.py rates "
            "from the studio rows each source names; a line whose source starts 'user <date>' is a user decision and survives a rewrite")
    if a.check:
        old = read_tsv(a.out, R1_SHEET_COLS, "rate sheet")
        fresh = {line_key(l): l for l in lines}
        bad = [r for r in old if line_key(r) not in fresh or any(str(fresh[line_key(r)][c]) != r[c] for c in R1_SHEET_COLS)]
        ok = not bad and len(old) == len(lines)
        print("SHEET %d line(s) (%d user); every other line equals the studio value its source names: %s" % (len(old), len(user), "YES" if ok else "NO"))
        for r in bad:
            print("      DIFFERS " + "\t".join(r[c] for c in R1_SHEET_COLS))
    else:
        write_sheet(lines, R1_SHEET_COLS, a.out, note)
        print("SHEET %d line(s) (%d user) -> %s" % (len(lines), len(user), os.path.relpath(a.out, TRUNK)))
        ok = True
    for p in problems:
        print("      PROBLEM " + p)
    return 0 if ok and not problems else 1


# --- simulate -------------------------------------------------------------------------------------------------------------------
# Two views. Per clear, by category: what a player sees, which mostly measures the dungeon's size, boss count and June's entry lists.
# The gate, per role: expected items per kill of each role against the same role of the baseline (Behemoth Heart at the same difficulty,
# or Sander Field 03), which is what the sheet controls. A baseline NPC whose studio row is food / potion only counts as a chest.
CATEGORIES = ["gear", "stone", "card", "set", "boss", "box"]
GATE_ROLES = ["mob", "mob-minor", "stage-boss", "final-boss", "box", "field-mob", "field-boss"]


def category(pool):
    head = re.split(r"[-.]", pool)[0]
    return {"gear": "gear", "stone": "stone", "card": "card", "set": "set", "boss": "boss", "jewel": "boss", "box": "box"}.get(head, "other")


def studio_category(case, items):
    """Category of a studio case for the baseline: the templates' own mapping, else by what the case holds."""
    for role, tab, diffs, npc, cases in RATE_TEMPLATES:
        for c, kind, pool in cases:
            if c == case:
                return category(pool)
    n = int(case[1:])
    if case[0] == "i" and items.has(n) and items.get(n, "name").startswith("Monster Card"):
        return "card"
    if case == "g4013":
        return "stone"                                  # magic stone 90 / 10, Sander's field rows
    if case[0] == "g" and 1160 <= n <= 1174:
        return "set"
    if case[0] == "g" and 2127 <= n <= 2137:
        return "boss"
    return "other"


def draws():
    with open(os.path.join(TRUNK, "X2Lib", "X2Define.h"), encoding="utf-8", errors="replace") as fh:
        m = re.search(r"SERV_IRUHADEV_OFFLINE_DROP_DRAWS\s*=\s*(\d+)", fh.read())
    return int(m.group(1)) if m else 1


def key_info(census, key):
    rows = [r for r in census if int(r["key"]) == key]
    if not rows:
        return None
    kind = rows[0]["scope"]
    region = next((rg for rg, (dk, fk) in region_keys().items() if key in dk or key in fk), None)
    diff = rows[0]["diff"] if kind == "dungeon" else "FIELD"
    level = max([int(r["level"]) for r in rows if r["level"].isdigit() and r["role"] not in NO_ROW_ROLES + ("box",)] or [0])
    base = key - DIFF_STEP.get(diff, 0) if kind == "dungeon" else key
    return rows, kind, region, diff, level, base


def simulate_stand_in(key, census, pools, sheet):
    """-> (per clear {category: items}, per role {role: [spawns, items]}, ED, notes, info), with `gen`'s semantics: a role's `item` lines
    put every pool item at prob, `group` lines one group at prob; mob-card lines join mob / mob-minor."""
    rows, kind, region, diff, level, base = key_info(census, key)
    out, roles, ed, notes = collections.Counter(), collections.defaultdict(lambda: [0.0, 0.0, 0.0]), 0.0, collections.Counter()
    for r in rows:
        if r["role"] in NO_ROW_ROLES or (r["state"] == "wildcard" and region in KEEP_WILDCARD_REGIONS):
            continue
        cnt, role = float(r["count"]), eff_role(region, r["role"], r["marks"])
        par = pick(sheet, "param", region, role, diff)
        if not par:
            notes["spawns with no param line (role %s)" % role] += cnt
            continue
        if re.match(r"^[A-Z0-9_]+$", r["npc"]) and role != "box":
            notes["spawns with an internal name (dummy / trigger?) counted as %s" % role] += cnt
        lines = pick(sheet, "item", region, role, diff) + pick(sheet, "group", region, role, diff)
        if role in ("mob", "mob-minor"):
            lines += [l for l in sheet if l["kind"] in ("item", "group") and l["role"] == "mob-card" and l["region"] in (region, "*")
                      and l["diff"] in (diff, "*")]
        got = 0.0
        for l in lines:
            its = pool_items(pools, l["pool"], region, level, key, base, int(r["npc_id"]) if r["npc_id"].isdigit() else -1)
            if its:
                x = cnt * (len(its) if l["kind"] == "item" else 1) * float(l["prob"]) / 100.0
                out[category(l["pool"])] += x
                got += x
        if region == "secret" and role == "stage-boss":
            role = "final-boss"
        roles[role][0] += cnt
        roles[role][1] += got
        roles[role][2] += cnt * float(par[0]["ed"] or 0)
        ed += cnt * float(par[0]["ed"] or 0) if role != "box" else 0
    return out, roles, notes, (kind, region, diff, level)


def simulate_studio(key, census, items, tab, secret=False):
    out, roles = collections.Counter(), collections.defaultdict(lambda: [0.0, 0.0, 0.0])
    for r in census:
        if int(r["key"]) != key or r["role"] in NO_ROW_ROLES:
            continue
        npc = int(r["npc_id"]) if r["npc_id"].isdigit() else None
        row = next((k for k in ((key, npc), (0, npc)) if k in studio()[tab]), None)
        cnt, role = float(r["count"]), r["role"]
        if secret:
            role = eff_role("secret", role, r["marks"])
            if role in ("stage-boss", "final-boss"):
                role = "final-boss"                 # the studio's secret rows pay one boss row (Velder hell pays its stage-4 boss, not the last)
                if row is None:
                    continue
            elif row is None:
                continue                            # a spawn the studio gave no row has nothing to compare
        if row is None:
            roles[role][0] += cnt
            continue
        exp, e, edp, cases = studio_row(tab, row[0], npc)
        cats = [studio_category(c, items) for c in cases]
        if cats and all(c == "box" for c in cats):
            role = "box"
        got = 0.0
        for c, p in cases.items():
            out[studio_category(c, items)] += cnt * p / 100.0
            got += cnt * p / 100.0
        roles[role][0] += cnt
        roles[role][1] += got
        roles[role][2] += cnt * float(e or 0)
    return out, roles


def cmd_simulate(a):
    census = read_tsv(a.census, COLUMNS, "census")
    pools = read_tsv(a.pools, R1_POOL_COLS, "pools")
    sheet = read_tsv(a.sheet, R1_SHEET_COLS, "rate sheet")
    items, n = Items(), draws()
    for key in a.keys:
        if key_info(census, key) is None:
            print("%d: no census rows" % key)
            continue
        out, roles, notes, (kind, region, diff, level) = simulate_stand_in(key, census, pools, sheet)
        bkey, btab = (40024, "fld") if kind == "field" else (DIFF3.get(diff, 33250), "mon")
        if region == "secret":
            bkey = SECRET_BASE[diff]
        bout, broles = simulate_studio(bkey, census, items, btab, region == "secret")
        name = next(r["key_name"] for r in census if int(r["key"]) == key)
        print("== %d %s (%s %s, NPC lv %d) against %d" % (key, name, region, diff, level, bkey))
        print("   per clear  " + "  ".join("%s %.2f (x%d %.2f; base %.2f)" % (c, out[c], n, out[c] * n, bout[c]) for c in CATEGORIES if out[c] or bout[c]))
        print("   %-11s %7s %10s %10s %6s %8s  gate: items per kill within 15 %%" % ("role", "spawns", "items/kill", "base", "ratio", "ED/kill"))
        for role in GATE_ROLES:
            if role not in roles and role not in broles:
                continue
            s, it, e = roles.get(role, [0, 0, 0])
            bs, bit, be = broles.get(role, [0, 0, 0])
            if not bs and s:                            # no such role in the baseline's census: its template row alone
                tpl = next(((tb, d, npc) for rl, tb, d, npc, cs in RATE_TEMPLATES if rl == role), None)
                if region == "secret":                  # the secret dungeons' own template rows, not Behemoth Heart's
                    sn = dict(SECRET_TEMPLATES).get(role, {}).get(diff)
                    tpl = ("mon", {diff: SECRET_BASE[diff]}, sn) if sn else None
                if tpl and (tpl[1].get(diff, -1), tpl[2]) in studio()[tpl[0]]:
                    _, e2, _, cases = studio_row(tpl[0], tpl[1][diff], tpl[2])
                    bs, bit, be = 1.0, sum(cases.values()) / 100.0, float(e2 or 0)
            mine, base = (it / s if s else 0.0), (bit / bs if bs else 0.0)
            ratio = mine / base if base else (1.0 if not mine else float("inf"))
            verdict = "n/a" if not s or not bs else ("PASS" if abs(ratio - 1) <= 0.15 else "FAIL")
            print("   %-11s %7s %10.4f %10.4f %6.2f %8s  %s" % (role, _fmt(s), mine, base, ratio,
                                                         "%.1f/%.1f" % (e / s if s else 0, be / bs if bs else 0), verdict))
        for k, v in notes.items():
            print("   note: %s: %s" % (k, _fmt(v)))
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("pools")
    p.add_argument("--out", default=R1_POOLS)
    p.add_argument("--census", default=OUT_DEFAULT)
    p.add_argument("--check", action="store_true", help="build in memory and compare with the file")
    p.set_defaults(fn=cmd_pools)
    r = sub.add_parser("rates")
    r.add_argument("--out", default=R1_SHEET)
    r.add_argument("--check", action="store_true", help="every non-user line equals the studio value its source names")
    r.set_defaults(fn=cmd_rates)
    s = sub.add_parser("simulate")
    s.add_argument("keys", type=int, nargs="+")
    s.add_argument("--census", default=OUT_DEFAULT)
    s.add_argument("--pools", default=R1_POOLS)
    s.add_argument("--sheet", default=R1_SHEET)
    s.set_defaults(fn=cmd_simulate)
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
    g = sub.add_parser("gen")
    g.add_argument("--only", default=None)
    g.add_argument("--keys", default="")
    g.add_argument("--dry-run", action="store_true")
    g.add_argument("--out-dir", default="")
    add_gen_paths(g)
    g.set_defaults(fn=cmd_gen)
    k = sub.add_parser("check")
    k.add_argument("--kim", default="staged")
    add_gen_paths(k)
    k.set_defaults(fn=cmd_check)
    a = ap.parse_args()
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")      # NPC names carry non-ASCII text
    return a.fn(a)


if __name__ == "__main__":
    sys.exit(main())
