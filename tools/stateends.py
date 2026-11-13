#!/usr/bin/env python3
"""stateends.py - a registered skill state that can never end (the "stuck after the skill, can't move, can't be hit" bug).

A skill state whose script plays its animation once and waits (PLAY_TYPE XAP_ONE_WAIT, the default) only leaves through its
OnEventProcess ( anim end -> WAIT ). When a class port registers the state without an OnEventProcess the player is stuck in it
for good, and an INVINCIBLE script makes him untouchable too. Seen on Elesis CH2a (Sonic Assault: Sting memo fire state: June copies
the skill state's EventProcess into it with `mov ecx,[esi+0x44]`, which the CH0 callback tables do not show) and suspected on Ara.

    python tools/stateends.py check X2Lib/X2GUEL.cpp ELESIS_KNIGHT            # one class file against its June script
    python tools/stateends.py check X2Lib/X2GUAra.cpp Ara_Martial_Artist --all   # also list the states that end by script

Reads every `MakeTableReference( "NAME", normalStateData | hyperStateData | stateData .stateID )` registration in the C++ file, collects the
`<that struct>.OnEventProcess` assignment ( or a `CopyStateCallbacks( <struct>, ... )` ) up to the next registration, and looks the state's table up in
june_src_fixed/data*_kom/<SCRIPT>.src.lua. A state with no OnEventProcess and PLAY_TYPE XAP_ONE_WAIT ( or no PLAY_TYPE ) is a STUCK hit;
exit 1 while one remains. Fix: find the June registration of that state id ( `charport_adis.py <class> <fn>`, the `UpdateUserUnitStateData` call
for the id ) and read which struct slot holds the EventProcess, including a slot copied from the skill state ( `mov ecx, [esi + 0x44]` ).
Every char-port phase that registers states runs this before the build ( x2-phase "Finish" ).
"""
import glob, os, re, sys

TRUNK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REG = re.compile(r'MakeTableReference\(\s*"([A-Za-z0-9_]+)"\s*,\s*(normalStateData|hyperStateData|stateData|subStateData)\.stateID')
ASSIGN = re.compile(r'\b(normalStateData|hyperStateData|stateData|subStateData)\.(OnEventProcess|StateInit|OnFrameMove)\b')


def script_text(name):
    for p in glob.glob(os.path.join(TRUNK, 'june_src_fixed', 'data*_kom', name + '.src.lua')):
        return open(p, 'rb').read().decode('utf-8', 'replace')
    sys.exit('no june_src_fixed script named %s' % name)


def play_type(lua, state):
    m = re.search(r'^%s\s*=\s*\{' % re.escape(state), lua, re.M)
    if not m:
        return None
    end = lua.find('\n}', m.end())
    body = lua[m.end():end if end > 0 else len(lua)]
    pt = re.search(r'PLAY_TYPE\s*=\s*XSKIN_ANIM_PLAYTYPE\.(\w+)', body)
    inv = re.search(r'INVINCIBLE\s*=', body) is not None
    return (pt.group(1) if pt else 'XAP_ONE_WAIT'), inv


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    show_all = '--all' in sys.argv
    if len(args) != 3 or args[0] != 'check':
        sys.exit(__doc__)
    path = args[1] if os.path.isabs(args[1]) else os.path.join(TRUNK, args[1])
    lines = open(path, 'rb').read().decode('cp949', 'replace').split('\n')
    lua = script_text(args[2])
    regs = [(i, m.group(1), m.group(2)) for i, l in enumerate(lines) for m in [REG.search(l)] if m]
    stuck = ended = unknown = 0
    seen = {}
    for k, (i, name, var) in enumerate(regs):
        stop = regs[k + 1][0] if k + 1 < len(regs) else len(lines)
        # a state struct block ends at its m_StateList store; a skill-slot variable ends at the next registration
        block = lines[i:stop]
        has_ep = any(re.search(r'\b%s\.OnEventProcess\s*=' % var, l) or re.search(r'CopyStateCallbacks\(\s*%s\s*,' % var, l)
                     for l in block)
        key = (name, var)
        seen[key] = seen.get(key, False) or has_ep
        seen.setdefault(('line', name, var), i + 1)
    for (key, has_ep) in sorted((k, v) for k, v in seen.items() if k[0] != 'line'):
        name, var = key
        pt = play_type(lua, name)
        line = seen[('line', name, var)]
        if pt is None:
            unknown += 1
            if show_all:
                print('  ?      %-60s %-15s line %d: no table in the script' % (name, var, line))
            continue
        if has_ep:
            ended += 1
            if show_all:
                print('  ok     %-60s %-15s line %d' % (name, var, line))
            continue
        ptype, inv = pt
        if ptype == 'XAP_ONE_WAIT':
            stuck += 1
            print('  STUCK  %-60s %-15s line %d: %s, no OnEventProcess%s' % (name, var, line, ptype, ', INVINCIBLE' if inv else ''))
        elif show_all:
            print('  script %-60s %-15s line %d: %s ends by script' % (name, var, line, ptype))
    print('stateends: %s: %d registered table(s), %d with OnEventProcess, %d STUCK, %d not in the script' % (os.path.basename(path), len(seen) // 2, ended, stuck, unknown))
    print('RESULT: %s' % ('PASS' if stuck == 0 else 'FAIL'))
    sys.exit(1 if stuck else 0)


if __name__ == '__main__':
    main()
