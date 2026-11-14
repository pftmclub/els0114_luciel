# June character Lu/Ciel (Stage 8) - Phase CH1o - Class core: Virtual wiring, June-only keys, hooks

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1o` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1n |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (compiled, unreferenced) and `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` (`//#define` until CH1 final) |
| Log tag | none (nothing runs until `_LIVE`; no play-test) |
| Review | kickoffs/char-luciel/CH1o-review.md (Opus 5.5 high; the phase writes it when done). **It covers CH1m, CH1n, CH1o** (user, 2026-11-10): every covered Done note, `flagcheck --range` over all of them, `--check-all`, retires any debug define |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Starting context" "Phase CH1 - Deploy unit: the class core" "Phase CH0 - Lu/Ciel class map (read-only)"
```
Then the planning digests in `june2017_re/char_luciel/` the task names, and the Done notes of the phases this one depends on
(their routed gaps). Verify a digest line before building on it.

## Task

Do the CH1o row: the 119 overrides wired against March's `CX2GUUser` vtable by role (CH0's column), the June-only keys CH0 gave CH1
(`ch0_keys.tsv`), and the checklist hooks CH0 assigned (`ch0_checklist_rows.tsv`).

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

- **Checklist rows** (`ch0_checklist_rows.tsv` col 5): §5 L184-188 keys DUNGEON_SPEECH_LUCIEL, LU, CIEL x4 (INIT_SYSTEM Duo L1-22, STR_ID_33342-333; §7 L260 vslot 510 skill lookup 32b3a310 (+ slot 545 = 0x884 32b3acf0): quick-slot specia; §7 L263 vslot 510 copy @32b3a61f after CheckSkillUsePVPOfficial: riding-key skill wait; §7ap L977 slot 0x56c skill icon override 32b16220 and slot 0x6a8 shown cool time 32aea8c0 .
- June-only keys and owners in `ch0_keys.tsv` (357 rows, counts = the plan's census). The keys `CX2GUUser::CommonStateStart(Future)` parses (`DUO_TYPE`, `MINOR_UNIT_*`, `CP_AMOUNT`, `USE_NOT_SELECT_*`, `USE_PHYSICPROCESS_NOT_SELECT_UNIT`, `FORCE_DYNAMIC_CAMERA_INDEX` @3205c0e8, `RETAIN_CAMERA_STATE` @3205b934, `SUPER_ARMOR_CUSTOM_COLOR` @3205941f) are DU1's parse; the class only reads the members. `DEFAULT_UNSELECTED_UNIT_MP_CHARGE_VALUE_PER_SECOND(_AT_PVP)` is read by InitComponent 0x3c `32b0f080` (CH1k).
- No June reader (dead data, nothing to port): `HEAD_IK`, `HEAD_IK_MAX_ANGLE`, `MINOR_SPEED_TIME0` (Duo L7969 typo), `STOP_TIME_ATK` (Demonio typo of `STOP_TIME_ATT`), `DAMAGE_DATA_MEMO`, `STATE_STRING`. Immunity keys (`31fdaaa0`) stay on the Stage 2 bullet, as for Chung.

## Amended by char-elesis CH1o (2026-11-13)

- Lu/Ciel (vtable `334f06fc`) is the only class that overrides the damage-state getters 0x46c-0x4a4 / 0x114 / 0x118. June's `CX2GUUser::IsDamageState` (vslot 0x534 `3208e8c0`) asks those getters; char-elesis CH1o wrote it with March's `USI_DAMAGE_*` names. When his getters are ported, make the base ask them (or override `IsDamageState`) so his own damage states count.

## Amended by char-luciel DU1b (2026-11-14)

- **User vslot 0x848** (base `31f3f070` empty; Lu/Ciel `32b18170`): June's Now `CommonStateStart` calls it when `IsActiveBodyChangeReached()`
  (`31fc9510`), just before clearing `m_bActiveBodyChangeWaitNow`. DU1b left a stand-in comment there: declare the virtual (name it by what
  `32b18170` does) and add the call. **Vslot 0x648**'s Lu/Ciel override (`32b0fa20`) clears `m_bActiveBodyChanged` (+0x84dc, DU1b's member).

## Amended by char-luciel CH1k (2026-11-14)

- Three Lu/Ciel overrides were written with no March base to override ( `X2GULuciel.h`, marked `+` ): declare the bases and widen the guards with `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`.
  **Vslot 0x2e4** `DoProcessWhenDamageDataCreated( const CX2DamageManager::DamageData* )` ( new, base `3208ddc0` empty ): add it to `CX2GameUnit` next to `DoProcessWhenDamageDataDestroyed` and the
  call June's DamageData init `32610da0` makes ( caller `31f8ee59`: the attacker's vslot 0x2e4( this ), the sibling of Ara's `32610d50` dtor hook ). **Vslot 0x2fc**
  `DoProcessWhenDamageDataDestroyed` ( `X2GameUnit.h`, `SERV_IRUHADEV_JUNE_CHAR_ARA_LIVE` guard ) and **0x300** `AdjustDamageDataBeforeDamageReact` ( `X2GUUser.h:1674` and the four user-attacker call
  sites, Raven's `_LIVE` guards ): widen to Lu/Ciel as the CH1k kickoff text says ( Lu/Ciel has summoned NPCs: CH1o decides `CX2GUNPC`'s `32778fe0` by that text ).
- `CX2GameUnit::GetNowMp / GetMaxMp / SetMaxMp / FlushMp / UpNowMp / SetNowMp` are already virtual ( flag `SERV_IRUHADEV_JUNE_CHAR_UNIT_MP_VIRTUAL`, CH1k ): the wiring list needs nothing for them.

## Amended by char-luciel CH1k2 (2026-11-14)

- **Eleven Lu/Ciel getters have no virtual base in March** ( `X2GULuciel.h`, marked `+`; declared `virtual` in the class only ): vslot 0x438 `GetWaitStateID` ( `X2GUUser.h:1759`, non-virtual ), 0x43c `GetWalkStateID`, 0x440 `GetDashStateID`, 0x444 `GetDashEndStateID`
  ( non-virtual, `X2GUUser.h:1742-1744` ), and 0x448 `GetDashJumpStateID`, 0x44c `GetDashJumpLandingStateID`, 0x450 `GetJumpReadyStateID`, 0x454 `GetJumpUpStateID`, 0x45c `GetJumpLandingStateID`, 0x460 `GetDieFrontStateID`, 0x464 `GetDieBackStateID`
  ( absent in March; June's bases are `3208ddd0 .. 33069eb0` returning the `USI_*` ids ). Add the bases ( non-const, as Lu/Ciel's ) under `SERV_IRUHADEV_JUNE_USER_DAMAGE_STATE_GETTERS`'s sibling guard or a new class-agnostic flag, make the
  engine and class readers ask them, and widen with `_LIVE`. The damage getters and `GetJumpDownStateID` are DU2's and already virtual.
- `StateChange( int, bool )` and `( int, int, bool )` are overridden in Lu/Ciel ( CH1k2 ): the March bases are already virtual.

## Amended by char-luciel CH1l (2026-11-15)

- Twelve Lu/Ciel overrides were written with no March base ( `X2GULuciel.h`, marked `+`, virtual in the class only ): add each base as a virtual ( non-const unless noted ), make the engine callers ask it, and widen with `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`:
  0x574 `CanPlayEmotion( EMOTION_TYPE )` ( base `32ec5990` returns true; `PlayEmotion` `32043380` @`320433d7` asks it first ), 0x580 `IsTransformSkillState` ( `31e4c550` false; `CommonStateStart` `32061976` stores `+0x94c0 = !it()` ),
  0x640 `SetShowViewListAndUpdate( bool )` ( `31fca010`: `SetShowViewList( b )`, then `UpdateNotRenderingOtherEquipList()` when b ), 0x650 `SetNowMpFromStoredValue` ( `31fceed0` = `SetNowMp( Now fNowMP, 0 )`, called every frame after `SetNowHp` in `CommonFrameMove` @`3206c1e6` ),
  0x648 `DoProcessWhenActiveBodyChangeApplied( int iDuoType, int iSkillID )` ( `33093ad0` empty; the call is June's `CommonStateStart` @`32051059`: ( DUO_TYPE value, the key int read before ) ), 0x570 `GetSkinAnimByForm( int ) -> CKTDGXSkinAnimPtr` ( base `31fd48a0`: form 1 ? `m_pMinorUnitXSkinAnim` : `m_pXSkinAnim` ),
  0x62c `SetHandEquipShow( bool )` ( `3208ddc0` empty; `StateChange` @`320349cb` with 0, `PlayEmotion` @`320447e9` with 1 ), 0x504 `GetSkillIconTexture( CKTDGUISlot*, const SkillTemplet* )` ( `31fdb990` = `SetItemTex( icon, piece )` ), 0x64c `SyncFutureMpFromGage` ( `31fe9da0` ),
  0x568 `CheckBoundingTarget() -> bool` ( `31e4c550` false; callers are the CH1h / CH1i states ), 0x87c `SetSDOnePieceAvatar( int, bool )` ( `3208e280` stores `m_bSDOnePieceAvatar` ), 0x5c4 `GetDanceWaitStateID` ( `3208e140` = the dance wait state ).
- `IsHyperModeState` ( 0x51c ) is declared only under `SERV_IRUHADEV_JUNE_HYPER_CUT_IN_TIME` in `X2GUUser.h`; widen its guard so `_LIVE` alone compiles Lu's override as an override.

## Amended by char-luciel CH1l2 (2026-11-15)

- Nine more Lu/Ciel overrides were written with no March virtual base ( `X2GULuciel.h`, marked `+` ): add each base as a virtual and widen with `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`:
  0x668 `StateChangeDashIfPossible` ( `31fc85b0`; March's is a plain member at `X2GUUser.h:3097` ), 0x658 `IsSkillChainReady( int, int )` ( `330fb7d0` stub, answers false ), 0x740 `StoreGageSnapshot` ( `32009780` ) and 0x744 `RestoreGageSnapshot` ( `3204da60` ) ( both Crest of Time, not ported: empty bases ),
  0x7fc `SpecialAttackUse( int, bool )`, 0x800 `SpecialAttackCooldown( int, float )` and 0x7f8 `CanUseSpecialAttack( ACTIVE_SKILL_USE_CONDITION )` ( bases `31aa1d30`, pure stubs; the other classes carry them as plain members ), 0x664 `ResetMaxMP` ( `31fd2810` = March's `CX2GUUser::ResetMaxMP` ), 0x848 `RegisterDamageEffectHitCountEntry` ( `31f3f070`, empty ).

## Amended by char-luciel CH1l3 review (2026-11-15)

- **Effect-set modifier tags 4, 10, 11** ( checklist 7a ): March's `CX2EffectSet::ApplyDamageEffectModifiers` ( `X2EffectSet.cpp` ) has no case for them, so the default only logs `[JUNE_FX] effect-set modifier tag N is not ported`. Lu/Ciel push all three: tag 4 ( CH1f2 Power of Oriax, an int on the damage effect, added ), 10 ( Punisher: CEffect `+0x2a4` = DamageData `+0xa4` `m_ExtraDamage.m_Damage`, a protected float ), 11 ( Punisher: CEffect `+0x7a8` = DamageData `+0x5a8` relate skill index byte, rounded ).
  Transcribe June `321db1b0`'s three cases ( jump table `321dbc60` ) into March's switch under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE`, as Eve did tags 5 / 12 / 16 / 20 under `_EVE_CORE`. In the decompile, the tag 10 and 11 NEXT-table loops write the main effect's own value again, not each table's: confirm it in the listing and keep it with a `JUNE_BUGS` row if so.
  Tag 9 ( force down ) is already in, but only under `SERV_IRUHADEV_JUNE_CHAR_ELSWORD_SKILL`; Lu/Ciel's Punisher needs it too ( widen the guard, or say why it stays ). Touches `X2EffectSet.cpp` ( add it to the STATUS Touches column ).
