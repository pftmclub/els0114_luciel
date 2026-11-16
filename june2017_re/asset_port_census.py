#!/usr/bin/env python3
"""June asset port census (plan june-asset-port, phase P0). Read-only on June's tree, the game dir and kom_contents.

    python june2017_re/asset_port_census.py                  # -> june2017_re/p0_census.tsv, p0_waves.tsv, p0_summary.tsv
    python june2017_re/asset_port_census.py --all            # also list the `same` rows in p0_census.tsv (92k+ rows)
    python june2017_re/asset_port_census.py --init-held      # write tools/data/asset_port_held.tsv if it does not exist yet
    python june2017_re/asset_port_census.py --jobs 6 --no-cache

One row per June entry (all 258 June koms), compared with March's bytes: the staged file in kom_contents/<kom>_kom/ (the live
archive's content; `kom.py pack` proves the two agree) against June's exact extract in <June data>\\<kom>_kom\\ (size first, then md5).

Buckets (column `bucket`):
  june-only       March has no file of that name (live or staged)            -> added from June's kom number
  same            March's file is byte-identical                             -> nothing to do (not listed unless --all)
  differs         March has the name with other bytes                        -> June replaces it
  march-unstaged  March's live kom has the name but kom_contents does not    -> restore first (P0 step 2); must be 0
  lua             .lua                                                       -> lua ports, not this plan
  held            a name in tools/data/asset_port_held.tsv (Rule 8) whose state is june-only / differs
Column `route` is the provisional wave (P2 new archive, P4a/b/c existing kom, P6 texture+sound, P7 TET/XET, P8 mesh+motion, P8b the
four _INT.txt, P9, P10); P5 refines the replace waves. Column `inbound` names the other plan that asked for the file (plan doc
"Inbound routes"). `march_kom` is the kom that holds March's copy (replace waves stage there), `june_kom` June's number
(a june-only file goes there). `note` carries march-in-N-koms, june-in-N-koms, june-koms-differ, extract-missing, size-mismatch.

Baseline check: the 2026-10-08 planning counts are printed beside today's; every `same` file staged after the cutoff (default
2026-10-09 00:00) is attributed to june-only (new) or differs (a backup under kom_backup_stage_june holds March's original), and to
the stage-plan TSV that names it, so the difference is explained by plan (p0_delta_summary.tsv; per-file rows in .build/p0_delta.tsv).
"""
import argparse, ast, collections, csv, datetime, hashlib, json, multiprocessing, os, re, sys, time

for _s in (sys.stdout, sys.stderr):
    try:
        _s.reconfigure(encoding="utf-8", errors="replace")
    except Exception:
        pass

HERE = os.path.dirname(os.path.abspath(__file__))
TRUNK = os.path.dirname(HERE)
sys.path.insert(0, os.path.join(TRUNK, "tools"))
import kom as komtool          # noqa: E402  live_names(), staged_names(), KOM_ROOT, GAME_DIR
import junekom as jk           # noqa: E402  Kom, all_koms(), decode(), JUNE_DATA

HELD_TSV = os.path.join(TRUNK, "tools", "data", "asset_port_held.tsv")
CACHE = os.path.join(TRUNK, ".build", "asset_port_census_cache.json")
OUT = HERE
BASELINE = {   # 2026-10-08 planning session ("Established facts")
    "june-only": 117562, "same": 92457, "differs": 4397, "march-unstaged": 1066, "lua": 7329, "total": 222811,
}
STAGE_BACKUP = os.path.join(os.path.dirname(komtool.KOM_ROOT), "kom_backup_stage_june")
R12B_BACKUP = os.path.join(STAGE_BACKUP, "20261018_064307")   # March's originals of the class Motion files (june-engine R12b)
R12B_RESTAGED = {"motion_raven.x", "motion_chung.x", "motion_el.x", "motion_el_sk.x"}   # June's already (R12b)
W3F_SOURCE = os.path.join(HERE, "w3b_asset_closure.py")

TEXTURE_SOUND = {"dds", "tga", "bmp", "png", "ogg", "wav", "jpg", "ttf", "swf", "gfx", "xml", "fx", "txt"}
MESH = {"x", "y"}
ATLAS = {"tet", "xet"}
INT_TXT = {"uimajorparticle_int.txt", "gamemajorparticle_int.txt", "uimajorxmeshplayer_int.txt", "gamemajorxmeshplayer_int.txt"}
FIXED_INBOUND = {   # plan doc "Inbound routes"
    "xenvboxrenderer.fx": "june-engine", "xheatshimmerrenderer.fx": "june-engine", "xrenderer_x2tool.fx": "june-engine",
    "perturbationmap.png": "june-engine", "13gang_protect.tga": "june-engine", "xrenderer.fx": "june-engine R2",
    "village_adboard.dds": "june-effect", "motion_riding_godbeast_fire.x": "june-effect GodBeast Fire",
    "riding_godbeast_fire.tga": "june-effect GodBeast Fire",
    "ruben_village_herb01.tga": "char-rena", "ruben_village_herb02.tga": "char-rena", "ruben_techhouse_ganpan.tga": "char-rena",
}
FIXED_ROUTE = {"xrenderer.fx": "P10"}


def p(*a):
    print(*a, flush=True)


# ------------------------------------------------------------------ held list
def w3f_names():
    with open(W3F_SOURCE, "r", encoding="utf-8") as f:
        tree = ast.parse(f.read())
    for n in tree.body:
        if isinstance(n, ast.Assign) and any(getattr(t, "id", "") == "KEPT_MARCH" for t in n.targets):
            return sorted(ast.literal_eval(n.value))
    raise SystemExit("KEPT_MARCH not found in %s" % W3F_SOURCE)


def init_held():
    if os.path.exists(HELD_TSV):
        p("held list exists, not touching: %s" % HELD_TSV)
        return
    rows = []
    for n in w3f_names():
        rows.append((n, "W3f: March route board / line map kept over June's (user 2026-11-12)", "june-dungeons W3f", "2026-11-12"))
    cls = []
    for sub in sorted(os.listdir(R12B_BACKUP)):
        d = os.path.join(R12B_BACKUP, sub)
        if os.path.isdir(d):
            for fn in sorted(os.listdir(d)):
                if fn.lower() not in R12B_RESTAGED:
                    cls.append(fn)
    for n in cls:
        rows.append((n, "class motion: June renamed the animations its class script plays; the class port stages it (R12b)",
                     "class port (JUNE2017_CHARACTER_PORT_CHECKLIST 7ad)", "2026-10-18"))
    os.makedirs(os.path.dirname(HELD_TSV), exist_ok=True)
    with open(HELD_TSV, "w", encoding="utf-8", newline="\n") as f:
        f.write("name\treason\towner\tdate\n")
        for r in rows:
            f.write("\t".join(r) + "\n")
    p("wrote %s: %d W3f + %d class-motion rows" % (HELD_TSV, len(w3f_names()), len(cls)))


def load_held():
    held = {}
    if not os.path.isfile(HELD_TSV):
        return held
    with open(HELD_TSV, "r", encoding="utf-8") as f:
        for r in csv.DictReader(f, delimiter="\t"):
            held[r["name"].strip().lower()] = r
    return held


# ------------------------------------------------------------------ inbound lists
def load_inbound():
    tags = collections.defaultdict(set)
    path = os.path.join(HERE, "atlas_a11_sizes.tsv")
    if os.path.isfile(path):
        with open(path, "r", encoding="utf-8") as f:
            for r in csv.DictReader(f, delimiter="\t"):
                if r["state"] in ("SHORT", "LONGER"):
                    tags[r["name"].lower()].add("atlas")
    path = os.path.join(TRUNK, "costume_shop", "withheld_files.tsv")
    if os.path.isfile(path):
        with open(path, "r", encoding="utf-8") as f:
            for r in csv.DictReader(f, delimiter="\t"):
                if r.get("missing_names"):
                    tags[r["missing_names"].strip().lower()].add("costume-shop")
    path = os.path.join(HERE, "char_eve", "ch0_effect_closure.tsv")
    for n in INT_TXT:
        tags[n].add("char-eve CH0")
    for n, t in FIXED_INBOUND.items():
        tags[n].add(t)
    return tags


# ------------------------------------------------------------------ hashing
def md5_file(path):
    h = hashlib.md5()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def _hash_task(path):
    try:
        return path, md5_file(path), ""
    except OSError as e:
        return path, "", str(e)


def _decode_task(task):
    idx, kom_path, name, size = task
    k = jk.Kom(kom_path)
    for e in k.entries:
        if e["name"] == name and e["size"] == size:
            with open(kom_path, "rb") as f:
                f.seek(e["offset"])
                raw = f.read(e["csize"])
            out, err = jk.decode(e, raw, k.v04)
            return idx, (hashlib.md5(out).hexdigest() if out is not None else ""), err
    return idx, "", "entry not found"


def load_cache(no_cache):
    if no_cache:
        return {}
    try:
        with open(CACHE, "r", encoding="utf-8") as f:
            return json.load(f)
    except (OSError, ValueError):
        return {}


def hash_all(paths, cache, jobs):
    """paths: iterable of file paths. Returns {path: md5}, updating cache by (size, mtime_ns)."""
    todo, out = [], {}
    for pth in paths:
        try:
            st = os.stat(pth)
        except OSError:
            continue
        ent = cache.get(pth)
        if ent and ent[0] == st.st_size and ent[1] == st.st_mtime_ns:
            out[pth] = ent[2]
        else:
            todo.append(pth)
    p("  hashing %d file(s) (%d cached)" % (len(todo), len(out)))
    if todo:
        t0 = time.time()
        with multiprocessing.Pool(jobs) as pool:
            for n, (pth, h, err) in enumerate(pool.imap_unordered(_hash_task, todo, chunksize=32), 1):
                if h:
                    st = os.stat(pth)
                    cache[pth] = [st.st_size, st.st_mtime_ns, h]
                    out[pth] = h
                if n % 20000 == 0:
                    p("    %d / %d (%.0fs)" % (n, len(todo), time.time() - t0))
    return out


# ------------------------------------------------------------------ census
# P4 wave cuts, re-cut 2026-11-16 from the live census by file count (the 2026-10-08 plan cut at 75 / 150: 4.2k / 5.2k / 27.2k files)
P4A_LAST, P4B_LAST = 150, 185


def wave_of(stem):
    m = re.fullmatch(r"data(i?)(\d{3})", stem)
    if not m:
        return "P4c"
    if m.group(1):
        return "P4c"
    n = int(m.group(2))
    return "P4a" if n <= P4A_LAST else ("P4b" if n <= P4B_LAST else "P4c")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--all", action="store_true", help="list `same` rows in p0_census.tsv too")
    ap.add_argument("--init-held", action="store_true", help="create tools/data/asset_port_held.tsv if missing, then exit")
    ap.add_argument("--jobs", type=int, default=max(1, (os.cpu_count() or 2) - 1))
    ap.add_argument("--no-cache", action="store_true")
    ap.add_argument("--cutoff", default="2026-10-09", help="baseline cutoff date for the delta explanation (default 2026-10-09)")
    ap.add_argument("--out-dir", default=OUT)
    a = ap.parse_args()
    if a.init_held:
        init_held()
        return
    t0 = time.time()
    held = load_held()
    inbound = load_inbound()
    p("== census: held list %d name(s), inbound lists %d name(s)" % (len(held), len(inbound)))

    koms, _skip = jk.all_koms()
    entries = []       # one dict per June entry
    problems = []
    for kp in koms:
        k = jk.Kom(kp)
        if k.problems:
            problems.append((k.stem, k.problems))
        for e in k.entries:
            entries.append(dict(june_kom=k.stem.lower(), kom_path=kp, name=e["name"], size=e["size"], ext=os.path.splitext(e["name"])[1].lstrip(".").lower()))
    p("  June: %d entries in %d koms (%.0fs)" % (len(entries), len(koms), time.time() - t0))
    for stem, pr in problems:
        p("  WARN June kom %s header problems: %s" % (stem, "; ".join(pr)))

    live = komtool.live_names()
    staged = komtool.staged_names()
    live_koms = {f[:-4].lower() for f in os.listdir(komtool.GAME_DIR) if re.fullmatch(r"(?i)datai?\d{3}\.kom", f)}
    june_koms_by_name = collections.defaultdict(set)
    for e in entries:
        june_koms_by_name[e["name"].lower()].add(e["june_kom"])
    p("  March: %d live names in %d koms, %d staged names" % (len(live), len(live_koms), len(staged)))

    june_data = jk.JUNE_DATA
    pairs = []     # (row, march_path, june_path)
    for e in entries:
        key = e["name"].lower()
        e["note"] = []
        if len(june_koms_by_name[key]) > 1:
            e["note"].append("june-in-%d-koms" % len(june_koms_by_name[key]))
        e["march_kom"] = ""
        tags = set(inbound.get(key, ()))
        if key.startswith("ridingpet_godbeast_fire"):   # june-effect C9h-4e: June's RidingPet_GodBeast_Fire script needs these
            tags.add("june-effect GodBeast Fire")
        e["inbound"] = ",".join(sorted(tags))
        if e["ext"] == "lua":
            e["bucket"] = "lua"
            continue
        sw = staged.get(key, {})
        lw = live.get(key, {})
        if not sw and not lw:
            e["bucket"] = "june-only"
            continue
        if not sw:
            e["bucket"] = "march-unstaged"
            e["march_kom"] = ",".join(sorted(lw))
            continue
        stems = sorted(sw)
        if len(stems) > 1:
            e["note"].append("march-in-%d-koms" % len(stems))
        e["march_kom"] = stems[0]
        mpath = os.path.join(komtool.KOM_ROOT, stems[0] + "_kom", sw[stems[0]])
        jpath = os.path.join(june_data, e["june_kom"] + "_kom", e["name"])
        e["_m"], e["_j"], e["bucket"] = mpath, jpath, "?"
        pairs.append(e)

    # size first, then hash
    need = []
    for e in pairs:
        try:
            msize = os.path.getsize(e["_m"])
        except OSError:
            e["bucket"], e["note"] = "march-unstaged", e["note"] + ["staged-file-unreadable"]
            continue
        if msize != e["size"]:
            e["bucket"] = "differs"
            e["note"].append("size %d vs june %d" % (msize, e["size"]))
            continue
        try:
            jsize = os.path.getsize(e["_j"])
        except OSError:
            jsize = -1
        if jsize != e["size"]:
            e["note"].append("extract-missing" if jsize < 0 else "extract-size-mismatch")
            e["_decode"] = True
        need.append(e)
    cache = load_cache(a.no_cache)
    paths = {e["_m"] for e in need} | {e["_j"] for e in need if not e.get("_decode")}
    hashes = hash_all(paths, cache, a.jobs)
    decode_tasks = [(i, e["kom_path"], e["name"], e["size"]) for i, e in enumerate(need) if e.get("_decode")]
    decoded = {}
    if decode_tasks:
        p("  decoding %d June entr%s whose extract is missing or short" % (len(decode_tasks), "y" if len(decode_tasks) == 1 else "ies"))
        with multiprocessing.Pool(a.jobs) as pool:
            for idx, h, err in pool.imap_unordered(_decode_task, decode_tasks, chunksize=4):
                decoded[idx] = (h, err)
    for i, e in enumerate(need):
        mh = hashes.get(e["_m"])
        if e.get("_decode"):
            jh, err = decoded.get(i, ("", "no result"))
            if err:
                e["note"].append("june-decode-failed: " + err)
        else:
            jh = hashes.get(e["_j"])
        if not mh or not jh:
            e["bucket"] = "differs"
            e["note"].append("unhashed")
        else:
            e["bucket"] = "same" if mh == jh else "differs"
    os.makedirs(os.path.dirname(CACHE), exist_ok=True)
    with open(CACHE, "w", encoding="utf-8") as f:
        json.dump(cache, f)

    # june-koms-differ: a name in several June koms with different sizes
    sizes = collections.defaultdict(set)
    for e in entries:
        sizes[e["name"].lower()].add(e["size"])
    for e in entries:
        if len(sizes[e["name"].lower()]) > 1:
            e["note"].append("june-koms-differ")

    # live-only March entries (no staged file)
    unstaged = []
    for key, where in live.items():
        for stem, spelling in where.items():
            if stem not in staged.get(key, {}):
                unstaged.append((stem, spelling))
    unstaged.sort()

    # held, routes
    for e in entries:
        key = e["name"].lower()
        if e["bucket"] in ("june-only", "differs") and key in held:
            e["bucket"] = "held"
            e["note"].append("held: " + held[key]["reason"][:60])
        b, ext = e["bucket"], e["ext"]
        if b in ("same", "lua", "held", "march-unstaged"):
            e["route"] = {"lua": "lua-port", "held": "held", "same": "-", "march-unstaged": "P0-restore"}[b]
        elif key in INT_TXT:
            e["route"] = "P8b"
        elif ext == "kim":
            e["route"] = "june-item-kim"
        elif ext == "txt":
            e["route"] = "txt-review"
        elif key in FIXED_ROUTE:
            e["route"] = FIXED_ROUTE[key]
        elif b == "june-only":
            e["route"] = wave_of(e["june_kom"]) if e["june_kom"] in live_koms else "P2"
        elif ext in ATLAS:
            e["route"] = "P7"
        elif ext in MESH:
            e["route"] = "P8"
        else:
            e["route"] = "P6"

    # ---- write
    od = a.out_dir
    cols = ["bucket", "route", "inbound", "june_kom", "march_kom", "name", "size", "ext", "note"]
    with open(os.path.join(od, "p0_census.tsv"), "w", encoding="utf-8", newline="\n") as f:
        f.write("\t".join(cols) + "\n")
        for e in sorted(entries, key=lambda x: (x["bucket"], x["june_kom"], x["name"].lower())):
            if e["bucket"] == "same" and not a.all:
                continue
            e["note"] = ";".join(e["note"]) if isinstance(e["note"], list) else e["note"]
            f.write("\t".join(str(e.get(c, "")) for c in cols) + "\n")
        for stem, spelling in unstaged:
            f.write("\t".join(["march-unstaged", "P0-restore", "", "", stem, spelling, "", os.path.splitext(spelling)[1].lstrip(".").lower(),
                               "live entry with no file in kom_contents"]) + "\n")
    for e in entries:
        if isinstance(e["note"], list):
            e["note"] = ";".join(e["note"])

    # summary: bucket x ext
    counts = collections.Counter(e["bucket"] for e in entries)
    counts["march-unstaged"] += len(unstaged) - sum(1 for e in entries if e["bucket"] == "march-unstaged")
    by_type = collections.defaultdict(collections.Counter)
    bytes_by = collections.Counter()
    for e in entries:
        by_type[e["bucket"]][e["ext"]] += 1
        bytes_by[e["bucket"]] += e["size"]
    with open(os.path.join(od, "p0_summary.tsv"), "w", encoding="utf-8", newline="\n") as f:
        f.write("bucket\text\tfiles\traw_bytes\n")
        agg = collections.defaultdict(lambda: [0, 0])
        for e in entries:
            agg[(e["bucket"], e["ext"])][0] += 1
            agg[(e["bucket"], e["ext"])][1] += e["size"]
        for (b, ext), (n, sz) in sorted(agg.items()):
            f.write("%s\t%s\t%d\t%d\n" % (b, ext, n, sz))

    # waves per kom
    wv = collections.defaultdict(lambda: collections.Counter())
    for e in entries:
        if e["bucket"] in ("june-only",):
            k = e["june_kom"]
            wv[k]["june_only"] += 1
            wv[k]["june_only_bytes"] += e["size"]
        elif e["bucket"] == "differs":
            k = e["march_kom"].split(",")[0]
            wv[k]["differs"] += 1
            wv[k]["differs_bytes"] += e["size"]
        elif e["bucket"] in ("same", "held", "lua"):
            wv[e["june_kom"]][e["bucket"]] += 1
    with open(os.path.join(od, "p0_waves.tsv"), "w", encoding="utf-8", newline="\n") as f:
        f.write("kom\tlive_archive\twave\tjune_only\tjune_only_bytes\tdiffers\tdiffers_bytes\tsame\theld\tlua\n")
        for k in sorted(wv):
            c = wv[k]
            live_a = "yes" if k in live_koms else "no"
            wave = "P2" if live_a == "no" else wave_of(k)
            f.write("%s\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n" % (k, live_a, wave, c["june_only"], c["june_only_bytes"], c["differs"],
                                                                   c["differs_bytes"], c["same"], c["held"], c["lua"]))

    # ---- delta explanation
    cutoff = datetime.datetime.strptime(a.cutoff, "%Y-%m-%d").timestamp()
    replaced = set()
    if os.path.isdir(STAGE_BACKUP):
        for stamp in os.listdir(STAGE_BACKUP):
            sd = os.path.join(STAGE_BACKUP, stamp)
            if os.path.isdir(sd):
                for sub in os.listdir(sd):
                    ssd = os.path.join(sd, sub)
                    if os.path.isdir(ssd):
                        replaced.update(fn.lower() for fn in os.listdir(ssd))
    plan_names = {}
    for pat_dir in (HERE, os.path.join(TRUNK, "june_item_kim")):
        for fn in sorted(os.listdir(pat_dir)):
            if fn.lower().endswith(".tsv") and re.search(r"stage|staged|need|replace|pending", fn, re.I):
                try:
                    with open(os.path.join(pat_dir, fn), "r", encoding="utf-8", errors="replace") as f:
                        head = f.readline().rstrip("\n").split("\t")
                        if "name" not in head:
                            continue
                        ni = head.index("name")
                        for line in f:
                            c = line.rstrip("\n").split("\t")
                            if len(c) > ni:
                                plan_names.setdefault(c[ni].lower(), fn)
                except OSError:
                    pass
    delta = []
    since_new = since_repl = 0
    attrib = collections.Counter()
    for e in entries:
        if e["bucket"] != "same" or not e.get("_m"):
            continue
        try:
            mt = os.path.getmtime(e["_m"])
        except OSError:
            continue
        if mt < cutoff:
            continue
        was = "differs" if e["name"].lower() in replaced else "june-only"
        src = plan_names.get(e["name"].lower(), "(no stage-plan tsv names it)")
        if was == "differs":
            since_repl += 1
        else:
            since_new += 1
        attrib[(was, src)] += 1
        delta.append((e["name"], e["march_kom"], was, src, datetime.datetime.fromtimestamp(mt).strftime("%Y-%m-%d %H:%M")))
    os.makedirs(os.path.join(TRUNK, ".build"), exist_ok=True)
    with open(os.path.join(TRUNK, ".build", "p0_delta.tsv"), "w", encoding="utf-8", newline="\n") as f:   # per-file evidence, not committed
        f.write("name\tmarch_kom\twas_at_baseline\tnamed_by\tstaged\n")
        for r in sorted(delta):
            f.write("\t".join(r) + "\n")
    with open(os.path.join(od, "p0_delta_summary.tsv"), "w", encoding="utf-8", newline="\n") as f:
        f.write("was_at_baseline\tnamed_by\tfiles\n")
        for (was, src), n in sorted(attrib.items(), key=lambda kv: -kv[1]):
            f.write("%s\t%s\t%d\n" % (was, src, n))

    # ---- report
    total = len(entries)
    p("")
    p("CENSUS %d June entries (%.0fs)" % (total, time.time() - t0))
    p("%-16s %9s %9s   %s" % ("bucket", "today", "2026-10-08", "raw bytes today"))
    for b in ("june-only", "same", "differs", "held", "march-unstaged", "lua"):
        p("%-16s %9d %9s   %.2f GB" % (b, counts.get(b, 0), BASELINE.get(b, "-"), bytes_by.get(b, 0) / 1e9))
    p("%-16s %9d %9d" % ("total", total, BASELINE["total"]))
    p("")
    p("baseline estimate from today's data (files staged after %s): same-since-new %d, same-since-replaced %d" % (a.cutoff, since_new, since_repl))
    p("  june-only baseline ~ %d (recorded %d); differs baseline ~ %d (recorded %d, incl. 34 txt, lua excluded)" % (
        counts.get("june-only", 0) + since_new, BASELINE["june-only"], counts.get("differs", 0) + counts.get("held", 0) + since_repl, BASELINE["differs"]))
    p("  by plan file that names them (p0_delta_summary.tsv):")
    for (was, src), n in attrib.most_common(14):
        p("    %6d %-9s %s" % (n, was, src))
    p("")
    p("by wave (route) of the actionable rows:")
    rc = collections.Counter(e["route"] for e in entries if e["bucket"] not in ("same",))
    for r, n in sorted(rc.items()):
        p("  %-14s %7d" % (r, n))
    new_arch = sorted(k for k in wv if k not in live_koms and (wv[k]["june_only"] or wv[k]["differs"]))
    p("")
    p("June kom numbers with no live archive: %d -> %s" % (len(new_arch), " ".join(new_arch)))
    p("live-only March entries with no file in kom_contents: %d" % len(unstaged))
    p("wrote p0_census.tsv (%s rows), p0_waves.tsv, p0_summary.tsv, p0_delta_summary.tsv in %s (per-file delta: .build/p0_delta.tsv)" % ("all" if a.all else "non-same", od))


if __name__ == "__main__":
    multiprocessing.freeze_support()
    main()
