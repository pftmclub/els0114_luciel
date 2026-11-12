"""char-add CH0: append the `Char Add CH0` rows to june2017_re/funcmap.tsv (idempotent: skips when the tag is already there; never duplicates an address that already has a row).
    python june2017_re/char_add/ch0_funcmap_rows.py [--dry]
Rows: the CX2GUAdd vtable overrides (ch0_vtable_slots.tsv + the class map's names / phases), the InitState body 32a25a90 and its two registrars,
SetEquippedSkillFuncBySkillSlotIndex 32a28e00, SetEquippedSkillLevel 329d1730, the 4 InitPassiveSkillState delegate adapters, the CX2AddGageData
vtable, the Add gauge UI InitUI, the dying Apocalypse 329fb6c0, and the identified unmapped callees funcmap lacks (ch0_unmapped_identified.tsv).
Append only; an address already in funcmap.tsv column 1 is skipped (existing rows are never edited).
"""
import os, re, sys
HERE = os.path.dirname(os.path.abspath(__file__))
FM = os.path.join(HERE, '..', 'funcmap.tsv')
TAG = 'Char Add CH0'
C = 'CX2GUAdd::'


def tsv(name):
    p = os.path.join(HERE, name)
    return [l.rstrip('\r\n').split('\t') for l in open(p, encoding='utf-8') if l.strip() and l[0] != '#']


cm = {r[0]: r for r in tsv('ch0_class_map.tsv') if r[0] != 'june_fn'}
slots = [r for r in tsv('ch0_vtable_slots.tsv') if r[0] != 'slot_off']
ui = [r for r in tsv('ch0_unmapped_identified.tsv') if r[0] != 'callee']

# hand rows: address -> (march symbol, status, evidence)
HAND = {
 '32a25a90': (C + 'InitState (vslot 0x7a4; no wrapper, the body registers the states)', 'confirmed', 'class registrar entry: calls the registrars 32a1e4f0 (118 names incl. the hyper row id 23) and 32a24c20 (68 names) and base 3202b110 (ch0c_listing_check 194 / 194); phase CH1m'),
 '32a1e4f0': (C + 'InitState registrar 1', 'confirmed', '118 registered names (incl. the hyper row id 23); called by InitState 32a25a90; none in March as a function; phase CH1m'),
 '32a24c20': (C + 'InitState registrar 2', 'confirmed', '68 registered names; called by InitState 32a25a90; none in March as a function; phase CH1m'),
 '32a28e00': (C + 'SetEquippedSkillFuncBySkillSlotIndex (vslot 0x7b4)', 'confirmed', 'three switches: (a) type 5 at 32a28f41 12 entries, (b) ids 29000..29233 234 entries 53 targets, (c) 29601..29605 5 entries; compares 29213 / 29231 -> Crest of Time 32a28f90, 29600 -> ANSI_HA_ALP_DOOMS_DAY, 900 -> ANSI_THROW_ITEM; phase CH1n'),
 '329d1730': (C + 'SetEquippedSkillLevel (vslot 0x7b8)', 'confirmed', '18 id blocks, 32 state calls; phase CH1i'),
 '32a2b340': (C + 'InitPassiveSkillState delegate adapter (thunk table)', 'confirmed', 'push edx; call 329d4190; ret; pushed by InitPassiveSkillState 32a1ccb0 @32a1d737; delegate 329d4190; phase CH4a'),
 '32a309a0': (C + 'InitPassiveSkillState delegate adapter (thunk table)', 'confirmed', 'push edx; call 32a04ce0; ret; pushed by InitPassiveSkillState 32a1ccb0 @32a1d29d; delegate 32a04ce0; phase CH4a'),
 '32a309b0': (C + 'InitPassiveSkillState delegate adapter (thunk table)', 'confirmed', 'push edx; call 32a01d20; ret; pushed by InitPassiveSkillState 32a1ccb0 @32a1d900; delegate 32a01d20; phase CH4a'),
 '32a309c0': (C + 'InitPassiveSkillState delegate adapter (thunk table)', 'confirmed', 'push edx; call 32a01dd0; ret; pushed by InitPassiveSkillState 32a1ccb0 @32a1d91c; delegate 32a01dd0; phase CH4a'),
 '3349b1e4': ('CX2AddGageData vtable (22 slots)', 'confirmed', 'RTTI 33734c08, COL 33618d40; ctor 324989c0 (X2GageData.h:915), copy ctor 32a30040; slots 0x2c / 0x30 are the shared base gauge virtuals, the rest in 32a32470..32a35ff0; March X2GageData.h:911 CX2AddGageData; phase CH1q'),
 '32e6e0c0': ('CX2AddMyGageUI::InitUI (Add gauge UI, vtable 335335b0, no RTTI)', 'candidate', 'loads DLG_Add_DP_And_Mutation.lua (g_pStatic_DP_Gage / Mutation_Gage, FormationConsumeDPGuide); March CX2AddMyGageUI::InitUI X2GageUI.cpp:4956; June keeps the class, only its RTTI name is gone; phase CH1q'),
 '329fb6c0': (C + 'dying Apocalypse (summon end)', 'candidate', 'Eve CH2b4 review row names it as the Apocalypse summon end on a UC_ADD_MASTERMIND owner; phase CH2b'),
}


def rows():
    out = []
    seen = set()
    for a, (n, s, e) in HAND.items():
        out.append((a, n, s, '%s: %s' % (TAG, e)))
        seen.add(a)
    for r in slots:
        fn = r[2]
        if fn in seen:
            continue
        seen.add(fn)
        c = cm.get(fn)
        name, march, ph, ins, note, src = r[4], r[6], '?', '?', '', 'slots'
        if c:
            march, ph, ins = c[3], c[6], c[4]
            for k in (2, 3, 5):
                x = c[k].split(' ')[0] if k == 3 else c[k]
                if x and x[0] not in '?-':
                    name, src = x, ('class map' if k != 5 else 'proposed')
                    break
            else:
                name = None
            note = re.sub(r'^base [0-9a-f]+; ', '', c[7]) if len(c) > 7 else ''
        if not name or name[0] in '?-':
            name = None
        if march.startswith('- (name unsettled)'):
            march = 'no March name settled'
        conf = bool(name) and (r[5].startswith('CONF') or src == 'class map')
        sym = C + name if name else 'CX2GUEL vslot %s (name unsettled)' % r[0]
        ev = '%s: override %s; %s; phase %s; %s; base %s' % (TAG, r[0], march, ph, ins, r[3])
        if not conf and note:
            ev += '; ' + (note if len(note) <= 160 else note[:160].rsplit(' ', 1)[0])
        out.append((fn, sym, 'confirmed' if conf else 'candidate', ev))
    return out


def callee_rows(have):
    out = []
    for r in ui:
        if r[0] in have:
            continue
        ev = r[4] if len(r[4]) <= 400 else r[4][:400].rsplit(' ', 1)[0]
        used = r[8] if len(r) > 8 and r[8] and r[8] != '-' else (r[7] if len(r) > 7 else '')
        out.append((r[0], r[2] if r[2] != '-' else 'unidentified callee (see evidence)', r[3] if r[3] in ('confirmed', 'candidate') else 'candidate',
                    '%s: %s; used by %s' % (TAG, ev, used)))
    return out


def main():
    s = open(FM, 'rb').read()
    if TAG.encode() in s:
        print('already has %s rows' % TAG)
        return
    have = set()
    for l in s.decode('utf-8', 'replace').split('\n'):
        for t in l.split('\t')[0].split():
            have.add(t.lower())
    R = rows()
    skipped = sorted({a for a, _, _, _ in R if a.lower() in have})
    R = [x for x in R if x[0].lower() not in have]
    R += callee_rows(have | {a for a, _, _, _ in R})
    add = ''.join('%s\t%s\t%s\t%s\n' % x for x in R)
    add = add.replace('\r', '')
    if '--dry' in sys.argv:
        print(add)
        print(len(R), 'rows (skipped, address already in funcmap.tsv: %s)' % (' '.join(skipped) or 'none'))
        return
    if not s.endswith(b'\n'):
        s += b'\n'
    open(FM, 'wb').write(s + add.encode('utf-8'))
    print('funcmap.tsv: %d rows %s' % (len(R), TAG))


if __name__ == '__main__':
    main()
