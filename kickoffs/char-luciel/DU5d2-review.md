# June character Lu/Ciel (Stage 8) - Phase DU5d2 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5d2 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5d2 done and its play-test passed (Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (DU5b's) and `SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY` (DU5c2's); no debug define, DU5d2 adds no log line |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU5d2 - Done (2026-11-16)"
```
and `grep "(DU5d2)" june2017_re/funcmap.tsv`, the `CX2UnitViewerUI::SetFixed` row of `tools/data/june_march_divergent.tsv`.

## Task

Review DU5d2's commit against June (Ghidra / `june2017_re/disasm_range.py`, read-only):
- `31f93920` and `3295a9e0`: the body test (`+0x15d4`), the flag write for both outcomes, the sub-equip render-param pick (`+0x240` skin -> vslot 0x14,
  else `+0x198`), colour at `+0x28`; the two `UpdateSwitchingView` calls (`@31f93cf8` / `@31f93e4b`: arguments, 1 x 4 / 0.3 x 4 at `3345993c`) and the
  `AddEqip` tail (`@31f9e859-31f9e8fc`: states 1 and 7, the `+0xd82` test, 0 x 4).
- `+0x190`: its readers (`3295b430` from `32963c80` / `3295c460`) and the ctor value (`@329614f6`), and the Stage 11 route.
- `31f933c0`: the type test (2 / 3, or 1 with `31fac390`), `31f91f90( 0 )`, and the false path = March's.
- `31fa5c90`: `@31fa5cdb`, `@31fa5d36`, `@31fa5d4d` placed in PlayByMotionType as June orders them; `@31fa61d0` and PlayEmotion's 8 writes
  behind the custom sit names (Stage 12 route).
- `31fa64f0`: the shown / back pick on `+0xe60`, NULL counted as ended, the `@31fa68ca` clear.
- The five swapped `SetUnitScale` sites (values against June) and the left-alone list in the Done note (one body, same value, no March counterpart,
  PvP).
- Flag-off = March by `cl /P` of `X2UnitViewerUI.cpp` and one caller file (flag on vs `//#define`).

Hygiene is one command: `python tools/finish.py --review --commits <DU5d2's commits> --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY SERV_IRUHADEV_JUNE_SKILL_CLASS_POPUP --skip-flag SERV_IRUHADEV_JUNE_DIVERGE_EQIP_SET_FIXED "#error guard target"`
(`-v`; explain every OTHER row and the deleted lines). Fix clear transcription errors under the same flag (then `tools/build.py --deploy`); hand
design decisions back. Write "DU5d2 - Review" into the plan doc, update the STATUS row, commit.
