#include "StdAfx.h"
#include "./X2GULuciel.h"

//{{ Iruha : 2026-11-14 // char-luciel CH1a: Lu/Ciel's class skeleton ( June 32ae4080-32b4e76f )
#ifdef SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE

// char-luciel CH1a: helpers of the skill-use virtual ( June 32b3acf0 )
namespace _CONST_LUCIEL_CH1A_
{
	// June's pop-up line: font 3, white text with a black outline, centred
	void PushInfoText( const WCHAR* wszText_ )
	{
		g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, wszText_, D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
	}

	// June 31fd0570: training game ( 0x12 ) or the photo zone ( 0x1a, June-only ); the photo zone has no March state
	bool IsTrainingOrPhotozone()
	{
		return CX2Main::XS_TRAINING_GAME == g_pMain->GetNowStateID();
	}

	const int	STR_ID_NOT_ENOUGH_COMBINATION_POINT	= 33027;	// June's string 0x8103; March's StringID enum has no entry for it
	const int	RANDOM_TABLE_OFFSET_NOBILITY		= 77;		// June's GetRandomFloat offset 0x4d in UpCombinationPoint
	const float	FANTASIE_2ND_COST_RATE_AFTER_1ST	= 0.8f;		// June's double 0.8 ( 3343d750 )
	const float	COMBINATION_POINT_MP_RATE			= 10.f;		// June's double 10.0 ( 33438088 )
}
using namespace _CONST_LUCIEL_CH1A_;

// char-luciel CH1a: June 32b1bd90; June's base ctor takes a sixth argument ( 0x251, the size of its state vector ), March's state list is a map
CX2GULuciel::CX2GULuciel( int iUnitIndex_, int iTeamNum_,
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	const WCHAR* pScriptFileName,
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	int iFrameBufferNum_, IN CX2Unit* pUnit_ )
: CX2GUUser( iUnitIndex_, iTeamNum_,
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	pScriptFileName,
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	iFrameBufferNum_, pUnit_ )
, m_fComboXXXZChargeTime( 0.f )
, m_fChaserOfDarknessFireTimer( 0.f )
, m_fChaserOfDarknessPowerRate( 1.f )
, m_bOtherEquipNotRendered( false )
, m_usStateLoopCount( 0 )
, m_usSoulStrikeLoopCount( 0 )
, m_iSnipingComboCount( 0 )
, m_fChargeTime( 0.f )
, m_fDeathCrisisOfMarbasWaitTime( 0.f )
, m_bDeadHandsActive( false )
, m_iSeizedByFearSkillLevel( 0 )
, m_fSeizedByFearRate( 1.f )
, m_fSeizedByFearTimer( 0.f )
, m_iSoulCollectorSkillLevel( 0 )
, m_fPowerOfOriaxTime( 0.f )
, m_ucSoulCollectorCount( 0 )
, m_fPowerOfOriaxDamageRate( 1.f )
, m_fSoulCollectorDamageRate( 1.f )
, m_usSoulCollectorMaxCount( 0 )
, m_fSoulExtortionDrainRate( 0.f )
, m_hDemonicBreathChargeEffectSet( INVALID_EFFECTSET_HANDLE )
, m_fFantasieImpromptuWaitTime( 0.f )
, m_fDeathCrisisOfMarbasChargeTime( 0.f )
, m_fLeftHandRotateDegree( 0.f )
, m_fDeathCrisisOfMarbasMinChargeTime( 1.f )
, m_fDeathCrisisOfMarbasMaxChargeTime( 1.f )
, m_hSoulShockEffectSet( INVALID_EFFECTSET_HANDLE )
, m_fCommunionRate( 1.f )
, m_fComboXXDownXXLoopTime( 0.f )
, m_hDashComboDownXEffectSet( INVALID_EFFECTSET_HANDLE )
, m_vDashComboDownXPosition( 0.f, 0.f, 0.f )
, m_usDancingBladeLoopCount( 0 )
, m_fSummonedDarknessElapsedTime( 0.f )
, m_iLordOfFearSkillLevel( 0 )
, m_fReduceDebuffDurationRate( 1.f )
, m_fLordOfFearDamageRate( 1.f )
, m_fFearDamageEffectCoolTime( 0.f )
, m_bAuraOfFearActive( false )
, m_fAuraOfFearTime( 0.f )
, m_fAuraOfFearDuration( 0.f )
, m_fEnhancedSpecterRate( 1.f )
, m_fEnhancedSpecterRange( 1.f )
, m_fSoulStrikeChargeTime( 0.f )
, m_iShadowBoltControlIndex( -1 )
, m_fShadowBoltTime( 0.f )
, m_fShadowBoltDuration( 0.f )
, m_bTwilightJudgementReattack( false )
, m_fTwilightJudgementTime( 0.f )
, m_fSoulStrikeCoolTime( 0.f )
, m_fCombinationPointOnHitCoolTime( 0.f )
, m_hDarkCrescentEffectSet( INVALID_EFFECTSET_HANDLE )
, m_iEvilClawHitCount( 0 )
, m_fSwitchingCoolTime( 0.f )
, m_iSpecialAttackCombinationPointCost( 3 )
, m_iLastUsedSkillID( 0 )
, m_fSoulStrikeMoveSpeed( 1.f )
, m_fSoulStrikeSpeedRate( 1.f )
, m_bBoundingTargetFound( false )
, m_iBoundingTargetUnitIndex( -1 )
, m_fBoundingKeyShowTime( -1.f )
, m_pDlgBoundingKey( NULL )
, m_bPhantomRushReady( false )
, m_fPhantomRushReadyTime( -1.f )
, m_hPhantomRushEffectSet( INVALID_EFFECTSET_HANDLE )
, m_fBoundingDamageRate( -1.f )
, m_fBoundingDamageRateEx( -1.f )
, m_fDeathTouchTime( -1.f )
, m_fDeathTouchRange( 1200.f )
, m_uidDeathTouchUnit( -1 )
, m_fRapidImpactCoolTime( -1.f )
, m_fRapidImpactRange( 9999.f )
, m_fRapidImpactMaxRange( 9999.f )
, m_fInvasionOfDarkEnergyTime( -1.f )
, m_fInvasionOfDarkEnergyDuration( -1.f )
, m_iDesperadoAttackCount( 0 )
, m_iDesperadoMaxAttackCount( 0 )
, m_fMadnessCraveAttackRate( 0.f )
, m_fMadnessCraveMagicAttackRate( 0.f )
, m_fMadnessCraveDefenceRate( 0.f )
, m_fMadnessCraveMagicDefenceRate( 0.f )
, m_fMadnessCraveHpRate( 0.f )
, m_fMadnessCraveCoolTime( 9999.f )
, m_fMadnessCraveElapsedTime( 0.f )
, m_fMadnessCraveDuration( 0.f )
, m_fMadnessDestroyRate( -1.f )
, m_fMadnessDestroyCoolTime( -1.f )
, m_iShrewdCutterAttackCount( 0 )
, m_hTransformedFormEffectSet( INVALID_EFFECTSET_HANDLE )
, m_hPunisherEffectSet( INVALID_EFFECTSET_HANDLE )
, m_clockCreated( clock() )
, m_iSoulSpreadCount( 0 )
, m_fThroneAuthorityRate( 0.f )
, m_bStableDarkEnergyLocked( false )
, m_vAbyssSnipingAimPosition( 0.f, 0.f, 0.f )
, m_vAbyssSnipingAimRange( 35.f, 35.f )
, m_fCombinationPointOverflowCoolTime( 0.f )
{
	m_afSeizedByFearValue[0] = m_afSeizedByFearValue[1] = 1.f;

#ifdef SPECIAL_USE_ITEM
	m_fThrowStartTime		= 0.66f;
	m_vThrowStartPosOffset	= D3DXVECTOR3( 40.f, 130.f, 0.f );
	m_vThrowStartVelocity	= D3DXVECTOR3( 2000.f, 100.f, 0.f );
#endif SPECIAL_USE_ITEM

	InitializeLucielMajorParticleArray();
	InitializeLucielMinorParticleArray();

	// stand-in: June news a MemoConditionLuciel ( 32aeb3f0 ) into the user's +0x7d10; March has no MemoCondition ( Skill S3f: it never applies )

	m_mapScaleHeadByForm[0] = false;
	m_mapScaleHeadByForm[1] = false;
}

// char-luciel CH1a: June 32b39380
/*virtual*/ CX2GULuciel::~CX2GULuciel()
{
	CX2GUUser::DeleteGUUserMajorParticle();
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_ahLucielMajorParticleInstance[LUCIEL_MAJOR_PII_BASE] );
	DeleteLucielMinorParticle();

	KillMySummonedNPC( CX2UnitManager::NUI_SI_SA_LLD_SUMMON_DARKNESS );
	KillMySummonedNPC( CX2UnitManager::NUI_SI_SA_LLD_SUMMON_DARKNESS_PVP );
	KillMySummonedNPC( CX2UnitManager::NUI_SI_SA_LCN_SHADOW_BOLT );
	EraseBuffTempletFromGameUnit( BTI_BUFF_SUMMON_DARKNESS );
	EraseBuffTempletFromGameUnit( BTI_BUFF_SHADOW_BOLT );

	const bool bShowSwitchingGuide = false;
	g_pX2Game->UpdateLucielSwitchingGuideUI( &bShowSwitchingGuide, 0.f );

	ResetPhantomRushReady();
	EndTransformedForm( 0, false );
	EndTransformedForm( 1, false );
	ReleaseTransformedSkinAnim( 0 );
	ReleaseTransformedSkinAnim( 1 );

	if ( NULL != m_pTransformSkillSlotUI )
	{
		// stand-in: CH2d20 adds the window's dialog release ( 31fd03f0 ) before this delete
		SAFE_DELETE( m_pTransformSkillSlotUI );
	}

	KillMySummonedNPC( CX2UnitManager::NUI_SA_LDN_GARDEN_OF_HAURES );
	SAFE_DELETE_DIALOG( m_pDlgBoundingKey );
}

// char-luciel CH1a: June inlines it in the ctor @32b1c4b4 ( Chung's InitializeChungMajorParticleArray pattern )
void CX2GULuciel::InitializeLucielMajorParticleArray()
{
	CX2GUUser::InitializeGUUserMajorParticleArray();

	for ( int index = 0; index < LUCIEL_MAJOR_PII_END; index++ )
	{
		m_ahLucielMajorParticleInstance[index] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
}

// char-luciel CH1a: June inlines it in the ctor @32b1c4c1
void CX2GULuciel::InitializeLucielMinorParticleArray()
{
	CX2GUUser::InitializeGUUserMinorParticleArray();

	for ( int index = 0; index < LUCIEL_MINOR_PII_END; index++ )
	{
		m_ahLucielMinorParticleInstance[index] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
}

// char-luciel CH1a: June 32b1eb10
void CX2GULuciel::DeleteLucielMinorParticle()
{
	CX2GUUser::DeleteGUUserMinorParticle();

	CKTDGParticleSystem* pMinorParticleSystem = g_pX2Game->GetMinorParticle();

	for ( int index = 0; index < LUCIEL_MINOR_PII_END; index++ )
	{
		pMinorParticleSystem->DestroyInstanceHandle( m_ahLucielMinorParticleInstance[index] );
	}
}

// char-luciel CH1a: June InitState 32b43d10 @32b43d52-32b43d9f, the cut-in set by unit class
void CX2GULuciel::SetSkillCutInSetIndexByUnitClass()
{
	switch ( GetUnit()->GetClass() )
	{
	case CX2Unit::UC_LUCIEL_CHILIARCH:
	case CX2Unit::UC_LUCIEL_ROYAL_GUARD:
	case CX2Unit::UC_LUCIEL_DIABLURA:
	case CX2Unit::UC_LUCIEL_DUO:
		m_iSkillCutInSetIndex = 0;
		break;

	case CX2Unit::UC_LUCIEL_DREAD_LORD:
		m_iSkillCutInSetIndex = 1;
		break;

	case CX2Unit::UC_LUCIEL_NOBLESSE:
		m_iSkillCutInSetIndex = 2;
		break;

	case CX2Unit::UC_LUCIEL_DEMONIO:
		m_iSkillCutInSetIndex = 3;
		break;

	default:
		break;
	}
}

// char-luciel CH1a: June 32b14db0 ( own virtual 0x880 ): one more hit for the entry of the damage's skill and damage effect
/*virtual*/ void CX2GULuciel::CountDamageEffectHit( const CX2DamageManager::DamageData* pDamageData_ )
{
	if ( true == m_vecDamageEffectHitCount.empty() || NULL == pDamageData_ )
		return;

	if ( 0 > pDamageData_->m_iDamageEffectIndex || 0 >= pDamageData_->m_iAppliedSkillID )
		return;

	// June 32b4cb80: the first entry with the same damage effect index, then the same skill
	std::vector<SDamageEffectHitCount>::iterator vit = m_vecDamageEffectHitCount.begin();
	for ( ; vit != m_vecDamageEffectHitCount.end(); ++vit )
	{
		if ( vit->m_iDamageEffectIndex == pDamageData_->m_iDamageEffectIndex && vit->m_iSkillID == pDamageData_->m_iAppliedSkillID )
			break;
	}

	if ( vit != m_vecDamageEffectHitCount.end() )
		++vit->m_iHitCount;
}

// char-luciel CH1a: June 32ae6130 ( own virtual 0x888 ), the Init of LSI_SA_F_COMMON_AURA_DEPRAVITY_ACCEL
/*virtual*/ void CX2GULuciel::PreloadAuraAccelResources()
{
	TextureReadyInBackground( L"AURA_DEPRAVITY_ACCEL.dds" );
	TextureReadyInBackground( L"AURA_DEPRAVITY_ACCEL_S.dds" );
	TextureReadyInBackground( L"ColorBallFire.dds" );
	TextureReadyInBackground( L"LD_PUNISHING_ACCEL.dds" );
	TextureReadyInBackground( L"LD_PUNISHING_ACCEL_S.dds" );
	TextureReadyInBackground( L"SDK_AIRWAVE_T01.dds" );
	XMeshReadyInBackground( L"AA_SUPPRESSION_SPECTER_P01.Y" );
	XMeshReadyInBackground( L"ADE_PE01.Y" );
	XMeshReadyInBackground( L"Roo_highLightMagical.Y" );
}

// char-luciel CH1a: June 32aeb1e0: the MP of the active form's gauge, or of the other form's when bCurrentForm_ is false
void CX2GULuciel::UpGageNowMpOfForm( const float fChangeOfNowMp_, const bool bCurrentForm_ )
{
	const int iActiveForm = ( true == IsMinorUnitActive() ? 1 : 0 );
	int iForm = iActiveForm;
	if ( false == bCurrentForm_ )
		iForm = ( 0 == iActiveForm ? 1 : 0 );

	if ( NULL == GetGageData() )
		return;

	if ( 3 == iForm )	// June's test for a form 3, which no path makes; kept
		iForm = iActiveForm;

	GetGageData()->UpNowMp( fChangeOfNowMp_, true, iForm );
}

// char-luciel CH1a: June 32b0fa00
int CX2GULuciel::GetNowCombinationPoint() const
{
	const CX2LucielGageData* pLucielGageData = static_cast<const CX2LucielGageData*>( GetGageData() );
	if ( NULL == pLucielGageData )
		return 0;

	return pLucielGageData->GetNowCombinationPoint();
}

// char-luciel CH1a: June 32b14860: combination points up or down, with Nobility's and Communion Absorb's bonuses and refunds
void CX2GULuciel::UpCombinationPoint( const int iCombinationPoint_ )
{
	if ( 0 < iCombinationPoint_ )	// spec A as Eve's port reads June's +0x6c50
		UpNowMpWithText( static_cast<float>( iCombinationPoint_ * static_cast<float>( GetCharacterSpecializationA() + 1.0 ) * COMBINATION_POINT_MP_RATE ) );

	int iBonusPoint = 0;
	CX2UserSkillTree& accessUserSkillTree = GetUnit()->AccessUnitData().m_UserSkillTree;

	const int iNobilityLevel = accessUserSkillTree.GetSkillLevel( CX2SkillTree::RNSI_P_LCN_NOBILITY, false, 0, true );
	if ( 0 < iNobilityLevel )
	{
		const CX2SkillTree::SkillTemplet* pNobilityTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::RNSI_P_LCN_NOBILITY );
		if ( NULL != pNobilityTemplet )
		{
			if ( 0 < iCombinationPoint_ )
			{
				const double dRandom = GetRandomFloat( RANDOM_TABLE_OFFSET_NOBILITY );
				if ( dRandom < pNobilityTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iNobilityLevel, false ) )
					iBonusPoint = 1;
			}

			if ( 0 > iCombinationPoint_ )
			{
				if ( -10 == iCombinationPoint_ && GetRandomFloat( 0 ) < GetCharacterSpecializationB() )	// June's literal -10
					iBonusPoint += 2;

				const float fCoolTimeChange = static_cast<float>( pNobilityTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TIME_ABS, iNobilityLevel, false ) * iCombinationPoint_ );
				for ( int iSlot = 0; iSlot < 10; ++iSlot )
				{
					const CX2UserSkillTree::SkillSlotData* pSkillSlotData = accessUserSkillTree.GetSkillSlot( iSlot % 5, 4 < iSlot );
					if ( NULL == pSkillSlotData )
						continue;

					const CX2SkillTree::SkillTemplet* pSlotSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSkillSlotData->m_eID );
					if ( NULL == pSlotSkillTemplet || CX2SkillTree::ST_HYPER_ACTIVE_SKILL == pSlotSkillTemplet->m_eType
						|| CX2SkillTree::ST_RELATIONSHIP_SKILL == pSlotSkillTemplet->m_eType )
						continue;

					float fCoolTimeLeft = 0.f;
					if ( 0.f <= pSkillSlotData->m_fCoolTimeLeft - fCoolTimeChange )
						fCoolTimeLeft = pSkillSlotData->m_fCoolTimeLeft - fCoolTimeChange;

					accessUserSkillTree.SetSkillCoolTimeLeft( pSlotSkillTemplet->m_eID, fCoolTimeLeft, true );
				}
			}
		}
	}

	CX2LucielGageData* pLucielGageData = static_cast<CX2LucielGageData*>( GetGageData() );

	const int iCommunionAbsorbLevel = accessUserSkillTree.GetSkillLevel( CX2SkillTree::RNSI_P_LDB_COMMUNION_ABSORB, false, 0, true );
	if ( 0 < iCommunionAbsorbLevel )
	{
		const CX2SkillTree::SkillTemplet* pCommunionAbsorbTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::RNSI_P_LDB_COMMUNION_ABSORB );
		if ( NULL != pCommunionAbsorbTemplet && 0 < iCombinationPoint_ )
		{
			const double dRandom = GetRandomFloat( 0 );
			if ( dRandom < pCommunionAbsorbTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iCommunionAbsorbLevel, false ) )
				++iBonusPoint;

			if ( NULL != pLucielGageData )
			{
				const int iOverflowPoint = ( pLucielGageData->GetNowCombinationPoint() - pLucielGageData->GetMaxCombinationPoint() ) + iBonusPoint + iCombinationPoint_;
				if ( 0 < iOverflowPoint && m_fCombinationPointOverflowCoolTime <= 0.f )
				{
					UpGageNowMpOfForm( pCommunionAbsorbTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_REGENERATION_ABS, iCommunionAbsorbLevel, false ) * iOverflowPoint, false );
					m_fCombinationPointOverflowCoolTime = pCommunionAbsorbTemplet->GetSkillAbilityValue( CX2SkillTree::SA_COOLTIME_1, iCommunionAbsorbLevel, false );
				}
			}
		}
	}

	if ( NULL != pLucielGageData )
	{
		pLucielGageData->SetCombinationPointChanged( true );
		pLucielGageData->SetNowCombinationPoint( pLucielGageData->GetNowCombinationPoint() + iBonusPoint + iCombinationPoint_ );
	}
}

// char-luciel CH1a: June 32b14f70: a skill's combination point cost ( the templet's FP consumption field )
int CX2GULuciel::GetSkillCombinationPointCost( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ )
{
	// June passes the level but reads its own; kept
	if ( 0 >= GetUnit()->AccessUnitData().m_UserSkillTree.GetSkillLevel( eSkillID_, false, 0, true ) )
		return 0;

	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID_ );
	if ( NULL == pSkillTemplet )
		return 0;

	const int iCost = pSkillTemplet->m_usFPConsumtion;

	switch ( g_pData->GetSkillTree()->GetUpgradeBaseSkillID( eSkillID_ ) )
	{
	case CX2SkillTree::RNSI_SA_LCN_FANTASIE_IMPROMPTU_2ND_MOVEMENT:
		if ( CX2SkillTree::RNSI_SA_LCG_FANTASIE_IMPROMPTU_1ST_MOVEMENT == g_pData->GetSkillTree()->GetUpgradeBaseSkillID( GetNowStateSkillID() ) )
			return static_cast<int>( iCost * FANTASIE_2ND_COST_RATE_AFTER_1ST );
		return iCost;

	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH:
	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH_UPGRADE:
		if ( LSI_A_LDB_F_PHANTOM_RUSH_TARGET == GetNowStateID() || LSI_A_LDB_M_PHANTOM_RUSH_TARGET == GetNowStateID() )
			return iCost;
		return 0;

	default:
		return iCost;
	}
}

// char-luciel CH1a: June 32b20100: kills my living ally NPCs of that id ( NUI_NONE = any )
void CX2GULuciel::KillMySummonedNPC( const CX2UnitManager::NPC_UNIT_ID eNPCUnitID_ )
{
	if ( NULL == g_pX2Game )
		return;

	for ( int iUnit = 0; iUnit < g_pX2Game->GetUnitNum(); ++iUnit )
	{
		CX2GameUnit* pGameUnit = g_pX2Game->GetUnit( iUnit );
		if ( NULL == pGameUnit || CX2GameUnit::GUT_NPC != pGameUnit->GetGameUnitType() )
			continue;

		CX2GUNPC* pNPC = static_cast<CX2GUNPC*>( pGameUnit );
		if ( CX2UnitManager::NUI_NONE != eNPCUnitID_ && eNPCUnitID_ != pNPC->GetUnitID() )
			continue;

		if ( NULL != pNPC->GetGageData() && false == ( 0.f < pNPC->GetNowHp() ) )
			continue;

		CX2AllyNPCAI* pAllyAI = static_cast<CX2AllyNPCAI*>( pNPC->GetNPCAI() );
		if ( NULL == pAllyAI || CX2NPCAI::NAT_ALLY != pAllyAI->GetAIType() || GetUnitUID() != pAllyAI->GetAllyUnitUID() )
			continue;

		pNPC->SetNowHp( 0.f );
	}
}

// char-luciel CH1a: June 32b34920: Phantom Rush's second use ends
void CX2GULuciel::ResetPhantomRushReady()
{
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hPhantomRushEffectSet );
	m_fPhantomRushReadyTime = -1.f;
	m_bPhantomRushReady = false;
	m_PhantomRushTarget = SGameUnitKey();

	if ( true == IsMyUnit() )
		g_pX2Game->UpdateSkillSlotUI( false );

	if ( 0 < GetUnit()->AccessUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::RNSI_P_LDN_STABLE_DARKENERGY, false, 0, true ) )
		m_bStableDarkEnergyLocked = false;
}

// char-luciel CH1a: June 32b35540: Altar of Evil's effect sets stop, both lists empty
void CX2GULuciel::ClearAltarOfEvilEffectSet()
{
	for ( std::vector<CX2EffectSet::Handle>::iterator vit = m_AltarOfEvilData.m_vecEffectSet.begin(); vit != m_AltarOfEvilData.m_vecEffectSet.end(); ++vit )
	{
		g_pX2Game->GetEffectSet()->StopEffectSet( *vit );
	}

	m_AltarOfEvilData.m_vecEffectSet.clear();
	m_AltarOfEvilData.m_vecPosition.clear();
}

// char-luciel CH1a: June 32aeaa10: the skill states the slot does not start ( mostly Ciel's ); true when it changed state
bool CX2GULuciel::StateChangeBySkillID( const CX2SkillTree::SKILL_ID eSkillID_ )
{
	switch ( g_pData->GetSkillTree()->GetUpgradeBaseSkillID( eSkillID_ ) )
	{
	case CX2SkillTree::RNSI_SA_LLC_RUINOUS_IMPACT:
	case CX2SkillTree::RNSI_SA_LLC_RUINOUS_IMPACT_UPGRADE:
		StateChange( LSI_SA_LLC_RUINOUS_IMPACT, false );
		return true;

	case CX2SkillTree::RNSI_SA_LLC_DARK_CRESCENT:
		StateChange( LSI_SA_LLC_DARK_CRESCENT, false );
		return true;

	case CX2SkillTree::RNSI_SA_LCG_ESCHATON:
		StateChange( LSI_SA_LCG_ESCHATON, false );
		return true;

	case CX2SkillTree::RNSI_SA_LCN_FANTASIE_IMPROMPTU_2ND_MOVEMENT:
		StateChange( LSI_SA_LCG_FANTASIE_IMPROMPTU_2ND_MOVEMENT, false );
		return true;

	case CX2SkillTree::RNSI_SA_LCN_TWILIGHT_JUDGEMENT:
		if ( true == HaveThisBuff( BTI_BUFF_TWILIGHT_JUDGEMENT ) )
		{
			StateChange( true == IsMinorUnitActive() ? LSI_SA_LCN_M_TWILIGHT_JUDGEMENT_REATTACK : LSI_SA_LCN_F_TWILIGHT_JUDGEMENT_REATTACK, false );
			return true;
		}
		return false;

	case CX2SkillTree::RNSI_SA_LCN_MAGDONIC_CANNON:
		if ( false == m_FrameDataFuture.unitCondition.bFootOnLine )
		{
			StateChange( LSI_SA_LCN_MAGDONIC_CANNON_FLY, false );
			return true;
		}
		return false;

	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH:
	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH_UPGRADE:
		if ( true == m_bPhantomRushReady )
		{
			StateChange( true == IsMinorUnitActive() ? LSI_A_LDB_F_PHANTOM_RUSH_TARGET : LSI_A_LDB_M_PHANTOM_RUSH_TARGET, true );
			return true;
		}
		if ( true == IsMinorUnitActive() )
		{
			StateChange( LSI_A_LDB_M_PHANTOM_RUSH, false );
			return true;
		}
		return false;

	default:
		break;
	}

	if ( false == IsMinorUnitActive() )
		return false;

	// Ciel's state of the skill; every case below tests the minor unit first in June
	switch ( g_pData->GetSkillTree()->GetUpgradeBaseSkillID( eSkillID_ ) )
	{
	case CX2SkillTree::SI_ETC_WS_COMMON_LOVE:
		StateChange( 263, false );	// no June name: the LSI_CIEL_THROW_ITEM table re-registered by the skill setter
		return true;

	case CX2SkillTree::SI_SA_COMMON_AURA_DARKNESS_ACCEL:
		StateChange( LSI_SA_M_COMMON_AURA_DEPRAVITY_ACCEL, false );
		return true;

	case CX2SkillTree::SI_A_LD_CHASER_OF_DARKNESS:
		StateChange( 268, false );	// no June name: an id June never registers
		return true;

	case CX2SkillTree::SI_A_LCG_SOUL_SHOCK:
		StateChange( LSI_A_LCG_M_SOUL_SHOCK, false );
		return true;

	case CX2SkillTree::RNSI_A_LUCIEL_HEAVY_HIT:
	case CX2SkillTree::RNSI_A_LUCIEL_HEAVY_HIT_UPGRADE:
		StateChange( LSI_A_LD_M_HEAVY_HIT, false );
		return true;

	case CX2SkillTree::RNSI_A_LUCIEL_HAUNTING_BEAT:
	case CX2SkillTree::RNSI_A_LUCIEL_HAUNTING_BEAT_UPGRADE:
		StateChange( LSI_A_LD_M_HAUNTING_BEAT, false );
		return true;

	case CX2SkillTree::RNSI_A_LLC_BLASTING_FEAR:
	case CX2SkillTree::RNSI_A_LLC_BLASTING_FEAR_UPGRADE:
		StateChange( LSI_A_LLC_M_BLASTING_FEAR, false );
		return true;

	case CX2SkillTree::RNSI_A_LLD_FEAR_ABSORPTION:
		StateChange( LSI_A_LLD_M_FEAR_ABSORPTION, false );
		return true;

	case CX2SkillTree::RNSI_A_LLC_BACKSTAB:
		StateChange( LSI_A_LLC_M_BACKSTAB, false );
		return true;

	case CX2SkillTree::RNSI_A_LLD_CHAINS_OF_BONDAGE:
		StateChange( LSI_A_LLD_M_CHAINS_OF_BONDAGE, false );
		return true;

	case CX2SkillTree::RNSI_A_LCG_SOUL_STRIKE:
	case CX2SkillTree::RNSI_A_LCG_SOUL_STRIKE_UPGRADE:
		StateChange( LSI_A_LCG_M_SOUL_STRIKE_READY, false );
		return true;

	case CX2SkillTree::RNSI_A_LCN_SUMMON_FAMILIAR:
		StateChange( LSI_A_LCN_M_SUMMON_FAMILIAR, false );
		return true;

	case CX2SkillTree::RNSI_A_LCG_SHADOW:
		StateChange( LSI_A_LCN_M_SHADOW, false );
		return true;

	case CX2SkillTree::RNSI_A_LCN_SWIFT_SMASH:
		StateChange( LSI_SA_LCN_M_SWIFT_SMASH, false );
		return true;

	case CX2SkillTree::RNSI_A_LDB_ABYSS_STRIKE:
	case CX2SkillTree::RNSI_A_LDB_ABYSS_STRIKE_UPGRADE:
		StateChange( LSI_A_LDB_M_ABYSS_STRIKE, false );
		return true;

	case CX2SkillTree::RNSI_A_LDN_RAPID_IMPACT:
		StateChange( LSI_A_LDN_M_RAPID_IMPACT, false );
		return true;

	case CX2SkillTree::RNSI_A_LDN_PUNISHER:
		StateChange( LSI_A_LDN_M_PUNISHER, false );
		return true;

	case CX2SkillTree::RNSI_A_LUCIEL_AURA_PUNISHMENT_ACCEL:
	case CX2SkillTree::RNSI_A_LUCIEL_AURA_PUNISHMENT_ACCEL_UPGRADE:
		StateChange( LSI_SA_M_COMMON_AURA_PUNISHING_ACCEL, false );
		return true;

	case CX2SkillTree::RNSI_BUFF_PHANTOM_CIRCLE:
		StateChange( LSI_SA_M_COMMON_BUFF_PHANTOM_CIRCLE, false );
		return true;

	case CX2SkillTree::RNSI_A_LLD_BLUFF:
		StateChange( LSI_A_LLD_BLUFF_CIEL, false );
		return true;

	case CX2SkillTree::RNSI_A_LCN_SOUL_SPREAD:
		StateChange( LSI_A_LCN_SOUL_SPREAD_CIEL_READY, false );
		return true;

	case CX2SkillTree::RNSI_A_LDN_RAPID_FURY:
		StateChange( LSI_A_LDN_RAPID_FURY_CIEL, false );
		return true;

	default:
		return false;
	}
}

// char-luciel CH1a: June 32b3acf0 ( own virtual 0x884 ): a skill used outside the slot keys, paid with MP and combination points
/*virtual*/ void CX2GULuciel::TryUseSkillWithCombinationPoint( const CX2SkillTree::SKILL_ID eSkillID_,
	const CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition_ )
{
	if ( false == ( m_fQuickSlotSpecialItemWaitTime <= 0.f ) )
		return;

	CX2UserSkillTree& accessUserSkillTree = GetUnit()->AccessUnitData().m_UserSkillTree;
	const CX2UserSkillTree::SkillSlotData* pSkillSlotData = accessUserSkillTree.GetSkillSlot( eSkillID_ );

	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID_ );
	if ( NULL == pSkillTemplet
		|| false == CheckSkillUseCondition( eActiveSkillUseCondition_, pSkillTemplet->m_eActiveSkillUseCondtion )
		|| false == CheckSkillUsePVPOfficial( eSkillID_ )
		|| false == CheckSkillUseNotPve( eSkillID_ )
		|| false == CheckSkillUseNotDungeon( eSkillID_ ) )
		return;

	const int iSkillLevel = accessUserSkillTree.GetSkillLevel( eSkillID_, false, 0, true );
	if ( 0 >= iSkillLevel )
		return;

	const float fMpConsume = GetActualMPConsume( eSkillID_, iSkillLevel );
	const int iCombinationPointCost = GetSkillCombinationPointCost( eSkillID_, iSkillLevel );

	if ( false == IsDevUnlimitedSkillUse() )
	{
		if ( NULL != pSkillSlotData && false == ( pSkillSlotData->m_fCoolTimeLeft <= 0.f ) )
		{
			PushInfoText( GET_STRING( STR_ID_226 ) );
			return;
		}

		if ( false == ( fMpConsume <= GetNowMp() ) )
		{
			PushInfoText( GET_STRING( STR_ID_2549 ) );
			return;
		}
	}

	if ( iCombinationPointCost > GetNowCombinationPoint() )
	{
		const int iStringID = STR_ID_NOT_ENOUGH_COMBINATION_POINT;
		PushInfoText( GET_STRING( iStringID ) );
		return;
	}

	if ( CX2SkillTree::ST_HYPER_ACTIVE_SKILL == pSkillTemplet->m_eType && false == IsTrainingOrPhotozone()
		&& 1 > g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE ) )
	{
		PushInfoText( GET_STRING( STR_ID_26119 ) );
		return;
	}

	const float fCoolTime = GetActualCoolTime( pSkillTemplet, iSkillLevel );
	if ( false == ( fabs( -1.f - fCoolTime ) <= FLT_EPSILON ) )	// June 31c7b640: a cool time of -1 is not stored
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, fCoolTime, true );

	UpNowMp( -fMpConsume );
	UpCombinationPoint( -iCombinationPointCost );

	if ( CX2SkillTree::ST_HYPER_ACTIVE_SKILL == pSkillTemplet->m_eType && CX2Main::XS_TRAINING_GAME != g_pMain->GetNowStateID() )
	{
		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );
		if ( NULL != pItem )
			g_pX2Game->Handler_EGS_USE_FINALITY_SKILL_REQ( pItem->GetItemData().m_ItemUID );
	}

	if ( true == SpecialAttackNoStageChange( pSkillTemplet ) || true == StateChangeBySkillID( pSkillTemplet->m_eID ) )
		CX2RandomMissionManager::GetInstance()->OnSkillUse( pSkillTemplet->m_eID );
}

#endif SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE
//}}
