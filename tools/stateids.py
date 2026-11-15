"""CX2GUUser::USER_STATE_ID vs June's Enum.lua table vs the staged data036 Enum.lua (plan june-state-ids, S1).

    python tools/stateids.py check [--write-tsv]

Names come from the C++ enum text in X2Lib/X2GUUser.h (comments and preprocessor lines stripped), June's table from
june_src_fixed/data036_kom/Enum.src.lua, the staged table from <kom_contents>/data036_kom/Enum.lua (USER_STATE_ID block, both are
`NAME = GAME_UNIT_STATE_ID.GUSI_END + n` rows). Each C++ value is the compiler's: june2017_re/enumprobe.py resolves
`CX2GUUser::NAME` with every flag in the enum as it is really set. Prints one row per mismatch and `RESULT: PASS` / `FAIL`.
`--write-tsv` rewrites tools/data/june_user_state_ids.tsv (name, june_value, cpp_value, live_enum_lua_value, note).
Reused by the Rose and Ain ports. Never launches the game.
"""
import os, re, subprocess, sys, tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
TRUNK = os.path.normpath(os.path.join(HERE, '..'))
HEADER = os.path.join(TRUNK, 'X2Lib', 'X2GUUser.h')
JUNE_LUA = os.path.join(TRUNK, 'june_src_fixed', 'data036_kom', 'Enum.src.lua')
TSV = os.path.join(HERE, 'data', 'june_user_state_ids.tsv')
KOM_CONTENTS = os.environ.get('X2_KOM_CONTENTS', r'F:\elsword stuff\elsword_2014\els_2014\237311\24965799\kom_contents')
LIVE_LUA = os.path.join(KOM_CONTENTS, 'data036_kom', 'Enum.lua')
GUSI_END = 5            # GAME_UNIT_STATE_ID.GUSI_END in both Enum tables


def cpp_names():
    t = open(HEADER, 'rb').read().decode('cp949', 'replace')
    s = t.index('enum USER_STATE_ID')
    body = t[t.index('{', s) + 1:t.index('};', s)]
    body = re.sub(r'/\*.*?\*/', '', body, flags=re.S)
    out = []
    for line in body.split('\n'):
        line = line.split('//')[0].strip()
        if not line or line.startswith('#'):
            continue
        for m in re.finditer(r'([A-Za-z_][A-Za-z0-9_]*)\s*(?:=[^,]*)?(?:,|$)', line):
            out.append(m.group(1))
    return list(dict.fromkeys(out))


def lua_table(path):
    t = open(path, 'rb').read().decode('utf-8-sig', 'replace')
    s = t.index('USER_STATE_ID = {')
    body = t[t.index('{', s) + 1:t.index('\n}', s)]
    rows = {}
    for line in body.split('\n'):
        line = line.split('--')[0].strip().rstrip(',')
        m = re.match(r'([A-Za-z_][A-Za-z0-9_]*)\s*=\s*GAME_UNIT_STATE_ID\.GUSI_END\s*\+\s*(\d+)$', line)
        if m:
            rows[m.group(1)] = GUSI_END + int(m.group(2))
        elif line:
            raise SystemExit('unparsed row in %s: %r' % (path, line))
    return rows


def probe(names):
    """enumprobe prints one C2440 per name and cl stops at 100 errors, so probe in chunks."""
    vals = {}
    items = os.path.join(tempfile.gettempdir(), 'stateids_items.txt')
    for i in range(0, len(names), 60):
        open(items, 'w').write('\n'.join('CX2GUUser::' + n for n in names[i:i + 60]) + '\n')
        r = subprocess.run([sys.executable, 'enumprobe.py', items], cwd=os.path.join(TRUNK, 'june2017_re'), capture_output=True)
        for line in r.stdout.decode('latin1').split('\n'):
            m = re.match(r'CX2GUUser::(\S+)\s+(\d+) \(0x', line)
            if m:
                vals[m.group(1)] = int(m.group(2))
    os.remove(items)
    return vals


def main():
    if len(sys.argv) < 2 or sys.argv[1] != 'check':
        raise SystemExit(__doc__)
    names = cpp_names()
    cpp = probe(names)
    june = lua_table(JUNE_LUA)
    live = lua_table(LIVE_LUA)
    allnames = list(names) + [n for n in june if n not in names] + [n for n in live if n not in names and n not in june]
    bad = 0
    rows = []
    for n in allnames:
        c, j, l = cpp.get(n), june.get(n), live.get(n)
        if c is None and n in names:
            note = 'C++ name did not resolve (enumprobe)'
        elif c is None:
            note = 'not in the C++ enum'
        elif j is None:
            note = 'not in June'
        elif c != j:
            note = 'C++ != June (%+d)' % (c - j)
        elif l != j:
            note = 'Enum.lua != June'
        else:
            note = ''
        if note:
            bad += 1
            print('MISMATCH %-44s cpp=%s june=%s enum.lua=%s  %s' % (n, c, j, l, note))
        rows.append((n, j, c, l, note))
    same = sum(1 for n in allnames if cpp.get(n) is not None and cpp.get(n) == june.get(n))
    print('%d / %d C++ = June; %d June names, %d staged Enum.lua names, %d C++ names' % (same, len(allnames), len(june), len(live), len(names)))
    if '--write-tsv' in sys.argv:
        rows.sort(key=lambda r: (r[1] if r[1] is not None else 10 ** 6, r[0]))
        with open(TSV, 'w', newline='\n') as f:
            f.write('name\tjune_value\tcpp_value\tlive_enum_lua_value\tnote\n')
            for n, j, c, l, note in rows:
                f.write('%s\t%s\t%s\t%s\t%s\n' % (n, '' if j is None else j, '' if c is None else c, '' if l is None else l, note))
        print('wrote', TSV)
    print('RESULT: %s' % ('PASS' if bad == 0 else 'FAIL'))
    return 0 if bad == 0 else 1


if __name__ == '__main__':
    sys.exit(main())
