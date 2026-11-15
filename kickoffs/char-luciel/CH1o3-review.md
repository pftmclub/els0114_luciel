# June character Lu/Ciel (Stage 8) - Phase CH1o3 review (covers CH1o2, CH1o3)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1o3 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1o3 done (`8b026393`); CH1o2 `4cd0f5c2` |
| Flag | `SERV_IRUHADEV_JUNE_USER_VIRTUAL_HOOKS` (CH1o2), `SERV_IRUHADEV_JUNE_USER_SKILL_STATE_BYTE` and `SERV_IRUHADEV_JUNE_CHAR_DUO_WIN_SPEECH` (CH1o3, defined, class-agnostic), `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE`, `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` (`//#define`) |
| Log tag | none (no play-test) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH1o2 - Done (2026-11-15)" "CH1o3 - Done (2026-11-15)"
```

and `june2017_re/char_luciel/ch1o_vcall_sites.tsv` (column `ch1o2_verdict`), `ch1n_skill_cases.tsv` (column `skill_state_extra`), `grep "(CH1o3)" june2017_re/funcmap.tsv`.

## Task

Review the two phases' commits:
- **CH1o2 (engine, defined flag, every class):** spot-check 4 routed sites against June in Ghidra ( `IsNotCommonMoveState` `31fd7640`, `CanRidingState` `31fd7930` with
  its new head test, `SetEnableDash` `31fc8540`, `PopFrameData` `3206cff0` ); each base getter still equals March's constant, so no live class changes; the `const` change of
  `GetDieFrontStateID` / `GetDieBackStateID` has no other caller; 5 random `automatic` and 3 `June-only` verdicts are right.
- **CH1o3 effect-set tags:** cases 4 / 10 / 11 against `321db1b0` ( table `321dbc60`, `june2017_re/ghidra/dis_0x321db1b0.txt` ): tag 4 `_ftol2` / `FIMUL` math on `m_DamageTime`,
  tag 10 protected float `m_ExtraDamage.m_Damage`, tag 11 chop rounding into the byte; JUNE_BUGS 725 ( the NEXT loops write the main's own value ). Tag 9's `#ifndef` copy equals Elsword's.
- **CH1o3 skill-state byte ( engine, every class ):** `m_bSkillState` sits where June has `+0x11` ( after `m_bHyperState` ), `Init()` clears it, `operator=` copies it;
  `IsSkillState` = June `3208e430` OR the slot tables ( no March class sets the byte, so the two engine readers answer as before for them ); Lu's three stores
  ( @`32b46622`, word 0x101 @`32b4665c`, state 265 @`32b468fa` ); the 15 other `skill_state_extra` ids are in the skill kickoffs; Lu's `ReservedSkillStart` = June @`32057c4c`-`cb2`.
- **CH1o3 win speech ( engine, every class ):** the `DUNGEON_SPEECH_LUCIEL` parse ( `3202b9e0`, JUNE_BUGS 724 ), `GetRandomWinSpeech` = `32009dd0` ( rand order, `+0xb54`, CIEL when > 0 ),
  March's path unchanged for other classes, `CX2Game::m_iWinSpeechForm` zeroed in the ctor ( @`3230770e` ). The CH5 amendment names the two routed readers.
- **Checklist rows:** L260 / L263 / L977 verdicts in the note match `32b3a310`.

Hygiene is one command: `python tools/finish.py --review --commits 4cd0f5c2 8b026393 --with SERV_IRUHADEV_JUNE_USER_SKILL_STATE_BYTE SERV_IRUHADEV_JUNE_CHAR_DUO_WIN_SPEECH
SERV_IRUHADEV_JUNE_USER_VIRTUAL_HOOKS --state X2Lib/X2GULuciel.cpp Luciel_Duo --state X2Lib/X2GULuciel.cpp Luciel_Demonio`, and `_CORE` on its own:
`python tools/flagcheck.py SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE --commits 4cd0f5c2 8b026393 --files X2Lib/X2EffectSet.cpp X2Lib/X2GULuciel.cpp X2Lib/X2GULuciel.h` ( with `_CORE` as a `--with`
partner flagcheck stops on CH1a's unevaluable `#if` at `Always.h:13430`, see the CH1o3 note ). Paste the blocks; explain every OTHER row and deleted line with `-v`.

No play-test, no full build (`/Zs` probes, plain and with `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`). Fix clear transcription errors under the same flag; hand design decisions
to the user. Write "CH1o3 - Review" into the plan doc, update STATUS, check `CH1p.md`, commit.
