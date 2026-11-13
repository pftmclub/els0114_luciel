# June character Lu/Ciel (Stage 8) - Phase NU3 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel NU3 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase NU3 done (its Done note and commit) and the user's NU3 play-test |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_MINOR_UNIT_TEMPLET` (parser), `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_DATA` (load line) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DATA`) (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "Phase NU - New-character foundation (NU1-NU4)" "NU3 - Done (2026-11-14)"
```
Then `june2017_re/char_luciel/nu0_templet_keys.tsv` (the 18 keys, June offsets and readers) and the NU3 kickoff's "Amended by NU0" list.

## Task

Review NU3's commit against `nu0_templet_keys.tsv` and June's parser `CX2UnitManager::AddUnitTemplet_LUA` `3237aa40`:
- **The members** (`X2Unit.h`, `UnitTemplet`): 17 new ones, June's key names and types (two name wstrings, 5 int promotion items, 7 motion
  wstrings, 2 floats). The three minor cut-ins were already parsed (`JUNE_HYPER_CUT_IN_TIME`): no second copy.
- **The parse** (`X2UnitManager.cpp`): each key's default equals June's miss value (decompile: name ids `-1` -> empty string via the string
  table, ints `0`, wstrings `L""` (`3344f1f4`), floats `0`), none fails the row, and the order is June's (names between `name` and
  `description`; minor items / motions after the game motions; rotations after `introMovieFIleName`). Spot-check 2-3 in Ghidra (read-only),
  e.g. `@3237aed7` (majorUnitName), `@3237cde7` (minor weapon item), `@3237f0da` (m_fLobbyRotationY).
- **The rows**: data036 `UnitTemplet.lua` = the 59 earlier rows byte for byte plus June's `UnitTemplet.src.lua:1723-2024` verbatim
  (re-run the luajit parsed-value diff the Done note describes). Every row carries the 16 keys March's parser fails without.
- **The `DATA` line**: Lu/Ciel rows 7 of 66, second-body rows 7 (no other class has a minor name), both names from the string table.
- **Enum.lua**: `UT_END = 13` (June `Enum.src.lua:313`), stale comment gone; no Lua reads it.

Hygiene: `python tools/flagcheck.py JUNE_CHAR_MINOR_UNIT_TEMPLET --with JUNE_CHAR_LUCIEL_DATA --range <NU3 code commit>^..<NU3 code commit>`.
Paste its `files:` / `in this flag's files:` / `RESULT:` lines; explain every OTHER row and deleted line. `python tools/junediverge.py --check-all`
must PASS; `luasrc.py ledger --check` and `core --check` PASS. Read the user's NU3 play-test logs.
Fix clear transcription errors under the same flags (then `tools/build.py --deploy`, play-test only if the fix changes behaviour); hand design
decisions to the user. Write "NU3 - Review" into the plan doc, update the NU3 row in STATUS.md (NU4 needs NU2's review too; DU1 becomes ready
on this review), check the NU4 / DU1 kickoffs ("Amended by NU3"), and commit.
