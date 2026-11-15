# June character Lu/Ciel (Stage 8) - Phase DU5b2 review (covers DU5b + DU5b2)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5b2 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5b2 done and the DU5b batch play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (+ any divergence flags DU5b2 adds) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "DU5b - Done (2026-11-16)" "DU5b2 - Done"
```
Then `grep "(DU5b)\|(DU5b2)" june2017_re/funcmap.tsv`.

## Task

Review DU5b's and DU5b2's commits against June:
- DU5b: the ctor values (`31fab020 @31fab7c9` word write, `@31fab7d8`), SetUnit's minor motion switch (`31faa6f0 @31faab03-31faac3c`: one push per
  case after the switch), the cash-shop branch of `+0xd80` (`@31faaf04`), `PlayAnim`'s two branches (`31fa3010 @31fa3062-31fa319d`), the
  filter (`31f94190`: the `SetShowObject` arguments and the kept-position test), `31f93a10`'s constants (0.3 float, 0.85 double), and that the
  local `m_pXSkinAnim` reference in `OnFrameMove` / render / `SetPositionOnScr` is the only change of meaning there; flag-off = March by `cl /P`.
- DU5b2: each replaced function line by line against its June body; the `[ pos ][ 3 ]` index at every site; every other class unchanged.

Hygiene is one command: `python tools/finish.py --review --commits <DU5b and DU5b2 commits> --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER`
(add DU5b2's divergence flags). Run it with `-v` and explain every OTHER row and every deleted line; a FAIL or ERROR is a finding.

Spot-check 3 items directly in Ghidra (read-only); if one fails, re-check the whole phase. Fix clear transcription errors under the same
flag; hand design decisions back to the user. Write "DU5b2 - Review" into the plan doc, update both STATUS rows, set DU5c / DU5d ready,
and commit.
