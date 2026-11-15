# June character Lu/Ciel (Stage 8) - Phase DU3c - Ciel's look at init, the equip's body readers, the play-test

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU3c` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU3b |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_EQUIP_VIEW` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |
| Review | `kickoffs/char-luciel/DU3-review.md` (Opus 5.5 high; covers DU3, DU3b, DU3c) |
| Hand-off | now (the seam of DU3 / DU3b / DU3c: full build + one play-test) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Phase DU - The two-body engine (DU1-DU5)" "DU3 - Done (2026-11-15)" "DU3b - Done"
```
Then `kickoffs/char-luciel/DU3.md` (its "Amended by" blocks DU1b, DU1b2, DU1d3b, DU1d5, CH1p, CH1q are this phase's list),
`grep "(DU3)\|(DU1d3b)\|(DU1b2)\|(CH1p)" june2017_re/funcmap.tsv`, and `ch0_checklist_rows.tsv` rows §7z / §7aa / §7as.

## Task

1. **Second-body equips at init:** June `InitEqip` `32024510 @32024551` calls `31fe4210` (on the view list) instead of March's
   `CX2ItemManager::UpdateEqip` (`X2GUUser.cpp:5427`); `31fe4210` -> `31fe3e30` (basic equips for positions 6-11 and 2, and 0x24 for unit type 4,
   from the unit templet's basic ids `+0xe4..+0xfc` / `+0x100`) -> `31fe2fb0` x8: Lu's equip on `+0x87d4`, and for an item with a minor-unit id
   (templet `+0x30`) Ciel's on `+0x8e88` with `CreateEquip( ..., body 1, ... )`. This is what makes Ciel draw a body (CH1q's play-test).
2. **Socket options by body:** `InitSocketData` `3206d770 @3206d8f7` / `@3206da0f`: skip an equip whose item minor-unit field (`3286a4b0`, 2 = both)
   is not both and not the active body.
3. **Hide-equip-by-body pass:** `3204e4a0` with its restore `3204df80`, the window floats `+0x8fac` / `+0x8fb0` and `NOT_SHOW_UNIT_BY_DUOTYPE`
   (`CommonStateStart` `@320618a8-3206194c`, values in DU3.md's CH1p block); the pass runs in `CommonFrameMove @320661f9`. JUNE_BUGS 673.
4. **`CommonFrameMove @320659e9-32065ae1`** (`+0x8e64` / `+0x8e68` / `+0x8e70`, the active body's anim time against `+0x8e68`): identify and port.
5. **CX2Eqip readers of the body:** `_UpdateSkin` `3295ae20`'s owner-type-10 term (attach every model when the body byte is clear and
   `m_pNormalXSkinModel[1]` is set; `X2Eqip.cpp:2044` comment, `SERV_IRUHADEV_JUNE_EQIP_SKIN`), the minor-unit attach-anim pair `3295dfa0
   @3295e2cf-3295e4f6` (`X2Eqip.cpp:2564`, `SERV_IRUHADEV_JUNE_EQIP_ATTACH_ANIM`), and the sub-equip chain byte `+0x15d5` (DU3's
   `m_bSkipSubEquipChainAdd`): `32960ad0 @32960c7a / @32960d33` skip `AddObjectChain` and the sub-equip's `+0x45a` write when set; the dtor
   `32961c80 @32962094` calls `RemoveObjectChain` only when set. Decide whether that asymmetry is a June bug (JUNE_BUGS row) before porting.
6. **Upgrade-weapon glow type 10:** `InitUpgradeWeaponParticle` `32299730`, `ProcessUpgradeWeaponParticle` `32292610` (Effect_LWeapon / RWeapon of
   the skin anim when unit `+0xe40` = 1), the hand sparks `31ff8510` (`X2GameUnit.cpp:20552` waits for "June cases 10-12").
7. **The `MAJOR:` / `MINOR:` equip dump** `31fd9200` / `31fda280` (labels rest on strings; port only if a March caller exists).

Then the full build and the play-test: Lu and Ciel each show their own equipment after a swap (Ciel draws a body); three March characters' fashion over
a broken item and a PvP item look as June's; CH1's owed check: Ciel dies with a buff on (`/invin 0`), the buff is gone after the revive. Write
`DU3-review.md` if missing.
