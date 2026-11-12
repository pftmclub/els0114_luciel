r"""char-add CH0 (helper session): build ch0_checklist_rows.tsv (Add's rows of JUNE2017_CHARACTER_PORT_CHECKLIST.md -> proposed phase / n/a).
    python june2017_re/char_add/ch0_build_checklist_rows.py
Modelled on char_elesis/ch0_build_checklist_rows.py, but without the CH0 class map / state table (not built by this helper): the evidence column
is the plan digest (plan_checklist_rows.md) plus what this helper re-read on the March side. The Opus CH0 session (2026-11-13) reviewed every phase: the proposals stand except the rows marked 'Opus CH0' in the note.
Columns: section, checklist_line, row, add_evidence, phase, note. checklist_line is the line in the CURRENT checklist (anchor search, `L<n>`);
the plan's own L numbers are from the 1055-line checklist of 2026-11-11, the live file now has 1080 lines (so every plan L moved by 0..+15).
Every plan Part A row (56) and every Part B row that is not also a Part A row (31) is covered by a key; the 12 Part B rows that are also Part A
(L258, 262, 265, 278, 280, 283, 286, 423, 427, 444, 917-932, 1051) are the Part A rows of the same L.
"""
import csv, re, os, collections
HERE = os.path.dirname(os.path.abspath(__file__)) + '/'
T = os.path.normpath(HERE + '../../') + '/'
CL = open(T + 'JUNE2017_CHARACTER_PORT_CHECKLIST.md', encoding='utf-8').read().split('\n')


def AL(hint, anchor):
    """current line of the first line >= hint - 10 holding `anchor`; whole-file fallback; stop when absent."""
    for k in range(max(1, hint - 10), len(CL) + 1):
        if anchor in CL[k - 1]:
            return k
    for k in range(1, len(CL) + 1):
        if anchor in CL[k - 1]:
            return k
    raise SystemExit('anchor not found (plan L%d): %r' % (hint, anchor))


# ---- plan rows (Part A and Part B) -> text by plan L
PL = open(HERE + 'plan_checklist_rows.md', encoding='utf-8').read().split('\n')
planA, planB = collections.OrderedDict(), collections.OrderedDict()
part = None
for l in PL:
    if l.startswith('## Part A'):
        part = 'A'
    elif l.startswith('## Part B'):
        part = 'B'
    elif l.startswith('## Part C'):
        part = None
    elif part and l.startswith('|') and not l.startswith('|---') and not l.startswith('| Sec') and not l.startswith('| Bucket'):
        c = [x.strip() for x in l.strip().strip('|').split('|')]
        if len(c) >= 3 and re.match(r'^\d', c[1]):
            key = int(re.match(r'\d+', c[1]).group(0))
            (planA if part == 'A' else planB)[key] = c
assert len(planA) == 56, len(planA)
B_NEW = [k for k, c in planB.items() if c[-1].strip() == '-']
assert len(B_NEW) == 31, len(B_NEW)

R = []
used = set()


def row(part, plan_l, anchor, evidence, phase, note, sec=None):
    src = planA if part == 'A' else planB
    c = src[plan_l]
    used.add((part, plan_l))
    text = c[2] if part == 'A' else c[2]
    text = re.sub(r'\s+', ' ', text)
    if len(text) > 330:
        text = text[:327] + '...'
    R.append([sec or c[0], 'L%d' % AL(plan_l, anchor), text, evidence, phase, note + ' [plan %s:%d]' % (part, plan_l)])


# ================= Part A (56)
row('A', 5, 'Ara, Elesis,', 'scope list only', 'n/a', 'scope')
row('A', 116, '`Ain_Angel`, `Rose_Mechanic`', "grep APPLIED_SKILL_ID in Add_NasodRuler.src.lua = 0; plan_charport_keys: Add has none", 'n/a', 'Add\'s script carries no APPLIED_SKILL_ID')
row('A', 166, '**Add** ×7', 'charport_scan ENUM: Add_NasodRuler {CUSTOM_RENDER_STATE: 7}; June script has 14 CUSTOM_RENDER_STATE tokens; today `luabind.py check Add_NasodRuler` (ENUM + STATE) prints RESULT: PASS (0 tables / 0 members missing); March has its own CUSTOM_RENDER_STATE in X2GUAdd.h:485 (4 members, other values)', 'CH1 final', 'enum gate re-run at CH1 final; the March enum stays separate (june-engine R5); the reader of the key is the 7w stand-in (CH1p)')
row('A', 239, '329f538a', 'inside the vslot-16 (0x40) range; X2GUAdd.cpp has no GetHyperModeCountByBuff use', 'CH3d', 'Opus CH0: 329f538a lies in 329f5200 = ANSI_HA_AMM_APOCALYPSE.FrameMove (hyper active 29601 / 29604), not an override')
row('A', 250, '329e5b00', 'UpNowDPValue X2GUAdd.cpp:9100 (the plan\'s checklist lines :9474 / :9086 are stale); the buff port\'s R1 edit is the JUNE_BUFF_NEW_STAT block :9137-9145', 'CH1q', 'DP rework whole')
row('A', 251, '32a2bbc0', 'InitPassiveSkillState X2GUAdd.cpp:385; BBT 92 CHANGE_MAXIMIZE deferred (buff Phase 5c)', 'CH4a', 'passive hook; routed if 5c has not landed')
row('A', 253, '32a07dc0', 'March inline cooldown X2GUAdd.cpp:2683 and RM1 hook :2686-2690 in StateChangeSpecialAttack :2647; class has no ReservedSkillStart', 'CH1k', 'ApplySkillCoolTimeRateByBuff call; Ara pattern SpecialAttackCooldown (_CORE)')
row('A', 257, '329e31e0', 'no ChangeMaxCharacterAddResource in X2GUAdd.cpp; the DP gauge max is CX2AddGageData::m_fMaxDPValue = MAX_DP_GAGE_VALUE 8000 (X2Define.h:88)', 'CH1q', 'declaration CH1h2 (0x324 / 0x2a4), body CH1q')
row('A', 258, '329e2f80', 'no 0x740 / 0x744 override in March\'s class (grep: 0); base engine routed here by char-chung', 'CH2a0', 'CH1i declares 0x740 as a stand-in; CH2a0 ports the shared engine, then Add\'s override calls the base')
row('A', 260, '329e2340', 'X2GUAdd.cpp:1342 and :2962 set nowAction (UAI_*) in CommonEventProcess / SpecialAttackNoStageChange; no TryHyperModeStateChange hook in the class', 'CH1q', 'hook + UAI_RELEASE_FORMATION_MODE write')
row('A', 261, '32a31550', 'no CanHyperMode / ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE in X2GUAdd.cpp (grep 0)', 'CH1q', 'awakening (formation mode) entry')
row('A', 262, '32a20e70', 'no slot-510 lookup override in March\'s class; ReservedSkillStart absent', 'CH1k', 'quick-slot wait prefix as Ara CH1k')
row('A', 265, '32a211cc', 'same slot-510 lookup', 'CH1k', 'riding-key wait as Ara CH1k')
row('A', 266, '329e65c0', 'no IsSkillUseCountExcludedState in the class (grep 0)', 'CH1o', 'state indices 0x123-0x126 / 0x12b mapped to ANSI_* names (plan CH1o)')
row('A', 278, '329e59f0', 'DP consume reader sits in UpNowDPValue; spec B reader 32a01b40 / 32a01ca2', 'CH1q', 'spec A with DP (CH1q); spec B reader is CH4b (row L945)')
row('A', 279, 'ConvertAndAdd', 'the 0.5 is X2SocketItem.h:1486 `fReformValue = ( UT_ADD == eUnitType_ ) ? 0.5f : 1.f` (not in X2GUAdd.cpp); the JUNE_SOCKET_PARITY flag already dropped it for Add\'s socket charge', 'CH1q', 'check Add\'s hyper charge code for a June 0.5 (plan item 7)')
row('A', 280, '32a2dec0', 'Add range summon case; NPC 1604 Apocalypse; March has two CreateNPCReq (X2GUAdd.cpp:6103 / :6120)', 'CH2b0', 'summon follow-ups of 3230c1a0')
row('A', 283, '329fdbe0', 'X2GUAdd.cpp has no 3201ad10 caller today; the call is June-side in Add range', 'CH1j', 'Opus CH0: 329fdbe0 is a CommonFrameMove helper (class map CH1j); the call is transcribed with it')
row('A', 285, '329e2410', 'ApplyHyperModeBuff X2GUAdd.cpp:9532; March reads BFI_BUFF_LIBRARY_OF_LIMITLESS_MANA_RECOVERY at :9538; CX2Unit +0x3c = transcendence flag', 'CH1q', 'port whole (BFI 846 / 315)')
row('A', 286, 'X2GUAdd.cpp:2455', 'SetSkillLevelStateData X2GUAdd.cpp:2458 (call site :2455 in SetEquippedSkillLevelStateData :2452)', 'CH1o', 'the ( 3201ad10( skill ) + 1.0 ) term, as Ara CH1o')
row('A', 287, 'SI_HA_APOCALYPSE', 'NPC script binder SetMultiplyPowerRate_LUA already ported by char-eve CH2b3; Apocalypse NPC script needs GetEffectSetInstance / SetLocalRotateDegree bound', 'CH2b0', 'luabind.py check on the NPC script; the hyper active itself is CH3b / CH3d')
row('A', 288, '329fb6c0', 'Mastermind member +0x98e4 absent from X2GUAdd.h; Eve CH2b4 wrote the erase + UpdateSkillSlotUI inline in X2GUUser.cpp:27518-27530 (ResetOnSummonEnd UC_ADD_MASTERMIND arm)', 'CH2b0', 'Opus CH0: the summon map owns the dying Apocalypse (ch0_summon.tsv): +0x98e4 store with the spawn')
row('A', 289, '32a080fe', 'DamageReact X2GUAdd.cpp:2810; no ResponseEmergency member', 'CH1h2', 'DamageReact 0x60 (32a07fc0)')
row('A', 306, '329f3980', 'effect-modifier tag sites: Stardust Shower, Reverse Circle / Void Field, Moonlight Breaker, Flick Disc, Quake Buster Hyper, Space Crack, Maximum Strike, Conqueror, Pulse Cannon', 'CH2a-CH3c', 'one tag call per skill, in the skill phase that ports the state')
row('A', 311, '321e3830', 'from 32a0e5f0, 32a04a2f, 32a04b03, 32a06d06', 'CH2a-CH3c', 'per skill (checklist 7a)')
row('A', 356, '329f4350', '12 sites (329f4350, 329f9ff0, 32a03b70, 32a27360; four via 32296f30)', 'CH2a-CH3c', 'one alpha-tag call per site')
row('A', 358, '322726f0', 'CLASS COLUMN IS WRONG: the row says Rena, but RNSI_SA_ALP_SUPERSONIC 29058 is Add\'s Lunatic Psyker skill: Enum.src.lua:15170 (29xxx = Add), the only script with a state for it is Add_NasodRuler.src.lua (ANSI_SA_ALP_SUPERSONIC :17887), DamageEffect_Character.src.lua:8678 / :8755 apply it, plan_numerical_rows.tsv lists DamageEffect_SA_ALP_SupersonicFX / _Final for 29058. The row\'s own note says "the skill is Rena\'s fourth line, classes 156-158": there is no such Rena skill; june-stat-calc D2b ported the gate as CX2GameUnit::CanApplyHpRateDamage (always true offline)', 'CH3a', 'correct the Class column to Add (Lunatic Psyker 122); the gate is already ported whole; CH3a plays Supersonic')
row('A', 380, '329f63a0', 'SetNasodArmorAttackTarget X2GUAdd.cpp:9863 (checklist\'s :9843 stale); June front gate already in under JUNE_DAMAGE_MATH :9865-9892; pass-in of SI_A_AN_PULSE_BULLET / SI_A_AN_PHASE_SHIFT :9931-9933', 'CH1q2', 'whole body (four reform ids 29201 / 29202 / 29216 / 29217)')
row('A', 382, '@329f64c1', 'rest of the function (:9893-) is March\'s body', 'CH1q2', 'read 329f63a0 end to end')
row('A', 372, 'ALP end windows', 'unit viewer: nothing to port (checklist says so)', 'n/a', 'nothing to port')
row('A', 412, 'SAT_ADD_DP_PERCENT_UP', 'item 100001054 (1 item); the ability adds to the DP gain; March DP code at X2GUAdd.cpp:9100-', 'CH1q', 'DP map')
row('A', 423, 'EFFECT_SET_LIST_EX_TRANS', "Add_NasodRuler.src.lua uses EFFECT_SET_LIST_EX x122, EFFECT_SET_LIST_FINAL x3 (as the row says; no _FINAL_ULTIMATE, no _TRANS_1 / _TRANS_2 keys); the loader is Eve CH1r's", 'CH1 final', 'script adoption; no class code')
row('A', 424, 'POSSIBLE_NASOD_BARRIER', 'script has 38 POSSIBLE_NASOD_BARRIER lines; March reader only CommonStateStartFuture X2GUAdd.cpp:1227-1240 (POSSIBLE_NASOD_BARRIER_BY_UNIT_CLASS, June script lists UC_ADD_LUNATIC_PSYKER x4)', 'CH1q2', 'user slots 0x4c4 / 0x4c8 + base virtuals')
row('A', 425, 'CUSTOM_STATE_DELETE_SOUND', 'script has SOUND_PLAY_EX x1, STATE_SOUND_PLAY x1, CUSTOM_STATE_DELETE_SOUND x0', 'CH1 final', 'no code: works once the script is adopted')
row('A', 427, '329d7220', 'USEFUL_VALUE x3 in the script; readers in Add range (Future 329d7220, 32a02f10)', 'CH1e-CH1g', 'state functions that read them (JuneGetUsefulValueFuture / Now)')
row('A', 444, '32a28e00', 'SetEquippedSkillFuncBySkillSlotIndex X2GUAdd.cpp:612 (slot + SlotB model already)', 'CH1n', 'drop the snapshot call; state byte +0x11 store dropped')
row('A', 470, '32a080cc', 'DamageReact X2GUAdd.cpp:2810', 'CH1h2', 'vslot 0x81c( 0, DD ) super-armour call at June\'s place')
row('A', 475, '329d4640', 'no IsPacketBuffRefused in the class; BTI 342 / 438 not checked here', 'CH1q2', 'plan CH1q2 registers BTI 342 / 438 if missing')
row('A', 478, '329f73a0', 'DieDamageEffectProc X2GUAdd.cpp:10412 checks SI_A_AN_PHASE_SHIFT only (:10416)', 'CH1q2', 'both Phase Shift ids (29201 / 29216)')
row('A', 511, 'ANSI_APT_DASH_JUMP_COMBO_ZX', 'state at Add_NasodRuler.src.lua:9470', 'CH1 final', 'script adoption (DAMAGE_BOUNDING_DOWN data)')
row('A', 616, 'SOUND_SET3_RATE', 'script has SOUND_SET3_RATE x1', 'CH1 final', 'script adoption; engine done')
row('A', 664, 'Add_NasodRuler` (`CRS_INVISIBLE`', 'script has 14 CUSTOM_RENDER_STATE tokens (CRS_INVISIBLE x6, CRS_UNIT_FADE_WHITE_WITHOUT_WEAPON x1 per the row); reader = june-engine R5', 'CH1 final', 'Opus CH0: nothing reads the key in June class code either; CH1 final keeps the enum row (gate PASS) and labels the stand-in; labelled stand-in for the reader (plan 7w); data arrives at CH1 final')
row('A', 680, '32a176c0', 'ANSI_A_AN_PHASE_SHIFT_BLINK_StateStart X2GUAdd.cpp:8197-8236 sets the return position at :8228-8229 with no land call (the checklist\'s :8224 is a commented line)', 'CH2a', 'Nasod Ruler (9) skill Phase Shift Blink: GetLandPosition( ..., true )')
row('A', 703, 'non-Add', 'engine R14c (UpdateEquipCollisionData pair exists for every class)', 'n/a', 'engine piece, ready elsewhere')
row('A', 722, 'Elesis and Add leave it blank', 'RNSI_* enumerators + states per class', 'CH2a-CH3c', 'enumerators and state binding with each skill phase')
row('A', 760, 'APT_NA_Ready', 'motion renames: APT_NA_Ready / APT_NA_preAttack; 4 June-only job files + 7 same-name files; target kom data033 (March has no data207.kom)', 'CH0b', 'stage; CH1 final deploys together (held motions)')
row('A', 781, '32021390', 'landing prediction 32021390 / line clamp 32294530 (Add callers)', 'CH2a-CH3c', 'per skill that calls them (CanDown call shape)')
row('A', 877, '328e0aa0', 'tree ctor 328e76a0 zeroes +0x138 / +0x139; install / G-Core marks 328e0290 for class 9 and 0xb', 'CH5', 'tree-UI icon blocks')
row('A', 896, 'Unit type 9', 'skill-page decide ACK refreshes install / G-Core marks (328e0290)', 'CH5', 'same marks as L877')
row('A', 924, 'A `32a04ce0`', 'registering function 32a1ccb0; pushes A 32a04ce0, A 329d4190, A 32a01d20, D 32a01dd0; March pushes are in X2GUAdd.cpp InitPassiveSkillState :385', 'CH4a', 'replace March\'s pushes with June\'s set')
row('A', 943, '329d46b0', 'no 0x73c override in March\'s class', 'CH4b', 'override whole (declaration CH1i)')
row('A', 945, '32a01ca2', 'GetCharacterSpecializationB reader', 'CH4b', 'spec B reader whole')
row('A', 968, 'Add / Ain', 'hyper StateStart 32a08150 counts +0x9330 before 31fc8c20; engine ported by T5 ("Add / Ain, not ported")', 'CH1q', 'the hyper StateStart (formation mode) is in the DP / formation phase')
row('A', 982, '329e23a0', '13 June readers of CX2Unit +0x3c in Add range (329e23a0 BFI 846 vs 315, 329e6f30 / 329e6f80 skill-id map gates, ...)', 'CH3d', 'each branch whole with the transcendence rows (329e23a0 is also row L285)')
row('A', 1009, 'GetX2UnitClassCompatibility', 'X2Unit.h:1456-2200: Add arms only for 9 / 32 / 122 (:2154-2192); 33 / 36 / 123 / 126 fall to default return false; the checklist row does not name Add (the plan\'s "l.1009" is now L1024-1026)', 'CHNa', 'cases 0x21 / 0x24 / 0x7b / 0x7e from June 31ed6500 under his flag')
row('A', 1053, '+0x92a0', 'user float +0x92a0 absent from March (ctor 3203e850, reset 31fe19d0, added by Add 32a1ccb0); vslot 0x824 GetStateAnimSpeedRate', 'CH4a', 'passives add it; vslot 0x824 override with it')

# ================= Part B rows not also in Part A (31)
row('B', 117, '| `NUMERICAL_DATA_INDEX` |', 'plan_numerical_rows.tsv: 140 c1a rows for 55 skill ids; charport_scan NUMERIC prints no line for Add_NasodRuler', 'CH2a-CH3c', 'numbers per skill with its phase')
row('B', 162, 'From `charport_scan.py`, section `ENUM`', 'Add_NasodRuler {CUSTOM_RENDER_STATE: 7}; luabind.py check Add_NasodRuler: ENUM 0 missing, RESULT PASS', 'CH1 final', 'enum gate re-run before the script adoption')
row('B', 178, 'Missing members of tables March *does* have', 'RNSI_* SKILL_ID names (June 123 distinct ids; the March enum lacks the June names)', 'CH2a-CH3c', 'names registered with each skill phase')
row('B', 184, '**Every non-DamageData key in a character script.**', 'luabind: june-only 8 keys (4 INIT_COMPONENT: DP_RATE_BY_FORMATION_MODE_USE_MP_3RD_LINE_SKILL, DP_USE_RATE_WITH_FORMATION, VOID_IMPACT_DOWN_ANGLE, VOID_IMPACT_UP_ANGLE; FORCE_DYNAMIC_CAMERA_INDEX x15 and RETAIN_CAMERA_STATE are Stage 9\'s), neither 4, march-only-site 4', 'CH1p', 'plan CH0 item 3 gives each key an owner; INIT_COMPONENT keys with CH1h (DP reads under _DP)')
row('B', 214, '**Standing check for every class port', 'X2GUAdd.cpp has no ReservedSkillStart (plan: DP cost goes in it)', 'CH1k', 'ReservedSkillStart override')
row('B', 216, '**Pilot close-out (2026-10-22).**', 'convention text: marks are written in place by each phase', 'n/a', 'convention; no code')
row('B', 255, '`31fc8200` (`IsSummonSkill`', 'Add summons: NPC 1604 Apocalypse + two particle prism NPCs (CreateNPCReq :6103 / :6120)', 'CH2b0', 'IsSummonSkill ids and the 3230c1a0 cases')
row('B', 256, 'Per-class code that writes the state power rate after `DoStateEndStart`', 'grep +0x7cc8-style writers in X2GUAdd.cpp: not checked line by line', 'CH1j', 'CH0 confirms whether Add writes it (Common* overrides)')
row('B', 263, "Every class's June skill-use check", 'class has no JuneIsDevUnlimitedSkillUse gate (plan Rules)', 'CH1k', 'wrap June\'s cool-time / MP check (standing rule)')
row('B', 329, '`31fdbbb0` / `31fdbcc0` readers in class content', 'memo condition class and readers for Add not listed', 'CH4b', 'plan CH4b: memo readers')
row('B', 330, 'the per-class blocks in funcmap rows', 'plan: Add not listed; funcmap row 198: June SetDamageExceptionProcess 329e4a50 is IDENTICAL to March X2GUAdd.cpp ProcessBodyOfTranscendence (inlined; March SetDamageExceptionProcess :8604-8610 calls it :10812)', 'n/a', 'identical to March')
row('B', 428, '`USI_WAIT_STATE_CHANGE` (June base id', 'registration in InitStateCommon X2GUAdd.cpp:1549-', 'CH1a', 'wait-state-change with the wait group (plan CH1a)')
row('B', 439, 'Class code multiplies `m_fN', 'SetSkillLevelStateData :2458 / SetEquippedSkillLevelStateData :2452', 'CH1o', 'the ( 3201ad10 + 1.0 ) term and +0x7cc8 multiply (row L286)')
row('B', 440, 'calls `JuneSnapshotBeforeSlotRebind` first', 'SetEquippedSkillFuncBySkillSlotIndex :612-', 'CH1n', 'same function as row L444')
row('B', 472, '| Bounding user states |', 'USI_BOUNDING_* registered in every June class InitState', 'CH1m', 'registration')
row('B', 509, '`<prefix>DAMAGE_BOUNDING_DOWN`', 'script has DAMAGE_BOUNDING_DOWN x1 (ANSI_APT_DASH_JUMP_COMBO_ZX -4000 per the table)', 'CH1 final', 'script adoption (data)')
row('B', 548, '`TIME_STOP_IMMUNE_STATE = TRUE` in 76 June states', 'script has TIME_STOP_IMMUNE_STATE x3', 'CH1 final', 'script adoption; engine side done')
row('B', 592, 'every June class script sets the key in its awakening states', 'script has SHOW_HYPER_CUT_IN_TIME x3 (:4509, :4580, :4653)', 'CH3d', 'plan CH3d: the SHOW_HYPER_CUT_IN_TIME states (7t)')
row('B', 595, 'm_HyperCutIn` per class (39 rows)', 'live UnitTemplet.lua rows for 9 / 32 / 122 only (:1482 / :1508 / :1534); June rows for all seven', 'CH5', 'UnitTemplet rows (plan CH5)')
row('B', 618, '**Sound sets (from C3t-2', 'overrides JuneIsBerserkByChance / JuneIsHyperModeState: whether Add\'s June vtable overrides them is not read here (check ch0_vtable)', 'CH1o', 'only if the vtable shows an override (unconfirmed)')
row('B', 624, '**Each class port replaces its table with June', 'X2GUAdd.cpp:303-320 InitState cut-in switch (set 0 for 9 / 32, set 1 for 122 and the default); no s_SkillCutInSet table in X2GUAdd.cpp', 'CH5', 'cut-in table and the index switch (plan CH5)')
row('B', 667, '### 7x. Combo hits must carry', 'script has AT_COMMAND x4 tokens in total (not the per-combo tag count)', 'CH1d', 'ground combos (CH1d) and dash combos (CH1d2): check the tag counts in the transcription')
row('B', 717, 'Per family (rows / no state', 'tree rows with no class state for the Add family', 'CH2a-CH3c', 'bind states per skill phase')
row('B', 729, 'charges them before the state change', 'March SpecialAttackEventProcess X2GUAdd.cpp:1360 charges MP / cool time before StateChange', 'CH1k', 'SpecialAttackEventProcess; bind states so the blank-state path is gone')
row('B', 738, 'ResetLeftSkillCoolTimeBySkillType( ST_SPECIAL_ACTIVE )', 'row says June\'s only callers are Ain\'s; no Add caller', 'n/a', 'no Add caller')
row('B', 783, 'Chung Artillery Strike Ready', 'CanDown call shape; Add landing helpers are Part A row L781', 'CH2a-CH3c', 'per skill with the Part A row')
row('B', 811, "June's combined index is A 0-4, B 5-9", 'X2GUAdd.cpp:2702-2717 decodes m_iPressedSkillSlotIndex ( > 3 / -4 ) in StateChangeSpecialAttack; the class\' X2GUUser "Elesis, Add not ported" set', 'CH1p', '7ag pressed-index space (decode, CountUsedSkill, readers together)')
row('B', 857, "A class port passes June's arguments at every call it ports", 'BuffFactor-by-BFI call sites in Add skill states', 'CH2a-CH3c', 'per ported call site')
row('B', 942, '| Item | Classes (June override) | What the class port brings |', 'vslot 0x73c Add 329d46b0 (Part A L943); spec B 32a01ca2 (L945)', 'CH4b', 'per-class passive hooks')
row('B', 957, 'Every class also brings:', 'transcend allow-list ids (Handlers_Quest.cpp TranscendCheck :1416-1451), LevelUpEvent TYPE row (LevelUpEvent.src.lua:604-621), DLG_Transcendental_Change_Guide_Add.lua, JuneGetHyperModeTableName cases (122 / 123 / 126 present, X2GUUser.cpp:34364-34368), hyper cut-in names, SPRT_HYPER_CLASS passives', 'CH3d', 'with the transcendence skill phase')
row('B', 991, 'A class port overrides the ones its June vtable overrides', 'Add is not named in 0x3ac / 0x3b0 / 0x56c / 0x584 / 0x65c / 0x6a8 (row lists Ara, Chung, Lire, Ain, Rose, Lu/Ciel); whether Add\'s vtable overrides 0x3ac / 0x6a8 is not read here', 'CH4b', 'only if the vtable shows an override (plan CH4b)')

# ---- coverage
missA = [k for k in planA if ('A', k) not in used]
missB = [k for k in B_NEW if ('B', k) not in used]
extraB = [k for (p, k) in used if p == 'B' and k not in B_NEW]
if missA or missB:
    print('PLAN ROWS NOT COVERED: A', missA, 'B', missB)
if extraB:
    print('B rows used that are also Part A:', extraB)
cnt = collections.Counter(r[4] for r in R)
n_na = cnt['n/a']
with open(HERE + 'ch0_checklist_rows.tsv', 'w', encoding='utf-8', newline='') as f:
    w = csv.writer(f, delimiter='\t', lineterminator='\n')
    w.writerow(['section', 'checklist_line', 'row', 'add_evidence', 'phase', 'note'])
    for r in R:
        w.writerow([c.replace('\t', ' ').replace('\n', ' ') for c in r])
    f.write('# totals: %d rows = %d with a phase + %d n/a; plan Part A %d, Part B not in A %d, uncovered A %s B %s; phases reviewed by the Opus CH0 session (2026-11-13; changed rows say Opus CH0)\n' % (len(R), len(R) - n_na, n_na, len(planA), len(B_NEW), missA or 'none', missB or 'none'))
print('rows', len(R), 'n/a', n_na, 'uncovered', missA, missB)
for k, v in sorted(cnt.items(), key=lambda x: -x[1]):
    print('  %-12s %d' % (k, v))
