# June character Lu/Ciel (Stage 8) - Phase CH1a3 - CX2LucielMyGageUI and the gage manager's Lu/Ciel cases (Opus)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1a3` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1a1 review |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (the UI class); a gage-manager case inside a March function goes under `_LIVE` with a `/D` probe |
| Log tag | none (inert until `_LIVE`; CH1 final play-tests it) |
| Review | kickoffs/char-luciel/CH1a3-review.md (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH1a1 - Done (2026-11-14)"
grep "3353375c\|3248e290\|32e6e190\|32e5eca0\|32e6d6c0" june2017_re/funcmap.tsv
```
March's siblings: `CX2RenaMyGageUI` / `CX2EveMyGageUI` / `CX2ChungMyGageUI` in `X2GageUI.h` / `.cpp` ( their CH1q ports are the idiom ).

## Task

CH1a1 routed the Combination Point gauge's UI here.
- **`CX2LucielMyGageUI`** ( RTTI `.?AVCX2LucielMyGageUI@@`, vt `3353375c`, secondary `33533744` for the second base at +0x44 ): find the ctor ( xrefs
  to the vtable ) and its size, the members, and port the overrides whole: slot 0 dtor `32e72770`, 1 `InitUI` `32e6e230` ( opens
  `DLG_Luciel_CombinationPoint.lua`, data034, CH5's asset; the load code is this phase's ), 2 `32e719b0` ( the frame move; compare Chung's
  `32e71820` / Rena's `32e71970` slot 2 ), 8 `SetShow` `32e5ed90`, 19 `SetCharacterImage` `32e62d50` ( base `32e5f3a0` ), 24 `32e63070` ( base
  `32e62d10`: name it from March's `CX2MyGageUI` virtual at that slot ), and the secondary vtable's slot 0. Every slot not overridden: one line why.
  The UI reads `CX2LucielGageData` ( `GetNowCombinationPoint`, `GetCombinationPointChanged`, `GetNowMp( 1 )` / `GetMaxMp( 1 )` for the resting form if
  it shows them ): use CH1a1's names.
- **Gage manager cases:** `CX2GageManager::CreateMyGageUI` `3248e290`, the Lu/Ciel case @`3248e618` ( new `CX2LucielGageData` 0x130 +
  `CX2LucielMyGageUI` ), and `3248ec80` ( new 0x130 + `32498e40`: name the function and its other cases from March's `X2GageManager.cpp` ). Inside
  March's functions -> `_LIVE` ( commented out ), proven with a `/D SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` probe.
- `CX2GULuciel::CreateGageData` `32b37c70` ( vslot 0xd0 ) stays CH1k2's.

June's numbers and quirks verbatim ( `JUNE_BUGS.md` rows ), a funcmap row per function ( `Char Luciel CH1a3` ). Done gate: every slot has a March body
or a recorded reason; `/Zs` probes of `X2GageUI.cpp` / `X2GageManager.cpp` ( also with `/D ..._LIVE` ) 0 errors; `junediverge.py --check` and
`enumscan.py --check` PASS; full build skipped ( no play-test ).
