# June character Lu/Ciel (Stage 8) - Phase CH2d20 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH2d20 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH2d20 done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_TRANSFORM_LINK` (the window and the user wrappers, the `UI_MENU` member), `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` (the five hooks inside March functions) |
| Log tag | none (inert until CH2d2 creates the window) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH2d20 - Done (2026-11-15)"
grep "(CH2d20)" june2017_re/funcmap.tsv
```

## Task

Review CH2d20's commit (`git log --oneline --grep "char-luciel Phase CH2d20"`) against June in Ghidra. What it could get wrong:
- the handle tests: June mixes `operator!=` ( `31d41cd0` / `31d41d10` ) and `operator==` ( `31d41d50` / `31d41d90` ); every body must run on a **valid** handle
  ( CH0's row for `31fd4930` said "invalid": the Done note says why that was wrong );
- `3200f7a0`'s x87 branch senses ( `test ah, 5; jp` vs `test ah, 1; jnz` ), the `+ 0.5` rounding, `STR_ID_2632` / `STR_ID_3738` and the cover / string order;
- `31fe12f0`'s constants ( 12.0, 0.5, 297.0 qwords ) and slot count ( 4 ), `31fe2b10`'s ( -297, -615 ) floats, `31fdfb20`'s key actions ( 2-5 = `GAMEACTION_SKILL1-4` );
- the hooks: `CX2GUUser::OnFrameMove` @`32070133` ( after the SIEE 15 call, elapsed from user +0x2b0 ), `ToggleUI` case 44 @`32158eab` ( break, not return ), `GetShow` case 44
  @`3215277d`, `SetShowDlgMenu` `324617c0` ( order of the three toggles in each branch ), `KeyProcess` @`322f7b41` ( class 0x81 only );
- the `UI_MENU` member: March's numbering differs from June's ( enumprobe: `UI_RIDING_SKILL_SLOT` 33 ); check no March code persists or sizes by `UI_MENU_END`.

Hygiene is one command, not a fresh script:
`python tools/finish.py --review --commits <the CH2d20 commit> --with SERV_IRUHADEV_JUNE_CHAR_TRANSFORM_LINK SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`. Paste its block
into the review. Run it with `-v` and explain every OTHER row and every deleted line; a FAIL or ERROR is a finding.

Spot-check 2-3 items directly in Ghidra (read-only); if one fails, re-check the whole phase.
Fix clear transcription errors under the same flag (a one-file `/Zs` probe proves the compile; no play-test: the window has no creator until CH2d2); hand
design decisions back to the user. Write "CH2d20 - Review" into the plan doc, update the phase's STATUS.md row, check the CH2d2 kickoff, and commit.
