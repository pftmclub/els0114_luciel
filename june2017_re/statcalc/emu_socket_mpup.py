"""june-stat-calc S3: June's socket MP-up-when-hit roll and its cool-time tick, run in Unicorn, against the port.

June 31fd9100 (CX2GUUser, thiscall( entries*, timers* ), called @32047d55 in DamageReact): per 12-byte entry ( chance, MP, cool time ) with
  its timer: when cool time > 0 and timer > 0 the entry is skipped (no roll), else roll = vslot 0x1cc( 8 + rolls so far ) and when
  roll < chance the timer = cool time and the MP is added. Returns the MP sum.
June 31fda180 (CX2GUUser, thiscall( elapsed ), called @3206f728 in OnFrameMove): entries = aggregate ( +0x87b4 ) +0x114, timers +0x87b8;
  each timer whose entry's cool time > 0 and timer > 0 is lowered by elapsed and set to 0 at or below 0.
The port (X2GUUser.cpp CalculateMpUpAttackedBySocketOption / CheckSocketMpUpAttackedCoolTime under SERV_IRUHADEV_JUNE_STATCALC_SOCKET) is
transcribed as port_*(). The timer vector is given the entries' size, so neither resize branch runs (both are vector::resize).

Usage: python june2017_re/statcalc/emu_socket_mpup.py      (prints one line per mismatch, then the summary line)
"""
import ctypes
import random
import struct

from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_ECX, UC_X86_REG_FPCW

EXE = r"F:\elsword stuff\elsword_2014\els_2014\237311\14JUN17\data\unpacked_x2.exe"
BASE = 0x31aa0000
TEXT_RVA, TEXT_RAW, TEXT_SIZE = 0x1000, 0x400, 0x1cbba00
STACK, DATA, STUB = 0x10000000, 0x20000000, 0x30000000
ESP0 = STACK + 0x8000
USER = DATA                     # the user object ( 0x9000 bytes )
AGG = DATA + 0x10000            # the socket aggregate
VTBL = DATA + 0x20000
ENTRIES = DATA + 0x21000
TIMERS = DATA + 0x22000
VEC_E, VEC_T = DATA + 0x23000, DATA + 0x23100
ROLLS = STUB + 0x100            # roll table indexed by the seed
HALT = STUB + 0x800             # the roll returns here: fstp dword [RESULT], then stops at HALT_END
HALT_END = HALT + 6
RESULT = STUB + 0x900
SRO_ADD_MANA_ON_ATTACK = 8


def f32(x):
    return ctypes.c_float(x).value


def port_roll(entries, timers, rolls):
    """CalculateMpUpAttackedBySocketOption: returns ( mp, timers, seeds used )"""
    timers = list(timers)
    mp = f32(0.0)
    seed = 0
    used = []
    for i, (chance, value, cool) in enumerate(entries):
        if 0.0 < cool and 0.0 < timers[i]:
            continue
        used.append(SRO_ADD_MANA_ON_ATTACK + seed)
        roll = rolls[SRO_ADD_MANA_ON_ATTACK + seed]
        seed += 1
        if roll < chance:
            timers[i] = cool
            mp = f32(mp + value)
    return mp, timers, used


def port_tick(entries, timers, elapsed):
    """CheckSocketMpUpAttackedCoolTime"""
    timers = list(timers)
    for i, (chance, value, cool) in enumerate(entries):
        if 0.0 < cool and 0.0 < timers[i]:
            timers[i] = f32(timers[i] - elapsed)
            if timers[i] <= 0.0:
                timers[i] = 0.0
    return timers


def make_emu():
    data = open(EXE, "rb").read()
    mu = Uc(UC_ARCH_X86, UC_MODE_32)
    mu.mem_map(BASE + TEXT_RVA, (TEXT_SIZE + 0xfff) & ~0xfff)
    mu.mem_write(BASE + TEXT_RVA, data[TEXT_RAW:TEXT_RAW + TEXT_SIZE])
    mu.mem_map(STACK, 0x10000)
    mu.mem_map(DATA, 0x30000)
    mu.mem_map(STUB, 0x1000)
    # vslot 0x1cc: mov eax, [esp+4]; fld dword [ROLLS + eax*4]; ret 4
    mu.mem_write(STUB, b"\x8b\x44\x24\x04" + b"\xd9\x04\x85" + struct.pack("<I", ROLLS) + b"\xc2\x04\x00")
    mu.mem_write(VTBL + 0x1cc, struct.pack("<I", STUB))
    mu.mem_write(HALT, b"\xd9\x1d" + struct.pack("<I", RESULT) + b"\x90")     # fstp dword [RESULT]; nop
    mu.mem_write(USER, struct.pack("<I", VTBL))
    seeds = []
    mu.hook_add(UC_HOOK_CODE, lambda uc, addr, size, ud: seeds.append(struct.unpack("<I", uc.mem_read(uc.reg_read(UC_X86_REG_ESP) + 4, 4))[0]),
                begin=STUB, end=STUB)
    return mu, seeds


def write_vectors(mu, entries, timers):
    mu.mem_write(ENTRIES, b"".join(struct.pack("<fff", *e) for e in entries) or b"\0")
    mu.mem_write(TIMERS, b"".join(struct.pack("<f", t) for t in timers) or b"\0")
    mu.mem_write(VEC_E, struct.pack("<III", ENTRIES, ENTRIES + 12 * len(entries), ENTRIES + 12 * len(entries)))
    mu.mem_write(VEC_T, struct.pack("<III", TIMERS, TIMERS + 4 * len(timers), TIMERS + 4 * len(timers)))
    # the tick reads the same vectors through the user: aggregate pointer +0x87b4, aggregate +0x114, timers +0x87b8
    mu.mem_write(USER + 0x87b4, struct.pack("<IIII", AGG, TIMERS, TIMERS + 4 * len(timers), TIMERS + 4 * len(timers)))
    mu.mem_write(AGG + 0x114, struct.pack("<III", ENTRIES, ENTRIES + 12 * len(entries), ENTRIES + 12 * len(entries)))


def read_timers(mu, n):
    return [struct.unpack("<f", mu.mem_read(TIMERS + 4 * i, 4))[0] for i in range(n)]


def june_roll(mu, seeds, entries, timers, rolls):
    write_vectors(mu, entries, timers)
    mu.mem_write(ROLLS, b"".join(struct.pack("<f", r) for r in rolls))
    esp = ESP0
    mu.mem_write(esp, struct.pack("<III", HALT, VEC_E, VEC_T))
    mu.reg_write(UC_X86_REG_ESP, esp)
    mu.reg_write(UC_X86_REG_ECX, USER)
    mu.reg_write(UC_X86_REG_FPCW, 0x027f)
    del seeds[:]
    mu.emu_start(0x31fd9100, HALT_END)
    return struct.unpack("<f", mu.mem_read(RESULT, 4))[0], read_timers(mu, len(timers)), list(seeds)


def june_tick(mu, entries, timers, elapsed):
    write_vectors(mu, entries, timers)
    mu.mem_write(ESP0, struct.pack("<If", HALT, elapsed))
    mu.reg_write(UC_X86_REG_ESP, ESP0)
    mu.reg_write(UC_X86_REG_ECX, USER)
    mu.reg_write(UC_X86_REG_FPCW, 0x027f)
    mu.emu_start(0x31fda180, HALT)
    return read_timers(mu, len(timers))


def main():
    rnd = random.Random(20261116)
    mu, seeds = make_emu()
    chances = [0.0, 0.05, 0.18, 0.5, 1.0]
    cools = [0.0, -1.0, 3.0, 10.0]
    timer_values = [0.0, -0.1, 0.5, 5.0, 10.0]
    bad = 0
    n_roll = n_tick = 0
    for _ in range(600):
        n = rnd.randint(0, 5)
        entries = [(f32(rnd.choice(chances + [rnd.random()])), f32(rnd.choice([1.0, 10.0, 20.0, 50.0, -5.0])), f32(rnd.choice(cools))) for _ in range(n)]
        timers = [f32(rnd.choice(timer_values)) for _ in range(n)]
        rolls = [f32(rnd.random()) for _ in range(32)]
        j = june_roll(mu, seeds, entries, timers, rolls)
        p = port_roll(entries, timers, rolls)
        n_roll += 1
        if j[0] != p[0] or j[1] != p[1] or j[2] != p[2]:
            bad += 1
            print("ROLL MISMATCH entries=%s timers=%s june=%s port=%s" % (entries, timers, j, p))
        elapsed = f32(rnd.choice([0.016, 0.033, 0.5, 1.0, 12.0]))
        jt = june_tick(mu, entries, timers, elapsed)
        pt = port_tick(entries, timers, elapsed)
        n_tick += 1
        if jt != pt:
            bad += 1
            print("TICK MISMATCH entries=%s timers=%s elapsed=%s june=%s port=%s" % (entries, timers, elapsed, jt, pt))
    print("EMU_SOCKET_MPUP: %d roll sets, %d tick sets, %d mismatch(es) -> %s" % (n_roll, n_tick, bad, "PASS" if bad == 0 else "FAIL"))


if __name__ == "__main__":
    main()
