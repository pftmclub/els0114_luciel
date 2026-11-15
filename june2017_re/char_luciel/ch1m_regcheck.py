"""Check Lu/Ciel's InitState / InitStateCommon ( X2GULuciel.cpp ) against the listing's registrations (char-luciel CH1m).

    python june2017_re/char_luciel/ch1m_regcheck.py      # run from Trunk

Every ch0_reg.tsv row of 32b3e8b0 / 32b43d10 ( ch0c_listing_check.py proved them 364 / 364 against the listing ) must match one C++
registration in order: the same table ( or the roll / hyper std::string ), the same id value ( LUCIEL_STATE_ID, or a March USI_ name whose
value is June's ), and per slot the member the June callback resolves to. A callback resolves through the class-map thunks to a
CX2GULuciel definition ( its `// char-luciel ...: June <va>` comment ), a CX2GUUser definition or funcmap row, or the vslot / base-helper
table below. Rows of 32b43d10 after LSI_WAIT_STATE_CHANGE must sit inside the Diablura / Demonio test. Left-out ids ( phoenix, dance,
riding 93-96 ) need their one-line comment; a later-phase callback needs `body in CHxx`, a state whose every callback is a later
phase's needs `registered by CHxx` instead of a registration. 3202b110 is InitStateCommonRandom's.
"""
import collections, os, re, sys
sys.stdout.reconfigure(encoding='utf-8')


def rd(p, enc='utf-8'):
    return open(p, 'rb').read().decode(enc, 'replace').replace('\r', '').split('\n')


def defs(lines, cls):
    out = collections.defaultdict(set)
    for i, l in enumerate(lines):
        s = l.strip()
        if not s.startswith('//'):
            continue
        m = re.search(r'June ([0-9a-f]{8})', s)
        if not m:
            continue
        for j in range(i + 1, min(i + 4, len(lines))):
            d = re.match(r'^(?:/\*virtual\*/ )?[\w:<>\*& ]*?\b' + cls + r'::(\w+)\s*\(', lines[j])
            if d:
                out[m.group(1)].add(d.group(1))
                break
            t = lines[j].strip()
            if t and not t.startswith('//') and not t.startswith('#'):
                break
    return out


SRC = rd('X2Lib/X2GULuciel.cpp')
LUC = defs(SRC, 'CX2GULuciel')
USR = defs(rd('X2Lib/X2GUUser.cpp', 'cp949'), 'CX2GUUser')
FM = collections.defaultdict(set)
for l in rd('june2017_re/funcmap.tsv')[1:]:
    p = l.split('\t')
    m = len(p) > 1 and re.match(r'(CX2GUUser|CX2GameUnit)::(?:June)?(\w+)', p[1].strip())
    if m and re.match(r'^[0-9a-f]{8}', p[0]):
        FM[p[0][:8]].add(m.group(2))
CM = dict((p[0], p) for p in (l.split('\t') for l in rd('june2017_re/char_luciel/ch0_class_map.tsv')) if len(p) > 6)
# vslot thunks ( jmp [eax + slot] ) and the CX2GUUser bodies only Lu / Ciel register ( CH1m wrote them in the class )
MAN = {'329cc0b0': 'BoundingDownEventProcess', '329cc150': 'BoundingLandCrashEventProcess', '329cc1e0': 'BoundingUpStateStartFuture',
       '329cc050': 'BoundingUpStateStart', '329cc0c0': 'BoundingUpEventProcess', '32adf320': 'HyperModeFrameMove', '329cc0e0': 'HyperModeEnd',
       '32cf0fa0': 'RidingHyperModeEnd', '31fcd0f0': 'DieFrontStartFuture', '31fcd1b0': 'DieBackStartFuture', '31fcd160': 'DieFrontEventProcess',
       '31fcd220': 'DieBackEventProcess', '320008f0': 'DieFrontFrameMove', '32000910': 'DieBackFrameMove', '31fd2680': 'DashEndStartFuture'}
EMPTY = {'3202a7d0', '31fc7fc0', '32002f70'}     # die StateEnd ( StateChange ), = CommonStateStart, riding attack X frame ( Stage 17 )
LEFT = {123, 124, 125, 126, 127, 128, 129, 571, 572, 573, 574, 590, 591, 592, 93, 94, 95, 96}
SLOT = {'StateInit': 'Init', 'StateStartFuture': 'StartFuture', 'StateStart': 'Start', 'OnFrameMoveFuture': 'FrameMoveFuture',
        'OnFrameMove': 'FrameMove', 'OnCameraMove': 'CameraMove', 'OnEventProcess': 'EventProcess', 'StateEndFuture': 'EndFuture', 'StateEnd': 'End'}
USI = {'USI_START': 9, 'USI_WIN': 11, 'USI_LOSE': 12, 'USI_HYPER_MODE': 23}   # March value = June value below 24
VAL = {}
inenum = False
for l in rd('X2Lib/X2GULuciel.h'):
    if 'enum LUCIEL_STATE_ID' in l:
        inenum = True
    elif inenum and l.strip().startswith('};'):
        break
    elif inenum:
        m = re.match(r'\s*(LSI_\w+)\s*=\s*(\d+)', l)
        if m:
            VAL[m.group(1)] = int(m.group(2))
VAL.update(USI)


def chase(t):
    for _ in range(5):
        p = CM.get(t)
        m = p and p[1] == 'thunk' and re.search(r'thunk>([0-9a-f]{8})', p[6])
        if not m:
            break
        t = m.group(1)
    return t


def want(t0):
    t = chase(t0)
    if t in MAN:
        return {MAN[t]}, None
    if t in LUC:
        return LUC[t], None
    p = CM.get(t)
    if p and p[1] == 'state-fn' and re.match(r'CH[2-6]', p[5]):
        return None, (p[3], p[5], t)
    return (USR.get(t) or FM.get(t) or set()), None


def body(name):
    s0 = next(i for i, l in enumerate(SRC) if re.match(r'^(/\*virtual\*/ )?void CX2GULuciel::' + name + r'\(\)', l))
    e0 = next(i for i in range(s0 + 1, len(SRC)) if SRC[i] == '}')
    return SRC[s0:e0]


def parse(name):
    regs, cur, depth_if, notes = [], None, False, []
    for l in body(name):
        s = l.strip()
        if s.startswith('if( CX2Unit::UC_LUCIEL_DIABLURA == eUnitClass'):
            depth_if = True
        elif s == '}' and l.startswith('\t}'):
            depth_if = False
        elif s.startswith('stateData.Init()'):
            cur = {'id': None, 'table': None, 'cb': {}, 'later': [], 'empty': [], 'gated': depth_if}
        elif cur is not None and re.match(r'stateData\.stateID\s*=', s):
            cur['id'] = re.match(r'stateData\.stateID\s*=\s*(\w+);', s).group(1)
        elif cur is not None and s.startswith('m_LuaManager.MakeTableReference('):
            cur['table'] = re.match(r'm_LuaManager\.MakeTableReference\(\s*(.+?),\s*stateData\.stateID', s).group(1).strip('" ')
        elif cur is not None and re.match(r'stateData\.\w+\s*=\s*SET_CB_FUNC', s):
            m = re.match(r'stateData\.(\w+)\s*=\s*SET_CB_FUNC\(\s*(\w+),\s*(\w+)\s*\);', s)
            cur['cb'][SLOT[m.group(1)]] = m.group(3)
        elif cur is not None and re.match(r'// June [0-9a-f]{8} \( \w+ \): \w+, body in CH', s):
            m = re.match(r'// June ([0-9a-f]{8}) \( (\w+) \): (\w+), body in (CH\w+)', s)
            cur['later'].append((SLOT[m.group(2)], m.group(3), m.group(4), m.group(1)))
        elif cur is not None and re.match(r'// June [0-9a-f]{8} \( \w+ \): ', s):
            m = re.match(r'// June ([0-9a-f]{8}) \( (\w+) \)', s)
            cur['empty'].append((SLOT[m.group(2)], m.group(1)))
        elif s.startswith('m_StateList[stateData.stateID] = stateData;'):
            regs.append(cur)
            cur = None
        elif re.match(r'// June 32b3e8b0 @\d+ \w+ \( id \d+ \): registered by CH', s):
            m = re.match(r'// June 32b3e8b0 @\d+ (\w+) \( id (\d+) \): registered by ([\w /]+) with', s)
            notes.append(('deferred', int(m.group(2)), m.group(1), m.group(3)))
        elif re.match(r'// June 32b3e8b0 @\d+ \w+ \( id (\d+) \) left out', s):
            notes.append(int(re.match(r'// June 32b3e8b0 @\d+ \w+ \( id (\d+) \)', s).group(1)))
    return regs, notes


ROLL = {('32b43d10', '9'): 'strStartTableNameUTF8.c_str()', ('32b43d10', '11'): 'strWinTableNameUTF8.c_str()',
        ('32b43d10', '12'): 'strLoseTableNameUTF8.c_str()', ('32b3e8b0', '23'): 'strLuHyperModeTable.c_str()',
        ('32b3e8b0', '204'): 'strCielHyperModeTable.c_str()'}
rows = [l.split('\t') for l in rd('june2017_re/char_luciel/ch0_reg.tsv')[1:] if l.strip()]
bad = n = 0
later = collections.Counter()
for cpp, fn in (('InitStateCommon', '32b3e8b0'), ('InitState', '32b43d10')):
    C, notes = parse(cpp)
    J = [r for r in rows if r[0] == fn]
    k = 0
    gate = False
    for r in J:
        i = int(r[2])
        if fn == '32b3e8b0' and i in LEFT:
            if i not in notes:
                print('LEFT OUT WITHOUT COMMENT', i, r[3]); bad += 1
            continue
        cb0 = dict((s.split('=')[0], s.split('=')[1].split('>')[-1]) for s in r[4].split(' ') if '=' in s)
        lts = [want(t)[1] for t in cb0.values()]
        if lts and all(lts):
            phs = ' / '.join(sorted(set(x[1] for x in lts)))
            if ('deferred', i, r[3], phs) not in notes:
                print('DEFERRED WITHOUT COMMENT', i, r[3], phs); bad += 1
            for x in lts:
                later[x[1]] += 1
            continue
        if k >= len(C):
            print('MISSING', fn, r[1], i, r[3]); bad += 1; continue
        x = C[k]; k += 1; n += 1
        table = ROLL.get((fn, r[2]), r[3])
        if x['table'] != table:
            print('TABLE', fn, r[1], i, r[3], '!=', x['table']); bad += 1
        v = VAL.get(x['id'], int(x['id']) if x['id'].isdigit() else None)
        if v != i:
            print('ID', fn, r[1], i, x['id'], v); bad += 1
        if fn == '32b43d10':
            if r[3] == 'LSI_WAIT_STATE_CHANGE':
                gate = True
            elif gate != x['gated']:
                print('CLASS GATE', r[1], r[3], 'gated' if x['gated'] else 'not gated'); bad += 1
        cb = dict((s.split('=')[0], s.split('=')[1].split('>')[-1]) for s in r[4].split(' ') if '=' in s)
        seen = set()
        for slot, t in cb.items():
            seen.add(slot)
            if t in EMPTY:
                if slot in x['cb'] or (slot, t) not in x['empty']:
                    print('EMPTY SLOT', r[3], slot, t); bad += 1
                continue
            names, lt = want(t)
            if lt:
                later[lt[1]] += 1
                if (slot, lt[0], lt[1], lt[2]) not in x['later'] or slot in x['cb']:
                    print('LATER', r[3], slot, lt); bad += 1
                continue
            if x['cb'].get(slot) not in names:
                print('DIFF', r[3], slot, t, sorted(names), x['cb'].get(slot)); bad += 1
        for slot in x['cb']:
            if slot not in seen:
                print('EXTRA SLOT', r[3], slot); bad += 1
    if k != len(C):
        print('NOT IN JUNE', cpp, [c['table'] for c in C[k:]]); bad += 1
print('later-phase slots:', dict(later))
print('matched %d registrations, %d problems' % (n, bad))
sys.exit(1 if bad else 0)
