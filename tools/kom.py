#!/usr/bin/env python3
"""Pack, compile and deploy March .kom archives so nobody types the elstools commands by hand.

    python tools/kom.py pack data036            # kom_contents/data036_kom -> kom_contents/data036_packed.kom
    python tools/kom.py pack 36 5 13            # several at once; data036 / 036 / 36 / data036_kom all work
    python tools/kom.py compile data036         # FOLD X.src.lua into BOM-source X.lua (T0 or newer; see tools/luasrc.py)
    python tools/kom.py compile data036 --bytecode   # old behaviour, rollback only: stale X.src.lua -> mar-2014 bytecode
    python tools/kom.py copy data036            # kom_contents/data036_packed.kom -> <game dir>/data036.kom
    python tools/kom.py status                  # recently touched archives that need a pack / a copy
    python tools/kom.py status data036          # one archive in detail (entry diff vs the live kom)
    python tools/kom.py stage-june <name|list-file> [--dry-run]   # June's exact file -> kom_contents/dataNNN_kom/ + manifest
    python tools/kom.py stage-server X.lua [Y.lua ...] [--kom data036]   # KncWX2Server/ServerResource/US/X.lua -> luajit bytecode in the kom dir

pack  : runs `elstools pack -i <dir> -o <packed> -v mar-2014`. NOTE (measured 2026-10-06): the pack is
        DIRECTORY-driven, not manifest-driven - it packs every file in the dir except *.src.lua and
        _manifest.json, and the manifest only supplies each file's algo; a file the manifest does not
        list is packed anyway with algo 2 (wrong for non-lua: the rule is lua 2, everything else 0).
        So before it runs it REFUSES if any such unlisted file exists (--allow-unlisted overrides),
        warns about manifest entries with no file (they are silently left out), and about stale
        X.src.lua (newer than X.lua). After it runs it re-reads the result with `elstools inspect`,
        checks entry set + per-entry algo against dir + manifest, then replaces the old packed kom
        atomically. Prints the entry diff against the LIVE kom.
        LUA GATE (Rule 5, LUA_SOURCE_PHASES.md; tools/luasrc.py): every .lua (not .src.lua) must be March bytecode
        (1B 4B 4C 81) or BOM + text that parses under the vendored luajit, or a ledger exception (tools/data/
        lua_source_ledger.tsv, tier EXC). It REFUSES (exit 4, nothing written): BOM-less text, June 0x82 bytecode,
        any other header, a parse error - each named with the parser's message. Manifest "0" .lua (user-packed
        server Lua) only WARNS, never refuses.
compile: folds every X.src.lua into X.lua = BOM + the source text (and removes the sidecar after snapshotting the
        old bytecode to kom_contents_bytecode/). Folds when the sidecar compiles byte-identical to the current
        X.lua (T0) or is NEWER than it (an intended edit, ledger tier EDIT, no equivalence claimed); REFUSES an
        older sidecar that is not T0 (a stale decompile beside a converted file) or one that does not compile.
        Per file, never elstools compile-dir (it overwrites every X.lua that has an X.src.lua beside it).
        --bytecode keeps the old elstools bytecode compile for rollback.
copy  : refuses while X2_offline.exe runs, if the kom_contents dir is newer than the packed kom, or
        if the packed kom DROPS entries the live kom has (--allow-removed overrides). Backs the
        live kom up to <kom_contents>/../kom_backup_tools/ (newest 5 kept per archive), copies via
        a .part file + atomic replace, verifies size+md5 and the entry count, prints `COPY OK`.

stage-june: for each name (or each line / `name` column of a list file) finds June's file with tools/junekom.py's strict
        decoder (June's own algorithms; refuses anything that does not decode to the manifest size, or whose June
        copies in different koms differ), picks the kom (March's number if a live kom or a staged dir already has
        the name, else June's), copies the exact bytes into kom_contents/dataNNN_kom/ and appends the manifest key
        (.lua/.txt/.kim "2", everything else "0"; a key already listed keeps its value; the file's own line ending
        is kept, a new manifest is CRLF with 2-space indent and no trailing newline). REFUSES a held name (tools/data/
        asset_port_held.tsv), a name March has in several koms, a name not in June (names over 59 characters stage since X4's
        SERV_IRUHADEV_KOM_LONG_NAMES shipped, proven in the deployed exe 2026-11-16; --long-names is a no-op). A replaced
        staged file is backed up to <kom_contents>/../kom_backup_stage_june/<stamp>/. --dry-run writes nothing;
        --plan-out F writes the per-file plan as TSV. Never writes into June's tree or the game dir.

june-asset-port gates (plan JUNE_ASSET_PORT_PHASES.md Rule 4; phase P0, 2026-11-16). A `--wave` / list file is a name list (one name per
        line, or a TSV with a `name` column). A name in tools/data/asset_port_held.tsv (Rule 8) is never staged or verified.
    verify-staged <list|kom|name> ...   gate (d): every staged file equals June's strict decode (every June copy decodes and they
        agree), sits in exactly one kom dir and has a manifest key. A kom argument checks every staged file June also has (.lua skipped:
        core-list sources differ from June by design; March-only files are counted, not failed). Exit 4 on any problem.
    pack <kom ...> --wave <list>        the normal pack, then gates (a) and (b) against the live kom: no entry removed; an entry added
        must be on the list; a `~changed` entry off the list is decoded from the live kom and byte-compared with the staged file
        (equal = recompression only, passes). Anything else exits 4 and names the entry.
    copy <kom ...> --wave <list>        the same gates (a)/(b), gate (c) `dupcheck` with this kom swapped in, then the normal copy.
    dupcheck [--with dataNNN ...]       gate (c): a name in two koms of the would-be-live set (every live kom, the `--with` koms' packed
        archives swapped in; default: every kom_contents packed kom that differs from live) deletes a whole kom at mount
        (NOT_ALLOW_DUPLICATE_FILES). Lists new and pre-existing duplicates; exit 4 if any.
    copy-loose <list|--all> --dest music|movie|eventscene [--dry-run]   June's loose file -> <game dir>/<dest>/: identical files are
        skipped, a replaced file is backed up to kom_backup_tools/loose/<stamp>/<dest>/, written as .part then replaced, md5-verified.
        Refuses while the game runs (exit 3).

stage-server (user, 2026-11-15: server-only Lua is packed by Claude, no longer by hand): compiles the BOM source
        KncWX2Server/ServerResource/US/X.lua with luajit (stripped, like the shipped files; the manifest key must already be "2"),
        backs the staged file up to <kom_contents>/../kom_backup_tools/X.lua.<stamp>.bytecode, writes the bytecode into the kom dir.
        Then `pack` and `copy` as for any kom. A repo source that does not compile is refused; nothing is written.

Exit codes: 0 ok, 1 failed, 2 usage, 3 game running, 4 refused (stale pack / dropped entries /
manifest mismatch).  Paths can be overridden with X2_GAME_DIR, X2_KOM_CONTENTS.
Never launches the game.
"""
import argparse, datetime, hashlib, json, os, re, shutil, subprocess, sys, time

for _s in (sys.stdout, sys.stderr):  # the Windows console defaults to cp1252
    try:
        _s.reconfigure(encoding="utf-8", errors="replace")
    except Exception:
        pass

TRUNK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ELSTOOLS = os.path.join(TRUNK, "elstools", "elstools.exe")
VERSION = "mar-2014"
GAME_DIR = os.environ.get("X2_GAME_DIR", r"F:\elsword stuff\elsword_2014\els_2014\237311\24965799\data")
KOM_ROOT = os.environ.get("X2_KOM_CONTENTS", r"F:\elsword stuff\elsword_2014\els_2014\237311\24965799\kom_contents")
BACKUP_ROOT = os.path.join(os.path.dirname(KOM_ROOT), "kom_backup_tools")
GAME_EXE = "X2_offline.exe"
KEEP_BACKUPS = 5
SHOW = 15  # max lines of any list


def p(*a):
    print(*a, flush=True)


def die(code, *a):
    p(*a)
    sys.exit(code)


def norm(arg):
    """'data036' / '036' / '36' / 'data036_kom' / 'data036.kom' / 'datai001' -> ('data036', '036')."""
    m = re.fullmatch(r"(?i)(?:data)?(i?)0*(\d+)(?:_kom|_packed|\.kom|_packed\.kom)?", arg.strip())
    if not m:
        die(2, "cannot read %r as an archive name (use data036, 036 or datai001)" % arg)
    num = m.group(2).zfill(3)
    return "data%s%s" % (m.group(1).lower(), num)


def paths(name):
    return {
        "dir": os.path.join(KOM_ROOT, name + "_kom"),
        "packed": os.path.join(KOM_ROOT, name + "_packed.kom"),
        "live": os.path.join(GAME_DIR, name + ".kom"),
    }


def md5(path):
    h = hashlib.md5()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def fmt_time(t):
    return datetime.datetime.fromtimestamp(t).strftime("%Y-%m-%d %H:%M:%S")


def running(image):
    """PIDs of processes with this image name (query only, never kill)."""
    try:
        out = subprocess.run(["tasklist", "/FI", "IMAGENAME eq %s" % image, "/FO", "CSV", "/NH"],
                             capture_output=True, text=True, errors="replace").stdout
    except OSError:
        return []
    pids = []
    for line in out.splitlines():
        cols = [c.strip('"') for c in line.split('","')]
        if len(cols) > 1 and cols[0].lower() == image.lower():
            try:
                pids.append(int(cols[1]))
            except ValueError:
                pass
    return pids


def elstools(args, timeout=1800):
    """Run elstools with stdin closed (a bash/while-read parent once starved it). Returns (rc, stdout)."""
    if not os.path.isfile(ELSTOOLS):
        die(1, "elstools.exe not found at %s" % ELSTOOLS)
    r = subprocess.run([ELSTOOLS] + args, stdin=subprocess.DEVNULL, capture_output=True, text=True,
                       encoding="utf-8", errors="replace", timeout=timeout)
    return r.returncode, r.stdout


def last_result(stdout):
    res = None
    for line in stdout.splitlines():
        line = line.strip()
        if line.startswith("{") and '"type":"result"' in line.replace(" ", ""):
            try:
                res = json.loads(line)
            except ValueError:
                pass
    return res


def inspect(kom):
    rc, out = elstools(["inspect", "-q", "-i", kom])
    try:
        j = json.loads(out)
    except ValueError:
        j = None
    if rc != 0 or not j or "files" not in j:
        die(1, "elstools inspect failed on %s (rc=%s): %s" % (kom, rc, out[-300:]))
    return j


def load_manifest(d):
    mp = os.path.join(d, "_manifest.json")
    if not os.path.isfile(mp):
        die(4, "REFUSED: %s has no _manifest.json (pack is manifest-driven; it would pack nothing)." % d)
    with open(mp, "r", encoding="utf-8-sig") as f:
        return json.load(f)


def content_files(d):
    """Relative names of every file in the kom dir, forward slashes."""
    out = {}
    for root, _dirs, files in os.walk(d):
        for fn in files:
            full = os.path.join(root, fn)
            out[os.path.relpath(full, d).replace(os.sep, "/")] = full
    return out


def newest_content(d):
    """(mtime, name) of the newest file that can change the packed output (not X.src.lua)."""
    best = (0.0, "")
    for rel, full in content_files(d).items():
        if rel.lower().endswith(".src.lua"):
            continue
        t = os.path.getmtime(full)
        if t > best[0]:
            best = (t, rel)
    return best


def stale_sources(d):
    """X.src.lua whose X.lua is missing or older than it (needs compiling)."""
    stale = []
    for rel, full in content_files(d).items():
        if not rel.lower().endswith(".src.lua"):
            continue
        lua = full[:-len(".src.lua")] + ".lua"
        if not os.path.isfile(lua) or os.path.getmtime(lua) < os.path.getmtime(full):
            stale.append(full)
    return sorted(stale)


def lst(items, label, limit=SHOW):
    for it in items[:limit]:
        p("    %s %s" % (label, it))
    if len(items) > limit:
        p("    ... and %d more" % (len(items) - limit))


def entry_map(j):
    return {e["name"]: (e["size"], e["comp"], str(e["algo"])) for e in j["files"]}


def diff(old, new):
    """old/new: entry maps. Returns (added, removed, changed) name lists."""
    added = sorted(set(new) - set(old))
    removed = sorted(set(old) - set(new))
    changed = sorted(n for n in set(old) & set(new) if old[n][:2] != new[n][:2] or old[n][2] != new[n][2])
    return added, removed, changed


def report_diff(label, added, removed, changed):
    p("  vs %s: +%d added, -%d removed, ~%d changed" % (label, len(added), len(removed), len(changed)))
    lst(added, "+")
    lst(removed, "-")
    lst(changed, "~")


# ----------------------------------------------------------------------------------------------
def cmd_pack(name, allow_unlisted=False, wave=None):
    pa = paths(name)
    d = pa["dir"]
    if not os.path.isdir(d):
        die(2, "no such kom dir: %s" % d)
    man = load_manifest(d)
    files = content_files(d)
    p("== pack %s  (%d manifest entries, %d files in dir)" % (name, len(man), len(files)))

    packable = sorted(n for n in files if n != "_manifest.json" and not n.lower().endswith(".src.lua"))
    missing = sorted(n for n in man if n not in files)
    if missing:
        p("  WARN %d manifest entr%s no file in the dir, so %s NOT packed:" % (len(missing), "y has" if len(missing) == 1 else "ies have", "it is" if len(missing) == 1 else "they are"))
        lst(missing, "!")
    unlisted = [n for n in packable if n not in man]
    if unlisted:
        p("%s %d file(s) are in the dir but NOT in _manifest.json. elstools packs them anyway with algo 2 (rule: lua 2, non-lua 0):" %
          ("  WARN" if allow_unlisted else "REFUSED:", len(unlisted)))
        lst(unlisted, "?")
        if not allow_unlisted:
            die(4, "Add them to _manifest.json (\"name\": \"2\" for .lua, \"0\" otherwise) or move them out; nothing was written. (--allow-unlisted to pack anyway)")
    stale = stale_sources(d)
    if stale:
        p("  WARN %d X.src.lua newer than X.lua (edited after the last compile) - run `kom.py compile %s` first if those edits should ship:" % (len(stale), name))
        lst([os.path.relpath(s, d) for s in stale], "!")

    refusals, warnings, notes = _luasrc().gate(d, name, man)
    for n in notes:
        p("  note: %s" % n)
    if warnings:
        p("  WARN %d lua gate warning(s):" % len(warnings))
        lst(warnings, "!")
    if refusals:
        p("REFUSED: %d .lua file(s) fail the Lua gate (Rule 5); nothing was packed:" % len(refusals))
        lst(refusals, "x", limit=40)
        die(4, "Fix each file (lead BOM-less text with EF BB BF, fix the parse error, run a June 0x82 file through ljconv / adopt-june). "
               "Only a ledger exception row (tools/data/lua_source_ledger.tsv, tier EXC) lets any other .lua through.")

    part = pa["packed"] + ".part.kom"
    if os.path.exists(part):
        os.remove(part)
    t0 = time.time()
    rc, out = elstools(["pack", "-q", "-i", d, "-o", part, "-v", VERSION])
    res = last_result(out)
    if rc != 0 or not res or res.get("failed") or res.get("skipped") or res.get("ok") != len(packable):
        if os.path.exists(part):
            os.remove(part)
        die(1, "PACK FAILED rc=%s result=%s\n%s" % (rc, res, out[-600:]))

    j = inspect(part)
    new = entry_map(j)
    bad = []
    if set(new) != set(packable):
        bad.append("entry set differs from the dir: %s" % sorted(set(new) ^ set(packable))[:SHOW])
    for n, algo in man.items():
        if n in new and new[n][2] != str(algo):
            bad.append("%s: packed algo %s, manifest says %s" % (n, new[n][2], algo))
    if bad:
        os.remove(part)
        p("PACK VERIFY FAILED (old packed kom left untouched):")
        lst(bad, "!")
        sys.exit(1)

    os.replace(part, pa["packed"])
    size = os.path.getsize(pa["packed"])
    p("  packed: %s" % pa["packed"])
    p("  %d entries, %d bytes, algos %s, %.1fs" % (j["file_count"], size, j["algos"], time.time() - t0))
    if os.path.isfile(pa["live"]):
        a, r, c = diff(entry_map(inspect(pa["live"])), new)
        report_diff("live %s.kom" % name, a, r, c)
        if r:
            p("  NOTE: the packed kom drops %d live entr%s; `copy` will refuse without --allow-removed." % (len(r), "y" if len(r) == 1 else "ies"))
    else:
        p("  (no live %s.kom in the game dir - this would be a new archive)" % name)
    p("PACK OK %s: %s" % (name, pa["packed"]))
    if wave is not None and not wave_gate(name, wave):
        die(4, "WAVE GATE FAILED %s: the packed kom was written, but `copy --wave` will refuse it. Fix the entries listed above." % name)
    if wave is not None:
        p("WAVE GATE OK %s" % name)


def _luasrc():
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import luasrc
    return luasrc


def cmd_compile(name, bytecode=False):
    d = paths(name)["dir"]
    if not os.path.isdir(d):
        die(2, "no such kom dir: %s" % d)
    if not bytecode:
        ls = _luasrc()
        folded, refused, skipped = ls.fold(d, name, phase="compile")
        p("== compile %s: fold X.src.lua into BOM-source X.lua" % name)
        for n, how in folded:
            p("    folded %s  [%s]" % (n, how))
        for n in skipped:
            p("    already folded (text equal): %s" % n)
        for n, why in refused:
            p("    REFUSED %s: %s" % (n, why))
        if refused:
            die(4, "COMPILE REFUSED %s: %d folded, %d refused (see above)." % (name, len(folded), len(refused)))
        if not folded and not skipped:
            p("nothing to compile (no X.src.lua in the dir).")
            return
        p("COMPILE OK %s: %d folded (BOM source), %d already folded. Old bytecode is in kom_contents_bytecode/." % (name, len(folded), len(skipped)))
        if folded:
            p("Next: kom.py pack %s" % name)
        return
    stale = stale_sources(d)
    p("== compile %s: %d stale X.src.lua" % (name, len(stale)))
    if not stale:
        p("nothing to compile (every X.lua is newer than its X.src.lua).")
        return
    args = ["compile-batch", "-q", "-v", VERSION]
    for s in stale:
        args += ["-i", os.path.abspath(s)]
    rc, out = elstools(args)
    res = last_result(out)
    lst([os.path.relpath(s, d) for s in stale], ">")
    if rc != 0 or not res or res.get("failed"):
        die(1, "COMPILE FAILED rc=%s result=%s\n%s" % (rc, res, out[-800:]))
    p("COMPILE OK %s: %s" % (name, {k: v for k, v in res.items() if k in ("ok", "failed", "skipped")}))
    p("Next: kom.py pack %s" % name)


def cmd_copy(name, allow_removed, force, wave=None):
    pa = paths(name)
    if not os.path.isfile(pa["packed"]):
        die(4, "REFUSED: %s does not exist - run `kom.py pack %s` first." % (pa["packed"], name))
    pids = running(GAME_EXE)
    if pids:
        die(3, "COPY REFUSED: %s is running (pid %s). Ask the user to close the game, then rerun." % (GAME_EXE, pids))
    if not os.path.isdir(GAME_DIR):
        die(1, "game dir not found: %s" % GAME_DIR)
    p("== copy %s" % name)
    if os.path.isdir(pa["dir"]):
        t, who = newest_content(pa["dir"])
        if t > os.path.getmtime(pa["packed"]) and not force:
            die(4, "COPY REFUSED: %s is newer than the packed kom (%s > %s). Run `kom.py pack %s` first (or --force)." %
                (who, fmt_time(t), fmt_time(os.path.getmtime(pa["packed"])), name))
    live_exists = os.path.isfile(pa["live"])
    if live_exists and os.path.getsize(pa["live"]) == os.path.getsize(pa["packed"]) and md5(pa["live"]) == md5(pa["packed"]):
        die(0, "COPY OK %s: already identical to the live kom (md5 match), nothing copied." % name)

    if wave is not None:   # gates (a)/(b) and (c), right before the copy (peers stage too)
        if not wave_gate(name, wave):
            die(4, "COPY REFUSED %s: wave gate failed (list above); nothing was copied." % name)
        new_d, old_d, inner_d = run_dupcheck([name], quiet=True)
        if new_d or old_d or inner_d:
            run_dupcheck([name])
            die(4, "COPY REFUSED %s: dupcheck found duplicate names (list above); nothing was copied." % name)
        p("  dupcheck OK with %s swapped in" % name)

    new_j = inspect(pa["packed"])
    if live_exists:
        a, r, c = diff(entry_map(inspect(pa["live"])), entry_map(new_j))
        report_diff("live %s.kom" % name, a, r, c)
        if r and not allow_removed:
            die(4, "COPY REFUSED: the packed kom drops %d entr%s the live kom has (list above). The kom dir is probably out of step with the live archive; "
                   "rerun with --allow-removed only if the user wants them gone." % (len(r), "y" if len(r) == 1 else "ies"))
        os.makedirs(BACKUP_ROOT, exist_ok=True)
        bak = os.path.join(BACKUP_ROOT, "%s.kom.%s" % (name, datetime.datetime.now().strftime("%Y%m%d_%H%M%S")))
        shutil.copy2(pa["live"], bak)
        old = sorted(f for f in os.listdir(BACKUP_ROOT) if f.startswith(name + ".kom."))
        for f in old[:-KEEP_BACKUPS]:
            os.remove(os.path.join(BACKUP_ROOT, f))
        p("  backup: %s" % bak)
    else:
        p("  NOTE: no live %s.kom existed - creating a new archive; confirm the client actually mounts it." % name)

    part = pa["live"] + ".part"
    try:
        shutil.copy2(pa["packed"], part)
        os.replace(part, pa["live"])  # new inode: also detaches any hard link the old file had
    except OSError as e:
        if os.path.exists(part):
            try:
                os.remove(part)
            except OSError:
                pass
        die(1, "COPY FAILED: %s (game running, a sharing lock, or Defender; the old live kom is untouched or restorable from %s)" % (e, BACKUP_ROOT))

    ok = os.path.getsize(pa["live"]) == os.path.getsize(pa["packed"]) and md5(pa["live"]) == md5(pa["packed"])
    n = inspect(pa["live"])["file_count"]
    if not ok or n != new_j["file_count"]:
        die(1, "COPY VERIFY FAILED: size/md5 match=%s entries live=%s packed=%s" % (ok, n, new_j["file_count"]))
    p("COPY OK %s: %s  %d bytes, %d entries, md5 %s" % (name, pa["live"], os.path.getsize(pa["live"]), n, md5(pa["live"])))


# ----------------------------------------------------------------------------------------------
# stage-june: June's exact file -> March's kom_contents/dataNNN_kom/ + its _manifest.json value
LONG_NAME = 59          # March's on-disk name field; longer names load under SERV_IRUHADEV_KOM_LONG_NAMES (X4: closed, defined, in the deployed exe)
STAGE_BACKUP_ROOT = os.path.join(os.path.dirname(KOM_ROOT), "kom_backup_stage_june")
LIVE_CACHE = os.path.join(TRUNK, ".build", "stage_june_live_index.json")
if "X2_GAME_DIR" in os.environ:   # a scratch game dir gets its own index, so tests never churn the real one
    LIVE_CACHE = os.path.join(TRUNK, ".build", "stage_june_live_index_%s.json" % hashlib.md5(GAME_DIR.lower().encode()).hexdigest()[:8])
HELD_TSV = os.path.join(TRUNK, "tools", "data", "asset_port_held.tsv")
ALGO2_EXT = (".lua", ".txt", ".kim")   # measured in live data036 (.txt all 2, item.kim 2); everything else 0


def manifest_value(name):
    return "2" if name.lower().endswith(ALGO2_EXT) else "0"


def _junekom():
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import junekom
    return junekom


def _stage_task(task):
    """Worker: strict-decode every June copy of one name. Returns (key, [(stem, err, sha1, size)], data|None)."""
    key, cands, want_bytes = task
    jk = _junekom()
    rows, first = [], None
    for path, stem, v04, e in cands:
        with open(path, "rb") as f:
            f.seek(e["offset"])
            raw = f.read(e["csize"])
        out, err = jk.decode(e, raw, v04)
        if out is None:
            rows.append((stem, err, "", 0))
            continue
        rows.append((stem, "", hashlib.sha1(out).hexdigest(), len(out)))
        if first is None and want_bytes:
            first = out
    return key, rows, first if len({r[2] for r in rows}) == 1 and all(not r[1] for r in rows) else None


def live_names():
    """lower(name) -> {kom stem: name as March spells it}, from `elstools inspect` of every live kom (cached by size+mtime)."""
    cache = {}
    try:
        with open(LIVE_CACHE, "r", encoding="utf-8") as f:
            cache = json.load(f)
    except (OSError, ValueError):
        pass
    out, fresh = {}, {}
    for fn in sorted(os.listdir(GAME_DIR)):
        if not re.fullmatch(r"(?i)datai?\d{3}\.kom", fn):
            continue
        full = os.path.join(GAME_DIR, fn)
        st = os.stat(full)
        sig = [st.st_size, st.st_mtime_ns]
        ent = cache.get(fn)
        if ent and ent["sig"] == sig:
            names = ent["names"]
        else:
            names = [e["name"] for e in inspect(full)["files"]]
        fresh[fn] = {"sig": sig, "names": names}
        stem = fn[:-4].lower()
        for n in names:
            out.setdefault(n.lower(), {})[stem] = n
    os.makedirs(os.path.dirname(LIVE_CACHE), exist_ok=True)
    with open(LIVE_CACHE, "w", encoding="utf-8") as f:
        json.dump(fresh, f)
    return out


def staged_names():
    """lower(name) -> {kom stem: spelling on disk}, from every kom_contents/dataNNN_kom/ (top-level files only)."""
    out = {}
    for d in sorted(os.listdir(KOM_ROOT)):
        m = re.fullmatch(r"(?i)(datai?\d{3})_kom", d)
        full = os.path.join(KOM_ROOT, d)
        if not m or not os.path.isdir(full):
            continue
        for e in os.scandir(full):
            if e.is_file() and e.name != "_manifest.json" and not e.name.lower().endswith(".src.lua"):
                out.setdefault(e.name.lower(), {})[m.group(1).lower()] = e.name
    return out


def load_held():
    """lower(name) -> reason, from tools/data/asset_port_held.tsv (june-asset-port Rule 8: a held name is never staged or verified)."""
    held = {}
    if not os.path.isfile(HELD_TSV):
        return held
    with open(HELD_TSV, "r", encoding="utf-8-sig") as f:
        rows = [l.rstrip("\r\n").split("\t") for l in f if l.strip()]
    for r in rows[1:]:
        if r and r[0].strip():
            held[r[0].strip().lower()] = r[1].strip() if len(r) > 1 else ""
    return held


def june_index(only_koms=None):
    """lower(name) -> [(kom path, stem, v04, entry)] over every June kom (or only these stems), and the kom path list."""
    jk = _junekom()
    koms, _skipped = jk.all_koms()
    june = {}
    for kp in koms:
        k = jk.Kom(kp)
        if only_koms and k.stem.lower() not in only_koms:
            continue
        for e in k.entries:
            june.setdefault(e["name"].lower(), []).append((kp, k.stem, k.v04, e))
    return june, koms


def read_names(arg):
    """A name, or a list file: one name per line, or a TSV whose header has a `name` column. '#' lines are comments."""
    if not os.path.isfile(arg):
        return [arg]
    with open(arg, "r", encoding="utf-8-sig") as f:
        lines = [l.rstrip("\r\n") for l in f if l.strip() and not l.lstrip().startswith("#")]
    if lines and "\t" in lines[0]:
        head = [h.strip().lower() for h in lines[0].split("\t")]
        if "name" in head:
            i = head.index("name")
            return [l.split("\t")[i].strip() for l in lines[1:] if len(l.split("\t")) > i and l.split("\t")[i].strip()]
        die(2, "list file %s is a TSV with no `name` column (header: %s)" % (arg, head))
    return [l.strip() for l in lines]


def add_manifest_keys(d, items):
    """Append {name: value} to <d>/_manifest.json by text, in the file's own line ending. New file: CRLF house format.
    Existing keys are left as they are. Returns the number of keys added."""
    mp = os.path.join(d, "_manifest.json")
    have = {}
    if os.path.isfile(mp):
        with open(mp, "rb") as f:
            raw = f.read()
        have = {k.lower() for k in json.loads(raw.decode("utf-8-sig"))}
    else:
        raw = None
    items = [(n, v) for n, v in items if n.lower() not in have]
    if not items:
        return 0
    rows = ['  %s: %s' % (json.dumps(n, ensure_ascii=False), json.dumps(v)) for n, v in items]
    if raw is None:
        text = "{\r\n" + ",\r\n".join(rows) + "\r\n}"
    else:
        eol = "\r\n" if b"\r\n" in raw else "\n"
        s = raw.decode("utf-8")
        end = s.rstrip().rfind("}")
        if end < 0:
            die(1, "%s has no closing brace; not touching it" % mp)
        body = s[:end].rstrip()
        sep = "" if body.endswith("{") else ","
        text = body + sep + eol + (","+eol).join(rows) + eol + s[end:]
    tmp = mp + ".part"
    with open(tmp, "wb") as f:
        f.write(text.encode("utf-8"))
    os.replace(tmp, mp)
    return len(items)


def cmd_stage_june(args, dry, long_names, jobs, plan_out, transform=None, only_koms=None, move_sidecars=False, exit_on_refuse=True,
                   transform_jobs=1):
    """transform(name, june_bytes, stem) -> (bytes or None, note): applied to the strict-decoded June bytes before they are planned
    and staged (adopt-june: ljconv, or decompile + T1); None refuses the name with `note`. only_koms: restrict June's candidates to
    these koms (stems). move_sidecars: a staged X.src.lua beside a replaced X.lua goes to the backup dir (compile would fold it back
    over the new file). Returns {plan, refused, staged, by_kom, backup}; exits 4 on any refusal unless exit_on_refuse is False."""
    jk = _junekom()
    names, seen = [], set()
    for a in args:
        for n in read_names(a):
            if n.lower() not in seen:
                seen.add(n.lower())
                names.append(n)
    p("== stage-june%s: %d name(s)" % (" (DRY RUN, nothing is written)" if dry else "", len(names)))
    t0 = time.time()
    june, koms = june_index(only_koms)
    held = load_held()
    live, staged = live_names(), staged_names()
    p("  index: %d June names in %d koms, %d live March names, %d staged names (%.0fs)" % (
        len(june), len(koms), len(live), len(staged), time.time() - t0))

    plan, refused = {}, []   # key -> dict(name, stem, src, cands, dst_name)
    for n in names:
        key = n.lower()
        cands = june.get(key)
        if not cands:
            refused.append((n, "", "not in any June kom" if not only_koms else "not in June kom %s" % ", ".join(sorted(only_koms)))); continue
        if "/" in n or "\\" in n:
            refused.append((n, "", "name has a path separator")); continue
        if key in held:
            refused.append((n, "", "held (june-asset-port Rule 8, tools/data/asset_port_held.tsv): %s" % held[key])); continue
        # names over LONG_NAME characters stage like any other since X4 (SERV_IRUHADEV_KOM_LONG_NAMES) shipped; --long-names is a no-op
        where = dict(live.get(key, {}))
        for stem, sp in staged.get(key, {}).items():
            where.setdefault(stem, sp)
        if len(where) > 1:
            refused.append((n, "", "March already has it in %d koms: %s" % (len(where), ", ".join(sorted(where))))); continue
        if where:
            (stem, spelling), = where.items()
            origin = "March"
        else:
            # June's number; several identical copies -> prefer a kom whose staged dir already exists, then the lowest number
            stems = sorted({c[1] for c in cands}, key=lambda s: (not os.path.isdir(os.path.join(KOM_ROOT, s + "_kom")), s))
            stem, spelling, origin = stems[0], cands[0][3]["name"], "June"
        plan[key] = dict(name=spelling, stem=stem, origin=origin, cands=cands)

    # strict decode of every June copy (identical bytes -> any copy will do; differing -> refuse and list the koms)
    tasks = [(k, v["cands"], not dry or transform is not None) for k, v in plan.items()]
    tasks.sort(key=lambda t: -sum(c[3]["csize"] * (1 if c[3]["algo"] == 0 else 25) for c in t[1]))
    if jobs > 1 and len(tasks) > 8:
        import multiprocessing
        with multiprocessing.Pool(jobs) as pool:
            results = list(pool.imap_unordered(_stage_task, tasks, chunksize=4))
    else:
        results = [_stage_task(t) for t in tasks]
    data = {}
    for key, rows, first in results:
        v = plan[key]
        bad = [r for r in rows if r[1]]
        if bad:
            refused.append((v["name"], v["stem"], "June file does not decode strictly: " + "; ".join("%s: %s" % (r[0], r[1]) for r in bad)))
            del plan[key]
        elif len({r[2] for r in rows}) > 1:
            refused.append((v["name"], v["stem"], "June koms disagree: " + ", ".join("%s=%d B/%s" % (r[0], r[3], r[2][:8]) for r in rows)))
            del plan[key]
        else:
            v["size"], v["sha1"], v["junes"] = rows[0][3], rows[0][2], sorted(r[0] for r in rows)
            data[key] = first
    if transform is not None:
        order = sorted(data, key=lambda k: plan[k]["name"].lower())
        if transform_jobs > 1 and len(order) > 1:       # the work is subprocesses (ljd, luajit): threads are enough
            import concurrent.futures as cf
            with cf.ThreadPoolExecutor(max_workers=transform_jobs) as ex:
                outs = list(ex.map(lambda k: transform(plan[k]["name"], data[k], plan[k]["stem"]), order))
        else:
            outs = [transform(plan[k]["name"], data[k], plan[k]["stem"]) for k in order]
        for key, (out, note) in zip(order, outs):
            v = plan[key]
            if out is None:
                refused.append((v["name"], v["stem"], note))
                del plan[key], data[key]
                continue
            data[key] = out
            v["size"], v["sha1"], v["note"] = len(out), hashlib.sha1(out).hexdigest(), note

    # per-kom plan / write
    by_kom = {}
    for key, v in plan.items():
        by_kom.setdefault(v["stem"], []).append(key)
    stamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    totals = {"new": 0, "replace": 0, "same": 0}
    staged_out, moved_sidecars = [], []
    tsv = ["name\tkom\tchosen_by\tjune_koms\tsize\tstate\tmanifest"]
    for stem in sorted(by_kom):
        d = os.path.join(KOM_ROOT, stem + "_kom")
        newkom = not os.path.isdir(d)
        man = {}
        if os.path.isfile(os.path.join(d, "_manifest.json")):
            man = {k.lower(): v for k, v in load_manifest(d).items()}
        keys = sorted(by_kom[stem], key=lambda k: plan[k]["name"].lower())
        cnt = {"new": 0, "replace": 0, "same": 0}
        listing, added = [], []
        for key in keys:
            v = plan[key]
            dst = os.path.join(d, v["name"])
            if os.path.isfile(dst):
                with open(dst, "rb") as f:
                    cur = f.read()
                state = "same" if hashlib.sha1(cur).hexdigest() == v["sha1"] else "replace"
            else:
                state = "new"
            cnt[state] += 1
            want = manifest_value(v["name"])
            have = man.get(key)
            mnote = want if have is None else ("kept %s" % have if have == want else "kept %s (rule says %s)" % (have, want))
            if have is None:
                added.append((v["name"], want))
            listing.append("    %-8s %-52s %9d B  %s%s  manifest %s" % (
                state, v["name"], v["size"], v["origin"] + "'s " + stem, "" if len(v["junes"]) == 1 else "  [June copies: %s, identical]" % ",".join(v["junes"]), mnote))
            tsv.append("\t".join([v["name"], stem, v["origin"], ",".join(v["junes"]), str(v["size"]), state, mnote]))
            staged_out.append(dict(name=v["name"], stem=stem, state=state, junes=list(v["junes"]), size=v["size"], sha1=v["sha1"],
                                   origin=v["origin"], manifest=want, note=v.get("note", "")))
            side = os.path.join(d, v["name"][:-4] + ".src.lua") if move_sidecars and v["name"].lower().endswith(".lua") else None
            if side and os.path.isfile(side):
                moved_sidecars.append((stem, v["name"][:-4] + ".src.lua"))
                listing.append("             sidecar %s is superseded: %s" % (v["name"][:-4] + ".src.lua", "(dry run)" if dry else "moved to the backup dir"))
                if not dry:
                    bk = os.path.join(STAGE_BACKUP_ROOT, stamp, stem + "_kom", v["name"][:-4] + ".src.lua")
                    os.makedirs(os.path.dirname(bk), exist_ok=True)
                    shutil.move(side, bk)
            if not dry and state != "same":
                if state == "replace":
                    bk = os.path.join(STAGE_BACKUP_ROOT, stamp, stem + "_kom", v["name"])
                    os.makedirs(os.path.dirname(bk), exist_ok=True)
                    shutil.copy2(dst, bk)
                os.makedirs(d, exist_ok=True)
                tmp = dst + ".part"
                with open(tmp, "wb") as f:
                    f.write(data[key])
                os.replace(tmp, dst)
        if not dry and added:
            add_manifest_keys(d, added)
        for k2 in cnt:
            totals[k2] += cnt[k2]
        p("  %s%s: %d name(s): %d new, %d replaced, %d identical; %d manifest key(s) %s" % (
            stem, " (NEW kom dir; no live archive of that number)" if newkom else "", len(keys), cnt["new"], cnt["replace"], cnt["same"],
            len(added), "to add" if dry else "added"))
        if len(plan) <= 200:
            for l in listing:
                p(l)
    for n, stem, why in sorted(refused, key=lambda r: r[0].lower()):
        p("  REFUSED %s%s: %s" % (n, " (" + stem + ")" if stem else "", why))
        tsv.append("\t".join([n, stem, "", "", "", "REFUSED", why]))
    if len(plan) > 200:
        p("  (more than 200 names: per-file lines are in --plan-out)")
    if plan_out:
        with open(plan_out, "w", encoding="utf-8", newline="\n") as f:
            f.write("\n".join(tsv) + "\n")
        p("  plan written: %s" % plan_out)
    if not dry and (plan or moved_sidecars):
        os.makedirs(STAGE_BACKUP_ROOT, exist_ok=True)
        p("  replaced files%s were backed up under %s\\%s" % (" and sidecars" if moved_sidecars else "", STAGE_BACKUP_ROOT, stamp))
    p("STAGE-JUNE %s: %d kom(s), %d staged (%d new, %d replaced, %d identical), %d refused, %.0fs" % (
        "DRY RUN" if dry else ("OK" if not refused else "PARTIAL"), len(by_kom), len(plan),
        totals["new"], totals["replace"], totals["same"], len(refused), time.time() - t0))
    if not dry and plan:
        p("Next: kom.py pack %s   (then `copy`)" % " ".join(sorted(by_kom)))
    if refused and exit_on_refuse:
        sys.exit(4)
    return dict(plan=plan, refused=refused, staged=staged_out, by_kom=sorted(by_kom), sidecars=moved_sidecars,
                backup=os.path.join(STAGE_BACKUP_ROOT, stamp))


def cmd_stage_server(names, kom):
    ls = _luasrc()
    d = paths(norm(kom))["dir"]
    man = os.path.join(d, "_manifest.json")
    if not os.path.isfile(man):
        die(2, "no manifest in %s" % d)
    with open(man, "rb") as f:
        keys = json.loads(f.read().decode("utf-8-sig"))
    src_dir = os.path.join(TRUNK, "KncWX2Server", "ServerResource", "US")
    p("== stage-server -> %s" % d)
    for n in names:
        src = os.path.join(src_dir, n)
        dst = os.path.join(d, n)
        if not os.path.isfile(src):
            die(2, "STAGE-SERVER REFUSED %s: no source at %s" % (n, src))
        if keys.get(n) != "2":
            die(4, "STAGE-SERVER REFUSED %s: manifest key is %r, not \"2\" (add it first)" % (n, keys.get(n)))
        with open(src, "rb") as f:
            text = f.read()
        if not text.startswith(b"\xef\xbb\xbf"):
            die(4, "STAGE-SERVER REFUSED %s: source has no UTF-8 BOM" % n)
        bc, err = ls.compile_text(text[3:])
        if not bc:
            die(4, "STAGE-SERVER REFUSED %s: does not compile: %s" % (n, err))
        if os.path.isfile(dst):
            os.makedirs(BACKUP_ROOT, exist_ok=True)
            bk = os.path.join(BACKUP_ROOT, "%s.%s.bytecode" % (n, time.strftime("%Y%m%d_%H%M%S")))
            shutil.copy2(dst, bk)
            p("  backup: %s" % bk)
        with open(dst, "wb") as f:
            f.write(bc)
        p("  staged %s: %d bytes, md5 %s" % (n, len(bc), hashlib.md5(bc).hexdigest()))
    p("STAGE-SERVER OK. Next: kom.py pack %s, then kom.py copy %s" % (norm(kom), norm(kom)))


def cmd_status(names, days):
    detail = bool(names)
    if not names:
        names = sorted({norm(f.split("_")[0].split(".")[0]) for f in os.listdir(KOM_ROOT)
                        if re.fullmatch(r"data[i]?\d{3}_kom", f)})
    rows = []
    for name in names:
        pa = paths(name)
        if not os.path.isdir(pa["dir"]):
            rows.append((name, "NO DIR", "")); continue
        t, who = newest_content(pa["dir"])
        packed = os.path.isfile(pa["packed"])
        live = os.path.isfile(pa["live"])
        lt = os.path.getmtime(pa["live"]) if live else 0
        pt = os.path.getmtime(pa["packed"]) if packed else 0
        if packed and t > pt:
            state, note = "PACK NEEDED", "%s newer than packed (%s)" % (who, fmt_time(t))
        elif packed:
            same = live and os.path.getsize(pa["live"]) == os.path.getsize(pa["packed"]) and md5(pa["live"]) == md5(pa["packed"])
            state, note = ("in sync", "") if same else ("COPY NEEDED", "packed %s differs from live" % fmt_time(pt))
        else:
            state, note = "-", ""
        if stale_sources(pa["dir"]):
            note += "  [stale .src.lua: compile]"
        if not detail and (not packed or max(t, pt) < time.time() - days * 86400):
            state, note = "-", ""  # never packed here, or untouched for `days`: not part of the current workflow
        rows.append((name, state, note.strip()))
    shown = [r for r in rows if detail or r[1] not in ("-", "in sync")]
    for n, s, note in shown:
        p("%-9s %-12s %s" % (n, s, note))
    if not detail:
        p("(%d archives scanned: %d need a pack, %d need a copy, %d in sync, %d not in play)" % (
            len(rows), sum(r[1] == "PACK NEEDED" for r in rows), sum(r[1] == "COPY NEEDED" for r in rows),
            sum(r[1] == "in sync" for r in rows), sum(r[1] == "-" for r in rows)))
        p("(only archives with a packed kom touched in the last %g day(s) are listed; --days N widens it, or name an archive)" % days)
    if detail:
        for name in names:
            pa = paths(name)
            if os.path.isfile(pa["packed"]) and os.path.isfile(pa["live"]):
                a, r, c = diff(entry_map(inspect(pa["live"])), entry_map(inspect(pa["packed"])))
                report_diff("live (what `copy` would change)", a, r, c)


# ----------------------------------------------------------------------------------------------
# june-asset-port P0 gates (plan JUNE_ASSET_PORT_PHASES.md, Rule 4): verify-staged, pack/copy --wave, dupcheck, copy-loose
SCRATCH = os.path.join(TRUNK, ".build", "kom_gate")
LOOSE_DESTS = ("music", "movie", "eventscene")
LOOSE_ROOT = os.path.join(BACKUP_ROOT, "loose")


def wave_plan(args):
    """Name list(s) -> (names, {kom stem: set(lower names staged there)}, unlocated names, held hits). Lists as for stage-june."""
    names, seen = [], set()
    for a in args:
        for n in read_names(a):
            if n.lower() not in seen:
                seen.add(n.lower())
                names.append(n)
    staged, held = staged_names(), load_held()
    by_kom, unlocated, hits = {}, [], []
    for n in names:
        key = n.lower()
        if key in held:
            hits.append(n)
        where = staged.get(key)
        if not where:
            unlocated.append(n)
            continue
        for stem in where:
            by_kom.setdefault(stem, set()).add(key)
    return names, by_kom, unlocated, hits


def _live_entry_bytes(live_path, names):
    """name -> (bytes or None, error): June's strict reader for algo-0 entries of the live kom; anything else (March's algo-2 key, an
    old header) comes from one elstools extract of the live kom into a scratch dir."""
    jk = _junekom()
    out, fallback = {}, []
    try:
        k = jk.Kom(live_path)
        by = {e["name"]: e for e in k.entries}
    except ValueError:
        k, by = None, {}
    if k is not None:
        with open(live_path, "rb") as f:
            for n in names:
                e = by.get(n)
                if e is None:
                    out[n] = (None, "not in the live kom")
                    continue
                data = None
                if e["algo"] == 0:
                    f.seek(e["offset"])
                    data, _err = jk.decode(e, f.read(e["csize"]), k.v04)
                if data is None:
                    fallback.append(n)
                else:
                    out[n] = (data, "")
    else:
        fallback = list(names)
    if fallback:
        stem = os.path.splitext(os.path.basename(live_path))[0]
        tmp = os.path.join(SCRATCH, "extract_" + stem)
        shutil.rmtree(tmp, ignore_errors=True)
        os.makedirs(tmp)
        rc, _o = elstools(["extract", "-q", "-i", live_path, "-o", tmp, "-v", VERSION])
        d = os.path.join(tmp, stem + "_kom")
        for n in fallback:
            fp = os.path.join(d, n)
            if os.path.isfile(fp):
                with open(fp, "rb") as f:
                    out[n] = (f.read(), "")
            else:
                out[n] = (None, "elstools extract (rc=%s) did not produce it" % rc)
        shutil.rmtree(tmp, ignore_errors=True)
    return out


def wave_gate(name, wave):
    """Gates (a) and (b) for one kom against its live archive. wave: {stem: set(lower names)}. True when both pass."""
    pa = paths(name)
    inc = wave.get(name, set())
    new = entry_map(inspect(pa["packed"]))
    live_exists = os.path.isfile(pa["live"])
    old = entry_map(inspect(pa["live"])) if live_exists else {}
    added, removed, changed = diff(old, new)
    bad = []
    if removed:
        bad.append("(a) no-loss: %d live entr%s removed" % (len(removed), "y" if len(removed) == 1 else "ies"))
        lst(removed, "-")
    extra = [n for n in added if n.lower() not in inc]
    if extra:
        bad.append("%d added entr%s not on the wave list (a peer's staging in this kom dir?)" % (len(extra), "y" if len(extra) == 1 else "ies"))
        lst(extra, "+")
    in_wave = [n for n in changed if n.lower() in inc]
    off = [n for n in changed if n.lower() not in inc]
    recomp, diffs = [], []
    if off:
        live_bytes = _live_entry_bytes(pa["live"], off)
        for n in off:
            data, err = live_bytes.get(n, (None, "no result"))
            with open(os.path.join(pa["dir"], n), "rb") as f:
                have = f.read()
            if data is None:
                diffs.append("%s (cannot decode the live entry: %s)" % (n, err))
            elif data != have:
                diffs.append("%s (live %d B, staged %d B)" % (n, len(data), len(have)))
            else:
                recomp.append(n)
        if diffs:
            bad.append("(b) %d unexplained ~changed entr%s: live bytes differ from the staged file" % (len(diffs), "y" if len(diffs) == 1 else "ies"))
            lst(diffs, "~")
    p("  wave gate %s: +%d added (%d on the list), -%d removed, ~%d changed (%d on the list, %d recompression only, %d unexplained); %d name(s) on the list for this kom%s" % (
        name, len(added), len(added) - len(extra), len(removed), len(changed), len(in_wave), len(recomp), len(diffs), len(inc),
        "" if live_exists else "; no live kom, new archive"))
    for b in bad:
        p("  WAVE GATE FAILED %s: %s" % (name, b))
    return not bad


def _packed_names(stem):
    pk = paths(stem)["packed"]
    return [e["name"] for e in inspect(pk)["files"]]


def run_dupcheck(withs, quiet=False):
    """Gate (c): names in two koms of the would-be-live set. Returns (new, old) duplicate dicts (lower name -> sorted koms)."""
    live = live_names()
    if withs:
        stems = [norm(w) for w in withs]
        for s in stems:
            if not os.path.isfile(paths(s)["packed"]):
                die(4, "DUPCHECK REFUSED: %s has no packed kom (`kom.py pack %s` first)" % (s, s))
    else:
        stems = []
        for d in sorted(os.listdir(KOM_ROOT)):
            m = re.fullmatch(r"(?i)(datai?\d{3})_packed\.kom", d)
            if not m:
                continue
            s = m.group(1).lower()
            pa = paths(s)
            if not os.path.isfile(pa["live"]) or os.path.getsize(pa["live"]) != os.path.getsize(pa["packed"]) or md5(pa["live"]) != md5(pa["packed"]):
                stems.append(s)
    would, swapped = {}, set(stems)
    for key, where in live.items():
        for stem in where:
            if stem not in swapped:
                would.setdefault(key, []).append(stem)
    inner = []
    for s in stems:
        seen = set()
        for n in _packed_names(s):
            k = n.lower()
            if k in seen:
                inner.append((s, n))
            seen.add(k)
            would.setdefault(k, []).append(s)
    new, old = {}, {}
    for key, where in would.items():
        if len(where) > 1:
            (new if set(where) & swapped else old)[key] = sorted(where)
    if not quiet:
        p("== dupcheck: %d kom(s) swapped in (%s), %d names in the would-be-live set" % (len(stems), " ".join(sorted(stems)) or "none", len(would)))
        for key in sorted(new)[:SHOW]:
            p("    NEW duplicate %s in %s" % (key, ", ".join(new[key])))
        for key in sorted(old)[:SHOW]:
            p("    live duplicate %s in %s" % (key, ", ".join(old[key])))
        for s, n in inner[:SHOW]:
            p("    twice inside %s: %s" % (s, n))
    return new, old, inner


def cmd_dupcheck(withs):
    new, old, inner = run_dupcheck(withs)
    n = len(new) + len(old) + len(inner)
    if n:
        die(4, "DUPCHECK FAILED: %d duplicate name(s) (%d introduced by the swapped koms, %d already live, %d inside one kom). "
               "NOT_ALLOW_DUPLICATE_FILES deletes the whole kom at mount and shows a MessageBox." % (n, len(new), len(old), len(inner)))
    p("DUPCHECK OK: 0 duplicate names")


def _verify_task(task):
    """Worker: staged file vs every June copy of the name (strict decode). Returns (key, problem or '')."""
    key, staged_path, cands = task
    jk = _junekom()
    try:
        with open(staged_path, "rb") as f:
            have = f.read()
    except OSError as ex:
        return key, "staged file unreadable: %s" % ex
    sha, seen = hashlib.sha1(have).hexdigest(), []
    for path, stem, v04, e in cands:
        with open(path, "rb") as f:
            f.seek(e["offset"])
            raw = f.read(e["csize"])
        out, err = jk.decode(e, raw, v04)
        if out is None:
            return key, "June %s does not decode strictly: %s" % (stem, err)
        seen.append((stem, hashlib.sha1(out).hexdigest(), len(out)))
    if len({s[1] for s in seen}) > 1:
        return key, "June koms disagree: " + ", ".join("%s=%d B" % (s[0], s[2]) for s in seen)
    if seen[0][1] != sha:
        return key, "STAGED != JUNE (staged %d B, June %s %d B)" % (len(have), seen[0][0], seen[0][2])
    return key, ""


def cmd_verify_staged(args, jobs):
    """Gate (d). Each arg is a list file, a kom name (every staged file June also has) or a single file name."""
    held, staged = load_held(), staged_names()
    work, problems, skipped_lua, march_only = {}, [], 0, 0
    june, _koms = june_index()
    for a in args:
        if not os.path.isfile(a) and re.fullmatch(r"(?i)(?:data)?i?0*\d+(?:_kom|\.kom)?", a.strip()):
            stem = norm(a)
            d = paths(stem)["dir"]
            if not os.path.isdir(d):
                die(2, "no such kom dir: %s" % d)
            for fn in sorted(os.listdir(d)):
                if fn == "_manifest.json" or fn.lower().endswith(".lua"):
                    skipped_lua += fn.lower().endswith(".lua") and not fn.lower().endswith(".src.lua")
                    continue
                if not os.path.isfile(os.path.join(d, fn)):
                    continue
                if fn.lower() not in june:
                    march_only += 1
                    continue
                work.setdefault(fn.lower(), (fn, stem))
        else:
            for n in read_names(a):
                work.setdefault(n.lower(), (n, None))
    tasks, manifests = [], {}
    for key, (n, stem) in sorted(work.items()):
        if key in held:
            problems.append((n, "held (Rule 8): %s" % held[key]))
            continue
        where = staged.get(key, {})
        if len(where) != 1:
            problems.append((n, "staged in %d koms: %s" % (len(where), ", ".join(sorted(where)) or "none")))
            continue
        (st, spelling), = where.items()
        if key not in june:
            problems.append((n, "not in any June kom"))
            continue
        if st not in manifests:
            mp = os.path.join(KOM_ROOT, st + "_kom", "_manifest.json")
            manifests[st] = {k.lower() for k in load_manifest(os.path.dirname(mp))} if os.path.isfile(mp) else set()
        if key not in manifests[st]:
            problems.append((n, "no _manifest.json key in %s_kom" % st))
        tasks.append((key, os.path.join(KOM_ROOT, st + "_kom", spelling), june[key]))
    p("== verify-staged: %d name(s)%s%s" % (len(work), ", %d .lua skipped (core-list sources differ from June by design)" % skipped_lua if skipped_lua else "",
                                             ", %d March-only file(s) not compared" % march_only if march_only else ""))
    tasks.sort(key=lambda t: -sum(c[3]["csize"] for c in t[2]))
    if jobs > 1 and len(tasks) > 8:
        import multiprocessing
        with multiprocessing.Pool(jobs) as pool:
            results = list(pool.imap_unordered(_verify_task, tasks, chunksize=4))
    else:
        results = [_verify_task(t) for t in tasks]
    for key, bad in results:
        if bad:
            problems.append((work[key][0], bad))
    for n, why in sorted(problems, key=lambda r: r[0].lower())[:40]:
        p("  PROBLEM %s: %s" % (n, why))
    if len(problems) > 40:
        p("  ... and %d more" % (len(problems) - 40))
    if problems:
        if any(w.startswith("STAGED != JUNE") for _n, w in problems) and any(not os.path.isfile(a) for a in args):
            p("  note: a kom argument also lists March's own copies that differ from June's and wait for a replace wave; "
              "gate a wave with its list file (`verify-staged june2017_re/pN_wave.tsv`), where every name must equal June's.")
        die(4, "VERIFY-STAGED FAILED: %d problem(s) in %d name(s)" % (len(problems), len(work)))
    p("VERIFY-STAGED OK: %d name(s) equal June's strict decode, 0 problems" % len(work))


def cmd_copy_loose(args, dest, use_all, dry):
    import junekom
    src_dir = os.path.join(junekom.JUNE_DATA, dest)
    dst_dir = os.path.join(GAME_DIR, dest)
    if not os.path.isdir(src_dir):
        die(2, "June has no %s: %s" % (dest, src_dir))
    held = load_held()
    if use_all:
        names = sorted(f for f in os.listdir(src_dir) if os.path.isfile(os.path.join(src_dir, f)))
    else:
        names, seen = [], set()
        for a in args:
            for n in read_names(a):
                if n.lower() not in seen:
                    seen.add(n.lower())
                    names.append(n)
    if not names:
        die(2, "no names: give a list file or --all")
    pids = running(GAME_EXE)
    if pids and not dry:
        die(3, "COPY-LOOSE REFUSED: %s is running (pid %s). Ask the user to close the game, then rerun." % (GAME_EXE, pids))
    stamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    p("== copy-loose%s %s: %d name(s)  %s -> %s" % (" (DRY RUN)" if dry else "", dest, len(names), src_dir, dst_dir))
    new = repl = same = 0
    failed = []
    for n in names:
        if n.lower() in held:
            failed.append((n, "held (Rule 8)"))
            continue
        if "/" in n or "\\" in n:
            failed.append((n, "name has a path separator"))
            continue
        src, dst = os.path.join(src_dir, n), os.path.join(dst_dir, n)
        if not os.path.isfile(src):
            failed.append((n, "not in June's %s" % dest))
            continue
        want = md5(src)
        exists = os.path.isfile(dst)
        if exists and os.path.getsize(dst) == os.path.getsize(src) and md5(dst) == want:
            same += 1
            continue
        if dry:
            new, repl = (new, repl + 1) if exists else (new + 1, repl)
            continue
        os.makedirs(dst_dir, exist_ok=True)
        if exists:
            bk = os.path.join(LOOSE_ROOT, stamp, dest, n)
            os.makedirs(os.path.dirname(bk), exist_ok=True)
            shutil.copy2(dst, bk)
        part = dst + ".part"
        try:
            shutil.copy2(src, part)
            os.replace(part, dst)
        except OSError as e:
            if os.path.exists(part):
                try:
                    os.remove(part)
                except OSError:
                    pass
            failed.append((n, "copy failed: %s" % e))
            continue
        if md5(dst) != want:
            failed.append((n, "md5 mismatch after the copy"))
            continue
        new, repl = (new, repl + 1) if exists else (new + 1, repl)
    for n, why in failed[:SHOW]:
        p("  FAILED %s: %s" % (n, why))
    if len(failed) > SHOW:
        p("  ... and %d more" % (len(failed) - SHOW))
    if repl and not dry:
        p("  replaced files were backed up under %s\\%s\\%s" % (LOOSE_ROOT, stamp, dest))
    if failed:
        die(1, "COPY-LOOSE FAILED %s: %d copied (%d new, %d replaced), %d identical, %d failed" % (dest, new + repl, new, repl, same, len(failed)))
    p("COPY-LOOSE %s %s: %d %s (%d new, %d replaced), %d identical, 0 failed" % (
        "DRY RUN" if dry else "OK", dest, new + repl, "would copy" if dry else "copied", new, repl, same))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    sv = sub.add_parser("verify-staged")
    sv.add_argument("targets", nargs="+", help="a list file, a kom (data048), or a file name")
    sv.add_argument("--jobs", type=int, default=max(1, (os.cpu_count() or 2) - 1), help="decode workers")
    sd = sub.add_parser("dupcheck")
    sd.add_argument("--with", dest="withs", nargs="*", default=[], metavar="dataNNN", help="koms whose packed archive replaces the live one (default: every packed kom that differs from live)")
    sl = sub.add_parser("copy-loose")
    sl.add_argument("lists", nargs="*", help="a file name, or a list file (one name per line, or a TSV with a `name` column)")
    sl.add_argument("--dest", required=True, choices=LOOSE_DESTS, help="June's data/<dest> -> game dir <dest>")
    sl.add_argument("--all", action="store_true", help="every file in June's <dest> dir")
    sl.add_argument("--dry-run", action="store_true", help="print the counts, write nothing")
    for c in ("pack", "compile", "copy", "status", "stage-june", "stage-server"):
        s = sub.add_parser(c)
        if c == "stage-server":
            s.add_argument("names", nargs="+", help="server Lua file names under KncWX2Server/ServerResource/US/, e.g. RandomItemTable.lua")
            s.add_argument("--kom", default="data036", help="target kom (default data036)")
            continue
        if c == "stage-june":
            s.add_argument("koms", nargs="+", help="a file name, or a list file (one name per line, or a TSV with a `name` column)")
            s.add_argument("--dry-run", action="store_true", help="print the plan, write nothing")
            s.add_argument("--long-names", action="store_true", help="allow names over 59 characters (only once X4's flag is on)")
            s.add_argument("--jobs", type=int, default=max(1, (os.cpu_count() or 2) - 1), help="decode workers for big lists")
            s.add_argument("--plan-out", help="write the per-file plan as a TSV")
            continue
        s.add_argument("koms", nargs="*" if c == "status" else "+", help="data036 | 036 | 36 | datai001 ...")
        if c == "copy":
            s.add_argument("--allow-removed", action="store_true", help="copy even if the packed kom drops entries the live kom has")
            s.add_argument("--force", action="store_true", help="copy even if the kom dir is newer than the packed kom")
        if c == "status":
            s.add_argument("--days", type=float, default=2, help="no-arg status: only archives touched within N days (default 2)")
        if c == "compile":
            s.add_argument("--bytecode", action="store_true", help="rollback only: compile stale X.src.lua to mar-2014 bytecode with elstools")
        if c == "pack":
            s.add_argument("--allow-unlisted", action="store_true", help="pack files the manifest does not list (they get algo 2)")
        if c in ("pack", "copy"):
            s.add_argument("--wave", nargs="+", metavar="LIST", help="june-asset-port wave list(s): run gates (a)/(b) (and (c) on copy) against the live kom")
    a = ap.parse_args()
    if a.cmd == "verify-staged":
        cmd_verify_staged(a.targets, a.jobs)
        return
    if a.cmd == "dupcheck":
        cmd_dupcheck(a.withs)
        return
    if a.cmd == "copy-loose":
        cmd_copy_loose(a.lists, a.dest, a.all, a.dry_run)
        return
    if a.cmd == "stage-server":
        cmd_stage_server(a.names, a.kom)
        return
    if a.cmd == "stage-june":
        cmd_stage_june(a.koms, a.dry_run, a.long_names, a.jobs, a.plan_out)
        return
    names = [norm(k) for k in a.koms]
    if a.cmd == "status":
        cmd_status(names, a.days)
        return
    wave = None
    if getattr(a, "wave", None):
        wnames, wave, unlocated, hits = wave_plan(a.wave)
        p("== wave: %d name(s) in %d kom dir(s)%s" % (len(wnames), len(wave), "; %d not staged anywhere" % len(unlocated) if unlocated else ""))
        if hits:
            die(4, "WAVE REFUSED: %d held name(s) on the list (Rule 8): %s" % (len(hits), ", ".join(hits[:5])))
        if unlocated:
            lst(unlocated, "?")
            die(4, "WAVE REFUSED: %d name(s) on the list are staged in no kom dir (run `stage-june` first)." % len(unlocated))
    for n in names:
        if a.cmd == "pack":
            cmd_pack(n, a.allow_unlisted, wave)
        elif a.cmd == "compile":
            cmd_compile(n, a.bytecode)
        else:
            cmd_copy(n, a.allow_removed, a.force, wave)


if __name__ == "__main__":
    main()
