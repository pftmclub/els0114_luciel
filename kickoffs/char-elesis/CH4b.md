# June character Elesis (Stage 7) - Phase CH4b - Overrides and memo

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH4b` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH4a. One at a time with CH1-CH4: they all edit `X2GUEL.*` |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE` (+ `..._PASSIVE_DEBUG`) |
| Log tag | `[JUNE_CHAR]` (`ELESIS PASSIVE`) |
| Review | none of its own: the CH4c review covers it (its `_DEBUG` define stays on) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md --no-sub "Rules" "Phase CH4 - Passives and class mechanics"
python tools/docsection.py JUNE_CHARACTER_ARA_PHASES.md "CH4b - Done"   # the worked example
python tools/docsection.py JUNE_CHARACTER_ELSWORD_PHASES.md "CH4b - Done (2026-11-05)"   # the shared Way of the Sword leftovers
```

## Task

Do CH4b: `SetDamageExceptionProcess` 0x2b8 `32b80f20` whole inside March's (`X2GUEL.cpp:9702`): the first part matches March, the June-only part
`@32b81008-32b811ef` reads 28416 / 28417 (`RNSI_P_SBH_TRANSCENDENCE`); `GetActualMPConsume` 0x6a0 `32b7ebe0` (March `:1844`) and `GetActualCoolTime`
0x6a4 `32b7ee10` (`:1929`, listing-only) with the memo reader `31fca9e0`; 0x428 `32b52260` (l.942: `1 - 0.002 x` a level term when vslot 0x524
answers `0x6d6e`: by name); 0x558 `32b6fb80`; `MemoConditionElesis` (vtable `334fa4f4`, `32b51980` / `32b519a0`, created in the ctor at
`+0x7d10`) and the readers of the four `MEMO` state tables and `BFI_ELESIS_ENDURANCE_MEMO`; in her class, what Elsword's CH4b took for the shared
Way of the Sword leftovers (`CureExtraDamage`, `ResetAnimSpeed`'s vigor factor, `SAT_WAY_OF_SWORD_GAUGE_UP`, `GetActualMPConsume`'s x0.7). Play-test.
No review of its own: CH4c's covers it. You are on Opus: delegate the reads, probes and builds.

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

- **Functions:** `awk -F'\t' '$7=="CH4b"' june2017_re/char_elesis/ch0_class_map.tsv` = 6 (4 listing-only: col 9 `n`; 3 `inside`, 3 `new`: col 5; proposed names col 6, `?` = name by the class pattern from the body).
- **Checklist rows** (`june2017_re/char_elesis/ch0_checklist_rows.tsv` col 5): L331, L333, L948.
- **`ch0_classn.tsv` rows:** 1 (filter on its `proposed_phase` column).
- 0x428 `GetDefenceRateBySkill` `32b52260`, 0x6a0 / 0x6a4 (skill 8023 + memo 608111 exempt; constants = March), 0x2b8 `32b80f20` (June-only 28416 / 28417 part), `MemoConditionElesis` (vtable `334fa4f4`).

## Amended by CH1o (2026-11-13)

- `GetActualCoolTime` 0x6a4 `32b7ee10` calls vslot 0x534 `@32b7ef2c` with `false`: that is `IsDamageState( false )`, written by CH1o under `_LIVE` (base `CX2GUUser::IsDamageState`, Elesis's override adds Endurance Power's two active states). Call it by name.

## Amended by CH4a (2026-11-14)

- CH4a declared and writes ( `X2GUEl.h`, `_PASSIVE` ) what your overrides read: `m_fMPConsumeRate` ( +0x9750, reset 1.0 in the 35 / 125 arm; `GetActualMPConsume` `32b7ebe0`
  @`32b7ecb9` multiplies by it ), `m_fAdvancedKnightMasteryEffectiveRate` ( +0x978c, SA_EFFECTIVE_RATE of 28404, -1.0 without it; June's roll `32b524f0` reads it, not yet ported:
  find its caller in your set ), `m_fAdvancedKnightMasteryDamagedRate` ( +0x9784, SA_DAMAGED_REL ), and the Blazing Heart transcendence trio `m_usBlazingHeartTranscendenceLevel` /
  `m_fBlazingHeartTranscendenceRate` / `m_fBlazingHeartTranscendenceDamageRate` ( +0x979a / +0x979c / +0x97a0: the 28417 part of `SetDamageExceptionProcess` ). Dignity's level is
  `m_usDignityOfGrandMasterLevel` ( 28416 part ). Never write them again in an override.
- Carried from CH4a's play-test if it did not show: the Advanced Knight Mastery quick stand on unit 29 ( both flags are now set by 28404 alone, no Get Chance test ).
