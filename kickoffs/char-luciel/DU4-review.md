# June character Lu/Ciel (Stage 8) - Phase DU4 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU4 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU4 done and its play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_NESI_RENUMBER` (no `_DEBUG`; retires `SERV_IRUHADEV_JUNE_CHARINFO_EQUIP_GROUP`) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`): `python tools/logs.py --grep "LUCIEL DUO DB MIG"` |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "Phase DU - The two-body engine (DU1-DU5)" "DU4 - Done (2026-11-15)"
```
Then `grep "(DU4)" june2017_re/funcmap.tsv`, the `OnRClickedItem` and `GetEquippingItemByEquipPos` rows of `tools/data/june_march_divergent.tsv`,
and JUNE_BUGS rows 741 / 742.

## Task

Review the DU4 commit against June:
- **Enum:** `X2Unit.h` `NOW_EQIP_SLOT_ID` and data036 `Enum.lua` `NOW_EQIP_SLOT_ID` both equal June's `Enum.src.lua:6653-6696` member by member
  (43 names; the March names keep their values 0-19).
- **Slot hide** `32e22ce0` against `IsEquippedSlotShown` (both case lists, the always-shown rest); **window fill** `329146d0` against
  `ResetNowEquipUI` (remap through my unit, `3286a350` per UID); **`GetEquippingItemByEquipPos`** `32869e10` (`@32869e9f`, `@32869ed1-32869ef4`).
- **Right-click** `321aa5d0` chunk `@321ae867-321aec39` (outside every Ghidra function: `june2017_re/disasm_range.py 0x321ae800 0x321aeea0`): the
  target body (three cases), the onepiece slot `0x26` at slot `+0x40` (equip position, not a slot id), the match; JUNE_BUGS 741 holds.
- **Unit `+0x294`:** judge the stand-in (inventory slot for June's UID -> slot map; `323574c0` / `32358ad0` callers, the slot-0 writers).
- **Save:** the v24 rung (two UPDATEs, one savepoint, the log line); `elsdb.py` `EQUIP_SLOTS` / `MINOR_SLOTS`; the emulator's `IsSuitableSlot`
  stand-in (body rule, minime / earring / second ring) and Ciel's starter costume slots.
- **Flag-off = March:** `cl /P` of `X2Inventory.cpp`, `X2UICharInfo.cpp`, `X2UIInventory.cpp` with the flag on and off differ only inside its blocks.

Hygiene: `python tools/finish.py --review --commits <DU4> --with SERV_IRUHADEV_JUNE_CHAR_NESI_RENUMBER`. Paste the block; explain every OTHER row
(enumscan's 190 rows predate DU4: intersect with `git diff -U0`). Write "DU4 - Review" into the plan doc, set DU4 done and DU5 ready in STATUS.md,
commit. No play-test unless a fix lands.
