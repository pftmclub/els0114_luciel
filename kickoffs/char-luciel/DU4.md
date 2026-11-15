# June character Lu/Ciel (Stage 8) - Phase DU4 - NESI renumber and the save migration

| | |
|---|---|
| Start | new conversation on **Opus 5.5, xhigh** effort, then type `/x2-phase char-luciel DU4` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU3 review |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_NESI_RENUMBER` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |
| Review | kickoffs/char-luciel/DU4-review.md (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Starting context" "Phase DU - The two-body engine (DU1-DU5)" "Phase NU0 - New-unit and two-body engine map (read-only)"
```
Then the planning digests in `june2017_re/char_luciel/` the task names, and the Done notes of the phases this one depends on
(their routed gaps). Verify a digest line before building on it.

## Task

Do the DU4 row of "Phase DU" (user, 2026-11-02: June's numbering, forward-only). **Snapshot `db_backup/` before the first test** and
write the migration from NU0 item 7's shape, one schema step. Retire `SERV_IRUHADEV_JUNE_CHARINFO_EQUIP_GROUP` and the `IS_FOR_MINOR_UNIT`
stand-in in the same commit. Play-test: three characters' equipment, fashion and onepiece show identically before and after, and survive a relog;
Ciel's minor-unit slots hold their items.

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

## Amended by NU0

NU0 (2026-11-02), `nu0_equip_nesi.tsv` (`migration` rows). **Map** (checked against June `Enum.src.lua:6653-6696`): `NESI_*` 0-19 keep their
values and names; `NESI_ONEPIECE_FASHION` 20 -> 35; `NESI_END` 21 -> 43; June adds Ciel's slots 20-34, 36, 38, 40, 42
(`NESI_MINORUNIT_DEFENCE_HAIR_FASHION` = 20 ...) and the main body's `NESI_AC_MINIME` 37, `NESI_AC_EARRING` 39, `NESI_AC_RING2` 41.
`SERV_NEW_ONE_PIECE_AVATAR_SLOT` is on for US, so a March save can hold slot 20.
**Save shape** (amended by CH1a2, 2026-11-14: schema v23 now, CH1a2's `unit.minor_cur_mp` / `minor_unit_form` under
`SERV_IRUHADEV_JUNE_CHAR_MINOR_UNIT_PLAY_STATUS`, `X2OfflineDB.h` `SCHEMA_VERSION` chain and the `iFrom < 23` rung; the step is **v24** under this
flag, nested over the v23 branch, `#error` guard on `SERV_IRUHADEV_JUNE_CHAR_MINOR_UNIT_PLAY_STATUS`):
1. `item.slot` where `category` = the equipped category (9): `UPDATE item SET slot = 35 WHERE category = 9 AND slot = 20`.
2. `inventory_size.size` for category 9: 21 -> 43 (`UPDATE inventory_size SET size = 43 WHERE category = 9 AND size = 21`); the loader only
   fills missing rows, so a stale 21 leaves slot 35 out of range.
No other table holds an NESI id (`unit_skill.slot`, `unit_skill_page.slot` and bag slots are not NESI). C++ in `X2Lib/Offline` is symbolic (comments
only carry numbers); `IsSuitableSlot` (`X2OfflineInventory.cpp:197-239`) needs the new slot cases. Tools: `elsdb.py` `EQUIP_SLOTS[20]` -> 35;
`createchar.py` `COSTUME_SLOTS` unchanged. Client stand-in to retire: `X2Inventory.cpp:1284`; slot hide `32e22ce0` (main {0,2,...,19,35,37,39,41}
hidden in the switching view; Ciel {20..34,36,38,40,42} shown only there).

## Amended by DU3b (2026-11-15)

- **The minor-unit slot test goes live on its own.** `CX2Inventory::IsMinorUnitEquipSlot` (June `32070ef0`; used by the view list, the unit's per-body
  tables and `IsMinorUnitEquipItem` `3286a350`) answers false for `CX2Unit::NESI_ONEPIECE_FASHION` before June's table, as `GetEquipGroup` does. After the
  renumber that enum is 35 and slot 20 is Ciel's: no code change needed there, but replace its literal case labels (`0x14..0x22`, `0x24`, `0x26`,
  `0x28`, `0x2a`) and the `0x29` / `0x2a` second-ring tests (`X2GUUser.cpp` `UpdateEquipViewListInfo`, `X2Unit.cpp` `AdjustEquipPositionForSecondRing`)
  with the NESI names you register.
- **Second-ring remap stand-in:** June's `3234fe90` reads unit `+0x294` (a per-UID map whose value starts with the slot id; not identified). DU3b's
  `CX2Unit::AdjustEquipPositionForSecondRing` reads `GetInventory().GetItemSlotID( uid )` (June `3286a310`) instead: identify `+0x294` and confirm or replace.
  Since the DU3 review it has a fourth caller, the `CX2Eqip` ctor (June `32961280 @329615f4`, `X2Eqip.cpp`): a second ring's equip takes `EP_AC_TEMP4`.
  The play-test's second-ring check covers it (the ring shows on the second ring's position).

## Amended after DU3c (user report, 2026-11-15)

- **User report:** with Lu active, right-clicking a Ciel costume puts it on Lu; in the live game it goes into Ciel's slots without switching. Cause here:
  March's equipped bag has no Ciel slots until this phase, and the inventory right-click (`CX2UIInventory::OnRClickedItem`, `X2UIInventory.cpp`
  equip loop over the character window's slots, March :6255) has no body test. Port June's test with the renumber, so the play-test covers it.
- **June's rule** (`OnRClickedItem 321aa5d0`, raw disassembly only: the chunk @`321ae867-321aec39` is outside every Ghidra function, read it with
  `june2017_re/disasm_range.py 0x321ae800 0x321aeea0`). Target body, before the slot loop (`[esp+0x28]`):
  1. not fashion (templet `+4` bit 0) and not `IT_ACCESSORY` (`+6 & 0xf` = 3) -> body 0;
  2. fashion / accessory, use condition not `UC_ANYONE` (`+4 & 0xf000` != 0x1000) -> the templet's `m_bIsForMinorUnit` (flags2 bit 2): **a Ciel costume
     always targets Ciel's slots, whoever is active**;
  3. `UC_ANYONE`: the item's `+0x80` id > 0 with a templet of unit type 10 -> that templet's flag (identify `+0x80`); else the unit's active body
     (`+0xe40`), or the character window's shown body (`+0x25a`) while it is shown (`+0x1e5`).
  Per character-window slot (`[esp+0x14]`): slot id `+0x40` == `321693b0( pos, &out )` (DU5's slot picker; port or stand in), for a non-accessory slot
  fashion `+0x3c` == templet fashion, and slot `IS_FOR_MINOR_UNIT` `+0x44` (March `CX2SlotItem::m_bIsForMinorUnit`, already parsed) == the target body.
  Slot 0x26 (`NESI_MINORUNIT_AC_MINIME`) with a `UC_ANYONE` item takes the active / shown body instead of the slot flag (@`321aeb5e-321aebe0`).
  CharInfo C4 already ported the same flag test for a drop onto the window (`32918410`, `X2UICharInfo.cpp` `IsMinorUnitItem`).
- The emulator's `IsSuitableSlot` (`X2OfflineInventory.cpp`) must accept a Ciel item in the Ciel slots and refuse it in Lu's. Play-test addition: with Lu
  active, right-click Ciel's Promotion Top Piece (41901, in testlu1's bag): it lands on Ciel, Lu keeps Lu's Promotion top; right-click Lu's: Lu.
