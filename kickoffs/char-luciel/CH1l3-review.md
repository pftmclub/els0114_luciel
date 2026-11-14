# June character Lu/Ciel (Stage 8) - Phase CH1l3 review (covers CH1l, CH1l2, CH1l3)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1l3 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1l, CH1l2 and CH1l3 done (their Done notes and commits `f82df8f5`, `12df5212` and the CH1l3 code commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (`_LIVE` stays `//#define`), `SERV_IRUHADEV_JUNE_ENUM_NAMES` (CH1l2's `BTI_BUFF_DEAD_HANDS`, CH1l3's six names) |
| Log tag | none (nothing runs until `_LIVE`; no play-test) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH1l - Done (2026-11-15)" "CH1l2 - Done (2026-11-15)" "CH1l3 - Done (2026-11-15)"
```
Then `awk -F'\t' '$6=="CH1l" || $6=="CH1l2" || $6=="CH1l3"' june2017_re/char_luciel/ch0_class_map.tsv`, `grep "(CH1l3)" june2017_re/funcmap.tsv` and JUNE_BUGS 709-720.

## Task

Review the three phases against June's listings. Spot-check at least these, in Ghidra or the capstone dump (read-only):
- **CH1l's big row, `UpdateNotRenderingOtherEquipList` `32b18220`:** compare with a Ghidra decompile ( the per-group position lists in `_CONST_LUCIEL_CH1L_`, the item list per body from `32b176a0` ), and `GetFormEquipItemList` itself ( stops at two; CH1l3's stand-in `IsEquipForMinorUnit` is false until DU3 ).
- **CH1l2's `CommonFrameMove` `32b397a0`, `CanUseSpecialAttack` `32b3a310`, `CommonStateStart` `32b37910`:** order of the blocks and the eight callees CH1l3 wrote.
- **CH1l3's DamageData offset map** ( June offset to March member, one probe each: `+0x8` attackType, `+0x14` hAttackerEffect, `+0x28` reActResult, `+0x46c` defenderType, `+0x470` optrDefenderGameUnit, `+0x590` m_eUsedSkillID,
  `+0x5c4` m_iAppliedSkillID, `+0x5a8` m_RelateSkillData byte, `+0x638` m_iDamageEffectIndex, `+0x50` extra damage type ( 0x10 / 0x43 ) ): the one unproved row is June `+0xa4` ( read by `32b0e4e0` ) taken as `m_ExtraDamage.m_Damage` ( March `+0x9c` ).
- **The Punisher modifier tags 9 / 10 / 11** ( `32b34c70` ): named force down value / extra damage / relate skill index from funcmap row `321db1b0` ( `+0x5f0`, `+0x2a4`, `+0x7a8` ); confirm the targets against the DamageData members.
- **`CreateChaserOfDarknessDamageEffect` `32b1f800`:** the 23-argument `CreateInstance` decode ( reserve time 1.0, the two bone / rotation vectors, land position `+0x80c4` ), the lock-on writes `+0x938` / `+0x940`, destroy + erase of the first handle.
- **`ShowActiveSkillCutInAndLight` `32b16940`:** the three tables ( `333a78e0` 12 names, `333a8080` 3, `333a74e0` 6 ), size 512 x 512 / position ( 256, 512, 0 ) per entry ( the constants `334367ac` / `33435598` ), index `+0x8ac8 * 3 + +0x8acc`, the transformed-form pick ( `31fc9d90( 0 / 1 )` = `IsTransformed( DT_MAJOR / DT_MINOR )` ) and the hyper-active third entry.
- **`GetOpenMindSkillID` `32b0ef80` / `IsOpenMindConditionMet` `32b4c7f0`:** class 127 -> 30416, 128 -> 30418, 129 -> 30420 ( the decompile; CH0's hint and CH1l2's comment had it reversed ), `points <= threshold` ( JUNE_BUGS 719 ).
- **`FrameMoveDeathTouchEffect` `32b34a90` / `ClearDeathTouch` `32b349f0`:** the `+0x97b8` / `+0x97bc` pair as two damage effect handles ( CH1a's UidType was wrong ), the +25 / +100 y offsets, `SetLinkMainEffectByPos` argument order.
- **`PlayAltarOfEvilEffectSet` `32b35600` / `FrameMoveAltarOfEvilEnd` `32b35c60`:** `+0x9748` as the per-altar MP ( renamed `m_fMpCostPerEffectSet` ), the low-effect-option position list ( `g_pMain +0x1ac == 2` is the effect option: funcmap `322720f0` / `X2GameUnit.cpp:7730` ), state 444 has no name ( `STATE_ID_ALTAR_OF_EVIL_UNNAMED` ).
- **Renames CH1l3 made in `X2GULuciel.h` ( CH1a names that were wrong ):** `m_ucAuraOfFearStackCount`, `m_ucTwilightJudgementStackCount`, `m_iShadowBoltNpcUID`, the two Death Touch handles, `m_fMpCostPerEffectSet`. `m_fRapidImpactRange` ( `+0x97c4`, ctor 9999.0, counted down every frame by `32b16760` ) keeps its name: CH4a renames it with its readers.

Hygiene is one command: `python tools/finish.py --review --commits f82df8f5 12df5212 <the CH1l3 code commit>` ( add `-v`; explain every OTHER row and deleted line ). `--check-all` runs inside `--review`.
No debug define in any of the three phases to retire. Fix transcription errors under the same flags ( one-file `/Zs` probes of `X2GULuciel.cpp`, no full build: no play-test ). Write "CH1l3 - Review" into the plan doc, update the STATUS rows ( CH1m is blocked on this review ), commit.

You are on Opus: delegate the reads, greps and known-address dumps ( x2-phase, *When the phase is Opus* ).
