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
, m_hSoulShockDamageEffect( INVALID_DAMAGE_EFFECT_HANDLE )
, m_fCommunionRate( 1.f )
, m_fComboXXDownXXLoopTime( 0.f )
, m_hDashComboDownXDamageEffect( INVALID_DAMAGE_EFFECT_HANDLE )
, m_vDashComboDownXPosition( 0.f, 0.f, 0.f )
, m_usDancingBladeLoopCount( 0 )
, m_fSummonedDarknessElapsedTime( 0.f )
, m_iLordOfFearSkillLevel( 0 )
, m_fReduceDebuffDurationRate( 1.f )
, m_fLordOfFearDamageRate( 1.f )
, m_fFearDamageEffectCoolTime( 0.f )
, m_ucAuraOfFearStackCount( 0 )
, m_fAuraOfFearTime( 0.f )
, m_fAuraOfFearDuration( 0.f )
, m_fEnhancedSpecterRate( 1.f )
, m_fEnhancedSpecterRange( 1.f )
, m_fSoulStrikeChargeTime( 0.f )
, m_iShadowBoltNpcUID( -1 )
, m_fShadowBoltTime( 0.f )
, m_fShadowBoltDuration( 0.f )
, m_ucTwilightJudgementStackCount( 0 )
, m_fTwilightJudgementTime( 0.f )
, m_fSoulStrikeCoolTime( 0.f )
, m_fCombinationPointOnHitCoolTime( 0.f )
, m_hDarkCrescentEffectSet( INVALID_EFFECTSET_HANDLE )
, m_iEvilClawHitCount( 0 )
, m_fSwitchingCoolTime( 0.f )
, m_iSkillUseMpForm( 3 )
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
, m_hDeathTouchBeamEffect( INVALID_DAMAGE_EFFECT_HANDLE )
, m_hDeathTouchTargetEffect( INVALID_DAMAGE_EFFECT_HANDLE )
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
, m_bSkillCameraOverride( false )
, m_fSkillCameraOverrideValue( 0.f )
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
	if( true == GetXSkinAnimFuture()->EventTimerOneshot( DASH_JUMP_COMBO_XX_START_AIM_TIME ) )
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

// char-luciel CH1f2: Duo combos part 2 ( June 32af2fb0 - 32b25cd0 ); the Ciel combos are Lu's bodies with Ciel's ids
namespace _CONST_LUCIEL_CH1F2_
{
	const float		CIEL_COMBO_XXXZ_LOOP_END_TIME						= 3.f;		// June 32af3fe0
	const float		JUMP_COMBO_Z_CAN_NOT_JUMP_TIME						= 0.2f;		// June 32b10540
	const float		DASH_JUMP_COMBO_XXXXZ_AIM_TIME						= 0.369f;	// June 32af59c0
	const float		DASH_JUMP_COMBO_XXXXZ_AIM_UP_DEGREE					= -20.f;	// June 32af59c0: GetNearestTargetAimDegree arguments
	const float		DASH_JUMP_COMBO_XXXXZ_AIM_DOWN_DEGREE				= 80.f;
	const float		DASH_JUMP_COMBO_XXXXZ_AIM_YAW_DEGREE				= 120.f;
	const float		DASH_JUMP_COMBO_XXXXZ_AIM_DISTANCE					= 1200.f;
	const float		DASH_JUMP_COMBO_XXXXZ_SPEED_X						= 3500.f;	// June 32af59c0: passive speed with no aim
	const float		DASH_JUMP_COMBO_XXXXZ_SPEED_Y						= -2500.f;
	const double	DASH_JUMP_COMBO_XXXXZ_AIM_ANGLE_OFFSET				= 90.0;		// June 32af59c0: passive speed x = -tan( ( aim + 90 ) in radians ) * -2500
	const double	DASH_JUMP_COMBO_XXXXZ_AIM_SPEED_SCALE				= -2500.0;
	const float		LU_COMBO_XXXZ_END_EFFECT_TIME						= 0.25f;	// June 32b21cd0
	const float		COMBO_XXXZ_END_SCALE_PER_CHARGE						= 0.5f;		// June 32b21cd0 / 32b248c0: scale = 1 + charge * 0.5
	const float		LU_DASH_JUMP_COMBO_ZX_PROJECTILE_TIME				= 0.1f;		// June 32b21f80
	const float		LU_DASH_JUMP_COMBO_ZX_PROJECTILE_MP					= 4.f;
	const float		CIEL_COMBO_X_PROJECTILE_TIME						= 0.28f;	// June 32b23820
	const float		CIEL_COMBO_X_PROJECTILE_MP							= 4.f;
	const float		CIEL_COMBO_XX_PROJECTILE_TIME						= 0.42f;	// June 32b23b80
	const float		CIEL_COMBO_XX_PROJECTILE_MP							= 4.f;
	const float		CIEL_COMBO_XXX_PROJECTILE1_TIME						= 0.622f;	// June 32b23ee0
	const float		CIEL_COMBO_XXX_PROJECTILE1_MP						= 4.f;
	const float		CIEL_COMBO_XXX_PROJECTILE2_TIME						= 0.667f;
	const float		CIEL_COMBO_XXX_PROJECTILE2_MP						= 2.f;
	const float		CIEL_COMBO_XXXX_PROJECTILE_TIME						= 0.577f;	// June 32b24560
	const float		CIEL_COMBO_XXXX_PROJECTILE_MP						= 5.f;
	const float		CIEL_COMBO_XXXZ_END_PROJECTILE_TIME					= 0.05f;	// June 32b248c0
	const float		CIEL_COMBO_XXXZ_END_PROJECTILE_MP					= 5.f;
	const float		CIEL_COMBO_XXXZ_END_PROJECTILE_OFFSET_Y				= 40.f;
	const float		CIEL_COMBO_XXXZ_END_PROJECTILE_OFFSET_FORWARD		= 35.f;
	const float		CIEL_DASH_COMBO_X_PROJECTILE_TIME					= 0.3f;		// June 32b24c40
	const float		CIEL_DASH_COMBO_X_PROJECTILE_MP						= 5.f;
	const float		CIEL_JUMP_COMBO_X_PROJECTILE_TIME					= 0.2f;		// June 32b24fa0
	const float		CIEL_JUMP_COMBO_X_PROJECTILE_MP						= 5.f;
	const float		CIEL_DASH_JUMP_COMBO_ZX_PROJECTILE_TIME				= 0.28f;	// June 32b25300
	const float		CIEL_DASH_JUMP_COMBO_ZX_PROJECTILE_MP				= 4.f;
	const float		CIEL_DASH_JUMP_COMBO_XX_PROJECTILE_TIME				= 0.28f;	// June 32b25610
	const float		CIEL_DASH_JUMP_COMBO_XX_PROJECTILE_MP				= 5.f;
	const float		CIEL_DASH_JUMP_COMBO_XXX_PROJECTILE_TIME			= 0.05f;	// June 32b25970
	const float		CIEL_DASH_JUMP_COMBO_XXX_PROJECTILE_MP				= 5.f;
	const float		CIEL_DASH_JUMP_COMBO_XXXX_PROJECTILE_TIME			= 0.378f;	// June 32b25cd0
	const float		CIEL_DASH_JUMP_COMBO_XXXX_PROJECTILE_MP				= 5.f;
	const int		EFFECT_MODIFIER_TAG_MAIN_MAX_LIFE_TIME				= 2;		// June 321db1b0 modifier tag ( no Enum entry ): the main effect's max life time, multiplied or added
	const int		EFFECT_MODIFIER_TAG_SOUL_COLLECTOR_MAX_COUNT		= 4;		// June 321db1b0 modifier tag ( no Enum entry ): an int on the damage effect, added
}
using namespace _CONST_LUCIEL_CH1F2_;

// char-luciel CH1f2: June 32af2fb0
void CX2GULuciel::LSI_CIEL_COMBO_ZZZZX_EventProcess()
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

// char-luciel CH1f2: June 32af31e0
void CX2GULuciel::LSI_CIEL_COMBO_X_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_CIEL_COMBO_XX )
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af3500 ( X + Down on a Dread Lord unit goes to the Demonio down-X state )
void CX2GULuciel::LSI_CIEL_COMBO_XX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		( true == m_InputData.pureDown && true == m_InputData.oneX && NULL != GetUnit() && CX2Unit::UC_LUCIEL_DREAD_LORD == GetUnit()->GetClass() ), LSI_LLD_M_COMBO_XX_DOWN_X )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_CIEL_COMBO_XXX )
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af38b0
void CX2GULuciel::LSI_CIEL_COMBO_XXX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange, ( true == m_InputData.oneX && true == IsInJobLine( 2 ) ), LSI_LCG_COMBO_XXXX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_CIEL_COMBO_XXXX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, m_InputData.oneZ, LSI_CIEL_COMBO_XXXZ_START )
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af3d30
void CX2GULuciel::LSI_CIEL_COMBO_XXXX_EventProcess()
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

// char-luciel CH1f2: June 32af3f60 ( the loop when Z is still held at the anim end, else the end )
void CX2GULuciel::LSI_CIEL_COMBO_XXXZ_START_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( true == m_InputData.pureZ ? LSI_CIEL_COMBO_XXXZ_LOOP : LSI_CIEL_COMBO_XXXZ_END );
	}

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af3fe0 ( the loop ends at 3 s or when Z is released; the cancels follow as else-ifs )
void CX2GULuciel::LSI_CIEL_COMBO_XXXZ_LOOP_EventProcess()
{
	if( m_FrameDataNow.unitCondition.fStateTime >= CIEL_COMBO_XXXZ_LOOP_END_TIME || false == m_InputData.pureZ )
	{
		StateChange( LSI_CIEL_COMBO_XXXZ_END );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af41e0
void CX2GULuciel::LSI_CIEL_COMBO_XXXZ_END_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_CIEL_COMBO_XXXZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af44d0
void CX2GULuciel::LSI_CIEL_COMBO_XXXZX_EventProcess()
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

// char-luciel CH1f2: June 32af4700 ( X goes to the job line's own state: events 0 / 1 / 2 are the Royal Guard / Chiliarch / Demonio lines )
void CX2GULuciel::LSI_CIEL_DASH_COMBO_Z_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, ( true == m_InputData.oneX && true == IsInJobLine( 2 ) ), LSI_LCG_DASH_COMBO_ZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, ( true == m_InputData.oneX && true == IsInJobLine( 1 ) ), LSI_LLC_M_DASH_COMBO_ZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange, ( true == m_InputData.oneX && true == IsInJobLine( 3 ) ), LSI_LDB_F_DASH_COMBO_ZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af4b90 ( Ciel's dash combo X has no start / landing split; the two lines' events are swapped against Lu's )
void CX2GULuciel::LSI_CIEL_DASH_COMBO_X_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, ( true == m_InputData.oneX && true == IsInJobLine( 2 ) ), LSI_LCG_DASH_COMBO_ZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, ( true == m_InputData.oneX && true == IsInJobLine( 1 ) ), LSI_LLC_M_DASH_COMBO_XX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af4f80
void CX2GULuciel::LSI_CIEL_JUMP_COMBO_X_EventProcess()
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

// char-luciel CH1f2: June 32af4fc0 ( the Chiliarch dash event is tested before the X event )
void CX2GULuciel::LSI_CIEL_DASH_JUMP_COMBO_Z_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		( true == IsInJobLine( 1 ) && ( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft ) ), LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_CIEL_DASH_JUMP_COMBO_ZX )

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af51b0
void CX2GULuciel::LSI_CIEL_DASH_JUMP_COMBO_ZX_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, ( true == m_InputData.oneZ && true == IsInJobLine( 2 ) ), LSI_LCG_M_DASH_JUMP_COMBO_ZXX )

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af52c0 ( the last branch needs the air special attack to return true and Z, JUNE_BUGS )
void CX2GULuciel::LSI_CIEL_DASH_JUMP_COMBO_X_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_CIEL_DASH_JUMP_COMBO_XX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		( true == IsInJobLine( 1 ) && ( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft ) ), LSI_LLC_M_DASH_JUMP_COMBO_X_DASH )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		( true == IsInJobLine( 2 ) && true == m_InputData.oneZ ), LSI_LCG_DASH_JUMP_COMBO_SNIPING_START )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 3, m_fEventTime[3].keyInputStart, m_fEventTime[3].keyInputEnd, m_fEventTime[3].stateChange,
		( true == IsInJobLine( 3 ) && true == m_InputData.oneZ ), LSI_LDB_F_DASH_JUMP_COMBO_XZ_START )
	else if( GetXSkinAnimFuture()->GetNowAnimationTime() > DASH_JUMP_COMBO_X_SPECIAL_ATTACK_TIME
		&& true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) && true == m_InputData.oneZ )
	{
		StateChange( LSI_CIEL_JUMP_COMBO_Z );
	}

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af5690
void CX2GULuciel::LSI_CIEL_DASH_JUMP_COMBO_XX_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_CIEL_DASH_JUMP_COMBO_XXX )

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af5790
void CX2GULuciel::LSI_CIEL_DASH_JUMP_COMBO_XXX_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_CIEL_DASH_JUMP_COMBO_XXXX )

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af5890
void CX2GULuciel::LSI_CIEL_DASH_JUMP_COMBO_XXXX_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ, LSI_CIEL_DASH_JUMP_COMBO_XXXXZ )

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af5990
void CX2GULuciel::LSI_CIEL_DASH_JUMP_COMBO_XXXXZ_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_CIEL_DASH_JUMP_COMBO_XXXXZ_LANDING );
	}

	CommonEventProcess();
}

// char-luciel CH1f2: June 32af59c0 ( June reads the future animation here too, JUNE_BUGS; the passive speed aims at the nearest target )
void CX2GULuciel::LSI_CIEL_DASH_JUMP_COMBO_XXXXZ_FrameMove()
{
	if( true == GetXSkinAnimFuture()->EventTimerOneshot( DASH_JUMP_COMBO_XXXXZ_AIM_TIME ) )
	{
		float fPassiveSpeedX = DASH_JUMP_COMBO_XXXXZ_SPEED_X;

		const D3DXVECTOR3 vAimDegree = GetNearestTargetAimDegree( DASH_JUMP_COMBO_XXXXZ_AIM_UP_DEGREE, DASH_JUMP_COMBO_XXXXZ_AIM_DOWN_DEGREE,
			DASH_JUMP_COMBO_XXXXZ_AIM_YAW_DEGREE, DASH_JUMP_COMBO_XXXXZ_AIM_DISTANCE );
		if( 0.f != vAimDegree.z )
		{
			float fAimRadian = (float)D3DXToRadian( vAimDegree.z + DASH_JUMP_COMBO_XXXXZ_AIM_ANGLE_OFFSET );
			float fAimTan = (float)tan( fAimRadian );
			fPassiveSpeedX = (float)( -fAimTan * DASH_JUMP_COMBO_XXXXZ_AIM_SPEED_SCALE );
		}

		m_PhysicParam.passiveSpeed.x = fPassiveSpeedX;
		m_PhysicParam.passiveSpeed.y = DASH_JUMP_COMBO_XXXXZ_SPEED_Y;
	}

	CommonFrameMove();
}

// char-luciel CH1f2: June 32af5af0
void CX2GULuciel::LSI_CIEL_DASH_JUMP_COMBO_XXXXZ_LANDING_EventProcess()
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

// char-luciel CH1f2: June 32b103b0 ( the charge rate: anim time over the anim's length, at most 1 )
void CX2GULuciel::LSI_LU_COMBO_XXXZ_CHARGE_StateEnd()
{
	CommonStateEnd();

	const float fChargeRate = GetXSkinAnimPtr()->GetNowAnimationTime() / GetXSkinAnimPtr()->GetMaxAnimationTime();
	m_fComboXXXZChargeTime = ( 1.0 < fChargeRate ) ? 1.f : fChargeRate;
}

// char-luciel CH1f2: June 32b10540 ( a held direction the unit faces sets the walk speed in the air )
void CX2GULuciel::LSI_LU_JUMP_COMBO_Z_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		if( true == m_FrameDataFuture.stateParam.bEventFlagList[0] )
		{
			m_fCanNotJumpTime = JUMP_COMBO_Z_CAN_NOT_JUMP_TIME;
		}

		StateChange( LSI_LU_JUMP_LANDING );
	}
	else if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight )
		|| ( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	CommonEventProcess();
}

// char-luciel CH1f2: June 32b11cf0
void CX2GULuciel::LSI_CIEL_JUMP_COMBO_Z_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		if( true == m_FrameDataFuture.stateParam.bEventFlagList[0] )
		{
			m_fCanNotJumpTime = JUMP_COMBO_Z_CAN_NOT_JUMP_TIME;
		}

		StateChange( LSI_LU_JUMP_LANDING );
	}
	else if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight )
		|| ( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	CommonEventProcess();
}

// char-luciel CH1f2: June 32b12ed0 ( state 338's EventProcess: one event, then Lu's jump combo Z body )
void CX2GULuciel::LSI_LU_JUMP_COMBO_Z_SECOND_EventProcess()
{
	if( true == m_FrameDataFuture.stateParam.bEventFlagList[1]
		&& ( m_fEventTime[1].stateChange < GetXSkinAnimFuture()->GetNowAnimationTime() || true == GetXSkinAnimFuture()->IsAnimationEnd() ) )
	{
		StateChange( LSI_LCG_F_DASH_JUMP_COMBO_XZZ_READY );
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( GetXSkinAnimFuture()->GetNowAnimationTime() < m_fEventTime[1].keyInputEnd && m_fEventTime[1].keyInputStart <= GetXSkinAnimFuture()->GetNowAnimationTime()
		&& true == m_InputData.oneDoubleZ && true == IsInJobLine( 2 ) )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
	}

	LSI_LU_JUMP_COMBO_Z_EventProcess();
}

// char-luciel CH1f2: June 32b21cd0 ( the end blast grows with the charge: power x ( 1 + charge ), size and set scale 1 + charge / 2 )
void CX2GULuciel::LSI_LU_COMBO_XXXZ_END_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( LU_COMBO_XXXZ_END_EFFECT_TIME ) )
	{
		const float fPowerRate = ( m_fComboXXXZChargeTime + 1.f ) * GetPowerRate();
		const float fScale = 1.f + m_fComboXXXZChargeTime * COMBO_XXXZ_END_SCALE_PER_CHARGE;
		const D3DXVECTOR3 vScale( fScale, fScale, fScale );

		const CX2EffectSet::Handle hEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_LD_F_Combo_XXXZ_End", this, NULL, IsHyperState(), fPowerRate, -1.f, vScale );
		if( INVALID_EFFECTSET_HANDLE != hEffectSet )
		{
			CX2EffectSet::EffectSetInstance* pEffectSetInstance = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hEffectSet );
			if( NULL != pEffectSetInstance )
			{
				pEffectSetInstance->SetEffectScale( vScale, true );
			}
		}
	}

	CommonFrameMove();
}

// char-luciel CH1f2: June 32b21f80
void CX2GULuciel::LSI_LU_DASH_JUMP_COMBO_ZX_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( LU_DASH_JUMP_COMBO_ZX_PROJECTILE_TIME ) )
	{
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );

		if( NULL != g_pX2Game && NULL != g_pX2Game->GetEffectSet() && true == IsEnoughMPForAttack( LU_DASH_JUMP_COMBO_ZX_PROJECTILE_MP, vBonePos ) )
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_LD_F_Dash_Jump_Combo_ZX_Projectile", this, NULL, IsHyperState(), GetPowerRate() );
		}
	}

	CommonFrameMove();
}

// char-luciel CH1f2: the projectile effect set of Ciel's combos, power and life time scaled by the two Duo rates ( June inlines it in each FrameMove )
void CX2GULuciel::PlayComboProjectileEffectSet( const WCHAR* pwszEffectSetName_, const float fConsumeMP_, const bool bAddMaxCountModifier_ )
{
	const D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );

	if( NULL == g_pX2Game || NULL == g_pX2Game->GetEffectSet() || false == IsEnoughMPForAttack( fConsumeMP_, vBonePos ) )
		return;

	// char-luciel CH1g2: the body moved to PlayRatedProjectileEffectSet, the Royal Guard and Chiliarch combos test the MP themselves
	PlayRatedProjectileEffectSet( pwszEffectSetName_, bAddMaxCountModifier_ );
}

// char-luciel CH1f2: June 32b23820
void CX2GULuciel::LSI_CIEL_COMBO_X_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( CIEL_COMBO_X_PROJECTILE_TIME ) )
	{
		PlayComboProjectileEffectSet( L"EffectSet_LD_M_Combo_X_Projectile", CIEL_COMBO_X_PROJECTILE_MP, true );
	}

	CommonFrameMove();
}

// char-luciel CH1f2: June 32b23b80
void CX2GULuciel::LSI_CIEL_COMBO_XX_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( CIEL_COMBO_XX_PROJECTILE_TIME ) )
	{
		PlayComboProjectileEffectSet( L"EffectSet_LD_M_Combo_X_Projectile", CIEL_COMBO_XX_PROJECTILE_MP, true );
	}

	CommonFrameMove();
}

// char-luciel CH1f2: June 32b23ee0 ( two projectiles, the second at 0.667 s for 2 MP )
void CX2GULuciel::LSI_CIEL_COMBO_XXX_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( CIEL_COMBO_XXX_PROJECTILE1_TIME ) )
	{
		PlayComboProjectileEffectSet( L"EffectSet_LD_M_Combo_XXX_Projectile1", CIEL_COMBO_XXX_PROJECTILE1_MP, true );
	}

	if( true == GetXSkinAnimPtr()->EventTimerOneshot( CIEL_COMBO_XXX_PROJECTILE2_TIME ) )
	{
		PlayComboProjectileEffectSet( L"EffectSet_LD_M_Combo_XXX_Projectile2", CIEL_COMBO_XXX_PROJECTILE2_MP, true );
	}

	CommonFrameMove();
}

// char-luciel CH1f2: June 32b24560
void CX2GULuciel::LSI_CIEL_COMBO_XXXX_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( CIEL_COMBO_XXXX_PROJECTILE_TIME ) )
	{
		PlayComboProjectileEffectSet( L"EffectSet_LD_M_Combo_XXXX_Projectile", CIEL_COMBO_XXXX_PROJECTILE_MP, true );
	}

	CommonFrameMove();
}

// char-luciel CH1f2: June 32b248c0 ( a damage effect, not an effect set: power x ( 1 + charge ), life time rate 1 + charge / 2; the MP test sits on the bone position )
void CX2GULuciel::LSI_CIEL_COMBO_XXXZ_END_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( CIEL_COMBO_XXXZ_END_PROJECTILE_TIME ) )
	{
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );

		D3DXVECTOR3 vPosition = vBonePos;
		vPosition.y += CIEL_COMBO_XXXZ_END_PROJECTILE_OFFSET_Y;

		const D3DXVECTOR3 vForwardOffset = GetDirVector() * CIEL_COMBO_XXXZ_END_PROJECTILE_OFFSET_FORWARD;
		if( true == GetIsRight() )
		{
			vPosition += vForwardOffset;
		}
		else
		{
			vPosition -= vForwardOffset;
		}

		if( NULL != g_pX2Game && NULL != g_pX2Game->GetEffectSet() && true == IsEnoughMPForAttack( CIEL_COMBO_XXXZ_END_PROJECTILE_MP, vBonePos ) )
		{
			const float fPowerRate = ( m_fComboXXXZChargeTime + 1.f ) * GetPowerRate();
			const float fLifeTimeRate = 1.f + m_fComboXXXZChargeTime * COMBO_XXXZ_END_SCALE_PER_CHARGE;

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_LD_M_Combo_XXXZ_Projectile", fPowerRate, vPosition, GetRotateDegree(), GetRotateDegree(),
				m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, fLifeTimeRate, -1, -1.f, 0, -1.f );
		}
	}

	CommonFrameMove();
}

// char-luciel CH1f2: June 32b24c40
void CX2GULuciel::LSI_CIEL_DASH_COMBO_X_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( CIEL_DASH_COMBO_X_PROJECTILE_TIME ) )
	{
		PlayComboProjectileEffectSet( L"EffectSet_LD_M_Combo_X_Projectile_Reverse", CIEL_DASH_COMBO_X_PROJECTILE_MP, true );
	}

	CommonFrameMove();
}

// char-luciel CH1f2: June 32b24fa0
void CX2GULuciel::LSI_CIEL_JUMP_COMBO_X_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( CIEL_JUMP_COMBO_X_PROJECTILE_TIME ) )
	{
		PlayComboProjectileEffectSet( L"EffectSet_LD_M_Combo_X_Projectile", CIEL_JUMP_COMBO_X_PROJECTILE_MP, true );
	}

	CommonFrameMove();
}

// char-luciel CH1f2: June 32b25300 ( the one projectile without the max count modifier )
void CX2GULuciel::LSI_CIEL_DASH_JUMP_COMBO_ZX_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( CIEL_DASH_JUMP_COMBO_ZX_PROJECTILE_TIME ) )
	{
		PlayComboProjectileEffectSet( L"EffectSet_LD_M_Dash_Jump_Combo_ZX_Projectile", CIEL_DASH_JUMP_COMBO_ZX_PROJECTILE_MP, false );
	}

	CommonFrameMove();
}

// char-luciel CH1f2: June 32b25610
void CX2GULuciel::LSI_CIEL_DASH_JUMP_COMBO_XX_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( CIEL_DASH_JUMP_COMBO_XX_PROJECTILE_TIME ) )
	{
		PlayComboProjectileEffectSet( L"EffectSet_LD_M_Dash_Jump_Combo_XX_Projectile", CIEL_DASH_JUMP_COMBO_XX_PROJECTILE_MP, true );
	}

	CommonFrameMove();
}

// char-luciel CH1f2: June 32b25970 ( the same effect set as XX at 0.05 s )
void CX2GULuciel::LSI_CIEL_DASH_JUMP_COMBO_XXX_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( CIEL_DASH_JUMP_COMBO_XXX_PROJECTILE_TIME ) )
	{
		PlayComboProjectileEffectSet( L"EffectSet_LD_M_Dash_Jump_Combo_XX_Projectile", CIEL_DASH_JUMP_COMBO_XXX_PROJECTILE_MP, true );
	}

	CommonFrameMove();
}

// char-luciel CH1f2: June 32b25cd0
void CX2GULuciel::LSI_CIEL_DASH_JUMP_COMBO_XXXX_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( CIEL_DASH_JUMP_COMBO_XXXX_PROJECTILE_TIME ) )
	{
		PlayComboProjectileEffectSet( L"EffectSet_LD_M_Dash_Jump_Combo_XXXX_Projectile", CIEL_DASH_JUMP_COMBO_XXXX_PROJECTILE_MP, true );
	}

	CommonFrameMove();
}

// char-luciel CH1g: Chiliarch / Royal Guard combos ( June 32ae5c70 - 32af8f30 ); June's expansions on the active body's animation
#define BWALK_CANCEL_AFTER_ON_LINE_( time ) \
	else if( (time) >= 0.f && GetXSkinAnimFuture()->GetNowAnimationTime() > (time) && true == IsOnSomethingFuture() && CAN_WALK_CANCEL ) \
	{ StateChange( LSI_LU_WAIT ); }
#define HYPER_MODE_CANCEL_AFTER_( time ) \
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && \
	m_InputData.oneLCtr == true && m_FrameDataFuture.syncData.m_HyperModeCount > 0 && \
	true == CanHyperMode() && \
	(time) >= 0.f && GetXSkinAnimFuture()->GetNowAnimationTime() > (time) && \
	false == IsQuickSlotSpecialItemWaiting() ) \
	{ m_bHyperModeStartedByInput = true; StateChange( GetHypetMpdeStateID() ); }

namespace _CONST_LUCIEL_CH1G_
{
	const float		COMBO_LOOP_END_TIME							= 3.f;		// June 32ae5ce0 / 32ae5ef0: the charge or loop ends at 3 s
	const double	COMBO_LOOP_RELEASE_TIME						= 0.4;		// June 32ae5ef0: X released after this ( compared as a double ) ends the loop
	const float		SNIPING_X_CHARGE_START_MP					= -10.f;	// June 32ae5cb0: MP change at the charge start
	const double	SNIPING_X_CHARGE_MP_PER_SECOND				= 10.0;		// June 32ae5d20: MP drained per second while charging
	const float		SNIPING_X_CHARGE_MAX						= 1.f;		// June 32ae5d20
	const int		SNIPING_Z_FINISH_COUNT						= 5;		// June 32af6330: Z count that ends the chain
	const int		SNIPING_DASH_JUMP_MAX_COUNT					= 8;		// June 32af69b0: cap of the dash jump Z count
	const float		DASH_COMBO_JUMP_LANDING_TIME				= 0.7f;		// June 32af0b50
	const float		EVENT_TIME_NEVER							= 9999.f;	// June 32af7960: the state change time of a state that waits for its animation end
}
using namespace _CONST_LUCIEL_CH1G_;

// char-luciel CH1g: June 32ae5c70
void CX2GULuciel::LSI_LCG_COMBO_ZZZX_StateStartFuture()
{
	CommonStateStartFuture();
	m_iSnipingComboCount = 0;
}

// char-luciel CH1g: June 32ae5c90
void CX2GULuciel::LSI_LCG_COMBO_SNIPING_Z_StateStartFuture()
{
	CommonStateStartFuture();
	++m_iSnipingComboCount;
}

// char-luciel CH1g: June 32ae5cb0
void CX2GULuciel::LSI_LCG_COMBO_SNIPING_X_CHARGE_StateStart()
{
	CommonStateStart();
	m_fChargeTime = 0.f;
	UpNowMp( SNIPING_X_CHARGE_START_MP );
}

// char-luciel CH1g: June 32ae5ce0
void CX2GULuciel::LSI_LCG_COMBO_SNIPING_X_CHARGE_EventProcess()
{
	if( m_FrameDataNow.unitCondition.fStateTime >= COMBO_LOOP_END_TIME || false == m_InputData.pureX )
	{
		StateChange( LSI_LCG_COMBO_SNIPING_X_FIRE );
	}

	CommonEventProcess();
}

// char-luciel CH1g: June 32ae5d20 ( the MP drains and the charge grows only while the MP lasts )
void CX2GULuciel::LSI_LCG_COMBO_SNIPING_X_CHARGE_FrameMove()
{
	CommonFrameMove();

	const float fMpDrain = static_cast<float>( m_fElapsedTime * SNIPING_X_CHARGE_MP_PER_SECOND );
	if( fMpDrain <= GetNowMp() && m_fChargeTime < SNIPING_X_CHARGE_MAX )
	{
		const float fChargeTime = m_fChargeTime + m_fElapsedTime;
		m_fChargeTime = ( SNIPING_X_CHARGE_MAX <= fChargeTime ) ? SNIPING_X_CHARGE_MAX : fChargeTime;
		UpNowMp( -fMpDrain );
	}
}

// char-luciel CH1g: June 32ae5db0
void CX2GULuciel::LSI_LCG_DASH_JUMP_COMBO_SNIPING_START_StateStartFuture()
{
	CommonStateStartFuture();
	m_iSnipingComboMaxCount = 0;
	m_iSnipingComboCount = 0;
}

// char-luciel CH1g: June 32ae5dd0
void CX2GULuciel::LSI_LCG_DASH_JUMP_COMBO_SNIPING_LOOP_StateStartFuture()
{
	CommonStateStartFuture();
	++m_iSnipingComboCount;
}

// char-luciel CH1g: June 32ae5e10
void CX2GULuciel::LSI_LCG_F_COMBO_ZZZX_StateStartFuture()
{
	CommonStateStartFuture();
	m_iSnipingComboCount = 0;
}

// char-luciel CH1g: June 32ae5e30 ( the Future slot runs CommonStateStart and the charge start, JUNE_BUGS 701 )
void CX2GULuciel::LSI_LCG_F_COMBO_ZZZXX_LOOP_StateStartFuture()
{
	CommonStateStart();
	m_fChargeTime = 0.f;
	UpNowMp( SNIPING_X_CHARGE_START_MP );
}

// char-luciel CH1g: June 32ae5e60 ( = 32ae5d20 )
void CX2GULuciel::LSI_LCG_F_COMBO_ZZZXX_LOOP_FrameMove()
{
	CommonFrameMove();

	const float fMpDrain = static_cast<float>( m_fElapsedTime * SNIPING_X_CHARGE_MP_PER_SECOND );
	if( fMpDrain <= GetNowMp() && m_fChargeTime < SNIPING_X_CHARGE_MAX )
	{
		const float fChargeTime = m_fChargeTime + m_fElapsedTime;
		m_fChargeTime = ( SNIPING_X_CHARGE_MAX <= fChargeTime ) ? SNIPING_X_CHARGE_MAX : fChargeTime;
		UpNowMp( -fMpDrain );
	}
}

// char-luciel CH1g: June 32ae5ef0
void CX2GULuciel::LSI_LCG_F_COMBO_ZZZXX_LOOP_EventProcess()
{
	if( m_FrameDataNow.unitCondition.fStateTime >= COMBO_LOOP_END_TIME
		|| ( false == m_InputData.pureX && m_FrameDataNow.unitCondition.fStateTime > COMBO_LOOP_RELEASE_TIME ) )
	{
		StateChange( LSI_LCG_F_COMBO_ZZZXX_END );
	}

	CommonEventProcess();
}

// char-luciel CH1g: June 32ae5f40
void CX2GULuciel::LSI_LCG_F_COMBO_ZZZXZ_LOOP_StateStart()
{
	CommonStateStart();
	++m_iSnipingComboCount;
}

// char-luciel CH1g: June 32ae5f60 ( ends in the ZZZXX END state, JUNE_BUGS 700 )
void CX2GULuciel::LSI_LCG_F_COMBO_XXXXX_LOOP_EventProcess()
{
	if( m_FrameDataNow.unitCondition.fStateTime >= COMBO_LOOP_END_TIME
		|| ( false == m_InputData.pureX && m_FrameDataNow.unitCondition.fStateTime > COMBO_LOOP_RELEASE_TIME ) )
	{
		StateChange( LSI_LCG_F_COMBO_ZZZXX_END );
	}

	CommonEventProcess();
}

// char-luciel CH1g: June 32ae5fb0 ( ends in the ZZZXX END state, JUNE_BUGS 700 )
void CX2GULuciel::LSI_LCG_F_DASH_COMBO_ZXXX_LOOP_EventProcess()
{
	if( m_FrameDataNow.unitCondition.fStateTime >= COMBO_LOOP_END_TIME
		|| ( false == m_InputData.pureX && m_FrameDataNow.unitCondition.fStateTime > COMBO_LOOP_RELEASE_TIME ) )
	{
		StateChange( LSI_LCG_F_COMBO_ZZZXX_END );
	}

	CommonEventProcess();
}

// char-luciel CH1g: June 32ae6000 ( ends in the ZZZXX END state, JUNE_BUGS 700 )
void CX2GULuciel::LSI_LCG_F_DASH_COMBO_XXXX_LOOP_EventProcess()
{
	if( m_FrameDataNow.unitCondition.fStateTime >= COMBO_LOOP_END_TIME
		|| ( false == m_InputData.pureX && m_FrameDataNow.unitCondition.fStateTime > COMBO_LOOP_RELEASE_TIME ) )
	{
		StateChange( LSI_LCG_F_COMBO_ZZZXX_END );
	}

	CommonEventProcess();
}

// char-luciel CH1g: June 32ae6090
void CX2GULuciel::LSI_LCG_F_DASH_JUMP_COMBO_XZZ_READY_StateStartFuture()
{
	CommonStateStartFuture();
	m_iSnipingComboMaxCount = 0;
	m_iSnipingComboCount = 0;
}

// char-luciel CH1g: June 32ae60b0
void CX2GULuciel::LSI_LCG_F_DASH_JUMP_COMBO_XZZ_LOOP_StateStartFuture()
{
	CommonStateStartFuture();
	++m_iSnipingComboCount;
}

// char-luciel CH1g: June 32af0270 ( Z goes on to the Dread Lord's own state )
void CX2GULuciel::LSI_LLC_COMBO_ZZZX_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		( true == m_InputData.oneZ && NULL != GetUnit() && CX2Unit::UC_LUCIEL_DREAD_LORD == GetUnit()->GetClass() ), LSI_LLD_COMBO_ZZZXZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af0570
void CX2GULuciel::LSI_LLC_DASH_COMBO_Z_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LLC_DASH_COMBO_ZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af0860
void CX2GULuciel::LSI_LLC_DASH_COMBO_ZX_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LLC_DASH_COMBO_ZXX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af0b50 ( on the line after 0.7 s the state lands; no animation end test )
void CX2GULuciel::LSI_LLC_DASH_COMBO_ZXX_EventProcess()
{
	if( true == IsOnSomethingFuture() && DASH_COMBO_JUMP_LANDING_TIME < GetXSkinAnimFuture()->GetNowAnimationTime() )
	{
		StateChange( LSI_LU_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		( true == m_InputData.oneZ && true == IsInJobLine( 1 ) ), LSI_LLC_F_DASH_COMBO_ZXXX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af0e50
void CX2GULuciel::LSI_LLC_DASH_COMBO_XX_EventProcess()
{
	if( true == IsOnSomethingFuture() && DASH_COMBO_JUMP_LANDING_TIME < GetXSkinAnimFuture()->GetNowAnimationTime() )
	{
		StateChange( LSI_LU_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		( true == m_InputData.oneZ && true == IsInJobLine( 1 ) ), LSI_LLC_F_DASH_COMBO_XXX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af5d20 ( event 1: Down + X on a Noblesse unit )
void CX2GULuciel::LSI_LCG_DASH_COMBO_ZX_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		( true == m_InputData.pureDown && true == m_InputData.oneX && NULL != GetUnit() && CX2Unit::UC_LUCIEL_NOBLESSE == GetUnit()->GetClass() ), LSI_LCN_DASH_COMBO_ZX_DOWN_X )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		( true == m_InputData.oneX && true == IsInJobLine( 2 ) ), LSI_LCG_DASH_COMBO_ZXX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af6110 ( the animation end test is an if of its own; the cancels run walk, dash, skill )
void CX2GULuciel::LSI_LCG_COMBO_ZZZX_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LCG_COMBO_SNIPING_READY );
	}
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af6330 ( the cancels run walk, dash, skill )
void CX2GULuciel::LSI_LCG_COMBO_SNIPING_Z_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( ( m_iSnipingComboCount < SNIPING_Z_FINISH_COUNT ) ? LSI_LCG_COMBO_SNIPING_READY : LSI_LCG_COMBO_SNIPING_Z_FINISH );
	}
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af6560 ( the cancels run walk, skill, dash )
void CX2GULuciel::LSI_LCG_COMBO_SNIPING_Z_FINISH_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af6780
void CX2GULuciel::LSI_LCG_COMBO_SNIPING_X_FIRE_EventProcess()
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
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af69b0
void CX2GULuciel::LSI_LCG_DASH_JUMP_COMBO_SNIPING_START_EventProcess()
{
	if( true == m_InputData.oneZ && m_iSnipingComboMaxCount < SNIPING_DASH_JUMP_MAX_COUNT )
	{
		++m_iSnipingComboMaxCount;
	}

	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LCG_DASH_JUMP_COMBO_SNIPING_LOOP );
	}

	CommonEventProcess();
}

// char-luciel CH1g: June 32af6a20 ( the cancel chain has the awakening branch only, no special attack )
void CX2GULuciel::LSI_LCG_DASH_JUMP_COMBO_SNIPING_LOOP_EventProcess()
{
	if( true == m_InputData.oneZ && m_iSnipingComboMaxCount < SNIPING_DASH_JUMP_MAX_COUNT )
	{
		++m_iSnipingComboMaxCount;
	}

	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( ( m_iSnipingComboMaxCount > m_iSnipingComboCount ) ? LSI_LCG_DASH_JUMP_COMBO_SNIPING_LOOP : LSI_LCG_DASH_JUMP_COMBO_SNIPING_FINISH );
	}
	HYPER_MODE_CANCEL_AFTER_( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af6c20
void CX2GULuciel::LSI_LCG_DASH_JUMP_COMBO_SNIPING_FINISH_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}

	CommonEventProcess();
}

// char-luciel CH1g: June 32af7960 ( from the state change time on, Ciel's dash jump combo X handles the state )
void CX2GULuciel::LSI_LLC_F_DASH_COMBO_ZXXX_EventProcess()
{
	if( m_fEventTime[0].stateChange <= GetXSkinAnimFuture()->GetNowAnimationTime() )
	{
		LSI_CIEL_DASH_JUMP_COMBO_X_EventProcess();
		return;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() && EVENT_TIME_NEVER == m_fEventTime[0].stateChange )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af7c10
void CX2GULuciel::LSI_LLC_F_DASH_COMBO_XXX_EventProcess()
{
	if( m_fEventTime[0].stateChange <= GetXSkinAnimFuture()->GetNowAnimationTime() )
	{
		LSI_CIEL_DASH_JUMP_COMBO_X_EventProcess();
		return;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() && EVENT_TIME_NEVER == m_fEventTime[0].stateChange )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af7ec0
void CX2GULuciel::LSI_LLC_M_COMBO_ZZZX_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		( true == m_InputData.oneZ && NULL != GetUnit() && CX2Unit::UC_LUCIEL_DREAD_LORD == GetUnit()->GetClass() ), LSI_LLD_M_COMBO_ZZZXZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af8230
void CX2GULuciel::LSI_LLC_M_DASH_COMBO_ZX_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		( true == m_InputData.oneX && true == IsInJobLine( 1 ) ), LSI_LLC_M_DASH_COMBO_ZXX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af85a0
void CX2GULuciel::LSI_LLC_M_DASH_COMBO_ZXX_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		( true == m_InputData.oneZ && true == IsInJobLine( 1 ) ), LSI_LLC_M_DASH_COMBO_ZXXX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af8910 ( from the state change time on, Lu's dash jump combo X handles the state )
void CX2GULuciel::LSI_LLC_M_DASH_COMBO_ZXXX_EventProcess()
{
	if( m_fEventTime[0].stateChange <= GetXSkinAnimFuture()->GetNowAnimationTime() )
	{
		LSI_LU_DASH_JUMP_COMBO_X_EventProcess();
		return;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() && EVENT_TIME_NEVER == m_fEventTime[0].stateChange )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af8bc0
void CX2GULuciel::LSI_LLC_M_DASH_COMBO_XX_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		( true == m_InputData.oneZ && true == IsInJobLine( 1 ) ), LSI_LLC_M_DASH_COMBO_XXX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g: June 32af8f30 ( from the state change time on, Lu's dash jump combo X handles the state )
void CX2GULuciel::LSI_LLC_M_DASH_COMBO_XXX_EventProcess()
{
	if( m_fEventTime[0].stateChange <= GetXSkinAnimFuture()->GetNowAnimationTime() )
	{
		LSI_LU_DASH_JUMP_COMBO_X_EventProcess();
		return;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() && EVENT_TIME_NEVER == m_fEventTime[0].stateChange )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g2: Chiliarch / Royal Guard combos part 2 ( June 32af91e0 - 32b3bc70 )
namespace _CONST_LUCIEL_CH1G2_
{
	const float		SNIPING_X_FIRE_POWER_RATE_BASE				= 1.f;		// June 32b1cba0: the state power rate is multiplied by charge + 1.0 ( added as a double )
	const float		LLC_COMBO_ZZZX_EFFECT_TIME					= 0.4f;		// June 32b22240
	const float		LLC_M_COMBO_ZZZX_EFFECT_TIME				= 0.28f;	// June 32b27b60
	const float		LLC_COMBO_ZZZX_EFFECT_MP					= 3.f;		// June 32b22240 / 32b27b60
	const float		LLC_COMBO_ZZZX_EXPLOSIVE_RANDOM_LIMIT		= 0.5f;		// June 32b22240: a random above it plays the explosive set
	const float		LLC_COMBO_ZZZX_ENHANCE_RANDOM_LIMIT			= 0.2f;		// June 32b22240: else a random above it plays the enhance set, else the healing set
	const float		SNIPING_Z_EFFECT_TIME						= 0.04f;	// June 32b26030
	const float		SNIPING_Z_EFFECT_MP							= 5.f;
	const float		SNIPING_X_FIRE_EFFECT_TIME					= 0.01f;	// June 32b266a0
	const float		DASH_JUMP_SNIPING_LOOP_EFFECT_TIME_1		= 0.03f;	// June 32b26a40 / 32b287b0: any of the three times plays the effect
	const float		DASH_JUMP_SNIPING_LOOP_EFFECT_TIME_2		= 0.1f;
	const float		DASH_JUMP_SNIPING_LOOP_EFFECT_TIME_3		= 0.16f;
	const float		DASH_JUMP_SNIPING_LOOP_EFFECT_MP			= 2.f;
	const float		F_COMBO_ZZZXX_END_EFFECT_TIME				= 0.01f;	// June 32b28020
	const float		F_COMBO_ZZZXZ_LOOP_EFFECT_TIME_1			= 0.0864f;	// June 32b282c0: any of the three times plays the effect
	const float		F_COMBO_ZZZXZ_LOOP_EFFECT_TIME_2			= 0.216f;
	const float		F_COMBO_ZZZXZ_LOOP_EFFECT_TIME_3			= 0.367f;
	const float		F_COMBO_ZZZXZ_LOOP_EFFECT_MP				= 1.f;
	const double	COMBO_EFFECT_ROTATE_RANGE					= 4.0;		// June 32b26030 / 32b26a40 / 32b282c0 / 32b287b0: z rotation = random * 4 - 2 degrees
	const double	COMBO_EFFECT_ROTATE_BASE					= 2.0;
	const float		SNIPING_READY_MIN_MP						= 10.f;		// June 32b3b530 / 32b3bc70: less MP than this and the X key shows the lack of MP
}
using namespace _CONST_LUCIEL_CH1G2_;

// char-luciel CH1g2: the projectile effect set of the Royal Guard and Chiliarch combos, no MP test ( the callers test it ); PlayComboProjectileEffectSet runs it after its own test
CX2EffectSet::Handle CX2GULuciel::PlayRatedProjectileEffectSet( const WCHAR* pwszEffectSetName_, const bool bAddMaxCountModifier_ )
{
	// June passes m_fSoulCollectorDamageRate as the life time too ( JUNE_BUGS 699 )
	const CX2EffectSet::Handle hEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( pwszEffectSetName_, this, NULL, IsHyperState(),
		GetPowerRate() * m_fPowerOfOriaxDamageRate, m_fSoulCollectorDamageRate, D3DXVECTOR3( 1.f, 1.f, 1.f ) );

	g_pX2Game->GetEffectSet()->AddDamageEffectModifier( hEffectSet, EFFECT_MODIFIER_TAG_MAIN_MAX_LIFE_TIME, m_fSoulCollectorDamageRate, false );

	if( true == bAddMaxCountModifier_ )
	{
		g_pX2Game->GetEffectSet()->AddDamageEffectModifier( hEffectSet, EFFECT_MODIFIER_TAG_SOUL_COLLECTOR_MAX_COUNT, static_cast<float>( m_usSoulCollectorMaxCount ), true );
	}

	return hEffectSet;
}

// char-luciel CH1g2: the effect set's local z rotation ( June writes the instance's +0xe4 .. +0xec with 0, 0, degree )
void CX2GULuciel::SetEffectSetLocalRotateZ( const CX2EffectSet::Handle hEffectSet_, const float fRotateDegree_ )
{
	CX2EffectSet::EffectSetInstance* pEffectSetInstance = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hEffectSet_ );
	if( NULL != pEffectSetInstance )
	{
		pEffectSetInstance->SetLocalRotateDegree( D3DXVECTOR3( 0.f, 0.f, fRotateDegree_ ) );
	}
}

// char-luciel CH1g2: a held direction in the air sets the facing and the walk speed ( the block of LSI_LU_JUMP_DOWN_NO_ATTACK_FrameMoveFuture; June inlines it in each dash jump combo )
void CX2GULuciel::SetAirWalkSpeedByInput()
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
}

// char-luciel CH1g2: June 32af91e0 ( the animation end test is an if of its own; the cancels run walk, dash, skill )
void CX2GULuciel::LSI_LCG_F_COMBO_ZZZX_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LCG_F_COMBO_ZZZX_WAIT );
	}
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g2: June 32af9400 ( the cancels run skill, walk, back walk, dash )
void CX2GULuciel::LSI_LCG_F_COMBO_ZZZXX_END_EventProcess()
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
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g2: June 32af98c0 ( the cancels run skill, walk, back walk, dash )
void CX2GULuciel::LSI_LCG_F_COMBO_ZZZXZ_END_EventProcess()
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
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g2: June 32af9690 ( the cancels run walk, dash, skill )
void CX2GULuciel::LSI_LCG_F_COMBO_ZZZXZ_LOOP_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( ( m_iSnipingComboCount < SNIPING_Z_FINISH_COUNT ) ? LSI_LCG_F_COMBO_ZZZX_WAIT : LSI_LCG_F_COMBO_ZZZXZ_END );
	}
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g2: June 32af9b90
void CX2GULuciel::LSI_LCG_F_DASH_COMBO_Z_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_LCG_F_DASH_COMBO_ZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g2: June 32af9e80 ( event 1: Down + X on a Noblesse unit )
void CX2GULuciel::LSI_LCG_F_DASH_COMBO_ZX_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		( true == m_InputData.pureDown && true == m_InputData.oneX && NULL != GetUnit() && CX2Unit::UC_LUCIEL_NOBLESSE == GetUnit()->GetClass() ), LSI_LCN_F_DASH_COMBO_ZX_DOWN_X )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		( true == m_InputData.oneX && true == IsInJobLine( 2 ) ), LSI_LCG_F_DASH_COMBO_ZXX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g2: June 32afa2d0 ( event 1: Down + X on a Noblesse unit )
void CX2GULuciel::LSI_LCG_F_DASH_COMBO_XX_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		( true == m_InputData.pureDown && true == m_InputData.oneX && NULL != GetUnit() && CX2Unit::UC_LUCIEL_NOBLESSE == GetUnit()->GetClass() ), LSI_LCN_F_DASH_COMBO_XX_DOWN_X )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		( true == m_InputData.oneX && true == IsInJobLine( 2 ) ), LSI_LCG_F_DASH_COMBO_XXX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g2: June 32afa720
void CX2GULuciel::LSI_LCG_F_DASH_JUMP_COMBO_ZXX_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32afa750
void CX2GULuciel::LSI_LCG_M_DASH_JUMP_COMBO_ZXX_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32afa780
void CX2GULuciel::LSI_LCG_F_DASH_JUMP_COMBO_XZZ_READY_EventProcess()
{
	if( true == m_InputData.oneZ && m_iSnipingComboMaxCount < SNIPING_DASH_JUMP_MAX_COUNT )
	{
		++m_iSnipingComboMaxCount;
	}

	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LCG_F_DASH_JUMP_COMBO_XZZ_LOOP );
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32afa7f0 ( the cancel chain has the awakening branch only, no special attack )
void CX2GULuciel::LSI_LCG_F_DASH_JUMP_COMBO_XZZ_LOOP_EventProcess()
{
	if( true == m_InputData.oneZ && m_iSnipingComboMaxCount < SNIPING_DASH_JUMP_MAX_COUNT )
	{
		++m_iSnipingComboMaxCount;
	}

	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( ( m_iSnipingComboMaxCount > m_iSnipingComboCount ) ? LSI_LCG_F_DASH_JUMP_COMBO_XZZ_LOOP : LSI_LCG_F_DASH_JUMP_COMBO_XZZ_END );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_LU_DASH_JUMP_COMBO_XX_START )
	HYPER_MODE_CANCEL_AFTER_( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g2: June 32afaac0
void CX2GULuciel::LSI_LCG_F_DASH_JUMP_COMBO_XZZ_END_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_LU_DASH_JUMP_COMBO_XX_START )

	CommonEventProcess();
}

// char-luciel CH1g2: June 32b105d0 ( events 2 and 3: Up and Down go to the two follow-up dashes )
void CX2GULuciel::LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_LU_DASH_JUMP_COMBO_ZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ, LSI_LU_JUMP_COMBO_Z )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		m_InputData.oneUp, LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_UP )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 3, m_fEventTime[3].keyInputStart, m_fEventTime[3].keyInputEnd, m_fEventTime[3].stateChange,
		m_InputData.oneDown, LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_DOWN )
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		SetAirWalkSpeedByInput();
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32b10980
void CX2GULuciel::LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_UP_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_LU_DASH_JUMP_COMBO_ZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ, LSI_LU_JUMP_COMBO_Z )
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		SetAirWalkSpeedByInput();
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32b10bd0
void CX2GULuciel::LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_DOWN_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_LU_DASH_JUMP_COMBO_ZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ, LSI_LU_JUMP_COMBO_Z )
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		SetAirWalkSpeedByInput();
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32b10e20 ( events 2 and 3: Up and Down go to the two follow-up dashes )
void CX2GULuciel::LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_LU_DASH_JUMP_COMBO_XX_START )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ, LSI_LU_JUMP_COMBO_Z )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		m_InputData.oneUp, LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_UP )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 3, m_fEventTime[3].keyInputStart, m_fEventTime[3].keyInputEnd, m_fEventTime[3].stateChange,
		m_InputData.oneDown, LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_DOWN )
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		SetAirWalkSpeedByInput();
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32b111d0
void CX2GULuciel::LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_UP_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_LU_DASH_JUMP_COMBO_XX_START )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ, LSI_LU_JUMP_COMBO_Z )
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		SetAirWalkSpeedByInput();
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32b11420
void CX2GULuciel::LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_DOWN_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_LU_DASH_JUMP_COMBO_XX_START )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ, LSI_LU_JUMP_COMBO_Z )
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		SetAirWalkSpeedByInput();
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32b11d80 ( events 2 and 3: Up and Down go to the two follow-up dashes )
void CX2GULuciel::LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_CIEL_DASH_JUMP_COMBO_ZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ, LSI_CIEL_JUMP_COMBO_Z )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		m_InputData.oneUp, LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_UP )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 3, m_fEventTime[3].keyInputStart, m_fEventTime[3].keyInputEnd, m_fEventTime[3].stateChange,
		m_InputData.oneDown, LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_DOWN )
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		SetAirWalkSpeedByInput();
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32b12130
void CX2GULuciel::LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_UP_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_CIEL_DASH_JUMP_COMBO_ZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ, LSI_CIEL_JUMP_COMBO_Z )
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		SetAirWalkSpeedByInput();
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32b12380
void CX2GULuciel::LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_DOWN_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_CIEL_DASH_JUMP_COMBO_ZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ, LSI_CIEL_JUMP_COMBO_Z )
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		SetAirWalkSpeedByInput();
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32b125d0 ( events 2 and 3: Up and Down go to the two follow-up dashes )
void CX2GULuciel::LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_CIEL_DASH_JUMP_COMBO_XX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ, LSI_CIEL_JUMP_COMBO_Z )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		m_InputData.oneUp, LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_UP )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 3, m_fEventTime[3].keyInputStart, m_fEventTime[3].keyInputEnd, m_fEventTime[3].stateChange,
		m_InputData.oneDown, LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_DOWN )
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		SetAirWalkSpeedByInput();
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32b12980
void CX2GULuciel::LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_UP_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_CIEL_DASH_JUMP_COMBO_XX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ, LSI_CIEL_JUMP_COMBO_Z )
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		SetAirWalkSpeedByInput();
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32b12bd0
void CX2GULuciel::LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_DOWN_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX, LSI_CIEL_DASH_JUMP_COMBO_XX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ, LSI_CIEL_JUMP_COMBO_Z )
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		SetAirWalkSpeedByInput();
	}

	CommonEventProcess();
}

// char-luciel CH1g2: June 32b1cba0 ( the charge grows the state power rate )
void CX2GULuciel::LSI_LCG_COMBO_SNIPING_X_FIRE_StateStart()
{
	CommonStateStart();
	m_fNowStatePowerRate *= static_cast<float>( m_fChargeTime + static_cast<double>( SNIPING_X_FIRE_POWER_RATE_BASE ) );
}

// char-luciel CH1g2: June 32b1cc60 ( also the start of three other F END states )
void CX2GULuciel::LSI_LCG_F_COMBO_ZZZXX_END_StateStart()
{
	CommonStateStart();
	m_fNowStatePowerRate *= static_cast<float>( m_fChargeTime + static_cast<double>( SNIPING_X_FIRE_POWER_RATE_BASE ) );
}

// char-luciel CH1g2: June 32b22240
void CX2GULuciel::LSI_LLC_COMBO_ZZZX_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( LLC_COMBO_ZZZX_EFFECT_TIME ) )
	{
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );

		if( NULL != g_pX2Game && NULL != g_pX2Game->GetEffectSet() && true == IsEnoughMPForAttack( LLC_COMBO_ZZZX_EFFECT_MP, vBonePos ) )
		{
			PlaySound( L"LuCiel_SwordFall.ogg", true, false );

			const WCHAR* pwszEffectSetName = L"EffectSet_LLC_F_Combo_ZZZX_HEALING";
			if( LLC_COMBO_ZZZX_EXPLOSIVE_RANDOM_LIMIT < GetRandomFloat( 0 ) )
			{
				pwszEffectSetName = L"EffectSet_LLC_F_Combo_ZZZX_EXPLOSIVE";
			}
			else if( LLC_COMBO_ZZZX_ENHANCE_RANDOM_LIMIT < GetRandomFloat( 0 ) )
			{
				pwszEffectSetName = L"EffectSet_LLC_F_Combo_ZZZX_ENHANCE";
			}

			g_pX2Game->GetEffectSet()->PlayEffectSet( pwszEffectSetName, this, NULL, IsHyperState(), GetPowerRate() );
		}
	}

	CommonFrameMove();
}

// char-luciel CH1g2: June 32b27b60 ( the M state plays the F effect sets: June has no M ones )
void CX2GULuciel::LSI_LLC_M_COMBO_ZZZX_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( LLC_M_COMBO_ZZZX_EFFECT_TIME ) )
	{
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );

		if( NULL != g_pX2Game && NULL != g_pX2Game->GetEffectSet() && true == IsEnoughMPForAttack( LLC_COMBO_ZZZX_EFFECT_MP, vBonePos ) )
		{
			PlaySound( L"LuCiel_SwordFall.ogg", true, false );

			const WCHAR* pwszEffectSetName = L"EffectSet_LLC_F_Combo_ZZZX_HEALING";
			if( LLC_COMBO_ZZZX_EXPLOSIVE_RANDOM_LIMIT < GetRandomFloat( 0 ) )
			{
				pwszEffectSetName = L"EffectSet_LLC_F_Combo_ZZZX_EXPLOSIVE";
			}
			else if( LLC_COMBO_ZZZX_ENHANCE_RANDOM_LIMIT < GetRandomFloat( 0 ) )
			{
				pwszEffectSetName = L"EffectSet_LLC_F_Combo_ZZZX_ENHANCE";
			}

			g_pX2Game->GetEffectSet()->PlayEffectSet( pwszEffectSetName, this, NULL, IsHyperState(), GetPowerRate() );
		}
	}

	CommonFrameMove();
}

// char-luciel CH1g2: June 32b26030
void CX2GULuciel::LSI_LCG_COMBO_SNIPING_Z_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( SNIPING_Z_EFFECT_TIME ) )
	{
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );

		if( NULL != g_pX2Game && NULL != g_pX2Game->GetEffectSet() && true == IsEnoughMPForAttack( SNIPING_Z_EFFECT_MP, vBonePos ) )
		{
			const float fRotateDegree = static_cast<float>( GetRandomFloat( 0 ) * COMBO_EFFECT_ROTATE_RANGE - COMBO_EFFECT_ROTATE_BASE );

			const CX2EffectSet::Handle hLoopEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_LCG_M_ComboZZZXZ_Loop", this );
			SetEffectSetLocalRotateZ( hLoopEffectSet, fRotateDegree );
			m_vecEffectSetToDeleteOnStateEnd.push_back( hLoopEffectSet );

			const CX2EffectSet::Handle hProjectileEffectSet = PlayRatedProjectileEffectSet( L"EffectSet_LCG_M_Sniping_Z_Projectile", true );
			SetEffectSetLocalRotateZ( hProjectileEffectSet, fRotateDegree );
		}
	}

	CommonFrameMove();
}

// char-luciel CH1g2: June 32b266a0 ( June reads the minor unit's animation here: the Sniping states are Ciel's )
void CX2GULuciel::LSI_LCG_COMBO_SNIPING_X_FIRE_FrameMove()
{
	if( true == m_pMinorUnitXSkinAnim->EventTimerOneshot( SNIPING_X_FIRE_EFFECT_TIME ) )
	{
		const CX2EffectSet::Handle hFinishEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_LCG_M_ComboZZZXX_Finish", this );
		m_vecEffectSetToDeleteOnStateEnd.push_back( hFinishEffectSet );

		PlayRatedProjectileEffectSet( L"EffectSet_LCG_M_Sniping_X_Projectile", true );
	}

	CommonFrameMove();
}

// char-luciel CH1g2: June 32b26a40 ( not enough MP ends the loop: the count goes to the max count + 1 )
void CX2GULuciel::LSI_LCG_DASH_JUMP_COMBO_SNIPING_LOOP_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( DASH_JUMP_SNIPING_LOOP_EFFECT_TIME_1 )
		|| true == GetXSkinAnimPtr()->EventTimerOneshot( DASH_JUMP_SNIPING_LOOP_EFFECT_TIME_2 )
		|| true == GetXSkinAnimPtr()->EventTimerOneshot( DASH_JUMP_SNIPING_LOOP_EFFECT_TIME_3 ) )
	{
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );

		if( NULL != g_pX2Game && NULL != g_pX2Game->GetEffectSet() && true == IsEnoughMPForAttack( DASH_JUMP_SNIPING_LOOP_EFFECT_MP, vBonePos ) )
		{
			const float fRotateDegree = static_cast<float>( GetRandomFloat( 0 ) * COMBO_EFFECT_ROTATE_RANGE - COMBO_EFFECT_ROTATE_BASE );

			const CX2EffectSet::Handle hLoopEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_LCG_M_DashJumpComboXZ_Loop", this );
			SetEffectSetLocalRotateZ( hLoopEffectSet, fRotateDegree );
			m_vecEffectSetToDeleteOnStateEnd.push_back( hLoopEffectSet );

			const CX2EffectSet::Handle hProjectileEffectSet = PlayRatedProjectileEffectSet( L"EffectSet_LCG_M_Dash_Jump_Combo_XZ_Projectile", true );
			SetEffectSetLocalRotateZ( hProjectileEffectSet, fRotateDegree );
		}
		else
		{
			m_iSnipingComboCount = m_iSnipingComboMaxCount + 1;
		}
	}

	CommonFrameMove();
}

// char-luciel CH1g2: June 32b28020
void CX2GULuciel::LSI_LCG_F_COMBO_ZZZXX_END_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( F_COMBO_ZZZXX_END_EFFECT_TIME ) )
	{
		PlayRatedProjectileEffectSet( L"EffectSet_LCG_F_ComboZZZXX_Loop", true );
	}

	CommonFrameMove();
}

// char-luciel CH1g2: June 32b282c0
void CX2GULuciel::LSI_LCG_F_COMBO_ZZZXZ_LOOP_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( F_COMBO_ZZZXZ_LOOP_EFFECT_TIME_1 )
		|| true == GetXSkinAnimPtr()->EventTimerOneshot( F_COMBO_ZZZXZ_LOOP_EFFECT_TIME_2 )
		|| true == GetXSkinAnimPtr()->EventTimerOneshot( F_COMBO_ZZZXZ_LOOP_EFFECT_TIME_3 ) )
	{
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );

		if( NULL != g_pX2Game && NULL != g_pX2Game->GetEffectSet() && true == IsEnoughMPForAttack( F_COMBO_ZZZXZ_LOOP_EFFECT_MP, vBonePos ) )
		{
			const float fRotateDegree = static_cast<float>( GetRandomFloat( 0 ) * COMBO_EFFECT_ROTATE_RANGE - COMBO_EFFECT_ROTATE_BASE );

			const CX2EffectSet::Handle hEffectSet = PlayRatedProjectileEffectSet( L"EffectSet_LCG_F_ComboZZZXZ_Loop", true );
			SetEffectSetLocalRotateZ( hEffectSet, fRotateDegree );
		}
	}

	CommonFrameMove();
}

// char-luciel CH1g2: June 32b287b0 ( not enough MP ends the loop: the count goes to the max count + 1 )
void CX2GULuciel::LSI_LCG_F_DASH_JUMP_COMBO_XZZ_LOOP_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( DASH_JUMP_SNIPING_LOOP_EFFECT_TIME_1 )
		|| true == GetXSkinAnimPtr()->EventTimerOneshot( DASH_JUMP_SNIPING_LOOP_EFFECT_TIME_2 )
		|| true == GetXSkinAnimPtr()->EventTimerOneshot( DASH_JUMP_SNIPING_LOOP_EFFECT_TIME_3 ) )
	{
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );

		if( NULL != g_pX2Game && NULL != g_pX2Game->GetEffectSet() && true == IsEnoughMPForAttack( DASH_JUMP_SNIPING_LOOP_EFFECT_MP, vBonePos ) )
		{
			const float fRotateDegree = static_cast<float>( GetRandomFloat( 0 ) * COMBO_EFFECT_ROTATE_RANGE - COMBO_EFFECT_ROTATE_BASE );

			const CX2EffectSet::Handle hEffectSet = PlayRatedProjectileEffectSet( L"EffectSet_LCG_F_DashJumpComboXZ_Loop", true );
			SetEffectSetLocalRotateZ( hEffectSet, fRotateDegree );
		}
		else
		{
			m_iSnipingComboCount = m_iSnipingComboMaxCount + 1;
		}
	}

	CommonFrameMove();
}

// char-luciel CH1g2: June 32b3a290 ( both bodies leave their transformed form, then the shared win start )
void CX2GULuciel::LSI_WIN_StateStart()
{
	EndTransformedForm( 0, false );
	EndTransformedForm( 1, false );
	WinStateStart();
}

// char-luciel CH1g2: June 32b3a2b0
void CX2GULuciel::LSI_LOSE_StateStart()
{
	EndTransformedForm( 0, false );
	EndTransformedForm( 1, false );
	LoseStateStart();
}

// char-luciel CH1g2: June 32b3b530 ( event 3 reads event 2's times, JUNE_BUGS 702; event 1 shows the lack of MP first, then goes on to its own state )
void CX2GULuciel::LSI_LCG_COMBO_SNIPING_READY_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneZ, LSI_LCG_COMBO_SNIPING_Z )
	else if( true == m_FrameDataFuture.stateParam.bEventFlagList[1]
		&& ( (float)m_fEventTime[1].stateChange < GetXSkinAnimFuture()->GetNowAnimationTime() || true == GetXSkinAnimFuture()->IsAnimationEnd() )
		&& GetNowMp() < SNIPING_READY_MIN_MP )
	{
		g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		( true == m_InputData.pureUp && true == m_InputData.oneX ), LSI_LCN_SNIPING_COMBO_UP_X )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 3, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		( true == m_InputData.pureDown && true == m_InputData.oneX ), LSI_LCN_SNIPING_COMBO_DOWN_X )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.pureX, LSI_LCG_COMBO_SNIPING_X_CHARGE )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1g2: June 32b3bc70 ( event 3 reads event 2's times, JUNE_BUGS 702; event 1 shows the lack of MP first, then goes on to its own state )
void CX2GULuciel::LSI_LCG_F_COMBO_ZZZX_WAIT_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneZ, LSI_LCG_F_COMBO_ZZZXZ_LOOP )
	else if( true == m_FrameDataFuture.stateParam.bEventFlagList[1]
		&& ( (float)m_fEventTime[1].stateChange < GetXSkinAnimFuture()->GetNowAnimationTime() || true == GetXSkinAnimFuture()->IsAnimationEnd() )
		&& GetNowMp() < SNIPING_READY_MIN_MP )
	{
		g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		( true == m_InputData.pureUp && true == m_InputData.oneX ), LSI_LCN_F_COMBO_ZZZX_UP_X )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 3, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		( true == m_InputData.pureDown && true == m_InputData.oneX ), LSI_LCN_F_COMBO_ZZZX_DOWN_X )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.pureX, LSI_LCG_F_COMBO_ZZZXX_LOOP )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: Dread Lord / Noblesse combos ( June 32ae5940 - 32b29570 )
namespace _CONST_LUCIEL_CH1H_
{
	const double	COMBO_XX_DOWN_XX_LOOP_REPEAT_TIME			= 0.3f;		// char-luciel CH1i review: June 32af16e0 / 32afb450 add 0.3f widened to a double ( 0.30000001 )
	const USHORT	DASH_COMBO_DOWN_XX_LOOP_MAX_COUNT			= 5;		// June 32af6fc0: loop count limit of the dash X loops
	const float		COMBO_DOWN_X_EFFECT_TIME					= 0.01f;	// June 32b15b20 / 32b270b0 and their siblings
	const float		DASH_COMBO_DOWN_XX_LOOP_TARGET_RANGE		= 700.f;	// June 32b15d40: the make target is followed within this distance
	const float		COMBO_END_EFFECT_TIME						= 0.01f;	// June 32b272c0 / 32b28ca0
	const float		COMBO_END_EFFECT_MP							= 10.f;		// June 32b272c0 / 32b28ca0
}
using namespace _CONST_LUCIEL_CH1H_;

// char-luciel CH1h: June 32ae5940
void CX2GULuciel::LSI_LLD_COMBO_XX_DOWN_XX_LOOP_StateStartFuture()
{
	CommonStateStartFuture();
	m_fComboXXDownXXLoopTime = m_fEventTime[0].keyInputStart;
}

// char-luciel CH1h: June 32ae60d0
void CX2GULuciel::LSI_LLD_M_COMBO_XX_DOWN_XX_LOOP_StateStartFuture()
{
	CommonStateStartFuture();
	m_fComboXXDownXXLoopTime = m_fEventTime[0].keyInputStart;
}

// char-luciel CH1h: June 32ae5df0
void CX2GULuciel::LSI_LCN_DASH_COMBO_ZX_DOWN_XX_LOOP_StateStartFuture()
{
	CommonStateStartFuture();
	++m_usStateLoopCount;
}

// char-luciel CH1h: June 32ae60f0
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_LOOP_StateStartFuture()
{
	CommonStateStartFuture();
	++m_usStateLoopCount;
}

// char-luciel CH1h: June 32ae6110
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_LOOP_StateStartFuture()
{
	CommonStateStartFuture();
	++m_usStateLoopCount;
}

// char-luciel CH1h: June 32af6c50 ( the make target, the effect handle and position, the loop count are reset )
void CX2GULuciel::LSI_LCN_DASH_COMBO_ZX_DOWN_X_StateStartFuture()
{
	CommonStateStartFuture();
	SetMakeTarget( -1, GUT_USER );
	m_hDashComboDownXDamageEffect = INVALID_DAMAGE_EFFECT_HANDLE;
	m_vDashComboDownXPosition = D3DXVECTOR3( 0.f, 0.f, 0.f );
	m_usStateLoopCount = 0;
}

// char-luciel CH1h: June 32afcdd0
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_ZX_DOWN_X_StateStartFuture()
{
	CommonStateStartFuture();
	SetMakeTarget( -1, GUT_USER );
	m_hDashComboDownXDamageEffect = INVALID_DAMAGE_EFFECT_HANDLE;
	m_vDashComboDownXPosition = D3DXVECTOR3( 0.f, 0.f, 0.f );
	m_usStateLoopCount = 0;
}

// char-luciel CH1h: June 32afd6e0
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_XX_DOWN_X_StateStartFuture()
{
	CommonStateStartFuture();
	SetMakeTarget( -1, GUT_USER );
	m_hDashComboDownXDamageEffect = INVALID_DAMAGE_EFFECT_HANDLE;
	m_vDashComboDownXPosition = D3DXVECTOR3( 0.f, 0.f, 0.f );
	m_usStateLoopCount = 0;
}

// char-luciel CH1h: June 32af1180
void CX2GULuciel::LSI_LLD_COMBO_ZZZXZ_EventProcess()
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

// char-luciel CH1h: June 32af1950
void CX2GULuciel::LSI_LLD_COMBO_XX_DOWN_XX_LOOP_X_EventProcess()
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

// char-luciel CH1h: June 32af72d0
void CX2GULuciel::LSI_LCN_DASH_COMBO_ZX_DOWN_XX_END_EventProcess()
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

// char-luciel CH1h: June 32af7500
void CX2GULuciel::LSI_LCN_SNIPING_COMBO_UP_X_EventProcess()
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
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32af7730
void CX2GULuciel::LSI_LCN_SNIPING_COMBO_DOWN_X_EventProcess()
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
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afabc0 ( the cancels run skill, walk, back walk, dash )
void CX2GULuciel::LSI_LLD_M_COMBO_ZZZXZ_EventProcess()
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
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afb6c0
void CX2GULuciel::LSI_LLD_M_COMBO_XX_DOWN_XX_END_EventProcess()
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
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afb950
void CX2GULuciel::LSI_LCN_F_COMBO_ZZZX_UP_X_EventProcess()
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
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afbbe0
void CX2GULuciel::LSI_LCN_F_COMBO_ZZZX_DOWN_X_EventProcess()
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
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afbe70
void CX2GULuciel::LSI_LCN_F_COMBO_XXXX_UP_X_EventProcess()
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
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afc100
void CX2GULuciel::LSI_LCN_F_COMBO_XXXX_DOWN_X_EventProcess()
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
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afc390
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_ZXX_UP_X_EventProcess()
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
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afc620
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_ZXX_DOWN_X_EventProcess()
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
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afc8b0
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_XXX_UP_X_EventProcess()
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
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afcb40
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_XXX_DOWN_X_EventProcess()
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
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afd450
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_END_EventProcess()
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
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afdd60
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_END_EventProcess()
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
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afb1c0 ( the animation end goes on to the loop )
void CX2GULuciel::LSI_LLD_M_COMBO_XX_DOWN_XX_READY_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LLD_M_COMBO_XX_DOWN_XX_LOOP );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32af13b0
void CX2GULuciel::LSI_LLD_COMBO_XX_DOWN_X_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LLD_COMBO_XX_DOWN_XX_READY )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afae50 ( X on a Dread Lord unit )
void CX2GULuciel::LSI_LLD_M_COMBO_XX_DOWN_X_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		( true == m_InputData.oneX && NULL != GetUnit() && CX2Unit::UC_LUCIEL_DREAD_LORD == GetUnit()->GetClass() ), LSI_LLD_M_COMBO_XX_DOWN_XX_READY )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32af16a0
void CX2GULuciel::LSI_LLD_COMBO_XX_DOWN_XX_READY_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LLD_COMBO_XX_DOWN_XX_LOOP );
	}

	CommonEventProcess();
}

// char-luciel CH1h: June 32af16e0 ( the cancel chain opens the function; the repeat and end tests follow it )
void CX2GULuciel::LSI_LLD_COMBO_XX_DOWN_XX_LOOP_EventProcess()
{
	if( false )
	{
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	else if( m_fEventTime[0].keyInputStart <= m_FrameDataFuture.unitCondition.fStateTime && m_FrameDataFuture.unitCondition.fStateTime <= m_fEventTime[0].keyInputEnd )
	{
		if( m_fComboXXDownXXLoopTime + COMBO_XX_DOWN_XX_LOOP_REPEAT_TIME < m_FrameDataFuture.unitCondition.fStateTime )
		{
			StateChange( LSI_LLD_COMBO_XX_DOWN_XX_LOOP_X );
		}
		if( true == m_InputData.oneX )
		{
			m_fComboXXDownXXLoopTime = m_FrameDataFuture.unitCondition.fStateTime;
		}
	}

	if( m_fEventTime[0].stateChange <= m_FrameDataFuture.unitCondition.fStateTime )
	{
		StateChange( LSI_LLD_COMBO_XX_DOWN_XX_LOOP_X );
	}

	CommonEventProcess();
}

// char-luciel CH1h: June 32afb450
void CX2GULuciel::LSI_LLD_M_COMBO_XX_DOWN_XX_LOOP_EventProcess()
{
	if( false )
	{
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	else if( m_fEventTime[0].keyInputStart <= m_FrameDataFuture.unitCondition.fStateTime && m_FrameDataFuture.unitCondition.fStateTime <= m_fEventTime[0].keyInputEnd )
	{
		if( m_fComboXXDownXXLoopTime + COMBO_XX_DOWN_XX_LOOP_REPEAT_TIME < m_FrameDataFuture.unitCondition.fStateTime )
		{
			StateChange( LSI_LLD_M_COMBO_XX_DOWN_XX_END );
		}
		if( true == m_InputData.oneX )
		{
			m_fComboXXDownXXLoopTime = m_FrameDataFuture.unitCondition.fStateTime;
		}
	}

	if( m_fEventTime[0].stateChange <= m_FrameDataFuture.unitCondition.fStateTime )
	{
		StateChange( LSI_LLD_M_COMBO_XX_DOWN_XX_END );
	}

	CommonEventProcess();
}

// char-luciel CH1h: June 32af6cc0 ( the event comes before the animation end )
void CX2GULuciel::LSI_LCN_DASH_COMBO_ZX_DOWN_X_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LCN_DASH_COMBO_ZX_DOWN_XX_LOOP )
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afce40
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_ZX_DOWN_X_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_LOOP )
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afd750
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_XX_DOWN_X_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_LOOP )
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LU_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32af6fc0 ( X again repeats the loop up to five times )
void CX2GULuciel::LSI_LCN_DASH_COMBO_ZX_DOWN_XX_LOOP_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, ( true == m_InputData.oneX && m_usStateLoopCount < DASH_COMBO_DOWN_XX_LOOP_MAX_COUNT ), LSI_LCN_DASH_COMBO_ZX_DOWN_XX_LOOP )
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LCN_DASH_COMBO_ZX_DOWN_XX_END );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afd140
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_LOOP_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, ( true == m_InputData.oneX && m_usStateLoopCount < DASH_COMBO_DOWN_XX_LOOP_MAX_COUNT ), LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_LOOP )
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_END );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32afda50
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_LOOP_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( LSI_LU_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, ( true == m_InputData.oneX && m_usStateLoopCount < DASH_COMBO_DOWN_XX_LOOP_MAX_COUNT ), LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_LOOP )
	else if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_END );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1h: June 32b15b20 ( the XX and non-F loops play the same set name ( JUNE_BUGS 703 ) )
void CX2GULuciel::LSI_LCN_DASH_COMBO_ZX_DOWN_XX_LOOP_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( COMBO_DOWN_X_EFFECT_TIME ) )
	{
		CX2GameUnit* pMakeTargetUnit = g_pX2Game->GetUnitByUIDAndType( GetMakeTargetUnitUID(), GetMakeTargetGameUnitType() );
		if( NULL != pMakeTargetUnit && 0.f < pMakeTargetUnit->GetNowHp() )
		{
			m_vDashComboDownXPosition = pMakeTargetUnit->GetPos();
		}

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_LCN_F_DashComboZXDownXXLoop", this, NULL, false, -1.f, -1.f, D3DXVECTOR3( 1.f, 1.f, 1.f ), true, m_vDashComboDownXPosition );
	}

	CommonFrameMove();
}

// char-luciel CH1h: June 32b15d40 ( a target within 700 units is followed )
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_LOOP_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( COMBO_DOWN_X_EFFECT_TIME ) )
	{
		CX2GameUnit* pMakeTargetUnit = g_pX2Game->GetUnitByUIDAndType( GetMakeTargetUnitUID(), GetMakeTargetGameUnitType() );
		if( NULL != pMakeTargetUnit && 0.f < pMakeTargetUnit->GetNowHp() )
		{
			const D3DXVECTOR3 vToTarget = pMakeTargetUnit->GetPos() - GetPos();
			if( D3DXVec3Length( &vToTarget ) < DASH_COMBO_DOWN_XX_LOOP_TARGET_RANGE )
			{
				m_vDashComboDownXPosition = pMakeTargetUnit->GetPos();
			}
		}

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_LCN_F_DashComboZXDownXXLoop", this, NULL, false, -1.f, -1.f, D3DXVECTOR3( 1.f, 1.f, 1.f ), true, m_vDashComboDownXPosition );
	}

	CommonFrameMove();
}

// char-luciel CH1h: June 32b15fe0
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_LOOP_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( COMBO_DOWN_X_EFFECT_TIME ) )
	{
		CX2GameUnit* pMakeTargetUnit = g_pX2Game->GetUnitByUIDAndType( GetMakeTargetUnitUID(), GetMakeTargetGameUnitType() );
		if( NULL != pMakeTargetUnit && 0.f < pMakeTargetUnit->GetNowHp() )
		{
			m_vDashComboDownXPosition = pMakeTargetUnit->GetPos();
		}

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_LCN_F_DashComboZXDownXXLoop", this, NULL, false, -1.f, -1.f, D3DXVECTOR3( 1.f, 1.f, 1.f ), true, m_vDashComboDownXPosition );
	}

	CommonFrameMove();
}

// char-luciel CH1h: June 32b270b0 ( the effect is made on the timer; its position is read every frame ( JUNE_BUGS 703 ) )
void CX2GULuciel::LSI_LCN_DASH_COMBO_ZX_DOWN_X_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( COMBO_DOWN_X_EFFECT_TIME ) )
	{
		m_hDashComboDownXDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( this, L"DamageEffect_LCN_F_DashComboZXDownX_Projectile", GetPowerRate(),
			GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, 0, -1.f );
	}

	CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_hDashComboDownXDamageEffect );
	if( NULL != pDamageEffect && NULL != pDamageEffect->GetMainEffect() )
	{
		m_vDashComboDownXPosition = pDamageEffect->GetMainEffect()->GetPos();
	}

	CommonFrameMove();
}

// char-luciel CH1h: June 32b29360
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_ZX_DOWN_X_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( COMBO_DOWN_X_EFFECT_TIME ) )
	{
		m_hDashComboDownXDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( this, L"DamageEffect_LCN_F_DashComboZXDownX_Projectile", GetPowerRate(),
			GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, 0, -1.f );
	}

	CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_hDashComboDownXDamageEffect );
	if( NULL != pDamageEffect && NULL != pDamageEffect->GetMainEffect() )
	{
		m_vDashComboDownXPosition = pDamageEffect->GetMainEffect()->GetPos();
	}

	CommonFrameMove();
}

// char-luciel CH1h: June 32b29570
void CX2GULuciel::LSI_LCN_F_DASH_COMBO_XX_DOWN_X_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( COMBO_DOWN_X_EFFECT_TIME ) )
	{
		m_hDashComboDownXDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( this, L"DamageEffect_LCN_F_DashComboZXDownX_Projectile", GetPowerRate(),
			GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, 0, -1.f );
	}

	CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_hDashComboDownXDamageEffect );
	if( NULL != pDamageEffect && NULL != pDamageEffect->GetMainEffect() )
	{
		m_vDashComboDownXPosition = pDamageEffect->GetMainEffect()->GetPos();
	}

	CommonFrameMove();
}

// char-luciel CH1h: June 32b272c0 ( the first set's handle is not kept )
void CX2GULuciel::LSI_LCN_SNIPING_COMBO_UP_X_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( COMBO_END_EFFECT_TIME ) )
	{
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );

		if( NULL != g_pX2Game && NULL != g_pX2Game->GetEffectSet() && true == IsEnoughMPForAttack( COMBO_END_EFFECT_MP, vBonePos ) )
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_LCN_M_SnipingUpX", this );
			PlayRatedProjectileEffectSet( L"EffectSet_LCN_M_SnipingUpX_Projectile", true );
		}
	}

	CommonFrameMove();
}

// char-luciel CH1h: June 32b27710
void CX2GULuciel::LSI_LCN_SNIPING_COMBO_DOWN_X_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( COMBO_END_EFFECT_TIME ) )
	{
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );

		if( NULL != g_pX2Game && NULL != g_pX2Game->GetEffectSet() && true == IsEnoughMPForAttack( COMBO_END_EFFECT_MP, vBonePos ) )
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_LCN_M_SnipingDownX", this );
			PlayRatedProjectileEffectSet( L"EffectSet_LCN_M_SnipingDownX_Projectile", true );
		}
	}

	CommonFrameMove();
}

// char-luciel CH1h: June 32b28ca0 ( one function behind four states )
void CX2GULuciel::LSI_LCN_F_COMBO_ZZZX_UP_X_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( COMBO_END_EFFECT_TIME ) )
	{
		PlayComboProjectileEffectSet( L"EffectSet_LCN_F_ComboZZZXUpX_End", COMBO_END_EFFECT_MP, true );
	}

	CommonFrameMove();
}

// char-luciel CH1h: June 32b29000 ( one function behind four states )
void CX2GULuciel::LSI_LCN_F_COMBO_ZZZX_DOWN_X_FrameMove()
{
	if( true == GetXSkinAnimPtr()->EventTimerOneshot( COMBO_END_EFFECT_TIME ) )
	{
		PlayComboProjectileEffectSet( L"EffectSet_LCN_F_ComboZZZXDownX_End", COMBO_END_EFFECT_MP, true );
	}

	CommonFrameMove();
}

// char-luciel CH1i: Demonio script states ( June 32ae9b40 - 32b39e70 ); the macro is June's expansion on the body not selected
#define ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_NOT_SELECT_( iEventFlagIndex, fInputTimingStart, fInputTiming, fChangeTiming, bCondition, iStateID ) \
	else if( true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() && m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && \
		( (float)(fChangeTiming) < GetNotSelectXSkinAnimFuture()->GetNowAnimationTime() || true == GetNotSelectXSkinAnimFuture()->IsAnimationEnd() ) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() && \
		GetNotSelectXSkinAnimFuture()->GetNowAnimationTime() < (float)(fInputTiming) && (float)(fInputTimingStart) <= GetNotSelectXSkinAnimFuture()->GetNowAnimationTime() && true == (bCondition) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }

namespace _CONST_LUCIEL_CH1I_
{
	const float		DASH_JUMP_LANDING_SPEED_X_ADD						= 1000.f;	// June 32ae9b40: LandingPrediction( 1000, -1000, pos, 5 )
	const float		DASH_JUMP_LANDING_MIN_Y_OFFSET						= -1000.f;	// June 32ae9b40
	const int		DASH_JUMP_LANDING_STEP_COUNT						= 5;		// June 32ae9b40
	const double	DASH_COMBO_Z_LANDING_FALL_OFFSET					= 45.0;		// June 32b0c220: the jump-down branch drops the unit ( a double; the other states use LINE_RADIUS )
	const double	BOUNDING_TARGET_UNIT_WIDTH_RATE						= 0.5;		// June 32b341b0 / 32b343b0 / 32b35fd0: half the target's width
	const float		BOUNDING_TARGET_LAND_RADIUS							= 30.f;		// June 32b341b0 / 32b343b0 / 32b35fd0: GetLandPosition radius
	const double	BOUNDING_TARGET_Z_START_STAND_DISTANCE_ADD			= 30.0;		// June 32b343b0
	const double	BOUNDING_TARGET_CIEL_STRIKER_STAND_DISTANCE_ADD		= 20.0;		// June 32b35fd0
	const float		RIDING_HYPER_MODE_SHOCK_TIME						= 0.1f;		// June 32b39e70
	const DWORD		HYPER_MODE_WORLD_COLOR								= 0xff222222;	// June 32b37d80: SetWorldColor inlined ( 2 / 15, alpha 1 )
	const float		HYPER_MODE_CRASH_CAMERA_DISTANCE					= 30.f;		// June 32b37d80
	const float		HYPER_MODE_CRASH_CAMERA_TIME						= 0.4f;		// June 32b37d80

	// June 32011d20 / 31fd66b0 / 32011f50 ( the awakening effect-set family ); stand-in: not ported ( Stage 12 ), June's empty-name default
	void AwakeningEffectBegin()			{}
	bool AwakeningEffectSetPresent()	{ return false; }
	void AwakeningEffectPlay()			{}
}
using namespace _CONST_LUCIEL_CH1I_;

// char-luciel CH1i: June 32ae9b40 ( the landing point is predicted; June does not call CommonStateStartFuture here )
void CX2GULuciel::LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING_StateStartFuture()
{
	D3DXVECTOR3 vLandPosition = m_FrameDataFuture.syncData.position;
	LandingPrediction( DASH_JUMP_LANDING_SPEED_X_ADD, DASH_JUMP_LANDING_MIN_Y_OFFSET, &vLandPosition, DASH_JUMP_LANDING_STEP_COUNT );
	m_PhysicParam.nowSpeed.x = 0.f;
	m_FrameDataFuture.syncData.position = vLandPosition;
}

// char-luciel CH1i: June 32b08230 ( duo and not-selected body, no USEFUL_VALUE gate )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_STRIKER_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_CIEL_COMMAND_X : LSI_BOUNDING_TARGET_LU_COMMAND_X ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b085b0 ( event 0 needs no key )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_X_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, true, ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_CIEL_COMMAND_X_LU_STRIKER : LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b08940 ( event 0 is timed on the body not selected )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_NOT_SELECT_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_CIEL_COMMAND_XX : LSI_BOUNDING_TARGET_LU_COMMAND_XX ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b08d30
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_CIEL_COMMAND_XXX : LSI_BOUNDING_TARGET_LU_COMMAND_XXX ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b090d0
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXX : LSI_BOUNDING_TARGET_LU_COMMAND_XXXX ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b09470
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXX : LSI_BOUNDING_TARGET_LU_COMMAND_XXXXX ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b09810
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXX : LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXX ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i review: June 32b09bb0 ( event 0 is timed on the body not selected; its state change also clears event flag 1, a word store at @32b09cc2 )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	else if( true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() && m_FrameDataFuture.stateParam.bEventFlagList[0] == true &&
		( m_fEventTime[0].stateChange < GetNotSelectXSkinAnimFuture()->GetNowAnimationTime() || true == GetNotSelectXSkinAnimFuture()->IsAnimationEnd() ) )
	{
		StateChange( ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX : LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX ) );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() &&
		GetNotSelectXSkinAnimFuture()->GetNowAnimationTime() < m_fEventTime[0].keyInputEnd && m_fEventTime[0].keyInputStart <= GetNotSelectXSkinAnimFuture()->GetNowAnimationTime() && true == m_InputData.oneX )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b09fa0 ( Demonio only: event 0 is timed on the body not selected )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	else if( NULL != GetUnit() && CX2Unit::UC_LUCIEL_DEMONIO == GetUnit()->GetClass() )
	{
		if( false )
		{
		}
		ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_NOT_SELECT_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ, ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_Z_START : LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_START ) )
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0a380 ( the next state is the other body's )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_START_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LOOP : LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_Z_LOOP ) );
	}

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0a3e0 ( the next state is the other body's )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LOOP_EventProcess()
{
	if( true == m_FrameDataFuture.unitCondition.bFootOnLine )
	{
		StateChange( ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LANDING : LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_Z_END ) );
	}

	CommonEventProcess();
}

// char-luciel CH1i review: June 32b0a420 ( the next state is the other body's, JUNE_BUGS 704 )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LANDING_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ, ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZ : LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZ ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i review: June 32b0a780 ( the next state is the other body's, JUNE_BUGS 704 )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZ_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ, ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZ : LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZZ ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i review: June 32b0ab20 ( the next state is the other body's, JUNE_BUGS 704 )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZ_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZX : LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZZX ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0aec0
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	else if( true == m_InputData.oneSit )
	{
		CheckBoundingTarget();
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0b190
void CX2GULuciel::LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZZX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	else if( true == m_InputData.oneSit )
	{
		CheckBoundingTarget();
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0b460
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_X_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XX_START : LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_START ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0b7c0
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_START_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XX_LOOP : LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LOOP ) );
	}

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0b820
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LOOP_EventProcess()
{
	if( true == m_FrameDataFuture.unitCondition.bFootOnLine )
	{
		StateChange( ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XX_LANDING : LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LANDING ) );
	}

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0b860
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LANDING_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ, ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XXZ : LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZ ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0bbc0
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZ_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, ( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XXZX : LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZX ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0bf60 ( no Sit event )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0c220 ( the jump-down branch drops the unit 45 units )
void CX2GULuciel::LSI_LDB_F_DASH_COMBO_Z_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		if( true == m_FrameDataFuture.unitCondition.bFootOnLine )
		{
			StateChange( LSI_LU_WAIT );
		}
		else
		{
			StateChange( LSI_LU_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y = static_cast<float>( m_FrameDataFuture.syncData.position.y - DASH_COMBO_Z_LANDING_FALL_OFFSET );
		}
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LDB_M_DASH_COMBO_ZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0c580
void CX2GULuciel::LSI_LDB_F_DASH_COMBO_ZX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, ( true == IsMinorUnitActive() ? LSI_LDB_M_DASH_COMBO_ZXX : LSI_LDB_F_DASH_COMBO_ZXX ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0c920
void CX2GULuciel::LSI_LDB_F_DASH_COMBO_ZXX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, ( true == IsMinorUnitActive() ? LSI_LDB_M_DASH_COMBO_ZXXX : LSI_LDB_F_DASH_COMBO_ZXXX ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0ccc0
void CX2GULuciel::LSI_LDB_F_DASH_COMBO_ZXXX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, ( true == IsMinorUnitActive() ? LSI_LDB_M_DASH_COMBO_ZXXXX : LSI_LDB_F_DASH_COMBO_ZXXXX ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0d060
void CX2GULuciel::LSI_LDB_F_DASH_COMBO_ZXXXX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ, ( true == IsMinorUnitActive() ? LSI_LDB_M_DASH_COMBO_ZXXXXZ : LSI_LDB_F_DASH_COMBO_ZXXXXZ ) )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0d400
void CX2GULuciel::LSI_LDB_F_DASH_COMBO_ZXXXXZ_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	else if( true == m_InputData.oneSit )
	{
		CheckBoundingTarget();
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0d6d0
void CX2GULuciel::LSI_LDB_F_DASH_JUMP_COMBO_XZ_START_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( ( true == IsMinorUnitActive() ? LSI_LDB_M_DASH_JUMP_COMBO_XZ_LANDING : LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING ) );
	}

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0d730
void CX2GULuciel::LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING_ATTACK );
	}

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0d770
void CX2GULuciel::LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING_ATTACK_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LDB_F_DASH_JUMP_COMBO_XZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0dac0
void CX2GULuciel::LSI_LDB_F_DASH_JUMP_COMBO_XZX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	else if( true == m_InputData.oneSit )
	{
		CheckBoundingTarget();
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0dd90
void CX2GULuciel::LSI_LDB_M_DASH_JUMP_COMBO_XZ_LANDING_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( LSI_LDB_M_DASH_JUMP_COMBO_XZ_LANDING_ATTACK );
	}

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0ddd0
void CX2GULuciel::LSI_LDB_M_DASH_JUMP_COMBO_XZ_LANDING_ATTACK_EventProcess()
{
	if( true == GetXSkinAnimFuture()->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX, LSI_LDB_M_DASH_JUMP_COMBO_XZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b0e120
void CX2GULuciel::LSI_LDB_M_DASH_JUMP_COMBO_XZX_EventProcess()
{
	CKTDGXSkinAnim* pFutureAnim = GetXSkinAnimFuture();
	if( 0.f < GetUsefulValueNow( 0, 0 ) && true == IsDuoEnabled() && NULL != GetNotSelectXSkinAnimFuture() )
	{
		pFutureAnim = GetNotSelectXSkinAnimFuture();
	}

	if( true == pFutureAnim->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? LSI_LU_WAIT : LSI_LU_JUMP_DOWN );
	}
	else if( true == m_InputData.oneSit )
	{
		CheckBoundingTarget();
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )

	CommonEventProcess();
}

// char-luciel CH1i: June 32b14c00
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_STRIKER_StateEnd()
{
	CommonStateEnd();

	if( true == m_bBoundingStrikeHit )
	{
		UpCombinationPoint( -m_iBoundingStrikeCount );
		m_bBoundingStrikeHit = false;
	}
}

// char-luciel CH1i: June 32b14c30 ( the position is the unit I attacked )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_X_StateStartFuture()
{
	D3DXVECTOR3 vPosition = GetPos( true );

	CX2GameUnit* pAttackedUnit = GetGameUnitAttackedByMe();
	if( NULL != pAttackedUnit )
	{
		vPosition = pAttackedUnit->GetPos( true );
	}
	m_FrameDataFuture.syncData.position = vPosition;

	CommonStateStartFuture();
}

// char-luciel CH1i: June 32b1f2b0 ( the hyper mode shock particles at the body bone; the STR_TOKANG text is a major particle here )
void CX2GULuciel::ShowMinorParticleHyperModeShockOfBody()
{
	CKTDGParticleSystem* pMinorParticleSystem = g_pX2Game->GetMinorParticle();
	if( NULL == pMinorParticleSystem )
		return;

	wstring wstrBoneName;
	wstrBoneName += ( true == IsMinorUnitActive() ? L"MINOR:" : L"MAJOR:" );	// char-luciel CH1i review: +0x8e90 == 1 picks MINOR: ( @32b1f336 )
	wstrBoneName += L"Bip01";
	const D3DXVECTOR3 vBonePos = GetBonePos( wstrBoneName.c_str() );

	CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWave = pMinorParticleSystem->GetInstanceSequence( GetHandleCommonMinorParticleByEnum( COMMON_MINOR_PII_GROUND_SHOCK_WAVE ) );
	PlaySequenceByTriggerCount( pSeqGroundShockWave, m_FrameDataNow.unitCondition.landPosition.x, m_FrameDataNow.unitCondition.landPosition.y + 5.0f, m_FrameDataNow.unitCondition.landPosition.z, 5, 10, 3 );

	CKTDGParticleSystem::CParticleEventSequence* pSeqWhiteShockWave = pMinorParticleSystem->GetInstanceSequence( GetHandleCommonMinorParticleByEnum( COMMON_MINOR_PII_WHITE_SHOCK_WAVE ) );
	PlaySequenceByTriggerCount( pSeqWhiteShockWave, vBonePos, 10, 10, 1 );

	CKTDGParticleSystem::CParticleEventSequence* pSeqImpactTick = pMinorParticleSystem->GetInstanceSequence( GetHandleCommonMinorParticleByEnum( COMMON_MINOR_PII_IMPACT_TICK ) );
	PlaySequenceByTriggerCount( pSeqImpactTick, vBonePos, 200, 200, 30 );

	if( true == GetShowSmallGageAndName() && NULL != g_pX2Game->GetMajorParticle() )
	{
		g_pX2Game->GetMajorParticle()->CreateSequence( this, L"LD_STR_TuKang_M", vBonePos );
	}
}

// char-luciel CH1i: June 32b37d80 ( the Lu / Ciel hyper mode change; the state table's FrameMove calls it with its two times )
void CX2GULuciel::CommonHyperModeFrameMove( const float fChangeTime_, const float fShockTime_, const bool bSound_ )
{
	g_pX2Game->GetWorld()->SetWorldColor( HYPER_MODE_WORLD_COLOR );

	if( true == GetXSkinAnimPtr()->EventTimerOneshot( fChangeTime_ ) )
	{
		ShowMinorParticleHyperModeChange( L"Bip01_Spine1" );

		if( true == bSound_ )
		{
			PlaySound( L"Stickslash1.ogg" );
		}
	}

	if( true == GetXSkinAnimPtr()->EventTimerOneshot( fShockTime_ ) )
	{
		AwakeningEffectBegin();
		if( true == AwakeningEffectSetPresent() )
		{
			AwakeningEffectPlay();
		}

		ShowMinorParticleHyperModeShockOfBody();

		UpDownCrashCamera( HYPER_MODE_CRASH_CAMERA_DISTANCE, HYPER_MODE_CRASH_CAMERA_TIME );
		g_pKTDXApp->GetDGManager()->ClearScreen();

		if( false == AwakeningEffectSetPresent() )
		{
			ShowMinorParticleHyperModeTrace();
		}

		ApplyHyperModeBuff();

		if( true == bSound_ )
		{
			PlaySound( L"Slash.ogg" );
			PlaySound( L"Break.ogg" );
			PlaySound( L"Change.ogg" );
		}
	}

	CommonFrameMove();
}

// char-luciel CH1i: June 32b34100
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_STRIKER_StateStartFuture()
{
	CX2GameUnit* pBoundingTarget = g_pX2Game->GetUnitByUIDAndType( m_BoundingTarget.m_uidGameUnit, m_BoundingTarget.m_eGameUnitType );
	if( true == g_pX2Game->IsValidUnit( pBoundingTarget ) && 0.f < pBoundingTarget->GetNowHp() )
	{
		m_FrameDataFuture.syncData.vNotSelectPosition = pBoundingTarget->GetPos();
	}

	CommonStateStartFuture();
}

// char-luciel CH1i: June 32b341b0 ( a stand point beside the bounding target )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_X_StateStartFuture()
{
	CommonStateStartFuture();

	CX2GameUnit* pBoundingTarget = g_pX2Game->GetUnitByUIDAndType( m_BoundingTarget.m_uidGameUnit, m_BoundingTarget.m_eGameUnitType );
	if( NULL == pBoundingTarget )
		return;

	D3DXVECTOR3 vStandPosition = GetPos( true );
	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	const bool bStandRight = ( false == GetIsRight() );
	const float fStandDistance = static_cast<float>( pBoundingTarget->GetUnitWidth( false ) * BOUNDING_TARGET_UNIT_WIDTH_RATE + GetUsefulValueFuture( 0, 0 ) );
	pLineMap->GetFixedDistancePositionForWeaponTaker( &pBoundingTarget->GetPos( true ), fStandDistance, bStandRight, vStandPosition );

	int iLandLineIndex = -1;
	vStandPosition = g_pX2Game->GetWorld()->GetLineMap()->GetLandPosition( vStandPosition, true, BOUNDING_TARGET_LAND_RADIUS, &iLandLineIndex );

	const CKTDGLineMap::LineData* pLandLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( iLandLineIndex );
	if( NULL != pLandLineData && CKTDGLineMap::LT_NORMAL == pLandLineData->lineType && true == pLandLineData->bEnable )
	{
		m_FrameDataFuture.syncData.position = vStandPosition;
		m_FrameDataFuture.syncData.position.y = pBoundingTarget->GetPos( true ).y;
	}
}

// char-luciel CH1i: June 32b343b0 ( a stand point beside the unit I attacked )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_START_StateStartFuture()
{
	D3DXVECTOR3 vPosition = GetPos( true );

	CX2GameUnit* pAttackedUnit = GetGameUnitAttackedByMe();
	if( NULL != pAttackedUnit )
	{
		D3DXVECTOR3 vStandPosition = vPosition;
		CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
		const bool bStandRight = ( false == GetIsRight() );
		const float fStandDistance = static_cast<float>( pAttackedUnit->GetUnitWidth( false ) * BOUNDING_TARGET_UNIT_WIDTH_RATE + BOUNDING_TARGET_Z_START_STAND_DISTANCE_ADD );
		pLineMap->GetFixedDistancePositionForWeaponTaker( &pAttackedUnit->GetPos( false ), fStandDistance, bStandRight, vStandPosition );

		int iLandLineIndex = -1;
		vStandPosition = g_pX2Game->GetWorld()->GetLineMap()->GetLandPosition( vStandPosition, true, BOUNDING_TARGET_LAND_RADIUS, &iLandLineIndex );

		const CKTDGLineMap::LineData* pLandLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( iLandLineIndex );
		if( NULL != pLandLineData && CKTDGLineMap::LT_NORMAL == pLandLineData->lineType && true == pLandLineData->bEnable )
		{
			vPosition = vStandPosition;
			vPosition.y = pAttackedUnit->GetPos( true ).y;
		}
		else
		{
			vPosition = pAttackedUnit->GetPos( true );
		}
	}
	m_FrameDataFuture.syncData.position = vPosition;

	CommonStateStartFuture();
}

// char-luciel CH1i: June 32b35fd0 ( both bodies get a stand point beside the bounding target )
void CX2GULuciel::LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture()
{
	CommonStateStartFuture();

	if( NULL == g_pX2Game->GetWorld() || NULL == g_pX2Game->GetWorld()->GetLineMap() )
		return;

	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	CX2GameUnit* pBoundingTarget = g_pX2Game->GetUnitByUIDAndType( m_BoundingTarget.m_uidGameUnit, m_BoundingTarget.m_eGameUnitType );
	if( NULL == pBoundingTarget || false == ( 0.f < pBoundingTarget->GetNowHp() ) )
		return;

	// char-luciel CH1i review: the target's line group must differ from mine
	const CKTDGLineMap::LineGroupID targetLineGroupID = pLineMap->GetLineGroupIncludesLineData( pBoundingTarget->GetLastTouchLineIndex( true ) );
	const CKTDGLineMap::LineGroupID myLineGroupID = pLineMap->GetLineGroupIncludesLineData( GetLastTouchLineIndex( true ) );
	if( false == targetLineGroupID.IsValid() || false == myLineGroupID.IsValid() || false == ( targetLineGroupID != myLineGroupID ) )
		return;

	D3DXVECTOR3 vStandPosition = GetPos( true );
	const bool bStandRight = ( false == GetIsRight() );
	const float fStandDistance = static_cast<float>( pBoundingTarget->GetUnitWidth( false ) * BOUNDING_TARGET_UNIT_WIDTH_RATE + BOUNDING_TARGET_CIEL_STRIKER_STAND_DISTANCE_ADD );
	pLineMap->GetFixedDistancePositionForWeaponTaker( &pBoundingTarget->GetPos( true ), fStandDistance, bStandRight, vStandPosition );

	int iLandLineIndex = -1;
	vStandPosition = pLineMap->GetLandPosition( vStandPosition, true, BOUNDING_TARGET_LAND_RADIUS, &iLandLineIndex );

	const CKTDGLineMap::LineData* pLandLineData = pLineMap->GetLineData( iLandLineIndex );
	if( NULL != pLandLineData && CKTDGLineMap::LT_NORMAL == pLandLineData->lineType && true == pLandLineData->bEnable )
	{
		m_FrameDataFuture.syncData.position = vStandPosition;
		m_FrameDataFuture.syncData.position.y = pBoundingTarget->GetPos( true ).y;
		m_FrameDataFuture.syncData.vNotSelectPosition = vStandPosition;
		m_FrameDataFuture.syncData.vNotSelectPosition.y = pBoundingTarget->GetPos( true ).y;
	}
}

// char-luciel CH1i: June 32b39e70
void CX2GULuciel::LSI_RIDING_HYPER_MODE_FrameMove()
{
	CommonHyperModeFrameMove( 0.f, RIDING_HYPER_MODE_SHOCK_TIME, true );
}


// char-luciel CH1k: overrides band 1 part 1 ( June 32ae4080 - 32b1e8e0 )
namespace _CONST_LUCIEL_CH1K_
{
	const float		BOUNDING_KEY_DIALOG_MOVE_TIME			= 0.07f;	// June 32b1e8e0 @32b1ea11 ( 3344b78c )
	const int		TRANSFORMED_AFTER_IMAGE_NUM				= 10;		// June 32b162e0 @32b1662b
	const float		TRANSFORMED_AFTER_IMAGE_COLOR_RATE		= 0.8f;		// June 32b162e0 @32b16654 ( 3344a514 )
	const int		MODEL_INDEX_COUNT						= 8;		// June 32b151b0 @32b151e7, CX2Item::MAX_MODEL_COUNT_A_ITEM
}
using namespace _CONST_LUCIEL_CH1K_;

// char-luciel CH1k: June 32b1e8e0 ( vslot 0x40 ); the Combination Point follows into the synced int, both transformed skins load, the bounding key dialog opens hidden
/*virtual*/ void CX2GULuciel::Init( bool bUseTeamPos_, int iStartPosIndex_ )
{
	CX2GUUser::Init( bUseTeamPos_, iStartPosIndex_ );

	SetHyperModeCountByBuff( 0 );	// June 32b4dd50

	if ( true == IsMyUnit() )
	{
		const CX2LucielGageData* pLucielGageData = static_cast<const CX2LucielGageData*>( GetGageData() );
		const int iNowCombinationPoint = ( NULL != pLucielGageData ) ? pLucielGageData->GetNowCombinationPoint() : 0;
		if ( m_FrameDataFuture.syncData.iClassSyncInt != iNowCombinationPoint )
			m_FrameDataFuture.syncData.iClassSyncInt = iNowCombinationPoint;
	}

	LoadTransformedSkinAnim( 0 );
	LoadTransformedSkinAnim( 1 );

	if ( true == IsMyUnit() )
	{
		m_pDlgBoundingKey = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_LUCIEL_BOUNDING_KEY.lua", BOUNDING_KEY_DIALOG_MOVE_TIME, 0 );
		if ( m_pDlgBoundingKey != NULL )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgBoundingKey );
			m_pDlgBoundingKey->SetShow( false );
		}
	}
}

// char-luciel CH1k: June 32b0f080 ( vslot 0x3c ); the resting form's MP regeneration, and the Combination Point / gauges at the start of PvP and training
/*virtual*/ void CX2GULuciel::InitComponent()
{
	CX2GUUser::InitComponent();

	if ( true == m_LuaManager.BeginTable( "INIT_COMPONENT" ) )
	{
		LUA_GET_VALUE( m_LuaManager, "DEFAULT_UNSELECTED_UNIT_MP_CHARGE_VALUE_PER_SECOND_AT_PVP", m_afFormMpRegenerationRate[0], 1.f );
		LUA_GET_VALUE( m_LuaManager, "DEFAULT_UNSELECTED_UNIT_MP_CHARGE_VALUE_PER_SECOND", m_afFormMpRegenerationRate[1], 1.f );

		// June also takes the PvP value in its state 0x17 ( XS_PVE_GAME ), which March lacks
		const float fRestFormMpPerSecond = ( CX2Main::XS_PVP_GAME == g_pMain->GetNowStateID() ) ? m_afFormMpRegenerationRate[0] : m_afFormMpRegenerationRate[1];
		if ( NULL != m_pGageData )
			static_cast<CX2LucielGageData*>( m_pGageData )->SetRestFormMpPerSecond( fRestFormMpPerSecond );

		m_LuaManager.EndTable();
	}

	if ( true == IsMyUnit() && NULL != m_pGageData )
	{
		const int iNowStateID = g_pMain->GetNowStateID();
		// June's XS_PVE_GAME ( 0x17 ) branch ( the main gauge full, the other form's empty ) has no March state
		if ( CX2Main::XS_PVP_GAME == iNowStateID || CX2Main::XS_TRAINING_GAME == iNowStateID )
		{
			CX2LucielGageData* pLucielGageData = static_cast<CX2LucielGageData*>( m_pGageData );
			pLucielGageData->SetNowCombinationPoint( 0 );
			pLucielGageData->SetCombinationPointChanged( true );

			m_pGageData->SetNowMp( 0.f, false, 0 );
			m_pGageData->SetNowMp( 0.f, false, 1 );
		}
	}
}

// char-luciel CH1k: June 32ae4f30 ( vslot 0x48 )
/*virtual*/ void CX2GULuciel::InitEffect()
{
	CX2GUUser::InitEffect();

	CreateMajorParticleArray();
	CreateMinorParticleArray();
}

// char-luciel CH1k: June 32ae4080 ( vslot 0x60 ); June's base DamageReact no longer changes the state, the class does
/*virtual*/ void CX2GULuciel::DamageReact( CX2DamageManager::DamageData* pDamageData_ )
{
	CX2GUUser::DamageReact( pDamageData_ );
	CX2GUUser::DamageReactStateChange( pDamageData_, -1, -1 );
}

// char-luciel CH1k: June 32aea610 ( vslot 0x90 )
/*virtual*/ void CX2GULuciel::Win()
{
	CX2GUUser::Win();

	if ( true == GetRidingOn() && NULL != m_ptrRidingPet )
		StateChange( LSI_RIDING_WAIT_HABIT );
	else
		StateChange( LSI_WIN );
}

// char-luciel CH1k: June 32b0eff0 ( vslot 0x94 )
/*virtual*/ void CX2GULuciel::Lose()
{
	CX2GUUser::Lose();

	if ( true == IsMyUnit() && 0.f < GetNowHp() )
	{
		if ( true == GetRidingOn() && NULL != m_ptrRidingPet )
			StateChange( LSI_RIDING_WAIT );
		else
			StateChange( LSI_LOSE );
	}
}

// char-luciel CH1k: June 32aea650 ( vslot 0x98 )
/*virtual*/ void CX2GULuciel::Draw()
{
	CX2GUUser::Draw();

	if ( true == GetRidingOn() && NULL != m_ptrRidingPet )
		StateChange( LSI_RIDING_WAIT_HABIT );
	else
		StateChange( LSI_WIN );
}

// char-luciel CH1k: June 32aeb0c0 ( vslot 0xd8 ); the MP functions answer for the gauge of the form in play ( 1 = Ciel )
/*virtual*/ float CX2GULuciel::GetNowMp() const
{
	if ( NULL == GetGageData() )
		return 0.f;

	return GetGageData()->GetNowMp( true == IsMinorUnitActive() ? 1 : 0 );
}

// char-luciel CH1k: June 32aeb0f0 ( vslot 0xdc )
/*virtual*/ float CX2GULuciel::GetMaxMp() const
{
	if ( NULL == GetGageData() )
		return 0.f;

	return GetGageData()->GetMaxMp( true == IsMinorUnitActive() ? 1 : 0 );
}

// char-luciel CH1k: June 32aeb120 ( vslot 0xe0 )
/*virtual*/ void CX2GULuciel::SetMaxMp( const float fMaxMp_ )
{
	if ( NULL != GetGageData() )
		GetGageData()->SetMaxMp( fMaxMp_, true == IsMinorUnitActive() ? 1 : 0 );
}

// char-luciel CH1k: June 32aeb280 ( vslot 0xe4 ); June reads the gauge pointer with no test here
/*virtual*/ bool CX2GULuciel::FlushMp( const float fFlushMp_, const int iMpGageIndex_ )
{
	return GetGageData()->FlushMp( fFlushMp_, IsMyUnit(), iMpGageIndex_ );
}

// char-luciel CH1k: June 32aeb160 ( vslot 0xf0 ); a tracked change on the form in play
/*virtual*/ void CX2GULuciel::UpNowMp( const float fChangeOfNowMp_ )
{
	if ( NULL != GetGageData() )
		GetGageData()->UpNowMp( fChangeOfNowMp_, true, true == IsMinorUnitActive() ? 1 : 0 );
}

// char-luciel CH1k: June 32aeb240 ( vslot 0xf8 )
/*virtual*/ void CX2GULuciel::SetNowMp( float fNowMp_, bool bTrackMp_ )
{
	if ( NULL != GetGageData() )
		GetGageData()->SetNowMp( fNowMp_, bTrackMp_, true == IsMinorUnitActive() ? 1 : 0 );
}

// char-luciel CH1k: June 32ae50f0; the gauge's SetNowMp for a form index, with no unit-side form lookup
void CX2GULuciel::SetGageNowMp( const float fNowMp_, const bool bTrackMp_, const int iMpGageIndex_ )
{
	if ( NULL != GetGageData() )
		GetGageData()->SetNowMp( fNowMp_, bTrackMp_, iMpGageIndex_ );
}

// char-luciel CH1k: June 32b0e620; the MP of the form in play, or of the other form when bCurrentForm_ is false
void CX2GULuciel::SetGageNowMpOfForm( const float fNowMp_, const bool bTrackMp_, const bool bCurrentForm_ )
{
	int iForm = ( true == IsMinorUnitActive() ? 1 : 0 );
	if ( false == bCurrentForm_ )
		iForm = ( 0 == iForm ? 1 : ( 1 == iForm ? 0 : iForm ) );

	if ( NULL != GetGageData() )
		GetGageData()->SetNowMp( fNowMp_, bTrackMp_, iForm );
}

// char-luciel CH1k: June 32aeb090; the Combination Point is flagged changed first, then stored
void CX2GULuciel::SetNowCombinationPoint( const int iCombinationPoint_ )
{
	CX2LucielGageData* pLucielGageData = static_cast<CX2LucielGageData*>( GetGageData() );
	if ( NULL == pLucielGageData )
		return;

	pLucielGageData->SetCombinationPointChanged( true );
	pLucielGageData->SetNowCombinationPoint( iCombinationPoint_ );
}

// char-luciel CH1k: June 32ae5480 ( vslot 0x29c ); the crowd-control debuffs run for the reduced rate
/*virtual*/ float CX2GULuciel::GetReduceDebuffDurationRate( BUFF_TEMPLET_ID eBuffTempletID_ )
{
	float fRate = 1.f;
	if ( 0.0 < m_fReduceDebuffDurationRate )
	{
		switch ( eBuffTempletID_ )
		{
		case BTI_DEBUFF_STUN:	// June 32d40b80's list: 11 entries
		case BTI_BUFF_TROCKTA_MAGICATTACKB_STUN:
		case BTI_STONE_GOD_ANGER:
		case BTI_DEBUFF_FATE_SPACE:
		case BTI_DEBUFF_RAINCOAT_CAT_STEB1_ATTACK_A:
		case BTI_DEBUFF_RAINCOAT_CAT_STEB2_ATTACK_A:
		case BTI_DEBUFF_RAINCOAT_CAT_STEB3_ATTACK_A:
		case BTI_DEBUFF_ANCIENT_FIRE:
		case BTI_DEBUFF_POWER_STUN:
		case BTI_DEBUFF_ERDA_RAUL:
		case BTI_DEBUFF_SLEEP:
		case BTI_DEBUFF_CONFUSION:	// 32d40ba0
		case BTI_DEBUFF_PANIC:	// 32d40bc0: panic, stone god anger ( listed above )
		case BTI_DEBUFF_PANDEMONIUM_FEAR:	// 32d40be0
		case BTI_DEBUFF_SUPPRESSION:	// 32d40c00
		case BTI_DEBUFF_AGING:	// 32d40c20
		case BTI_DEBUFF_CURSE:	// 32d40c40: curse, curse of abyss
		case BTI_DEBUFF_CURSE_OF_ABYSS:
			fRate = m_fReduceDebuffDurationRate;
			break;

		default:
			break;
		}
	}

	return fRate;
}

// char-luciel CH1k: June 32aea530 ( vslot 0x2b8 ); Soul Collector's stacks multiply the Rain of Chaos hit and are spent
/*virtual*/ void CX2GULuciel::SetDamageExceptionProcess( CX2DamageManager::DamageData* pDamageData_, float& fPowerRate_ )
{
	CX2GUUser::SetDamageExceptionProcess( pDamageData_, fPowerRate_ );

	if ( true == pDamageData_->IsAttackTrigger( CX2DamageManager::DTT_RAIN_OF_CHAOS_GUN ) )
	{
		const float fSoulCollectorCount = static_cast<float>( GetBuffAccumulationCountNowInFactor( BTI_BUFF_SOUL_COLLECTOR ) + 1 );
		pDamageData_->damage.fPhysic *= fSoulCollectorCount;
		pDamageData_->damage.fMagic *= fSoulCollectorCount;
		EraseBuffTempletFromGameUnit( BTI_BUFF_SOUL_COLLECTOR );
	}

	CountDamageEffectHit( pDamageData_ );
}

// char-luciel CH1k: June 32b0e3f0 ( vslot 0x318 ); a kill while transformed shortens the debuffs and the transform skills' cool time
/*virtual*/ void CX2GULuciel::DoProcessWhenKillEnemy( const CX2GameUnit* pDefender_ )
{
	UNREFERENCED_PARAMETER( pDefender_ );

	if ( true == IsTransformed( CX2Unit::DT_BOTH ) && 0.f < m_fMadnessDestroyRate )
	{
		MultiplyAllDebuffTimeFinalizerDuration( m_fMadnessDestroyCoolTime );
		ApplyTransformSkillCoolTime( m_fMadnessDestroyRate, false );
	}
}

// char-luciel CH1k: June 32b0efc0 ( vslot 0x248 ); the effect set remembers the damage effect index of its caster
/*virtual*/ void CX2GULuciel::StampEffectSetIndex( CX2EffectSet::EffectSetInstance* pEffectSetInstance_ )
{
	if ( NULL != pEffectSetInstance_ )
		pEffectSetInstance_->m_iDamageEffectIndex = m_iDamageEffectIndex;
}

// char-luciel CH1k: June 32b0efe0 ( vslot 0x2e0 )
/*virtual*/ int CX2GULuciel::GetDamageEffectIndex() const
{
	return m_iDamageEffectIndex;
}

// char-luciel CH1k: June 32b14cd0 ( vslot 0x324 ); the resource bonus is a share of the maximum Combination Point
/*virtual*/ void CX2GULuciel::ChangeMaxCharacterAddResource( const float fRate_ )
{
	const CX2LucielGageData* pLucielGageData = static_cast<const CX2LucielGageData*>( GetGageData() );
	if ( NULL == pLucielGageData )
		return;

	UpCombinationPoint( static_cast<int>( static_cast<double>( pLucielGageData->GetMaxCombinationPoint() ) * static_cast<double>( fRate_ ) ) );
}

// char-luciel CH1k: June 32b151b0 ( vslot 0x2b0 ); the first weapon with a model at the index for the body in play ( Ciel: the normal anim, Lu: the normal model ), else 0
/*virtual*/ int CX2GULuciel::GetWeaponIndexHavingModel( const int iModelIndex_ ) const
{
	const bool bMinorUnitActive = IsMinorUnitActive();

	int iWeaponIndex = 0;
	for ( std::vector<Weapon*>::const_iterator vit = m_vecpWeapon.begin(); vit != m_vecpWeapon.end(); ++vit, ++iWeaponIndex )
	{
		Weapon* pWeapon = *vit;
		if ( NULL == pWeapon || NULL == pWeapon->m_pUserWeaponEquip )	// stand-in: June tests only the weapon, `NULL == pWeapon && ...`, and dereferences the equip
			continue;

		if ( iModelIndex_ < 0 || MODEL_INDEX_COUNT <= iModelIndex_ )
			continue;

		if ( true == bMinorUnitActive )
		{
			if ( NULL != pWeapon->m_pUserWeaponEquip->GetNormalAnim( iModelIndex_ ) )
				return iWeaponIndex;
		}
		else
		{
			if ( NULL != pWeapon->m_pUserWeaponEquip->GetNormalModel( iModelIndex_ ) )	// char-luciel CH1k2 review: June CX2Eqip +0x5a4 is the normal model ( 3295bdf0 ), June has one more anim array before it
				return iWeaponIndex;
		}
	}

	return 0;
}

// char-luciel CH1k: June 32b16810; one hit of the damage's skill and damage effect is taken back, the entry goes at 0
void CX2GULuciel::EraseDamageEffectIndexEntry( const CX2DamageManager::DamageData* pDamageData_ )
{
	if ( true == m_vecDamageEffectHitCount.empty() || NULL == pDamageData_ )
		return;

	if ( 0 > pDamageData_->m_iDamageEffectIndex || 0 >= pDamageData_->m_iAppliedSkillID )
		return;

	std::vector<SDamageEffectHitCount>::iterator vit = m_vecDamageEffectHitCount.begin();
	for ( ; vit != m_vecDamageEffectHitCount.end(); ++vit )
	{
		if ( vit->m_iDamageEffectIndex == pDamageData_->m_iDamageEffectIndex && vit->m_iSkillID == pDamageData_->m_iAppliedSkillID )
			break;
	}

	if ( vit == m_vecDamageEffectHitCount.end() )
		return;

	--vit->m_iHitCount;
	if ( 0 >= vit->m_iHitCount )
		m_vecDamageEffectHitCount.erase( vit );
}

// char-luciel CH1o: June 32b17660 ( vslot 0x2e4, base 3208ddc0 ); called on the old attacker when a damage data is reset ( SimpleInit )
/*virtual*/ void CX2GULuciel::DoProcessWhenDamageDataReset( const CX2DamageManager::DamageData* pDamageData_ )
{
	if ( NULL != pDamageData_ )
		EraseDamageEffectIndexEntry( pDamageData_ );
}

// char-luciel CH1k: June 32b17680 ( vslot 0x2fc, base 3208ddc0 ); called on the attacker when a damage data is destroyed
/*virtual*/ void CX2GULuciel::DoProcessWhenDamageDataDestroyed( const CX2DamageManager::DamageData* pDamageData_ )
{
	if ( NULL != pDamageData_ )
		EraseDamageEffectIndexEntry( pDamageData_ );
}

// char-luciel CH1k: June 32b16260; the unit is one of the Invasion of Dark Energy targets
bool CX2GULuciel::IsInvasionOfDarkEnergyTarget( const CX2GameUnit* pGameUnit_ )
{
	if ( NULL == pGameUnit_ )
		return false;

	for ( std::vector<STimedGameUnitKey>::const_iterator vit = m_vecInvasionOfDarkEnergyTarget.begin(); vit != m_vecInvasionOfDarkEnergyTarget.end(); ++vit )
	{
		const CX2GameUnit* pTargetUnit = g_pX2Game->GetUnitByUIDAndType( vit->m_GameUnitKey.m_uidGameUnit, vit->m_GameUnitKey.m_eGameUnitType );
		if ( NULL != pTargetUnit && pTargetUnit->GetGameUnitType() == pGameUnit_->GetGameUnitType() && pTargetUnit->GetUnitUID() == pGameUnit_->GetUnitUID() )
			return true;
	}

	return false;
}

// char-luciel CH1k: June 32b166e0; the unit leaves the Invasion of Dark Energy targets
void CX2GULuciel::EraseInvasionOfDarkEnergyTarget( const CX2GameUnit* pGameUnit_ )
{
	if ( NULL == pGameUnit_ )
		return;

	for ( std::vector<STimedGameUnitKey>::iterator vit = m_vecInvasionOfDarkEnergyTarget.begin(); vit != m_vecInvasionOfDarkEnergyTarget.end(); ++vit )
	{
		if ( vit->m_GameUnitKey.m_eGameUnitType == pGameUnit_->GetGameUnitType() && vit->m_GameUnitKey.m_uidGameUnit == pGameUnit_->GetUnitUID() )
		{
			m_vecInvasionOfDarkEnergyTarget.erase( vit );
			return;
		}
	}
}

// char-luciel CH1k: June 32b1e720 ( vslot 0x300 ); Soul Strike's extra damage, the Bounding force down, and the Invasion of Dark Energy mark on the defender
/*virtual*/ void CX2GULuciel::AdjustDamageDataBeforeDamageReact( CX2DamageManager::DamageData* pDamageData_ )
{
	if ( NULL == pDamageData_ )
		return;

	if ( CX2SkillTree::RNSI_A_LCG_SOUL_STRIKE == pDamageData_->m_iAppliedSkillID )
	{
		const float fExtraDamage = pDamageData_->m_ExtraDamage.m_Damage;
		const float fSoulStrikeExtraDamage = fExtraDamage * m_fSoulStrikeSpeedRate;
		if ( fSoulStrikeExtraDamage != fExtraDamage )
			pDamageData_->m_ExtraDamage.m_Damage = fSoulStrikeExtraDamage;
	}

	if ( CX2DamageManager::AT_COMMAND == pDamageData_->attackType )
	{
		const float fBoundingDamageRateEx = m_fBoundingDamageRateEx;
		if ( 0.f <= fBoundingDamageRateEx )
			pDamageData_->fForceDownValue *= fBoundingDamageRateEx;
	}

	if ( CX2DamageManager::AT_SPECIAL != pDamageData_->attackType && false == pDamageData_->m_bNoDamage && CX2DamageManager::RT_NO_DAMAGE != pDamageData_->reActType
		&& 0.f < pDamageData_->damage.fMagic + pDamageData_->damage.fPhysic && null != pDamageData_->optrDefenderGameUnit && true == IsInvasionOfDarkEnergyActive() )
	{
		CX2GameUnit* pDefenderUnit = pDamageData_->optrDefenderGameUnit.GetObservable();
		if ( true == IsInvasionOfDarkEnergyTarget( pDefenderUnit ) )
		{
			pDamageData_->m_fDecreaseForceDown += m_fInvasionOfDarkEnergyDuration;
			pDamageData_->damage.fPhysic *= m_fInvasionOfDarkEnergyTime;
			pDamageData_->damage.fMagic *= m_fInvasionOfDarkEnergyTime;
			EraseInvasionOfDarkEnergyTarget( pDefenderUnit );
		}
	}
}

// char-luciel CH1k: June 32b162e0; the form's transformed skin ( Lu: Motion_Lu_LDB_Evil.x, Ciel: Motion_Ciel_LDN_Evil.x ) loads into its set
void CX2GULuciel::LoadTransformedSkinAnim( const int iForm_ )
{
	STransformedSkin& transformedSkin = m_aTransformedSkin[0 != iForm_ ? 1 : 0];

	if ( NULL != transformedSkin.m_pMotion )
		ReleaseTransformedSkinAnim( iForm_ );

	const WCHAR* wszMotionName = ( 0 == iForm_ ) ? L"Motion_Lu_LDB_Evil.x" : L"Motion_Ciel_LDN_Evil.x";

	transformedSkin.m_pMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( wszMotionName, L"Bip01" );

	transformedSkin.m_pXSkinAnim.reset();
	transformedSkin.m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnimPtr();
	if ( NULL != transformedSkin.m_pXSkinAnim.get() )
	{
		transformedSkin.m_pXSkinAnim->SetAnimXSkinMesh( transformedSkin.m_pMotion );
		transformedSkin.m_pXSkinAnim->AddModelXSkinMesh( transformedSkin.m_pMotion, NULL, NULL, NULL, false, true );	// June adds __FILEW__ / __LINE__ for a log only
		transformedSkin.m_pXSkinAnim->SetModelDetailPercent( g_pData->GetModelDetailPercent() );
	}

	transformedSkin.m_pMotionFuture = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( wszMotionName, L"Bip01" );

	transformedSkin.m_pXSkinAnimFuture = CKTDGXSkinAnim::CreateSkinAnim();
	if ( NULL != transformedSkin.m_pXSkinAnimFuture )
	{
		transformedSkin.m_pXSkinAnimFuture->SetAnimXSkinMesh( transformedSkin.m_pMotionFuture );
		transformedSkin.m_pXSkinAnimFuture->AddModelXSkinMesh( transformedSkin.m_pMotionFuture, NULL, NULL, NULL, false, true );
		transformedSkin.m_pXSkinAnimFuture->SetModelDetailPercent( g_pData->GetModelDetailPercent() );
	}

	CKTDGXSkinAfterImage* pAfterImage = NULL;
	if ( NULL != transformedSkin.m_pXSkinAnim.get() )
		pAfterImage = CKTDGXSkinAfterImage::CreateSkinAfterImage( transformedSkin.m_pXSkinAnim.get(), TRANSFORMED_AFTER_IMAGE_NUM, XL_EFFECT_0 );

	transformedSkin.m_pAfterImage = pAfterImage;
	if ( NULL != m_pAfterImage && NULL != pAfterImage )
	{
		pAfterImage->SetAfterImageColor( D3DXCOLOR( TRANSFORMED_AFTER_IMAGE_COLOR_RATE, TRANSFORMED_AFTER_IMAGE_COLOR_RATE, TRANSFORMED_AFTER_IMAGE_COLOR_RATE, 1.f ) );
		pAfterImage->AddMesh( transformedSkin.m_pMotion );
	}
}

// char-luciel CH1k: June 32aeb450; the state id of a common state for the body in play, and the transformed set while either body is transformed
int CX2GULuciel::GetStateIDByForm( const int iStateID_ )
{
	const bool bMinorUnitActive = IsMinorUnitActive();

	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
	{
		switch ( iStateID_ )
		{
		case LSI_LU_DIE_FRONT:
		case LSI_LU_DIE_BACK:
		case LSI_WIN:
		case LSI_LOSE:
			return iStateID_;

		case LSI_LU_WALK:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WALK : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WALK;
		case LSI_LU_JUMP_READY:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_READY : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_READY;
		case LSI_LU_JUMP_UP:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_UP : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_UP;
		case LSI_LU_JUMP_DOWN:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_DOWN : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_DOWN;
		case LSI_LU_JUMP_LANDING:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_LANDING : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_LANDING;
		case LSI_LU_DASH:
		case LSI_CIEL_DASH:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH;
		case LSI_LU_DASH_END:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_END : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH_END;
		case LSI_LU_DASH_JUMP:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_JUMP : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH_JUMP;
		case LSI_LU_DASH_JUMP_LANDING:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_JUMP_LANDING : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH_JUMP_LANDING;
		case LSI_LU_THROW_ITEM:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_THROW_ITEM : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_THROW_ITEM;
		case LSI_LU_COMBO_Z:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_Z : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_Z;
		case LSI_LU_COMBO_X:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_X : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_X;
		case LSI_LU_JUMP_COMBO_Z:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_Z : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_Z;
		case LSI_LU_JUMP_COMBO_X:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_X : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_X;

		case LSI_LU_HYPER_MODE:
			{
				const bool bTranscended = ( NULL != GetUnit() && 0 != GetUnit()->GetUnitData().m_iUnitClassTran );	// June 32070d40
				if ( false == bMinorUnitActive )
					return ( true == bTranscended ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_HYPER_MODE_TR : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_HYPER_MODE;

				return ( true == bTranscended ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_HYPER_MODE_TR : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_HYPER_MODE;
			}

		default:
			return ( false == bMinorUnitActive ) ? LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT : LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT;
		}
	}

	if ( false == bMinorUnitActive )
		return iStateID_;

	switch ( iStateID_ )
	{
	case LSI_LU_DIE_FRONT:
		return LSI_CIEL_DIE_FRONT;
	case LSI_LU_DIE_BACK:
		return LSI_CIEL_DIE_BACK;
	case LSI_LU_WAIT:
		return LSI_CIEL_WAIT;
	case LSI_LU_WALK:
		return LSI_CIEL_WALK;
	case LSI_LU_JUMP_READY:
		return LSI_CIEL_JUMP_READY;
	case LSI_LU_JUMP_UP:
		return LSI_CIEL_JUMP_UP;
	case LSI_LU_JUMP_DOWN:
		return LSI_CIEL_JUMP_DOWN;
	case LSI_LU_JUMP_LANDING:
		return LSI_CIEL_JUMP_LANDING;
	case LSI_LU_DASH:
		return LSI_CIEL_DASH;
	case LSI_LU_DASH_END:
		return LSI_CIEL_DASH_END;
	case LSI_LU_DASH_JUMP:
		return LSI_CIEL_DASH_JUMP;
	case LSI_LU_DASH_JUMP_LANDING:
		return LSI_CIEL_DASH_JUMP_LANDING;
	case LSI_LU_HYPER_MODE:
		return LSI_CIEL_HYPER_MODE;
	case LSI_LU_DAMAGE_GROGGY:
		return LSI_CIEL_DAMAGE_GROGGY;
	case LSI_LU_DAMAGE_SMALL_FRONT:
		return LSI_CIEL_DAMAGE_SMALL_FRONT;
	case LSI_LU_DAMAGE_SMALL_BACK:
		return LSI_CIEL_DAMAGE_SMALL_BACK;
	case LSI_LU_DAMAGE_BIG_FRONT:
		return LSI_CIEL_DAMAGE_BIG_FRONT;
	case LSI_LU_DAMAGE_BIG_BACK:
		return LSI_CIEL_DAMAGE_BIG_BACK;
	case LSI_LU_DAMAGE_DOWN_FRONT:
		return LSI_CIEL_DAMAGE_DOWN_FRONT;
	case LSI_LU_DAMAGE_DOWN_BACK:
		return LSI_CIEL_DAMAGE_DOWN_BACK;
	case LSI_LU_DAMAGE_STANDUP_FRONT:
		return LSI_CIEL_DAMAGE_STANDUP_FRONT;
	case LSI_LU_DAMAGE_STANDUP_BACK:
		return LSI_CIEL_DAMAGE_STANDUP_BACK;
	case LSI_LU_DAMAGE_AIR_SMALL:
		return LSI_CIEL_DAMAGE_AIR_SMALL;
	case LSI_LU_DAMAGE_AIR_DOWN:
		return LSI_CIEL_DAMAGE_AIR_DOWN;
	case LSI_LU_DAMAGE_AIR_DOWN_INVINCIBLE:
		return LSI_CIEL_DAMAGE_AIR_DOWN_INVINCIBLE;
	case LSI_LU_DAMAGE_AIR_DOWN_LANDING:
		return LSI_CIEL_DAMAGE_AIR_DOWN_LANDING;
	case LSI_LU_DAMAGE_AIR_FALL:
		return LSI_CIEL_DAMAGE_AIR_FALL;
	case LSI_LU_DAMAGE_AIR_UP:
		return LSI_CIEL_DAMAGE_AIR_UP;
	case LSI_LU_DAMAGE_AIR_FLY_FRONT:
		return LSI_CIEL_DAMAGE_AIR_FLY_FRONT;
	case LSI_LU_DAMAGE_AIR_FLY_BACK:
		return LSI_CIEL_DAMAGE_AIR_FLY_BACK;
	case LSI_LU_DAMAGE_REVENGE:
		return LSI_CIEL_DAMAGE_REVENGE;
	case LSI_LU_PEPPER_RUN_READY:
		return LSI_CIEL_PEPPER_RUN_READY;
	case LSI_LU_PEPPER_RUN:
		return LSI_CIEL_PEPPER_RUN;
	case LSI_LU_PEPPER_RUN_END:
		return LSI_CIEL_PEPPER_RUN_END;
	case LSI_LU_PEPPER_RUN_JUMP_UP:
		return LSI_CIEL_PEPPER_RUN_JUMP_UP;
	case LSI_LU_PEPPER_RUN_JUMP_DOWN:
		return LSI_CIEL_PEPPER_RUN_JUMP_DOWN;
	case LSI_LU_THROW_ITEM:
		return LSI_CIEL_THROW_ITEM;
	case LSI_LU_DAMAGE_BOUNDING_DOWN:
		return LSI_CIEL_DAMAGE_BOUNDING_DOWN;
	case LSI_LU_DAMAGE_BOUNDING_LANDCRASH:
		return LSI_CIEL_DAMAGE_BOUNDING_LANDCRASH;
	case LSI_LU_DAMAGE_BOUNDING_UP:
		return LSI_CIEL_DAMAGE_BOUNDING_UP;
	case LSI_LU_AMUSEMENT_ITEM_PHOENIX_RUN:
		return LSI_CIEL_AMUSEMENT_ITEM_PHOENIX_RUN;
	case 124:	// no state of that name in Luciel_Duo.lua
		return 572;	// June 0x23c
	case 125:
		return 573;	// June 0x23d
	case LSI_LU_AMUSEMENT_ITEM_PHOENIX_RUN_END:
		return LSI_CIEL_AMUSEMENT_ITEM_PHOENIX_RUN_END;
	case LSI_LU_DANCE_Z:
		return LSI_CIEL_DANCE_Z;
	case LSI_LU_DANCE_X:
		return LSI_CIEL_DANCE_X;
	case LSI_LU_DANCE_WAIT:
		return LSI_CIEL_DANCE_WAIT;
	default:
		return iStateID_;
	}
}

// char-luciel CH1k2: overrides band 1 part 2 ( June 32b1f0a0 - 32b4db90 )
namespace _CONST_LUCIEL_CH1K2_
{
	const int		KILL_LEAD_FOR_HALF_MP					= 1;		// June 32b373e0 @32b375ac: a lead of exactly one kill halves the other form's MP, a larger lead empties it
	const double	KILL_LEAD_OTHER_FORM_MP_RATE			= 0.5;		// June 32b373e0 @32b37646 ( 3342ebc8 )
	const double	AMUSEMENT_OTHER_FORM_MP_DIVIDER			= 3.0;		// June 32b373e0 @32b376c6 ( 33439b50 )
}
using namespace _CONST_LUCIEL_CH1K2_;

// char-luciel CH1k2: June 32b1f0a0 ( vslot 0x4c ); the base's handles go first, then the class's
/*virtual*/ void CX2GULuciel::DeleteMinorParticle()
{
	CX2GUUser::DeleteMinorParticle();

	DeleteLucielMinorParticle();
}

// char-luciel CH1k2: June 32b1f0b0 ( vslot 0xc4 ); no hyper boost for a transcended unit or one with an awakening effect set
/*virtual*/ void CX2GULuciel::HyperModeBuffEffectStart()
{
	CX2GUUser::HyperModeBuffEffectStart();

	if ( NULL != GetUnit() && 0 != GetUnit()->GetUnitData().m_iUnitClassTran )
		return;

	if ( true == AwakeningEffectSetPresent() )
		return;

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRFoot )
		m_hHyperBoostRFoot = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this, L"HyperBoostRight_Luciel", 0, 0, 0, 0, 0 );

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLFoot )
		m_hHyperBoostLFoot = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this, L"HyperBoostLeft_Luciel", 0, 0, 0, 0, 0 );

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRArm )
		m_hHyperBoostRArm = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this, L"HyperBoostRight_Luciel", 0, 0, 0, 0, 0 );

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLArm )
		m_hHyperBoostLArm = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this, L"HyperBoostLeft_Luciel", 0, 0, 0, 0, 0 );
}

// char-luciel CH1k2: June 32b373e0 ( vslot 0x44 ); a revived unit drops its summons, follows the Combination Point, and the other form's MP and the Combination Point follow the PvP score
/*virtual*/ void CX2GULuciel::ReInit( bool bUseTeamPos_, int iStartPosIndex_ )
{
	CX2GUUser::ReInit( bUseTeamPos_, iStartPosIndex_ );

	KillMySummonedNPC( CX2UnitManager::NUI_SI_SA_LLD_SUMMON_DARKNESS );
	KillMySummonedNPC( CX2UnitManager::NUI_SI_SA_LLD_SUMMON_DARKNESS_PVP );
	KillMySummonedNPC( CX2UnitManager::NUI_SI_SA_LCN_SHADOW_BOLT );

	if ( true == IsMyUnit() )
	{
		const CX2LucielGageData* pLucielGageData = static_cast<const CX2LucielGageData*>( GetGageData() );
		const int iNowCombinationPoint = ( NULL != pLucielGageData ) ? pLucielGageData->GetNowCombinationPoint() : 0;
		if ( m_FrameDataFuture.syncData.iClassSyncInt != iNowCombinationPoint )
			m_FrameDataFuture.syncData.iClassSyncInt = iNowCombinationPoint;
	}

	if ( CX2Room::TN_RED == GetTeam() || CX2Room::TN_BLUE == GetTeam() )
	{
		CX2PVPRoom* pPVPRoom = g_pData->GetPVPRoom();	// June tests the PvP room, not the game type
		if ( NULL != pPVPRoom )
		{
			const int iOtherForm = ( true == IsMinorUnitActive() ) ? 0 : 1;

			if ( CX2PVPRoom::PGT_AMUSEMENT_ITEM == pPVPRoom->GetPVPGameType() )
			{
				const float fOtherFormNowMp = GetGageData()->GetNowMp( iOtherForm );	// June reads the gauge pointer with no test here
				if ( NULL != m_pGageData )
					m_pGageData->SetNowMp( static_cast<float>( fOtherFormNowMp / AMUSEMENT_OTHER_FORM_MP_DIVIDER ), false, iOtherForm );
			}
			else
			{
				int iKillLead = 0;
				if ( CX2Room::TN_RED == GetTeam() )
					iKillLead = g_pX2Game->GetRedTeamKill() - g_pX2Game->GetBlueTeamKill();
				else
					iKillLead = g_pX2Game->GetBlueTeamKill() - g_pX2Game->GetRedTeamKill();

				if ( KILL_LEAD_FOR_HALF_MP < iKillLead )
				{
					if ( NULL != m_pGageData )
						m_pGageData->SetNowMp( 0.f, false, iOtherForm );

					if ( NULL != m_pGageData )
					{
						CX2LucielGageData* pLucielGageData = static_cast<CX2LucielGageData*>( m_pGageData );
						pLucielGageData->SetCombinationPointChanged( true );
						pLucielGageData->SetNowCombinationPoint( 0 );
					}
				}
				else if ( KILL_LEAD_FOR_HALF_MP == iKillLead )
				{
					const float fOtherFormNowMp = GetGageData()->GetNowMp( iOtherForm );	// June reads the gauge pointer with no test here
					SetGageNowMpOfForm( static_cast<float>( fOtherFormNowMp * KILL_LEAD_OTHER_FORM_MP_RATE ), false, false );
					SetNowCombinationPoint( 0 );
				}
			}
		}
	}

	ResetPhantomRushReady();
	ClearAltarOfEvilEffectSet();

	if ( NULL != m_pTransformSkillSlotUI )
	{
		// stand-in: CH2d20 adds the window's dialog release ( 31fd03f0 ) before this delete, as in the destructor
		SAFE_DELETE( m_pTransformSkillSlotUI );
	}

	KillMySummonedNPC( CX2UnitManager::NUI_SA_LDN_GARDEN_OF_HAURES );
}

// char-luciel CH1k2: June 32b37c70 ( vslot 0xd0 ); the local player's new gauge copies the one the gauge manager holds
/*virtual*/ CX2GageData* CX2GULuciel::CreateGageData()
{
	if ( true == IsMyUnit() )
	{
		const CX2LucielGageData* pLucielGageData = static_cast<const CX2LucielGageData*>( CX2GageManager::GetInstance()->GetMyGageData() );

		if ( NULL != pLucielGageData )
			return new CX2LucielGageData( *pLucielGageData );

		return new CX2LucielGageData();
	}

	return new CX2LucielGageData();
}

// char-luciel CH1k2: June 32b4b580; the time is a damage multiplier ( -1 = off ), June tests 0 <= time where CH0's hint read 0 < time
bool CX2GULuciel::IsInvasionOfDarkEnergyActive() const
{
	return 0.f <= m_fInvasionOfDarkEnergyTime;
}

// char-luciel CH1k2: June 32b4db90 ( vslot 0x5c ); the state id is mapped for the body in play before the base queues it
/*virtual*/ bool CX2GULuciel::StateChange( int iState_, bool bDirectChange_ )
{
	return CX2GUUser::StateChange( GetStateIDByForm( iState_ ), bDirectChange_ );
}

// char-luciel CH1k2: June 32b4dbb0 ( vslot 0x434 )
/*virtual*/ bool CX2GULuciel::StateChange( int iState_, int iSubState_, bool bDirectChange_ )
{
	return CX2GUUser::StateChange( GetStateIDByForm( iState_ ), iSubState_, bDirectChange_ );
}

// char-luciel CH1k2: June 32b4d3c0 ( vslot 0x438 ); riding first, then the wait state of the body in play
/*virtual*/ int CX2GULuciel::GetWaitStateID()
{
	if ( true == GetRidingOn() )
		return LSI_RIDING_WAIT;

	return GetStateIDByForm( m_CommonState.m_Wait );
}

// char-luciel CH1k2: June 32b4d3f0 ( vslot 0x43c ); the common state getters below pick the transformed set while either body is transformed, then Ciel's id or Lu's
/*virtual*/ int CX2GULuciel::GetWalkStateID()
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WALK : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WALK;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_WALK : LSI_LU_WALK;
}

// char-luciel CH1k2: June 32b4d430 ( vslot 0x440 )
/*virtual*/ int CX2GULuciel::GetDashStateID()
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DASH : LSI_LU_DASH;
}

// char-luciel CH1k2: June 32b4d470 ( vslot 0x444 )
/*virtual*/ int CX2GULuciel::GetDashEndStateID()
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH_END : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_END;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DASH_END : LSI_LU_DASH_END;
}

// char-luciel CH1k2: June 32b4d4b0 ( vslot 0x448 )
/*virtual*/ int CX2GULuciel::GetDashJumpStateID()
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH_JUMP : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_JUMP;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DASH_JUMP : LSI_LU_DASH_JUMP;
}

// char-luciel CH1k2: June 32b4d4f0 ( vslot 0x44c )
/*virtual*/ int CX2GULuciel::GetDashJumpLandingStateID()
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH_JUMP_LANDING : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_JUMP_LANDING;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DASH_JUMP_LANDING : LSI_LU_DASH_JUMP_LANDING;
}

// char-luciel CH1k2: June 32b4d530 ( vslot 0x450 )
/*virtual*/ int CX2GULuciel::GetJumpReadyStateID()
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_READY : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_READY;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_JUMP_READY : LSI_LU_JUMP_READY;
}

// char-luciel CH1k2: June 32b4d570 ( vslot 0x454 )
/*virtual*/ int CX2GULuciel::GetJumpUpStateID()
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_UP : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_UP;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_JUMP_UP : LSI_LU_JUMP_UP;
}

// char-luciel CH1k2: June 32b4d610 ( vslot 0x45c )
/*virtual*/ int CX2GULuciel::GetJumpLandingStateID()
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_LANDING : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_LANDING;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_JUMP_LANDING : LSI_LU_JUMP_LANDING;
}

// char-luciel CH1k2: June 32b4d650 ( vslot 0x460 ); a transformed unit dies in Lu's state ( const: CH1o2 )
/*virtual*/ int CX2GULuciel::GetDieFrontStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return LSI_LU_DIE_FRONT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DIE_FRONT : LSI_LU_DIE_FRONT;
}

// char-luciel CH1k2: June 32b4d690 ( vslot 0x464 ) ( const: CH1o2 )
/*virtual*/ int CX2GULuciel::GetDieBackStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return LSI_LU_DIE_BACK;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DIE_BACK : LSI_LU_DIE_BACK;
}

// char-luciel CH1k2: June 32b4d6d0 ( vslot 0x468 ); a transformed unit reacts to every damage in its transformed wait state
/*virtual*/ int CX2GULuciel::GetDamageGroggyStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_GROGGY : LSI_LU_DAMAGE_GROGGY;
}

// char-luciel CH1k2: June 32b4d710 ( vslot 0x46c )
/*virtual*/ int CX2GULuciel::GetDamageSmallFrontStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_SMALL_FRONT : LSI_LU_DAMAGE_SMALL_FRONT;
}

// char-luciel CH1k2: June 32b4d750 ( vslot 0x470 )
/*virtual*/ int CX2GULuciel::GetDamageSmallBackStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_SMALL_BACK : LSI_LU_DAMAGE_SMALL_BACK;
}

// char-luciel CH1k2: June 32b4d790 ( vslot 0x474 )
/*virtual*/ int CX2GULuciel::GetDamageBigFrontStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_BIG_FRONT : LSI_LU_DAMAGE_BIG_FRONT;
}

// char-luciel CH1k2: June 32b4d7d0 ( vslot 0x478 )
/*virtual*/ int CX2GULuciel::GetDamageBigBackStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_BIG_BACK : LSI_LU_DAMAGE_BIG_BACK;
}

// char-luciel CH1k2: June 32b4d890 ( vslot 0x47c )
/*virtual*/ int CX2GULuciel::GetDamageStandUpFrontStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_STANDUP_FRONT : LSI_LU_DAMAGE_STANDUP_FRONT;
}

// char-luciel CH1k2: June 32b4d8d0 ( vslot 0x480 )
/*virtual*/ int CX2GULuciel::GetDamageStandUpBackStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_STANDUP_BACK : LSI_LU_DAMAGE_STANDUP_BACK;
}

// char-luciel CH1k2: June 32b4d910 ( vslot 0x484 )
/*virtual*/ int CX2GULuciel::GetDamageAirSmallStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_AIR_SMALL : LSI_LU_DAMAGE_AIR_SMALL;
}

// char-luciel CH1k2: June 32b4d950 ( vslot 0x488 )
/*virtual*/ int CX2GULuciel::GetDamageAirDownStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_AIR_DOWN : LSI_LU_DAMAGE_AIR_DOWN;
}

// char-luciel CH1k2: June 32b4da50 ( vslot 0x498 )
/*virtual*/ int CX2GULuciel::GetDamageAirUpStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_AIR_UP : LSI_LU_DAMAGE_AIR_UP;
}

// char-luciel CH1l: constants and the helper of the band 2 overrides ( part 1 )
namespace _CONST_LUCIEL_CH1L_
{
	const float		COMMON_BUFF_CRASH_TIME						= 0.83f;	// June 32b12fd0: the first event time of the common buff state
	const float		COMMON_BUFF_ACCELERATOR_TIME				= 0.85f;	// June 32b12fd0: the second event time ( the accelerator buff )
	const float		COMMON_BUFF_CRASH_CAMERA_GAP				= 30.f;		// June 32b12fd0
	const float		COMMON_BUFF_CRASH_CAMERA_TIME				= 0.4f;		// June 32b12fd0
	const float		HYPER_MODE_STATE_CHANGE_TIME				= 0.05f;	// June 32b38080: the hyper mode change time of the state FrameMove
	const float		HYPER_MODE_STATE_SHOCK_TIME					= 0.83f;	// June 32b38080
	const int		DAMAGE_IMPACT_DRAW_COUNT					= 2;		// June 32b154f0: the first four damage impact particles
	const int		DAMAGE_IMPACT_RED_DRAW_COUNT				= 1;		// June 32b154f0: the three red ones
	const int		MINOR_PARTICLE_DRAW_COUNT					= 2;		// June 32b37780: the shock wave and tick particles

	// June 32b18220: the equip's body ( CX2Eqip +0x15d4 ); every equip is Lu's until DU3
	bool IsEquipForMinorUnit( const CX2EqipPtr& pEquip_ )
	{
		UNREFERENCED_PARAMETER( pEquip_ );
		return false;	// stand-in: the equip's minor-unit byte ( +0x15d4 ) is DU3's
	}

	// June 32b18220: the equip positions each division group hides ( June's case lists, the hair position included where June handles it )
	const CX2Unit::EQIP_POSITION	aeHideOnePieceAvatar[]				= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_DEFENCE_BODY, CX2Unit::EP_DEFENCE_LEG, CX2Unit::EP_DEFENCE_HAND, CX2Unit::EP_DEFENCE_FOOT };
	const CX2Unit::EQIP_POSITION	aeHideNotRenderingAcBody[]			= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_DEFENCE_BODY, CX2Unit::EP_DEFENCE_LEG, CX2Unit::EP_DEFENCE_HAND, CX2Unit::EP_DEFENCE_FOOT, CX2Unit::EP_AC_BODY };
	const CX2Unit::EQIP_POSITION	aeHideNotRenderingArm[]				= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_DEFENCE_BODY, CX2Unit::EP_DEFENCE_LEG, CX2Unit::EP_DEFENCE_HAND, CX2Unit::EP_DEFENCE_FOOT, CX2Unit::EP_AC_ARM };
	const CX2Unit::EQIP_POSITION	aeHideNotRenderingFace1[]			= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_DEFENCE_BODY, CX2Unit::EP_DEFENCE_LEG, CX2Unit::EP_DEFENCE_HAND, CX2Unit::EP_DEFENCE_FOOT, CX2Unit::EP_AC_FACE1, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideNotRenderingFace1Face2[]		= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_DEFENCE_BODY, CX2Unit::EP_DEFENCE_LEG, CX2Unit::EP_DEFENCE_HAND, CX2Unit::EP_DEFENCE_FOOT, CX2Unit::EP_AC_FACE1, CX2Unit::EP_AC_FACE2, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideNotRenderingFaceFace2Face3[]	= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_DEFENCE_FACE, CX2Unit::EP_DEFENCE_BODY, CX2Unit::EP_DEFENCE_LEG, CX2Unit::EP_DEFENCE_HAND, CX2Unit::EP_DEFENCE_FOOT, CX2Unit::EP_AC_FACE2, CX2Unit::EP_AC_FACE3, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideNotRenderingHairAndAllAccessory[]	= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_DEFENCE_BODY, CX2Unit::EP_DEFENCE_LEG, CX2Unit::EP_DEFENCE_HAND, CX2Unit::EP_DEFENCE_FOOT, CX2Unit::EP_AC_FACE1, CX2Unit::EP_AC_FACE2, CX2Unit::EP_AC_BODY, CX2Unit::EP_AC_LEG, CX2Unit::EP_AC_ARM, CX2Unit::EP_AC_NECKLESS, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideRenderingHairAndFace1[]		= { CX2Unit::EP_DEFENCE_BODY, CX2Unit::EP_DEFENCE_LEG, CX2Unit::EP_DEFENCE_HAND, CX2Unit::EP_DEFENCE_FOOT };
	const CX2Unit::EQIP_POSITION	aeHideRenderingHairNotFace1[]		= { CX2Unit::EP_DEFENCE_BODY, CX2Unit::EP_DEFENCE_LEG, CX2Unit::EP_DEFENCE_HAND, CX2Unit::EP_DEFENCE_FOOT, CX2Unit::EP_AC_FACE1 };
	const CX2Unit::EQIP_POSITION	aeHideOnlyRenderingHairAndFace[]	= { CX2Unit::EP_DEFENCE_BODY, CX2Unit::EP_DEFENCE_LEG, CX2Unit::EP_DEFENCE_HAND, CX2Unit::EP_DEFENCE_FOOT, CX2Unit::EP_AC_FACE1, CX2Unit::EP_AC_FACE2, CX2Unit::EP_AC_FACE3, CX2Unit::EP_AC_BODY, CX2Unit::EP_AC_LEG, CX2Unit::EP_AC_ARM, CX2Unit::EP_AC_NECKLESS, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideOnlyRenderingHairAndFaceFace2Face3[]	= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_DEFENCE_BODY, CX2Unit::EP_DEFENCE_LEG, CX2Unit::EP_DEFENCE_HAND, CX2Unit::EP_DEFENCE_FOOT, CX2Unit::EP_AC_BODY, CX2Unit::EP_AC_LEG, CX2Unit::EP_AC_ARM, CX2Unit::EP_AC_NECKLESS, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideOnlyRenderingWeapon[]			= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_DEFENCE_FACE, CX2Unit::EP_DEFENCE_BODY, CX2Unit::EP_DEFENCE_LEG, CX2Unit::EP_DEFENCE_HAND, CX2Unit::EP_DEFENCE_FOOT, CX2Unit::EP_AC_FACE1, CX2Unit::EP_AC_FACE2, CX2Unit::EP_AC_FACE3, CX2Unit::EP_AC_BODY, CX2Unit::EP_AC_LEG, CX2Unit::EP_AC_ARM, CX2Unit::EP_AC_RING, CX2Unit::EP_AC_NECKLESS, CX2Unit::EP_AC_TEMP3, CX2Unit::EP_AC_TEMP4, CX2Unit::EP_RAVEN_LEFT_ARM };
	const CX2Unit::EQIP_POSITION	aeHideNotRenderingAllFaceAccessory[]	= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_DEFENCE_BODY, CX2Unit::EP_DEFENCE_LEG, CX2Unit::EP_DEFENCE_HAND, CX2Unit::EP_DEFENCE_FOOT, CX2Unit::EP_AC_FACE1, CX2Unit::EP_AC_FACE2, CX2Unit::EP_AC_FACE3, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideNotRenderingFaceAndAllFaceAccessory[]	= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_DEFENCE_FACE, CX2Unit::EP_DEFENCE_BODY, CX2Unit::EP_DEFENCE_LEG, CX2Unit::EP_DEFENCE_HAND, CX2Unit::EP_DEFENCE_FOOT, CX2Unit::EP_AC_FACE1, CX2Unit::EP_AC_FACE2, CX2Unit::EP_AC_FACE3, CX2Unit::EP_AC_TEMP3 };

	// June 32b18220: the face accessory items of the AC_FACE1 / AC_FACE2 worn equips hide these positions ( the hair one keeps the hair mesh unless said )
	const CX2Unit::EQIP_POSITION	aeHideFace1NotRenderingHairFace2Face3[]	= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_AC_FACE2, CX2Unit::EP_AC_FACE3, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideFace1NotRenderingHairFace2[]		= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_AC_FACE2, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideNotRenderingHairHidden[]			= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_AC_FACE1, CX2Unit::EP_AC_FACE2, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideNotRenderingHairAlone[]			= { CX2Unit::EP_DEFENCE_HAIR };
	const CX2Unit::EQIP_POSITION	aeHideFace1AndEarring[]					= { CX2Unit::EP_AC_FACE1, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideNotRenderingHairFaceFace2Face3[]	= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_DEFENCE_FACE, CX2Unit::EP_AC_FACE2, CX2Unit::EP_AC_FACE3, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideNotRenderingHairAndAllFaceAccessory[]	= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_AC_FACE2, CX2Unit::EP_AC_FACE3, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideFace2AndEarring[]					= { CX2Unit::EP_AC_FACE2, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideNotRenderingHairFaceFace1Face3[]	= { CX2Unit::EP_DEFENCE_HAIR, CX2Unit::EP_DEFENCE_FACE, CX2Unit::EP_AC_FACE1, CX2Unit::EP_AC_FACE3, CX2Unit::EP_AC_TEMP3 };
	const CX2Unit::EQIP_POSITION	aeHideFace1Face2AndEarring[]			= { CX2Unit::EP_AC_FACE1, CX2Unit::EP_AC_FACE2, CX2Unit::EP_AC_TEMP3 };

	// June 32b18220: one division group's pass over the view equips; the shown equips of one body at the given positions are hidden.
	// A hair equip also becomes the hair mesh when none is kept ( Lu's own hair only ) and marks the body only when bMarkHairHidden_.
	// pbHidden_ ( NULL for the face accessory groups ) is set when an equip was hidden.
	void HideFormEquips( std::vector<CX2EqipPtr>& vecViewEquip_, CKTDXDeviceXSkinMesh*& pHairXSkinMesh_, const bool bMinorUnitEquip_,
		const CX2Unit::EQIP_POSITION* paeHidePosition_, const int iHidePositionCount_, const bool bKeepHairMesh_, bool* pbHidden_, const bool bMarkHairHidden_ )
	{
		BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, vecViewEquip_ )
		{
			if ( false == pViewEquip->GetShowObject() || bMinorUnitEquip_ != IsEquipForMinorUnit( pViewEquip ) )
				continue;

			bool bHide = false;
			for ( int i = 0; i < iHidePositionCount_; ++i )
			{
				if ( paeHidePosition_[i] == pViewEquip->GetEqipPos() )
					bHide = true;
			}

			if ( false == bHide )
				continue;

			pViewEquip->SetShowObject( false );

			if ( CX2Unit::EP_DEFENCE_HAIR == pViewEquip->GetEqipPos() )
			{
				if ( true == bMarkHairHidden_ && NULL != pbHidden_ )
					*pbHidden_ = true;

				if ( true == bKeepHairMesh_ && NULL == pHairXSkinMesh_ && false == IsEquipForMinorUnit( pViewEquip ) )
					pHairXSkinMesh_ = pViewEquip->GetNormalModel( 0 );
			}
			else if ( NULL != pbHidden_ )
			{
				*pbHidden_ = true;
			}
		}
	}

	// June 32282440: the unit stands on something ( a user reads its Now condition, an NPC IsOnSomething ), any other type false
	bool IsGameUnitOnSomething( CX2GameUnit* pGameUnit_ )
	{
		switch ( pGameUnit_->GetGameUnitType() )
		{
		case CX2GameUnit::GUT_USER:
			return static_cast<CX2GUUser*>( pGameUnit_ )->IsOnSomethingNow();

		case CX2GameUnit::GUT_NPC:
			return static_cast<CX2GUNPC*>( pGameUnit_ )->IsOnSomething();

		default:
			return false;
		}
	}
}
using namespace _CONST_LUCIEL_CH1L_;

// char-luciel CH1l: June 32ae9bf0 ( vslot 0x574 ); no emotion while either body is transformed
/*virtual*/ bool CX2GULuciel::CanPlayEmotion( const CX2Unit::EMOTION_TYPE eEmotionType_ )
{
	UNREFERENCED_PARAMETER( eEmotionType_ );

	return false == IsTransformed( CX2Unit::DT_BOTH );
}

// char-luciel CH1l: June 32ae9c00 ( vslot 0x57c ); the transform state of a transform skill, 0 for any other
/*virtual*/ int CX2GULuciel::GetTransformedSkillStateID( const CX2SkillTree::SKILL_ID eSkillID_ )
{
	switch ( g_pData->GetSkillTree()->GetUpgradeBaseSkillID( eSkillID_ ) )
	{
	case CX2SkillTree::RNSI_SA_LDB_DEATH_TOUCH:
		return LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DEATH_TOUCH;

	case CX2SkillTree::RNSI_SA_LDB_MORTAL_FEAR:
		return LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_MORTAL_FEAR;

	case CX2SkillTree::RNSI_SA_LDB_DARK_PLASMA:
		return LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DARK_PLASMA;

	case CX2SkillTree::RNSI_SA_LDN_SHADOW_FURY:
		return LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHADOW_FURY;

	case CX2SkillTree::RNSI_SA_LDN_SHREWD_CUTTER:
		return LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHREWD_CUTTER;

	case CX2SkillTree::RNSI_SA_LDN_SILENT_RAGE:
		return LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SILENT_RAGE;

	default:
		return 0;
	}
}

// char-luciel CH1l: June 32ae9de0 ( vslot 0x580 ); transformed: the state skill is a transform skill; not transformed: June tests the same id against six transform state ids
/*virtual*/ bool CX2GULuciel::IsTransformSkillState()
{
	const int iBaseSkillID = g_pData->GetSkillTree()->GetUpgradeBaseSkillID( GetNowStateSkillID() );

	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
	{
		return CX2SkillTree::RNSI_SA_LDB_ALTAR_OF_EVIL == iBaseSkillID || CX2SkillTree::RNSI_SA_LDN_TRANSCENDENCE == iBaseSkillID;
	}

	switch ( iBaseSkillID )
	{
	case LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT:
	case LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_END:
	case LSI_SA_LDB_ALTAR_OF_EVIL_END:
	case LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT:
	case LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_END:
	case LSI_SA_LDN_TRANSCENDENCE_END:
		return true;

	default:
		return false;
	}
}

// char-luciel CH1l: June 32ae9f20 ( vslot 0x640 ); showing goes through the active body's view list
/*virtual*/ void CX2GULuciel::SetShowViewListAndUpdate( const bool bShow_ )
{
	if ( true == bShow_ )
		UpdateShowViewListByActiveBody();
	else
		SetShowViewList( bShow_ );
}

// char-luciel CH1l: June 32aea690 ( vslot 0x54c )
/*virtual*/ void CX2GULuciel::InitEnchantWeaponEffectUnitSpecific()
{
	BOOST_TEST_FOREACH( Weapon*, pWeapon, m_vecpWeapon )
	{
		ASSERT( NULL != pWeapon );
		pWeapon->InitEnchantParticleForBow();
	}
}

// char-luciel CH1l: June 32aeb2c0 ( vslot 0x650 ); each form's stored MP goes to its gauge
/*virtual*/ void CX2GULuciel::SetNowMpFromStoredValue()
{
	SetGageNowMp( m_FrameDataNow.syncData.fNowMP, false, 0 );
	SetGageNowMp( m_FrameDataNow.syncData.fMinorUnitNowMP, false, 1 );
}

// char-luciel CH1o: June 32b0fa20 ( vslot 0x648 ); the first state start after the active body changed ( DUO_TYPE, CP_AMOUNT )
/*virtual*/ void CX2GULuciel::DoProcessWhenActiveBodyChangeApplied( const int iDuoType_, const int iCpAmount_ )
{
	UNREFERENCED_PARAMETER( iDuoType_ );

	if ( true == m_bActiveBodyChanged )
	{
		m_fSwitchingCoolTime = 1.f;
		m_iLastUsedSkillID = 0;
		m_bActiveBodyChanged = false;

		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( GetFutureStateSkillID() );	// June passes level 1 as a second argument

		if ( ++m_iDamageEffectIndex < 0 )
			m_iDamageEffectIndex = 0;

		if ( NULL == pSkillTemplet )
			m_iLastUsedSkillID = iCpAmount_;	// char-luciel CH1o: June stores the state's CP_AMOUNT here ( JUNE_BUGS 723 )
	}
}

// char-luciel CH1l: June 32b12fd0 ( vslot 0x5f0 )
/*virtual*/ void CX2GULuciel::COMMON_BUFF_FrameMove()
{
	if ( true == GetXSkinAnimPtr()->EventTimerOneshot( COMMON_BUFF_CRASH_TIME ) )
	{
		UpDownCrashCamera( COMMON_BUFF_CRASH_CAMERA_GAP, COMMON_BUFF_CRASH_CAMERA_TIME );
		g_pKTDXApp->GetDGManager()->ClearScreen();
	}
	else if ( true == GetXSkinAnimPtr()->EventTimerOneshot( COMMON_BUFF_ACCELERATOR_TIME ) )
	{
		ProcessAcceleraterBuff( GetNowStateSkillID(), IsHyperState() );
	}

	CommonFrameMove();
}

// char-luciel CH1l: June 32b13b60 ( vslot 0x570 ); transformed: June crosses the bodies ( form 0 with Ciel in play reads Lu's own anim ), base 31fd48a0 otherwise
/*virtual*/ CKTDGXSkinAnimPtr CX2GULuciel::GetSkinAnimByForm( const int iForm_ )
{
	CKTDGXSkinAnimPtr pXSkinAnim;

	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
	{
		if ( 0 == iForm_ )
			pXSkinAnim = ( true == IsMinorUnitActive() ) ? m_pXSkinAnim : m_aTransformedSkin[0].m_pXSkinAnim;
		else if ( 1 == iForm_ )
			pXSkinAnim = ( true == IsMinorUnitActive() ) ? m_aTransformedSkin[1].m_pXSkinAnim : m_pMinorUnitXSkinAnim;
	}
	else
	{
		pXSkinAnim = ( 1 == iForm_ ) ? m_pMinorUnitXSkinAnim : m_pXSkinAnim;
	}

	return pXSkinAnim;
}

// char-luciel CH1l: June 32b143e0 ( vslot 0x62c ); Lu's hand and arm equips and the one-piece avatar's attached model
/*virtual*/ void CX2GULuciel::SetHandEquipShow( const bool bShow_ )
{
	if ( true == IsMinorUnitActive() )
		return;

	if ( true == m_bOtherEquipNotRendered )
	{
		CX2EqipPtr pOnePieceEquip = GetEquipPtrFromEquipViewListByEquipPosition( CX2Unit::EP_ONEPIECE_FASHION );
		if ( NULL != pOnePieceEquip )
		{
			CKTDXDeviceXSkinMesh* pAttachedModel = pOnePieceEquip->GetNormalModel( 2 );
			if ( NULL == pAttachedModel )
				pAttachedModel = pOnePieceEquip->GetNormalModel( 1 );

			if ( NULL != pAttachedModel )
				pAttachedModel->SetNoRenderable( false == bShow_ );
		}
	}

	const bool bShowHandEquip = ( true == m_bOtherEquipNotRendered ) ? false : bShow_;
	BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_ViewEqipList )
	{
		const bool bEquipForMinorUnit = false;	// stand-in: the equip's minor-unit byte ( +0x15d4 ) is DU3's
		if ( CX2Unit::EP_DEFENCE_HAND == pViewEquip->GetEqipPos() && false == bEquipForMinorUnit )
			pViewEquip->SetShowObject( bShowHandEquip );
	}

	CX2EqipPtr pArmEquip = GetEquipPtrFromEquipViewListByEquipPosition( CX2Unit::EP_AC_ARM );
	const bool bArmEquipForMinorUnit = false;	// stand-in: the equip's minor-unit byte ( +0x15d4 ) is DU3's
	if ( NULL != pArmEquip && false == bArmEquipForMinorUnit )
		pArmEquip->SetShowObject( bShow_ );
}

// char-luciel CH1l: June 32b14e10 ( vslot 0x628 )
/*virtual*/ bool CX2GULuciel::CanRidingState()
{
	bool bCanRiding = false;

	if ( GetNowStateID() == GetWaitStateID() || GetNowStateID() == GetWalkStateID() || GetNowStateID() == GetDashStateID() || GetNowStateID() == GetDashEndStateID()
		|| LSI_RIDING_WAIT == GetNowStateID() || LSI_RIDING_WALK == GetNowStateID() || LSI_RIDING_WAIT_HABIT == GetNowStateID()
		|| LSI_RIDING_DASH == GetNowStateID() || LSI_RIDING_DASH_END == GetNowStateID() )
	{
		bCanRiding = true;
	}

	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return false;

	return bCanRiding;
}

// char-luciel CH1l: June 32b15340 ( vslot 0x578 ); the move states of both transformed sets
/*virtual*/ bool CX2GULuciel::IsClassCommonMoveState()
{
	const int iNowStateID = GetNowStateID();

	return ( LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT <= iNowStateID && iNowStateID <= LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_JUMP_LANDING )
		|| ( LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT <= iNowStateID && iNowStateID <= LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH_JUMP_LANDING );
}

// char-luciel CH1l: June 32b154f0 ( vslot 0x540 )
/*virtual*/ void CX2GULuciel::CreateMajorParticleArray()
{
	CX2GUUser::CreateMajorParticleArray();

	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT, L"DamageImpact", DAMAGE_IMPACT_DRAW_COUNT );
	if ( NULL != pSeq )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_CORE, L"DamageImpactCore", DAMAGE_IMPACT_DRAW_COUNT );
	if ( NULL != pSeq )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_SLASH, L"DamageImpactSlash", DAMAGE_IMPACT_DRAW_COUNT );
	if ( NULL != pSeq )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_RED, L"DamageImpactRed", DAMAGE_IMPACT_DRAW_COUNT );
	if ( NULL != pSeq )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_RING_RED, L"DamageImpactRingRed", DAMAGE_IMPACT_RED_DRAW_COUNT );
	if ( NULL != pSeq )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_CORE_RED, L"DamageImpactCoreRed", DAMAGE_IMPACT_RED_DRAW_COUNT );
	if ( NULL != pSeq )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_SLASH_RED, L"DamageImpactSlashRed", DAMAGE_IMPACT_RED_DRAW_COUNT );
	if ( NULL != pSeq )
		pSeq->SetTriggerWait( true );
}

// char-luciel CH1l: June 32b15900 ( vslot 0x504 ); the slot icon of a skill templet, Summon Darkness and Twilight Judgement follow their buffs
/*virtual*/ void CX2GULuciel::GetSkillIconTexture( CKTDGUISlot* pSlot_, const CX2SkillTree::SkillTemplet* pSkillTemplet_ )
{
	if ( NULL == pSlot_ || NULL == pSkillTemplet_ )
		return;

	wstring wstrIconTexName = pSkillTemplet_->m_wstrIconName;
	wstring wstrIconTexKey = pSkillTemplet_->m_wstrIconPieceName;

	switch ( g_pData->GetSkillTree()->GetUpgradeBaseSkillID( pSkillTemplet_->m_eID ) )
	{
	case CX2SkillTree::RNSI_SA_LLD_SUMMON_DARKNESS:
		if ( true == HaveThisBuff( BTI_BUFF_SUMMON_DARKNESS ) )
		{
			wstrIconTexName = L"DLG_SKILL_LUCIEL_Active_03.tga";
			wstrIconTexKey = L"SI_SA_LLD_SUMMON_DARKNESS_END";
		}
		else
		{
			wstrIconTexName = L"DLG_SKILL_LUCIEL_Active_02.tga";
			wstrIconTexKey = L"SI_SA_LLD_SUMMON_DARKNESS";
		}
		break;

	case CX2SkillTree::RNSI_SA_LCN_TWILIGHT_JUDGEMENT:
		wstrIconTexName = L"DLG_SKILL_LUCIEL_Active_03.tga";
		if ( true == HaveThisBuff( BTI_BUFF_TWILIGHT_JUDGEMENT ) )
		{
			if ( 2 <= GetBuffAccumulationCountNowInFactor( BTI_BUFF_TWILIGHT_JUDGEMENT ) )
				wstrIconTexKey = L"SI_SA_LCN_TWILIGHT_JUDGEMENT_02";
			else
				wstrIconTexKey = L"SI_SA_LCN_TWILIGHT_JUDGEMENT_01";
		}
		else
		{
			wstrIconTexKey = L"SI_SA_LCN_TWILIGHT_JUDGEMENT";
		}
		break;

	default:
		break;
	}

	pSlot_->SetItemTex( wstrIconTexName.c_str(), wstrIconTexKey.c_str() );
}

// char-luciel CH1l: June 32b16220 ( vslot 0x56c ); Phantom Rush shows its chain icon while the chain is ready
/*virtual*/ void CX2GULuciel::SkillIconOverride( const int iSkillID_, wstring& wstrIconTexName_, wstring& wstrIconTexKey_ )
{
	switch ( iSkillID_ )
	{
	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH:
	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH_UPGRADE:
		if ( true == m_bPhantomRushReady )
		{
			wstrIconTexName_ = L"DLG_SKILL_LUCIEL_Active_04.tga";
			wstrIconTexKey_ = L"RNSI_A_LDB_PHANTOM_RUSH_CHAIN";
		}
		break;

	default:
		break;
	}
}

// char-luciel CH1l: June 32b1c960 ( vslot 0x64c ); each form's gauge MP goes to the Future sync data when it differs ( June tests no null gauge )
/*virtual*/ void CX2GULuciel::SyncFutureMpFromGage()
{
	const float fFirstFormMp = GetGageData()->GetNowMp( 0 );
	if ( fFirstFormMp != m_FrameDataFuture.syncData.fNowMP )
		m_FrameDataFuture.syncData.fNowMP = fFirstFormMp;

	const float fSecondFormMp = GetGageData()->GetNowMp( 1 );
	if ( fSecondFormMp != m_FrameDataFuture.syncData.fMinorUnitNowMP )
		m_FrameDataFuture.syncData.fMinorUnitNowMP = fSecondFormMp;
}

// char-luciel CH1l: June 32b34680 ( vslot 0x568 ); the bounding target is kept while it is a living enemy on its feet or in a bounding state, then the strike state starts
/*virtual*/ bool CX2GULuciel::CheckBoundingTarget()
{
	if ( m_BoundingTarget.m_uidGameUnit < 0 || true == m_bBoundingTargetFound )
		return false;

	bool bValidTarget = true;

	CX2GameUnit* pBoundingTarget = g_pX2Game->GetUnitByUIDAndType( m_BoundingTarget.m_uidGameUnit, m_BoundingTarget.m_eGameUnitType );
	if ( false == g_pX2Game->IsValidUnit( pBoundingTarget ) || pBoundingTarget->GetTeam() == GetTeam() || false == ( 0.f < pBoundingTarget->GetNowHp() ) )
	{
		bValidTarget = false;
	}
	else
	{
		const bool bTargetOnSomething = IsGameUnitOnSomething( pBoundingTarget );
		const bool bTargetBounding = ( true == pBoundingTarget->IsBoundingLandCrash() || true == pBoundingTarget->IsBoundingUp() );

		if ( false == bTargetBounding && true == bTargetOnSomething )
		{
			bValidTarget = false;
		}
		else if ( false == bTargetOnSomething && pBoundingTarget->GetLastHitAttackSerial() < 0 )
		{
			bValidTarget = false;
		}
		else if ( true == GetRidingOn() )
		{
			bValidTarget = false;
		}
	}

	if ( 1 < m_iBoundingStrikeCount )
		bValidTarget = false;

	if ( false == bValidTarget )
	{
		m_iBoundingTargetUnitIndex = -1;
		m_bBoundingTargetFound = false;
		m_BoundingTarget.m_uidGameUnit = -1;
		m_BoundingTarget.m_eGameUnitType = CX2GameUnit::GUT_USER;
		return false;
	}

	// June starts Lu's strike state while Ciel is in play and Ciel's while Lu is
	StateChange( true == IsMinorUnitActive() ? LSI_BOUNDING_TARGET_LU_STRIKER : LSI_BOUNDING_TARGET_CIEL_STRIKER, false );
	m_bBoundingTargetFound = true;
	return true;
}

// char-luciel CH1l: June 32b37730 ( vslot 0x4e0 ); the base first, then the class's hit readers in June's order
/*virtual*/ void CX2GULuciel::AttackResultByType( CX2DamageManager::DamageData& pDamageData_ )
{
	CX2GUUser::AttackResultByType( pDamageData_ );

	CreateChaserOfDarknessDamageEffect( &pDamageData_ );
	ProcessPhantomRushHit( &pDamageData_ );
	PlayPunisherEffectSet( &pDamageData_ );
	RegisterDesperadoTarget( &pDamageData_ );
	AccumulateShadowFuryDamage( &pDamageData_ );
	PlayBluffEffectSet( &pDamageData_ );
	CheckBoundingStrikeHit( &pDamageData_ );
	AddCombinationPointByDamageEffectHit( &pDamageData_ );
}

// char-luciel CH1l: June 32b37780 ( vslot 0x548 )
/*virtual*/ void CX2GULuciel::CreateMinorParticleArray()
{
	CX2GUUser::CreateMinorParticleArray();

	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

	pSeq = SetLucielMinorParticleByEnum( LUCIEL_MINOR_PII_REVENGE_WAIT, L"RevengeWait", -1 );
	if ( NULL != pSeq )
		pSeq->SetTriggerWait( true );

	pSeq = SetLucielMinorParticleByEnum( LUCIEL_MINOR_PII_GROUND_SHOCK_WAVE_BLUE, L"GroundShockWaveBlue", MINOR_PARTICLE_DRAW_COUNT );
	if ( NULL != pSeq )
		pSeq->SetTriggerWait( true );

	pSeq = SetLucielMinorParticleByEnum( LUCIEL_MINOR_PII_IMPACT_TICK_BLUE, L"ImpactTickBlue", MINOR_PARTICLE_DRAW_COUNT );
	if ( NULL != pSeq )
		pSeq->SetTriggerWait( true );

	pSeq = SetLucielMinorParticleByEnum( LUCIEL_MINOR_PII_KU_AANG, L"STR_KuAAang", -1 );
	if ( NULL != pSeq )
		pSeq->SetTriggerWait( true );

	pSeq = SetLucielMinorParticleByEnum( LUCIEL_MINOR_PII_GROUND_SHOCK_WAVE_RED, L"GroundShockWaveRed", MINOR_PARTICLE_DRAW_COUNT );
	if ( NULL != pSeq )
		pSeq->SetTriggerWait( true );

	pSeq = SetLucielMinorParticleByEnum( LUCIEL_MINOR_PII_IMPACT_TICK_RED, L"ImpactTickRed", MINOR_PARTICLE_DRAW_COUNT );
	if ( NULL != pSeq )
		pSeq->SetTriggerWait( true );

	pSeq = SetLucielMinorParticleByEnum( LUCIEL_MINOR_PII_MAGNUM_BLADE_FIRE_WAVE, L"MagnumBladeFireWave", -1 );
	if ( NULL != pSeq )
		pSeq->SetTriggerWait( true );
}

// char-luciel CH1l: June 32b38080 ( vslot 0x600 )
/*virtual*/ void CX2GULuciel::HyperModeFrameMove()
{
	CommonHyperModeFrameMove( HYPER_MODE_STATE_CHANGE_TIME, HYPER_MODE_STATE_SHOCK_TIME, false );
}

// char-luciel CH1l: June 32b4d5b0 ( vslot 0x458 ); riding first, then the common getter pattern ( see CH1k2 )
/*virtual*/ int CX2GULuciel::GetJumpDownStateID()
{
	if ( true == GetRidingOn() )
		return LSI_RIDING_JUMP_DOWN;

	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_DOWN : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_DOWN;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_JUMP_DOWN : LSI_LU_JUMP_DOWN;
}

// char-luciel CH1l: June 32b4d810 ( vslot 0x114 ); DU2's name, the damage getters answer the transformed WAIT state while transformed
/*virtual*/ int CX2GULuciel::GetDamageDownLandFront() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_DOWN_FRONT : LSI_LU_DAMAGE_DOWN_FRONT;
}

// char-luciel CH1l: June 32b4d850 ( vslot 0x118 )
/*virtual*/ int CX2GULuciel::GetDamageDownLandBack() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_DOWN_BACK : LSI_LU_DAMAGE_DOWN_BACK;
}

// char-luciel CH1l: June 32b4d990 ( vslot 0x48c )
/*virtual*/ int CX2GULuciel::GetDamageAirDownInvincibleStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_AIR_DOWN_INVINCIBLE : LSI_LU_DAMAGE_AIR_DOWN_INVINCIBLE;
}

// char-luciel CH1l: June 32b4d9d0 ( vslot 0x490 )
/*virtual*/ int CX2GULuciel::GetDamageAirDownLanding() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_AIR_DOWN_LANDING : LSI_LU_DAMAGE_AIR_DOWN_LANDING;
}

// char-luciel CH1l: June 32b4da10 ( vslot 0x494 )
/*virtual*/ int CX2GULuciel::GetDamageAirFallStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_AIR_FALL : LSI_LU_DAMAGE_AIR_FALL;
}

// char-luciel CH1l: June 32b4da90 ( vslot 0x49c )
/*virtual*/ int CX2GULuciel::GetDamageAirFlyFrontStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_AIR_FLY_FRONT : LSI_LU_DAMAGE_AIR_FLY_FRONT;
}

// char-luciel CH1l: June 32b4dad0 ( vslot 0x4a0 )
/*virtual*/ int CX2GULuciel::GetDamageAirFlyBackStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_AIR_FLY_BACK : LSI_LU_DAMAGE_AIR_FLY_BACK;
}

// char-luciel CH1l: June 32b4db10 ( vslot 0x4a4 )
/*virtual*/ int CX2GULuciel::GetDamageRevengeStateID() const
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DAMAGE_REVENGE : LSI_LU_DAMAGE_REVENGE;
}

// char-luciel CH1l: June 32b4db50 ( vslot 0x5c4 )
/*virtual*/ int CX2GULuciel::GetDanceWaitStateID()
{
	if ( true == IsTransformed( CX2Unit::DT_BOTH ) )
		return ( true == IsMinorUnitActive() ) ? LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT : LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT;

	return ( true == IsMinorUnitActive() ) ? LSI_CIEL_DANCE_WAIT : LSI_LU_DANCE_WAIT;
}

// char-luciel CH1l: June 32b4dbe0 ( vslot 0x51c ); June reads the state record index, which is the state id here
/*virtual*/ bool CX2GULuciel::IsHyperModeState() const
{
	return LSI_LU_HYPER_MODE == GetNowStateID() || LSI_CIEL_HYPER_MODE == GetNowStateID();
}

// char-luciel CH1l: June 32b18220 ( vslot 0x630 ); the base's one-piece avatar rules run once per body ( June's list of one-piece items comes from 32b176a0, at most one per body ),
// an equip is only touched by its own body's item. June drops the base's Arme / Ain and near-portal exceptions.
/*virtual*/ void CX2GULuciel::UpdateNotRenderingOtherEquipList()
{
	SetHideNoRenderable( false );

	bool abOnePieceHidden[2] = { false, false };		// June local_b0: a one-piece group hid an equip of the body
	bool abRenderHairAndFace1[2] = { false, false };	// June local_80
	bool abRenderHair[2] = { false, false };			// June local_88
	m_pHairXSkinMesh = NULL;

	CX2ItemManager* pItemManager = g_pData->GetItemManager();

	std::vector<SFormEquipItem> vecFormEquipItem;
	GetFormEquipItemList( CX2Unit::EP_ONEPIECE_FASHION, vecFormEquipItem );

	m_mapScaleHeadByForm[0] = false;
	m_mapScaleHeadByForm[1] = false;

	for ( UINT i = 0; i < vecFormEquipItem.size(); ++i )
	{
		const int iOnePieceItemID = vecFormEquipItem[i].m_iItemID;
		const bool bMinorUnitEquip = vecFormEquipItem[i].m_bMinorUnitEquip;
		const int iForm = ( true == bMinorUnitEquip ) ? 1 : 0;

		// June also flags a draw-face-accessory item ( DITT_ITEM_ID_DRAW_FACE_ACC_WITH_NO_ONE_PEACE_AVATAR_SET1 ) per body in four of these groups; nothing reads it, so it is not kept
		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PIECE_AVATAR, iOnePieceItemID ) )
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideOnePieceAvatar, _countof( aeHideOnePieceAvatar ), true, &abOnePieceHidden[iForm], true );

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PIECE_AVATAR_NOT_RENDERING_AC_BODY, iOnePieceItemID ) )
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideNotRenderingAcBody, _countof( aeHideNotRenderingAcBody ), true, &abOnePieceHidden[iForm], true );

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PIECE_AVATAR_NOT_RENDERING_ARM, iOnePieceItemID ) )
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideNotRenderingArm, _countof( aeHideNotRenderingArm ), true, &abOnePieceHidden[iForm], true );

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PIECE_AVATAR_NOT_RENDERING_FACE1, iOnePieceItemID ) )
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideNotRenderingFace1, _countof( aeHideNotRenderingFace1 ), true, &abOnePieceHidden[iForm], true );

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PIECE_AVATAR_NOT_RENDERING_FACE1_FACE2, iOnePieceItemID ) )
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideNotRenderingFace1Face2, _countof( aeHideNotRenderingFace1Face2 ), true, &abOnePieceHidden[iForm], true );

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PIECE_AVATAR_NOT_RENDERING_FACE_FACE2_FACE3, iOnePieceItemID ) )
		{
			SetHideNoRenderable( true );
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideNotRenderingFaceFace2Face3, _countof( aeHideNotRenderingFaceFace2Face3 ), true, &abOnePieceHidden[iForm], true );
		}

		// the hair equip of this group does not mark the body ( June 32b18220 )
		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PIECE_AVATAR_NOT_RENDERING_HAIR_AND_ALL_ACCESSORY, iOnePieceItemID ) )
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideNotRenderingHairAndAllAccessory, _countof( aeHideNotRenderingHairAndAllAccessory ), true, &abOnePieceHidden[iForm], false );

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PIECE_AVATAR_RENDERING_HAIR_AND_FACE1, iOnePieceItemID ) )
		{
			abRenderHairAndFace1[iForm] = true;
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideRenderingHairAndFace1, _countof( aeHideRenderingHairAndFace1 ), false, &abOnePieceHidden[iForm], true );
		}

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PIECE_AVATAR_RENDERING_HAIR_NOT_FACE1, iOnePieceItemID ) )
		{
			abRenderHair[iForm] = true;
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideRenderingHairNotFace1, _countof( aeHideRenderingHairNotFace1 ), false, &abOnePieceHidden[iForm], true );
		}

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PIECE_AVATAR_ONLY_RENDERING_HAIR_AND_FACE, iOnePieceItemID ) )
		{
			abRenderHair[iForm] = true;
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideOnlyRenderingHairAndFace, _countof( aeHideOnlyRenderingHairAndFace ), false, &abOnePieceHidden[iForm], true );
		}

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PIECE_AVATAR_RENDERING_HAIR, iOnePieceItemID ) )
		{
			abRenderHair[iForm] = true;

			BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_ViewEqipList )
			{
				if ( false == pViewEquip->GetShowObject() || bMinorUnitEquip != IsEquipForMinorUnit( pViewEquip ) )
					continue;

				// the equip's own item decides: a not-rendering-hair item also hides the second face accessory and the earring
				const bool bNotRenderingHairItem = pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_HAIR, pViewEquip->GetItemID() );
				bool bHide = false;
				switch ( pViewEquip->GetEqipPos() )
				{
				case CX2Unit::EP_DEFENCE_BODY:
				case CX2Unit::EP_DEFENCE_LEG:
				case CX2Unit::EP_DEFENCE_HAND:
				case CX2Unit::EP_DEFENCE_FOOT:
					bHide = true;
					break;

				case CX2Unit::EP_AC_FACE2:
				case CX2Unit::EP_AC_TEMP3:
					bHide = bNotRenderingHairItem;
					break;

				default:
					break;
				}

				if ( true == bHide )
				{
					pViewEquip->SetShowObject( false );
					abOnePieceHidden[iForm] = true;
				}
			}
		}

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PIECE_AVATAR_ONLY_RENDERING_HAIR_AND_FACE_FACE2_FACE3, iOnePieceItemID ) )
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideOnlyRenderingHairAndFaceFace2Face3, _countof( aeHideOnlyRenderingHairAndFaceFace2Face3 ), true, &abOnePieceHidden[iForm], false );

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PIECE_AVATAR_ONLY_RENDERING_WEAPON, iOnePieceItemID ) )
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideOnlyRenderingWeapon, _countof( aeHideOnlyRenderingWeapon ), true, &abOnePieceHidden[iForm], false );

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PIECE_AVATAR_NOT_RENDERING_ALL_FACE_ACCESSARY, iOnePieceItemID ) )
		{
			SetHideNoRenderable( true );
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideNotRenderingAllFaceAccessory, _countof( aeHideNotRenderingAllFaceAccessory ), true, &abOnePieceHidden[iForm], true );
		}

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_ONE_PICE_AVATAR_NOT_RENDERING_FACE_AND_ALL_FACE_ACCESSARY, iOnePieceItemID ) )
		{
			SetHideNoRenderable( true );
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideNotRenderingFaceAndAllFaceAccessory, _countof( aeHideNotRenderingFaceAndAllFaceAccessory ), true, &abOnePieceHidden[iForm], true );
		}

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_SD_ONE_PEACE_AVATAR, iOnePieceItemID ) )
			SetSDOnePieceAvatar( iForm, true );
	}

	// no one-piece avatar equip: a not-rendering-Nasod-hand item hides the Raven left arm equips
	CX2EqipPtr pOnePieceEquip = GetEquipPtrFromEquipViewListByEquipPosition( CX2Unit::EP_ONEPIECE_FASHION );
	if ( NULL == pOnePieceEquip )
	{
		BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_ViewEqipList )
		{
			if ( true == pViewEquip->GetShowObject() && true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_NASOD_HAND, pViewEquip->GetItemID() ) )
			{
				BOOST_TEST_FOREACH( CX2EqipPtr, pArmEquip, m_ViewEqipList )
				{
					if ( CX2Unit::EP_RAVEN_LEFT_ARM == pArmEquip->GetEqipPos() )
						pArmEquip->SetShowObject( false );
				}
			}
		}
	}

	// the worn arm accessory of each body: a not-rendering-hand item hides the same body's hand equip
	vecFormEquipItem.clear();
	GetFormEquipItemList( CX2Unit::EP_AC_ARM, vecFormEquipItem );
	for ( UINT i = 0; i < vecFormEquipItem.size(); ++i )
	{
		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_HAND, vecFormEquipItem[i].m_iItemID ) )
		{
			BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_ViewEqipList )
			{
				if ( true == pViewEquip->GetShowObject() && vecFormEquipItem[i].m_bMinorUnitEquip == IsEquipForMinorUnit( pViewEquip ) && CX2Unit::EP_DEFENCE_HAND == pViewEquip->GetEqipPos() )
					pViewEquip->SetShowObject( false );
			}
		}
	}

	// the worn first face accessory of each body
	vecFormEquipItem.clear();
	GetFormEquipItemList( CX2Unit::EP_AC_FACE1, vecFormEquipItem );
	for ( UINT i = 0; i < vecFormEquipItem.size(); ++i )
	{
		const int iFaceItemID = vecFormEquipItem[i].m_iItemID;
		const bool bMinorUnitEquip = vecFormEquipItem[i].m_bMinorUnitEquip;
		const bool bOnePieceHidden = abOnePieceHidden[( true == bMinorUnitEquip ) ? 1 : 0];

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_HAIR_FACE2_FACE3, iFaceItemID ) )
		{
			if ( NULL == pOnePieceEquip )
				HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideFace1NotRenderingHairFace2Face3, _countof( aeHideFace1NotRenderingHairFace2Face3 ), true, NULL, false );
		}
		else if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_HAIR_FACE2, iFaceItemID ) )
		{
			HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideFace1NotRenderingHairFace2, _countof( aeHideFace1NotRenderingHairFace2 ), true, NULL, false );
		}
		else if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_HAIR, iFaceItemID ) )
		{
			if ( true == bOnePieceHidden )
				HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideNotRenderingHairHidden, _countof( aeHideNotRenderingHairHidden ), false, NULL, false );
			else
				HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideNotRenderingHairAlone, _countof( aeHideNotRenderingHairAlone ), true, NULL, false );
		}
		else if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_HAIR_FACE_FACE2_FACE3, iFaceItemID ) )
		{
			if ( true == bOnePieceHidden )
			{
				HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideFace1AndEarring, _countof( aeHideFace1AndEarring ), false, NULL, false );
			}
			else
			{
				SetHideNoRenderable( true );
				HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideNotRenderingHairFaceFace2Face3, _countof( aeHideNotRenderingHairFaceFace2Face3 ), true, NULL, false );
			}
		}
		else if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_HAIR_AND_ALL_FACE_ACCESSARY, iFaceItemID ) )
		{
			if ( true == bOnePieceHidden )
			{
				HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideFace1AndEarring, _countof( aeHideFace1AndEarring ), false, NULL, false );
			}
			else
			{
				SetHideNoRenderable( true );
				HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideNotRenderingHairAndAllFaceAccessory, _countof( aeHideNotRenderingHairAndAllFaceAccessory ), true, NULL, false );
			}
		}
	}

	// the worn second face accessory of each body
	vecFormEquipItem.clear();
	GetFormEquipItemList( CX2Unit::EP_AC_FACE2, vecFormEquipItem );
	for ( UINT i = 0; i < vecFormEquipItem.size(); ++i )
	{
		const int iFaceItemID = vecFormEquipItem[i].m_iItemID;
		const bool bMinorUnitEquip = vecFormEquipItem[i].m_bMinorUnitEquip;
		const bool bOnePieceHidden = abOnePieceHidden[( true == bMinorUnitEquip ) ? 1 : 0];

		if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_HAIR_FACE_FACE1_FACE3, iFaceItemID ) )
		{
			if ( true == bOnePieceHidden )
			{
				HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideFace2AndEarring, _countof( aeHideFace2AndEarring ), false, NULL, false );
			}
			else
			{
				SetHideNoRenderable( true );
				HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideNotRenderingHairFaceFace1Face3, _countof( aeHideNotRenderingHairFaceFace1Face3 ), true, NULL, false );
			}
		}
		else if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_HAIR, iFaceItemID ) )
		{
			if ( true == bOnePieceHidden )
				HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideFace1Face2AndEarring, _countof( aeHideFace1Face2AndEarring ), false, NULL, false );
			else
				HideFormEquips( m_ViewEqipList, m_pHairXSkinMesh, bMinorUnitEquip, aeHideNotRenderingHairAlone, _countof( aeHideNotRenderingHairAlone ), true, NULL, false );
		}
	}

	// stand-in: June's SetpHairSkinMesh ( 3322a5d0 ) also refs the mesh; March's only stores it
	if ( NULL != m_pAfterImage )
		m_pAfterImage->SetpHairSkinMesh( m_pHairXSkinMesh );

	m_bOtherEquipNotRendered = abOnePieceHidden[0];

	// Lu in play with no emotion and not riding: Lu's hand equip hides, so does the one-piece avatar's attached model when it hid equips; an arm accessory that is a not-rendering-hand item hides itself
	if ( false == IsMinorUnitActive() && CX2Unit::ET_NONE == m_ePlayedEmotion )
	{
		BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_ViewEqipList )
		{
			if ( true == pViewEquip->GetShowObject() && CX2Unit::EP_DEFENCE_HAND == pViewEquip->GetEqipPos() && false == IsEquipForMinorUnit( pViewEquip ) && false == GetRidingOn() )
				pViewEquip->SetShowObject( false );
		}

		if ( true == m_bOtherEquipNotRendered && false == GetRidingOn() )
		{
			CX2EqipPtr pAttachedOnePieceEquip = GetEquipPtrFromEquipViewListByEquipPosition( CX2Unit::EP_ONEPIECE_FASHION );
			if ( NULL != pAttachedOnePieceEquip )
			{
				SetHideNoRenderable( true );

				CKTDXDeviceXSkinMesh* pAttachedModel = pAttachedOnePieceEquip->GetNormalModel( 2 );
				if ( NULL == pAttachedModel )
					pAttachedModel = pAttachedOnePieceEquip->GetNormalModel( 1 );

				if ( NULL != pAttachedModel )
					pAttachedModel->SetNoRenderable( true );
			}
		}

		CX2EqipPtr pArmEquip = GetEquipPtrFromEquipViewListByEquipPosition( CX2Unit::EP_AC_ARM );
		if ( NULL != pArmEquip && true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_HAND, pArmEquip->GetItemID() ) )
			pArmEquip->SetShowObject( false );
	}

	// the face accessory items of each body ( June runs one pass per body; the two passes read only the equip list )
	bool abFace1HidesHair[2] = { false, false };		// June local_7c
	bool abFace1HidesFace[2] = { false, false };		// June local_a8
	bool abFace2HidesHair[2] = { false, false };		// June local_a4
	bool abFace2HidesFace[2] = { false, false };		// June uStack_84
	BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_ViewEqipList )
	{
		const int iEquipForm = ( true == IsEquipForMinorUnit( pViewEquip ) ) ? 1 : 0;
		if ( CX2Unit::EP_AC_FACE1 == pViewEquip->GetEqipPos() )
		{
			if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_HAIR, pViewEquip->GetItemID() )
				|| true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_HAIR_FACE2_FACE3, pViewEquip->GetItemID() )
				|| true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_DRAW_FACE_ACC_WITH_NO_ONE_PEACE_AVATAR_SET1, pViewEquip->GetItemID() ) )
			{
				abFace1HidesHair[iEquipForm] = true;
			}
			else if ( true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_HAIR_FACE_FACE2_FACE3, pViewEquip->GetItemID() ) )
			{
				abFace1HidesHair[iEquipForm] = true;
				abFace1HidesFace[iEquipForm] = true;
			}
		}
		else if ( CX2Unit::EP_AC_FACE2 == pViewEquip->GetEqipPos()
			&& true == pItemManager->IsDivisionItem( CX2ItemManager::DITT_ITEM_ID_NOT_RENDERING_HAIR_FACE_FACE1_FACE3, pViewEquip->GetItemID() ) )
		{
			abFace2HidesHair[iEquipForm] = true;
			abFace2HidesFace[iEquipForm] = true;
		}
	}

	// each body shows or hides its hair and face equips by those flags; Lu's pass runs while Lu is in play or both bodies show, Ciel's the same for Ciel
	for ( int iForm = 0; iForm < 2; ++iForm )
	{
		const bool bMinorUnitEquip = ( 1 == iForm );
		if ( bMinorUnitEquip != IsMinorUnitActive() && false == m_bDuoEnabled )
			continue;

		if ( true == abOnePieceHidden[iForm] && false == abRenderHairAndFace1[iForm] )
		{
			BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_ViewEqipList )
			{
				if ( bMinorUnitEquip != IsEquipForMinorUnit( pViewEquip ) )
					continue;

				if ( CX2Unit::EP_DEFENCE_HAIR == pViewEquip->GetEqipPos()
					|| ( CX2Unit::EP_AC_FACE1 == pViewEquip->GetEqipPos() && true == abFace1HidesHair[iForm] )
					|| ( CX2Unit::EP_AC_FACE2 == pViewEquip->GetEqipPos() && true == abFace2HidesHair[iForm] ) )
					pViewEquip->SetShowObject( false );
			}
		}
		else if ( true == abFace1HidesHair[iForm] )
		{
			BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_ViewEqipList )
			{
				if ( bMinorUnitEquip != IsEquipForMinorUnit( pViewEquip ) )
					continue;

				switch ( pViewEquip->GetEqipPos() )
				{
				case CX2Unit::EP_DEFENCE_HAIR:
				case CX2Unit::EP_AC_FACE2:
					pViewEquip->SetShowObject( false );
					break;

				case CX2Unit::EP_DEFENCE_FACE:
					pViewEquip->SetShowObject( false == abFace1HidesFace[iForm] );
					break;

				case CX2Unit::EP_AC_FACE1:
					pViewEquip->SetShowObject( true );
					break;

				default:
					break;
				}
			}
		}
		else if ( true == abFace2HidesHair[iForm] )
		{
			BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_ViewEqipList )
			{
				if ( bMinorUnitEquip != IsEquipForMinorUnit( pViewEquip ) )
					continue;

				switch ( pViewEquip->GetEqipPos() )
				{
				case CX2Unit::EP_DEFENCE_HAIR:
				case CX2Unit::EP_AC_FACE1:
					pViewEquip->SetShowObject( false );
					break;

				case CX2Unit::EP_DEFENCE_FACE:
					pViewEquip->SetShowObject( false == abFace2HidesFace[iForm] );
					break;

				case CX2Unit::EP_AC_FACE2:
					pViewEquip->SetShowObject( true );
					break;

				default:
					break;
				}
			}
		}

		if ( true == abRenderHair[iForm] )
		{
			BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_ViewEqipList )
			{
				if ( bMinorUnitEquip == IsEquipForMinorUnit( pViewEquip ) && CX2Unit::EP_DEFENCE_HAIR == pViewEquip->GetEqipPos() )
					pViewEquip->SetShowObject( true );
			}
		}
	}

	BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_ViewEqipList )
	{
		if ( CX2Unit::EP_AC_TEMP4 == pViewEquip->GetEqipPos() )	// June EP_AC_RING2
			pViewEquip->SetShowObject( false );
	}
}

// char-luciel CH1l2: constants and helpers of the band 2 overrides ( part 2 )
namespace _CONST_LUCIEL_CH1L2_
{
	const int		STR_ID_SKILL_REPLACED_BY_UPGRADE		= 35758;	// June's string 0x8bae; March's StringID enum has no entry for it
	const int		EVENT_FLAG_DUO_TO_MINOR_UNIT			= 8;		// June +0x7d8d: DUO_TYPE 1 began while Lu was active ( CX2GUUser's DU1b constant is file-local there )
	const int		EVENT_FLAG_DUO_TO_FIRST_UNIT			= 9;		// June +0x7d8e: DUO_TYPE 0 began while Ciel was active
	const float		BODY_SWITCH_EFFECT_EVENT_TIME			= 0.01f;	// June 32b397a0 ( 33437810 ): the animation time of the body switch effect sets
	const int		UAI_CANCEL_SUMMON_DARKNESS				= 42;		// June 32b1c880 / 32b37c40 compare the user action with 0x2a; Enum.lua has no name for it
	const float		DANCING_BLADE_LOOP_MP_COST				= 30.f;		// June 32b14640 ( 33439c68 ): the MP cost one Dancing Blade loop takes off
	const float		ALTAR_OFFSET_FAR						= 140.f;	// June 32ae9d00 ( 334bac38 )
	const float		ALTAR_OFFSET_MID						= 120.f;	// June 32ae9d00 ( 33439d10 )
	const float		ALTAR_OFFSET_NEAR						= 80.f;		// June 32ae9d00 ( 33438090 )

	// June 32ae9d00 ( sole caller 32b35600, CH1l3 ): the eight places around the Altar of Evil, a ring in the x-y plane
	void GetEffectOffsetByIndex( D3DXVECTOR3* pvOffset_, const int iIndex_ )
	{
		*pvOffset_ = D3DXVECTOR3( 0.f, 0.f, 0.f );

		switch ( iIndex_ )
		{
		case 0:
			pvOffset_->y = ALTAR_OFFSET_FAR;
			break;

		case 1:
			pvOffset_->x = ALTAR_OFFSET_NEAR;
			pvOffset_->y = ALTAR_OFFSET_NEAR;
			break;

		case 2:
			pvOffset_->x = ALTAR_OFFSET_MID;
			break;

		case 3:
			pvOffset_->x = ALTAR_OFFSET_NEAR;
			pvOffset_->y = -ALTAR_OFFSET_NEAR;
			break;

		case 4:
			pvOffset_->y = -ALTAR_OFFSET_FAR;
			break;

		case 5:
			pvOffset_->x = -ALTAR_OFFSET_NEAR;
			pvOffset_->y = -ALTAR_OFFSET_NEAR;
			break;

		case 6:
			pvOffset_->x = -ALTAR_OFFSET_MID;
			break;

		case 7:
			pvOffset_->x = -ALTAR_OFFSET_NEAR;
			pvOffset_->y = ALTAR_OFFSET_NEAR;
			break;

		default:
			break;
		}
	}
}
using namespace _CONST_LUCIEL_CH1L2_;

// char-luciel CH1l2: June 3201f850 ( CX2GUUser's, carried per class as Eve and Ara do ): the state's skill level, power rate ( times the additional power rate + 1 ) and skill id
void CX2GULuciel::SetSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, UserUnitStateData& stateData_ )
{
	if ( NULL == pSkillTemplet_ || NULL == GetUnit() )
		return;

	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet_->m_eID, false, 0, true ) );

	stateData_.m_SPLevel	= static_cast<char>( iSkillTempletLevel );
	stateData_.m_fPowerRate	= pSkillTemplet_->GetSkillPowerRateValue( iSkillTempletLevel );
	stateData_.m_eSkillID	= pSkillTemplet_->m_eID;
	stateData_.m_fPowerRate	= ( GetSkillAdditionalPowerRate( static_cast<int>( pSkillTemplet_->m_eID ) ) + 1.f ) * stateData_.m_fPowerRate;
}

// char-luciel CH1l2: June 32028190 ( vslot 0x7a8 ); the same for the state with this id, none when that state is not registered
void CX2GULuciel::SetSkillLevelStateDataByID( const CX2SkillTree::SkillTemplet* pSkillTemplet_, const int iStateID_ )
{
	if ( NULL == pSkillTemplet_ || NULL == GetUnit() )
		return;

	UserUnitStateDataMap::iterator itState = GetStateDataIterator( iStateID_ );
	if ( itState != m_StateList.end() )
		SetSkillLevelStateData( pSkillTemplet_, itState->second );
}

// char-luciel CH1l2: June 32ae40c0 ( vslot 0x7b8 ); 47 blocks in June's order, each keyed by its skill id, the upgrade swapped templet, a state that is not registered is skipped
/*virtual*/ void CX2GULuciel::SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ )
{
	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LUCIEL_STOMP == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LUCIEL_STOMP, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LD_STOMP_LANDING );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LUCIEL_HEAVY_HIT == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LUCIEL_HEAVY_HIT, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LD_M_HEAVY_HIT );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LUCIEL_HAUNTING_BEAT == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LUCIEL_HAUNTING_BEAT, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LD_M_HAUNTING_BEAT );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_A_LD_CHASER_OF_DARKNESS == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LD_CHASER_OF_DARKNESS, true );
		SetSkillLevelStateDataByID( pSkillTemplet, 268 );	// June's id; no state is registered there
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LLC_DARK_CRESCENT == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LLC_DARK_CRESCENT, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LLC_DARK_CRESCENT );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LLC_RUINOUS_IMPACT == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LLC_RUINOUS_IMPACT, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LLC_RUINOUS_IMPACT );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LCG_DEMONIC_BREATH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LCG_DEMONIC_BREATH, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_DEMONIC_BREATH_CHARGING );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_DEMONIC_BREATH_FIRE );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LCG_FANTASIE_IMPROMPTU_1ST_MOVEMENT == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LCG_FANTASIE_IMPROMPTU_1ST_MOVEMENT, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_FANTASIE_IMPROMPTU_CANTABILE_WAIT );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_FANTASIE_IMPROMPTU_CANTABILE_FRONT_MOVE );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_FANTASIE_IMPROMPTU_CANTABILE_BACK_MOVE );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_READY );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_WAIT );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_FRONT_MOVE );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_BACK_MOVE );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_END );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LCG_DEATH_CRISIS_OF_MARBAS == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LCG_DEATH_CRISIS_OF_MARBAS, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_WAIT );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_SMALL_FIRE );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_BIG_CHARGE );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_BIG_FIRE );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_END );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LCG_ESCHATON == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LCG_ESCHATON, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_ESCHATON );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LCN_FANTASIE_IMPROMPTU_2ND_MOVEMENT == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LCN_FANTASIE_IMPROMPTU_2ND_MOVEMENT, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCG_FANTASIE_IMPROMPTU_2ND_MOVEMENT );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LLC_BLASTING_FEAR == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LLC_BLASTING_FEAR, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LLC_M_BLASTING_FEAR );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LLC_BACKSTAB == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LLC_BACKSTAB, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LLC_M_BACKSTAB );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_A_LCG_SOUL_SHOCK == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LCG_SOUL_SHOCK, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LCG_M_SOUL_SHOCK );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LCG_SOUL_STRIKE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LCG_SOUL_STRIKE, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LCG_F_SOUL_STRIKE_LOOP );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LCG_F_SOUL_STRIKE_END );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LCG_M_SOUL_STRIKE_READY );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LCG_M_SOUL_STRIKE_LOOP );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LCG_M_SOUL_STRIKE_END );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_LLD_DANCING_BLADE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_LLD_DANCING_BLADE, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LLD_DANCING_BLADE_LOOP );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LLD_PHANTOM_BLADE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LLD_PHANTOM_BLADE, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LLD_PHANTOM_BLADE_ATTACK );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LLD_PHANTOM_BLADE_FAIL );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LLD_DANCING_BLADE_LOOP );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LCN_SWIFT_SMASH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LCN_SWIFT_SMASH, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCN_M_SWIFT_SMASH );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LCN_SHADOW_BOLT == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LCN_SHADOW_BOLT, true );
		for ( int iStateID = LSI_SA_LCN_SHADOW_BOLT_CONTROL; iStateID <= LSI_SA_LCN_SHADOW_BOLT_END; ++iStateID )	// June lists 405-414 one by one; 406-413 have no state name
			SetSkillLevelStateDataByID( pSkillTemplet, iStateID );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LCN_TWILIGHT_JUDGEMENT == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LCN_TWILIGHT_JUDGEMENT, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCN_F_TWILIGHT_JUDGEMENT_REATTACK );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCN_M_TWILIGHT_JUDGEMENT_REATTACK );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LLD_FEAR_ABSORPTION == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LLD_FEAR_ABSORPTION, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LLD_M_FEAR_ABSORPTION );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LLD_CHAINS_OF_BONDAGE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LLD_CHAINS_OF_BONDAGE, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LLD_F_CHAINS_OF_BONDAGE_DASH_ATTACK );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LLD_M_CHAINS_OF_BONDAGE );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LLD_M_CHAINS_OF_BONDAGE_DASH_ATTACK );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LCN_SUMMON_FAMILIAR == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LCN_SUMMON_FAMILIAR, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LCN_M_SUMMON_FAMILIAR );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LCG_SHADOW == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LCG_SHADOW, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LCN_M_SHADOW );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LDB_LAND_CRASHER == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LDB_LAND_CRASHER, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_LAND_CRASHER_LANDING );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_LAND_CRASHER_CHAIN );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LDB_NIGHTMARE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LDB_NIGHTMARE, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_NIGHTMARE_TARGET );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LDB_F_PHANTOM_RUSH_TARGET );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LDB_M_PHANTOM_RUSH );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LDB_M_PHANTOM_RUSH_TARGET );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LDB_ABYSS_STRIKE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LDB_ABYSS_STRIKE, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LDB_M_ABYSS_STRIKE );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LDN_DESPERADO == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LDN_DESPERADO, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_DESPERADO_ATTACK_READY );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_DESPERADO_ATTACK );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_DESPERADO_ATTACK_END );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_DESPERADO_ATTACK_UP );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_DESPERADO_ATTACK_DOWN );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_BUFF_PHANTOM_CIRCLE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_BUFF_PHANTOM_CIRCLE, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_M_COMMON_BUFF_PHANTOM_CIRCLE );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LDN_RAPID_IMPACT == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LDN_RAPID_IMPACT, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LDN_F_RAPID_IMPACT_MOVEATTACK );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LDN_M_RAPID_IMPACT );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LDN_M_RAPID_IMPACT_MOVEATTACK );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LDN_PUNISHER == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LDN_PUNISHER, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LDN_M_PUNISHER );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LDB_ALTAR_OF_EVIL == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LDB_ALTAR_OF_EVIL, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_START );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WALK );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_READY );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_UP );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_DOWN );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_LANDING );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_END );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_JUMP );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_JUMP_LANDING );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_END );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_END );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LDB_DEATH_TOUCH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LDB_DEATH_TOUCH, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DEATH_TOUCH );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LDB_MORTAL_FEAR == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LDB_MORTAL_FEAR, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_MORTAL_FEAR );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LDB_DARK_PLASMA == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LDB_DARK_PLASMA, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DARK_PLASMA );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LDN_TRANSCENDENCE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LDN_TRANSCENDENCE, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_START );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WALK );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_READY );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_UP );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_DOWN );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_LANDING );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH_END );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH_JUMP );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH_JUMP_LANDING );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_END );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_END );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LDN_SHADOW_FURY == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LDN_SHADOW_FURY, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHADOW_FURY );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LDN_SHREWD_CUTTER == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LDN_SHREWD_CUTTER, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHREWD_CUTTER );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHREWD_CUTTER_ATTACK_A );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHREWD_CUTTER_ATTACK_B );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHREWD_CUTTER_END );
		SetSkillLevelStateDataByID( pSkillTemplet, 499 );	// June's id; no state is registered there
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LDN_SILENT_RAGE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LDN_SILENT_RAGE, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SILENT_RAGE );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LLD_BLUFF == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LLD_BLUFF, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LLD_BLUFF_CIEL );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LCN_SOUL_SPREAD == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LCN_SOUL_SPREAD, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LCN_SOUL_SPREAD_LU_LOOP );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LCN_SOUL_SPREAD_LU_END );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LCN_SOUL_SPREAD_CIEL_READY );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LCN_SOUL_SPREAD_CIEL_LOOP );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LCN_SOUL_SPREAD_CIEL_END );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_LDN_RAPID_FURY == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LDN_RAPID_FURY, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_A_LDN_RAPID_FURY_CIEL );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LDN_ABYSS_SNIPING == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LDN_ABYSS_SNIPING, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_ABYSS_SNIPING_UP );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_ABYSS_SNIPING_ATTACK );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_ABYSS_SNIPING_DOWN );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_ABYSS_SNIPING_END );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LDN_DEMONIC_RUSH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LDN_DEMONIC_RUSH, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_DEMONIC_RUSH_LAST );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_DEMONIC_RUSH_LANDING );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LDN_SINISTER_MIND == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LDN_SINISTER_MIND, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LDN_SINISTER_MIND_FLY );
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_LCN_MAGDONIC_CANNON == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_LCN_MAGDONIC_CANNON, true );
		SetSkillLevelStateDataByID( pSkillTemplet, LSI_SA_LCN_MAGDONIC_CANNON_FLY );
	}
}

// char-luciel CH1l2: June 32ae4f60 ( vslot 0x7c0 ); June's body is a jump to the base
/*virtual*/ void CX2GULuciel::CommonStateStartFuture()
{
	CX2GUUser::CommonStateStartFuture();
}

// char-luciel CH1l2: June 32ae4f70 ( vslot 0x7d8 ); June's body is a jump to the base
/*virtual*/ void CX2GULuciel::CommonStateEndFuture()
{
	CX2GUUser::CommonStateEndFuture();
}

// char-luciel CH1l2: June 32ae4f80 ( vslot 0x7dc ); June's body is a jump to the base
/*virtual*/ void CX2GULuciel::CommonStateEnd()
{
	CX2GUUser::CommonStateEnd();
}

// char-luciel CH1l2: June 32ae4f90 ( vslot 0x7e8 )
/*virtual*/ RENDER_HINT CX2GULuciel::CommonRender_Prepare()
{
	return CX2GUUser::CommonRender_Prepare();
}

// char-luciel CH1l2: June 32ae4fb0 ( vslot 0x7f4 ); the lookup, then the use; a skill of use condition 3 / 5 gives the chosen values back
/*virtual*/ bool CX2GULuciel::SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition_ )
{
	const float fSavedReservedMp = m_fReservedSkillMp;
	const int iSavedSlotIndex = m_iChosenSkillSlotIndex;
	const int iSavedSkillID = m_iChosenSkillID;

	bool bUsed = false;
	if ( true == CanUseSpecialAttack( eActiveSkillUseCondition_ ) && 0 != m_iChosenSkillID )
	{
		bUsed = true;
		SpecialAttackUse( m_iChosenSkillID, true );
		Send_SKILL_USE_REQ( static_cast<CX2SkillTree::SKILL_ID>( m_iChosenSkillID ) );
	}

	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( static_cast<CX2SkillTree::SKILL_ID>( m_iChosenSkillID ), true );
	if ( NULL != pSkillTemplet && ( CX2SkillTree::ASUT_ANYTIME == pSkillTemplet->m_eActiveSkillUseCondtion || CX2SkillTree::ASUT_ANYTIME_EXCEPT_AIR == pSkillTemplet->m_eActiveSkillUseCondtion ) )
	{
		m_iChosenSkillID = iSavedSkillID;
		m_fReservedSkillMp = fSavedReservedMp;
		m_iChosenSkillSlotIndex = iSavedSlotIndex;
	}

	if ( true == bUsed )
	{
		m_iNowSpecialAttack = m_iChosenSkillSlotIndex + 1;
		return true;
	}

	m_iChosenSkillID = iSavedSkillID;
	return false;
}

// char-luciel CH1l2: June 32ae5400 ( vslot 0x668 ); the base first, then the dash, riding dash or walk state by the body in play ( the state change maps Lu's id to Ciel's by form )
/*virtual*/ void CX2GULuciel::StateChangeDashIfPossible()
{
	CX2GUUser::StateChangeDashIfPossible();

	if ( false == GetEnableDash( EDS_END ) )
	{
		StateChange( LSI_LU_WALK, false );
		return;
	}

	if ( true == GetRidingOn() )
		StateChange( LSI_RIDING_DASH, false );
	else if ( true == IsMinorUnitActive() )
		StateChange( LSI_CIEL_DASH, false );
	else
		StateChange( LSI_LU_DASH, false );
}

// char-luciel CH1l2: June 32ae9bd0 ( vslot 0x658 ); only the two Phantom Rush skills answer, with their chain being ready
/*virtual*/ bool CX2GULuciel::IsSkillChainReady( const int iSkillID_, const int iUnused_ )
{
	UNREFERENCED_PARAMETER( iUnused_ );

	switch ( iSkillID_ )
	{
	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH:
	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH_UPGRADE:
		return m_bPhantomRushReady;

	default:
		return false;
	}
}

// char-luciel CH1l2: June 32aea8c0 ( vslot 0x6a8 ); Summon Darkness, Twilight Judgement and Phantom Rush show no cool time while their effect or chain is up
/*virtual*/ float CX2GULuciel::ShownSkillCoolTime( const float fCoolTime_, const int iSkillID_ )
{
	switch ( g_pData->GetSkillTree()->GetUpgradeBaseSkillID( static_cast<CX2SkillTree::SKILL_ID>( iSkillID_ ) ) )
	{
	case CX2SkillTree::RNSI_SA_LLD_SUMMON_DARKNESS:
		if ( true == HaveThisBuff( BTI_BUFF_SUMMON_DARKNESS ) )
			return 0.f;
		break;

	case CX2SkillTree::RNSI_SA_LCN_TWILIGHT_JUDGEMENT:
		if ( 0 != GetBuffAccumulationCountNowInFactor( BTI_BUFF_TWILIGHT_JUDGEMENT ) )
			return 0.f;
		break;

	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH:
	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH_UPGRADE:
		if ( true == m_bPhantomRushReady )
			return 0.f;
		break;

	default:
		break;
	}

	return fCoolTime_;
}

// char-luciel CH1l2: June 32aeb380 ( vslot 0x670 ); job line 1 is Chiliarch / Dread Lord, 2 Royal Guard / Noblesse, 3 Diablura / Demonio
bool CX2GULuciel::IsInJobLine( int iJobLine_ )
{
	const int iUnitClass = ( NULL != GetUnit() ) ? static_cast<int>( GetUnit()->GetClass() ) : 0;

	switch ( iJobLine_ )
	{
	case 1:
		return CX2Unit::UC_LUCIEL_CHILIARCH == iUnitClass || CX2Unit::UC_LUCIEL_DREAD_LORD == iUnitClass;

	case 2:
		return CX2Unit::UC_LUCIEL_ROYAL_GUARD == iUnitClass || CX2Unit::UC_LUCIEL_NOBLESSE == iUnitClass;

	case 3:
		return CX2Unit::UC_LUCIEL_DIABLURA == iUnitClass || CX2Unit::UC_LUCIEL_DEMONIO == iUnitClass;

	default:
		return false;
	}
}

// char-luciel CH1l2: June 32aeb410 ( vslot 0x740 ); the own unit runs the base, stand-in: the base 32009780 Crest of Time snapshot is not ported
/*virtual*/ void CX2GULuciel::StoreGageSnapshot()
{
	if ( false == IsMyUnit() )
		return;
}

// char-luciel CH1l2: June 32aeb430 ( vslot 0x744 ); the own unit runs the base, stand-in: the base 3204da60 Crest of Time restore is not ported
/*virtual*/ void CX2GULuciel::RestoreGageSnapshot()
{
	if ( false == IsMyUnit() )
		return;
}

// char-luciel CH1l2: June 32b0e440 ( vslot 0x6b0 ); a transformed body takes its own animation and after-image, any body other than the first reads the second
/*virtual*/ void CX2GULuciel::UpdateAfterImageEquip( const AFTER_IMAGE_BODY eBody_ )
{
	if ( AIB_FIRST_UNIT == eBody_ )
	{
		const bool bTransformed = IsTransformed( CX2Unit::DT_MAJOR );
		CKTDGXSkinAnimPtr pXSkinAnim = ( true == bTransformed ) ? m_aTransformedSkin[0].m_pXSkinAnim : m_pXSkinAnim;
		CKTDGXSkinAfterImage* pAfterImage = ( true == bTransformed ) ? m_aTransformedSkin[0].m_pAfterImage : m_pAfterImage;

		if ( NULL != pAfterImage && NULL != pXSkinAnim )
			pAfterImage->UpdateEquipView( pXSkinAnim.get() );
	}
	else
	{
		const bool bTransformed = IsTransformed( CX2Unit::DT_MINOR );
		CKTDGXSkinAnimPtr pXSkinAnim = ( true == bTransformed ) ? m_aTransformedSkin[1].m_pXSkinAnim : m_pMinorUnitXSkinAnim;
		CKTDGXSkinAfterImage* pAfterImage = ( true == bTransformed ) ? m_aTransformedSkin[1].m_pAfterImage : m_pMinorUnitAfterImage;

		if ( NULL != pAfterImage && NULL != pXSkinAnim )
			pAfterImage->UpdateEquipView( pXSkinAnim.get() );
	}
}

// char-luciel CH1l2: June 32b0e670 ( vslot 0x794 ); the three Demonio skills of the body in play take the skill-group power sockets
/*virtual*/ void CX2GULuciel::SumSocketOptionAfterSlots( CX2SocketItem::SocketData& kAggregate_, const CX2SocketItem::SocketData& kSocketData_ )
{
	UNREFERENCED_PARAMETER( kAggregate_ );	// AddSkillAdditionalPowerRateBySlot writes m_SocketData, the aggregate SumSocketOption passes

	if ( NULL == GetUnit() )
		return;

	const CX2SkillTree::SKILL_ID aeLuSkillID[3] = { CX2SkillTree::RNSI_SA_LDB_DEATH_TOUCH, CX2SkillTree::RNSI_SA_LDB_MORTAL_FEAR, CX2SkillTree::RNSI_SA_LDB_DARK_PLASMA };
	const CX2SkillTree::SKILL_ID aeCielSkillID[3] = { CX2SkillTree::RNSI_SA_LDN_SHADOW_FURY, CX2SkillTree::RNSI_SA_LDN_SHREWD_CUTTER, CX2SkillTree::RNSI_SA_LDN_SILENT_RAGE };

	for ( int i = 0; i < 3; ++i )
		AddSkillAdditionalPowerRateBySlot( kSocketData_, ( true == IsMinorUnitActive() ) ? aeCielSkillID[i] : aeLuSkillID[i] );
}

// char-luciel CH1l2: June 32b0f940 ( vslot 0x7fc ); the use of the skill the lookup chose, a skill with no state change pays at once
/*virtual*/ void CX2GULuciel::SpecialAttackUse( const int iSkillID_, const bool bUnused_ )
{
	UNREFERENCED_PARAMETER( bUnused_ );

	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( static_cast<CX2SkillTree::SKILL_ID>( iSkillID_ ), true );

	if ( true == SpecialAttackNoStageChange( pSkillTemplet ) )
	{
		SpecialAttackCooldown( pSkillTemplet->m_eID, m_fChosenSkillMp );
		SkillMemoMpRefund( iSkillID_, true, m_fChosenSkillMp );
		m_fChosenSkillMp = 0.f;
		m_bSpecialAttackEventProcessedAtThisFrame = true;
		return;
	}

	ReserveSkillUse( pSkillTemplet->m_eID );
	m_iSkillUseMpForm = ( true == IsMinorUnitActive() ) ? 1 : 0;

	if ( false == StateChangeBySkillID( pSkillTemplet->m_eID ) )
		StateChangeBySkillSlot( m_iChosenSkillSlotIndex );

	m_bSpecialAttackEventProcessedAtThisFrame = true;
}

// char-luciel CH1l2: June 32b0fa90 ( vslot 0x664 ); the base's max MP once per form, each form's gauge gets its own max
/*virtual*/ void CX2GULuciel::ResetMaxMP()
{
	const float fBaseMaxMp = ONE_CHARGE * 3.0f;

	m_AdditionalMaxMp.SetBasicStatValue( fBaseMaxMp );

	for ( int iForm = 0; iForm < 2; ++iForm )
	{
		const float fResultMaxMp = GetUnit()->ResetMaxMp( fBaseMaxMp, iForm );
		const float fAbilityRate = 0.f;	// June 32358970( 5 ) x 0.01: June's per-unit ability map, 0 in March ( the base's stand-in )
		const float fRate = fAbilityRate + m_SocketData.m_Stat.m_ExtraStat.m_fIncreaseMPRate;
		const float fMaxMpWithSocket = fResultMaxMp + ( fResultMaxMp * fRate + m_SocketData.m_iAddMPValue );

		m_AdditionalMaxMp.SetOptionValue( fMaxMpWithSocket - fBaseMaxMp );
		m_AdditionalMaxMp.CheckAndUpdateResultValue();

		GetGageData()->SetMaxMp( m_AdditionalMaxMp.GetResultValue(), iForm );
	}
}

// char-luciel CH1l2: June 32b14640 ( vslot 0x6a0 ); no MP in the effect of Twilight Judgement, Summon Darkness or a ready Phantom Rush chain, Dancing Blade loops lower it, Open Mind scales the rest
/*virtual*/ float CX2GULuciel::GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const
{
	float fMpConsumption = CX2GUUser::GetActualMPConsume( eSkillID_, iSkillLevel_ );

	CX2GULuciel* pThis = const_cast<CX2GULuciel*>( this );	// HaveThisBuff is not const in March
	const CX2SkillTree::SKILL_ID eBaseSkillID = g_pData->GetSkillTree()->GetUpgradeBaseSkillID( eSkillID_ );

	switch ( eBaseSkillID )
	{
	case CX2SkillTree::RNSI_SA_LCN_TWILIGHT_JUDGEMENT:
		if ( true == pThis->HaveThisBuff( BTI_BUFF_TWILIGHT_JUDGEMENT ) )
			fMpConsumption = 0.f;
		break;

	case CX2SkillTree::SI_SA_LLD_DANCING_BLADE:
		fMpConsumption = max( 0.f, fMpConsumption - m_usDancingBladeLoopCount * DANCING_BLADE_LOOP_MP_COST );
		break;

	case CX2SkillTree::RNSI_SA_LLD_SUMMON_DARKNESS:
		if ( true == pThis->HaveThisBuff( BTI_BUFF_SUMMON_DARKNESS ) )
			fMpConsumption = 0.f;
		break;

	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH:
	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH_UPGRADE:
		if ( true == m_bPhantomRushReady )
			fMpConsumption = 0.f;
		break;

	default:
		break;
	}

	// Open Mind: the skills of Lu's body ( first list ), of Ciel's ( second list ) and of both ( third list ) cost the passive's MP rate while its combination points are reached
	bool bOpenMindApplies = false;
	switch ( eBaseSkillID )
	{
	case CX2SkillTree::RNSI_SA_LUCIEL_TRIPLE_SHOT:
	case CX2SkillTree::RNSI_SA_LUCIEL_FLICK_SHOT:
	case CX2SkillTree::RNSI_SA_LLC_DARK_CRESCENT:
	case CX2SkillTree::RNSI_SA_LLD_SUPREME_PUNISHMENT:
	case CX2SkillTree::RNSI_SA_LLD_PHANTOM_BLADE:
	case CX2SkillTree::RNSI_SA_LCG_DEATH_CRISIS_OF_MARBAS:
	case CX2SkillTree::RNSI_SA_LCG_SOUL_EXTORTION:
	case CX2SkillTree::RNSI_SA_LCG_FANTASIE_IMPROMPTU_1ST_MOVEMENT:
	case CX2SkillTree::RNSI_SA_LCN_TWILIGHT_JUDGEMENT:
	case CX2SkillTree::RNSI_SA_LUCIEL_TRIPLE_SHOT_UPGRADE:
	case CX2SkillTree::RNSI_SA_LUCIEL_FLICK_SHOT_UPGRADE:
	case CX2SkillTree::RNSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_UPGRADE:
	case CX2SkillTree::RNSI_SA_LCG_SOUL_EXTORTION_UPGRADE:
	case CX2SkillTree::RNSI_SA_LDN_DESPERADO:
	case CX2SkillTree::RNSI_SA_LDN_SINISTER_MIND:
	case CX2SkillTree::RNSI_SA_LDN_GARDEN_OF_HAURES:
	case CX2SkillTree::RNSI_SA_LLD_FEAR_TOMORROW:
	case CX2SkillTree::RNSI_SA_LCN_MAGDONIC_CANNON:
	case CX2SkillTree::RNSI_SA_LDN_ABYSS_SNIPING:
		bOpenMindApplies = ( true == IsOpenMindConditionMet() && false == IsMinorUnitActive() );
		break;

	case CX2SkillTree::RNSI_SA_LUCIEL_STOMP:
	case CX2SkillTree::RNSI_SA_LUCIEL_EVIL_CLAW:
	case CX2SkillTree::RNSI_SA_LLC_RUINOUS_IMPACT:
	case CX2SkillTree::RNSI_SA_LLC_DEAD_HANDS:
	case CX2SkillTree::RNSI_SA_LLC_POWER_OF_ORIAX:
	case CX2SkillTree::RNSI_SA_LLD_SUMMON_DARKNESS:
	case CX2SkillTree::RNSI_SA_LCN_SHADOW_BOLT:
	case CX2SkillTree::RNSI_SA_LCG_DEMONIC_BREATH:
	case CX2SkillTree::RNSI_SA_LCN_SPECTRAL_SPEAR:
	case CX2SkillTree::RNSI_SA_LUCIEL_STOMP_UPGRADE:
	case CX2SkillTree::RNSI_SA_LUCIEL_EVIL_CLAW_UPGRADE:
	case CX2SkillTree::RNSI_SA_LLC_RUINOUS_IMPACT_UPGRADE:
	case CX2SkillTree::RNSI_SA_LLC_DEAD_HANDS_UPGRADE:
	case CX2SkillTree::RNSI_SA_LDB_LAND_CRASHER:
	case CX2SkillTree::RNSI_SA_LDB_BLOODLUST:
	case CX2SkillTree::RNSI_SA_LDB_NIGHTMARE:
	case CX2SkillTree::RNSI_SA_LDB_LAND_CRASHER_UPGRADE:
	case CX2SkillTree::RNSI_SA_LLD_DEAD_CROSS:
	case CX2SkillTree::RNSI_SA_LCN_SOUL_LOCK:
	case CX2SkillTree::RNSI_SA_LDN_DEMONIC_RUSH:
	case CX2SkillTree::RNSI_A_LCN_SWIFT_SMASH:
		bOpenMindApplies = ( true == IsOpenMindConditionMet() && true == IsMinorUnitActive() );
		break;

	case CX2SkillTree::RNSI_A_LLC_BLASTING_FEAR:
	case CX2SkillTree::RNSI_A_LLC_BLASTING_FEAR_UPGRADE:
	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH:
	case CX2SkillTree::RNSI_A_LDN_RAPID_IMPACT:
	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH_UPGRADE:
		bOpenMindApplies = IsOpenMindConditionMet();
		break;

	default:
		break;
	}

	if ( true == bOpenMindApplies )
		fMpConsumption = GetLearnedSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL, GetOpenMindSkillID() ) * fMpConsumption;

	return fMpConsumption;
}

// char-luciel CH1l2: June 32b15050 ( vslot 0x6a4 ); -1 ( no cool time is stored ) for Summon Darkness and Twilight Judgement while their effect lives and for the Phantom Rush target states, never below 0
/*virtual*/ float CX2GULuciel::GetActualCoolTime( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_, IN int iSkillLevel_ ) const
{
	if ( NULL == pSkillTemplet_ )
		return 0.f;

	// June's base takes a third argument ( 0 ) March does not have; the two skill memo cases ( 10003, 30207 ) read June's GetEqippedSkillMemo stub, which answers 0: never applied, not written
	float fCoolTime = CX2GUUser::GetActualCoolTime( pSkillTemplet_, iSkillLevel_ );

	CX2GULuciel* pThis = const_cast<CX2GULuciel*>( this );	// HaveThisBuff is not const in March

	switch ( g_pData->GetSkillTree()->GetUpgradeBaseSkillID( pSkillTemplet_->m_eID ) )
	{
	case CX2SkillTree::RNSI_SA_LLD_SUMMON_DARKNESS:
		if ( true == pThis->HaveThisBuff( BTI_BUFF_SUMMON_DARKNESS ) )
			return -1.f;
		break;

	case CX2SkillTree::RNSI_SA_LCN_TWILIGHT_JUDGEMENT:
		if ( 0 != GetBuffAccumulationCountNowInFactor( BTI_BUFF_TWILIGHT_JUDGEMENT ) )
			return -1.f;
		break;

	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH:
	case CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH_UPGRADE:
		if ( LSI_A_LDB_F_PHANTOM_RUSH_TARGET == GetNowStateID() || LSI_A_LDB_M_PHANTOM_RUSH_TARGET == GetNowStateID() )
			return -1.f;
		break;

	default:
		break;
	}

	return max( 0.f, fCoolTime );
}

// char-luciel CH1l2: June 32b15220 ( vslot 0x828 ); the base's test less the two power landing states
/*virtual*/ bool CX2GULuciel::IsNotCommonMoveState()
{
	if ( false == CX2GUUser::IsNotCommonMoveState() )
		return false;

	return LSI_LU_DASH_JUMP_POWER_LANDING != GetNowStateID() && LSI_CIEL_DASH_JUMP_POWER_LANDING != GetNowStateID();
}

// char-luciel CH1l2: June 32b15430 ( vslot 0x850 ); both after-images follow, they draw nothing in either body's wait state
/*virtual*/ void CX2GULuciel::AfterImageFrameMove()
{
	const int iNowStateID = GetNowStateID();
	const bool bEnable = ( LSI_LU_WAIT != iNowStateID && LSI_CIEL_WAIT != iNowStateID );

	if ( NULL != m_pAfterImage )
	{
		if ( true == bEnable )
			m_pAfterImage->Enable();
		else
			m_pAfterImage->Disable();
	}

	if ( NULL != m_pMinorUnitAfterImage )
	{
		if ( true == bEnable )
			m_pMinorUnitAfterImage->Enable();
		else
			m_pMinorUnitAfterImage->Disable();
	}

	if ( NULL != m_pAfterImage )
		m_pAfterImage->OnFrameMove( m_fTime, m_fElapsedTime );

	if ( NULL != m_pMinorUnitAfterImage )
		m_pMinorUnitAfterImage->OnFrameMove( m_fTime, m_fElapsedTime );
}

// char-luciel CH1l2: June 32b16c80 ( vslot 0x834 ); the cut-in by the unit's cut-in set sub index
/*virtual*/ void CX2GULuciel::ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ )
{
	ShowActiveSkillCutInAndLight( fTimeToShow_, m_iSkillCutInSetSubIndex, bOnlyLight_ );
}

// char-luciel CH1l2: June 32b171c0 ( vslot 0x66c ); each body scales its head bone, or its spine bone while it wears the one-piece avatar
/*virtual*/ void CX2GULuciel::ScaleHead( float fHeadScale_, float fFaceScale_ )
{
	for ( int iForm = 0; iForm < 2; ++iForm )
		ScaleHeadBone( ( true == m_mapScaleHeadByForm[iForm] ) ? L"Bip01_Spine1" : L"Bip01_Head", fHeadScale_, fFaceScale_, iForm );
}

// char-luciel CH1l2: June 32b172b0 ( vslot 0x87c )
/*virtual*/ void CX2GULuciel::SetSDOnePieceAvatar( const int iForm_, const bool bSDOnePieceAvatar_ )
{
	m_mapScaleHeadByForm[iForm_] = bSDOnePieceAvatar_;
}

// char-luciel CH1l2: June 32b18170 ( vslot 0x848 ); a skill state that gains combination points adds the skill's hit counter entry of this damage effect index once
/*virtual*/ void CX2GULuciel::RegisterDamageEffectHitCountEntry()
{
	if ( false == IsMyUnit() )
		return;

	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( GetNowStateSkillID() );
	if ( NULL == pSkillTemplet || 0 >= pSkillTemplet->m_usFPGainCount )
		return;

	SDamageEffectHitCount hitCountEntry;
	hitCountEntry.m_iSkillID = g_pData->GetSkillTree()->GetUpgradeBaseSkillID( pSkillTemplet->m_eID );
	hitCountEntry.m_iDamageEffectIndex = m_iDamageEffectIndex;
	hitCountEntry.m_iHitCount = 0;

	// June 32b4cb40: std::find over the 12-byte entries, the same skill and damage effect index
	std::vector<SDamageEffectHitCount>::iterator vit = m_vecDamageEffectHitCount.begin();
	for ( ; vit != m_vecDamageEffectHitCount.end(); ++vit )
	{
		if ( vit->m_iDamageEffectIndex == hitCountEntry.m_iDamageEffectIndex && vit->m_iSkillID == hitCountEntry.m_iSkillID )
			break;
	}

	if ( vit == m_vecDamageEffectHitCount.end() )
		m_vecDamageEffectHitCount.push_back( hitCountEntry );
}

// char-luciel CH1l2: June 32b1c880 ( vslot 0x810 ); Summon Darkness used while its effect lives ends it by a user action instead of a state change
/*virtual*/ bool CX2GULuciel::SpecialAttackNoStageChange( const CX2SkillTree::SkillTemplet* pSkillTemplet_ )
{
	if ( NULL == pSkillTemplet_ )
		return false;

	if ( CX2SkillTree::RNSI_SA_LLD_SUMMON_DARKNESS != g_pData->GetSkillTree()->GetUpgradeBaseSkillID( pSkillTemplet_->m_eID ) )
		return false;

	if ( true == GetRidingOn() )
		return false;

	if ( false == HaveThisBuff( BTI_BUFF_SUMMON_DARKNESS ) )
		return false;

	if ( UAI_CANCEL_SUMMON_DARKNESS != m_FrameDataFuture.syncData.nowAction )
		m_FrameDataFuture.syncData.nowAction = UAI_CANCEL_SUMMON_DARKNESS;

	return true;
}

// char-luciel CH1l2: June 32b1edd0 ( vslot 0x7c8 ); the combination point follows into the synced int, the switching cool time runs down, the switching guide shows it
/*virtual*/ void CX2GULuciel::CommonFrameMoveFuture()
{
	CX2GUUser::CommonFrameMoveFuture();

	const int iNowCombinationPoint = GetNowCombinationPoint();	// June reads the gauge's value cell, 0 without a gauge
	if ( m_FrameDataFuture.syncData.iClassSyncInt != iNowCombinationPoint )
		m_FrameDataFuture.syncData.iClassSyncInt = iNowCombinationPoint;

	if ( 0.f < m_fSwitchingCoolTime )
	{
		m_fSwitchingCoolTime -= m_fElapsedTime;
		if ( m_fSwitchingCoolTime < 0.f )
			m_fSwitchingCoolTime = 0.f;
	}

	bool bShowSwitchingGuide = true;
	g_pX2Game->UpdateLucielSwitchingGuideUI( &bShowSwitchingGuide, m_fSwitchingCoolTime );
}

// char-luciel CH1l2: June 32b1eef0 ( vslot 0x800 ); the own unit pays the stored combination points, the MP, the combination points of the skill, the finality stone and the cool time
/*virtual*/ void CX2GULuciel::SpecialAttackCooldown( const int iSkillID_, const float fMpConsume_ )
{
	if ( false == IsMyUnit() )
		return;

	UpCombinationPoint( m_iLastUsedSkillID );	// June passes the stored value as a point count ( the member's name is CH1a's )
	m_iLastUsedSkillID = 0;

	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( static_cast<CX2SkillTree::SKILL_ID>( iSkillID_ ), true );
	if ( NULL == GetUnit() || NULL == pSkillTemplet )
		return;

	MarkSkillSlotUsed( m_iChosenSkillSlotIndex, false );
	UpGageNowMp( -fMpConsume_, m_iSkillUseMpForm );

	if ( CX2SkillTree::ST_HYPER_ACTIVE_SKILL == pSkillTemplet->m_eType && false == IsTrainingOrPhotozone() )
	{
		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );
		if ( NULL != pItem )
			g_pX2Game->Handler_EGS_USE_FINALITY_SKILL_REQ( pItem->GetItemData().m_ItemUID );
	}

	const int iSkillLevel = max( 1, GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( pSkillTemplet->m_eID, false, 0, true ) );

	UpCombinationPoint( -GetSkillCombinationPointCost( static_cast<CX2SkillTree::SKILL_ID>( iSkillID_ ), iSkillLevel ) );

	const float fCoolTime = GetActualCoolTime( pSkillTemplet, iSkillLevel );
	if ( false == ( fabs( -1.f - fCoolTime ) <= FLT_EPSILON ) )	// June 31c7b640: a cool time of -1 is not stored
	{
		GetUnit()->AccessUnitData().m_UserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, fCoolTime, true );
		GetUnit()->ApplySkillCoolTimeRateByBuff( pSkillTemplet->m_eID );
	}

	CX2RandomMissionManager::GetInstance()->OnSkillUse( pSkillTemplet->m_eID );
}

// char-luciel CH1l2: June 32b37910 ( vslot 0x7c4 ); skills with a combination point cost take the specialization B rate, the Communion skills their rate, the camera override clears, the Altar of Evil effect sets stop
/*virtual*/ void CX2GULuciel::CommonStateStart()
{
	CX2GUUser::CommonStateStart();

	const CX2SkillTree::SkillTemplet* pNowStateSkillTemplet = GetEquippedActiveSkillTemplet( GetNowStateSkillID(), true );
	if ( NULL != pNowStateSkillTemplet && 0 < pNowStateSkillTemplet->m_usFPConsumtion )
		m_fNowStatePowerRate = m_fNowStatePowerRate * static_cast<float>( GetCharacterSpecializationB() + 1.0 );

	switch ( g_pData->GetSkillTree()->GetUpgradeBaseSkillID( GetNowStateSkillID() ) )
	{
	case CX2SkillTree::RNSI_SA_LUCIEL_NETHER_SHREDDER:
	case CX2SkillTree::RNSI_SA_LUCIEL_SOUL_INFEST:
	case CX2SkillTree::RNSI_SA_LLC_APOLLYON:
	case CX2SkillTree::RNSI_SA_LLD_OBLIVION:
	case CX2SkillTree::RNSI_SA_LCG_ESCHATON:
	case CX2SkillTree::RNSI_SA_LCN_FANTASIE_IMPROMPTU_2ND_MOVEMENT:
	case CX2SkillTree::RNSI_SA_LUCIEL_NETHER_SHREDDER_UPGRADE:
	case CX2SkillTree::RNSI_SA_LUCIEL_SOUL_INFEST_UPGRADE:
	case CX2SkillTree::RNSI_SA_LLD_DEMONIC_CANNON:
	case CX2SkillTree::RNSI_SA_LCN_RAIN_OF_CHAOS:
		m_fNowStatePowerRate = m_fNowStatePowerRate * m_fCommunionRate;
		break;

	default:
		break;
	}

	if ( true == IsMyUnit() && false == CX2UISkillTreeNew::IsSameOrUpgradeSkill( GetNowStateSkillID(), CX2SkillTree::RNSI_SA_LCG_DEATH_CRISIS_OF_MARBAS )
		&& LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_WAIT != GetNowStateID() && LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_SMALL_FIRE != GetNowStateID()
		&& LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_BIG_CHARGE != GetNowStateID() && LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_BIG_FIRE != GetNowStateID()
		&& LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_END != GetNowStateID() )
	{
		m_bSkillCameraOverride = false;
		m_fSkillCameraOverrideValue = 0.f;
	}

	switch ( GetNowStateID() )
	{
	case LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DEATH_TOUCH:
	case LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_MORTAL_FEAR:
	case LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DARK_PLASMA:
	case LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_END:
	case LSI_SA_LDB_ALTAR_OF_EVIL_END:
		ClearAltarOfEvilEffectSet();
		break;

	default:
		break;
	}
}

// char-luciel CH1l2: June 32b37c40 ( vslot 0x80c ); the base's user actions, then the Summon Darkness cancel action
/*virtual*/ void CX2GULuciel::NoStateChangeActionFrameMove()
{
	UserNoStateChangeActionFrameMove();

	if ( UAI_CANCEL_SUMMON_DARKNESS == m_FrameDataNow.syncData.nowAction )
		CancelSummonDarkness();
}

// char-luciel CH1l2: June 32b397a0 ( vslot 0x7cc ); the base, the per-form timers and effects of Lu's skills, the remote unit's combination point, the transformed-state helpers of CH1l3
/*virtual*/ void CX2GULuciel::CommonFrameMove()
{
	CX2GUUser::CommonFrameMove();

	FrameMoveChaserOfDarknessEffect();

	if ( false == IsMyUnit() )
	{
		CX2LucielGageData* pLucielGageData = static_cast<CX2LucielGageData*>( GetGageData() );
		if ( NULL != pLucielGageData )
		{
			pLucielGageData->SetCombinationPointChanged( true );
			pLucielGageData->SetNowCombinationPoint( m_FrameDataNow.syncData.iClassSyncInt );
		}
	}

	if ( true == m_bNeedToUpdateWeaponEffect )
	{
		UpdateWeaponEnchantEffectAndExtraDamageType();
		m_bNeedToUpdateWeaponEffect = false;
	}

	// the first animation event of Lu's four basic skills plays the body switch effect sets June's start flags asked for ( June reads the Future list )
	if ( true == GetXSkinAnimPtr()->EventTimerOneshot( BODY_SWITCH_EFFECT_EVENT_TIME ) )
	{
		switch ( GetNowStateSkillID() )
		{
		case CX2SkillTree::RNSI_SA_LUCIEL_TRIPLE_SHOT:
		case CX2SkillTree::RNSI_SA_LUCIEL_STOMP:
		case CX2SkillTree::RNSI_SA_LUCIEL_FLICK_SHOT:
		case CX2SkillTree::RNSI_SA_LUCIEL_EVIL_CLAW:
		case CX2SkillTree::RNSI_SA_LUCIEL_TRIPLE_SHOT_UPGRADE:
		case CX2SkillTree::RNSI_SA_LUCIEL_STOMP_UPGRADE:
		case CX2SkillTree::RNSI_SA_LUCIEL_FLICK_SHOT_UPGRADE:
		case CX2SkillTree::RNSI_SA_LUCIEL_EVIL_CLAW_UPGRADE:
			if ( true == m_FrameDataFuture.stateParam.bEventFlagList[EVENT_FLAG_DUO_TO_MINOR_UNIT] )
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_LD_F_SW_Out", this );

			if ( true == m_FrameDataFuture.stateParam.bEventFlagList[EVENT_FLAG_DUO_TO_FIRST_UNIT] )
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_LD_M_SW_Out", this );
			break;

		default:
			break;
		}
	}

	if ( 0.f < m_fSeizedByFearTimer )
	{
		m_fSeizedByFearTimer -= m_fElapsedTime;
		if ( m_fSeizedByFearTimer < 0.f )
			m_fSeizedByFearTimer = 0.f;
	}

	if ( 0.f < m_fPowerOfOriaxTime )
	{
		m_fPowerOfOriaxTime -= m_fElapsedTime;
		if ( m_fPowerOfOriaxTime < 0.f )
			m_fPowerOfOriaxTime = 0.f;
	}

	CX2DamageEffect::CEffect* pSoulShockDamageEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_hSoulShockDamageEffect, true );
	if ( NULL != pSoulShockDamageEffect )
		pSoulShockDamageEffect->SetPos( GetPos( false ) );

	const bool bDeadHandsBuff = HaveThisBuff( BTI_BUFF_DEAD_HANDS );
	if ( false == bDeadHandsBuff && true == m_bDeadHandsActive )
		PlaySound( L"Lu_SP_DeadHands02_Release.ogg", true, false );

	m_bDeadHandsActive = bDeadHandsBuff;

	// the body out of play waits in its pose while the two bodies are not both shown
	if ( false == m_bDuoEnabled )
	{
		if ( false == IsMinorUnitActive() )
		{
			m_pMinorUnitXSkinAnim->ChangeAnim( L"Wait", false );
			m_pMinorUnitXSkinAnim->Play( CKTDGXSkinAnim::XAP_LOOP );
		}
		else
		{
			m_pXSkinAnim->ChangeAnim( L"Wait3", false );
			m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_LOOP );
		}
	}

	if ( false == HaveThisBuff( BTI_BUFF_SUMMON_DARKNESS ) )
	{
		KillMySummonedNPC( CX2UnitManager::NUI_SI_SA_LLD_SUMMON_DARKNESS );
		KillMySummonedNPC( CX2UnitManager::NUI_SI_SA_LLD_SUMMON_DARKNESS_PVP );
	}

	if ( 0.f < m_fSoulStrikeCoolTime )
	{
		m_fSoulStrikeCoolTime -= m_fElapsedTime;
		if ( m_fSoulStrikeCoolTime < 0.f )
			m_fSoulStrikeCoolTime = 0.f;
	}

	if ( 0.f < m_fFearDamageEffectCoolTime )
	{
		m_fFearDamageEffectCoolTime -= m_fElapsedTime;
		if ( m_fFearDamageEffectCoolTime < 0.f )
			m_fFearDamageEffectCoolTime = 0.f;
	}

	if ( true == HaveThisBuff( BTI_BUFF_SUMMON_DARKNESS ) )
		m_fSummonedDarknessElapsedTime += m_fElapsedTime;

	FrameMoveShadowBoltSpear();
	FrameMoveTwilightJudgementMp();
	FrameMoveAuraOfFear();

	if ( 0.f < m_fCombinationPointOnHitCoolTime )
	{
		m_fCombinationPointOnHitCoolTime -= m_fElapsedTime;
		if ( m_fCombinationPointOnHitCoolTime < 0.f )
			m_fCombinationPointOnHitCoolTime = 0.f;
	}

	if ( 0.f <= m_fPhantomRushReadyTime )
	{
		m_fPhantomRushReadyTime -= m_fElapsedTime;
		if ( m_fPhantomRushReadyTime < 0.f )
			ResetPhantomRushReady();
	}

	FrameMoveDeathTouchEffect();
	UpdateInvasionOfDarkEnergyTargetTime();
	FrameMoveAltarOfEvilEnd();
	FrameMoveBoundingKeyDialog();

	if ( NULL != GetUnit() && 0 != GetUnit()->GetUnitData().m_iUnitClassTran )
	{
		UpdateOpenMindBuff();
		UpdateStableDarkEnergyBuff();
	}

	if ( LSI_A_LDN_M_PUNISHER != GetNowStateID() && INVALID_EFFECTSET_HANDLE != m_hPunisherEffectSet )
		g_pX2Game->GetEffectSet()->StopEffectSet( m_hPunisherEffectSet );

	if ( 0.f < m_fCombinationPointOverflowCoolTime )
	{
		m_fCombinationPointOverflowCoolTime -= m_fElapsedTime;
		if ( m_fCombinationPointOverflowCoolTime < 0.f )
			m_fCombinationPointOverflowCoolTime = 0.f;
	}
}

// char-luciel CH1l2: June 32b3a310 ( vslot 0x7f8 ); chooses the skill of the pressed slot and checks that it can be used now: use condition, bans, cool time, MP, combination points, finality stone
/*virtual*/ bool CX2GULuciel::CanUseSpecialAttack( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition_ )
{
	if ( true == IsQuickSlotSpecialItemWaiting() )
		return false;

	CX2UserSkillTree& accessUserSkillTree = GetUnit()->AccessUnitData().m_UserSkillTree;
	const CX2UserSkillTree::SkillSlotData* pSkillSlotData = NULL;
	int iPressedSkillSlotIndex = -1;

	if ( false == CommonSpecialAttackEventProcess( accessUserSkillTree, pSkillSlotData, iPressedSkillSlotIndex ) )
		return false;

	if ( NULL == pSkillSlotData )
		return false;

	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( pSkillSlotData->m_eID, true );
	if ( NULL == pSkillTemplet )
		return false;

	if ( pSkillTemplet->m_eID != pSkillSlotData->m_eID )
	{
		const int iStringID = STR_ID_SKILL_REPLACED_BY_UPGRADE;
		PushInfoText( GET_STRING( iStringID ) );
		return false;
	}

	m_iChosenSkillID = pSkillTemplet->m_eID;

	// with Stable Dark Energy learned: a locked Phantom Rush is usable anywhere but in the air, Sinister Mind's air keys count as the ground key
	CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eKeyCondition = eActiveSkillUseCondition_;
	CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eSkillCondition = pSkillTemplet->m_eActiveSkillUseCondtion;
	if ( 0 < accessUserSkillTree.GetSkillLevel( CX2SkillTree::RNSI_P_LDN_STABLE_DARKENERGY, false, 0, true ) )
	{
		if ( CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH == pSkillTemplet->m_eID || CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH_UPGRADE == pSkillTemplet->m_eID )
		{
			if ( true == m_bStableDarkEnergyLocked && CX2SkillTree::ASUT_AIR != eKeyCondition && CX2SkillTree::ASUT_AIR_ONLY != eKeyCondition )
				eSkillCondition = CX2SkillTree::ASUT_ANYTIME_EXCEPT_AIR;
		}
		else if ( CX2SkillTree::RNSI_SA_LDN_SINISTER_MIND == g_pData->GetSkillTree()->GetUpgradeBaseSkillID( pSkillTemplet->m_eID ) )
		{
			if ( CX2SkillTree::ASUT_AIR == eKeyCondition || CX2SkillTree::ASUT_AIR_ONLY == eKeyCondition )
				eKeyCondition = CX2SkillTree::ASUT_GROUND;
		}
	}

	if ( false == CheckSkillUseCondition( eKeyCondition, eSkillCondition ) )
		return false;

	if ( false == CheckSummonSpecialAttack( pSkillTemplet->m_eID ) )
		return false;

	if ( false == CheckSkillUsePVPOfficial( pSkillTemplet->m_eID ) )
		return false;

	if ( false == CheckSkillUseNotPve( pSkillTemplet->m_eID ) )
		return false;

	if ( false == CheckSkillUseNotDungeon( pSkillTemplet->m_eID ) )
		return false;

	if ( true == IsRidingKeySkillWaiting() )
		return false;

	const int iSkillTempletLevel = max( 1, accessUserSkillTree.GetSkillLevel( pSkillTemplet->m_eID, false, 0, true ) );

	m_fChosenSkillMp = GetActualMPConsume( pSkillTemplet->m_eID, iSkillTempletLevel );
	const int iCombinationPointCost = GetSkillCombinationPointCost( pSkillTemplet->m_eID, iSkillTempletLevel );

	if ( false == IsDevUnlimitedSkillUse() )
	{
		const float fCoolTimeLeft = ShownSkillCoolTime( pSkillSlotData->m_fCoolTimeLeft, pSkillTemplet->m_eID );
		if ( 0.f < fCoolTimeLeft )
		{
			PushInfoText( GET_STRING( STR_ID_226 ) );
			return false;
		}

		if ( m_fReservedSkillMp + m_fChosenSkillMp > GetNowMp() )
		{
			PushInfoText( GET_STRING( STR_ID_2549 ) );
			return false;
		}

		if ( GetNowCombinationPoint() < iCombinationPointCost )
		{
			const int iStringID = STR_ID_NOT_ENOUGH_COMBINATION_POINT;
			PushInfoText( GET_STRING( iStringID ) );
			return false;
		}

		if ( CX2SkillTree::ST_HYPER_ACTIVE_SKILL == pSkillTemplet->m_eType && false == IsTrainingOrPhotozone()
			&& g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE ) <= 0 )
		{
			PushInfoText( GET_STRING( STR_ID_26119 ) );
			return false;
		}
	}

	m_iChosenSkillID = pSkillTemplet->m_eID;
	m_iChosenSkillSlotIndex = iPressedSkillSlotIndex;
	return true;
}

// char-luciel CH1l3: the helper bodies CH1l and CH1l2 declared, and the hit readers of AttackResultByType ( June 32aeb1a0 - 32b4d260 )
namespace _CONST_LUCIEL_CH1L3_
{
	const int		MP_FORM_ACTIVE_BODY									= 3;		// June 32aeb1a0: the form argument that means the body in play
	const float		TWILIGHT_JUDGEMENT_MP_DIVISOR						= 6.f;		// June 32aebb60 ( 3343a468, a double )
	const float		SHADOW_FURY_DAMAGE_RATE								= 0.5f;		// June 32b0e4e0 ( 3342ebc8, a double )
	const float		SUMMON_DARKNESS_DURATION							= 30.f;		// June 32b204d0 ( 33439c68, a double ): the buff's full time
	const float		SUMMON_DARKNESS_REFUND_RATE							= 0.5f;		// June 32b204d0 ( 3342ebc8, a double )
	const float		SHADOW_BOLT_SELF_DESTRUCT_INTERVAL					= 1.f;		// June 32b20780 ( 334355b0, a double ): seconds between two held-state checks
	const double	MILLISECONDS_PER_SECOND								= 1000.0;	// June 32b20780 ( 3342f008 )
	const float		CHASER_OF_DARKNESS_FIRE_INTERVAL					= 0.3f;		// June 32b1f800 ( 3345993c )
	const float		DEATH_TOUCH_SELF_BONE_OFFSET_Y						= 25.f;		// June 32b34a90 ( 33474890, a double ): added to Lu's Bip01
	const float		DEATH_TOUCH_TARGET_BONE_OFFSET_Y					= 25.f;		// June 32b34a90 ( 33474890, a double ): added to the target's Bip01
	const float		DEATH_TOUCH_TARGET_POSITION_OFFSET_Y				= 100.f;	// June 32b34a90 ( 3342f360, a double ): added to the target's position when it has no Bip01
	const float		PUNISHER_RANGE										= 500.f;	// June 32b34c70 ( 334ed3dc )
	const int		PUNISHER_MAX_TARGET_COUNT							= 5;		// June 32b34c70
	const float		PUNISHER_FIRST_HIT_EFFECT_RATE						= 0.5f;		// June 32b34c70 ( 334ed42c ): the modifier value while the relate skill index is 0
	const float		PUNISHER_LATER_HIT_EFFECT_RATE						= 0.25f;	// June 32b34c70 ( 33437818 )
	const int		EFFECT_MODIFIER_TAG_FORCE_DOWN_VALUE				= 9;		// June 321db1b0 modifier tag ( no Enum entry ): the damage data's force down value, multiplied or added
	const int		EFFECT_MODIFIER_TAG_EXTRA_DAMAGE					= 10;		// June 321db1b0 modifier tag ( no Enum entry ): the damage data's extra damage, multiplied or added
	const int		EFFECT_MODIFIER_TAG_RELATE_SKILL_INDEX				= 11;		// June 321db1b0 modifier tag ( no Enum entry ): the damage data's relate skill index byte
	const int		ALTAR_OF_EVIL_MAX_EFFECT_COUNT						= 8;		// June 32b35600
	const int		STATE_ID_ALTAR_OF_EVIL_UNNAMED						= 444;		// June 32b35c60 ( 0x1bc ): Enum.lua has no name for it ( between COMBO_XXX_END 443 and DEATH_TOUCH 446 )
	const int		OPEN_MIND_DEFAULT_COMBINATION_POINT					= 5;		// June 32b4c7f0: the threshold when the skill templet is missing
	const int		MAX_AURA_OF_FEAR_STACK_COUNT						= 10;		// June 32b15780

	// June 32007c40: the acquired skill's ability value ( Ara's and Rena's copies, carried per class )
	bool TryGetAcquiredSkillAbilityValue( CX2Unit* pUnit_, const CX2SkillTree::SKILL_ABILITY_TYPE eType_, const CX2SkillTree::SKILL_ID eSkillID_, const bool bMemoSwap_, float& fOut_ )
	{
		if ( NULL == pUnit_ )
			return false;

		const int iSkillLevel = pUnit_->GetUnitData().m_UserSkillTree.GetSkillLevel( eSkillID_, false, 0, true );
		if ( iSkillLevel <= 0 )
			return false;

		const CX2SkillTree::SkillTemplet* pSkillTemplet = pUnit_->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( eSkillID_, bMemoSwap_ );
		if ( NULL == pSkillTemplet )
			return false;

		const bool bPvpOrPve = ( NULL != g_pX2Game && true == g_pX2Game->GetIsNowPVPCorrectionGame() );	// June 322cc1a0: PVP or PVE
		return pSkillTemplet->TryGetSkillAbilityValue( eType_, iSkillLevel, bPvpOrPve, fOut_ );
	}

	// June 337ad268 ( initializer 333a78e0 ): the cut-ins of four sets, three bodies each ( Lu, Ciel, both ); a set's entry is set index * 3 + the unit's cut-in sub index
	const CX2GUUser::SkillCutIn		s_aSkillCutIn[] =
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Lu_01" ),			CX2GUUser::SkillCutIn( L"HQ_CutIn_Ciel_01" ),			CX2GUUser::SkillCutIn( L"HQ_CutIn_LuCiel_01" ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Lu_02_LLD" ),		CX2GUUser::SkillCutIn( L"HQ_CutIn_Ciel_02_LLD" ),		CX2GUUser::SkillCutIn( L"HQ_CutIn_LuCiel_02_LLD" ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Lu_02_LCN" ),		CX2GUUser::SkillCutIn( L"HQ_CutIn_Ciel_02_LCN" ),		CX2GUUser::SkillCutIn( L"HQ_CutIn_LuCiel_02_LCN" ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Lu_02_LDB" ),		CX2GUUser::SkillCutIn( L"HQ_CutIn_Ciel_02_LDN" ),		CX2GUUser::SkillCutIn( L"HQ_CutIn_Luciel_02_LDN" ),
	};

	// June 337ad4a8 ( initializer 333a8080 ): the third job's transformed forms ( Lu, Ciel, hyper active outside both )
	const CX2GUUser::SkillCutIn		s_aTransformedFormSkillCutIn[] =
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Lu_02_LDB_Evil" ),	CX2GUUser::SkillCutIn( L"HQ_CutIn_Ciel_02_LDN_Evil" ),	CX2GUUser::SkillCutIn( L"HQ_CutIn_LuCiel_02_LDN_Evil" ),
	};

	// June 337ad148 / 337ad1d8 ( initializer 333a74e0 ): the cut-ins of code event 0x8b3, three bodies each; the even random roll takes the first
	const CX2GUUser::SkillCutIn		s_aEventSkillCutInFirst[] =
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_LU_CASH_21" ),		CX2GUUser::SkillCutIn( L"HQ_CutIn_CIEL_CASH_21" ),		CX2GUUser::SkillCutIn( L"HQ_CutIn_LUCIEL_CASH_21" ),
	};
	const CX2GUUser::SkillCutIn		s_aEventSkillCutInSecond[] =
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_LU_CASH_22" ),		CX2GUUser::SkillCutIn( L"HQ_CutIn_CIEL_CASH_22" ),		CX2GUUser::SkillCutIn( L"HQ_CutIn_LUCIEL_CASH_22" ),
	};
	const int		SKILL_CUT_IN_COUNT_PER_SET							= 3;		// June 32b16940: set index * 3 + sub index
}
using namespace _CONST_LUCIEL_CH1L3_;

// char-luciel CH1l3: June 32aeb1a0; the gauge changes the MP of one form, 3 = the body in play
void CX2GULuciel::UpGageNowMp( const float fChangeOfNowMp_, const int iMpForm_ )
{
	if ( NULL == GetGageData() )
		return;

	int iMpGageIndex = iMpForm_;
	if ( MP_FORM_ACTIVE_BODY == iMpForm_ )
		iMpGageIndex = ( true == IsMinorUnitActive() ) ? 1 : 0;

	GetGageData()->UpNowMp( fChangeOfNowMp_, true, iMpGageIndex );
}

// char-luciel CH1l3: June 32aebb60; the Twilight Judgement stacks pay back MP once the buff is gone, then the stack count and the buff time follow the buff
void CX2GULuciel::FrameMoveTwilightJudgementMp()
{
	if ( 0 != m_ucTwilightJudgementStackCount && false == HaveThisBuff( BTI_BUFF_TWILIGHT_JUDGEMENT ) )
	{
		const float fMpConsume = GetActualMPConsume( GetUpgradeSwappedSkillID( CX2SkillTree::RNSI_SA_LCN_TWILIGHT_JUDGEMENT ), 1 );
		UpNowMpWithText( static_cast<float>( m_ucTwilightJudgementStackCount ) * static_cast<float>( fMpConsume / TWILIGHT_JUDGEMENT_MP_DIVISOR ) );
	}

	m_ucTwilightJudgementStackCount = GetBuffAccumulationCountNowInFactor( BTI_BUFF_TWILIGHT_JUDGEMENT );
	if ( true == HaveThisBuff( BTI_BUFF_TWILIGHT_JUDGEMENT ) )
		m_fTwilightJudgementTime += m_fElapsedTime;
}

// char-luciel CH1l3: June 32b0e4e0; Shadow Fury keeps half of the mana damage its hits do
void CX2GULuciel::AccumulateShadowFuryDamage( const CX2DamageManager::DamageData* pDamageData_ )
{
	if ( NULL == pDamageData_ || CX2SkillTree::RNSI_SA_LDN_SHADOW_FURY != pDamageData_->m_iAppliedSkillID )
		return;

	if ( CX2DamageManager::EDT_MANA_DAMAGE_WITH_NO_DISPLAY != pDamageData_->m_ExtraDamage.m_ExtraDamageType && CX2DamageManager::EDT_MANA_DAMAGE != pDamageData_->m_ExtraDamage.m_ExtraDamageType )
		return;

	const float fDamage = static_cast<float>( pDamageData_->m_ExtraDamage.m_Damage ) * SHADOW_FURY_DAMAGE_RATE;
	if ( 0.f < fDamage )
		m_fShadowFuryAccumulatedDamage += fDamage;
}

// char-luciel CH1l3: June 32b0e550; the bounding key dialog hides when its show time ends
void CX2GULuciel::FrameMoveBoundingKeyDialog()
{
	if ( 0.f <= m_fBoundingKeyShowTime && NULL != m_pDlgBoundingKey )
	{
		m_fBoundingKeyShowTime -= m_fElapsedTime;
		if ( m_fBoundingKeyShowTime <= 0.f )
		{
			m_pDlgBoundingKey->SetShow( false );
			m_fBoundingKeyShowTime = -1.f;
		}
	}
}

// char-luciel CH1l3: June 32b0eed0; the Stable Dark Energy buff follows the transformed forms
void CX2GULuciel::UpdateStableDarkEnergyBuff()
{
	if ( false == HaveThisBuff( BTI_BUFF_P_LDN_STABLE_DARKENERGY ) )
	{
		if ( true == HaveThisBuff( BTI_BUFF_TRANSCENDENCE ) || true == HaveThisBuff( BTI_BUFF_ALTAR_OF_EVIL ) )
		{
			if ( 0 < GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::RNSI_P_LDN_STABLE_DARKENERGY, false, 0, true ) )
				SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_P_LDN_STABLE_DARKENERGY, 1, NULL, 0, false );	// 7ai arguments
		}
	}

	if ( true == HaveThisBuff( BTI_BUFF_P_LDN_STABLE_DARKENERGY ) && false == HaveThisBuff( BTI_BUFF_TRANSCENDENCE ) && false == HaveThisBuff( BTI_BUFF_ALTAR_OF_EVIL ) )
		EraseBuffTempletFromGameUnit( BTI_BUFF_P_LDN_STABLE_DARKENERGY );
}

// char-luciel CH1l3: June 32b0ef80; the Open Mind passive of the unit's third job, -1 for any other class
int CX2GULuciel::GetOpenMindSkillID() const
{
	if ( NULL == GetUnit() )
		return -1;

	switch ( GetUnit()->GetClass() )
	{
	case CX2Unit::UC_LUCIEL_DREAD_LORD:
		return CX2SkillTree::RNSI_P_LLD_OPEN_MIND;

	case CX2Unit::UC_LUCIEL_NOBLESSE:
		return CX2SkillTree::RNSI_P_LCN_OPEN_MIND;

	case CX2Unit::UC_LUCIEL_DEMONIO:
		return CX2SkillTree::RNSI_P_LDN_OPEN_MIND;

	default:
		return -1;
	}
}

// char-luciel CH1l3: June 32b4c7f0; the class's Open Mind is learned and the combination points are at or below its threshold ( ability 2, 5 without a templet ), JUNE_BUGS 719
bool CX2GULuciel::IsOpenMindConditionMet() const
{
	if ( NULL == GetUnit() || NULL == GetGageData() )
		return false;

	const int iOpenMindSkillID = GetOpenMindSkillID();
	if ( -1 == iOpenMindSkillID )
		return false;

	const CX2SkillTree::SKILL_ID eOpenMindSkillID = static_cast<CX2SkillTree::SKILL_ID>( iOpenMindSkillID );
	const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( eOpenMindSkillID, false, 0, true );
	if ( iSkillLevel <= 0 )
		return false;

	int iCombinationPointThreshold = OPEN_MIND_DEFAULT_COMBINATION_POINT;
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eOpenMindSkillID );
	if ( NULL != pSkillTemplet )
		iCombinationPointThreshold = static_cast<int>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_COUNT, iSkillLevel, false ) );

	return GetNowCombinationPoint() <= iCombinationPointThreshold;
}

// char-luciel CH1l3: June 32b14d10; the Open Mind buff follows the condition above
void CX2GULuciel::UpdateOpenMindBuff()
{
	if ( false == HaveThisBuff( BTI_BUFF_P_LUCIEL_OPEN_MIND ) )
	{
		if ( true == IsOpenMindConditionMet() )
		{
			const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( static_cast<CX2SkillTree::SKILL_ID>( GetOpenMindSkillID() ), false, 0, true );
			SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_P_LUCIEL_OPEN_MIND, iSkillLevel, NULL, 0, false );	// 7ai arguments
		}
	}
	else if ( false == IsOpenMindConditionMet() )
	{
		EraseBuffTempletFromGameUnit( BTI_BUFF_P_LUCIEL_OPEN_MIND );
	}
}

// char-luciel CH1l3: June 32b153d0; a command attack made while a bounding strike state runs marks the strike as landed
void CX2GULuciel::CheckBoundingStrikeHit( const CX2DamageManager::DamageData* pDamageData_ )
{
	if ( NULL == pDamageData_ || CX2DamageManager::AT_COMMAND != pDamageData_->attackType || null == pDamageData_->optrDefenderGameUnit )
		return;

	if ( LSI_BOUNDING_TARGET_LU_STRIKER == GetNowStateID() || LSI_BOUNDING_TARGET_CIEL_STRIKER == GetNowStateID() )
		m_bBoundingStrikeHit = true;
}

// char-luciel CH1l3: June 32b15780; when the Aura of Fear time ends the stacks ( 10 at most ) pay MP back, then the count, time and duration reset
void CX2GULuciel::FrameMoveAuraOfFear()
{
	m_fAuraOfFearDuration -= m_fElapsedTime;

	if ( 0.f < m_fAuraOfFearTime )
	{
		m_fAuraOfFearTime -= m_fElapsedTime;
		if ( m_fAuraOfFearTime < 0.f )
		{
			const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::RNSI_A_LLD_FEAR_ABSORPTION, false, 0, true );
			const CX2SkillTree::SkillTemplet* pSkillTemplet = ( 0 < iSkillLevel ) ? GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_LLD_FEAR_ABSORPTION, true ) : NULL;
			if ( NULL != pSkillTemplet )
			{
				const float fRecoverMp = GetSkillNumericalByName( L"SA_RECOVER_MP_ABS", CX2SkillTree::RNSI_A_LLD_FEAR_ABSORPTION, true );
				const float fRecoverMpPerStack = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_RECOVER_MP_ABS_2, iSkillLevel, false );
				const int iStackCount = min( static_cast<int>( m_ucAuraOfFearStackCount ), MAX_AURA_OF_FEAR_STACK_COUNT );
				UpNowMpWithText( static_cast<float>( iStackCount ) * fRecoverMpPerStack + fRecoverMp );
			}

			m_ucAuraOfFearStackCount = 0;
			m_fAuraOfFearTime = 0.f;
			m_fAuraOfFearDuration = 1.f;
		}
	}
}

// char-luciel CH1l3: June 32b16760; the Invasion of Dark Energy marks run out with their time, the Rapid Impact timer counts down
void CX2GULuciel::UpdateInvasionOfDarkEnergyTargetTime()
{
	for ( std::vector<STimedGameUnitKey>::iterator vit = m_vecInvasionOfDarkEnergyTarget.begin(); vit != m_vecInvasionOfDarkEnergyTarget.end(); )
	{
		vit->m_fTime -= m_fElapsedTime;
		if ( false == ( vit->m_fTime <= 0.f ) )
			++vit;
		else
			vit = m_vecInvasionOfDarkEnergyTarget.erase( vit );
	}

	if ( 0.f < m_fRapidImpactRange )	// June counts it down every frame ( CH4a names the member with its readers )
		m_fRapidImpactRange -= m_fElapsedTime;
}

// char-luciel CH1l3: June 32b16880; a damage effect hit of a skill that gains combination points pays them once and drops its counter entry
void CX2GULuciel::AddCombinationPointByDamageEffectHit( const CX2DamageManager::DamageData* pDamageData_ )
{
	if ( NULL == pDamageData_ || true == m_vecDamageEffectHitCount.empty() || pDamageData_->m_iDamageEffectIndex < 0 || pDamageData_->m_iAppliedSkillID <= 0 )
		return;

	for ( std::vector<SDamageEffectHitCount>::iterator vit = m_vecDamageEffectHitCount.begin(); vit != m_vecDamageEffectHitCount.end(); ++vit )
	{
		if ( vit->m_iDamageEffectIndex == pDamageData_->m_iDamageEffectIndex && vit->m_iSkillID == pDamageData_->m_iAppliedSkillID && NULL != GetUnit() )
		{
			const CX2SkillTree::SKILL_ID eSwappedSkillID = GetUnit()->GetUnitData().m_UserSkillTree.GetUpgradeSwappedSkillID( static_cast<CX2SkillTree::SKILL_ID>( pDamageData_->m_iAppliedSkillID ) );
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSwappedSkillID );
			if ( NULL != pSkillTemplet )
			{
				UpCombinationPoint( pSkillTemplet->m_usFPGainCount );
				m_vecDamageEffectHitCount.erase( vit );
				return;
			}
		}
	}
}

// char-luciel CH1l3: June 32b16940 ( the cut-in sets are file-local: CH5 owns the cut-in data ); the cut-in of a skill at an animation time, JUNE_BUGS 718
void CX2GULuciel::ShowActiveSkillCutInAndLight( const float fTimeToShow_, const int iSkillCutInSetSubIndex_, const bool bOnlyLight_ )
{
	UNREFERENCED_PARAMETER( iSkillCutInSetSubIndex_ );	// June ignores it and reads m_iSkillCutInSetSubIndex

	if ( false == GetXSkinAnimPtr()->EventTimerOneshot( fTimeToShow_ ) || false == GetShowCutInAndChangeWorldColor() || false == GetShowActiveSkillShow() )
		return;

	CX2GUUser::SkillCutIn cutIn = s_aSkillCutIn[m_iSkillCutInSetIndex * SKILL_CUT_IN_COUNT_PER_SET + m_iSkillCutInSetSubIndex];

	// June 31fd8a50 / 3264e800: the applied customize item's cut-in replaces it ( CX2CustomizeManager, backlog Stage 12 ): stand-in, no item is ever applied
	const bool bCustomizeCutInApplied = false;

	if ( true == IsInJobLine( 3 ) )
	{
		bool bNowStateHyperActive = false;	// June 31fcef00 == 5: the now state's skill is a hyper active skill
		if ( NULL != GetUnit() && NULL != g_pData->GetSkillTree() && CX2SkillTree::SI_NONE != GetNowStateSkillID() )
		{
			const CX2SkillTree::SkillTemplet* pNowSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( GetNowStateSkillID() );
			if ( NULL != pNowSkillTemplet && CX2SkillTree::ST_HYPER_ACTIVE_SKILL == pNowSkillTemplet->m_eType )
				bNowStateHyperActive = true;
		}

		if ( true == IsTransformed( CX2Unit::DT_MAJOR ) )
			cutIn = s_aTransformedFormSkillCutIn[0];
		else if ( true == IsTransformed( CX2Unit::DT_MINOR ) )
			cutIn = s_aTransformedFormSkillCutIn[1];
		else if ( true == bNowStateHyperActive && false == bCustomizeCutInApplied )
			cutIn = s_aTransformedFormSkillCutIn[2];
	}

	IF_EVENT_ENABLED( 0x8b3 )
	{
		cutIn = ( 0 == ( rand() & 1 ) ) ? s_aEventSkillCutInFirst[m_iSkillCutInSetSubIndex] : s_aEventSkillCutInSecond[m_iSkillCutInSetSubIndex];	// June 32b16b9b: an even roll takes CASH_21
	}

	ActiveSkillShow( L"Dummy1_Rhand", cutIn.fileName.c_str(), cutIn.vSize, cutIn.vPosition, IsHyperState(), bOnlyLight_ );
}

// char-luciel CH1l3: June 32b176a0; appends { body byte, item id } of the worn equips at the position, two at most
void CX2GULuciel::GetFormEquipItemList( const CX2Unit::EQIP_POSITION eEquipPosition_, std::vector<SFormEquipItem>& vecFormEquipItem_ )
{
	BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_ViewEqipList )
	{
		if ( NULL == pViewEquip || eEquipPosition_ != pViewEquip->GetEqipPos() )
			continue;

		SFormEquipItem formEquipItem;
		formEquipItem.m_bMinorUnitEquip = IsEquipForMinorUnit( pViewEquip );	// stand-in until DU3, as CH1l's UpdateNotRenderingOtherEquipList
		formEquipItem.m_iItemID = pViewEquip->GetItemID();
		vecFormEquipItem_.push_back( formEquipItem );

		if ( 2 <= vecFormEquipItem_.size() )
			break;
	}
}

// char-luciel CH1l3: June 32b18060; a Desperado hit registers its defender once ( the entry's time is the raw word -1, JUNE_BUGS 720 )
void CX2GULuciel::RegisterDesperadoTarget( const CX2DamageManager::DamageData* pDamageData_ )
{
	if ( NULL == pDamageData_ || null == pDamageData_->optrDefenderGameUnit
		|| CX2SkillTree::RNSI_SA_LDN_DESPERADO != pDamageData_->m_iAppliedSkillID || CX2SkillTree::RNSI_SA_LDN_DESPERADO != pDamageData_->m_eUsedSkillID )
		return;

	CX2GameUnit* pDefenderUnit = pDamageData_->optrDefenderGameUnit.GetObservable();
	if ( NULL == pDefenderUnit )
		return;

	for ( std::vector<STimedGameUnitKey>::const_iterator vit = m_vecDesperadoTarget.begin(); vit != m_vecDesperadoTarget.end(); ++vit )
	{
		const CX2GameUnit* pRegisteredUnit = g_pX2Game->GetUnitByUIDAndType( vit->m_GameUnitKey.m_uidGameUnit, vit->m_GameUnitKey.m_eGameUnitType );
		if ( NULL != pRegisteredUnit && pRegisteredUnit->GetGameUnitType() == pDefenderUnit->GetGameUnitType() && pRegisteredUnit->GetUnitUID() == pDefenderUnit->GetUnitUID() )
			return;
	}

	STimedGameUnitKey desperadoTarget;
	*reinterpret_cast<DWORD*>( &desperadoTarget.m_fTime ) = 0xffffffff;
	desperadoTarget.m_GameUnitKey.m_uidGameUnit = pDefenderUnit->GetUnitUID();
	desperadoTarget.m_GameUnitKey.m_eGameUnitType = pDefenderUnit->GetGameUnitType();
	m_vecDesperadoTarget.push_back( desperadoTarget );
}

// char-luciel CH1l3: June 32b1eca0 ( Chung's SetChungMinorParticleByEnum pattern ); the handle's sequence, created on first use
CKTDGParticleSystem::CParticleEventSequence* CX2GULuciel::SetLucielMinorParticleByEnum( LUCIEL_MINOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ )
{
	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_ahLucielMinorParticleInstance[eVal_] )
	{
		m_ahLucielMinorParticleInstance[eVal_] = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this, wstrParticleName_.c_str(), D3DXVECTOR3( 0.f, 0.f, 0.f ), 0, 0, iDrawCount_, 0 );
	}

	return g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_ahLucielMinorParticleInstance[eVal_] );
}

// char-luciel CH1l3: June 32b1f650; the Chaser of Darkness ready effects follow Lu's Bip01, the fire timer counts down
void CX2GULuciel::FrameMoveChaserOfDarknessEffect()
{
	if ( true == m_vecChaserOfDarknessEffect.empty() || NULL == g_pX2Game || NULL == g_pX2Game->GetDamageEffect() )
		return;

	for ( std::vector<CX2DamageEffect::CEffectHandle>::iterator vit = m_vecChaserOfDarknessEffect.begin(); vit != m_vecChaserOfDarknessEffect.end(); )
	{
		CX2DamageEffect::CEffect* pChaserEffect = g_pX2Game->GetDamageEffect()->GetInstance( *vit, true );
		if ( NULL == pChaserEffect )
		{
			vit = m_vecChaserOfDarknessEffect.erase( vit );
		}
		else
		{
			pChaserEffect->SetPos( GetBonePos( L"Bip01" ) );
			++vit;
		}
	}

	if ( 0.f < m_fChaserOfDarknessFireTimer )
	{
		m_fChaserOfDarknessFireTimer -= m_fElapsedTime;
		if ( m_fChaserOfDarknessFireTimer <= 0.f )
			m_fChaserOfDarknessFireTimer = 0.f;
	}
}

// char-luciel CH1l3: June 32b1f800; a hit of the unit ( not a Chaser projectile ) turns the oldest ready effect into a projectile that locks on the defender
void CX2GULuciel::CreateChaserOfDarknessDamageEffect( const CX2DamageManager::DamageData* pDamageData_ )
{
	if ( NULL == pDamageData_ || 0.f < m_fChaserOfDarknessFireTimer || true == m_vecChaserOfDarknessEffect.empty() || NULL == g_pX2Game || NULL == g_pX2Game->GetDamageEffect() )
		return;

	if ( null == pDamageData_->optrDefenderGameUnit || CX2DamageManager::AT_UNIT != pDamageData_->defenderType
		|| CX2DamageManager::AT_SPECIAL == pDamageData_->attackType || CX2DamageManager::RT_DUMMY_DAMAGE == pDamageData_->reActResult )
		return;

	if ( INVALID_DAMAGE_EFFECT_HANDLE != pDamageData_->hAttackerEffect )
	{
		CX2DamageEffect::CEffect* pAttackerEffect = g_pX2Game->GetDamageEffect()->GetInstance( pDamageData_->hAttackerEffect, true );
		if ( NULL != pAttackerEffect && ( 0 == wcscmp( L"DamageEffect_LD_M_CHASER_OF_DARKNESS_Projectile", pAttackerEffect->GetName() ) || 0 == wcscmp( L"DamageEffect_LD_F_CHASER_OF_DARKNESS_Projectile", pAttackerEffect->GetName() ) ) )
			return;
	}

	CX2DamageEffect::CEffectHandle hChaserEffect = m_vecChaserOfDarknessEffect.front();
	CX2DamageEffect::CEffect* pChaserEffect = g_pX2Game->GetDamageEffect()->GetInstance( hChaserEffect, true );
	if ( NULL == pChaserEffect || NULL == pChaserEffect->GetMainEffect() )
		return;

	const D3DXVECTOR3 vBonePos = pChaserEffect->GetMainEffect()->GetBonePos( L"Dummy02" );
	const bool bMaleReadyEffect = ( 0 == wcscmp( L"DamageEffect_LD_M_CHASER_OF_DARKNESS_Ready", pChaserEffect->GetName() ) );
	const WCHAR* pwszProjectileName = ( true == bMaleReadyEffect ) ? L"DamageEffect_LD_M_CHASER_OF_DARKNESS_Projectile" : L"DamageEffect_LD_F_CHASER_OF_DARKNESS_Projectile";
	const float fPowerRate = GetPowerRate() * m_fChaserOfDarknessPowerRate;

	// char-luciel CH1l3 review: June @32b1fa67 passes -1.0 as the reserve time ( was 1.0 )
	CX2DamageEffect::CEffect* pProjectile = g_pX2Game->GetDamageEffect()->CreateInstance( this, pwszProjectileName, fPowerRate, vBonePos, GetRotateDegree(), GetRotateDegree(),
		m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, 0, -1.f );
	if ( NULL != pProjectile )
	{
		CX2GameUnit* pDefenderUnit = pDamageData_->optrDefenderGameUnit.GetObservable();
		if ( CX2GameUnit::GUT_USER == pDefenderUnit->GetGameUnitType() )
			pProjectile->SetLockOnUnitUID( pDefenderUnit->GetUnitUID() );
		else if ( CX2GameUnit::GUT_NPC == pDefenderUnit->GetGameUnitType() )
			pProjectile->SetLockOnNPCUID( static_cast<int>( pDefenderUnit->GetUnitUID() ) );
	}

	g_pX2Game->GetDamageEffect()->DestroyInstanceHandleSilently( hChaserEffect );
	m_vecChaserOfDarknessEffect.erase( m_vecChaserOfDarknessEffect.begin() );
	m_fChaserOfDarknessFireTimer = CHASER_OF_DARKNESS_FIRE_INTERVAL;
	SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_CHASER_OF_DARKNESS_MEMO, 1, NULL, 0, false );	// 7ai arguments
}

// char-luciel CH1l3: June 32b204d0; ends Summon Darkness: half the unspent time's MP comes back, the summons die, the skill slots refresh
void CX2GULuciel::CancelSummonDarkness()
{
	if ( false == HaveThisBuff( BTI_BUFF_SUMMON_DARKNESS ) )
		return;

	float fRemainTime = 0.f;
	std::vector<CX2BuffFinalizerTempletPtr> vecFinalizer;
	GetBuffFinalizerTempletPtrList( BTI_BUFF_SUMMON_DARKNESS, vecFinalizer );
	for ( std::vector<CX2BuffFinalizerTempletPtr>::iterator vit = vecFinalizer.begin(); vit != vecFinalizer.end(); ++vit )
	{
		const CX2BuffTimeFinalizerTemplet* pTimeFinalizer = dynamic_cast<const CX2BuffTimeFinalizerTemplet*>( vit->get() );
		if ( NULL != pTimeFinalizer )
			fRemainTime = pTimeFinalizer->GetDurationTime();
	}

	// June calls the base's cost ( 31fd1840 ), not this class's: Summon Darkness costs 0 while its buff is on
	const float fMpConsume = CX2GUUser::GetActualMPConsume( GetUpgradeSwappedSkillID( CX2SkillTree::RNSI_SA_LLD_SUMMON_DARKNESS ), 1 );
	UpNowMpWithText( ( fRemainTime / SUMMON_DARKNESS_DURATION ) * ( fMpConsume * SUMMON_DARKNESS_REFUND_RATE ) );

	EraseBuffTempletFromGameUnit( BTI_BUFF_SUMMON_DARKNESS );
	KillMySummonedNPC( CX2UnitManager::NUI_SI_SA_LLD_SUMMON_DARKNESS );
	KillMySummonedNPC( CX2UnitManager::NUI_SI_SA_LLD_SUMMON_DARKNESS_PVP );

	if ( true == IsMyUnit() )
		g_pX2Game->UpdateSkillSlotUI( false );
}

// char-luciel CH1l3: June 32b20780; the Shadow Bolt spear: gone without its buff, a held spear owner blows it up each second, its stack timer plays the spear effect set
void CX2GULuciel::FrameMoveShadowBoltSpear()
{
	if ( false == HaveThisBuff( BTI_BUFF_SHADOW_BOLT ) )
	{
		KillMySummonedNPC( CX2UnitManager::NUI_SI_SA_LCN_SHADOW_BOLT );
		return;
	}

	CX2GUNPC* pSpearNpc = g_pX2Game->GetNPCUnitByUID( m_iShadowBoltNpcUID );
	if ( NULL != pSpearNpc )
	{
		const int iElapsedClock = static_cast<int>( static_cast<float>( clock() - m_clockCreated ) );
		if ( SHADOW_BOLT_SELF_DESTRUCT_INTERVAL < static_cast<double>( iElapsedClock ) / MILLISECONDS_PER_SECOND )
		{
			m_clockCreated = clock();
			if ( true == GetHold() || true == GetGrap() )
				pSpearNpc->StateChange_LUA( "SHOT_SHADOW_BOLT_SELF_DESTRUCTION", true );
		}
	}

	if ( 0.f < m_fShadowBoltTime )
	{
		m_fShadowBoltTime -= m_fElapsedTime;
		if ( m_fShadowBoltTime <= 0.f )
		{
			AddBuffAccumulationCount( BFI_BUFF_SHADOW_BOLT, -1, 1, 0 );

			if ( true == HaveThisBuff( BTI_BUFF_SHADOW_BOLT ) )
			{
				CX2GUNPC* pStackNpc = g_pX2Game->GetNPCUnitByUID( m_iShadowBoltNpcUID );
				if ( NULL != pStackNpc )
				{
					g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SI_SA_LCN_SHADOW_BOLT_Spear", this, pStackNpc, IsHyperState(), m_fShadowBoltDuration, -1.f,
						D3DXVECTOR3( 1.f, 1.f, 1.f ), true, pStackNpc->GetPos( false ) );
				}
			}

			TryGetAcquiredSkillAbilityValue( GetUnit(), CX2SkillTree::SA_COOLTIME_1, CX2SkillTree::RNSI_SA_LCN_SHADOW_BOLT, true, m_fShadowBoltTime );
		}
	}
}

// char-luciel CH1l3: June 32b347f0; a Phantom Rush hit makes the next Phantom Rush ready, pointed at the defender
void CX2GULuciel::ProcessPhantomRushHit( const CX2DamageManager::DamageData* pDamageData_ )
{
	if ( NULL == pDamageData_ || null == pDamageData_->optrDefenderGameUnit || true == m_bPhantomRushReady
		|| CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH != pDamageData_->m_iAppliedSkillID || CX2SkillTree::RNSI_A_LDB_PHANTOM_RUSH != pDamageData_->m_eUsedSkillID )
		return;

	g_pX2Game->GetEffectSet()->StopEffectSet( m_hPhantomRushEffectSet );

	if ( 0 < GetNowCombinationPoint() )
	{
		m_fPhantomRushReadyTime = 1.f;
		m_bPhantomRushReady = true;

		CX2GameUnit* pDefenderUnit = pDamageData_->optrDefenderGameUnit.GetObservable();
		m_PhantomRushTarget.m_eGameUnitType = pDefenderUnit->GetGameUnitType();
		m_PhantomRushTarget.m_uidGameUnit = pDefenderUnit->GetUnitUID();

		if ( true == IsMyUnit() )
			g_pX2Game->UpdateSkillSlotUI( false );

		if ( 0 < GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::RNSI_P_LDN_STABLE_DARKENERGY, false, 0, true ) )
			m_bStableDarkEnergyLocked = true;
	}
}

// char-luciel CH1l3: June 32b349f0; Death Touch ends: the target is forgotten, both effects and the buff go
void CX2GULuciel::ClearDeathTouch()
{
	m_fDeathTouchTime = -1.f;
	m_DeathTouchTarget.m_uidGameUnit = -1;
	m_DeathTouchTarget.m_eGameUnitType = CX2GameUnit::GUT_USER;

	g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hDeathTouchBeamEffect, false );
	g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hDeathTouchTargetEffect, false );
	EraseBuffTempletFromGameUnit( BTI_BUFF_DEATH_TOUCH );
}

// char-luciel CH1l3: June 32b34a90; Death Touch: the beam links Lu's Bip01 to the target while it is in range, otherwise everything ends
void CX2GULuciel::FrameMoveDeathTouchEffect()
{
	if ( m_fDeathTouchTime < 0.f )
		return;

	m_fDeathTouchTime -= m_fElapsedTime;
	if ( 0.f <= m_fDeathTouchTime )
	{
		const CX2GameUnit* pTargetUnit = g_pX2Game->GetUnitByUIDAndType( m_DeathTouchTarget.m_uidGameUnit, m_DeathTouchTarget.m_eGameUnitType );
		if ( NULL != pTargetUnit )
		{
			D3DXVECTOR3 vMyPos = GetBonePos( L"Bip01" );
			vMyPos.y += DEATH_TOUCH_SELF_BONE_OFFSET_Y;

			D3DXVECTOR3 vTargetPos;
			if ( true == pTargetUnit->GetBonePos( L"Bip01", vTargetPos ) )
			{
				vTargetPos.y += DEATH_TOUCH_TARGET_BONE_OFFSET_Y;
			}
			else
			{
				vTargetPos = pTargetUnit->GetPos( false );
				vTargetPos.y += DEATH_TOUCH_TARGET_POSITION_OFFSET_Y;
			}

			if ( GetDistance( vMyPos, vTargetPos ) <= m_fDeathTouchRange )
			{
				CX2DamageEffect::CEffect* pBeamEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_hDeathTouchBeamEffect, true );
				if ( NULL != pBeamEffect )
					pBeamEffect->SetLinkMainEffectByPos( vMyPos, vTargetPos );

				CX2DamageEffect::CEffect* pTargetEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_hDeathTouchTargetEffect, true );
				if ( NULL != pTargetEffect )
					pTargetEffect->SetPos( vTargetPos );

				return;
			}
		}
	}

	ClearDeathTouch();
}

// char-luciel CH1l3: June 32b34c70; a Punisher hit plays its effect set on up to five enemies around the defender ( one on a boss )
void CX2GULuciel::PlayPunisherEffectSet( const CX2DamageManager::DamageData* pDamageData_ )
{
	if ( NULL == pDamageData_ || null == pDamageData_->optrDefenderGameUnit || CX2SkillTree::RNSI_A_LDN_PUNISHER != pDamageData_->m_iAppliedSkillID
		|| CX2DamageManager::EDT_MANA_DAMAGE != pDamageData_->m_ExtraDamage.m_ExtraDamageType || 2 <= pDamageData_->m_RelateSkillData.m_byteSkillIndex )
		return;

	CX2GameUnit* pDefenderUnit = pDamageData_->optrDefenderGameUnit.GetObservable();
	if ( NULL == pDefenderUnit )
		return;

	const std::vector<CX2GameUnit*> vecTargetUnit = g_pX2Game->GetEnemyUnitListInRange( pDefenderUnit, GetTeam(), PUNISHER_RANGE );

	int iEffectCount = PUNISHER_MAX_TARGET_COUNT;
	if ( true == pDefenderUnit->GetIsBosRaidNPC() )
		iEffectCount = 1;
	else
		iEffectCount = min( static_cast<int>( vecTargetUnit.size() ), PUNISHER_MAX_TARGET_COUNT );

	const BYTE byteRelateSkillIndex = pDamageData_->m_RelateSkillData.m_byteSkillIndex;
	const float fEffectRate = ( 0 == byteRelateSkillIndex ) ? PUNISHER_FIRST_HIT_EFFECT_RATE : PUNISHER_LATER_HIT_EFFECT_RATE;

	for ( int iTarget = 0; iTarget < iEffectCount && iTarget < static_cast<int>( vecTargetUnit.size() ); ++iTarget )	// June reads element 0 of an empty list on a boss with no target
	{
		const WCHAR* pwszEffectSetName = L"EffectSet_A_LDN_PUNISHER";
		if ( 0 < GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::RNSI_P_LDN_STABLE_DARKENERGY, false, 0, true ) )
			pwszEffectSetName = L"EffectSet_A_LDN_PUNISHER_TRANSCENDENCE";

		const CX2EffectSet::Handle hEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( pwszEffectSetName, this, vecTargetUnit[iTarget], false, -1.f, -1.f, D3DXVECTOR3( 1.f, 1.f, 1.f ) );
		g_pX2Game->GetEffectSet()->AddDamageEffectModifier( hEffectSet, EFFECT_MODIFIER_TAG_FORCE_DOWN_VALUE, fEffectRate, false );
		g_pX2Game->GetEffectSet()->AddDamageEffectModifier( hEffectSet, EFFECT_MODIFIER_TAG_EXTRA_DAMAGE, fEffectRate, false );
		g_pX2Game->GetEffectSet()->AddDamageEffectModifier( hEffectSet, EFFECT_MODIFIER_TAG_RELATE_SKILL_INDEX, static_cast<float>( byteRelateSkillIndex + 1 ), true );
	}
}

// char-luciel CH1l3: June 32b35600; one more Altar of Evil effect set ( eight at most ) for the form's MP, placed on its ring place
void CX2GULuciel::PlayAltarOfEvilEffectSet()
{
	const int iAltarCount = static_cast<int>( m_AltarOfEvilData.m_vecEffectSet.size() );
	if ( ALTAR_OF_EVIL_MAX_EFFECT_COUNT <= iAltarCount )
		return;

	if ( true != FlushMp( m_AltarOfEvilData.m_fMpCostPerEffectSet, ( true == IsMinorUnitActive() ) ? 1 : 0 ) )
		return;

	const CX2EffectSet::Handle hEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_LDB_ALTAR_OF_EVIL_XXX", this, this, false, -1.f, -1.f, D3DXVECTOR3( 1.f, 1.f, 1.f ) );
	CX2EffectSet::EffectSetInstance* pEffectSetInstance = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hEffectSet );
	if ( NULL == pEffectSetInstance )
		return;

	D3DXVECTOR3 vOffset;
	GetEffectOffsetByIndex( &vOffset, iAltarCount );
	pEffectSetInstance->m_vInstancePositionOffset = vOffset;
	m_AltarOfEvilData.m_vecEffectSet.push_back( hEffectSet );

	if ( CX2GameOption::OL_LOW == g_pMain->GetGameOption().GetOptionList().m_eEffect )	// June 3241d2a0 on g_pMain +0x180 ( the game option ): the effect option is low
	{
		GetEffectOffsetByIndex( &vOffset, iAltarCount );
		const D3DXVECTOR3 vEffectPos = g_pX2Game->GetEffectSet()->GetEffectPosition_LUA( hEffectSet, 0 );
		m_AltarOfEvilData.m_vecPosition.push_back( vOffset + vEffectPos );
	}
}

// char-luciel CH1l3: June 32b35c60; while altars exist: ends the state when the X key is released ( my unit ), otherwise places the next altar every interval
void CX2GULuciel::FrameMoveAltarOfEvilEnd()
{
	if ( true == m_AltarOfEvilData.m_vecEffectSet.empty() )
		return;

	if ( true == IsMyUnit() && false == m_InputData.pureX )
	{
		SetNoDetonationByState( false );
		StateChange( STATE_ID_ALTAR_OF_EVIL_UNNAMED, false );
	}
	else
	{
		SetNoDetonationByState( true );
		m_AltarOfEvilData.m_fElapsedTime += m_fElapsedTime;
		if ( m_AltarOfEvilData.m_fInterval <= m_AltarOfEvilData.m_fElapsedTime )
		{
			m_AltarOfEvilData.m_fElapsedTime = 0.f;
			PlayAltarOfEvilEffectSet();
		}
	}
}

// char-luciel CH1l3: June 32b37090; a Bluff hit plays the effect set of the body in play, the super armor one when the defender has super armor
void CX2GULuciel::PlayBluffEffectSet( const CX2DamageManager::DamageData* pDamageData_ )
{
	if ( NULL == pDamageData_ || null == pDamageData_->optrDefenderGameUnit
		|| CX2SkillTree::RNSI_A_LLD_BLUFF != pDamageData_->m_iAppliedSkillID || CX2SkillTree::RNSI_A_LLD_BLUFF != pDamageData_->m_eUsedSkillID )
		return;

	CX2GameUnit* pDefenderUnit = pDamageData_->optrDefenderGameUnit.GetObservable();
	if ( NULL == pDefenderUnit )
		return;

	const bool bDefenderSuperArmor = pDefenderUnit->IsSuperArmor();
	const WCHAR* pwszEffectSetName = NULL;
	if ( false == IsMinorUnitActive() )
		pwszEffectSetName = ( true == bDefenderSuperArmor ) ? L"EffectSet_A_LLD_Bluff_Lu_Super" : L"EffectSet_A_LLD_Bluff_Lu";
	else
		pwszEffectSetName = ( true == bDefenderSuperArmor ) ? L"EffectSet_A_LLD_Bluff_Ciel_Super" : L"EffectSet_A_LLD_Bluff_Ciel";

	g_pX2Game->GetEffectSet()->PlayEffectSet( pwszEffectSetName, this, NULL, false, -1.f, -1.f, D3DXVECTOR3( 1.f, 1.f, 1.f ) );
}


namespace _CONST_LUCIEL_CH1M_
{
	const float		DIE_STEP_DUST_TIME						= 0.19f;	// June 33475a18
	const float		DIE_SACRIFICE_OF_HERO_TIME				= 1.12f;	// June 33475a1c
}
using namespace _CONST_LUCIEL_CH1M_;

// char-luciel CH1m: June 31fcd0f0 ( a CX2GUUser body only Lu / Ciel register )
void CX2GULuciel::DieFrontStartFuture()
{
	CommonStateStartFuture();
	GetXSkinAnimFuture()->SetPlaySpeed( DIE_PLAY_SPEED );
}

// char-luciel CH1m: June 320008f0
void CX2GULuciel::DieFrontFrameMove()
{
	DieFrameMove( DIE_STEP_DUST_TIME, DIE_SACRIFICE_OF_HERO_TIME );
}

// char-luciel CH1m: June 31fcd160 ( no CommonEventProcess, as June )
void CX2GULuciel::DieFrontEventProcess()
{
	if( false == GetShowObject() && true == GetXSkinAnimFuture()->IsAnimationEnd() )
		StateChange( GUSI_DIE, false );
}

// char-luciel CH1m: June 31fcd1b0 ( identical to 31fcd0f0 )
void CX2GULuciel::DieBackStartFuture()
{
	CommonStateStartFuture();
	GetXSkinAnimFuture()->SetPlaySpeed( DIE_PLAY_SPEED );
}

// char-luciel CH1m: June 32000910 ( identical to 320008f0 )
void CX2GULuciel::DieBackFrameMove()
{
	DieFrameMove( DIE_STEP_DUST_TIME, DIE_SACRIFICE_OF_HERO_TIME );
}

// char-luciel CH1m: June 31fcd220 ( identical to 31fcd160 )
void CX2GULuciel::DieBackEventProcess()
{
	if( false == GetShowObject() && true == GetXSkinAnimFuture()->IsAnimationEnd() )
		StateChange( GUSI_DIE, false );
}

// char-luciel CH1m: June 31fd2680 ( a CX2GUUser body only Lu / Ciel register )
void CX2GULuciel::DashEndStartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x = GetDashSpeed();
}

// char-luciel CH1m: June 32b43d10 ( vslot 0x7a4 )
/*virtual*/ void CX2GULuciel::InitState()
{
	SetSkillCutInSetIndexByUnitClass();	// June @32b43d52-@32b43d9f
	m_ChargeMpState				= LSI_LU_WAIT;	// June +0x7d04 = 0xd
	m_CommonState.m_Wait		= LSI_LU_WAIT;	// June +0x4088 = 0xd
	m_CommonState.m_Thrown		= LSI_LU_DAMAGE_AIR_FLY_FRONT;	// June +0x408c = 0x28

	InitStateCommon();			// June 32b3e8b0

	std::string strStartTableNameUTF8;
	std::string strWinTableNameUTF8;
	std::string strLoseTableNameUTF8;
	InitStateCommonRandom( strStartTableNameUTF8, strWinTableNameUTF8, strLoseTableNameUTF8 );	// June 3202b110 then 31fe22d0

	UserUnitStateData stateData;

	stateData.Init();
	stateData.stateID			= USI_START;
	m_LuaManager.MakeTableReference( strStartTableNameUTF8.c_str(), stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, StartStateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_WIN;
	m_LuaManager.MakeTableReference( strWinTableNameUTF8.c_str(), stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_WIN_StateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RandomLoseEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_LOSE;
	m_LuaManager.MakeTableReference( strLoseTableNameUTF8.c_str(), stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LOSE_StateStart );
	m_StateList[stateData.stateID] = stateData;

	InitWaitStateChange( "LSI_WAIT_STATE_CHANGE" );	// june-state-ids S2: June @32b44174 ( id 0x82 = USI_WAIT_STATE_CHANGE, thunk 329cb720, checklist 7h )

	// June tests the class: only Diablura and Demonio register the bounding target commands and dash combos
	const CX2Unit::UNIT_CLASS eUnitClass = GetUnit()->GetClass();
	if( CX2Unit::UC_LUCIEL_DIABLURA == eUnitClass || CX2Unit::UC_LUCIEL_DEMONIO == eUnitClass )
	{
		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_START;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_START", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_START_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_START_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LOOP;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LOOP", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LOOP_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LANDING;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LANDING", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LANDING_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZ;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZ", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZ_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZ;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZ", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZ_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZX", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_X;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_X", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_X_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_X_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_START;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_START", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_START_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LOOP;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LOOP", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LOOP_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LANDING;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LANDING", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LANDING_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZ;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZ", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZ_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZX", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_Z_START;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_Z_START", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_START_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_START_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_Z_LOOP;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_Z_LOOP", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LOOP_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_Z_END;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_Z_END", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LANDING_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZ;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZ", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZ_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZZ;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZZ", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZ_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZZX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZZX", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZZX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_X;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_X", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_X_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_X_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XX_START;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XX_START", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_START_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XX_LOOP;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XX_LOOP", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LOOP_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XX_LANDING;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XX_LANDING", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LANDING_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XXZ;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XXZ", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZ_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XXZX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XXZX", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_STRIKER;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_STRIKER", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_STRIKER_EventProcess );
		stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_STRIKER_StateEnd );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_STRIKER;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_STRIKER", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_STRIKER_EventProcess );
		stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_STRIKER_StateEnd );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_X;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_X", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XX", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXX", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXX", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXX", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXX", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_X;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_X", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_X_LU_STRIKER;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_X_LU_STRIKER", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XX", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXX", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXX", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXX", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXX", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX;
		m_LuaManager.MakeTableReference( "LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_F_DASH_COMBO_Z;
		m_LuaManager.MakeTableReference( "LSI_LDB_F_DASH_COMBO_Z", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_COMBO_Z_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_F_DASH_COMBO_ZX;
		m_LuaManager.MakeTableReference( "LSI_LDB_F_DASH_COMBO_ZX", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_COMBO_ZX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_F_DASH_COMBO_ZXX;
		m_LuaManager.MakeTableReference( "LSI_LDB_F_DASH_COMBO_ZXX", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_COMBO_ZXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_F_DASH_COMBO_ZXXX;
		m_LuaManager.MakeTableReference( "LSI_LDB_F_DASH_COMBO_ZXXX", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_COMBO_ZXXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_F_DASH_COMBO_ZXXXX;
		m_LuaManager.MakeTableReference( "LSI_LDB_F_DASH_COMBO_ZXXXX", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_COMBO_ZXXXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_F_DASH_COMBO_ZXXXXZ;
		m_LuaManager.MakeTableReference( "LSI_LDB_F_DASH_COMBO_ZXXXXZ", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_COMBO_ZXXXXZ_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_M_DASH_COMBO_ZX;
		m_LuaManager.MakeTableReference( "LSI_LDB_M_DASH_COMBO_ZX", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_COMBO_ZX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_M_DASH_COMBO_ZXX;
		m_LuaManager.MakeTableReference( "LSI_LDB_M_DASH_COMBO_ZXX", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_COMBO_ZXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_M_DASH_COMBO_ZXXX;
		m_LuaManager.MakeTableReference( "LSI_LDB_M_DASH_COMBO_ZXXX", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_COMBO_ZXXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_M_DASH_COMBO_ZXXXX;
		m_LuaManager.MakeTableReference( "LSI_LDB_M_DASH_COMBO_ZXXXX", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_COMBO_ZXXXX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_M_DASH_COMBO_ZXXXXZ;
		m_LuaManager.MakeTableReference( "LSI_LDB_M_DASH_COMBO_ZXXXXZ", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_COMBO_ZXXXXZ_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_F_DASH_JUMP_COMBO_XZ_START;
		m_LuaManager.MakeTableReference( "LSI_LDB_F_DASH_JUMP_COMBO_XZ_START", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_JUMP_COMBO_XZ_START_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING;
		m_LuaManager.MakeTableReference( "LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING_ATTACK;
		m_LuaManager.MakeTableReference( "LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING_ATTACK", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING_ATTACK_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_F_DASH_JUMP_COMBO_XZX;
		m_LuaManager.MakeTableReference( "LSI_LDB_F_DASH_JUMP_COMBO_XZX", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_JUMP_COMBO_XZX_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_M_DASH_JUMP_COMBO_XZ_START;
		m_LuaManager.MakeTableReference( "LSI_LDB_M_DASH_JUMP_COMBO_XZ_START", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_JUMP_COMBO_XZ_START_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_M_DASH_JUMP_COMBO_XZ_LANDING;
		m_LuaManager.MakeTableReference( "LSI_LDB_M_DASH_JUMP_COMBO_XZ_LANDING", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING_StateStartFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_M_DASH_JUMP_COMBO_XZ_LANDING_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_M_DASH_JUMP_COMBO_XZ_LANDING_ATTACK;
		m_LuaManager.MakeTableReference( "LSI_LDB_M_DASH_JUMP_COMBO_XZ_LANDING_ATTACK", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_M_DASH_JUMP_COMBO_XZ_LANDING_ATTACK_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LSI_LDB_M_DASH_JUMP_COMBO_XZX;
		m_LuaManager.MakeTableReference( "LSI_LDB_M_DASH_JUMP_COMBO_XZX", stateData.stateID );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LDB_M_DASH_JUMP_COMBO_XZX_EventProcess );
		m_StateList[stateData.stateID] = stateData;
	}

	InitPassiveSkillState();					// June vslot 0x690 ( 32b45820, CH4a )
	InitEquippedActiveSkillState( false );		// June vslot 0x684
	CX2GUUser::InitState();						// June 31fd8f00
}

// char-luciel CH1m: June 32b3e8b0 ( 294 registrations, no class test )
void CX2GULuciel::InitStateCommon()
{
	UserUnitStateData stateData;
	const std::string strLuHyperModeTable = GetHyperModeTableName( false );	// June 31fdd490( ..., 0 ) @32b3fae6
	const std::string strCielHyperModeTable = GetHyperModeTableName( true );	// June 31fdd490( ..., 1 ) @32b40fdd

	stateData.Init();
	stateData.stateID			= LSI_LU_PEPPER_RUN_READY;
	m_LuaManager.MakeTableReference( "LSI_LU_PEPPER_RUN_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_PEPPER_RUN;
	m_LuaManager.MakeTableReference( "LSI_LU_PEPPER_RUN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_PEPPER_RUN_END;
	m_LuaManager.MakeTableReference( "LSI_LU_PEPPER_RUN_END", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_PEPPER_RUN_JUMP_UP;
	m_LuaManager.MakeTableReference( "LSI_LU_PEPPER_RUN_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_PEPPER_RUN_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "LSI_LU_PEPPER_RUN_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	// June 32b3e8b0 @351 LSI_LU_AMUSEMENT_ITEM_PHOENIX_RUN ( id 123 ) left out: PvP amusement mode, no PvP offline

	// June 32b3e8b0 @397 LSI_LU_AMUSEMENT_ITEM_PHOENIX_RUN_END ( id 126 ) left out: PvP amusement mode, no PvP offline

	// June 32b3e8b0 @444 LSI_LU_AMUSEMENT_ITEM_PHOENIX_RUN ( id 124 ) left out: PvP amusement mode, no PvP offline

	// June 32b3e8b0 @491 LSI_LU_AMUSEMENT_ITEM_PHOENIX_RUN ( id 125 ) left out: PvP amusement mode, no PvP offline

	// June 32b3e8b0 @536 LSI_LU_DANCE_Z ( id 127 ) left out: dance state, Stage 10

	// June 32b3e8b0 @561 LSI_LU_DANCE_X ( id 128 ) left out: dance state, Stage 10

	// June 32b3e8b0 @586 LSI_LU_DANCE_WAIT ( id 129 ) left out: dance state, Stage 10

	InitBoundingStates( "LSI_LU_" );	// june-state-ids S2: June @32b3f302 / @32b3f381 / @32b3f41b ( ids 0x78-0x7a = USI_BOUNDING_*, vslots 0x5d4-0x5e4 ); sets m_bBoundingStatesReady

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_BOUNDING_DOWN;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_BOUNDING_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, BoundingDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_BOUNDING_LANDCRASH;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_BOUNDING_LANDCRASH", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, BoundingLandCrashEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_BOUNDING_UP;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_BOUNDING_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, BoundingUpStateStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, BoundingUpStateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, BoundingUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_THROW_ITEM;
	m_LuaManager.MakeTableReference( "LSI_LU_THROW_ITEM", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMove );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_CameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DIE_FRONT;
	m_LuaManager.MakeTableReference( "LSI_LU_DIE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LU_DIE_FRONT_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, DieFrontEventProcess );
	// June 3202a7d0 ( StateEnd ): runs in CX2GUUser::StateChange for the die states
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DIE_BACK;
	m_LuaManager.MakeTableReference( "LSI_LU_DIE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, DieBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LU_DIE_BACK_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, DieBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, DieBackEventProcess );
	// June 3202a7d0 ( StateEnd ): runs in CX2GUUser::StateChange for the die states
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_START_INTRUDE;
	m_LuaManager.MakeTableReference( "LSI_START_INTRUDE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, StartIntrudeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, StartIntrudeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartIntrudeEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_WAIT;
	m_LuaManager.MakeTableReference( "LSI_LU_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LU_WAIT_StateStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LU_WAIT_StateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_WAIT_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_WALK;
	m_LuaManager.MakeTableReference( "LSI_LU_WALK", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_WALK_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_JUMP_READY;
	m_LuaManager.MakeTableReference( "LSI_LU_JUMP_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_JUMP_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_JUMP_UP;
	m_LuaManager.MakeTableReference( "LSI_LU_JUMP_UP", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_JUMP_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "LSI_LU_JUMP_DOWN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_JUMP_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "LSI_LU_JUMP_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_JUMP_LANDING_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_END;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, DashEndStartFuture );
	// June 31fc7fc0 ( StateStart ): = CommonStateStart, which an empty slot runs
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_JUMP;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_EventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, DashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_LANDING_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_JUMP_POWER_LANDING;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_JUMP_POWER_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_POWER_LANDING_StateStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_POWER_LANDING_StateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_POWER_LANDING_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_HYPER_MODE;
	m_LuaManager.MakeTableReference( strLuHyperModeTable.c_str(), stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, HyperModeStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, HyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, HyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, HyperModeEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, HyperModeEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_GROGGY;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_GROGGY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DAMAGE_GROGGY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_SMALL_FRONT;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_SMALL_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_SMALL_BACK;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_SMALL_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_BIG_FRONT;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_BIG_FRONT", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageBigStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageBigEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_BIG_BACK;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_BIG_BACK", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageBigStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageBigEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_DOWN_FRONT;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_DOWN_FRONT", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LU_DAMAGE_DOWN_FRONT_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DAMAGE_DOWN_FRONT_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_DOWN_BACK;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_DOWN_BACK", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LU_DAMAGE_DOWN_BACK_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DAMAGE_DOWN_BACK_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_STANDUP_FRONT;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_STANDUP_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_STANDUP_BACK;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_STANDUP_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_STANDUP_ROLLING_FRONT_FRONT;
	m_LuaManager.MakeTableReference( "LSI_LU_STANDUP_ROLLING_FRONT_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ROLLING_FRONT_FRONT_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ROLLING_FRONT_FRONT_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_STANDUP_ROLLING_FRONT_BACK;
	m_LuaManager.MakeTableReference( "LSI_LU_STANDUP_ROLLING_FRONT_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ROLLING_FRONT_BACK_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ROLLING_FRONT_BACK_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_STANDUP_ROLLING_BACK_FRONT;
	m_LuaManager.MakeTableReference( "LSI_LU_STANDUP_ROLLING_BACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ROLLING_BACK_FRONT_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ROLLING_BACK_FRONT_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_STANDUP_ROLLING_BACK_BACK;
	m_LuaManager.MakeTableReference( "LSI_LU_STANDUP_ROLLING_BACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ROLLING_BACK_BACK_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ROLLING_BACK_BACK_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_STANDUP_ATTACK_FRONT;
	m_LuaManager.MakeTableReference( "LSI_LU_STANDUP_ATTACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ATTACK_FRONT_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ATTACK_FRONT_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_STANDUP_ATTACK_BACK;
	m_LuaManager.MakeTableReference( "LSI_LU_STANDUP_ATTACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ATTACK_BACK_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ATTACK_BACK_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_STANDUP_ATTACK_FRONT_NO_DOWN;
	m_LuaManager.MakeTableReference( "LSI_LU_STANDUP_ATTACK_FRONT_NO_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ATTACK_FRONT_NO_DOWN_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ATTACK_FRONT_NO_DOWN_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_STANDUP_ATTACK_BACK_NO_DOWN;
	m_LuaManager.MakeTableReference( "LSI_LU_STANDUP_ATTACK_BACK_NO_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ATTACK_BACK_NO_DOWN_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_LU_STANDUP_ATTACK_BACK_NO_DOWN_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_AIR_SMALL;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_AIR_SMALL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DamageAirSmallStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_AIR_DOWN;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_AIR_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_AIR_DOWN_INVINCIBLE;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_AIR_DOWN_INVINCIBLE", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownInvincibleEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_AIR_DOWN_LANDING;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_AIR_DOWN_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DAMAGE_DOWN_FRONT_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_AIR_FALL;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_AIR_FALL", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_AIR_UP;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_AIR_UP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_AIR_FLY_FRONT;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_AIR_FLY_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_AIR_FLY_BACK;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_AIR_FLY_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DAMAGE_REVENGE;
	m_LuaManager.MakeTableReference( "LSI_LU_DAMAGE_REVENGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageRevengeStart );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, DamageRevengeCameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageRevengeEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_SWITCHING;
	m_LuaManager.MakeTableReference( "LSI_LU_SWITCHING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LU_SWITCHING_StateStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericGroundComboEndEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_COMBO_Z;
	m_LuaManager.MakeTableReference( "LSI_LU_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "LSI_LU_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_ZZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_COMBO_ZZZ;
	m_LuaManager.MakeTableReference( "LSI_LU_COMBO_ZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_ZZZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_COMBO_ZZZZ;
	m_LuaManager.MakeTableReference( "LSI_LU_COMBO_ZZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_ZZZZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_COMBO_ZZZZX;
	m_LuaManager.MakeTableReference( "LSI_LU_COMBO_ZZZZX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_ZZZZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_COMBO_X;
	m_LuaManager.MakeTableReference( "LSI_LU_COMBO_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_COMBO_XX;
	m_LuaManager.MakeTableReference( "LSI_LU_COMBO_XX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_XX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_COMBO_XXX;
	m_LuaManager.MakeTableReference( "LSI_LU_COMBO_XXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_XXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_COMBO_XXXX;
	m_LuaManager.MakeTableReference( "LSI_LU_COMBO_XXXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_XXXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_COMBO_XXXZ_READY;
	m_LuaManager.MakeTableReference( "LSI_LU_COMBO_XXXZ_READY", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_XXXZ_READY_StateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_XXXZ_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_COMBO_XXXZ_CHARGE;
	m_LuaManager.MakeTableReference( "LSI_LU_COMBO_XXXZ_CHARGE", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_XXXZ_CHARGE_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_XXXZ_CHARGE_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_COMBO_XXXZ_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LU_COMBO_XXXZ_LOOP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_XXXZ_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_COMBO_XXXZ_END;
	m_LuaManager.MakeTableReference( "LSI_LU_COMBO_XXXZ_END", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_XXXZ_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_XXXZ_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_COMBO_XXXZX;
	m_LuaManager.MakeTableReference( "LSI_LU_COMBO_XXXZX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_COMBO_XXXZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_COMBO_Z;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_COMBO_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_COMBO_X_START;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_COMBO_X_START", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_COMBO_X_START_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_COMBO_X_LANDING;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_COMBO_X_LANDING", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_COMBO_X_LANDING_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_JUMP_COMBO_Z;
	m_LuaManager.MakeTableReference( "LSI_LU_JUMP_COMBO_Z", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LU_JUMP_COMBO_Z_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_JUMP_COMBO_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_JUMP_COMBO_X;
	m_LuaManager.MakeTableReference( "LSI_LU_JUMP_COMBO_X", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LU_JUMP_COMBO_X_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_JUMP_COMBO_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_JUMP_COMBO_Z;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_JUMP_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_COMBO_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_JUMP_COMBO_ZX;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_JUMP_COMBO_ZX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_COMBO_ZX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_COMBO_ZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_JUMP_COMBO_X;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_JUMP_COMBO_X", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_COMBO_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_JUMP_COMBO_XX_START;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_JUMP_COMBO_XX_START", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_COMBO_XX_START_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_COMBO_XX_START_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_JUMP_COMBO_XX_END;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_JUMP_COMBO_XX_END", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_COMBO_XX_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_JUMP_COMBO_XXX;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_JUMP_COMBO_XXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_COMBO_XXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_JUMP_COMBO_XXXX;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_JUMP_COMBO_XXXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_COMBO_XXXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_DASH_JUMP_COMBO_XXXXZ;
	m_LuaManager.MakeTableReference( "LSI_LU_DASH_JUMP_COMBO_XXXXZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_COMBO_XXXXZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_COMBO_ZZZX;
	m_LuaManager.MakeTableReference( "LSI_LLC_COMBO_ZZZX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LLC_COMBO_ZZZX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_COMBO_ZZZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_DASH_COMBO_Z;
	m_LuaManager.MakeTableReference( "LSI_LLC_DASH_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_DASH_COMBO_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_DASH_COMBO_ZX;
	m_LuaManager.MakeTableReference( "LSI_LLC_DASH_COMBO_ZX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_DASH_COMBO_ZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_DASH_COMBO_ZXX;
	m_LuaManager.MakeTableReference( "LSI_LLC_DASH_COMBO_ZXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_DASH_COMBO_ZXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_DASH_COMBO_XX;
	m_LuaManager.MakeTableReference( "LSI_LLC_DASH_COMBO_XX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_DASH_COMBO_XX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH;
	m_LuaManager.MakeTableReference( "LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_UP;
	m_LuaManager.MakeTableReference( "LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_UP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_DOWN;
	m_LuaManager.MakeTableReference( "LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_F_DASH_JUMP_COMBO_X_DASH;
	m_LuaManager.MakeTableReference( "LSI_LLC_F_DASH_JUMP_COMBO_X_DASH", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_UP;
	m_LuaManager.MakeTableReference( "LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_UP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_DOWN;
	m_LuaManager.MakeTableReference( "LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LU_JUMP_DOWN_NO_ATTACK;
	m_LuaManager.MakeTableReference( "LSI_LU_JUMP_DOWN_NO_ATTACK", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LU_JUMP_DOWN_NO_ATTACK_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_JUMP_DOWN_NO_ATTACK_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLD_COMBO_ZZZXZ;
	m_LuaManager.MakeTableReference( "LSI_LLD_COMBO_ZZZXZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLD_COMBO_ZZZXZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLD_COMBO_XX_DOWN_X;
	m_LuaManager.MakeTableReference( "LSI_LLD_COMBO_XX_DOWN_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLD_COMBO_XX_DOWN_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLD_COMBO_XX_DOWN_XX_READY;
	m_LuaManager.MakeTableReference( "LSI_LLD_COMBO_XX_DOWN_XX_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLD_COMBO_XX_DOWN_XX_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLD_COMBO_XX_DOWN_XX_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LLD_COMBO_XX_DOWN_XX_LOOP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LLD_COMBO_XX_DOWN_XX_LOOP_StateStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLD_COMBO_XX_DOWN_XX_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLD_COMBO_XX_DOWN_XX_LOOP_X;
	m_LuaManager.MakeTableReference( "LSI_LLD_COMBO_XX_DOWN_XX_LOOP_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLD_COMBO_XX_DOWN_XX_LOOP_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_PEPPER_RUN_READY;
	m_LuaManager.MakeTableReference( "LSI_CIEL_PEPPER_RUN_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_PEPPER_RUN;
	m_LuaManager.MakeTableReference( "LSI_CIEL_PEPPER_RUN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_PEPPER_RUN_END;
	m_LuaManager.MakeTableReference( "LSI_CIEL_PEPPER_RUN_END", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_PEPPER_RUN_JUMP_UP;
	m_LuaManager.MakeTableReference( "LSI_CIEL_PEPPER_RUN_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_PEPPER_RUN_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "LSI_CIEL_PEPPER_RUN_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	// June 32b3e8b0 @1129 LSI_CIEL_AMUSEMENT_ITEM_PHOENIX_RUN ( id 571 ) left out: PvP amusement mode, no PvP offline

	// June 32b3e8b0 @1133 LSI_CIEL_AMUSEMENT_ITEM_PHOENIX_RUN_END ( id 574 ) left out: PvP amusement mode, no PvP offline

	// June 32b3e8b0 @1138 LSI_CIEL_AMUSEMENT_ITEM_PHOENIX_RUN ( id 572 ) left out: PvP amusement mode, no PvP offline

	// June 32b3e8b0 @1143 LSI_CIEL_AMUSEMENT_ITEM_PHOENIX_RUN ( id 573 ) left out: PvP amusement mode, no PvP offline

	// June 32b3e8b0 @1146 LSI_CIEL_DANCE_Z ( id 590 ) left out: dance state, Stage 10

	// June 32b3e8b0 @1149 LSI_CIEL_DANCE_X ( id 591 ) left out: dance state, Stage 10

	// June 32b3e8b0 @1152 LSI_CIEL_DANCE_WAIT ( id 592 ) left out: dance state, Stage 10

	stateData.Init();
	stateData.stateID			= LSI_CIEL_THROW_ITEM;
	m_LuaManager.MakeTableReference( "LSI_CIEL_THROW_ITEM", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMove );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_CameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DIE_FRONT;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DIE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, DieFrontEventProcess );
	// June 3202a7d0 ( StateEnd ): runs in CX2GUUser::StateChange for the die states
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DIE_BACK;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DIE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, DieBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, DieBackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, DieBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, DieBackEventProcess );
	// June 3202a7d0 ( StateEnd ): runs in CX2GUUser::StateChange for the die states
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_WAIT;
	m_LuaManager.MakeTableReference( "LSI_CIEL_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LU_WAIT_StateStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LU_WAIT_StateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_WAIT_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_WALK;
	m_LuaManager.MakeTableReference( "LSI_CIEL_WALK", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_WALK_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_JUMP_READY;
	m_LuaManager.MakeTableReference( "LSI_CIEL_JUMP_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_JUMP_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_JUMP_UP;
	m_LuaManager.MakeTableReference( "LSI_CIEL_JUMP_UP", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_JUMP_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "LSI_CIEL_JUMP_DOWN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_JUMP_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "LSI_CIEL_JUMP_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_JUMP_LANDING_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH_END;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, DashEndStartFuture );
	// June 31fc7fc0 ( StateStart ): = CommonStateStart, which an empty slot runs
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH_JUMP;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_EventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, DashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_LANDING_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH_JUMP_POWER_LANDING;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH_JUMP_POWER_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_POWER_LANDING_StateStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LU_DASH_JUMP_POWER_LANDING_StateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_POWER_LANDING_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_HYPER_MODE;
	m_LuaManager.MakeTableReference( strCielHyperModeTable.c_str(), stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, HyperModeStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, HyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, HyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, HyperModeEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, HyperModeEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_GROGGY;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_GROGGY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DAMAGE_GROGGY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_SMALL_FRONT;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_SMALL_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_SMALL_BACK;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_SMALL_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_BIG_FRONT;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_BIG_FRONT", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageBigStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageBigEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_BIG_BACK;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_BIG_BACK", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageBigStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageBigEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_DOWN_FRONT;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_DOWN_FRONT", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DAMAGE_DOWN_FRONT_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DAMAGE_DOWN_FRONT_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_DOWN_BACK;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_DOWN_BACK", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DAMAGE_DOWN_BACK_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DAMAGE_DOWN_BACK_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_STANDUP_FRONT;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_STANDUP_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_STANDUP_BACK;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_STANDUP_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_STANDUP_ROLLING_FRONT_FRONT;
	m_LuaManager.MakeTableReference( "LSI_CIEL_STANDUP_ROLLING_FRONT_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_STANDUP_ROLLING_FRONT_FRONT_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_STANDUP_ROLLING_FRONT_FRONT_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_STANDUP_ROLLING_FRONT_BACK;
	m_LuaManager.MakeTableReference( "LSI_CIEL_STANDUP_ROLLING_FRONT_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_STANDUP_ROLLING_FRONT_BACK_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_STANDUP_ROLLING_FRONT_BACK_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_STANDUP_ROLLING_BACK_FRONT;
	m_LuaManager.MakeTableReference( "LSI_CIEL_STANDUP_ROLLING_BACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_STANDUP_ROLLING_BACK_FRONT_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_STANDUP_ROLLING_BACK_FRONT_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_STANDUP_ROLLING_BACK_BACK;
	m_LuaManager.MakeTableReference( "LSI_CIEL_STANDUP_ROLLING_BACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_STANDUP_ROLLING_BACK_BACK_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_STANDUP_ROLLING_BACK_BACK_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_STANDUP_ATTACK_FRONT;
	m_LuaManager.MakeTableReference( "LSI_CIEL_STANDUP_ATTACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_STANDUP_ATTACK_FRONT_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_STANDUP_ATTACK_FRONT_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_STANDUP_ATTACK_BACK;
	m_LuaManager.MakeTableReference( "LSI_CIEL_STANDUP_ATTACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_STANDUP_ATTACK_BACK_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_STANDUP_ATTACK_BACK_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_AIR_SMALL;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_AIR_SMALL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DamageAirSmallStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_AIR_DOWN;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_AIR_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_AIR_DOWN_INVINCIBLE;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_AIR_DOWN_INVINCIBLE", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownInvincibleEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_AIR_DOWN_LANDING;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_AIR_DOWN_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DAMAGE_DOWN_FRONT_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_AIR_FALL;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_AIR_FALL", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_AIR_UP;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_AIR_UP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_AIR_FLY_FRONT;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_AIR_FLY_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_AIR_FLY_BACK;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_AIR_FLY_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DAMAGE_REVENGE;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DAMAGE_REVENGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageRevengeStart );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, DamageRevengeCameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageRevengeEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_SWITCHING;
	m_LuaManager.MakeTableReference( "LSI_CIEL_SWITCHING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LU_SWITCHING_StateStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericGroundComboEndEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_COMBO_Z;
	m_LuaManager.MakeTableReference( "LSI_CIEL_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "LSI_CIEL_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_ZZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_COMBO_ZZZ;
	m_LuaManager.MakeTableReference( "LSI_CIEL_COMBO_ZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_ZZZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_COMBO_ZZZZ;
	m_LuaManager.MakeTableReference( "LSI_CIEL_COMBO_ZZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_ZZZZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_COMBO_ZZZZX;
	m_LuaManager.MakeTableReference( "LSI_CIEL_COMBO_ZZZZX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_ZZZZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_COMBO_X;
	m_LuaManager.MakeTableReference( "LSI_CIEL_COMBO_X", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_COMBO_XX;
	m_LuaManager.MakeTableReference( "LSI_CIEL_COMBO_XX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_XX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_XX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_COMBO_XXX;
	m_LuaManager.MakeTableReference( "LSI_CIEL_COMBO_XXX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_XXX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_XXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_COMBO_XXXX;
	m_LuaManager.MakeTableReference( "LSI_CIEL_COMBO_XXXX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_XXXX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_XXXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_COMBO_XXXZ_START;
	m_LuaManager.MakeTableReference( "LSI_CIEL_COMBO_XXXZ_START", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_XXXZ_START_StateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_XXXZ_START_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_COMBO_XXXZ_LOOP;
	m_LuaManager.MakeTableReference( "LSI_CIEL_COMBO_XXXZ_LOOP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_XXXZ_LOOP_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_XXXZ_LOOP_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_COMBO_XXXZ_END;
	m_LuaManager.MakeTableReference( "LSI_CIEL_COMBO_XXXZ_END", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_XXXZ_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_XXXZ_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_COMBO_XXXZX;
	m_LuaManager.MakeTableReference( "LSI_CIEL_COMBO_XXXZX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_COMBO_XXXZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH_COMBO_Z;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_COMBO_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH_COMBO_X;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH_COMBO_X", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_COMBO_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_COMBO_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_JUMP_COMBO_Z;
	m_LuaManager.MakeTableReference( "LSI_CIEL_JUMP_COMBO_Z", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_JUMP_COMBO_Z_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_JUMP_COMBO_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_JUMP_COMBO_X;
	m_LuaManager.MakeTableReference( "LSI_CIEL_JUMP_COMBO_X", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_JUMP_COMBO_X_FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_JUMP_COMBO_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_JUMP_COMBO_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH_JUMP_COMBO_Z;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH_JUMP_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_COMBO_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH_JUMP_COMBO_ZX;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH_JUMP_COMBO_ZX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_COMBO_ZX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_COMBO_ZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH_JUMP_COMBO_X;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH_JUMP_COMBO_X", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_COMBO_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH_JUMP_COMBO_XX;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH_JUMP_COMBO_XX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_COMBO_XX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_COMBO_XX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH_JUMP_COMBO_XXX;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH_JUMP_COMBO_XXX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_COMBO_XXX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_COMBO_XXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH_JUMP_COMBO_XXXX;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH_JUMP_COMBO_XXXX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_COMBO_XXXX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_COMBO_XXXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH_JUMP_COMBO_XXXXZ;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH_JUMP_COMBO_XXXXZ", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_COMBO_XXXXZ_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_COMBO_XXXXZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_DASH_JUMP_COMBO_XXXXZ_LANDING;
	m_LuaManager.MakeTableReference( "LSI_CIEL_DASH_JUMP_COMBO_XXXXZ_LANDING", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_DASH_JUMP_COMBO_XXXXZ_LANDING_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH;
	m_LuaManager.MakeTableReference( "LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_UP;
	m_LuaManager.MakeTableReference( "LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_UP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_DOWN;
	m_LuaManager.MakeTableReference( "LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_M_DASH_JUMP_COMBO_X_DASH;
	m_LuaManager.MakeTableReference( "LSI_LLC_M_DASH_JUMP_COMBO_X_DASH", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_UP;
	m_LuaManager.MakeTableReference( "LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_UP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_DOWN;
	m_LuaManager.MakeTableReference( "LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_CIEL_JUMP_DOWN_NO_ATTACK;
	m_LuaManager.MakeTableReference( "LSI_CIEL_JUMP_DOWN_NO_ATTACK", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_JUMP_DOWN_NO_ATTACK_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_CIEL_JUMP_DOWN_NO_ATTACK_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_COMBO_ZZZX;
	m_LuaManager.MakeTableReference( "LSI_LCG_COMBO_ZZZX", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_ZZZX_StateStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_ZZZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_COMBO_XXXX;
	m_LuaManager.MakeTableReference( "LSI_LCG_COMBO_XXXX", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_ZZZX_StateStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_ZZZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_DASH_COMBO_ZX;
	m_LuaManager.MakeTableReference( "LSI_LCG_DASH_COMBO_ZX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_DASH_COMBO_ZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_DASH_COMBO_ZXX;
	m_LuaManager.MakeTableReference( "LSI_LCG_DASH_COMBO_ZXX", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_ZZZX_StateStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_ZZZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_COMBO_SNIPING_READY;
	m_LuaManager.MakeTableReference( "LSI_LCG_COMBO_SNIPING_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_SNIPING_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_COMBO_SNIPING_Z;
	m_LuaManager.MakeTableReference( "LSI_LCG_COMBO_SNIPING_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_SNIPING_Z_StateStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_SNIPING_Z_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_SNIPING_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_COMBO_SNIPING_Z_FINISH;
	m_LuaManager.MakeTableReference( "LSI_LCG_COMBO_SNIPING_Z_FINISH", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_SNIPING_Z_FINISH_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_COMBO_SNIPING_X_CHARGE;
	m_LuaManager.MakeTableReference( "LSI_LCG_COMBO_SNIPING_X_CHARGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_SNIPING_X_CHARGE_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_SNIPING_X_CHARGE_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_SNIPING_X_CHARGE_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_COMBO_SNIPING_X_FIRE;
	m_LuaManager.MakeTableReference( "LSI_LCG_COMBO_SNIPING_X_FIRE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_SNIPING_X_FIRE_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_SNIPING_X_FIRE_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_COMBO_SNIPING_X_FIRE_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_DASH_JUMP_COMBO_SNIPING_START;
	m_LuaManager.MakeTableReference( "LSI_LCG_DASH_JUMP_COMBO_SNIPING_START", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_DASH_JUMP_COMBO_SNIPING_START_StateStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_DASH_JUMP_COMBO_SNIPING_START_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_DASH_JUMP_COMBO_SNIPING_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LCG_DASH_JUMP_COMBO_SNIPING_LOOP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_DASH_JUMP_COMBO_SNIPING_LOOP_StateStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_DASH_JUMP_COMBO_SNIPING_LOOP_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_DASH_JUMP_COMBO_SNIPING_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_DASH_JUMP_COMBO_SNIPING_FINISH;
	m_LuaManager.MakeTableReference( "LSI_LCG_DASH_JUMP_COMBO_SNIPING_FINISH", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_DASH_JUMP_COMBO_SNIPING_FINISH_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_DASH_COMBO_ZX_DOWN_X;
	m_LuaManager.MakeTableReference( "LSI_LCN_DASH_COMBO_ZX_DOWN_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_DASH_COMBO_ZX_DOWN_X_StateStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_DASH_COMBO_ZX_DOWN_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_DASH_COMBO_ZX_DOWN_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_DASH_COMBO_ZX_DOWN_XX_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LCN_DASH_COMBO_ZX_DOWN_XX_LOOP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_DASH_COMBO_ZX_DOWN_XX_LOOP_StateStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_DASH_COMBO_ZX_DOWN_XX_LOOP_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_DASH_COMBO_ZX_DOWN_XX_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_DASH_COMBO_ZX_DOWN_XX_END;
	m_LuaManager.MakeTableReference( "LSI_LCN_DASH_COMBO_ZX_DOWN_XX_END", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_DASH_COMBO_ZX_DOWN_XX_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_SNIPING_COMBO_UP_X;
	m_LuaManager.MakeTableReference( "LSI_LCN_SNIPING_COMBO_UP_X", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_SNIPING_COMBO_UP_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_SNIPING_COMBO_UP_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_SNIPING_COMBO_DOWN_X;
	m_LuaManager.MakeTableReference( "LSI_LCN_SNIPING_COMBO_DOWN_X", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_SNIPING_COMBO_DOWN_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_SNIPING_COMBO_DOWN_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_F_DASH_COMBO_ZXXX;
	m_LuaManager.MakeTableReference( "LSI_LLC_F_DASH_COMBO_ZXXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_F_DASH_COMBO_ZXXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_F_DASH_COMBO_XXX;
	m_LuaManager.MakeTableReference( "LSI_LLC_F_DASH_COMBO_XXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_F_DASH_COMBO_XXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_M_COMBO_ZZZX;
	m_LuaManager.MakeTableReference( "LSI_LLC_M_COMBO_ZZZX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LLC_M_COMBO_ZZZX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_M_COMBO_ZZZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_M_DASH_COMBO_ZX;
	m_LuaManager.MakeTableReference( "LSI_LLC_M_DASH_COMBO_ZX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_M_DASH_COMBO_ZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_M_DASH_COMBO_ZXX;
	m_LuaManager.MakeTableReference( "LSI_LLC_M_DASH_COMBO_ZXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_M_DASH_COMBO_ZXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_M_DASH_COMBO_ZXXX;
	m_LuaManager.MakeTableReference( "LSI_LLC_M_DASH_COMBO_ZXXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_M_DASH_COMBO_ZXXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_M_DASH_COMBO_XX;
	m_LuaManager.MakeTableReference( "LSI_LLC_M_DASH_COMBO_XX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_M_DASH_COMBO_XX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLC_M_DASH_COMBO_XXX;
	m_LuaManager.MakeTableReference( "LSI_LLC_M_DASH_COMBO_XXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLC_M_DASH_COMBO_XXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_COMBO_ZZZX;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_COMBO_ZZZX", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZX_StateStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_COMBO_ZZZX_WAIT;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_COMBO_ZZZX_WAIT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZX_WAIT_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_COMBO_ZZZXX_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_COMBO_ZZZXX_LOOP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_LOOP_StateStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_LOOP_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_COMBO_ZZZXX_END;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_COMBO_ZZZXX_END", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_END_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_COMBO_ZZZXZ_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_COMBO_ZZZXZ_LOOP", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_LOOP_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_LOOP_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_COMBO_ZZZXZ_END;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_COMBO_ZZZXZ_END", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_COMBO_XXXX;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_COMBO_XXXX", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZX_StateStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_COMBO_XXXXX_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_COMBO_XXXXX_LOOP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_LOOP_StateStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_LOOP_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_XXXXX_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_COMBO_XXXXX_END;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_COMBO_XXXXX_END", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_END_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_COMBO_XXXXZ_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_COMBO_XXXXZ_LOOP", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_LOOP_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_LOOP_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_COMBO_XXXXZ_END;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_COMBO_XXXXZ_END", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_COMBO_Z;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_DASH_COMBO_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_COMBO_ZX;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_COMBO_ZX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_DASH_COMBO_ZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_COMBO_ZXX;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_COMBO_ZXX", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZX_StateStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_COMBO_ZXXZ_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_COMBO_ZXXZ_LOOP", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_LOOP_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_LOOP_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_COMBO_ZXXZ_END;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_COMBO_ZXXZ_END", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_COMBO_ZXXX_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_COMBO_ZXXX_LOOP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_LOOP_StateStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_LOOP_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_DASH_COMBO_ZXXX_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_COMBO_ZXXX_END;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_COMBO_ZXXX_END", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_END_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_COMBO_XX;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_COMBO_XX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_DASH_COMBO_XX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_COMBO_XXX;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_COMBO_XXX", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZX_StateStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_COMBO_ZXXZ_END;	// June registers table LSI_LCG_F_DASH_COMBO_XXXZ_END at this id
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_COMBO_XXXZ_END", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_COMBO_XXXX_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_COMBO_XXXX_LOOP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_LOOP_StateStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_LOOP_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_DASH_COMBO_XXXX_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_COMBO_XXXX_END;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_COMBO_XXXX_END", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_END_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXX_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_COMBO_XXXZ_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_COMBO_XXXZ_LOOP", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_LOOP_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_LOOP_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_COMBO_XXXZ_END;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_COMBO_XXXZ_END", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_COMBO_ZZZXZ_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_JUMP_COMBO_ZXX;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_JUMP_COMBO_ZXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_DASH_JUMP_COMBO_ZXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_M_DASH_JUMP_COMBO_ZXX;
	m_LuaManager.MakeTableReference( "LSI_LCG_M_DASH_JUMP_COMBO_ZXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_M_DASH_JUMP_COMBO_ZXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= 338;	// no name: June registers table LSI_LU_JUMP_COMBO_Z again
	m_LuaManager.MakeTableReference( "LSI_LU_JUMP_COMBO_Z", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LU_JUMP_COMBO_Z_SECOND_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LU_JUMP_COMBO_Z_SECOND_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_JUMP_COMBO_XZZ_READY;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_JUMP_COMBO_XZZ_READY", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_DASH_JUMP_COMBO_XZZ_READY_StateStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_DASH_JUMP_COMBO_XZZ_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_JUMP_COMBO_XZZ_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_JUMP_COMBO_XZZ_LOOP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_DASH_JUMP_COMBO_XZZ_LOOP_StateStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_DASH_JUMP_COMBO_XZZ_LOOP_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_DASH_JUMP_COMBO_XZZ_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCG_F_DASH_JUMP_COMBO_XZZ_END;
	m_LuaManager.MakeTableReference( "LSI_LCG_F_DASH_JUMP_COMBO_XZZ_END", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCG_F_DASH_JUMP_COMBO_XZZ_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLD_M_COMBO_ZZZXZ;
	m_LuaManager.MakeTableReference( "LSI_LLD_M_COMBO_ZZZXZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLD_M_COMBO_ZZZXZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLD_M_COMBO_XX_DOWN_X;
	m_LuaManager.MakeTableReference( "LSI_LLD_M_COMBO_XX_DOWN_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLD_M_COMBO_XX_DOWN_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLD_M_COMBO_XX_DOWN_XX_READY;
	m_LuaManager.MakeTableReference( "LSI_LLD_M_COMBO_XX_DOWN_XX_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLD_M_COMBO_XX_DOWN_XX_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLD_M_COMBO_XX_DOWN_XX_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LLD_M_COMBO_XX_DOWN_XX_LOOP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LLD_M_COMBO_XX_DOWN_XX_LOOP_StateStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLD_M_COMBO_XX_DOWN_XX_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LLD_M_COMBO_XX_DOWN_XX_END;
	m_LuaManager.MakeTableReference( "LSI_LLD_M_COMBO_XX_DOWN_XX_END", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LLD_M_COMBO_XX_DOWN_XX_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_F_COMBO_ZZZX_UP_X;
	m_LuaManager.MakeTableReference( "LSI_LCN_F_COMBO_ZZZX_UP_X", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_COMBO_ZZZX_UP_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_COMBO_ZZZX_UP_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_F_COMBO_ZZZX_DOWN_X;
	m_LuaManager.MakeTableReference( "LSI_LCN_F_COMBO_ZZZX_DOWN_X", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_COMBO_ZZZX_DOWN_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_COMBO_ZZZX_DOWN_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_F_COMBO_XXXX_UP_X;
	m_LuaManager.MakeTableReference( "LSI_LCN_F_COMBO_XXXX_UP_X", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_COMBO_ZZZX_UP_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_COMBO_XXXX_UP_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_F_COMBO_XXXX_DOWN_X;
	m_LuaManager.MakeTableReference( "LSI_LCN_F_COMBO_XXXX_DOWN_X", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_COMBO_ZZZX_DOWN_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_COMBO_XXXX_DOWN_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_F_DASH_COMBO_ZXX_UP_X;
	m_LuaManager.MakeTableReference( "LSI_LCN_F_DASH_COMBO_ZXX_UP_X", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_COMBO_ZZZX_UP_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_ZXX_UP_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_F_DASH_COMBO_ZXX_DOWN_X;
	m_LuaManager.MakeTableReference( "LSI_LCN_F_DASH_COMBO_ZXX_DOWN_X", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_COMBO_ZZZX_DOWN_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_ZXX_DOWN_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_F_DASH_COMBO_XXX_UP_X;
	m_LuaManager.MakeTableReference( "LSI_LCN_F_DASH_COMBO_XXX_UP_X", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_COMBO_ZZZX_UP_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_XXX_UP_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_F_DASH_COMBO_XXX_DOWN_X;
	m_LuaManager.MakeTableReference( "LSI_LCN_F_DASH_COMBO_XXX_DOWN_X", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_COMBO_ZZZX_DOWN_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_XXX_DOWN_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_F_DASH_COMBO_ZX_DOWN_X;
	m_LuaManager.MakeTableReference( "LSI_LCN_F_DASH_COMBO_ZX_DOWN_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_ZX_DOWN_X_StateStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_ZX_DOWN_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_ZX_DOWN_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_LOOP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_LOOP_StateStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_LOOP_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_END;
	m_LuaManager.MakeTableReference( "LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_END", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_F_DASH_COMBO_XX_DOWN_X;
	m_LuaManager.MakeTableReference( "LSI_LCN_F_DASH_COMBO_XX_DOWN_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_XX_DOWN_X_StateStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_XX_DOWN_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_XX_DOWN_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_LOOP;
	m_LuaManager.MakeTableReference( "LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_LOOP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_LOOP_StateStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_LOOP_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_LOOP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_END;
	m_LuaManager.MakeTableReference( "LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_END", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_START;
	m_LuaManager.MakeTableReference( "LSI_RIDING_START", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingStartStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingStartStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingStartEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_ON;
	m_LuaManager.MakeTableReference( "LSI_RIDING_ON", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingOnStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingOnStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOnEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_OFF;
	m_LuaManager.MakeTableReference( "LSI_RIDING_OFF", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOffEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOffEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_WAIT_HABIT;
	m_LuaManager.MakeTableReference( "LSI_RIDING_WAIT_HABIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_WAIT;
	m_LuaManager.MakeTableReference( "LSI_RIDING_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_WALK;
	m_LuaManager.MakeTableReference( "LSI_RIDING_WALK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWalkStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWalkStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWalkEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_JUMP_UP;
	m_LuaManager.MakeTableReference( "LSI_RIDING_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpUpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpUpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "LSI_RIDING_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpDownStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpDownStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "LSI_RIDING_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_DASH;
	m_LuaManager.MakeTableReference( "LSI_RIDING_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_DASH_END;
	m_LuaManager.MakeTableReference( "LSI_RIDING_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashEndStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashEndStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingDashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_DASH_JUMP;
	m_LuaManager.MakeTableReference( "LSI_RIDING_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "LSI_RIDING_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_DAMAGE_FRONT;
	m_LuaManager.MakeTableReference( "LSI_RIDING_DAMAGE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_DAMAGE_BACK;
	m_LuaManager.MakeTableReference( "LSI_RIDING_DAMAGE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageBackStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_DIE;
	m_LuaManager.MakeTableReference( "LSI_RIDING_DIE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULuciel, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULuciel, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULuciel, DieFrontEventProcess );
	// June 3202a7d0 ( StateEnd ): runs in CX2GUUser::StateChange for the die states
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_ATTACK_Z;
	m_LuaManager.MakeTableReference( "LSI_RIDING_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackZStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "LSI_RIDING_JUMP_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_ATTACK_X;
	m_LuaManager.MakeTableReference( "LSI_RIDING_ATTACK_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackXStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackXStart );
	// June 32002f70 ( OnFrameMove ): none in March, riding Stage 17
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_ATTACK_SPECIAL;
	m_LuaManager.MakeTableReference( "LSI_RIDING_ATTACK_SPECIAL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_SPECIAL_MOVE;
	m_LuaManager.MakeTableReference( "LSI_RIDING_SPECIAL_MOVE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEndFutrue );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LSI_RIDING_HYPER_MODE;
	m_LuaManager.MakeTableReference( "LSI_RIDING_HYPER_MODE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingHyperModeStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingHyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, LSI_RIDING_HYPER_MODE_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingHyperModeEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingHyperModeEnd );
	m_StateList[stateData.stateID] = stateData;

	// June 32b3e8b0 @1878 LSI_RIDING_SPECIFIC ( id 93 ) left out: riding, Stage 17

	// June 32b3e8b0 @1883 LSI_RIDING_ATTACK_X_READY ( id 94 ) left out: riding, Stage 17

	// June 32b3e8b0 @1888 LSI_RIDING_ATTACK_X_READY_2 ( id 95 ) left out: riding, Stage 17

	// June 32b3e8b0 @1893 LSI_RIDING_ATTACK_X_END ( id 96 ) left out: riding, Stage 17

	// June 32b3e8b0 @1902 LSI_SA_LLC_DARK_CRESCENT ( id 298 ): registered by CH2b with its callbacks

	// June 32b3e8b0 @1910 LSI_SA_LLC_RUINOUS_IMPACT ( id 299 ): registered by CH2b with its callbacks

	// June 32b3e8b0 @1916 LSI_SA_LCG_ESCHATON ( id 363 ): registered by CH2c / CH2c2 with its callbacks

	// June 32b3e8b0 @1922 LSI_SA_LCG_FANTASIE_IMPROMPTU_2ND_MOVEMENT ( id 364 ): registered by CH3b with its callbacks
}

// char-luciel CH1n: June 32b46580 ( vslot 0x7b4 ); its batch 3207c1a0 / 31fdf970 only re-protects the edited entries ( anti-tamper ), nothing to port
/*virtual*/ void CX2GULuciel::SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ )
{
	if ( iSlotIndex_ < 0 || iSlotIndex_ > 4 )
		return;

	const int aNormalSkillStateID[]			= { USI_SPECIAL_ATTACK_1, USI_SPECIAL_ATTACK_2, USI_SPECIAL_ATTACK_3, USI_SPECIAL_ATTACK_4, USI_SPECIAL_ATTACK_5 };	// June 334ed540
	const int aHyperSkillStateID[]			= { USI_SPECIAL_ATTACK_HYPER_1, USI_SPECIAL_ATTACK_HYPER_2, USI_SPECIAL_ATTACK_HYPER_3, USI_SPECIAL_ATTACK_HYPER_4, USI_SPECIAL_ATTACK_HYPER_5 };	// June 334ed554
	const int aNormalSkillStateIDSlotB[]	= { USI_SPECIAL_ATTACK_SLOTB_1, USI_SPECIAL_ATTACK_SLOTB_2, USI_SPECIAL_ATTACK_SLOTB_3, USI_SPECIAL_ATTACK_SLOTB_4, USI_SPECIAL_ATTACK_SLOTB_5 };	// June 334ed568
	const int aHyperSkillStateIDSlotB[]		= { USI_SPECIAL_ATTACK_SLOTB_HYPER_1, USI_SPECIAL_ATTACK_SLOTB_HYPER_2, USI_SPECIAL_ATTACK_SLOTB_HYPER_3, USI_SPECIAL_ATTACK_SLOTB_HYPER_4, USI_SPECIAL_ATTACK_SLOTB_HYPER_5 };	// June 334ed57c

	const int iNormalStateID = ( false == bSlotB_ ? aNormalSkillStateID[iSlotIndex_] : aNormalSkillStateIDSlotB[iSlotIndex_] );
	UserUnitStateData& normalStateData = m_StateList[ iNormalStateID ];
	normalStateData.Init();		// June's 31fd2af0 snapshot before it is the engine's ( checklist 7i )
	normalStateData.stateID = iNormalStateID;	// June's +0x11 = 1 store is IsSkillSlotState ( X2GUUser.cpp )

	const int iHyperStateID = ( false == bSlotB_ ? aHyperSkillStateID[iSlotIndex_] : aHyperSkillStateIDSlotB[iSlotIndex_] );
	UserUnitStateData& hyperStateData = m_StateList[ iHyperStateID ];
	hyperStateData.Init();
	hyperStateData.m_bHyperState = true;	// June word +0x10 = 0x101
	hyperStateData.stateID = iHyperStateID;

	const CX2SkillTree::SkillTemplet* pSkillTempletEquipped = GetEquippedActiveSkillTemplet( eSkillID_, true );
	if ( NULL == pSkillTempletEquipped )
		return;

	const int iTranscendenceSkillID = GetTranscendenceMemoSkillID( eSkillID_ );
	if ( 0 != iTranscendenceSkillID && NULL != g_pData->GetSkillTree()->GetSkillTemplet( static_cast<CX2SkillTree::SKILL_ID>( iTranscendenceSkillID ) ) )
	{
		if ( CX2SkillTree::RNSI_SA_LLD_PHANTOM_BLADE_ADD_TRANS != g_pData->GetSkillTree()->GetUpgradeBaseSkillID( static_cast<CX2SkillTree::SKILL_ID>( iTranscendenceSkillID ) ) )
			return;

		// June @32b466c5: LSI_SA_LLD_DANCING_BLADE on both slot states + LSI_SA_LLD_DANCING_BLADE_LOOP ( 380 ), registered by CH3a
		return;
	}

	UserUnitStateData stateData;
	const int iBaseSkillID = g_pData->GetSkillTree()->GetUpgradeBaseSkillID( eSkillID_ );	// June 32207210

	switch ( pSkillTempletEquipped->m_eType )
	{
	case CX2SkillTree::ST_BUFF:
		{
			switch ( iBaseSkillID )
			{
			case CX2SkillTree::SI_SA_COMMON_AURA_DARKNESS_ACCEL:
				{
					m_LuaManager.MakeTableReference( "LSI_SA_F_COMMON_AURA_DEPRAVITY_ACCEL", normalStateData.stateID );
					normalStateData.StateInit		= SET_CB_FUNC( CX2GULuciel, PreloadAuraAccelResources );	// June 32b4c1a0 > vslot 0x888
					normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, COMMON_BUFF_FrameMove );	// June 329ccd90 > vslot 0x5f0
					normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );	// June 329cba40 > 31fcbd60

					m_LuaManager.MakeTableReference( "LSI_SA_F_COMMON_AURA_DEPRAVITY_ACCEL", hyperStateData.stateID );
					hyperStateData.m_bHyperState	= true;
					hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
					hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;

					stateData.Init();
					stateData.stateID			= LSI_SA_M_COMMON_AURA_DEPRAVITY_ACCEL;
					m_LuaManager.MakeTableReference( "LSI_SA_M_COMMON_AURA_DEPRAVITY_ACCEL", stateData.stateID );
					stateData.m_eSkillID		= CX2SkillTree::SI_SA_COMMON_AURA_DARKNESS_ACCEL;
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, COMMON_BUFF_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );
					m_StateList[stateData.stateID] = stateData;
				} break;

			case CX2SkillTree::RNSI_A_LUCIEL_AURA_PUNISHMENT_ACCEL:
			case CX2SkillTree::RNSI_A_LUCIEL_AURA_PUNISHMENT_ACCEL_UPGRADE:
				{
					m_LuaManager.MakeTableReference( "LSI_SA_F_COMMON_AURA_PUNISHING_ACCEL", normalStateData.stateID );
					normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, COMMON_BUFF_FrameMove );
					normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );

					m_LuaManager.MakeTableReference( "LSI_SA_F_COMMON_AURA_PUNISHING_ACCEL", hyperStateData.stateID );
					hyperStateData.m_bHyperState	= true;
					hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
					hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;

					stateData.Init();
					stateData.stateID			= LSI_SA_M_COMMON_AURA_PUNISHING_ACCEL;
					m_LuaManager.MakeTableReference( "LSI_SA_M_COMMON_AURA_PUNISHING_ACCEL", stateData.stateID );
					stateData.m_eSkillID		= eSkillID_;	// June also sets byte +0x11 here ( no March member; routed to CH1o )
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, COMMON_BUFF_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );
					m_StateList[stateData.stateID] = stateData;
				} break;

			case CX2SkillTree::RNSI_BUFF_PHANTOM_CIRCLE:
				{
					m_LuaManager.MakeTableReference( "LSI_SA_F_COMMON_BUFF_PHANTOM_CIRCLE", normalStateData.stateID );
					normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, COMMON_BUFF_FrameMove );
					normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );

					m_LuaManager.MakeTableReference( "LSI_SA_F_COMMON_BUFF_PHANTOM_CIRCLE", hyperStateData.stateID );
					hyperStateData.m_bHyperState	= true;
					hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
					hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;

					stateData.Init();
					stateData.stateID			= LSI_SA_M_COMMON_BUFF_PHANTOM_CIRCLE;
					m_LuaManager.MakeTableReference( "LSI_SA_M_COMMON_BUFF_PHANTOM_CIRCLE", stateData.stateID );
					stateData.m_eSkillID		= eSkillID_;
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GULuciel, COMMON_BUFF_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );
					m_StateList[stateData.stateID] = stateData;
				} break;

			default:
				break;
			}
		}
		// no break: June falls into the active-skill ids ( none of them is a buff id; JUNE_BUGS 722 )

	case CX2SkillTree::ST_ACTIVE:
	case CX2SkillTree::ST_SPECIAL_ACTIVE:
	case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
	case CX2SkillTree::ST_SPECIAL_ACTIVE_GRADE_1:
	case CX2SkillTree::ST_SPECIAL_ACTIVE_GRADE_2:
	case CX2SkillTree::ST_SPECIAL_ACTIVE_GRADE_3:
		// June @32b469d6: the 59 skill-id cases on iBaseSkillID, each written by its skill phase ( june2017_re/char_luciel/ch1n_skill_cases.tsv )
		return;

	case CX2SkillTree::ST_RELATIONSHIP_SKILL:
		{
			if ( CX2SkillTree::SI_ETC_WS_COMMON_LOVE == eSkillID_ )	// June compares the raw id here
			{
				m_LuaManager.MakeTableReference( "LSI_LU_THROW_ITEM", normalStateData.stateID );
				normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_FrameMove );	// June 329ccda0 > vslot 0x5f4
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_EventProcess );	// June 329ccdb0 > vslot 0x5f8

				m_LuaManager.MakeTableReference( "LSI_LU_THROW_ITEM", hyperStateData.stateID );
				hyperStateData.m_bHyperState	= true;
				hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
				hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;

				stateData.Init();
				stateData.stateID			= 263;	// no June name: the LSI_CIEL_THROW_ITEM table again ( InitStateCommon binds it to 228 )
				m_LuaManager.MakeTableReference( "LSI_CIEL_THROW_ITEM", stateData.stateID );
				stateData.m_eSkillID		= CX2SkillTree::SI_ETC_WS_COMMON_LOVE;
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} return;

	default:
		return;
	}
}

#endif SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE
//}}
