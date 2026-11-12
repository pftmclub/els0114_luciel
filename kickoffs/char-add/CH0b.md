# June character Add (Stage 7) - Phase CH0b - Every June Add asset, whole, first

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase char-add CH0b` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | CH0 (and its amendments); char-elesis CH6 walk done (the game and save are in use until then) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_ASSET` only if CH0 found a March reader a June asset breaks; otherwise none |
| Log tag | `[JUNE_CHAR]` (`ADD DATA`) |
| Review | none (the play-test is the check) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Rules" "Phase CH0b - Every June Add asset, whole, first"
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "CH0b - Done"
head -5 june2017_re/char_add/ch0b_stage_list.tsv   # then filter by route as the amendment says
```

## Task

Do "Phase CH0b": every non-Lua row of `ch0b_stage_list.tsv` that CH0c did not stage, byte-exact (`kom.py stage-june`) into the kom CH0 named
(held motions packed and kept for CH1 final), every kom packed (`/elstools pack`) and copied (`/x2-kom-packed copy`; no ask; if the game runs, ask the
user to close it). The class-change dialogs through `adopt-june` and `luabind.py check`. Save `data\ErrorLog_pre_CH0b_add.txt` first. After a
`db_backup`, `createchar.py` a level-40 Nasod Ruler (9) and a level-99 Lunatic Psyker (122) (33 / 36 / 123 / 126 wait for CHNa). Gate: `python
tools/skillicons.py check <Add's skill-id range from CH0>` = PASS (stage what it lists; a June-side gap is a `JUNE_BUGS.md` row).

Done gate: the stage list re-runs 0 `missing` / `march-copy`; the play-test (one Add per March class, village and dungeon) shows no new `KEM_ERROR` /
`Motion Error` / missing texture or mesh against the pre-CH0b log; a costume, a weapon and three skills look right. Record each replaced March asset
in the Done note and `MODS.md`.

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

- Checklist rows (`june2017_re/char_add/ch0_checklist_rows.tsv` col 5): L768.
- Stage: `python tools/kom.py stage-june june2017_re/char_add/ch0b_stage_list.tsv` (7,686 rows). CH0 dry run: `STAGE-JUNE DRY RUN: 29 kom(s), 7686 staged (7637 new, 49 replaced, 0 identical), 0 refused`. New kom **data207** (57 June-only motions: `Motion_ADD_AT.x`, `_MM.x`, `_TT.x`, `_DE.x`, `Motion_Standard_Add.X`, ~50 `Motion_Add_Emotion_*`); `Motion_Npc_Add_Arctracer.x` -> data148; same-name motions go to data033 now: `Motion_ADD.x`, `_Common`, `_Field`, `_Lobby`, `Motion_Add_Emotion.x`.
- **Held for CH1 final** (`june2017_re/char_add/ch1_held_motions.tsv`, out of the list): `Motion_ADD_PT.X` (June renamed `APT_NA_Ready` / `APT_NA_preAttack` / three `SI_*_APT_*` skills to `NA_*` / `SI_*_AN_*` and moved them into `Motion_ADD.x`) and `Motion_ADD_LP.x` (June moved `SI_A_ALP_TWIRL_RUSH` / `_HYPER` into `_PT`): March's script plays the old names.
- Adopt (bytecode): `DLG_1ST_Class_Change_Guide_Add.lua`, `DLG_2ND_Class_Change_Guide_Add.lua` (new), `DLG_Class_Change_Add_AT` / `_DE` / `_LP` / `_MM` / `_PT` / `_TT.lua`, `Class_Change_Guide_Add.lua`; `DLG_Unit_Create_ClassChange_Button_Add.lua` is CH0c's. Already June's in March: `CREST_OF_TIME_Ring.dds` / `_Time.tga`, the Moonlight Rhapsody `.tga`s, `DLG_Add_DP_And_Mutation.lua`, `DLG_Transcendental_Change_Guide_Add.lua` (its `POPUP_TR_L_PSYKER` / `_MASTERMIND` / `_D_ESPER` pictures are in the list, data215).
- Skill icons: `python tools/skillicons.py check 106-106 900-900 29000-29060 29201-29233 29400-29420 29600-29605` = 149 misses on 7 sheets today (`DLG_Add_SKILL_ScreenShot_01-04`, `DLG_SKILL_ADD_Active_02-04`, .tga + .tet), all in the list: must PASS after the copy.
- Effect closure (`ch0_effect_closure.tsv`): 9 names exist in no June kom (June's own gaps): `HQ_CutIn_Ara_normal.tga`, `HQ_CutIn_Ara_Rage.tga`, `Energy.wav`, `Break.wav`, `Change.wav`, `Add_NA_Effect04.ogg`, `AddVoice_Stasis_Field04.ogg`, code literals `add_EFFECT_Cycle_Blr01.dds`, `ADD_PULSE_GATLING_Dron_Mesh_01.X`.
