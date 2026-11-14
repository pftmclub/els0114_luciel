"""playtest.py - per-phase play-test sections, linted, assembled into one detailed hand-off at a batch seam.

    python tools/playtest.py list     <plan>                       # sections found in kickoffs/<plan>/playtest.md
    python tools/playtest.py check    <plan> [ID ...]              # lint every section (or the named ones); exit 1 on FAIL
    python tools/playtest.py assemble <plan> [ID ...]              # check, then print the hand-off for those sections (default: all)
    python tools/playtest.py --selftest

Why: a phase writes its test while it still knows what it changed; the batch hand-off is assembled from those sections, never rewritten
later from memory. The lint exists so a section cannot be a one-line summary: the user needs to know exactly what to test.

File shape (kickoffs/<plan>/playtest.md; template kickoffs/TEMPLATE-playtest.md). One `## <phase id> - <title>` per phase (the id is the
first word, a trailing `:` is dropped; `-`, an en / em dash or `:` may precede the title), each with all six `###` parts, in any order:
    ### Dev console     full copy-ready lines (`/additem 12345 1`, the whole Lua line), or `none - <reason>`
    ### Loadout         a markdown table, one row per key (A S D C F Q W E R T), or `n/a - <reason>`
    ### Steps           numbered, concrete: where to go, what to press, what to cast
    ### Expect          what must be seen, with numbers / names / log lines
    ### If not          what a failure looks like and which skill, state or file to name
    ### Logs            the log tag(s) to read afterwards, as [TAG] or `TAG`
A heading that starts with a part name (`### Loadout page 2`) is folded into that part and printed under its own bold label, so a
second skill page is never dropped. Any other `###` heading is an unknown part and FAILs.
Rules enforced: every part present and non-empty; no `<placeholder>` left from the template; Dev console has a copy-ready command line
(a `/command`, or a call such as `obj:Method( 3 )`) or `none - <reason>`; Loadout is a table with a data row or `n/a - <reason of 10+
characters>`; Steps has 2+ numbered lines and 60+ characters; Expect 40+; If not 30+; Logs has a [TAG] or `TAG`. A file with no parsable
section FAILs. WARN (does not fail): Expect with no digit, backtick or quote names nothing observable.
"""
import os, re, sys

for _s in (sys.stdout, sys.stderr):
    try:
        _s.reconfigure(encoding="utf-8", errors="replace")
    except Exception:
        pass

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PARTS = ["dev console", "loadout", "steps", "expect", "if not", "logs"]
KEYS = "Enter|Esc|Escape|Tab|Shift|Ctrl|Alt|Space|Home|End|Up|Down|Left|Right|Delete|Insert|PageUp|PageDown|Backspace|Return|F\\d+"
PLACEHOLDER_RE = re.compile(r"<(?!(?:%s)>)[A-Za-z][^<>\n]*>" % KEYS)      # <where to go>, <itemID>; a key name such as <Enter> is real text
TAG_RE = re.compile(r"\[([A-Za-z0-9_]{3,})\]|`([^`]+)`")
LABEL_RE = re.compile(r"^\*\*(?:%s)[^*]*\*\*$" % "|".join(re.escape(p) for p in PARTS), re.I)      # the bold label of a folded '### Loadout page 2'


def path_for(plan):
    return os.path.join(ROOT, "kickoffs", plan, "playtest.md")


def known_part(heading):
    """The part a ### heading belongs to: its own name, or the part whose name it starts with ('Loadout page 2'). None if unknown."""
    h = heading.strip().lower()
    for p in PARTS:
        if h == p or h.startswith(p + " ") or h.startswith(p + ":") or h.startswith(p + "-"):
            return p
    return None


def parse(text):
    """-> list of (id, title, {part: body}) in file order. Unknown ### headings are kept under their own key (lint fails them)."""
    out, cur, part = [], None, None
    for line in text.lstrip("\ufeff").splitlines():
        m = re.match(r"^##\s+(\S+)\s*(.*)$", line)
        if m and not line.startswith("###"):
            sid = m.group(1).rstrip(":")
            title = re.sub(r"^[-:\u2013\u2014]+\s*", "", m.group(2)).strip()
            cur = (sid, title, {})
            out.append(cur)
            part = None
            continue
        m = re.match(r"^###\s+(.+?)\s*$", line)
        if m and cur is not None:
            heading = m.group(1).strip()
            part = known_part(heading) or heading.lower()
            body = cur[2].setdefault(part, [])
            if known_part(heading) and heading.lower() != part:
                body.append("**%s**" % heading)
            continue
        if cur is not None and part is not None:
            cur[2][part].append(line)
    return [(i, t, {k: "\n".join(v).strip() for k, v in d.items()}) for i, t, d in out]


def lint(sec_id, parts):
    """-> (fails, warns)."""
    f, w = [], []
    for p in parts:
        if p not in PARTS:
            f.append("unknown part: ### %s (allowed: %s)" % (p.title(), ", ".join(x.title() for x in PARTS)))
    for p in PARTS:
        if p not in parts:
            f.append("missing part: ### %s" % p.title())
        elif not parts[p] or all(LABEL_RE.match(l.strip()) for l in parts[p].splitlines() if l.strip()):
            f.append("empty part: ### %s" % p.title())
        elif PLACEHOLDER_RE.search(parts[p]):
            f.append("%s: still holds a template placeholder %s" % (p.title(), PLACEHOLDER_RE.search(parts[p]).group(0)))
    g = lambda k: parts.get(k, "")
    dc = g("dev console")
    if dc and not PLACEHOLDER_RE.search(dc):
        lines = [l.strip().strip("`").strip() for l in dc.splitlines() if l.strip()]
        ok = re.match(r"(?i)none\s*-\s*\S", dc) or any(re.match(r"^/[A-Za-z]", l) or re.match(r"^[A-Za-z_][\w:.]*\s*\(", l) for l in lines)
        if not ok:
            f.append("Dev console: give a copy-ready line (a `/command`, or a call like `obj:Method( 3 )`) or `none - <reason>`")
    lo = g("loadout")
    if lo and not PLACEHOLDER_RE.search(lo):
        rows = [l for l in lo.splitlines() if l.strip().startswith("|")]
        is_sep = lambda l: re.match(r"^\s*\|[\s:\-|]*-[\s:\-|]*$", l)
        body = [l for l in rows if not is_sep(l) and any(c.strip() for c in l.strip().strip("|").split("|"))]
        data = body[1:] if any(is_sep(l) for l in rows) else []      # a header, a separator row, then a row with some text
        na = re.match(r"(?i)n/a\s*-\s*(.{10,})", lo)
        if not (data or na):
            f.append("Loadout: a markdown table with at least one data row, or `n/a - <reason of 10+ characters>`")
    st = g("steps")
    if st and not PLACEHOLDER_RE.search(st):
        nums = [l for l in st.splitlines() if re.match(r"^\s*\d+[.)]\s+\S", l)]
        if len(nums) < 2:
            f.append("Steps: needs 2+ numbered lines (found %d)" % len(nums))
        if len(st) < 60:
            f.append("Steps: too short (%d chars, need 60+): say where, what to press, what to cast" % len(st))
    ex = g("expect")
    if ex and len(ex) < 40:
        f.append("Expect: too short (%d chars, need 40+)" % len(ex))
    if ex and not re.search(r"[0-9`\"']", ex):
        w.append("Expect names nothing observable (no number, name, log line)")
    ifn = g("if not")
    if ifn and len(ifn) < 30:
        f.append("If not: too short (%d chars, need 30+): name the failure and what to report" % len(ifn))
    lg = g("logs")
    if lg and not PLACEHOLDER_RE.search(lg) and not TAG_RE.search(lg):
        f.append("Logs: name the log tag(s) as [TAG] or `TAG`")
    return f, w


def load(plan):
    p = path_for(plan)
    if not os.path.exists(p):
        print("playtest.py: no %s" % os.path.relpath(p, ROOT), file=sys.stderr)
        sys.exit(2)
    with open(p, "r", encoding="utf-8-sig") as fh:
        return parse(fh.read())


def pick(secs, ids):
    if not ids:
        return secs
    have = {s[0] for s in secs}
    miss = [i for i in ids if i not in have]
    if miss:
        print("playtest.py: no section for: %s" % ", ".join(miss), file=sys.stderr)
        sys.exit(2)
    return [s for s in secs if s[0] in ids]


def tags_of(logs):
    return sorted({a or b for a, b in TAG_RE.findall(logs)})


def do_check(secs):
    if not secs:
        print("PLAYTEST RESULT: FAIL (no section found: each needs a line `## <phase id> - <title>`)")
        return 1
    bad = 0
    for sid, title, parts in secs:
        f, w = lint(sid, parts)
        print("  %-10s %-5s %s" % (sid, "FAIL" if f else "PASS", title))
        for x in f:
            print("      FAIL: " + x)
        for x in w:
            print("      WARN: " + x)
        bad += bool(f)
    print("PLAYTEST RESULT: %s (%d sections, %d failed)" % ("FAIL" if bad else "PASS", len(secs), bad))
    return 1 if bad else 0


GOOD = (
    "## CH1a3 - gage UI\n### Dev console\n`/additem 123456 1`\n### Loadout\n| key | skill |\n|---|---|\n| A | Strike |\n"
    "### Steps\n1. Create the Lu/Ciel unit and enter a dungeon.\n2. Take damage until the MP bar moves, then cast `Strike` twice.\n"
    "### Expect\nThe MP bar shows 10 combination points and the portrait swaps on the minor form.\n"
    "### If not\nA missing bar or a crash on entering the dungeon: report the last [LUCIEL_GAGE] line.\n### Logs\n[LUCIEL_GAGE]\n"
)


def selftest():
    ok = True

    def chk(cond, what):
        nonlocal ok
        if not cond:
            print("  selftest FAIL: " + what)
            ok = False

    L = lambda s: lint(*parse(s)[0][::2])[0]
    secs = parse(GOOD)
    chk(len(secs) == 1 and secs[0][0] == "CH1a3" and L(GOOD) == [], "good section passes")
    f, _ = lint("X", parse("## X - t\n### Steps\nTest it.\n### Expect\nWorks.\n")[0][2])
    chk(len(f) >= 5, "vague section fails")
    chk(L(GOOD.replace("| key | skill |\n|---|---|\n| A | Strike |", "n/a - no skill slots in this phase")) == [], "n/a loadout ok")
    chk(any("Loadout" in x for x in L(GOOD.replace("| A | Strike |\n", ""))), "table without data row fails")
    chk(any("Loadout" in x for x in L(GOOD.replace("|---|---|\n| A | Strike |", "|---|---\n"))), "separator without trailing pipe is not a data row")
    chk(L(GOOD.replace("`/additem 123456 1`", "`g_pX2Game:GetMyUnit():SetHyperModeCount( 3 )`")) == [], "Lua call line accepted as Dev console")
    chk(any("Dev console" in x for x in L(GOOD.replace("`/additem 123456 1`", "Lua as usual"))), "vague Dev console fails")
    chk(any("placeholder" in x for x in L(GOOD.replace("`/additem 123456 1`", "`/additem <itemID> [qty]`"))), "placeholder fails")
    two = GOOD.replace("### Steps", "### Loadout page 2\n| key | skill |\n|---|---|\n| S | Dash |\n### Steps")
    s2 = parse(two)[0][2]
    chk(L(two) == [] and "Dash" in s2["loadout"] and "**Loadout page 2**" in s2["loadout"], "Loadout page 2 folded into Loadout")
    chk(any("unknown part" in x for x in L(GOOD.replace("### Logs", "### Note\nhi\n### Logs"))), "unknown ### part fails")
    chk(any("Dev console" in x for x in L(GOOD.replace("`/additem 123456 1`", "Open the dev console (END key) and run the usual commands"))), "prose with a parenthesis is not a command")
    chk(any("Dev console" in x for x in L(GOOD.replace("`/additem 123456 1`", "None needed (no console)"))), "'None needed (...)' is not 'none - reason'")
    chk(any("Loadout" in x for x in L(GOOD.replace("| A | Strike |", "| | |"))), "empty table row is not a data row")
    chk(any("Loadout" in x for x in L(GOOD.replace("|---|---|\n", "| key | skill |\n"))), "two header rows (no separator) fail")
    chk(L(GOOD.replace("1. Create", "1. Press <Enter>, create")) == [], "<Enter> is a key, not a placeholder")
    chk(L(GOOD.replace("[LUCIEL_GAGE]\n", "**[LUCIEL_GAGE]**\n")) == [], "bold tag is a tag")
    chk(L(GOOD.replace("[LUCIEL_GAGE]\n", "`JUNE_FX gage`\n")) == [], "a backticked grep form with a space is a tag")
    chk(any("Logs" in x for x in L(GOOD.replace("[LUCIEL_GAGE]\n", "TBD\n"))), "TBD logs fails")
    chk(tags_of("[LUCIEL_GAGE], NOT the packet log") == ["LUCIEL_GAGE"], "tags only from [TAG] / `TAG`")
    em = GOOD.replace("## CH1a3 - gage UI", "## CH1a3 \u2014 gage UI")
    chk(parse(em)[0][0] == "CH1a3" and parse(em)[0][1] == "gage UI", "em dash heading")
    chk(parse(GOOD.replace("## CH1a3 - gage UI", "## CH1a3: gage UI"))[0][0] == "CH1a3", "colon after id dropped")
    chk(parse(GOOD.replace("## CH1a3 - gage UI", "## CH1a3 gage UI"))[0][0] == "CH1a3", "heading without dash still a section")
    chk(parse("\ufeff" + GOOD)[0][0] == "CH1a3", "BOM tolerated")
    chk(parse(GOOD.replace("\n", "\r\n"))[0][0] == "CH1a3" and L(GOOD.replace("\n", "\r\n")) == [], "CRLF input")
    import io, contextlib
    with contextlib.redirect_stdout(io.StringIO()):
        none_rc = do_check([])
    chk(none_rc == 1, "no sections fails")
    print("playtest.py selftest:", "PASS" if ok else "FAIL")
    return 0 if ok else 1


def main(argv):
    if argv and argv[0] == "--selftest":
        return selftest()
    if len(argv) < 2 or argv[0] not in ("list", "check", "assemble"):
        print(__doc__)
        return 2
    cmd, plan, ids = argv[0], argv[1], argv[2:]
    secs = load(plan)
    if cmd == "list":
        for sid, title, parts in secs:
            print("  %-10s %s" % (sid, title))
        print("%d section(s)" % len(secs))
        return 0 if secs else 1
    everything, secs = secs, pick(secs, ids)
    if ids:
        broken = [s[0] for s in everything if s not in secs and lint(s[0], s[2])[0]]
        if broken:
            print("WARN: other section(s) fail the lint and are outside this run: %s (a stray `## ` line inside a part starts a new section)" % ", ".join(broken))
    rc = do_check(secs)
    if cmd == "check" or rc:
        return rc
    print("\n" + "=" * 72)
    print("PLAY-TEST HAND-OFF  %s  (%s)" % (plan, ", ".join(s[0] for s in secs)))
    print("=" * 72)
    tags = []
    for sid, title, parts in secs:
        print("\n## %s - %s" % (sid, title))
        for p in PARTS:
            print("\n### %s\n%s" % (p.title(), parts[p]))
        tags += tags_of(parts["logs"])
    print("\nWhen done say 'check logs'. Tags to read: %s" % ", ".join(sorted(set(tags))))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
