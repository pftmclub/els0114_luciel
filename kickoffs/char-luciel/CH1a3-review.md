# June character Lu/Ciel (Stage 8) - Phase CH1a3 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1a3 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH1a3 done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (the UI class, the factory) and `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` (commented out: the three manager hunks) |
| Log tag | none (inert until `_LIVE`; CH1 final play-tests it) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH1a3 - Done (2026-11-14)"
```
Then `grep "Char Luciel CH1a3" june2017_re/funcmap.tsv` (19 rows) and `JUNE_BUGS.md` rows 682-684.

## Task

Review CH1a3's commit against June (Ghidra read-only; uncarved bodies through `june2017_re/disasm_range.py`):
- **Slot map.** Dump vt `3353375c` against the base `3353336c` (and Rena `3353363c` / Eve `335336cc`): overrides 0 / 1 / 2 / 8 / 19 / 24 only;
  slot 24 = `UpdateMpAndHyperAndDetonationGage` (base `32e62d10`, called by the base frame move `32e71620` right after the HP gauge); vslots
  `0x64` / `0x6c` = `UpdateDetonationGage` / `UpdateHyperGage`. Secondary vt `33533744` slot 0 = the adjustor thunk `32e72670`.
- **Bodies.** ctor `32e647d0` (0x64 bytes), dtor `32e6ad10` (inverted destroy test, JUNE_BUGS 682), `InitUI` `32e6e230`, frame move `32e719b0`,
  `SetShow` `32e5ed90`, `SetCharacterImage` `32e62d50`, slot 24 `32e63070` (hyper before MP), `32e62ef0`, `32e62410` (the `31c966d0` stand-in:
  `UpdateGageForUV` behind the 0..1 gate), `32e6e2c0` (the left-out `CX2GageManager +0xd1` test: confirm every writer sits behind code 2155),
  `32e6ae10` (offsets 42 / 38, `+0x170` / `+0x2f4` = `SetOverUI` / `SetResolutionConvert`), `32e62190` (strings byte-exact, transcend rows).
- **Manager.** `3248e290 @3248e618` (the seven classes, under `_LIVE`), `3248ec80` = `CreateGageDataByUnitClass` (`_CORE`), its callers
  `324910c0` (= March `InsertPartyMemberUI` but the factory) and `32491200`; the PvP routing bullet in backlog Stage 9 and the Stage 17 note.
- **Routing.** CH1's kickoff carries the `DLG_Luciel_CombinationPoint.lua` adoption note.

Spot-check 3 items directly in Ghidra; if one fails, re-check the whole phase. Hygiene: `python tools/flagcheck.py JUNE_CHAR_LUCIEL_CORE --range
<CH1a3 commit>^..<CH1a3 commit> --with SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`, `git log -S<flag> <commit>..HEAD -- X2Lib/X2GageUI.* X2Lib/X2GageManager.*`,
`python tools/junediverge.py --check-all`, `python tools/enumscan.py --check X2Lib/X2GageUI.h X2Lib/X2GageUI.cpp X2Lib/X2GageManager.h
X2Lib/X2GageManager.cpp`. Paste the summary lines; explain every OTHER row and deleted line.

Write "CH1a3 - Review" into the plan doc, update the CH1a3 row in STATUS.md (done; CH1's CH1a3 dependency met), and commit. No build unless a fix
lands (then `/Zs` probes of `X2GageUI.cpp` / `X2GageManager.cpp`, plain and `/D SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`; no play-test).
