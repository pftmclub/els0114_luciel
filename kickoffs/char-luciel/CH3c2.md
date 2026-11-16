# June character Lu/Ciel (Stage 8) - Phase CH3c2 - Demonio (129) skills, part 2

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase char-luciel CH3c2` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH3c, CH3c20 review
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL SKILL` / `NUM`) (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |
| Review | kickoffs/char-luciel/CH3c2-review.md (Opus 5.5 high; the phase writes it when done). **It covers CH3c, CH3c2** (user, 2026-11-10): every covered Done note, `flagcheck --range` over all of them, `--check-all`, retires any debug define |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Starting context" "Phase CH3 - Skills: second jobs (127, 128, 129), hyper actives, transcendence" "Phase CH1 - Deploy unit: the class core"
```
Then the planning digests in `june2017_re/char_luciel/` the task names, and the Done notes of the phases this one depends on
(their routed gaps). Verify a digest line before building on it.

## Task

Do the `CH3c2` row of "Phase CH3 - Skills: second jobs (127, 128, 129), hyper actives, transcendence", from `ch0_state_skill.tsv` / `ch0_class_map.tsv` (CH0's amendment names the filters). Each skill is tested
from an A and a B slot on the body that casts it; the hand-off gives the unit, class, skill id and slot, never slot 4 / 9 before
transcendence.

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

- **Functions: 40 to transcribe** (+ 41 thunks) (`awk -F'\t' '$6=="CH3c2"' june2017_re/char_luciel/ch0_class_map.tsv`; all new).
  June addresses: `32b06fb0` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_X_EventProcess `32b072f0` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_XX_EventProcess `32b07630` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_XXX_READY_EventProcess `32b076d0` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_XXX_LOOP_EventProcess `32b07780` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_XXX_END_EventProcess `32b07a40` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHREWD_CUTTER_StateStart `32b07ac0` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHREWD_CUTTER_EventProcess `32b07b20` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHREWD_CUTTER_ATTACK_A_StateStartFuture `32b07c30` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHADOW_FURY_EventProcess `32b07eb0` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_END_EventProcess `32b07ef0` LSI_SA_LDN_TRANSCENDENCE_END_EventProcess `32b0ed50` LSI_SA_LDN_ABYSS_SNIPING_READY_EventProcess `32b0ee00` LSI_SA_LDN_ABYSS_SNIPING_UP_EventProcess `32b0ee40` LSI_SA_LDN_DEMONIC_RUSH_START_EventProcess `32b0ee80` LSI_SA_LDN_DEMONIC_RUSH_LAST_EventProcess `32b135e0` LSI_SA_LDN_DESPERADO_FrameMove `32b13730` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_READY_EventProcess `32b13860` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_Z_LOOP_EventProcess `32b14030` `32b15290` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHREWD_CUTTER_ATTACK_A_EventProcess `32b16fc0` LSI_SA_LDN_DESPERADO_StateStart `32b17040` LSI_SA_LDN_DESPERADO_ATTACK_END_StateStart `32b172d0` LSI_SA_LDN_ABYSS_SNIPING_READY_FrameMove `32b31e90` LSI_SA_LDN_DESPERADO_ATTACK_READY_StateStart `32b32090` LSI_SA_LDN_DESPERADO_ATTACK_FrameMove `32b32550` LSI_A_LDN_F_RAPID_IMPACT_MOVEATTACK_StateStart `32b32800` LSI_A_LDN_F_PUNISHER_FrameMove `32b32b30` LSI_A_LDN_M_PUNISHER_FrameMove `32b32fe0` LSI_SA_LDN_SINISTER_MIND_FrameMove `32b33480` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT_EventProcess `32b336a0` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WALK_EventProcess `32b338c0` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_LANDING_EventProcess `32b33b00` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHADOW_FURY_StateEnd `32b33bf0` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHREWD_CUTTER_END_StateStartFuture `32b33dc0` LSI_SA_LDN_GARDEN_OF_HAURES_FrameMove `32b36d90` LSI_SA_LDN_ABYSS_SNIPING_UP_FrameMove `32b3a2f0` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_END_StateEnd `32b43b10` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_START_StateStartFuture `32b43ce0` LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_START_StateStart `32b4ce50`.
  Still `?` (name it by the class pattern from the hint in col 7): `32b14030` `32b4ce50`.
- **States:** the ones whose callbacks are listed above (the state table's col 8 keeps the unsplit `CH3c`).
- **Checklist rows** (`ch0_checklist_rows.tsv` col 5): §7y L680 GetLandPosition bMonster true in Luciel class code; §7ac L774 ProcessLockOn (3200fa20) is shared with Luciel; §7ai L835-841 pass June skip / adjust / level / creator at every ported BFI call.
- **Depends on CH3c20** (ProcessLockOn, Garden binders). Garden of Haures spawns 2269 from `32b33dc0` (`@32b340ce`).

## Amended by char-luciel CH1l3 (2026-11-15)

- `RegisterDesperadoTarget` ( `32b18060` ) pushes each target with its float time set to the raw word -1 ( a NaN, JUNE_BUGS 720 ); the Desperado frame move reads `m_vecDesperadoTarget` as June does ( no clamp or compare on that time unless June has one ).

## Amended by char-luciel CH2d20 (2026-11-15)

- The transform window is whole ( `CX2TransformSkillSlotUI`, `X2GUUser.h` / `.cpp`, flag `SERV_IRUHADEV_JUNE_CHAR_TRANSFORM_LINK` ). The transform start ( `32b43910` / `32b43b10` ) calls,
  by March name: `new CX2TransformSkillSlotUI()` ( `3200f380` ), `SetSlotSkill( iSlot, eSkillID, iSkillLevel )` ( `31fdc4c0` ), `SetShow( bShow )` ( `31fe2b80` ),
  `SetTransformEndTexture( wstrFile, wstrKey )` ( `31fd4930` ); before every delete `ReleaseDialog()` ( `31fd03f0`, already in the three Lu/Ciel delete sites ).
  The user's wrappers are `HasTransformSkillSlotUI` ( `31fd0380` ), `GetShowTransformSkillSlotUI` ( `31fd03b0` ), `SetShowTransformSkillSlotUI` ( `31fe3c90` ); the UI manager
  menu is `CX2UIManager::UI_TRANSFORM_SKILL_SLOT` ( June 44 ). The per-frame cool-time count, the menu toggles and the training-F6 reset are hooked ( `LUCIEL_LIVE` ): do not add them again.

## Amended by char-luciel CH3c20 (2026-11-15)

- **Lock-on:** `m_ptrShrewdCutterLockOn` ( +0x9830 ) is `CX2GUUser::sLockOnSystemPtr` now ( flag `SERV_IRUHADEV_JUNE_CHAR_LOCKON_SYSTEM` ); June `3200fa20` =
  `m_ptrShrewdCutterLockOn->ProcessLockOn( this )`, its range setters / `GetSafePos` / `GetDestPos` / `SetFacingLineRange` as Ara's. CH3c creates the
  instance ( InitState @`32b490e0` ); if CH3c has not, this phase must not dereference a null pointer.
- **Garden of Haures:** the NPC script's four methods are bound ( `SERV_IRUHADEV_JUNE_CHAR_NPC_LUA_BINDS`; `luabind.py check SA_LDN_GARDEN_OF_HAURES` PASS );
  its plants ( 2270 ) carry the owner the script passes ( `GetOwnerUserUnitUID` ), so check that the Garden NPC itself gets the user as its owner
  ( June's `322e67c0` arguments at `@32b340ce`: ally UID and `NAT_ALLY` ), or `IsSameOwnerUserID` never matches. The `NPCTemplet` rows of 2269 / 2270 are CH3a0's.

## Amended by char-luciel CH2d3 (2026-11-16)

- **Ciel's form model is already wired:** the buff hook `SetSpecificValueByBuffTempletID` ( `32b37d40` ) starts `StartTransformedForm( 1 )` whenever Ciel is the body in play
  ( `Motion_Ciel_LDN_Evil.X`, anim `Wait`, `EffectSet_LDN_Ciel_Horn` ), for BTI 763 or 764. Your Transcendence start state ( `32b43b10` ) only applies the buff and the window,
  as CH2d2's Altar of Evil start does; the form's end goes through `EndTransformedForm( 1, ... )` from your end state. The transform window's key labels and A / B type
  follow the skill tree ( CH2d3's `UpdateTransformSkillSlotString` / `SetTransformSkillUIType` hooks ).

## Amended by char-luciel CH2d3 fix (2026-11-16): form states call the base StateChange

- **Every transform-form state function calls `CX2GUUser::StateChange` where June calls `320348c0` / `32034c00` directly**, never the bare `StateChange` ( Lu/Ciel's override
  `32b4db90` sends any id through `GetStateIDByForm`, which turns every id it does not list into the form's WAIT while transformed: Lu's form was stuck in WAIT, CH2d3 play-test ).
  June's direct sites in the Transcendence form ( `june2017_re/char_luciel/` has no table; regenerate with the CH2d3 Done note's method ): `32ae95a0`, `32b059a0`-`32b07eb0`,
  `32b13730`, `32b13860`, `32b15290`, `32b33480`, `32b336a0`, `32b338c0`, `32b33bf0`. Common ids ( `LSI_LU_*` ) stay on the virtual call, as June. Tag each such line
  `// char-luciel <phase>: June calls the base, not the form mapper`.

## Amended by char-luciel CH3c (2026-11-16)

- **Three cases are yours to land.** Transcendence ( 30049 / 30080 ), Abyss Sniping ( 30059 ) and Demonic Rush ( 30060 ) are not registered in `SetEquippedSkillFuncBySkillSlotIndex`: their states would run `XAP_ONE_WAIT`
  forever without your event processes ( `stateends.py` FAIL ). The finished case bodies, from June's listing with the physical stack offsets, are in `june2017_re/char_luciel/ch3c_held_cases.txt`: paste them into the switch
  ( before the HEAVY_HIT case ), turn each `// June <thunk> ( <slot> ): <name>, body in CH3c2` line into its `SET_CB_FUNC` line as you write the function, and run `stateends.py check` ( it must print PASS ).
- **The other cases are written** and wait for your callbacks the same way: every `body in CH3c2` comment in the seven registered Demonio cases ( Desperado, Sinister Mind, Garden of Haures, Rapid Impact, Punisher; Abyss Extinction and Rapid Fury have none ).
- **Slot attribution:** `ch0_skill.tsv` puts the preload of the male / Ciel states ( Rapid Impact 465, Punisher 467, Rapid Fury 581 ) in StartFuture and the move-attack start ( 464 / 466 ) in Start: the physical depth of the cdecl delegate makers says
  **Init** for the preloads and **StartFuture** for 32b32550 ( the class map's `..._MOVEATTACK_StateStart` is a StateStartFuture ). The registered cases already use that; name your function `LSI_A_LDN_F_RAPID_IMPACT_MOVEATTACK_StateStartFuture`.
- **Written here, unused until the held cases land:** the Transcendence form's jump, combo and start event processes ( June's `32ae95a0` .. `32b06cd0` ), the three state starts, and `m_ptrShrewdCutterLockOn = SLockOnSystem::CreateLockOnSystem()` ( in the held case ).
- **`m_bSkillState`** is also set on 494, 495, 500 ( Transcendence ) and on the Punisher Ciel state ( 467 ): the CH1o3 digest listed only 466 and 581.
- `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL_DEBUG` is on ( `Always.h` ); your review retires it.

## Amended by char-luciel CH3c fix ( 2026-11-16 )

- Already written and registered by the CH3c fix ( commit `a6edaf9d`, play-tested ): `LSI_SA_LDN_SINISTER_MIND_FrameMove` ( `32b32fe0` ), `LSI_A_LDN_F_PUNISHER_FrameMove` ( `32b32800` ), `LSI_A_LDN_M_PUNISHER_FrameMove` ( `32b32b30` ),
  `LSI_SA_LDN_GARDEN_OF_HAURES_FrameMove` ( `32b33dc0` ). Skip them; the remaining `body in CH3c2` comments are yours ( Desperado's start / frame moves, Rapid Impact's move-attack start future ) plus the three held cases.
