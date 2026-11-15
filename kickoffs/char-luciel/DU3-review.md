# June character Lu/Ciel (Stage 8) - Phase DU3 review ( covers DU3, DU3b, DU3c )

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU3 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU3c done and its play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_EQUIP_VIEW` (no `_DEBUG`) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "Phase DU - The two-body engine (DU1-DU5)" "DU3 - Done (2026-11-15)" "DU3b - Done" "DU3c - Done"
```
Then `grep "(DU3)\|(DU3b)\|(DU3c)" june2017_re/funcmap.tsv` and the three `stage8` rows of `tools/data/june_march_divergent.tsv`.

## Task

Review the three commits against June:
- **DU3:** the ctor's two arguments land at `+0x15d4` / `+0x15d5` (`32961280 @32961493`), the `UC_ANYONE` gate (`@32961362`), the three keyed reads
  pass the body (`32867660` / `32867880` / `32867960`); `32882540` / `32882440` / `328825c0` against `GetEffectiveEnchantLevel` /
  `IsUsableEquip` line by line; the PC-bang stand-in is labelled.
- **DU3b:** `3201b930`, `31fd2f00`, `31ffe990` against their transcriptions (open two branches each in the listing), every override and caller
  passes June's slot id, the per-body unit tables' writers.
- **DU3c:** the init build `31fe4210` / `31fe3e30` / `31fe2fb0`, the socket and hide passes, the CX2Eqip body readers, the glow cases.
- **Flag-off = March:** `cl /P` of `X2GUUser.cpp`, `X2Eqip.cpp`, `X2Item.cpp` with the flag on and off differ only inside its blocks.

Hygiene: `python tools/finish.py --review --commits <DU3> <DU3b> <DU3c> --with SERV_IRUHADEV_JUNE_CHAR_EQUIP_VIEW`. Paste the block; explain every
OTHER row. Write "DU3 - Review" into the plan doc, set DU3 / DU3b / DU3c done and DU4 ready in STATUS.md, commit. No play-test unless a fix lands.

## Amended by DU3c (2026-11-15)

- What DU3c landed and what it routed is in "DU3c - Done". Check in Ghidra: `UpdateEqip` = `31fe4210` (both branches, the push order), the two
  `ReplaceEmptyToBasic` = `31fe3e30` / `31fe2fb0` (the `[pos][3]` table from `31fca7e0`, the eight calls), `GetEquipDuoType` = `3286a4b0`, the socket skip
  `@3206dc6a`, `NOT_SHOW_UNIT_BY_DUOTYPE` `@320618a8-3206194c`, `UpdateHideEquipByBody` = `3204e4a0` (its loop is an inlined `SetShowObject( false )`),
  the `_UpdateSkin` term `3295ae20`.
- Judge the dummy-item stand-in (heap items the equip owns, where June's equip keeps no item pointer) and the chain-byte rename / default `true`.
- **Fix owed (found after DU3c):** DU3c's `CX2Inventory::GetEquipDuoType` duplicates skill S7k's `CX2Inventory::GetEquipGroup` (also June `3286a4b0`,
  `X2Inventory.cpp`, used by `X2Unit.cpp` / `X2UICharInfo.cpp`). Point `InitSocketData`'s skip at `GetEquipGroup`, delete `GetEquipDuoType`, fix the
  funcmap `3286a4b0 (DU3c)` row. Behaviour is the same (both read the slot id); `/Zs` probe after.
