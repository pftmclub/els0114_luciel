# June character Lu/Ciel (Stage 8) - Phase CH1a1 - The Combination Point gauge: CX2LucielGageData's virtuals, CX2LucielMyGageUI, the gage manager cases (Opus)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1a1` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1a0 review |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (class data / UI); a gage-manager case inside a March function goes under `_LIVE` with a `/D` probe |
| Log tag | none (inert until `_LIVE`; CH1 final play-tests it) |
| Review | kickoffs/char-luciel/CH1a1-review.md (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH1a0 - Done (2026-11-14)"
grep "Char Luciel CH1a0" june2017_re/funcmap.tsv | grep -i "gage"
ls june2017_re/char_luciel/ghidra/gage/
```

## Task

CH0 mapped only the gauge's ctor, copy ctor and point setter; CH1a0 found (RTTI `.?AVCX2LucielGageData@@`, vtable `3349b2a4`) that the class
overrides **20 of the 22** `CX2GageData` virtuals (all but slots 12 and 14), so Lu/Ciel's two forms keep their own MP inside the gauge. Port them:
- **Map the 22 June gauge vslots to March's `CX2GageData` virtuals** (March declares fewer: compare Chung's / Ara's vtables `3349b064` / `3349b124`,
  where slots 1-10 are the base `322b2xxx` bodies and 15-21 the class overrides). A June-only base virtual is declared in `CX2GageData` under the flag
  with its base body (no `June` in the name).
- **Write each override whole** into `CX2LucielGageData` (`X2GageData.h` / `.cpp`, CP949: `bytepatch.py`), including `operator=`, `GetCloneGageData`
  ( `32a360d0` ), `CopyGageData`, the play-status packet pair and `UpdateDataFromGameUnit`. Decompiles are in `june2017_re/char_luciel/ghidra/gage/`;
  10 bodies are uncarved (`32a31a90`, `32a35430`, `32a31ac0`, `32a31800`, `32a31af0`, `32a35000`, `32a35020`, `32a350c0`, `32a350d0`, `32a35150`):
  use `disassemble_function` / capstone, never invent. Name `m_fRestFormMpRate` ( +0x12c ) by what `32a35170` really does.
- **`CX2LucielMyGageUI`** (RTTI in the exe; find its vtable and ctor) and the gage manager's Lu/Ciel cases: `CreateMyGageUI` `3248e290` @`3248e618`,
  `3248ec80` (new 0x130 + `32498e40`); `DLG_Luciel_CombinationPoint.lua` (data034) is CH5's asset, the code that loads it is this phase's.
- `CX2GULuciel::CreateGageData` `32b37c70` (vslot 0xd0) stays CH1k2's; the `+0x110` "changed" byte writers ( `32aeb090` ) are CH1k's.

June's numbers and quirks verbatim (a `JUNE_BUGS.md` row each), a funcmap row per function (`Char Luciel CH1a1`). Done gate: every slot has a
March body or a recorded reason; `/Zs` probes of `X2GageData.cpp` / `X2GageUI.cpp` / `X2GageManager.cpp` 0 errors; `junediverge.py --check` and
`enumscan.py --check` PASS; full build skipped (no play-test).
