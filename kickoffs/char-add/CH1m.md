# June character Add (Stage 7) - Phase CH1m - Registration

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-add CH1m` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | CH1k2 review. One at a time with CH1-CH4: they all edit `X2GUAdd.*` |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_LIVE` |
| Log tag | `[JUNE_CHAR]` (`ADD CORE`) where the phase logs; otherwise none |
| Review | `kickoffs/char-add/CH1m-review.md` (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Rules" "Phase CH0 - Add map (read-only)" "Phase CH1 - Deploy unit: June script, motions and the class core"
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "CH1m - Done" "CH1m - Review"   # the worked example, once written; else Ara's
awk -F'\t' '$7=="CH1m"' june2017_re/char_add/ch0_class_map.tsv
```

## Task

Registration under `_LIVE`: June's InitState wrapper and body (CH0 step 0) against March's `InitState` (`X2GUAdd.cpp:303`), `InitStateCommon` (`:1549`) and `InitStateByUnitClass` (`:2314`), from CH0's registration map (every row passed `ch0c_listing_check.py`): the hyper-mode table names, START / WIN / LOSE roll from `INIT_COMMON_RANDOM_STATE`, the base event states (bounding states §7 l.472 / l.509, `USI_WAIT_STATE_CHANGE` l.428), the `_LIVE` gate. One-file probe with `_LIVE` defined; no full build (no play-test).

You are on Opus: delegate the reads, greps, known-address Ghidra dumps, probe compiles and flagcheck runs (x2-phase, *When the phase is Opus*). CP949 sources: `bytepatch.py` only.

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
Phase tag (user, 2026-11-09): every edited or new code block carries a one-line `// char-add <phase>` comment (at the start of an existing one-line comment, never a second line).

## Amended by CH0 (2026-11-13)

- Class map: **3** functions (0 listing-only; 3 `inside`, 0 `new`), filter `awk -F'\t' '$7=="CH1m"' june2017_re/char_add/ch0_class_map.tsv` (col 7 phase, col 5 inside / new, col 6 proposed name, col 9 carved y / n).
- Checklist rows (`june2017_re/char_add/ch0_checklist_rows.tsv` col 5): L480.
- `june2017_re/char_add/ch0_reg.tsv`: 195 rows = registrar `32a1e4f0` 118 + `32a24c20` 68 + InitState body `32a25a90` 4 (no wrapper) + base `3202b110` 5; `ch0c_listing_check` **194 / 194** (one identical pair, id 198 `ANSI_COMBO_NASOD_ARMOR_LANDING` at lines 136 / 237 of `32a24c20`). Hand rows: `~` ids 270 / 237 / 265; roll ids 9 / 11 / 12 (`pppuVar5`); hyper id 23 (`puVar4`, Start `32a31190`); id 130 `ANSI_WAIT_STATE_CHANGE`. June registers 5 names with no table (ids 86, 164, 279, 285, 286) and 5 `USI_EVENT_*` (111-116). `ADD_NASODRULER_STATE_ID` lacks 159 of June's 220 names; all 61 shared names differ in raw value, 27 still differ after the USI_END rebase (100 -> 152): register by June's id (`ch0_state_enum_names.tsv`). March's `InitStateByUnitClass` (`:2314`) and `ParseCommonRandomState` (`:272-298`) class switches follow June's registration.

## Amended by user report (2026-11-15)

- **Win pose does not return (`june-state-ids` S1 play-test, repeated after CH0b):** an Add (LunaticPsykerT, AddLP99) stays in the win pose on the dungeon result screen. `USI_WAIT_STATE_CHANGE` is not registered for him. CH1m registers it with the wait group and the hand-off adds: finish a dungeon on 9, 32 and 122 and the unit returns to the normal state on the result screen (log: the `[STATE_IDS]` unregistered line for his class must be gone).
