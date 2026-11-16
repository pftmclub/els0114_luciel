# June character Lu/Ciel (Stage 8) - Phase CH4b review ( covers CH4a and CH4b )

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH4b review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phases CH4a and CH4b done (their Done notes and commits) and their play-tests passed |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_PASSIVE` (+ `_PASSIVE_DEBUG`, retired here) |
| Log tag | `[JUNE_CHAR] LUCIEL PASSIVE` (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL PASSIVE"`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "CH4a - Done (2026-11-16)" "CH4b - Done (2026-11-16)"
```
Then `kickoffs/char-luciel/CH4b.md` (the amendments), the `funcmap.tsv` rows tagged `(CH4a)` and `(CH4b)`, and `JUNE_BUGS.md` rows 766-769.

## Task

Review CH4a's commit (`a31855e9`) and CH4b's (`5d07d7dc`) against June in Ghidra (read-only) and the exe listing. One stage review: CH4a has none of its own.
- CH4a: `InitPassiveSkillState` `32b45820` ( the 15 passive reads in June's order, the resets, the ten delegates ), 3 of the delegates spot-checked line by line
  ( `ApplyFearOnHit` `32b3d380`, `PlaySoulExtortionDrain` `32b435f0`, `FrameMoveMadnessCrave` `32b3e470` ), `CreateFearDamageEffect` `32b3ca30`, the 18-entry debuff
  group list against `3373daa4..e8`.
- CH4b, both DamageReacts ( user `320462a0`, NPC `327b1f30` ): the Backstab test ( `@320483c2-3204842e` / `@327b3382-327b33ed`: Now `bAttackerFront` clear,
  `GUT_USER`, `UT_LUCIEL`, `USED_SKILL_ID` 30205, the three `IGNORE_DEFENCE` members, the upgraded check `32071e70` -> `m_fCriticalRate` 1.0 ) placed before
  `GetFinalDamage`; the hit-from-behind block ( `@32048cdb-32048d82` / `@327b39cf-327b3a8d`: `m_bHitFromBehind`, AT_SPECIAL x `+0x95f8` unless the used skill's
  type is hyper active, else x `32ae5120`, then Backstab x `+0x9600` ); the Shadow step `@3204963d-32049674` ( outside the attacker block; 0.2 / 0.8 ) and `32aebaf0`.
- CH4b, the buff hook: `CX2GameUnit` slot 251 called at `@322a886e-322a8880` ( before slot 7 ) and `CX2GUUser`'s `3204dcd0` ( Tribute chance =
  `SA_EFFECTIVE_RATE` against `GetRandomFloat( 0 )`, debuff templet, BTI 462, first debuff erased, BFI 523 count -1, Noble Devotion `SA_RECOVERY_VALUE_REL` /
  `SA_RECOVER_MP_ABS` or `_2` by `322cc180`, `UT_LUCIEL` -> `UpCombinationPoint`, `SetNowMp`, the MP text push ).
- `DamageData::m_bHitFromBehind` reset in `SimpleInit` ( June zeroes the word `+0x5c0` / `+0x5c1` = `m_bMakeTarget` ).

Hygiene is one command: `python tools/finish.py --review --commits a31855e9 5d07d7dc 9bdc3e6a --with JUNE_CHAR_LUCIEL_PASSIVE JUNE_CHAR_LUCIEL_PASSIVE_DEBUG -v`;
explain every OTHER row and deleted line ( CH4b's `X2GameUnit.cpp` file holds a peer's `JUNE_STATCALC_UNIT_STAT` hunks: only the 3-line slot-251 call is CH4b's ).

**Owed play-test checks ( user, 2026-11-16: skipped in CH4b, the review hands them off before retiring the debug define ):** on `LuPasCN` ( unit 221, Noblesse,
level 99, transcended; keys A Soul Extortion, S Stomp, D Strong Strike, C Flick Shot, Q Triple Shot, W Shadow ):
- Shadow: W, then take monster hits ( no `/invin` ) -> `shadow shield took X, left Y`, then `, broken: both shadow buffs erased`.
- Tribute / Noblesse Oblige: hit monsters until `soul collector stack` lines, then `/buffFactor 55 10` several times ( 27.5% each ) -> `tribute: BTI ... landed,
  debuff ... erased, one soul collector stack spent ( chance 0.28 )` and `noble devotion: 1 point(s), mp +10.0` ( MP +10 text, gauge +1 ).
So the review keeps `_PASSIVE_DEBUG` on until those lines are read: hand off, read the logs, then retire it.

Read the play-test's `[JUNE_CHAR] LUCIEL PASSIVE` lines ( CH4a's checks and CH4b's: `backstab from behind`, `hit from behind`, `lord of fear effect`, `shadow
shield`, `tribute`, `noble devotion` ). Retire `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_PASSIVE_DEBUG` ( comment out the define: no build ). Fix clear transcription
errors under the same flag; hand design decisions back to the user. Write "CH4b - Review" into the plan doc, update the STATUS rows ( CH4a / CH4b done; CH4c
ready ), check `kickoffs/char-luciel/CH4c.md`, and commit.
