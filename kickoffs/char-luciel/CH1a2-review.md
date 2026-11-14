# June character Lu/Ciel (Stage 8) - Phase CH1a2 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1a2 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH1a2 done (its Done note and commit) |
| Flag | class-agnostic `SERV_IRUHADEV_JUNE_CHAR_MINOR_UNIT_PLAY_STATUS` (live; second homes `ServerDefine.h` and the `X2ServerProtocol_2010.vcxproj` X2TOOL defines) and `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (no `_DEBUG`) |
| Log tag | none (no play-test: Lu/Ciel is inert; the v23 rung logs `[JUNE_CHAR] LUCIEL DATA DB MIG v23 done` on the first launch of the next deployed build) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH1a2 - Done (2026-11-14)"
```
Then `grep "Char Luciel CH1a2" june2017_re/funcmap.tsv` and the `JUNE_BUGS.md` rows the Done note names.

## Task

Review CH1a2's commit against June (Ghidra read-only; uncarved bodies through `python june2017_re/disasm_range.py <start> <end>`):
- **Struct.** `KGamePlayStatus` Get `320dbbd0` / Put `320bcbd0` ( three members after the riding-pet map: int `+0x5a`, int `+0x5e`, char `+0x62`
  through the char serializer `33051d30` / `33051ce0`, not the bool one ), ctor `31f3b0a0` ( 0, 0, `0xff`; `m_iMaxHP` still unset ), copy ctor
  `31e85320`. Re-run an offset probe in X2Lib and X2ServerProtocol ( expected 0x5a / 0x5e / 0x62, size 0x63, CAC_COMBINATION_POINT 6 ).
- **CAC values.** The gauge setters' type compares in `32a30000`-`32a40000` ( 1 Elsword / Elesis, 3 Ara, 4 Add, 5 Rena, 6 Lu/Ciel ).
- **Gauge pair ( `_CORE` ).** `32a32530`: now MP( 1 ) to `+0x5e` first, max MP( 1 ) to `+0x5a`, the point `movsx` into `+0x16`, the form byte inline
  ( no NULL test ), base `32a32200` last, its return kept. `32a350d0`: base `32a33fa0` first, `SetMinorUnitForm` `3234b8c0`, slot 5 then slot 2 with
  index 1, point ( type 6 ) before max 10, `+0x110` = 1. Confirm `32a32200` / `32a33fa0` = March's base pair.
- **Emulator.** `MakeGamePlayStatus` ( stand-in max for `UT_LUCIEL` only ), the CAC map case, `Handlers_Field.cpp`'s on-change save, the v23 rung
  ( defaults 0 / -1 leave existing rows' meaning unchanged ), `UNIT_COLUMNS` / `ReadUnitRow` indices 24 / 25.

Spot-check 3 items directly in Ghidra; if one fails, re-check the whole phase. Hygiene: `python tools/flagcheck.py
SERV_IRUHADEV_JUNE_CHAR_MINOR_UNIT_PLAY_STATUS --range <CH1a2 commit>^..<CH1a2 commit>` and the same for `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE`
( the gauge files are `_CORE`'s; `--with` both makes Always.h's `LIVE && !CORE` guard unevaluable ), `git log -S<flag> <commit>..HEAD`,
`python tools/junediverge.py --check-all`, `python tools/enumscan.py --check X2Lib/X2GageData.cpp X2Lib/Offline/X2OfflineServer.cpp
X2Lib/Offline/X2OfflineStatTable.cpp`. Paste the summary lines; explain every OTHER row and deleted line.

Write "CH1a2 - Review" into the plan doc, update the CH1a2 row in STATUS.md, and commit. No build unless a fix lands (then `/Zs` probes of
`X2GageData.cpp`, `X2ServerPacket.cpp`, `Offline/X2OfflineDB.cpp`; no play-test).
