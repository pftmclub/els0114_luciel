# June spa villages - Phase S2a - Code events, spa village plumbing, Ruben omissions

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-spa S2a` |
| Plan doc | `JUNE_SPA_PHASES.md` (branch `main`) |
| Depends on | S0, S1; not beside another session editing `X2OfflineDB.*`, `X2LocationManager.*` or `X2TFieldGame.*` |
| Flag | `SERV_IRUHADEV_OFFLINE_CODE_EVENTS`, `SERV_IRUHADEV_JUNE_SPA_ENTRY`, `SERV_IRUHADEV_JUNE_SPA_DEBUG` |
| Log tag | `[JUNE_SPA]` (`python tools/logs.py --tag JUNE_SPA`) |
| Review | inside `kickoffs/june-spa/S2b-review.md` |
| Hand-off | batch at S2b (write section `S2a` in `kickoffs/june-spa/playtest.md`) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_SPA_PHASES.md "Rules" "Starting context" "Design" "Decisions" "Phase S2a - Code events, spa village plumbing, Ruben omissions" "Phase S0 - Done" "Phase S1 - Done" "Play-test hand-off"
```

Then the map rows marked `S2a` in `june2017_re/spa/spa_map.tsv` (`awk -F'\t' '$5 ~ /^S2a/' june2017_re/spa/spa_map.tsv`).

## Task

1. **Code-event switch** ("Design"):
   - Back up the save (`elsdb.py`).
   - Add the `code_events` migration, seeded `( 31, 1, 'CEI_SPA_ON_OFF' )`.
   - Send `EGS_UPDATE_CODE_EVENT_NOT` at login in June's shape, after the unit list. Read the client's handler `31f00970`, and what C9h-2d
     ported of it, first.
   - Add `tools/elsdb.py code-event list | on <id> | off <id>`.
   - Register June's `CEI_SPA_ON_OFF = 31` beside March's `CEI_ALLOW_ELESIS_FIRST_CLASS_OLD_AND_NEW_YEAR_EVENT` under the flag and use it
     (S0's rule note: same value, different meaning).
2. **`SpaData`** (`32501bb0` @325024dc-32502f9e): read it in March's local-map loader (`X2LocationManager.cpp:265-365`), pass it to the
   templet and call `VillageParsing` for it as June does, so `GetVillageMapTemplet( 1500 / 1501 )` is not null.
3. **The S2a rows outside the field game**, June's spa cases inside March's functions: `IsValidWarpField`, `WarpByCommand` position 19,
   `CX2UIInventory::CreateWarpDest`, `CX2StateMenu::CreateWarpDest` (with June's code-event 31 gate on both rows), the warp-destination
   level-10 gate in `CX2StateMenu::UICustomEventProc`, `LoadFieldImage`, `LoadUI`'s loading-screen clause, `OpenFieldName`, both
   `X2FieldMapBase` functions, the minimap channel combo, the offline enum seed. Register `VMI_SPA_RUBEN` wherever C++ lacks it.
4. **Emulator:** last position per "Decisions" 2 (saved like any village; confirm `Handler_EGS_STATE_CHANGE_FIELD_REQ` needs no change, and
   that login loads a saved 1500 / 1501 once the templet exists).
5. **Debug lines** under `_DEBUG`, throttled: `code-event send ids=<...>`, `spadata local=<id> village=<id>`.

No door code (S2b), no soak (S3b), `+0x624` stays null (S4b).

Run the gates with `tools/finish.py --with SERV_IRUHADEV_OFFLINE_CODE_EVENTS SERV_IRUHADEV_JUNE_SPA_ENTRY SERV_IRUHADEV_JUNE_SPA_DEBUG`.
Probe build only (full build skipped: batch at S2b).

Write section `S2a` of `playtest.md` (`python tools/playtest.py check june-spa S2a` PASS). The checks:
- console warp into Ruben spa (position 19) and Ranox spa (20): loading image, field title, minimap, camera;
- the warp item / state menu lists show both spa rows with code event 31 on; `elsdb.py code-event off 31`, relog: the rows and the signs are gone;
- a relog while standing in a spa loads the spa.

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
