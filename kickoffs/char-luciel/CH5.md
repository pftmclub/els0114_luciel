# June character Lu/Ciel (Stage 8) - Phase CH5 - UI and data

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase char-luciel CH5` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1 review, DU5 review |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_UI` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL UI`) (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |
| Review | kickoffs/char-luciel/CH5-review.md (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Starting context" "Phase CH5 - UI and data"
```
Then the planning digests in `june2017_re/char_luciel/` the task names, and the Done notes of the phases this one depends on
(their routed gaps). Verify a digest line before building on it.

## Task

Do "Phase CH5". Hand-off: a select-screen pass of all ten characters, and both bodies' skill / hyper cut-ins and portraits.

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

## Amended by CH0 (2026-11-02)

- **Checklist rows** (`ch0_checklist_rows.tsv` col 5): §3 L153 EmotionData, EventScene* (Lu/Ciel rows); §7t L617-622 skill cut-in names -> June .gfx: replace s_SkillCutInSet and the InitState index; §7ah L827 class statics Static_LucielSwitchingGuide / g_pStatic_LucielCooltime in the 10-s; §7aj L862-864 slot data type 6 (328d9b00 / 328da050): Luciel transform slot, ctor caller 3200f; §7aj L865 Luciel's portrait 328dcba0 / 328db7a0: form flag at tree +0x8c from unit +0xe40,.
- Adopt `DLG_LUCIEL_TRANSFORM_SKILL_SLOT.lua` and `DLG_Luciel_CombinationPoint.lua` (data034). The switching guide (`322e6300`) is CH1a0's, the transform HUD CH2d20's.

## Amended by NU1 (2026-11-14)

- The unit-picture loops now walk June's 12 pictures (`X2StateDungeonRoom.cpp`, `X2StatePVPResult.cpp`, `X2StatePVPRoom.cpp`; null-safe), but
  the live dialogs are March's with 9: `DLG_PVP_Result_Card_Back.lua`, `DLG_PVP_Result_Card_Back_MatchPvp.lua`, `DLG_PVP_Room_Character_Slot.lua`
  (data034, no `LUCIEL` key). June's copies (`june_src_fixed/data034_kom/*.src.lua`) carry `LUCIEL_DUEL` etc. Adopt them (`adopt-june`, asset
  rule), then a Lu/Ciel shows his picture in the dungeon-room slot. PvP is not reachable offline, so only the dungeon-room slot is testable.

## Amended by char-luciel CH1l3 (2026-11-15)

- `ShowActiveSkillCutInAndLight` ( `32b16940` ) reads file-local tables in `_CONST_LUCIEL_CH1L3_` of `X2GULuciel.cpp` ( `s_aSkillCutIn` 4 sets x 3, `s_aTransformedFormSkillCutIn`, `s_aEventSkillCutInFirst` / `Second`; June `337ad268` / `337ad4a8` / `337ad148` / `337ad1d8`,
  names with no extension ). This phase confirms the asset names against the staged `.gfx` and the set index ( `SetSkillCutInSetIndexByUnitClass` ), and moves the tables if the creation-screen work needs them; the customize-manager cut-in override stays a stand-in ( backlog Stage 12 ).

## Amended by char-luciel CH1o3 (2026-11-15)

- **Dungeon-clear picture ( June `324ce260` @`324ce351`-`324ce3f1` ):** for Lu/Ciel's classes it shows Ciel's picture ( `HQ_Dungeon_Clear_Ciel_TGA`, `HQ_Dungeon_Clear_LUCIEL_ROYAL_GU...`, `HQ_Dungeon_Clear_Ciel_tga` ) when `g_pX2Game->GetWinSpeechForm()` ( June game `+0xb54` ) is 1, else the default. CH1o3 wrote the member and the roll in `CX2GUUser::GetRandomWinSpeech` ( flag `SERV_IRUHADEV_JUNE_CHAR_DUO_WIN_SPEECH` ); add the cases to March's clear-picture switch.
- **Dungeon ending speech ( June `327e0040`, from the DungeonData loader `327e0f80` @`327e625e` ):** `ENDING_SPEECH` gains a `LUCIEL = { { LU = id, CIEL = id }, ... }` set ( June `DungeonData.src.lua`, e.g. lines 454-463 ), read into a vector of two-string vectors; March's `X2DungeonManager.cpp` `ENDING_SPEECH` reader has no `LUCIEL` key. Port the reader and find which body its consumer shows.
