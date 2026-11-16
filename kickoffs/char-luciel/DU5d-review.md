# June character Lu/Ciel (Stage 8) - Phase DU5d review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5d review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5d done and its play-test passed (Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (DU5b's; no debug define, DU5d adds no log line) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU5d - Done (2026-11-16)"
```
and `grep "(DU5d)" june2017_re/funcmap.tsv`, the `CX2UnitViewerUI::InitWeaponTrace` row of `tools/data/june_march_divergent.tsv`.

## Task

Review DU5d's commit against June (Ghidra / `june2017_re/disasm_range.py`, read-only):
- `31f9a2a0`: the body loop, fashion-first pick, the one shared frame set (last body wins), Lu's model 0 / model 1 line frames ( `+0xecc` / `+0xed0`
  / `+0xefc` / `+0xf00` ); the replacement of DU5b2's stand-in hunks ( flag-off = March by `cl /P` ).
- `31f93050` ( `ProcessEnchantWeaponEffectForLuciel` ): lerp order and the skin `+0x1e8` on the Lu branch; `31fa8ce0 @31fa96fe` case 10.
- `31f961a0` case 10 ( `@31f96536` / `@31f96603` ): bone, skin, rates.
- `31f92910` case 10; `31f9a900 @31f9b488`: the register map of handles 1-10 to the two groups, the emit 0 on the second group in the finger
  branches, the state test ( 10 / `UVS_CASHSHOP` ), the form-byte branch and its `@31f9ad60` frame refresh.
- `31fa3010 @31fa3219` and `31fa8ce0 @31fa92e9`: the effect-set duo argument = the shown body.
- Flag-off = March by `cl /P` of `X2UnitViewerUI.cpp` (flag on vs `//#define`).

Hygiene is one command: `python tools/finish.py --review --commits <DU5d's commits> --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (`-v`; explain every
OTHER row and the deleted lines). Fix clear transcription errors under the same flag (then `tools/build.py --deploy`); hand design decisions back.
Write "DU5d - Review" into the plan doc, update the STATUS row, commit.
