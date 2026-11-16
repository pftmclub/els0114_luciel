#pragma once


#include "./X2BaseUnitViewerUI.h"

#ifdef SERV_IRUHADEV_JUNE_UNIT_VIEWER_EFFECT_PLAYER
	// June effect completion C9c-2b: June's CX2UnitViewerUI derives from CX2BaseUnitViewerUI, which holds the effect player
	class CX2UnitViewerUI : public CX2BaseUnitViewerUI
#else SERV_IRUHADEV_JUNE_UNIT_VIEWER_EFFECT_PLAYER
#ifdef MONSTER_TROPHY
	class CX2UnitViewerUI : public CX2BaseUnitViewerUI
#else 
	class CX2UnitViewerUI : public CKTDGObject
#endif MONSTER_TROPHY
#endif SERV_IRUHADEV_JUNE_UNIT_VIEWER_EFFECT_PLAYER
{
	public:

#ifdef SERV_IRUHADEV_JUNE_ANIMATION_NAME_TABLE
		// June2017 Enum.lua UNIT_VIEWER_UI_MOTION_TYPE (HABIT 6, SIT 7-9, WAIT_DELETE 6 -> 10); March's identifiers are kept
		enum UNIT_VIEWER_UI_MOTION_TYPE
		{
			UVUMT_NONE = -1,
			UVUMT_WAIT = 0,
			UVUMT_WAIT1,
			UVVMT_WAIT2,
			UVVMT_DOWN_LANDING,
			UVUMT_READY,
			UVUMT_LEVEL_UP,
			UVUMT_HABIT,
			UVUMT_SIT_READY,
			UVUMT_SIT_WAIT,
			UVUMT_STANDUP,
			UVUMT_WAIT_DELETE,
		};
#else SERV_IRUHADEV_JUNE_ANIMATION_NAME_TABLE
		enum UNIT_VIEWER_UI_MOTION_TYPE
		{
			UVUMT_WAIT = 0,
			UVUMT_WAIT1,
			UVVMT_WAIT2,
			UVVMT_DOWN_LANDING,
			UVUMT_READY,
			UVUMT_LEVEL_UP,
//#ifdef SERV_UNIT_WAIT_DELETE
			UVUMT_WAIT_DELETE,
//#endif //SERV_UNIT_WAIT_DELETE
		};
#endif SERV_IRUHADEV_JUNE_ANIMATION_NAME_TABLE

		enum UNIT_VIEWER_STATE
		{
			UVS_NONE		= 0,
			UVS_CASHSHOP,
			UVS_ROOM,
			UVS_FIELD,
			UVS_CLASSCHANGE,
			UVS_CREATEUNIT,
			UVS_SELECTUNIT,
			UVS_CHARINFO,
			UVS_WATCH,
		};

	private:
		CX2UnitViewerUI(void);
		virtual ~CX2UnitViewerUI(void);

	public: 
		static CX2UnitViewerUI* CreateUnitViewerUI() { return new CX2UnitViewerUI; }

		virtual void	NotifyShowObjectChanged();
		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

//{{ robobeg : 2008-10-28
        virtual RENDER_HINT   OnFrameRender_Prepare();
		//virtual HRESULT OnFrameRender();
        virtual void    OnFrameRender_Draw();
//}} robobeg : 2008-10-28

//{{ kimhc // 2010.9.9 // 홈페이지 개편에서 아바타 이미지 표현으로 사용될 캐릭터 이미지 캡쳐 기능 테스트
#ifdef	AVATAR_IMAGE_TEST
		void Render_DrawForAvartaScreenCapture();
		RENDER_HINT OnRender_PrepareForAvartaScreenCapture();
#endif	AVATAR_IMAGE_TEST
//}} kimhc // 2010.9.9 // 홈페이지 개편에서 아바타 이미지 표현으로 사용될 캐릭터 이미지 캡쳐 기능 테스트
		
		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
		enum UNIT_VIEWER_PLAY_BODY	// char-luciel DU5b: June PlayAnim 31fa3010's fourth argument ( no June Enum )
		{
			UVPB_MAJOR_UNIT	= 0,
			UVPB_MINOR_UNIT	= 1,
			UVPB_BOTH		= 2,
		};
		void PlayAnim( const WCHAR* pName, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType = CKTDGXSkinAnim::XAP_LOOP, bool bTransition = false, const int iPlayBody_ = UVPB_BOTH );	// char-luciel DU5b2 fix: every June caller outside the viewer passes 2 ( square unit 323fad80, result, create, pet )
#else SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
		void PlayAnim( const WCHAR* pName, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType = CKTDGXSkinAnim::XAP_LOOP, bool bTransition = false );
#endif SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER

		void SetUnit( CX2Unit* pUnit, CX2UnitViewerUI::UNIT_VIEWER_STATE eState = UVS_NONE );

		//{{ robobeg : 2011-03-18
		static void         AppendToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pUnit, CX2UnitViewerUI::UNIT_VIEWER_STATE eState = UVS_NONE );
		//}} robobeg : 2011-03-18

		CX2Unit* GetUnit() { return m_pUnit; }

		void SetPickUnit(bool val);
		// 케릭터의 인벤토리 안에서 처리
		void UpdateEqip( bool bEconomical = false );
		void RemoveEqip( UidType itemUID, bool bInNoReplaceEmptyToBasic = false );
		void AddEqip( UidType itemUID );

		// 모든 아이템 템플릿에 관해 처리
//{{ robobeg : 2008-10-17
		//void AddEquipByTID( int itemTID );
		//void RemoveEquipByTID( int itemTID );

#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
		void AddEquipByTID( int itemTID, bool bBasicItem = false, bool bMinorUnit = false, int iEnchantLevel = -1 );	// char-luciel DU5b2: June 31f9c640
#else SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
        void AddEquipByTID( int itemTID, bool bBasicItem = false );
#endif SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
//}} robobeg : 2008-10-17
		void RemoveAllEquips();
#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
		int GetViewSlotPosition( CX2Eqip* pX2Eqip_, const CX2Item::ItemTemplet* pItemTemplet_ ) const;	// char-luciel DU5b2
		void SetShowFormEquipOnly( bool bShowFormEquipOnly_ ) { m_bShowFormEquipOnly = bShowFormEquipOnly_; }	// char-luciel DU5b2: June +0x118c
#endif SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER

#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
		void InitEquipVisibility();
		void UpdateEquipSlotVisibility();
#endif	NOT_RENDERING_OTHER_EQUIP_POS

		CKTDGXRenderer::RenderParam* GetRenderParam();
#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
		// char-luciel DU5b: June 31fac3c0 / 31f90ff0 return the shown body, Ciel's ( +0xd7c ) while the switching view is on
		CKTDGXSkinAnim* GetXSkinAnim(){ return AccessShownXSkinAnim().get(); }
		CKTDGXSkinAnimPtr GetXSkinAnimPtr() { return AccessShownXSkinAnim(); }
		CKTDGXSkinAnimPtr& AccessShownXSkinAnim() { return true == m_bSwitchingView ? m_pMinorUnitXSkinAnim : m_pXSkinAnim; }	// June 31fac430
		CKTDGXSkinAnimPtr GetBackXSkinAnimPtr() const;
		void SetSwitchingView( const bool bSwitchingView_ );
		bool GetSwitchingView() const { return m_bSwitchingView; }
		void UpdateSwitchingView();
		void ApplySwitchingViewFilter( const std::vector<CX2Unit::EQIP_POSITION>& vecKeepEquipPosition_ );
		void SetEquipColorByBody( const bool bMinorUnit_, const D3DXCOLOR& color_ );	// char-luciel DU5d2: June 31f93920
		void SetEnableDuoView( const bool bEnableDuoView_ ) { m_bEnableDuoView = bEnableDuoView_; }
#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_AVATAR
		void SetSwitchingViewValue( const bool bSwitchingView_ ) { m_bSwitchingView = bSwitchingView_; }	// char-luciel DU5c: June's inline +0xd80 write ( 323f3290 @323f3394 ), no UpdateSwitchingView
#endif SERV_IRUHADEV_JUNE_CHAR_DUO_AVATAR
#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY
		void BackUnitFrameMove( double fTime, float fElapsedTime );	// char-luciel DU5c2: June 31fa8ce0( t, dt, 0 )
		void SetUnitScale( const D3DXVECTOR3& vScale_ );	// char-luciel DU5c2: June 31f969a0
		void SetBackUnitOffsetRate( const float fBackUnitOffsetRate_ ) { m_fBackUnitOffsetRate = fBackUnitOffsetRate_; }	// char-luciel DU5d2 fix: June +0xd84 writes ( cash shop 0.33, class change 1.5 )
		void SetMinorUnitRotation( const D3DXVECTOR3& vRotation_ ) { if ( NULL != m_pMinorUnitXSkinAnim ) m_pMinorUnitXSkinAnim->GetMatrix().Rotate( vRotation_ ); }	// char-luciel DU5c2: June 31f90fb0
#endif SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY
#else SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
		CKTDGXSkinAnim* GetXSkinAnim(){ return m_pXSkinAnim.get(); }
		CKTDGXSkinAnimPtr GetXSkinAnimPtr() { return m_pXSkinAnim; }
#endif SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
		void SetLightPos( float x, float y, float z )
		{ 
			m_LightPos.x = x;
			m_LightPos.y = y;
			m_LightPos.z = z;
		}
		void SetLightPos( D3DXVECTOR3 lightPos )
		{ 
			m_LightPos = lightPos;
		}
		D3DXVECTOR3 GetLightPos() { return m_LightPos; }

		void SetBillBoardType( CKTDGMatrix::BILLBOARD_TYPE billBoardType ){ m_BillBoardType = billBoardType; }

		void ReplaceEmptyToBasic();
		
		static void SetShowUnitViewerUI( bool bShow ) { m_sbShow = bShow; }

#ifdef SERV_IRUHADEV_JUNE_ANIMATION_NAME_TABLE
		void PlayByMotionType( CX2UnitViewerUI::UNIT_VIEWER_UI_MOTION_TYPE motionType, bool bTransition = true );
#else SERV_IRUHADEV_JUNE_ANIMATION_NAME_TABLE
		void PlayByMotionType( CX2UnitViewerUI::UNIT_VIEWER_UI_MOTION_TYPE motionType );
#endif SERV_IRUHADEV_JUNE_ANIMATION_NAME_TABLE
#ifdef SERV_IRUHADEV_JUNE_EQUIP_EFFECT_SET
		void GetDivisionEquips( int iDivisionType, std::vector<CX2EqipPtr>& vecOut );	// June 31fa2f20
#endif SERV_IRUHADEV_JUNE_EQUIP_EFFECT_SET

		static bool GetUnitClicked() { return m_sbUnitClicked; }
		static void SetUnitClicked( bool bCheck ) { m_sbUnitClicked = bCheck; }

		int GetEnchantExtraDamageType();

//#ifdef TITLE_SYSTEM
        void UpdateTitle();
//#endif
	
		void SetShowAttribEnchant( bool bShow );

//{{ robobeg : 2008-10-17



// X2UnitVIewerUI 가 KTDGManager 에 등록, 삭제될 때 장착 부품들도 같이 등록, 삭제하기 위해 사용됨
        virtual void    NotifyDGManagerChanged( CKTDGManager& refInManager_ );
//}} robobeg : 2008-10-17

#ifdef NEW_VILLAGE_UI
		void SetPositionOnScr(float x, float y, float z = 0.8f, float scale = 1.f );
		void SetFixed(bool val);
		bool GetFixed() { return m_bFixed; }
#endif


#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
		// char-luciel DU5b2: no live caller, June has none; the slot of one body
		bool    IsFashionItemEquippedAt( CX2Unit::EQIP_POSITION eqipPos, int iBody_ = 0 )
		{
			ASSERT( eqipPos >= CX2Unit::EQIP_POSITION( 0 ) && eqipPos < CX2Unit::EP_END );
			return m_ViewEquipFashion[ eqipPos ][ iBody_ ] != 0;
		}
#else SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
		bool    IsFashionItemEquippedAt( CX2Unit::EQIP_POSITION eqipPos )
		{
			ASSERT( eqipPos >= CX2Unit::EQIP_POSITION( 0 ) && eqipPos < CX2Unit::EP_END );
			return m_ViewEquipFashion[ eqipPos ] != 0;
		}
#endif SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER

#ifdef UNIT_EMOTION
		void EmotionFrameMove();
		bool PlayEmotion(CX2Unit::EMOTION_TYPE eEmotionId, wstring &wstrEmotionName);
		bool GetPlayerEmotion();
		CX2Unit::EMOTION_TYPE GetNowEmotionId() { return m_ePlayedEmotion; }
		void SetNowEmotionId(CX2Unit::EMOTION_TYPE eId) { m_ePlayedEmotion = eId; }
#endif

		float GetScale() { return m_fScale; }

		void SetHideSelectUnitWeapon();		/// 케릭터 선택 화면등에서 꺼내든 무기 숨기는 함수
		void SetShowObjectMeshPlayer(CKTDGXMeshPlayer::CXMeshInstanceHandle MeshInstHandle_, bool bShow_);
#ifdef SERV_RENA_NIGHT_WATCHER
		void SetClassChange( IN bool bValue_ ) { m_bChangeClass = bValue_; }
#endif SERV_RENA_NIGHT_WATCHER

#ifdef ADD_UPGRADE_WEAPON_PARTICLE	/// 강화 무기에 대한 이펙트 출력 처리
		void InitUpgradeWeaponParticle();	/// 강화 무기 이펙트 초기화
#ifdef SERV_IRUHADEV_JUNE_DIVERGE_UPGRADE_WEAPON_PARTICLE
		void CreateUpgradeWeaponParticle( const WCHAR* pName1, const WCHAR* pName2 = NULL, const WCHAR* pName3 = NULL, const WCHAR* pName4 = NULL,
			const WCHAR* pName5 = NULL, const WCHAR* pName6 = NULL, const WCHAR* pName7 = NULL, const WCHAR* pName8 = NULL, const WCHAR* pName9 = NULL, const WCHAR* pName10 = NULL );	// June 31f914c0
#else SERV_IRUHADEV_JUNE_DIVERGE_UPGRADE_WEAPON_PARTICLE
		void CreateUpgradeWeaponParticle( const WCHAR* pName1, const WCHAR* pName2 = NULL, const WCHAR* pName3 = NULL, const WCHAR* pName4 = NULL );	/// 강화 무기 이펙트 생성
#endif SERV_IRUHADEV_JUNE_DIVERGE_UPGRADE_WEAPON_PARTICLE
		void ProcessUpgradeWeaponParticle();/// 강화 무기 이펙트 갱신
		void DeleteUpgradeWeaponParticle();	/// 강화 무기 이펙트 제거
#endif ADD_UPGRADE_WEAPON_PARTICLE

#ifdef TAKE_OFF_ALL_ITEM
		void SetShowInvenEquip( bool bVal_ ) { m_bShowInvenEquip = bVal_; }
#endif // TAKE_OFF_ALL_ITEM

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
		bool IsPlayAvatarEmotionSoundWithouEmotion ( CX2Unit::EMOTION_TYPE eEmotionId );
#endif // CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
	private:
		
		void ReplaceEmptyToBasic( CX2Unit::EQIP_POSITION eqipPos, int itemID );
//{{ robobeg : 2008-10-17
		//void CheckEmptyEqipPosition();
//}} robobeg : 2008-10-17



#ifdef SERV_IRUHADEV_JUNE_CHARINFO_VIEW_ATTRIB_ENCHANT
		void CreateEnchantedWeaponParticleAtWeapon( int iNameCount, ... );	// June 31f9d1f0
		void CreateEnchantedWeaponParticleAtHand( int iNameCount, ... );	// June 31f9d3b0
#else SERV_IRUHADEV_JUNE_CHARINFO_VIEW_ATTRIB_ENCHANT
		void CreateEnchantedWeaponParticleAtWeapon( const WCHAR* pName1, const WCHAR* pName2 = NULL, const WCHAR* pName3 = NULL );
		void CreateEnchantedWeaponParticleAtHand( const WCHAR* pName1, const WCHAR* pName2 = NULL, const WCHAR* pName3 = NULL, const WCHAR* pName4 = NULL );
#endif SERV_IRUHADEV_JUNE_CHARINFO_VIEW_ATTRIB_ENCHANT
		void ProcessEnchantedWeaponEffectAtHand( CX2Unit::UNIT_TYPE unitType );
		void InitEnchantedWeaponEffectAtHand();


		

		void InitEnchantWeaponEffectForSword();
		void InitEnchantWeaponEffectForBow();

		void DeleteEnchantWeaponEffect();

		void ProcessEnchantWeaponEffectForSword();
		void ProcessEnchantWeaponEffectForBow();
		void ProcessEnchantWeaponEffectForEve();
#ifdef ARA_CHARACTER_BASE
		void ProcessEnchantWeaponEffectForAra();
#endif
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
		void ProcessEnchantWeaponEffectForAdd();
#endif //SERV_9TH_NEW_CHARACTER
#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
#if !defined( SERV_IRUHADEV_JUNE_CHARINFO_VIEW_ATTRIB_ENCHANT ) || !defined( ADD_UPGRADE_WEAPON_PARTICLE ) || !defined( ARA_CHARACTER_BASE )
#error SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER needs SERV_IRUHADEV_JUNE_CHARINFO_VIEW_ATTRIB_ENCHANT, ADD_UPGRADE_WEAPON_PARTICLE and ARA_CHARACTER_BASE ( Lu's weapon effects and trace )
#endif
		void ProcessEnchantWeaponEffectForLuciel();	// char-luciel DU5d: June 31f93050
#endif SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER

		void InitWeaponTrace();

//{{ robobeg : 2008-10-17
// 현재의 EQUIP_POSITION 의 부품 착용 여부를 알려주는 함수
#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
		// char-luciel DU5b2: no live caller under the flag ( June's ReplaceEmptyToBasic reads the slots itself ); the slot of one body
		bool    IsItemEquippedAt( CX2Unit::EQIP_POSITION eqipPos, int iBody_ = 0 )
		{
			ASSERT( eqipPos >= CX2Unit::EQIP_POSITION( 0 ) && eqipPos < CX2Unit::EP_END );
			return m_ViewEquipNormal[ eqipPos ][ iBody_ ] != 0 || m_ViewEquipFashion[ eqipPos ][ iBody_ ] != 0;
		}
#else SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
        bool    IsItemEquippedAt( CX2Unit::EQIP_POSITION eqipPos )
        {
            ASSERT( eqipPos >= CX2Unit::EQIP_POSITION( 0 ) && eqipPos < CX2Unit::EP_END );
            return m_ViewEquipNormal[ eqipPos ] != 0 || m_ViewEquipFashion[ eqipPos ] != 0;
        }//IsItemEquippedAt()
#endif SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
//}} robobeg : 2008-10-17

#ifdef ELSWORD_SECOND_CLASS_CHANGE
		void SetUnitReForm();
#endif

		//{{ kimhc // 2010-01-05 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
		void RemovePCBangEquips();
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-05 // PC방 프리미엄 서비스

//{{ kimhc // 2010.12.21 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		void GetWeaponTraceStartAndEndName( OUT wstring* pwstrTraceStartName_, OUT wstring* pwstrTraceEndName_ );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.21 //  2010-12-23 New Character CHUNG

#ifdef RIDING_SYSTEM
		bool CanNotShowEnchantedWeaponEffectToRiding();
#endif //RIDING_SYSTEM
#ifdef SERV_IRUHADEV_JUNE_DIVERGE_UPGRADE_WEAPON_PARTICLE
		bool CanShowWeaponEffect();	// June 31f91230
#endif SERV_IRUHADEV_JUNE_DIVERGE_UPGRADE_WEAPON_PARTICLE

		CX2Unit*				m_pUnit;
		CKTDXDeviceXSkinMesh*	m_pMotion;
        /// 내부적으로 관리하는 객체이므로 smart pointer를 사용하지 않는다.
        /// - jintaeks on 2009-01-12, 16:12		
		// dmlee, 2009-03-12 백그라운드 로딩시에 dangling pointer 참조 오류 생기는 문제 때문에 smart poiner로 바꿨습니다.
		CKTDGXSkinAnimPtr		m_pXSkinAnim;
#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
		CKTDXDeviceXSkinMesh*	m_pMinorUnitMotion;			// char-luciel DU5b: June +0xd78
		CKTDGXSkinAnimPtr		m_pMinorUnitXSkinAnim;		// June +0xd7c, Ciel's skin
		bool					m_bSwitchingView;			// June +0xd80, Ciel is the shown body
		bool					m_bEnableDuoView;			// June +0xd81, the other body is drawn behind ( ctor true, the character window false )
		bool					m_bShowDuoBackUnit;			// June +0xd82, the back body's emotion gate ( its writers are DU5c's )
		bool					m_bEmotionOnBackUnitOnly;	// char-luciel DU5d2: June +0xe60, the emotion end waits on the back body ( setter: backlog Stage 10 )
		bool					m_bShowFormEquipOnly;		// char-luciel DU5b2: June +0x118c, a field / room viewer adds only the shown body's equipment ( writers: DU5c )
#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY
		float					m_fBackUnitOffsetRate;		// char-luciel DU5d2 fix: June +0xd84, the back body sits at ( rate + 1 ) x its offset
#endif SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY
#endif SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
		D3DXVECTOR3				m_LightPos;

//{{ robobeg : 2008-10-17
		//vector<CX2Eqip*>		m_ViewEqipList;
		//bool					m_ViewEqipPos[CX2Unit::EP_END];
		//bool					m_ViewEquipedFashion[CX2Unit::EP_END];

        typedef std::map<UidType,CX2EqipPtr>    ViewEquipMap;

        ViewEquipMap            m_ViewEquipMap;								// ED템, 패션템 모두 들어있음
#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
#ifndef SERV_IRUHADEV_JUNE_VIEWER_EQUIP_VISIBILITY
#error SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER needs SERV_IRUHADEV_JUNE_VIEWER_EQUIP_VISIBILITY ( the March slot visibility reads one body )
#endif
		UidType					m_ViewEquipNormal[CX2Unit::EP_END][3];		// char-luciel DU5b2: June +0x208, [ pos ][ body ]
		UidType					m_ViewEquipFashion[CX2Unit::EP_END][3];		// June +0x5b0, [ pos ][ body ]
#else SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
        UidType                 m_ViewEquipNormal[CX2Unit::EP_END];
        UidType                 m_ViewEquipFashion[CX2Unit::EP_END];
#endif SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
		bool					m_bViewEquipVisible[ CX2Unit::EP_END ];
#endif	NOT_RENDERING_OTHER_EQUIP_POS
//}} robobeg : 2008-10-17

		CKTDGMatrix::BILLBOARD_TYPE m_BillBoardType;

		static bool										m_sbShow;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandFire;

#ifdef SERV_IRUHADEV_JUNE_CHARINFO_VIEW_ATTRIB_ENCHANT
		std::vector<CKTDGParticleSystem::CParticleEventSequenceHandle>	m_vecSeqWeaponFire;	// June +0xd90
		std::vector<CKTDGParticleSystem::CParticleEventSequenceHandle>	m_vecSeqWeaponWind;	// June +0xda0: the hand sparks
#else SERV_IRUHADEV_JUNE_CHARINFO_VIEW_ATTRIB_ENCHANT
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqWeapon_Fire;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqWeapon_Fire2;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqWeapon_Fire3;

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqWeapon_Wind;		// 속성 인챈트된 무기를 들었을 때 손에 붙는 이펙트 
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWeapon_Wind2;
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWeapon_Wind3;
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWeapon_Wind4;
#endif SERV_IRUHADEV_JUNE_CHARINFO_VIEW_ATTRIB_ENCHANT

#ifdef NEW_CHARACTER_EL
		// 파이로 나이트 불
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandFireSPK1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandFireSPK2;
#endif //NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqEnergyBullet1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqEnergyBullet2;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqEnergyBullet3;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

		static bool										m_sbUnitClicked;

		int												m_ExtraDamageType;
		
		// 속성 인챈트 이펙트를 표시하기 위해서 사용. trace에는 사용 안함
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_TRACE_START[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_TRACE_END[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];

#ifdef ARA_CHARACTER_BASE
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_WEAPON_BONE[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
#endif

//#ifdef TITLE_SYSTEM
        CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEmblem;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGParticleSystem::CParticleHandle				m_hPart_Emblem_200;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGParticleSystem::CParticle*						m_pPart_Emblem_200;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

        int                                             m_iTitleId;
//#endif
#ifdef NEW_VILLAGE_UI
		D3DXVECTOR3 m_vScrPos;
		bool		m_bFixed;
#endif

		bool		m_bPickUnit;

		CKTDXDeviceSound*		m_pSound_Eve_LevelUp;

#ifdef UNIT_EMOTION
		CX2UnitViewerUI::UNIT_VIEWER_UI_MOTION_TYPE		m_eOldMotionType;
		CX2Unit::EMOTION_TYPE							m_ePlayedEmotion;
#endif


#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
		bool m_bEnableEquipSlotVisibilityUpdate;
#endif	NOT_RENDERING_OTHER_EQUIP_POS

		bool m_bMixedEmotion[AVATAR_EMOTION_NUM];
		CKTDXDeviceSound*				m_pAvatarEmotionSound;

		CKTDXTimer m_TimerResetWeaponTraceFrame;
		float		m_fScale;

#ifdef CHUNG_SECOND_CLASS_CHANGE
		bool		m_bShowGun;
		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hChungMiniGunL;	/// 청 미니건 왼쪽
		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hChungMiniGunR;	/// 청 미니건 오른쪽
#endif

#ifdef ELSWORD_SHEATH_KNIGHT
		bool		m_bShowSword;
		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hElswordSword;
#endif ELSWORD_SHEATH_KNIGHT

#ifdef	SERV_TRAPPING_RANGER_TEST
		bool												m_bShowLTrapSword;
		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hInstLTrapSwordReady;
#endif	SERV_TRAPPING_RANGER_TEST
		
#ifdef SERV_RENA_NIGHT_WATCHER
		bool												m_bShowRNWTrapSword;
		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hInstRNWTrapSwordReady;
		bool												m_bChangeClass;
#endif SERV_RENA_NIGHT_WATCHER

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 강화 무기 이펙트 객체
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_ATTACK_LINE_END0;
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_ATTACK_LINE_START0_FRONT;
#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_ATTACK_LINE_END0_RWeapon;		// char-luciel DU5d: June +0xefc, Lu's second weapon model
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_ATTACK_LINE_START0_RWeapon;	// June +0xf00
#endif SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqUpgradeWeapon;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqUpgradeWeapon2;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqUpgradeWeapon3;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqUpgradeWeapon4;
#ifdef SERV_IRUHADEV_JUNE_DIVERGE_UPGRADE_WEAPON_PARTICLE
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqUpgradeWeapon5;	// June +0xee4..+0xef8: the +13 spark tier
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqUpgradeWeapon6;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqUpgradeWeapon7;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqUpgradeWeapon8;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqUpgradeWeapon9;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqUpgradeWeapon10;
#endif SERV_IRUHADEV_JUNE_DIVERGE_UPGRADE_WEAPON_PARTICLE
#endif ADD_UPGRADE_WEAPON_PARTICLE
		
#ifdef TAKE_OFF_ALL_ITEM
		bool												m_bShowInvenEquip;
#endif // TAKE_OFF_ALL_ITEM


#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandFireSBH1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandFireSBH2;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandFireSBH3;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
		UNIT_VIEWER_STATE									m_eUnitViewerState;
		
#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
		bool												m_bIsPlayAvatarEmotionSoundWithoutEmotion;
#endif // CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandElectricALP1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandElectricALP2;
#endif //SERV_ADD_LUNATIC_PSYKER

#ifdef SERV_IRUHADEV_JUNE_UNIT_VIEWER_EFFECT_PLAYER
		// June 31fa3010 request queue (+0x11a0): one 0x30-byte entry per pending set, drained by the frame move 31fa8ce0
		struct ViewerFxRequest
		{
			std::wstring	m_wstrSetName;
			int				m_iUnitType;
			int				m_iUnitClass;
			float			m_fStartTime;
			bool			m_bLoop;

			ViewerFxRequest() : m_iUnitType( 0 ), m_iUnitClass( 0 ), m_fStartTime( 0.f ), m_bLoop( false ) {}
		};
		std::vector<ViewerFxRequest>					m_vecViewerFxRequest;
#endif SERV_IRUHADEV_JUNE_UNIT_VIEWER_EFFECT_PLAYER

};
