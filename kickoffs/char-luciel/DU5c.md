# June character Lu/Ciel (Stage 8) - Phase DU5c - Both bodies on the create / select screens and the village avatar

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5c` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5b2 review (covers DU5b + DU5b2) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (DU5b's), or a new `SERV_IRUHADEV_JUNE_CHAR_DUO_AVATAR` for the square unit if DU5b's review prefers |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | `kickoffs/char-luciel/DU5c-review.md` (Opus 5.5 high; the phase writes it when done) |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "Phase DU - The two-body engine (DU1-DU5)" "DU5 - Done (2026-11-15)" "DU5b - Done"
```
Then `kickoffs/char-luciel/DU5.md` "Amended by NU0" (both-bodies and Square-unit bullets), `nu0_create.tsv` rows `d1`, `d8`, `s3`,
`nu0_type_switches.tsv` row `X2SquareUnit.cpp:220`, and `grep "(DU5)" june2017_re/funcmap.tsv`.

## Task

- **Create screen:** SelectLuciel `325559f0 @32555fe8` (viewer `+0xd7c`: `31edf260( class, 1 )` rotation of the minor skin, `PlayAnim( .., 2 )`), and the
  basic / promotion body meshes of Ciel (`nu0_create.tsv` `d8`). NU4 shows Lu only.
- **Select screen:** `32564400` (CreateServerSelectUnitViewerUI, `@32564883`) and `3255e570`.
- **Village avatar:** the rest of `CX2SquareUnit::SetMinorUnitForm` `323f3290` (DU5 ported the head): the SD mesh by form `323f1490( form, type )`
  and the viewer `+0x2ec` rebuild `@323f331e-323f3479`; then the NOT's `323fa700` / `323fad80` stand-ins in
  `CX2TFieldGame::Handler_EGS_CHANGE_UNIT_FORM_NOT`; June's 24-name SD mesh table `31ee3470` (array `3372c1e8`, `X2SquareUnit.cpp:220`).
- **Other players' form:** the three CX2Unit-from-user-info writers `32367330` / `32367da0` / `32368300` read a form byte at struct `+0xe8` / `+0x2a` /
  `+0x50`; only if the village or room needs it offline (no other players: likely route to DU5e with the packet fields).

Done gate: the create screen and the select screen show Lu and Ciel where June does; in the village the avatar becomes Ciel / Lu when the window's
switch is pressed (the form NOT), and keeps it after a relog.

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

## Amended by june-engine R14e (2026-11-15): select-screen onepiece missing (DU4 fallout, every class)

- **Symptom (R14e play-test):** `jtLordKnight` wears Spa Wear 163569 (save: cat 9, slot 35). It shows in play and in the U window, but the select
  screen shows his normal costume.
- **Cause:** a select-screen unit is built by `new CX2Unit( kUnitInfo )` (`X2StateServerSelect.cpp:11142`, `Handler_EGS_CHARACTER_LIST_ACK`), and
  `ResetInventorySize` never runs on it (only the selected-unit and in-game paths call it: `:3721`, `:3874`, `:4685`, `X2StateBeginning.cpp:2192`).
  Its equip bag therefore keeps the constructor default `MAX_INVENTORY_SIZE` 24 (`X2Inventory.cpp:25`, `X2Inventory.h:3`).
  `CX2UnitViewerUI::UpdateEqip` loops `i < GetItemMaxNum( ST_E_EQUIP )` (`X2UnitViewerUI.cpp:3024`, and `:2961` in the other branch), so slot 35
  (`NESI_ONEPIECE_FASHION` since DU4's renumber; it was 20) and every slot from 24 up (41 / 42 second rings, June minor-unit slots) are never added.
  The emulator sends them (`X2OfflineInventory.cpp:772-790` over 43 slots).
- **Fix here, with the select screen's two bodies:** give that unit June's equip size (`NESI_END`) the way June's character-list handler does
  (find June's counterpart of `:11142`; do not invent a size path). Check the other `GetItemMaxNum( ST_E_EQUIP )` loops on units without a reset
  (`X2Unit.cpp:3038`, `:6321`). Play-test: `jtLordKnight` with 163569 shows the Spa Wear alone on the select screen.

## Amended by char-luciel DU5b (2026-11-16): the back body is yours

DU5b gave the viewer Ciel's skin (`m_pMinorUnitXSkinAnim`), the switching view (`m_bSwitchingView`, `UpdateSwitchingView` = `31f93a10`,
`ApplySwitchingViewFilter` = `31f94190`) and `GetBackXSkinAnimPtr` (`31f91020`, non-NULL only while `m_bEnableDuoView` and `m_bShowDuoBackUnit`:
the ctor sets both, the character window clears the first). The character window shows one body; the create / select screens and the cash shop
show both, so these come with this phase:
- the back body's frame move: `31faa550`'s second `31fa8ce0( t, dt, 0 )` (position = viewer position + ( `+0xd84` + 1.0 ) x ( 50, 0, 70 ), the double 334355b0 and the floats 33732708-33732710 in
  `31fa8ce0`'s first `param_4 == 0` branch; read the others too) and its draw in `31f974b0` / `31fa00b0` (both read `+0xd81` / `+0xd82`);
  DU5b's `OnFrameMove` / render alias the shown body only and leave a stand-in line for this;
- the `+0xd82` writers: `PlayEmotion` `31fa52b0` (8 sites) and `31fa5c90` (3 sites);
- `31f90fb0` (rotate the minor skin: SelectLuciel's `m_fMinorUnitLobbyRotationY`), `31f99f70`'s back-body move (`SetPositionOnScr`);
- the equips' position / scale / colour copies by body (`31f93710` / `31f93820` / `31f93920`: equip `+0x908`, `+0x914`, `+0x190`, no March
  members; find their June readers first) and `31fa4ec0`'s duo sit sets (customize: Stage 12 unless the table rows need it).

## Amended by char-luciel DU5b2 (2026-11-16): field / room viewers and the fixed equips

- **`m_bShowFormEquipOnly`** (viewer `+0x118c`, DU5b2's member, ctor false): June sets it to 1 in `CX2SquareUnit::InitViewer` (`323f7380 @323f7417`,
  right after the viewer ctor) and in `CX2Room` `31ed46e0` (`@31ed4815` the room slot's viewer, `@31ed489b` my unit's); call
  `SetShowFormEquipOnly( true )` there (X2Room.cpp is DU5e's file: coordinate). Its readers (`AddEqip` / `AddEquipByTID`) are ported.
- **`CX2Eqip::SetFixed`** (`3295a560`): June sets `m_bFixed` only; March also hides the sub-equips (`SUB_EQUIP_TEST`). Find how June draws a fixed
  equip's sub-equips (village square units), then replace March's under a divergence flag and drop the two `june_diverge_allow.tsv` rows
  (`AddEquipByTID` / `AddEqip`) and turn the `CX2Eqip::SetFixed` row to ported.
- **Equip copies:** DU5b2 left three more `3295b260` stand-ins (equip `+0x908` state, `+0x90c` position, `+0x918` scale, set by `AddEqip` /
  `AddEquipByTID` from the viewer, or from `+0x11b0` in viewer state 9) beside DU5b's `31f93710` / `31f93820` / `31f93920`; `UpdateEqip` ends with the
  same copies by body.
- **The frame move's enchant lookup** (`31fa8ce0 @31fa9040`): June asks the worn item on the equip's body (`32869e10( pos, 0, body )`); March's
  two-argument call stays until the back body's frame move lands here.

## Split by DU5c (2026-11-16)

DU5c did the village avatar ( the square unit's SD table and SD mesh by form, Init's form head, SetMinorUnitForm's viewer rebuild, the NOT's replays,
`m_bShowFormEquipOnly` in `InitViewer` and `CX2Room` ). The create / select screens, the viewer's back body, `CX2Eqip::SetFixed`, R14e's onepiece and
the other players' form byte moved to **DU5c2** ( `kickoffs/char-luciel/DU5c2.md` ).
