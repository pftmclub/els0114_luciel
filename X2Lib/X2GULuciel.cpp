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
	const double	FANTASIE_2ND_COST_RATE_AFTER_1ST	= 0.8;		// char-luciel CH1a review: June's double 0.8 ( 3343d750 )
	const double	COMBINATION_POINT_MP_RATE			= 10.0;		// char-luciel CH1a review: June's double 10.0 ( 33438088 )
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
	if ( 0 < iCombinationPoint_ )	// char-luciel CH1a review: June's raw +0x6c50 ( spec A ) with no + 1, unlike Eve's 3297ee20 ( JUNE_BUGS 688 )
		UpNowMpWithText( static_cast<float>( iCombinationPoint_ * static_cast<double>( GetCharacterSpecializationA() ) * COMBINATION_POINT_MP_RATE ) );

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

// char-luciel CH1a4: June 32b13d30: the form's transformed set closed and freed
void CX2GULuciel::ReleaseTransformedSkinAnim( const int iForm_ )
{
	STransformedSkin& transformedSkin = m_aTransformedSkin[0 != iForm_ ? 1 : 0];

	if ( NULL != transformedSkin.m_pMotionFuture && NULL != g_pKTDXApp->GetDeviceManager() )
	{
		g_pKTDXApp->GetDeviceManager()->CloseDevice( transformedSkin.m_pMotionFuture->GetDeviceID() );
		transformedSkin.m_pMotionFuture = NULL;
	}

	if ( NULL != transformedSkin.m_pMotion && NULL != g_pKTDXApp->GetDeviceManager() )
	{
		g_pKTDXApp->GetDeviceManager()->CloseDevice( transformedSkin.m_pMotion->GetDeviceID() );
		transformedSkin.m_pMotion = NULL;
	}

	SAFE_DELETE_KTDGOBJECT( transformedSkin.m_pXSkinAnimFuture );
	SAFE_DELETE_KTDGOBJECT( transformedSkin.m_pAfterImage );
	transformedSkin.m_pXSkinAnim.reset();
}

// char-luciel CH1a4: June 32b13e20: the form's body and the set trade motions, anims and after-image
void CX2GULuciel::SetFormSkinAnim( const int iForm_, STransformedSkin& transformedSkin_ )
{
	CKTDXDeviceXSkinMesh*& pMotionFuture		= 0 != iForm_ ? m_pMinorUnitMotionFuture : m_pMotionFuture;
	CKTDXDeviceXSkinMesh*& pMotion				= 0 != iForm_ ? m_pMinorUnitMotion : m_pMotion;
	CKTDGXSkinAnim*& pXSkinAnimFuture			= 0 != iForm_ ? m_pMinorUnitXSkinAnimFuture : m_pXSkinAnimFuture;
	CKTDGXSkinAnimPtr& pXSkinAnim				= 0 != iForm_ ? m_pMinorUnitXSkinAnim : m_pXSkinAnim;
	CKTDGXSkinAfterImage*& pAfterImage			= 0 != iForm_ ? m_pMinorUnitAfterImage : m_pAfterImage;

	CKTDXDeviceXSkinMesh* const pBodyMotion			= pMotion;
	CKTDXDeviceXSkinMesh* const pBodyMotionFuture	= pMotionFuture;
	CKTDGXSkinAnim* const pBodyXSkinAnimFuture		= pXSkinAnimFuture;
	CKTDGXSkinAnimPtr pBodyXSkinAnim				= pXSkinAnim;
	CKTDGXSkinAfterImage* const pBodyAfterImage		= pAfterImage;

	bool bAfterImageEnabled = false;
	if ( NULL != pBodyAfterImage )
	{
		bAfterImageEnabled = pBodyAfterImage->GetEnable();
		if ( true == bAfterImageEnabled )
		{
			pBodyAfterImage->SetShowAfterImage( false );
			DisableAfterImage( true, AIB_ACTIVE_UNIT );
		}
	}

	pMotion				= transformedSkin_.m_pMotion;
	pMotionFuture		= transformedSkin_.m_pMotionFuture;
	pXSkinAnim			= transformedSkin_.m_pXSkinAnim;
	pXSkinAnimFuture	= transformedSkin_.m_pXSkinAnimFuture;
	pAfterImage			= transformedSkin_.m_pAfterImage;
	if ( NULL != pAfterImage && true == bAfterImageEnabled )
		EnableAfterImage( AIB_ACTIVE_UNIT );

	transformedSkin_.m_pMotion			= pBodyMotion;
	transformedSkin_.m_pMotionFuture	= pBodyMotionFuture;
	transformedSkin_.m_pXSkinAnim		= pBodyXSkinAnim;
	transformedSkin_.m_pXSkinAnimFuture	= pBodyXSkinAnimFuture;
	transformedSkin_.m_pAfterImage		= pBodyAfterImage;
	transformedSkin_.m_bSwapped			= !transformedSkin_.m_bSwapped;
}

// char-luciel CH1a4: June 32b38f30: the form's own skin back, equipment shown, Altar of Evil / Transcendence ended
void CX2GULuciel::EndTransformedForm( const int iForm_, const bool bByDie_ )
{
	const BUFF_TEMPLET_ID eTransformBuffID = 0 != iForm_ ? BTI_BUFF_TRANSCENDENCE : BTI_BUFF_ALTAR_OF_EVIL;

	if ( true == IsTransformed( iForm_ ) )
	{
		SetTransformed( iForm_, false );

		STransformedSkin& transformedSkin = m_aTransformedSkin[0 != iForm_ ? 1 : 0];
		if ( false == transformedSkin.m_bSwapped )
			return;	// June quirk: skips the effect-set and buff clears below ( JUNE_BUGS 689 )

		SetFormSkinAnim( iForm_, transformedSkin );

		CKTDGXSkinAnimPtr pXSkinAnim = 0 != iForm_ ? m_pMinorUnitXSkinAnim : m_pXSkinAnim;
		CKTDXDeviceXSkinMesh::MultiAnimFrame*& pFrameHead = 0 != iForm_ ? m_pFrame_MinorUnit_Bip01_Head : m_pFrame_Bip01_Head;
		m_CollisionListSet.clear();
		m_AttackListSet.clear();
		pFrameHead = pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
		SetShowViewList( true );

		BOOST_TEST_FOREACH( Weapon*, pWeapon, m_vecpWeapon )
		{
			if ( NULL == pWeapon || NULL == pWeapon->m_pUserWeaponEquip )
				continue;

			for ( int iModel = 0; iModel < CX2Item::MAX_MODEL_COUNT_A_ITEM; ++iModel )
			{
				if ( NULL != pWeapon->m_pUserWeaponEquip->GetNormalAnim( iModel ) )
					pWeapon->m_pUserWeaponEquip->GetNormalAnim( iModel )->SetShowObject( true );

				if ( NULL != pWeapon->m_pUserWeaponEquip->GetHyperAnim( iModel ) )
					pWeapon->m_pUserWeaponEquip->GetHyperAnim( iModel )->SetShowObject( true );
			}
		}

		UpdateEquipCollisionData();
		UpdateShowViewListByActiveBody();

		if ( true == bByDie_ )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_LDB_TRANSFORM_END_BY_DIE", this );

		if ( true == IsMyUnit() )
		{
			if ( NULL != m_pTransformSkillSlotUI )
			{
				// stand-in: CH2d20 adds the window's dialog release ( 31fd03f0 ) before this delete
				SAFE_DELETE( m_pTransformSkillSlotUI );
			}

			const int iCombinationPoint = static_cast<int>( static_cast<int>( GetBuffTimeFinalizerDuration( eTransformBuffID ) ) / 6.0 );
			if ( 0 < iCombinationPoint )
				UpCombinationPoint( iCombinationPoint );
		}
	}

	ClearAltarOfEvilEffectSet();
	EraseBuffTempletFromGameUnit( eTransformBuffID );
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hTransformedFormEffectSet );
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

// char-luciel CH1b: Lu's common states ( June 32ae5540 - 32b3a270 ); the constants are June's immediates
namespace _CONST_LUCIEL_CH1B_
{
	const float		STAND_UP_INVINCIBLE_TIME				= 0.3f;		// June 3345993c
	const float		SWITCHING_COOL_TIME_AFTER_START			= 1.f;		// June 32ae5920 stores 0x3f800000
	const float		DASH_END_STEP_DUST_TIME					= 0.4f;		// June 33446fa0
	const float		DAMAGE_DOWN_FRONT_STEP_DUST_TIME		= 0.19f;	// June 33475a18
	const float		DAMAGE_DOWN_BACK_STEP_DUST_TIME			= 0.32f;	// June 334ad0ac
	const float		DAMAGE_DOWN_STAND_UP_INPUT_TIME			= 0.33f;	// June 3343bd70 ( the double of 0.33f )
	const float		DASH_END_COMBO_INPUT_RATE				= 0.125f;	// June 33439ba8
	const double	DASH_JUMP_POWER_LANDING_INPUT_DIVISOR	= 6.0;		// June 3343a468
	const float		DASH_JUMP_POWER_LANDING_SPEED_RATE		= 0.5f;		// June 3342ebc8
	const float		DIE_PLAY_SPEED							= 0.5f;		// June 3343777c
}
using namespace _CONST_LUCIEL_CH1B_;

// char-luciel CH1b: June 31fd2d60 ( CX2GUUser's shared DIE_FRONT start; March's CX2GUEL::DieFrontStart )
void CX2GULuciel::DieFrontStart()
{
	CommonStateStart();
	DyingStart();

	GetXSkinAnimPtr()->SetPlaySpeed( DIE_PLAY_SPEED );
}

// char-luciel CH1b: June 31fd2e30 ( identical to 31fd2d60 )
void CX2GULuciel::DieBackStart()
{
	CommonStateStart();
	DyingStart();

	GetXSkinAnimPtr()->SetPlaySpeed( DIE_PLAY_SPEED );
}

// char-luciel CH1b: June 32ae5540
void CX2GULuciel::LSI_LU_JUMP_UP_EventProcess()
{
	if( true == m_InputData.pureDoubleLeft || true == m_InputData.pureDoubleRight )
	{
		if( true == GetEnableDash( EDS_END ) )
		{
			StateChange( LSI_LU_DASH_JUMP );
		}
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_LU_JUMP_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_LU_JUMP_COMBO_X );
	}
	else if( 0.f < m_PhysicParam.nowSpeed.y )
	{
		if( true == GetRidingOn() )
		{
			StateChange( LSI_RIDING_ON );
		}
	}
	else
	{
		StateChange( LSI_LU_JUMP_DOWN );
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32ae55e0
void CX2GULuciel::LSI_LU_DASH_JUMP_POWER_LANDING_StateStartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x *= DASH_JUMP_POWER_LANDING_SPEED_RATE;
}

// char-luciel CH1b: June 32ae5610
void CX2GULuciel::LSI_LU_DASH_JUMP_POWER_LANDING_StateStart()
{
	CommonStateStart();
	CreateStepDust();
}

// char-luciel CH1b: June 32ae5630
void CX2GULuciel::LSI_LU_STANDUP_ROLLING_FRONT_FRONT_StateEnd()
{
	CommonStateEnd();
	SetForceInvincible( STAND_UP_INVINCIBLE_TIME );
	SetShowInvincible( STAND_UP_INVINCIBLE_TIME );
}

// char-luciel CH1b: June 32ae5670
void CX2GULuciel::LSI_LU_STANDUP_ROLLING_FRONT_BACK_StateEnd()
{
	CommonStateEnd();
	SetForceInvincible( STAND_UP_INVINCIBLE_TIME );
	SetShowInvincible( STAND_UP_INVINCIBLE_TIME );
}

// char-luciel CH1b: June 32ae56b0
void CX2GULuciel::LSI_LU_STANDUP_ROLLING_BACK_FRONT_StateEnd()
{
	CommonStateEnd();
	SetForceInvincible( STAND_UP_INVINCIBLE_TIME );
	SetShowInvincible( STAND_UP_INVINCIBLE_TIME );
}

// char-luciel CH1b: June 32ae56f0
void CX2GULuciel::LSI_LU_STANDUP_ROLLING_BACK_BACK_StateEnd()
{
	CommonStateEnd();
	SetForceInvincible( STAND_UP_INVINCIBLE_TIME );
	SetShowInvincible( STAND_UP_INVINCIBLE_TIME );
}

// char-luciel CH1b: June 32ae5730
void CX2GULuciel::LSI_LU_STANDUP_ATTACK_FRONT_StateEnd()
{
	CommonStateEnd();
	SetForceInvincible( STAND_UP_INVINCIBLE_TIME );
	SetShowInvincible( STAND_UP_INVINCIBLE_TIME );
}

// char-luciel CH1b: June 32ae5770
void CX2GULuciel::LSI_LU_STANDUP_ATTACK_BACK_StateEnd()
{
	CommonStateEnd();
	SetForceInvincible( STAND_UP_INVINCIBLE_TIME );
	SetShowInvincible( STAND_UP_INVINCIBLE_TIME );
}

// char-luciel CH1b: June 32ae58a0
void CX2GULuciel::LSI_LU_STANDUP_ATTACK_FRONT_NO_DOWN_StateEnd()
{
	CommonStateEnd();
	SetForceInvincible( STAND_UP_INVINCIBLE_TIME );
	SetShowInvincible( STAND_UP_INVINCIBLE_TIME );
}

// char-luciel CH1b: June 32ae58e0
void CX2GULuciel::LSI_LU_STANDUP_ATTACK_BACK_NO_DOWN_StateEnd()
{
	CommonStateEnd();
	SetForceInvincible( STAND_UP_INVINCIBLE_TIME );
	SetShowInvincible( STAND_UP_INVINCIBLE_TIME );
}

// char-luciel CH1b: June 32ae5920
void CX2GULuciel::LSI_LU_SWITCHING_StateStartFuture()
{
	CommonStateStartFuture();
	m_fSwitchingCoolTime = SWITCHING_COOL_TIME_AFTER_START;
}

// char-luciel CH1b: June 32aebc10 ( the active body's future anim )
void CX2GULuciel::LSI_LU_WAIT_StateStartFuture()
{
	CommonStateStartFuture();
	PlayAnimationWaitStart( GetXSkinAnimFuture(), m_FrameDataFuture );
}

// char-luciel CH1b: June 32aebc60
void CX2GULuciel::LSI_LU_JUMP_DOWN_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_LANDING );
	}
	else if( true == m_InputData.pureDoubleLeft || true == m_InputData.pureDoubleRight )
	{
		if( true == GetEnableDash( EDS_END ) )
		{
			StateChange( LSI_LU_DASH_JUMP );
		}
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_LU_JUMP_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_LU_JUMP_COMBO_X );
	}
	else if( 0.f > m_PhysicParam.nowSpeed.y )
	{
		if( true == GetRidingOn() )
		{
			StateChange( LSI_RIDING_ON );
		}
	}
	else
	{
		StateChange( LSI_LU_JUMP_UP );
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32aebd10 ( the thunk 32aec050 is the same body )
void CX2GULuciel::LSI_LU_DAMAGE_DOWN_FRONT_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( true == m_FrameDataFuture.stateParam.bEventFlagList[1] )
		{
			StateChange( LSI_LU_STANDUP_ATTACK_FRONT );
		}
		else if( true == m_FrameDataFuture.stateParam.bEventFlagList[4] )
		{
			if( GetRandomInt() < m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent )
			{
				StateChange( LSI_LU_STANDUP_ATTACK_FRONT_NO_DOWN );
			}
			else
			{
				StateChange( LSI_LU_STANDUP_ATTACK_FRONT );
			}
		}
		else if( true == m_FrameDataFuture.stateParam.bEventFlagList[2] )
		{
			StateChange( LSI_LU_STANDUP_ROLLING_FRONT_FRONT );
		}
		else if( true == m_FrameDataFuture.stateParam.bEventFlagList[3] )
		{
			StateChange( LSI_LU_STANDUP_ROLLING_FRONT_BACK );
		}
		else
		{
			StateChange( LSI_LU_DAMAGE_STANDUP_FRONT );
		}
	}
	else if( GetXSkinAnimFuture()->GetNowAnimationTime() > DAMAGE_DOWN_STAND_UP_INPUT_TIME )
	{
		if( true == m_InputData.oneZ )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
			m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
		}
		else if( true == m_InputData.oneX )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[4] = true;
		}
		else if( true == m_FrameDataFuture.syncData.bIsRight )
		{
			if( true == m_InputData.pureRight )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
			else if( true == m_InputData.pureLeft )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
		}
		else
		{
			if( true == m_InputData.pureRight )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
			else if( true == m_InputData.pureLeft )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
		}
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32aebeb0 ( = 32aebd10 with the back-side states )
void CX2GULuciel::LSI_LU_DAMAGE_DOWN_BACK_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( true == m_FrameDataFuture.stateParam.bEventFlagList[1] )
		{
			StateChange( LSI_LU_STANDUP_ATTACK_BACK );
		}
		else if( true == m_FrameDataFuture.stateParam.bEventFlagList[4] )
		{
			if( GetRandomInt() < m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent )
			{
				StateChange( LSI_LU_STANDUP_ATTACK_BACK_NO_DOWN );
			}
			else
			{
				StateChange( LSI_LU_STANDUP_ATTACK_BACK );
			}
		}
		else if( true == m_FrameDataFuture.stateParam.bEventFlagList[2] )
		{
			StateChange( LSI_LU_STANDUP_ROLLING_BACK_FRONT );
		}
		else if( true == m_FrameDataFuture.stateParam.bEventFlagList[3] )
		{
			StateChange( LSI_LU_STANDUP_ROLLING_BACK_BACK );
		}
		else
		{
			StateChange( LSI_LU_DAMAGE_STANDUP_BACK );
		}
	}
	else if( GetXSkinAnimFuture()->GetNowAnimationTime() > DAMAGE_DOWN_STAND_UP_INPUT_TIME )
	{
		if( true == m_InputData.oneZ )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
			m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
		}
		else if( true == m_InputData.oneX )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[4] = true;
		}
		else if( true == m_FrameDataFuture.syncData.bIsRight )
		{
			if( true == m_InputData.pureRight )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
			else if( true == m_InputData.pureLeft )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
		}
		else
		{
			if( true == m_InputData.pureRight )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
			else if( true == m_InputData.pureLeft )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
		}
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32aec060 ( the roll stand-ups: not on the line is a jump down, the end of the roll walks on or waits )
void CX2GULuciel::LSI_LU_STANDUP_ROLLING_FRONT_FRONT_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32aec0f0
void CX2GULuciel::LSI_LU_STANDUP_ROLLING_FRONT_BACK_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32aec180 ( the back rolls sink by 1.5 line radii )
void CX2GULuciel::LSI_LU_STANDUP_ROLLING_BACK_FRONT_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32aec210
void CX2GULuciel::LSI_LU_STANDUP_ROLLING_BACK_BACK_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32aec2a0 ( the attack stand-ups test the anim end first )
void CX2GULuciel::LSI_LU_STANDUP_ATTACK_FRONT_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( LSI_LU_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32aec330
void CX2GULuciel::LSI_LU_STANDUP_ATTACK_BACK_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( LSI_LU_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32af0150 ( uncarved in Ghidra: read from the listing )
void CX2GULuciel::LSI_LU_STANDUP_ATTACK_FRONT_NO_DOWN_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( LSI_LU_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32af01e0
void CX2GULuciel::LSI_LU_STANDUP_ATTACK_BACK_NO_DOWN_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( LSI_LU_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32af1150
void CX2GULuciel::LSI_LU_JUMP_DOWN_NO_ATTACK_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_LANDING );
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32b0fd90 ( the active body's now anim )
void CX2GULuciel::LSI_LU_WAIT_StateStart()
{
	CommonStateStart();
	PlayAnimationWaitStart( GetXSkinAnimPtr().get(), m_FrameDataNow );
}

// char-luciel CH1b: June 32b0fde0
void CX2GULuciel::LSI_LU_JUMP_READY_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_LU_JUMP_COMBO_Z );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_LU_JUMP_COMBO_X );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
	{
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() || true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( LSI_LU_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32b0fef0
void CX2GULuciel::LSI_LU_DASH_END_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimer( DASH_END_STEP_DUST_TIME ) )
	{
		CreateStepDust();
	}

	CommonFrameMove();
}

// char-luciel CH1b: June 32b0ff90 ( the tail after the chain runs on every path, state change or not )
void CX2GULuciel::LSI_LU_DASH_JUMP_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		if( true == m_FrameDataFuture.stateParam.bEventFlagList[0] )
		{
			StateChange( LSI_LU_DASH_JUMP );
			m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
		}
		else if( true == m_bDownForce )
		{
			StateChange( LSI_LU_DASH_JUMP_POWER_LANDING );
		}
		else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_DASH_JUMP_LANDING );
		}

		m_bDownForce = false;
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_LU_DASH_JUMP_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_LU_DASH_JUMP_COMBO_X );
	}
	else
	{
		// turning against the dash direction ends the dash jump: held keys decide the side, a double press needs a fresh press of the other way
		bool bTurnAround = false;
		if( false == m_InputData.pureRight || false == m_InputData.pureLeft )
		{
			if( false == m_InputData.pureRight )
			{
				bTurnAround = ( true == m_InputData.pureLeft && true == m_FrameDataFuture.syncData.bIsRight );
			}
			else
			{
				bTurnAround = ( false == m_FrameDataFuture.syncData.bIsRight );
			}
		}
		else
		{
			bTurnAround = ( true == m_InputData.oneRight && false == m_FrameDataFuture.syncData.bIsRight )
				|| ( true == m_InputData.oneLeft && true == m_FrameDataFuture.syncData.bIsRight );
		}

		if( true == bTurnAround )
		{
			if( GetJumpSpeed() < m_PhysicParam.nowSpeed.y )
			{
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();
			}

			if( m_PhysicParam.nowSpeed.y <= 0.f )
			{
				StateChange( LSI_LU_JUMP_DOWN );
			}
			else
			{
				StateChange( LSI_LU_JUMP_UP );
			}
		}
	}

	if( true == m_InputData.oneUp && m_PhysicParam.nowSpeed.y <= 0.f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	if( true == m_InputData.oneDown )
	{
		m_bDownForce = true;
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( true == GetRidingOn() )
	{
		StateChange( LSI_RIDING_ON );
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32b10260 ( the active body's now anim )
void CX2GULuciel::LSI_LU_DAMAGE_DOWN_FRONT_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( DAMAGE_DOWN_FRONT_STEP_DUST_TIME ) )
	{
		CreateStepDust();
	}

	CommonFrameMove();
}

// char-luciel CH1b: June 32b10310 ( no one-shot here, as June )
void CX2GULuciel::LSI_LU_DAMAGE_DOWN_BACK_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimer( DAMAGE_DOWN_BACK_STEP_DUST_TIME ) )
	{
		CreateStepDust();
	}

	CommonFrameMove();
}

// char-luciel CH1b: June 32b11670
void CX2GULuciel::LSI_LU_JUMP_DOWN_NO_ATTACK_FrameMoveFuture()
{
	bool bSetWalkSpeed = true;

	if( false == m_InputData.pureRight || false == m_InputData.pureLeft )
	{
		if( true == m_InputData.pureRight )
		{
			m_FrameDataFuture.syncData.bIsRight = true;
		}
		else if( true == m_InputData.pureLeft )
		{
			m_FrameDataFuture.syncData.bIsRight = false;
		}
		else
		{
			bSetWalkSpeed = false;
		}
	}
	else if( true == m_InputData.oneRight )
	{
		m_FrameDataFuture.syncData.bIsRight = true;
	}
	else if( true == m_InputData.oneLeft )
	{
		m_FrameDataFuture.syncData.bIsRight = false;
	}

	if( true == bSetWalkSpeed )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	if( true == m_InputData.pureDown && false == m_InputData.pureUp )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
	}

	CommonFrameMoveFuture();
}

// char-luciel CH1b: June 32b20b30
void CX2GULuciel::LSI_LU_WAIT_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_InputData.oneSit && m_fSwitchingCoolTime <= 0.f )
	{
		StateChange( LSI_LU_SWITCHING );
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_LU_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_LU_COMBO_X );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
	{
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( LSI_LU_WALK );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( LSI_LU_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( LSI_LU_HYPER_MODE )
	else if( true == m_InputData.oneDown
		&& true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetRidingOn() )
	{
		StateChange( LSI_RIDING_ON );
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32b20d70
void CX2GULuciel::LSI_LU_WALK_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_InputData.oneSit && m_fSwitchingCoolTime <= 0.f )
	{
		StateChange( LSI_LU_SWITCHING );
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_LU_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_LU_COMBO_X );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
	{
	}
	else if( false == m_InputData.pureRight && false == m_InputData.pureLeft )
	{
		StateChange( LSI_LU_WAIT );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( LSI_LU_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( LSI_LU_HYPER_MODE )
	else if( true == m_InputData.oneDown
		&& true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetRidingOn() )
	{
		StateChange( LSI_RIDING_ON );
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32b20f70
void CX2GULuciel::LSI_LU_JUMP_LANDING_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	else if( true == m_InputData.oneSit && m_fSwitchingCoolTime <= 0.f )
	{
		StateChange( LSI_LU_SWITCHING );
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_LU_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_LU_COMBO_X );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
	{
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( LSI_LU_WALK );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( LSI_LU_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( LSI_LU_HYPER_MODE )
	else if( true == m_InputData.oneDown
		&& true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXCAS
	{
		StateChange( LSI_LU_WAIT, false );
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32b21250 ( a dash that leaves the line is a dash jump; the Z branch picks the job line's dash combo: line 1 Chiliarch, 2 Royal Guard, 3 Diablura, no Enum )
void CX2GULuciel::LSI_LU_DASH_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_InputData.oneSit && m_fSwitchingCoolTime <= 0.f )
	{
		StateChange( LSI_LU_SWITCHING );
	}
	else if( true == m_InputData.oneZ )
	{
		if( true == IsInJobLine( 1 ) )
		{
			StateChange( LSI_LLC_DASH_COMBO_Z );
		}
		else if( true == IsInJobLine( 2 ) )
		{
			StateChange( LSI_LCG_F_DASH_COMBO_Z );
		}
		else if( true == IsInJobLine( 3 ) )
		{
			StateChange( LSI_LDB_F_DASH_COMBO_Z );
		}
		else
		{
			StateChange( LSI_LU_DASH_COMBO_Z );
		}
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_LU_DASH_COMBO_X_START );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
	{
	}
	else if( true == m_InputData.oneUp )
	{
		StateChange( LSI_LU_DASH_JUMP );
		m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( LSI_LU_HYPER_MODE )
	else if( ( false == m_InputData.pureRight && false == m_InputData.pureLeft )
		|| ( true == m_InputData.pureRight && ( true == m_InputData.pureLeft || false == m_FrameDataFuture.syncData.bIsRight ) )
		|| ( true == m_InputData.pureLeft && true == m_FrameDataFuture.syncData.bIsRight ) )
	{
		StateChange( LSI_LU_DASH_END );
	}
	else if( true == m_InputData.oneDown
		&& true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( LSI_LU_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetRidingOn() )
	{
		StateChange( LSI_RIDING_ON );
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32b214c0 ( the combo keys in the first eighth of the dash end give the dash combos )
void CX2GULuciel::LSI_LU_DASH_END_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_InputData.oneSit && m_fSwitchingCoolTime <= 0.f )
	{
		StateChange( LSI_LU_SWITCHING );
	}
	else if( true == m_InputData.oneZ )
	{
		if( GetXSkinAnimFuture()->GetNowAnimationTime() <= GetXSkinAnimFuture()->GetMaxAnimationTime() * DASH_END_COMBO_INPUT_RATE )
		{
			if( true == IsInJobLine( 1 ) )
			{
				StateChange( LSI_LLC_DASH_COMBO_Z );
			}
			else if( true == IsInJobLine( 2 ) )
			{
				StateChange( LSI_LCG_F_DASH_COMBO_Z );
			}
			else
			{
				StateChange( LSI_LU_DASH_COMBO_Z );
			}
		}
		else
		{
			StateChange( LSI_LU_COMBO_Z );
		}
	}
	else if( true == m_InputData.oneX )
	{
		if( GetXSkinAnimFuture()->GetNowAnimationTime() <= GetXSkinAnimFuture()->GetMaxAnimationTime() * DASH_END_COMBO_INPUT_RATE )
		{
			StateChange( LSI_LU_DASH_COMBO_X_START );
		}
		else
		{
			StateChange( LSI_LU_COMBO_X );
		}
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
	{
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft
		|| ( true == m_InputData.oneRight && true == m_FrameDataFuture.syncData.bIsRight )
		|| ( true == m_InputData.oneLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( LSI_LU_WALK );
	}
	else if( true == m_InputData.oneDown
		&& true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_InputData.oneUp )
	{
		StateChange( LSI_LU_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( LSI_LU_HYPER_MODE )
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	else if( true == GetRidingOn() )
	{
		StateChange( LSI_RIDING_ON );
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32b21830 ( June's first block has no else: a unit off the line also runs the chain below, JUNE_BUGS 692 )
void CX2GULuciel::LSI_LU_DASH_JUMP_LANDING_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}

	if( true == m_InputData.oneSit && m_fSwitchingCoolTime <= 0.f )
	{
		StateChange( LSI_LU_SWITCHING );
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_LU_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_LU_COMBO_X );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
	{
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft
		|| ( true == m_InputData.oneRight && true == m_FrameDataFuture.syncData.bIsRight )
		|| ( true == m_InputData.oneLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( LSI_LU_WALK );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( LSI_LU_JUMP_READY );
	}
	else if( true == m_InputData.oneDown
		&& true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32b21a70 ( the power landing waits a sixth of the anim before it takes input )
void CX2GULuciel::LSI_LU_DASH_JUMP_POWER_LANDING_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_InputData.oneRight || true == m_InputData.oneLeft )
	{
		StateChange( LSI_LU_WALK );
	}
	else if( GetXSkinAnimFuture()->GetNowAnimationTime() > static_cast<float>( GetXSkinAnimFuture()->GetMaxAnimationTime() / DASH_JUMP_POWER_LANDING_INPUT_DIVISOR ) )
	{
		if( true == m_InputData.oneSit && m_fSwitchingCoolTime <= 0.f )
		{
			StateChange( LSI_LU_SWITCHING );
		}
		else if( true == m_InputData.oneZ )
		{
			StateChange( LSI_LU_COMBO_Z );
		}
		else if( true == m_InputData.oneX )
		{
			StateChange( LSI_LU_COMBO_X );
		}
		else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
		{
		}
		else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft
			|| true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else if( true == m_InputData.pureUp )
		{
			StateChange( LSI_LU_JUMP_READY );
		}
		else if( true == m_InputData.oneDown
			&& true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( LSI_LU_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1b: June 32b3a250 ( both bodies leave their transformed form, then the shared die start )
void CX2GULuciel::LSI_LU_DIE_FRONT_StateStart()
{
	EndTransformedForm( 0, true );
	EndTransformedForm( 1, true );
	DieFrontStart();
}

// char-luciel CH1b: June 32b3a270
void CX2GULuciel::LSI_LU_DIE_BACK_StateStart()
{
	EndTransformedForm( 0, true );
	EndTransformedForm( 1, true );
	DieBackStart();
}

// char-luciel CH1d: Ciel's common states ( June 32ae5960 - 32b235c0 ); the ids are June's: Lu's for the states Ciel shares, Ciel's own for her combos
// char-luciel CH1d: June 32ae5960
void CX2GULuciel::LSI_CIEL_JUMP_UP_EventProcess()
{
	if( true == m_InputData.pureDoubleLeft || true == m_InputData.pureDoubleRight )
	{
		if( true == GetEnableDash( EDS_END ) )
		{
			StateChange( LSI_LU_DASH_JUMP );
		}
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_CIEL_JUMP_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_CIEL_JUMP_COMBO_X );
	}
	else if( 0.f < m_PhysicParam.nowSpeed.y )
	{
		if( true == GetRidingOn() )
		{
			StateChange( LSI_RIDING_ON );
		}
	}
	else
	{
		StateChange( LSI_LU_JUMP_DOWN );
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32ae5a00
void CX2GULuciel::LSI_CIEL_STANDUP_ROLLING_FRONT_FRONT_StateEnd()
{
	CommonStateEnd();
	SetForceInvincible( STAND_UP_INVINCIBLE_TIME );
	SetShowInvincible( STAND_UP_INVINCIBLE_TIME );
}

// char-luciel CH1d: June 32ae5a40
void CX2GULuciel::LSI_CIEL_STANDUP_ROLLING_FRONT_BACK_StateEnd()
{
	CommonStateEnd();
	SetForceInvincible( STAND_UP_INVINCIBLE_TIME );
	SetShowInvincible( STAND_UP_INVINCIBLE_TIME );
}

// char-luciel CH1d: June 32ae5a80
void CX2GULuciel::LSI_CIEL_STANDUP_ROLLING_BACK_FRONT_StateEnd()
{
	CommonStateEnd();
	SetForceInvincible( STAND_UP_INVINCIBLE_TIME );
	SetShowInvincible( STAND_UP_INVINCIBLE_TIME );
}

// char-luciel CH1d: June 32ae5ac0
void CX2GULuciel::LSI_CIEL_STANDUP_ROLLING_BACK_BACK_StateEnd()
{
	CommonStateEnd();
	SetForceInvincible( STAND_UP_INVINCIBLE_TIME );
	SetShowInvincible( STAND_UP_INVINCIBLE_TIME );
}

// char-luciel CH1d: June 32ae5b00
void CX2GULuciel::LSI_CIEL_STANDUP_ATTACK_FRONT_StateEnd()
{
	CommonStateEnd();
	SetForceInvincible( STAND_UP_INVINCIBLE_TIME );
	SetShowInvincible( STAND_UP_INVINCIBLE_TIME );
}

// char-luciel CH1d: June 32ae5b40
void CX2GULuciel::LSI_CIEL_STANDUP_ATTACK_BACK_StateEnd()
{
	CommonStateEnd();
	SetForceInvincible( STAND_UP_INVINCIBLE_TIME );
	SetShowInvincible( STAND_UP_INVINCIBLE_TIME );
}

// char-luciel CH1d: June 32af1b80
void CX2GULuciel::LSI_CIEL_JUMP_DOWN_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_LANDING );
	}
	else if( true == m_InputData.pureDoubleLeft || true == m_InputData.pureDoubleRight )
	{
		if( true == GetEnableDash( EDS_END ) )
		{
			StateChange( LSI_LU_DASH_JUMP );
		}
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_CIEL_JUMP_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_CIEL_JUMP_COMBO_X );
	}
	else if( 0.f > m_PhysicParam.nowSpeed.y )
	{
		if( true == GetRidingOn() )
		{
			StateChange( LSI_RIDING_ON );
		}
	}
	else
	{
		StateChange( LSI_LU_JUMP_UP );
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32af1c30 ( the stand-up pick has no no-down roll )
void CX2GULuciel::LSI_CIEL_DAMAGE_DOWN_FRONT_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( true == m_FrameDataFuture.stateParam.bEventFlagList[1] || true == m_FrameDataFuture.stateParam.bEventFlagList[4] )
		{
			StateChange( LSI_CIEL_STANDUP_ATTACK_FRONT );
		}
		else if( true == m_FrameDataFuture.stateParam.bEventFlagList[2] )
		{
			StateChange( LSI_CIEL_STANDUP_ROLLING_FRONT_FRONT );
		}
		else if( true == m_FrameDataFuture.stateParam.bEventFlagList[3] )
		{
			StateChange( LSI_CIEL_STANDUP_ROLLING_FRONT_BACK );
		}
		else
		{
			StateChange( LSI_LU_DAMAGE_STANDUP_FRONT );
		}
	}
	else if( GetXSkinAnimFuture()->GetNowAnimationTime() > DAMAGE_DOWN_STAND_UP_INPUT_TIME )
	{
		if( true == m_InputData.oneZ )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
			m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
		}
		else if( true == m_InputData.oneX )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[4] = true;
		}
		else if( true == m_FrameDataFuture.syncData.bIsRight )
		{
			if( true == m_InputData.pureRight )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
			else if( true == m_InputData.pureLeft )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
		}
		else
		{
			if( true == m_InputData.pureRight )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
			else if( true == m_InputData.pureLeft )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
		}
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32af1da0 ( the stand-up pick has no no-down roll )
void CX2GULuciel::LSI_CIEL_DAMAGE_DOWN_BACK_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( true == m_FrameDataFuture.stateParam.bEventFlagList[1] || true == m_FrameDataFuture.stateParam.bEventFlagList[4] )
		{
			StateChange( LSI_CIEL_STANDUP_ATTACK_BACK );
		}
		else if( true == m_FrameDataFuture.stateParam.bEventFlagList[2] )
		{
			StateChange( LSI_CIEL_STANDUP_ROLLING_BACK_FRONT );
		}
		else if( true == m_FrameDataFuture.stateParam.bEventFlagList[3] )
		{
			StateChange( LSI_CIEL_STANDUP_ROLLING_BACK_BACK );
		}
		else
		{
			StateChange( LSI_CIEL_DAMAGE_STANDUP_BACK );
		}
	}
	else if( GetXSkinAnimFuture()->GetNowAnimationTime() > DAMAGE_DOWN_STAND_UP_INPUT_TIME )
	{
		if( true == m_InputData.oneZ )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
			m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
		}
		else if( true == m_InputData.oneX )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
			m_FrameDataFuture.stateParam.bEventFlagList[4] = true;
		}
		else if( true == m_FrameDataFuture.syncData.bIsRight )
		{
			if( true == m_InputData.pureRight )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
			else if( true == m_InputData.pureLeft )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
		}
		else
		{
			if( true == m_InputData.pureRight )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
			else if( true == m_InputData.pureLeft )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[2] = true;
				m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
				m_FrameDataFuture.stateParam.bEventFlagList[4] = false;
			}
		}
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32af1f20
void CX2GULuciel::LSI_CIEL_STANDUP_ROLLING_FRONT_FRONT_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32af1fb0
void CX2GULuciel::LSI_CIEL_STANDUP_ROLLING_FRONT_BACK_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32af2040
void CX2GULuciel::LSI_CIEL_STANDUP_ROLLING_BACK_FRONT_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32af20d0
void CX2GULuciel::LSI_CIEL_STANDUP_ROLLING_BACK_BACK_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32af2160
void CX2GULuciel::LSI_CIEL_STANDUP_ATTACK_FRONT_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( LSI_LU_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32af21f0
void CX2GULuciel::LSI_CIEL_STANDUP_ATTACK_BACK_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( LSI_LU_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32af5cf0 ( uncarved: read from charport_adis.py )
void CX2GULuciel::LSI_CIEL_JUMP_DOWN_NO_ATTACK_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_LANDING );
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32b11720
void CX2GULuciel::LSI_CIEL_JUMP_READY_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_CIEL_JUMP_COMBO_Z );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_CIEL_JUMP_COMBO_X );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
	{
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() || true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( LSI_LU_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32b11830
void CX2GULuciel::LSI_CIEL_DASH_END_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimer( DASH_END_STEP_DUST_TIME ) )
	{
		CreateStepDust();
	}

	CommonFrameMove();
}

// char-luciel CH1d: June 32b118d0
void CX2GULuciel::LSI_CIEL_DASH_JUMP_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		if( true == m_FrameDataFuture.stateParam.bEventFlagList[0] )
		{
			StateChange( LSI_LU_DASH_JUMP );
			m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
		}
		else if( true == m_bDownForce )
		{
			StateChange( LSI_CIEL_DASH_JUMP_POWER_LANDING );
		}
		else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( LSI_LU_DASH_JUMP_LANDING );
		}

		m_bDownForce = false;
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_CIEL_DASH_JUMP_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_CIEL_DASH_JUMP_COMBO_X );
	}
	else
	{
		// turning against the dash direction ends the dash jump: held keys decide the side, a double press needs a fresh press of the other way
		bool bTurnAround = false;
		if( false == m_InputData.pureRight || false == m_InputData.pureLeft )
		{
			if( false == m_InputData.pureRight )
			{
				bTurnAround = ( true == m_InputData.pureLeft && true == m_FrameDataFuture.syncData.bIsRight );
			}
			else
			{
				bTurnAround = ( false == m_FrameDataFuture.syncData.bIsRight );
			}
		}
		else
		{
			bTurnAround = ( true == m_InputData.oneRight && false == m_FrameDataFuture.syncData.bIsRight )
				|| ( true == m_InputData.oneLeft && true == m_FrameDataFuture.syncData.bIsRight );
		}

		if( true == bTurnAround )
		{
			if( GetJumpSpeed() < m_PhysicParam.nowSpeed.y )
			{
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();
			}

			if( m_PhysicParam.nowSpeed.y <= 0.f )
			{
				StateChange( LSI_LU_JUMP_DOWN );
			}
			else
			{
				StateChange( LSI_LU_JUMP_UP );
			}
		}
	}

	if( true == m_InputData.oneUp && m_PhysicParam.nowSpeed.y <= 0.f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	if( true == m_InputData.oneDown )
	{
		m_bDownForce = true;
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( true == GetRidingOn() )
	{
		StateChange( LSI_RIDING_ON );
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32b11ba0
void CX2GULuciel::LSI_CIEL_DAMAGE_DOWN_FRONT_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( DAMAGE_DOWN_FRONT_STEP_DUST_TIME ) )
	{
		CreateStepDust();
	}

	CommonFrameMove();
}

// char-luciel CH1d: June 32b11c50
void CX2GULuciel::LSI_CIEL_DAMAGE_DOWN_BACK_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimer( DAMAGE_DOWN_BACK_STEP_DUST_TIME ) )
	{
		CreateStepDust();
	}

	CommonFrameMove();
}

// char-luciel CH1d: June 32b12e20
void CX2GULuciel::LSI_CIEL_JUMP_DOWN_NO_ATTACK_FrameMoveFuture()
{
	bool bSetWalkSpeed = true;

	if( false == m_InputData.pureRight || false == m_InputData.pureLeft )
	{
		if( true == m_InputData.pureRight )
		{
			m_FrameDataFuture.syncData.bIsRight = true;
		}
		else if( true == m_InputData.pureLeft )
		{
			m_FrameDataFuture.syncData.bIsRight = false;
		}
		else
		{
			bSetWalkSpeed = false;
		}
	}
	else if( true == m_InputData.oneRight )
	{
		m_FrameDataFuture.syncData.bIsRight = true;
	}
	else if( true == m_InputData.oneLeft )
	{
		m_FrameDataFuture.syncData.bIsRight = false;
	}

	if( true == bSetWalkSpeed )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	if( true == m_InputData.pureDown && false == m_InputData.pureUp )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
	}

	CommonFrameMoveFuture();
}

// char-luciel CH1d: June 32b22710
void CX2GULuciel::LSI_CIEL_WAIT_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_InputData.oneSit && m_fSwitchingCoolTime <= 0.f )
	{
		StateChange( LSI_CIEL_SWITCHING );
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_CIEL_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_CIEL_COMBO_X );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
	{
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( LSI_LU_WALK );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( LSI_LU_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( LSI_LU_HYPER_MODE )
	else if( true == m_InputData.oneDown
		&& true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetRidingOn() )
	{
		StateChange( LSI_RIDING_ON );
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32b22950
void CX2GULuciel::LSI_CIEL_WALK_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_InputData.oneSit && m_fSwitchingCoolTime <= 0.f )
	{
		StateChange( LSI_CIEL_SWITCHING );
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_CIEL_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_CIEL_COMBO_X );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
	{
	}
	else if( false == m_InputData.pureRight && false == m_InputData.pureLeft )
	{
		StateChange( LSI_LU_WAIT );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( LSI_LU_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( LSI_LU_HYPER_MODE )
	else if( true == m_InputData.oneDown
		&& true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetRidingOn() )
	{
		StateChange( LSI_RIDING_ON );
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32b22b50
void CX2GULuciel::LSI_CIEL_JUMP_LANDING_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	else if( true == m_InputData.oneSit && m_fSwitchingCoolTime <= 0.f )
	{
		StateChange( LSI_CIEL_SWITCHING );
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_CIEL_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_CIEL_COMBO_X );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
	{
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( LSI_LU_WALK );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( LSI_LU_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( LSI_LU_HYPER_MODE )
	else if( true == m_InputData.oneDown
		&& true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXCAS
	{
		StateChange( LSI_LU_WAIT, false );
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32b22e30 ( no job-line split: the dash combos are Ciel's own )
void CX2GULuciel::LSI_CIEL_DASH_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_InputData.oneSit && m_fSwitchingCoolTime <= 0.f )
	{
		StateChange( LSI_CIEL_SWITCHING );
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_CIEL_DASH_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_CIEL_DASH_COMBO_X );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
	{
	}
	else if( true == m_InputData.oneUp )
	{
		StateChange( LSI_LU_DASH_JUMP );
		m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( LSI_LU_HYPER_MODE )
	else if( ( false == m_InputData.pureRight && false == m_InputData.pureLeft )
		|| ( true == m_InputData.pureRight && ( true == m_InputData.pureLeft || false == m_FrameDataFuture.syncData.bIsRight ) )
		|| ( true == m_InputData.pureLeft && true == m_FrameDataFuture.syncData.bIsRight ) )
	{
		StateChange( LSI_LU_DASH_END );
	}
	else if( true == m_InputData.oneDown
		&& true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( LSI_LU_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetRidingOn() )
	{
		StateChange( LSI_RIDING_ON );
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32b23050 ( the dash combos are Ciel's own, no job-line split )
void CX2GULuciel::LSI_CIEL_DASH_END_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_InputData.oneSit && m_fSwitchingCoolTime <= 0.f )
	{
		StateChange( LSI_CIEL_SWITCHING );
	}
	else if( true == m_InputData.oneZ )
	{
		if( GetXSkinAnimFuture()->GetNowAnimationTime() <= GetXSkinAnimFuture()->GetMaxAnimationTime() * DASH_END_COMBO_INPUT_RATE )
		{
			StateChange( LSI_CIEL_DASH_COMBO_Z );
		}
		else
		{
			StateChange( LSI_CIEL_COMBO_Z );
		}
	}
	else if( true == m_InputData.oneX )
	{
		if( GetXSkinAnimFuture()->GetNowAnimationTime() <= GetXSkinAnimFuture()->GetMaxAnimationTime() * DASH_END_COMBO_INPUT_RATE )
		{
			StateChange( LSI_CIEL_DASH_COMBO_X );
		}
		else
		{
			StateChange( LSI_CIEL_COMBO_X );
		}
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
	{
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft
		|| ( true == m_InputData.oneRight && true == m_FrameDataFuture.syncData.bIsRight )
		|| ( true == m_InputData.oneLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( LSI_LU_WALK );
	}
	else if( true == m_InputData.oneDown
		&& true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_InputData.oneUp )
	{
		StateChange( LSI_LU_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( LSI_LU_HYPER_MODE )
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	else if( true == GetRidingOn() )
	{
		StateChange( LSI_RIDING_ON );
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32b23380 ( the off-the-line block has no else, as Lu's: JUNE_BUGS 693 )
void CX2GULuciel::LSI_CIEL_DASH_JUMP_LANDING_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}

	if( true == m_InputData.oneSit && m_fSwitchingCoolTime <= 0.f )
	{
		StateChange( LSI_CIEL_SWITCHING );
	}
	else if( true == m_InputData.oneZ )
	{
		StateChange( LSI_CIEL_COMBO_Z );
	}
	else if( true == m_InputData.oneX )
	{
		StateChange( LSI_CIEL_COMBO_X );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
	{
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft
		|| ( true == m_InputData.oneRight && true == m_FrameDataFuture.syncData.bIsRight )
		|| ( true == m_InputData.oneLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( LSI_LU_WALK );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( LSI_LU_JUMP_READY );
	}
	else if( true == m_InputData.oneDown
		&& true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}

	CommonEventProcess();
}

// char-luciel CH1d: June 32b235c0
void CX2GULuciel::LSI_CIEL_DASH_JUMP_POWER_LANDING_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_InputData.oneRight || true == m_InputData.oneLeft )
	{
		StateChange( LSI_LU_WALK );
	}
	else if( GetXSkinAnimFuture()->GetNowAnimationTime() > static_cast<float>( GetXSkinAnimFuture()->GetMaxAnimationTime() / DASH_JUMP_POWER_LANDING_INPUT_DIVISOR ) )
	{
		if( true == m_InputData.oneSit && m_fSwitchingCoolTime <= 0.f )
		{
			StateChange( LSI_CIEL_SWITCHING );
		}
		else if( true == m_InputData.oneZ )
		{
			StateChange( LSI_CIEL_COMBO_Z );
		}
		else if( true == m_InputData.oneX )
		{
			StateChange( LSI_CIEL_COMBO_X );
		}
		else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_GROUND ) )
		{
		}
		else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft
			|| true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else if( true == m_InputData.pureUp )
		{
			StateChange( LSI_LU_JUMP_READY );
		}
		else if( true == m_InputData.oneDown
			&& true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( LSI_LU_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
		{
			StateChange( LSI_LU_WAIT );
		}
	}

	CommonEventProcess();
}

// char-luciel CH1f: Duo combos part 1 ( June 32ae57b0 - 32af2cc0 ); the macros are June's expansions on the active body's animation
#define ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( iEventFlagIndex, fInputTimingStart, fInputTiming, fChangeTiming, bCondition, iStateID ) \
	else if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && \
		( (float)(fChangeTiming) < GetXSkinAnimFuture()->GetNowAnimationTime() || true == GetXSkinAnimFuture()->IsAnimationEnd() ) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( GetXSkinAnimFuture()->GetNowAnimationTime() < (float)(fInputTiming) && (float)(fInputTimingStart) <= GetXSkinAnimFuture()->GetNowAnimationTime() && true == (bCondition) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
#define WALK_CANCEL_AFTER_ON_LINE_( time ) \
	else if( (time) >= 0.f && GetXSkinAnimFuture()->GetNowAnimationTime() > (time) && true == IsOnSomethingFuture() && \
	( true == m_InputData.pureRight || true == m_InputData.pureLeft ) ) \
	{ StateChange( LSI_LU_WAIT ); }
#define DASH_CANCEL_AFTER_ON_LINE_( time ) \
	else if( (time) >= 0.f && GetXSkinAnimFuture()->GetNowAnimationTime() > (time) && true == IsOnSomethingFuture() && \
	( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft ) ) \
	{ StateChangeDashIfPossible(); }

namespace _CONST_LUCIEL_CH1F_
{
	const float		COMBO_XXXZ_LOOP_END_TIME					= 2.f;		// June 32ae57d0: the loop ends at 2 s or when Z is released
	const double	COMBO_XXXZ_CHARGE_TIME_DIVISOR				= 1.08;		// June 32ae5ba0: the charge rate is the state time over this, up to 1
	const double	COMBO_XXXZ_CHARGE_TIME_MAX					= 1.0;		// June 32ae5ba0
	const float		DASH_COMBO_X_LANDING_START_TIME				= 0.15f;	// June 32aee9e0
	const float		JUMP_COMBO_X_CAN_NOT_JUMP_TIME				= 0.2f;		// June 32aeee30
	const float		DASH_JUMP_COMBO_X_SPECIAL_ATTACK_TIME		= 0.2f;		// June 32aef160 ( compared as a double )
	const float		DASH_JUMP_COMBO_XXXX_JUMP_DOWN_TIME			= 0.9f;		// June 32aefc40 ( compared as a double )
	const float		DASH_JUMP_COMBO_XX_START_AIM_TIME			= 0.369f;	// June 32aef530
	const float		DASH_JUMP_COMBO_XX_START_AIM_UP_DEGREE		= -20.f;	// June 32aef530: GetNearestTargetAimDegree arguments
	const float		DASH_JUMP_COMBO_XX_START_AIM_DOWN_DEGREE	= 80.f;
	const float		DASH_JUMP_COMBO_XX_START_AIM_YAW_DEGREE		= 120.f;
	const float		DASH_JUMP_COMBO_XX_START_AIM_DISTANCE		= 1200.f;
	const float		DASH_JUMP_COMBO_XX_START_SPEED_X			= 1700.f;	// June 32aef530: passive speed with no aim
	const float		DASH_JUMP_COMBO_XX_START_SPEED_Y			= -2000.f;
	const double	DASH_JUMP_COMBO_XX_START_AIM_ANGLE_OFFSET	= 90.0;		// June 32aef530: passive speed x = -tan( ( aim + 90 ) in radians ) * -2000
	const double	DASH_JUMP_COMBO_XX_START_AIM_SPEED_SCALE	= -2000.0;
	const int		STATE_ID_DASH_JUMP_COMBO_X_THIRD_JOB_LINE	= 338;		// June 32aef160: no name in June ( registered with the script table of LSI_LU_JUMP_COMBO_Z, JUNE_BUGS )
}
using namespace _CONST_LUCIEL_CH1F_;

// char-luciel CH1f: June 32ae57b0
void CX2GULuciel::LSI_LU_COMBO_XXXZ_READY_StateStart()
{
	CommonStateStart();
	m_fComboXXXZChargeTime = 0.f;
}

// char-luciel CH1f: June 32ae57d0
void CX2GULuciel::LSI_LU_COMBO_XXXZ_LOOP_EventProcess()
{
	if( m_FrameDataNow.unitCondition.fStateTime >= COMBO_XXXZ_LOOP_END_TIME || false == m_InputData.pureZ )
	{
		StateChange( LSI_LU_COMBO_XXXZ_END );
	}

	CommonEventProcess();
}

// char-luciel CH1f: June 32ae5810
void CX2GULuciel::LSI_LU_JUMP_COMBO_Z_FrameMoveFuture()
{
	if( true == m_InputData.pureDown )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
	}

	CommonFrameMoveFuture();
}

// char-luciel CH1f: June 32ae5850
void CX2GULuciel::LSI_LU_JUMP_COMBO_X_FrameMoveFuture()
{
	if( true == m_InputData.pureDown )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
	}

	CommonFrameMoveFuture();
}

// char-luciel CH1f: June 32ae5b80
void CX2GULuciel::LSI_CIEL_COMBO_XXXZ_START_StateStart()
{
	CommonStateStart();
	m_fComboXXXZChargeTime = 0.f;
}

// char-luciel CH1f: June 32ae5ba0 ( the charge rate: state time over 1.08, at most 1 )
void CX2GULuciel::LSI_CIEL_COMBO_XXXZ_LOOP_StateEnd()
{
	CommonStateEnd();

	const double fChargeTime = m_FrameDataNow.unitCondition.fStateTime / COMBO_XXXZ_CHARGE_TIME_DIVISOR;
	m_fComboXXXZChargeTime = ( COMBO_XXXZ_CHARGE_TIME_MAX < fChargeTime ) ? (float)COMBO_XXXZ_CHARGE_TIME_MAX : (float)fChargeTime;
}

// char-luciel CH1f: June 32ae5be0
void CX2GULuciel::LSI_CIEL_JUMP_COMBO_Z_FrameMoveFuture()
{
	if( true == m_InputData.pureDown )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
	}

	CommonFrameMoveFuture();
}

// char-luciel CH1f: June 32ae5c20
void CX2GULuciel::LSI_CIEL_JUMP_COMBO_X_FrameMoveFuture()
{
	if( true == m_InputData.pureDown )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
	}

	CommonFrameMoveFuture();
}

// char-luciel CH1f: June 32ae6050 ( state 338 is registered with the script table of LSI_LU_JUMP_COMBO_Z; the body is the same as 32ae5810 )
void CX2GULuciel::LSI_LU_JUMP_COMBO_Z_SECOND_FrameMoveFuture()
{
	if( true == m_InputData.pureDown )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
	}

	CommonFrameMoveFuture();
}

// char-luciel CH1f: June 32aec3c0
void CX2GULuciel::LSI_LU_COMBO_Z_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ, LSI_LU_COMBO_ZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aec6b0
void CX2GULuciel::LSI_LU_COMBO_ZZ_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ, LSI_LU_COMBO_ZZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aec9a0 ( Z goes on; X goes to the job line's own state )
void CX2GULuciel::LSI_LU_COMBO_ZZZ_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ, LSI_LU_COMBO_ZZZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, ( true == m_InputData.oneX && true == IsInJobLine( 1 ) ), LSI_LLC_COMBO_ZZZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange, ( true == m_InputData.oneX && true == IsInJobLine( 2 ) ), LSI_LCG_F_COMBO_ZZZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aece00
void CX2GULuciel::LSI_LU_COMBO_ZZZZ_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LU_COMBO_ZZZZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aed0f0
void CX2GULuciel::LSI_LU_COMBO_ZZZZX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aed320 ( a walk key against the facing clears the three flags first; the anim-end test comes after the key tests )
void CX2GULuciel::LSI_LU_COMBO_X_EventProcess()
{
	if( CAN_WALK_CANCEL )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LU_COMBO_XX )
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aed640 ( X + Down on a Dread Lord unit goes to the down-X state )
void CX2GULuciel::LSI_LU_COMBO_XX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		( true == m_InputData.pureDown && true == m_InputData.oneX && NULL != GetUnit() && CX2Unit::UC_LUCIEL_DREAD_LORD == GetUnit()->GetClass() ), LSI_LLD_COMBO_XX_DOWN_X )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LU_COMBO_XXX )
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aed9f0
void CX2GULuciel::LSI_LU_COMBO_XXX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange, ( true == m_InputData.oneX && true == IsInJobLine( 2 ) ), LSI_LCG_F_COMBO_XXXX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LU_COMBO_XXXX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, m_InputData.oneZ, LSI_LU_COMBO_XXXZ_READY )
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aede70
void CX2GULuciel::LSI_LU_COMBO_XXXX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aee0a0 ( Z released at any time before the end sets the flag that picks the end state )
void CX2GULuciel::LSI_LU_COMBO_XXXZ_READY_EventProcess()
{
	if( false == m_InputData.pureZ )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.stateParam.bEventFlagList[0] ? LSI_LU_COMBO_XXXZ_END : LSI_LU_COMBO_XXXZ_CHARGE );
	}

	CommonEventProcess();
}

// char-luciel CH1f: June 32aee130
void CX2GULuciel::LSI_LU_COMBO_XXXZ_CHARGE_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_COMBO_XXXZ_LOOP );
	}
	else if( false == m_InputData.pureZ )
	{
		StateChange( LSI_LU_COMBO_XXXZ_END );
	}

	CommonEventProcess();
}

// char-luciel CH1f: June 32aee1b0 ( the anim end leaves for the jump down off the line, else for the wait )
void CX2GULuciel::LSI_LU_COMBO_XXXZ_END_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( LSI_LU_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LU_COMBO_XXXZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aee4a0
void CX2GULuciel::LSI_LU_COMBO_XXXZX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aee6d0
void CX2GULuciel::LSI_LU_DASH_COMBO_Z_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( LSI_LU_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, ( true == m_InputData.oneX && true == IsInJobLine( 2 ) ), LSI_LCG_F_DASH_COMBO_ZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aee9e0
void CX2GULuciel::LSI_LU_DASH_COMBO_X_START_EventProcess()
{
	if( true == IsOnSomethingFuture() && GetXSkinAnimFuture()->GetNowAnimationTime() > DASH_COMBO_X_LANDING_START_TIME )
	{
		StateChange( LSI_LU_DASH_COMBO_X_LANDING );
	}

	CommonEventProcess();
}

// char-luciel CH1f: June 32aeea40
void CX2GULuciel::LSI_LU_DASH_COMBO_X_LANDING_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( LSI_LU_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( LSI_LU_WAIT );
		}
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, ( true == m_InputData.oneX && true == IsInJobLine( 1 ) ), LSI_LLC_DASH_COMBO_XX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, ( true == m_InputData.oneX && true == IsInJobLine( 2 ) ), LSI_LCG_F_DASH_COMBO_XX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aeee30 ( the jump landing from the jump combo X blocks the next jump for 0.2 s when the flag is set )
void CX2GULuciel::LSI_LU_JUMP_COMBO_X_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		if( true == m_FrameDataFuture.stateParam.bEventFlagList[0] )
		{
			m_fCanNotJumpTime = JUMP_COMBO_X_CAN_NOT_JUMP_TIME;
		}

		StateChange( LSI_LU_JUMP_LANDING );
	}

	CommonEventProcess();
}

// char-luciel CH1f: June 32aeee70
void CX2GULuciel::LSI_LU_DASH_JUMP_COMBO_Z_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LU_DASH_JUMP_COMBO_ZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		( true == IsInJobLine( 1 ) && ( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft ) ), LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aef050
void CX2GULuciel::LSI_LU_DASH_JUMP_COMBO_ZX_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, ( true == m_InputData.oneZ && true == IsInJobLine( 2 ) ), LSI_LCG_F_DASH_JUMP_COMBO_ZXX )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aef160 ( the last branch ends in a jump combo Z that needs both the air special attack and Z, JUNE_BUGS )
void CX2GULuciel::LSI_LU_DASH_JUMP_COMBO_X_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LU_DASH_JUMP_COMBO_XX_START )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		( true == IsInJobLine( 1 ) && ( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft ) ), LSI_LLC_F_DASH_JUMP_COMBO_X_DASH )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		( true == m_InputData.oneZ && true == IsInJobLine( 2 ) ), STATE_ID_DASH_JUMP_COMBO_X_THIRD_JOB_LINE )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 3, m_fEventTime[3].keyInputStart, m_fEventTime[3].keyInputEnd, m_fEventTime[3].stateChange,
		( true == IsInJobLine( 3 ) && true == m_InputData.oneZ ), LSI_LDB_M_DASH_JUMP_COMBO_XZ_START )
	else if( GetXSkinAnimFuture()->GetNowAnimationTime() > DASH_JUMP_COMBO_X_SPECIAL_ATTACK_TIME
		&& true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) && true == m_InputData.oneZ )
	{
		StateChange( LSI_LU_JUMP_COMBO_Z );
	}

	CommonEventProcess();
}

// char-luciel CH1f: June 32aef530 ( June reads the future animation here; the passive speed aims at the nearest target )
void CX2GULuciel::LSI_LU_DASH_JUMP_COMBO_XX_START_FrameMove()
{
	if( true == GetXSkinAnimFuture()->EventTimer( DASH_JUMP_COMBO_XX_START_AIM_TIME ) )
	{
		float fPassiveSpeedX = DASH_JUMP_COMBO_XX_START_SPEED_X;

		const D3DXVECTOR3 vAimDegree = GetNearestTargetAimDegree( DASH_JUMP_COMBO_XX_START_AIM_UP_DEGREE, DASH_JUMP_COMBO_XX_START_AIM_DOWN_DEGREE,
			DASH_JUMP_COMBO_XX_START_AIM_YAW_DEGREE, DASH_JUMP_COMBO_XX_START_AIM_DISTANCE );
		if( 0.f != vAimDegree.z )
		{
			float fAimRadian = (float)D3DXToRadian( vAimDegree.z + DASH_JUMP_COMBO_XX_START_AIM_ANGLE_OFFSET );
			float fAimTan = (float)tan( fAimRadian );
			fPassiveSpeedX = (float)( -fAimTan * DASH_JUMP_COMBO_XX_START_AIM_SPEED_SCALE );
		}

		m_PhysicParam.passiveSpeed.x = fPassiveSpeedX;
		m_PhysicParam.passiveSpeed.y = DASH_JUMP_COMBO_XX_START_SPEED_Y;
	}

	CommonFrameMove();
}

// char-luciel CH1f: June 32aef660
void CX2GULuciel::LSI_LU_DASH_JUMP_COMBO_XX_START_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_COMBO_XX_END );
	}

	CommonEventProcess();
}

// char-luciel CH1f: June 32aef690
void CX2GULuciel::LSI_LU_DASH_JUMP_COMBO_XX_END_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LU_DASH_JUMP_COMBO_XXX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aef960 ( the jump down here has no position change )
void CX2GULuciel::LSI_LU_DASH_JUMP_COMBO_XXX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LU_DASH_JUMP_COMBO_XXXX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aefc40 ( the jump down waits for the anim time to pass 0.9 )
void CX2GULuciel::LSI_LU_DASH_JUMP_COMBO_XXXX_EventProcess()
{
	if( false == IsOnSomethingFuture() && GetXSkinAnimFuture()->GetNowAnimationTime() > DASH_JUMP_COMBO_XXXX_JUMP_DOWN_TIME )
	{
		StateChange( LSI_LU_JUMP_DOWN );
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ, LSI_LU_DASH_JUMP_COMBO_XXXXZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32aeff50
void CX2GULuciel::LSI_LU_DASH_JUMP_COMBO_XXXXZ_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32af2280 ( Ciel's combo ends use Lu's jump down / wait ids, as the common states do )
void CX2GULuciel::LSI_CIEL_COMBO_Z_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ, LSI_CIEL_COMBO_ZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32af2570
void CX2GULuciel::LSI_CIEL_COMBO_ZZ_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ, LSI_CIEL_COMBO_ZZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32af2860 ( X goes to the job line's own state: the Royal Guard line is checked in event 1, the Chiliarch line in event 2 )
void CX2GULuciel::LSI_CIEL_COMBO_ZZZ_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ, LSI_CIEL_COMBO_ZZZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, ( true == m_InputData.oneX && true == IsInJobLine( 2 ) ), LSI_LCG_COMBO_ZZZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange, ( true == m_InputData.oneX && true == IsInJobLine( 1 ) ), LSI_LLC_M_COMBO_ZZZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f: June 32af2cc0
void CX2GULuciel::LSI_CIEL_COMBO_ZZZZ_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_CIEL_COMBO_ZZZZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

#endif SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE
//}}
