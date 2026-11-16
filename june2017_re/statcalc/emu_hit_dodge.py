"""june-stat-calc S1b: June's hit-rate arithmetic and ResetMaxHP's two multiplies, run in Unicorn, against the port.

June 32610a00 (CX2DamageManager::CalcHitDodgePercent) @32610b31-32610b98:
  accuracy = attacker protected float +0x4be0; blind = attacker vslot 0x1d0; avoidance = defender protected float +0x4c94;
  extra = [esp+0x10] (GetEvadeUpPerBySMA, preloaded); hit = (float)( blind * accuracy ) - (float)( avoidance + extra - 1.0 ).
June 31feba70 (CX2GUUser::ResetMaxHP) @31febb3e-31febb53 / @31febba6-31febbbb: (float)( hp * (double)0.7f ) and (float)( hp * 1.5 ).
The port (X2DamageManager.cpp / X2GUUser.cpp under SERV_IRUHADEV_JUNE_STATCALC_HIT_DODGE / _UNIT_STAT) is transcribed as port_*().
x87 precision control 53-bit (the client's FPCW 0x027f), so a double product rounded to float is the port's expression.

Usage: python june2017_re/statcalc/emu_hit_dodge.py      (prints one line per mismatch, then the summary line)
"""
import ctypes
import random
import struct

from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EDI, UC_X86_REG_ESI, UC_X86_REG_EBP, UC_X86_REG_FPCW

EXE = r"F:\elsword stuff\elsword_2014\els_2014\237311\14JUN17\data\unpacked_x2.exe"
BASE = 0x31aa0000
TEXT_RVA, TEXT_RAW, TEXT_SIZE = 0x1000, 0x400, 0x1cbba00
RDATA_LO, RDATA_HI = 0x33420000, 0x33480000      # the double constants 334355b0 (1.0), 3344af88 (0.7f), 334396c8 (1.5)
STACK, DATA, STUB = 0x10000000, 0x20000000, 0x30000000
ESP = STACK + 0x8000
ATT, DEF = DATA, DATA + 0x10000                  # attacker / defender objects ( 0x5000 bytes each is enough )
VTBL = DATA + 0x20000


def f32(x):
    return ctypes.c_float(x).value


def fbits(x):
    return struct.unpack("<I", struct.pack("<f", x))[0]


def port_hit(acc, blind, avoid, extra):
    """X2DamageManager.cpp CalcHitDodgePercent under _HIT_DODGE"""
    att = f32(blind * acc)                       # GetAttackRateByAbnormalityBlind() * GetAccuracyPercent()
    ev = f32(float(avoid) + extra - 1.0)         # static_cast<float>( (double)avoid + extra - 1.0 )
    return f32(att - ev)


def port_maxhp(hp):
    """X2GUUser.cpp ResetMaxHP under _UNIT_STAT"""
    return f32(float(hp) * float(f32(0.7))), f32(hp * 1.5)


def make_emu():
    data = open(EXE, "rb").read()
    mu = Uc(UC_ARCH_X86, UC_MODE_32)
    size = (TEXT_SIZE + 0xfff) & ~0xfff
    mu.mem_map(BASE + TEXT_RVA, size)
    mu.mem_write(BASE + TEXT_RVA, data[TEXT_RAW:TEXT_RAW + TEXT_SIZE])
    mu.mem_map(STACK, 0x10000)
    mu.mem_map(DATA, 0x30000)
    mu.mem_map(STUB, 0x1000)
    return mu


def put_protected(mu, base, value):
    """June's protected float: value = [base+8] ^ ( [base+4] ^ base ); key 0"""
    mu.mem_write(base + 4, struct.pack("<II", 0, fbits(value) ^ base))


def june_hit(mu, acc, blind, avoid, extra):
    put_protected(mu, ATT + 0x4be0, acc)
    put_protected(mu, DEF + 0x4c94, avoid)
    mu.mem_write(STUB + 0x100, struct.pack("<f", blind))
    mu.mem_write(STUB, b"\xd9\x05" + struct.pack("<I", STUB + 0x100) + b"\xc3")     # fld dword [blind]; ret
    mu.mem_write(VTBL + 0x1d0, struct.pack("<I", STUB))
    mu.mem_write(ATT, struct.pack("<I", VTBL))
    mu.mem_write(ATT + 0x264, struct.pack("<I", 1))
    mu.mem_write(ESP + 0x10, struct.pack("<f", extra))
    mu.reg_write(UC_X86_REG_ESP, ESP)
    mu.reg_write(UC_X86_REG_EDI, ATT)
    mu.reg_write(UC_X86_REG_ESI, DEF)
    mu.reg_write(UC_X86_REG_EBP, 0)
    mu.reg_write(UC_X86_REG_FPCW, 0x027f)
    mu.emu_start(0x32610b31, 0x32610b9c)
    esp = mu.reg_read(UC_X86_REG_ESP)            # ebp still pushed: the result sits at [esp + 0x14]
    return struct.unpack("<f", mu.mem_read(esp + 0x14, 4))[0]


def june_maxhp(mu, hp):
    out = []
    for start, stop in ((0x31febb3e, 0x31febb53), (0x31febba6, 0x31febbbb)):
        mu.mem_write(ESP + 0x30, struct.pack("<f", hp))
        mu.reg_write(UC_X86_REG_ESP, ESP)
        mu.reg_write(UC_X86_REG_FPCW, 0x027f)
        mu.emu_start(start, stop)                # stops after fstp [esp+0x14] ( the push ecx moved esp by 4 )
        esp = mu.reg_read(UC_X86_REG_ESP)
        out.append(struct.unpack("<f", mu.mem_read(esp + 0x14, 4))[0])
    return tuple(out)


def same(a, b):
    return fbits(a) == fbits(b)


def main():
    data = open(EXE, "rb").read()
    mu = make_emu()
    rnd = random.Random(20261116)
    accs = [0.0, 0.5, 0.9, 1.0, 1.05, 1.2, 2.0] + [f32(rnd.uniform(0.0, 2.0)) for _ in range(3)]
    blinds = [1.0, 0.7, 0.5, 0.0] + [f32(rnd.uniform(0.0, 1.0)) for _ in range(2)]
    avoids = [1.0, 0.8, 1.15, 1.3] + [f32(rnd.uniform(0.5, 1.5)) for _ in range(2)]
    extras = [0.0, 0.1, -0.2] + [f32(rnd.uniform(-0.3, 0.3)) for _ in range(2)]
    n_hit = bad = 0
    accs, blinds, avoids, extras = ([f32(x) for x in l] for l in (accs, blinds, avoids, extras))   # the members are floats
    for a in accs:
        for b in blinds:
            for v in avoids:
                for e in extras:
                    j, p = june_hit(mu, a, b, v, e), port_hit(a, b, v, e)
                    n_hit += 1
                    if not same(j, p):
                        bad += 1
                        print("HIT MISMATCH acc=%r blind=%r avoid=%r extra=%r june=%r port=%r" % (a, b, v, e, j, p))
    hps = [0.0, 1.0, 157000.0, 166419.99, 226204.0, 235624.0, 1e6, 12345.678] + [f32(rnd.uniform(1, 5e5)) for _ in range(42)]
    n_hp = 0
    for hp in hps:
        j, p = june_maxhp(mu, f32(hp)), port_maxhp(f32(hp))
        n_hp += 1
        if not (same(j[0], p[0]) and same(j[1], p[1])):
            bad += 1
            print("MAXHP MISMATCH hp=%r june=%r port=%r" % (hp, j, p))
    print("EMU_HIT_DODGE: %d hit-rate sets, %d max-HP sets, %d mismatch(es) -> %s" % (n_hit, n_hp, bad, "PASS" if bad == 0 else "FAIL"))


if __name__ == "__main__":
    main()
