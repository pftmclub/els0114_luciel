"""Write the bytepatch spec that puts Elesis's June registration into X2GUEL.cpp under _LIVE (char-elesis CH1m).

    python june2017_re/char_elesis/ch1m_spec.py <gen dir> <spec out>   # run from Trunk; <gen dir> = ch1m_gen.py's output

Ara's ch1m_spec.py with Elesis's names. The InitState wrapper (32b87a60: cut-in index, START / WIN / LOSE, wait-state-change, tail),
InitStateID (June inlines it @32b87af5: the sword wait survives) and ParseCommonRandomState (June 31feaac0 by class name) are written
here; InitStateCommon (32b81be0) and InitStateByUnitClass (32b864f0) come from ch1m_gen.py. Each function gets `#ifdef _LIVE` June
`#else` March `#endif` around its whole body. Also the registration guards of the _LIVE block (Always.h). No berserk table for Elesis.
"""
import os, sys
F = 'SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE'
gd, out = sys.argv[1], sys.argv[2]


def inc(name):
    return open(os.path.join(gd, name), 'rb').read().decode('ascii').replace('\r\n', '\n').rstrip('\n').split('\n')


C = 'CX2Unit::'
CLASS = '\tconst CX2Unit::UNIT_CLASS eClass = ( NULL != GetUnit() ) ? GetUnit()->GetUnitData().m_UnitClass : CX2Unit::UC_NONE;'
init_state_head = [
    '#ifdef ' + F,
    '\t// char-elesis CH1m: June 32b87a60 ( vslot 0x7a4 )',
    CLASS,
    '',
    '\tswitch( eClass )	// June byte table 32b87fec: every class but 120 / 121 / 125 gets 0, Dark Knight included',
    '\t{',
    '\tcase %sUC_ELESIS_GRAND_MASTER:' % C,
    '\t\tm_iSkillCutInSetIndex = 1;',
    '\t\tbreak;',
    '\tcase %sUC_ELESIS_BLAZING_HEART:' % C,
    '\t\tm_iSkillCutInSetIndex = 2;',
    '\t\tbreak;',
    '\tcase %sUC_ELESIS_CRIMSON_AVENGER:' % C,
    '\t\tm_iSkillCutInSetIndex = 3;',
    '\t\tbreak;',
    '\tdefault:',
    '\t\tm_iSkillCutInSetIndex = 0;',
    '\t\tbreak;',
    '\t}',
    '',
    '\tInitStateID();				// June inlines it @32b87af5',
    '\tInitStateCommon();			// June 32b81be0',
    '\tInitStateByUnitClass();		// June 32b864f0 ( June runs InitEventStates first; their ids do not meet )',
    '',
    '\tstd::string strStartTableNameUTF8;',
    '\tstd::string strWinTableNameUTF8;',
    '\tstd::string strLoseTableNameUTF8;',
    '\tInitStateCommonRandom( strStartTableNameUTF8, strWinTableNameUTF8, strLoseTableNameUTF8 );	// June 3202b110 then 31fe22d0',
    '',
    '\tUserUnitStateData stateData;',
    '',
    '\tstateData.Init();',
    '\tstateData.stateID			= USI_START;',
    '\tm_LuaManager.MakeTableReference( strStartTableNameUTF8.c_str(), stateData.stateID );',
    '\tstateData.StateStart		= SET_CB_FUNC( CX2GUUser, StartStateStart );',
    '\tstateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartEventProcess );',
    '\tstateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );',
    '\tm_StateList[stateData.stateID] = stateData;',
    '',
    '\tstateData.Init();',
    '\tstateData.stateID			= USI_WIN;',
    '\tm_LuaManager.MakeTableReference( strWinTableNameUTF8.c_str(), stateData.stateID );',
    '\tstateData.StateStart		= SET_CB_FUNC( CX2GUUser, WinStateStart );',
    '\tstateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RandomLoseEventProcess );',
    '\tm_StateList[stateData.stateID] = stateData;',
    '',
    '\tstateData.Init();',
    '\tstateData.stateID			= USI_LOSE;',
    '\tm_LuaManager.MakeTableReference( strLoseTableNameUTF8.c_str(), stateData.stateID );',
    '\tstateData.StateStart		= SET_CB_FUNC( CX2GUUser, LoseStateStart );',
    '\tm_StateList[stateData.stateID] = stateData;',
    '',
    '\tInitWaitStateChange( "ESI_WAIT_STATE_CHANGE" );	// June @32b87ed1 ( id 0x82, checklist 7h )',
    '',
    '\tInitPassiveSkillState();					// June vslot 0x690',
    '\tInitEquippedActiveSkillState( false );		// June vslot 0x684',
    '\tCX2GUUser::InitState();						// June 31fd8f00',
    '#else ' + F,
]
state_id_head = [
    '#ifdef ' + F,
    '\t// char-elesis CH1m: June inlines it in 32b87a60 @32b87af5, a sword wait set before InitState survives',
    '\tif( ESI_SWORD_WAIT != m_CommonState.m_Wait )',
    '\t\tm_CommonState.m_Wait = USI_WAIT;',
    '#else ' + F,
]
random_head = [
    '#ifdef ' + F,
    '\tParseCommonRandomStateByUnitClass();	// char-elesis CH1m: June parses in CX2GUUser::Init ( 31feaac0 ) by class name, 35 / 125 included',
    '#else ' + F,
]
common_head = [
    '#ifdef ' + F,
    '\t// char-elesis CH1m: June 32b81be0 ( InitState part 1, no class test )',
    '\tUserUnitStateData stateData;',
    '\tconst std::string strHyperModeTable = GetHyperModeTableName( false );	// June 31fdd490( ..., 0 )',
    '',
] + inc('InitStateCommon.inc') + ['#else ' + F]
by_class_head = [
    '#ifdef ' + F,
    '\t// char-elesis CH1m: June 32b864f0 ( InitState part 2, the per-class part )',
    '\tUserUnitStateData stateData;',
    CLASS,
    '',
] + inc('InitStateByUnitClass.inc') + ['#else ' + F]
END = ['#endif ' + F]

guards = [
    '',
    '//////////////////////////////////////////////////////////////////////////',
    '// Author: Iruha',
    '// Date: 2026-11-13',
    '// Description: June character port ( Elesis ) Phase CH1m - guards only ( no new flag ): the registration calls the shared registrars ( GetHyperModeTableName, StartStateStart, InitEventStates, RandomLoseEventProcess, InitWaitStateChange, InitBoundingStates, ParseCommonRandomStateByUnitClass ) and the hyper StartFuture callbacks',
    '#if defined( SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE ) && ( !defined( SERV_IRUHADEV_JUNE_CHAR_ARME_LIVE ) || !defined( HYPER_MODE_FIX ) || !defined( SERV_IRUHADEV_JUNE_COMMON_RANDOM_STATE ) || !defined( SERV_IRUHADEV_JUNE_EVENT_STATES ) || !defined( SERV_IRUHADEV_JUNE_WAIT_STATE_CHANGE ) || !defined( SERV_IRUHADEV_JUNE_BOUNDING_STATE ) )',
    '#error SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE needs SERV_IRUHADEV_JUNE_CHAR_ARME_LIVE ( ParseCommonRandomStateByUnitClass ), HYPER_MODE_FIX, SERV_IRUHADEV_JUNE_COMMON_RANDOM_STATE, SERV_IRUHADEV_JUNE_EVENT_STATES, SERV_IRUHADEV_JUNE_WAIT_STATE_CHANGE and SERV_IRUHADEV_JUNE_BOUNDING_STATE',
    '#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE',
    '//////////////////////////////////////////////////////////////////////////',
]


def edit(op, anchor, text):
    return ['@@ ' + op, '@@ anchor'] + anchor + ['@@ text'] + text + ['@@ end']


S = ['@@ file X2Lib/X2GUEL.cpp']
S += edit('insert_after', ['/*virtual*/ void CX2GUEL::ParseCommonRandomState()', '{'], random_head)
S += edit('insert_after', ['\t\tm_LuaManager.EndTable(); // INIT_COMMON_RANDOM_STATE', '\t}'], END)
S += edit('insert_after', ['/*virtual*/ void CX2GUEL::InitState()', '{'], init_state_head)
S += edit('insert_after', ['\tInitEquippedActiveSkillState();', '', '\tCX2GUUser::InitState();'], END)
S += edit('insert_after', ['void CX2GUEL::InitStateID()', '{'], state_id_head)
S += edit('insert_before', ['}', '', '/** @function\t: InitStateCommon'], END)
S += edit('insert_after', ['void CX2GUEL::InitStateCommon()', '{'], common_head)
S += edit('insert_before', ['}', '', 'void CX2GUEL::InitStateByUnitClass()'], END)
S += edit('insert_before', ['\tUserUnitStateData stateData;', '\tswitch ( GetUnitClass() )'], by_class_head)
S += edit('insert_before', ['}', '', '', 'void CX2GUEL::SetEquippedSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, EL_STATE_ID eStateID_ )'], END)
S += ['@@ file KTDXLIB/Always.h']
S += edit('insert_after occurrence=3', ['#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE', '//////////////////////////////////////////////////////////////////////////'], guards)
open(out, 'w', encoding='ascii', newline='\n').write('\n'.join(S) + '\n')
print('spec lines', len(S))
