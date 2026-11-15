# June character Lu/Ciel (Stage 8) - Phase CH2b review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH2b review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH2a, CH2a2 and CH2b done (their Done notes and commits) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` and the temporary `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL_DEBUG` (this review retires it) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL SKILL`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH2a - Done (2026-11-15)" "CH2a2 - Done (2026-11-15)" "CH2b - Done (2026-11-15)"
```

## Task

Review the commits of CH2a, CH2a2 and CH2b (`git log --oneline --grep "char-luciel Phase CH2a"` / `CH2b`) against June in Ghidra and the capstone listings
(`june2017_re/charport_adis.py luciel --phase CH2a` / `CH2b`). This review covers all three (user, 2026-11-10: one review per CH stage). What CH2b could get wrong:
- `LSI_SA_LLC_DEAD_HANDS_EventProcess` ( `32aff850` ): the extra head ( `m_fEventTime[0].stateChange <= now`, `jnp` after `test ah, 0x41` ) against the shared cancel chain;
  the setter cases' hyper copies ( Dead Hands copies StartFuture / FrameMoveFuture / FrameMove / EventProcess, Apollyon copies Start / FrameMove / EventProcess, Blasting Fear / Backstab only EventProcess );
- `m_eStateStartSkillID` ( June +0x9444 ): written after `CX2GUUser::CommonStateStart` in `CX2GULuciel::CommonStateStart`; read by Ruinous Impact's start ( before its own CommonStateStart:
  the previous state's skill ) and Dark Crescent's start-future; no other June reader of +0x9444 outside `32b14f70` ( which still reads `GetNowStateSkillID` ) was missed;
- `LSI_SA_LLC_DARK_CRESCENT_StateStartFuture` ( `32b2ac20` ): the filter order, AT_SPECIAL for the NPC invincible test, `CanDown` ( 9 args ), the pick rule ( compared with Aisha's `32cd4830` ), the flip and the +20 landing;
- `LSI_SA_LLC_APOLLYON_FrameMove` ( `32b2b050` ): ability types ( 0x1d / 6 / 2 / 0x1f / 5 ), the `[esp+0x34]` slot = the size rate ( both sets ), the 1.2 / 1.0 bases, the `<=` roll,
  the Throne Authority roll direction ( JUNE_BUGS 740 ), and that `GetInvincible()` ( slot 0x180 ) is the right test;
- `TryUseSkillWithCombinationPoint` now `bool` ( June's `al`: true only after `SpecialAttackNoStageChange || StateChangeBySkillID` ); CH2a's use / cooldown debug lines stay correct;
- CH2a2's `bMarchSlotIndex` arm and CH2a's cases ( already play-tested ): diff only.

Hygiene is one command, not a fresh script:
`python tools/finish.py --review --commits <the CH2a, CH2a2 and CH2b commits> --with SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL_DEBUG`. Paste its block
into the review. Run it with `-v` and explain every OTHER row and every deleted line; a FAIL or ERROR is a finding.

Retire the debug define ( comment out `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL_DEBUG` in `Always.h`: no build, "build skipped: debug retire only" ) once the play-test of CH2b has passed.
Spot-check 2-3 items directly in Ghidra (read-only); if one fails, re-check the whole phase. Fix clear transcription errors under the same flag; hand design decisions back to the user.
Write "CH2b - Review" into the plan doc, update the phase's STATUS.md row, check the CH2c kickoff, and commit.
