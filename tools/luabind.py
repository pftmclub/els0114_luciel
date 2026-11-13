#!/usr/bin/env python3
"""luabind.py - which Lua methods a June script calls that March never bound.

A June script that calls `obj:Name()` for a Name March did not register with lua_tinker dies at that line at run
time: one deduped ErrorLog line ("can't find 'Name' class variable"), the rest of the function silently skipped.
That is how Oberon's slash (GetIsNowPVPCorrectionGame) went missing. This reads the scripts instead of waiting for a
play-test.

  python tools/luabind.py check NAME [NAME ...]    June script(s) by name (june_src_fixed/*/NAME.src.lua), or a path
  python tools/luabind.py check --dir june_src_fixed/data037_kom   every script in a folder, with a per-name census
  python tools/luabind.py names                    list the names March registers (for a spot check)

Exit 1 when a script calls a method that is neither registered in March nor listed in tools/data/lua_unbound_known.tsv
(columns: name, phase, reason; a name goes there only when a phase decided to leave it absent, and says why).
A registration inside a dead #if is counted as bound (a false negative, never a false alarm).
Two more FAIL categories (june-dungeons C3y-2b, 2026-11-13: `slot1:PlayEventScene()` passed because March binds
PlayEventScene as a GLOBAL while June binds it as a CX2DungeonGame method):
  METHOD BOUND ONLY AS GLOBAL  `x:Name(` where March registers Name only through lua_tinker::def / set / decl, never
                               class_def / class_mem: the method call dies at run time ("can't find 'Name'").
  METHOD NOT ON RECEIVER       with `--receiver CLASS`: `x:Name(` whose Name is class_def'd on no class in CLASS plus its
                               class_inh bases (CX2DungeonGame accepts CX2Game methods). The Lua variable's class is not
                               known, so every `:Name(` is held to CLASS unless `--receiver-var slot1,...` limits it to
                               calls on those variables. A name the script defines itself (`function T:Name`) is skipped.

`check` also runs two enum gates (added by char-ara CH1, 2026-11-11, after Ara's June script died at load and her
CHAIN_SYSTEM read nil ids):
  ENUM   every `TABLE.NAME` the script reads, where TABLE is a top-level table of June's Enum.lua, must resolve in the
         staged March data036 Enum.lua (core-list source). A missing TABLE kills the whole file at load ("can't find
         'X' class variable"); a missing NAME reads nil (a C++ reader then pops "StateID" or similar). A NAME June's own
         Enum.lua lacks too reads nil in June as well: listed, not a failure. Fix: add June's member to Enum.lua.
  STATE  every `<X>_STATE_ID.NAME` / `USER_STATE_ID.NAME` / `GAME_UNIT_STATE_ID.NAME` the script reads is evaluated in
         the staged Enum.lua and compared with the compiled C++ enum (cl /Zs probe, X2Lib's US_SERVICE defines). A
         differing value is a failure: Lua hands these ids back to C++ (CHAIN_SYSTEM, training scripts) and a wrong
         one matches the wrong state. A name with no C++ member is a failure too (its value can land on another
         state's id): add the member. Skip with --no-cpp (no compiler run).
An ENUM / STATE exception goes in lua_unbound_known.tsv as `TABLE.NAME` with the reason.
"""
import argparse, collections, csv, glob, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC_DIRS = ("X2Lib", "KTDXLIB", "X2ServerProtocol")
KNOWN = os.path.join(ROOT, "tools", "data", "lua_unbound_known.tsv")
GAP = os.path.join(ROOT, "june2017_re", "c3z4_class_gap.tsv")
KOM_ROOT = os.environ.get("X2_KOM_CONTENTS", r"F:\elsword stuff\elsword_2014\els_2014\237311\24965799\kom_contents")
STAGED_ENUM = os.path.join(KOM_ROOT, "data036_kom", "Enum.lua")
JUNE_ENUM = os.path.join(ROOT, "june_src_fixed", "data036_kom", "Enum.src.lua")
# Lua state-id table -> (C++ class, header); a class script hands these ids back to C++
STATE_TABLES = {
    "GAME_UNIT_STATE_ID": ("CX2GameUnit", "X2GameUnit.h"),
    "USER_STATE_ID": ("CX2GUUser", "X2GUUser.h"),
    "ELSWORD_SWORDMAN_STATE_ID": ("CX2GUElsword_SwordMan", "X2GUElsword_SwordMan.h"),
    "ARME_VIOLEMAGE_STATE_ID": ("CX2GUArme_VioletMage", "X2GUArme_VioletMage.h"),
    "LIRE_ELVENRANGER_STATE_ID": ("CX2GULire_ElvenRanger", "X2GULire_ElvenRanger.h"),
    "RAVEN_STATE_ID": ("CX2GURaven", "X2GURaven.h"),
    "EVE_STATE_ID": ("CX2GUEve", "X2GuEve.h"),
    "CHUNG_STATE_ID": ("CX2GUChung", "X2GUChung.h"),
    "ARA_STATE_ID": ("CX2GUAra", "X2GUAra.h"),
}
ENUM_REF = re.compile(r'\b([A-Z][A-Z0-9_]*)\.([A-Za-z_]\w*)')

# lua_tinker::class_def<T>( L, "Name", ...), decl( L, "Name", ...), def(...), class_mem(...): the name is the 2nd argument
REG = re.compile(r'(class_def|class_mem|class_add|decl|def|set)\s*(?:<([^()]*?)>)?\s*\(\s*[\w>.\-*&()]+?\s*,\s*"(\w+)"')
INH = re.compile(r'class_inh\s*<\s*(\w+)\s*,\s*(\w+)\s*>')
CALL = re.compile(r'(\w+)?:(\w+)\s*\(')
LUA_DEF = re.compile(r'\bfunction\s+[\w.]*[.:](\w+)\s*\(|\b(\w+)\s*=\s*function\b')


class Bindings(object):
    """What March registers: all names, global-only names, {method: {classes}} (class_def / class_mem), class_inh bases."""
    def __init__(self):
        self.names, self.glob = set(), set()
        self.methods, self.bases = collections.defaultdict(set), collections.defaultdict(set)

    def ancestry(self, cls):
        out, todo = set(), [cls]
        while todo:
            c = todo.pop()
            if c not in out:
                out.add(c)
                todo += self.bases.get(c, ())
        return out

    def only_global(self, n):
        return n in self.glob and n not in self.methods


def march_bindings():
    b = Bindings()
    for d in SRC_DIRS:
        for f in glob.glob(os.path.join(ROOT, d, "*.cpp")) + glob.glob(os.path.join(ROOT, d, "*.h")):
            t = open(f, "rb").read().decode("cp949", "replace")
            if "lua_tinker" not in t and "LuaBinder" not in t:
                continue
            for kind, cls, name in REG.findall(t):
                b.names.add(name)
                if kind in ("class_def", "class_mem"):
                    b.methods[name].add(cls.split(",")[0].strip() if cls else "?")
                elif kind != "class_add":
                    b.glob.add(name)
            for cls, base in INH.findall(t):
                b.bases[cls].add(base)
    return b


def march_names():
    return march_bindings().names


def known():
    out = {}
    if os.path.exists(KNOWN):
        for row in csv.reader(open(KNOWN, encoding="utf-8"), delimiter="\t"):
            if row and not row[0].startswith("#") and row[0] != "name":
                out[row[0]] = row[1:]
    return out


def gap_notes():
    out = {}
    if os.path.exists(GAP):
        for row in csv.DictReader(open(GAP, encoding="utf-8"), delimiter="\t"):
            out.setdefault(row["method"], []).append("%s:%s" % (row["june_class"], row["march"]))
    return out


def find_script(name):
    if os.path.exists(name):
        return name
    hits = glob.glob(os.path.join(ROOT, "june_src_fixed", "*", name + ".src.lua"))
    hits += glob.glob(os.path.join(ROOT, "june_src_fixed", "*", name))
    return hits[0] if hits else None


def call_sites(path):
    """([(line, receiver variable or None, method name)], method names the script defines itself in Lua)."""
    t = open(path, encoding="utf-8-sig", errors="replace").read()
    t = re.sub(r"--\[\[.*?\]\]", "", t, flags=re.S)
    t = re.sub(r"--[^\n]*", "", t)
    sites = []
    for no, line in enumerate(t.split("\n"), 1):
        for m in CALL.finditer(line):
            sites.append((no, m.group(1), m.group(2)))
    return sites, {a or b for a, b in LUA_DEF.findall(t)}


def calls_in(path):
    uses = collections.defaultdict(list)
    for no, _, n in call_sites(path)[0]:
        uses[n].append(no)
    return uses


def lua_tables(path):
    """Top-level `NAME = { k = expr, ... }` tables of an Enum.lua -> {table: {member: expr text}}."""
    t = open(path, encoding="utf-8-sig", errors="replace").read().replace("\r\n", "\n")
    t = re.sub(r"--[^\n]*", "", t)
    out = {}
    for m in re.finditer(r"^(\w+) = \{\n(.*?)^\}", t, re.M | re.S):
        out[m.group(1)] = dict(re.findall(r"^\s*(\w+)\s*=\s*([^,\n]+?)\s*,?\s*$", m.group(2), re.M))
    return out


def lua_value(tables, tbl, name, depth=0):
    """Evaluate `k`, `T.N`, `T.N + k` / `- k` as the staged Enum.lua defines it; None when it does not resolve."""
    expr = tables.get(tbl, {}).get(name)
    if expr is None or depth > 8:
        return None
    m = re.match(r"^(-?\d+)$", expr)
    if m:
        return int(m.group(1))
    m = re.match(r"^(\w+)\.(\w+)\s*(?:([+-])\s*(\d+))?$", expr)
    if not m:
        return None
    base = lua_value(tables, m.group(1), m.group(2), depth + 1)
    if base is None:
        return None
    k = int(m.group(4) or 0)
    return base + k if m.group(3) != "-" else base - k


def enum_refs(path):
    t = open(path, encoding="utf-8-sig", errors="replace").read()
    t = re.sub(r"--\[\[.*?\]\]", "", t, flags=re.S)
    t = re.sub(r"--[^\n]*", "", t)
    t = re.sub(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'', '""', t)
    refs = collections.defaultdict(list)
    for no, line in enumerate(t.split("\n"), 1):
        for m in ENUM_REF.finditer(line):
            refs[(m.group(1), m.group(2))].append(no)
    return refs


def cpp_values(exprs):
    """{C++ constant expression: value or None}, by one /Zs probe (june2017_re/enumprobe.py's C2440 trick)."""
    sys.path.insert(0, os.path.join(ROOT, "june2017_re"))
    cwd = os.getcwd()
    os.chdir(os.path.join(ROOT, "june2017_re"))
    try:
        import census as C
    finally:
        os.chdir(cwd)
    heads = sorted({h for _, h in STATE_TABLES.values()})
    src = ["#define private public", "#define protected public", '#include "stdafx.h"'] + ['#include "%s"' % h for h in heads]
    src.append("static int g_probe = 0;")
    base = len(src)
    for i, e in enumerate(exprs):
        src.append("char (*probe_%d)[(%s) + 2] = &g_probe;" % (i, e))
    p = os.path.join(ROOT, "X2Lib", "_luabind_state_probe.cpp")
    open(p, "w", newline="\r\n").write("\n".join(src) + "\n")
    try:
        cmd = [a for a in C.cl_cmd(p, "x.i") if not a.startswith("/Fi") and a != "/P"] + ["/c", "/Zs", "/EHsc"]
        r = subprocess.run(cmd, cwd=os.path.join(ROOT, "X2Lib"), env=C.cl_env(), capture_output=True)
    finally:
        os.remove(p)
    out = r.stdout.decode("latin1")
    vals = dict.fromkeys(exprs)
    for m in re.finditer(r"_luabind_state_probe\.cpp\((\d+)\) : error C2440.*?char \(\*\)\[(\d+)\]", out):
        vals[exprs[int(m.group(1)) - base - 1]] = int(m.group(2)) - 2
    if "C2440" not in out and exprs:
        print("   (probe produced no values; compiler output tail follows)\n" + out[-1500:])
    return vals


def enum_gate(files, kn, use_cpp):
    """ENUM and STATE gates (module docstring); returns the number of failures."""
    staged, june = lua_tables(STAGED_ENUM), lua_tables(JUNE_ENUM)
    fails = 0
    state_refs = {}
    for p in files:
        refs = enum_refs(p)
        rel = os.path.relpath(p, ROOT)
        miss_tbl, miss_name, june_nil = [], [], []
        for (tbl, name), lines in sorted(refs.items()):
            if tbl not in june and tbl not in staged:
                continue
            key = "%s.%s" % (tbl, name)
            if key in kn or ("%s.*" % tbl) in kn:
                continue
            if tbl not in staged:
                miss_tbl.append((key, lines))
            elif name not in staged[tbl]:
                (miss_name if name in june.get(tbl, {}) else june_nil).append((key, lines))
            elif tbl in STATE_TABLES:
                state_refs[(tbl, name)] = lines
        if len(files) <= 12 or miss_tbl or miss_name:
            print("%s: ENUM %d table(s) missing, %d member(s) missing, %d nil in June too" % (rel, len(miss_tbl), len(miss_name), len(june_nil)))
        for key, lines in miss_tbl:
            print("   ENUM MISSING TABLE  %-50s line %s   (the file dies at load: add June's table to Enum.lua)" % (key, ",".join(map(str, lines[:4]))))
        for key, lines in miss_name:
            print("   ENUM MISSING NAME   %-50s line %s   (reads nil: add June's member to Enum.lua)" % (key, ",".join(map(str, lines[:4]))))
        for key, lines in (june_nil if len(files) <= 12 else []):
            print("   enum nil in June    %-50s line %s" % (key, ",".join(map(str, lines[:4]))))
        fails += len(miss_tbl) + len(miss_name)
    if not use_cpp or not state_refs:
        if state_refs:
            print("STATE: %d state-id reference(s) not compared (--no-cpp)" % len(state_refs))
        return fails
    keys = sorted(state_refs)
    exprs = ["%s::%s" % (STATE_TABLES[t][0], n) for t, n in keys]
    vals = cpp_values(exprs)
    bad = 0
    for (t, n), e in zip(keys, exprs):
        lv, cv = lua_value(staged, t, n), vals[e]
        if cv is None:
            print("   STATE NO C++ MEMBER %-50s Lua %s   (add the member to %s, then mirror its value)" % ("%s.%s" % (t, n), lv, STATE_TABLES[t][1]))
            bad += 1
        elif lv != cv:
            print("   STATE MISMATCH      %-50s Lua %s, C++ %s   (Enum.lua must carry the C++ value)" % ("%s.%s" % (t, n), lv, cv))
            bad += 1
    print("STATE: %d state-id reference(s) compared with the C++ enum, %d wrong" % (len(keys), bad))
    return fails + bad


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cmd", choices=("check", "names"))
    ap.add_argument("targets", nargs="*", help="script name or path")
    ap.add_argument("--dir", help="check every *.lua in this folder")
    ap.add_argument("--no-cpp", action="store_true", help="skip the STATE gate's compiler probe")
    ap.add_argument("--receiver", help="C++ class the scripts' `x:Name(` receivers are (e.g. CX2DungeonGame): every name must be class_def'd on it or a class_inh base")
    ap.add_argument("--receiver-var", help="with --receiver: comma list of Lua variable names to check (default: every receiver)")
    a = ap.parse_args()
    bnd = march_bindings()
    reg = bnd.names
    if a.cmd == "names":
        print("\n".join(sorted(reg)))
        print("# %d names" % len(reg), file=sys.stderr)
        return 0

    files = []
    for t in a.targets:
        p = find_script(t)
        if not p:
            print("NOT FOUND: %s (june_src_fixed/*/%s.src.lua)" % (t, t))
            return 2
        files.append(p)
    if a.dir:
        files += sorted(glob.glob(os.path.join(a.dir, "*.lua")))
    if not files:
        ap.error("give a script name or --dir")

    kn, notes = known(), gap_notes()
    new = collections.defaultdict(list)
    glob_only, off_recv = collections.defaultdict(list), collections.defaultdict(list)
    allowed = bnd.ancestry(a.receiver) if a.receiver else set()
    rvars = set(a.receiver_var.split(",")) if a.receiver_var else None
    bad_files = 0
    for p in files:
        sites, own = call_sites(p)
        uses = collections.defaultdict(list)
        for no, _, n in sites:
            uses[n].append(no)
        unb = {n: l for n, l in uses.items() if n not in reg and n not in kn}
        # a name bound only as a global (def/set/decl) is not callable as a method; a name class_def'd off the receiver's classes neither
        gl = {n: l for n, l in uses.items() if n in reg and n not in kn and n not in own and bnd.only_global(n)}
        rc = {}
        if a.receiver:
            for no, rv, n in sites:
                if n in reg and n not in kn and n not in own and n not in gl and (rvars is None or rv in rvars) and not (bnd.methods.get(n, set()) & allowed):
                    rc.setdefault(n, []).append(no)
        if len(files) <= 12 or unb or gl or rc:
            print("%s: %d method(s) called, %d unbound, %d only-global, %d off-receiver" % (os.path.relpath(p, ROOT), len(uses), len(unb), len(gl), len(rc)))
        for n, lines in sorted(unb.items()):
            print("   UNBOUND %-40s line %s   %s" % (n, ",".join(map(str, lines[:4])), " ".join(notes.get(n, ["(not in c3z4_class_gap.tsv)"]))))
            new[n].append(os.path.basename(p))
        for n, lines in sorted(gl.items()):
            print("   METHOD BOUND ONLY AS GLOBAL %-34s line %s   (March: lua_tinker::def global; June: class_def method, bind it on the class)" % (n, ",".join(map(str, lines[:4]))))
            glob_only[n].append(os.path.basename(p))
        for n, lines in sorted(rc.items()):
            print("   METHOD NOT ON RECEIVER %-39s line %s   (%s bases: %s; class_def'd on: %s)" % (n, ",".join(map(str, lines[:4])), a.receiver, ",".join(sorted(allowed - {a.receiver})) or "none", ",".join(sorted(bnd.methods[n])[:4])))
            off_recv[n].append(os.path.basename(p))
        bad_files += bool(unb or gl or rc)
    if len(files) > 12:
        print("\n== census: %d unbound name(s) across %d of %d script(s)" % (len(new), bad_files, len(files)))
        for n, fs in sorted(new.items(), key=lambda kv: -len(kv[1]))[:40]:
            print("   %4d script(s)  %s" % (len(fs), n))
    efails = enum_gate(files, kn, not a.no_cpp)
    why = []
    if new:
        why.append("%d unbound name(s): bind each whole from June's function, or add it to tools/data/lua_unbound_known.tsv with the reason" % len(new))
    if glob_only:
        why.append("%d name(s) bound only as a global (called as methods): bind the name with class_def on the receiver's class (June's binding), see METHOD BOUND ONLY AS GLOBAL" % len(glob_only))
    if off_recv:
        why.append("%d method name(s) not bound on %s or its bases: see METHOD NOT ON RECEIVER" % (len(off_recv), a.receiver))
    if efails:
        why.append("%d ENUM / STATE failure(s): fix Enum.lua (data036, core-list source) or the C++ enum, see the lines above" % efails)
    print("RESULT: %s" % ("FAIL, " + "; ".join(why) if why else "PASS"))
    return 1 if why else 0


if __name__ == "__main__":
    sys.exit(main())
