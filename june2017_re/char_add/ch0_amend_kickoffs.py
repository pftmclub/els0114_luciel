r"""char-add CH0: write the "Amended by CH0 (2026-11-13)" section of every kickoff (rewrites the section each run: idempotent).
    python june2017_re/char_add/ch0_amend_kickoffs.py [--dry]
Counts per phase: class-map functions (ch0_class_map.tsv col 7; col 9 y = carved, n = listing-only; col 5 inside / new; thunk / dead / not-Add
rows left out), script states (ch0_state_skill.tsv part 1 col 8), skill ids (part 2 col 6), checklist rows (ch0_checklist_rows.tsv col 5), plus
the Opus verdicts in NOTES.
"""
import os, re, sys, collections

HERE = os.path.dirname(os.path.abspath(__file__))
T = os.path.normpath(os.path.join(HERE, '..', '..'))
K = os.path.join(T, 'kickoffs', 'char-add')
D = 'june2017_re/char_add/'
HEAD = '## Amended by CH0 (2026-11-13)'
PHASES = ('CH0c CH0b CH1a CH1b CH1c CH1d0 CH1d CH1d2 CH1e CH1f CH1f2 CH1g CH1h CH1h2 CH1h3 CH1h4 CH1i CH1j CH1j2 CH1k CH1k2 CH1m CH1n CH1o CH1p '
          'CH1q CH1q2 CH1 CH5 CHNa CHNb CH2a0 CH2b0 CH2a CH2b CH2c CH2d CH3a CH3b CH3c CH3c2 CH3d CH4a CH4b CH6 Z').split()
SKIP_KIND = ('thunk', 'dead', 'not-Add')


def tsv(name):
    p = os.path.join(HERE, name)
    return [l.rstrip('\r\n').split('\t') for l in open(p, encoding='utf-8') if l.strip() and not l.startswith('#')]


cm = [r for r in tsv('ch0_class_map.tsv') if re.fullmatch(r'[0-9a-f]{8}', r[0]) and len(r) > 8]
st_rows, sk_rows, part = [], [], 1
for l in open(os.path.join(HERE, 'ch0_state_skill.tsv'), encoding='utf-8'):
    if l.startswith('# part 2'):
        part = 2
    if l.startswith('#') or not l.strip():
        continue
    r = l.rstrip('\r\n').split('\t')
    if part == 1 and r[0].startswith('ANSI_') and len(r) > 7:
        st_rows.append(r)
    if part == 2 and r[0].isdigit():
        sk_rows.append(r)
cl = [r for r in tsv('ch0_checklist_rows.tsv') if r[0] != 'section']

NOTES = {
 'CH0c': [
  'Map: `' + D + 'ch0_create.tsv` (22 items; raw June / March pairs and decompile notes in `ch0_create_raw.tsv`). Under ~25 functions: no split.',
  '**Stat rows: none to write.** The flat `StatTable.lua` (e37c9211) already has 99 rows for 9 / 32 / 33 / 36 / 122 / 123 / 126; decision 1 is moot.',
  'Code: lift the US limit for `UT_ADD` only, under `_CREATE`, at `X2StateCreateUnit.cpp:132-135` (AddCreateUnitSlot) and the seven `X2UIPersonalShopBoard.cpp` combo blocks (:662, :713, :768, :815, :862, :909, :956); leave the default-select switch `:175-220` (lifting the constant would open the screen on Add; June selects Ain). Check `X2UIItemExchangeShop.h:11-21` (US 8 slots per set) against June and include Add\'s slot if June shows it.',
  'June and March agree on every Add piece of the screen (SelectAdd `32555320` / `:2346`, buttons, stand mesh `32551e30`, dummy `3255b800`, stat chart, flag colour, background, voice files, movies): no transcription expected; read SelectAdd once against the June decompile (scratch dumps are gone with the CH0 session: re-pull).',
  'Data: adopt June\'s `DLG_Unit_Create_ClassChange_Button_Add.lua` (6 radio buttons + `ClassChange_ID` 3 pictures, March 2 + 1; `luabind.py check` 22 calls, 0 unbound, 0 enum missing); the four June-only buttons are harmless until CHNa (lines 2 / 3 return UC_NONE, `SelectUpperClass` returns at `:3403`). Stage June\'s `DLG_UI_Common_Texture_GUI_character_info_09.tga` / `.TET` (data057; same size, different bytes). Keep March\'s `DLG_Unit_Create_Unit_Button_New.lua` (Add\'s slot exists, only hidden; June\'s layout comes with Lu/Ciel NU4).',
  'Open checks in the phase: does June\'s `SelectUpperClass` `325519d0` write the selected class (`+0x2c0`) that its create request `325584f0` sends (March sends the unit type; for base Add both are 9); March\'s voice format `L"%d.ogg "` has a trailing space (`:4050`): the play-test listens for the create voice.',
  'Emulator: nothing refuses class 9 (`Handlers_Unit.cpp:311` / `:330`); `SeedBeginnerGear` gives 112991-112995 and costume 41757-41761 (`X2OfflineInventory.cpp:1236`): update the stale "unreachable" comments at `:1213` / `:1236`.',
 ],
 'CH0b': [
  'Stage: `python tools/kom.py stage-june ' + D + 'ch0b_stage_list.tsv` (7,686 rows). CH0 dry run: `STAGE-JUNE DRY RUN: 29 kom(s), 7686 staged (7637 new, 49 replaced, 0 identical), 0 refused`. New kom **data207** (57 June-only motions: `Motion_ADD_AT.x`, `_MM.x`, `_TT.x`, `_DE.x`, `Motion_Standard_Add.X`, ~50 `Motion_Add_Emotion_*`); `Motion_Npc_Add_Arctracer.x` -> data148; same-name motions go to data033 now: `Motion_ADD.x`, `_Common`, `_Field`, `_Lobby`, `Motion_Add_Emotion.x`.',
  '**Held for CH1 final** (`' + D + 'ch1_held_motions.tsv`, out of the list): `Motion_ADD_PT.X` (June renamed `APT_NA_Ready` / `APT_NA_preAttack` / three `SI_*_APT_*` skills to `NA_*` / `SI_*_AN_*` and moved them into `Motion_ADD.x`) and `Motion_ADD_LP.x` (June moved `SI_A_ALP_TWIRL_RUSH` / `_HYPER` into `_PT`): March\'s script plays the old names.',
  'Adopt (bytecode): `DLG_1ST_Class_Change_Guide_Add.lua`, `DLG_2ND_Class_Change_Guide_Add.lua` (new), `DLG_Class_Change_Add_AT` / `_DE` / `_LP` / `_MM` / `_PT` / `_TT.lua`, `Class_Change_Guide_Add.lua`; `DLG_Unit_Create_ClassChange_Button_Add.lua` is CH0c\'s. Already June\'s in March: `CREST_OF_TIME_Ring.dds` / `_Time.tga`, the Moonlight Rhapsody `.tga`s, `DLG_Add_DP_And_Mutation.lua`, `DLG_Transcendental_Change_Guide_Add.lua` (its `POPUP_TR_L_PSYKER` / `_MASTERMIND` / `_D_ESPER` pictures are in the list, data215).',
  'Skill icons: `python tools/skillicons.py check 106-106 900-900 29000-29060 29201-29233 29400-29420 29600-29605` = 149 misses on 7 sheets today (`DLG_Add_SKILL_ScreenShot_01-04`, `DLG_SKILL_ADD_Active_02-04`, .tga + .tet), all in the list: must PASS after the copy.',
  'Effect closure (`ch0_effect_closure.tsv`): 9 names exist in no June kom (June\'s own gaps): `HQ_CutIn_Ara_normal.tga`, `HQ_CutIn_Ara_Rage.tga`, `Energy.wav`, `Break.wav`, `Change.wav`, `Add_NA_Effect04.ogg`, `AddVoice_Stasis_Field04.ogg`, code literals `add_EFFECT_Cycle_Blr01.dds`, `ADD_PULSE_GATLING_Dron_Mesh_01.X`.',
 ],
 'CH1a': ['`ANSI_CAN_NOT_ATTACK_JUMP_DOWN` has two tables in June\'s script (lines 4343 / 4348; the second wins). `ANSI_THROW_ITEM` = skill 900 (type 12). The four hyper tables (`ANSI_HYPER_MODE`, `_ALP_`, `_AMM_`, `_ADE_HYPER_MODE`) come from the base hyper row id 23 (registrar `32a1e4f0` @832, Start `32a31190`).'],
 'CH1b': ['Every job-tagged START / WIN / LOSE table (36: APT / ALP / AAT / AMM / ATT / ADE x START, WIN, WIN2, LOSE, LOSE2) is here, rolled by `INIT_COMMON_RANDOM_STATE` (ids 9 / 11 / 12). June registers `ANSI_RIDING_DAMAGE_BACK` (id 86) and `ANSI_WALL_LANDING` (164) with no script table (`ANSI_RIDING_DAMAGE_FRONT` has two tables, lines 6024 / 6031).'],
 'CH1c': ['Only 2 class functions: the riding, pepper-run and dance states use shared callbacks. A short phase; it is not merged (plan rule).'],
 'CH1d0': ['`' + D + 'ch0_unmapped_identified.tsv` (58 rows: 45 same, 11 june-only, 1 differs, 1 unknown): the 12 non-`same` rows whose first caller is a Sonnet phase are this phase\'s; `32294530` is flagged needs-Opus (~14 KB). `32d27770` (differs) is CH1q\'s. No other `CH<n>0` phase: every other unmapped callee is `same` (std / compiler helpers or March functions).'],
 'CH1d': ['Calls `32293d70` (line-side test): CH1d0 writes it first. The untagged nasod-armor combos (`ANSI_COMBO_NASOD_ARMOR_*`, 11 tables) are here; their nasod lines go under `_NASOD` inside the `_LIVE` body. March\'s class tests in `NasodArmorComboZZEventProcess` / `NasodArmorComboXEventProcess` (`X2GUAdd.cpp:5798` / `:5910`) follow June\'s bodies.'],
 'CH1e': ['`APT` / `ALP` combo states. `ANSI_APT_DASH_JUMP_COMBO_ZX_LANDING` (id 270) is a `~` registration row (passed the listing check).'],
 'CH1f': ['CH0 split: CH1f = the `AAT` / `AMM` states and their non-container helpers (41); CH1f2 = the container helpers at `0x32a2c000`+ they reach (21). `ANSI_AAT_CREATE_DYNAMO_FACTORY` is registered five times (ids 291-294, 299; Start `32a288d0` via thunk `32a31730`, EventProcess `32a18ec0`): key by id. `ANSI_AMM_COMBO_XXDownX_END` (id 237) is a `~` row. June registers `ANSI_AMM_DASH_COMBO_XX` (279) with no table.'],
 'CH1f2': ['CH0 split off CH1f: 21 container / template helpers (`32a2cbc0` .. `32a315f0`); several are also called from outside the class (shared template code): write each once, plain names.'],
 'CH1g': ['`ATT` / `ADE` states; the ADE formation-mode combos (`ANSI_ADE_COMBO_XXXZ_` / `XXXX_FORMATION_MODE` / `_NO_FORMATION_MODE`) carry DP lines under `_DP`. `ANSI_ADE_COMBO_XXXX_FORMATION_MODE` (id 265) is a `~` row. June registers `ANSI_ADE_COMBO_ZZXXX` (285) / `_END` (286) with no table.'],
 'CH1h': ['CH0 split: CH1h = carved overrides below 0x600 + ctor `32a22170` (vtable stores 334daaec / 334daae4) + **dtor body `32a23b90`** (the plan called it a second ctor) + scalar deleting dtor `32a316c0` + secondary-vtable adjustors `32a31530` / `32a31540` (25); CH1h3 = their container helpers (33). InitComponent `329cf870` reads the four June-only `INIT_COMPONENT` keys (`DP_RATE_BY_FORMATION_MODE_USE_MP_3RD_LINE_SKILL`, `DP_USE_RATE_WITH_FORMATION`, `VOID_IMPACT_UP_ANGLE` / `_DOWN_ANGLE`; DP reads under `_DP`). Unnamed for this phase to name by body: 0x2a0 `329e21f0`, 0x3dc `329f9a30`, 0x4c0 `329e7ca0`, 0xd0 `329ffba0`. 0xc0 ApplyHyperModeBuff and 0x2c4 / 0x3e0 / 0x4c4 / 0x4c8 / 0x4bc are not here (CH1q / CH1q2 / CH1o).'],
 'CH1h2': ['CH0 split by vslot: CH1h2 = listing-only overrides 0x30-0xc4 (0x30 NotifyDGManagerChanged, 0x40 Init `329ea8b0`, 0x44 ReInit `329ffcd0`, 0x5c StateChange `329d40f0`, 0x60 DamageReact `32a07fc0` (ResponseEmergency byte `+0x8fc2` @32a080fe L292; super-armour vslot 0x81c @32a080cc L478; March\'s UT_ADD damage block `X2GUUser.cpp:11572-11585` follows it), 0x94 Lose, 0xb4 GetHyperModeTime `32a31590`, 0xc4 HyperModeBuffEffectStart) and their helpers (27); CH1h4 takes 0x1d8-0x54c. Declarations only: 0x2a4 / 0x324 / 0x560 (CH1q), 0x3e0 (CH1q2).'],
 'CH1h3': ['CH0 split off CH1h: 33 container / template helpers the ctor, dtor and CH1h overrides reach (`32a2bc20` .. `32a300b0`); `32a2fcf0`, `32a30040`, `32a300b0` are also called from outside the class.'],
 'CH1h4': ['CH0 split off CH1h2: 0x1d8 `329e5fe0`, 0x24c / 0x250 Set / UnSetSpecificValueByBuffTempletID, 0x29c `329d4730`, 0x2ac `329d4870`, 0x2b8 SetDamageExceptionProcess `329e4a50`, 0x3d8 `329e58d0`, 0x4b8 `329d49e0`, 0x4d4 DetonationStateChange, 0x4e0 AttackResultByType `32a28860`, 0x500 `329e5bb0`, 0x540 / 0x548 particle arrays, 0x54c InitEnchantWeaponEffectUnitSpecific, their helpers and container helpers (30). Unnamed slots (0x1d8, 0x29c, 0x2ac, 0x3d8, 0x4b8, 0x500) are named by body here.'],
 'CH1i': ['0x600 HyperModeFrameMove `329ffe90`; 0x7b8 SetEquippedSkillLevel `329d1730` (18 blocks, 32 vslot-0x7a8 state calls, no member write: `' + D + 'ch0_setskilllevel_blocks.tsv`); 0x6d4 / 0x6d8 SetStasisfieldHitValue / DamageValue `329e25b0` / `329e25e0`; 0x6dc-0x6e8 the UpDPBy*ThisFrame accessors (June keeps March\'s bug: both getters read the attack flag `+0x95f5`). **Training keys settled:** slot 0x6c4 = `329e21d0` SetDPValue and 0x6f4 = `329e2130` SetMutationCount (clamp 0..6); the backlog\'s `329e2170` / `329e20e0` are GetMaxDPValue / GetMutationCount. All four are CH1q\'s (DP), declared here. 0x690 (CH4a), 0x73c (CH4b), 0x6c8 / 0x6cc / 0x6d0 / 0x740 / 0x744 (CH1q) are declarations.'],
 'CH1j': ['CH0 split: CH1j = the Common* overrides (0x7c0, 0x7c4 CommonStateStart `329ff970` (repeats the dynamo-factory test, then vslot 0x524), 0x7c8, 0x7cc CommonFrameMove `32a24560`, 0x7d4, 0x7d8, 0x7dc, 0x7e8) and their non-container helpers (50); CH1j2 = the container helpers (21). `329fdbe0` (CommonFrameMove helper) calls `3201ad10` (checklist L286): transcribe the call with it.'],
 'CH1j2': ['CH0 split off CH1j: 21 container / template helpers of CommonFrameMove (`32a2c550` .. `32a2fc70`).'],
 'CH1k': ['0x7f4 SpecialAttackEventProcess `32a07b60`, 0x7f8 slot-510 lookup `32a20e70` (L265 / L268; dev free-skill gate L266), 0x7fc `32a1e3c0`, 0x800 skill use `32a07dc0` (DP or MP), 0x80c NoStateChangeActionFrameMove `32a28350` (pushes state 207 = `ANSI_SA_APT_STASIS_FIELD_ACTIVE`). Read `32a07dc0` and GetActualDPConsume `329e7820` (`ch0_dp.tsv`) before writing ReservedSkillStart.'],
 'CH1k2': ['7 rows: 0x810 `329e8960`, 0x818 `329d4250`, 0x828 `329e6100`, 0x834 `329e80f0`, 0x84c `329d49b0`, 0x854 `329ff960` (listing-only) and one helper. The review covers CH1k-CH1k2 (CH1h-CH1h4 and CH1i-CH1j2 have their own).'],
 'CH1m': ['`' + D + 'ch0_reg.tsv`: 195 rows = registrar `32a1e4f0` 118 + `32a24c20` 68 + InitState body `32a25a90` 4 (no wrapper) + base `3202b110` 5; `ch0c_listing_check` **194 / 194** (one identical pair, id 198 `ANSI_COMBO_NASOD_ARMOR_LANDING` at lines 136 / 237 of `32a24c20`). Hand rows: `~` ids 270 / 237 / 265; roll ids 9 / 11 / 12 (`pppuVar5`); hyper id 23 (`puVar4`, Start `32a31190`); id 130 `ANSI_WAIT_STATE_CHANGE`. June registers 5 names with no table (ids 86, 164, 279, 285, 286) and 5 `USI_EVENT_*` (111-116). `ADD_NASODRULER_STATE_ID` lacks 159 of June\'s 220 names; all 61 shared names differ in raw value, 27 still differ after the USI_END rebase (100 -> 152): register by June\'s id (`ch0_state_enum_names.tsv`). March\'s `InitStateByUnitClass` (`:2314`) and `ParseCommonRandomState` (`:272-298`) class switches follow June\'s registration.'],
 'CH1n': ['`' + D + 'ch0_skill_switches.tsv` (255 rows) / `ch0_skill_listing_rows.tsv` (151): frame skill id `[esp+0xa8]`, slot `[esp+0xac]`, hyper flag `[esp+0xb0]`; (a) type - 5 at `32a28f41` (12 entries: 5 -> COMMON_BUFF / Crest compares, 12 -> THROW_ITEM, 6 / 7 / 13-16 -> (b)), (b) ids 29000-29233 at `32a29015` (234 entries, 53 targets), (c) 29601-29605 at `32a2af8e`; compares 29213 / 29231 (Crest), 29600 (Doom\'s Day), 900. No memo (V) switch; hyper copy `329cc2c0` (10 sites); 35 extra states via `31fd89b0`. 106 and the type-8 passives reach no state.'],
 'CH1o': ['0x524 `32a315a0` (listing-only, 70 bytes) and the four `329cc340` callers: `32a10c80` (`ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK.FrameMove`), `32a26480` (`ANSI_SA_ADE_REVERSE_STIGMA.FrameMove`), `32a26be0` x2 (`ANSI_A_AN_MIND_BREAK.FrameMove`). 0x4bc IsSkillUseCountExcludedState `329e65c0`: indices 0x123-0x126 / 0x12b = state ids 291-294 / 299, all `ANSI_AAT_CREATE_DYNAMO_FACTORY`. SetSkillLevelStateData term L289.'],
 'CH1p': ['Keys (`' + D + 'ch0_keys.tsv`, 222): the 4 June-only `INIT_COMPONENT` keys are InitComponent\'s (CH1h); `FORCE_DYNAMIC_CAMERA_INDEX` / `RETAIN_CAMERA_STATE` are Stage 9; `BBT_CHANGE_NOW_HP_PER_SECOND` / `BFT_TIME` (pepper-run tables) and the neither keys `DASHL_CANCEL_AFTER`, `HEAD_IK` (March reader commented), `SPEED_y`, `SOUND_SET3_RATE` / `SOUND_SET4_RATE` have no reader on either side: n/a. `POSSIBLE_NASOD_BARRIER` is CH1q2\'s. `CUSTOM_RENDER_STATE` is CH1 final\'s labelled stand-in. CH1p keeps 7ag and the hooks.'],
 'CH1q': ['`' + D + 'ch0_dp.tsv` (35 rows; 22 class-map functions): not split. Verdicts: UpNowDPValue outer `329e5b00` adds a spend factor `1 - protected float +0x6c50` (the plan said +0x6bb0) then `329e3060`; the body `329e56c0` equals March\'s buff-port R1 edit (`JUNE_BUFF_NEW_STAT`); ChangeMaxCharacterAddResource 0x324 is a new override (gain gauge max x rate); 0x6c8 / 0x6cc are Get / SetIsFormationMode; CanHyperMode 0x560 -> vslot 0x6d0 GetPossibleChangeFormationMode (virtual in June); 0x740 / 0x744 = StoreGageSnapshot / RestoreGageSnapshot (DP to `+0x92ac`, mutation to `+0x92b0`; base call per CH2a0). **Gauge:** `CX2AddGageData` vtable `3349b1e4` (22 slots, ctor `324989c0`); June keeps an Add gauge UI with no RTTI name: InitUI `32e6e0c0` (vtable `335335b0`) loads `DLG_Add_DP_And_Mutation.lua` (June\'s already in March) = March `CX2AddMyGageUI::InitUI` (`X2GageUI.cpp:4956`). The emulator has no DP code and needs none. `32d27770` (riding-pet wait-state getter: `+0x548` when byte `+0x5a0` is set, else `+0x4e4`; March reads `m_WaitState`, `X2GUUser.cpp:49277`) differs: a divergent row and the replacement here, or the March read kept with the reason.'],
 'CH1q2': ['`' + D + 'ch0_nasod.tsv` (10 rows). Corrections: slots 0x4c4 `329e7d60` / 0x4c8 `32a047a0` are **effect-set name hooks** (Phantom Seek `_Enhanced` with memo 29418; Supersonic, Reverse Circle, Void Field, Maximum Strike, Spear Dynamo, Pion Rusing, QuickSilver Strike variants), not barrier logic; they sit around each play in `32007570` with the `POSSIBLE_NASOD_BARRIER` flag. `PlayLoopSound` `329e2660` is June\'s already (`JUNE_SOUND_AUTO_RELEASE`, C9h-7a): confirm only. DieDamageEffectProc adds 29216 `RNSI_A_AN_PHASE_SHIFT_UPGRADE`. 0x4bc is CH1o\'s.'],
 'CH1': ['Enum gate today: `luabind.py check Add_NasodRuler` = **PASS** (0 methods unbound, 0 enum missing; `CAMERA_TYPE.CT_PARTS_LOOK_TRACKINGT` is nil in June too: June\'s typo, not a gap). Copy the held motions (`Motion_ADD_PT.X`, `Motion_ADD_LP.x`, `' + D + 'ch1_held_motions.tsv`) with the script. `CUSTOM_RENDER_STATE` x7: labelled stand-in (no reader in June class code either; june-engine R5).'],
 'CH5': ['`' + D + 'ch0_unittemplet_add.tsv`: June\'s 9 / 32 / 122 rows add only `m_fLobbyRotationY` (30 / 50 / 5) and, for 122, `m_HyperCutIn = "HQ_Cutin_ALP_Transcendence"`; the four new rows: 33 (`STR_ID_30383` / `30381`), 123 (`30386` / `30384`), 36 (`31923` / `31924`), 126 (`31925` / `31926`), every string present. **CHNa needs these rows** (live `UnitTemplet.lua` has none for 33 / 36 / 123 / 126). Cut-in switch `X2GUAdd.cpp:305-320` (9 / 32 -> set 0, default -> 122 set 1); lobby particles `X2UnitViewerUI.cpp:775` / `:2624` (ALP only).'],
 'CHNa': ['`' + D + 'ch0_classn.tsv` (48 rows; owners: Ara CHNa 4, Ara CHNb 3, Add CHNa 7, CHNb 3, CH5 3, others, 18 need nothing; Elesis\'s CHNa has not landed). Bounds: `UC_VALIDE_END` 123, `UC_END` 124 (cl /P): 123 passes `< UC_END`, 126 fails both, but after Ara\'s CHNa no refusing test remains. CHNa sites: `GetX2UnitClassCompatibility` (`X2Unit.h:2154-2192`, arms for 9 / 32 / 122 only; June `31ed6500`), the second / third line helpers `X2Unit.h:2826-2852` (assert-only), `CreateMyGageUI` (`X2GageManager.cpp:219-232`: the four must get `CX2AddGageData`, `CX2GUAdd` casts it), the stat chart (`X2StateCreateUnit.cpp:3892-3905`), `GetCharacterImageName` (`X2Data.cpp:2821`), `RotateYSelectCharacterType` (`X2Data.cpp:3835-3843`), `tools/createchar.py` (CLASSES / PARENT / PROMO). **No stat rows** (flat table). Needs CH5\'s UnitTemplet rows first. June\'s case lists `3248e290`, `3254ee40`, `31ee0de0` were not read in CH0.'],
 'CHNb': ['`' + D + 'ch0_classn_route.tsv`: Arc Tracer 35540-35543 (`CJT_JOB_SECOND`, from 9 at 15), Mastermind 35550-35554 (from 33 at 35), Time Tracer 35580-35583 (`CJT_JOB_THIRD`), Diabolic Esper 35590-35594 (from 36); level-35 chains are `CJT_NONE` and only the fifth quest grants the class. `Handlers_Quest.cpp:586-625` grants any class; auto-learn rows exist for all four. `X2OfflineClassChangeSeed.h` has no Add rows; `GetClassChangeDialogName` (`X2StateClassChange.cpp:304-380`) already has 33 / 36 / 123 / 126. `LevelUpEvent.lua`: level 15 needs only the dialog name; level 35 lacks Arc / Time Tracer and the guide; no level-70 row. March lacks `DLG_1ST_` / `2ND_Class_Change_Guide_Add`, `DLG_Class_Change_Add_AT` / `MM` / `TT` / `DE` (CH0b stages them).'],
 'CH2a0': ['`' + D + 'ch0_crest.tsv`. **Correction:** Crest of Time is cast by `RNSI_SA_COMMON_CREST_OF_TIME` 29213 / `_UPGRADE` 29231 ("Seal of Time", type 5, classes 36 / 126; type-5 compares at `32a28f4f` / `32a28f56` -> `32a28f90`), first in **CH2d**: CH2a0 runs before CH2d. The plan\'s "207 / 29212 / 29230" were a state id (207 = `ANSI_SA_APT_STASIS_FIELD_ACTIVE`), Space Crack and Countdown Bomb upgrade. Stand-ins to restore: Chung (`X2GUChung.cpp:38551-38578`), Elsword (`X2GUElsword_SwordMan.cpp:700-715`), Rena (`X2GULire_ElvenRanger.cpp:34468-34480`), Ara (`X2GUAra.cpp:13044-13059`), Elesis if her pair has landed, Add\'s pair from CH1q. Aisha\'s class already handles `UAI_CREST_OF_TIME_REVIVAL` 0x29 (`X2GUArme_VioletMage.cpp:33430`): check whether June handles it in the base.'],
 'CH2b0': ['`' + D + 'ch0_summon.tsv`: five live sites (one more, `329f350a` in `329f3070`, is dead code: no caller in the exe). Particle Prism `32a0c740` -> 1576 / 1577 (pairs with March\'s two `CreateNPCReq`, `X2GUAdd.cpp:6103` / `:6120`; class 9, **first in CH2a: CH2b0 runs before CH2a**), ADE Void Impact `329fe650` -> 5188, AMM Charged Impulsar `32a05470` -> 5189 or 5190 (formation mode and mutation > 0), Apocalypse `329f5200` -> 1604 (the plan\'s `329f5190` is Doom\'s Day\'s End). 5189 / 5190 lack `X2UnitManager.h` members. `luabind.py check`: `PlayEffectSet_CustomSet_LUA` unbound in both Charged Impulsar scripts (bind whole from June or row it); Apocalypse\'s two known rows (`GetEffectSetInstance`, `SetLocalRotateDegree`) are this phase\'s. Dying Apocalypse `329fb6c0` (L291) is here.'],
 'CH3c': ['CH0 split: CH3c = the special actives (46), CH3c2 = the actives `RNSI_A_*` (Mind Break, Force Finger, Reverse Field; 16). Void Impact\'s spawn (`329fe650`) is CH2b0\'s; the dead tables `ANSI_SA_ATT_VOID_IMPACT`, `ANSI_SA_ATT_STARDUST_SHOWER_*`, `ANSI_A_ADE_FORCE_NOVA`, `ANSI_SA_ADE_DIMENSION_BREAKER_END` have no registration.'],
 'CH3c2': ['CH0 split off CH3c: the 126 actives (`ANSI_A_AN_MIND_BREAK`, `ANSI_A_ADE_FORCE_FINGER`, `ANSI_A_ADE_REVERSE_FIELD*`; 16 functions). `ANSI_A_ADE_REVERSE_FIELD_END` is registered twice (ids 305 / 307, different callbacks): key by id.'],
 'CH3d': ['Hyper actives: 29600 / 29603 Doom\'s Day (122), 29601 / 29604 Apocalypse (123, spawn via CH2b0), 29602 / 29605 (126). Transcendence: `TranscendCheck` (`Handlers_Quest.cpp:1416-1451`) has no Add block; the transcend image prefix (`X2Data.cpp:3054-3091`) has no Add arm; Add\'s `+0x3c` readers `329e23a0`, `329e6f30` (L997).'],
 'CH4a': ['14 functions incl. InitPassiveSkillState `32a1ccb0` (0x690) and the 4 delegate adapters `32a2b340`, `32a309a0`, `32a309b0`, `32a309c0` (delegates `329d4190`, `32a04ce0`, `32a01d20`, `32a01dd0`); 22 passive skill ids with no state. `' + D + 'ch0_passive_ids.tsv`: 36 `SI_*` reads, 25 differ (9xxx -> 29xxx); spelling `SI_SA_APT_CONQUEROR` vs June `RNSI_SA_APT_CONQUERER`; `SI_SA_AHM_CHAIN_LIGHTNING` (an Aisha id) is read; `SI_A_AN_PULSE_BULLET`, `SI_HA_ALP_DOOMS_DAY`, `SI_P_AN_CHARGED_IMPULSAR` are not live in the `.i`. `+0x92a0` (L1068).'],
 'CH4b': ['0x73c `329d46b0` (L955); specialization B reader `32a01ca2` in `32a01b40` (L957; CH1q transcribes the function, CH4b checks the reader); memo consumers `31fdbbb0` (L332).'],
 'CH6': ['S3 baseline `' + D + 'ch0_s3_baseline.txt`: 261 tree rows (9: 15, 32 / 33 / 36: 29, 122 / 123 / 126: 53), 254 no_state, 7 has_state (id 900 per class). Rerun and compare.'],
 'Z': ['Plan-doc corrections CH0 found are in its Done note; carry the template paragraph.'],
}
COMMON_SKILL = ['Skill ids and their states: `awk -F\'\\t\' \'$6=="{p}"\' ' + D + 'ch0_state_skill.tsv` (part 2); the state functions: `awk -F\'\\t\' \'$7=="{p}"\' ' + D + 'ch0_class_map.tsv`.']


def section(p):
    fn = [r for r in cm if r[6] == p and r[1] not in SKIP_KIND]
    lo = sum(1 for r in fn if r[8] == 'n')
    ins = sum(1 for r in fn if r[4].startswith('inside'))
    new = sum(1 for r in fn if r[4].startswith('new'))
    st = [r for r in st_rows if r[7] == p]
    sk = [r for r in sk_rows if r[5] == p]
    chk = [r for r in cl if r[4] == p or (p.startswith(('CH2', 'CH3')) and r[4].startswith('CH2a-CH3'))]
    out = [HEAD, '']
    if fn or st or sk:
        out.append('- Class map: **%d** functions (%d listing-only; %d `inside`, %d `new`), filter `awk -F\'\\t\' \'$7=="%s"\' %sch0_class_map.tsv` '
                   '(col 7 phase, col 5 inside / new, col 6 proposed name, col 9 carved y / n).' % (len(fn), lo, ins, new, p, D))
        if st:
            out.append('- Script states: %d (`%sch0_state_skill.tsv` part 1, col 8).' % (len(st), D))
        if sk:
            out.append('- Skill ids: %d: %s.' % (len(sk), ', '.join('%s %s' % (r[0], r[1]) for r in sk)))
    if chk:
        out.append('- Checklist rows (`%sch0_checklist_rows.tsv` col 5): %s.' % (D, ', '.join('L%s' % r[1].lstrip('L') for r in chk)))
    for n in NOTES.get(p, []):
        out.append('- ' + n)
    if p.startswith(('CH2a', 'CH2b', 'CH2c', 'CH2d', 'CH3a', 'CH3b', 'CH3c')) and p not in ('CH2a0', 'CH2b0'):
        out.append('- ' + COMMON_SKILL[0].format(p=p))
    return '\n'.join(out) + '\n'


def main():
    dry = '--dry' in sys.argv
    for p in PHASES:
        f = os.path.join(K, p + '.md')
        if not os.path.exists(f):
            print('MISSING', f)
            continue
        s = open(f, encoding='utf-8').read()
        head, sep, rest = s.partition('\n## Amended by CH0')
        m = re.search(r'\n## ', rest)                 # keep any later section (e.g. "Amended by char-ara CHNa")
        tail = rest[m.start():] if m else ''
        new = head.rstrip('\n') + '\n\n' + section(p) + tail
        if dry:
            print('== %s\n%s' % (p, section(p)))
        else:
            open(f, 'w', encoding='utf-8', newline='').write(new)
    print('done', len(PHASES))


if __name__ == '__main__':
    main()
