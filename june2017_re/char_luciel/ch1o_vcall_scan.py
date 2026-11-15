import os, sys, re, capstone, csv
RE = sys.argv[1]
sys.path.insert(0, RE)
from disasm_range import load_text, BASE, TV
T = load_text(); md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
fm = {}
for row in csv.reader(open(os.path.join(RE,'funcmap.tsv'), encoding='utf-8', errors='replace'), delimiter='\t'):
    if row and len(row) > 1: fm.setdefault(row[0].lower(), row[1])
def fstart(o):
    while o > 2:
        if T[o-1] == 0xcc and T[o-2] == 0xcc: return o + BASE + TV
        o -= 1
    return 0
LU0, LU1 = 0x32ae4080, 0x32b4e76f
for disp in [int(x,16) for x in sys.argv[2:]]:
    pat = disp.to_bytes(4,'little'); i = T.find(pat); out = []
    while i >= 0:
        s = i-2  # ff 9x disp32 / ff 90+r
        for back in (2,3):
            s = i-back
            ins = list(md.disasm(T[s:s+16], s+BASE+TV, 1))
            ok = False
            if ins and ('+ '+hex(disp)+']') in ins[0].op_str and ins[0].size >= back+4 and 'esp' not in ins[0].op_str and 'ebp' not in ins[0].op_str:
                if ins[0].mnemonic in ('call','jmp'): ok = True
                elif ins[0].mnemonic == 'mov' and ins[0].op_str.startswith(('eax,','ebx,','ecx,','edx,','esi,','edi,')):
                    r = ins[0].op_str.split(',')[0]
                    nxt = list(md.disasm(T[s+ins[0].size:s+ins[0].size+40], ins[0].address+ins[0].size, 8))
                    for n in nxt:
                        if n.mnemonic in ('call','jmp') and n.op_str == r: ok = True; break
                        if n.op_str.startswith(r+','): break
            if ok:
                f = fstart(s); va = ins[0].address
                tag = 'LU' if LU0 <= va <= LU1 else ''
                out.append(f"  {va:08x} f={f:08x} {tag:2} {ins[0].mnemonic} {ins[0].op_str:28} {fm.get('%08x'%f,'?')}")
                break
        i = T.find(pat, i+1)
    print(f"vslot {hex(disp)}: {len(out)}")
    print("\n".join(out))
