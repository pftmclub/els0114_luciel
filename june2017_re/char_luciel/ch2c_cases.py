"""Annotated listing of one skill case of CX2GULuciel's setter ( June 32b46580 ), char-luciel CH2c.

    python june2017_re/char_luciel/ch2c_cases.py <start_va_hex> [<end_va_hex>]      # run from Trunk

Prints the capstone listing from ghidra/disasm_32b46580_SetEquippedSkillFunc_capstone.txt starting at <start_va> ( an entry_va of
ch1n_skill_cases.tsv, or a few instructions before it ) until the case's closing jump into the tail ( 32b4a7d0 and later ), or <end_va>.
Annotations:
  * `mov eax, 0x32b4XXXX`  -> the callback the thunk jumps to ( ch0_class_map.tsv name, else the vtable name ), marked with its phase;
  * `mov [edi + 0xNN], ...` / `[ebp + 0xNN]` -> the state-struct slot ( edi = the normal slot state, ebp = the hyper one ):
    0x14 / 0x18 StateInit, 0x1c / 0x20 StateStartFuture, 0x24 / 0x28 StateStart, 0x2c / 0x30 OnFrameMoveFuture, 0x34 / 0x38 OnFrameMove,
    0x44 / 0x48 OnEventProcess, 0x4c / 0x50 StateEndFuture, 0x54 / 0x58 StateEnd ( the odd offset is the object, always `this` );
  * `call 0x329cc2c0` right after the hyper's table reference = CX2GUUser::CopyStateCallbacks( hyper, normal ) ( all nine );
  * `byte ptr [..+0x35]` / `[+0x11]` stores are the skill-state byte ( UserUnitStateData::m_bSkillState ).
CH2c wrote Soul Extortion's case with it and left the other six ( Soul Shock, Demonic Breath, Fantasie Impromptu, Death Crisis of Marbas,
Soul Strike, Shadow ) to CH2c2, which owns the callbacks they still lack.
"""
import re
import sys

D = 'june2017_re/char_luciel/ghidra/disasm_32b46580_SetEquippedSkillFunc_capstone.txt'
SLOT = {0x14: 'Init obj', 0x18: 'StateInit', 0x1c: 'StartFuture obj', 0x20: 'StateStartFuture', 0x24: 'Start obj', 0x28: 'StateStart',
        0x2c: 'FrameMoveFuture obj', 0x30: 'OnFrameMoveFuture', 0x34: 'FrameMove obj', 0x38: 'OnFrameMove', 0x44: 'EP obj',
        0x48: 'OnEventProcess', 0x4c: 'EndFuture obj', 0x50: 'StateEndFuture', 0x54: 'End obj', 0x58: 'StateEnd', 0x10: 'flags'}

rows = [l.rstrip('\n').split('\t') for l in open('june2017_re/char_luciel/ch0_class_map.tsv', encoding='utf8')][1:]
thunk = {}
for r in rows:
    if len(r) > 6 and r[1] == 'thunk':
        m = re.search(r'jmp ([0-9a-f]{8})', r[6])
        if m:
            thunk[r[0]] = m.group(1)
name = {r[0]: (r[3], r[5]) for r in rows if len(r) > 5 and r[1] != 'thunk'}
vt = {}
for l in open('june2017_re/char_luciel/ch0_vtable_all.tsv', encoding='utf8'):
    c = l.rstrip('\n').split('\t')
    if len(c) > 5 and c[0].startswith('0x'):
        vt[c[2]] = c[5]


def fn_name(va):
    target = thunk.get(va, va)
    if target in name:
        return '%s ( %s, thunk %s )' % (name[target][0], name[target][1], va)
    return '%s ( thunk %s )' % (vt.get(target, '?'), va) if va in thunk else '?'


start = int(sys.argv[1], 16)
end = int(sys.argv[2], 16) if len(sys.argv) > 2 else None
seen_call = False
for l in open(D, encoding='utf8').read().splitlines():
    m = re.match(r'([0-9a-f]{8}): (.*)', l)
    a = int(m.group(1), 16)
    if a < start or (end is not None and a > end):
        continue
    ins = m.group(2)
    note = ''
    mm = re.match(r'mov eax, 0x(32[0-9a-f]{6})$', ins)
    if mm:
        note = '= ' + fn_name(mm.group(1))
    mm = re.match(r'mov (dword|byte) ptr \[(edi|ebp) \+ (0x[0-9a-f]+)\], (.*)$', ins)
    if mm:
        note = (note + ' ' if note else '') + '[%s] %s' % (mm.group(2), SLOT.get(int(mm.group(3), 16), mm.group(3)))
    mm = re.match(r'push 0x(334[0-9a-f]{5})$', ins)
    if mm:
        note = 'string / table name at ' + mm.group(1)
    print('%08x %-62s %s' % (a, ins[:62], note))
    jm = re.match(r'jmp 0x(32[0-9a-f]{6})$', ins)
    if end is None and jm and int(jm.group(1), 16) >= 0x32b4a7d0:
        break
