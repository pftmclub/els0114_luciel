# June character Lu/Ciel (Stage 8) - Phase CH1q - Play-test fix: Lu's skin-type weapon crashes field entry

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1q` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1 ( `5e880f3c`, fix 1 `975e8b87` ) |
| Flag | a new class-agnostic `SERV_IRUHADEV_JUNE_*` flag for the weapon / eqip engine change ( not `_LIVE`: every class's weapon path ) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL CORE` / `DATA`) (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |
| Review | none of its own: the CH1 review covers it ( `CH1-review.md` gains the commit ) |
| Hand-off | now ( it resumes CH1's play-test ) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH1 - Done"
grep "(CH1)" june2017_re/funcmap.tsv
```
June's `CX2GUUser::InitComponent` `31fee090` decompile: `june2017_re/char_luciel/ghidra/ch1q/31fee090_CX2GUUser_InitComponent.c`.

## Task

**The crash ( both CH1 play-test runs, CrashRpt `%TEMP%\X2_*.tmp`, `tools/mdscan.py` ).** Entering battlefield 40000 as Lu crashes in
`CKTDGXSkinAfterImage`'s ctor: `CX2GUUser::InitComponent` ( X2Lib ) loops the weapons ( `+0x3e00` vector, `X2GUUser.cpp` InitComponent ) ->
`CX2GameUnit::Weapon::LoadDataForUser` ( `X2GameUnit.cpp:11672` ) -> `CreateSkinAfterImage( m_pUserWeaponEquip->GetNormalAnim( i ), 10, ... )` with a **NULL anim**:
`GetNormalModel( i )` is set but `GetNormalAnim( i )` is not. Lu's weapons ( 113300 Cobo Dual Weapon, 41890 Lu's Promotion Dual Weapon,
`Mesh_Lu_Cash_Weapon_Sorted.X` ) are **`UT_SKIN`** in June's item.kim; every March class's weapon is `UT_ATTACH_ANIM`. The skin path ( `X2Eqip.cpp`
~2331, `SERV_IRUHADEV_JUNE_EQIP_SKIN` ) opens models and never builds a normal anim; March's `LoadDataForUser` assumed a weapon always has one.
- CH1's fix 1 ( `SERV_IRUHADEV_JUNE_SKIN_AFTER_IMAGE_NULL_MESH`, June `3322abc0` @`3322af08` ) is a real June guard but sits after the crash point
  ( the ctor dereferences the NULL origin anim at line 48 first ). Keep it; its Done-note line is corrected.

**Do:** find June's `Weapon::LoadDataForUser` ( called from `31fee090`'s weapon loop; June's Lua keys are XOR-encrypted, so go through the caller ) and
port June's handling of a skin-type weapon whole: the after-image ( skipped, or built from the unit's skin anim ), the slash-trace bones, anything else
in the loop that reads the eqip's normal anim. Then check every other March reader of `GetNormalAnim` / `GetHyperAnim` on a weapon eqip that June
guards ( `X2GameUnit.cpp`, `X2GUUser.cpp`, the weapon's frame move / render ) and port those guards in the same flag. If June builds weapon state for
`UT_SKIN` weapons elsewhere ( the eqip's skin init, DU3's equip-view rewrite ), say which part belongs here ( playability ) and which stays DU3's.
Gates, then `build.py --deploy`, then hand back CH1's play-test ( `CH1.md` hand-off, from "enter the field" ).
