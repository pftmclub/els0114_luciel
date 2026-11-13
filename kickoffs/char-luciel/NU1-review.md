# June character Lu/Ciel (Stage 8) - Phase NU1 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel NU1 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase NU1 done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_UNIT_TYPE_RANGE` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL NEWUNIT`) (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "Phase NU - New-character foundation (NU1-NU4)" "NU1 - Done (2026-11-14)"
```
Then `june2017_re/char_luciel/nu0_unit_type_sites.tsv` (55 rows) and the NU1 kickoff's "Amended by NU0" list.

## Task

Review NU1's commit against `nu0_unit_type_sites.tsv` and the kickoff's amendments:
- **The enum.** `UT_END` is 13 with the flag and every member keeps its value (`UT_LUCIEL` 10, `UT_ROSE` 11, `UT_AIN` 12); flag off gives March's
  enum byte for byte. Grep every `UT_END` / `UT_END - 1` use again (`X2Lib`, `KTDXLIB`, `X2`): each one now reaches 13 without an edit, so
  confirm each live one is either a NU0 `widen` row or harmless (the Done note lists them: the cash-shop costume check, the two `PlaySound*`
  guards owned by NU4 / DU1, the census log). A live site the Done note does not name is a finding.
- **The script table.** 13 entries (index 10 `LUCIEL_DUO.LUA`, 11 / 12 NULL) and the `BOOST_STATIC_ASSERT` that replaces the compiled-out
  `ASSERT`; the skip path (no script, or not mounted) takes the NULL branch and logs once per type.
- **The class guards.** `X2ItemManager_Preprocessing.cpp` (June `325bd515`, 0..202) and `X2ItemStatCalculator.cpp` (June `328d6b30`, 100..136).
  `X2SocketItem.cpp:2899` was already June's (char-ara CHNa, `SERV_IRUHADEV_JUNE_CLASS_PATH`): confirm no second copy.
- **`ChangeUnitButtonInfo`.** June `32564d50 @325651bd-325651d0`: templet NULL or type outside 1..12 pops STR 707 with the same arguments.
- **Leave rows.** `X2OfflineStatTable.cpp` stays March's identity for the nine base classes (pinned to `UT_ADD`); `X2GameEdit.cpp` untouched.

NU1 is `18460a26` (code) + `6aa77bde` (docs) + the fix 1 commit (the NEWUNIT line moved to ErrorLog, misplaced include dropped).
Hygiene: `python tools/flagcheck.py JUNE_CHAR_UNIT_TYPE_RANGE --base 18460a26^ --files <the 6 NU1 code files> KTDXLIB/Always.h`. Paste its `files:` /
`in this flag's files:` / `RESULT:` lines; explain every OTHER row and deleted line. `python tools/junediverge.py --check-all` must PASS.
Spot-check 2-3 bounds in Ghidra (read-only), e.g. `3237a6b3`, `328d6b30`, `325651cd`. Read the user's NU1 play-test logs if they ran it.
Fix clear transcription errors under the same flag (then `tools/build.py --deploy`, play-test only if the fix changes behaviour); hand design
decisions to the user. Write "NU1 - Review" into the plan doc, update the NU1 row in STATUS.md (NU2, NU3, DU2 become ready), check the NU2 /
NU3 / DU2 kickoffs, and commit.
