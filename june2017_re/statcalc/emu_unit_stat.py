"""june-stat-calc S1: June's KStat -> Stat conversion and CX2Stat::Stat::SetStat, run in Unicorn, against the port.

June 322814a0 (CX2GameUnit::SetGameStat( const KStat& ), vslot 120):
  @322814e4-32281552  the 13-dword Stat built on the stack from the 7-field KStat { float hp; int atkP, atkM, rangeP,
                      rangeM, defP, defM }: [0..5] = 0, [6] = hp, [7..12] = (float) the six ints.
June 322a91a0 (CX2Stat::Stat::SetStat): [6..12] copied, [0..5] = 0 (ret 4).
The port (X2Stat.h SetKStat / SetStat under SERV_IRUHADEV_JUNE_STATCALC_UNIT_STAT) is transcribed as `port_*()`: member
order is March's (basics first, ExtraStat last), compared by name. The range pair is Stat [9] / [10].

Usage: python june2017_re/statcalc/emu_unit_stat.py      (prints one line per mismatch, then the summary line)
"""
import ctypes
import random
import struct

from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_FPCW

EXE = r"F:\elsword stuff\elsword_2014\els_2014\237311\14JUN17\data\unpacked_x2.exe"
BASE = 0x31aa0000
TEXT_RVA, TEXT_RAW, TEXT_SIZE = 0x1000, 0x400, 0x1cbba00
STACK, DATA = 0x10000000, 0x20000000
ESP = STACK + 0x8000
RET = 0x30000000

NAMES = ["incHP", "incMP", "incAtkP", "incAtkM", "incDefP", "incDefM",
         "hp", "atkP", "atkM", "rangeP", "rangeM", "defP", "defM"]


def f32(x):
    return ctypes.c_float(x).value


def port_set_kstat(k):
    """X2Stat.h Stat::SetKStat under _UNIT_STAT: hp float as is, the six ints converted, the ExtraStat rates 0"""
    hp, atkp, atkm, rngp, rngm, defp, defm = k
    return [0.0] * 6 + [f32(hp), f32(float(atkp)), f32(float(atkm)), f32(float(rngp)), f32(float(rngm)),
                        f32(float(defp)), f32(float(defm))]


def port_set_stat(s):
    """X2Stat.h Stat::SetStat under _UNIT_STAT: the seven basics copied, the ExtraStat rates 0"""
    return [0.0] * 6 + [f32(v) for v in s[6:13]]


def make_emu():
    data = open(EXE, "rb").read()
    mu = Uc(UC_ARCH_X86, UC_MODE_32)
    size = (TEXT_SIZE + 0xfff) & ~0xfff
    mu.mem_map(BASE + TEXT_RVA, size)
    mu.mem_write(BASE + TEXT_RVA, data[TEXT_RAW:TEXT_RAW + TEXT_SIZE])
    mu.mem_map(STACK, 0x10000)
    mu.mem_map(DATA, 0x1000)
    mu.mem_map(RET, 0x1000)
    return mu


def floats(b):
    return list(struct.unpack("<13f", b))


def june_kstat(mu, k):
    mu.mem_write(DATA, struct.pack("<f6i", *k))
    mu.mem_write(ESP + 0xbc, struct.pack("<I", DATA))
    mu.mem_write(ESP + 0x80, struct.pack("<13I", *[0xdeadbeef] * 13))   # the stored block (rep movsd source)
    mu.reg_write(UC_X86_REG_ESP, ESP)
    mu.reg_write(UC_X86_REG_FPCW, 0x027f)
    mu.emu_start(0x322814e4, 0x3228154e)      # stops before the hp basic is pushed; the push ecx @32281514 moved esp
    esp = mu.reg_read(UC_X86_REG_ESP)
    return floats(mu.mem_read(esp + 0x14, 52))   # pre-push esp + 0x10


def june_set_stat(mu, s):
    mu.mem_write(DATA + 0x100, struct.pack("<13f", *s))
    mu.mem_write(DATA + 0x200, struct.pack("<13I", *[0xdeadbeef] * 13))
    mu.mem_write(ESP, struct.pack("<II", RET, DATA + 0x100))
    mu.reg_write(UC_X86_REG_ESP, ESP)
    mu.reg_write(UC_X86_REG_ECX, DATA + 0x200)
    mu.reg_write(UC_X86_REG_FPCW, 0x027f)
    mu.emu_start(0x322a91a0, RET)
    return floats(mu.mem_read(DATA + 0x200, 52))


def same(a, b):
    return all(struct.pack("<f", x) == struct.pack("<f", y) for x, y in zip(a, b))


def main():
    mu = make_emu()
    rnd = random.Random(20261116)
    edge_i = [0, 1, -1, 7, 1500, 4321, 99999, 2 ** 24 + 1, 2 ** 31 - 1, -2 ** 31]
    edge_f = [0.0, 1.0, -1.0, 0.5, 12345.678, 3.0e38, -0.0]
    sets = []
    for i in edge_i:
        for f in edge_f:
            sets.append((f, i, i // 2, i // 10, i // 3, i, -i if i != -2 ** 31 else i))
    for _ in range(300):
        sets.append((f32(rnd.uniform(-1e6, 1e6)),) + tuple(rnd.randint(-2 ** 31, 2 ** 31 - 1) for _ in range(6)))
    bad = 0
    for k in sets:
        j, p = june_kstat(mu, k), port_set_kstat(k)
        if not same(j, p):
            bad += 1
            print("KSTAT MISMATCH", k, [(n, a, b) for n, a, b in zip(NAMES, j, p) if a != b])
    ssets = [[f32(rnd.uniform(-1e7, 1e7)) for _ in range(13)] for _ in range(300)]
    ssets += [[0.0] * 13, [1.0] * 13, [-0.0] * 13]
    for s in ssets:
        j, p = june_set_stat(mu, s), port_set_stat(s)
        if not same(j, p):
            bad += 1
            print("SETSTAT MISMATCH", [(n, a, b) for n, a, b in zip(NAMES, j, p) if a != b])
    print("EMU_UNIT_STAT: %d KStat sets, %d SetStat sets, %d mismatch(es) -> %s"
          % (len(sets), len(ssets), bad, "PASS" if bad == 0 else "FAIL"))


if __name__ == "__main__":
    main()
