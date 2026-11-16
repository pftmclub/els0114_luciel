#!/usr/bin/env python3
"""statcalc_check.py - check the [JUNE_STATCALC] per-hit lines of a play-test against the damage formulas (plan june-stat-calc, D6).

    python tools/statcalc_check.py --units 204 205                      # the live offline_server.log
    python tools/statcalc_check.py --log saved.log --units 204 205      # a saved copy (the game rewrites the log per launch)
    python tools/statcalc_check.py --units 204 205 --socket 204 205     # also compare socketed 204 against plain 205

Rules checked (every line that has the unit as attacker or defender):
  RATE    HIT rate = defence / ( dlv * 39.35f + 258.65f + defence ) to 4 decimals (June 32284aa0, D1)
  CHAIN   GFD2 base = motion * drate (+-2); GFD2 dmg = HIT dmg (D2)
  ZERO    dmg 0 only with motion 0, or state=0 / recv=0 on its GFD2 (the defender's state takes no damage) (D2b)
  REACT   HP before - after = REACT dmg (+-1, or after = 0) (D4a / D4b)
  SOCKET  with --socket S N: the socketed unit's GFD patk (or matk) out is above the plain one's for the same skill (D2a)
Exit 1 when a rule fails. Lines are paired by their position: HIT, GFD, GFD2, REACT of one hit are printed back to back."""
import argparse, os, re, statistics, sys

DEFAULT = r"F:\elsword stuff\elsword_2014\els_2014\237311\24965799\data\offline_server.log"
KV = re.compile(r"(\w+)=(\S+)")
F32 = lambda x: __import__("struct").unpack("f", __import__("struct").pack("f", x))[0]
CURVE_K, CURVE_C = float(F32(39.35)), float(F32(258.65))


def num(s):
    m = re.match(r"-?\d+(?:\.\d+)?(?:e[-+]?\d+)?", s)
    return float(m.group(0)) if m else float("nan")


def parse(path):
    hits, cur = [], None
    for line in open(path, encoding="utf-8-sig", errors="replace"):
        m = re.search(r"\[(\d\d:\d\d:\d\d\.\d+)\] \[JUNE_STATCALC\] (HIT|GFD2|GFD|REACT) (.*)", line)
        if not m:
            continue
        t, kind, rest = m.groups()
        d = {k: v for k, v in KV.findall(rest)}
        if kind == "HIT":
            cur = {"t": t, "HIT": d, "line": line.strip()}
            hits.append(cur)
        elif cur is not None and cur["HIT"].get("att") == d.get("att") and cur["HIT"].get("def") == d.get("def") and kind not in cur:
            cur[kind] = d
    return hits


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--log", default=DEFAULT)
    ap.add_argument("--units", nargs="+", required=True, help="unit uids: hits where one is the attacker or the defender")
    ap.add_argument("--socket", nargs=2, metavar=("SOCKETED", "PLAIN"))
    ap.add_argument("--socket-min", type=float, default=100, help="least median difference the sockets must add (default 100)")
    ap.add_argument("--after", default="", help="only lines at or after HH:MM:SS")
    a = ap.parse_args()
    units = set(a.units)
    hits = [h for h in parse(a.log) if (h["HIT"]["att"] in units or h["HIT"]["def"] in units) and h["t"] >= a.after]
    fails, n = [], dict(rate=0, chain=0, zero=0, react=0)

    def fail(rule, h, why):
        fails.append("%s %s: %s\n    %s" % (rule, h["t"], why, h["line"][:200]))

    for h in hits:
        H, G, G2, R = h["HIT"], h.get("GFD"), h.get("GFD2"), h.get("REACT")
        dlv, defn, rate, dmg = num(H["dlv"]), num(H["defence"]), num(H["rate"]), num(H["dmg"])
        want = defn / (dlv * CURVE_K + CURVE_C + defn)
        n["rate"] += 1
        if abs(want - rate) > 6e-5:
            fail("RATE", h, "rate %.4f, formula %.4f (dlv %d defence %.0f)" % (rate, want, dlv, defn))
        if G2 is None:
            fail("CHAIN", h, "no GFD2 line for this hit")
            continue
        n["chain"] += 1
        motion, drate, base = num(G2["motion"]), num(G2["drate"]), num(G2["base"])
        if abs(base - motion * drate) > motion * 6e-5 + 2:
            fail("CHAIN", h, "base %.0f != motion %.0f x drate %.4f = %.1f" % (base, motion, drate, motion * drate))
        if abs(num(G2["dmg"]) - dmg) > 0.5:
            fail("CHAIN", h, "GFD2 dmg %s != HIT dmg %s" % (G2["dmg"], H["dmg"]))
        if dmg <= 0 and motion != 0:
            if num(G2["state"]) == 0 or num(G2["recv"]) == 0:
                n["zero_state"] = n.get("zero_state", 0) + 1	# the defender's own state takes no damage (invincible skill / dash state)
            else:
                n["zero"] += 1
                fail("ZERO", h, "dmg 0 with motion %.0f" % motion)
        if R is not None and "hp" in R:
            n["react"] += 1
            b, _, af = R["hp"].partition(">")
            before, after, rd = num(b), num(af), num(R["dmg"])
            if not (abs((before - after) - rd) <= 1 or after == 0):
                fail("REACT", h, "hp %s falls by %.0f, dmg %.0f" % (R["hp"], before - after, rd))

    print("hits read: %d (units %s)   checked: rate %d, chain %d, react %d; %d zero-damage hit(s) on a no-damage defender state" % (len(hits), " ".join(sorted(units)), n["rate"], n["chain"], n["react"], n.get("zero_state", 0)))
    # per attacker / defender summary
    summ = {}
    for h in hits:
        H = h["HIT"]
        s = summ.setdefault((H["att"], H["def"]), [0, 1e18, 0, 0, set()])
        s[0] += 1
        s[1] = min(s[1], num(H["dmg"]))
        s[2] = max(s[2], num(H["dmg"]))
        s[3] += (num(H["dmg"]) <= 0)
        s[4].add("%s/%s" % (H["dlv"], H["rate"]))
    print("%-6s %-6s %5s %9s %9s %5s  dlv/rate" % ("att", "def", "hits", "min dmg", "max dmg", "zero"))
    for (att, dfn), s in sorted(summ.items()):
        print("%-6s %-6s %5d %9.0f %9.0f %5d  %s" % (att, dfn, s[0], s[1], s[2], s[3], " ".join(sorted(s[4]))[:60]))
    if a.socket:
        S, P = a.socket
        out = {S: {}, P: {}}
        for h in hits:
            G = h.get("GFD")
            if G is not None and h["HIT"]["att"] in out:
                key = "patk" if h["HIT"]["dt"] == "0" else "matk"
                out[h["HIT"]["att"]].setdefault(key, []).append(num(G[key].split(">")[1]))
        for key in ("patk", "matk"):
            if len(out[S].get(key, [])) >= 10 and len(out[P].get(key, [])) >= 10:
                ms, mp = statistics.median(out[S][key]), statistics.median(out[P][key])
                print("SOCKET %s out: socketed %s median %.0f (%d hits), plain %s median %.0f (%d hits), difference %.0f" % (key, S, ms, len(out[S][key]), P, mp, len(out[P][key]), ms - mp))
                if ms - mp < a.socket_min:
                    fails.append("SOCKET %s: socketed median %.0f is not %.0f above plain %.0f" % (key, ms, a.socket_min, mp))
    for f in fails[:40]:
        print("FAIL", f)
    print("RESULT: %s (%d failure(s)%s)" % ("PASS" if not fails else "FAIL", len(fails), ", first 40 shown" if len(fails) > 40 else ""))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
