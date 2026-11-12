r"""char-add CH0 (helper session): write ch0_classn.tsv, the client + emulator sites that test a unit class against UC_END / UC_VALIDE_END
or switch on the unit class with Add arms (9 / 32 / 122) and no arm for 33 / 36 / 123 / 126 (read-only survey 2026-11-13).
    python june2017_re/char_add/ch0_build_classn_sites.py

Every row was read from the file at the quoted lines (X2Lib/, tools/, the live kom_contents Lua, june_src_fixed). Compiler facts: UC_VALIDE_END 123,
UC_END 124, UT_ADD 9, UT_END 10 (/Zs probe, see ch0_march_side.md section 0). `owner` values: covered by Ara CHNa / CHNb (flag
SERV_IRUHADEV_JUNE_CLASS_PATH / _CLASS_CHANGE_GUIDE), none, or the Add phase that edits the site. Elesis's CHNa has not landed
(SERV_IRUHADEV_JUNE_CHAR_ELESIS_CLASS is OFF), so no row is covered by it. June case values: 33 = 0x21, 36 = 0x24, 123 = 0x7b, 126 = 0x7e.
"""
import csv, os
HERE = os.path.dirname(os.path.abspath(__file__)) + '/'
H = ['site', 'function', 'test', 'effect_on_33_36_123_126_today', 'junes_behaviour', 'class_generic', 'proposed_phase', 'evidence', 'owner']
R = []


def add(*a):
    assert len(a) == 8, a[0]
    R.append(list(a))  # the owner column is filled from OWN below, in row order


# ---- covered by Ara's class-generic work (flag SERV_IRUHADEV_JUNE_CLASS_PATH / _CLASS_CHANGE_GUIDE)
add('X2Lib/X2SocketItem.cpp:2897-2902', 'CX2SocketItem::AddSkillLevelUpSocketGroup_LUA',
    'eUnitClass_ <= UC_NONE || > UC_AIN_ANGEL (202) under SERV_IRUHADEV_JUNE_CLASS_PATH; March test >= UC_END (124) in the #else',
    '33 / 36 / 123 / 126 ACCEPTED (bound 202). The March test refused only 126 (> 124); 123 passed (< 124)',
    'June 3271a810: (class - 1) < 0xca, class 1..202', 'yes', 'covered by Ara CHNa (flag SERV_IRUHADEV_JUNE_CLASS_PATH)',
    'code read; Always.h:12479 flag; X2Lib/X2SocketItem.cpp:2897-2902')
add('X2Lib/X2Unit.cpp:3021-3029', 'CX2Unit::GetClassLevel',
    'June body under SERV_IRUHADEV_JUNE_CLASS_PATH: (class - 10) < 190 -> ( class > 99 ) ? 2 : 1, else 0 (March switch in the #else)',
    '33 -> 1, 36 -> 1, 123 -> 2, 126 -> 2 (9 -> 0)', 'June 3234c170', 'yes', 'covered by Ara CHNa (X2Unit.cpp:3023-3028)', 'code read')
add('kom_contents/data036_kom/CorrelateUnitClassTemplet.lua (loader X2UnitManager.cpp:1445-1499; reader X2SkillNoteManager.cpp:113)',
    'CX2UnitManager::GetLowRankUnitClass / AddCorrelate', 'class -> parent chain read from the Lua table; no bound test',
    'the live file names UC_ADD_ARC_TRACER, UC_ADD_TIME_TRACER, UC_ADD_MASTERMIND, UC_ADD_DIABOLIC_ESPER (byte scan: 1 hit each)',
    'June table', 'yes', 'covered by Ara CHNa (data036 file is June\'s)', 'byte scan of the live file')
add('X2Lib/X2Unit.h:2912-2975 (GetUnitClassByTierAndLine :2914-2935, Add row :2927; GetUnitClassLineByUnitClass :2937-2967, 33 :2950, 36 :2957, 123 :2953, 126 :2960; GetUnitClassTierByUnitClass :2969-2974)',
    'June 3234b390 / 31eca340 / 31ef05e0 (class-change guide tables)',
    '[type][tier][line] table and line / tier arithmetic under SERV_IRUHADEV_JUNE_CLASS_CHANGE_GUIDE; the Add row is { 9, 9, 9, 9 } / { 32, 33, 36 } / { 122, 123, 126 }',
    '33 / 36 / 123 / 126 present: line 2 / 3 / 2 / 3, tier 1 / 1 / 2 / 2', 'June tables', 'yes',
    'covered by Ara CHNb (flag SERV_IRUHADEV_JUNE_CLASS_CHANGE_GUIDE; the Add row came with it)',
    'code read; the Add row doubles as June\'s three-line layout for Add\'s helpers (row below)')
add('X2Lib/X2CashShop.cpp:2639-2775 (map _CONST_CASH_SHOP_CHNB_: 32 -> 272810 :2674, 33 -> 273862 :2675, 36 -> 276353 :2676, 122 -> 273232 :2707, 123 -> 273860 :2708, 126 -> 276354 :2709; OpenClassChangeGuideItem :2729) and :13901-13966 (GetUnitClassChangeItemID: 32 :13927, 33 :13928, 36 :13931, 122 :13956, 123 :13957, 126 :13960)',
    'CX2CashShop::OpenClassChangeGuideItem / GetUnitClassChangeItemID', 'class -> item id maps (June 3379fcf8 / 32230020)',
    'all four present; every id checked against item.kim (guide items 272810 / 273862 / 276353 are unit_class 9 level 15, 273232 / 273860 / 276354 level 35; tickets 273863 / 273864 / 276302 / 273233 / 273861 / 276303)',
    'June maps', 'yes', 'covered by Ara CHNb (X2CashShop.cpp:2639, flag SERV_IRUHADEV_JUNE_CLASS_CHANGE_GUIDE; SKILL_CLASS_ITEM_CHOICE)', 'code read + python tools/kim.py find --ids')
add('X2Lib/X2LVUpEventMgr.cpp:469-858 (UICustomEventProc, `GuideAcceptClassUpQuest( QT_CHANGE_JOB, CJT_JOB_FIRST / SECOND / THIRD )` :567 / :573 / :579), :858-1022 (InitClassChangeGuide); X2Lib/X2QuestManager.cpp:4082-4119 (GuideAcceptClassUpQuest); X2SkillNoteManager.cpp:374-459',
    'level-up class-change guide runtime', 'class-generic: the three Accept buttons pick the quest by CHANGE_JOB_TYPE FIRST / SECOND / THIRD, the 2nd guide fills from the next class',
    'works for Add\'s three paths as written (35470 = FIRST, 35540 = SECOND, 35580 = THIRD)', 'June runtime', 'yes',
    'covered by Ara CHNb (flag SERV_IRUHADEV_JUNE_CLASS_CHANGE_GUIDE; only the data rows are Add\'s: CHNb)', 'code read; route tsv QUESTS')
add('X2Lib/X2QuestManager.cpp:2580, :4070; X2Lib/X2SkillNoteManager.cpp:349, :446; X2Lib/X2StateClassChange.cpp:466-480', 'class arithmetic (tier / picture by class number)',
    '(class - 10) < 0xbe arithmetic', '33, 36, 123, 126 all inside 10..199', 'June', 'yes', 'covered by Ara CHNa (nothing to do; already June\'s)', 'Elesis CH0 read these lines; unchanged')
add('X2Lib/X2StateClassChange.cpp:304-380 (GetClassChangeDialogName: case 32 :330, 33 :331, 36 :334, 122 :367, 123 :368, 126 :371; the old switch :831-838 is the dead #else of SERV_IRUHADEV_JUNE_SKILL_CLASS_POPUP :590)',
    'CX2ClassChangePopup::SetShow', 'class -> class-change dialog name (June table already in)',
    'names present for 32 / 33 / 36 / 122 / 123 / 126; the files DLG_Class_Change_Add_AT / MM / TT / DE are absent from every March kom_contents dir, PT / LP exist in data034_kom as 1,860 B March copies (June data217_kom has 1,385 B copies)',
    'June 324886b0 -> 32486e60', 'yes', 'CHNb (stage the four dialogs and replace PT / LP byte-exact; luabind.py check; CH0b may stage earlier)',
    'ch0_classn_route.tsv SECTION DIALOGS')
# ---- optional / no-effect end-marker tests
add('X2Lib/X2ItemManager_Preprocessing.cpp:688', 'CX2ItemManager::KProxy2::AddItemTemplet_LUA', 'eUnitClass >= 0 && < UC_END else ErrorLogMsg XEM_ERROR144 INVALID_UNITCLASS_<n>',
    '126 logs (123 / 33 / 36 pass); the templet is still stored; item.kim holds 17 items of class 123 and 12 of class 126, but the kim path does not run this Lua adder', 'not read', 'yes',
    'none (log only; Ara left it)', 'code read; python tools/kim.py census staged --field unit_class')
add('X2Lib/X2ItemManager.cpp:549 (census print :594)', 'IruhaKimCensus (diagnostic)', 'counts templets with class >= UC_END', '126 counted as out of range (12 items); nothing is refused', 'n/a', 'yes', 'none (leave)', 'code read')
add('X2Lib/X2GameEdit.cpp:1646', 'CX2GameEdit::ToggleClassChangeCheat', 'dev class list: for i = 1; i < UC_END', '33 / 36 / 123 offered, 126 not', 'n/a', 'yes', 'none (dev list; Ara left it; optional bound 202 with CHNa)', 'code read')
add('X2Lib/X2ItemStatCalculator.cpp:203', 'CX2ItemStatCalculator::InvalideDataCheck', 'loop UC_ELSWORD_LORD_KNIGHT .. < UC_VALIDE_END (123): m_mapClassStatDataTable must hold every class id', '123 / 126 are outside the loop (nothing checked for them); nothing refuses', 'n/a', 'yes', 'none (checked, not refusing)', 'code read')
add('X2Lib/X2Item_Preprocessing.inl:1036; KncWX2Server/Common/CommonPacket.h:1457 / :2279 / :3006, ClientPacket.h:136 / :2857', 'BOOST_STATIC_ASSERT( UC_END <= 256 ); class id in packets (m_cUnitClass is a signed char)', 'UC_END <= 1 << 8; 126 fits a signed char (< 127)', 'passes', 'n/a', 'yes', 'none', 'Elesis CH0 read these lines')
# ---- Add's own arms
add('X2Lib/X2Unit.h:1456-2200 (GetX2UnitClassCompatibility; Add arms :2154-2192: 9 -> { 9 } :2154, 32 -> { 9, 32 } :2166, 122 -> { 9, 32, 122 } :2180 under SERV_ADD_LUNATIC_PSYKER; default `return false` :2199)',
    'GetX2UnitClassCompatibility( unitClass, stuffClass )', 'switch( unitClass ); no arm for 33 / 36 / 123 / 126',
    'every item refused for a 33 / 36 / 123 / 126 unit (equip, shop, quest item checks): no Add item is wearable',
    'June 31ed6500 cases 0x21 / 0x24 / 0x7b / 0x7e (not read; Ara\'s arms for 34 / 124 are the pattern at :2055-2082)', 'no (one arm per class)',
    'CHNa (flag SERV_IRUHADEV_JUNE_CHAR_ADD_CLASS: 33 -> { 9, 33 }, 36 -> { 9, 36 }, 123 -> { 9, 33, 123 }, 126 -> { 9, 36, 126 } to be confirmed against June)',
    'code read; checklist 7ar (L1024-1026 in the checklist of 2026-11-13; the plan says l.1009)')
add('X2Lib/X2Unit.h:2807-2866 (GetFirstLineNewCharacterClassTypeByUnitClassType :2807-2824 = 9 / 32 / 122; GetSecondLine... :2826-2838 and GetThirdLine... :2840-2852 = every arm breaks to ASSERT + UC_NONE; GetNewCharacterClassByUnitClassTypeAndLineType :2854-2868; UT_ADD arm of GetUnitClassByUnitClassTypeAndLineType :2900-2901)',
    'Get{First,Second,Third}LineNewCharacterClassTypeByUnitClassType', 'UCT_BASIC / FIRST / SECOND per line; second and third line are assert-only',
    'a creation-screen click on line 2 or 3 (X2StateCreateUnit.cpp:786 / :791 / :801 / :806 -> SelectUpperClass :3397 -> GetUnitClassByUnitClassTypeAndLineType :3402) returns UC_NONE and does nothing; ChangeCreateUnitClassChangeInfoDlg :3232 likewise',
    'June 3234b390 row for type 9: line 2 = 9 / 33 / 123, line 3 = 9 / 36 / 126 (the table at X2Unit.h:2927)', 'no',
    'CHNa (second line: BASIC 9, FIRST 33, SECOND 123; third line: BASIC 9, FIRST 36, SECOND 126; as Ara\'s :2677-2695 under her flag)', 'code read')
add('X2Lib/X2GageManager.cpp:219-232 (CreateMyGageUI, function :139; Add labels :223 / :224 / :226 under SERV_ADD_LUNATIC_PSYKER, `new CX2AddGageData` :229-230, default :262-266)',
    'CX2GageManager::CreateMyGageUI', 'switch( eGameUnitClass_ ); Add arm lists 9 / 32 / 122',
    'the four fall to default: generic CX2GageData + CX2MyGageUI; CX2GUAdd::CreateGageData (:1513-1527) and GetAddGageData (:8744) then static_cast the manager\'s gauge to CX2AddGageData (undefined behaviour: DP, mutation and formation readers run on a generic object)',
    'June 3248e290 case list for Add (not read; Ara\'s 34 / 124 arm :192-205 is the pattern)', 'no',
    'CHNa (four case labels under SERV_IRUHADEV_JUNE_CHAR_ADD_CLASS; must land before any of the four loads)', 'code read; ch0_march_side.md section 4')
add('X2Lib/X2StateCreateUnit.cpp:3775-3907 (Add arms :3892-3905: 9 -> MID, HIGH, MID; 32 and 122 -> MID, MID, MID; default NONE x3 :3906)', 'CX2StateCreateUnit::GetUnitStatInfomationChart',
    'switch( class ) -> three stat bars of the creation screen\'s class info; default (NONE, NONE, NONE)', 'the four show empty bars', 'June 3254ee40 cases 0x21 / 0x24 / 0x7b / 0x7e (not read; Ara\'s arm :3867-3872 is the pattern)', 'no',
    'CHNa (one arm for 33 / 36 / 123 / 126, values from June)', 'code read')
add('X2Lib/X2Data.cpp:2821 (GetCharacterImageName; Add 9 -> 52 :2940, 32 -> 53 :2941, 122 -> 54 :2945; Ara\'s 55 / 56 :2947-2951; default :2953; index -1 -> return false :2956-2957) and the nine positional arrays :136 / :235 / :335 / :433 / :531 / :631 / :727 / :826 / :926 (Ara\'s tails under SERV_IRUHADEV_JUNE_CHAR_ARA_CLASS)',
    'CX2Data::GetCharacterImageName( out, out, class, CHARACTER_IMAGE_TYPE )', 'class -> image row index 0..56 -> file / piece in nine arrays; -1 -> return false',
    'no portrait for the four (return false): the character select / party / dungeon-clear pictures stay empty', 'June 31ee0de0 (rows 0x39 / 0x3a for 34 / 124 are Ara\'s; Add\'s rows not read)', 'no',
    'CHNa (append four rows after Ara\'s 55 / 56 to all nine arrays and the switch; Elesis\'s 57 / 58 are planned first, so Add\'s start at 59 unless Add lands first; read the rows from June)', 'code read')
add('X2Lib/X2Data.cpp:3596-3850 (RotateYSelectCharacterType; Add arms :3835-3843: 9 -> 20, 122 -> 3; default 10 :3845-3848)', 'CX2Data::RotateYSelectCharacterType', 'switch( class ) -> character select rotation; default 10',
    '32 / 33 / 36 / 123 / 126 take the default 10 (32 already does today)', 'June rotation (templet reader or table; not read)', 'yes', 'CHNa optional (cosmetic; or the UnitTemplet reader in CH5)', 'code read')
add('X2Lib/X2Data.cpp:3030-3170 (five-argument GetCharacterImageName; transcend prefix switch :3054-3091, Ara\'s :3085-3088, default `return true` :3091)', 'CX2Data::GetCharacterImageName( ..., bTranscend )',
    'class -> TR_ prefix; no Add arm (122 / 123 / 126)', 'a transcended Add keeps the plain image', 'June row pushes (not read)', 'no', 'CH3d (with Ara\'s pattern: three prefixes, texture file)', 'code read')
add('tools/createchar.py:33-53 (CLASSES: 9 :36, 32 :43, 122 :50), :134-144 (PARENT :143 `32: 9, 122: 32`), :157-165 (PROMO: only 9, 41757-41761)', 'createchar.py class table',
    'class id must be a key of CLASSES; PARENT gives the gear lineage; PROMO (base class) is the fallback when UnitTemplet.lua has no complete row for the class',
    'createchar refuses 33 / 36 / 123 / 126; after adding them the promotion set falls back on class 9\'s because the live UnitTemplet.lua has no June row for them',
    'n/a', 'no', 'CHNa (CLASSES 33 / 36 / 123 / 126 = ADD_ARC_TRACER / ADD_TIME_TRACER / ADD_MASTERMIND / ADD_DIABOLIC_ESPER; PARENT 33: 9, 36: 9, 123: 33, 126: 36; promotion gear 41780-41785 / 41828-41833 + 41840 / 41774-41779 / 41834-41839 + 41841 from item.kim after CH5 lands the rows)', 'code read')
add('kom_contents/data036_kom/UnitTemplet.lua (BOM source; live rows: 9 :1482, 32 :1508, 122 :1534; June rows: 9 :1557, 32 :1583, 122 :1611, 33 :1637, 123 :1665, 36 :1691, 126 :1719) (loader X2UnitManager.cpp:836-907 AddUnitTemplet_LUA; GetUnitTemplet :689-699)',
    'CX2UnitManager::GetUnitTemplet / AddUnitTemplet_LUA', 'map lookup by class; the loader has no >= UC_END test',
    'GetUnitTemplet( 33 / 36 / 123 / 126 ) returns NULL: CX2Unit::GetType / GetClass (X2Unit.h:734-735) dereference m_pUnitTemplet, so a unit of these classes crashes or loses its type until the rows land',
    'June rows', 'no', 'CH5 (add the four rows and take June\'s rows for 9 / 32 / 122; core-list file; must land before CHNa plays a unit)', 'line scan of both files')
add('KncWX2Server/ServerResource/US/StatTable.lua (1,554,104 B, 2026-11-11, commit e37c9211) / X2Lib/Offline/X2OfflineStatTable.cpp:332-369', 'CX2OfflineStatTable::GetUnitStat',
    'StatTable has `ReserveMemory( c, 99 )` and 99 SetUnitStat rows for classes 9, 32, 33, 36, 122, 123 and 126 (flat curve, `-- FLAT99`, tools/statflat.py); the unknown-class fallback (UnitTypeOf, synthetic curve) is not reached',
    'stats exist for all four from the flat table: the plan\'s CHNa "stat rows 33 / 36 / 123 / 126" and CH0 item 12 / Open decision 1 are MOOT', 'June relative rows (CharacterStatTable.src.lua:159-409) are replaced by the flat curve by user decision 2026-11-11',
    'yes', 'none (the user packs StatTable.lua; the file is in the tree)', 'grep of the file; git log -- StatTable.lua')
add('X2Lib/Offline/X2OfflineStatTable.cpp:289-319', 'CX2OfflineStatTable::CharAbilTypeOf', 'switch( UnitTypeOf( class ) ): UT_ELSWORD, UT_CHUNG, UT_ARA (and UT_LIRE under June NF) have arms; no UT_ADD arm',
    'every Add class (9 / 32 / 122 and the four) reports CAC_NONE for the char-ability type; the DP gauge packs into m_iCharAbilCount (X2GageData.cpp:1243) regardless', 'GSGameDBThread.cpp:792-809 mirror; June server unknown', 'yes', 'CH1q (DP map: what the emulator answers; not a CH-N edit)', 'code read')
add('X2Lib/Offline/Handlers_Quest.cpp:586-625 (GetChangeUnitClass :590 -> X2OfflineQuest.cpp:3320-3330; SaveUnitClass :596; SeedAutoLearn :607; PushSkillGrant :615-617); X2OfflineQuest.cpp:1455-1461 (class-set eligibility)', 'quest turn-in: job advancement',
    'iNewClass = the QT_CHANGE_JOB quest\'s reward m_eChangeUnitClass; any class != UC_NONE and != current is saved, auto-learn rows seeded (X2OfflineSkillAutoLearn.h: 33 11 rows, 36 11, 123 21, 126 21), skills pushed',
    'works for all four as soon as the quest runs (reward class is a plain int from the templet)', 'June server unknown (stand-in)', 'yes', 'none (works after Ara; verified by CHNb\'s play-test)', 'code read; route tsv HANDLER')
add('X2Lib/Offline/Handlers_Quest.cpp:1416-1451 (TranscendCheck; class gate :1421; allow-list chain :1424-1448; Ara\'s block :1438-1441)', 'CX2OfflineServer::TranscendCheck', 'class 100..199 and level >= 70, then a per-character allow-list (no Add class on it)',
    '122 / 123 / 126 pass the gate (<= 199) and are refused by the allow-list (error 1726); 33 / 36 fail the gate', 'June server unknown (T0 conditions + stand-in list)', 'no', 'CH3d (a SERV_IRUHADEV_JUNE_CHAR_ADD_TRANSCEND block for 122 / 123 / 126, as Ara\'s)', 'code read')
add('X2Lib/Offline/X2OfflineClassChangeSeed.h:124-243 (ROWS: no Add row), :255-306 (PARENT_ROWS: no Add class); Handlers_Shop.cpp:409-516 (ClassChangeParentOf :444: 0 = "unknown, do not refuse")', 'cash class-change tables',
    'cash item -> class; class -> parent; the file has no row naming an Add item (tickets 273863 / 273864 / 276302 / 273233 / 273861 / 276303)', 'a cash class-change of an Add unit is not modelled; unknown parent does not refuse', 'June cash route not read', 'yes',
    'CHNb optional (generated file: regenerate, never hand-edit; PARENT_ROWS 32 -> 9, 33 -> 9, 36 -> 9, 122 -> 32, 123 -> 33, 126 -> 36)', 'code read; grep UC_ADD|UT_ADD in the header: 0')
add('kom_contents/data036_kom/LevelUpEvent.lua (core-list BOM source) vs june_src_fixed/data036_kom/LevelUpEvent.src.lua: level-15 row June :137-152 / live :137-152; level-35 row June :358-376 / live :303-316; level-70 row June :604-621 / live none',
    'CX2LVUpEventMgr event table (Add TYPE8 rows)', 'level 15 { 9 }, guide dialog named Class_Change_Guide_Add.lua (March); level 35 { 32 } with sounds only; level 70: no Add row',
    'a level-15 Add sees March\'s old two-path guide, a level-35 Arc / Time Tracer sees no guide, no level-70 transcendence guide for 122 / 123 / 126', 'June rows (DLG_1ST_ / DLG_2ND_Class_Change_Guide_Add.lua, DLG_Transcendental_Change_Guide_Add.lua)', 'no',
    'CHNb (level-15 dialog name; level-35 class list PT + AT + TT and DIALOG_LIST; stage the guides) and CH3d (level-70 row)', 'route tsv SECTION LEVELUP')
add('kom_contents/data036_kom/FieldQuest.lua + FieldSubQuest.lua (June bytecode) / june_src_fixed FieldQuest.src.lua :98695-100880', 'QT_CHANGE_JOB quests 35540-35543, 35550-35554, 35580-35583, 35590-35594',
    'quests exist in the shipped file (titles byte-scanned); ids, classes, levels and rewards confirmed in the route tsv', 'accepted by class set; reward class 33 / 123 / 36 / 126 on the last quest', 'June', 'yes',
    'none (data present); CHNb play-test with /qdone', 'ch0_classn_route.tsv SECTION QUESTS')
# ---- X2GUAdd.cpp: class switches inside the class
add('X2Lib/X2GUAdd.cpp:272-298 (ParseCommonRandomState; switch :277-297; arms 9 :279, 32 :283, 122 :288; default ASSERT( !"NO" ) :292-295)', 'CX2GUAdd::ParseCommonRandomState',
    'switch( m_pUnit->GetClass() ) -> INIT_COMMON_RANDOM_STATE table name ADD_NASOD_RULER / ADD_PSYCHIC_TRACER / ADD_LUNATIC_PSYKER', '33 / 36 / 123 / 126 hit the ASSERT at unit init (the table name stays empty)',
    'June script has the tables ADD_ARC_TRACER :149, ADD_MASTERMIND :177, ADD_TIME_TRACER :215, ADD_DIABOLIC_ESPER :243 of Add_NasodRuler.src.lua', 'no', 'CH1m (June\'s body replaces the switch whole; NOT a CHNa edit)', 'code read; luabind source')
add('X2Lib/X2GUAdd.cpp:303-320 (InitState; cut-in switch :305-320: 9 / 32 -> set 0, `default:` falls into 122 -> set 1, UC_NONE -> ASSERT)', 'CX2GUAdd::InitState (skill cut-in set index)',
    'switch( GetUnitClass() )', '33 / 36 / 123 / 126 take the default and fall into the 122 arm: cut-in set 1 (no assert; unlike Elesis\'s default-assert)', 'June table (not read)', 'no',
    'CH5 (cut-in table and the index switch; the kickoff says so)', 'code read')
add('X2Lib/X2GUAdd.cpp:385 (InitPassiveSkillState; class switch :498; arms 122 :501, 32 :555)', 'CX2GUAdd::InitPassiveSkillState', 'per-class passive arms for 122 / 32', 'no passives for 33 / 36 / 123 / 126', 'June passives per class', 'no', 'CH4a (as planned; NOT CHNa)', 'code read')
add('X2Lib/X2GUAdd.cpp:2314-2370 (InitStateByUnitClass; switch :2317; arms 9 :2319, 122 :2326, 32 :2364; no default arm)', 'CX2GUAdd::InitStateByUnitClass', 'per-class state registration', 'the four register no class states (a 33 / 36 / 123 / 126 unit has the common states only)',
    'June InitState body per class (AAT / AMM / ATT / ADE tags)', 'no', 'CH1e / CH1f / CH1g (Psychic Tracer + Lunatic Psyker; Arc Tracer + Mastermind; Time Tracer + Diabolic Esper), registration CH1m', 'code read')
add('X2Lib/X2GUAdd.cpp:5798, :5910 (NasodArmorComboZZEventProcess, NasodArmorComboXEventProcess)', 'class tests in nasod-armor combos', '`( UC_ADD_LUNATIC_PSYKER == GetUnitClass() || UC_ADD_LUNATIC_PSYKER == GetUnitClass() ) &&` (the same class twice: a March typo)',
    'only 122 passes; Mastermind (123) does not', 'June bodies (CH1 transcription replaces them)', 'no', 'CH1e / CH1f (June body decides whether 123 joins; NOT CHNa)', 'code read')
add('X2Lib/X2GUAdd.cpp:3101, :3133 (SetSpecificValueByBuffTempletID / UnSet...: BTI_FORMATION_MODE) and X2Lib/X2GUAdd.h:29 (ELSE_IF_CAN_NASOD_ARMOR_ATTACK)', 'class != UC_ADD_NASOD_RULER tests', 'every class but 9 gets the nasod armor buff factor / attack macro', 'the four behave as 32 / 122', 'June bodies', 'no', 'none here (CH1q2 owns the nasod map)', 'code read')
add('X2Lib/X2GUUser.cpp:11572-11585 (UT_ADD damage block: SetStasisfieldDamageValue; `case UC_ADD_LUNATIC_PSYKER:` SetReverseReactorTransitionDamage)', 'CX2GUUser::DamageReact Add block', 'unit type 9 -> Stasis Field value; class 122 only -> Reverse Reactor value',
    '123 / 126 get the Stasis Field value, no Reverse Reactor value', 'June test (not read)', 'no', 'CH1h2 (0x60 DamageReact `32a07fc0`) / CH4b; NOT CHNa', 'code read')
add('X2Lib/X2GUUser.cpp:34364-34368 (GetHyperModeTableName: 122 ANSI_ALP_HYPER_MODE, 123 ANSI_AMM_HYPER_MODE, 126 ANSI_ADE_HYPER_MODE under SERV_IRUHADEV_JUNE_ENUM_NAMES); :27518 (ResetOnSummonEnd: UC_ADD_MASTERMIND arm, BTI_BUFF_APOCALYPSE)', 'CX2GUUser::GetHyperModeTableName / ResetOnSummonEnd',
    'transcended-class names and the Mastermind summon-end hook', 'present for 122 / 123 / 126', 'June', 'yes', 'none here (CH3d owns the table check; Eve CH2b2 wrote the Mastermind arm)', 'code read')
add('X2Lib/X2UnitViewerUI.cpp:775, :2624 (frame move: `UC_ADD_LUNATIC_PSYKER == class` and ALP_LobbyWait 3.6-4.5 s hand-electricity particles; SetUnit: ADD_LP_LobbyWait01)', 'CX2UnitViewerUI lobby particles', 'class 122 only', '123 / 126 show no lobby particles (June\'s rule not read)', 'not read', 'yes', 'Stage 9 / CH5 optional (cosmetic)', 'code read')
add('X2Lib/X2UnitViewerUI.cpp:4152-5346 (SERV_IRUHADEV_JUNE_ANIMATION_NAME_TABLE, 4460-5309 are the March per-class arms in the dead #else) and X2Lib/X2SquareUnit.cpp:913-1098 (same flag); kom_contents/data036_kom/AnimationNameTable.lua', 'lobby / square animation names by class',
    'the live AnimationNameTable.lua (June bytecode) carries AAT_ / ATT_ / AMM_ / ADE_ LobbyReadyAction / LobbyDownLanding / LobbyWait rows (byte scan of UC_ADD_ARC_TRACER, UC_ADD_TIME_TRACER, UC_ADD_MASTERMIND, UC_ADD_DIABOLIC_ESPER)', 'covered by the table', 'June table', 'yes', 'none', 'byte scan; flag state by cl /P (ON)')
add('X2Lib/X2UISkillTreeNew2015.cpp:5614-5680 (class -> name: ADD_ARC_TRACER :5639, ADD_TIME_TRACER :5642, ADD_MASTERMIND :5676, ADD_DIABOLIC_ESPER :5679); X2Lib/X2UISkillTreeNew.cpp:6528-6556 (dead: #else of SERV_IRUHADEV_JUNE_SKILL_TREE_UI)', 'skill tree class names / images', 'the live June tree names all seven classes; the March tree is dead', 'covered', 'June', 'yes', 'none', 'code read; flag ON by cl /P')
add('X2Lib/X2ComboTree.cpp:212-323 (CX2ComboTree name list: ADD_NASOD_RULER :215, ADD_PSYCHIC_TRACER :319, ADD_LUNATIC_PSYKER :323)', 'CX2ComboTree (old combo window)', 'class -> ComboTree.lua table name; no 33 / 36 / 123 / 126 entry', 'the old combo window shows nothing for the four (the live ComboTree.lua is June\'s)', 'n/a', 'yes', 'none (Stage 9; Ara\'s map left it)', 'Elesis CH0 note; code read')
add('X2Lib/X2StateCreateUnit.cpp:25-29, :111, :132-134, :207-211, :224-227, :775, :853-855, :1242-1246, :2346-2450 (SelectAdd), :2607, :2680, :2883, :2965-2967, :3166, :3321, :3518, :3582, :4039; X2Lib/X2UIPersonalShopBoard.cpp:635-956 (seven `>= UT_ADD` tests at :662, :713, :768, :815, :862, :909, :956)',
    'creation screen, Add entry; personal shop board filter', 'all keyed on UT_ADD / the base class UC_ADD_NASOD_RULER; hidden by `_CONST_LIMIT_UNIT_::iLimitType` = 8 < UT_ADD (US_SERVICE cl /P)', 'the entry is unreachable today; the June-only class buttons need the line helpers and the stat chart (rows above)', 'June SelectAdd 32555320', 'no',
    'CH0c (the March entry, limit lift) + CHNa (the June-only entries: class buttons, previews, Movie_Create_Add_*)', 'ch0_march_side.md section 1')
add('X2Lib/Offline/X2OfflineInventory.cpp:1236 (GBASE_ITEM row for unit type 9: 112991-112995 / promotion 41757-41761, commented "unreachable")', 'CX2OfflineInventory::SeedBeginnerGear', 'keyed by unit type of the class', 'unreachable until the creation entry opens', 'live DB row', 'yes', 'CH0c', 'code read')
add('X2Lib/Offline/X2OfflineLuaEnumSeed.h:70, :93, :116-118', 'UNIT_CLASS seed for the server Lua globals', 'name -> value table; Add names 9 / 32 / 122 only; Ara\'s 34 / 124 are not there either; the only US server Lua naming a class is RandomItemTable.lua (UC_ADD_NASOD_RULER)',
    'none: no server Lua names 33 / 36 / 123 / 126', 'n/a', 'yes', 'none (generated file, do not hand-edit; the plan\'s CHNa "X2OfflineLuaEnumSeed.h names" item is not needed)', 'grep of ServerResource/US')
add('june_src_fixed/data036_kom/CharCustomData.src.lua :33-36, :122-125, :424-453 (free motion / skill cut-in / shop rows by class)', 'CX2CustomizeManager', 'free motion 91005-91008 and cut-in 91015 for classes 9 / 32 / 33 / 36; shop rows for 450065-450067 / 450096-450098', 'present in June\'s file', 'June', 'yes', 'none (Stage 12)', 'line scan')
add('X2Lib/X2CashShop.cpp:8265, :8272, :8606 (UNIT_CLASS_CHANGE_ITEM descriptions: UC_ADD_PSYCHIC_TRACER :8265, UC_ADD_LUNATIC_PSYKER :8272 / :8606) and :14268 (UC_ADD_LUNATIC_PSYKER in the dead #else of SKILL_CLASS_ITEM_CHOICE)', 'cash class-change dialogs / item maps', 'March ids for 32 / 122 only', 'no effect on the job-change route (live maps are the CHNb rows above)', 'n/a', 'yes', 'none', 'code read')
add('X2Lib/X2Eqip.cpp, X2SocketItem.cpp / .h, X2UICharInfo.cpp, X2UISocketItem.cpp, X2Game.cpp, X2GameUnit.cpp, X2StateServerSelect.cpp, X2StateDungeonGame.cpp, X2StatePVPGame.cpp, X2EventScene.cpp, X2SquareUnit.cpp:220, X2DungeonManager.cpp:442 / :589 and the other UT_ADD sites (~100 lines)',
    'unit-TYPE tests (CX2Unit::UT_ADD)', 'keyed on unit type 9, not on the class', 'all seven classes share unit type 9: nothing to do', 'n/a', 'yes', 'none', 'grep -rn UT_ADD X2Lib: every hit read as a type test')
add('KncWX2Server/Common/X2Data/XSLUnit.cpp / XSLItem.cpp / XSLUnit.h', 'server class helpers', 'server-side class chain', 'n/a (offline: the emulator is the server)', 'n/a', 'n/a', 'none', 'Elesis CH0 note; not read again')

# primary owner of each row, in row order (the proposed_phase text carries the detail); `none` = no edit needed
OWN = ['Ara CHNa', 'Ara CHNa', 'Ara CHNa', 'Ara CHNb', 'Ara CHNb', 'Ara CHNb', 'Ara CHNa', 'CHNb',
       'none', 'none', 'none', 'none', 'none',
       'CHNa', 'CHNa', 'CHNa', 'CHNa', 'CHNa', 'CHNa', 'CH3d', 'CHNa', 'CH5',
       'none', 'CH1q', 'none', 'CH3d', 'CHNb', 'CHNb', 'none',
       'CH1m', 'CH5', 'CH4a', 'CH1e-g', 'CH1e-g', 'none', 'CH1h2', 'none', 'CH5', 'none', 'none', 'none',
       'CH0c + CHNa', 'CH0c', 'none', 'none', 'none', 'none', 'none']
assert len(OWN) == len(R), (len(OWN), len(R))
for r, o in zip(R, OWN):
    r.append(o)

with open(HERE + 'ch0_classn.tsv', 'w', encoding='utf-8', newline='') as f:
    w = csv.writer(f, delimiter='\t', lineterminator='\n')
    w.writerow(H)
    for r in R:
        w.writerow([c.replace('\t', ' ').replace('\n', ' ') for c in r])
import collections
print('rows', len(R), dict(collections.Counter(OWN)))
