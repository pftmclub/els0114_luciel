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
Later phases add: pools, simulate (R1).

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


def pool_items(pools, name, region, level, key, base, npc):
    """The items of pool `name` that apply to this dungeon / field and NPC, in file order, an id once."""
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
        npcs = collections.OrderedDict()
        for r in rows:
            npcs.setdefault(int(r["npc_id"]), []).append(r["role"])
        studio = smon if kind == "dungeon" else sfld
        made = 0
        for npc in sorted(npcs):
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
    return (m.group(1), parse_keys(m.group(2))) if m else (None, None)


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
