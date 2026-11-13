# June character Elesis (Stage 7) - Phase CHNa review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CHNa review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CHNa done (its Done note, commit and the user's play-test) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_CLASS` (Elesis rows), `SERV_IRUHADEV_JUNE_CHAR_ELESIS_CLASS_DEBUG` (retire it here); both in `KTDXLIB/Always.h`. Reused, not re-reviewed: Ara's `SERV_IRUHADEV_JUNE_CLASS_PATH` |
| Log tag | `[JUNE_CHAR]` (`ELESIS CLASS gauge class <c> -> CX2ElesisGageData`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "CHNa - Done (2026-11-13)"
awk -F'\t' '$7 ~ /Elesis CHNa/' june2017_re/char_elesis/ch0_classn.tsv
```

## Task

Check CHNa against June from the exe bytes (Ghidra read-only; the image tables are written by static initializers: read them with
`june2017_re/char_image_rows.py`, not `.data`). Spot-check at least:
- **Gauge** `X2GageManager.cpp`: 35 / 125 labels in Elesis's arm = June `3248e290` case list (8, 0x1d, 0x1e, 0x23, 0x78, 0x79, 0x7d).
- **Compatibility** `X2Unit.h` `GetX2UnitClassCompatibility`: 35 -> {8, 35}, 125 -> {8, 35, 125} = June `31ed6500` (decompile cases 0x23 / 0x7d).
- **Creation screen** `GetThirdLineElesisClassTypeByUnitClassType`: 8 / 35 / 125 = June `3234b390` table `337a1660` (Elesis block, filled by `3234afb0`;
  `337a1900` = 0x23, `337a1914` = 0x7d) and = Ara CHNb's `GetUnitClassByTierAndLine` row (`X2Unit.h`, Elesis row). `GetUnitStatInfomationChart` 35 / 125 =
  (MIDDLE, LOW, LOW) = June `3254ee40` 2,1,1.
- **Portraits** `X2Data.cpp`: index 57 / 58 in all nine arrays and the switch = June `31ee0de0` rows 0x3b / 0x3c (`char_image_rows.py 35:0x3b 125:0x3c`; the
  no-argument run reproduces March's Little Devil / Yama Raja rows). Every index 0..56 entry above must still be compiled in (the arrays are positional).
  data057 has June's `DLG_UI_Common_Texture85_NEW` `.tga` + `.TET` (staged by CHNa: `EL_SDK_*` / `EL_SCA_*` pieces).
- **Divergence row** `GetThirdLineElesisClassTypeByUnitClassType` in `tools/data/june_march_divergent.tsv`. `tools/createchar.py` has 35 / 125 and their parents.
- **Not here.** `RotateYSelectCharacterType`'s templet reader (Lu/Ciel NU4; 35 / 125 show at the default angle); the transcended 35 / 125 portrait pair in the
  five-argument `GetCharacterImageName` (CH3d); job change and the class-change dialogs (CHNb); Dark Knight / Crimson Avenger skills (CH2d / CH3c).

Hygiene: `flagcheck.py JUNE_CHAR_ELESIS_CLASS --with JUNE_CHAR_ELESIS_CLASS_DEBUG --range <CHNa commit>^..<CHNa commit>`, `junediverge.py --check-all`,
`enumscan.py --check` on the changed lines. Read the user's play-test logs with `python tools/logs.py --grep "ELESIS CLASS"`. Retire `_CLASS_DEBUG` (comment out
the `#define`; the build is not awaited, per the 2026-11-03 rule). Fix clear errors in place (bytepatch only). Write "CHNa - Review" into the plan doc, update
the STATUS row, check `kickoffs/char-elesis/CHNb.md`, commit with explicit pathspecs.
