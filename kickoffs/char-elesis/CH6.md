# June character Elesis (Stage 7) - Phase CH6 - Whole-character walk and gap reruns

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, medium** effort, then type `/x2-phase char-elesis CH6` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH4c review, CH5 review (done 2026-11-13), CHNb review (done 2026-11-13) |
| Flag | none (a defect goes back to its phase's flag) |
| Log tag | `[JUNE_CHAR]` (`ELESIS *`) |
| Review | none (the user's walk is the check) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md --no-sub "Phase CH6 - Whole-character walk and gap reruns"
python tools/docsection.py JUNE_CHARACTER_AISHA_PHASES.md "Template for Stage 7"   # the transcendence rows and the transition rule
```

## Task

Do "Phase CH6": back up the save; units of 8 / 29 / 30 / 35 / 120 / 121 / 125 and one transcended per second job. Re-run `census.py`,
`charport_scan.py`, `charport_keys.py ELESIS_KNIGHT` and `s3_no_state.py --family X2GUEL.cpp --preprocess`; before / after numbers in the Done note
(0 unexplained `no_state`). Hand the user the walk: every skill in an A and a B slot (each cast twice), a hyper, the Way of the Sword (fill both
ways, vigor, destruction, a stage change), each of the four summons, blood sucking and Blood Runaway on 35 / 125, the passives, one dungeon per
job, the level-15 class-change dialog with all three paths on fresh units (CHNb's quests: Accept in the guide, then `/qdone` 35569-35572 -> 35,
35320-35323 -> 29, 35330-35333 -> 30) and the level-35 one to Crimson Avenger (35573-35577 -> 125; start NPC 10025), the creation
screen's six Elesis entries, then leave the game and return to the village; **the becoming-transcended transition** on un-transcended 120 / 121 /
125 by `/tc 1`, `/qdone 42030`, `/additem 288493 1` (guide popup with the class picture, new skill-tree rows, HUD portrait, relog).

No hardcoded enum literals (user, 2026-10-26; CLAUDE.md): an id that has a name in an Enum (June's
`Enum.lua`, or a C++ enum) is never written as a number, in any new or touched code: use the member, and
register it under a `SERV_IRUHADEV_` flag with June's value and name if C++ lacks it. A value with no Enum
entry stays a number with a one-line comment. The one exception is a table whose rows are the data itself
(a `.tsv` of ids), not code.

No `June` prefix (user, 2026-10-28; CLAUDE.md): June's body goes inside March's own function under the flag,
March's in `#else`. A function March lacks gets a name in the class's own pattern, never with `June` in it.
Every name (locals and constants too) says what it is in context: no Ghidra address, phase tag or `June` in an identifier.
Call a March function from June code only if June's namesake does the same; otherwise replace March's definition.
Run `python tools/junediverge.py --list` before transcribing; a newly found divergent function gets its `june_march_divergent.tsv` row and the replacement in the same commit.
Phase tag (user, 2026-11-09): every edited or new code block carries a one-line `// char-elesis <phase>` comment (at the start of an existing one-line comment, never a second line).

## Amended by CH0 (2026-11-12)

- S3 baseline: `june2017_re/char_elesis/ch0_s3_baseline.txt` (261 rows, 254 no_state, 35 / 125 included). Reruns: `charport_scan.py`, `charport_keys.py ELESIS_KNIGHT` (CH0: 248 keys, 9 june-only, 1 neither, 4 march-only-site, all owned).

## Amended by CH5 (2026-11-13)

- Skills were not castable at CH5, so its cut-in checks are owed here: every skill cast shows `[JUNE_CHAR] ELESIS UI cut-in set <n> name <gfx>` (8 / 29 / 30 -> set 0 `HQ_CutIn_Elesis_01`, 120 -> 1 `_GM_01`, 121 -> 2 `_BH_01`, 125 -> 3 `_CA_01`, 35 -> 0), each matched by a `[JUNE_GFX] CUTIN play`; the hyper on 120 / 121 / 125 shows `ELESIS UI hyper cut-in class <c> name HQ_Cutin_EGM / EBH / ECA_Transcendence`.
