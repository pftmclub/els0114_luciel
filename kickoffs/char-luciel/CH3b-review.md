# June character Lu/Ciel (Stage 8) - Phase CH3b review ( covers CH3a and CH3b )

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH3b review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phases CH3a and CH3b done (their Done notes and commits) and the CH3b play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` (+ `_SKILL_DEBUG`, retired here) |
| Log tag | `[JUNE_CHAR] LUCIEL SKILL` (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL SKILL"`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "CH3a - Done (2026-11-16)" "CH3b - Done (2026-11-16)"
```
Then `kickoffs/char-luciel/CH3b.md` (the amendments), the `funcmap.tsv` rows tagged `(CH3a)` / `(CH3b)` for Luciel, and `JUNE_BUGS.md` rows 770 and 772-774.

## Task

Review CH3a's commit (`c63ae472`) and CH3b's against June in Ghidra (read-only) and the exe listing. One stage review: CH3a has none of its own.
- CH3b, the 45 functions ( `ch0_class_map.tsv` rows with phase `CH3b` ), three of them line by line: Spectral Spear's frame move `32b2f4b0` ( the two
  upgraded timers, the five soul timers, the seed `int( ( time - 0.3 ) / 0.1 + 0.5 )`, the three random rotations, `AddBuffAccumulationCount( 523, -1, 1, 0 )` ),
  Shadow Bolt's start frame move `32b2fba0` ( the offset +-120, y + 300 / 240, the `CanDown` argument order, the `CreateNPCReq` list, the 12 + Noble Devotion stacks ) and
  Shadow Bolt's control event process `32b30090` ( the dead -750 test, the 1500 height test, the nine key rows in order ).
- The setter cases ( `32b47737 .. 32b4a37d` ): the callbacks each hyper state copies ( not the Init ), the nine control states ( 405 .. 413 ) and the end state ( 414 ),
  the Soul Spread loop functions registered in the event process slot ( the class map calls them StateEndFuture ), the skill-state byte on 404 / 584 / 402 / 578.
- `m_fShadowBoltDuration` renamed `m_fShadowBoltPowerRate` ( June `32b2fba0` stores `GetPowerRate()` ); the 5 enum names in `X2BuffFactor.h`.

Hygiene is one command: `python tools/finish.py --review --commits c63ae472 <the CH3b commit> --with JUNE_CHAR_LUCIEL_SKILL JUNE_CHAR_LUCIEL_SKILL_DEBUG JUNE_ENUM_NAMES -v`;
explain every OTHER row and deleted line.

Read the play-test's `[JUNE_CHAR] LUCIEL SKILL` lines ( CH3a's and CH3b's: `fantasie 2nd movement attackbox`, `swift smash effect set`, `spectral spear specter`,
`shadow bolt spawn / stacks / control state`, `twilight judgement start / reattack`, `summon familiar`, `soul spread loops` ). Retire
`SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL_DEBUG` ( comment out the define: no build ). Fix clear transcription errors under the same flag; hand design decisions back to
the user. Write "CH3b - Review" into the plan doc, update the STATUS rows ( CH3a / CH3b done; CH3c ready ), check `kickoffs/char-luciel/CH3c.md`, and commit.
