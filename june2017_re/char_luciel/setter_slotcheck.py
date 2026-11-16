"""Compare a Lu/Ciel skill setter case's callback slots with June's listing (char-luciel CH3c2 review).

    python june2017_re/char_luciel/setter_slotcheck.py <dir with adis/setter.txt> "RNSI_SA_LDN_DESPERADO:,RNSI_SA_LDN_ABYSS_SNIPING:"

adis/setter.txt = `python june2017_re/charport_adis.py luciel 32b46580 > DIR/adis/setter.txt`. A June store `mov [esp + X], reg` of a
callback constant is slot X - pushes - 0x1c of the stack UserUnitStateData ( +0x18 Init, +0x20 StartFuture, +0x28 Start, +0x30 FrameMoveFuture,
+0x38 FrameMove, +0x48 EventProcess, +0x50 StateEndFuture, +0x58 StateEnd ); [edi + X] is the normal state, [ebp + X] the hyper one ( a copy from
[edi + X] prints `copy` ). The C++ side is each case's SET_CB_FUNC line with its `// June <thunk>` comment, in order. Only order differences and
the trailing `----` are expected. Cases whose callbacks come from cdecl delegate makers ( `call 0x32b4xxxx` returning { this, fn } ) are not decoded:
read those by hand.
"""
import re,sys
S=sys.argv[1]
L=open(S+'/adis/setter.txt',encoding='utf-8',errors='replace').read().split('\n')
SLOT={0x18:'StateInit',0x20:'StateStartFuture',0x28:'StateStart',0x30:'OnFrameMoveFuture',0x38:'OnFrameMove',0x40:'OnCameraMove',0x48:'OnEventProcess',0x50:'StateEndFuture',0x58:'StateEnd'}
RESET={'0x31fd89b0','0x320775a0','0x32078bc0','0x32fe9a40'}
ins=[]
for l in L:
    m=re.match(r'([0-9a-f]{8}): (\S+)\s+(.*)$',l)
    if not m: continue
    ar=re.split(r'\s{2,}',m.group(3).strip())[0]
    ins.append((int(m.group(1),16),m.group(2),ar))
def june(lo,hi):
    out=[];depth=0;regs={};mem={}
    for a,op,ar in ins:
        if a<lo or a>=hi: continue
        if op=='push': depth+=4; continue
        if op=='call':
            if ar.split()[0] in RESET: depth=0
            if ar.split()[0]=='0x31fd89b0': out.append(('----',''))
            continue
        if op=='add' and ar.startswith('esp'): depth-=int(ar.split(',')[1],0); continue
        if op=='sub' and ar.startswith('esp'): depth+=int(ar.split(',')[1],0); continue
        if op=='mov' and ',' in ar:
            d,s=[x.strip() for x in ar.split(',',1)]
            mm=re.match(r'0x(3[0-9a-f]{7})$',s)
            if mm and d in('eax','ecx','edx'): regs[d]=mm.group(1); continue
            mm=re.match(r'dword ptr \[(edi|esi|ebx) \+ (0x[0-9a-f]+)\]$',s)
            if mm and mm.group(1)=='edi' and d in('eax','ecx','edx'): mem[d]=int(mm.group(2),16); continue
            mm=re.match(r'dword ptr \[(esp|edi|ebp) \+ (0x[0-9a-f]+)\]$',d)
            if mm:
                base=mm.group(1); off=int(mm.group(2),16)
                so=off-depth-0x1c if base=='esp' else off
                tag={'edi':'normal','ebp':'hyper','esp':'state'}[base]
                if s in regs: out.append((tag+'.'+SLOT.get(so,hex(so)),regs.pop(s)))
                elif s in mem and base=='ebp': out.append((tag+'.'+SLOT.get(so,hex(so)),'copy'));mem.pop(s)
    return [x for x in out if not x[0].endswith(('0x14','0x24','0x34','0x44','0x1c','0x2c','0x3c','0x4c','0x54'))]
C=open('X2Lib/X2GULuciel.cpp',encoding='utf-8',errors='replace').read().split('\n')
cases=[(i,int(m.group(1),16)) for i,l in enumerate(C) for m in [re.search(r'case CX2SkillTree::\w+:\s*// char-luciel \S+ June @([0-9a-f]{8})',l)] if m]
cases.sort(key=lambda x:x[1])
want=sys.argv[2].split(',')
for k,(ci,anc) in enumerate(cases):
    if not any(w in C[ci] for w in want): continue
    nxt=cases[k+1][1] if k+1<len(cases) else anc+0x2000
    J=june(anc-0x12,nxt-0x12)
    # C++ case body
    cp=[]; j=ci+1
    while j<len(C) and not re.search(r'^\t\tcase |^\t\tdefault',C[j]):
        l=C[j]
        m=re.search(r'(normalStateData|hyperStateData|stateData)\.(\w+)\s*=\s*SET_CB_FUNC\(\s*\w+,\s*\w+\s*\);\s*// June ([0-9a-f]{8})',l)
        if m: cp.append(({'normalStateData':'normal','hyperStateData':'hyper','stateData':'state'}[m.group(1)]+'.'+m.group(2),m.group(3)))
        m=re.search(r'hyperStateData\.(\w+)\s*=\s*normalStateData\.\1',l)
        if m: cp.append(('hyper.'+m.group(1),'copy'))
        if re.search(r'm_StateList\[\s*stateData\.stateID\s*\]\s*=',l): cp.append(('----',''))
        j+=1
    jj=[x for x in J]; 
    print('==',C[ci].strip()[:80], 'june',len(jj),'c++',len(cp))
    import difflib
    for d in difflib.unified_diff([a+' '+b for a,b in jj],[a+' '+b for a,b in cp],'june','c++',lineterm='',n=1): print('  ',d)
