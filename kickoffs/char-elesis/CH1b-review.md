# June character Elesis (Stage 7) - Phase CH1b review (covers CH1a-CH1b)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH1b review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH1a and CH1b done (their Done notes and commits) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (`//#define`; no `_CORE` row so far) |
| Log tag | none (nothing runs until `_LIVE`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "CH1a - Done" "CH1b - Done"
awk -F'\t' '$7=="CH1a" || $7=="CH1b"' june2017_re/char_elesis/ch0_class_map.tsv
```

## Task

Review the 56 functions of CH1a (33) and CH1b (23) against June (`python june2017_re/charport_adis.py elesis <va>...`; the listing carries the `StateChange` arguments and the `fcomp` / `test ah` direction; 25 of the 56 are `listing-only`). Spot-check at least:
- **`32b52b50` / `32b52f40`** (DamageDownFront / Back EventProcess): flags 5 / 6 / 7 fire on `stateChange < time || IsAnimationEnd()` (the file-local `ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_`, defined in CH1b), the time tests run before the key test, the flag-5 / 6 / 7 conditions (Front: all three `RollingFrontFront`; Back: `RollingBackFront` / `RollingBackBack` / `AttackBack`), the state ids per listing, and **no `GetRandomInt` on the X key** (June goes straight to `ESI_QUICK_STANDUP_ATTACK_FRONT` / `_BACK`, `0xb8` / `0xb9`); the key window start `0.33`.
- **`32b53300` / `32b53440` / `32b53580` / `32b536c0`** (StandUpRolling EventProcess): the flag-0 state per function (`0xf3` BACK_ADVANCED for FrontFront / BackFront, `0xf4` FRONT_ADVANCED for FrontBack / BackBack), `y -= 30` (Front pair) / `45` (Back pair), the animation-end clause.
- **`32b52b00`** (DamageDownFront FrameMove): `EventTimerOneshot( 0.19 )`; **`32b52f00`** (Back): `EventTimer( 0.32 )`.
- **CH1a:** PlayAnimationSwordWaitStart `32b6e0c0` (0.15 / 0.25 fatal threshold with `m_uiChivalryLevel`), DashJumpEventProcess `32b6e660` (the both-keys-held branch), the `QUICK` group, JumpAttackZ / X, the Dash / DashEnd states.
- `june2017_re/char_elesis/ch1_offsets.tsv` (CH1a and CH1b rows, incl. `+0x9638` / `+0x9639`) against the listings.
- The CH1b-routed lead for CH4a: does `m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent` still have a June writer (`InitPassiveSkillState` `32b88300`)? Add one line to `kickoffs/char-elesis/CH4a.md` either way.

Hygiene: `python tools/flagcheck.py SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --range <CH1a commit>^..<CH1b commit>` (FAIL only for the commented-out `#define`); `python tools/junediverge.py --check-all`; one-file `/Zs` probe of `X2GUEL.cpp` off and with `/D SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`. Fix clear transcription errors in place (bytepatch only). Write "CH1b - Review" into the plan doc, update the STATUS rows (CH1b done, CH1c ready), check `kickoffs/char-elesis/CH1c.md`, commit with explicit pathspecs.
