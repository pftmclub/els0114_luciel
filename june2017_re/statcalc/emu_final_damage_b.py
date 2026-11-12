"""june-stat-calc D2b: June's GetFinalDamage ranges from @32285ccf on, run in Unicorn, against the port's expressions.

June 32284aa0 (CX2GameUnit::GetFinalDamage) and its helpers; the port is X2GameUnit.cpp under SERV_IRUHADEV_JUNE_STATCALC_FINAL_DAMAGE.
  @32285e71 / @32285e86-32286037  level-damage path from the game-rate pick to the floor (GetPowerRate and pow stubbed)
  @3228619f-3228626e              final-defence / melee-range / user-NPC terms and their sum order
  @322862f6-322863b2              state defence (reduce %, guard %, cap 100) times the received-damage rate (vslot 0x2ac stubbed)
  @32286455-32286482              the NPC part rate (+0x8fd4) when above 1
  @322867e0-3228682a              atomic shield by g_pMain state (June numbering; the port matches March's states by name)
  322749cc-322749ea               GetIgnoreDefenceRate's cap (a NaN sum is kept)
  322726f0                        the HP-rate gate, whole, on a fake NPC whose vtable points at June's own 32755a60 / 32755a70 / 32755a80
  static                          the PvP channel ids, the Space of Thought ids, the minor-unit slot table and the dev log strings
C float assignments round to float; x87 intermediates are 53-bit (control word 0x027F), which Python's doubles reproduce here.

Usage: python june2017_re/statcalc/emu_final_damage_b.py      (one line per mismatch, then the summary line)
"""
import itertools
import math
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from emu_final_damage_a import Emu, f32, div, same, june_states, MARCH_STATES, TRUNK, BASE, ESP, DATA, OBJ, STUB  # noqa: E402
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EBX, UC_X86_REG_EBP, UC_X86_REG_ESI, UC_X86_REG_EDI  # noqa: E402

GMAIN_PAGE, GMAIN_PTR = 0x33775000, 0x3377585c
PORT_SHIELD_STATES = {"XS_PVP_GAME", "XS_DUNGEON_GAME", "XS_TRAINING_GAME", "XS_BATTLE_FIELD"}
SUPERSONIC = 29058                       # RNSI_SA_ALP_SUPERSONIC, June Enum.lua
D01 = float(f32(0.1))


def enc(e, base, value_bits, key=0x13579bdf):
    """KProtectedType as June decodes it: value = [base + 8] ^ [base + 4] ^ base"""
    e.wd(base + 4, key)
    e.wd(base + 8, value_bits ^ key ^ base)


def fbits(v):
    return struct.unpack("<I", struct.pack("<f", v))[0]


# ---- the port's expressions ----

def port_level(lvf, lv_def, pvp, drate, prate, user, power, memo, dtype, physic, magic, atk, m, u):
    lv_att = int(lvf)
    corr, game = 1.0, 1.0
    if pvp:
        game = prate
    else:
        game = drate
        if lv_def - lv_att > 5:
            corr = f32(float(lv_def - lv_att - 5) * D01)
            corr = f32(1.0 - corr)
            if corr < 0.0:
                corr = 0.0
    p = power if user else 1.0
    p = f32(memo * p)
    fix = 2000.0
    if dtype == 0:
        fix = f32(physic * atk)
    elif dtype == 1:
        fix = magic
    sq = f32(math.pow(lvf, 2.0))
    fin = f32((sq * 35.0 * corr * p + fix) * game)
    inc = f32(fin * (m - 1.0))
    unit = f32(f32(u - 1.0) * fin)
    fin = f32((unit + inc) + fin)
    if fin <= 1.0:
        fin = 1.0
    return fin


def port_sum(fin, wos, d, m, u):
    fin = f32(wos * fin)
    dec = f32(fin * (d - 1.0))
    inc = f32(fin * (m - 1.0))
    unit = f32(f32(u - 1.0) * fin)
    fin = f32(fin + dec)
    return f32(fin + (unit + inc))


def port_state(fin, pen, ign, reduce_pct, guard, guard_pct, recv):
    x = 0.0
    if not pen and not ign:
        if reduce_pct > 0:
            x = float(reduce_pct)
        if guard:
            x = f32(float(guard_pct) + x)
    if x > 100.0:
        x = 100.0
    rate = f32((100.0 - x) / 100.0)
    fin = f32(fin * rate)
    return f32(recv * fin)


def port_part(fin, v):
    return f32(v * fin) if 1.0 < v else fin


def port_shield(fin, r, state_name):
    if r < 1.0:
        if state_name in PORT_SHIELD_STATES:
            return f32((1.0 - r) * fin)
        return f32(r * fin)
    return fin


def port_ignore_cap_clamps(s):
    return s > 1.0


def port_gate(skill, npc, gage, name, grade, immune, main_boss):
    if skill != SUPERSONIC or not npc:
        return True
    if gage or name or grade in (1, 2):
        return False
    if immune:                                             # D3: GetImmuneToHpAttack (June vslot 0x430 32755a80)
        return False
    if main_boss:
        return False
    return grade not in (1, 2)


# ---- June ----

def setup_level(e, lvf, lv_def, drate, prate, user, memo, dtype, physic, magic, atk, melee, m, def_user, u):
    dd, att, dfn = DATA + 0x200, OBJ, OBJ + 0x10000
    e.wd(dd + 0, dtype)
    e.wf(dd + 0x30, drate); e.wf(dd + 0x538, prate); e.wf(dd + 0x5e4, memo)
    e.mu.mem_write(dd + 0x2c, bytes([1 if melee else 0]))
    e.wd(att + 0x264, 0 if user else 1)
    e.wd(att, OBJ + 0x8000)
    enc(e, att + 0x5730, fbits(m if melee else 7.0)); enc(e, att + 0x57e4, fbits(7.0 if melee else m))
    enc(e, att + 0x5e38, fbits(u if def_user else 9.0)); enc(e, att + 0x5d84, fbits(9.0 if def_user else u))
    e.wd(dfn + 0x264, 0 if def_user else 1)
    e.esp()
    e.wf(ESP + 0x24, lvf); e.wf(ESP + 0x34, 1.0); e.wd(ESP + 0x38, dfn)
    e.wf(ESP + 0x4c, physic); e.wf(ESP + 0x50, magic); e.wf(ESP + 0x20, atk)
    for reg, v in ((UC_X86_REG_EBX, dd), (UC_X86_REG_EBP, att), (UC_X86_REG_ESI, int(lvf)), (UC_X86_REG_EDI, lv_def)):
        e.mu.reg_write(reg, v & 0xffffffff)


def june_level(e, lvf, lv_def, pvp, drate, prate, user, power, memo, dtype, physic, magic, atk, melee, m, def_user, u):
    setup_level(e, lvf, lv_def, drate, prate, user, memo, dtype, physic, magic, atk, melee, m, def_user, u)
    start = 0x32285e71 if pvp else 0x32285e86
    hit = e.run(start, 0x32286037, stops=(0x32285ee9, 0x32285f2b))
    if hit == 0x32285ee9:                                   # vslot 0x204 GetPowerRate
        hit = e.run(0x32285eeb, 0x32286037, fld=power, stops=(0x32285f2b,))
    assert hit == 0x32285f2b, hex(hit)
    e.run(0x32285f30, 0x32286037, fld=f32(math.pow(lvf, 2.0)))  # pow (32fe2c8e), stored as float right after
    return e.rf(ESP + 0x30)


def june_sum(e, fin, wos, d, melee, m, def_user, u):
    dd, att, dfn = DATA + 0x200, OBJ, OBJ + 0x10000
    e.mu.mem_write(dd + 0x2c, bytes([1 if melee else 0])); e.wd(dd + 8, 0)
    enc(e, dfn + 0x5898, fbits(d)); e.wd(dfn + 0x264, 0 if def_user else 1)
    enc(e, att + 0x5730, fbits(m if melee else 7.0)); enc(e, att + 0x57e4, fbits(7.0 if melee else m))
    enc(e, att + 0x5e38, fbits(u if def_user else 9.0)); enc(e, att + 0x5d84, fbits(9.0 if def_user else u))
    e.esp(); e.wf(ESP + 0x14, fin)
    for reg, v in ((UC_X86_REG_EBX, dd), (UC_X86_REG_EBP, att), (UC_X86_REG_ESI, dfn)):
        e.mu.reg_write(reg, v)
    e.run(0x3228619b, 0x3228626e, fld=wos)                    # right after vslot 0x74 ( GetFinalDamage_WayOfSword ) returns
    return e.rf(ESP + 0x14)


def june_state(e, fin, pen, ign, reduce_pct, guard, guard_pct, recv):
    dd, this = DATA + 0x200, OBJ + 0x10000
    e.mu.mem_write(dd + 0x4c5, bytes([pen])); e.mu.mem_write(dd + 0x541, bytes([ign])); e.mu.mem_write(dd + 0x478, bytes([guard]))
    enc(e, this + 0x9e4, reduce_pct); e.wd(this + 0x9f0, guard_pct); e.wd(this, OBJ + 0x8000)
    e.esp(); e.wf(ESP + 0x14, fin); e.wf(ESP + 0x20, 0.0)
    e.mu.reg_write(UC_X86_REG_EBX, dd); e.mu.reg_write(UC_X86_REG_EDI, this)
    e.run(0x322862f6, 0x3228638f)                           # up to call vslot 0x2ac
    e.run(0x32286391, 0x322863b2, fld=recv)
    return e.rf(ESP + 0x14)


def june_part(e, fin, v):
    this = OBJ + 0x10000
    e.wf(this + 0x8fd4, v)
    e.esp(); e.wf(ESP + 0x14, fin)
    e.mu.reg_write(UC_X86_REG_EDI, this)
    e.run(0x32286455, 0x32286482)
    return e.rf(ESP + 0x14)


def june_shield(e, fin, r, state):
    this, main = OBJ + 0x10000, GMAIN_PAGE + 0x100
    e.wf(this + 0x412c, r); e.wd(main + 0x8c, state); e.wd(GMAIN_PTR, main)
    e.esp(); e.wf(ESP + 0x14, fin)
    e.mu.reg_write(UC_X86_REG_EDI, this)
    e.run(0x322867e0, 0x3228682a)
    return e.rf(ESP + 0x14)


def june_ignore_cap_clamps(e, s):
    e.esp(); e.wf(ESP + 0xc, s)
    return e.run(0x322749cc, 0x322749ea, stops=(0x322749e0, 0x322749ea)) == 0x322749e0


def june_gate(e, skill, npc, gage, name, grade, immune, main_boss):
    dd, dfn, vt, ret = DATA + 0x200, OBJ + 0x10000, OBJ + 0x8000, STUB + 0xf00
    e.wd(dd + 0x5c4, skill)
    e.wd(dfn, vt)
    for slot, fn in ((0x3c8, 0x32755a60), (0x3cc, 0x32755a70), (0x430, 0x32755a80)):
        e.wd(vt + slot, fn)
    e.wd(dfn + 0x264, 1 if npc else 0)
    e.mu.mem_write(dfn + 0x8ae9, bytes([name])); e.mu.mem_write(dfn + 0x8aeb, bytes([gage]))
    e.mu.mem_write(dfn + 0x8e80, bytes([main_boss])); e.mu.mem_write(dfn + 0x8f0c, bytes([immune]))
    e.wd(dfn + 0x8f08, grade)
    sp = ESP - 0x10
    e.wd(sp, ret); e.wd(sp + 4, dd); e.wd(sp + 8, dfn)
    e.mu.reg_write(UC_X86_REG_ESP, sp); e.mu.reg_write(UC_X86_REG_ECX, OBJ); e.mu.reg_write(UC_X86_REG_EAX, 0)
    e.mu.emu_start(0x322726f0, ret)
    return (e.mu.reg_read(UC_X86_REG_EAX) & 0xff) != 0


# ---- static checks ----

def june_enum(table):
    src = open(os.path.join(TRUNK, "june_src_fixed", "data036_kom", "Enum.src.lua"), encoding="utf-8-sig").read()
    body = re.search(r"\n%s = \{(.*?)\}" % table, src, re.S).group(1)
    return {k: int(v) for k, v in re.findall(r"(\w+) = (-?\d+)", body)}


def statics(e):
    raw = lambda va, n: e.text[va - BASE - 0xc00:va - BASE - 0xc00 + n]
    bad = 0
    enum_src = open(os.path.join(TRUNK, "june_src_fixed", "data036_kom", "Enum.src.lua"), encoding="utf-8-sig").read()
    pcc = {k: int(v) for k, v in re.findall(r"\b(PCC_\w+) = (-?\d+)", enum_src)}
    code = raw(0x32285e48, 0x1d)                              # cmp esi, 0xc / 0xd / 0xe; push; call; cmp eax, 0xb; cmp esi, 0xf
    imms = [code[2], code[7], code[12], code[0x17], code[0x1c]]
    want = [pcc["PCC_OFFICIAL"], pcc["PCC_GUILD"], pcc["PCC_EVENT_MODE"], pcc["PCC_PLAY"], pcc["PCC_1VS1_PVP_CHALLENGE"]]
    ok = imms == want and code[0x16] == 0xf8 and code[0x1b] == 0xfe
    bad += not ok
    print("STATIC %-34s %s june=%s port=%s" % ("level path PvP channel ids", "ok " if ok else "BAD", imms, want))

    dsrc = open(os.path.join(TRUNK, "june_src_fixed", "data036_kom", "DungeonEnum.src.lua"), encoding="utf-8-sig").read()
    dungeons = {k: int(v) for k, v in re.findall(r"(DI_SPACE_OF_THOUGHT_\w+) = (\d+)", dsrc)}
    base = 41480
    jt = struct.unpack("<2I", raw(0x328638a8, 8))              # [0] -> mov al, 1 (3286389c); [1] -> xor al, al (328638a1)
    table = raw(0x328638b0, 0x1f)
    got = sorted(base + i for i, b in enumerate(table) if jt[b] == 0x3286389c)
    want = sorted(dungeons[k] for k in ("DI_SPACE_OF_THOUGHT_1", "DI_SPACE_OF_THOUGHT_2", "DI_SPACE_OF_THOUGHT_3", "DI_SPACE_OF_THOUGHT_DUMMY"))
    ok = got == want and struct.unpack("<i", raw(0x32863885, 4))[0] == -base and jt == (0x3286389c, 0x328638a1)
    bad += not ok
    print("STATIC %-34s %s june=%s port=%s" % ("Space of Thought dungeon ids", "ok " if ok else "BAD", got, want))

    nesi = {k: int(v) for k, v in re.findall(r"\b(NESI_\w+) = (\d+)", enum_src)}
    targets = struct.unpack("<2I", raw(0x32869f5c, 8))
    btab = raw(0x32869f64, 23)
    got = sorted(0x14 + i for i, b in enumerate(btab) if targets[b] == 0x32869eeb)
    want = sorted(v for k, v in nesi.items() if k.startswith("NESI_MINORUNIT_"))
    ok = got == want
    bad += not ok
    print("STATIC %-34s %s june=%s port=%s" % ("minor-unit slot ids (stand-in)", "ok " if ok else "BAD", got, want))

    src = open(os.path.join(TRUNK, "X2Lib", "X2GameUnit.cpp"), "rb").read().decode("cp949")
    block = src[src.index("std::wstringstream wstrmDefenceIgnoreLog;"):]
    lits = re.findall(r'L"((?:[^"\\]|\\.)*)"', block[:block.index("AddChatLog")])

    def unesc(s):
        return re.sub(r"\\x([0-9A-Fa-f]{4})|\\n", lambda mm: chr(int(mm.group(1), 16)) if mm.group(1) else "\n", s)

    def wstr(va):
        o = va - BASE - 0xc00
        end = o
        while e.text[end:end + 2] != b"\0\0":
            end += 2
        return e.text[o:end].decode("utf-16le")
    got = [unesc(s) for s in lits]
    want = [wstr(va) for va in (0x33487b80, 0x33487b54, 0x33487b2c, 0x33487b24)]
    ok = got == want
    bad += not ok
    print("STATIC %-34s %s (%d strings)" % ("dev defence-ignore log strings", "ok " if ok else "BAD", len(want)))
    return bad


def review_inputs():
    """D2 review: 50 more sets - level gaps around the 5 / 6 / 7 edge, 0 and huge power, NaN and edge multipliers, state defence, part rate, shield r"""
    out = []
    nan = math.nan
    # level rows: lvf, lv_def, pvp, user, dtype, power, memo, melee, m, def_user, u    (14)
    for lvf, lv_def, pvp, user, dtype in ((60.0, 74, False, True, 0), (60.0, 78, False, True, 1), (50.0, 73, False, False, 2),
                                          (40.0, 66, False, True, 0), (70.0, 76, False, True, 1), (70.0, 77, False, True, 2),
                                          (80.0, 60, False, True, 0), (60.0, 74, True, True, 2), (50.0, 73, True, True, 0),
                                          (70.0, 77, True, False, 1)):
        out.append(("level", (lvf, lv_def, pvp, user, dtype, 1.0, 1.0, True, 1.0, False, 1.0)))
    out.append(("level", (60.0, 74, False, True, 0, 0.0, 1.0, True, 1.15, False, 1.2)))     # power 0
    out.append(("level", (70.0, 76, False, True, 1, 1.0, 0.0, False, 1.3, True, 0.3)))      # memo 0
    out.append(("level", (60.0, 74, False, True, 0, 1e30, 1.0, True, 1.0, False, 1.0)))     # huge power
    out.append(("level", (60.0, 78, False, True, 1, 1.0, 1e30, False, 0.7, True, 1.05)))    # huge memo
    # sum rows: fin, wos, d, m, u, melee, def_user    (7)
    out.append(("sum", (12345.6, 1.0000001, 1.0, 1.0, 1.0, True, False)))
    out.append(("sum", (12345.6, 1.0, 1.0000001, 1.0, 1.0, True, True)))
    out.append(("sum", (987654.25, 1.0, 1.0, 1.0000001, 0.5, False, False)))
    out.append(("sum", (12345.6, 0.5, 0.5, 1.0, 1.0, False, True)))
    out.append(("sum", (12345.6, 1.0, nan, 1.0, 1.0, True, False)))                         # NaN defence-rate multiplier
    out.append(("sum", (12345.6, 1.0, 1.0, nan, 1.0, True, True)))                          # NaN melee / magic multiplier
    out.append(("sum", (12345.6, 1.0, 1.0, 1.0, nan, False, False)))                        # NaN user multiplier
    # state rows: fin, pen, ign, reduce, guard, guard_pct, recv    (reduce and guard % are ints in June)    (8)
    out.append(("state", (54321.0, 0, 0, 100, 0, 0, 2.5)))
    out.append(("state", (54321.0, 0, 0, 101, 0, 0, 1.0)))
    out.append(("state", (1000.5, 0, 0, 0, 0, 0, 0.0)))
    out.append(("state", (1000.5, 0, 0, 99, 0, 0, 2.5)))
    out.append(("state", (54321.0, 0, 0, 2147483647, 0, 0, 1.0)))                           # 0x7fffffff
    out.append(("state", (54321.0, 0, 0, 1, 1, 99, 1.0)))                                   # 1 + 99 = 100 exactly
    out.append(("state", (54321.0, 0, 0, -1, 1, 100, 0.0)))
    out.append(("state", (54321.0, 1, 0, 100, 1, 100, 2.5)))                                # penetrated: no state defence
    # part rows: fin, v    (5)
    for fin, v in ((1000.5, 1.0), (1000.5, 1.0000001), (7654321.0, 0.9999999), (1000.5, nan), (7654321.0, 3.0)):
        out.append(("part", (fin, v)))
    # shield rows: state name, fin, r    (14)
    for name, rs in (("XS_PVP_GAME", (0.0, 0.9999999, 1.0, nan)), ("XS_DUNGEON_GAME", (0.0, 0.9999999, 1.0, nan)),
                     ("XS_TRAINING_GAME", (0.9999999, nan)), ("XS_BATTLE_FIELD", (0.0, nan)), ("XS_VILLAGE_MAP", (0.9999999, nan))):
        for r in rs:
            out.append(("shield", (name, 1000.5, r)))
    # ignore-defence cap sums    (2)
    out.append(("ignore", (0.9999999,)))
    out.append(("ignore", (-float("inf"),)))
    assert len(out) == 50, len(out)
    return out


def main():
    e = Emu()
    e.mu.mem_map(GMAIN_PAGE, 0x1000)
    n = bad = 0

    def check(label, j, p):
        nonlocal n, bad
        n += 1
        ok = same(j, p) if isinstance(j, float) else j == p
        if not ok:
            bad += 1
            print("MISMATCH %s june=%r port=%r" % (label, j, p))

    levels = [(18.0, 18), (99.0, 99), (99.0, 104), (60.0, 70), (70.0, 99), (1.0, 99), (99.0, 1), (65.0, 71), (65.0, 80)]
    for (lvf, lv_def), pvp, user, dtype in itertools.product(levels, (False, True), (True, False), (0, 1, 2)):
        for power, memo, melee, m, def_user, u in ((1.0, 1.0, True, 1.0, False, 1.0), (1.25, 1.1, True, 1.15, False, 1.2),
                                                   (0.8, 1.0, False, 1.3, True, 0.3), (1.0, 2.0, False, 0.7, True, 1.05)):
            vals = dict(drate=f32(1.25), prate=f32(0.6), physic=f32(1.8), magic=f32(2.3), atk=f32(41234.5))
            j = june_level(e, lvf, lv_def, pvp, vals["drate"], vals["prate"], user, f32(power), f32(memo), dtype, vals["physic"],
                           vals["magic"], vals["atk"], melee, f32(m), def_user, f32(u))
            p = port_level(lvf, lv_def, pvp, vals["drate"], vals["prate"], user, f32(power), f32(memo), dtype, vals["physic"],
                           vals["magic"], vals["atk"], f32(m), f32(u))
            check("level lv=%r/%d pvp=%d user=%d dt=%d power=%r memo=%r m=%r u=%r" % (lvf, lv_def, pvp, user, dtype, power, memo, m, u), j, p)

    for fin, wos, d, m, u, melee, def_user in itertools.product([0.0, 1.0, 12345.6, 987654.25, 3.0e7], [1.0, 1.15], [1.0, 0.85, 0.5],
                                                                [1.0, 1.1, 1.35], [1.0, 1.2, 0.3, 0.45, 2.5], (True, False), (True, False)):
        fin, wos, d, m, u = f32(fin), f32(wos), f32(d), f32(m), f32(u)
        check("sum fin=%r wos=%r d=%r m=%r u=%r melee=%d user=%d" % (fin, wos, d, m, u, melee, def_user), june_sum(e, fin, wos, d, melee, m, def_user, u),
              port_sum(fin, wos, d, m, u))

    for fin, pen, ign, rp, guard, gp, recv in itertools.product([1000.5, 54321.0], (0, 1), (0, 1), [0, 30, 100, 150, -20], (0, 1), [0, 50, 80],
                                                                 [1.0, 0.75, 0.0]):
        fin, recv = f32(fin), f32(recv)
        check("state fin=%r pen=%d ign=%d reduce=%d guard=%d gp=%d recv=%r" % (fin, pen, ign, rp, guard, gp, recv),
              june_state(e, fin, pen, ign, rp & 0xffffffff, guard, gp, recv), port_state(fin, pen, ign, rp, guard, gp, recv))

    for fin, v in itertools.product([0.0, 1000.5, 7654321.0], [1.0, 2.0, 0.5, 1.0000001, 0.0, math.nan, 3.75]):
        fin, v = f32(fin), f32(v)
        check("part fin=%r v=%r" % (fin, v), june_part(e, fin, v), port_part(fin, v))

    jnames = june_states()
    gap = 0
    for s in sorted(jnames):
        name = jnames[s]
        for fin, r in itertools.product([1000.5, 98765.0], [0.6, 0.4, 0.8, 1.0, 1.5, 0.0]):
            fin, r = f32(fin), f32(r)
            if name == "XS_PVE_GAME":
                gap += 1
                continue
            check("shield state=%d %s r=%r fin=%r" % (s, name, r, fin), june_shield(e, fin, r, s),
                  port_shield(fin, r, name if name in MARCH_STATES else None))

    for s in [0.0, 0.5, 1.0, 1.0000001, 2.0, math.nan, -0.5, float("inf")]:
        s = f32(s)
        check("ignore-defence cap sum=%r" % s, june_ignore_cap_clamps(e, s), port_ignore_cap_clamps(s))

    immune_sets = 0
    for skill, npc, gage, name, grade, immune, main_boss in itertools.product((SUPERSONIC, 24403, 0), (True, False), (0, 1), (0, 1),
                                                                               (0, 1, 2, 3), (0, 1), (0, 1)):
        j = june_gate(e, skill, npc, gage, name, grade, immune, main_boss)
        p = port_gate(skill, npc, gage, name, grade, immune, main_boss)
        immune_sets += immune
        check("hp gate skill=%d npc=%d gage=%d name=%d grade=%d immune=%d main=%d" % (skill, npc, gage, name, grade, immune, main_boss), j, p)

    rn = len(review_inputs())
    jidx = {name: k for k, name in jnames.items()}
    for group, a in review_inputs():
        if group == "level":
            lvf, lv_def, pvp, user, dtype, power, memo, melee, m, def_user, u = a
            vals = dict(drate=f32(1.25), prate=f32(0.6), physic=f32(1.8), magic=f32(2.3), atk=f32(41234.5))
            j = june_level(e, lvf, lv_def, pvp, vals["drate"], vals["prate"], user, f32(power), f32(memo), dtype, vals["physic"],
                           vals["magic"], vals["atk"], melee, f32(m), def_user, f32(u))
            p = port_level(lvf, lv_def, pvp, vals["drate"], vals["prate"], user, f32(power), f32(memo), dtype, vals["physic"],
                           vals["magic"], vals["atk"], f32(m), f32(u))
        elif group == "sum":
            fin, wos, d, m, u, melee, def_user = a
            fin, wos, d, m, u = f32(fin), f32(wos), f32(d), f32(m), f32(u)
            j, p = june_sum(e, fin, wos, d, melee, m, def_user, u), port_sum(fin, wos, d, m, u)
        elif group == "state":
            fin, pen, ign, rp, guard, gp, recv = a
            fin, recv = f32(fin), f32(recv)
            j = june_state(e, fin, pen, ign, rp & 0xffffffff, guard, gp, recv)
            p = port_state(fin, pen, ign, rp, guard, gp, recv)
        elif group == "part":
            fin, v = f32(a[0]), f32(a[1])
            j, p = june_part(e, fin, v), port_part(fin, v)
        elif group == "shield":
            name, fin, r = a
            fin, r = f32(fin), f32(r)
            j, p = june_shield(e, fin, r, jidx[name]), port_shield(fin, r, name if name in MARCH_STATES else None)
        else:
            s = f32(a[0])
            j, p = june_ignore_cap_clamps(e, s), port_ignore_cap_clamps(s)
        check("review %s %r" % (group, a), j, p)
        if isinstance(j, float) and not same(j, p):
            print("  bits june=%08x port=%08x" % (fbits(j), fbits(p)))

    sbad = statics(e)
    print("EMU_FINAL_DAMAGE_B review sets: %d of the %d input sets below" % (rn, n))
    print("EMU_FINAL_DAMAGE_B %s: %d input sets, %d mismatch(es), %d static mismatch(es); %d XS_PVE_GAME set(s) not compared (no March state), "
          "%d HP-gate set(s) with IMMUNE_TO_HP_ATTACK compared (D3)" % ("OK" if bad == 0 and sbad == 0 else "FAIL", n, bad, sbad, gap, immune_sets))


if __name__ == "__main__":
    main()
