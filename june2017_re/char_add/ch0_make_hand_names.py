r"""char-add CH0: write ch0_hand_names.tsv (fn, proposed_name, why) for every class-map row still `new?`. Copy of char_elesis/ch0_make_hand_names.py.

    NO_HAND_NAMES=1 python june2017_re/char_add/ch0_build_class_map.py ; python june2017_re/char_add/ch0_make_hand_names.py ; python june2017_re/char_add/ch0_build_class_map.py

A name is proposed only where it is settled by the class itself: the constructor and destructor bodies (inside) and the coordinator's names for the DP / formation /
hyper / gauge overrides (inside by funcmap-free name, March has no declaration: they stay `new` with the given name). Every other row stays `?` with a hint (callers, vslot,
base function) for the transcribing phase to name by the class pattern (no `June`, no address, no phase tag in a name).
"""
import os, re
HERE = os.path.dirname(os.path.abspath(__file__))
rows = {}
for l in open(os.path.join(HERE, 'ch0_class_map.tsv'), encoding='utf-8').read().splitlines()[1:]:
    p = l.split('\t')
    if re.fullmatch(r'[0-9a-f]{8}', p[0]):
        rows[p[0]] = p
SPECIAL = {
    '32a22170': ('CX2GUAdd', 'inside: X2GUAdd.h constructor (vtable stores 32a221d6; its only caller 32a24539 is inside the unit factory)'),
    '32a23b90': ('~CX2GUAdd', 'inside: X2GUAdd.h:511 destructor body (vtable store 32a23bbc; the scalar deleting dtor 32a316c0 calls it)'),
}
# coordinator-given names (Opus, 2026-11-13) for the DP / formation / hyper / gauge virtuals and non-override helpers: March does not declare them, so `new`
NAMED = {
    '329e5b00': ('UpNowDPValue', 'coordinator: vslot 0x2a4'), '329e31e0': ('ChangeMaxCharacterAddResource', 'coordinator: vslot 0x324 (X2GameUnit.h virtual)'),
    '32a31550': ('CanHyperMode', 'coordinator: vslot 0x560'), '329e21d0': ('SetDPValue', 'coordinator: vslot 0x6c4'), '329e2340': ('GetIsFormationMode', 'coordinator: vslot 0x6c8'),
    '329e2360': ('SetIsFormationMode', 'coordinator: vslot 0x6cc'), '329e2130': ('SetMutationCount', 'coordinator: vslot 0x6f4'),
    '329e2f80': ('StoreGageSnapshot', 'coordinator: vslot 0x740'), '329e3000': ('RestoreGageSnapshot', 'coordinator: vslot 0x744'),
    '329e2170': ('GetMaxDPValue', 'coordinator: non-override helper'), '329e20e0': ('GetMutationCount', 'coordinator: non-override helper'),
    '329e3060': ('?', 'coordinator: DP add + clamp helper (name by the class pattern, e.g. AddDPValue)'),
    '329f63a0': ('SetNasodArmorAttackTarget', 'brief: non-override helper'), '329e2660': ('PlayLoopSound', 'brief: non-override helper'),
}
out = ['fn\tproposed_name\twhy']
for fn, p in sorted(rows.items()):
    if p[4] != 'new?':
        continue
    if fn in SPECIAL:
        out.append('%s\t%s\t%s' % ((fn,) + SPECIAL[fn]))
        continue
    if p[1].startswith('override'):
        out.append('%s\t?\tvirtual %s (base %s); not named by any sibling class map, funcmap or X2GUUser.h; read the body' % (fn, p[1].split()[1], p[7].replace('base ', '')))
        continue
    callers = re.findall(r'[0-9a-f]{8}', p[2])
    names = []
    for c in callers[:4]:
        r = rows.get(c)
        names.append('%s(%s)' % (c, (r[2].split(' / ')[0][:28] if r and r[1].startswith(('override', 'state-fn', 'registrar')) else r[1] if r else '?')))
    out.append('%s\t?\t%s%s' % (fn, p[2] if not callers else 'called by ' + ' '.join(names), ('; ' + p[7]) if p[7] and 'phase unsettled' not in p[7] else ''))
# char-elesis CH0: merge ch0_new_names.tsv (june_fn, phase, proposed_name, confidence, evidence) from the naming pass: a row with a name (not `?`) becomes the
# proposed name; an evidence starting `MARCH HAS` becomes an `inside:` row (March owns the function); a `?` row only gains the description. SPECIAL rows win.
NN = os.path.join(HERE, 'ch0_new_names.tsv')
if os.path.exists(NN):
    nn = {}
    for l in open(NN, encoding='utf-8').read().splitlines()[1:]:
        f = (l.split('\t') + ['', '', '', '', ''])[:5]
        nn[f[0]] = f
    merged = 0
    for i, l in enumerate(out):
        f = l.split('\t')
        if i == 0 or f[0] in SPECIAL or f[0] not in nn:
            continue
        _, _, name, conf, ev = nn[f[0]]
        if name != '?':
            why = ('inside: ' + ev) if ev.startswith('MARCH HAS') else 'named (%s): %s' % (conf, ev)
            out[i] = '%s\t%s\t%s' % (f[0], name, why)
        else:
            out[i] = '%s\t?\t%s; read: %s' % (f[0], f[2], ev)
        merged += 1
    print(merged, 'names merged from ch0_new_names.tsv')
open(os.path.join(HERE, 'ch0_hand_names.tsv'), 'w', encoding='utf-8', newline='\n').write('\n'.join(out) + '\n')
print(len(out) - 1, 'rows')
