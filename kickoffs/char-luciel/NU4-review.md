# June character Lu/Ciel (Stage 8) - Phase NU4 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel NU4 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase NU4 done (its Done note and commit) and the user's NU4 play-test |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_CREATE`, `SERV_IRUHADEV_JUNE_SELECT_VOICE_TABLE`, `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_STARTER_ITEMS` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL CREATE`) (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Phase NU - New-character foundation (NU1-NU4)" "NU4 - Done (2026-11-14)"
```
Then `june2017_re/char_luciel/nu0_create.tsv` (rows a1-s6, f1-f3) and the NU4 kickoff's amendments.

## Task

Review NU4's commit against `nu0_create.tsv` and June:
- **SelectLuciel** (`X2StateCreateUnit.cpp`) against June `325559f0`: guard, side-flag colour (0.08 / 0.13 / 0.88), background, viewer
  placement, `LobbyDownLanding`, class / type, dialog, stand board, movie, info dialog, voice. The promotion preview uses
  `EquipPromotionItemByUnitTemplet` (stand-in for June's preload `3254e410`): confirm the preview wears the templet's costume.
- **Rotation** (`X2Data.cpp`, `RotateYSelectCharacterType`) against June `31edf260`: the case table (re-read `charport_consts.py --fn
  31edf260` and the decompile's case lists), the templet-first test, Noblesse's minor / main split. Every class now reads its templet's
  `m_fLobbyRotationY` (56 of 59 March rows set it): spot-check that two March classes still face the camera.
- **Voices** (`X2UnitManager.cpp`) against June `32376270` / `32379ed0` / `3255f240` / `32550aa0`: suffix table, the `QuestComplete_`
  prefix, the random digit, first-row-wins insert. The class -> type read is a stand-in through the templet (June `31eca1d0`): confirm it
  agrees with June's switch for the 7 Lu/Ciel classes and two March jobs.
- **Create request** stand-in (base class of the type) and the stat fallback (`BaseClassOf`).
- **Data:** data034 `DLG_Unit_Create_Unit_Button_New.lua` = the T1 decompile + only Lu/Ciel's slot; data036 `Enum.lua` members; data057
  staged textures = June's bytes. `luasrc.py ledger --check` / `core --check` PASS; `luabind.py check` on the shipped button file,
  `SelectVoiceTable`, `DLG_Unit_Create_ClassChange_Button_Luciel` PASS.
- **Image rows** (`X2Data.cpp`, indices 59-65) against `char_image_rows.py 200:0x3f 37:0x40 38:0x41 39:0x4d 127:0x42 128:0x43 129:0x4e`.

Hygiene: `python tools/flagcheck.py JUNE_CHAR_CREATE --with JUNE_SELECT_VOICE_TABLE JUNE_CHAR_LUCIEL_STARTER_ITEMS --range <NU4 code commit>^..<NU4
code commit>`; paste its `files:` / `in this flag's files:` / `RESULT:` lines and explain every OTHER row. `python tools/junediverge.py
--check-all` must PASS. Read the user's NU4 play-test logs (this phase's lines only).
Fix clear transcription errors under the same flags (then `tools/build.py --deploy`, play-test only if the fix changes behaviour); hand design
decisions to the user. Write "NU4 - Review" into the plan doc, update the NU4 row in STATUS.md (CH1 needs this review), and commit.
