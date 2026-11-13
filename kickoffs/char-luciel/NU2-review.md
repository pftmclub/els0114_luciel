# June character Lu/Ciel (Stage 8) - Phase NU2 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel NU2 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase NU2 done (its Done note and commit), its play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_UNIT_CLASS_WORD` (Always.h + `ServerDefine.h` + `X2ServerProtocol_2010.vcxproj` X2TOOL) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL NEWUNIT`) (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "Phase NU - New-character foundation (NU1-NU4)" "NU2 - Done (2026-11-14)"
```
Then `june2017_re/char_luciel/nu0_unit_class_width.tsv` (202 rows) and the NU2 kickoff's "Amended by NU0" list.

## Task

Review NU2's commit against `nu0_unit_class_width.tsv` and the Done note's verdicts:
- **Members.** Every live unit-class member is a `short` with the flag and March's `char` in `#else`. Re-derive "live" yourself: `cl /P` a
  stdafx-only X2Lib probe with the flag and list every `m_c*Class` member that survives (the Done note counts 35, including
  `ClientPacket_Global.h`'s `KEGS_FINAL_DELETE_UNIT_ACK`, which the tsv missed). A live `char` unit-class member left out is a finding; unit
  types (`m_cUnitType`, `m_cAttUnitType`) are not classes.
- **Layout.** June's four offsets (`KUnitInfo` +0x15 / transcendence +0x17, `KRoomUserInfo` +0x21, `KFieldUserInfo` +0x24, `KSquareUserInfo`
  +0x4a) in both projects; spot-check two in Ghidra (read-only), e.g. `3235eb7b`, `32361e4f`, and that June's serializer writes 2 bytes there.
- **Narrowing.** No path still squeezes a class through a `char`: the emulator's `(char)` casts, the four widened parameters
  (`MakeDefaultUnitInfo`, `AllocateRelationDummyUnit`, `SetCharIcon`, `CX2Main::ChangeUnitClass`), X2Room.cpp:1105, X2UIPersonalShopBoard.cpp:2258.
  Re-grep `(char)` / `static_cast< char >` near `UnitClass` and any `char` local or parameter that receives a member.
- **Left rows.** The dead `UPGRADE_SKILL_SYSTEM_2013` `#else` (X2SkillTree, X2UIInventory), `KPartyUserInfo` 4683, the Chung event / guild invite
  packets, `GetChungUIClass`, `BOT_CAST`'s initialisers, the server-only headers: confirm each verdict.
- **Second homes.** Always.h, `ServerDefine.h` and the vcxproj (`NAME=`, empty) define the same flag; `SECOND_HOMES` has the row.

Hygiene: `python tools/flagcheck.py SERV_IRUHADEV_JUNE_CHAR_UNIT_CLASS_WORD --range <NU2 commit>^..<NU2 commit> --files <the NU2 code files>
KTDXLIB/Always.h KncWX2Server/Common/ServerDefine.h`. Paste its `files:` / `in this flag's files:` / `RESULT:` lines; explain every OTHER row,
deleted line and EOL row (the Done note expects 6 EOL rows in the two guild structs). `python tools/junediverge.py --check-all` must PASS.
Read the user's NU2 play-test logs if they ran it (NEWUNIT line for the scratch class-200 unit; nothing new for existing characters), and
confirm the scratch unit was deleted from the save. Fix clear transcription errors under the same flag (then `tools/build.py --deploy`,
play-test only if the fix changes behaviour); hand design decisions to the user. Write "NU2 - Review" into the plan doc, update the NU2 row in
STATUS.md (NU4 needs NU2 and NU3 reviews), check the NU4 kickoff, and commit.
