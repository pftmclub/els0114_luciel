"""Generate Elesis's June registrars as C++ (char-elesis CH1m): InitState part 1 32b81be0 (-> InitStateCommon) and part 2 32b864f0
(-> InitStateByUnitClass), from CH0's listing-checked ch0_reg.tsv.

    python june2017_re/char_elesis/ch1m_gen.py <out dir>   # run from Trunk

Ara's ch1m_gen.py with Elesis's names and three changes: 32b81be0 has no class test and no berserk row (id 155 is a standup state
here); 32b864f0 is one switch on the unit class ( 29 / 120, 30 / 121, 35 / 125; 8 and every other class register nothing ), with no
fall-through; neither part calls the skill-chain lookup 32d3ee60. Ids: the `June script id N` member CH1a-CH1k wrote (X2GUEl.h), else
March's own registration of the table, else the enum member of that name. Callbacks: funcmap.tsv names checked against the class
headers, else CH0's `inside` member, else CH0's shared March callback, else CH0's proposed name.
Writes InitStateCommon.inc / InitStateByUnitClass.inc / idmap.tsv (CRLF, ASCII) and prints every name it could not resolve.
"""
import collections, os, re, sys
sys.stdout.reconfigure(encoding='utf-8')
CN = 'CX2GUEL'
CPP = open('X2Lib/X2GUEL.cpp', 'rb').read().decode('cp949', 'replace').split('\n')
HTXT = open('X2Lib/X2GUEl.h', 'rb').read().decode('cp949', 'replace')
HDR = {CN: HTXT, 'CX2GUUser': open('X2Lib/X2GUUser.h', 'rb').read().decode('cp949', 'replace')}
FIELD = [('Init', 'StateInit'), ('StartFuture', 'StateStartFuture'), ('Start', 'StateStart'), ('FrameMoveFuture', 'OnFrameMoveFuture'),
         ('FrameMove', 'OnFrameMove'), ('CameraMove', 'OnCameraMove'), ('EventProcess', 'OnEventProcess'), ('EndFuture', 'StateEndFuture'),
         ('End', 'StateEnd')]
PAD = {f: f + '\t' * max(1, (28 - len('stateData.' + f) + 3) // 4) for _, f in FIELD}
LO, HI = 0x32b4e770, 0x32b90b70
# thunks that jump through a vslot ( Elesis vtable: +0x600 = 32b7f090 HyperModeFrameMove, +0x604 / +0x608 base )
VSLOT = {'32adf320': (CN, 'HyperModeFrameMove'), '329cc0e0': ('CX2GUUser', 'HyperModeEnd'), '32cf0fa0': ('CX2GUUser', 'RidingHyperModeEnd')}
LEFT_OUT = {'123': 'PvP amusement mode, no PvP offline', '124': 'PvP amusement mode, no PvP offline', '125': 'PvP amusement mode, no PvP offline',
            '126': 'PvP amusement mode, no PvP offline', '127': 'dance state, Stage 10', '128': 'dance state, Stage 10', '129': 'dance state, Stage 10',
            '93': 'riding, Stage 17', '94': 'riding, Stage 17', '95': 'riding, Stage 17', '96': 'riding, Stage 17'}
EMPTY = {'3202a7d0': 'runs in CX2GUUser::StateChange for the die states', '32002f70': 'none in March, riding Stage 17'}
BOUNDING = {'ESI_DAMAGE_BOUNDING_DOWN', 'ESI_DAMAGE_BOUNDING_LANDCRASH', 'ESI_DAMAGE_BOUNDING_UP'}
CHAIN = set()   # neither part calls 32d3ee60


fm = collections.defaultdict(list)
for l in open('june2017_re/funcmap.tsv', 'rb').read().decode('utf-8', 'replace').split('\n')[1:]:
    p = l.split('\t')
    if len(p) > 1 and re.match(r'^[0-9a-f]{8}$', p[0]):
        m = re.match(r'^(CX2GU(?:EL|User))::(\w+)', p[1].strip())
        if m:
            fm[p[0]].append((m.group(1), m.group(2)))


def declared(cls, name):
    return re.search(r'\b' + name + r'\s*\(', HDR[cls]) is not None


shared, inside, proposed = {}, {}, {}
for l in open('june2017_re/char_elesis/ch0_state_skill.tsv', encoding='utf-8').read().split('\n')[2:]:
    p = l.split('\t')
    if len(p) > 8:
        cbs = re.findall(r'(\w+)=\S+>([0-9a-f]{8})', p[4])
        for x in p[8].split(' | '):
            m = re.match(r'(\w+)=inside X2GU\w+\.cpp:\d+ (CX2GU\w+)::(\w+)$', x)
            if m:
                for slot, tg in cbs:
                    if slot == m.group(1):
                        inside.setdefault((p[0], slot, tg), (m.group(2), m.group(3)))
        for x in re.findall(r'(\w+)=(CX2GUUser)::(\w+)@\d+', p[5]):
            shared.setdefault((p[0], x[0]), (x[1], x[2]))
        if len(p) > 9:
            for x in p[9].split(' | '):
                m = re.match(r'(\w+)=(\w+)$', x.strip())
                if m and m.group(2) != '-':
                    for slot, tg in cbs:
                        if slot == m.group(1):
                            proposed.setdefault((p[0], slot, tg), (CN, m.group(2)))

bad = []


def resolve(table, slot, target):
    if target in VSLOT:
        return VSLOT[target]
    cands = []
    for c, n in fm.get(target, []):
        if declared(c, n):
            cands.append((c, n))
        elif n.startswith('June') and declared(c, n[4:]):
            cands.append((c, n[4:]))
    cands = list(dict.fromkeys(cands))
    inrange = LO <= int(target, 16) < HI
    cands = [x for x in cands if (x[0] == CN) == inrange] or cands
    if not cands:
        for d in (inside, proposed):
            v = d.get((table, slot, target))
            if v and declared(*v):
                cands = [v]
                break
    if not cands and not inrange and (table, slot) in shared and declared(*shared[(table, slot)]):
        cands = [shared[(table, slot)]]
    if len(cands) != 1:
        bad.append('%s %s %s -> %s' % (table, slot, target, cands or 'NONE'))
        return None
    return cands[0]


byjune = {}
for l in HTXT.split('\n'):
    m = re.match(r'\s*(\w+),\s*/// char-elesis CH1\w+: June script id (\d+)\b', l)
    if m:
        byjune[m.group(2)] = m.group(1)
mid = collections.defaultdict(set)
cur = None
for l in CPP:
    m = re.match(r'\s*stateData\.stateID\s*=\s*(\w+(?:\(\))?);', l)
    if m:
        cur = m.group(1)
    m = re.match(r'\s*m_LuaManager\.MakeTableReference\(\s*"(\w+)",\s*stateData\.stateID', l)
    if m and cur:
        mid[m.group(1)].add(cur)
IDMAP = collections.defaultdict(set)


def state_id(table, jid):
    m = state_id_(table, jid)
    IDMAP[jid].add(m)
    return m


# March registers the table ESI_DASH_JUMP_COMBO_SPK_XZ for two members too; June's StartFuture 32b50500 / 32b50520 = March's two
HAND = {'227': 'ESI_DASH_JUMP_COMBO_SPK_XZ', '228': 'ESI_DASH_JUMP_COMBO_SPK_XZ_REATTACK'}


def state_id_(table, jid):
    if jid in byjune:
        return byjune[jid]
    if jid in HAND:
        return HAND[jid]
    s = mid.get(table)
    if s and len(s) == 1:
        return next(iter(s))
    if s:
        bad.append('ID MULTI %s %s (June %s)' % (table, sorted(s), jid))
        return sorted(s)[0]
    if re.search(r'^\s*' + table + r'\b', HTXT, re.M) or re.search(r'^\s*' + table + r'\b', HDR['CX2GUUser'], re.M):
        return table
    bad.append('ID NONE %s (June %s)' % (table, jid))
    return table


def block(r, ind):
    t = '\t' * ind
    table, jid = r['table'], r['id']
    if jid in LEFT_OUT:
        return [t + '// June %s @%s %s ( id %s ) left out: %s' % (r['fn'], r['line'], table, jid, LEFT_OUT[jid])]
    if table in BOUNDING:
        return None
    out = [t + 'stateData.Init();']
    if jid == '23':
        out += [t + 'stateData.stateID\t\t\t= USI_HYPER_MODE;', t + 'm_LuaManager.MakeTableReference( strHyperModeTable.c_str(), stateData.stateID );']
    else:
        out += [t + 'stateData.stateID\t\t\t= %s;' % state_id(table, jid), t + 'm_LuaManager.MakeTableReference( "%s", stateData.stateID );' % table]
    for slot, f in FIELD:
        if slot not in r['cb']:
            continue
        tg = r['cb'][slot]
        if tg in EMPTY:
            out.append(t + '// June %s ( %s ): %s' % (tg, f, EMPTY[tg]))
            continue
        c = resolve(table, slot, tg)
        if c:
            out.append(t + 'stateData.%s= SET_CB_FUNC( %s, %s );' % (PAD[f], c[0], c[1]))
    if jid in CHAIN:
        out.append(t + 'stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrByStateID( stateData.stateID );')
    out.append(t + 'm_StateList[stateData.stateID] = stateData;')
    return out


def rows(fn):
    out = []
    for l in open('june2017_re/char_elesis/ch0_reg.tsv', encoding='utf-8').read().split('\n')[1:]:
        p = l.split('\t')
        if len(p) < 4 or p[0] != fn:
            continue
        out.append({'fn': fn, 'line': p[1].lstrip('~'), 'id': p[2], 'table': p[3],
                    'cb': dict((x.split('=')[0], x.split('>')[-1]) for x in (p[4] if len(p) > 4 else '').split(' ') if '=' in x)})
    return out


def emit(regs, ind, bnd):
    out = []
    for r in regs:
        b = block(r, ind)
        if b is None:
            if not bnd[0]:
                out += ['\t' * ind + '// June %s @%s, the three DAMAGE_BOUNDING states ( checklist 7l )' % (r['fn'], r['line']),
                        '\t' * ind + 'InitBoundingStates( "ESI_" );', '']
                bnd[0] = True
            continue
        out += b + ([''] if len(b) > 1 else [])
    while out and out[-1] == '':
        out.pop()
    return out


# 32b864f0's cases in decompile order: ( case labels, first line, last line, falls through ); none falls through
CASES = [(['UC_ELESIS_SABER_KNIGHT', 'UC_ELESIS_GRAND_MASTER'], 45, 138, False),
         (['UC_ELESIS_PYRO_KNIGHT', 'UC_ELESIS_BLAZING_HEART'], 139, 257, False),
         (['UC_ELESIS_DARK_KNIGHT', 'UC_ELESIS_CRIMSON_AVENGER'], 260, 402, False)]


def by_class(regs, ind):
    t = '\t' * ind
    out = [t + '// June 32b864f0 @44: one switch on the unit class ( 8 and every other class register nothing )',
           t + 'switch( eClass )', t + '{']
    bnd = [False]
    for labels, lo, hi, fall in CASES:
        for c in labels:
            out.append(t + 'case CX2Unit::%s:' % c)
        out.append(t + '\t{')
        out += emit([r for r in regs if lo <= int(r['line']) <= hi], ind + 2, bnd)
        out.append(t + '\t}' + ('' if fall else ' break;'))
        if fall:
            out.append(t + '\t// falls through, as June')
        out.append('')
    out += [t + 'default:', t + '\tbreak;', t + '}']
    got = sum(1 for r in regs for lo_, hi_ in [(c[1], c[2]) for c in CASES] if lo_ <= int(r['line']) <= hi_)
    if got != len(regs):
        bad.append('32b864f0: %d rows, %d inside the case ranges' % (len(regs), got))
    return out


def write(od, name, lines):
    open(os.path.join(od, name), 'wb').write(('\r\n'.join(lines) + '\r\n').encode('ascii'))


if __name__ == '__main__':
    od = sys.argv[1]
    write(od, 'InitStateCommon.inc', emit(rows('32b81be0'), 1, [False]))
    write(od, 'InitStateByUnitClass.inc', by_class(rows('32b864f0'), 1))
    rev = collections.defaultdict(set)
    for j, ms in IDMAP.items():
        if len(ms) > 1:
            bad.append('June id %s -> %s' % (j, sorted(ms)))
        for m in ms:
            rev[m].add(j)
    for m, js in rev.items():
        if len(js) > 1:
            bad.append('member %s <- June ids %s' % (m, sorted(js, key=int)))
    with open(os.path.join(od, 'idmap.tsv'), 'w', encoding='utf-8', newline='\n') as f:
        f.write('june_id\tmember\n')
        for j in sorted(IDMAP, key=int):
            f.write('%s\t%s\n' % (j, ' '.join(sorted(IDMAP[j]))))
    print('ids mapped', len(IDMAP))
    for b in dict.fromkeys(bad):
        print('UNRESOLVED', b)
