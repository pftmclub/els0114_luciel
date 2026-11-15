# June character Lu/Ciel (Stage 8) - Phase CH2d2 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH2d2 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH2d, CH2d2 and CH2d3 done (their Done notes and commits; the play-tests passed and their logs were read) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL`, `SERV_IRUHADEV_JUNE_ENUM_NAMES` (`BFI_BUFF_DEATH_TOUCH`, `BFI_BUFF_ALTAR_OF_EVIL`) and the temporary `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL_DEBUG` (this review retires it) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL SKILL`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH2d - Done (2026-11-16)" "CH2d2 - Done (2026-11-16)" "CH2d3 - Done"
```

## Task

Review the commits of CH2d and CH2d2 (`git log --oneline --grep "char-luciel Phase CH2d"`) against June in Ghidra and the capstone listings
(`june2017_re/charport_adis.py luciel --phase CH2d` / `CH2d2`; the four setter cases with `june2017_re/char_luciel/ch2c_cases.py <entry_va>`: Nightmare `32b47d2e`,
Altar of Evil `32b47de9`, Abyss Strike `32b49cd0`, Phantom Rush `32b49d69`). This review covers CH2d, CH2d2 and CH2d3 (the form's model swap and slot UI fix, user report after the CH2d2 play-test; user, 2026-11-10: one review per CH stage).
What CH2d2 could get wrong ( every item below was read from a listing, not decompiled ):
- **The setter cases.** Slot offsets were decoded with a stack-offset script ( state struct base = esp0 + 0x1c, a push moves every later offset by 4 ): re-read two states by hand. Nightmare's hyper copies StartFuture + EventProcess only
  ( no StateStart ); Phantom Rush's hyper copies Init + FrameMove + EventProcess; state 461's FrameMove is `32b38340` ( the class map calls it CameraMove ) in the +0x38 slot; states 446-448 and 459 / 461 carry the skill-state byte;
  444 / 445 re-register the ready / end tables; 449 / 450 ( the form's hyper mode ) pick the `_TR` table by `32070d40`.
- **Nightmare**: the target state's EventProcess is `32b178b0` byte for byte ( its key window re-enters 422 ); the target start's stand-position order ( behind the target, then in front, then on the target ), the
  `GetLandPosition` result is read only for its line index; the hit list vs the target list.
- **Death Touch** ( `32b318e0` ): SA 0x1f / 0x90 / 1 values and defaults, the three damage effects' positions ( ARM at my bone, Linking_Attack at the target, Link at my bone ), handle stores +0x97b8 / +0x97bc, BFI 786.
- **Transformed form movement** ( 9 event processes ): each differs from Lu's by the inserts; compare `32b31260` / `32b31480` / `32b133d0` / `32b316a0` with the listing again for the order of the tests ( WAIT tests the walk before the double step: JUNE_BUGS 758 ).
- **`SetTransformBuffFactorToSelf`** ( `32b3e720` ): `AdjustBuffFactorByIndex` takes `GetNowStateSkillID()` ( vslot 0x524 ), not the state buff skill id; no can-apply test; the PvP / PvE time halving.
- **`CreateAltarOfEvilProjectile`** ( `32b358b0` ): the cone arguments ( facing true, 180, no invincible skip ), the effect-option test ( `3241d2a0` on g_pMain +0x180 ), the lock-on by unit type.
- **Phantom Rush target start**: stand beside the target with the useful value 0 as the distance add, `CanDown` or the nearest line ( `GetNearestLine` = June's inlined scan: skips disabled, wall and monster-road lines, `<=` on the distance ), `ResetPhantomRushReady` on every path.
- retire `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL_DEBUG` ( `//#define` in `Always.h` ); the lines it guards stay.

Hygiene is one command: `python tools/finish.py --review --commits <the reviewed phases' commits> --with JUNE_CHAR_LUCIEL_SKILL JUNE_ENUM_NAMES JUNE_CHAR_LUCIEL_SKILL_DEBUG`.
Run it with `-v` and explain every OTHER row and every deleted line; a FAIL or ERROR is a finding.

Spot-check 3-4 items directly in Ghidra (read-only); if one fails, re-check the whole phase. Fix clear transcription errors under the same flag; hand design decisions back to the user.
Write "CH2d2 - Review" into the plan doc, update the STATUS.md rows of CH2d2, CH2d, CH3a0 and CH4a, check `kickoffs/char-luciel/CH3a0.md`, and commit. A debug-define-only change needs no build.
