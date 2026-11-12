r"""char-add CH0: the sub-phase splits (groups over ~45 functions), applied after the builders.

    python june2017_re/char_add/ch0_build_state_skill.py
    python june2017_re/char_add/ch0_build_class_map.py
    python june2017_re/char_add/ch0_apply_splits.py          # rewrites the phase column of both tsvs in place (idempotent)

Rules (Opus, CH0):
- Container helpers (template / std:: instantiations at 0x32a2c000 and up) that only one group reaches get that group's split phase:
  CH1f -> CH1f2, CH1h -> CH1h3, CH1j -> CH1j2, CH1h2 -> CH1h4.
- CH1h2 (listing-only overrides below 0x600) splits by vslot: 0x30-0xc4 (Init, ReInit, StateChange, DamageReact, Lose, the DG notify, GetHyperModeTime,
  HyperModeBuffEffectStart) stay CH1h2; 0x1d8-0x54c (AttackResultByType 0x4e0 and the rest) go to CH1h4. A helper follows its first caller's split.
- CH3c splits special actives from actives: ANSI_A_* states and RNSI_A_* skills -> CH3c2; helpers follow their first caller. (CH3a at 47 and
  CH3b at 45 stay whole: an actives split would leave 7.)
"""
import os, re

HERE = os.path.dirname(os.path.abspath(__file__))
CONTAINER_LO = 0x32a2c000
CONT = {'CH1f': 'CH1f2', 'CH1h': 'CH1h3', 'CH1j': 'CH1j2', 'CH1h2': 'CH1h4'}
ACT = {'CH3c': 'CH3c2'}
BASE = {'CH1f2': 'CH1f', 'CH1h3': 'CH1h', 'CH1j2': 'CH1j', 'CH1h4': 'CH1h2', 'CH3a2': 'CH3a', 'CH3c2': 'CH3c'}


def first_callers(note):
    m = re.search(r'called by ((?:[0-9a-f]{8}(?:\([^)]*\))? ?)+)', note)
    return re.findall(r'([0-9a-f]{8})', m.group(1)) if m else []


def split_class_map():
    p = os.path.join(HERE, 'ch0_class_map.tsv')
    lines = open(p, encoding='utf-8').read().split('\n')
    rows = [l.split('\t') for l in lines]
    body = [r for r in rows[1:] if len(r) > 6]
    for r in body:                                   # idempotent: undo an earlier run first
        r[6] = BASE.get(r[6], r[6])
    by = {r[0]: r for r in body}
    for r in body:
        ph, kind = r[6], r[1]
        if ph in ACT and kind == 'state-fn' and re.match(r'ANSI_A_', r[2]):
            r[6] = ACT[ph]
    # CH1h2 overrides by vslot (the kind column reads "override 0xNNN")
    for r in body:
        m = re.match(r'override 0x([0-9a-f]+)', r[1])
        if r[6] == 'CH1h2' and m and int(m.group(1), 16) >= 0x1d8:
            r[6] = 'CH1h4'
    changed = True
    while changed:                                    # helpers follow their first caller's split
        changed = False
        for r in body:
            if r[1] != 'helper' or r[6] not in ('CH1h2', 'CH3a', 'CH3c'):
                continue
            cs = [c for c in first_callers(r[7]) if c in by]
            if cs and by[cs[0]][6] in ('CH1h4', 'CH3a2', 'CH3c2') and BASE[by[cs[0]][6]] == r[6]:
                r[6] = by[cs[0]][6]
                changed = True
    for r in body:
        if r[1] == 'helper' and int(r[0], 16) >= CONTAINER_LO and r[6] in CONT:
            r[6] = CONT[r[6]]
    open(p, 'w', encoding='utf-8', newline='').write('\n'.join('\t'.join(r) for r in rows))
    tally = {}
    for r in body:
        if r[1] not in ('thunk', 'dead', 'not-Add') and not r[1].startswith('not-'):
            tally[r[6]] = tally.get(r[6], 0) + 1
    return tally


def split_state_skill():
    p = os.path.join(HERE, 'ch0_state_skill.tsv')
    lines = open(p, encoding='utf-8').read().split('\n')
    part = 0
    for i, l in enumerate(lines):
        if l.startswith('# part 2'):
            part = 2
            continue
        f = l.split('\t')
        if part == 0 and len(f) == 10 and f[0].startswith('ANSI_'):
            f[7] = BASE.get(f[7], f[7])
            if f[7] in ACT and f[0].startswith('ANSI_A_'):
                f[7] = ACT[f[7]]
        elif part == 2 and len(f) == 7 and f[0].isdigit():
            f[5] = BASE.get(f[5], f[5])
            if f[5] in ACT and f[1].startswith('RNSI_A_'):
                f[5] = ACT[f[5]]
        lines[i] = '\t'.join(f)
    open(p, 'w', encoding='utf-8', newline='').write('\n'.join(lines))


if __name__ == '__main__':
    split_state_skill()
    t = split_class_map()
    for k in sorted(t):
        print('%-8s %d' % (k, t[k]))
