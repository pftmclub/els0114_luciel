# June character Lu/Ciel (Stage 8) - Phase CH1a4 - The transformed-form skin: CX2GUUser's pair and the three class bodies (Opus)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1a4` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1a review |
| Flag | class-agnostic `SERV_IRUHADEV_JUNE_CHAR_TRANSFORM_LINK` (the `CX2GUUser` members and engine pieces) and `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (the class bodies) |
| Log tag | none (inert until `_LIVE`; CH2d2 / CH3c2 hand off the transform play-test) |
| Review | kickoffs/char-luciel/CH1a4-review.md (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Phase CH1 - Deploy unit: the class core" "CH1a0 - Done (2026-11-14)" "CH1a - Done (2026-11-14)"
```
Then `grep "(CH1a)" june2017_re/funcmap.tsv` (the routed row names this phase), `june2017_re/char_luciel/ch1_offsets.tsv`, and the CH1a
annotated dumps: `python june2017_re/charport_adis.py luciel 32b13d30 32b13e20 32b38f30`.

## Task

CH1a declared these three in `X2GULuciel.h` ( "body in CH1a4" ) and the dtor calls two of them; they lean on base-user pieces March lacks:
- **`CX2GUUser`'s transformed-skin pair** ( June `+0x8f74` / `+0x8f8c`, 0x18 bytes each, ctor `32073d50` at `@32040eb0` / `@32040ec3`, the anim refs
  released in `~CX2GUUser` `@3203b530`-`@3203b56a` ): `{ bool swapped; motion future; motion; anim future; CKTDGXSkinAnimPtr anim; after-image }`, the
  set a body swaps in. One array member under `TRANSFORM_LINK` (class-agnostic: Rose / Ain widen it, never copy it). Find every other user reader of
  `+0x8f74`..`+0x8fa0` outside the class ( a byte scan found `31fde67f` / `31fde69c` in `31fde420` RestoreHead and the user ctor / dtor only; confirm )
  and route or port each.
- **`ReleaseTransformedSkinAnim( int iForm )`** `32b13d30`: close the two motion devices ( `31cdd8d0` = device manager CloseDevice ), delete the anim
  future and after-image, release the anim ref.
- **`SetFormSkinAnim( int iForm, <pair>& )`** `32b13e20`: swap body `iForm`'s motion / anim / after-image ( `m_pMotionFuture` .. `m_pAfterImage`, or the
  DU1 `m_pMinorUnit*` set and `m_pMinorUnitAfterImage` ) with the pair, re-enabling the after-image as June ( `322720f0` / `32275a30` mode 3 ), flip the bool.
- **`EndTransformedForm( int iForm, bool bByDie )`** `32b38f30`: the not-transformed early path ( `ClearAltarOfEvilEffectSet`, erase BTI 763 / 764 by form,
  stop `m_hTransformedFormEffectSet` ), June's quirk that returns at once when the pair's bool is clear, `SetFormSkinAnim`, the `+0x200` / `+0x210`
  set clears, `Bip01_Head` re-fetch into `m_pFrame_Bip01_Head` / `m_pFrame_MinorUnit_Bip01_Head`, `SetShowViewList( true )` ( vslot 0x63c ), the `+0x3cfc`
  equipment loop ( 8 pairs at `+0x1c0`, `+0x172` show byte, vfunc 0x28 ), user vslot 0x634 ( name it ), `UpdateShowViewListByActiveBody`
  ( `31fd6260` ), `EffectSet_SA_LDB_TRANSFORM_END_BY_DIE` when `bByDie`, and for my unit: the transform window delete ( CH2d20's dialog release ),
  `GetBuffTimeFinalizerDuration( BTI ) / 6` points back through `UpCombinationPoint`.
- Register `BTI_BUFF_ALTAR_OF_EVIL = 763` / `BTI_BUFF_TRANSCENDENCE = 764` ( June `Enum.lua` ) under `SERV_IRUHADEV_JUNE_ENUM_NAMES` ( `X2BuffFactor.h` ).

Gate: one-file `/Zs` probes of `X2GULuciel.cpp` and `X2GUUser.cpp` ( 0 errors ), `flagcheck` both flags, `junediverge.py --check`, `enumscan.py --check`.
No play-test ( inert ). funcmap rows `Char Luciel CH1a4`, a JUNE_BUGS row for the early return.

No hardcoded enum literals; no `June` prefix; one-line comments with the phase tag ( `// char-luciel CH1a4` ). Run `python tools/junediverge.py
--list` before transcribing.
