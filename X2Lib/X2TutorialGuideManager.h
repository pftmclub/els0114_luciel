#pragma once

// june-dungeons C3z-4t1: June's CX2TutorialGuideManager (vtable 33471a4c), an embedded member of CX2Main (June +0x8c4); map june2017_re/c3z4t_scope.tsv.

#ifdef SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE

class CX2TutorialGuideAdapter;

class CX2TutorialGuideManager
{
public:
	// June 0x9c B scene, filled by LoadTutorialGuideInfo 31fbe4a0 from TutorialGuideInfo.lua; the June scene offset is in each comment
	struct GuideScene
	{
		int		m_iGuideNo;							// +0x00 GUIDE_NO
		bool	m_bWait;							// +0x04 WAIT
		bool	m_bClickWait;						// +0x05 CLICK_WAIT
		int		m_iConditionState;					// +0x08 CONDITION_STATE
		int		m_iConditionFieldID;				// +0x0c CONDITION_FIELD_ID
		int		m_iConditionGoingQuestID;			// +0x10 CONDITION_GOING_QUEST_ID
		int		m_iConditionNewQuestID;				// +0x14 CONDITION_NEW_QUEST_ID
		int		m_iConditionCompleteQuestID;		// +0x18 CONDITION_COMPLETE_QUEST_ID
		int		m_iConditionFailedToEnterVillage;	// +0x1c CONDITION_FAILED_TO_ENTER_VILLAGE
		bool	m_bFirstGuide;						// +0x20 FIRST_GUIDE
		bool	m_bFixSkillTypeA;					// +0x21 FIX_SKILL_TYPE_A
		bool	m_bUseLevelUpRepeat;				// +0x22 USE_LEVLE_UP_REPEAT (June's spelling of the key)
		bool	m_bUseStartTime;					// +0x23 USE_START_TIME
		float	m_fStartTime;						// +0x24 START_TIME
		int		m_iOpenUIWithGuideStart;			// +0x28 OPEN_UI_WITH_GUIDE_START
		int		m_iChangeInventoryTab;				// +0x2c CHANGE_INVENTORY_TAB
		int		m_iPriorityLevel;					// +0x30 PRIORITY_LEVEL
		bool	m_bUseKeyLimitInGuide;				// +0x34 USE_KEY_LIMIT_IN_GUIDE
		float	m_fKeyLimitTime;					// +0x38 KEY_LIMIT_TIME
		bool	m_bUseLimitZoom;					// +0x3c USE_LIMIT_ZOOM
		bool	m_bUseChangeMPValue;				// +0x3d USE_CHANGE_MP_VALUE
		float	m_fChangeMPValue;					// +0x40 CHANGE_MP_VALUE
		bool	m_bUseCameraMove;					// +0x44 USE_CAMERA_MOVE
		int		m_iCameraTargetNPCID;				// +0x48 CAMERA_TARGET
		bool	m_bNeedNPCLoaded;					// +0x4c NEED_NPC_LOADED
		bool	m_bNoStopFrame;						// +0x4d NO_STOP_FRAME
		bool	m_bAnyKeyAllow;						// +0x4e ANY_KEY_ALLOW
		bool	m_bEscKeyAllow;						// +0x4f ESC_KEY_ALLOW
		bool	m_bDirKeyAllow;						// +0x50 DIR_KEY_ALLOW
		bool	m_bAttackKeyAllow;					// +0x51 ATTACK_KEY_ALLOW
		bool	m_bSkillKeyAllow;					// +0x52 SKILL_KEY_ALLOW
		bool	m_bAwakeKeyAllow;					// +0x53 AWAKE_KEY_ALLOW
		int		m_iServerEventID;					// +0x54 June event id 0x101 when SERVER_EVENT_TYPE == SEVER_EVENT_INVENTORY_SLOT_ITEM, read by t3's UIServerEventProc hook
		int		m_iCustomEventMsg;					// +0x58 CUSTOM_EVENT_MSG
		int		m_iCustomEventSubMsg1;				// +0x5c CUSTOM_EVENT_SUB_MSG1
		int		m_iCustomEventSubMsg2;				// +0x60 CUSTOM_EVENT_SUB_MSG2
		bool	m_bMouseEventAllow;					// +0x64 MOUSE_EVENT_ALLOW
		int		m_iButtonEventAllowMsg1;			// +0x68 BUTTON_EVENT_ALLOW_MSG1
		int		m_iButtonEventAllowMsg2;			// +0x6c BUTTON_EVENT_ALLOW_MSG2
		int		m_iButtonEventAllowMsg3;			// +0x70 BUTTON_EVENT_ALLOW_MSG3
		bool	m_bDialogCreateAllow;				// +0x74 DIALOG_CREATE_ALLOW
		int		m_iGFxUINo;							// +0x78 GFX_UI_NO
		bool	m_bUseOffUIInEndCheck;				// +0x7c USE_OFF_UI_IN_END_CHECK
		bool	m_bUseAutoGuideStop;				// +0x7d USE_AUTO_GUIDE_STOP
		int		m_iReplaceGuideNo;					// +0x80 REPLASE_GUIDE (June's spelling of the key)
		bool	m_bUseEndTime;						// +0x84 USE_END_TIME
		float	m_fEndTime;							// +0x88 END_TIME
		bool	m_bAutoNextGuide;					// +0x8c AUTO_NEXT_GUIDE
		int		m_iNextGuideNo;						// +0x90 NEXT_GUIDE_NO
		bool	m_bReplayGuide;						// +0x94 REPLAY_GUIDE
		int		m_iReplayGuideNo;					// +0x98 REPLAY_GUIDE_NO

		GuideScene() { Init(); }
		void Init();								// June 31fc67a0
	};

	// June's guide state at +0x14; June's Lua has no enum for it
	enum GUIDE_STATE
	{
		GS_NONE					= 0,
		GS_RUNNING				= 1,
		GS_WAIT_EVENT_NPC		= 2,
		GS_KEY_LIMIT			= 3,
		GS_WAIT_INPUT			= 4,
		GS_WAIT_END_TIME		= 5,
		GS_WAIT_CLICK			= 6,
	};

	// the object June reads at +0xc4 (vslot 4 -> bool); no June writer found, so it stays NULL
	class GuideStartBlocker
	{
	public:
		virtual ~GuideStartBlocker() {}
		virtual bool IsBlockingGuideStart() = 0;
	};

public:
	CX2TutorialGuideManager();						// June 31fc3a40
	virtual ~CX2TutorialGuideManager();				// June 31fbe320

	void	Load();									// June 31fc3c20
	HRESULT	OnFrameMove( double fTime, float fElapsedTime );	// June 31fc6510

	bool	IsGuideRunning() const { return GS_NONE != m_iGuideState; }								// June 31fbda90
	bool	IsGuideRunningWithStopFrame() const { return GS_NONE != m_iGuideState && false == m_NowGuideScene.m_bNoStopFrame; }	// June 31fbdab0 / 31fbdad0
	bool	IsAnyKeyNotAllowed() const { return true != m_NowGuideScene.m_bAnyKeyAllow; }			// June 31fbdb20

	// Lua (binder 31d95080 @31e3a0ab-31e3b29b)
	float	CheckEventTriggerNPCPosX( int iNPCID );		// June 31fc3e80
	float	CheckEventTriggerNPCPos( int iNPCID );		// June 31fc3f80
	void	ShowInputGuideUI( int iInputGuideStep );	// June 31fc4fa0
	void	HideInputGuideUI();							// June 31fbdcf0
	int		GetNowGuideNo() const { return m_NowGuideScene.m_iGuideNo; }	// June 31fbdcb0
	bool	CheckAndResetInputAlarm( float fAlarmTime, float fResetTime );	// June 31fbdcc0
	bool	CheckToNeedTutorialGuide();					// June 31fbde90
	void	OnBeginGuide();								// June 31fc6320
	void	OnBeginGuideNo( int iGuideNo );				// June 31fc5e20 -> 31fc5d70

	void	OnBeginGuideByGame();						// June 31fc5e30
	void	OnBeginGuideWhenNPCLoaded();				// June 31fc5e70
	void	StopGuide();								// June 31fc6010
	void	BeginReplayGuide();							// June 31fc6020
	void	OnExitTutorial();							// June 31fc3cc0
	void	SetShowInputGuideUI( bool bShow );			// June 31fbddf0

	bool	LoadTutorialGuideInfo();					// June 31fbe4a0

private:
	bool	SetGuideStartState();						// June 31fbdc40
	bool	CanBeginGuide( bool bOutsideDungeonOnly, bool bHighPriority, int iConditionState );	// June 31fc4530
	void	CheckAccountHasHighLevelUnit();				// June 31fbdf40
	void	PlayGuideStep();							// June 31fc4060
	void	EndGuide();									// June 31fc4c40
	void	WaitEventNPC();								// June 31fc4d80
	void	CheckCameraArrive();						// June 31fc4e30
	void	BeginGuideScene( int iGuideNo );			// June 31fc5d70
	void	AbortGuide();								// June 31fc5f80
	void	FinishGuideStep();							// June 31fc5ec0
	void	EndGuideStep();								// June 31fc6360
	void	EndGuideStepWithCustomEvent();				// June 31fc6450
	bool	CheckAllowedKeyPressed();					// June 31fc6130
	void	OnFrameMoveKeyLimitTime( float fElapsedTime );	// June 31fc6490
	void	OnFrameMoveStartTime( float fElapsedTime );	// June 31fc6050
	void	ResetFocusEventNPC();						// June 31fc4430
	void	StopGameFrame();							// June 31fc3e10
	void	ResumeGameFrame();							// June 31fc3da0
	void	SetShowQuickSlotByGuide();					// June 31fbdfa0
	void	ResetInputGuideState();						// June 31fc3990
	void	ShowInputSideGuideUI( int iInputGuideStep );	// June 31fc46d0
	void	BlinkInputGuideKey( CKTDGUIStatic* pStatic, int iPictureIndex, float* pfBlinkTime, float fElapsedTime );	// June 31fbdd30
	void	SetInputGuideKeyPressed( CKTDGUIStatic* pStatic, int iPictureIndex, bool bPressed, float fElapsedTime );	// June 31fc4960
	void	OnFrameMoveInputGuideKey( float fElapsedTime );		// June 31fc5570
	void	OnFrameMoveInputGuideStep( float fElapsedTime );	// June 31fc52f0
	void	ShowGuideMovie( int iGFxUINo );				// June adapter 32965a60
	void	HideGuideMovie();							// June adapter 329659b0

private:
	std::map< int, GuideScene >	m_mapGuideScene;			// +0x04
	int							m_iGuideState;				// +0x14 GUIDE_STATE
	GuideScene					m_NowGuideScene;			// +0x18
	bool						m_bWaitCameraArrive;		// +0xb4
	CX2TutorialGuideAdapter*	m_pTutorialGuideAdapter;	// +0xb8
	float						m_fGuideTime;				// +0xbc
	bool						m_bEndTimeRunning;			// +0xc0
	bool						m_bStartTimeRunning;		// +0xc1
	bool						m_bKeyLimitTimeRunning;		// +0xc2
	GuideStartBlocker*			m_pGuideStartBlocker;		// +0xc4
	bool						m_bGuideOffByF2;			// +0xc8
	bool						m_bAccountHasHighLevelUnit;	// +0xc9
	int							m_iReservedGuideNo;			// +0xcc
	bool						m_bSkipNextGuide;			// +0xd0
	bool						m_bReplayGuideReserved;		// +0xd1
	int							m_iReplayGuideNo;			// +0xd4
	int							m_iLastCustomEventMsg;		// +0xd8
	CKTDGUIDialogType			m_pDLGInputGuide;			// +0xdc DLG_UI_Tutorial_Input_Guide.lua
	int							m_iInputGuideStep;			// +0xe4 INPUT_GUIDE_STEP
	int							m_iReservedInputGuideStep;	// +0xe8
	float						m_fInputGuideBlinkTime;		// +0xec
	float						m_fInputGuideBlinkTimeSub;	// +0xf0
	float						m_fDashGuideTime;			// +0xf4
	float						m_fInputGuideHideTime;		// +0xf8
	float						m_fInputAlarmTime;			// +0xfc
	bool						m_bDashGuideDone;			// +0x100
	std::map< int, bool >		m_mapMoveGuideKeyPressed;	// +0x104 picture index -> pressed
	CKTDGUIDialogType			m_pDLGInputSideGuide;		// +0x114 DLG_UI_Tutorial_Input_Side_Guide.lua
	bool						m_bInputGuideHidden;		// +0x11c
};

#endif SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE
