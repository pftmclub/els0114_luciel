r"""char-add CH0: ch0_setskilllevel_blocks.tsv = the blocks of June's SetEquippedSkillLevel override 329d1730 (CX2GUAdd vslot 0x7b8), read from its LISTING.
Same shape as char_elesis/ch0_setskilllevel_blocks.tsv (Elesis 32b4e790). Each block is `cmp bl, 1 / je / cmp ebp, <skill id> / jne next`, then
`call 31fca8e0( id, 1 )` (the skill templet), then one `call [vtable + 0x7a8]( templet, state id )` per state.

    python june2017_re/funcdis.py --func 329d1730 > <scratch>/slv.txt
    python june2017_re/char_add/ch0_setskilllevel_blocks.py <scratch>/slv.txt

State names: the registration map ch0_reg.tsv (id -> table name), then ch0_skill_listing_rows.tsv; `not found` when neither has the id. Skill names:
s3_reform_ids_no_state.tsv (family X2GUAdd.cpp). writes_to_members = any store into [esi + ...] (esi = this) in the block, else `none`.
"""
import os, re, sys, collections

HERE = os.path.dirname(os.path.abspath(__file__))
T = os.path.normpath(os.path.join(HERE, '..', '..'))

names = collections.defaultdict(list)
for fn in ('ch0_reg.tsv', 'ch0_skill_listing_rows.tsv'):
    for l in open(os.path.join(HERE, fn), encoding='utf-8').read().splitlines()[1:]:
        f = l.split('\t')
        if fn == 'ch0_reg.tsv':
            sid, nm = f[2], f[3]
        else:
            sid, nm = f[2], f[3]
        if sid.isdigit() and nm and nm not in names[int(sid)] and re.match(r'^[A-Z]+_', nm):
            names[int(sid)].append(nm)
sk = {}
for l in open(os.path.join(T, 'june2017_re/s3_reform_ids_no_state.tsv'), encoding='utf-8').read().splitlines()[1:]:
    f = l.split('\t')
    if f[2] == 'X2GUAdd.cpp':
        sk[int(f[3])] = f[4]

ins = []
for l in open(sys.argv[1]).read().splitlines():
    m = re.match(r'([0-9a-f]{8}): (\S+) ?(.*)', l)
    if m:
        ins.append((int(m.group(1), 16), m.group(2), m.group(3)))
blocks = []
cur = None
for k, (a, mn, op) in enumerate(ins):
    if mn == 'cmp' and op.startswith('ebp, 0x') and k + 1 < len(ins) and ins[k + 1][1] == 'jne':
        cur = {'sid': int(op.split(',')[1], 16), 'cmp': a, 'getter': None, 'calls': [], 'states': [], 'writes': []}
        blocks.append(cur)
        pushes = []
        continue
    if cur is None:
        continue
    if mn == 'push':
        pushes.append(op.strip())
    elif mn == 'call':
        if op == '0x31fca8e0':
            cur['getter'] = a
        elif op == 'eax' and len(pushes) >= 2 and re.match(r'0x[0-9a-f]+$', pushes[-2]):
            cur['calls'].append(a)
            cur['states'].append(int(pushes[-2], 16))
        pushes = []
    elif mn == 'ret':
        break
    if mn in ('mov', 'add', 'sub', 'or', 'and', 'xor') and re.match(r'(byte |word |dword )?ptr \[esi( [+-] 0x[0-9a-f]+)?\],', op) and not op.startswith('dword ptr [esi],'):
        cur['writes'].append('%08x' % a)
rows = ['block\tskill_id\tskill_id_dec\tskill_name\tcmp_va\ttemplet_getter_call\tcalls\tstate_ids\tstate_names\twrites_to_members']
for n, b in enumerate(blocks, 1):
    rows.append('%d\t0x%x\t%d\t%s\t%08x\t%08x\t%s\t%s\t%s\t%s' % (
        n, b['sid'], b['sid'], sk.get(b['sid'], 'not found'), b['cmp'], b['getter'] or 0, ','.join('%08x' % c for c in b['calls']),
        ','.join(map(str, b['states'])), ','.join('|'.join(names[s]) if names[s] else 'not found' for s in b['states']), ','.join(b['writes']) or 'none'))
open(os.path.join(HERE, 'ch0_setskilllevel_blocks.tsv'), 'w', encoding='utf-8', newline='\n').write('\n'.join(rows) + '\n')
print(len(blocks), 'blocks;', sum(len(b['states']) for b in blocks), 'state calls')
