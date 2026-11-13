#include "StdAfx.h"

#ifdef SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE
#include "X2TutorialGuideManager.h"
#ifdef SERV_IRUHADEV_JUNE_GFX_MANAGER
#include "../KTDXLIB/KTDGGFXManager.h"
#endif SERV_IRUHADEV_JUNE_GFX_MANAGER
#ifdef SERV_IRUHADEV_OFFLINE
#include "Offline/X2OfflineLog.h"
#endif SERV_IRUHADEV_OFFLINE

// june-dungeons C3z-4t1: log tag JUNE_FX C3z-4t ( offline_server.log ); every function names its June address.

namespace _CONST_TUTORIAL_GUIDE_C3Z4T1_
{
	const int	QUEST_ID_VISIT_RUBEN				= 11000;	// June 31fbde9c 0x2af8 ( TutorialGuideInfo.lua CQI_VISIT_RUBEN )
	const int	BEGIN_GUIDE_MAX_LEVEL				= 7;		// June 31fc45ec
	const int	ACCOUNT_HIGH_LEVEL					= 10;		// June 31fbdf8a / 31fbdf10
	const float	NO_EVENT_NPC_DISTANCE				= 100000.f;	// June 33438ae0
	const float	INPUT_GUIDE_HIDE_TIME				= 0.5f;		// June 3343777c
	const float	INPUT_GUIDE_BLINK_ON_TIME			= 1.5f;		// June 33471a1c
	const float	INPUT_GUIDE_BLINK_OFF_TIME			= 0.3f;		// June 3345993c
	const float	INPUT_GUIDE_BLINK_SUB_START			= 0.75f;	// June 33439b44
	const float	DASH_GUIDE_ARROW2_TIME				= 0.45f;	// June 33471c08
	const float	DASH_GUIDE_ARROW3_TIME				= 0.9f;		// June 3343a488
	const float	DASH_GUIDE_ARROW4_TIME				= 1.35f;	// June 33471c04
	const float	DASH_GUIDE_DONE_TIME				= 1.5f;		// June 33471a1c
	const float	CAMERA_ARRIVE_DISTANCE				= 30.f;		// June 33439c68
	const float	INPUT_GUIDE_DIALOG_LAYER			= 0.07f;	// June 3344b78c
	const int	GUIDE_NO_SKILL_KEY					= 101;		// June 31fc5687 0x65 ( TutorialGuideInfo.lua scene 101 )
	const int	GUIDE_NO_AWAKE_KEY					= 102;		// June 31fc5745 0x66 ( TutorialGuideInfo.lua scene 102 )
	const int	LAST_CUSTOM_EVENT_NONE				= -20;		// June 31fc3a40 / 31fc4d5e 0xffffffec
	const int	SERVER_EVENT_ID_INVENTORY_SLOT_ITEM	= 0x101;	// June 31fc1d93: the event id SERVER_EVENT_TYPE 1 waits for ( t3 maps it )
}
using namespace _CONST_TUTORIAL_GUIDE_C3Z4T1_;

void CX2TutorialGuideManager::GuideScene::Init()
{
	m_iGuideNo							= 0;
	m_bWait								= false;
	m_bClickWait						= false;
	m_iConditionState					= 0;
	m_iConditionFieldID					= 0;
	m_iConditionGoingQuestID			= 0;
	m_iConditionNewQuestID				= 0;
	m_iConditionCompleteQuestID			= 0;
	m_iConditionFailedToEnterVillage	= 0;
	m_bFirstGuide						= false;
	m_bFixSkillTypeA					= false;
	m_bUseLevelUpRepeat					= false;
	m_bUseStartTime						= false;
	m_fStartTime						= 0.f;
	m_iOpenUIWithGuideStart				= 0;
	m_iChangeInventoryTab				= 0;
	m_iPriorityLevel					= 0;
	m_bUseKeyLimitInGuide				= false;
	m_fKeyLimitTime						= 0.f;
	m_bUseLimitZoom						= false;
	m_bUseChangeMPValue					= false;
	m_fChangeMPValue					= 0.f;
	m_bUseCameraMove					= false;
	m_iCameraTargetNPCID				= 0;
	m_bNeedNPCLoaded					= false;
	m_bNoStopFrame						= false;
	m_bAnyKeyAllow						= false;
	m_bEscKeyAllow						= false;
	m_bDirKeyAllow						= false;
	m_bAttackKeyAllow					= false;
	m_bSkillKeyAllow					= false;
	m_bAwakeKeyAllow					= false;
	m_iServerEventID					= 0;
	m_iCustomEventMsg					= -10;	// June 31fc67a0 0xfffffff6
	m_iCustomEventSubMsg1				= -10;
	m_iCustomEventSubMsg2				= -10;
	m_bMouseEventAllow					= false;
	m_iButtonEventAllowMsg1				= -10;
	m_iButtonEventAllowMsg2				= -10;
	m_iButtonEventAllowMsg3				= -10;
	m_bDialogCreateAllow				= false;
	m_iGFxUINo							= 0;
	m_bUseOffUIInEndCheck				= false;
	m_bUseAutoGuideStop					= false;
	m_iReplaceGuideNo					= 0;
	m_bUseEndTime						= false;
	m_fEndTime							= 0.f;
	m_bAutoNextGuide					= false;
	m_iNextGuideNo						= 0;
	m_bReplayGuide						= false;
	m_iReplayGuideNo					= 0;
}

CX2TutorialGuideManager::CX2TutorialGuideManager() :
m_iGuideState( GS_NONE ),
m_bWaitCameraArrive( false ),
m_pTutorialGuideAdapter( NULL ),
m_fGuideTime( 0.f ),
m_bEndTimeRunning( false ),
m_bStartTimeRunning( false ),
m_bKeyLimitTimeRunning( false ),
m_pGuideStartBlocker( NULL ),
m_bGuideOffByF2( false ),
m_bAccountHasHighLevelUnit( false ),
m_iReservedGuideNo( 0 ),
m_bSkipNextGuide( false ),
m_bReplayGuideReserved( false ),
m_iReplayGuideNo( 0 ),
m_iLastCustomEventMsg( LAST_CUSTOM_EVENT_NONE ),
m_pDLGInputGuide( NULL ),
m_iInputGuideStep( -1 ),
m_iReservedInputGuideStep( -1 ),
m_fInputGuideBlinkTime( 0.f ),
m_fInputGuideBlinkTimeSub( 0.f ),
m_fDashGuideTime( 0.f ),
m_fInputGuideHideTime( 0.f ),
m_fInputAlarmTime( 0.f ),
m_bDashGuideDone( false ),
m_pDLGInputSideGuide( NULL ),
m_bInputGuideHidden( false )
{
	ResetInputGuideState();
}

CX2TutorialGuideManager::~CX2TutorialGuideManager()
{
	// June 31fbe320 frees both dialogs through the dialog manager; at CX2Main's end the manager may be gone already
	if( NULL != g_pKTDXApp && NULL != g_pKTDXApp->GetDGManager() && NULL != g_pKTDXApp->GetDGManager()->GetDialogManager() )
	{
		SAFE_DELETE_DIALOG( m_pDLGInputGuide );
		SAFE_DELETE_DIALOG( m_pDLGInputSideGuide );
	}
	m_mapMoveGuideKeyPressed.clear();
	m_mapGuideScene.clear();
}

void CX2TutorialGuideManager::Load()
{
	LoadTutorialGuideInfo();
	// stand-in: GFx adapter is t2 ( June 31fc3c20 creates it on g_pKTDXApp+0x1c8 +0x98 here )
	m_pTutorialGuideAdapter = NULL;
}

bool CX2TutorialGuideManager::LoadTutorialGuideInfo()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	if( false == g_pKTDXApp->LoadAndDoMemory( &luaManager, L"TutorialGuideInfo.lua" ) )
	{
#ifdef SERV_IRUHADEV_OFFLINE
		CX2OfflineLog::Server( L"JUNE_FX C3z-4t load failed: TutorialGuideInfo.lua did not load (data036); the tutorial guide is off" );
#endif SERV_IRUHADEV_OFFLINE
		return false;
	}

	if( false == luaManager.BeginTable( "TUTORIAL_GUIDE_INFO" ) )
		return false;

	if( true == luaManager.BeginTable( "GUIDE_SCENE" ) )
	{
		int iIndex = 1;
		while( true == luaManager.BeginTable( iIndex ) )
		{
			GuideScene kScene;
			LUA_GET_VALUE( luaManager, "GUIDE_NO",							kScene.m_iGuideNo,							0 );
			LUA_GET_VALUE( luaManager, "WAIT",								kScene.m_bWait,								false );
			LUA_GET_VALUE( luaManager, "CLICK_WAIT",						kScene.m_bClickWait,						false );
			LUA_GET_VALUE( luaManager, "CONDITION_STATE",					kScene.m_iConditionState,					0 );
			LUA_GET_VALUE( luaManager, "CONDITION_GOING_QUEST_ID",			kScene.m_iConditionGoingQuestID,			0 );
			LUA_GET_VALUE( luaManager, "CONDITION_NEW_QUEST_ID",			kScene.m_iConditionNewQuestID,				0 );
			LUA_GET_VALUE( luaManager, "CONDITION_COMPLETE_QUEST_ID",		kScene.m_iConditionCompleteQuestID,			0 );
			LUA_GET_VALUE( luaManager, "CONDITION_FIELD_ID",				kScene.m_iConditionFieldID,					0 );
			LUA_GET_VALUE( luaManager, "CONDITION_FAILED_TO_ENTER_VILLAGE",	kScene.m_iConditionFailedToEnterVillage,	0 );
			LUA_GET_VALUE( luaManager, "FIRST_GUIDE",						kScene.m_bFirstGuide,						false );
			LUA_GET_VALUE( luaManager, "FIX_SKILL_TYPE_A",					kScene.m_bFixSkillTypeA,					false );
			LUA_GET_VALUE( luaManager, "USE_LEVLE_UP_REPEAT",				kScene.m_bUseLevelUpRepeat,					false );
			LUA_GET_VALUE( luaManager, "USE_START_TIME",					kScene.m_bUseStartTime,						false );
			LUA_GET_VALUE( luaManager, "START_TIME",						kScene.m_fStartTime,						0.f );
			LUA_GET_VALUE( luaManager, "OPEN_UI_WITH_GUIDE_START",			kScene.m_iOpenUIWithGuideStart,				0 );
			LUA_GET_VALUE( luaManager, "CHANGE_INVENTORY_TAB",				kScene.m_iChangeInventoryTab,				0 );
			LUA_GET_VALUE( luaManager, "PRIORITY_LEVEL",					kScene.m_iPriorityLevel,					0 );
			LUA_GET_VALUE( luaManager, "USE_KEY_LIMIT_IN_GUIDE",			kScene.m_bUseKeyLimitInGuide,				false );
			LUA_GET_VALUE( luaManager, "KEY_LIMIT_TIME",					kScene.m_fKeyLimitTime,						0.f );
			LUA_GET_VALUE( luaManager, "USE_LIMIT_ZOOM",					kScene.m_bUseLimitZoom,						false );
			LUA_GET_VALUE( luaManager, "USE_CHANGE_MP_VALUE",				kScene.m_bUseChangeMPValue,					false );
			LUA_GET_VALUE( luaManager, "CHANGE_MP_VALUE",					kScene.m_fChangeMPValue,					0.f );
			LUA_GET_VALUE( luaManager, "USE_CAMERA_MOVE",					kScene.m_bUseCameraMove,					false );
			LUA_GET_VALUE( luaManager, "CAMERA_TARGET",						kScene.m_iCameraTargetNPCID,				0 );
			LUA_GET_VALUE( luaManager, "NEED_NPC_LOADED",					kScene.m_bNeedNPCLoaded,					false );
			LUA_GET_VALUE( luaManager, "NO_STOP_FRAME",						kScene.m_bNoStopFrame,						false );
			LUA_GET_VALUE( luaManager, "ANY_KEY_ALLOW",						kScene.m_bAnyKeyAllow,						false );
			LUA_GET_VALUE( luaManager, "ESC_KEY_ALLOW",						kScene.m_bEscKeyAllow,						false );
			LUA_GET_VALUE( luaManager, "DIR_KEY_ALLOW",						kScene.m_bDirKeyAllow,						false );
			LUA_GET_VALUE( luaManager, "ATTACK_KEY_ALLOW",					kScene.m_bAttackKeyAllow,					false );
			LUA_GET_VALUE( luaManager, "SKILL_KEY_ALLOW",					kScene.m_bSkillKeyAllow,					false );
			LUA_GET_VALUE( luaManager, "AWAKE_KEY_ALLOW",					kScene.m_bAwakeKeyAllow,					false );

			int iServerEventType = 0;
			LUA_GET_VALUE( luaManager, "SERVER_EVENT_TYPE",					iServerEventType,							0 );
			if( 1 == iServerEventType )		// TutorialGuideInfo.lua SEVER_EVENT_INVENTORY_SLOT_ITEM
				kScene.m_iServerEventID = SERVER_EVENT_ID_INVENTORY_SLOT_ITEM;

			LUA_GET_VALUE( luaManager, "CUSTOM_EVENT_MSG",					kScene.m_iCustomEventMsg,					-10 );
			LUA_GET_VALUE( luaManager, "CUSTOM_EVENT_SUB_MSG1",				kScene.m_iCustomEventSubMsg1,				-10 );
			LUA_GET_VALUE( luaManager, "CUSTOM_EVENT_SUB_MSG2",				kScene.m_iCustomEventSubMsg2,				-10 );
			LUA_GET_VALUE( luaManager, "MOUSE_EVENT_ALLOW",					kScene.m_bMouseEventAllow,					false );
			LUA_GET_VALUE( luaManager, "BUTTON_EVENT_ALLOW_MSG1",			kScene.m_iButtonEventAllowMsg1,				-10 );
			LUA_GET_VALUE( luaManager, "BUTTON_EVENT_ALLOW_MSG2",			kScene.m_iButtonEventAllowMsg2,				-10 );
			LUA_GET_VALUE( luaManager, "BUTTON_EVENT_ALLOW_MSG3",			kScene.m_iButtonEventAllowMsg3,				-10 );
			LUA_GET_VALUE( luaManager, "DIALOG_CREATE_ALLOW",				kScene.m_bDialogCreateAllow,				false );
			LUA_GET_VALUE( luaManager, "GFX_UI_NO",							kScene.m_iGFxUINo,							0 );
			LUA_GET_VALUE( luaManager, "USE_OFF_UI_IN_END_CHECK",			kScene.m_bUseOffUIInEndCheck,				false );
			LUA_GET_VALUE( luaManager, "USE_AUTO_GUIDE_STOP",				kScene.m_bUseAutoGuideStop,					false );
			LUA_GET_VALUE( luaManager, "REPLASE_GUIDE",						kScene.m_iReplaceGuideNo,					0 );
			LUA_GET_VALUE( luaManager, "USE_END_TIME",						kScene.m_bUseEndTime,						false );
			LUA_GET_VALUE( luaManager, "END_TIME",							kScene.m_fEndTime,							0.f );
			LUA_GET_VALUE( luaManager, "AUTO_NEXT_GUIDE",					kScene.m_bAutoNextGuide,					false );
			LUA_GET_VALUE( luaManager, "NEXT_GUIDE_NO",						kScene.m_iNextGuideNo,						0 );
			LUA_GET_VALUE( luaManager, "REPLAY_GUIDE",						kScene.m_bReplayGuide,						false );
			LUA_GET_VALUE( luaManager, "REPLAY_GUIDE_NO",					kScene.m_iReplayGuideNo,					0 );

			// June 31fc3912: std::map insert, so a repeated GUIDE_NO keeps the first row
			m_mapGuideScene.insert( std::make_pair( kScene.m_iGuideNo, kScene ) );

			++iIndex;
			luaManager.EndTable();
		}
		luaManager.EndTable();
	}
	luaManager.EndTable();

#ifdef SERV_IRUHADEV_OFFLINE
	CX2OfflineLog::Server( L"JUNE_FX C3z-4t load %d scene(s) from TutorialGuideInfo.lua", (int)m_mapGuideScene.size() );
#endif SERV_IRUHADEV_OFFLINE
	return true;
}

HRESULT CX2TutorialGuideManager::OnFrameMove( double fTime, float fElapsedTime )
{
	CKTDIDevice* pKeyboard = g_pKTDXApp->GetDIManager()->Getkeyboard();
	if( TRUE == pKeyboard->GetKeyState( DIK_F2 ) )
	{
		pKeyboard->SetLock( DIK_F2, FALSE );
		m_bGuideOffByF2 = g_pMain->GetGameOption().GetOptionList().m_bPlayGuide;	// June g_pMain+0x1cd
	}

	if( true == IsGuideRunning() && true == m_bGuideOffByF2 )
	{
		AbortGuide();
		return S_OK;
	}

	if( GS_WAIT_INPUT == m_iGuideState && true == CheckAllowedKeyPressed() )
		EndGuideStep();

	CheckCameraArrive();
	OnFrameMoveKeyLimitTime( fElapsedTime );
	OnFrameMoveStartTime( fElapsedTime );

	if( true == m_bEndTimeRunning )
	{
		m_fGuideTime += fElapsedTime;
		if( m_fGuideTime > m_NowGuideScene.m_fEndTime )
		{
			FinishGuideStep();
			m_fGuideTime = 0.f;
			m_bEndTimeRunning = false;
		}
	}

	OnFrameMoveInputGuideKey( fElapsedTime );
	OnFrameMoveInputGuideStep( fElapsedTime );
	return S_OK;
}

float CX2TutorialGuideManager::CheckEventTriggerNPCPosX( int iNPCID )
{
	if( NULL == g_pX2Game )
		return NO_EVENT_NPC_DISTANCE;

	CX2GUUser* pMyUnit = g_pX2Game->GetMyUnit();
	CX2GUNPC* pNPC = g_pX2Game->GetNPCUnitByType( iNPCID );
	if( NULL == pMyUnit || NULL == pNPC )
		return NO_EVENT_NPC_DISTANCE;

	return pNPC->GetPos( false ).x - pMyUnit->GetPos( false ).x;
}

float CX2TutorialGuideManager::CheckEventTriggerNPCPos( int iNPCID )
{
	if( NULL == g_pX2Game )
		return NO_EVENT_NPC_DISTANCE;

	CX2GUUser* pMyUnit = g_pX2Game->GetMyUnit();
	CX2GUNPC* pNPC = g_pX2Game->GetNPCUnitByType( iNPCID );
	if( NULL == pMyUnit || NULL == pNPC )
		return NO_EVENT_NPC_DISTANCE;

	return GetDistance( pMyUnit->GetPos( false ), pNPC->GetPos( false ) );
}

void CX2TutorialGuideManager::HideInputGuideUI()
{
	if( NULL != m_pDLGInputGuide )
	{
		m_bInputGuideHidden = true;
		m_fInputGuideHideTime = INPUT_GUIDE_HIDE_TIME;
	}
}

bool CX2TutorialGuideManager::CheckAndResetInputAlarm( float fAlarmTime, float fResetTime )
{
	if( m_fInputAlarmTime > fAlarmTime )
	{
		m_fInputAlarmTime = fResetTime;
		return true;
	}
	return false;
}

bool CX2TutorialGuideManager::CheckToNeedTutorialGuide()
{
	if( NULL != g_pData->GetQuestManager() && true == g_pData->GetQuestManager()->GetUnitCompleteQuest( QUEST_ID_VISIT_RUBEN ) )
		return false;

	CX2User* pMyUser = g_pData->GetMyUser();
	CX2Unit* pSelectUnit = pMyUser->GetSelectUnit();
	for( int i = 0; i < pMyUser->GetUnitNum(); ++i )
	{
		CX2Unit* pUnit = pMyUser->GetUnitByIndex( i );
		if( pSelectUnit == pUnit )
			continue;

		// June 31fbdee2: another character of the same unit type at level 10+ means no guide
		const int iUnitType = ( NULL != pUnit->GetUnitTemplet() ? (int)pUnit->GetUnitTemplet()->m_UnitType : 0 );
		const int iSelectUnitType = ( NULL != pSelectUnit->GetUnitTemplet() ? (int)pSelectUnit->GetUnitTemplet()->m_UnitType : 0 );
		if( iSelectUnitType == iUnitType && (int)pUnit->GetUnitData().m_Level >= ACCOUNT_HIGH_LEVEL )
			return false;
	}
	return true;
}

void CX2TutorialGuideManager::OnBeginGuide()
{
	if( GS_RUNNING != m_iGuideState )
		return;

	if( true == m_NowGuideScene.m_bUseCameraMove )
	{
		m_iGuideState = GS_WAIT_EVENT_NPC;
		WaitEventNPC();
		return;
	}

	if( NULL != m_pGuideStartBlocker && true == m_pGuideStartBlocker->IsBlockingGuideStart() )
		return;

	PlayGuideStep();
}

void CX2TutorialGuideManager::OnBeginGuideNo( int iGuideNo )
{
	BeginGuideScene( iGuideNo );
}

void CX2TutorialGuideManager::OnBeginGuideByGame()
{
	OnBeginGuide();		// June 31fc5e30 is 31fc6320's twin
}

void CX2TutorialGuideManager::OnBeginGuideWhenNPCLoaded()
{
	if( false == m_NowGuideScene.m_bNeedNPCLoaded || GS_RUNNING != m_iGuideState )
		return;

	OnBeginGuide();
}

void CX2TutorialGuideManager::StopGuide()
{
	m_iReservedGuideNo = 0;
	EndGuide();
}

void CX2TutorialGuideManager::BeginReplayGuide()
{
	if( true == m_bReplayGuideReserved )
		BeginGuideScene( m_iReplayGuideNo );

	m_iReplayGuideNo = 0;
	m_bReplayGuideReserved = false;
}

void CX2TutorialGuideManager::OnExitTutorial()
{
	SAFE_DELETE_DIALOG( m_pDLGInputGuide );
	SAFE_DELETE_DIALOG( m_pDLGInputSideGuide );
	m_iInputGuideStep = -1;
	ResetInputGuideState();
}

void CX2TutorialGuideManager::SetShowInputGuideUI( bool bShow )
{
	if( NULL != m_pDLGInputGuide && false == m_bInputGuideHidden )
		m_pDLGInputGuide->SetShow( bShow );

	if( NULL != m_pDLGInputSideGuide )
		m_pDLGInputSideGuide->SetShow( bShow );
}

bool CX2TutorialGuideManager::SetGuideStartState()
{
	if( true == m_NowGuideScene.m_bWait )
	{
		m_iGuideState = GS_RUNNING;
		return true;
	}

	if( true == m_NowGuideScene.m_bUseStartTime )
	{
		m_iGuideState = GS_RUNNING;
		m_bStartTimeRunning = true;
		return true;
	}

	if( NULL != m_pGuideStartBlocker && true == m_pGuideStartBlocker->IsBlockingGuideStart() )
	{
		m_iGuideState = GS_RUNNING;
		return true;
	}

	if( true == m_NowGuideScene.m_bUseCameraMove )
	{
		m_iGuideState = GS_WAIT_EVENT_NPC;
		return true;
	}

	if( true == m_NowGuideScene.m_bClickWait )
	{
		m_iGuideState = GS_WAIT_CLICK;
		return true;
	}

	return false;
}

bool CX2TutorialGuideManager::CanBeginGuide( bool bNotInDungeon, bool bHighPriority, int iConditionState )
{
	if( true == m_bSkipNextGuide )
	{
		m_bSkipNextGuide = false;
		return false;
	}

	if( m_iLastCustomEventMsg == m_NowGuideScene.m_iCustomEventMsg )
		return false;

	// June 31fc4530: the level blocks apply outside a dungeon only
	if( NULL == g_pX2Game || CX2Game::GT_DUNGEON != g_pX2Game->GetGameType() )
	{
		if( NULL == g_pData->GetMyUser() || NULL == g_pData->GetMyUser()->GetSelectUnit() )
			return false;

		if( (int)g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level > BEGIN_GUIDE_MAX_LEVEL )
		{
#ifdef SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG
			CX2OfflineLog::Server( L"JUNE_FX C3z-4t refuse: unit level above %d outside a dungeon (June 31fc4530)", BEGIN_GUIDE_MAX_LEVEL );
#endif SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG
			return false;
		}

		CheckAccountHasHighLevelUnit();
		if( true == m_bAccountHasHighLevelUnit )
		{
#ifdef SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG
			CX2OfflineLog::Server( L"JUNE_FX C3z-4t refuse: a character on the account is level %d+ (June 31fbdf40, kept verbatim)", ACCOUNT_HIGH_LEVEL );
#endif SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG
			return false;
		}
	}

	if( true == m_bGuideOffByF2 )
		return false;

	if( GS_NONE != m_iGuideState && false == bHighPriority )
		return false;

	if( true == bNotInDungeon && NULL != g_pX2Game && CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() )
		return false;

	CX2PartyManager* pPartyManager = g_pData->GetPartyManager();
	if( true == pPartyManager->DoIHaveParty() && false == pPartyManager->AmIPartyLeader() )
		return false;

	// stand-in: June also blocks on party manager +0x8a, a June-only flag with no March member
	if( true == pPartyManager->GetProcessDungeonMatch() )
		return false;

	if( 1 == iConditionState )		// TutorialGuideInfo.lua SC_SHOW_QUEST_UI
	{
		CX2UIQuestNew* pUIQuestNew = g_pData->GetUIManager()->GetUIQuestNew();
		if( NULL == pUIQuestNew || false == pUIQuestNew->m_bUserShowQuickQuestDLG )
			return false;
	}
	return true;
}

void CX2TutorialGuideManager::CheckAccountHasHighLevelUnit()
{
	CX2User* pMyUser = g_pData->GetMyUser();
	if( NULL == pMyUser )
		return;

	for( int i = 0; i < pMyUser->GetUnitNum(); ++i )
	{
		CX2Unit* pUnit = pMyUser->GetUnitByIndex( i );
		if( NULL != pUnit && (int)pUnit->GetUnitData().m_Level >= ACCOUNT_HIGH_LEVEL )
		{
			m_bAccountHasHighLevelUnit = true;	// June never clears it again
			return;
		}
	}
}

void CX2TutorialGuideManager::PlayGuideStep()
{
#ifdef SERV_IRUHADEV_JUNE_GFX_MANAGER
	CKTDGGFXManager* pGFXManager = g_pKTDXApp->GetGFXManager();
	if( NULL != pGFXManager )
	{
		pGFXManager->moveToTopZorder( 6 );		// June GFx UI id, no Enum entry
		if( true == pGFXManager->isOpenUI( 8 ) )
			pGFXManager->unloadWidget( 8 );
	}
#endif SERV_IRUHADEV_JUNE_GFX_MANAGER

	CKTDIDevice* pKeyboard = g_pKTDXApp->GetDIManager()->Getkeyboard();
	pKeyboard->SetUseAllowedKeyList( true );
	pKeyboard->AddAllowedKey( DIK_ESCAPE );
	pKeyboard->AddAllowedKey( DIK_F2 );

	if( true == m_NowGuideScene.m_bUseLimitZoom )
	{
		g_pMain->GetGameOption().GetOptionList().m_iZoomLevel = -1;	// June g_pMain+0x1d0
		// stand-in: June also sets g_pMain+0x3f5 ( zoom lock, June-only, read by the zoom key handler 3245a8f0, not ported )
	}

	if( true == m_NowGuideScene.m_bFixSkillTypeA && NULL != g_pData && NULL != g_pData->GetUIManager() &&
		NULL != g_pData->GetUIManager()->GetUISkillTree() )
	{
		g_pData->GetUIManager()->GetUISkillTree()->SetSkillUIType( true );
	}

	// stand-in: CUSTOM_EVENT_MSG 122 in a guild sets CX2StateMenu +0x43d ( June-only rest-EXP gauge mode, not ported )

	if( true == m_NowGuideScene.m_bFirstGuide )
	{
		CX2State* pState = dynamic_cast<CX2State*>( g_pMain->GetNowState() );
		if( NULL != pState )
		{
			pState->MoveMouseClickUIToTop();
			CX2StateMenu* pStateMenu = dynamic_cast<CX2StateMenu*>( pState );	// June CX2State vslot 0xa4: only CX2StateMenu overrides it
			if( NULL != pStateMenu )
				pStateMenu->ToggleSystemMenu( false );
#ifdef SERV_IRUHADEV_JUNE_GFX_MANAGER
			if( NULL != g_pKTDXApp->GetGFXManager() )
				g_pKTDXApp->GetGFXManager()->unloadWidget( 3 );		// June GFx UI id, no Enum entry
#endif SERV_IRUHADEV_JUNE_GFX_MANAGER
			// stand-in: June CX2State vslot 0xb0( false ) has no March counterpart ( the field override 3245c560 hides a gauge )
			pState->SetShowMouseClickUI( false, (CX2State::MouseClickUI::HIDE_REASON)3 );	// June-only hide reason 3, no Enum entry
		}
		g_pData->GetUIManager()->CloseAll();
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ClearAllOKAndCancelMsgDlg();
	}

	CX2StateMenu* pStateMenu = dynamic_cast<CX2StateMenu*>( g_pMain->GetNowState() );
	if( NULL != pStateMenu )
		pStateMenu->ToggleSystemMenu( false );

	if( NULL != g_pData->GetPremiumBuffManager() )
	{
		g_pData->GetPremiumBuffManager()->SetFirstComeBack( false );
		g_pData->GetPremiumBuffManager()->SetShowFirstComBackUI( false );
	}

	if( NULL != g_pTFieldGame && true == g_pTFieldGame->GetJoinNpc() )
		g_pTFieldGame->SetShowNpcMessage( false );

	// stand-in: June writes the dialog manager's +0x111 here ( DIALOG_CREATE_ALLOW; June-only, its quest-reward reader is t3 )

	if( false == m_NowGuideScene.m_bNoStopFrame )
		StopGameFrame();

	if( 1 == m_NowGuideScene.m_iOpenUIWithGuideStart )		// TutorialGuideInfo.lua OPEN_UI_INVEN
	{
		g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_INVEN );
		if( 0 != m_NowGuideScene.m_iChangeInventoryTab && NULL != g_pData->GetUIManager()->GetUIInventory() )
			g_pData->GetUIManager()->GetUIInventory()->SetInventorySort( (CX2Inventory::SORT_TYPE)m_NowGuideScene.m_iChangeInventoryTab );
	}

	SetShowQuickSlotByGuide();

	if( NULL != m_pTutorialGuideAdapter )
		ShowGuideMovie( m_NowGuideScene.m_iGFxUINo );

	if( true == m_NowGuideScene.m_bUseChangeMPValue && NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
		g_pX2Game->GetMyUnit()->SetNowMp( m_NowGuideScene.m_fChangeMPValue, false );

#ifdef SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG
	CX2OfflineLog::Server( L"JUNE_FX C3z-4t step %d: gfx %d, key limit %d, stop frame %d", m_NowGuideScene.m_iGuideNo, m_NowGuideScene.m_iGFxUINo,
		m_NowGuideScene.m_bUseKeyLimitInGuide ? 1 : 0, m_NowGuideScene.m_bNoStopFrame ? 0 : 1 );
#endif SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG

	if( true == m_NowGuideScene.m_bUseKeyLimitInGuide )
	{
		m_bKeyLimitTimeRunning = true;
		m_iGuideState = GS_KEY_LIMIT;
		return;
	}

	m_iGuideState = GS_WAIT_INPUT;
	if( true == m_NowGuideScene.m_bAnyKeyAllow )
		pKeyboard->SetUseAllowedKeyList( false );
}

void CX2TutorialGuideManager::EndGuide()
{
#ifdef SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG
	if( GS_NONE != m_iGuideState )
		CX2OfflineLog::Server( L"JUNE_FX C3z-4t end %d", m_NowGuideScene.m_iGuideNo );
#endif SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG

	ResumeGameFrame();

	CX2State* pState = dynamic_cast<CX2State*>( g_pMain->GetNowState() );
	if( NULL != pState )
		pState->SetShowMouseClickUI( true, (CX2State::MouseClickUI::HIDE_REASON)3 );	// June-only hide reason 3, no Enum entry

	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetUseAllowedKeyList( false );
	// stand-in: June clears g_pMain+0x3f5 here ( the June-only zoom lock )

	ResetFocusEventNPC();

	if( true == m_NowGuideScene.m_bFixSkillTypeA && NULL != g_pMain )
		g_pMain->GetGameOption().SetIsSkillUITypeA( g_pMain->GetGameOption().GetIsSkillUITypeA(), true );

	// stand-in: CUSTOM_EVENT_MSG 122 in a guild clears CX2StateMenu +0x43d ( June-only, not ported )

	m_NowGuideScene.Init();
	m_fGuideTime = 0.f;
	m_iGuideState = GS_NONE;
	m_bWaitCameraArrive = false;
	m_bEndTimeRunning = false;
	m_bStartTimeRunning = false;
	m_bKeyLimitTimeRunning = false;

	if( NULL != m_pTutorialGuideAdapter )
		HideGuideMovie();

	m_iLastCustomEventMsg = LAST_CUSTOM_EVENT_NONE;
	// stand-in: June clears the dialog manager's +0x111 here ( June-only, t3 )
}

void CX2TutorialGuideManager::WaitEventNPC()
{
	if( GS_WAIT_EVENT_NPC != m_iGuideState )
		return;

	if( NULL != g_pX2Game )
	{
		CX2GUNPC* pNPC = g_pX2Game->GetNPCUnitByType( m_NowGuideScene.m_iCameraTargetNPCID );
		if( NULL != pNPC )
		{
			pNPC->SetFocusCameraForce( true );
			m_bWaitCameraArrive = true;
			m_iGuideState = GS_WAIT_EVENT_NPC;
			return;
		}

		if( true == m_NowGuideScene.m_bNeedNPCLoaded )
		{
			m_iGuideState = GS_RUNNING;
			return;
		}
	}
	EndGuide();
}

void CX2TutorialGuideManager::CheckCameraArrive()
{
	if( true != m_bWaitCameraArrive )
		return;

	if( NULL == g_pX2Game )
	{
		EndGuide();
		return;
	}

	CX2GameUnit* pFocusUnit = g_pX2Game->GetFocusUnit();
	if( NULL == pFocusUnit || pFocusUnit == g_pX2Game->GetMyUnit() )
		return;

	const D3DXVECTOR3& vFocusPos = pFocusUnit->GetPos( false );
	CX2Camera* pCamera = g_pX2Game->GetX2Camera();
	if( NULL != pCamera && fabs( vFocusPos.x - pCamera->GetCamera().GetLookAt().x ) < CAMERA_ARRIVE_DISTANCE )
	{
		m_bWaitCameraArrive = false;
		PlayGuideStep();
	}
}

void CX2TutorialGuideManager::BeginGuideScene( int iGuideNo )
{
	std::map< int, GuideScene >::const_iterator mit = m_mapGuideScene.find( iGuideNo );
	if( m_mapGuideScene.end() == mit )
		return;

	const GuideScene& kScene = mit->second;
	if( false == CanBeginGuide( false, 2 == kScene.m_iPriorityLevel, kScene.m_iConditionState ) )		// TutorialGuideInfo.lua PRIORITY_HIGH
		return;

	if( 2 == kScene.m_iPriorityLevel && GS_NONE != m_iGuideState )		// TutorialGuideInfo.lua PRIORITY_HIGH
		EndGuide();

	m_NowGuideScene = kScene;
#ifdef SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG
	CX2OfflineLog::Server( L"JUNE_FX C3z-4t begin %d", iGuideNo );
#endif SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG

	if( true == SetGuideStartState() )
	{
		WaitEventNPC();
		return;
	}
	PlayGuideStep();
}

void CX2TutorialGuideManager::AbortGuide()
{
	bool bReserved = false;
	int iNextGuideNo = 0;
	if( 0 != m_iReservedGuideNo )
	{
		std::map< int, GuideScene >::const_iterator mit = m_mapGuideScene.find( m_iReservedGuideNo );
		GuideScene kReservedScene;
		if( m_mapGuideScene.end() != mit )
			kReservedScene = mit->second;

		bReserved = true;
		m_iReservedGuideNo = 0;
		iNextGuideNo = kReservedScene.m_iReplaceGuideNo;	// June 31fc5fe5 reads the reserved scene's +0x80
	}

	EndGuide();
	if( true == bReserved )
		BeginGuideScene( iNextGuideNo );
}

void CX2TutorialGuideManager::FinishGuideStep()
{
	bool bNext = m_NowGuideScene.m_bAutoNextGuide;
	int iNextGuideNo = m_NowGuideScene.m_iNextGuideNo;
	if( false == bNext && 0 != m_iReservedGuideNo )
	{
		std::map< int, GuideScene >::const_iterator mit = m_mapGuideScene.find( m_iReservedGuideNo );
		GuideScene kReservedScene;
		if( m_mapGuideScene.end() != mit )
			kReservedScene = mit->second;

		bNext = true;
		m_iReservedGuideNo = 0;
		iNextGuideNo = kReservedScene.m_iReplaceGuideNo;	// June 31fc5f2a reads the reserved scene's +0x80
	}

	m_bReplayGuideReserved = false;
	m_iReplayGuideNo = 0;
	if( true == m_NowGuideScene.m_bReplayGuide )
	{
		m_bReplayGuideReserved = true;
		m_iReplayGuideNo = m_NowGuideScene.m_iReplayGuideNo;
	}

	EndGuide();
	if( true == bNext )
		BeginGuideScene( iNextGuideNo );
}

void CX2TutorialGuideManager::EndGuideStep()
{
	ResumeGameFrame();
	if( true == m_NowGuideScene.m_bUseOffUIInEndCheck )
	{
		if( NULL != m_pTutorialGuideAdapter )
			HideGuideMovie();
		ResetFocusEventNPC();
	}

	if( 1 == m_NowGuideScene.m_iOpenUIWithGuideStart )		// TutorialGuideInfo.lua OPEN_UI_INVEN
		g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_INVEN );

	if( true == m_NowGuideScene.m_bUseEndTime )
	{
		m_iGuideState = GS_WAIT_END_TIME;
		m_bEndTimeRunning = true;
		return;
	}
	FinishGuideStep();
}

void CX2TutorialGuideManager::EndGuideStepWithCustomEvent()
{
	if( 0 < m_NowGuideScene.m_iCustomEventMsg && NULL != g_pMain->GetNowState() )
		g_pMain->GetNowState()->MsgProc( 0, CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_NowGuideScene.m_iCustomEventMsg, 0 );

	EndGuideStep();
}

bool CX2TutorialGuideManager::CheckAllowedKeyPressed()
{
	CKTDIManager* pDIManager = g_pKTDXApp->GetDIManager();
	if( TRUE == pDIManager->Getkeyboard()->GetKeyState( DIK_ESCAPE ) )
	{
		AbortGuide();
		return false;
	}

	if( true == m_NowGuideScene.m_bDirKeyAllow )
	{
		if( TRUE == pDIManager->GetActionState( GA_LEFT ) || TRUE == pDIManager->GetActionState( GA_RIGHT ) ||
			TRUE == pDIManager->GetActionState( GA_UP ) || TRUE == pDIManager->GetActionState( GA_DOWN ) )
			return true;
		return false;
	}

	if( true == m_NowGuideScene.m_bAttackKeyAllow )
	{
		if( TRUE == pDIManager->GetActionState( GA_ATTACK_FAST ) )
		{
			pDIManager->SetLock( GA_ATTACK_FAST, FALSE );
			return true;
		}
		if( TRUE == pDIManager->GetActionState( GA_ATTACK_STRONG ) )
		{
			pDIManager->SetLock( GA_ATTACK_STRONG, FALSE );
			return true;
		}
		return false;
	}

	if( true == m_NowGuideScene.m_bSkillKeyAllow )
	{
		if( TRUE == pDIManager->GetActionState( GA_SKILL1 ) )
		{
			pDIManager->SetLock( GA_SKILL1, FALSE );
			return true;
		}
		if( TRUE == pDIManager->GetActionState( GA_SKILL4 ) )
		{
			pDIManager->SetLock( GA_SKILL4, FALSE );
			return true;
		}
		return false;
	}

	if( true == m_NowGuideScene.m_bAwakeKeyAllow )
	{
		if( TRUE != pDIManager->GetActionState( GA_AWAKE ) )
			return false;

		pDIManager->SetLock( GA_AWAKE, FALSE );
		if( NULL == g_pX2Game || NULL == g_pX2Game->GetMyUnit() )
			return true;

		// June 31fc7be0 ( n = 0 ): add n orbs and awaken my unit
		CX2GUUser* pMyUnit = g_pX2Game->GetMyUnit();
		if( true == pMyUnit->IsMyUnit() )
		{
			pMyUnit->SetHyperModeCount( pMyUnit->GetHyperModeCount() + 0 );
			pMyUnit->StateChange( CX2GUUser::USI_HYPER_MODE, false );
		}
		return true;
	}
	return false;
}

void CX2TutorialGuideManager::OnFrameMoveKeyLimitTime( float fElapsedTime )
{
	if( false == m_bKeyLimitTimeRunning )
		return;

	m_fGuideTime += fElapsedTime;
	if( m_fGuideTime > m_NowGuideScene.m_fKeyLimitTime )
	{
		m_fGuideTime = 0.f;
		m_bKeyLimitTimeRunning = false;
		if( true == m_NowGuideScene.m_bUseAutoGuideStop )
		{
			EndGuideStepWithCustomEvent();
			return;
		}

		m_iGuideState = GS_WAIT_INPUT;
		if( true == m_NowGuideScene.m_bAnyKeyAllow )
			g_pKTDXApp->GetDIManager()->Getkeyboard()->SetUseAllowedKeyList( false );
	}
}

void CX2TutorialGuideManager::OnFrameMoveStartTime( float fElapsedTime )
{
	if( false == m_bStartTimeRunning )
		return;

	m_fGuideTime += fElapsedTime;
	if( m_fGuideTime > m_NowGuideScene.m_fStartTime )
	{
		if( GS_RUNNING == m_iGuideState )
		{
			if( true == m_NowGuideScene.m_bUseCameraMove )
			{
				m_iGuideState = GS_WAIT_EVENT_NPC;
				WaitEventNPC();
				m_fGuideTime = 0.f;
				m_bStartTimeRunning = false;
				return;
			}

			if( NULL == m_pGuideStartBlocker || true != m_pGuideStartBlocker->IsBlockingGuideStart() )
				PlayGuideStep();
		}
		m_bStartTimeRunning = false;
		m_fGuideTime = 0.f;
	}
}

void CX2TutorialGuideManager::ResetFocusEventNPC()
{
	if( NULL == g_pX2Game )
		return;

	CX2GameUnit* pFocusUnit = g_pX2Game->GetFocusUnit();
	if( NULL != pFocusUnit && pFocusUnit == g_pX2Game->GetMyUnit() )
		return;

	CX2GUNPC* pNPC = dynamic_cast<CX2GUNPC*>( pFocusUnit );
	if( NULL != pNPC )
	{
		pNPC->SetFocusCameraForce( false );
		g_pX2Game->InitFocusUnit();		// June CX2Game vslot 0x8c
	}
}

void CX2TutorialGuideManager::StopGameFrame()
{
	// stand-in: June 322cc020( true ) writes CX2Game +0x2d8, a June-only byte with no reader found in June
	if( NULL != g_pTFieldGame )
		g_pTFieldGame->SetEnableKeyProcess( false );	// June g_pTFieldGame +0x12c = 0
}

void CX2TutorialGuideManager::ResumeGameFrame()
{
	// stand-in: June 322cc020( false ) writes CX2Game +0x2d8, a June-only byte with no reader found in June
	if( NULL != g_pTFieldGame )
		g_pTFieldGame->SetEnableKeyProcess( true );		// June g_pTFieldGame +0x12c = 1
}

void CX2TutorialGuideManager::SetShowQuickSlotByGuide()
{
	if( NULL == g_pData || NULL == g_pData->GetUIManager() )
		return;

	// stand-in: June also writes the now-state's +0x378 the same way ( member not identified )
	g_pData->GetUIManager()->SetShowQuickSlot( true != g_pMain->GetIsPlayingTutorial() );
}

void CX2TutorialGuideManager::ResetInputGuideState()
{
	m_fInputGuideBlinkTime = 0.f;
	m_fInputGuideBlinkTimeSub = INPUT_GUIDE_BLINK_SUB_START;
	m_fDashGuideTime = 0.f;
	m_bDashGuideDone = false;
	m_fInputGuideHideTime = 0.f;
	m_mapMoveGuideKeyPressed[1] = false;	// June 31fc3990: the move guide's four arrow pictures 1 / 3 / 5 / 7
	m_mapMoveGuideKeyPressed[3] = false;
	m_mapMoveGuideKeyPressed[5] = false;
	m_mapMoveGuideKeyPressed[7] = false;
	if( 1 != m_iInputGuideStep )		// INPUT_GUIDE_STEP IGS_DASH keeps its alarm
		m_fInputAlarmTime = 0.f;
}

void CX2TutorialGuideManager::ShowInputGuideUI( int iInputGuideStep )
{
	if( NULL == m_pDLGInputGuide )
	{
		m_pDLGInputGuide = new CKTDGUIDialog( NULL, L"DLG_UI_Tutorial_Input_Guide.lua", INPUT_GUIDE_DIALOG_LAYER, 0 );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGInputGuide );
	}

	CKTDGUIStatic* pStaticMove = static_cast<CKTDGUIStatic*>( m_pDLGInputGuide->GetControl( L"g_pStaticMoveGuide" ) );
	CKTDGUIStatic* pStaticDash = static_cast<CKTDGUIStatic*>( m_pDLGInputGuide->GetControl( L"g_pStaticDashGuide" ) );
	CKTDGUIStatic* pStaticAttack = static_cast<CKTDGUIStatic*>( m_pDLGInputGuide->GetControl( L"g_pStaticAttackGuide" ) );
	CKTDGUIStatic* pStaticSkill = static_cast<CKTDGUIStatic*>( m_pDLGInputGuide->GetControl( L"g_pStaticSkillGuide" ) );
	CKTDGUIStatic* pStaticAwake = static_cast<CKTDGUIStatic*>( m_pDLGInputGuide->GetControl( L"g_pStaticAwakeGuide" ) );
	if( NULL == pStaticMove || NULL == pStaticDash || NULL == pStaticAttack || NULL == pStaticSkill || NULL == pStaticAwake )
		return;

	if( -1 != m_iInputGuideStep && true == m_pDLGInputGuide->GetShow() )
	{
		if( m_fInputGuideHideTime <= 0.f )
			HideInputGuideUI();
		m_iReservedInputGuideStep = iInputGuideStep;
		return;
	}

	m_bInputGuideHidden = false;
	m_pDLGInputGuide->SetShow( true );
	m_pDLGInputGuide->SetColor( D3DXCOLOR( 1.f, 1.f, 1.f, 1.f ) );

	switch( iInputGuideStep )
	{
	case 0:		// INPUT_GUIDE_STEP IGS_MOVE
		pStaticMove->SetShow( true );	pStaticDash->SetShow( false );	pStaticAttack->SetShow( false );	pStaticSkill->SetShow( false );	pStaticAwake->SetShow( false );
		break;
	case 1:		// IGS_DASH
		pStaticMove->SetShow( false );	pStaticDash->SetShow( true );	pStaticAttack->SetShow( false );	pStaticSkill->SetShow( false );	pStaticAwake->SetShow( false );
		break;
	case 2:		// IGS_ATTACK
		pStaticMove->SetShow( false );	pStaticDash->SetShow( false );	pStaticAttack->SetShow( true );	pStaticSkill->SetShow( false );	pStaticAwake->SetShow( false );
		break;
	case 3:		// IGS_SKILL
		pStaticMove->SetShow( false );	pStaticDash->SetShow( false );	pStaticAttack->SetShow( false );	pStaticSkill->SetShow( true );	pStaticAwake->SetShow( false );
		break;
	case 4:		// IGS_AWAKE
		pStaticMove->SetShow( false );	pStaticDash->SetShow( false );	pStaticAttack->SetShow( false );	pStaticSkill->SetShow( false );	pStaticAwake->SetShow( true );
		break;
	default:
		m_bInputGuideHidden = true;
		break;
	}

	if( false == m_bInputGuideHidden )
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Tuto_Ui_Notice01.ogg", false, false );

	ShowInputSideGuideUI( m_iInputGuideStep );
	m_iInputGuideStep = iInputGuideStep;
	ResetInputGuideState();
}

void CX2TutorialGuideManager::ShowInputSideGuideUI( int iInputGuideStep )
{
	if( NULL == m_pDLGInputSideGuide )
	{
		m_pDLGInputSideGuide = new CKTDGUIDialog( NULL, L"DLG_UI_Tutorial_Input_Side_Guide.lua", INPUT_GUIDE_DIALOG_LAYER, 0 );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGInputSideGuide );
	}

	CKTDGUIStatic* pStaticMove = static_cast<CKTDGUIStatic*>( m_pDLGInputSideGuide->GetControl( L"g_pStaticMoveGuide_Side" ) );
	CKTDGUIStatic* pStaticDash = static_cast<CKTDGUIStatic*>( m_pDLGInputSideGuide->GetControl( L"g_pStaticDashGuide_Side" ) );
	CKTDGUIStatic* pStaticAttack = static_cast<CKTDGUIStatic*>( m_pDLGInputSideGuide->GetControl( L"g_pStaticAttackGuide_Side" ) );
	CKTDGUIStatic* pStaticSkill = static_cast<CKTDGUIStatic*>( m_pDLGInputSideGuide->GetControl( L"g_pStaticSkillGuide_Side" ) );
	CKTDGUIStatic* pStaticAwake = static_cast<CKTDGUIStatic*>( m_pDLGInputSideGuide->GetControl( L"g_pStaticAwakeGuide_Side" ) );
	CKTDGUIStatic* pStaticESC = static_cast<CKTDGUIStatic*>( m_pDLGInputSideGuide->GetControl( L"g_pStaticESCGuide_Side" ) );
	if( NULL == pStaticMove || NULL == pStaticDash || NULL == pStaticAttack || NULL == pStaticSkill || NULL == pStaticAwake || NULL == pStaticESC )
		return;

	// June 31fc46d0: the side list shows the steps already passed ( the step before the new one and all earlier )
	switch( iInputGuideStep )
	{
	case 0:		// INPUT_GUIDE_STEP IGS_MOVE
		pStaticMove->SetShow( false );	pStaticDash->SetShow( false );	pStaticAttack->SetShow( false );	pStaticSkill->SetShow( false );	pStaticAwake->SetShow( false );
		break;
	case 1:		// IGS_DASH
		pStaticMove->SetShow( true );	pStaticDash->SetShow( false );	pStaticAttack->SetShow( false );	pStaticSkill->SetShow( false );	pStaticAwake->SetShow( false );
		break;
	case 2:		// IGS_ATTACK
		pStaticMove->SetShow( true );	pStaticDash->SetShow( true );	pStaticAttack->SetShow( false );	pStaticSkill->SetShow( false );	pStaticAwake->SetShow( false );
		break;
	case 3:		// IGS_SKILL
		pStaticMove->SetShow( true );	pStaticDash->SetShow( true );	pStaticAttack->SetShow( true );	pStaticSkill->SetShow( false );	pStaticAwake->SetShow( false );
		break;
	case 4:		// IGS_AWAKE
		pStaticMove->SetShow( true );	pStaticDash->SetShow( true );	pStaticAttack->SetShow( true );	pStaticSkill->SetShow( true );	pStaticAwake->SetShow( false );
		break;
	default:
		pStaticESC->SetShow( false );
		pStaticMove->SetShow( false );	pStaticDash->SetShow( false );	pStaticAttack->SetShow( false );	pStaticSkill->SetShow( false );	pStaticAwake->SetShow( false );
		break;
	}
}

void CX2TutorialGuideManager::BlinkInputGuideKey( CKTDGUIStatic* pStatic, int iPictureIndex, float* pfBlinkTime, float fElapsedTime )
{
	if( NULL == pStatic )
		return;

	CKTDGUIControl::CPictureData* pPictureUp = pStatic->GetPicture( iPictureIndex );
	CKTDGUIControl::CPictureData* pPictureDown = pStatic->GetPicture( iPictureIndex + 1 );
	if( NULL == pPictureUp || NULL == pPictureDown )
		return;

	*pfBlinkTime += fElapsedTime;
	if( true == pPictureUp->GetShow() )
	{
		if( *pfBlinkTime > INPUT_GUIDE_BLINK_ON_TIME )
		{
			pPictureUp->SetShow( false );
			pPictureDown->SetShow( true );
			*pfBlinkTime = 0.f;
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Tuto_Ui_Notice01.ogg", false, false );
		}
	}
	else if( *pfBlinkTime > INPUT_GUIDE_BLINK_OFF_TIME )
	{
		pPictureUp->SetShow( true );
		pPictureDown->SetShow( false );
		*pfBlinkTime = 0.f;
	}
}

void CX2TutorialGuideManager::SetInputGuideKeyPressed( CKTDGUIStatic* pStatic, int iPictureIndex, bool bPressed, float fElapsedTime )
{
	if( NULL == pStatic )
		return;

	if( 8 != iPictureIndex )	// June 31fc4960: picture 8 is the dash guide's arrow run
	{
		CKTDGUIControl::CPictureData* pPictureUp = pStatic->GetPicture( iPictureIndex );
		CKTDGUIControl::CPictureData* pPictureDown = pStatic->GetPicture( iPictureIndex + 1 );
		if( NULL == pPictureUp || NULL == pPictureDown )
			return;

		if( true != bPressed )
		{
			pPictureUp->SetShow( true );
			pPictureDown->SetShow( false );
			return;
		}

		pPictureUp->SetShow( false );
		pPictureDown->SetShow( true );
		std::map< int, bool >::iterator mit = m_mapMoveGuideKeyPressed.find( iPictureIndex );
		if( m_mapMoveGuideKeyPressed.end() != mit )
			mit->second = true;
		return;
	}

	CKTDGUIControl::CPictureData* pKeyUp = pStatic->GetPicture( 2 );
	CKTDGUIControl::CPictureData* pKeyDown = pStatic->GetPicture( 3 );
	CKTDGUIControl::CPictureData* pArrow1 = pStatic->GetPicture( 5 );
	CKTDGUIControl::CPictureData* pArrow2 = pStatic->GetPicture( 6 );
	CKTDGUIControl::CPictureData* pArrow3 = pStatic->GetPicture( 7 );
	CKTDGUIControl::CPictureData* pArrow4 = pStatic->GetPicture( 8 );
	CKTDGUIControl::CPictureData* pDashOn1 = pStatic->GetPicture( 9 );
	CKTDGUIControl::CPictureData* pDashOn2 = pStatic->GetPicture( 10 );
	if( NULL == pKeyUp || NULL == pKeyDown || NULL == pArrow1 || NULL == pArrow2 || NULL == pArrow3 || NULL == pArrow4 ||
		NULL == pDashOn1 || NULL == pDashOn2 )
		return;

	CX2GUUser* pMyUnit = ( NULL != g_pX2Game ? g_pX2Game->GetMyUnit() : NULL );
	if( NULL != pMyUnit )
	{
		const int iStateID = pMyUnit->GetNowStateID();
		if( ( CX2GUUser::USI_DASH == iStateID || CX2GUUser::USI_DASH_JUMP == iStateID || CX2GUUser::USI_DASH_JUMP_LANDING == iStateID ) &&
			true == pMyUnit->GetIsRight() )
		{
			m_fDashGuideTime += fElapsedTime;
			if( false == pArrow1->GetShow() )
				pArrow1->SetShow( true );
			if( m_fDashGuideTime > DASH_GUIDE_ARROW2_TIME )
				pArrow2->SetShow( true );
			if( m_fDashGuideTime > DASH_GUIDE_ARROW3_TIME )
				pArrow3->SetShow( true );
			if( m_fDashGuideTime > DASH_GUIDE_ARROW4_TIME )
				pArrow4->SetShow( true );
			if( m_fDashGuideTime > DASH_GUIDE_DONE_TIME )
				m_bDashGuideDone = true;

			pKeyUp->SetShow( false );
			pKeyDown->SetShow( false );
			pDashOn1->SetShow( true );
			pDashOn2->SetShow( true );
			return;
		}
	}

	pArrow1->SetShow( false );
	pArrow2->SetShow( false );
	pArrow3->SetShow( false );
	pArrow4->SetShow( false );
	pKeyUp->SetShow( true );
	pKeyDown->SetShow( true );
	pDashOn1->SetShow( false );
	pDashOn2->SetShow( false );
	m_fDashGuideTime = 0.f;
}

void CX2TutorialGuideManager::OnFrameMoveInputGuideKey( float fElapsedTime )
{
	if( NULL != m_pDLGInputGuide && false == m_pDLGInputGuide->GetShow() )
		return;
	if( NULL == m_pDLGInputGuide )
		return;		// June 31fc5570 dereferences the dummy dialog here; nothing to blink without the dialog

	CKTDIManager* pDIManager = g_pKTDXApp->GetDIManager();
	switch( m_iInputGuideStep )
	{
	case 0:		// INPUT_GUIDE_STEP IGS_MOVE
		{
			CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDLGInputGuide->GetControl( L"g_pStaticMoveGuide" ) );
			if( NULL == pStatic )
				break;

			if( TRUE == pDIManager->GetPureActionState( GA_RIGHT ) )
			{
				m_fInputGuideBlinkTime = 0.f;
				SetInputGuideKeyPressed( pStatic, 7, true, 0.f );
			}
			else
				BlinkInputGuideKey( pStatic, 7, &m_fInputGuideBlinkTime, fElapsedTime );

			SetInputGuideKeyPressed( pStatic, 1, FALSE != pDIManager->GetPureActionState( GA_UP ), 0.f );
			SetInputGuideKeyPressed( pStatic, 5, FALSE != pDIManager->GetPureActionState( GA_LEFT ), 0.f );
			SetInputGuideKeyPressed( pStatic, 3, FALSE != pDIManager->GetPureActionState( GA_DOWN ), 0.f );
		}
		break;
	case 1:		// IGS_DASH
		{
			CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDLGInputGuide->GetControl( L"g_pStaticDashGuide" ) );
			if( NULL != pStatic )
				SetInputGuideKeyPressed( pStatic, 8, FALSE != pDIManager->GetPureActionState( GA_RIGHT ), fElapsedTime );
		}
		break;
	case 2:		// IGS_ATTACK
		{
			CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDLGInputGuide->GetControl( L"g_pStaticAttackGuide" ) );
			if( NULL == pStatic )
				break;
			SetInputGuideKeyPressed( pStatic, 2, FALSE != pDIManager->GetPureActionState( GA_ATTACK_FAST ), 0.f );
			SetInputGuideKeyPressed( pStatic, 4, FALSE != pDIManager->GetPureActionState( GA_ATTACK_STRONG ), 0.f );
		}
		break;
	case 3:		// IGS_SKILL
		{
			CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDLGInputGuide->GetControl( L"g_pStaticSkillGuide" ) );
			if( NULL == pStatic )
				break;

			const BOOL bSkill1 = pDIManager->GetPureActionState( GA_SKILL1 );
			const BOOL bSkill4 = pDIManager->GetPureActionState( GA_SKILL4 );
			if( FALSE == bSkill1 && FALSE == bSkill4 && GUIDE_NO_SKILL_KEY == m_NowGuideScene.m_iGuideNo )
			{
				BlinkInputGuideKey( pStatic, 2, &m_fInputGuideBlinkTime, fElapsedTime );
				BlinkInputGuideKey( pStatic, 4, &m_fInputGuideBlinkTimeSub, fElapsedTime );
				break;
			}
			m_fInputGuideBlinkTime = 0.f;
			m_fInputGuideBlinkTimeSub = INPUT_GUIDE_BLINK_SUB_START;
			SetInputGuideKeyPressed( pStatic, 2, FALSE != bSkill1, 0.f );
			SetInputGuideKeyPressed( pStatic, 4, FALSE != bSkill4, 0.f );
		}
		break;
	case 4:		// IGS_AWAKE
		{
			CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDLGInputGuide->GetControl( L"g_pStaticAwakeGuide" ) );
			if( NULL == pStatic )
				break;

			const BOOL bAwake = pDIManager->GetPureActionState( GA_AWAKE );
			if( FALSE == bAwake && GUIDE_NO_AWAKE_KEY == m_NowGuideScene.m_iGuideNo )
			{
				BlinkInputGuideKey( pStatic, 0, &m_fInputGuideBlinkTime, fElapsedTime );
				break;
			}
			m_fInputGuideBlinkTime = 0.f;
			SetInputGuideKeyPressed( pStatic, 0, FALSE != bAwake, 0.f );
		}
		break;
	}
}

void CX2TutorialGuideManager::OnFrameMoveInputGuideStep( float fElapsedTime )
{
	if( 0 == m_iInputGuideStep || 1 == m_iInputGuideStep )		// INPUT_GUIDE_STEP IGS_MOVE / IGS_DASH
		m_fInputAlarmTime += fElapsedTime;

	if( NULL == m_pDLGInputGuide || false == m_pDLGInputGuide->GetShow() )
		return;		// June 31fc52f0 reads the dummy dialog when there is none

	if( true == m_bInputGuideHidden && m_fInputGuideHideTime > 0.f && true == m_pDLGInputGuide->GetShow() )
	{
		m_fInputGuideHideTime -= fElapsedTime;
		if( m_fInputGuideHideTime > 0.f )
		{
			m_pDLGInputGuide->SetColor( D3DXCOLOR( 1.f, 1.f, 1.f, m_fInputGuideHideTime + m_fInputGuideHideTime ) );
			return;
		}

		m_fInputGuideHideTime = 0.f;
		m_pDLGInputGuide->SetShow( false );
		ShowInputSideGuideUI( m_iInputGuideStep );
		if( -1 != m_iReservedInputGuideStep )
		{
			ShowInputGuideUI( m_iReservedInputGuideStep );
			m_iReservedInputGuideStep = -1;
		}
		return;
	}

	switch( m_iInputGuideStep )
	{
	case 0:		// INPUT_GUIDE_STEP IGS_MOVE: hide once all four arrows were pressed
		{
			for( std::map< int, bool >::const_iterator mit = m_mapMoveGuideKeyPressed.begin(); mit != m_mapMoveGuideKeyPressed.end(); ++mit )
			{
				if( false == mit->second )
					return;
			}
			HideInputGuideUI();
		}
		break;
	case 1:		// IGS_DASH
		if( m_fDashGuideTime > DASH_GUIDE_DONE_TIME )
			HideInputGuideUI();
		break;
	case 3:		// IGS_SKILL
		if( NULL != g_pData->GetUIManager() && false == g_pData->GetUIManager()->GetShowSkillslot() )
			g_pData->GetUIManager()->SetShowSkillSlot( true );
		break;
	case 4:		// IGS_AWAKE
		if( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() && true == g_pX2Game->GetMyUnit()->GetHyperModeUsed() )
			HideInputGuideUI();
		break;
	}
}

void CX2TutorialGuideManager::ShowGuideMovie( int iGFxUINo )
{
	// stand-in: GFx adapter is t2 ( June 32965a60 showGuide )
}

void CX2TutorialGuideManager::HideGuideMovie()
{
	// stand-in: GFx adapter is t2 ( June 329659b0 hideGuide )
}

CX2TutorialGuideManager* CX2Game::GetTutorialGuideManager()
{
	return &g_pMain->GetTutorialGuideManager();	// June 322cc010
}
#endif SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE
