# June damage and stat calculation - Phase D4b - user DamageReact: from the final damage to the HP change

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc D4b` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | D4a; not beside another phase that edits `X2GUUser.*` |
| Flag | `SERV_IRUHADEV_JUNE_STATCALC_DAMAGE_REACT` (D4a's) (+ `_STATCALC_DEBUG`) |
| Log tag | `[JUNE_STATCALC]` (`python tools/logs.py --tag JUNE_STATCALC`) |
| Review | `kickoffs/june-stat-calc/D4-review.md` (D4b writes it; it covers D4a and D4b) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Starting context" "Phase D4 - DamageReact: from the final damage to the HP change" "Phase D4 - Cut (2026-11-12)" "Phase D4a - Done" "Phase D0 - Done" "Play-test hand-off"
```

Then the `user` rows of `june2017_re/statcalc/dr_blocks.tsv`, the map rows whose `phase` names `D4b`, and
`june2017_re/c3b_damage_react_sites.tsv`. June: the listing `june2017_re/buffre/dec/320462a0.s` (Ghidra's decompiler fails on this
function; use `disasm_range.py` and the listing).

## Task

Port the damage arithmetic of user `DamageReact` `320462a0` from the `GetFinalDamage` call (`@3204843b`) to the defender's `UpNowHp`
(`@32049b30`; D0's `@32048fab` is the attacker's HP-drain call) and the tutorial clamp after it, one `dr_blocks.tsv` row at a time:

- Every `absent` and `mixed` row (`32048446`, `320489f3`, `32048cdb`, `320492a3`, `320492e3`, `3204963f`, `3204968f`, `32049779`,
  `32049998`, `32049a1f`): identify first; class bodies (Elesis provoke, Ara, Add, Lunatic Psyker) are called where June calls them and
  stay with their char ports (rule 5; `stand-in: <class> not ported` where the class is not).
- Every `unknown` row: re-read against the exe.
- `GetSocketReducedDamagePercentInCase` `32000320`: June's body goes into March's own function (live body
  `GetSocketReducedDamagePercent`); the dead twin and its callers go (its `june_march_divergent.tsv` row closes). Find which call in the
  basic-reduction block (`320487fe`, vslot `0x348`) reaches it; spot-check `GetBasicReducedDamagePercent` `3201aff0` (map row 19).
- The HP-rate overwrite (`@32049727`) keeps June's order verbatim; `322cfca0` and the tutorial clamp: check their place only.
- `REACT` debug lines as D4a's.

Amended 2026-11-13 by D4a (read its Done note first):
- The cut's "state 10" is unit type 10: `31e4d0e0` = `GetUnitType`, 10 = `UT_LUCIEL`. `32048cdb` and the `32aebaf0` multiply are Lu/Ciel's: write a
  `stand-in: Lu/Ciel not ported` comment as D4a did (checklist 7aw); DD `+0x5c0` is read only by Lu/Ciel `32b3d380`.
- Slot `0x378` (`@320489f3`) is `ApplyEnchantResist( DD +0x4fc m_ResistType, &damage )` (`3226dd20`, = March): check only its place.
- `GetDrainHpBuffValue` (`32071c80`) is already replaced by D4a (divergence row); check only its place in the user order.
- The `REACT` line: copy D4a's trace (`_CONST_GUNPC_D4A_` in X2GUNPC.cpp) into a `_CONST_GUUSER_D4B_` namespace with the user's groups.
- Play-test both sides: D4a added no hand-off of its own. NPC lines to see: `[JUNE_STATCALC] REACT att= def=` per monster, `dmg` = `hp` drop; a
  Henir Nasod element (1805 / 1806) takes 1 while its barrier (1767 / 1768) has flag 3; Eve's Electronic Field or Nasod Watch takes 0 in a dungeon;
  `DAMAGED_FUNC` runs on Treasure_Mimic / FOOLSDAY NPCs if a reachable dungeon spawns them (else say so).

Gate: the play-test of the phase section, for both sides (ten logged hits each way; a unit with damage-reduction sockets and one
without): `REACT`'s HP change equals the `GFD` damage after the listed reductions. Full build, `DEPLOY OK`, `EXESTR OK` for the `REACT` tag.

No hardcoded enum literals (user, 2026-10-26; CLAUDE.md): an id that has a name in an Enum (June's
`Enum.lua`, or a C++ enum) is never written as a number, in any new or touched code: use the member, and
register it under a `SERV_IRUHADEV_` flag with June's value and name if C++ lacks it. A value with no Enum
entry stays a number with a one-line comment. The one exception is a table whose rows are the data itself
(a `.tsv` of ids), not code.

No `June` prefix (user, 2026-10-28; CLAUDE.md): June's body goes inside March's own function under the flag,
March's in `#else`. A function March lacks gets a name in the class's own pattern, never with `June` in it.
Every name (locals and constants too) says what it is in context: no Ghidra address, phase tag or `June` in an identifier.
Call a March function from June code only if June's namesake does the same; otherwise replace March's definition.
Run `python tools/junediverge.py --list` before transcribing; a newly found divergent function gets its `june_march_divergent.tsv` row and the replacement in the same commit.
