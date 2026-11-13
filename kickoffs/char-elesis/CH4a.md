# June character Elesis (Stage 7) - Phase CH4a - Passives, the 0x83c hook and delegates

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH4a` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH3d review. One at a time with CH1-CH4: they all edit `X2GUEL.*` |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE` (+ `..._PASSIVE_DEBUG`) |
| Log tag | `[JUNE_CHAR]` (`ELESIS PASSIVE`) |
| Review | none of its own: the CH4c review covers it (its `_DEBUG` define stays on) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md --no-sub "Rules" "Phase CH4 - Passives and class mechanics"
python tools/docsection.py JUNE_CHARACTER_ARA_PHASES.md "CH4a - Done"   # the worked example
cat june2017_re/char_elesis/ch0_passive_ids.tsv
```

## Task

Do CH4a: `InitPassiveSkillState` `32b88300` (vslot 0x690, 4,453 B) whole on June's ids (March `X2GUEL.cpp:350-659`, class arms `:414` / `:479` /
`:533` / `:612`, plus arms for 35 / 125), her passive ids, their objects and readers, and its 7 attack-result delegate pushes replacing March's own
(checklist l.920 / l.930; the Blood Runaway delegate's body `32b87880` is CH4c's: push it under `_BLOOD`); the June-only hook 0x83c `32b732e0`
(skill-level read, buff erase by BTI, weapon extra-damage type; CH1o added the base virtual); 0x29c `32b6fc40` (the `GetReduceDebuffDurationRate`
shape, in the `SERV_IRUHADEV_JUNE_BUFF_GROUP` block); the class float writer `32b73bf0` / reader `32b7d550` (l.496); the plan's passive-id check
(`ch0_passive_ids.tsv`: March 8000s against June 28000s). Play-test per passive (CH0 names the stat or log line). No review of its own: CH4c's
covers it; `_PASSIVE_DEBUG` stays on. You are on Opus: delegate the reads, probes and builds.

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

- **Functions:** `awk -F'\t' '$7=="CH4a"' june2017_re/char_elesis/ch0_class_map.tsv` = 12 (5 listing-only: col 9 `n`; 5 `inside`, 7 `new`: col 5; proposed names col 6, `?` = name by the class pattern from the body).
- **Skill ids** (22): 106, 28400, 28401, 28402, 28403, 28404, 28405, 28406, 28407, 28408, 28409, 28410, 28411, 28412, 28413, 28414, 28415, 28416, 28417, 28418, 28419, 28420.
- **Checklist rows** (`june2017_re/char_elesis/ch0_checklist_rows.tsv` col 5): L245, L929, L938.
- **`ch0_blood.tsv` rows:** 4 (filter on its `proposed_phase` column).
- **`ch0_classn.tsv` rows:** 1 (filter on its `proposed_phase` column).
- `InitPassiveSkillState` `32b88300` pushes 7 delegates through the thunk-table adapters `32b8ecf0` / `32b8ed00` / `32b8ffd0` / `32b8ffe0` / `32b906d0` / `32b90ab0` / `32b90ac0` (`push edx; call; ret`): `32b52470`, `32b524b0` (Scalet Rose hit counter `+0x9788`), `32b6fd50`, `32b6f820`, `HealByBurningPhantomOnAttack` `32b7e3b0`, `32b872d0` (here) and `ApplyBloodRunawayOnAttack` `32b87880` (CH4c). It also writes the Dark Force members (`+0x9714` / `+0x9718` / `+0x971c` from 28410), Judgment (`+0x9748` level / `+0x974c` chance of 28411) and Blood Runaway (`+0x9760` cool time of 28414). `ApplyGrandMasterAura` `32b7e740` (BFI 909 / 910) and `FireBlossomsProc_FrameMove` `32b734d0` (March `:9347`) are here. Passive ids: `june2017_re/char_elesis/ch0_passive_ids.tsv` (55 March reads, 41 differ, 12 cross-line).
- **CH1b review (2026-11-12):** `m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent` (`+0x4060`) has **no** writer in June's `32b88300` (it writes `+0x95d0` / `+0x9638` / `+0x9639` only); the shared base (Get Chance, `X2GUUser.cpp:6443`) stays its only writer, and June's DamageDown no longer reads it. Port no class writer.

## Amended by CH1j (2026-11-12)

- CH1j declared under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE` ( `//#define` in `Always.h`; define it with `_LIVE` when the bodies exist ) and calls from `CommonFrameMove`: `BloodRunawayProc_FrameMove` `32b734d0` ( the Blood Runaway tick, skill 28414: CH0 mislabelled it as the Fire Blossoms twin; Fire Blossoms is `32b87520`, written in CH1j ), `GrandMasterTranscendenceProc_FrameMove` `32b7e580` ( skill 28415, moved here from CH4c ) and `ApplyGrandMasterAura` `32b7e740`. Write the three bodies in place of the declarations.
- `InitPassiveSkillState` writes members CH1j declared ( `X2GUEl.h`, `_CORE` ): `m_usBlazeAbsoluLevel` ( +0x97a4, `32b88cdd` ), `m_fAdvancedKnightMasteryMPValue` ( +0x9780, ability 0x2b of 28404, `32b88689` ). `m_fDashChargeTime` ( +0x9790 ) is read by `32b7e580`; `m_fDashElapsedTime` ( +0x977c ) by CH1k's range.
- `awk -F'	' '$7=="CH4a"' june2017_re/char_elesis/ch0_class_map.tsv` now lists 13 rows ( 32b7e580 added ).

## Amended by CH1o (2026-11-13)

- **0x83c is done, and it is not what the Task line says.** `32b732e0` is March's `ChangeDamageData` (First Strike; no buff erase, no weapon type): CH1o put June's body inside it under `_LIVE` and gated it on `RNSI_P_SSK_FIRST_STRIKE` (28401) learned. Drop 0x83c from this phase.
- So `InitPassiveSkillState` must write `m_cFirstStrikeData` from 28401: June `32b88898-32b888ac` stores abilities 0x4e / 0xf / 0x2b of that templet into `+0x9660` / `+0x9664` / `+0x9668` (stop time / damage / MP rate); March's writer (`X2GUEL.cpp:759`) reads `SI_P_SGM_FIRST_STRIKE` (8406).

## Amended by CH1q (2026-11-13)

- **Dark Force members are declared (under `_WOS`).** `InitPassiveSkillState` `32b88300` first resets `m_fWayOfSwordPointMultiplier` (`+0x9714`) = 1.0, `m_fVigorHpIntakeRate` (`+0x9718`) = 0.0, `m_fDestructionDamageRate` (`+0x971c`) = 1.0, `m_fDestructionBleedingDamageRate` (`+0x9720`) = 1.0 (`@32b88cf2-32b88d0f`), then, when `RNSI_P_SDK_DARK_FORCE` (28410) is learned and its templet exists, writes SA 147 `SA_WAY_OF_SWORD_POINT_MULTIPLIER` / SA 49 `SA_HP_INTAKE_REL_DAMAGE` / SA 15 `SA_DAMAGE_REL` into the first three (`@32b88d4a` / `@32b88d61` / `@32b88d78`, `GetSkillAbilityValue( sa, level, 0, 0.0 )`: a learned Dark Force whose data lacks a key writes 0.0, keep it). Those writes sit in `_WOS` lines inside your `_PASSIVE` / `_LIVE` body (the members exist only with `_WOS`).

## Amended by CH3b0 (2026-11-14)

- CH3b0 declared (under `_SUMMON`, ctor 0) and reads, but does not write: `m_usBloodRunawayLevel` (+0x9724, level of 28414), `m_fBloodRunawayCleanseIntervalTier1-3`
  (+0x9728 / +0x972c / +0x9730, abilities 0x94 / 0x95 / 0x96), `m_fBloodRunawayCleanseElapsedTime` (+0x9734), `m_eBloodRunawaySaveDebuffID` (+0x9738), `m_ucScaletRoseHitCount`
  (+0x9788). `InitPassiveSkillState` writes the level / intervals, `BloodRunawayProc_FrameMove` (`32b734d0`) runs the timer and uses `IsBloodRunawayCleanseReady()`
  (`32b6fb10`, written) and the saved debuff, the delegate `32b524b0` counts Scalet Rose hits (28211 / 28232, DD+0x3dc > -500). Widen `_SUMMON`'s members, never copy them.
- Vslot 0x24c's Fire Blossoms tail reads March's `m_uiFireBlossomsSkillLevel` / `m_bIsFireBlossoms` (+0x95f4 / +0x95f0): June's writers are this phase's.
