# June character Lu/Ciel (Stage 8) - Phase CH1l3 - Class core: Overrides, band 2, part 3

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase char-luciel CH1l3` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1l2, CH1l30 review; one at a time in `X2GULuciel.*`
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (compiled, unreferenced) and `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` (`//#define` until CH1 final) |
| Log tag | none (nothing runs until `_LIVE`; no play-test) |
| Review | kickoffs/char-luciel/CH1l3-review.md (Opus 5.5 high; the phase writes it when done). **It covers CH1l, CH1l2, CH1l3** (user, 2026-11-10): every covered Done note, `flagcheck --range` over all of them, `--check-all`, retires any debug define |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Starting context" "Phase CH1 - Deploy unit: the class core" "Phase CH0 - Lu/Ciel class map (read-only)"
```
Then the planning digests in `june2017_re/char_luciel/` the task names, and the Done notes of the phases this one depends on
(their routed gaps). Verify a digest line before building on it.

## Task

Transcribe every function the class map gives phase `CH1l3` (`awk -F'\t' '$6=="CH1l3"' june2017_re/char_luciel/ch0_class_map.tsv`;
CH0's amendment names the column), whole, under the plan's "Transcription rules (CH1b-CH1l)". Leave `InitState*` alone (CH1m registers).
Gate: every row written (the Done note lists them with June addresses); build passes; `junediverge.py --check` and `enumscan.py --check` on the
class files PASS. Nothing to play-test: the hand-off says so and names the next phase.

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

## Amended by CH0 (2026-11-02)

- **Functions: 32 to transcribe** (+ 0 thunks) (`awk -F'\t' '$6=="CH1l3"' june2017_re/char_luciel/ch0_class_map.tsv`; all new).
  June addresses: `32aeb1a0` UpGageNowMp `32aebb60` `32b0e4e0` AccumulateShadowFuryDamage `32b0e550` `32b0eed0` UpdateStableDarkEnergyBuff `32b0ef80` GetOpenMindSkillID `32b14d10` UpdateOpenMindBuff `32b153d0` `32b15780` `32b16760` `32b16880` `32b16940` `32b176a0` `32b18060` `32b1eca0` `32b1f650` `32b1f800` CreateChaserOfDarknessDamageEffect `32b204d0` `32b20780` `32b347f0` `32b349f0` `32b34a90` `32b34c70` PlayPunisherEffectSet `32b35600` PlayAltarOfEvilEffectSet `32b35c60` `32b37090` PlayBluffEffectSet `32b4c7f0` `32b4cb40` `32b4cef0` `32b4cff0` `32b4d140` `32b4d260`.
  Still `?` (name it by the class pattern from the hint in col 7): `32aebb60` `32b0e550` `32b153d0` `32b15780` `32b16760` `32b16880` `32b16940` `32b176a0` `32b18060` `32b1eca0` `32b1f650` `32b204d0` `32b20780` `32b347f0` `32b349f0` `32b34a90` `32b35c60` `32b4c7f0` `32b4cb40` `32b4cef0` `32b4cff0` `32b4d140` `32b4d260`.
- **Checklist rows** (`ch0_checklist_rows.tsv` col 5): §7 L240 SetBuffAccumulationCount 322a7660 / AddBuffAccumulationCount 322a8a70 callers in; §7ai L835-841 pass June skip / adjust / level / creator at every ported BFI call.
- **Depends on CH1l30** (unit scans, effect aim).

## Amended by char-luciel CH1l30 (2026-11-14)

- `322f1a90` is ported: `CX2Game::GetEnemyUnitListInRange( pUnit_, iMyTeam_, fRange_ )` returns the units, nearest first ( no count cap ), under `SERV_IRUHADEV_JUNE_CHAR_ENEMY_SCAN`; `32b34c70` calls it on `g_pX2Game` with Lu/Ciel's team and range 500.
- `31f75980` is March's `CX2DamageEffect::CEffect::SetLinkMainEffectByPos` ( `32b34a90` ).

## Amended by CH1l (2026-11-15)

- CH1l declared these helpers in `X2GULuciel.h` ( `body in CH1l3` ); write each body under exactly that name and signature ( every `DamageData` argument is `const CX2DamageManager::DamageData*` ): `SetLucielMinorParticleByEnum` `32b1eca0`
  ( `eVal_`, wstring, draw count; the Chung `SetChungMinorParticleByEnum` pattern on `m_ahLucielMinorParticleInstance` ), `CreateChaserOfDarknessDamageEffect` `32b1f800`, `ProcessPhantomRushHit` `32b347f0` ( CH0 `?` ), `PlayPunisherEffectSet` `32b34c70`,
  `RegisterDesperadoTarget` `32b18060` ( CH0 `?`, `m_vecDesperadoTarget` ), `AccumulateShadowFuryDamage` `32b0e4e0`, `PlayBluffEffectSet` `32b37090`, `CheckBoundingStrikeHit` `32b153d0` ( CH0 `?`, sets `m_bBoundingStrikeHit` ),
  `AddCombinationPointByDamageEffectHit` `32b16880` ( CH0 `?`, walks `m_vecDamageEffectHitCount` ), `GetFormEquipItemList` `32b176a0` ( CH0 `?`: `( eEquipPosition_, vector<SFormEquipItem>& )` appends { body byte, item id } of each worn equip at that position, stops at two ).
- `LUCIEL_MINOR_PARTICLE_INSTANCE_ID` now names the seven handles ( `LUCIEL_MINOR_PII_REVENGE_WAIT` .. `_MAGNUM_BLADE_FIRE_WAVE`, `_END` = 7 ): index the array by them.
- The equip's body byte ( `CX2Eqip +0x15d4` ) is DU3's. `GetFormEquipItemList` fills `SFormEquipItem::m_bMinorUnitEquip` with the same stand-in as CH1l's file-local `IsEquipForMinorUnit` ( false ), one-line comment, so both agree until DU3.
- The review covers CH1l too: read its Done note ( `UpdateNotRenderingOtherEquipList` is the large row; the review compares it with a Ghidra decompile of `32b18220` and the per-group position lists in `_CONST_LUCIEL_CH1L_` ).

## Amended by char-luciel CH1l2 (2026-11-15)

- CH1l2 declared these class helpers in `X2GULuciel.h` ( `body in CH1l3`, comment `June <va>` ): write each body under exactly that name and signature. Names for the rows CH0 left `?`: `32b16940` `ShowActiveSkillCutInAndLight( fTimeToShow, iSkillCutInSetSubIndex, bOnlyLight )`
  ( CH1l2's `ShowActiveSkillCutInAndLightByScript` passes `m_iSkillCutInSetSubIndex` ), `32b204d0` `CancelSummonDarkness` ( the revenge-buff end: June's `NoStateChangeActionFrameMove` calls it on user action 42, `SpecialAttackNoStageChange` posts that action ), `32b4c7f0` `IsOpenMindConditionMet() const`
  ( the class's Open Mind passive is learned and its combination point threshold, ability 2 default 5, is reached ), `32b0ef80` `GetOpenMindSkillID() const`, `32b14d10` `UpdateOpenMindBuff`, `32b0eed0` `UpdateStableDarkEnergyBuff`, `32aeb1a0` `UpGageNowMp( fChange, iMpForm )` ( CH1l2 passes `m_iSkillUseMpForm`, 0 / 1 or the ctor's 3 ),
  and the eight CommonFrameMove callees: `32b1f650` `FrameMoveChaserOfDarknessEffect`, `32b20780` `FrameMoveShadowBoltSpear`, `32aebb60` `FrameMoveTwilightJudgementMp`, `32b15780` `FrameMoveAuraOfFear`, `32b34a90` `FrameMoveDeathTouchEffect`, `32b16760` `UpdateInvasionOfDarkEnergyTargetTime`, `32b35c60` `FrameMoveAltarOfEvilEnd`, `32b0e550` `FrameMoveBoundingKeyDialog`.
- `32b4cb40` ( CH0 `?`, the `std::find` over `m_vecDamageEffectHitCount` ) is written inline in CH1l2's `RegisterDamageEffectHitCountEntry`: no body owed. `32ae9d00` `GetEffectOffsetByIndex( D3DXVECTOR3*, int )` is a file-local function in `_CONST_LUCIEL_CH1L2_` ( `X2GULuciel.cpp` ): call it from `PlayAltarOfEvilEffectSet` `32b35600`.
- Member names from CH1a that CH1l2 found wrong: `m_iSpecialAttackCombinationPointCost` is now `m_iSkillUseMpForm` ( +0x96b8, the form that pays a skill's MP ), `m_hSoulShockEffectSet` is now `m_hSoulShockDamageEffect` ( a `CX2DamageEffect::CEffectHandle` ). `m_iLastUsedSkillID` ( +0x96bc ) is paid to `UpCombinationPoint` by `SpecialAttackCooldown`, so it holds a point count, not a skill id ( name kept; the CH1p duo-switch phase renames it with its readers ).
- The review covers CH1l2 too: read its Done note and compare `CommonFrameMove` ( `32b397a0` ), `CanUseSpecialAttack` ( `32b3a310` ), `CommonStateStart` ( `32b37910` ) with a Ghidra decompile.
