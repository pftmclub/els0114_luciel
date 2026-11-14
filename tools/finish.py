"""finish.py - run every pre-commit gate of a phase in one command and print one block for the Done note.

    python tools/finish.py --files X2Lib/X2GageUI.cpp X2Lib/X2GageManager.cpp      # uncommitted work vs HEAD
    python tools/finish.py --range c29941ce^..c29941ce                              # one committed phase (exactly A..B)
    python tools/finish.py --commits 1a2b3c4 5d6e7f8                                # several commits, each checked as c^..c
    python tools/finish.py --review --range <first>^..<last>                        # a review: also junediverge --check-all
    python tools/finish.py --files ... --flag SERV_IRUHADEV_X                       # name a flag the changed lines do not show
    python tools/finish.py --files ... --skip-flag SERV_IRUHADEV_Y "guard target, not this phase's flag"
    python tools/finish.py --files ... --no-flag "comment-only edit, no behaviour"  # say why no flag applies
    python tools/finish.py --files X2Lib/X2GUEL.cpp --state X2Lib/X2GUEL.cpp ELESIS_KNIGHT --icons 25000-25999 --lua ELESIS_KNIGHT
    python tools/finish.py --selftest

Flags run are those on a changed directive line (#if / #ifdef / #elif / #else / #endif / #define) and those of the INNERMOST #if block that
encloses a changed line (a body edit inside `#ifdef SERV_IRUHADEV_X` names X though the diff never shows the directive). Outer enclosing
flags are printed on one INFO line and not run (the change sits inside their blocks; --flag NAME runs one). A changed commented-out
`//#define X` is `retired` and checks nothing (a debug retire). Runs, for the code files given:
  flagcheck   once per flag run / given (every *_DEBUG flag found is passed as --with to each run); --worktree --files for
              --files, --range for --range, --commits for --commits. A passing flag prints its `files:` line; -v lists the OTHER rows
  junediverge --check <files under X2 / X2Lib / KTDXLIB>; --review adds --check-all (which opens only files with a June flag)
  enumscan    --check <the same files>
Runs only when you give the arguments, and prints SKIPPED with the reason otherwise: stateends (--state FILE SCRIPT, repeatable),
skillicons (--icons RANGE ...), luabind (--lua NAME ...).
Range / commit runs read the WORKING TREE for junediverge and enumscan (they have no revision mode); files deleted since are skipped there.

FAIL, never a silent PASS: a git call that fails (exit 2); a client file changed with no flag found or given (pass --flag, or
--no-flag REASON); a code file outside the known code dirs (exit 2). `INERT` = flagcheck's only complaint is that a `_LIVE` flag is
not #defined yet (inert transcription: _CORE first, _LIVE later) and nothing else failed; it does not fail the run.
Not run here (slow or conditional): build.py --deploy and exestr.py. Their lines go into the Done note beside this block.
Exit 0 when no gate FAILED (SKIPPED / INERT do not fail), 1 on any FAIL, 2 on bad input or a failed git call.
"""
import argparse, datetime, os, re, subprocess, sys

for _s in (sys.stdout, sys.stderr):
    try:
        _s.reconfigure(encoding="utf-8", errors="replace")
    except Exception:
        pass

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PY = sys.executable
CODE_EXT = (".cpp", ".h", ".c", ".hpp", ".inl", ".cxx")                                   # = flagcheck.CODE_EXT
CODE_DIRS = ("X2/", "X2Lib/", "KTDXLIB/", "X2ServerProtocol/", "KncWX2Server/")            # = flagcheck.CODE_DIRS
CLIENT_DIRS = ("X2/", "X2Lib/", "KTDXLIB/")                                                # junediverge / enumscan scope
FLAG_RE = re.compile(rb"SERV_IRUHADEV_[A-Z0-9_]+")
DIRECTIVE_RE = re.compile(rb"^\s*#\s*(ifdef|ifndef|if|elif|else|endif)\b(.*)$")
DEFINE_RE = re.compile(rb"^\s*(//\s*)?#\s*(?:define|undef)\b(.*)$")                      # group 1 set = commented out (retired)
CLASS_FILE_RE = re.compile(r"^X2Lib/X2GU[A-Za-z0-9_]*\.(cpp|h)$")
RANGE_RE = re.compile(r"^[^\s.][^\s]*\.\.[^\s.][^\s]*$")


def die(msg):
    print("finish.py: " + msg, file=sys.stderr)
    sys.exit(2)


def run(cmd):
    """-> (returncode, text). Bytes in, decoded with replacement: sources and tool output may be CP949."""
    p = subprocess.run(cmd, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    return p.returncode, p.stdout.decode("utf-8", errors="replace")


def git_try(args):
    p = subprocess.run(["git", "-c", "core.quotepath=false"] + args, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return p.returncode, p.stdout, p.stderr.decode("utf-8", "replace").strip()


def git(args):
    rc, out, err = git_try(args)
    if rc:
        die("git %s failed (exit %d): %s" % (" ".join(args[:4]), rc, err.splitlines()[-1] if err else "no message"))
    return out


def norm(p):
    """Repo-relative, forward slashes, no ./ segments; an absolute path under the repo is made relative."""
    p = os.path.normpath(os.path.join(ROOT, p)) if not os.path.isabs(p) else os.path.normpath(p)
    try:
        rel = os.path.relpath(p, ROOT)
    except ValueError:
        return p.replace("\\", "/")
    return rel.replace("\\", "/")


def is_code(p):
    return p.lower().endswith(CODE_EXT)


def check_range(r):
    if "..." in r or not RANGE_RE.match(r):
        die("--range must be exactly A..B (two revisions, no '...'): got %r" % r)
    a, b = r.split("..", 1)
    for rev in (a, b):
        git(["rev-parse", "--verify", "--quiet", rev + "^{commit}"])
    return a, b


def parse_hunks(diff):
    """-> {path: set(new-side line numbers touched)} from `git diff -U0`."""
    out, cur, prev = {}, None, b""
    for line in diff.split(b"\n"):
        was, prev = prev, line
        if line.startswith(b"+++ ") and was.startswith(b"--- "):       # a header only directly after its `--- ` line, never an added `++ x` line
            t = line[4:].split(b"\t")[0].strip()
            cur = None if t == b"/dev/null" else t[2:].decode("utf-8", "replace") if t.startswith(b"b/") else t.decode("utf-8", "replace")
            if cur:
                out.setdefault(cur, set())
        elif line.startswith(b"@@") and cur:
            m = re.match(rb"@@ -\d+(?:,\d+)? \+(\d+)(?:,(\d+))? @@", line)
            if m:
                start, cnt = int(m.group(1)), (int(m.group(2)) if m.group(2) is not None else 1)
                out[cur].update(range(start, start + max(cnt, 1) + (1 if cnt == 0 else 0)))
    return out


def enclosing_flags(text, lines):
    """-> (run, outer, retired) sets of SERV_IRUHADEV_ names for the given 1-based lines of a source (bytes), via a small #if stack.
    run     = flags on a changed directive line (#if / #ifdef / #elif / #else / #endif / #define) plus the flags of the INNERMOST enclosing
              block that has any; a run for an outer block proves nothing (the change sits inside it), so those go to
    outer   = reported on one line, not run.
    retired = flags of a changed commented-out define (`//#define X`): nothing left to check."""
    run, outer, retired, stack = set(), set(), set(), []
    for n, raw in enumerate(text.split(b"\n"), 1):
        m = DIRECTIVE_RE.match(raw)
        own = set()
        if m:
            kind, rest = m.group(1), m.group(2)
            own = set(FLAG_RE.findall(rest))
            if kind in (b"if", b"ifdef", b"ifndef"):
                stack.append(set(own))
            elif kind == b"elif" and stack:
                stack[-1] |= own
            elif kind == b"endif" and stack:
                stack.pop()
        if n in lines:
            d = DEFINE_RE.match(raw)
            if d:
                flags = set(FLAG_RE.findall(d.group(2)))
                (retired if d.group(1) else run).update(flags)
            run |= own
            frames = [s for s in stack if s]
            if frames:
                run |= frames[-1]
                for s in frames[:-1]:
                    outer |= s
    dec = lambda s: {f.decode() for f in s}
    run, outer, retired = dec(run), dec(outer), dec(retired)
    return run, outer - run, retired - run


def read_at(rev, path):
    if rev is None:
        try:
            with open(os.path.join(ROOT, path), "rb") as fh:
                return fh.read()
        except OSError:
            return None
    rc, out, _ = git_try(["show", "%s:%s" % (rev, path)])
    return out if rc == 0 else None


def collect(specs, files):
    """specs = [(a, b)]; b None = working tree. -> (flags to run, outer flags not run, retired flags, changed paths)."""
    flags, outer, retired, changed = set(), set(), set(), set()
    for a, b in specs:
        args = ["diff", "-U0", "--no-color", a] + ([b] if b else []) + ["--"] + files
        for path, lines in parse_hunks(git(args)).items():
            if not is_code(path) or not path.startswith(CODE_DIRS):
                continue
            changed.add(path)
            text = read_at(b, path)
            if text is not None:
                r, o, d = enclosing_flags(text, lines)
                flags |= r
                outer |= o
                retired |= d
        if b is None:
            _, new, _ = git_try(["ls-files", "--others", "--exclude-standard", "-z", "--"] + files)
            for path in [x.decode("utf-8", "replace") for x in new.split(b"\0") if x]:
                text = read_at(None, path)
                if text is not None and is_code(path) and path.startswith(CODE_DIRS):
                    changed.add(path)
                    r, o, d = enclosing_flags(text, set(range(1, text.count(b"\n") + 2)))
                    flags |= r
                    outer |= o
                    retired |= d
    return flags, outer - flags, retired - flags, changed


def result_line(text):
    for line in reversed(text.splitlines()):
        if line.strip().startswith("RESULT:"):
            return line.strip()
    return ""


ROW_RE = re.compile(r"^\s+(OTHER|DEFINE|FAIL|ERROR|PASS~)\s")


VERBOSE = False
NONZERO_RE = re.compile(r"\b[1-9]\d* (?:deleted|EOL-mismatched|encoding|non-ASCII|#else)")


def flagcheck_detail(text, brief=False):
    """The summary lines and every non-PASS row, so 'explain every OTHER row and deleted line' is answerable from the block.
    brief (a passing run, not -v): the `files:` line, plus the in-this-flag's-files line only when a deleted / EOL / encoding / non-ASCII
    count is non-zero; the OTHER rows (changes that belong to other flags) are listed with -v. Keeps a stage review inside the Done note."""
    keep, rows = [], []
    for l in text.splitlines():
        s = l.strip()
        if s.startswith("files:"):
            keep.append(s)
        elif s.startswith("in this flag's files:"):
            if not brief or NONZERO_RE.search(s):
                keep.append(s)
        elif s.startswith("define:") and not brief:
            keep.append(s)
        elif ROW_RE.match(l) and not brief:
            rows.append(s)
    return keep + rows[:12] + (["... %d more rows (-v)" % (len(rows) - 12)] if len(rows) > 12 else [])


def only_undefined_live(flag, text):
    """True only when flagcheck's sole complaint is 'not #defined' for a _LIVE flag and every count says nothing else failed."""
    if not flag.endswith("_LIVE") or "NOTHING CHECKED" in text:
        return False
    m = re.search(r"(\d+) FAIL, (\d+) ERROR", text)
    if not m or m.group(1) != "0" or m.group(2) != "0":
        return False
    for key in (r"deleted line\(s\)", r"EOL-mismatched inserted line\(s\)", r"encoding change\(s\)"):
        k = re.search(r"(\d+) " + key, text)
        if k and k.group(1) != "0":
            return False
    fails = [l for l in text.splitlines() if l.strip().startswith("FAIL:")]
    return bool(fails) and all("not #defined anywhere tracked" in l for l in fails)


def gate(name, cmd, rows, flag=None):
    print("  running %s ..." % name, file=sys.stderr, flush=True)
    rc, text = run(cmd)
    note = ""
    res = result_line(text)
    if rc and flag and only_undefined_live(flag, text):
        rows.append((name, "INERT", "flag not #defined yet (_LIVE); flagcheck's only complaint", flagcheck_detail(text), ""))
        return
    detail = flagcheck_detail(text, brief=(rc == 0 and not VERBOSE)) if flag else []
    rows.append((name, "PASS" if rc == 0 else "FAIL", (res or ("exit %d" % rc)) + note, detail, text if rc else ""))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--files", nargs="+", default=[], help="your changed files (uncommitted run)")
    ap.add_argument("--range", help="exactly A..B (post-commit run); files come from the range")
    ap.add_argument("--commits", nargs="+", default=[], help="commits, each checked as c^..c")
    ap.add_argument("--review", action="store_true", help="also run junediverge --check-all (a review)")
    ap.add_argument("--flag", nargs="+", default=[], help="flags to check in addition to those found")
    ap.add_argument("--skip-flag", nargs=2, action="append", metavar=("FLAG", "REASON"), default=[])
    ap.add_argument("--no-flag", metavar="REASON", help="client files changed but no SERV_IRUHADEV_ flag applies: say why")
    ap.add_argument("--state", nargs=2, action="append", metavar=("CLASS_FILE", "SCRIPT"), default=[])
    ap.add_argument("--icons", nargs="+", default=[], metavar="RANGE")
    ap.add_argument("--lua", nargs="+", default=[], metavar="NAME")
    ap.add_argument("--selftest", action="store_true")
    ap.add_argument("-v", "--verbose", action="store_true", help="print each gate's full output")
    a = ap.parse_args()
    global VERBOSE
    VERBOSE = a.verbose

    if a.selftest:
        return selftest()
    modes = [bool(a.files), bool(a.range), bool(a.commits)]
    if sum(modes) != 1:
        die("give exactly one of --files, --range A..B, --commits c1 c2 ...")

    if a.range:
        ra, rb = check_range(a.range)
        specs, label = [(ra, rb)], a.range
        files = []
    elif a.commits:
        for c in a.commits:
            git(["rev-parse", "--verify", "--quiet", c + "^{commit}"])
        specs, label = [(c + "^", c) for c in a.commits], "commits " + ",".join(c[:8] for c in a.commits)
        files = []
    else:
        files = [norm(f) for f in a.files]
        for f in files:
            if os.path.isdir(os.path.join(ROOT, f)):
                die("a directory is not a file list (it would pull in peers' changes): %s" % f)
            if not os.path.exists(os.path.join(ROOT, f)) and git_try(["ls-files", "--error-unmatch", "--", f])[0] != 0:
                die("no such file (and not a tracked deletion): %s" % f)
            if is_code(f) and not f.startswith(CODE_DIRS):
                die("code file outside the known code dirs %s: %s" % ("/".join(d.rstrip("/") for d in CODE_DIRS), f))
        specs, label = [("HEAD", None)], "worktree"

    flags, outer, retired, changed = collect(specs, files)
    if a.range or a.commits:
        names = set()
        for sa, sb in specs:
            names |= {x.decode("utf-8", "replace") for x in git(["diff", "--name-only", "-z", sa, sb]).split(b"\0") if x}
        changed = {n for n in names if is_code(n) and n.startswith(CODE_DIRS)} | changed
    else:
        changed = {f for f in files if is_code(f)} | changed
    code = sorted(changed)
    client_now = [f for f in code if f.startswith(CLIENT_DIRS) and os.path.exists(os.path.join(ROOT, f))]

    rows = []
    skip = {f if f.startswith("SERV_IRUHADEV_") else "SERV_IRUHADEV_" + f: why for f, why in a.skip_flag}
    flags |= {f if f.startswith("SERV_IRUHADEV_") else "SERV_IRUHADEV_" + f for f in a.flag}
    for f, why in sorted(skip.items()):
        flags.discard(f)
        rows.append(("flagcheck " + f.replace("SERV_IRUHADEV_", ""), "SKIPPED", "skipped by you: " + why, [], ""))
    retired -= flags
    for f in sorted(retired):
        rows.append(("flagcheck " + f.replace("SERV_IRUHADEV_", ""), "SKIPPED", "retired: its #define is commented out, nothing left to check", [], ""))
    if outer - flags - retired:
        rows.append(("enclosing flags (not run)", "INFO", ", ".join(sorted(x.replace("SERV_IRUHADEV_", "") for x in outer - flags - retired))
                     + "  (the change sits inside their blocks; add --flag NAME to run one)", [], ""))
    debug = sorted(f for f in flags if f.endswith("_DEBUG"))
    base = sorted(f for f in flags if f not in debug) or debug
    withs = [d for d in debug if d not in base]

    if not code:
        rows.append(("code gates", "SKIPPED", "no source file among the changes", [], ""))
    else:
        if not base and not retired:
            if a.no_flag:
                rows.append(("flagcheck", "SKIPPED", "no flag applies: " + a.no_flag, [], ""))
            else:
                rows.append(("flagcheck", "FAIL", "no SERV_IRUHADEV_ flag left to check (found none, or all skipped): pass --flag NAME, or --no-flag \"reason\"", [], ""))
        for f in base:
            cmd = [PY, "tools/flagcheck.py", f]
            if a.range:
                cmd += ["--range", a.range]
            elif a.commits:
                cmd += ["--commits"] + a.commits
            else:
                cmd += ["--worktree", "--files"] + code
            if withs:
                cmd += ["--with"] + withs
            gate("flagcheck " + f.replace("SERV_IRUHADEV_", ""), cmd, rows, flag=f)
        if client_now:
            gate("junediverge", [PY, "tools/junediverge.py", "--check"] + client_now, rows)
        else:
            rows.append(("junediverge", "SKIPPED", "no X2 / X2Lib / KTDXLIB file left in the working tree", [], ""))
        if a.review:    # --check-all opens only files that hold a SERV_IRUHADEV_JUNE_ flag, so it adds to --check, never replaces it
            gate("junediverge --check-all", [PY, "tools/junediverge.py", "--check-all"], rows)
        if client_now:
            gate("enumscan", [PY, "tools/enumscan.py", "--check"] + client_now, rows)
        else:
            rows.append(("enumscan", "SKIPPED", "no X2 / X2Lib / KTDXLIB file left in the working tree", [], ""))

    if a.state:
        for cf, script in a.state:
            gate("stateends " + os.path.basename(cf), [PY, "tools/stateends.py", "check", cf, script], rows)
    elif any(CLASS_FILE_RE.match(f) for f in code):
        rows.append(("stateends", "SKIPPED", "a class file changed: pass --state FILE SCRIPT if skill states were registered", [], ""))
    else:
        rows.append(("stateends", "SKIPPED", "not requested (--state FILE SCRIPT when a phase registers skill states)", [], ""))
    if a.icons:
        gate("skillicons", [PY, "tools/skillicons.py", "check"] + a.icons, rows)
    else:
        rows.append(("skillicons", "SKIPPED", "not requested (--icons RANGE when a phase adds skills)", [], ""))
    if a.lua:
        gate("luabind", [PY, "tools/luabind.py", "check"] + a.lua, rows)
    else:
        rows.append(("luabind", "SKIPPED", "not requested (--lua NAME when a phase stages or depends on a June script)", [], ""))

    print("FINISH %s  %s  files=%d client=%d  flags=%s" % (datetime.date.today().isoformat(), label, len(code), len(client_now),
          ",".join(f.replace("SERV_IRUHADEV_", "") for f in base) or "-"))
    bad = skipped = 0
    for name, status, res, detail, text in rows:
        print("  %-34s %-8s %s" % (name, status, res))
        for d in detail:
            print("      " + d)
        if status == "FAIL":
            bad += 1
        elif status == "SKIPPED":
            skipped += 1
        if text and (status == "FAIL" or a.verbose):
            print("\n".join("      | " + l for l in text.strip().splitlines()[-25:]))
    gates = sum(1 for r in rows if r[1] != "INFO")
    print("FINISH RESULT: %s (%d gates, %d failed, %d skipped)" % ("FAIL" if bad else "PASS", gates, bad, skipped))
    return 1 if bad else 0


def selftest():
    ok = True

    def chk(cond, what):
        nonlocal ok
        if not cond:
            print("  selftest FAIL: " + what)
            ok = False

    chk(is_code("X2Lib/A.cpp") and not is_code("tools/a.py") and is_code("KncWX2Server/Common/ServerDefine.h"), "is_code")
    chk(norm(".\\X2Lib\\A.cpp") == "X2Lib/A.cpp" and norm("./././X2Lib/A.cpp") == "X2Lib/A.cpp", "norm")
    chk(norm(os.path.join(ROOT, "X2Lib", "A.cpp")) == "X2Lib/A.cpp", "norm absolute")
    chk(result_line("a\nRESULT: PASS\nb") == "RESULT: PASS" and result_line("nothing") == "", "result_line")
    chk(bool(CLASS_FILE_RE.match("X2Lib/X2GUEL.cpp")) and not CLASS_FILE_RE.match("X2Lib/X2GageUI.cpp"), "class file re")
    for bad in ("abc", "a...b", "a..", "..b", "a b..c"):
        chk(not RANGE_RE.match(bad) or "..." in bad, "range re rejects " + bad)
    chk(bool(RANGE_RE.match("c29941ce^..c29941ce")), "range re accepts A^..B")
    # the flag stack: a body edit inside a nested block names both flags, a line after #endif names neither
    src = (b"int a;\n#ifdef SERV_IRUHADEV_A\n#ifdef SERV_IRUHADEV_B\nbody;\n#else\nold;\n#endif SERV_IRUHADEV_B\n#endif SERV_IRUHADEV_A\nafter;\n"
           b"#if defined( SERV_IRUHADEV_C ) && !defined( SERV_IRUHADEV_D )\n#error x\n#endif SERV_IRUHADEV_C\n")
    S = lambda *n: {"SERV_IRUHADEV_" + x for x in n}
    chk(enclosing_flags(src, {4}) == (S("B"), S("A"), set()), "nested body: innermost runs, outer is listed")
    chk(enclosing_flags(src, {6}) == (S("B"), S("A"), set()), "#else body")
    chk(enclosing_flags(src, {9}) == (set(), set(), set()) and enclosing_flags(src, {1}) == (set(), set(), set()), "outside blocks")
    chk(enclosing_flags(src, {10}) == (S("C", "D"), set(), set()), "guard line names both")
    chk(enclosing_flags(src, {12}) == (S("C"), set(), set()), "endif token names its flag")
    chk(enclosing_flags(src, {3}) == (S("B"), S("A"), set()), "a changed #ifdef line runs its own flag, the enclosing one is outer")
    dsrc = b"//#define SERV_IRUHADEV_OLD_DEBUG\n#define SERV_IRUHADEV_NEW\nint x;\n"
    chk(enclosing_flags(dsrc, {1}) == (set(), set(), S("OLD_DEBUG")), "commented-out define is retired")
    chk(enclosing_flags(dsrc, {2}) == (S("NEW"), set(), set()), "a changed #define names its flag")
    chk(enclosing_flags(dsrc, {3}) == (set(), set(), set()), "plain line after defines names nothing")
    h = b"diff --git a/X2Lib/A.cpp b/X2Lib/A.cpp\n--- a/X2Lib/A.cpp\n+++ b/X2Lib/A.cpp\n@@ -5,0 +6,2 @@\n+x\n+y\n@@ -9 +12 @@\n-o\n+n\n"
    chk(parse_hunks(h) == {"X2Lib/A.cpp": {6, 7, 12}}, "parse_hunks")
    h2 = b"diff --git a/X b/X\n--- a/X\n+++ b/X\n@@ -1,0 +2,2 @@\n+++ m_i;\n+y\n@@ -9,0 +12 @@\n+z\n"
    chk(parse_hunks(h2) == {"X": {2, 3, 12}}, "an added '++ x' line is not a file header")
    undef = ("  FAIL: the flag is not #defined anywhere tracked (add it to KTDXLIB/Always.h)\nRESULT: FAIL\n"
             "files: 0 PASS, 0 PASS~ (blank/marker only), 0 DEFINE, 4 OTHER (other flags), 0 FAIL, 0 ERROR\n"
             "in this flag's files: 0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)")
    chk(only_undefined_live("SERV_IRUHADEV_X_LIVE", undef), "inert accepted")
    chk(not only_undefined_live("SERV_IRUHADEV_X_CORE", undef), "inert only for _LIVE")
    chk(not only_undefined_live("SERV_IRUHADEV_X_LIVE", undef.replace("0 FAIL, 0 ERROR", "1 FAIL, 1 ERROR")), "inert refuses a file FAIL / ERROR")
    chk(not only_undefined_live("SERV_IRUHADEV_X_LIVE", undef.replace("0 deleted", "3 deleted")), "inert refuses deleted lines")
    chk(not only_undefined_live("SERV_IRUHADEV_X_LIVE", "RESULT: NOTHING CHECKED (no changed file)\n" + undef), "inert refuses NOTHING CHECKED")
    chk(not only_undefined_live("SERV_IRUHADEV_X_LIVE", undef + "\n  FAIL: 3 deleted line(s)"), "inert refuses a second FAIL")
    fc_out = ("  OTHER  abc  X2Lib/A.cpp  +1\nfiles: 0 PASS, 1 OTHER, 0 FAIL, 0 ERROR\n  PASS  abc  X2Lib/B.cpp\n"
              "in this flag's files: 0 deleted line(s), 0 non-ASCII inserted line(s), 0 encoding change(s)")
    chk(len(flagcheck_detail(fc_out)) == 3, "detail keeps summary lines and non-PASS rows")
    chk(len(flagcheck_detail(fc_out, brief=True)) == 1, "brief keeps only the files: line when every count is zero")
    chk(len(flagcheck_detail(fc_out.replace("0 deleted", "2 deleted"), brief=True)) == 2, "brief adds the flag-files line when a count is non-zero")
    fc = os.path.join(ROOT, "tools", "flagcheck.py")
    try:
        with open(fc, "r", encoding="utf-8", errors="replace") as fh:
            t = fh.read()
        chk(re.search(r"CODE_EXT = \(%s\)" % re.escape(", ".join('"%s"' % e for e in CODE_EXT)), t) is not None, "CODE_EXT matches flagcheck")
        chk(re.search(r"CODE_DIRS = \(%s\)" % re.escape(", ".join('"%s"' % d for d in CODE_DIRS)), t) is not None, "CODE_DIRS matches flagcheck")
    except OSError:
        chk(False, "flagcheck.py readable")
    print("finish.py selftest:", "PASS" if ok else "FAIL")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
