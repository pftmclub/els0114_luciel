# June character Lu/Ciel (Stage 8) - Phase DU3b - the equip view list ( June's rewrite, every class )

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU3b` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU3 (2026-11-15, the item / equip foundation) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_EQUIP_VIEW` (DU3's; this phase adds its view-list blocks) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | none of its own: `DU3-review.md` covers DU3, DU3b and DU3c at DU3c |
| Hand-off | batch at DU3c (no section of its own unless a March character's view changes: then write `kickoffs/char-luciel/playtest.md` section DU3b) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Phase DU - The two-body engine (DU1-DU5)" "DU3 - Done (2026-11-15)"
```
Then `kickoffs/char-luciel/DU3.md` (all its "Amended by" blocks), the funcmap rows `grep "Char Chung CH1l0" june2017_re/funcmap.tsv` (rows
`31fe21a0`-`32bdc750`) and `grep "(DU3)" june2017_re/funcmap.tsv`, `june2017_re/char_luciel/nu0_equip_nesi.tsv` rows 2-8, and the three `stage8`
rows of `tools/data/june_march_divergent.tsv` (`GetAddItemAndRemoveItemToViewList`, `AddEquipToViewList`, `UpdateEquipViewListInfo`).

## Task

Port June's equip view list whole, for every class, on DU3's members (`CX2Eqip::IsMinorUnitEquip`, the ctor's two arguments,
`CX2Item::GetEffectiveEnchantLevel` / `IsUsableEquip`). March's body stays in each `#else`. Mark the funcmap / divergence rows above done.

1. **`UpdateEquipViewListInfo( pAdd, pRemove, short slotId )`** `3201b930` (RET 0xc, vslot 0x644) replaces March's base body
   (`X2GUUser.cpp:39089` / `:39127`, both `#if` branches: check which is live with `cl /P`). DU3 read it: no `GetAddItemAndRemoveItemToViewList`
   call; remove-only: a Ciel slot id (0x14..0x22, 0x24, 0x26, 0x28, 0x2a) picks body 1; slot 0x29 / 0x2a reads position 0x1c (`EP_AC_TEMP4`, June's
   `EP_AC_RING2`: the enum rule keeps March's name); fashion or item type 3: remove the view equip at ( pos, body ), then for a non-type-3 item a normal
   item there ( `IsNormalEquipInThePosition` per body `31fac260` -> `GetEquippingItemByEquipPos` 3 args `32869e10` -> `IsUsableEquip` ) else
   `CreateBasicEquip` `323516d0`; a normal item: remove every equip at pos whose body's fashion slot ( unit `+0xb7d` [pos*3 + body] ) is empty, then
   the basic equip. Add: remove the equip at ( pos, active body `32070ef0` ) for fashion / type 3, or every equip at pos with no fashion over it;
   then `AddEquipToViewList( item, slotId )`. Tail: unit `+0x8f6e` or `+0x8f6f` -> vslot 0x63c ( identify ). Returns 1, 0 when a templet is missing.
2. **The overrides take the third argument:** Chung `32bdc750` (= March's body + `slotId`), Ara `32bc89e0`, Arme `32cc9950` (`X2GUChung.cpp:11716`,
   `X2GUAra.cpp:8454`, `X2GUArme_VioletMage.cpp:34312`; read Ara's and Arme's June bodies). Every caller passes June's slot id: `X2StateMenu.cpp:4806`,
   `:4954`, `:5063`, `X2UIInventory.cpp:3842`, `:4000`, `X2GUArme_VioletMage.cpp:27143` (xrefs of vslot 0x644 in June: `call [reg + 0x644]`).
   `X2GUArme_VioletMage.cpp` may hold a peer's uncommitted edit: never stage their hunks.
3. **`GetAddItemAndRemoveItemToViewList`** `31fd2f00` (March `X2GUUser.cpp:46045`): the per-body fashion test (unit `+0xb7d` [pos*3 + equip body]),
   `IsNormalEquipInThePosition` per body, `GetEquippingItemByEquipPos( pos, 0, 0 )`, `IsUsableEquip`. Still called by Chung (and Ara?).
4. **`AddEquipToViewList( item, short slotId, bool bForceBasic )`** `31ffe990` (March `:39435`): enchant `GetEffectiveEnchantLevel`; bBasic =
   bForceBasic or `IsBasicEquip`; normal non-type-3 item on Lu/Ciel (`31fac140`, unit type `UT_LUCIEL`): when no view equip at ( pos, 1 ) yet
   (`31fd63c0`), Ciel's copy: templet `GetItemIDForMinorUnit()` (0 -> only when the item has a Ciel model row `32867880( 10, 1 )`, then the item
   itself), the user's `+0x9570` (position) / `+0x9574` (item id) override ( identify ), built with `32883640`; then body 0 when no view equip at
   ( pos, 0 ); fashion / type 3: body 1 for a Ciel slot id ( 0x14..0x2a except 0x23 / 0x25 / 0x27 / 0x29 ) else 0. Every copy:
   `CreateEquip( item, GetSkinAnimByBody( body ) vslot 0x570 31fd48a0, enchant, bBasic, true, unit type, body, true )` (June passes the unit type
   always; DU3's ctor gate keeps it only for `UC_ANYONE`). Then March's tail per new equip (weapon index for `EP_WEAPON_HAND` 0 / `EP_WEAPON_SECOND` 1,
   the weapon particles) from `@31ffee45` (read it; DU3 stopped there).
5. The two helpers, named by the class pattern: vslot 0x570 `31fd48a0` (body 1 -> `m_pMinorUnitXSkinAnim` `+0x8e88`, else `m_pXSkinAnim`) and
   `31fd63c0` (view list has an equip at ( pos, body )).
6. **Unit per-body position tables** (`+0xb7d` fashion, `+0xbf2` normal, both [pos*3 + body], `+0xc67` normal flat for pos outside 16..29):
   find June's writers (March `X2Unit.cpp:607-700`, `IsNormalEquipInThePosition` `:851`) and port them under the flag; DU4 renumbers NESI, not this.

Rules: no hardcoded enum literals (slot ids have no C++ name until DU4: comment each range with June's NESI names from `Enum.src.lua`), no `June`
prefix, one-line comments with the June address and `// char-luciel DU3b`. `junediverge.py --list` first. Probe-build only (0 errors, break-test);
the full build and the play-test are DU3c's.
