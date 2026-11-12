"""june-stat-calc D2a: June's GetFinalDamage ranges before @32285ccf, run in Unicorn, against the port's expressions.

June 32284aa0 (CX2GameUnit::GetFinalDamage) and its callees; the port is X2GameUnit.cpp under SERV_IRUHADEV_JUNE_STATCALC_FINAL_DAMAGE.
  @322850f5-3228512b  dragon breath: rate = (float)( r - 1.0 + (double)5e-8f ); P += P * rate; M += M * rate
  @32285153-32285189  El blessing: the same on the two defences
  @32285009-32285055  Manaflow Overdrive: P *= (float)( 1 + mp / maxMp * (double)0.06f * value )  (calls stubbed)
  @32284e51           Confidence Victory gate: now / max <= (double)0.3f  (apply @32284e62, skip @32284e96)
  @32284f2a           Pride Victory gate: now / max against its SA_MAX_HP_REL value  (apply @32284f3b, skip @32284f6f)
  @3229a7a1           Rage Fungus gate in 3229a530: lock count against SA_EFFECTIVE_COUNT  (apply @3229a7c9, skip @3229a7ef)
  @3228518f-322852db  socket stat rates by g_pMain state (June numbering; the port matches March's states by name)
  static              CalcExtraAttackPower 3226e530's three jump tables against the port's case lists
Each call inside a range is replaced by a stub that loads the call's float result and returns past it. C float assignments
round to float; x87 intermediates are 53-bit (control word 0x027F), which Python's doubles reproduce here.

Usage: python june2017_re/statcalc/emu_final_damage_a.py      (one line per mismatch, then the summary line)
"""
import ctypes
import itertools
import math
import os
import re
import struct

from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_EBP, UC_X86_REG_EBX

EXE = r"F:\elsword stuff\elsword_2014\els_2014\237311\14JUN17\data\unpacked_x2.exe"
TRUNK = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BASE = 0x31aa0000
TEXT_RVA, TEXT_RAW, TEXT_SIZE = 0x1000, 0x400, 0x1cbba00
STACK, DATA, STUB, OBJ = 0x10000000, 0x20000000, 0x30000000, 0x40000000
ESP = STACK + 0x8000
CW, VAL = STUB + 0x800, STUB + 0x804

# March CX2Main::X2_STATE (X2Lib/X2Main.h:764-797, values from a cl /P of X2GameUnit.cpp's includes, 2026-11-12)
MARCH_STATES = ["XS_INVALID", "XS_START_UP", "XS_LOGIN", "XS_JOIN", "XS_LOADING", "XS_SERVER_SELECT", "XS_CREATE_UNIT", "XS_PVP_LOBBY",
                "XS_PVP_ROOM", "XS_PVP_GAME", "XS_DUNGEON_ROOM", "XS_DUNGEON_GAME", "XS_PVP_RESULT", "XS_DUNGEON_RESULT", "XS_VILLAGE_MAP",
                "XS_SQUARE_GAME", "XS_TRAINING_SCHOOL", "XS_TRAINING_GAME", "XS_TRAINING_RESULT", "XS_CLASS_CHANGE", "XS_ARCADE_RESULT",
                "XS_ARCADE_GAME", "XS_ARCADE_LOBBY", "XS_ARCADE_ROOM", "XS_BATTLE_FIELD", "XS_WEDDING_GAME", "XS_END"]
PORT_SOCKET_STATES = {"XS_PVP_GAME", "XS_DUNGEON_GAME", "XS_TRAINING_GAME", "XS_BATTLE_FIELD"}
# March CX2DamageManager::EXTRA_DAMAGE_TYPE values the port names (X2DamageManager.h:382-504)
EDT = {"EDT_FIRE_SMASH": 10, "EDT_COLD_SMASH": 11, "EDT_EVENT_SMASH": 12, "EDT_EVENT_SMASH_2": 30, "EDT_CAMILLA_SMASH": 57}
ET = {"ET_BLAZE": 1, "ET_WATER": 2, "ET_DARK": 6}   # CX2EnchantItem::ENCHANT_TYPE (X2EnchantItem.h:13-21)


def f32(x):
    return ctypes.c_float(x).value


def div(a, b):
    if b == 0.0:
        if a == 0.0 or math.isnan(a):
            return math.nan
        return math.copysign(math.inf, a) * math.copysign(1.0, b)
    return a / b


def same(a, b):
    return (math.isnan(a) and math.isnan(b)) or struct.pack("<f", a) == struct.pack("<f", b)


D5E8, D006, D03 = float(f32(5e-8)), float(f32(0.06)), float(f32(0.3))


def june_states():
    src = open(os.path.join(TRUNK, "june_src_fixed", "data036_kom", "Enum.src.lua"), encoding="utf-8-sig").read()
    body = re.search(r"\nX2_STATE = \{(.*?)\}", src, re.S).group(1)
    return {int(v): k for k, v in re.findall(r"(XS_\w+) = (\d+)", body)}


class Emu:
    def __init__(self):
        data = open(EXE, "rb").read()
        self.text = data
        mu = Uc(UC_ARCH_X86, UC_MODE_32)
        mu.mem_map(BASE + TEXT_RVA, (TEXT_SIZE + 0xfff) & ~0xfff)
        mu.mem_write(BASE + TEXT_RVA, data[TEXT_RAW:TEXT_RAW + TEXT_SIZE])
        for a, n in ((STACK, 0x10000), (DATA, 0x1000), (STUB, 0x1000), (OBJ, 0x20000)):
            mu.mem_map(a, n)
        mu.mem_write(CW, struct.pack("<H", 0x027f))
        self.mu = mu
        self.stubs = {}
        self.stops = set()
        self.hit = None
        mu.hook_add(UC_HOOK_CODE, self._hook)

    def _hook(self, mu, address, size, user):
        if address in self.stops:
            self.hit = address
            mu.emu_stop()

    def stub(self, target, add_esp=0, fld=None):
        """fninit; fldcw 0x027F; [add esp, n]; [fld dword VAL]; jump to target (push / ret). One address per stub:
        Unicorn keeps translated code, so a stub is never rewritten in place (only VAL, which is data)."""
        if fld is not None:
            self.mu.mem_write(VAL, struct.pack("<f", fld))
        key = (target, add_esp, fld is not None)
        if key not in self.stubs:
            code = b"\xdb\xe3" + b"\xd9\x2d" + struct.pack("<I", CW)
            if add_esp:
                code += b"\x83\xc4" + bytes([add_esp])
            if fld is not None:
                code += b"\xd9\x05" + struct.pack("<I", VAL)
            code += b"\x68" + struct.pack("<I", target) + b"\xc3"
            addr = STUB + 0x40 * len(self.stubs)
            self.mu.mem_write(addr, code)
            self.stubs[key] = addr
        return self.stubs[key]

    def run(self, target, until, add_esp=0, fld=None, stops=()):
        self.stops, self.hit = set(stops), None
        self.mu.emu_start(self.stub(target, add_esp, fld), until)
        return self.hit if self.hit is not None else until

    def wf(self, addr, v):
        self.mu.mem_write(addr, struct.pack("<f", v))

    def rf(self, addr):
        return struct.unpack("<f", self.mu.mem_read(addr, 4))[0]

    def wd(self, addr, v):
        self.mu.mem_write(addr, struct.pack("<I", v & 0xffffffff))

    def esp(self):
        self.mu.reg_write(UC_X86_REG_ESP, ESP)


# ---- the port's expressions (X2GameUnit.cpp, SERV_IRUHADEV_JUNE_STATCALC_FINAL_DAMAGE) ----

def port_world(r, a, b):
    rate = f32(f32(r) - 1.0 + D5E8)              # CalculateWorldBuffIncreasingRate
    return f32(a + a * rate), f32(b + b * rate)


def port_manaflow(mp, maxmp, value, p):
    rel = f32(1.0 + div(f32(mp), maxmp) * D006 * value)
    return f32(p * rel)


def port_confidence(now, mx):
    return div(now, mx) <= D03


def port_pride(now, mx, threshold):
    return div(now, mx) >= threshold


def port_rage(count, value):
    return float(count) <= value


def port_socket(state_name, att_user, this_user, rates, vals):
    p, m, pd, md = vals
    if state_name in PORT_SOCKET_STATES:
        if att_user:
            if rates[0] != 0.0:
                p = f32(p + p * rates[0])
            if rates[1] != 0.0:
                m = f32(m + m * rates[1])
        if this_user:
            if rates[2] != 0.0:
                pd = f32(pd + pd * rates[2])
            if rates[3] != 0.0:
                md = f32(md + md * rates[3])
    return p, m, pd, md


# ---- June ----

def june_world(e, r, a, b, el_bless):
    e.esp()
    e.wf(ESP + 0x18, f32(r))
    if el_bless:
        e.wf(ESP + 0x48, a); e.wf(ESP + 0x44, b)
        e.run(0x32285153, 0x32285189)
        return e.rf(ESP + 0x48), e.rf(ESP + 0x44)
    e.wf(ESP + 0x28, a); e.wf(ESP + 0x2c, b)
    e.run(0x322850f5, 0x3228512b)
    return e.rf(ESP + 0x28), e.rf(ESP + 0x2c)


def june_manaflow(e, mp, maxmp, value, p):
    e.esp()
    e.wf(ESP + 0x40, f32(mp)); e.wf(ESP + 0x28, p)
    e.mu.reg_write(UC_X86_REG_EDX, DATA)
    e.run(0x32285009, 0x32285019)                       # up to call vslot 0xdc (GetMaxMp)
    e.run(0x3228501b, 0x32285036, fld=maxmp)            # up to call 32206e50 (GetSkillAbilityValue, ret 0x10)
    e.run(0x3228503b, 0x32285055, add_esp=0x10, fld=value)
    return e.rf(ESP + 0x28)


def june_gate(e, target, apply_at, skip_at, maxhp, writes):
    e.esp()
    for off, v in writes:
        e.wf(ESP + off, v)
    hit = e.run(target, skip_at, fld=maxhp, stops=(apply_at, skip_at))
    return hit == apply_at


def june_rage(e, count, value):
    e.esp()
    e.mu.reg_write(UC_X86_REG_EAX, count)
    e.mu.reg_write(UC_X86_REG_EBX, OBJ)
    e.run(0x3229a7a1, 0x3229a7b9)                       # up to call 32007b10 (two pushes on the stack)
    hit = e.run(0x3229a7be, 0x3229a7ef, add_esp=8, fld=value, stops=(0x3229a7c9, 0x3229a7ef))
    return hit == 0x3229a7c9


def june_socket(e, state, att_user, this_user, rates, vals):
    main, att, this = DATA + 0x100, OBJ, OBJ + 0x10000
    agg_att, agg_this = OBJ + 0x9000, OBJ + 0x19000
    e.wd(main + 0x8c, state)
    for unit, agg, user, (r0, r1), off in ((att, agg_att, att_user, rates[0:2], 0x10), (this, agg_this, this_user, rates[2:4], 0x18)):
        e.wd(unit + 0x264, 0 if user else 1)
        e.wd(unit + 0x87b4, agg)
        key = 0x5a5a1234
        e.wd(agg + 0x2f4, key)
        for k, r in enumerate((r0, r1)):
            bits = struct.unpack("<I", struct.pack("<f", r))[0]
            e.wd(agg + 0x2f0 + off + 4 * k, bits ^ key ^ (agg + 0x2f0))
    e.esp()
    e.wd(ESP + 0x20, this)
    for off, v in zip((0x28, 0x2c, 0x48, 0x44), vals):
        e.wf(ESP + off, v)
    e.mu.reg_write(UC_X86_REG_ECX, main)
    e.mu.reg_write(UC_X86_REG_EBP, att)
    e.run(0x3228518f, 0x322852db)
    return tuple(e.rf(ESP + off) for off in (0x28, 0x2c, 0x48, 0x44))


def jump_tables(e):
    """CalcExtraAttackPower 3226e530: three switches on EDT - 10 (0..0x2f), dword table + byte index table each"""
    raw = lambda va, n: e.text[va - BASE - 0xc00:va - BASE - 0xc00 + n]
    out = []
    for dt, bt, n in ((0x3226e754, 0x3226e75c, 2), (0x3226e78c, 0x3226e79c, 4), (0x3226e7cc, 0x3226e7d8, 3)):
        targets = [struct.unpack("<I", raw(dt + 4 * k, 4))[0] for k in range(n)]
        cases = {}
        for i, b in enumerate(raw(bt, 0x30)):
            if b != n - 1:
                cases.setdefault(targets[b], set()).add(i + 10)
        out.append(cases)
    weapon = set(out[0].get(0x3226e5ec, ()))
    resist = {1: out[1].get(0x3226e646, set()), 2: out[1].get(0x3226e657, set()), 6: out[1].get(0x3226e660, set())}
    scale = {0.2: out[2].get(0x3226e68b, set()), 0.4: out[2].get(0x3226e697, set())}
    smash = {EDT[k] for k in EDT}
    want_resist = {ET["ET_BLAZE"]: {EDT["EDT_FIRE_SMASH"]}, ET["ET_WATER"]: {EDT["EDT_COLD_SMASH"]},
                   ET["ET_DARK"]: {EDT["EDT_EVENT_SMASH"], EDT["EDT_EVENT_SMASH_2"], EDT["EDT_CAMILLA_SMASH"]}}
    want_scale = {0.2: {EDT["EDT_FIRE_SMASH"], EDT["EDT_COLD_SMASH"]}, 0.4: {EDT["EDT_EVENT_SMASH"], EDT["EDT_EVENT_SMASH_2"], EDT["EDT_CAMILLA_SMASH"]}}
    bad = 0
    for name, got, want in (("weapon / stat cases", weapon, smash), ("resist cases", resist, want_resist), ("scale cases", scale, want_scale)):
        ok = got == want
        bad += not ok
        print("TABLE %-20s %s june=%s port=%s" % (name, "ok " if ok else "BAD", sorted(got.items()) if isinstance(got, dict) else sorted(got),
                                                  sorted(want.items()) if isinstance(want, dict) else sorted(want)))
    return bad


def main():
    e = Emu()
    n = bad = 0

    def check(label, j, p):
        nonlocal n, bad
        n += 1
        jt = j if isinstance(j, tuple) else (j,)
        pt = p if isinstance(p, tuple) else (p,)
        ok = all((same(a, b) if isinstance(a, float) else a == b) for a, b in zip(jt, pt))
        if not ok:
            bad += 1
            print("MISMATCH %s june=%r port=%r" % (label, j, p))

    rates = [0.0, 0.5, 1.0, 1.05, 1.1, 1.2, 1.5, 2.0, -1.0, 0.99999994, 1.00000012]
    stats = [0.0, 1.0, 1234.5, 98765.0, 1e6, -10.0, 3.4e38]
    for r, a, el in itertools.product(rates, stats, (False, True)):
        b = f32(a * 0.75)
        check("world r=%r a=%r el=%d" % (r, a, el), june_world(e, r, a, b, el), port_world(r, a, b))

    for mp, mx, v, p in itertools.product([0.0, 1.0, 250.0, 999.0, 1000.0, 300.5], [1000.0, 300.0, 1.0, 0.0], [0.0, 1.0, 2.5, 4.0], [1000.0, 54321.7]):
        p = f32(p)
        check("manaflow mp=%r max=%r v=%r p=%r" % (mp, mx, v, p), june_manaflow(e, mp, mx, v, p), port_manaflow(mp, mx, v, p))

    hp = [(3.0, 10.0), (0.3, 1.0), (f32(0.3), 1.0), (29999.0, 100000.0), (30000.0, 100000.0), (30001.0, 100000.0), (1.0, 3.0),
          (5.0, 10.0), (0.0, 10.0), (10.0, 0.0), (math.nan, 10.0), (299.0, 1000.0), (301.0, 1000.0), (8.0, 10.0), (7.9999995, 10.0)]
    for now, mx in hp:
        now, mx = f32(now), f32(mx)
        j = june_gate(e, 0x32284e51, 0x32284e62, 0x32284e96, mx, [(0x18, now)])
        check("confidence now=%r max=%r" % (now, mx), j, port_confidence(now, mx))
        for th in (0.0, 0.3, f32(0.3), 0.5, 0.8, f32(0.8), 1.0, math.nan):
            th = f32(th)
            j = june_gate(e, 0x32284f2a, 0x32284f3b, 0x32284f6f, mx, [(0x18, now), (0x40, th)])
            check("pride now=%r max=%r threshold=%r" % (now, mx, th), j, port_pride(now, mx, th))

    for count, value in itertools.product(range(0, 8), [0.0, 1.0, 2.0, 3.0, 2.5, 5.0, math.nan]):
        check("rage fungus count=%d value=%r" % (count, value), june_rage(e, count, value), port_rage(count, value))

    jnames = june_states()
    gap = 0
    for s in sorted(jnames):
        name = jnames[s]
        for att_user, this_user in itertools.product((True, False), repeat=2):
            for rr in ((0.1, 0.2, 0.15, 0.05), (0.0, -0.5, 0.0, 1.0), (0.25, 0.0, 0.3, 0.0)):
                vals = (f32(1000.5), f32(2000.25), f32(800.0), f32(600.0))
                j = june_socket(e, s, att_user, this_user, rr, vals)
                pn = name if name in MARCH_STATES else None
                p = port_socket(pn, att_user, this_user, rr, vals)
                if name == "XS_PVE_GAME":     # June state 23: no March state of that name (March's 23 is XS_ARCADE_ROOM)
                    gap += 1
                    continue
                check("socket state=%d %s att_user=%d this_user=%d rates=%r" % (s, name, att_user, this_user, rr), j, p)

    tbad = jump_tables(e)
    print("EMU_FINAL_DAMAGE_A %s: %d input sets, %d mismatch(es), %d table mismatch(es); %d XS_PVE_GAME set(s) not compared (no March state)"
          % ("OK" if bad == 0 and tbad == 0 else "FAIL", n, bad, tbad, gap))


if __name__ == "__main__":
    main()
