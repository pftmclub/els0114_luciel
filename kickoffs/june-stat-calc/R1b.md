# June damage and stat calculation - Phase R1b - MP cost and MP rate

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc R1b` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | R1 review; Stages 7 and 8 need not be closed (user, 2026-11-16); D6 review; one S / R phase at a time |
| Flag | `SERV_IRUHADEV_JUNE_STATCALC_MP_COST` (+ `_STATCALC_DEBUG`) |
| Log tag | `[JUNE_STATCALC]` (`python tools/logs.py --tag JUNE_STATCALC`) |
| Review | `kickoffs/june-stat-calc/R1b-review.md` (the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Starting context" "Design: the map and the verification" "Phases S1-S4, R1 - Stat sheet and resource gain (after Stages 7 and 8)" "Phase S0 - Done" "Play-test hand-off"
```

Then the map's rows with `phase = R1b`.

## Task

Port the map's `phase = R1b` rows whole (June's body inside March's function, March's in `#else`). Added by S0 (2026-11-16):
- **`GetActualMPConsume` `3203cf60`** re-read (claimed June by buff 16-step3b3) with **June's buff reader `32039a50`**: the port's
  `GetSkillRateByBuffBehavior` reads the ChangeConsumeMpRate factor dump, not the rate `CX2BuffChangeConsumeMpRate::SetFactor` `32e7cef0`
  stores (rule 3: setter and reader together; the class callers `X2GuEve.cpp:28187 / 44385` follow).
- **`ResetMPChangeRate` `31fd9e00`:** June's three gates (byte `+0x6894`, event 0x8f4, game mode `+0xd8` != 8).
- **`CX2GUUser::SetMPChangeRateValue`** (the per-frame rate from buffs and sockets, `X2GUUser.cpp:48554`): locate June's twin and port it;
  the gage tick `32a344a0` is already June (char-luciel CH1a1). `SERV_IRUHADEV_MP_REGEN_BOOST` stays commented out.

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
