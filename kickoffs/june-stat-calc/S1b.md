# June damage and stat calculation - Phase S1b - max HP, hit / dodge, the ability map

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc S1b` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | S1 review; backlog Stages 7 and 8 closed (user, 2026-10-27); D6 review; one S / R phase at a time |
| Flag | `SERV_IRUHADEV_JUNE_STATCALC_UNIT_STAT` (`ResetMaxHP`), `SERV_IRUHADEV_JUNE_STATCALC_HIT_DODGE` (+ `_STATCALC_DEBUG`) |
| Log tag | `[JUNE_STATCALC]` (`python tools/logs.py --tag JUNE_STATCALC`) |
| Review | `kickoffs/june-stat-calc/S1b-review.md` (the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Starting context" "Design: the map and the verification" "Phases S1-S4, R1 - Stat sheet and resource gain (after Stages 7 and 8)" "Phase S0 - Done" "Play-test hand-off"
```

Then the map's rows with `phase = S1b`.

## Task

Port the map's `phase = S1b` rows whole (June's body inside March's function, March's in `#else`). Added by S0 (2026-11-16):
- **`ResetMaxHP` `31feba70`** (called from `InitComponent` before vslot 0x664): gage max HP = Stat [6], then the two room-mode multipliers
  (room `+0xd8` == 4 with event 0x891 and my slot `+0x58` / `+0x10` -> x 0.7; room `+0xd8` == 5 with room `+0xf8` == 0x8a9 and slot
  `+0x58` -> x 1.5). Name the room members and events, port them as June has them, and say whether offline can reach either.
- **`CalcHitDodgePercent` `32610a00`** whole: June has no Chung Deadly Chaser accuracy block and reads Raven Blade Master's Exquisite
  Swordmanship by 24406 (`RNSI_P_RST_EXQUISITE_SWORDMANSHIP`); name both in the hand-off for char-chung / char-raven (rule 5). With it, the
  dodge roll's index offset in the four collision paths (@326253e2 / @326285b9 / @3262b2fe / @3262f11a): an NPC defender whose stat byte
  `+0xe8` is set rolls `SRO_HIT_DODGE` + the attacker's (or NPC attacker's owner's) `GetRandomInt( 0 )`. The byte is 1 only when the NPC's
  stat came from an ExtraNPCStat row (`323752f0`, called by the fetch `32384d10`); the port knows that from `CX2UnitManager::m_mapExtraNPCStat`.
- **The ability map `32358970`** (`CX2Unit +0xaac`, table `g_pData +0x5a4` `32608cb0`, UI `3260a360` / `3260ceb0`): identify the packet
  that fills it and the table's loader. If it is server data, close the row as June's absent value 0 (memory design-missing-june-pieces);
  if the table is client data, port it with every reader the map lists (keys 1 / 2 / 4 / 5 / 6 / 11 / 13-16).

Per row: list every reader and refresher and move them together (rule 3); one emulation script per pure function under
`june2017_re/statcalc/`; set the row to `June`. Play-test on every ported character at level 99, with and without sockets: the
hand-off names the numbers the character window and the `[JUNE_STATCALC]` lines must show.

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
