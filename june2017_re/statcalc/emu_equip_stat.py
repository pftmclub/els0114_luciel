"""june-stat-calc S2: June's item range / enchant arithmetic and the defence resist switch, run in Unicorn, against the port.

June 32882630 (CX2Item::GetEnchantStat) @328827f9-32882867: the stat copy, then range[9] / [10] = (float)( (double)atk * damage_range * 0.01f )
  ( templet +0x2c ); @3288297a-328829a0: abs( added + level ) < 0x54 -> 32071640( stat, ENCHANT_STAT_SCALE[ index ], false ).
June 32358df0 (CX2Unit::GetEnchantStat) @32358efa-32359588: the switch on the defence item's EDT ( 32707ce0 ), six floats at unit +0xd24..+0xd38.
The port: X2Item.cpp ( GetDamageRangeStat, GetEnchantStat ) and the switch in X2Unit.cpp under SERV_IRUHADEV_JUNE_STATCALC_EQUIP_STAT, which
this script parses; EDT values from june_src_fixed/data036_kom/Enum.src.lua. x87 precision control 53-bit (the client's FPCW 0x027f).

Usage: python june2017_re/statcalc/emu_equip_stat.py      (prints one line per mismatch, then the summary line)
"""
import ctypes
import os
import random
import re
import struct

from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_ESI, UC_X86_REG_EBX, UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_FPCW

HERE = os.path.dirname(os.path.abspath(__file__))
TRUNK = os.path.dirname(os.path.dirname(HERE))
EXE = r"F:\elsword stuff\elsword_2014\els_2014\237311\14JUN17\data\unpacked_x2.exe"
STACK, DATA, STUB = 0x10000000, 0x20000000, 0x30000000
ESP = STACK + 0x8000
FIELDS = ["Blaze", "Water", "Nature", "Wind", "Light", "Dark"]       # unit +0xd24 .. +0xd38 = CX2Stat::EnchantStat order
VALUES = {"CX2EnchantItem::EAR_VALUE": 75.0, "CX2EnchantItem::EAR_MASTER_VALUE": 110.0, "EAR_DOUBLE_MASTER_VALUE": 135.0}
ENCHANT_STAT_SCALE = [1.0, 1.03, 1.06, 1.09, 1.16, 1.23, 1.30, 1.45, 1.60, 1.75, 2.15, 2.55, 2.95, 3.35, 3.75, 4.25, 4.75, 5.25, 5.75, 6.75, 8.75]


def f32(x):
    return ctypes.c_float(x).value


def fbits(x):
    return struct.unpack("<I", struct.pack("<f", x))[0]


def make_emu():
    data = open(EXE, "rb").read()
    pe = struct.unpack_from("<I", data, 0x3c)[0]
    nsec = struct.unpack_from("<H", data, pe + 6)[0]
    opt = struct.unpack_from("<H", data, pe + 20)[0]
    base = struct.unpack_from("<I", data, pe + 24 + 28)[0]
    mu = Uc(UC_ARCH_X86, UC_MODE_32)
    for i in range(nsec):
        vs, va, rs, ra = struct.unpack_from("<IIII", data, pe + 24 + opt + 40 * i + 8)
        size = (max(vs, rs) + 0xfff) & ~0xfff
        mu.mem_map(base + va, size)
        mu.mem_write(base + va, data[ra:ra + min(rs, size)])
    mu.mem_map(STACK, 0x10000)
    mu.mem_map(DATA, 0x10000)
    mu.mem_map(STUB, 0x1000)
    return mu


# ------------------------------------------------------------------ the port, read from source
def port_resist_table():
    enum = open(os.path.join(TRUNK, "june_src_fixed", "data036_kom", "Enum.src.lua"), encoding="utf-8", errors="replace").read()
    edt = {}
    for name, value in re.findall(r"\b(EDT_ENCHANT_\w+)\s*=\s*(\d+)", enum):
        edt.setdefault(name, int(value))
    src = open(os.path.join(TRUNK, "X2Lib", "X2Unit.cpp"), "rb").read().decode("cp949", "replace")
    body = src[src.index("// june-stat-calc S2: June 32358df0"):]
    body = body[:body.index("#else SERV_IRUHADEV_JUNE_STATCALC_EQUIP_STAT")]
    table = {}
    for name, adds in re.findall(r"case CX2DamageManager::(EDT_ENCHANT_\w+):\s*(.*?)\bbreak;", body):
        row = [0.0] * 6
        for field, value in re.findall(r"m_EnchantStat\.m_fDef(\w+) \+= ([\w:]+);", adds):
            row[FIELDS.index(field)] += VALUES[value]
        table[edt[name]] = row
    return table


def port_range(atk, damage_range):
    """CX2Item::GetDamageRangeStat"""
    return f32(float(atk) * float(damage_range) * float(f32(0.01)))


# ------------------------------------------------------------------ June
def june_resist(mu, edt_value):
    mu.mem_write(DATA + 0xd24, b"\x00" * 24)
    mu.reg_write(UC_X86_REG_ESP, ESP)
    mu.reg_write(UC_X86_REG_ESI, DATA)
    mu.reg_write(UC_X86_REG_EAX, edt_value & 0xffffffff)
    mu.reg_write(UC_X86_REG_FPCW, 0x027f)
    mu.emu_start(0x32358efa, 0x32359588)
    return list(struct.unpack("<6f", mu.mem_read(DATA + 0xd24, 24)))


def june_range(mu, stat7, damage_range):
    """@328827f9-32882867 with a fldz in front ( the 0.0 June keeps on the x87 stack from its fldz at entry )"""
    out, item, templet = DATA + 0x100, DATA + 0x200, DATA + 0x400
    mu.mem_write(ESP + 0x28, struct.pack("<7f", *stat7))
    mu.mem_write(ESP + 0x48, struct.pack("<I", out))
    mu.mem_write(out, b"\xcc" * 0x34)
    mu.mem_write(item + 0xc0, struct.pack("<I", templet))
    mu.mem_write(templet + 0x2c, struct.pack("<f", damage_range))
    mu.mem_write(STUB, b"\xd9\xee" + b"\xe9" + struct.pack("<i", 0x328827f9 - (STUB + 7)))   # fldz; jmp 328827f9
    mu.reg_write(UC_X86_REG_ESP, ESP)
    mu.reg_write(UC_X86_REG_EBX, item)
    mu.reg_write(UC_X86_REG_FPCW, 0x027f)
    mu.emu_start(STUB, 0x32882867)
    return struct.unpack("<13f", mu.mem_read(out, 0x34))


def june_enchant(mu, stat13, added, level):
    """@3288297a-328829a0: abs( added + level ) < 0x54 -> 32071640( stat, ENCHANT_STAT_SCALE[ index ], false )"""
    out, added_ptr = DATA + 0x100, DATA + 0x300
    mu.mem_write(out, struct.pack("<13f", *stat13))
    mu.mem_write(added_ptr, struct.pack("<i", added))
    mu.mem_write(ESP + 0x4c, struct.pack("<I", added_ptr))
    mu.reg_write(UC_X86_REG_ESP, ESP)
    mu.reg_write(UC_X86_REG_ESI, out)
    mu.reg_write(UC_X86_REG_ECX, level & 0xffffffff)
    mu.reg_write(UC_X86_REG_FPCW, 0x027f)
    mu.emu_start(0x3288297a, 0x328829a0)
    return struct.unpack("<13f", mu.mem_read(out, 0x34))


def same(a, b):
    return all(fbits(x) == fbits(y) for x, y in zip(a, b))


def main():
    mu = make_emu()
    rnd = random.Random(20261116)
    bad = 0

    table = port_resist_table()
    tested = sorted(table) + [0, 1, 1000, 1002, 2001, 4198, 4308, 5296, 6000, 7000, -1] + [rnd.randint(1000, 8000) for _ in range(40)]
    n_resist = 0
    for value in tested:
        j = june_resist(mu, value)
        p = table.get(value, [0.0] * 6)
        n_resist += 1
        if not same(j, p):
            bad += 1
            print("RESIST MISMATCH edt=%d june=%r port=%r" % (value, j, p))

    atks = [0.0, 1.0, 7.0, 6.0, 1000.0, 19398.0, 15685.0, 12345.678, 99999.0] + [f32(rnd.uniform(0, 60000)) for _ in range(21)]
    ranges = [0.0, 10.0, 25.0, 55.0, 37.5] + [f32(rnd.uniform(0, 60)) for _ in range(5)]
    n_range = 0
    for a in atks:
        for r in ranges:
            a2 = f32(rnd.uniform(0, 60000))
            stat7 = (f32(rnd.uniform(0, 1e5)), f32(a), a2, 0.0, 0.0, f32(rnd.uniform(0, 9e3)), f32(rnd.uniform(0, 9e3)))
            j = june_range(mu, stat7, f32(r))
            want = [0.0] * 6 + [stat7[0], stat7[1], stat7[2], port_range(stat7[1], f32(r)), port_range(a2, f32(r)), stat7[5], stat7[6]]
            n_range += 1
            if not same(j, want):
                bad += 1
                print("RANGE MISMATCH atk=%r/%r damage_range=%r june=%r port=%r" % (stat7[1], a2, r, j[6:], want[6:]))

    n_enchant = 0
    for level in list(range(-20, 21)) + [21, 40, 83, 84, 100, -84, -100]:
        for added in (0, 1, -1):
            stat13 = tuple([f32(rnd.uniform(0, 2)) for _ in range(6)] + [f32(rnd.uniform(0, 6e4)) for _ in range(7)])
            j = june_enchant(mu, stat13, added, level)
            index = abs(added + level)
            if index <= 20:
                scale = f32(ENCHANT_STAT_SCALE[index])
                want = list(stat13[:6]) + [f32(x * scale) for x in stat13[6:]]
            elif index >= 0x54:
                want = list(stat13)                  # June and the port skip the multiply
            else:
                continue                             # 21..83 read past the table in both ( JUNE_BUGS ); the bytes differ
            n_enchant += 1
            if not same(j, want):
                bad += 1
                print("ENCHANT MISMATCH level=%d added=%d june=%r port=%r" % (level, added, j, want))

    print("EMU_EQUIP_STAT: %d resist sets ( %d EDT cases in the port ), %d range sets, %d enchant sets, %d mismatch(es) -> %s"
          % (n_resist, len(table), n_range, n_enchant, bad, "PASS" if bad == 0 else "FAIL"))


if __name__ == "__main__":
    main()
