# June character Lu/Ciel (Stage 8) - Phase CH1a4 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1a4 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH1a4 done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_TRANSFORM_LINK` (user pieces, `KTDGXSkinAfterImage.h`), `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (class bodies), `SERV_IRUHADEV_JUNE_ENUM_NAMES` (BTI 763 / 764) |
| Log tag | none (inert until `_LIVE`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH1a4 - Done (2026-11-14)"
python june2017_re/charport_adis.py luciel 32b13d30 32b13e20 32b38f30
```
Then `grep "(CH1a4)" june2017_re/funcmap.tsv`.

## Task

Review CH1a4's commit against June:
- **`STransformedSkin`** ( `X2GUUser.h` ): field order and types = the pair ctor `32073d50` and the users ( `32b13d30` closes `+4` / `+8`, deletes `+0xc` /
  `+0x14`, releases `+0x10` ); the user ctor `@32040eb0` / `@32040ec3` and dtor `@3203b530`-`@3203b56a` need nothing else.
- **`SetFormSkinAnim` `32b13e20`:** the body slot per form ( `+0x87c8` = `m_pMotionFuture`, `+0x87cc` = `m_pMotion`, `+0x6754` / `+0x6758` ), which
  saved value goes back into which set field, the after-image test ( `+0x194` = `m_bEnable` ) and the mode-3 Disable / Enable order, the byte flip.
- **`EndTransformedForm` `32b38f30`:** the BTI pick ( `0x2fb + ( form != 0 )` ), the early return ( JUNE_BUGS 689 ) skipping the tail, the tail order
  ( `ClearAltarOfEvilEffectSet`, erase, stop ), the `m_vecpWeapon` loop reading ( Weapon `+0x1c0` = `m_pUserWeaponEquip`, CX2Eqip `+0x544` / `+0x564` =
  normal / hyper anims: both offsets sit 4 bytes past March's, as June's `CKTDGObject` does ), vslot 0x634 = `UpdateEquipCollisionData()`, the
  effect-set call's defaults, the `(int)( (int)duration / 6.0 )` conversion.
- **`RestoreHeadBySkinAnim` `31fdc9d0`** and its two calls in `RestoreHead` ( after the second-body block, unconditional ); **`SetShowAfterImage`**
  `3322a9c0`; BTI 763 / 764 = June's `Enum.src.lua`.

Hygiene is one command: `python tools/finish.py --review --commits <CH1a4's commit>` ( add `-v`; explain every OTHER row and deleted line ). The
worktree run hit two tool limits the Done note explains ( DUO_BODY / ENUM_NAMES FAIL on `X2GUUser.cpp` = TRANSFORM_LINK's lines inside an enclosing
block; LUCIEL_CORE Unevaluable on Chung's compound guard ): re-run those per file as the Done note did.
Spot-check 2-3 items in Ghidra (read-only); if one fails, re-check the whole phase. Fix transcription errors under the same flag (one-file `/Zs`
probes, no full build: no play-test). Write "CH1a4 - Review" into the plan doc, update the STATUS row ( CH1's CH1a4 dependency ), commit.
