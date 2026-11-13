#include "StdAfx.h"
#include "X2GUEL.h"
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS_DEBUG
#include "Offline/X2OfflineLog.h"	// char-elesis CH1q
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS_DEBUG
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
#include "Offline/X2OfflineLog.h"	// char-elesis CH1: the ELESIS DATA line
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE

#ifdef NEW_CHARACTER_EL

namespace _CONST_EL_
{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 기사의 숙명, HP 범위 30% -> 50% 로 변경
	const float CHIVALRY_BUFF_CONDITION_HP_RATE = 0.5f;	 // 기사의 숙명 버프 조건 연동 체력 배율
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	const float CHIVALRY_BUFF_CONDITION_HP_RATE = 0.3f;	 // 기사의 숙명 버프 조건 연동 체력 배율
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

	const float MAX_EXTINCTION_CHARGE_TIME		= 1.f;	 // 익스팅션 최대 충전 시간
	const float LIMIT_EXTINCTION_POWER_RATE		= 0.2f;  // 익스팅션 공격력 증가 제한 값

	const float MIN_EXTINCTION_DEGREE_Z			= -45.f; // 익스팅션 최소 각도
	const float MAX_EXTINCTION_DEGREE_Z			= 0.f;   // 익스팅션 최고 각도
	const float BASE_EXTINCTION_DEGREE_Z		= -30.f; // 익스팅션 기본 각도
	const float EXTINCTION_DEGREE_Z_CONTROL		= 100.f; // 익스팅션 각도 조절 값
	
	// 검의 길
	const float GALE_COOL_TIME_DECREASE_RATE		= 0.2f;	// 질풍 스킬 쿨타임 Rate
	const float GALE_COOL_TIME_DECREASE_RATE_PVP	= 0.6f;	// 질풍 스킬 쿨타임 Rate(대전 1/2)

	const float GALE_MP_CONSUME_RATE				= 0.6f; // 질풍 스킬 MP 소모량 Rate
	const float GALE_MP_CONSUME_RATE_PVP			= 0.8f; // 질풍 스킬 MP 소모량 Rate(대전 1/2)

	const float GALE_FORCE_DOWN_DECREASE_RATE		= 0.6f;	// 질풍 커맨드 다운수치 감소Rate
	const float GALE_FORCE_DOWN_DECREASE_RATE_PVP	= 0.8f;	// 질풍 커맨드 다운수치 감소Rate(대전 1/2)

	// 커맨드 
	const UINT MAX_DASH_JUMP_COMBO_SPK_XZ_REATTACK	= 2;	// ESI_DASH_JUMP_COMBO_SPK_XZ 추가 공격 가능 횟수

	// MP 소모
	const float FIRE_BALL_MP_COST					= 6.f;
	const float SPK_DASH_JUMP_XZ_MP_COST			= 4.f;


#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	const int MAX_XX_DOWN_X_LOOP_COUNT = 4;				// 그랜드 마스터, XX밑X 커맨드 최대 루프 횟수
	const float MAX_DRASTIC_CHARGE_LOOP_TIME = 2.0f;	// 그랜드 마스터 액티브, 드래스틱 차지 최대 루프 시간
	const float MIN_DRASTIC_CHARGE_LOOP_TIME = 0.5f;	// 그랜드 마스터 액티브, 드래스틱 차지 최소 루프 시간

	const float MAX_JUGGERNAUT_BUSTER_LOOP_TIME = 1.0f;

	const float SBM_DASH_COMBO_DELAY_TIME = 0.5;	// 블레이징 하트 대쉬 콤보 시작 딜레이  타임. 대시 콤보의 마지막( XXX나 XXXZ ) 후에 다시 대시 콤보를 시작하기 까지의 시간
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트



}

#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
// char-elesis CH1b: June's expansion of ELSE_IF_STATE_CHANGE_ON_EX_ ( the flag also fires when the future animation ends, the time tests run before the key test )
#define ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( iEventFlagIndex, fInputTimingStart, fInputTiming, fChangeTiming, bCondition, iStateID ) \
	else if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && \
		( (float)(fChangeTiming) < m_pXSkinAnimFuture->GetNowAnimationTime() || true == m_pXSkinAnimFuture->IsAnimationEnd() ) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(fInputTiming) && (float)(fInputTimingStart) <= m_pXSkinAnimFuture->GetNowAnimationTime() && true == (bCondition) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
// char-elesis CH1g: the same expansion as ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_, starting with if ( June 32b64ee0 )
#define IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( iEventFlagIndex, fInputTimingStart, fInputTiming, fChangeTiming, bCondition, iStateID ) \
	if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && \
		( (float)(fChangeTiming) < m_pXSkinAnimFuture->GetNowAnimationTime() || true == m_pXSkinAnimFuture->IsAnimationEnd() ) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(fInputTiming) && (float)(fInputTimingStart) <= m_pXSkinAnimFuture->GetNowAnimationTime() && true == (bCondition) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
// char-elesis CH1b review: June's walk / back-walk / dash / ZX cancels also test IsOnSomethingFuture() ( 32b5f7c0 @32b5f901 ), as Ara's
#define WALK_CANCEL_AFTER_ON_LINE_( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && true == IsOnSomethingFuture() && \
	( true == m_InputData.pureRight || true == m_InputData.pureLeft ) ) \
	{ StateChange( USI_WAIT ); }
#define BWALK_CANCEL_AFTER_ON_LINE_( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && true == IsOnSomethingFuture() && \
	( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true) || \
	( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false) ) ) \
	{ StateChange( USI_WAIT ); }
#define DASH_CANCEL_AFTER_ON_LINE_( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && true == IsOnSomethingFuture() && \
	( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft ) ) \
	{ StateChangeDashIfPossible(); }
#define ZX_CANCEL_AFTER_EX_ON_LINE_( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && true == IsOnSomethingFuture() && true == m_InputData.oneZ ) \
	{ StateChange( GetComboZStateID() ); } \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && true == IsOnSomethingFuture() && true == m_InputData.oneX ) \
	{ StateChange( GetComboXStateID() ); }
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
#ifndef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
// char-elesis CH1e: the flag-chain and on-line cancel macros above again, for the _CORE functions while _LIVE is off ( as Ara's )
#define ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( iEventFlagIndex, fInputTimingStart, fInputTiming, fChangeTiming, bCondition, iStateID ) \
	else if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && \
		( (float)(fChangeTiming) < m_pXSkinAnimFuture->GetNowAnimationTime() || true == m_pXSkinAnimFuture->IsAnimationEnd() ) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(fInputTiming) && (float)(fInputTimingStart) <= m_pXSkinAnimFuture->GetNowAnimationTime() && true == (bCondition) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
#define WALK_CANCEL_AFTER_ON_LINE_( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && true == IsOnSomethingFuture() && \
	( true == m_InputData.pureRight || true == m_InputData.pureLeft ) ) \
	{ StateChange( USI_WAIT ); }
#define BWALK_CANCEL_AFTER_ON_LINE_( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && true == IsOnSomethingFuture() && \
	( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true) || \
	( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false) ) ) \
	{ StateChange( USI_WAIT ); }
#define DASH_CANCEL_AFTER_ON_LINE_( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && true == IsOnSomethingFuture() && \
	( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft ) ) \
	{ StateChangeDashIfPossible(); }
#define ZX_CANCEL_AFTER_EX_ON_LINE_( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && true == IsOnSomethingFuture() && true == m_InputData.oneZ ) \
	{ StateChange( GetComboZStateID() ); } \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && true == IsOnSomethingFuture() && true == m_InputData.oneX ) \
	{ StateChange( GetComboXStateID() ); }
// char-elesis CH1g: the flag-chain macro that starts with if, again for the _CORE functions while _LIVE is off
#define IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( iEventFlagIndex, fInputTimingStart, fInputTiming, fChangeTiming, bCondition, iStateID ) \
	if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && \
		( (float)(fChangeTiming) < m_pXSkinAnimFuture->GetNowAnimationTime() || true == m_pXSkinAnimFuture->IsAnimationEnd() ) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(fInputTiming) && (float)(fInputTimingStart) <= m_pXSkinAnimFuture->GetNowAnimationTime() && true == (bCondition) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
// char-elesis CH1e: constants of the Grand Master states ( June 32b86080 / 32b72fe0 / 32b731a0 )
namespace _CONST_EL_CH1E_
{
	const float XX_ATTACK_MP_COST			= 2.f;		// also DashComboX's MP test ( CH1d2 )
	const float XX_ATTACK_TIME_OFFSET		= 0.3f;
	const float XX_ATTACK_TIME_DIVISOR		= 10.f;
	const float XX_DOWN_X_FORWARD_SPEED		= 700.f;
	const float XX_DOWN_X_BACKWARD_SPEED	= -500.f;
}
using namespace _CONST_EL_CH1E_;
// char-elesis CH1f: constants of the Pyro Knight line dash-jump Z-X states ( June 32b52300 / 32b7d2b0 )
namespace _CONST_EL_CH1F_
{
	const UINT DASH_JUMP_ZX_LOOP_COUNT		= 5;
	const float DASH_JUMP_ZX_LOOP_MP_COST	= 2.f;
	const float DASH_JUMP_ZX_LOOP_EVENT_TIME	= 0.1f;
}
using namespace _CONST_EL_CH1F_;
// char-elesis CH1f2: constants of the Blazing Heart X-Z fireball states ( June 32b78390 / 32b785b0 / 32b78920 )
namespace _CONST_EL_CH1F2_
{
	const float XZ_FIRE_MP_COST		= 2.f;
}
using namespace _CONST_EL_CH1F2_;
// char-elesis CH1g: constants of the Dark Knight dash-jump X-X-X and Z-X states ( June 32b7c0b0 / 32b7d550 / 32b73c90 / 32b52350 )
namespace _CONST_EL_CH1G_
{
	const float DASH_JUMP_ZX_CHARGE_TIME_LIMIT			= 1.f;		// the charge time stops growing here
	const float DASH_JUMP_ZX_CHARGE_FIRE_TIME			= 3.f;		// state time at which a held X fires
	const float DASH_JUMP_ZX_FIRE_DEGREE				= -45.f;	// class-skill float of a shot aimed down; 0 while the facing key is held
	const float DASH_JUMP_ZX_FIRE_MP_COST				= 5.f;
	const float DASH_JUMP_ZX_FIRE_SCALE_PER_CHARGE		= 0.5f;
	const float DASH_JUMP_XXX_FIRE_MP_COST				= 3.f;
	const float DASH_JUMP_FIRE_EVENT_TIME				= 0.01f;
}
using namespace _CONST_EL_CH1G_;
// char-elesis CH1g2: constants of the Crimson Avenger dash combo states ( June 32b66ac0 / 32b677d0 / 32b523d0 / 32b52420 / 32b7c2e0 / 32b7c510 / 32b7c940 / 32b7cd70 / 32b7d920 )
namespace _CONST_EL_CH1G2_
{
	const USHORT DASH_COMBO_ZZZ_LOOP_MAX_COUNT				= 4;		// a Z press starts another loop only below this count
	const float DASH_COMBO_ZZZ_MP_COST						= 3.f;
	const float DASH_COMBO_ZZZ_START_FIRE_TIME				= 0.43f;
	const float DASH_COMBO_ZZZ_LOOP_1_FIRST_FIRE_TIME		= 0.01f;
	const float DASH_COMBO_ZZZ_LOOP_1_SECOND_FIRE_TIME		= 0.25f;
	const float DASH_COMBO_ZZZ_LOOP_2_FIRST_FIRE_TIME		= 0.08f;
	const float DASH_COMBO_ZZZ_LOOP_2_SECOND_FIRE_TIME		= 0.31f;
	const float DASH_COMBO_ZZZ_END_FIRE_TIME				= 0.37f;
	const float DASH_COMBO_ZX_LOOP_TIME_LIMIT				= 2.f;		// state time from which the loop ends with its animation even with X held
	const float DASH_COMBO_XZ_CHARGE_TIME_LIMIT				= 3.f;		// the charge time stops growing here
	const float DASH_COMBO_XZ_CHARGE_FIRE_TIME				= 3.1f;		// state time at which a held Z fires
	const float DASH_COMBO_XZ_FIRE_EVENT_TIME				= 0.01f;
	const float DASH_COMBO_XZ_FIRE_MP_COST					= 5.f;
	const float DASH_COMBO_XZ_FIRE_POWER_PER_CHARGE			= 0.5f;
	const float DASH_COMBO_XZ_FIRE_LIFE_TIME_PER_CHARGE		= 0.333f;
	const float DASH_COMBO_XZ_FIRE_SCALE_PER_CHARGE			= 0.133f;
}
using namespace _CONST_EL_CH1G2_;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
// char-elesis CH1h: constants of the HP tier ( June 32b6fb80 ) and of Chivalry's debuff immunity ( June 32b6fc40 )
namespace _CONST_EL_CH1H_
{
	const float HP_TIER_3_BELOW_RATE			= 0.3f;
	const float HP_TIER_2_BELOW_RATE			= 0.5f;
	const float HP_TIER_1_BELOW_RATE			= 0.7f;
	const float CHIVALRY_DEBUFF_IMMUNE_HP_RATE	= 0.15f;	// June multiplies the max HP by the double 0.15f
}
using namespace _CONST_EL_CH1H_;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
// char-elesis CH1j: constants of Chivalry's defence check ( June 32b7ac20 )
namespace _CONST_EL_CH1J_
{
	const float CHIVALRY_DEFENCE_HP_RATE		= 0.5f;
	const float CHIVALRY_DEFENCE_HP_RATE_PVP	= 0.25f;	// in a PvP game state
}
using namespace _CONST_EL_CH1J_;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
// char-elesis CH2a: constants of the Sonic Assault: Sting memo and Wild Shock states ( June 32b75580 / 32b4f840 / 32b70db0 / 32b75760 )
namespace _CONST_EL_CH2A_
{
	const float SONIC_ASSAULT_CHARGE_MP_COST_PER_SECOND		= 100.f;	// June: frame time x 50 x 2
	const float SONIC_ASSAULT_CHARGE_TIME_LIMIT				= 0.5f;		// the charge time and the MP drain stop here
	const float SONIC_ASSAULT_CHARGE_HOLD_TIME				= 2.f;		// held this long, the charge state fires without a release
	const float SONIC_ASSAULT_FIRE_EVENT_TIME				= 0.01f;
	const float SONIC_ASSAULT_FIRE_SPEED_RATE				= 2.75f;	// times the walk speed
	const float SONIC_ASSAULT_FIRE_SPEED_PER_CHARGE_RATE	= 0.5f;
	const float SONIC_ASSAULT_FIRE_SPEED_TIME				= 0.11f;
	const float WILD_SHOCK_DAMAGE_EVENT_TIME				= 0.55f;
}
using namespace _CONST_EL_CH2A_;
// char-elesis CH2b: constants of the Saber Knight special actives ( June 32b75f90 / 32b5cb40 / 32b76710 / 32b5cbd0 / 32b60650 )
namespace _CONST_EL_CH2B_
{
	const float EXTINCTION_ATTACK_SCALE_VICTORIOUS_SWORD			= 1.2f;	// the attack effect's scale while the Victorious Sword buff is applied
	const float EXTINCTION_ATTACK_SCALE_VICTORIOUS_SWORD_UPGRADE	= 1.4f;	// the same with the enhanced Victorious Sword buff
	const float JUDGEMENT_DRIVE_CHARGE_LIMIT						= 2.f;
	const float JUDGEMENT_DRIVE_CHARGE_LIMIT_UPGRADE				= 1.f;
	const float JUDGEMENT_DRIVE_HOLD_TIME_AFTER_EVENT				= 2.1f;	// the hold state ends this long after its event time
	const float JUDGEMENT_DRIVE_DAMAGE_EVENT_TIME					= 0.01f;
	const float JUDGEMENT_DRIVE_MIN_CHARGE							= 0.15f;	// a shorter charge counts as none
	const float JUDGEMENT_DRIVE_POWER_PER_CHARGE					= 0.25f;
	const float JUDGEMENT_DRIVE_POWER_PER_CHARGE_UPGRADE			= 0.5f;
	const float CRITICAL_DIVE_HIT_TIME								= 0.425f;
	const float CRITICAL_DIVE_MP_REFUND_RATE						= 0.5f;
}
using namespace _CONST_EL_CH2B_;
// char-elesis CH2b2: constants of the Saber Knight actives ( June 32b75210 / 32b70d30 )
namespace _CONST_EL_CH2B2_
{
	const float LEAP_CRASH_JUMP_SPEED_X							= 2200.f;
	const float LEAP_CRASH_JUMP_SPEED_X_AFTER_DASH				= 2700.f;	// used while the dash time ( +0x977c ) is above 0
	const float LEAP_CRASH_JUMP_SPEED_Y							= 1000.f;
	const float LEAP_CRASH_LANDING_EVENT_TIME					= 0.01f;
	const float LEAP_CRASH_LANDING_SCALE_PER_DASH_TIME			= 0.4f;
	const float LEAP_CRASH_LANDING_SCALE_LIMIT					= 2.f;
	const int	LEAP_CRASH_UPGRADE_NUMERICAL_INDEX				= 3;		// the 4th numerical table of the enhanced skill ( June @32b75456 )
	const int	EFFECT_MODIFIER_TAG_SCALE						= 15;		// June 321dad70 tag with no enum name ( as Eve's )
}
using namespace _CONST_EL_CH2B2_;
// char-elesis CH2c: constants of Avatar of the Fire ( June 32b7db80 )
namespace _CONST_EL_CH2C_
{
	const float AVATAR_OF_THE_FIRE_AURA_EVENT_TIME			= 0.01f;
	const float AVATAR_OF_THE_FIRE_DAMAGE_EVENT_TIME		= 0.1f;
	const float AVATAR_OF_THE_FIRE_ABSORB_RANGE				= 1000.f;
	const float AVATAR_OF_THE_FIRE_MP_PER_ABSORBED_EFFECT	= 10.f;
}
using namespace _CONST_EL_CH2C_;
// char-elesis CH2d: constants and helpers of the Dark Knight skills ( June 32b89c10 / 32b8b410 / 32b8b850 / 32b73ae0 / 32b51d10 / 32b89470 )
namespace _CONST_EL_CH2D_
{
	const float		BRUTAL_CUTTER_EVENT_TIME					= 0.2f;
	const USHORT	BRUTAL_CUTTER_BLEEDING_HIT_COUNT			= 8;
	const float		EJECTION_BUSTER_ATTACK_EVENT_TIME			= 0.01f;
	const float		BURNING_PHANTOM_TICK_TIME					= 0.15f;
	const USHORT	BURNING_PHANTOM_FIRST_TICK					= 4;		// the loop may end from this tick on, MP is paid above it
	const USHORT	BURNING_PHANTOM_LAST_TICK					= 9;
	const float		BURNING_PHANTOM_MP_PER_TICK					= 10.f;
	const float		BURNING_PHANTOM_MIN_MP						= 10.f;
	const float		INJECTION_STIGMA_HP_COST_EVENT_TIME			= 0.01f;
	const float		INJECTION_STIGMA_DAMAGE_EVENT_TIME			= 0.2f;
	const float		DARK_WAVE_DAMAGE_EVENT_TIME					= 0.3f;
	const float		DARK_WAVE_IMPACT_EVENT_TIME					= 0.8f;
	const USHORT	BLEEDING_SINGLE_HIT_COUNT					= 1;
	const float		BLEEDING_HP_COST_DAMAGE_RATE				= 0.1f;		// the share of the skill's damage a bleeding cost takes per hit
	const float		BLEEDING_MIN_HP_RATE						= 0.2f;		// no HP is paid below this HP share
	const float		BLEEDING_KEEP_HP_RATE						= 0.1f;		// the cost leaves this share of max HP
	const float		BLEEDING_BLOOD_RUNAWAY_COST_RATE			= 0.2f;		// June multiplies the cost by the double 0.2f
	const float		BLEEDING_DAMAGE_RATE						= 1.4f;
	const float		BLEEDING_DAMAGE_RATE_PVP_OR_PVE				= 1.3f;

	// June 32007c40: the acquired skill's ability value ( char-ara CH1h's copy )
	bool TryGetAcquiredSkillAbilityValue( CX2Unit* pUnit_, const CX2SkillTree::SKILL_ABILITY_TYPE eType_, const CX2SkillTree::SKILL_ID eSkillID_, const bool bMemoSwap_, float& fOut_ )
	{
		if( NULL == pUnit_ )
			return false;

		const int iSkillLevel = pUnit_->GetUnitData().m_UserSkillTree.GetSkillLevel( eSkillID_, false, 0, true );
		if( iSkillLevel <= 0 )
			return false;

		const CX2SkillTree::SkillTemplet* pSkillTemplet = pUnit_->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( eSkillID_, bMemoSwap_ );
		if( NULL == pSkillTemplet )
			return false;

		const bool bPvpOrPve = ( NULL != g_pX2Game && true == g_pX2Game->GetIsNowPVPCorrectionGame() );	// June 322cc1a0: PVP or PVE
		return pSkillTemplet->TryGetSkillAbilityValue( eType_, iSkillLevel, bPvpOrPve, fOut_ );
	}

	// June 32028250 type 0xf, inlined in 32b89c10 @32b89ea4: the factor's first time finalizer x the skill memo's first value
	void RescaleBuffFactorTime( CX2BuffFactor& refFactor_, const float fRate_ )
	{
		const KBuffFinalizerFactor* pFirstTime = refFactor_.GetFirstFinalizer( BFT_TIME );
		if ( NULL == pFirstTime || pFirstTime->m_vecValues.size() <= 1 )
			return;
		refFactor_.ApplyExternalFactorToFinalizer( BFT_TIME, pFirstTime->m_vecValues[1] * fRate_, false );
	}
}
using namespace _CONST_EL_CH2D_;
// char-elesis CH3a: constants of the Grand Master special actives ( June 32b6f550 / 32b6f620 / 32b77de0 / 32b78170 )
namespace _CONST_EL_CH3A_
{
	const float		JUGGERNAUT_BUSTER_START_SPEED_RATE			= 0.75f;	// of the dash speed
	const float		JUGGERNAUT_BUSTER_LOOP_SPEED_RATE			= 1.05f;
	const float		JUGGERNAUT_BUSTER_BLACK_HOLE_EVENT_TIME		= 0.8f;
	const float		JUGGERNAUT_BUSTER_FINISH_EVENT_TIME			= 0.29f;
}
using namespace _CONST_EL_CH3A_;
// char-elesis CH3a2: constants of the Grand Master actives ( June 32b85aa0 )
namespace _CONST_EL_CH3A2_
{
	const float		PROVOKE_REVENGE_MP_CONSUME		= 10.f;	// MP the second press of the skill key spends ( UpNowMp( -10 ) )
}
using namespace _CONST_EL_CH3A2_;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON
// char-elesis CH3b0: constants of the summons and the Blood Runaway cleanse check ( June 32b79db0 / 32b8b0c0 / 32b6fb10 )
namespace _CONST_EL_CH3B0_
{
	const unsigned char	SCALET_ROSE_MAX_FIRE_BLOSSOM_COUNT		= 3;
	const double		FIRE_BLOSSOM_RANDOM_DISTANCE			= 450.0;	// June multiplies and adds in double, then stores a float
	const double		FIRE_BLOSSOM_MIN_DISTANCE				= 50.0;
	const float			BLOOD_CUTTER_NPC_DELAY					= 0.3f;
	const float			BLOOD_RUNAWAY_NO_TIER_CLEANSE_INTERVAL	= 9999.f;	// over 70 % HP the timer never reaches it
}
using namespace _CONST_EL_CH3B0_;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
namespace _CONST_EL_CH1C_ { bool ElesisAwakeningEffectSetPresent(); }	// char-elesis CH1h: defined with the CH1c stand-ins below
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_UI
// char-elesis CH5: June 333a8450 ( table 337ad5c0, stride 0x90 = 3 x 0x30; GFx names without extension ): set 0 Elesis ( 8 / 29 / 30 / 35 ), 1 Grand Master, 2 Blazing Heart, 3 Crimson Avenger
static const CX2GUUser::SkillCutInSet s_SkillCutInSet[] =
{
	{ CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_01" ), CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_01" ), CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_01" ), },
	{ CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_GM_01" ), CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_GM_01" ), CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_GM_01" ), },
	{ CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_BH_01" ), CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_BH_01" ), CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_BH_01" ), },
	{ CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_CA_01" ), CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_CA_01" ), CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_CA_01" ), },
};
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_UI
static const CX2GUUser::SkillCutInSet s_SkillCutInSet[] =
{
	{ 
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis.dds"),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_Pyro_Knight.dds"),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_Saber_Knight.dds"),
	},

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
		{ /// 그랜드 마스터
			CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_SGM.dds"),
			CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_SGM.dds"),
			CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_SGM.dds"),
		},

		{ /// 블레이징 하트
			CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_SBH.dds"),
			CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_SBH.dds"),
			CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_SBH.dds"),
		}
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
};

#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_UI
/*static */CX2GUEL* CX2GUEL::CreateGUEL( int unitIndex, int teamNum, lua_State*   pScriptLuaState, int frameBufferNum, CX2Unit* pUnit )
{
	return new CX2GUEL( unitIndex, teamNum, pScriptLuaState, frameBufferNum, pUnit );
};

CX2GUEL::CX2GUEL( int unitIndex, int teamNum, lua_State*   pScriptLuaState, int frameBufferNum, CX2Unit* pUnit )
: CX2GUUser( unitIndex, teamNum, pScriptLuaState, frameBufferNum, pUnit )
#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
, m_iPreComboCount( -1 )
#endif // ELSWORD_WAY_OF_SWORD
, m_iPressedSkillSlotIndex( -1 )
, m_fMPConsume ( 0.f )
, m_fFireBallPowerRate(1.f)
, m_fChivalryBuffCheckTime ( 0.f )
, m_uiChivalryAttackBuffFactorID ( 0 )
, m_uiChivalryDefenceBuffFactorID ( 0 )
, m_uiChivalryLevel( 0 )
, m_bIsActiveEndurancePower( false )
, m_fExtinctionRemainTime( 0.f )
, m_fExtinctionDegreeZ ( 30.f )
, m_bIsApplyVictoriousSword ( false )
, m_fDodgeAndSlashCoolTimeDecreaseRate ( 1.f )
, m_hExtinctionArrow(INVALID_PARTICLE_SEQUENCE_HANDLE)
, m_hExctionBG(INVALID_PARTICLE_SEQUENCE_HANDLE)
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_hExtinctionArrowParticle(INVALID_PARTICLE_HANDLE)
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_pExtinctionArrow(NULL)
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_pSkillTemplet(NULL)
, m_bIsDashAttackPossible(true)
, m_uiSPKXZReattakCount(0)
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
, m_fDashJumpZXChargeTime(0.f)	// char-elesis CH1g
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
, m_fSonicAssaultStingChargeTime(0.f)	// char-elesis CH2a
, m_uiDodgeAndSlashAttackCount(0)
, m_iWaltzOfSwordDodgeAbility(0)
, m_fDodgeAndSlashConsumedMp(0.f)
, m_usBurningPhantomLoopCount(0)	// char-elesis CH2d
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
, m_usDashComboZZZLoopCount(0)	// char-elesis CH1g2
, m_usBlazeAbsoluLevel(0)	// char-elesis CH1j
, m_fDashElapsedTime(0.f)
, m_fAdvancedKnightMasteryMPValue(0.f)
, m_fDashChargeTime(0.f)
, m_usDignityOfGrandMasterLevel(0)	// char-elesis CH1k
, m_fPunishEffectPowerRate(0.f)
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_BLOOD
, m_usMadnessLevel(0)	// char-elesis CH1h
, m_bIsMadnessActive(false)
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_BLOOD
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
, m_fWayOfSwordPointMultiplier(1.f)	// char-elesis CH1q: June 32b70ee0 @32b71fb6 / @32b71fd3 / @32b71fbc / @32b71fc4
, m_fVigorHpIntakeRate(0.f)
, m_fDestructionDamageRate(1.f)
, m_fDestructionBleedingDamageRate(1.f)
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
, m_fSnapshotWayOfSwordPoint(0.f)	// char-elesis CH1q
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON
, m_ucScaletRoseHitCount(0)	// char-elesis CH3b0: June 32b70ee0
, m_usBloodRunawayLevel(0)
, m_fBloodRunawayCleanseIntervalTier1(0.f)
, m_fBloodRunawayCleanseIntervalTier2(0.f)
, m_fBloodRunawayCleanseIntervalTier3(0.f)
, m_fBloodRunawayCleanseElapsedTime(0.f)
, m_eBloodRunawaySaveDebuffID(BTI_NONE)
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON
, m_uiDodgeAndSlashMaxReuseCount(0)
, m_uiDodgeAndSlashReuseCount(0)
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
// 그랜드 마스터 멤버 변수
, m_iComboXXdownXNowLoopCount (0)
, m_iComboXXdownXMaxLoopCount (0)
, m_bAbleQuickStand ( false )				
, m_bAbleQuickStandAttack ( false )			
, m_iRushingSwordAddLoopValue ( 0 )		
, m_iRushingSwordNowLoopValue ( 0 )
, m_iDecreaseMPRushingSword ( 0 )		
, m_cFirstStrikeData ( 0, 0, 0 )			
, m_fDrasticChargeNowLoopTime ( 0 )
, m_fDrasticChargeLoopDecreaseMPValue ( 0 )
, m_bIsProvokeStateChange ( false )
// 블레이징 하트 멤버 변수
, m_fStigmaOfFireCoolTime( 0.f )
, m_bIsStigmaOfFire( false )
, m_uiSkillLevelStigmaOfFire( 0 )
, m_eFireBlossomsSaveDebuffID( BTI_NONE )
, m_fFireBlossomsCoolTime( 0.f )
, m_fFireBlossomsCheckTime( -1.f )
, m_bIsFireBlossoms( false )
, m_uiFireBlossomsSkillLevel( 0 )
, m_fJuggernautBusterNowLoopTime ( 0.f )
, m_bJuggernautBusterIsStartDirectionRight ( false )
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_hSBHScaletRoseDamageEffect( INVALID_DAMAGE_EFFECT_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_pSBHScaletRoseDamageEffect( NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_bIsThereBlazingDanceTarget( false )
, m_bIsFrameSword( false )
, m_uiSkillLevelFlameSword( 0 )
, m_bIsLearnUnextinguishableFire( false )
, m_bIsBlazingDanceLastAttack( false )
, m_hSnapCresherLoop ( INVALID_EFFECTSET_HANDLE )
, m_hSnapCreaherLoopAttack ( INVALID_EFFECTSET_HANDLE )
, m_hDrasticChargeLoop ( INVALID_EFFECTSET_HANDLE )
, m_hDrasticChargeGroundFire ( INVALID_EFFECTSET_HANDLE )
, m_hXXDownXLoop ( INVALID_EFFECTSET_HANDLE )

, m_fWallOfProminenceLifeTime( 0.0f )
, m_fStigmaOfFireDamageValue( 1.0f )
, m_fDashComboStartDelay( -1.0f )
, m_fAncientFireRenderTime( -1.0f )
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
{
	m_fThrowStartTime = 0.66f;
	m_vThrowStartPosOffset = D3DXVECTOR3(40.f, 130.f, 0.f);
	m_vThrowStartVelocity = D3DXVECTOR3(2000.f, 100.f, 0.f);

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	m_vecBlazingDanceTargetUid.clear();
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

}

CX2GUEL::~CX2GUEL(void)
{
}

/*static*/ void CX2GUEL::AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
}

/*static*/ void CX2GUEL::AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
}

/*static*/ void CX2GUEL::AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ )
{
	CX2GUEL::AppendMajorParticleToDeviceList( listInOut_ );
	CX2GUEL::AppendMinorParticleToDeviceList( listInOut_ );
	CX2GameUnit::Weapon::AppendEnchantParticleForSwordToDeviceList( listInOut_, eDamageType_ );
}

/*virtual */bool CX2GUEL::CanRidingState()
{
	if( true == CX2GUUser::CanRidingState() )
		return true;

 	switch( GetNowStateID() )
 	{
 	case ESI_SWORD_WAIT:
 		return true;
 	}

	return false;
}

/*virtual*/ void CX2GUEL::Init( bool bUseTeamPos_, int iStartPosIndex_ )
{
	CX2GUUser::Init( bUseTeamPos_, iStartPosIndex_ );

	SetNeedToUpdateWeaponEffect( true );
	UpdateEquipCollisionData();
}
/*virtual*/ void CX2GUEL::ParseCommonRandomState()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	ParseCommonRandomStateByUnitClass();	// char-elesis CH1m: June parses in CX2GUUser::Init ( 31feaac0 ) by class name, 35 / 125 included
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_LuaManager.BeginTable( "INIT_COMMON_RANDOM_STATE" ) )
	{
		const char* tableName = "";
		switch( GetUnit()->GetClass() )
		{
		case CX2Unit::UC_ELESIS_KNIGHT:
			{
				tableName = "EL_KNIGHT";
			} break;
		case CX2Unit::UC_ELESIS_SABER_KNIGHT:
			{
				tableName = "EL_SABER_KNIGHT";
			} break;
		case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
			{
				tableName = "EL_PYRO_KNIGHT";
			} break;
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	// 김종훈, 엘리시스 1-2 그랜드 마스터
		case CX2Unit::UC_ELESIS_GRAND_MASTER:
			{
				tableName = "EL_GRAND_MASTER";
			} break;
		case CX2Unit::UC_ELESIS_BLAZING_HEART:
			{
				tableName = "EL_BLAZING_HEART";
			} break;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE	// 김종훈, 엘리시스 1-2 그랜드 마스터
		default:
			{
				ASSERT( !"NO" );
			} break;
		}

		CX2GUUser::ParseCommonRandomState(tableName);
		m_LuaManager.EndTable(); // INIT_COMMON_RANDOM_STATE
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
/*virtual*/ void CX2GUEL::InitState()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1m: June 32b87a60 ( vslot 0x7a4 )
	const CX2Unit::UNIT_CLASS eClass = ( NULL != GetUnit() ) ? GetUnit()->GetUnitData().m_UnitClass : CX2Unit::UC_NONE;

	switch( eClass )	// June byte table 32b87fec: every class but 120 / 121 / 125 gets 0, Dark Knight included
	{
	case CX2Unit::UC_ELESIS_GRAND_MASTER:
		m_iSkillCutInSetIndex = 1;
		break;
	case CX2Unit::UC_ELESIS_BLAZING_HEART:
		m_iSkillCutInSetIndex = 2;
		break;
	case CX2Unit::UC_ELESIS_CRIMSON_AVENGER:
		m_iSkillCutInSetIndex = 3;
		break;
	default:
		m_iSkillCutInSetIndex = 0;
		break;
	}

	InitStateID();				// June inlines it @32b87af5
	InitStateCommon();			// June 32b81be0
	InitStateByUnitClass();		// June 32b864f0 ( June runs InitEventStates first; their ids do not meet )

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
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, WinStateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RandomLoseEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_LOSE;
	m_LuaManager.MakeTableReference( strLoseTableNameUTF8.c_str(), stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, LoseStateStart );
	m_StateList[stateData.stateID] = stateData;

	InitWaitStateChange( "ESI_WAIT_STATE_CHANGE" );	// June @32b87ed1 ( id 0x82, checklist 7h )

	InitPassiveSkillState();					// June vslot 0x690
	InitEquippedActiveSkillState( false );		// June vslot 0x684
	CX2GUUser::InitState();						// June 31fd8f00
	// char-elesis CH1: not June, the done-gate line ( June's script, its ESI wait-state-change table and the motion loaded )
	CX2OfflineLog::Server( L"[JUNE_CHAR] ELESIS DATA class %d script %s motion %s states %d wait_state_change %d", static_cast<int>( eClass ),
		m_ScriptFileName.c_str(), NULL != m_pMotion ? m_pMotion->GetDeviceID().c_str() : L"(none)", static_cast<int>( m_StateList.size() ),
		m_StateList.end() != m_StateList.find( static_cast<UserUnitStateDataMap::key_type>( USI_WAIT_STATE_CHANGE ) ) ? 1 : 0 );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	switch ( GetUnitClass() )
	{
	case CX2Unit::UC_ELESIS_KNIGHT:
	case CX2Unit::UC_ELESIS_SABER_KNIGHT:
	case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
		{
			m_iSkillCutInSetIndex = 0;
		} break;
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	case CX2Unit::UC_ELESIS_GRAND_MASTER:
		{
			m_iSkillCutInSetIndex = 1;
		} break;
	case CX2Unit::UC_ELESIS_BLAZING_HEART:
		{
			m_iSkillCutInSetIndex = 2;
		} break;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
	default:
	case CX2Unit::UC_NONE:
		{
			ASSERT( !L"Unit Class Is fault!" );
			m_iSkillCutInSetIndex = 0;
		} break;
	}

	InitStateID();
	InitStateCommon();
	InitStateByUnitClass();

	std::string strStartTableNameUTF8;
	std::string strWinTableNameUTF8;
	std::string strLoseTableNameUTF8;

	InitStateCommonRandom( strStartTableNameUTF8, strWinTableNameUTF8, strLoseTableNameUTF8 );

	UserUnitStateData stateData;

	stateData.Init();
	stateData.stateID			= USI_START;
	m_LuaManager.MakeTableReference( strStartTableNameUTF8.c_str(), stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_WIN;
	m_LuaManager.MakeTableReference( strWinTableNameUTF8.c_str(), stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, WinStateStart );	
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_LOSE;
	m_LuaManager.MakeTableReference( strLoseTableNameUTF8.c_str(), stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, LoseStateStart );	
	m_StateList[stateData.stateID] = stateData;

	InitPassiveSkillState();
	InitEquippedActiveSkillState();

	CX2GUUser::InitState();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
/*virtual*/ void CX2GUEL::InitComponent()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1h: June 32b81710
	CX2GUUser::InitComponent();

	if( true == IsMyUnit() )
	{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
		// PvP or PvE both start the Way of the Sword from zero ( June tests game type 1 or 4 )
		if( true == g_pX2Game->GetIsNowPVPCorrectionGame() )
		{
			static_cast<CX2ElesisGageData*>(m_pGageData)->SetWayOfSwordPoint( 0.f );
			static_cast<CX2ElesisGageData*>(m_pGageData)->SetWayOfSwordState( 0 );
		}
		m_FrameDataFuture.syncData.m_CannonBallCount = GetWayOfSwordState();

		switch ( GetWayOfSwordState() )
		{
		case WSS_VIGOR:
			{
				Start_SwordmanVigor();
			} break;
		case WSS_DESTRUCTION:
			{
				Start_SwordmanDestruction();
			} break;
		default:
			break;
		}

		// June 324943c0 / 32489220 / vslot 0x5c ( 32e69e70 ): the gauge refresh after the mode start
		const CX2MyGageUI* pMyGageUI = CX2GageManager::GetInstance()->GetMyGageUI();
		if( NULL != pMyGageUI )
		{
			const_cast<CX2MyGageUI*>( pMyGageUI )->RefreshWayOfSwordUI();
		}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
		ASSERT( NULL != g_pX2Game );
		switch ( g_pX2Game->GetGameType() )
		{
		case CX2Game::GT_PVP:
			{
				static_cast<CX2ElesisGageData*>(m_pGageData)->SetWayOfSwordPoint( 0.f );
				static_cast<CX2ElesisGageData*>(m_pGageData)->SetWayOfSwordState( 0 );
				m_FrameDataFuture.syncData.m_CannonBallCount = GetWayOfSwordState();
			} break;

		default:
			{
				m_FrameDataFuture.syncData.m_CannonBallCount = GetWayOfSwordState();
			} break;
		}

		switch ( GetWayOfSwordState() )
		{
		case WSS_VIGOR:
			{
				Start_SwordmanVigor();
			} break;
		case WSS_DESTRUCTION:
			{
				Start_SwordmanDestruction();
			} break;
		default:
			break;
		}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CX2GUUser::InitComponent();

#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
	if( true == IsMyUnit() )
	{
		ASSERT( NULL != g_pX2Game );
		switch ( g_pX2Game->GetGameType() )
		{
		case CX2Game::GT_PVP:
			{
				static_cast<CX2ElesisGageData*>(m_pGageData)->SetWayOfSwordPoint( 0.f );
				static_cast<CX2ElesisGageData*>(m_pGageData)->SetWayOfSwordState( 0 );
				m_FrameDataFuture.syncData.m_CannonBallCount = GetWayOfSwordState();				
			} break;

		default:
			{
				m_FrameDataFuture.syncData.m_CannonBallCount = GetWayOfSwordState();
			} break;
		}

		switch ( GetWayOfSwordState() )
		{
		case WSS_VIGOR:
			{
				Start_SwordmanVigor();
			} break;
		case WSS_DESTRUCTION:
			{
				Start_SwordmanDestruction();
			} break;
		default:
			break;
		}
	}
#endif // ELSWORD_WAY_OF_SWORD
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
/*virtual*/ void CX2GUEL::InitPassiveSkillState()
{
	CX2GUUser::InitPassiveSkillState();
 	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
 	CX2SkillTree* const pSkillTree = g_pData->GetSkillTree();
 
	// 집중력 향상 초기화
	SetAddMagicAttackCriticalDamageRate(0.f);
	SetAddMagicAttackCriticalRate(0.f);
	// 순발력 향상 초기화
	SetAddAnimSpeedFactor(0.f);

	// 불의 친화 초기화
	SetFireBallPowerRate(1.f);

	// 기사의 숙명 버프 ID 초기화
	m_uiChivalryAttackBuffFactorID = 0;
	m_uiChivalryDefenceBuffFactorID = 0;
	m_uiChivalryLevel = 0;

	// 닷지&슬래시 재사용 횟수
	m_uiDodgeAndSlashMaxReuseCount = 0;

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	// 불꽃의 낙인 초기화
	m_fStigmaOfFireCoolTime = 0.f;
	m_bIsStigmaOfFire = false;
	m_uiSkillLevelStigmaOfFire = 0;
	m_fStigmaOfFireDamageValue = 1.f;
	// 불꽃 개화 초기화
	m_eFireBlossomsSaveDebuffID = BTI_NONE;
	m_fFireBlossomsCoolTime = 0.f;
	m_fFireBlossomsCheckTime = -1.f;
	m_bIsFireBlossoms = false;
	m_uiFireBlossomsSkillLevel = 0;
	// 꺼지지 않는 불꽃 초기화
	m_bIsLearnUnextinguishableFire = false;
	//파이어 월 유지시간
	m_fWallOfProminenceLifeTime = 0.f;
	// 홍련의 검 초기화
	m_uiSkillLevelFlameSword = 0;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

	// 노전직 공용 스킬
	{
		// 닷지&슬래시
		{
			const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_A_EL_DODGE_AND_SLASH;
			const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
			if ( 0 < iSkillLevel )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
				if( NULL != pSkillTemplet )
				{
					// 스킬 재사용 횟수 
					m_uiDodgeAndSlashMaxReuseCount = static_cast<UINT>(pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_COUNT, iSkillLevel  ));
				}
			}
		}
	}

	switch( GetUnitClass() )
	{
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	case CX2Unit::UC_ELESIS_GRAND_MASTER :
		{
			// 상급 기사 숙련
			{
				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_SGM_ADVANCED_KNIGHT_MASTERY;
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
				if ( 0 < iSkillLevel )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );

					if( NULL != pSkillTemplet )
					{
						// 재빠른 기상 가능 여부
						m_bAbleQuickStand = true;

						// 기회의 포착이 있다면, 재빠른 기상 후 공격이 가능하다.
						int iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_COMMON_GET_CHANCE, true );
						if( iSkillLevel > 0 )
						{
							m_bAbleQuickStandAttack = true;
						}
					}
				}
			}
			// 검의 왈츠
			{

				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_SGM_WALTZ_OF_SWORD;
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
				if ( 0 < iSkillLevel )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );

					if( NULL != pSkillTemplet )
					{
						// 러싱 소드 루프 횟수 증가
						m_iRushingSwordAddLoopValue = static_cast<int> ( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_COUNT, iSkillLevel  ) );

						// 러싱 소드 MP 소모량 감소
						m_iDecreaseMPRushingSword = static_cast<int> ( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL, iSkillLevel ) );
					}
				}
			}

			// 선수 필승
			{
				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_SGM_FIRST_STRIKE;
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
				if ( 0 < iSkillLevel )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );

					if( NULL != pSkillTemplet )
					{
						float fStopTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_TIME, iSkillLevel  );
						float fAddDamage = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel  );
						float fIncreaseMPRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_REGENERATION_ABS, iSkillLevel  );

						m_cFirstStrikeData.SetFirstStrikeData( fStopTime, fAddDamage, fIncreaseMPRate );
					}
				}
			}
		} // break 있으면 안됨.
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	// 1-1 세이버 나이트
	case CX2Unit::UC_ELESIS_SABER_KNIGHT:
		{
			// 순발력 향상
			{
 				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_SSK_AGILITY_MASTERY;
 				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
 				if ( 0 < iSkillLevel )
 				{
 					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
 
 					if( NULL != pSkillTemplet )
 					{
 						// 이동 속도 증가
 						const float fMoveSpeed = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MOVE_SPEED, iSkillLevel  );
 				
						
#ifdef FIXED_INCREASE_MOVE_SPEED_INITIALIZING // 130925 김종훈, 이동속도 증가 효과가 적용되지 않는 문제 수정
						m_AdditionalWalkSpeed.SetPassiveMultiple( m_AdditionalWalkSpeed.GetPassiveMultiple() * ( 1.f + fMoveSpeed ) );
						m_AdditionalDashSpeed.SetPassiveMultiple( m_AdditionalDashSpeed.GetPassiveMultiple() * ( 1.f + fMoveSpeed ) );
#else //  FIXED_INCREASE_MOVE_SPEED_INITIALIZING // 130925 김종훈, 이동속도 증가 효과가 적용되지 않는 문제 수정
						if( 0 > fMoveSpeed)
						{
							m_PhysicParam.fWalkSpeed += m_PhysicParam.fWalkSpeed *= fMoveSpeed;
							m_PhysicParam.fWalkSpeed += m_PhysicParam.fRunSpeed *= fMoveSpeed;
						}
#endif // FIXED_INCREASE_MOVE_SPEED_INITIALIZING // 130925 김종훈, 이동속도 증가 효과가 적용되지 않는 문제 수정

						
						// 동작 속도 증가
						float fAddAnimSpeedFactor = CalculateIncreasingRate( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ANI_SPEED, iSkillLevel  ) );
 						SetAddAnimSpeedFactor( max(0.f, fAddAnimSpeedFactor) );
 					}
 				}
			}
			// 기사의 숙명
			{
 				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_SSK_CHIVALRY;
 				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
 				if ( 0 < iSkillLevel )
 				{
					m_uiChivalryLevel = static_cast<UINT>(iSkillLevel);
 					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
 					if( NULL != pSkillTemplet && 
 						2 <= pSkillTemplet->m_vecBuffFactorID.size() )
 					{					
  						m_uiChivalryAttackBuffFactorID = pSkillTemplet->m_vecBuffFactorID[0];
  						m_uiChivalryDefenceBuffFactorID = pSkillTemplet->m_vecBuffFactorID[1];
 					}
 				}
			}
		} break;	
		
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	// 2-2 블레이징 하트
	case CX2Unit::UC_ELESIS_BLAZING_HEART:
		{
			// 불꽃 개화
			{
				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_SBH_FIRE_BLOSSOMS;
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
				if( 0 < iSkillLevel )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
					if( NULL != pSkillTemplet  )
					{	
						//재사용시간 저장
						m_fFireBlossomsCheckTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillLevel  );
						//스킬 레벨 저장
						m_uiFireBlossomsSkillLevel = iSkillLevel;
						//처음엔 충전된 상태로 시작
						m_bIsFireBlossoms = true;
					}
				}
			}

			// 불꽃의 낙인
			{
				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_SBH_STIGMA_OF_FIRE;
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
				if ( 0 < iSkillLevel )
				{
					m_uiSkillLevelStigmaOfFire = iSkillLevel;
					InsertDelegateProcessToAttackResult( SET_DELEGATE_PROCESS_WITH_DAMAGE_DATA(CX2GUEL, ApplyDebuffStigmaOfFire ) );

					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
					if( NULL != pSkillTemplet  )
					{	
						//스킬 레벨에 따른 데미지 증가량 저장
						m_fStigmaOfFireDamageValue = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel  );
					}
				}
			}
			
			// 홍련의 검
			{
				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_A_SBH_FLAME_SWORD;
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
				if ( 0 < iSkillLevel )
				{
					m_uiSkillLevelFlameSword = iSkillLevel;
					InsertDelegateProcessToAttackResult( SET_DELEGATE_PROCESS_WITH_DAMAGE_DATA(CX2GUEL, ApplyDebuffFlameSword ) );
				}
			}

			// 꺼지지 않는 불꽃
			{
				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_SBH_UNEXTINGUISHABLE_FIRE;
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
				if ( 0 < iSkillLevel )
				{
					m_bIsLearnUnextinguishableFire = true;
				}
			}

			// 파이어 월
			{
				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_SA_SBH_WALL_OF_PROMINENCE;
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
				if( 0 < iSkillLevel )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
					if( NULL != pSkillTemplet  )
					{	
						// 유지시간 저장
						m_fWallOfProminenceLifeTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillLevel  );
					}		
				}				
			}


		} // break 있으면 안됨.
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
	// 1-2 파이로 나이트
	case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
		{
			// 집중력 향상
			{
 				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_SPK_CONCENTRATION_MASTERY;
 				
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				// 패시브 스킬인데 스킬 증감 효과 받는 부분 수정
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID, true );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
 				if ( 0 < iSkillLevel )
 				{
 					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
 					if( NULL != pSkillTemplet )
 					{
 						// 마법 공격력 크리티컬 데미지 배율 증가
						SetAddMagicAttackCriticalDamageRate( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_CRITICAL_DAM_ABS, iSkillLevel  ) );
 						// 마법 공격력 크리티컬 발동 확률 증가
 						SetAddMagicAttackCriticalRate( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_CRITICAL_RATE_ABS, iSkillLevel  ) );
 					}
 				}
			}
			// 불의 신체
			{
 #ifndef CHUNG_FIRST_CLASS_CHANGE
 				불의 신체 패시브 오류! CX2GUUser::m_EnchantStatFromPassiveSkill 를 사용 할 수 없는 상태
 #endif //CHUNG_FIRST_CLASS_CHANGE
 				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_SPK_BODY_OF_FIRE;
 				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
 				if ( 0 < iSkillLevel )
 				{
 					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
 					if( NULL != pSkillTemplet )
 					{
 						// 파이어볼 공격력 증가
						SetFireBallPowerRate( GetFireBallPowerRate() * pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel  ) );
 
 						// 불 저항 증가방법은 SA_FIRE_DEFENCE_ABS 검색 통해서 확인 하면 됨.
 					}
 				}
			}
		} break;
	default:
		break;
	}
}
/*virtual*/ void CX2GUEL::SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ )
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1n: June 32b8c470 up to its id switch; its batch 3207c1a0 / 31fdf970 only re-protects the edited entries ( anti-tamper ), nothing to port
	if ( iSlotIndex_ < 0 || iSlotIndex_ > 4 )
		return;

	const int iNormalStateID = ( false == bSlotB_ ? _CONST_GUUSER_::aNormalSkillStateID[iSlotIndex_] : _CONST_GUUSER_::aNormalSkillStateIDSlotB[iSlotIndex_] );	// June 334f8238 / 334f8260
	UserUnitStateData& normalStateData = m_StateList[ iNormalStateID ];
	normalStateData.Init();		// June's 31fd2af0 snapshot before it is the engine's ( checklist 7i )
	normalStateData.stateID = iNormalStateID;	// June's +0x11 = 1 store is IsSkillSlotState ( X2GUUser.cpp )

	const int iHyperStateID = ( false == bSlotB_ ? _CONST_GUUSER_::aHyperSkillStateID[iSlotIndex_] : _CONST_GUUSER_::aHyperSkillStateIDSlotB[iSlotIndex_] );	// June 334f824c / 334f8274
	UserUnitStateData& hyperStateData = m_StateList[ iHyperStateID ];
	hyperStateData.Init();
	hyperStateData.m_bHyperState = true;	// June word +0x10 = 0x101
	hyperStateData.stateID = iHyperStateID;

	const CX2SkillTree::SkillTemplet* pSkillTempletEquipped = GetEquippedActiveSkillTemplet( eSkillID_, true );
	if ( NULL == pSkillTempletEquipped )
		return;

	CX2SkillTree::SKILL_ID eStateSkillID = eSkillID_;	// the id switch key: June's memo arm jumps into the 28010 / 28016 case bodies ( @32b8d0f4 / @32b8d355 )
	const int iTranscendenceSkillID = GetTranscendenceMemoSkillID( eSkillID_ );
	if ( 0 != iTranscendenceSkillID && NULL != g_pData->GetSkillTree()->GetSkillTemplet( static_cast<CX2SkillTree::SKILL_ID>( iTranscendenceSkillID ) ) )
	{
		switch ( iTranscendenceSkillID )	// June @32b8c5bb: 28031-28061, only these two skills register, every other transcended skill returns
		{
		case CX2SkillTree::RNSI_SA_SGM_JUGGERNAUT_BUSTER_TRANS:
		case CX2SkillTree::RNSI_SA_SGM_JUGGERNAUT_BUSTER_TRANS_UPGRADE:
			eStateSkillID = CX2SkillTree::RNSI_SA_SGM_JUGGERNAUT_BUSTER;	// June 32b8c5c2 = the 28010 / 28048 case's registration
			break;

		case CX2SkillTree::RNSI_SA_SBH_BLAZING_DANCE_TRANS:
		case CX2SkillTree::RNSI_SA_SBH_BLAZING_DANCE_TRANS_UPGRADE:
			eStateSkillID = CX2SkillTree::RNSI_SA_SBH_BLAZING_DANCE;	// June 32b8c72f = the 28016 / 28052 case's registration
			break;

		default:
			return;
		}
	}
	else
	{
		switch ( pSkillTempletEquipped->m_eType )
		{
		case CX2SkillTree::ST_BUFF:
			{
				m_LuaManager.MakeTableReference( "ESI_COMMON_BUFF", normalStateData.stateID );
				normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, COMMON_BUFF_FrameMove );	// June 329ccd90 = vslot 0x5f0 ( 32b6def0 )
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );	// June 329cba40 = 31fcbd60

				m_LuaManager.MakeTableReference( "ESI_COMMON_BUFF", hyperStateData.stateID );
				hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
				hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
			} return;

		case CX2SkillTree::ST_ACTIVE:
		case CX2SkillTree::ST_SPECIAL_ACTIVE:
		case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
		case CX2SkillTree::ST_SPECIAL_ACTIVE_GRADE_1:
		case CX2SkillTree::ST_SPECIAL_ACTIVE_GRADE_2:
		case CX2SkillTree::ST_SPECIAL_ACTIVE_GRADE_3:
			break;	// June's hyper split ( 28600 and up ) is CH3d's; its id switch takes the raw id, March's cases until CH2-CH3

		case CX2SkillTree::ST_RELATIONSHIP_SKILL:
			{
				if ( CX2SkillTree::SI_ETC_WS_COMMON_LOVE == eSkillID_ )
				{
					m_LuaManager.MakeTableReference( "ESI_THROW_ITEM", normalStateData.stateID );
					normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_FrameMove );		// June 329ccda0 = vslot 0x5f4
					normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_EventProcess );	// June 329ccdb0 = vslot 0x5f8

					m_LuaManager.MakeTableReference( "ESI_THROW_ITEM", hyperStateData.stateID );
					hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
					hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
				}
			} return;

		default:
			return;
		}
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( iSlotIndex_ < 0 || iSlotIndex_ >= ARRAY_SIZE( _CONST_GUUSER_::aNormalSkillStateID ) )
		return;

	int iNormalStateID = ( bSlotB_ == false ? _CONST_GUUSER_::aNormalSkillStateID[iSlotIndex_] : _CONST_GUUSER_::aNormalSkillStateIDSlotB[iSlotIndex_] );
	UserUnitStateData& normalStateData = m_StateList[ iNormalStateID ];
	normalStateData.Init();
	normalStateData.stateID = iNormalStateID;

	int iHyperStateID = ( bSlotB_ == false ? _CONST_GUUSER_::aHyperSkillStateID[iSlotIndex_] : _CONST_GUUSER_::aHyperSkillStateIDSlotB[iSlotIndex_] );
	UserUnitStateData& hyperStateData  = m_StateList[ iHyperStateID ];
	hyperStateData.Init();	
	hyperStateData.m_bHyperState = true;
	hyperStateData.stateID = iHyperStateID;

	const CX2SkillTree::SkillTemplet* pSkillTempletEquipped = GetEquippedActiveSkillTemplet( eSkillID_ );
	if( NULL == pSkillTempletEquipped )
		return;

	switch( pSkillTempletEquipped->m_eType )
	{	
	case CX2SkillTree::ST_BUFF:
		{	// 버프
			m_LuaManager.MakeTableReference( "ESI_COMMON_BUFF", normalStateData.stateID );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, COMMON_BUFF_FrameMove );	
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );	

			m_LuaManager.MakeTableReference( "ESI_COMMON_BUFF", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;

			return;
		} break;
	case CX2SkillTree::ST_ACTIVE:
	case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef FINALITY_SKILL_SYSTEM //JHKang
	case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif //FINALITY_SKILL_SYSTEM
		{
			// ST_ACTIVE, ST_SPECIAL_ACTIVE는 아래 구문에서 수행
		} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2SkillTree::ST_RELATIONSHIP_SKILL:
		{
			switch( eSkillID_ )
			{
			case CX2SkillTree::SI_ETC_WS_COMMON_LOVE:
				{
					m_LuaManager.MakeTableReference( "ESI_THROW_ITEM", normalStateData.stateID );
					normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_FrameMove );	
					normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_EventProcess );	

					m_LuaManager.MakeTableReference( "ESI_THROW_ITEM", hyperStateData.stateID );
					hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
					hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
				} break;
			}
			return;
		} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
	default:
		{
			return;
		} break;
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////// 여기서 부터 ACTIVE, SPECIAL ACTIVE 스킬//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region ACTIVE, SPECIAL ACTIVE 스킬
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL_DEBUG
	CX2OfflineLog::Server( L"[JUNE_CHAR] ELESIS SKILL equip skill %d slot %d bank %c state %d", static_cast<int>( eStateSkillID ), iSlotIndex_, ( false == bSlotB_ ? L'A' : L'B' ), iNormalStateID );	// char-elesis CH2a
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL_DEBUG
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	switch ( eStateSkillID )	// char-elesis CH1n: June's raw id, or the memo arm's skill
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	switch ( eSkillID_ )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	{
	case CX2SkillTree::SI_SA_SSK_SPIRAL_BLAST:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_EL_SPIRAL_BLAST:	// char-elesis CH2a: June 32b8c87c
	case CX2SkillTree::RNSI_SA_EL_SPIRAL_BLAST_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{	/// 스파이럴 블래스트
			m_LuaManager.MakeTableReference( "ESI_SI_SA_SSK_SPIRAL_BLAST", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_SPIRAL_BLAST_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_SPIRAL_BLAST_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_SA_SSK_SPIRAL_BLAST", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_SA_SSK_EXTINCTION:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_SSK_EXTINCTION:	// char-elesis CH2b: June 32b8c470 @311
	case CX2SkillTree::RNSI_SA_SSK_EXTINCTION_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{ // 세이버 - 익스팅션
			m_LuaManager.MakeTableReference( "ESI_SI_SA_SSK_EXTINCTION_START", normalStateData.stateID );

			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_START_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_START_Start );
			normalStateData.StateStartFuture= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_START_StartFuture );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_START_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_SA_SSK_EXTINCTION_START", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_SA_SSK_EXTINCTION_CHARGE;
				m_LuaManager.MakeTableReference( "ESI_SI_SA_SSK_EXTINCTION_CHARGE", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_CHARGE_FrameMove );
				stateData.OnFrameMoveFuture = SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_CHARGE_FrameMoveFuture );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_CHARGE_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_SA_SSK_EXTINCTION_ATTACK;
				m_LuaManager.MakeTableReference( "ESI_SI_SA_SSK_EXTINCTION_ATTACK", stateData.stateID );
				stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_ATTACK_StartFuture );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_ATTACK_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_ATTACK_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP;
				m_LuaManager.MakeTableReference( "ESI_SI_SA_SSK_EXTINCTION_DEGREE", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP_FrameMove );
				stateData.OnFrameMoveFuture = SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP_FrameMoveFuture );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN;
				m_LuaManager.MakeTableReference( "ESI_SI_SA_SSK_EXTINCTION_DEGREE", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN_FrameMove );
				stateData.OnFrameMoveFuture = SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN_FrameMoveFuture );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
	case CX2SkillTree::SI_SA_SSK_VICTORIOUS_SWORD:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_SGM_VICTORIOUS_SWORD:	// char-elesis CH3a: June 32b8c470 @421
	case CX2SkillTree::RNSI_SA_SGM_VICTORIOUS_SWORD_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{	/// 승리의 검
			m_LuaManager.MakeTableReference( "ESI_SI_SA_SSK_VICTORIOUS_SWORD", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_VICTORIOUS_SWORD_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_VICTORIOUS_SWORD_EventProcess );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_VICTORIOUS_SWORD_FrameMove );

			m_LuaManager.MakeTableReference( "ESI_SI_SA_SSK_VICTORIOUS_SWORD", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_SA_SSK_JUDGEMENT_DRIVE:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_SSK_JUDGEMENT_DRIVE:	// char-elesis CH2b: June 32b8c470 @387
	case CX2SkillTree::RNSI_SA_SSK_JUDGEMENT_DRIVE_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{ // 저지먼트 드라이브
			m_LuaManager.MakeTableReference( "ESI_SI_SA_SSK_JUDGEMENT_DRIVE", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_JUDGEMENT_DRIVE_Init );
#ifdef ADD_MEMO_1ST_CLASS //김창한
			normalStateData.StateStartFuture = SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_JUDGEMENT_DRIVE_StateStartFuture );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_JUDGEMENT_DRIVE_StateStart );
#endif //ADD_MEMO_1ST_CLASS
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_JUDGEMENT_DRIVE_EventProcess );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_JUDGEMENT_DRIVE_FrameMove );	// char-elesis CH2b: June 32b8c470 @32b8cd94
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

			m_LuaManager.MakeTableReference( "ESI_SI_SA_SSK_JUDGEMENT_DRIVE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
#ifdef ADD_MEMO_1ST_CLASS //김창한
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			hyperStateData.StateStart			= normalStateData.StateStart;
#endif //ADD_MEMO_1ST_CLASS
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			{	// char-elesis CH2b: June 32b8c470 @32b8ce09 ( state 223 )
				UserUnitStateData stateData;
				stateData.Init();
				stateData.stateID			= ESI_SI_SA_SSK_JUDGEMENT_DRIVE_FINAL;
				m_LuaManager.MakeTableReference( "ESI_SI_SA_SSK_JUDGEMENT_DRIVE_FINAL", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_JUDGEMENT_DRIVE_FINAL_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SSK_JUDGEMENT_DRIVE_FINAL_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		} break;
	case CX2SkillTree::SI_SA_SPK_BURST_RISING:
		{	/// 버스트 라이징
			m_LuaManager.MakeTableReference( "ESI_SI_SA_SPK_BURST_RISING", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SPK_BURST_RISING_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SPK_BURST_RISING_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_SA_SPK_BURST_RISING", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_SA_SPK_BURST_WAVE:
 		{ // 버스트 웨이브
 			m_LuaManager.MakeTableReference( "ESI_SI_SA_SPK_BURST_WAVE", normalStateData.stateID );
 			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SPK_BURST_WAVE_Init );
 			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SPK_BURST_WAVE_EventProcess );
 
 			m_LuaManager.MakeTableReference( "ESI_SI_SA_SPK_BURST_WAVE", hyperStateData.stateID );
 			hyperStateData.m_bHyperState		= true;
 			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
 			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
 		} break;
	case CX2SkillTree::SI_SA_SPK_SWORDFIRE:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_SPK_SWORDFIRE:	// char-elesis CH2c: June 32b8c470 @503
	case CX2SkillTree::RNSI_SA_SPK_SWORDFIRE_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{	/// 소드 파이어
			m_LuaManager.MakeTableReference( "ESI_SI_SA_SPK_SWORDFIRE", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SPK_SWORDFIRE_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SPK_SWORDFIRE_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_SA_SPK_SWORDFIRE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			{	// char-elesis CH2c: June 32b8c470 @503 ( the air and the ground state, EventProcess only )
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_SA_SPK_SWORDFIRE_FLY;
				m_LuaManager.MakeTableReference( "ESI_SI_SA_SPK_SWORDFIRE_FLY", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SPK_SWORDFIRE_FLY_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_SA_SPK_SWORDFIRE_GROUND;
				m_LuaManager.MakeTableReference( "ESI_SI_SA_SPK_SWORDFIRE_GROUND", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SPK_SWORDFIRE_GROUND_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		} break;
	case CX2SkillTree::SI_SA_SPK_INFERNAL_BLADE:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_SPK_INFERNAL_BLADE:	// char-elesis CH2c: June 32b8c470 @534
	case CX2SkillTree::RNSI_SA_SPK_INFERNAL_BLADE_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{ // 인페르날 블레이드
			m_LuaManager.MakeTableReference( "ESI_SI_SA_SPK_INFERNAL_BLADE", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SPK_INFERNAL_BLADE_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SPK_INFERNAL_BLADE_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_SA_SPK_INFERNAL_BLADE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_SA_SPK_ETERNAL_FIRE:
		{ // 이터널 파이어
		 	m_LuaManager.MakeTableReference( "ESI_SI_SA_SPK_ETERNAL_FIRE", normalStateData.stateID );
		 	normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SPK_ETERNAL_FIRE_Init );
		 	normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SPK_ETERNAL_FIRE_EventProcess );
		 
		 	m_LuaManager.MakeTableReference( "ESI_SI_SA_SPK_ETERNAL_FIRE", hyperStateData.stateID );
		 	hyperStateData.m_bHyperState		= true;
		 	hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
		 	hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	case CX2SkillTree::SI_SA_SBH_SCALDIC_SWORD:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_SPK_SCALDIC_SWORD:	// char-elesis CH2c: June 32b8c470 @527
	case CX2SkillTree::RNSI_SA_SPK_SCALDIC_SWORD_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{	//스칼딕 소드
			m_LuaManager.MakeTableReference( "ESI_SI_SA_SBH_SCALDIC_SWORD", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_SCALDIC_SWORD_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_SCALDIC_SWORD_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_SA_SBH_SCALDIC_SWORD", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_SA_SBH_WALL_OF_PROMINENCE:
		{	//파이어 월
			m_LuaManager.MakeTableReference( "ESI_SI_SA_SBH_WALL_OF_PROMINENCE", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_WALL_OF_PROMINENCE_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_WALL_OF_PROMINENCE_EventProcess );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_WALL_OF_PROMINENCE_FrameMove );

			m_LuaManager.MakeTableReference( "ESI_SI_SA_SBH_WALL_OF_PROMINENCE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_SA_SBH_BLAZE_WING:
		{	//블레이즈 윙
			m_LuaManager.MakeTableReference( "ESI_SI_SA_SBH_BLAZE_WING", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZE_WING_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZE_WING_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_SA_SBH_BLAZE_WING", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_SA_SBH_BLAZING_DANCE:
		{	//블레이징 댄스
			m_LuaManager.MakeTableReference( "ESI_SI_SA_SBH_BLAZING_DANCE_START", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_START_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_START_StateStart );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_START_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_SA_SBH_BLAZING_DANCE_START", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;	

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_SA_SBH_BLAZING_DANCE_1ST;
				m_LuaManager.MakeTableReference( "ESI_SI_SA_SBH_BLAZING_DANCE_1ST", stateData.stateID );
				stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_1ST_StartFuture );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_1ST_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_SA_SBH_BLAZING_DANCE_2ND;
				m_LuaManager.MakeTableReference( "ESI_SI_SA_SBH_BLAZING_DANCE_2ND", stateData.stateID );
				stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_2ND_StartFuture );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_2ND_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_SA_SBH_BLAZING_DANCE_3RD;
				m_LuaManager.MakeTableReference( "ESI_SI_SA_SBH_BLAZING_DANCE_3RD", stateData.stateID );
				stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_3RD_StartFuture );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_3RD_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_SA_SBH_BLAZING_DANCE_4TH;
				m_LuaManager.MakeTableReference( "ESI_SI_SA_SBH_BLAZING_DANCE_4TH", stateData.stateID );
				stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_4TH_StartFuture );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_4TH_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_SA_SBH_BLAZING_DANCE_FINISH;
				m_LuaManager.MakeTableReference( "ESI_SI_SA_SBH_BLAZING_DANCE_FINISH", stateData.stateID );
				stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_StartFuture );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_EventProcess );
				stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_StateEnd );
				stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_FrameMoveFuture );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
	case CX2SkillTree::SI_HA_SBH_ANCIENT_FIRE:
		{	//에이션트 파이어
			m_LuaManager.MakeTableReference( "ESI_SI_HA_SBH_ANCIENT_FIRE", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_HA_SBH_ANCIENT_FIRE_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_HA_SBH_ANCIENT_FIRE_EventProcess );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_HA_SBH_ANCIENT_FIRE_FrameMove );

			m_LuaManager.MakeTableReference( "ESI_SI_HA_SBH_ANCIENT_FIRE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;

		} break;
	case CX2SkillTree::SI_A_SBH_FLAME_SWORD:
		{	//홍련의 검
			m_LuaManager.MakeTableReference( "ESI_SI_A_SBH_FLAME_SWORD", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SBH_FLAME_SWORD_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SBH_FLAME_SWORD_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_A_SBH_FLAME_SWORD", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_A_SBH_FLAME_SWORD_FINISH;
				m_LuaManager.MakeTableReference( "ESI_SI_A_SBH_FLAME_SWORD_FINISH", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SBH_FLAME_SWORD_FINISH_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
	case CX2SkillTree::SI_A_SBH_FIREWORKS:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_A_SPK_FIREWORKS:	// char-elesis CH2c: June 32b8c470 @1123
	case CX2SkillTree::RNSI_A_SPK_FIREWORKS_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{	//불놀이
			m_LuaManager.MakeTableReference( "ESI_SI_A_SBH_FIREWORKS", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SBH_FIREWORKS_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SBH_FIREWORKS_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_A_SBH_FIREWORKS", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_A_SBH_SCALET_ROSE:
		{	//불꽃 장미
			m_LuaManager.MakeTableReference( "ESI_SI_A_SBH_SCALET_ROSE", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SBH_SCALET_ROSE_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SBH_SCALET_ROSE_EventProcess );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SBH_SCALET_ROSE_FrameMove );
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SBH_SCALET_ROSE_StateEnd );

			m_LuaManager.MakeTableReference( "ESI_SI_A_SBH_SCALET_ROSE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.StateEnd				= normalStateData.StateEnd;
			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_A_SBH_SCALET_ROSE_SUCCESS;
				m_LuaManager.MakeTableReference( "ESI_SI_A_SBH_SCALET_ROSE_SUCCESS", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SBH_SCALET_ROSE_SUCCESS_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
	case CX2SkillTree::SI_A_SPK_SWORD_ERUPTION:
		{ // 소드 이럽션
			m_LuaManager.MakeTableReference( "ESI_SI_A_SPK_SWORD_ERUPTION", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SPK_SWORD_ERUPTION_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SPK_SWORD_ERUPTION_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_A_SPK_SWORD_ERUPTION", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_A_SPK_BIG_BURST:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_A_SPK_BIG_BURST:	// char-elesis CH2c: June 32b8c470 @1089
	case CX2SkillTree::RNSI_A_SPK_BIG_BURST_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
 	{	/// 대폭살
 		m_LuaManager.MakeTableReference( "ESI_SI_A_SPK_BIG_BURST", normalStateData.stateID );
 		normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SPK_BIG_BURST_Init );
 		normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SPK_BIG_BURST_EventProcess );
 
 		m_LuaManager.MakeTableReference( "ESI_SI_A_SPK_BIG_BURST", hyperStateData.stateID );
 		hyperStateData.m_bHyperState		= true;
 		hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
 		hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
 	} break;
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_A_SPK_AVATAR_OF_THE_FIRE:	// char-elesis CH2c: June 32b8c470 @1104
	case CX2SkillTree::RNSI_A_SPK_AVATAR_OF_THE_FIRE_UPGRADE:
		{
			m_LuaManager.MakeTableReference( "ESI_SI_A_SPK_AVATAR_OF_THE_FIRE", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SPK_AVATAR_OF_THE_FIRE_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SPK_AVATAR_OF_THE_FIRE_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SPK_AVATAR_OF_THE_FIRE_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_A_SPK_AVATAR_OF_THE_FIRE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_SDK_EJECTION_BUSTER:	// char-elesis CH2d: June 32b8c470 @596
	case CX2SkillTree::RNSI_SA_SDK_EJECTION_BUSTER_UPGRADE:
		{
			m_LuaManager.MakeTableReference( "SI_SA_SDK_EJECTION_BUSTER_CHARGE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_EJECTION_BUSTER_CHARGE_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_EJECTION_BUSTER_CHARGE_StateStartFuture );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_EJECTION_BUSTER_CHARGE_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_EJECTION_BUSTER_CHARGE_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_EJECTION_BUSTER_CHARGE_EventProcess );

			m_LuaManager.MakeTableReference( "SI_SA_SDK_EJECTION_BUSTER_CHARGE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			{
				UserUnitStateData stateData;
				stateData.Init();
				stateData.stateID			= SI_SA_SDK_EJECTION_BUSTER_ATTACK;
				m_LuaManager.MakeTableReference( "SI_SA_SDK_EJECTION_BUSTER_ATTACK", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_EJECTION_BUSTER_ATTACK_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_EJECTION_BUSTER_ATTACK_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
	case CX2SkillTree::RNSI_SA_SDK_BRUTAL_CUTTER:	// char-elesis CH2d: June 32b8c470 @630
	case CX2SkillTree::RNSI_SA_SDK_BRUTAL_CUTTER_UPGRADE:
		{
			m_LuaManager.MakeTableReference( "SI_SA_SDK_BRUTAL_CUTTER", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_BRUTAL_CUTTER_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_BRUTAL_CUTTER_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_BRUTAL_CUTTER_EventProcess );

			m_LuaManager.MakeTableReference( "SI_SA_SDK_BRUTAL_CUTTER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::RNSI_SA_SDK_BURNING_PHANTOM:	// char-elesis CH2d: June 32b8c470 @638
	case CX2SkillTree::RNSI_SA_SDK_BURNING_PHANTOM_UPGRADE:
		{
			m_LuaManager.MakeTableReference( "SI_SA_SDK_BURNING_PHANTOM_READY", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_BURNING_PHANTOM_READY_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_BURNING_PHANTOM_READY_StateStart );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_BURNING_PHANTOM_READY_EventProcess );

			m_LuaManager.MakeTableReference( "SI_SA_SDK_BURNING_PHANTOM_READY", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			{
				UserUnitStateData stateData;
				stateData.Init();
				stateData.stateID			= SI_SA_SDK_BURNING_PHANTOM_START;
				m_LuaManager.MakeTableReference( "SI_SA_SDK_BURNING_PHANTOM_START", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_BURNING_PHANTOM_START_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
			{
				UserUnitStateData stateData;
				stateData.Init();
				stateData.stateID			= SI_SA_SDK_BURNING_PHANTOM_LOOP;
				m_LuaManager.MakeTableReference( "SI_SA_SDK_BURNING_PHANTOM_LOOP", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_BURNING_PHANTOM_LOOP_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_BURNING_PHANTOM_LOOP_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
			{
				UserUnitStateData stateData;
				stateData.Init();
				stateData.stateID			= SI_SA_SDK_BURNING_PHANTOM_END;
				m_LuaManager.MakeTableReference( "SI_SA_SDK_BURNING_PHANTOM_END", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, SI_SA_SDK_BURNING_PHANTOM_END_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
	case CX2SkillTree::RNSI_A_SDK_DARK_WAVE:	// char-elesis CH2d: June 32b8c470 @1190
	case CX2SkillTree::RNSI_A_SDK_DARK_WAVE_UPGRADE:
		{
			m_LuaManager.MakeTableReference( "SI_A_SDK_DARK_WAVE", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, SI_A_SDK_DARK_WAVE_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, SI_A_SDK_DARK_WAVE_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, SI_A_SDK_DARK_WAVE_EventProcess );

			m_LuaManager.MakeTableReference( "SI_A_SDK_DARK_WAVE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::RNSI_A_SDK_INJECTION_STIGMA:	// char-elesis CH2d: June 32b8c470 @1198
	case CX2SkillTree::RNSI_A_SDK_INJECTION_STIGMA_UPGRADE:
		{
			m_LuaManager.MakeTableReference( "SI_A_SDK_INJECTION_STIGMA", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, SI_A_SDK_INJECTION_STIGMA_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, SI_A_SDK_INJECTION_STIGMA_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, SI_A_SDK_INJECTION_STIGMA_EventProcess );

			m_LuaManager.MakeTableReference( "SI_A_SDK_INJECTION_STIGMA", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::RNSI_A_SDK_SHADE_SCRATCH:	// char-elesis CH2d: June 32b8c470 @1206
	case CX2SkillTree::RNSI_A_SDK_SHADE_SCRATCH_UPGRADE:
		{
			m_LuaManager.MakeTableReference( "SI_A_SCA_SHADE_SCRATCH", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, SI_A_SCA_SHADE_SCRATCH_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, SI_A_SCA_SHADE_SCRATCH_EventProcess );

			m_LuaManager.MakeTableReference( "SI_A_SCA_SHADE_SCRATCH", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			{
				UserUnitStateData stateData;
				stateData.Init();
				stateData.stateID			= SI_A_SCA_SHADE_SCRATCH_ATTACK;
				m_LuaManager.MakeTableReference( "SI_A_SCA_SHADE_SCRATCH_ATTACK", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, SI_A_SCA_SHADE_SCRATCH_ATTACK_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_SGM_CROSS_SLASH:	// char-elesis CH3a: June 32b8c470 @755 ( no Init; the hyper state takes the normal state's callbacks )
		{
			m_LuaManager.MakeTableReference( "ESI_SA_SGM_CROSS_SLASH", normalStateData.stateID );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_CROSS_SLASH_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SA_SGM_CROSS_SLASH", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			CopyStateCallbacks( hyperStateData, normalStateData );	// June 329cc2c0
		} break;
	case CX2SkillTree::RNSI_SA_SGM_DIVINE_BLADE:	// char-elesis CH3a: June 32b8c470 @763
		{
			m_LuaManager.MakeTableReference( "ESI_SA_SGM_DIVINE_BLADE", normalStateData.stateID );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_DIVINE_BLADE_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SA_SGM_DIVINE_BLADE", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			CopyStateCallbacks( hyperStateData, normalStateData );
		} break;
	case CX2SkillTree::RNSI_SA_SGM_GIGA_BLAST:	// char-elesis CH3a: June 32b8c470 @803 ( June's Init 32b8f1e0 is a bare ret: not registered )
		{
			m_LuaManager.MakeTableReference( "ESI_SA_SGM_GIGA_BLAST", normalStateData.stateID );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_GIGA_BLAST_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SA_SGM_GIGA_BLAST", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			CopyStateCallbacks( hyperStateData, normalStateData );
			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ESI_SA_SGM_GIGA_BLAST_CHARGE;
				m_LuaManager.MakeTableReference( "ESI_SA_SGM_GIGA_BLAST_CHARGE", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_GIGA_BLAST_CHARGE_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SA_SGM_GIGA_BLAST_END;
				m_LuaManager.MakeTableReference( "ESI_SA_SGM_GIGA_BLAST_END", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );	// June 329cbfc0 > 31fcc740
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
	case CX2SkillTree::RNSI_A_SGM_UPPER_SLASH:	// char-elesis CH3a2: June 32b8c470 @1264 ( EventProcess only; the hyper state takes the normal state's callbacks )
		{
			m_LuaManager.MakeTableReference( "ESI_A_SGM_UPPER_SLASH", normalStateData.stateID );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_UPPER_SLASH_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_A_SGM_UPPER_SLASH", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			CopyStateCallbacks( hyperStateData, normalStateData );
		} break;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::SI_A_SSK_CROSS_SLASH:
		{	/// 크로스 슬래시
			m_LuaManager.MakeTableReference( "ESI_SI_A_SSK_CROSS_SLASH", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SSK_CROSS_SLASH_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_SSK_CROSS_SLASH_EventProcess );
			
			m_LuaManager.MakeTableReference( "ESI_SI_A_SSK_CROSS_SLASH", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_A_SSK_POWER_BREAK:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_A_SSK_POWER_BREAK:	// char-elesis CH2b2: June 32b8c470 @954
	case CX2SkillTree::RNSI_A_SSK_POWER_BREAK_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{	/// 파워 브레이크
			m_LuaManager.MakeTableReference( "ESI_SI_A_SSK_POWER_BREAK", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL,ESI_SI_A_SSK_POWER_BREAK_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL,ESI_SI_A_SSK_POWER_BREAK_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_A_SSK_POWER_BREAK", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_A_SSK_HEAVY_STUNNER:
		{	/// 헤비 스터너
			m_LuaManager.MakeTableReference( "ESI_SI_A_SSK_HEAVY_STUNNER", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL,ESI_SI_A_SSK_HEAVY_STUNNER_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL,ESI_SI_A_SSK_HEAVY_STUNNER_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_A_SSK_HEAVY_STUNNER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
//////////////////////////////////////////////////////////////////////////////////////////////////////
//아래 노전직 스킬
//////////////////////////////////////////////////////////////////////////////////////////////////////		
	case CX2SkillTree::SI_SA_EL_UNLIMITED_BLADE:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_EL_UNLIMITED_BLADE:	// char-elesis CH2a: June 32b8cac5
	case CX2SkillTree::RNSI_SA_EL_UNLIMITED_BLADE_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{ // 언리미티드 블레이드
			m_LuaManager.MakeTableReference( "ESI_SI_SA_EL_UNLIMITED_BLADE", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_EL_UNLIMITED_BLADE_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_EL_UNLIMITED_BLADE_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_SA_EL_UNLIMITED_BLADE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_SA_EL_WILD_SHOCK:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_EL_WILD_SHOCK:	// char-elesis CH2a: June 32b8c8d3
	case CX2SkillTree::RNSI_SA_EL_WILD_SHOCK_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{	/// 와일드 쇼크
			m_LuaManager.MakeTableReference( "ESI_SI_SA_EL_WILD_SHOCK", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_EL_WILD_SHOCK_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_EL_WILD_SHOCK_EventProcess );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_EL_WILD_SHOCK_FrameMove );	// char-elesis CH2a: June 32b75760; the hyper state copies it
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

			m_LuaManager.MakeTableReference( "ESI_SI_SA_EL_WILD_SHOCK", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_SA_EL_MEGA_BURSTER:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_EL_MEGA_BURSTER:	// char-elesis CH2a: June 32b8c941
	case CX2SkillTree::RNSI_SA_EL_MEGA_BURSTER_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{ // 메가 버스터
			m_LuaManager.MakeTableReference( "ESI_SI_SA_EL_MEGA_BURSTER", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_EL_MEGA_BURSTER_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_EL_MEGA_BURSTER_EventProcess );
	
			m_LuaManager.MakeTableReference( "ESI_SI_SA_EL_MEGA_BURSTER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_SA_EL_POWER_BURSTER:
		{ // 파워 버스터
			m_LuaManager.MakeTableReference( "ESI_SI_SA_EL_POWER_BURSTER", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_EL_POWER_BURSTER_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_SA_EL_POWER_BURSTER_EventProcess );
	
			m_LuaManager.MakeTableReference( "ESI_SI_SA_EL_POWER_BURSTER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_A_EL_SONIC_ASSAULT_STING:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_EL_SONIC_ASSAULT_STING:	// char-elesis CH2a: June 32b8c976
	case CX2SkillTree::RNSI_SA_EL_SONIC_ASSAULT_STING_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{	/// 소닉 어썰트 - 스팅
			m_LuaManager.MakeTableReference( "ESI_SI_A_EL_SONIC_ASSAULT_STING", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_SONIC_ASSAULT_STING_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_SONIC_ASSAULT_STING_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_A_EL_SONIC_ASSAULT_STING", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			{	// char-elesis CH2a: June 32b8c976 registers the three memo states with the case
				UserUnitStateData stateData;
				stateData.Init();
				stateData.stateID			= ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_READY;
				m_LuaManager.MakeTableReference( "ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_READY", stateData.stateID );
				stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_READY_StateStartFuture );
				stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_READY_StateStart );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_READY_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_CHARGE;
				m_LuaManager.MakeTableReference( "ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_CHARGE", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_CHARGE_StateStart );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_CHARGE_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_CHARGE_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_FIRE;
				m_LuaManager.MakeTableReference( "ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_FIRE", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_FIRE_StateStart );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_FIRE_FrameMove );
				stateData.OnEventProcess	= normalStateData.OnEventProcess;	// char-elesis CH2a: June 32b8ca71 copies the skill state's EventProcess ( anim end -> WAIT ); without it the fire state never ends
				m_StateList[stateData.stateID] = stateData;
			}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		} break;
	case CX2SkillTree::SI_A_EL_SONIC_ASSAULT_STAB:
		{	/// 소닉 어썰트 - 스탭
			m_LuaManager.MakeTableReference( "ESI_SI_A_EL_SONIC_ASSAULT_STAB", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_SONIC_ASSAULT_STAB_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_SONIC_ASSAULT_STAB_EventProcess );
	
			m_LuaManager.MakeTableReference( "ESI_SI_A_EL_SONIC_ASSAULT_STAB", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_A_EL_MEGA_SLASH:
		{ // 메가 슬래시
			m_LuaManager.MakeTableReference( "ESI_SI_A_EL_MEGA_SLASH", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_MEGA_SLASH_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_MEGA_SLASH_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_A_EL_MEGA_SLASH", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	case CX2SkillTree::SI_A_EL_KICK:
	{ // 발차기
		m_LuaManager.MakeTableReference( "ESI_SI_A_EL_KICK", normalStateData.stateID );
		normalStateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_KICK_Start );
		normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_KICK_Init );
		normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_KICK_EventProcess );
	
		m_LuaManager.MakeTableReference( "ESI_SI_A_EL_KICK", hyperStateData.stateID );
		hyperStateData.m_bHyperState		= true;
		hyperStateData.StateStart			= normalStateData.StateStart;
		hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
	} break;
	case CX2SkillTree::SI_A_EL_LEAP_CRASH:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_A_SSK_LEAP_CRASH:	// char-elesis CH2b2: June 32b8c470 @961
	case CX2SkillTree::RNSI_A_SSK_LEAP_CRASH_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	{ // 도약
		m_LuaManager.MakeTableReference( "ESI_SI_A_EL_LEAP_CRASH", normalStateData.stateID );
		normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_LEAP_CRASH_Init );
		normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_LEAP_CRASH_EventProcess );
	
		m_LuaManager.MakeTableReference( "ESI_SI_A_EL_LEAP_CRASH", hyperStateData.stateID );
		hyperStateData.m_bHyperState		= true;
		hyperStateData.StateStart			= normalStateData.StateStart;
		hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;

		{
			UserUnitStateData stateData;
			stateData.Init();
			stateData.stateID			= ESI_SI_A_EL_LEAP_CRASH_JUMP;
			m_LuaManager.MakeTableReference( "ESI_SI_A_EL_LEAP_CRASH_JUMP", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_LEAP_CRASH_JUMP_EventProcess );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_LEAP_CRASH_JUMP_StateStartFuture );	// char-elesis CH2b2: June 32b70d30
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_SI_A_EL_LEAP_CRASH_JUMP_LANDING;
			m_LuaManager.MakeTableReference( "ESI_SI_A_EL_LEAP_CRASH_JUMP_LANDING", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_LEAP_CRASH_JUMP_LANDING_EventProcess );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_LEAP_CRASH_JUMP_LANDING_FrameMove );	// char-elesis CH2b2: June 32b75210
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			m_StateList[stateData.stateID] = stateData;
		}
	} break;
	case CX2SkillTree::SI_A_EL_RUSHING_SWORD:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_A_SSK_RUSHING_SWORD:	// char-elesis CH2b2: June 32b8c470 @908
	case CX2SkillTree::RNSI_A_SSK_RUSHING_SWORD_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{ // 러싱 소드
			m_LuaManager.MakeTableReference( "ESI_SI_A_EL_RUSHING_SWORD", normalStateData.stateID );
			normalStateData.StateStartFuture = SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_RUSHING_SWORD_StartFuture );
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
			// 그랜드 마스터 패시브, 검의 왈츠 추가를 위해 StateStart 함수 추가
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_RUSHING_SWORD_StateStart );
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_RUSHING_SWORD_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_RUSHING_SWORD_EventProcess );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			normalStateData.StateEndFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_RUSHING_SWORD_StateEndFuture );	// char-elesis CH2b2: June 32b72d70
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

			m_LuaManager.MakeTableReference( "ESI_SI_A_EL_RUSHING_SWORD", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			hyperStateData.StateEndFuture		= normalStateData.StateEndFuture;	// char-elesis CH2b2: June 32b72d70
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
			// 그랜드 마스터 패시브, 검의 왈츠 추가를 위해 StateStart 함수 추가
			hyperStateData.StateStart			= normalStateData.StateStart;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			{
				UserUnitStateData stateData;
				stateData.Init();
				stateData.stateID			= ESI_SI_A_EL_RUSHING_SWORD_2nd;
				m_LuaManager.MakeTableReference( "ESI_SI_A_EL_RUSHING_SWORD_2nd", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_RUSHING_SWORD_2nd_EventProcess );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
				stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_RUSHING_SWORD_StateStart );	// char-elesis CH2b2: June 32b705b0
				stateData.StateEndFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_RUSHING_SWORD_StateEndFuture );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
				stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_RUSHING_SWORD_2nd_StartFuture );
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_A_EL_RUSHING_SWORD_3rd;
				m_LuaManager.MakeTableReference( "ESI_SI_A_EL_RUSHING_SWORD_3rd", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_RUSHING_SWORD_3rd_EventProcess );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
				stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_RUSHING_SWORD_StateStart );	// char-elesis CH2b2: June 32b705b0
				stateData.StateEndFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_RUSHING_SWORD_StateEndFuture );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
	case CX2SkillTree::SI_A_EL_DODGE_AND_SLASH:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_A_EL_DODGE_AND_SLASH:	// char-elesis CH2a: June 32b8da07
	case CX2SkillTree::RNSI_A_EL_DODGE_AND_SLASH_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{	/// 닷지&슬래시
			m_LuaManager.MakeTableReference( "ESI_SI_A_EL_DODGE_AND_SLASH", normalStateData.stateID );
			normalStateData.StateStartFuture = SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_StartFuture );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SI_A_EL_DODGE_AND_SLASH", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_StateStart );	// char-elesis CH2a: June 32b8da07
			normalStateData.StateEndFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_StateEndFuture );
			hyperStateData.StateStart		= normalStateData.StateStart;
			hyperStateData.StateEndFuture	= normalStateData.StateEndFuture;
			{	// June's re-entry states: 189 and 311 ( back ) count the re-use down, 190 is the Z / X attack; March's second state is not registered
				UserUnitStateData stateData;
				stateData.Init();
				stateData.stateID			= ESI_SI_A_EL_DODGE_AND_SLASH;
				m_LuaManager.MakeTableReference( "ESI_SI_A_EL_DODGE_AND_SLASH", stateData.stateID );
				stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_SECOND_StartFuture );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_EventProcess );
				stateData.StateEndFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_StateEndFuture );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_A_EL_DODGE_AND_SLASH_BACK;
				m_LuaManager.MakeTableReference( "ESI_SI_A_EL_DODGE_AND_SLASH_BACK", stateData.stateID );
				stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_SECOND_StartFuture );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_EventProcess );
				stateData.StateEndFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_StateEndFuture );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z;
				m_LuaManager.MakeTableReference( "ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z_StateStart );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z_EventProcess );
				stateData.StateEndFuture	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_StateEndFuture );
				m_StateList[stateData.stateID] = stateData;
			}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			{
				UserUnitStateData stateData;
				stateData.Init();
				stateData.stateID			= ESI_SI_A_EL_DODGE_AND_SLASH_SECOND;
				m_LuaManager.MakeTableReference( "ESI_SI_A_EL_DODGE_AND_SLASH", stateData.stateID );
				stateData.StateStartFuture  = SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_SECOND_StartFuture );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_FrameMove );	//공용
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_EventProcess ); //공용
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z;
				m_LuaManager.MakeTableReference( "ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		} break;

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	// 그랜드 마스터 액티브
	// 도발
	case CX2SkillTree::SI_A_SGM_PROVOKE :
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_A_SGM_PROVOKE:	// char-elesis CH3a2: June 32b8c470 @989
	case CX2SkillTree::RNSI_A_SGM_PROVOKE_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{
			m_LuaManager.MakeTableReference( "ESI_A_SGM_PROVOKE", normalStateData.stateID );			
			// 도발 시작
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_PROVOKE_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_PROVOKE_StateStart );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_PROVOKE_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_PROVOKE_StateEnd );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_PROVOKE_StateStartFuture );	// char-elesis CH3a2: June 32b8f0b0 > 32b50b00
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			

			m_LuaManager.MakeTableReference( "ESI_A_SGM_PROVOKE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.StateEnd				= normalStateData.StateEnd;
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;	// char-elesis CH3a2
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL


			// 도발 성공
			{
				UserUnitStateData subStateData;
				subStateData.Init();
				subStateData.stateID			= ESI_A_SGM_PROVOKE_REVENGE;
				m_LuaManager.MakeTableReference( "ESI_A_SGM_PROVOKE_REVENGE", subStateData.stateID );
				subStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_PROVOKE_REVENGE_EventProcess );
			
				m_StateList[subStateData.stateID] = subStateData;
			}
		}
		break;
	// 드래스틱 차지
	case CX2SkillTree::SI_A_SGM_DRASTIC_CHARGE:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_A_SGM_DRASTIC_CHARGE:	// char-elesis CH3a2: June 32b8c470 @1021
	case CX2SkillTree::RNSI_A_SGM_DRASTIC_CHARGE_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{
			m_LuaManager.MakeTableReference( "ESI_A_SGM_DRASTIC_CHARGE_START", normalStateData.stateID );			
			// 드래스틱 차지 스타트
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_DRASTIC_CHARGE_START_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_DRASTIC_CHARGE_START_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_DRASTIC_CHARGE_START_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_DRASTIC_CHARGE_START_EventProcess );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_DRASTIC_CHARGE_START_StartFurture );


			m_LuaManager.MakeTableReference( "ESI_A_SGM_DRASTIC_CHARGE_START", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			// 드래스틱 차지 루프
			{
				UserUnitStateData subStateData;
				subStateData.Init();
				subStateData.stateID			= ESI_A_SGM_DRASTIC_CHARGE_LOOP;
				m_LuaManager.MakeTableReference( "ESI_A_SGM_DRASTIC_CHARGE_LOOP", subStateData.stateID );
 				subStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_DRASTIC_CHARGE_LOOP_EventProcess );
				subStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_DRASTIC_CHARGE_LOOP_FrameMove );

				m_StateList[subStateData.stateID] = subStateData;
			}
			// 드래스틱 차지 엔드
			{
				UserUnitStateData subStateData;
				subStateData.Init();
				subStateData.stateID			= ESI_A_SGM_DRASTIC_CHARGE_END;
				m_LuaManager.MakeTableReference( "ESI_A_SGM_DRASTIC_CHARGE_END", subStateData.stateID );
				subStateData.StateStart			= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_DRASTIC_CHARGE_END_StateStart );
				subStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_DRASTIC_CHARGE_END_EventProcess );

				m_StateList[subStateData.stateID] = subStateData;
			}

			if ( NULL != GetUnit() )
			{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
				const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_SGM_DRASTIC_CHARGE, true );	// char-elesis CH3a2: June 32b8e07b ( the equipped upgrade id reaches the base id )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
				const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_SGM_DRASTIC_CHARGE );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
				if ( NULL != pSkillTemplet )
				{
					const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
					const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID, false, 0, true ) );	// char-elesis CH3a2: June 32b8e0a7
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
					const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨	
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
					m_fDrasticChargeLoopDecreaseMPValue = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL, iSkillTempletLevel ); // 유지 시 사용되는 MP 량
				}
			}
		}
		break;
	// 스냅 크래셔
	case CX2SkillTree::SI_A_SGM_SNAP_CRASHER:
		{
			m_LuaManager.MakeTableReference( "SI_A_SGM_SNAP_CRESHER_READY", normalStateData.stateID );			
			// 스냅 크래셔, 공중에서 사용
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_SNAP_CRASHER_START_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_SNAP_CRASHER_START_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_SNAP_CRASHER_START_StateEnd );

			m_LuaManager.MakeTableReference( "SI_A_SGM_SNAP_CRESHER_READY", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.StateEnd				= normalStateData.StateEnd;

			// 스냅 크래셔, 공중에서 사용 후 내려옴
			{
				UserUnitStateData subStateData;
				subStateData.Init();
				subStateData.stateID			= ESI_A_SGM_SNAP_CRASHER_LOOP;
				m_LuaManager.MakeTableReference( "SI_A_SGM_SNAP_CRESHER_LOOP", subStateData.stateID );
				subStateData.StateStart			= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_SNAP_CRASHER_LOOP_StateStart );
				subStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_SNAP_CRASHER_LOOP_EventProcess );
				
				m_StateList[subStateData.stateID] = subStateData;
			}
			// 스냅 크래셔, 공중에서 사용 후 내려와서 땅 찍기
			{
				UserUnitStateData subStateData;
				subStateData.Init();
				subStateData.stateID			= ESI_A_SGM_SNAP_CRASHER_END;
				m_LuaManager.MakeTableReference( "SI_A_SGM_SNAP_CRESHER_END", subStateData.stateID );
				subStateData.StateStart			= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_SNAP_CRASHER_END_StateStart );
				subStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_A_SGM_SNAP_CRASHER_END_EventProcess );
				
				m_StateList[subStateData.stateID] = subStateData;
			}
		}
		break;

		// 액티브 끝

	// 스페셜 액티브 시작
	// 소닉 블레이드
	case CX2SkillTree::SI_SA_SGM_SONIC_BLADE:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_SGM_SONIC_BLADE:	// char-elesis CH3a: June 32b8c470 @496
	case CX2SkillTree::RNSI_SA_SGM_SONIC_BLADE_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{
			m_LuaManager.MakeTableReference( "ESI_SA_SGM_SONIC_BLADE", normalStateData.stateID );			
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_SONIC_BLADE_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_SONIC_BLADE_EventProcess );

			m_LuaManager.MakeTableReference( "ESI_SA_SGM_SONIC_BLADE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		}
		break;
	
	// 크리티컬 다이브
	case CX2SkillTree::SI_SA_SGM_CRITICAL_DIVE:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_SSK_CRITICAL_DIVE:	// char-elesis CH2b: June 32b8c470 @367
	case CX2SkillTree::RNSI_SA_SSK_CRITICAL_DIVE_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{
			m_LuaManager.MakeTableReference( "ESI_SA_SGM_CRITICAL_DIVE", normalStateData.stateID );			
			// 크리티컬 다이브, 시작 ~ 적이 안맞았다면 끝
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_CRITICAL_DIVE_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_CRITICAL_DIVE_EventProcess );
			

			m_LuaManager.MakeTableReference( "ESI_SA_SGM_CRITICAL_DIVE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			// 스냅 크래셔, 공중에서 사용 후 내려와서 땅 찍기
			{
				UserUnitStateData subStateData;
				subStateData.Init();
				subStateData.stateID			= ESI_SA_SGM_CRITICAL_DIVE_HIT;
				m_LuaManager.MakeTableReference( "ESI_SA_SGM_CRITICAL_DIVE_HIT", subStateData.stateID );
				subStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_CRITICAL_DIVE_HIT_EventProcess );
				
				m_StateList[subStateData.stateID] = subStateData;
			}
		}
		break;
	
	// 저거넛 버스터
	case CX2SkillTree::SI_SA_SGM_JUGGERNAUT_BUSTER:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_SGM_JUGGERNAUT_BUSTER:	// char-elesis CH3a: June 32b8c470 @453 ( the memo arm reaches it with the base id )
	case CX2SkillTree::RNSI_SA_SGM_JUGGERNAUT_BUSTER_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	{
		m_LuaManager.MakeTableReference( "ESI_SA_SGM_JUGGERNAUT_BUSTER_START", normalStateData.stateID );			
		// 저거넛 버스터, 시작 ~ 루프 전
		normalStateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_JUGGERNAUT_BUSTER_START_Init );
		normalStateData.StateStart			= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_JUGGERNAUT_BUSTER_START_StateStart );		
		normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_JUGGERNAUT_BUSTER_START_EventProcess );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_JUGGERNAUT_BUSTER_START_FrameMove );	// char-elesis CH3a: June 32b906e0 > 32b77de0
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		

		m_LuaManager.MakeTableReference( "ESI_SA_SGM_JUGGERNAUT_BUSTER_START", hyperStateData.stateID );
		hyperStateData.m_bHyperState		= true;
		hyperStateData.StateInit			= normalStateData.StateInit;
		hyperStateData.StateStart			= normalStateData.StateStart;
		hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;	// char-elesis CH3a: June 32b8c470 @32b8d0b5
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		// 저거넛 버스터, 루프 ~ 끝나기 전
		{
			UserUnitStateData subStateData;
			subStateData.Init();
			subStateData.stateID			= ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP;
			m_LuaManager.MakeTableReference( "ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP", subStateData.stateID );
			subStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_EventProcess );
			subStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_FrameMove );

			m_StateList[subStateData.stateID] = subStateData;
		}
		// 저거넛 버스터, 끝나기 전 ~ 끝
		{
			UserUnitStateData subStateData;
			subStateData.Init();
			subStateData.stateID			= ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST;
			m_LuaManager.MakeTableReference( "ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST", subStateData.stateID );
			subStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST_EventProcess );
			subStateData.StateEnd			= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST_StartEnd );

			m_StateList[subStateData.stateID] = subStateData;
		}
		// 저거넛 버스터, ~ 끝
		{
			UserUnitStateData subStateData;
			subStateData.Init();
			subStateData.stateID			= ESI_SA_SGM_JUGGERNAUT_BUSTER_END;
			m_LuaManager.MakeTableReference( "ESI_SA_SGM_JUGGERNAUT_BUSTER_END", subStateData.stateID );
			subStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_JUGGERNAUT_BUSTER_END_EventProcess );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			subStateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_JUGGERNAUT_BUSTER_END_FrameMove );	// char-elesis CH3a: June 32b906f0 > 32b78170
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

			m_StateList[subStateData.stateID] = subStateData;
		}
	}
	break;
	
	// 전장의 숨결
	case CX2SkillTree::SI_SA_SGM_WAR_PRELUDE:
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	case CX2SkillTree::RNSI_SA_SGM_WAR_PRELUDE:	// char-elesis CH3a fix: June 32b8c470 @429
	case CX2SkillTree::RNSI_SA_SGM_WAR_PRELUDE_UPGRADE:
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		{
			m_LuaManager.MakeTableReference( "ESI_SA_SGM_WAR_PRELUDE", normalStateData.stateID );			
			// 전장의 숨결
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_WAR_PRELUDE_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_WAR_PRELUDE_EventProcess );
			

			m_LuaManager.MakeTableReference( "ESI_SA_SGM_WAR_PRELUDE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
			{	// char-elesis CH3a: June 32b8c470 @429 ( the air state and the ground state, EventProcess only )
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ESI_SA_SGM_WAR_PRELUDE_FLY;
				m_LuaManager.MakeTableReference( "ESI_SA_SGM_WAR_PRELUDE_FLY", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_WAR_PRELUDE_FLY_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESI_SA_SGM_WAR_PRELUDE_GROUND;
				m_LuaManager.MakeTableReference( "ESI_SA_SGM_WAR_PRELUDE_GROUND", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SA_SGM_WAR_PRELUDE_GROUND_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		}
		break;
	// 스페셜 액티브 끝

	// 하이퍼 액티브 시작
	// 소드 오브 렐릭스
	case CX2SkillTree::SI_HA_SGM_SWORD_OF_RELICS:
		{
			m_LuaManager.MakeTableReference( "ESI_HA_SGM_SWORD_OF_RELICS", normalStateData.stateID );			
			// 소드 오브 렐릭스
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_HA_SGM_SWORD_OF_RELICS_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUEL, ESI_HA_SGM_SWORD_OF_RELICS_EventProcess );
			

			m_LuaManager.MakeTableReference( "ESI_HA_SGM_SWORD_OF_RELICS", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		}
		break;

	// 하이퍼 액티브 끝



#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트





	default:
		break;
	}
#pragma endregion ACTIVE, SPECIAL ACTIVE 스킬
}


/*virtual*/ void CX2GUEL::SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ )
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1i: June 32b4e790, 22 blocks in June's order keyed by RNSI_ ids, the upgrade swapped templet, a null templet skips its block
	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_EL_SONIC_ASSAULT_STING == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_EL_SONIC_ASSAULT_STING, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_READY );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_CHARGE );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_FIRE );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_SDK_EJECTION_BUSTER == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_SDK_EJECTION_BUSTER, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, SI_SA_SDK_EJECTION_BUSTER_ATTACK );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_SCA_BRANDISH_BREAKER == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_SCA_BRANDISH_BREAKER, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, SI_SA_SDK_BRANDISH_BREAKER_LOOP );
			SetEquippedSkillLevelStateData( pSkillTemplet, SI_SA_SDK_BRANDISH_BREAKER_FINISH );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_SDK_BURNING_PHANTOM == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_SDK_BURNING_PHANTOM, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, SI_SA_SDK_BURNING_PHANTOM_START );
			SetEquippedSkillLevelStateData( pSkillTemplet, SI_SA_SDK_BURNING_PHANTOM_LOOP );
			SetEquippedSkillLevelStateData( pSkillTemplet, SI_SA_SDK_BURNING_PHANTOM_END );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_SCA_DREAD_WEAPON == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_SCA_DREAD_WEAPON, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, SI_SA_SCA_DREAD_WEAPON_LOOP );
			SetEquippedSkillLevelStateData( pSkillTemplet, SI_SA_SCA_DREAD_WEAPON_END );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_SDK_SHADE_SCRATCH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_SDK_SHADE_SCRATCH, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, SI_A_SCA_SHADE_SCRATCH_ATTACK );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_SPK_SWORDFIRE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_SPK_SWORDFIRE, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SPK_SWORDFIRE_FLY );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SPK_SWORDFIRE_GROUND );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_EL_DODGE_AND_SLASH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_EL_DODGE_AND_SLASH, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_DODGE_AND_SLASH );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_DODGE_AND_SLASH_BACK );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_SSK_RUSHING_SWORD == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_SSK_RUSHING_SWORD, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_RUSHING_SWORD_2nd );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_RUSHING_SWORD_3rd );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_SSK_EXTINCTION == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_SSK_EXTINCTION, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SSK_EXTINCTION_CHARGE );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SSK_EXTINCTION_ATTACK );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP );	// June script ids 220 / 221 share one name
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_EL_ENDURANCE_POWER == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_EL_ENDURANCE_POWER, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_SSK_LEAP_CRASH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_SSK_LEAP_CRASH, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_LEAP_CRASH_JUMP );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_LEAP_CRASH_JUMP_LANDING );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_SSK_JUDGEMENT_DRIVE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_SSK_JUDGEMENT_DRIVE, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SSK_JUDGEMENT_DRIVE_FINAL );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_SGM_JUGGERNAUT_BUSTER == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_SGM_JUGGERNAUT_BUSTER, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_SGM_JUGGERNAUT_BUSTER_END );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_SSK_CRITICAL_DIVE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_SSK_CRITICAL_DIVE, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_SGM_CRITICAL_DIVE_HIT );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_SGM_PROVOKE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_SGM_PROVOKE, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_A_SGM_PROVOKE_REVENGE );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_SGM_DRASTIC_CHARGE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_SGM_DRASTIC_CHARGE, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_A_SGM_DRASTIC_CHARGE_LOOP );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_A_SGM_DRASTIC_CHARGE_END );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_SBH_SCALET_ROSE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_SBH_SCALET_ROSE, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_SBH_SCALET_ROSE_SUCCESS );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_SBH_BLAZING_DANCE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_SBH_BLAZING_DANCE, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SBH_BLAZING_DANCE_1ST );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SBH_BLAZING_DANCE_ATTACK_BACK );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SBH_BLAZING_DANCE_ATTACK_FRONT );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SBH_BLAZING_DANCE_FINISH );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_A_SBH_FLAME_SWORD == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_SBH_FLAME_SWORD, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_SBH_FLAME_SWORD_FINISH );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_SGM_WAR_PRELUDE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_SGM_WAR_PRELUDE, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_SGM_WAR_PRELUDE );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_SGM_WAR_PRELUDE_FLY );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_SGM_WAR_PRELUDE_GROUND );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::RNSI_SA_SGM_GIGA_BLAST == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_SGM_GIGA_BLAST, true );
		if ( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_SGM_GIGA_BLAST_CHARGE );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_SGM_GIGA_BLAST_END );
		}
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_EL_DODGE_AND_SLASH == eSkillID_ )
	{/// 닷지&슬래시
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_EL_DODGE_AND_SLASH );
		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_DODGE_AND_SLASH_SECOND );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z );
		}
	}	
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_EL_RUSHING_SWORD == eSkillID_ )
	{/// 러싱 소드
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_EL_RUSHING_SWORD );
		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_RUSHING_SWORD_2nd );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_RUSHING_SWORD_3rd );
		}
	}	
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_SSK_EXTINCTION == eSkillID_ )
	{/// 세이버 - 익스팅션
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_SSK_EXTINCTION );
		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SSK_EXTINCTION_CHARGE );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SSK_EXTINCTION_ATTACK );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN );
		}
	}
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_EL_ENDURANCE_POWER == eSkillID_ )
	{/// 극기 - 강(强)
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_EL_ENDURANCE_POWER );
		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_MEMO );
			
		}
	}
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_EL_LEAP_CRASH == eSkillID_ )
	{/// 도약
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_EL_LEAP_CRASH );
		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_LEAP_CRASH_JUMP );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_EL_LEAP_CRASH_JUMP_LANDING );
		}
	}
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_SGM_JUGGERNAUT_BUSTER == eSkillID_ )
	{/// 저거넛 버스터
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_SGM_JUGGERNAUT_BUSTER );
		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_SGM_JUGGERNAUT_BUSTER_START );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_SGM_JUGGERNAUT_BUSTER_END );
		}
	}
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_SGM_CRITICAL_DIVE == eSkillID_ )
	{/// 크리티컬 다이브
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_SGM_CRITICAL_DIVE );
		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SA_SGM_CRITICAL_DIVE_HIT );
		}
	}
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_SGM_SNAP_CRASHER == eSkillID_ )
	{/// 스냅 크래셔
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_SGM_SNAP_CRASHER );
		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_A_SGM_SNAP_CRASHER_LOOP );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_A_SGM_SNAP_CRASHER_END );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_A_SGM_SNAP_CRASHER_GROUND );
		}
	}
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_SGM_PROVOKE == eSkillID_ )
	{/// 도발	
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_SGM_PROVOKE );
		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_A_SGM_PROVOKE_REVENGE );
		}
	}
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_SGM_DRASTIC_CHARGE == eSkillID_ )
	{/// 드래스틱 차지	
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_SGM_DRASTIC_CHARGE );
		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_A_SGM_DRASTIC_CHARGE_LOOP );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_A_SGM_DRASTIC_CHARGE_END );

		}
	}


	if ( true == bChangeAll_ || CX2SkillTree::SI_A_SBH_SCALET_ROSE == eSkillID_ )
	{/// 불꽃 장미
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_SBH_SCALET_ROSE );
		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_SBH_SCALET_ROSE_SUCCESS );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_SBH_BLAZING_DANCE == eSkillID_ )
	{/// 블레이징 댄스
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_SBH_BLAZING_DANCE );
		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SBH_BLAZING_DANCE_1ST );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SBH_BLAZING_DANCE_2ND );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SBH_BLAZING_DANCE_3RD );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SBH_BLAZING_DANCE_4TH );
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_SA_SBH_BLAZING_DANCE_FINISH );
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_A_SBH_FLAME_SWORD == eSkillID_ )
	{/// 홍련의 검
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_SBH_FLAME_SWORD );
		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ESI_SI_A_SBH_FLAME_SWORD_FINISH );
		}
	}
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
			 
/*virtual*/ void CX2GUEL::CommonStateStartFuture()
{
	KTDXPROFILE();
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1j: June 32b70180 ( the base runs first )
	CX2GUUser::CommonStateStartFuture();
	m_bIsDashAttackPossible = ( ESI_DASH_JUMP_COMBO_SPK_XZ != GetNowStateID() && ESI_DASH_JUMP_COMBO_SPK_XZ_REATTACK != GetNowStateID() );
	switch( GetNowStateID() )
	{
	case ESI_SI_SA_SPK_SWORDFIRE_FLY:
	case ESI_SI_SA_SPK_SWORDFIRE_GROUND:
	case ESI_SI_SA_SPK_ETERNAL_FIRE:
		if( 0 < m_usBlazeAbsoluLevel )
			ResetAnimSpeed( m_fAnimSpeed, true, true );
		break;
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CX2GUUser::CommonStateStartFuture();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
/*virtual*/ void CX2GUEL::CommonStateStart()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1j: June 32b70220 ( the base runs first; the reset passes bFuture = true as in the future state )
	CX2GUUser::CommonStateStart();
	switch( GetNowStateID() )
	{
	case ESI_SI_SA_SPK_SWORDFIRE_FLY:
	case ESI_SI_SA_SPK_SWORDFIRE_GROUND:
	case ESI_SI_SA_SPK_ETERNAL_FIRE:
		if( 0 < m_usBlazeAbsoluLevel )
			ResetAnimSpeed( m_fAnimSpeed, true, true );
		break;
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CX2GUUser::CommonStateStart();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
			 
/*virtual*/ void CX2GUEL::CommonFrameMoveFuture()
{
	KTDXPROFILE();
	CX2GUUser::CommonFrameMoveFuture();
#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
	m_FrameDataFuture.syncData.m_CannonBallCount = GetWayOfSwordState();
#endif // ELSWORD_WAY_OF_SWORD
}

/*virtual*/ void CX2GUEL::CommonFrameMove()
{
	KTDXPROFILE();
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1j: June 32b88070 ( base, Way of the Sword, the per-frame passives, then the dash timers and the passive / blood ticks )
	CX2GUUser::CommonFrameMove();

#ifdef ELSWORD_WAY_OF_SWORD
	WayOfSwordStateChange();	// June 32b7f370: body in CH1q
	GaleBuffFrameMove();		// June 32b6e050 = March
#endif // ELSWORD_WAY_OF_SWORD

	ChivalryProc_FrameMove();	// June 32b7ac20

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE
	DrasticChargeProc_FrameMove();	// June 32b7b5f0
	XX_DOWN_XProc_FrameMove();		// June 32b7b6c0 = March
	SnapCrasherProc_FrameMove();	// June 32b7b760 = March
	StigmaOfFireProc_FrameMove();	// June inline @32b880ce = March
	FireBlossomsProc_FrameMove();	// June 32b87520

	if( 0.0f <= m_fDashComboStartDelay )
		m_fDashComboStartDelay -= m_fElapsedTime;

	if( 0.0f < m_fAncientFireRenderTime )
	{
		m_fAncientFireRenderTime -= m_fElapsedTime;

		if( m_fAncientFireRenderTime <= 0.0f )
		{
			SetLayer(XL_UNIT_0);
			m_bSetCustomRenderParam = false;
			SetAlphaObject(false);
			m_fAncientFireRenderTime = -1.f;
		}
	}
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE
	BloodRunawayProc_FrameMove();	// June 32b734d0: body in CH4a
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_BLOOD
	ShadowEdgeRecordFrameMove();	// June 32b813d0: body in CH4c
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_BLOOD

	// June @32b88185: the dash time runs while a dash state lasts and stays through Leap Crash
	if( true == IsDashState() )
		m_fDashElapsedTime += m_fElapsedTime;
	else if( CX2SkillTree::RNSI_A_SSK_LEAP_CRASH != GetNowStateSkillID() && CX2SkillTree::RNSI_A_SSK_LEAP_CRASH_UPGRADE != GetNowStateSkillID() )
		m_fDashElapsedTime = 0.f;

	// June @32b881bf: Advanced Knight Mastery's value over the current MP ( at least 1 ) is the passive MP change rate
	{
		const float fNowMp = GetNowMp();
		const float fMpDivisor = ( 1.0f <= fNowMp ? fNowMp : 1.0f );
		SetAdditionalMPChangeRateByPassive( m_fAdvancedKnightMasteryMPValue / fMpDivisor );
	}

	// June @32b8828d: the charge time grows while dashing and runs down by the frame time afterwards
	if( true == IsDashState() )
		m_fDashChargeTime += m_fElapsedTime;
	else if( 0.0f != m_fDashChargeTime )
	{
		if( 0.0f <= m_fDashChargeTime )
			m_fDashChargeTime -= m_fElapsedTime;
		else
			m_fDashChargeTime = 0.0f;
	}

#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE
	GrandMasterTranscendenceProc_FrameMove();	// June 32b7e580: body in CH4a
	ApplyGrandMasterAura();						// June 32b7e740: body in CH4a
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_BLOOD
	TranscendenceBerserkerFrameMove();			// June 32b7e9a0: body in CH4c
	MadnessFrameMove();							// June 32b6ff20: body in CH4c
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_BLOOD
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CX2GUUser::CommonFrameMove();

#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
	WayOfSwordStateChange();
	GaleBuffFrameMove();
#endif // ELSWORD_WAY_OF_SWORD

	// 기사의 숙명 패시브 처리
	ChivalryProc_FrameMove();

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	DrasticChargeProc_FrameMove ();			// 드래스틱 차지 이펙트
	XX_DOWN_XProc_FrameMove ();				// XX 밑 X 이펙트
	SnapCrasherProc_FrameMove ();			// 드래스틱 차지 이펙트
	
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	// 불꽃의 낙인 패시브 처리
	StigmaOfFireProc_FrameMove();
	// 불꽃 개화 패시브 처리
	FireBlossomsProc_FrameMove();

	// 대시 콤보의 마지막( XXX나 XXXZ ) 후에 다시 대시 콤보를 시작하기 까지의 시간
	if( 0.0f <= m_fDashComboStartDelay )
		m_fDashComboStartDelay -= m_fElapsedTime;

	if( 0.0f < m_fAncientFireRenderTime )
	{
		m_fAncientFireRenderTime -= m_fElapsedTime;

		if( m_fAncientFireRenderTime <= 0.0f )
		{
			SetLayer(XL_UNIT_0);	
			m_bSetCustomRenderParam = false;
			SetAlphaObject(false);
			m_fAncientFireRenderTime = -1.f;
		}
	}

#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
			 
/*virtual*/ void CX2GUEL::CommonStateEndFuture()
{
	KTDXPROFILE();
	CX2GUUser::CommonStateEndFuture();
}

/*virtual*/ void CX2GUEL::CommonStateEnd()
{
	KTDXPROFILE();
	CX2GUUser::CommonStateEnd();
}

/*virtual*/ void CX2GUEL::DetonationStateChange( CX2DamageManager::DamageData* pDamageData )
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1h: June 32b73e10 ( the Destruction detonation enters its own Endurance Power state, not DoStateEndurancePowerRage )
	if( WSS_DESTRUCTION == GetWayOfSwordState() )
	{
		StateChange( ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_DETONATION, true );
		SetNowChargeMpForDetonation( 0.f );
	}
	else
	{
		CX2GUUser::DetonationStateChange( pDamageData );
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
#ifdef ELSWORD_WAY_OF_SWORD // 검의 길 섬멸 기폭
	if( WSS_DESTRUCTION == GetWayOfSwordState() )
	{ // 섬멸 게이지로 기폭 시 슈아팅
		DoStateEndurancePowerRage( true );
		SetNowChargeMpForDetonation( 0.f ) ;
	}
	else
#endif // ELSWORD_WAY_OF_SWORD
	{
		CX2GUUser::DetonationStateChange( pDamageData );
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
			 
/*virtual*/ RENDER_HINT	CX2GUEL::CommonRender_Prepare()
{
	KTDXPROFILE();
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1j: June 32b4edc0 ( only the cartoon texture is set: no render type, no outline colour )
	if( WSS_VIGOR == GetWayOfSwordState() )
		m_RenderParam.cartoonTexType = CKTDGXRenderer::CTT_SKYBLUE;
	else if( WSS_DESTRUCTION == GetWayOfSwordState() )
		m_RenderParam.cartoonTexType = CKTDGXRenderer::CTT_SCARLET;
	return CX2GUUser::CommonRender_Prepare();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE

#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
	if( GetWayOfSwordState() == WSS_VIGOR )
	{
		m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
		m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_SKYBLUE;
		m_RenderParam.outLineColor		= 0xffffffff;
	}
	else if( GetWayOfSwordState() == WSS_DESTRUCTION )
	{
		m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
		m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_SCARLET;
		m_RenderParam.outLineColor		= 0xffffffff;
	}
#endif // ELSWORD_WAY_OF_SWORD

	return CX2GUUser::CommonRender_Prepare();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
			 
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
// char-elesis CH1k: helpers and constants of the skill-use functions ( June 32b84a50 / 32b73e60 / 32b80c60 )
namespace _CONST_EL_CH1K_
{
	// June's pop-up line: font 3, white text with a black outline, centred
	void PushInfoText( const int iStringID_ )
	{
		g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( iStringID_ ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
	}

	// June 31fd0570: training game ( 0x12 ) or the photo zone ( 0x1a, June-only ); the photo zone has no March state
	bool IsTrainingOrPhotozone()
	{
		return CX2Main::XS_TRAINING_GAME == g_pMain->GetNowStateID();
	}

	const float INJECTION_STIGMA_HP_COST_RATE_BLOOD_RUNAWAY	= 0.2f;	// June multiplies the HP cost by the double 0.2f
}
using namespace _CONST_EL_CH1K_;

// char-elesis CH1k: June 32b73e60 ( user vslot 0x800, step 512 ): the MP, the finality stone and the cool time of a skill use
void CX2GUEL::SpecialAttackCooldown( const int iSkillID_, const float fMpConsume_ )
{
	if ( false == IsMyUnit() )
		return;

	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( static_cast<CX2SkillTree::SKILL_ID>( iSkillID_ ), true );
	if ( NULL == GetUnit() || NULL == pSkillTemplet )
		return;

	CX2UserSkillTree& accessUserSkillTree = GetUnit()->AccessUnitData().m_UserSkillTree;

	MarkSkillSlotUsed( m_iChosenSkillSlotIndex, false );
	UpNowMp( -fMpConsume_ );

	if ( CX2SkillTree::ST_HYPER_ACTIVE_SKILL == pSkillTemplet->m_eType && false == IsTrainingOrPhotozone() )
	{
		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );
		if ( NULL != pItem )
			g_pX2Game->Handler_EGS_USE_FINALITY_SKILL_REQ( pItem->GetItemData().m_ItemUID );
	}

	int iSkillLevel = accessUserSkillTree.GetSkillLevel( pSkillTemplet->m_eID, false, 0, true );
	if ( iSkillLevel < 1 )
		iSkillLevel = 1;

	accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, GetActualCoolTime( pSkillTemplet, iSkillLevel ) );

	// 608025 has no Enum entry ( a memo past March's sixteen ); June's memo test reads false
	if ( CX2SkillTree::SI_SA_EL_UNLIMITED_BLADE == pSkillTemplet->m_eID && true == GetEqippedSkillMemo( static_cast<CX2SkillTree::SKILL_MEMO_ID>( 608025 ) )
		&& false == HaveThisBuff( BTI_DEBUFF_UNLIMITED_BLADE_MEMO ) )
	{
		SetBuffFactorToGameUnitByBuffFactorID( BFI_DEBUFF_UNLIMITED_BLADE_MEMO, 1, NULL, 0, false );
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, 0.f );
	}

	GetUnit()->ApplySkillCoolTimeRateByBuff( pSkillTemplet->m_eID );
	CX2RandomMissionManager::GetInstance()->OnSkillUse( pSkillTemplet->m_eID );
}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
// char-elesis CH1k: June 337ad560 / 337ad590: the cut-ins of code event 0x8b3 ( GFx names without extension; the cut-in sets are CH5 )
static const CX2GUUser::SkillCutIn s_SkillCutInSetEvent8b3[] =
{
	CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_CASH_21"),
	CX2GUUser::SkillCutIn( L"HQ_CutIn_Elesis_CASH_22"),
};

// char-elesis CH1k: June 32b70290 ( user vslot 0x828 ): the base test less the hyper wait ( future ) and the dash-jump power landing ( now )
bool CX2GUEL::IsNotCommonMoveState()
{
	if ( false == CX2GUUser::IsNotCommonMoveState() )	// June 31fd7640
		return false;

	if ( ESI_SWORD_WAIT == m_FrameDataFuture.syncData.nowState )
		return false;

	return ESI_DASH_JUMP_POWER_LANDING != GetNowStateID();
}

// char-elesis CH1k: June 32057b80 @32057c4c-cb2: the reserved skill's state starts, so step 512 spends its MP and sets the cool time, and 0x764 refunds the memo share
/*virtual*/ void CX2GUEL::ReservedSkillStart( const CX2SkillTree::SkillTemplet* pSkillTemplet_ )
{
	const int iNowStateID = GetNowStateID();
	bool bSkillSlotState = false;	// June's state-data byte +0x11
	for ( int i = 0; i < ARRAY_SIZE( _CONST_GUUSER_::aNormalSkillStateID ); ++i )
	{
		if ( _CONST_GUUSER_::aNormalSkillStateID[i] == iNowStateID || _CONST_GUUSER_::aHyperSkillStateID[i] == iNowStateID ||
			_CONST_GUUSER_::aNormalSkillStateIDSlotB[i] == iNowStateID || _CONST_GUUSER_::aHyperSkillStateIDSlotB[i] == iNowStateID )
			bSkillSlotState = true;
	}

	const int iSkillID = static_cast<int>( pSkillTemplet_->m_eID );
	SpecialAttackCooldown( iSkillID, m_fReservedSkillMp );
	SkillMemoMpRefund( iSkillID, bSkillSlotState, m_fReservedSkillMp );
}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
/*virtual*/ bool CX2GUEL::SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition_ /*= CX2SkillTree::ASUT_GROUND */)
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1k: June 32b4ee20 ( user vslot 0x7f4 ): the lookup, then the use; a skill of use condition 3 / 5 gives the chosen values back
	const float fSavedReservedMp = m_fReservedSkillMp;
	const int iSavedSlotIndex = m_iChosenSkillSlotIndex;
	const int iSavedSkillID = m_iChosenSkillID;

	bool bUsed = false;
	if ( true == CanUseSpecialAttack( eActiveSkillUseCondition_ ) && 0 != m_iChosenSkillID )
	{
		bUsed = true;
		StateChangeSpecialAttack( static_cast<CX2SkillTree::SKILL_ID>( m_iChosenSkillID ) );
		Send_SKILL_USE_REQ( static_cast<CX2SkillTree::SKILL_ID>( m_iChosenSkillID ) );
	}

	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( static_cast<CX2SkillTree::SKILL_ID>( m_iChosenSkillID ), true );
	if ( NULL != pSkillTemplet && ( CX2SkillTree::ASUT_ANYTIME == pSkillTemplet->m_eActiveSkillUseCondtion || CX2SkillTree::ASUT_ANYTIME_EXCEPT_AIR == pSkillTemplet->m_eActiveSkillUseCondtion ) )
	{
		m_fReservedSkillMp = fSavedReservedMp;
		m_iChosenSkillID = iSavedSkillID;
		m_iChosenSkillSlotIndex = iSavedSlotIndex;
	}

	if ( true == bUsed )
	{
		m_iNowSpecialAttack = m_iChosenSkillSlotIndex + 1;
		return true;
	}

	m_iChosenSkillID = iSavedSkillID;
	return false;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
#ifdef SERV_IRUHADEV_JUNE_BUFF_QUICKSLOT_ITEM
	/// June buff port Phase 7d: June EL 32b84a50 (the skill lookup its SpecialAttackEventProcess calls first) starts with
	/// "if +0x8d78 > 0 return false": no skill while a quick-slot special item waits
	if ( true == IsQuickSlotSpecialItemWaiting() )
		return false;
#endif SERV_IRUHADEV_JUNE_BUFF_QUICKSLOT_ITEM
	KTDXPROFILE();

	if ( false == CanUseSpecialAttack( eActiveSkillUseCondition_ ) )
		return false;

	if( NULL == m_pSkillTemplet )
	{
		return false;
	}
	else
	{
		StateChangeSpecialAttack( m_pSkillTemplet );
	}

#ifdef SERV_SKILL_USE_SUBQUEST
	Send_SKILL_USE_REQ( m_pSkillTemplet->m_eID );
#endif SERV_SKILL_USE_SUBQUEST

	return true;

#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
/** @function	: NoStateChangeActionFrameMove
	@brief		: 스테이트의 변화가 없는 스킬 사용 시 수행되는 FrameMove
*/
/*virtual*/ void CX2GUEL::NoStateChangeActionFrameMove()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1k: June 32b818b0 ( user vslot 0x80c ): the base action first; Endurance Power also shows the skill names
	UserNoStateChangeActionFrameMove();

	if ( UAI_ENDURANCE_POWER == m_FrameDataNow.syncData.nowAction )
	{
		ProcessEndurancePowerSkill();
		ShowSkillNameStateString( CX2SkillTree::RNSI_A_EL_ENDURANCE_POWER );
		ShowSkillNameStateString( CX2SkillTree::RNSI_A_EL_ENDURANCE_POWER_UPGRADE );
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	switch( m_FrameDataNow.syncData.nowAction )
	{
	case UAI_ENDURANCE_POWER:
		{
			ProcessEndurancePowerSkill();
		} break;
	default:
		break;
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

/*virtual*/ float CX2GUEL::GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const
{
	float fMpConsumption = CX2GUUser::GetActualMPConsume( eSkillID_, iSkillLevel_ );
	
	switch ( eSkillID_ )
	{
		// 닷지&슬래시
	case CX2SkillTree::SI_A_EL_DODGE_AND_SLASH:
		{
			// 승리의 검 버프가 적용 된 상태라면
			if( true == m_bIsApplyVictoriousSword )
			{
				fMpConsumption = 0.f;
			}

		} break;



#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	case CX2SkillTree::SI_A_EL_RUSHING_SWORD:
		{
			// 검의 왈츠로 인해 m_iSGMDecreaseMPRushingSword 값이 있을 경우, 해당 값 만큼의 MP 소모량을 줄인다.
			fMpConsumption = fMpConsumption - m_iDecreaseMPRushingSword;
		} break;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef ADD_MEMO_1ST_CLASS //김창한
	case CX2SkillTree::SI_A_SSK_POWER_BREAK:
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELESIS_MEMO5 ) == true )
				fMpConsumption *= 0.9f;
		} break;

	case CX2SkillTree::SI_SA_SPK_ETERNAL_FIRE:
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELESIS_MEMO16 ) == true )
				fMpConsumption -= 30.f;
		} break;

	case CX2SkillTree::SI_SA_SSK_VICTORIOUS_SWORD:
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELESIS_MEMO8 ) == true )
				fMpConsumption *= 0.9f;
		}	
#endif //ADD_MEMO_1ST_CLASS

	default:
		break;
	}

#ifdef ELSWORD_WAY_OF_SWORD
//////////////////////////////////////////////////////////////////////////
// 질풍 스킬 MP 예외처리.	
	// 오현빈
	// GetEquippedActiveSkillTemplet()을 두번 호출 하는 단점이 있지만
	// UI에서도 공용으로 사용 할 수 있도록 현재 위치에서 처리
	if( CX2GUUser::WSS_VIGOR == GetWayOfSwordState() )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet 
			= GetEquippedActiveSkillTemplet( eSkillID_ );

		if( NULL != pSkillTemplet && 
			WSS_VIGOR == pSkillTemplet->m_iWOSType )
		{
			// 대전에서는 감소량 1/2
			if( NULL != g_pX2Game && CX2Game::GT_PVP == g_pX2Game->GetGameType())	
				fMpConsumption *= _CONST_EL_::GALE_MP_CONSUME_RATE_PVP;
			else
				fMpConsumption *= _CONST_EL_::GALE_MP_CONSUME_RATE;
		}
	}
#endif // ELSWORD_WAY_OF_SWORD

#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( eSkillID_ );
	float fMpDecreaseRate = 1.0f;
	if( NULL != pSkillTemplet )
		fMpDecreaseRate  =  g_pData->GetMyUser()->GetSelectUnit()->GetSkillMpDecreaseRate(eSkillID_, pSkillTemplet->m_eType);

	return fMpConsumption * fMpDecreaseRate;
#else SERV_BALANCE_FINALITY_SKILL_EVENT
	return fMpConsumption;
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
}
/*virtual*/ float CX2GUEL::GetActualCoolTime( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_, IN int iSkillLevel ) const
{
	if( NULL == pSkillTemplet_ )
		return 0.f;

	float fSkillCoolTime = CX2GUUser::GetActualCoolTime( pSkillTemplet_, iSkillLevel );
#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
	if( WSS_VIGOR == GetWayOfSwordState() )
	{ // 질풍 스킬 쿨타임 감소
		if( WSS_VIGOR == pSkillTemplet_->m_iWOSType )
		{
			float fGaleCoolTimeDescraseRate = _CONST_EL_::GALE_COOL_TIME_DECREASE_RATE;

			// 대전에서는 감소량 1/2
			if( NULL != g_pX2Game && CX2Game::GT_PVP == g_pX2Game->GetGameType())	
				fGaleCoolTimeDescraseRate = _CONST_EL_::GALE_COOL_TIME_DECREASE_RATE_PVP;

			fSkillCoolTime *= fGaleCoolTimeDescraseRate;
		}
	}
#endif // ELSWORD_WAY_OF_SWORD

	switch ( pSkillTemplet_->m_eID )
	{
	// 닷지&슬래시
	case CX2SkillTree::SI_A_EL_DODGE_AND_SLASH:
		{
			// 승리의 검 버프가 적용 된 상태라면
			if( true == m_bIsApplyVictoriousSword )
			{
				ASSERT( m_fDodgeAndSlashCoolTimeDecreaseRate < 1.f );
				fSkillCoolTime *= m_fDodgeAndSlashCoolTimeDecreaseRate;
			}

		} break;
	default:
		break;
	}

	return max( 0.f, fSkillCoolTime );
}
/*virtual*/ CX2GageData* CX2GUEL::CreateGageData()
{
	if ( true == IsMyUnit() )
	{
		const CX2ElesisGageData* pElswordGageData = 
			static_cast<const CX2ElesisGageData*>( CX2GageManager::GetInstance()->GetMyGageData() );

		if ( NULL != pElswordGageData )
			return new CX2ElesisGageData( *pElswordGageData );
		else
			return new CX2ElesisGageData();
	}
	else
		return new CX2ElesisGageData();
}

/*virtual*/	bool CX2GUEL::ShouldResetNotBusyTimer()
{	
	if ( false == CX2GUUser::ShouldResetNotBusyTimer() )
		return false;
	else if ( ESI_SWORD_WAIT == m_FrameDataFuture.syncData.nowState )
		return false;

	return true; // 바쁜 상태 라면 리셋 해야 함
}

void CX2GUEL::InitStateID()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1m: June inlines it in 32b87a60 @32b87af5, a sword wait set before InitState survives
	if( ESI_SWORD_WAIT != m_CommonState.m_Wait )
		m_CommonState.m_Wait = USI_WAIT;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_CommonState.m_Wait	= USI_WAIT;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

/** @function	: InitStateCommon
	@brief		: 모든 캐릭터가 공통적으로 쓰는 상태 및 전직에 상관없이 공통으로 쓰는 상태 초기화
*/
void CX2GUEL::InitStateCommon()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1m: June 32b81be0 ( InitState part 1, no class test )
	UserUnitStateData stateData;
	const std::string strHyperModeTable = GetHyperModeTableName( false );	// June 31fdd490( ..., 0 )

	stateData.Init();
	stateData.stateID			= USI_DIE_FRONT;
	m_LuaManager.MakeTableReference( "ESI_DIE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DieFrontEventProcess );
	// June 3202a7d0 ( StateEnd ): runs in CX2GUUser::StateChange for the die states
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DIE_BACK;
	m_LuaManager.MakeTableReference( "ESI_DIE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, DieBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, DieBackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, DieBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DieBackEventProcess );
	// June 3202a7d0 ( StateEnd ): runs in CX2GUUser::StateChange for the die states
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_START_INTRUDE;
	m_LuaManager.MakeTableReference( "ESI_START_INTRUDE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, StartIntrudeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, StartIntrudeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartIntrudeEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_WAIT;
	m_LuaManager.MakeTableReference( "ESI_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, WaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, WaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, WaitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_WALK;
	m_LuaManager.MakeTableReference( "ESI_WALK", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, WalkEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_READY;
	m_LuaManager.MakeTableReference( "ESI_JUMP_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, JumpReadyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_UP;
	m_LuaManager.MakeTableReference( "ESI_JUMP_UP", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, JumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ESI_JUMP_DOWN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, JumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ESI_JUMP_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, JumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH;
	m_LuaManager.MakeTableReference( "ESI_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_END;
	m_LuaManager.MakeTableReference( "ESI_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, DashEndStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, DashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, DashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_HYPER_MODE;
	m_LuaManager.MakeTableReference( strHyperModeTable.c_str(), stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, HyperModeStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, HyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, HyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, HyperModeEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, HyperModeEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_GROGGY;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_GROGGY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DAMAGE_GROGGY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_FRONT;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_SMALL_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_BACK;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_SMALL_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_FRONT;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_BIG_FRONT", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageBigStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageBigEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_BACK;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_BIG_BACK", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageBigStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageBigEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_FRONT;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_DOWN_FRONT", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, DamageDownFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DamageDownFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_BACK;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_DOWN_BACK", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, DamageDownBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DamageDownBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_FRONT;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_STANDUP_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_BACK;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_STANDUP_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_SMALL;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_SMALL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DamageAirSmallStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_INVINCIBLE;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_DOWN_INVINCIBLE", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownInvincibleEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_LANDING;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_DOWN_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DamageAirDownLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FALL;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_FALL", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_UP;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_UP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_FRONT;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_FLY_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_BACK;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_FLY_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_REVENGE;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_REVENGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageRevengeStart );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, DamageRevengeCameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageRevengeEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_THROW_ITEM;
	m_LuaManager.MakeTableReference( "ESI_THROW_ITEM", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMove );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_CameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_READY;
	m_LuaManager.MakeTableReference( "ESI_PEPPER_RUN_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN;
	m_LuaManager.MakeTableReference( "ESI_PEPPER_RUN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_END;
	m_LuaManager.MakeTableReference( "ESI_PEPPER_RUN_END", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_UP;
	m_LuaManager.MakeTableReference( "ESI_PEPPER_RUN_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ESI_PEPPER_RUN_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	// June 32b81be0 @1089 ESI_AMUSEMENT_ITEM_PHOENIX_RUN ( id 123 ) left out: PvP amusement mode, no PvP offline
	// June 32b81be0 @1093 ESI_AMUSEMENT_ITEM_PHOENIX_RUN_END ( id 126 ) left out: PvP amusement mode, no PvP offline
	// June 32b81be0 @1098 ESI_AMUSEMENT_ITEM_PHOENIX_RUN ( id 124 ) left out: PvP amusement mode, no PvP offline
	// June 32b81be0 @1103 ESI_AMUSEMENT_ITEM_PHOENIX_RUN ( id 125 ) left out: PvP amusement mode, no PvP offline
	// June 32b81be0 @1106 ESI_DANCE_Z ( id 127 ) left out: dance state, Stage 10
	// June 32b81be0 @1109 ESI_DANCE_X ( id 128 ) left out: dance state, Stage 10
	// June 32b81be0 @1112 ESI_DANCE_WAIT ( id 129 ) left out: dance state, Stage 10
	// June 32b81be0 @1115, the three DAMAGE_BOUNDING states ( checklist 7l )
	InitBoundingStates( "ESI_" );

	stateData.Init();
	stateData.stateID			= GetRidingStartStateID();
	m_LuaManager.MakeTableReference( "ESI_RIDING_START", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingStartStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingStartStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingStartEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_ON;
	m_LuaManager.MakeTableReference( "ESI_RIDING_ON", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingOnStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingOnStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOnEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOnEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_OFF;
	m_LuaManager.MakeTableReference( "ESI_RIDING_OFF", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOffEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOffEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT_HABIT;
	m_LuaManager.MakeTableReference( "ESI_RIDING_WAIT_HABIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT;
	m_LuaManager.MakeTableReference( "ESI_RIDING_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_WALK;
	m_LuaManager.MakeTableReference( "ESI_RIDING_WALK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWalkStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWalkStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWalkEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_UP;
	m_LuaManager.MakeTableReference( "ESI_RIDING_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpUpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpUpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ESI_RIDING_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpDownStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpDownStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ESI_RIDING_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH;
	m_LuaManager.MakeTableReference( "ESI_RIDING_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_END;
	m_LuaManager.MakeTableReference( "ESI_RIDING_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashEndStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashEndStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingDashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP;
	m_LuaManager.MakeTableReference( "ESI_RIDING_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ESI_RIDING_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_FRONT;
	m_LuaManager.MakeTableReference( "ESI_RIDING_DAMAGE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_BACK;
	m_LuaManager.MakeTableReference( "ESI_RIDING_DAMAGE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageBackStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DIE;
	m_LuaManager.MakeTableReference( "ESI_RIDING_DIE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DieFrontEventProcess );
	// June 3202a7d0 ( StateEnd ): runs in CX2GUUser::StateChange for the die states
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ESI_RIDING_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackZStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ESI_RIDING_JUMP_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_X;
	m_LuaManager.MakeTableReference( "ESI_RIDING_ATTACK_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackXStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackXStart );
	// June 32002f70 ( OnFrameMove ): none in March, riding Stage 17
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_ATTACK;
	m_LuaManager.MakeTableReference( "ESI_RIDING_ATTACK_SPECIAL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_MOVE;
	m_LuaManager.MakeTableReference( "ESI_RIDING_SPECIAL_MOVE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEndFutrue );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEnd );
	m_StateList[stateData.stateID] = stateData;

	// June 32b81be0 @1250 ESI_RIDING_SPECIFIC ( id 93 ) left out: riding, Stage 17
	stateData.Init();
	stateData.stateID			= USI_RIDING_HYPER_MODE;
	m_LuaManager.MakeTableReference( "ESI_RIDING_HYPER_MODE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingHyperModeStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingHyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, RidingHyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingHyperModeEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingHyperModeEnd );
	m_StateList[stateData.stateID] = stateData;

	// June 32b81be0 @1262 ESI_RIDING_ATTACK_X_READY ( id 94 ) left out: riding, Stage 17
	// June 32b81be0 @1267 ESI_RIDING_ATTACK_X_READY_2 ( id 95 ) left out: riding, Stage 17
	// June 32b81be0 @1272 ESI_RIDING_ATTACK_X_END ( id 96 ) left out: riding, Stage 17
	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ROLLING_FRONT_FRONT;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ROLLING_FRONT_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpRollingFrontFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpRollingFrontFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ROLLING_FRONT_BACK;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ROLLING_FRONT_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpRollingFrontBackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpRollingFrontBackEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ROLLING_BACK_FRONT;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ROLLING_BACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpRollingBackFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpRollingBackFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ROLLING_BACK_BACK;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ROLLING_BACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpRollingBackBackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpRollingBackBackEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ATTACK_FRONT;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ATTACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpAttackFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpAttackFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ATTACK_BACK;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ATTACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpAttackBackEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUEL, StandUpAttackBackStateEndFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpAttackBackEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ATTACK_FRONT_NO_DOWN;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ATTACK_FRONT_NO_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpAttackFrontNoDownEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpAttackFrontNoDownEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ATTACK_BACK_NO_DOWN;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ATTACK_BACK_NO_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpAttackBackNoDownEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpAttackBackNoDownEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_JUMP_POWER_LANDING;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_POWER_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, DashJumpPowerLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, DashJumpPowerLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpPowerLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_WALL_LANDING;
	m_LuaManager.MakeTableReference( "ESI_WALL_LANDING", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, WallLandingEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, WallLandingEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_Z;
	m_LuaManager.MakeTableReference( "ESI_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "ESI_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_ZZZ;
	m_LuaManager.MakeTableReference( "ESI_COMBO_ZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboZZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_ZZZZ;
	m_LuaManager.MakeTableReference( "ESI_COMBO_ZZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboZZZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_ZZZX;
	m_LuaManager.MakeTableReference( "ESI_COMBO_ZZZX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboZZZXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_X;
	m_LuaManager.MakeTableReference( "ESI_COMBO_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_XX;
	m_LuaManager.MakeTableReference( "ESI_COMBO_XX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboXXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_XXX;
	m_LuaManager.MakeTableReference( "ESI_COMBO_XXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboXXXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_XXZ;
	m_LuaManager.MakeTableReference( "ESI_COMBO_XXZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboXXZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ESI_JUMP_ATTACK_Z", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUEL, JumpAttackZFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, JumpAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_JUMP_ATTACK_X;
	m_LuaManager.MakeTableReference( "ESI_JUMP_ATTACK_X", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUEL, JumpAttackXFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, JumpAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_COMBO_Z;
	m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashComboZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashComboZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_COMBO_X;
	m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashComboXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_JUMP_COMBO_Z;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpComboZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_JUMP_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpComboZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_JUMP_COMBO_ZZZ;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_ZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpComboZZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_JUMP_COMBO_X;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpComboXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_JUMP_COMBO_XX;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_XX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpComboXXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_JUMP_COMBO_XX_LANDING;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_XX_LANDING", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpComboXXLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_QUICK_STANDUP_ATTACK_FRONT;
	m_LuaManager.MakeTableReference( "ESI_QUICK_STANDUP_ATTACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpAttackFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpAttackFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_QUICK_STANDUP_ATTACK_BACK;
	m_LuaManager.MakeTableReference( "ESI_QUICK_STANDUP_ATTACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpAttackBackEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUEL, StandUpAttackBackStateEndFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpAttackBackEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_SWORD_WAIT;
	m_LuaManager.MakeTableReference( "ESI_SWORD_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, SwordWaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, SwordWaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, SwordWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE;
	m_LuaManager.MakeTableReference( "ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE", stateData.stateID );
	stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_Init );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_StateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_MEMO;
	m_LuaManager.MakeTableReference( "ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_MEMO", stateData.stateID );
	stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_Init );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_StateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_DETONATION;
	m_LuaManager.MakeTableReference( "ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE", stateData.stateID );
	stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_Init );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_DETONATION_StateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_CAN_NOT_ATTACK_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ESI_CAN_NOT_ATTACK_JUMP_DOWN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_CAN_NOT_ATTACK_JUMP_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	UserUnitStateData stateData;

	stateData.Init();
	stateData.stateID			= USI_DIE_FRONT;
	m_LuaManager.MakeTableReference( "ESI_DIE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DieFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DIE_BACK;
	m_LuaManager.MakeTableReference( "ESI_DIE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, DieBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, DieBackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, DieBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DieBackEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_START_INTRUDE;
	m_LuaManager.MakeTableReference( "ESI_START_INTRUDE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, StartIntrudeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, StartIntrudeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartIntrudeEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_WAIT;
	m_LuaManager.MakeTableReference( "ESI_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, WaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, WaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, WaitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_WALK;
	m_LuaManager.MakeTableReference( "ESI_WALK", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, WalkEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_READY;
	m_LuaManager.MakeTableReference( "ESI_JUMP_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, JumpReadyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_UP;
	m_LuaManager.MakeTableReference( "ESI_JUMP_UP", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, JumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ESI_JUMP_DOWN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, JumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ESI_JUMP_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, JumpLandingStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, JumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH;
	m_LuaManager.MakeTableReference( "ESI_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_END;
	m_LuaManager.MakeTableReference( "ESI_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, DashEndStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, DashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, DashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, DashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUEL, DashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUEL, DashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_HYPER_MODE;
	m_LuaManager.MakeTableReference( "ESI_HYPER_MODE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, HyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, HyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, HyperModeEventProcess );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, HyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, HyperModeEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_GROGGY;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_GROGGY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DAMAGE_GROGGY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_FRONT;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_SMALL_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DamageSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_BACK;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_SMALL_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DamageSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_FRONT;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_BIG_FRONT", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, DamageBigStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DamageBigEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_BACK;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_BIG_BACK", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, DamageBigStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DamageBigEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_FRONT;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_DOWN_FRONT", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, DamageDownFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DamageDownFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_BACK;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_DOWN_BACK", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, DamageDownBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DamageDownBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_FRONT;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_STANDUP_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_BACK;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_STANDUP_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_SMALL;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_SMALL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DamageAirSmallStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_INVINCIBLE;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_DOWN_INVINCIBLE", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownInvincibleEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_LANDING;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_DOWN_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DamageAirDownLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FALL;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_FALL", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_UP;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_UP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_FRONT;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_FLY_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_BACK;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_AIR_FLY_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_REVENGE;
	m_LuaManager.MakeTableReference( "ESI_DAMAGE_REVENGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageRevengeStart );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, DamageRevengeCameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageRevengeEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_THROW_ITEM;
	m_LuaManager.MakeTableReference( "ESI_THROW_ITEM", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMove );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_CameraMove );		
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_READY;
	m_LuaManager.MakeTableReference( "ESI_PEPPER_RUN_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN;
	m_LuaManager.MakeTableReference( "ESI_PEPPER_RUN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_END;
	m_LuaManager.MakeTableReference( "ESI_PEPPER_RUN_END", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_UP;
	m_LuaManager.MakeTableReference( "ESI_PEPPER_RUN_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ESI_PEPPER_RUN_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

#ifdef RIDING_SYSTEM
	stateData.Init();
	stateData.stateID			= GetRidingStartStateID();
	m_LuaManager.MakeTableReference( "ESI_RIDING_START", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingStartStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingStartStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingStartEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_ON;
	m_LuaManager.MakeTableReference( "ESI_RIDING_ON", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingOnStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingOnStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOnEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOnEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_OFF;
	m_LuaManager.MakeTableReference( "ESI_RIDING_OFF", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOffEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOffEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT_HABIT;
	m_LuaManager.MakeTableReference( "ESI_RIDING_WAIT_HABIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT;
	m_LuaManager.MakeTableReference( "ESI_RIDING_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_WALK;
	m_LuaManager.MakeTableReference( "ESI_RIDING_WALK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWalkStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWalkStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWalkEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_UP;
	m_LuaManager.MakeTableReference( "ESI_RIDING_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpUpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpUpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ESI_RIDING_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpDownStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpDownStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ESI_RIDING_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH;
	m_LuaManager.MakeTableReference( "ESI_RIDING_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_END;
	m_LuaManager.MakeTableReference( "ESI_RIDING_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashEndStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashEndStart );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashEndStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingDashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP;
	m_LuaManager.MakeTableReference( "ESI_RIDING_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ESI_RIDING_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_FRONT;
	m_LuaManager.MakeTableReference( "ESI_RIDING_DAMAGE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_BACK;
	m_LuaManager.MakeTableReference( "ESI_RIDING_DAMAGE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageBackStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DIE;
	m_LuaManager.MakeTableReference( "ESI_RIDING_DIE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DieFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ESI_RIDING_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackZStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ESI_RIDING_JUMP_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_X;
	m_LuaManager.MakeTableReference( "ESI_RIDING_ATTACK_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackXStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackXStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_ATTACK;
	m_LuaManager.MakeTableReference( "ESI_RIDING_ATTACK_SPECIAL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStart );
#ifdef FIX_SCORPION_SPECIAL_SKILL_BUG
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackFrameMove );
#endif // FIX_SCORPION_SPECIAL_SKILL_BUG
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_MOVE;
	m_LuaManager.MakeTableReference( "ESI_RIDING_SPECIAL_MOVE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEndFutrue );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEnd );
	m_StateList[stateData.stateID] = stateData;
#endif //RIDING_SYSTEM

#ifdef MODIFY_RIDING_PET_AWAKE
	stateData.Init();
	stateData.stateID			= USI_RIDING_HYPER_MODE;
	m_LuaManager.MakeTableReference( "ESI_RIDING_HYPER_MODE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingHyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, RidingHyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingHyperModeEventProcess );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingHyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingHyperModeEnd );
	m_StateList[stateData.stateID] = stateData;
#endif // MODIFY_RIDING_PET_AWAKE

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ROLLING_FRONT_FRONT;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ROLLING_FRONT_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpRollingFrontFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpRollingFrontFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ROLLING_FRONT_BACK;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ROLLING_FRONT_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpRollingFrontBackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpRollingFrontBackEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ROLLING_BACK_FRONT;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ROLLING_BACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpRollingBackFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpRollingBackFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ROLLING_BACK_BACK;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ROLLING_BACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpRollingBackBackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpRollingBackBackEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ATTACK_FRONT;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ATTACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpAttackFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpAttackFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ATTACK_BACK;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ATTACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpAttackBackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpAttackBackEnd );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUEL, StandUpAttackBackStateEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ATTACK_FRONT_NO_DOWN;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ATTACK_FRONT_NO_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpAttackFrontNoDownEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpAttackFrontNoDownEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_STANDUP_ATTACK_BACK_NO_DOWN;
	m_LuaManager.MakeTableReference( "ESI_STANDUP_ATTACK_BACK_NO_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpAttackBackNoDownEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpAttackBackNoDownEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_JUMP_POWER_LANDING;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_POWER_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, DashJumpPowerLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, DashJumpPowerLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpPowerLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_WALL_LANDING;
	m_LuaManager.MakeTableReference( "ESI_WALL_LANDING", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, WallLandingEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, WallLandingEndFuture );
	m_StateList[stateData.stateID] = stateData;

//=====================================================================================
// 공격 스테이트

	stateData.Init();
	stateData.stateID			= ESI_COMBO_Z;
	m_LuaManager.MakeTableReference( "ESI_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "ESI_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_ZZZ;
	m_LuaManager.MakeTableReference( "ESI_COMBO_ZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboZZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_ZZZZ;
	m_LuaManager.MakeTableReference( "ESI_COMBO_ZZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboZZZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_ZZZX;
	m_LuaManager.MakeTableReference( "ESI_COMBO_ZZZX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboZZZXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_X;
	m_LuaManager.MakeTableReference( "ESI_COMBO_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_XX;
	m_LuaManager.MakeTableReference( "ESI_COMBO_XX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboXXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_XXX;
	m_LuaManager.MakeTableReference( "ESI_COMBO_XXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboXXXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_COMBO_XXZ;
	m_LuaManager.MakeTableReference( "ESI_COMBO_XXZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ComboXXZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ESI_JUMP_ATTACK_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, JumpAttackZEventProcess );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUEL, JumpAttackZFrameMoveFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_JUMP_ATTACK_X;
	m_LuaManager.MakeTableReference( "ESI_JUMP_ATTACK_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, JumpAttackXEventProcess );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUEL, JumpAttackXFrameMoveFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_COMBO_Z;
	m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashComboZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashComboZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_COMBO_X;
	m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashComboXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_JUMP_COMBO_Z;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpComboZEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= ESI_DASH_JUMP_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpComboZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_JUMP_COMBO_ZZZ;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_ZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpComboZZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_JUMP_COMBO_X;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpComboXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_JUMP_COMBO_XX;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_XX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpComboXXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_DASH_JUMP_COMBO_XX_LANDING;
	m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_XX_LANDING", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, DashJumpComboXXLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_QUICK_STANDUP_ATTACK_FRONT;
	m_LuaManager.MakeTableReference( "ESI_QUICK_STANDUP_ATTACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpAttackFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpAttackFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_QUICK_STANDUP_ATTACK_BACK;
	m_LuaManager.MakeTableReference( "ESI_QUICK_STANDUP_ATTACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, StandUpAttackBackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, StandUpAttackBackEnd );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUEL, StandUpAttackBackStateEndFuture );
	m_StateList[stateData.stateID] = stateData;


#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
	stateData.Init();
	stateData.stateID			= ESI_SWORD_WAIT;
	m_LuaManager.MakeTableReference( "ESI_SWORD_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, SwordWaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, SwordWaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, SwordWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;
#endif // ELSWORD_WAY_OF_SWORD

	// 극기 - 강(强)
	// 섬멸 기폭에서도 사용하기 위해 스킬 구분 없이 무조건 스테이트 로딩하도록 변경
	stateData.Init();
	stateData.stateID			= ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE;
	m_LuaManager.MakeTableReference( "ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_StateStart );
	stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_Init );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_MEMO;
	m_LuaManager.MakeTableReference( "ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_MEMO", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_StateStart );
	stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_Init );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	
#ifdef ADD_CAN_NOT_ATTACK_JUMP_DOWN
	stateData.Init();
	stateData.stateID			= ESI_CAN_NOT_ATTACK_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ESI_CAN_NOT_ATTACK_JUMP_DOWN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_CAN_NOT_ATTACK_JUMP_DOWN_EventProcess );	
	m_StateList[stateData.stateID] = stateData;
#endif // ADD_CAN_NOT_ATTACK_JUMP_DOWN
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

void CX2GUEL::InitStateByUnitClass()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1m: June 32b864f0 ( InitState part 2, the per-class part )
	UserUnitStateData stateData;
	const CX2Unit::UNIT_CLASS eClass = ( NULL != GetUnit() ) ? GetUnit()->GetUnitData().m_UnitClass : CX2Unit::UC_NONE;

	// June 32b864f0 @44: one switch on the unit class ( 8 and every other class register nothing )
	switch( eClass )
	{
	case CX2Unit::UC_ELESIS_SABER_KNIGHT:
	case CX2Unit::UC_ELESIS_GRAND_MASTER:
		{
			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SGM_XZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SGM_XZ", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SGM_XZ_Init );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SGM_XZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SGM_XZ_LOOP;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SGM_XZ_LOOP", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SGM_XZ_LOOP_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SGM_XZ_DOWN;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SGM_XZ_DOWN", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SGM_XZ_DOWN_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED;
			m_LuaManager.MakeTableReference( "ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED;
			m_LuaManager.MakeTableReference( "ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SGM_ZZZX;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SGM_ZZZX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SGM_ZZZX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SGM_XX_ATTACK;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SGM_XX_ATTACK", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SGM_XX_ATTACK_StateStartFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SGM_XX_ATTACK_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SGM_XX_END;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SGM_XX_END", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SGM_XX_END_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_P_SGM_ADVANCED_KNIGHT_MASTERY;
			m_LuaManager.MakeTableReference( "ESI_P_SGM_ADVANCED_KNIGHT_MASTERY", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_P_SGM_ADVANCED_KNIGHT_MASTERY_StateStart );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_P_SGM_ADVANCED_KNIGHT_MASTERY_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_P_SGM_ADVANCED_KNIGHT_MASTERY_SMALL;
			m_LuaManager.MakeTableReference( "ESI_P_SGM_ADVANCED_KNIGHT_MASTERY_SMALL", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_P_SGM_ADVANCED_KNIGHT_MASTERY_StateStart );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_P_SGM_ADVANCED_KNIGHT_MASTERY_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SSK_ZZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SSK_ZZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SSK_ZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SSK_ZZZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SSK_ZZZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SSK_ZZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SSK_ZZX;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SSK_ZZX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SSK_ZZX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SSK_XX;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SSK_XX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SSK_XX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SSK_XXX;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SSK_XXX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SSK_XXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SGM_XX_DOWN_X_START;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SGM_XX_DOWN_X_START", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SGM_XX_DOWN_X_START_Init );
			stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SGM_XX_DOWN_X_START_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SGM_XX_DOWN_X_START_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SGM_XX_DOWN_X_START_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SGM_XX_DOWN_X_LOOP;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SGM_XX_DOWN_X_LOOP", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SGM_XX_DOWN_X_LOOP_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SGM_XX_DOWN_X_END;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SGM_XX_DOWN_X_END", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SGM_XX_DOWN_X_END_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SSK_ZZX;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SSK_ZZX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SSK_XX_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;

	case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
	case CX2Unit::UC_ELESIS_BLAZING_HEART:
		{
			stateData.Init();
			stateData.stateID			= ESI_COMBO_SBH_XZ;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SBH_XZ", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZ_Init );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SBH_XZZ;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SBH_XZZ", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZZ_Init );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SBH_XZZZ;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SBH_XZZZ", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZZZ_Init );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SBH_X;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SBH_X", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_X_Init );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_X_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_X_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_X_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SBH_XX;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SBH_XX", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XX_Init );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XX_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SBH_XXX;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SBH_XXX", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXX_Init );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXX_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SBH_XXXZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SBH_XXXZ", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXXZ_Init );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXXZ_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXXZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXXZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SBH_ZZZX;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SBH_ZZZX", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SPK_ZX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SPK_ZX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SPK_ZX;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SPK_ZX", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SPK_ZX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SPK_ZX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SPK_ZZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SPK_ZZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SPK_ZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SPK_ZZZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SPK_ZZZ", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SPK_ZZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SPK_ZZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SPK_XZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SPK_XZ", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_XZ_StartFurture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_XZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_XZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SPK_XZ_REATTACK;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SPK_XZ", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_XZ_REATTACK_StartFurture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_XZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_XZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SPK_ZX_READY;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SPK_ZX_READY", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_ZX_READY_StateStartFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_ZX_READY_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SPK_ZX_LOOP;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SPK_ZX_LOOP", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_ZX_LOOP_StateStartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_ZX_LOOP_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_ZX_LOOP_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;

	case CX2Unit::UC_ELESIS_DARK_KNIGHT:
	case CX2Unit::UC_ELESIS_CRIMSON_AVENGER:
		{
			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SCA_ZZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SCA_ZZ", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZZ_StateStart );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SCA_ZZZ_START;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SCA_ZZZ_START", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZZZ_START_Init );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZZZ_START_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZZZ_START_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SCA_ZZZ_LOOP_1;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SCA_ZZZ_LOOP_1", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZZZ_LOOP_1_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZZZ_LOOP_1_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZZZ_LOOP_1_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SCA_ZZZ_LOOP_2;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SCA_ZZZ_LOOP_2", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZZZ_LOOP_1_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZZZ_LOOP_2_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZZZ_LOOP_2_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SCA_ZZZ_END;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SCA_ZZZ_END", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZZZ_END_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZZZ_END_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SCA_ZX_START;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SCA_ZX_START", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZX_START_StateStart );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZX_START_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SCA_ZX_ATTACK;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SCA_ZX_ATTACK", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZX_ATTACK_StateStart );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZX_ATTACK_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZX_ATTACK_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SCA_ZX_LOOP;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SCA_ZX_LOOP", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZX_LOOP_StateStart );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZX_LOOP_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZX_LOOP_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SCA_ZX_END;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SCA_ZX_END", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_ZX_END_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SCA_XZ_CHARGE;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SCA_XZ_CHARGE", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_XZ_CHARGE_StateStartFuture );
			stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_XZ_CHARGE_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_XZ_CHARGE_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_XZ_CHARGE_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SCA_XZ_FIRE;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SCA_XZ_FIRE", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_XZ_FIRE_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SCA_XZ_FIRE_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SDK_ZZZX;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SDK_ZZZX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SDK_ZZZX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SDK_ZZZXX;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SDK_ZZZXX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SDK_ZZZXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SDK_XX;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SDK_XX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SDK_XX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SDK_XXX;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SDK_XXX", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SDK_XXX_Init );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SDK_XXX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SDK_XXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SDK_XX;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SDK_XX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SDK_XX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SDK_XXX;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SDK_XXX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SDK_XXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SDK_XXXX;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SDK_XXXX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SDK_XXXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SDK_XXXXX;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SDK_XXXXX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SDK_XXXXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SDK_XXXXXX;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SDK_XXXXXX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SDK_XXXXXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SDK_XXXXXXX;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SDK_XXXXXXX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SDK_XXXXXXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SDK_ZX_CHARGE;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SDK_ZX_CHARGE", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SDK_ZX_CHARGE_StateStartFuture );
			stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SDK_ZX_CHARGE_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SDK_ZX_CHARGE_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SDK_ZX_CHARGE_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SDK_ZX_FIRE;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SDK_ZX_FIRE", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SDK_ZX_FIRE_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SDK_ZX_FIRE_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;

	default:
		break;
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	UserUnitStateData stateData;
	switch ( GetUnitClass() )
	{
	case CX2Unit::UC_ELESIS_KNIGHT:
		{

		} break;
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	case CX2Unit::UC_ELESIS_GRAND_MASTER:
		{
			// 커맨드, 콤보 시작
			// 대시 점프 콤보 XZ
			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SGM_XZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SGM_XZ", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SGM_XZ_Init );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SGM_XZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			// 커맨드, 콤보 시작
			// 대시 점프 콤보 XZ
			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SGM_XZ_LOOP;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SGM_XZ_LOOP", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SGM_XZ_LOOP_EventProcess );

			m_StateList[stateData.stateID] = stateData;

			// 대시 점프 콤보 XZ 다운
			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SGM_XZ_DOWN;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SGM_XZ_DOWN", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SGM_XZ_DOWN_EventProcess );

			m_StateList[stateData.stateID] = stateData;

			// 콤보 XX 다운 X 시작
			stateData.Init();
			stateData.stateID			= ESI_COMBO_SGM_XX_DOWN_X_START;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SGM_XX_DOWN_X_START", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SGM_XX_DOWN_X_START_Init );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SGM_XX_DOWN_X_START_FrameMove );
			stateData.StateStart		= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SGM_XX_DOWN_X_START_StateStart );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SGM_XX_DOWN_X_START_EventProcess );

			m_StateList[stateData.stateID] = stateData;

			// 콤보 XX 다운 X 루프
			stateData.Init();
			stateData.stateID			= ESI_COMBO_SGM_XX_DOWN_X_LOOP;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SGM_XX_DOWN_X_LOOP", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SGM_XX_DOWN_X_LOOP_EventProcess );

			m_StateList[stateData.stateID] = stateData;

			// 콤보 XX 다운 X 끝
			stateData.Init();
			stateData.stateID			= ESI_COMBO_SGM_XX_DOWN_X_END;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SGM_XX_DOWN_X_END", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SGM_XX_DOWN_X_END_EventProcess );

			m_StateList[stateData.stateID] = stateData;

			// 상급 기사 숙련, 대미지를 입어 뒤로 뒤를 때 공격 입력 시, 해당 State 로 전환
			stateData.Init();
			stateData.stateID			= ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED;
			m_LuaManager.MakeTableReference( "ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED_EventProcess );
			
			m_StateList[stateData.stateID] = stateData;

			// 상급 기사 숙련, 대미지를 입어 앞으로 뒤를 때 공격 입력 시, 해당 State 로 전환
			stateData.Init();
			stateData.stateID			= ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED;
			m_LuaManager.MakeTableReference( "ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED_EventProcess );

			m_StateList[stateData.stateID] = stateData;
			// 커맨드, 콤보 끝
		} //break;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	case CX2Unit::UC_ELESIS_SABER_KNIGHT:
		{
			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SSK_ZZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SSK_ZZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SSK_ZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SSK_ZZZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SSK_ZZZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SSK_ZZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SSK_ZZX;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SSK_ZZX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SSK_ZZX_EventProcess );
			m_StateList[stateData.stateID] = stateData;
			
			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SSK_XX;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SSK_XX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SSK_XX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SSK_XXX;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SSK_XXX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SSK_XXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

		} break;
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	case CX2Unit::UC_ELESIS_BLAZING_HEART:
		{
			stateData.Init();
			stateData.stateID			= ESI_COMBO_SBH_XZ;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SBH_XZ", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZ_Init );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZ_EventProcess );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZ_FrameMove );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SBH_XZZ;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SBH_XZZ", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZZ_Init );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_COMBO_SBH_XZZZ;
			m_LuaManager.MakeTableReference( "ESI_COMBO_SBH_XZZZ", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZZZ_Init );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZZZ_EventProcess );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_COMBO_SBH_XZZZ_FrameMove );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SBH_X;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SBH_X", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_X_Init );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_X_EventProcess );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_X_FrameMove );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_X_FrameMoveFuture );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SBH_XX;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SBH_XX", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XX_Init );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XX_EventProcess );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XX_FrameMove );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XX_FrameMoveFuture );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SBH_XXX;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SBH_XXX", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXX_Init );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXX_EventProcess );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXX_FrameMove );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXX_FrameMoveFuture );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SBH_XXXZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SBH_XXXZ", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXXZ_Init );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXXZ_EventProcess );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXXZ_FrameMove );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SBH_XXXZ_FrameMoveFuture );
			m_StateList[stateData.stateID] = stateData;
		}
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
	case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
		{
			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SPK_ZX;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SPK_ZX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SPK_ZX_EventProcess );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SPK_ZX_FrameMove );
			m_StateList[stateData.stateID] = stateData;
			
			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SPK_ZZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SPK_ZZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SPK_ZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_COMBO_SPK_ZZZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_COMBO_SPK_ZZZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SPK_ZZZ_EventProcess );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_COMBO_SPK_ZZZ_FrameMove );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SPK_XZ;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SPK_XZ", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_XZ_StartFurture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_XZ_EventProcess );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_XZ_FrameMove );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= ESI_DASH_JUMP_COMBO_SPK_XZ_REATTACK;
			m_LuaManager.MakeTableReference( "ESI_DASH_JUMP_COMBO_SPK_XZ", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_XZ_REATTACK_StartFurture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_XZ_EventProcess );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUEL, ESI_DASH_JUMP_COMBO_SPK_XZ_FrameMove );
			m_StateList[stateData.stateID] = stateData;
		} break;
	default:
		ASSERT(!"GetUnitClass Error");
		break;
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}


void CX2GUEL::SetEquippedSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, EL_STATE_ID eStateID_ )
{
	if ( NULL != pSkillTemplet_ )
		SetSkillLevelStateData( pSkillTemplet_, m_StateList[eStateID_] );
}

void CX2GUEL::SetSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, UserUnitStateData& stateData_ )
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1o: June 3201f850 ( reached through vslot 0x7a8 32028190 ): the upgrade-swap level and the additional power rate
	if ( NULL == pSkillTemplet_ || NULL == GetUnit() )
		return;

	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet_->m_eID, false, 0, true ) );

	stateData_.m_SPLevel	= iSkillTempletLevel;
	stateData_.m_fPowerRate	= pSkillTemplet_->GetSkillPowerRateValue( iSkillTempletLevel );
	stateData_.m_eSkillID	= pSkillTemplet_->m_eID;
	stateData_.m_fPowerRate	= ( GetSkillAdditionalPowerRate( static_cast<int>( pSkillTemplet_->m_eID ) ) + 1.f ) * stateData_.m_fPowerRate;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( NULL == pSkillTemplet_ )
		return;

	if ( NULL == GetUnit() )
		return;

	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet_->m_eID ) );	/// 스킬 레벨

	stateData_.m_SPLevel	= iSkillTempletLevel;
	stateData_.m_fPowerRate	= pSkillTemplet_->GetSkillPowerRateValue( iSkillTempletLevel );
	stateData_.m_eSkillID	= pSkillTemplet_->m_eID;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ShowActiveSkillCutInAndLight( float fTimeToShow_, bool bOnlyLight_ /*= false*/ )
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1k: June 32b70af0 ( 32b70d10 ShowActiveSkillCutInAndLightByScript jumps here )
	if ( true == m_pXSkinAnim->EventTimerOneshot( fTimeToShow_ ) && true == GetShowCutInAndChangeWorldColor() && true == GetShowActiveSkillShow() )
	{
		SkillCutIn cutIn = s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[0];

		// not ported: the cut-in item override ( 31fd8a50, CX2CustomizeManager: backlog Stage 12 )
		IF_EVENT_ENABLED( 0x8b3 )
			cutIn = s_SkillCutInSetEvent8b3[ ( 0 == ( rand() & 1 ) ) ? 0 : 1 ];	// even rand() -> CASH_21

#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_UI
		CX2OfflineLog::Server( L"[JUNE_CHAR] ELESIS UI cut-in set %d name %s", m_iSkillCutInSetIndex, cutIn.fileName.c_str() );	// char-elesis CH5
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_UI
		ActiveSkillShow( L"Dummy2_Lhand", cutIn.fileName.c_str(), cutIn.vSize, cutIn.vPosition, IsHyperState(), bOnlyLight_ );
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( fTimeToShow_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( fTimeToShow_ ) == true && EventCheck( fTimeToShow_, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( GetShowCutInAndChangeWorldColor() && GetShowActiveSkillShow() )
		{
			ActiveSkillShow( L"Dummy2_Lhand", s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[0].fileName.c_str(),
				s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[0].vSize,
				s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[0].vPosition, IsHyperState(), bOnlyLight_ );
		}
	}	
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
/*virtual*/ void CX2GUEL::ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ )
{
	ShowActiveSkillCutInAndLight( fTimeToShow_, bOnlyLight_ );
}
bool CX2GUEL::CanUseSpecialAttack( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition/* = CX2SkillTree::ASUT_GROUND*/ )
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1k: June 32b84a50 ( user vslot 0x7f8, step 510 ): chooses the skill of the pressed slot and checks that it can be used now
	if ( true == IsQuickSlotSpecialItemWaiting() )
		return false;

	if ( NULL == GetUnit() )
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
		PushInfoText( 0x8bae );
		return false;
	}

	m_iChosenSkillID = pSkillTemplet->m_eID;

	int iSkillTempletLevel = accessUserSkillTree.GetSkillLevel( pSkillTemplet->m_eID, false, 0, true );
	if ( iSkillTempletLevel < 1 )
		iSkillTempletLevel = 1;

	// War Prelude is a ground skill until Dignity of Grand Master is learned
	CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eSkillUseCondition = pSkillTemplet->m_eActiveSkillUseCondtion;
	if ( m_usDignityOfGrandMasterLevel < 1 && ( CX2SkillTree::RNSI_SA_SGM_WAR_PRELUDE == pSkillTemplet->m_eID || CX2SkillTree::RNSI_SA_SGM_WAR_PRELUDE_UPGRADE == pSkillTemplet->m_eID ) )
		eSkillUseCondition = CX2SkillTree::ASUT_GROUND;

	if ( false == CheckSkillUseCondition( eActiveSkillUseCondition, eSkillUseCondition ) )
		return false;

#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON
	if ( false == CheckSummonSpecialAttack( pSkillTemplet->m_eID ) )	// June 3202daf0
		return false;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON

	if ( false == CheckSkillUsePVPOfficial( pSkillTemplet->m_eID ) )
		return false;

	if ( false == CheckSkillUseNotPve( pSkillTemplet->m_eID ) )
		return false;

	if ( false == CheckSkillUseNotDungeon( pSkillTemplet->m_eID ) )
		return false;

	if ( true == IsRidingKeySkillWaiting() )
		return false;

	if ( false == IsValideSkillRidingOn( pSkillTemplet->m_eID ) )
		return false;

	m_fChosenSkillMp = GetActualMPConsume( pSkillTemplet->m_eID, iSkillTempletLevel );
	m_fMPConsume = m_fChosenSkillMp;	// stand-in: the March Rushing Sword states read it until CH2a

	const float fCoolTimeLeft = pSkillSlotData->m_fCoolTimeLeft;

#ifdef SERV_IRUHADEV_JUNE_DEV_UNLIMITED_SKILL
	if ( false == IsDevUnlimitedSkillUse() )	// June vslot 0x82c answers false ( event code 10 )
#endif SERV_IRUHADEV_JUNE_DEV_UNLIMITED_SKILL
	{
		if ( 0.f < fCoolTimeLeft )
		{
			PushInfoText( 226 );
			return false;
		}

		if ( m_fReservedSkillMp + m_fChosenSkillMp > GetNowMp() )
		{
			PushInfoText( 2549 );
			return false;
		}

		if ( CX2SkillTree::ST_HYPER_ACTIVE_SKILL == pSkillTemplet->m_eType && false == IsTrainingOrPhotozone() )
		{
			if ( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE ) <= 0
				&& false == g_pData->GetMyUser()->GetSelectUnit()->IsInfinityElEssence() )
			{
				PushInfoText( 26119 );
				return false;
			}
		}
	}

	// Injection Stigma costs HP: half the physical attack ( a fifth of that under Blood Runaway ), and the HP must stay above it
	if ( true == CX2UISkillTreeNew::IsSameOrUpgradeSkill( pSkillTemplet->m_eID, CX2SkillTree::RNSI_A_SDK_INJECTION_STIGMA ) )
	{
		float fHpCost = GetLearnedSkillAbilityValue( CX2SkillTree::SA_ATK_PHYSIC_REL, pSkillTemplet->m_eID ) * GetPhysicAttackValue();
		if ( true == HaveThisBuff( BTI_BUFF_BLOOD_RUNAWAY_PASSIVE ) )
			fHpCost *= INJECTION_STIGMA_HP_COST_RATE_BLOOD_RUNAWAY;

		if ( GetNowHp() <= fHpCost )
		{
			PushInfoText( 2627 );
			return false;
		}
	}

	m_iChosenSkillID = pSkillTemplet->m_eID;
	m_iChosenSkillSlotIndex = iPressedSkillSlotIndex;
	return true;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	KTDXPROFILE();

	if( NULL != GetUnit() )
	{
		const CX2UserSkillTree& cUserSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;	// 유저가 배운 스킬 트리

		m_iPressedSkillSlotIndex = -1;
		const CX2UserSkillTree::SkillSlotData* pSkillSlotData = NULL;

		// 사용한 스킬의 스킬슬롯 데이터와 그 인덱스를 얻어옴
		if ( false == CommonSpecialAttackEventProcess( cUserSkillTree, pSkillSlotData, m_iPressedSkillSlotIndex ) )
			return false;

		if( NULL == pSkillSlotData )
			return false;

		// 사용한 스킬
		const CX2SkillTree::SkillTemplet* pSkillTemplet 
			= GetEquippedActiveSkillTemplet( pSkillSlotData->m_eID );

		if( NULL == pSkillTemplet )
			return false;

		const int iSkillTempletLevel = max( 1, cUserSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

		// 스킬을 사용할 수 있는 조건인지 판단( ex: 공중에서는 사용 할 수 없는 필살기 등)
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( false == CheckSkillUseCondition( eActiveSkillUseCondition, pSkillTemplet->m_eActiveSkillUseCondtion ) )
#else //ADD_MEMO_1ST_CLASS
		if( false == CheckSkillUseCondition( eActiveSkillUseCondition, pSkillTemplet ) )
#endif //ADD_MEMO_1ST_CLASS
			return false;

		//소환 필살기 종류는 게임이 끝난 상태에서 사용할 수 없다.
		if( false == CheckSummonSpecialAttack( pSkillTemplet->m_eID ) )
			return false;

		// 공식대전에서 사용 가능한 스킬인지 검사
		if( false == CheckSkillUsePVPOfficial( pSkillTemplet->m_eID ) )
			return false;
#ifdef SERV_IRUHADEV_JUNE_BUFF_RIDING_KEY_WAIT
		/// June buff port Phase 7e: June Elesis 32b84a50 (slot 510, which holds June's only Elesis CheckSkillUsePVPOfficial
		/// call, so March's home is CanUseSpecialAttack) @32b84d61, after 3202cba0 and June's 3202d140 / 3202d3c0: no skill
		/// while the riding-key skill wait runs (329cc370)
		if( true == IsRidingKeySkillWaiting() )
			return false;
#endif SERV_IRUHADEV_JUNE_BUFF_RIDING_KEY_WAIT

#ifdef FIX_NO_STATE_SKILL_BUG
		// 탈 것 탑승 상태에서 사용 가능한 스킬인지 검사
		if( false == IsValideSkillRidingOn( pSkillTemplet->m_eID)  )
			return false;
#endif // FIX_NO_STATE_SKILL_BUG

		// 엠피 소모량을 얻어옴
		m_fMPConsume = GetActualMPConsume( pSkillTemplet->m_eID, iSkillTempletLevel );

		float fCoolTimeLeft = pSkillSlotData->m_fCoolTimeLeft;

		bool bSlotB = (m_iPressedSkillSlotIndex > 3) ? true : false;
		int iSlotIndex = (m_iPressedSkillSlotIndex > 3) ? m_iPressedSkillSlotIndex-4 : m_iPressedSkillSlotIndex;


#ifdef TOGGLE_UNLIMITED_SKILL_USE
#if defined( _IN_HOUSE_ ) || defined( _OPEN_TEST_ )
		if( false == g_pMain->IsMyAuthLevelHigherThan( CX2User::XUAL_OPERATOR ) || false == g_pMain->IsUnlimitedSkillUse() )
#endif //defined( _IN_HOUSE_ ) || defined( _OPEN_TEST_ )
#else //TOGGLE_UNLIMITED_SKILL_USE
#ifndef _SERVICE_
		if( false == g_pMain->IsMyAuthLevelHigherThan( CX2User::XUAL_DEV ) )
#endif _SERVICE_
#endif //TOGGLE_UNLIMITED_SKILL_USE
		{
			if( 0.f < fCoolTimeLeft )
			{
#ifdef ALWAYS_SCREEN_SHOT_TEST
				if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
				{
					return false;
				}
#endif ALWAYS_SCREEN_SHOT_TEST
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_226 ), D3DXCOLOR(1,1,1,1),
					D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				return false;
			}
			if ( GetNowMp() < m_fMPConsume )
			{
#ifdef ALWAYS_SCREEN_SHOT_TEST
				if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
				{
					return false;
				}
#endif ALWAYS_SCREEN_SHOT_TEST
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1),
					D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				return false;
			}

#ifdef FINALITY_SKILL_SYSTEM //JHKang
			if ( pSkillTemplet->m_eType == CX2SkillTree::ST_HYPER_ACTIVE_SKILL && g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
			{
				const int iItemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );

				if( iItemNum <= 0 
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
					&& false == g_pData->GetMyUser()->GetSelectUnit()->IsInfinityElEssence()
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
					)
				{
#ifdef ALWAYS_SCREEN_SHOT_TEST
				if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
				{
					return false;
				}
#endif ALWAYS_SCREEN_SHOT_TEST
					g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_26119 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
					return false;
				}
			}
#endif //FINALITY_SKILL_SYSTEM
		}

		if( NULL == pSkillTemplet )		/// pSkillTemplet을 위에서 새로 대입하기 때문에, NULL처리 추가
			return false;

		m_pSkillTemplet = pSkillTemplet;

		return true;
	}
	else
	{
		return false;
	}

	return false;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
// char-elesis CH1k: June 32b52590 ( user vslot 0x7fc, step 511 ): the use of the skill the lookup chose; June's second argument is not read
void CX2GUEL::StateChangeSpecialAttack( const CX2SkillTree::SKILL_ID eSkillID_ )
{
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( eSkillID_, true );
	if ( NULL == pSkillTemplet )	// June reads the templet without a test
		return;

	if ( true == SpecialAttackNoStageChange( pSkillTemplet ) )
	{
		SpecialAttackCooldown( pSkillTemplet->m_eID, m_fChosenSkillMp );
		SkillMemoMpRefund( eSkillID_, true, m_fChosenSkillMp );
		m_fChosenSkillMp = 0.0f;
	}
	else
	{
		ReserveSkillUse( pSkillTemplet->m_eID );

		if ( CX2SkillTree::RNSI_SA_EL_SONIC_ASSAULT_STING_UPGRADE == pSkillTemplet->m_eID )
			StateChange( ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_READY, false );
		else
			StateChangeBySkillSlot( m_iChosenSkillSlotIndex );
	}

	m_bSpecialAttackEventProcessedAtThisFrame = true;
}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
void CX2GUEL::StateChangeSpecialAttack( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ )
{
	if( NULL != GetUnit() )
	{
		CX2UserSkillTree& accessUserSkillTree =  GetUnit()->AccessUnitData().m_UserSkillTree;

		CountUsedSkill( m_iPressedSkillSlotIndex );
		UpNowMp( -m_fMPConsume );

#ifdef FINALITY_SKILL_SYSTEM //JHKang
		if ( pSkillTemplet_->m_eType == CX2SkillTree::ST_HYPER_ACTIVE_SKILL && g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
		{
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
			if ( true == g_pData->GetMyUser()->GetSelectUnit()->IsInfinityElEssence() )
				g_pX2Game->Handler_EGS_USE_FINALITY_SKILL_REQ();
			else
			{
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );

			if ( NULL != pItem )
				g_pX2Game->Handler_EGS_USE_FINALITY_SKILL_REQ( pItem->GetItemData().m_ItemUID );
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
			}
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
		}
#endif //FINALITY_SKILL_SYSTEM

		const int iSkillTempletLevel = max( 1, accessUserSkillTree.GetSkillLevel( pSkillTemplet_->m_eID ) );	/// 스킬 레벨
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet_->m_eID, GetActualCoolTime( pSkillTemplet_, iSkillTempletLevel) );

#ifdef SERV_IRUHADEV_JUNE_BUFF_NEW_SKILL
		/// June buff port Phase 6b: June CX2GUEL vtable +0x800 (32b73e60) @32b73fcd calls the cool-time helper 3235a710 (BBT 100) after the cooldown is set
		if ( NULL != GetUnit() )
			GetUnit()->ApplySkillCoolTimeRateByBuff( pSkillTemplet_->m_eID );
#endif SERV_IRUHADEV_JUNE_BUFF_NEW_SKILL
#ifdef SERV_IRUHADEV_JUNE_RANDOM_MISSION
		/// June 32b73e60 @32b73fd8: random mission events 10-12, the skill-use hook 31fcd3d0 right after 3235a710
		CX2RandomMissionManager::GetInstance()->OnSkillUse( pSkillTemplet_->m_eID );
#endif SERV_IRUHADEV_JUNE_RANDOM_MISSION
		if( false == SpecialAttackNoStageChange( pSkillTemplet_ ) )
		{
			if( GetRemainHyperModeTime() <= 0.f )
			{
				if( m_iPressedSkillSlotIndex > 3 ) //슬롯 B일때
				{
					StateChange( _CONST_GUUSER_::aNormalSkillStateIDSlotB[m_iPressedSkillSlotIndex-4] );
				}
				else
				{
					StateChange( _CONST_GUUSER_::aNormalSkillStateID[m_iPressedSkillSlotIndex] );
				}
			}
			else
			{
				if( m_iPressedSkillSlotIndex > 3 ) //슬롯 B일때
				{
					StateChange( _CONST_GUUSER_::aHyperSkillStateIDSlotB[m_iPressedSkillSlotIndex-4] );
				}
				else
				{
					StateChange( _CONST_GUUSER_::aHyperSkillStateID[m_iPressedSkillSlotIndex] );
				}
			}
		}

		m_iNowSpecialAttack = m_iPressedSkillSlotIndex + 1;
		m_bSpecialAttackEventProcessedAtThisFrame = true;
	}

}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
#ifdef FIX_NO_STATE_SKILL_BUG
/** @function : IsValideSkillRidingOn
	@brief : 탈 것 탑승중에 사용 할 수 있는 스킬인지 체크.
	@return : 사용 가능 : true 사용 불가 falle
*/
/*virtual*/ bool CX2GUEL::IsValideSkillRidingOn( CX2SkillTree::SKILL_ID eSkill_ID_ )
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1k: June 32b4ef10 ( user vslot 0x818 ): no Endurance Power ( March's id and June's two ) while riding
	if ( true == GetRidingOn() )
	{
		switch( eSkill_ID_ )
		{
		case CX2SkillTree::SI_A_EL_ENDURANCE_POWER:
		case CX2SkillTree::RNSI_A_EL_ENDURANCE_POWER:
		case CX2SkillTree::RNSI_A_EL_ENDURANCE_POWER_UPGRADE:
			return false;
		}
	}

	return true;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == GetRidingOn() )
	{
		switch( eSkill_ID_ ) 
		{
		case CX2SkillTree::SI_A_EL_ENDURANCE_POWER: // 극기 - 강(强)
			{
				return false;
			} break;
		default:
			return true;
			break;
		}
	}
	else
	{
		return true;
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
#endif // FIX_NO_STATE_SKILL_BUG
/*virtual*/ void CX2GUEL::ReInit( bool bTeamPos /*= false*/, int iStartPosIndex /*= -1*/ )
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1h: June 32b81900
	CX2GUUser::ReInit( bTeamPos, iStartPosIndex );

	if( true == IsMyUnit() )
	{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
		// the point moves one step toward the centre first ( June 334355b0 = 1.0 )
		const int iWayOfSwordState = GetWayOfSwordState();
		if( WSS_DESTRUCTION == iWayOfSwordState )
			SetWayOfSwordPoint( static_cast<float>( GetWayOfSwordPoint() + 1.0 ) );
		else if( WSS_VIGOR == iWayOfSwordState )
			SetWayOfSwordPoint( static_cast<float>( GetWayOfSwordPoint() - 1.0 ) );

		// only a PvP game ( type 1, not 4 ) resets the losing team; every other game falls into the TOWARD branch
		if( CX2Game::GT_PVP == g_pX2Game->GetGameType() &&
			( ( CX2Room::TN_RED == GetTeam() && g_pX2Game->GetRedTeamKill() > g_pX2Game->GetBlueTeamKill() ) ||
			  ( CX2Room::TN_BLUE == GetTeam() && g_pX2Game->GetRedTeamKill() < g_pX2Game->GetBlueTeamKill() ) ) )
		{
			if( WSS_VIGOR == iWayOfSwordState )
			{
				End_SwordmanVigor();
			}
			else if( WSS_DESTRUCTION == iWayOfSwordState )
			{
				End_SwordmanDestruction();
			}
			SetWayOfSwordPoint( 0 );
			SetWayOfSwordState( static_cast<int>( WSS_CENTER ) );
		}
		else
		{
			if( WSS_VIGOR == iWayOfSwordState )
			{
				End_SwordmanVigor();
				SetWayOfSwordState( static_cast<int>( WSS_TOWARD_VIGOR ) );
			}
			else if( WSS_DESTRUCTION == iWayOfSwordState )
			{
				End_SwordmanDestruction();
				SetWayOfSwordState( static_cast<int>( WSS_TOWARD_DESTRUCTION ) );
			}
		}
		m_FrameDataFuture.syncData.m_CannonBallCount = GetWayOfSwordState();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
		if( CX2Game::GT_PVP == g_pX2Game->GetGameType() )
		{
			const WAY_OF_SWORD_STATE eWayOfSwordState = static_cast<const WAY_OF_SWORD_STATE>( GetWayOfSwordState() );

			if( GetTeam() == CX2Room::TN_RED && g_pX2Game->GetRedTeamKill() > g_pX2Game->GetBlueTeamKill() ||
				GetTeam() == CX2Room::TN_BLUE && g_pX2Game->GetRedTeamKill() < g_pX2Game->GetBlueTeamKill() )
			{
				if( eWayOfSwordState == WSS_VIGOR )
				{
					End_SwordmanVigor();
				}
				else if( eWayOfSwordState == WSS_DESTRUCTION )
				{
					End_SwordmanDestruction();
				}
				SetWayOfSwordPoint( 0 );
				SetWayOfSwordState( static_cast<int>( WSS_CENTER ) );
			}
			else
			{
				if( eWayOfSwordState == WSS_VIGOR )
				{
					End_SwordmanVigor();
					SetWayOfSwordState( static_cast<int>( WSS_TOWARD_VIGOR ) );
				}
				else if( eWayOfSwordState == WSS_DESTRUCTION )
				{
					End_SwordmanDestruction();
					SetWayOfSwordState( static_cast<int>( WSS_TOWARD_DESTRUCTION ) );
				}
			}
			m_FrameDataFuture.syncData.m_CannonBallCount = GetWayOfSwordState();
		}
		else
		{
			switch ( GetWayOfSwordState() )
			{
			case WSS_VIGOR:
				{
					Start_SwordmanVigor();
				} break;
			case WSS_DESTRUCTION:
				{
					Start_SwordmanDestruction();
				} break;
			default:
				break;
			}
		}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
	}

#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_BLOOD
	// every unit, after the Way of the Sword block ( June 32b81b85 )
	ClearShadowEdgeRecords();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_BLOOD
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CX2GUUser::ReInit( bTeamPos, iStartPosIndex );
#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
	// 대전 중 지고 있다면 검의길 효과 초기화 
	if( true == IsMyUnit() )
	{
		if( CX2Game::GT_PVP == g_pX2Game->GetGameType() )
		{
			const WAY_OF_SWORD_STATE eWayOfSwordState = static_cast<const WAY_OF_SWORD_STATE>( GetWayOfSwordState() );

			if( GetTeam() == CX2Room::TN_RED && g_pX2Game->GetRedTeamKill() > g_pX2Game->GetBlueTeamKill() ||
				GetTeam() == CX2Room::TN_BLUE && g_pX2Game->GetRedTeamKill() < g_pX2Game->GetBlueTeamKill() )
			{
				if( eWayOfSwordState == WSS_VIGOR )
				{
					End_SwordmanVigor();
				}
				else if( eWayOfSwordState == WSS_DESTRUCTION )
				{
					End_SwordmanDestruction();
				}
				SetWayOfSwordPoint( 0 );
				SetWayOfSwordState( static_cast<int>( WSS_CENTER ) );
			}
			else
			{
				if( eWayOfSwordState == WSS_VIGOR )
				{
					End_SwordmanVigor();
					SetWayOfSwordState( static_cast<int>( WSS_TOWARD_VIGOR ) );
				}
				else if( eWayOfSwordState == WSS_DESTRUCTION )
				{
					End_SwordmanDestruction();
					SetWayOfSwordState( static_cast<int>( WSS_TOWARD_DESTRUCTION ) );
				}
			}
			m_FrameDataFuture.syncData.m_CannonBallCount = GetWayOfSwordState();
		}
		else
		{
			switch ( GetWayOfSwordState() )
			{
			case WSS_VIGOR:
				{
					Start_SwordmanVigor();
				} break;
			case WSS_DESTRUCTION:
				{
					Start_SwordmanDestruction();
				} break;
			default:
				break;
			}
		}
	}
#endif // ELSWORD_WAY_OF_SWORD

#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
/*virtual*/ void CX2GUEL::DamageReact( OUT CX2DamageManager::DamageData* pDamageData_ )
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1h: June 32b73ff0
	KTDXPROFILE();

	if( NULL == pDamageData_ )
		return;

	if( WSS_DESTRUCTION == GetWayOfSwordState() &&
		true == IsDamagedReaction( pDamageData_ ) &&
		CX2DamageManager::DT_BUFF != pDamageData_->damageType &&
		true == DamageFlushMp() &&
		false == g_pMain->GetIsPlayingTutorial() )	// June +0x16a of g_pMain
	{
		DetonationStateChange( pDamageData_ );
	}
	else if( true == m_bIsActiveEndurancePower &&
			 true == CanSpecialReactProcess( pDamageData_->reActType ) &&
			 false == IsSuperArmor() )
	{
		// the upgraded Endurance Power ( the base skill is deleted once it is learned ) has its own state
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_EL_ENDURANCE_POWER, true );
		if( NULL != pSkillTemplet && 0 != pSkillTemplet->m_iDeleteSkillIDByUpgrade )
			StateChange( ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_MEMO, true );
		else
			StateChange( ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE, true );
	}
	else
	{
		CX2GUUser::DamageReact( pDamageData_ );
		CX2GUUser::DamageReactStateChange( pDamageData_, -1, -1 );
	}

	if(	IsMyUnit() == true &&
		pDamageData_->reActResult != CX2DamageManager::RT_REVENGE &&
		true == CanSpecialReactProcess(pDamageData_->reActType) )
	{
		const int iWayOfSwordState = GetWayOfSwordState();
		if( WSS_VIGOR != iWayOfSwordState )
		{
			float fWSPGain = g_pData->GetDamageManager()->CalcWayOfSwordPointOnDamageReact( pDamageData_->damage.GetAllDamage(), iWayOfSwordState );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
			fWSPGain = static_cast<float>( fWSPGain * static_cast<double>( GetWayOfSwordPointMultiplier() ) );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_BLOOD
			if( 0 < m_usMadnessLevel && true == m_bIsMadnessActive )
				fWSPGain += fWSPGain;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_BLOOD

			SetWayOfSwordPoint( GetWayOfSwordPoint() + fWSPGain );
		}
	}

	if( true == m_bIsProvokeStateChange && CX2DamageManager::DT_BUFF != pDamageData_->damageType )
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_CSI_CIP_IRON_WILL", this );
		StateChange( ESI_A_SGM_PROVOKE_REVENGE, true );
		m_bIsProvokeStateChange = false;
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	KTDXPROFILE();

	if( WSS_DESTRUCTION == GetWayOfSwordState() && // 섬멸 상태 일 때 기폭 예외처리
		true == IsDamagedReaction( pDamageData_ ) &&
		true == DamageFlushMp() )
	{
		// 섬멸 기폭 시 Damage를 입지 않도록 하기 위해 DamageReact 타지 않도록 예외처리
		DetonationStateChange( pDamageData_ );
	}
	// 극기 - 강(强)
	else if( true == m_bIsActiveEndurancePower &&
			 true == CanSpecialReactProcess( pDamageData_->reActType ) 
#ifdef FIX_ENDURANCE_POWER
			&& false == IsSuperArmor() 
#endif // FIX_ENDURANCE_POWER
			)
	{
		DoStateEndurancePowerRage( false );
	}
	else
	{
		CX2GUUser::DamageReact( pDamageData_ );
		CX2GUUser::DamageReactStateChange( pDamageData_ );
	}


#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
	if(	IsMyUnit() == true &&
		pDamageData_->reActResult != CX2DamageManager::RT_REVENGE &&
		true == CanSpecialReactProcess(pDamageData_->reActType) )
	{
		const int iWayOfSwordState = GetWayOfSwordState();
		if( WSS_VIGOR != iWayOfSwordState )
		{ // 엘리 검의길은 질풍 상태에서 소모시키지 않음.
			const float fWSPGain = 
				g_pData->GetDamageManager()->CalcWayOfSwordPointOnDamageReact( pDamageData_->damage.GetAllDamage(), iWayOfSwordState );

			SetWayOfSwordPoint( GetWayOfSwordPoint() + fWSPGain );
		}
	}
#endif //ELSWORD_WAY_OF_SWORD
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	if ( true == GetIsProvokeStateChange() )
	{		
		StateChange( ESI_A_SGM_PROVOKE_REVENGE, true );
		SetIsProvokeStateChange( false );
	}
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
/*virtual*/ void CX2GUEL::Win()
{
	CX2GUUser::Win();
	StateChange( GetWinStateID() );
}
/*virtual*/ void CX2GUEL::Lose()
{
	CX2GUUser::Lose();

	if ( true == IsMyUnit() && 0.f < GetNowHp() )
	{
		StateChange( GetLoseStateID() );
	}
}
/*virtual*/ void CX2GUEL::Draw()
{
	CX2GUUser::Draw();
	StateChange( GetWinStateID() );
}
/*virtual*/ void CX2GUEL::InitEffect()
{
	CX2GUUser::InitEffect();

	CreateMajorParticleArray();
	CreateMinorParticleArray();
}
/*virtual*/ void CX2GUEL::InitEnchantWeaponEffectUnitSpecific()
{
	BOOST_TEST_FOREACH( Weapon*, pWeapon, m_vecpWeapon )
	{
		ASSERT( NULL != pWeapon );
		pWeapon->InitEnchantParticleForSword();
	}
}
/*virtual*/ void CX2GUEL::CreateMajorParticleArray()
{
	CX2GUUser::CreateMajorParticleArray();
	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

	// small damage 타격을 줬을 때 나오는 파티클
	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT,			L"DamageImpact",			2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_CORE,		L"DamageImpactCore",		2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_SLASH,		L"DamageImpactSlash",		2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	// AttackResult의 switch case에서 default:로 처리되는 타격에 대한 파티클 (현재는 small 이외의 파티클)
	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_RED,		L"DamageImpactRed",			2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_RING_RED,	L"DamageImpactRingRed",		1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_CORE_RED,	L"DamageImpactCoreRed",		1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_SLASH_RED,	L"DamageImpactSlashRed",	1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );
}
/*virtual*/ void CX2GUEL::CreateMinorParticleArray()
{
	CX2GUUser::CreateMinorParticleArray();
}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
// char-elesis CH1o: June 32b90430 ( vslot 0x534 ): Endurance Power's two active states count too, for the now state only
/*virtual*/ bool CX2GUEL::IsDamageState( const bool bFuture_ )
{
	if ( true == CX2GUUser::IsDamageState( bFuture_ ) )
		return true;

	if ( false == bFuture_
		&& ( ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_MEMO == GetNowStateID() || ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE == GetNowStateID() ) )
		return true;

	return false;
}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
/*static*/ void CX2GUEL::AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
	if ( g_pData->GetGameMajorParticle() == NULL )
		return;

	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"DamageImpact" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"DamageImpactCore" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"DamageImpactSlash" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"DamageImpactRed" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"DamageImpactRingRed" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"DamageImpactCoreRed" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"DamageImpactSlashRed" );
}
/*static*/ void CX2GUEL::AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ )
{

}
/*virtual*/ bool CX2GUEL::SpecialAttackNoStageChange( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ )
{
	if( NULL == pSkillTemplet_ )
		return false;

#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1k: June 32b72550 ( user vslot 0x810 ): Endurance Power ( March's id and June's two ) acts without a state change
	switch( pSkillTemplet_->m_eID )
	{
	case CX2SkillTree::SI_A_EL_ENDURANCE_POWER:
	case CX2SkillTree::RNSI_A_EL_ENDURANCE_POWER:
	case CX2SkillTree::RNSI_A_EL_ENDURANCE_POWER_UPGRADE:
		m_FrameDataFuture.syncData.nowAction = UAI_ENDURANCE_POWER;
		return true;

	default:
		return false;
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	switch( pSkillTemplet_->m_eID )
	{
	case CX2SkillTree::SI_A_EL_ENDURANCE_POWER:
		{
			m_FrameDataFuture.syncData.nowAction = UAI_ENDURANCE_POWER;
		} break;
	default:
		return false;
		break;
	}
	return true;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
/*virtual*/ void CX2GUEL::COMMON_BUFF_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.2f ) && true == EventCheck( 0.2f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		PlayCommonBuffMinorParticle();
		UpDownCrashCamera( 30.0f, 0.4f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( 0.25f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( true == m_pXSkinAnim->EventTimer( 0.25f ) && true == EventCheck( 0.25f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		int	iSkillSlotIndex = 0;
		bool bSlotB = false;

		if ( true == GetSkillSlotIndexUsed( iSkillSlotIndex, bSlotB ) )
		{
			const CX2Unit::UnitData* pUnitData = &GetUnit()->GetUnitData();

			const CX2UserSkillTree::SkillSlotData* pSkillSlotData = pUnitData->m_UserSkillTree.GetSkillSlot( iSkillSlotIndex, bSlotB );
			CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_NONE;

			if( NULL != pSkillSlotData )
				eSkillID = pSkillSlotData->m_eID;

			ProcessAcceleraterBuff( eSkillID, IsHyperState() );
		}
	}

	CommonFrameMove();
}
/*virtual*/ void CX2GUEL::HyperModeFrameMove()
{
#ifdef MODIFY_RIDING_PET_AWAKE
	CommonHyperModeFrameMove( 0.05f, 0.83f );
#else
	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.05f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.05f ) == true && EventCheck(0.05f, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		ShowMinorParticleHyperModeChange();
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.83f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.83f ) == true && EventCheck(0.83f, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		ShowMinorParticleHyperModeShock();

		UpDownCrashCamera( 30.0f, 0.4f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

		ShowMinorParticleHyperModeTrace();
		ApplyHyperModeBuff();
	}
	CommonFrameMove();
#endif // MODIFY_RIDING_PET_AWAKE
}
/*virtual*/ void CX2GUEL::HyperModeBuffEffectStart()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1h: June 32b74300; the hyper boost handles are skipped for a transcended unit ( unit +0x3c ) or an awakening effect set
	CX2GUUser::HyperModeBuffEffectStart();

	if( ( NULL != GetUnit() && 0 != GetUnit()->GetUnitData().m_iUnitClassTran ) || true == _CONST_EL_CH1C_::ElesisAwakeningEffectSetPresent() )
		return;

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRFoot )
		m_hHyperBoostRFoot	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostRight",	0, 0, 0, 0, 0);

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLFoot )
		m_hHyperBoostLFoot	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostLeft",	0, 0, 0, 0, 0);

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRArm )
		m_hHyperBoostRArm	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostRight",	0, 0, 0, 0, 0);

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLArm )
		m_hHyperBoostLArm	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostLeft",	0, 0, 0, 0, 0);
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CX2GUUser::HyperModeBuffEffectStart();

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRFoot )
		m_hHyperBoostRFoot	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostRight",	0, 0, 0, 0, 0);

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLFoot )
		m_hHyperBoostLFoot	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostLeft",	0, 0, 0, 0, 0);

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRArm )
		m_hHyperBoostRArm	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostRight",	0, 0, 0, 0, 0);

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLArm )
		m_hHyperBoostLArm	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostLeft",	0, 0, 0, 0, 0);
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
/** @function	: SetSpecificValueByBuffTempletID
	@brief		: 각 유닛마다 특정 버프가 실행 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 시전중이라는 플래그 설정 등...)
	@param		: 버프템플릿ID(eBuffTempletId_)
*/
/*virtual*/ void CX2GUEL::SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ )
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON
	// char-elesis CH3b0: June 32b89260 ( vslot 0x24c ): June's skill ids, the enhanced Victorious Sword, Soul Ignition's effect-set rate, the Blood Runaway saved debuff
	CX2GUUser::SetSpecificValueByBuffTempletID( eBuffTempletId_ );

	switch ( eBuffTempletId_ )
	{
	case BTI_BUFF_FLAME_SWORD:
		m_bIsFrameSword = true;
		break;

	case BTI_BUFF_ENDURANCE_POWER:
		m_bIsActiveEndurancePower = true;
		break;

	case BTI_BUFF_VICTORIOUS_SWORD:
	case BTI_BUFF_SGM_VICTORIOUS_SWORD:
	case BTI_BUFF_SGM_VICTORIOUS_SWORD_UPGRADE:
		{
			m_fDodgeAndSlashCoolTimeDecreaseRate = 1.f;

			const int iSkillLevel = ( NULL != GetUnit() ? GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::RNSI_SA_SGM_VICTORIOUS_SWORD, false, 0, true ) : 0 );
			if ( 0 < iSkillLevel )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_SGM_VICTORIOUS_SWORD, true );
				if ( NULL != pSkillTemplet )
					m_fDodgeAndSlashCoolTimeDecreaseRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DECREASE_REL, iSkillLevel, false );
			}

			m_bIsApplyVictoriousSword = true;
		} break;

	case BTI_BUFF_SOUL_IGNITION:
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_SA_SBH_SOUL_IGNITION, true );
			if ( NULL != pSkillTemplet )
			{
				const int iSkillLevel = ( NULL != GetUnit() ? GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::RNSI_SA_SBH_SOUL_IGNITION, false, 0, true ) : 0 );
				m_fCreateEffectSetRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevel, false );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON_DEBUG
				CX2OfflineLog::Server( L"[JUNE_CHAR] ELESIS SUMMON soul ignition level %d effect set rate %.3f", iSkillLevel, m_fCreateEffectSetRate );	// char-elesis CH3b0
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON_DEBUG
			}
		} break;

	default:
		break;
	}

	if ( 0 < m_uiFireBlossomsSkillLevel && true == m_bIsFireBlossoms )
	{
		CX2BuffTempletPtr ptrBuffTemplet = CX2BuffTempletManager::GetInstance()->GetBuffTempletPtr( eBuffTempletId_ );
		if ( NULL != ptrBuffTemplet && true == ptrBuffTemplet->IsDeBuff() && false == ptrBuffTemplet->IsWorldBuff() && false == ptrBuffTemplet->IsNonControlDebuff() )
			m_eFireBlossomsSaveDebuffID = eBuffTempletId_;
	}

	if ( 0 != m_usBloodRunawayLevel && true == IsBloodRunawayCleanseReady() )
	{
		CX2BuffTempletPtr ptrBuffTemplet = CX2BuffTempletManager::GetInstance()->GetBuffTempletPtr( eBuffTempletId_ );
		if ( NULL != ptrBuffTemplet && true == ptrBuffTemplet->IsDeBuff() && false == ptrBuffTemplet->IsWorldBuff() && false == ptrBuffTemplet->IsNonControlDebuff() )
			m_eBloodRunawaySaveDebuffID = eBuffTempletId_;
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON
	CX2GUUser::SetSpecificValueByBuffTempletID( eBuffTempletId_ );

 	switch ( eBuffTempletId_ )
 	{
	// 극기 - 강(强)
//{{ Iruha : 2026-10-03 // June buff port Phase 2 (JUNE_BUFF_PORT_PHASES.md)
#ifdef SERV_IRUHADEV_JUNE_BUFF_ENUM
	case BTI_BUFF_ENDURANCE_POWER:
#else
	case BTI_BUFF_INDURANCE_POWER:
#endif SERV_IRUHADEV_JUNE_BUFF_ENUM
//}}
		{
			m_bIsActiveEndurancePower = true;
		} break;
	// 승리의 검
	case BTI_BUFF_VICTORIOUS_SWORD:
	case BTI_BUFF_SGM_VICTORIOUS_SWORD:
		{
			// 닷지&슬래시 쿨타임 감소 비율 적용
			{
				m_fDodgeAndSlashCoolTimeDecreaseRate = 1.f;

				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_SA_SSK_VICTORIOUS_SWORD;
				const int iSkillLevel = GetMyUnitSkillLevel(eSkillID);
				if ( 0 < iSkillLevel )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( eSkillID );
					if( NULL != pSkillTemplet )
					{
						// 닷지&슬래시 쿨타임 감소 비율
						m_fDodgeAndSlashCoolTimeDecreaseRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DECREASE_REL, iSkillLevel );
					}
				}
			}

			// 닷지&슬래시 쿨타임, MP 변경 기능 활성화를 위한 변수 설정
			m_bIsApplyVictoriousSword = true;
		} break;
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	// 홍련의 검 활성화
	case BTI_BUFF_FLAME_SWORD:
		{
			m_bIsFrameSword = true;
		} break;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
 	default:
 		break;
 	}

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	//스킬을 배웠는지, 체크 타임이 지나 활성화 되었는지 체크
	if( 0 < m_uiFireBlossomsSkillLevel && true == m_bIsFireBlossoms )
	{
		// 디버프가 걸렸을때인지 체크
		CX2BuffTempletPtr ptrBuffTemplet = CX2BuffTempletManager::GetInstance()->GetBuffTempletPtr( eBuffTempletId_ );
		if( NULL != ptrBuffTemplet && true == ptrBuffTemplet->IsDeBuff() && false == ptrBuffTemplet->IsWorldBuff() )
		{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			// 행동 불능 디버프에 대한 조건 추가
			if ( false == ptrBuffTemplet->IsNonControlDebuff() )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			{
				// 지금 SetSpecificValueByBuffTempletID 함수 이후에 유닛에 버프를 적용시키는 vector에 eBuffTempletId_를 넣는다.
				// 따라서 여기서 버프를 지워주는 건 의미가 없고 나중에 지워주기 위해 eBuffTempletId_를 저장해 놓는다.
				m_eFireBlossomsSaveDebuffID = eBuffTempletId_;
			}
		}
	}
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON
}
/** @function	: UnSetSpecificValueByBuffTempletID
	@brief		: 각 유닛마다 특정 버프가 해제 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 해제 됬다는 플래그 설정 등...)
	@param		: 버프템플릿ID(eBuffTempletId_)
*/
/*virtual*/ void CX2GUEL::UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ )
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON
	// char-elesis CH3b0: June 32b74500 ( vslot 0x250 ): the owner's client spawns the blossom, the enhanced Victorious Sword, Soul Ignition's reset
	CX2GUUser::UnSetSpecificValueByBuffTempletID( eBuffTempletId_ );

	switch ( eBuffTempletId_ )
	{
	case BTI_BUFF_FLAME_SWORD:
		m_bIsFrameSword = false;
		break;

	case BTI_BUFF_ENDURANCE_POWER:
		m_bIsActiveEndurancePower = false;
		break;

	case BTI_BUFF_FIRE_BLOSSOMS:
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_P_SBH_FIRE_BLOSSOMS_Fire", this );

#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON_DEBUG
			CX2OfflineLog::Server( L"[JUNE_CHAR] ELESIS SUMMON req npc %d fire blossoms end mine %d pos %.0f %.0f %.0f", static_cast<int>( CX2UnitManager::NUI_ELESIS_FIRE_BLOSSOM ), static_cast<int>( IsMyUnit() ), GetLandPosition().x, GetLandPosition().y, GetLandPosition().z );	// char-elesis CH3b0
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON_DEBUG
			if ( true == IsMyUnit() )
			{
				g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_ELESIS_FIRE_BLOSSOM, 0, true, GetLandPosition(),
					GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) GetTeam(), CX2NPCAI::NAT_ALLY, GetUnitUID() );
			}
		} break;

	case BTI_BUFF_VICTORIOUS_SWORD:
	case BTI_BUFF_SGM_VICTORIOUS_SWORD:
	case BTI_BUFF_SGM_VICTORIOUS_SWORD_UPGRADE:
		m_bIsApplyVictoriousSword = false;
		m_fDodgeAndSlashCoolTimeDecreaseRate = 1.f;
		break;

	case BTI_BUFF_SOUL_IGNITION:
		m_fCreateEffectSetRate = 0.f;
		break;

	default:
		break;
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON
	CX2GUUser::UnSetSpecificValueByBuffTempletID( eBuffTempletId_ );

	switch ( eBuffTempletId_ )
	{
	// 극기 - 강(强)
//{{ Iruha : 2026-10-03 // June buff port Phase 2 (JUNE_BUFF_PORT_PHASES.md)
#ifdef SERV_IRUHADEV_JUNE_BUFF_ENUM
	case BTI_BUFF_ENDURANCE_POWER:
#else
	case BTI_BUFF_INDURANCE_POWER:
#endif SERV_IRUHADEV_JUNE_BUFF_ENUM
//}}
		{
			m_bIsActiveEndurancePower = false;
		} break;
	// 승리의 검
	case BTI_BUFF_VICTORIOUS_SWORD:
	case BTI_BUFF_SGM_VICTORIOUS_SWORD:
		{
			// 닷지&슬래시 쿨타임, MP 변경 기능 비활성화를 위한 변수 설정
			m_fDodgeAndSlashCoolTimeDecreaseRate = 1.f;
			m_bIsApplyVictoriousSword = false;
		} break;
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	// 불꽃 개화 - 버프가 해제되면 현재 위치에 불꽃 개화 npc를 생성한다.
	case BTI_BUFF_FIRE_BLOSSOMS:
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_P_SBH_FIRE_BLOSSOMS_Fire", this );

			D3DXVECTOR3 vPos = GetLandPosition();
			const int iHyperMode = 0;
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_ELESIS_FIRE_BLOSSOM, iHyperMode, true, vPos, 
				GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) GetTeam(), CX2NPCAI::NAT_ALLY, GetUnitUID() );
		} break;
	// 홍련의 검 - 비활성화
	case BTI_BUFF_FLAME_SWORD:
		{
			m_bIsFrameSword = false;
		} break;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
	default:
		break;
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON
}


#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON
// char-elesis CH3b0: June 32b6fb10: the cleanse timer reached the interval of her HP tier ( 32b6fa50 = GetHpTier's body )
bool CX2GUEL::IsBloodRunawayCleanseReady()
{
	if ( true == GetRidingOn() )
		return false;

	float fCleanseInterval = BLOOD_RUNAWAY_NO_TIER_CLEANSE_INTERVAL;
	switch ( GetHpTier() )
	{
	case 1:
		fCleanseInterval = m_fBloodRunawayCleanseIntervalTier1;
		break;

	case 2:
		fCleanseInterval = m_fBloodRunawayCleanseIntervalTier2;
		break;

	case 3:
		fCleanseInterval = m_fBloodRunawayCleanseIntervalTier3;
		break;
	}

	return ( m_fBloodRunawayCleanseElapsedTime >= fCleanseInterval );
}

// char-elesis CH3b0: June 32b79db0 ( slot 0x58, CH3b2 registers it ): a Fire Blossom per Scalet Rose hit, at most 3, 50 to 500 ahead
void CX2GUEL::ESI_SI_A_SBH_SCALET_ROSE_SUCCESS_End()
{
	for ( unsigned char ucBlossomIndex = 0; ucBlossomIndex < min( m_ucScaletRoseHitCount, SCALET_ROSE_MAX_FIRE_BLOSSOM_COUNT ); ++ucBlossomIndex )
	{
		D3DXVECTOR3 vPos = GetPos();
		const float fDistance = static_cast<float>( GetRandomFloat( ucBlossomIndex ) * FIRE_BLOSSOM_RANDOM_DISTANCE + FIRE_BLOSSOM_MIN_DISTANCE );
		g_pX2Game->GetWorld()->GetLineMap()->GetFixedDistancePositionForWeaponTaker( &GetPos(), fDistance, GetIsRight(), vPos );

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_P_SBH_FIRE_BLOSSOMS_Fire", this, NULL, false, -1.f, -1.f, D3DXVECTOR3( 1.f, 1.f, 1.f ), true, vPos );

#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON_DEBUG
		CX2OfflineLog::Server( L"[JUNE_CHAR] ELESIS SUMMON req npc %d scalet rose %d of %d mine %d distance %.0f", static_cast<int>( CX2UnitManager::NUI_ELESIS_FIRE_BLOSSOM ), static_cast<int>( ucBlossomIndex ) + 1, static_cast<int>( m_ucScaletRoseHitCount ), static_cast<int>( IsMyUnit() ), fDistance );	// char-elesis CH3b0
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON_DEBUG
		if ( true == IsMyUnit() )
		{
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_ELESIS_FIRE_BLOSSOM, 0, true, vPos,
				GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) GetTeam(), CX2NPCAI::NAT_ALLY, GetUnitUID() );
		}
	}

	m_ucScaletRoseHitCount = 0;
	CommonStateEnd();
}

// char-elesis CH3b0: June 32b7a0a0 @32b7a3c0-32b7a4f5 ( CH3b's FrameMove passes the position ): the Fire King wall once Blaze Absolu is learned
void CX2GUEL::CreateWallOfProminenceNPC( const D3DXVECTOR3& vPos_ )
{
	const CX2UnitManager::NPC_UNIT_ID eWallNPCID = ( 0 == m_usBlazeAbsoluLevel ?
		CX2UnitManager::NUI_SI_SA_SBH_WALL_OF_PROMINENCE : CX2UnitManager::NUI_SI_SA_SBH_WALL_OF_PROMINENCE_FIREKING );

#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON_DEBUG
	CX2OfflineLog::Server( L"[JUNE_CHAR] ELESIS SUMMON req npc %d wall blaze absolu %d mine %d pos %.0f %.0f %.0f", static_cast<int>( eWallNPCID ), static_cast<int>( m_usBlazeAbsoluLevel ), static_cast<int>( IsMyUnit() ), vPos_.x, vPos_.y, vPos_.z );	// char-elesis CH3b0
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON_DEBUG
	if ( true == IsMyUnit() )
	{
		g_pX2Game->CreateNPCReq( eWallNPCID, ( true == IsHyperState() ? 1 : 0 ), true, vPos_,
			GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) GetTeam(), CX2NPCAI::NAT_ALLY, GetUnitUID() );
	}
}

// char-elesis CH3b0: June 32b8b0c0 @32b8b2e6-32b8b3b7 ( CH3c's FrameMove calls it )
void CX2GUEL::CreateBloodCutterNPC()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON_DEBUG
	CX2OfflineLog::Server( L"[JUNE_CHAR] ELESIS SUMMON req npc %d blood cutter mine %d", static_cast<int>( CX2UnitManager::NUI_SI_SA_SCA_BLOOD_CUTTER ), static_cast<int>( IsMyUnit() ) );	// char-elesis CH3b0
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON_DEBUG
	if ( true == IsMyUnit() )
	{
		g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_SI_SA_SCA_BLOOD_CUTTER, 0, true, GetPos(),
			GetIsRight(), BLOOD_CUTTER_NPC_DELAY, true, -1, (CX2Room::TEAM_NUM) GetTeam(), CX2NPCAI::NAT_ALLY, GetUnitUID() );
	}
}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON
#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
// 검의 길
/*virtual*/ int	CX2GUEL::GetWayOfSwordState() const
{ 
	if( NULL != GetELGageData() )
		return GetELGageData()->GetWayOfSwordState();

	return 0;
}
void CX2GUEL::SetWayOfSwordState( const int iWayOfSwordState_ )
{ 
	if( NULL != GetELGageData() )
		GetELGageData()->SetWayOfSwordState( iWayOfSwordState_ );
}
/*virtual*/ float CX2GUEL::GetWayOfSwordPoint() const
{
	if( NULL != GetELGageData() )
		return GetELGageData()->GetWayOfSwordPoint();

	return 0.f;
}

/*virtual*/ void CX2GUEL::SetWayOfSwordPoint( float fWayOfSwordPoint_ )
{
	if( fWayOfSwordPoint_ > g_pData->GetDamageManager()->GetVigorStateThreshold() )
		fWayOfSwordPoint_ = g_pData->GetDamageManager()->GetVigorStateThreshold();

	if( fWayOfSwordPoint_ < g_pData->GetDamageManager()->GetDestStateThreshold() )
		fWayOfSwordPoint_ = g_pData->GetDamageManager()->GetDestStateThreshold();

	if( NULL != GetELGageData() )
		GetELGageData()->SetWayOfSwordPoint( fWayOfSwordPoint_ );
}
// 질풍 시작
void CX2GUEL::Start_SwordmanVigor()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
	// char-elesis CH1q: June 32b74760 ( job line 3 = Dark Knight / Crimson Avenger takes its own gale buff )
	if ( NULL != g_pX2Game->GetWorld() && GetShowCutInAndChangeWorldColor() )
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xff222255 );
		g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 4.0f );
	}

	SetBuffFactorToGameUnitByBuffFactorID( ( true == IsInJobLine( 3 ) ) ? BFI_BUFF_GALE_WAY_OF_SWORD_DARK_KNIGHT : BFI_BUFF_GALE_WAY_OF_SWORD, 1, NULL, 0, false );
	PlaySound( L"elsword_vitality_use_VOX.ogg" );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS_DEBUG
	CX2OfflineLog::Server( L"[JUNE_CHAR] ELESIS WOS vigor start: point %.1f, class %d, dark knight line %d", GetWayOfSwordPoint(), static_cast<int>( GetUnit()->GetClass() ), IsInJobLine( 3 ) ? 1 : 0 );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS_DEBUG

	m_CommonState.m_Wait = ESI_SWORD_WAIT;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
	if ( NULL != g_pX2Game->GetWorld() && GetShowCutInAndChangeWorldColor() )
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xff222255 );
		g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 4.0f );
	}

	SetBuffFactorToGameUnitByBuffFactorID(BFI_BUFF_GALE_WAY_OF_SWORD);	
	PlaySound( L"elsword_vitality_use_VOX.ogg" );

	m_CommonState.m_Wait = ESI_SWORD_WAIT;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
}
// 질풍 종료
void CX2GUEL::End_SwordmanVigor()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
	// char-elesis CH1q: June 32b74930 ( every class: the Dark Knight gale buff and its end set go first )
	EraseBuffTempletFromGameUnit( BTI_BUFF_GALE_WAY_OF_SWORD_DARK_KNIGHT );
	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_P_SDK_DARK_FORCE_J_End", this );
	EraseBuffTempletFromGameUnit( BTI_BUFF_GALE_WAY_OF_SWORD );

	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_VigorEnd", this );
	PlaySound( L"elsword_vitality_end.ogg" );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS_DEBUG
	CX2OfflineLog::Server( L"[JUNE_CHAR] ELESIS WOS vigor end: point %.1f", GetWayOfSwordPoint() );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS_DEBUG

	m_CommonState.m_Wait = USI_WAIT;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
	EraseBuffTempletFromGameUnit(BTI_BUFF_GALE_WAY_OF_SWORD);

	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_VigorEnd", this ); 	
	PlaySound( L"elsword_vitality_end.ogg" );

	m_CommonState.m_Wait = USI_WAIT;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
}
// 섬멸 시작
void CX2GUEL::Start_SwordmanDestruction()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
	// char-elesis CH1q: June 32b7f0b0 ( job line 3 takes its own annihilation buffs; the PvP test is game type 1 only, as March )
	if ( NULL != g_pX2Game->GetWorld() && GetShowCutInAndChangeWorldColor() )
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xff552222 );
		g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 4.0f );
	}

	const bool bPvpGame = ( NULL != g_pX2Game && CX2Game::GT_PVP == g_pX2Game->GetGameType() );
	if( true == IsInJobLine( 3 ) )
		SetBuffFactorToGameUnitByBuffFactorID( bPvpGame ? BFI_BUFF_ANNIHILATION_WAY_OF_SWORD_PVP_DARK_KNIGHT : BFI_BUFF_ANNIHILATION_WAY_OF_SWORD_DARK_KNIGHT, 1, NULL, 0, false );
	else
		SetBuffFactorToGameUnitByBuffFactorID( bPvpGame ? BFI_BUFF_ANNIHILATION_WAY_OF_SWORD_PVP : BFI_BUFF_ANNIHILATION_WAY_OF_SWORD, 1, NULL, 0, false );

	PlaySound( L"elsword_demolition_use_VOX.ogg" );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS_DEBUG
	CX2OfflineLog::Server( L"[JUNE_CHAR] ELESIS WOS destruction start: point %.1f, class %d, dark knight line %d", GetWayOfSwordPoint(), static_cast<int>( GetUnit()->GetClass() ), IsInJobLine( 3 ) ? 1 : 0 );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS_DEBUG

	m_CommonState.m_Wait = ESI_SWORD_WAIT;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
	if ( NULL != g_pX2Game->GetWorld() && GetShowCutInAndChangeWorldColor() )
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xff552222 );
		g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 4.0f );
	}

	if( NULL != g_pX2Game && CX2Game::GT_PVP == g_pX2Game->GetGameType() )
		SetBuffFactorToGameUnitByBuffFactorID(BFI_BUFF_ANNIHILATION_WAY_OF_SWORD_PVP);	
	else
		SetBuffFactorToGameUnitByBuffFactorID(BFI_BUFF_ANNIHILATION_WAY_OF_SWORD);	

	PlaySound( L"elsword_demolition_use_VOX.ogg" );

	m_CommonState.m_Wait = ESI_SWORD_WAIT;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
}
// 섬멸 종료
void CX2GUEL::End_SwordmanDestruction()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
	// char-elesis CH1q: June 32b74bf0 ( every class: the Dark Knight annihilation buff and its end set go first )
	SetNowChargeMpForDetonation( 0.f );
	EraseBuffTempletFromGameUnit( BTI_BUFF_ANNIHILATION_WAY_OF_SWORD_DARK_KNIGHT );
	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_P_SDK_DARK_FORCE_S_End", this );
	EraseBuffTempletFromGameUnit( BTI_BUFF_ANNIHILATION_WAY_OF_SWORD );

	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_DestructionEnd", this );
	PlaySound( L"elsword_demolition_end.ogg" );
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS_DEBUG
	CX2OfflineLog::Server( L"[JUNE_CHAR] ELESIS WOS destruction end: point %.1f", GetWayOfSwordPoint() );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS_DEBUG

	m_CommonState.m_Wait = USI_WAIT;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
	SetNowChargeMpForDetonation( 0.f ) ;
	EraseBuffTempletFromGameUnit(BTI_BUFF_ANNIHILATION_WAY_OF_SWORD);

	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_DestructionEnd", this ); 	
	PlaySound( L"elsword_demolition_end.ogg" );

	m_CommonState.m_Wait = USI_WAIT;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
}

void CX2GUEL::WayOfSwordStateChange()
{
	int iNextWayOfSwordState = 0;
	if( IsMyUnit() == true )
	{
		const float fWayOfSwordPoint = GetWayOfSwordPoint();
		if( fWayOfSwordPoint >= g_pData->GetDamageManager()->GetVigorStateThreshold() )
			iNextWayOfSwordState = WSS_VIGOR;
		else if( fWayOfSwordPoint <= g_pData->GetDamageManager()->GetDestStateThreshold() )
			iNextWayOfSwordState = WSS_DESTRUCTION;
		else if( fWayOfSwordPoint > 0 )
			iNextWayOfSwordState = WSS_TOWARD_VIGOR;
		else if( fWayOfSwordPoint < 0 )
			iNextWayOfSwordState = WSS_TOWARD_DESTRUCTION;
		else if( fWayOfSwordPoint == 0 )
			iNextWayOfSwordState = WSS_CENTER;
	}		
	else
	{
		iNextWayOfSwordState = m_FrameDataNow.syncData.m_CannonBallCount;
	}
	
	const WAY_OF_SWORD_STATE eWayofSwordState = static_cast<const WAY_OF_SWORD_STATE>( GetWayOfSwordState() );

	if( iNextWayOfSwordState == WSS_VIGOR && 
		eWayofSwordState != WSS_VIGOR )
	{
		if( eWayofSwordState == WSS_DESTRUCTION )
			End_SwordmanDestruction();

		Start_SwordmanVigor();
		SetWayOfSwordState( static_cast<int>( WSS_VIGOR ) );
	}
	else if( iNextWayOfSwordState == WSS_DESTRUCTION &&
		eWayofSwordState != WSS_DESTRUCTION )
	{
		if( eWayofSwordState == WSS_VIGOR )
			End_SwordmanVigor();

		Start_SwordmanDestruction();
		SetWayOfSwordState( static_cast<int>( WSS_DESTRUCTION ) );
	}
	else if( iNextWayOfSwordState == WSS_TOWARD_VIGOR &&
		eWayofSwordState != WSS_TOWARD_VIGOR && eWayofSwordState != WSS_VIGOR )
	{
		if( eWayofSwordState == WSS_DESTRUCTION )
			End_SwordmanDestruction();

		SetWayOfSwordState( static_cast<int>( WSS_TOWARD_VIGOR ) );
	}
	else if( iNextWayOfSwordState == WSS_TOWARD_DESTRUCTION && 
		eWayofSwordState != WSS_TOWARD_DESTRUCTION && eWayofSwordState != WSS_DESTRUCTION )
	{
		if( eWayofSwordState == WSS_VIGOR )
			End_SwordmanVigor();

		SetWayOfSwordState( static_cast<int>( WSS_TOWARD_DESTRUCTION ) );
	}
	else if( iNextWayOfSwordState == WSS_CENTER && 
		eWayofSwordState != WSS_CENTER )
	{
		if( eWayofSwordState == WSS_VIGOR )
			End_SwordmanVigor();
		else if( eWayofSwordState == WSS_DESTRUCTION )
			End_SwordmanDestruction();

		SetWayOfSwordState( static_cast<int>( WSS_CENTER ) );
	}
	else // Do Nothing
	{
		;
	}
}
/** @function : GaleBuffFrameMove()
	@brief : 매 5회 히트시 MP회수율 증가 버프 적용 시키기 위한 프레임 무브
*/
void CX2GUEL::GaleBuffFrameMove()
{
	if( WSS_VIGOR != GetWayOfSwordState() )
		return;

	const int iComboCount = ( NULL != GetComboManager() ? GetComboManager()->GetCombo() : 0 );

	// 히트 카운트 변화가 없다면 수행하지 않음
	if( iComboCount == m_iPreComboCount )
		return;
	
	m_iPreComboCount = iComboCount;

	// 매 5회 히트 시 MP 회수율 증가 버프 적용
	if( 0 < iComboCount && 0 == ( iComboCount % 5) )
	{
		SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_GALE_MP_GAIN_INCREASE );
	}
}
void CX2GUEL::SwordWaitStartFuture()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b700d0
	CommonStateStartFuture();
	PlayAnimationSwordWaitStart( m_pXSkinAnimFuture, m_FrameDataFuture );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateStartFuture();
	PlayAnimationSwordWaitStart( m_pXSkinAnimFuture, m_FrameDataFuture );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

void CX2GUEL::SwordWaitStart()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b70100
	CommonStateStart();
	PlayAnimationSwordWaitStart( m_pXSkinAnim.get(), m_FrameDataNow );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateStart();
	PlayAnimationSwordWaitStart( m_pXSkinAnim.get(), m_FrameDataNow );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

void CX2GUEL::SwordWaitEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b7f580
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ESI_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESI_COMBO_X );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if( GetWayOfSwordState() != WSS_VIGOR && GetWayOfSwordState() != WSS_DESTRUCTION )
	{
		StateChange( USI_WAIT );
	}
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ESI_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESI_COMBO_X );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if( GetWayOfSwordState() != WSS_VIGOR && GetWayOfSwordState() != WSS_DESTRUCTION )
	{
		StateChange( USI_WAIT );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

void CX2GUEL::PlayAnimationSwordWaitStart( CKTDGXSkinAnim* pXSkinAnim_, const FrameData& refFrameData_ )
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b6e0c0: Chivalry learned lowers the fatal threshold to 0.15
	CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType;
	LUA_GET_VALUE_ENUM ( m_LuaManager, "PLAY_TYPE", playType, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE, CKTDGXSkinAnim::XAP_ONE_WAIT );

	const float FATAL_HP_PERCENT = ( 0 < m_uiChivalryLevel ) ? 0.15f : 0.25f;

	if( GetMaxHp() * FATAL_HP_PERCENT >= refFrameData_.syncData.fNowHP )
	{
		bool bTransition = true;
		LUA_GET_VALUE( m_LuaManager, "TRANSITION", bTransition, true );

		if( false == refFrameData_.unitCondition.bTransitionChange || false == bTransition )
			pXSkinAnim_->ChangeAnim( L"Fatal", false );
		else
			pXSkinAnim_->ChangeAnim( L"Fatal", true );
	}
	else
	{
		bool bTransition = true;
		LUA_GET_VALUE( m_LuaManager, "TRANSITION", bTransition, true );
		if( false == refFrameData_.unitCondition.bTransitionChange || false == bTransition )
		{
			if( GetWayOfSwordState() == WSS_VIGOR )
				pXSkinAnim_->ChangeAnim( L"Wait_W0", false );
			else if( GetWayOfSwordState() == WSS_DESTRUCTION )
				pXSkinAnim_->ChangeAnim( L"Wait_W1", false );
			else
				pXSkinAnim_->ChangeAnim( L"Wait", false );
		}
		else
		{
			if( GetWayOfSwordState() == WSS_VIGOR )
				pXSkinAnim_->ChangeAnim( L"Wait_W0", true );
			else if( GetWayOfSwordState() == WSS_DESTRUCTION )
				pXSkinAnim_->ChangeAnim( L"Wait_W1", true );
			else
				pXSkinAnim_->ChangeAnim( L"Wait", true );
		}
	}

	pXSkinAnim_->Play( playType );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType;
	LUA_GET_VALUE_ENUM ( m_LuaManager, "PLAY_TYPE", playType, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE, CKTDGXSkinAnim::XAP_ONE_WAIT );

	// FATAL_HP_PERCENT 보다 HP가 적으면 fatal 애니메이션이 실행
	const float FATAL_HP_PERCENT = 0.25f;

	if( GetMaxHp() * FATAL_HP_PERCENT >= refFrameData_.syncData.fNowHP )
	{
		bool bTransition = true;
		LUA_GET_VALUE( m_LuaManager, "TRANSITION", bTransition, true );

		if( false == refFrameData_.unitCondition.bTransitionChange || false == bTransition )
			pXSkinAnim_->ChangeAnim( L"Fatal", false );
		else
			pXSkinAnim_->ChangeAnim( L"Fatal", true );
	}
	else
	{
		bool bTransition = true;
		LUA_GET_VALUE( m_LuaManager, "TRANSITION", bTransition, true );
		if( false == refFrameData_.unitCondition.bTransitionChange || false == bTransition )
		{
			if( GetWayOfSwordState() == WSS_VIGOR )
				pXSkinAnim_->ChangeAnim( L"Wait_W0", false );
			else if( GetWayOfSwordState() == WSS_DESTRUCTION )
				pXSkinAnim_->ChangeAnim( L"Wait_W1", false );
			else
				pXSkinAnim_->ChangeAnim( L"Wait", false );
		}
		else
		{
			if( GetWayOfSwordState() == WSS_VIGOR )
				pXSkinAnim_->ChangeAnim( L"Wait_W0", true );
			else if( GetWayOfSwordState() == WSS_DESTRUCTION )
				pXSkinAnim_->ChangeAnim( L"Wait_W1", true );
			else
				pXSkinAnim_->ChangeAnim( L"Wait", true );
		}
	}

	pXSkinAnim_->Play( playType );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
bool CX2GUEL::CanSpecialReactProcess( CX2DamageManager::REACT_TYPE eReActType_ )
{
	if( 0.f >= GetNowHp() )
		return false;

	if( true == GetInvincible() )
		return false;

	switch( eReActType_ ) 
	{
	case CX2DamageManager::RT_NO_DAMAGE:
	case CX2DamageManager::RT_DUMMY_DAMAGE:
	case CX2DamageManager::RT_SENSOR:
		return false;
	default:
		break;
	}

	return true;
}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
// char-elesis CH1i: June 32b64290 ( user vslot 0x670 ): the unit is in the job line, 1 Saber Knight / Grand Master, 2 Pyro Knight / Blazing Heart, 3 Dark Knight / Crimson Avenger
bool CX2GUEL::IsInJobLine( int iJobLine_ )
{
	if( NULL == GetUnit() )
		return false;

	switch( iJobLine_ )
	{
	case 1:
		return ( CX2Unit::UC_ELESIS_SABER_KNIGHT == GetUnitClass() || CX2Unit::UC_ELESIS_GRAND_MASTER == GetUnitClass() );

	case 2:
		return ( CX2Unit::UC_ELESIS_PYRO_KNIGHT == GetUnitClass() || CX2Unit::UC_ELESIS_BLAZING_HEART == GetUnitClass() );

	case 3:
		return ( CX2Unit::UC_ELESIS_DARK_KNIGHT == GetUnitClass() || CX2Unit::UC_ELESIS_CRIMSON_AVENGER == GetUnitClass() );
	}

	return false;
}
// char-elesis CH1h: June 32b6fb80 ( vslot 0x558 )
int CX2GUEL::GetHpTier()
{
	if( NULL == GetGageData() )	// June reads 1.0 / 1.0 without a gage data
		return 0;

	const float fHpRate = GetNowHp() / GetMaxHp();
	if( fHpRate < HP_TIER_3_BELOW_RATE )
		return 3;
	if( HP_TIER_2_BELOW_RATE > fHpRate )
		return 2;
	if( fHpRate < HP_TIER_1_BELOW_RATE )
		return 1;
	return 0;
}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
// char-elesis CH1h: June 32b6fc40 ( vslot 0x29c ); Chivalry: at or under 15 % of the max HP the listed debuffs get no duration
/*virtual*/ float CX2GUEL::GetReduceDebuffDurationRate( BUFF_TEMPLET_ID eBuffTempletID_ )
{
	if( 0 < m_uiChivalryLevel && GetNowHp() <= static_cast<double>( GetMaxHp() ) * static_cast<double>( CHIVALRY_DEBUFF_IMMUNE_HP_RATE ) )
	{
		switch( eBuffTempletID_ )
		{
		case BTI_DEBUFF_STUN:
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
		case BTI_DEBUFF_CONFUSION:
		case BTI_DEBUFF_PANIC:
		case BTI_DEBUFF_PANDEMONIUM_FEAR:
		case BTI_DEBUFF_SUPPRESSION:
		case BTI_DEBUFF_AGING:
		case BTI_DEBUFF_BLEEDING:
		case BTI_DEBUFF_WOUND:
		case BTI_DEBUFF_BLIND:
		case BTI_DEBUFF_BLIND_SMOKE:
		case BTI_DEBUFF_MOVE_JUMP_SLOWDOWN:
		case BTI_DEBUFF_MOTION_SLOWDOWN:
			return 0.f;
		default:
			break;
		}
	}
	return 1.f;
}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
/** @function : GetFinalDamage_WayOfSword
	@brief : -GetFinalDamage 계산 할 때 검의길 타입에 따라 데미지 변경 시켜 주기 위한 함수
			 -엘소드와 엘리시스가 다른 동작을 할 수 있도록 virtual 함수로 변경
*/
/*virtual*/float CX2GUEL::GetFinalDamage_WayOfSword( CX2DamageManager::WAY_OF_SWORD_TYPE eWayOfSwordType_ )
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
	// char-elesis CH1q: June 32b74ed0 ( the Destruction rates are scaled by the Dark Force damage rate; active / special now enter it too )
	float fDamageMagnifications = 1.f;
	if( WSS_DESTRUCTION == GetWayOfSwordState() )
	{
		switch( eWayOfSwordType_ )
		{
		case CX2DamageManager::WST_DESTRUCTION_NORMAL:
			{
				fDamageMagnifications = ( CX2Game::GT_PVP == g_pX2Game->GetGameType() ) ? 1.25f : 1.5f;
				fDamageMagnifications = m_fDestructionDamageRate * fDamageMagnifications;
			} break;
		case CX2DamageManager::WST_DESTRUCTION_ACTIVE:
		case CX2DamageManager::WST_DESTRUCTION_SPECIAL:
			{
				fDamageMagnifications = m_fDestructionDamageRate * fDamageMagnifications;
			} break;
		default:
			break;
		}
	}

	return fDamageMagnifications;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
	float fDamageMagnifications = 1.f;
	if( WSS_DESTRUCTION == GetWayOfSwordState() )
	{
		switch( eWayOfSwordType_ ) 
		{
		case CX2DamageManager::WST_DESTRUCTION_NORMAL:
			{
				if( CX2Game::GT_PVP == g_pX2Game->GetGameType() )
					fDamageMagnifications = 1.25f;
				else 
					fDamageMagnifications = 1.5f;
			} break;
		default:
			break;
		}
	}

	return fDamageMagnifications;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
}
/** @function : GetCriticalPercent_WayOfSword
	@brief : 검의길 타입에 따른 크리티컬 배율 변경
*/
/*virtual*/ void CX2GUEL::SetCriticalPercent_WayOfSword( IN CX2DamageManager::WAY_OF_SWORD_TYPE eWayOfSwordType_ ,
												OUT float& fCriticalPercent_)
{
	if( WSS_DESTRUCTION == GetWayOfSwordState() )
	{
		switch( eWayOfSwordType_ ) 
		{
		case CX2DamageManager::WST_DESTRUCTION_NORMAL:
		case CX2DamageManager::WST_DESTRUCTION_ACTIVE:
		case CX2DamageManager::WST_DESTRUCTION_SPECIAL:
			{
				fCriticalPercent_ = 1.f;
			} break;
		default:
			break;
		}
	}
}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
// char-elesis CH1q: June 32b903e0 / 32b903f0 / 32b90400 ( user vslots 0x720 / 0x724 / 0x72c )
float CX2GUEL::GetWayOfSwordPointMultiplier() const { return m_fWayOfSwordPointMultiplier; }
float CX2GUEL::GetVigorHpIntakeRate() const { return m_fVigorHpIntakeRate; }
float CX2GUEL::GetDestructionBleedingDamageRate() const { return m_fDestructionBleedingDamageRate; }
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
// char-elesis CH1q: June 32b6ac80 ( user vslot 0x740 ): the own unit keeps the point, not the state; base 32009780 not ported
void CX2GUEL::StoreGageSnapshot()
{
	if( true == IsMyUnit() )
		m_fSnapshotWayOfSwordPoint = GetWayOfSwordPoint();
}
// char-elesis CH1q: June 32b6acb0 ( user vslot 0x744 ): base 3204da60 not ported
void CX2GUEL::RestoreGageSnapshot()
{
	if( true == IsMyUnit() )
		SetWayOfSwordPoint( m_fSnapshotWayOfSwordPoint );
}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
#endif // ELSWORD_WAY_OF_SWORD

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 스테이트 함수
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ESI_DIE_FRONT
void CX2GUEL::DieFrontStartFuture()
{
	CommonStateStartFuture();

#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
	const WAY_OF_SWORD_STATE eWayofSwordState = static_cast<const WAY_OF_SWORD_STATE>( GetWayOfSwordState() );

	if( eWayofSwordState == WSS_VIGOR )
		End_SwordmanVigor();
	else if( eWayofSwordState == WSS_DESTRUCTION )
		End_SwordmanDestruction();
#endif // ELSWORD_WAY_OF_SWORD

}
void CX2GUEL::DieFrontStart()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b52850
	CommonStateStart();
	DyingStart();
	m_pXSkinAnim->SetPlaySpeed( 0.5f );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateStart();
	DyingStart();
	m_pXSkinAnim->SetPlaySpeed( 0.5f );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DieFrontFrameMove()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b4efc0
	const float TIME_CREATE_STEP_DUST = 0.19f;
	const float TIME_SACRIFICE_OF_HERO = 1.12f;

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	const float TIME_CREATE_STEP_DUST = 0.19f;		// 연기가 발생되야하는 애니메이션 시간
	const float TIME_SACRIFICE_OF_HERO = 1.12f;		// 영웅의 희생이 실행되어야 하는 애니메이션 시간

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DieFrontEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b528c0
	if( false == GetShowObject() && true == m_pXSkinAnim->IsAnimationEnd() )
	{
		StateChange( GUSI_DIE );
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( GetShowObject() == false && m_pXSkinAnim->IsAnimationEnd() == true )
	{
		StateChange( GUSI_DIE );
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_DIE_BACK
void CX2GUEL::DieBackStartFuture()
{
	CommonStateStartFuture();

#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
	const WAY_OF_SWORD_STATE eWayofSwordState = static_cast<const WAY_OF_SWORD_STATE>( GetWayOfSwordState() );

	if( eWayofSwordState == WSS_VIGOR )
		End_SwordmanVigor();
	else if( eWayofSwordState == WSS_DESTRUCTION )
		End_SwordmanDestruction();
#endif // ELSWORD_WAY_OF_SWORD

}
void CX2GUEL::DieBackStart()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b52900
	CommonStateStart();
	DyingStart();
	m_pXSkinAnim->SetPlaySpeed( 0.5f );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateStart();
	DyingStart();
	m_pXSkinAnim->SetPlaySpeed( 0.5f );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DieBackFrameMove()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b4efe0
	const float TIME_CREATE_STEP_DUST = 0.19f;
	const float TIME_SACRIFICE_OF_HERO = 1.12f;

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	const float TIME_CREATE_STEP_DUST = 0.19f;		// 연기가 발생되야하는 애니메이션 시간
	const float TIME_SACRIFICE_OF_HERO = 1.12f;		// 영웅의 희생이 실행되어야 하는 애니메이션 시간

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DieBackEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b52970
	if( false == GetShowObject() && true == m_pXSkinAnim->IsAnimationEnd() )
	{
		StateChange( GUSI_DIE );
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( GetShowObject() == false  && m_pXSkinAnim->IsAnimationEnd() == true )
	{
		StateChange( GUSI_DIE );
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_WAIT
void CX2GUEL::WaitStartFuture()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b4f000
	CommonStateStartFuture();
	PlayAnimationWaitStart( m_pXSkinAnimFuture, m_FrameDataFuture );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateStartFuture();
	PlayAnimationWaitStart( m_pXSkinAnimFuture, m_FrameDataFuture );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::WaitStart()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b529b0
	CommonStateStart();
	PlayAnimationWaitStart( m_pXSkinAnim.get(), m_FrameDataNow );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateStart();
	PlayAnimationWaitStart( m_pXSkinAnim.get(), m_FrameDataNow );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::WaitEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b7f820
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ESI_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESI_COMBO_X );
	}
	else if( true == SpecialAttackEventProcess() )
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
				LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
				&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if( ( GetWayOfSwordState() == WSS_VIGOR || GetWayOfSwordState() == WSS_DESTRUCTION ) && 
			m_FrameDataNow.unitCondition.fStateTime >= 0.1f )
	{
		StateChange( ESI_SWORD_WAIT );
	}
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ESI_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESI_COMBO_X );
	}
	else if( true == SpecialAttackEventProcess() )
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
				LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
				&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
	else if( ( GetWayOfSwordState() == WSS_VIGOR || GetWayOfSwordState() == WSS_DESTRUCTION ) && 
			m_FrameDataNow.unitCondition.fStateTime >= 0.1f )
	{
		StateChange( ESI_SWORD_WAIT );
	}
#endif // ELSWORD_WAY_OF_SWORD
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM
	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_WALK
void CX2GUEL::WalkEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b7fad0
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ESI_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESI_COMBO_X );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_InputData.pureRight == false && m_InputData.pureLeft == false )
	{
		StateChange( USI_WAIT );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ESI_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESI_COMBO_X );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_InputData.pureRight == false && m_InputData.pureLeft == false )
	{
		StateChange( USI_WAIT );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_JUMP_READY
void CX2GUEL::JumpReadyEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b6e570
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() || true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() || true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_JUMP_UP
void CX2GUEL::JumpUpEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b4f030
	if( (m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true ) &&
		GetEnableDash() == true )
	{
		StateChange( USI_DASH_JUMP );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ESI_JUMP_ATTACK_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ESI_JUMP_ATTACK_X );
	}
	else if( 0.0f >= m_PhysicParam.nowSpeed.y )
	{
		StateChange( USI_JUMP_DOWN );
	}
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( (m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true ) &&
		GetEnableDash() == true )
	{
		StateChange( USI_DASH_JUMP );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ESI_JUMP_ATTACK_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ESI_JUMP_ATTACK_X );
	}
	else if( 0.0f >= m_PhysicParam.nowSpeed.y )
	{
		StateChange( USI_JUMP_DOWN );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_JUMP_DOWN
void CX2GUEL::JumpDownEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b529e0
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
	else if( (m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true ) &&
		GetEnableDash() == true )
	{
		StateChange( USI_DASH_JUMP );
	}
	else if( true == m_FrameDataFuture.unitCondition.bFootOnWall && 
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( ESI_WALL_LANDING );
	}
	else if ( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ESI_JUMP_ATTACK_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ESI_JUMP_ATTACK_X );
	}
	else if ( 0.0f <= m_PhysicParam.nowSpeed.y )
	{
		StateChange( USI_JUMP_UP );
	}
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
	else if( (m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true ) &&
		GetEnableDash() == true )
	{
		StateChange( USI_DASH_JUMP );
	}
	else if( true == m_FrameDataFuture.unitCondition.bFootOnWall && 
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( ESI_WALL_LANDING );
	}
	else if ( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ESI_JUMP_ATTACK_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ESI_JUMP_ATTACK_X );
	}
	else if ( 0.0f <= m_PhysicParam.nowSpeed.y )
	{
		StateChange( USI_JUMP_UP );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_JUMP_LANDING
void CX2GUEL::JumpLandingStartFuture()
{
	m_bIsDashAttackPossible = true;
	CommonStateStartFuture();
}
void CX2GUEL::JumpLandingEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b7fd00
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ESI_COMBO_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ESI_COMBO_X );
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( USI_WALK );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXCAS
	{
		StateChange( USI_WAIT, false );
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ESI_COMBO_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ESI_COMBO_X );
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( USI_WALK );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXCAS
	{
		StateChange( USI_WAIT, false );
	}

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_DASH
void CX2GUEL::DashEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b80020
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ESI_DASH_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		if( GetUnitClass() == CX2Unit::UC_ELESIS_BLAZING_HEART )
		{
			if( m_fDashComboStartDelay < 0.f )
				StateChange( ESI_DASH_COMBO_SBH_X );
		}
		else
		StateChange( ESI_DASH_COMBO_X );
	}
	else if ( true == SpecialAttackEventProcess() )
	{
	}
	else if( true == m_InputData.oneUp )
	{
		StateChange( USI_DASH_JUMP );
		m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( false == m_InputData.pureRight && false == m_InputData.pureLeft )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureRight == true && m_InputData.pureLeft == true )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_DASH_JUMP );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ESI_DASH_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
		if( GetUnitClass() == CX2Unit::UC_ELESIS_BLAZING_HEART )
		{
			if( m_fDashComboStartDelay < 0.f )
				StateChange( ESI_DASH_COMBO_SBH_X );
		}
		else
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
		StateChange( ESI_DASH_COMBO_X );
	}
	else if ( true == SpecialAttackEventProcess() )
	{
	}
	else if( true == m_InputData.oneUp )
	{
		StateChange( USI_DASH_JUMP );
		m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( false == m_InputData.pureRight && false == m_InputData.pureLeft )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureRight == true && m_InputData.pureLeft == true )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_DASH_JUMP );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_DASH_END
void CX2GUEL::DashEndStartFuture()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b6e620
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x = GetDashSpeed();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x = GetDashSpeed();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashEndFrameMove()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b52ac0
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true )
		CreateStepDust();

	CommonFrameMove();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true )
		CreateStepDust();

	CommonFrameMove();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashEndEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b802a0
	const float TRIGGER_TIME_FACTOR_DASH_ATTACK = 0.3f;

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() * TRIGGER_TIME_FACTOR_DASH_ATTACK )
		{
			StateChange( ESI_COMBO_Z );
		}
		else
		{
			StateChange( ESI_DASH_COMBO_Z );
		}
	}
	else if( m_InputData.oneX == true )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() * TRIGGER_TIME_FACTOR_DASH_ATTACK )
		{
			StateChange( ESI_COMBO_X );
		}
		else
		{
			if( GetUnitClass() == CX2Unit::UC_ELESIS_BLAZING_HEART )
				StateChange( ESI_DASH_COMBO_SBH_X );
			else
			StateChange( ESI_DASH_COMBO_X );
		}
	}
	else if( SpecialAttackEventProcess() == true)
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.oneRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.oneLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if( m_InputData.oneUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// DashEnd에서 공격 키를 눌렀을 때 대시 공격인지, 일반 공격인지를 정하기 위한 const 값. 
	const float TRIGGER_TIME_FACTOR_DASH_ATTACK = 0.3f;

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() * TRIGGER_TIME_FACTOR_DASH_ATTACK )
		{
			StateChange( ESI_COMBO_Z );
		}
		else
		{
			StateChange( ESI_DASH_COMBO_Z );
		}
	}
	else if( m_InputData.oneX == true )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() * TRIGGER_TIME_FACTOR_DASH_ATTACK )
		{
			StateChange( ESI_COMBO_X );
		}
		else
		{
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
			if( GetUnitClass() == CX2Unit::UC_ELESIS_BLAZING_HEART )
				StateChange( ESI_DASH_COMBO_SBH_X );
			else
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
			StateChange( ESI_DASH_COMBO_X );
		}
	}
	else if( SpecialAttackEventProcess() == true)
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.oneRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.oneLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if( m_InputData.oneUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_DASH_JUMP
void CX2GUEL::DashJumpStartFuture()
{
	CommonStateStartFuture();

	const float MAGIC_NUMBER_MULTIPLY_GRAVITY_ACCEL = 1.2f;

	m_PhysicParam.fDownAccel		= m_PhysicParam.fGAccel * MAGIC_NUMBER_MULTIPLY_GRAVITY_ACCEL;
	m_bDownForce					= false;

}
void CX2GUEL::DashJumpStart()
{
	CommonStateStart();

	if( GetMatrix().GetYPos() == m_FrameDataNow.unitCondition.landPosition.y )
		CreateStepDust();
}
void CX2GUEL::DashJumpFrameMoveFuture()
{
	const float MAGIC_NUMBER_MULTIPLY_DASH_SPEED = 1.6f;

	if( m_InputData.pureRight == true && m_FrameDataNow.syncData.bIsRight	== true )
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * MAGIC_NUMBER_MULTIPLY_DASH_SPEED;
	}
	else if( m_InputData.pureLeft == true && m_FrameDataNow.syncData.bIsRight	== false )
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * MAGIC_NUMBER_MULTIPLY_DASH_SPEED;
	}

	CommonFrameMoveFuture();
}
void CX2GUEL::DashJumpEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b6e660: both direction keys held turn the dash jump round
	bool &bCanDashJump = m_FrameDataFuture.stateParam.bEventFlagList[0];

		if( true == IsOnSomethingFuture() )
	{
		if( true == bCanDashJump )
		{
			StateChange( USI_DASH_JUMP );
			m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
			m_bDownForce = false;
		}
		else if ( true == m_bDownForce )
		{
			StateChange( ESI_DASH_JUMP_POWER_LANDING );
			m_bDownForce = false;
		}
		else
		{
			if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
			{
				StateChangeDashIfPossible();
			}
			else
				StateChange( USI_DASH_JUMP_LANDING );

			m_bDownForce = false;
		}
	}
	else if( true == m_FrameDataFuture.unitCondition.bFootOnWall &&
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( ESI_WALL_LANDING );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if ( true == m_InputData.oneZ && true == m_bIsDashAttackPossible)
	{
		StateChange( ESI_DASH_JUMP_COMBO_Z );
	}
	else if ( true == m_InputData.oneX && true == m_bIsDashAttackPossible)
	{
		StateChange( ESI_DASH_JUMP_COMBO_X );
	}
	else if( true == m_InputData.pureRight && true == m_InputData.pureLeft )
	{
		if( ( true == m_InputData.oneRight && false == m_FrameDataFuture.syncData.bIsRight ) ||
			( true == m_InputData.oneLeft && true == m_FrameDataFuture.syncData.bIsRight ) )
		{
			if( GetJumpSpeed() < m_PhysicParam.nowSpeed.y )
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();

			if( 0.0f < m_PhysicParam.nowSpeed.y )
				StateChange( USI_JUMP_UP );
			else
				StateChange( USI_JUMP_DOWN );
		}
	}
	else if( true == m_InputData.pureRight )
	{
		if( false == m_FrameDataFuture.syncData.bIsRight )
		{
			if( GetJumpSpeed() < m_PhysicParam.nowSpeed.y )
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();

			if( 0.0f < m_PhysicParam.nowSpeed.y )
				StateChange( USI_JUMP_UP );
			else
				StateChange( USI_JUMP_DOWN );
		}
	}
	else if( true == m_InputData.pureLeft )
	{
		if( true == m_FrameDataFuture.syncData.bIsRight )
		{
			if( GetJumpSpeed() < m_PhysicParam.nowSpeed.y )
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();

			if( 0.0f < m_PhysicParam.nowSpeed.y )
				StateChange( USI_JUMP_UP );
			else
				StateChange( USI_JUMP_DOWN );
		}
	}
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}

	if( true == m_InputData.oneUp  && 0.0f >= m_PhysicParam.nowSpeed.y  )
	{
		bCanDashJump = true;
	}
	if( true == m_InputData.oneDown )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed * 2;
		m_bDownForce = true;
		bCanDashJump = false;
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	bool &bCanDashJump = m_FrameDataFuture.stateParam.bEventFlagList[0];

		if( true == IsOnSomethingFuture() )
	{
		// 위쪽 화살표를 한번 더 눌렀으면
		if( true == bCanDashJump )
		{
			StateChange( USI_DASH_JUMP );	// 계속해서 대시 점프
			m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
			m_bDownForce = false;
		}
		else if ( true == m_bDownForce )
		{
			StateChange( ESI_DASH_JUMP_POWER_LANDING );
			m_bDownForce = false;
		} // if
		else
		{
			if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
			{
				StateChangeDashIfPossible();
			}
			else
				StateChange( USI_DASH_JUMP_LANDING );

			m_bDownForce = false;
		}
	}
	else if( true == m_FrameDataFuture.unitCondition.bFootOnWall &&
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( ESI_WALL_LANDING );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if ( true == m_InputData.oneZ && true == m_bIsDashAttackPossible)
	{
		StateChange( ESI_DASH_JUMP_COMBO_Z );
	}
	else if ( true == m_InputData.oneX && true == m_bIsDashAttackPossible)
	{
		StateChange( ESI_DASH_JUMP_COMBO_X );
	}
	else if( true == m_InputData.pureRight )
	{
		if( false == m_FrameDataFuture.syncData.bIsRight )
		{
			if( GetJumpSpeed() < m_PhysicParam.nowSpeed.y )
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();

			if( 0.0f < m_PhysicParam.nowSpeed.y )
				StateChange( USI_JUMP_UP );
			else
				StateChange( USI_JUMP_DOWN );
		}
	}
	else if( true == m_InputData.pureLeft )
	{
		if( true == m_FrameDataFuture.syncData.bIsRight )
		{
			if( GetJumpSpeed() < m_PhysicParam.nowSpeed.y )
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();

			if( 0.0f < m_PhysicParam.nowSpeed.y )
				StateChange( USI_JUMP_UP );
			else
				StateChange( USI_JUMP_DOWN );
		}
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	if( true == m_InputData.oneUp  && 0.0f >= m_PhysicParam.nowSpeed.y  )
	{
		bCanDashJump = true;
	}
	if( true == m_InputData.oneDown )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed * 2;
		m_bDownForce = true;
		bCanDashJump = false;
	}

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashJumpEndFuture()
{
	CommonStateEndFuture();
	m_PhysicParam.fDownAccel	= 0.0f;
}

//ESI_DASH_JUMP_LANDING
void CX2GUEL::DashJumpLandingEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b805d0
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ESI_COMBO_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ESI_COMBO_X );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.oneRight && true == m_FrameDataFuture.syncData.bIsRight )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.oneLeft && false == m_FrameDataFuture.syncData.bIsRight )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( USI_WALK );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( USI_JUMP_READY );
	}
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ESI_COMBO_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ESI_COMBO_X );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.oneRight && true == m_FrameDataFuture.syncData.bIsRight )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.oneLeft && false == m_FrameDataFuture.syncData.bIsRight )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( USI_WALK );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( USI_JUMP_READY );
	}
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashJumpLandingStartFuture()
{
	CX2GUUser::DashJumpLandingStartFuture();
	m_bIsDashAttackPossible = true;
}
//ESI_DAMAGE_SMALL_FRONT
//ESI_DAMAGE_SMALL_BACK
void CX2GUEL::DamageSmallEventProcess()
{
	if( m_FrameDataFuture.syncData.position.y - m_FrameDataFuture.unitCondition.landPosition.y > 200.0f )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_DAMAGE_AIR_SMALL );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}	
	CommonEventProcess();
}

//ESI_DAMAGE_BIG_FRONT
void CX2GUEL::DamageBigStart()
{
	CommonStateStart();
	CreateStepDust();
}
void CX2GUEL::DamageBigEventProcess()
{
	float fGapFromUnitToLineMap = m_FrameDataFuture.syncData.position.y - m_FrameDataFuture.unitCondition.landPosition.y;
	const float ACCEPTABLE_GAP = 200.0f;	// 이것보다 거리가 크면 공중에 있는 상태임

	if( ACCEPTABLE_GAP < fGapFromUnitToLineMap )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_DAMAGE_AIR_SMALL );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( 0.93f < m_pXSkinAnimFuture->GetNowAnimationTime() )
		{
			StateChange( USI_WAIT );

		}
	}

	CommonEventProcess();
}

//ESI_DAMAGE_DOWN_FRONT
void CX2GUEL::DamageDownFrontFrameMove()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b52b00
	if( true == m_pXSkinAnim->EventTimerOneshot( 0.19f ) )
		CreateStepDust();

	CommonFrameMove();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.19f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.19f ) && true == EventCheck( 0.19f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		CreateStepDust();

	CommonFrameMove();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DamageDownFrontEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b52b50
	bool& bChangeStandUpAttackFront			= m_FrameDataFuture.stateParam.bEventFlagList[1];
	bool& bChangeStandUpRollingFrontFront	= m_FrameDataFuture.stateParam.bEventFlagList[2];
	bool& bChangeStandUpRollingFrontBack	= m_FrameDataFuture.stateParam.bEventFlagList[3];
	bool& bChangeStandUpBigAttackFront		= m_FrameDataFuture.stateParam.bEventFlagList[4];
	const float fInputStartTime = ( true == m_bAbleQuickStand ) ? m_fEventTime[0].keyInputStart : 0.33f;

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == bChangeStandUpAttackFront )
			StateChange( ESI_STANDUP_ATTACK_FRONT );
		else if( true == bChangeStandUpBigAttackFront )
			StateChange( ESI_QUICK_STANDUP_ATTACK_FRONT );
		else if( true == bChangeStandUpRollingFrontFront )
			StateChange( ESI_STANDUP_ROLLING_FRONT_FRONT );
		else if( true == bChangeStandUpRollingFrontBack )
			StateChange( ESI_STANDUP_ROLLING_FRONT_BACK );
		else
			StateChange( USI_DAMAGE_STANDUP_FRONT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 5, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpRollingFrontFront, ESI_STANDUP_ROLLING_FRONT_FRONT )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 6, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpRollingFrontFront, ESI_STANDUP_ROLLING_FRONT_BACK )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 7, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpRollingFrontFront, ESI_STANDUP_ATTACK_FRONT )
	else if( fInputStartTime < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		if( true == m_InputData.oneZ )
		{
			bChangeStandUpAttackFront		= true;
			bChangeStandUpRollingFrontFront	= false;
			bChangeStandUpRollingFrontBack	= false;
			bChangeStandUpBigAttackFront	= false;
		}
		else if( true == m_InputData.oneX )
		{
			bChangeStandUpAttackFront		= false;
			bChangeStandUpRollingFrontFront	= false;
			bChangeStandUpRollingFrontBack	= false;
			bChangeStandUpBigAttackFront	= true;
		}
		else if( true == m_FrameDataFuture.syncData.bIsRight )
		{
			if( true == m_InputData.pureRight )
			{
				bChangeStandUpAttackFront		= false;
				bChangeStandUpRollingFrontBack	= false;
				bChangeStandUpRollingFrontFront	= true;
				bChangeStandUpBigAttackFront	= false;
			}
			else if( true == m_InputData.pureLeft )
			{
				bChangeStandUpAttackFront		= false;
				bChangeStandUpRollingFrontFront	= false;
				bChangeStandUpRollingFrontBack	= true;
				bChangeStandUpBigAttackFront	= false;
			}
		}
		else
		{
			if( true == m_InputData.pureRight )
			{
				bChangeStandUpAttackFront		= false;
				bChangeStandUpRollingFrontFront	= false;
				bChangeStandUpRollingFrontBack	= true;
				bChangeStandUpBigAttackFront	= false;
			}
			else if( true == m_InputData.pureLeft )
			{
				bChangeStandUpAttackFront		= false;
				bChangeStandUpRollingFrontBack	= false;
				bChangeStandUpRollingFrontFront	= true;
				bChangeStandUpBigAttackFront	= false;
			}
		}
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	bool& bChangeStandUpAttackFront			= m_FrameDataFuture.stateParam.bEventFlagList[1];
	bool& bChangeStandUpRollingFrontFront	= m_FrameDataFuture.stateParam.bEventFlagList[2];
	bool& bChangeStandUpRollingFrontBack	= m_FrameDataFuture.stateParam.bEventFlagList[3];
	bool& bChangeStandUpBigAttackFront		= m_FrameDataFuture.stateParam.bEventFlagList[4];
	
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	float fInputStartTime = ( true == m_bAbleQuickStand ) ? m_fEventTime[0].keyInputStart : 0.33f;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트	
	
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	// 넘어진 후
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{	
		if( true == bChangeStandUpAttackFront )
		{
			StateChange( ESI_STANDUP_ATTACK_FRONT );
		}
		else if ( true == bChangeStandUpBigAttackFront )
		{
			if( GetRandomInt() < m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent )
			{
				StateChange( ESI_QUICK_STANDUP_ATTACK_FRONT );
			}
			else
			{
				StateChange( ESI_STANDUP_ATTACK_FRONT );
			}
		}
		else if( true == bChangeStandUpRollingFrontFront )
		{
			StateChange( ESI_STANDUP_ROLLING_FRONT_FRONT );
		}
		else if( true == bChangeStandUpRollingFrontBack )
		{
			StateChange( ESI_STANDUP_ROLLING_FRONT_BACK );
		}
		else
		{
			StateChange( USI_DAMAGE_STANDUP_FRONT );
		}
	}

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트 
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 5, 0, true == m_bAbleQuickStand && true == bChangeStandUpRollingFrontFront, ESI_STANDUP_ROLLING_FRONT_FRONT )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 6, 0, true == m_bAbleQuickStand && true == bChangeStandUpRollingFrontFront, ESI_STANDUP_ROLLING_FRONT_BACK )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 7, 0, true == m_bAbleQuickStand && true == bChangeStandUpRollingFrontFront, ESI_STANDUP_ATTACK_FRONT )
	
	else if( fInputStartTime < m_pXSkinAnimFuture->GetNowAnimationTime() )
#else SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	else if( 0.33f < m_pXSkinAnimFuture->GetNowAnimationTime() )
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	{
		if( true == m_InputData.oneZ )
		{	
			bChangeStandUpAttackFront		= true;
			bChangeStandUpRollingFrontFront	= false;
			bChangeStandUpRollingFrontBack	= false;
			bChangeStandUpBigAttackFront	= false;

		}
		else if ( true == m_InputData.oneX )
		{
			bChangeStandUpAttackFront		= false;
			bChangeStandUpRollingFrontFront	= false;
			bChangeStandUpRollingFrontBack	= false;
			bChangeStandUpBigAttackFront	= true;
		}
		// 우측 방향을 향하고 있는 경우에
		else if( true == m_FrameDataFuture.syncData.bIsRight )
		{
			// 우측 화살표 키보드를 누른 경우
			if( true == m_InputData.pureRight )
			{	
				// 앞으로 롤링
				bChangeStandUpAttackFront		= false;
				bChangeStandUpRollingFrontBack	= false;
				bChangeStandUpRollingFrontFront	= true;
				bChangeStandUpBigAttackFront	= false;

			}
			// 좌측 화살표 키보드를 누른 경우
			else if( true == m_InputData.pureLeft )
			{	
				// 뒤로 롤링
				bChangeStandUpAttackFront		= false;
				bChangeStandUpRollingFrontFront	= false;
				bChangeStandUpRollingFrontBack	= true;
				bChangeStandUpBigAttackFront	= false;

			}
		}
		// 좌측 방향을 향하고 있는 경우에
		else
		{
			// 우측 화살표 키보드를 누른 경우
			if( true == m_InputData.pureRight )
			{	
				// 뒤로 롤링
				bChangeStandUpAttackFront		= false;
				bChangeStandUpRollingFrontFront	= false;
				bChangeStandUpRollingFrontBack	= true;
				bChangeStandUpBigAttackFront	= false;

			}
			else if( true == m_InputData.pureLeft )
			{	
				// 앞을 롤링
				bChangeStandUpAttackFront		= false;
				bChangeStandUpRollingFrontBack	= false;
				bChangeStandUpRollingFrontFront	= true;
				bChangeStandUpBigAttackFront	= false;
			}
		}
	}

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_DAMAGE_DOWN_BACK
void CX2GUEL::DamageDownBackFrameMove()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b52f00
	if( true == m_pXSkinAnim->EventTimer( 0.32f ) )
		CreateStepDust();

	CommonFrameMove();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_pXSkinAnim->EventTimer( 0.32f ) )
		CreateStepDust();

	CommonFrameMove();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DamageDownBackEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b52f40
	bool& bChangeStandUpAttackBack			= m_FrameDataFuture.stateParam.bEventFlagList[1];
	bool& bChangeStandUpRollingBackFront	= m_FrameDataFuture.stateParam.bEventFlagList[2];
	bool& bChangeStandUpRollingBackBack	= m_FrameDataFuture.stateParam.bEventFlagList[3];
	bool& bChangeStandUpBigAttackBack		= m_FrameDataFuture.stateParam.bEventFlagList[4];
	const float fInputStartTime = ( true == m_bAbleQuickStand ) ? m_fEventTime[0].keyInputStart : 0.33f;

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == bChangeStandUpAttackBack )
			StateChange( ESI_STANDUP_ATTACK_BACK );
		else if( true == bChangeStandUpBigAttackBack )
			StateChange( ESI_QUICK_STANDUP_ATTACK_BACK );
		else if( true == bChangeStandUpRollingBackFront )
			StateChange( ESI_STANDUP_ROLLING_BACK_FRONT );
		else if( true == bChangeStandUpRollingBackBack )
			StateChange( ESI_STANDUP_ROLLING_BACK_BACK );
		else
			StateChange( USI_DAMAGE_STANDUP_BACK );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 5, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpRollingBackFront, ESI_STANDUP_ROLLING_BACK_FRONT )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 6, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpRollingBackBack, ESI_STANDUP_ROLLING_BACK_BACK )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 7, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpAttackBack, ESI_STANDUP_ATTACK_BACK )
	else if( fInputStartTime < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		if( true == m_InputData.oneZ )
		{
			bChangeStandUpAttackBack		= true;
			bChangeStandUpRollingBackFront	= false;
			bChangeStandUpRollingBackBack	= false;
			bChangeStandUpBigAttackBack	= false;
		}
		else if( true == m_InputData.oneX )
		{
			bChangeStandUpAttackBack		= false;
			bChangeStandUpRollingBackFront	= false;
			bChangeStandUpRollingBackBack	= false;
			bChangeStandUpBigAttackBack	= true;
		}
		else if( true == m_FrameDataFuture.syncData.bIsRight )
		{
			if( true == m_InputData.pureRight )
			{
				bChangeStandUpAttackBack		= false;
				bChangeStandUpRollingBackBack	= false;
				bChangeStandUpRollingBackFront	= true;
				bChangeStandUpBigAttackBack	= false;
			}
			else if( true == m_InputData.pureLeft )
			{
				bChangeStandUpAttackBack		= false;
				bChangeStandUpRollingBackFront	= false;
				bChangeStandUpRollingBackBack	= true;
				bChangeStandUpBigAttackBack	= false;
			}
		}
		else
		{
			if( true == m_InputData.pureRight )
			{
				bChangeStandUpAttackBack		= false;
				bChangeStandUpRollingBackFront	= false;
				bChangeStandUpRollingBackBack	= true;
				bChangeStandUpBigAttackBack	= false;
			}
			else if( true == m_InputData.pureLeft )
			{
				bChangeStandUpAttackBack		= false;
				bChangeStandUpRollingBackBack	= false;
				bChangeStandUpRollingBackFront	= true;
				bChangeStandUpBigAttackBack	= false;
			}
		}
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	bool& bChangeStandUpAttackBack			= m_FrameDataFuture.stateParam.bEventFlagList[1];
	bool& bChangeStandUpRollingBackFront	= m_FrameDataFuture.stateParam.bEventFlagList[2];
	bool& bChangeStandUpRollingBackBack		= m_FrameDataFuture.stateParam.bEventFlagList[3];
	bool& bChangeStandUpBigAttackBack		= m_FrameDataFuture.stateParam.bEventFlagList[4];
	// 넘어졌는데 밑에 라인맵 등이 없는 경우
	
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	float fInputStartTime = ( true == m_bAbleQuickStand ) ? m_fEventTime[0].keyInputStart : 0.33f;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트	
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );

		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	// 넘어진 후
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{	
		// Z 키를 눌렀을 때
		if( true == bChangeStandUpAttackBack )
		{
			StateChange( ESI_STANDUP_ATTACK_BACK );
		}
		// X 키를 눌렀을 때
		else if ( true == bChangeStandUpBigAttackBack )
		{
			if( GetRandomInt() < m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent )
			{
				StateChange( ESI_QUICK_STANDUP_ATTACK_BACK );
			}
			else
			{
				StateChange( ESI_STANDUP_ATTACK_BACK );
			}
		}
		else if( true == bChangeStandUpRollingBackFront )
		{
			StateChange( ESI_STANDUP_ROLLING_BACK_FRONT );
		}
		else if( true == bChangeStandUpRollingBackBack )
		{
			StateChange( ESI_STANDUP_ROLLING_BACK_BACK );
		}
		else
		{
			StateChange( USI_DAMAGE_STANDUP_BACK );
		}
	}

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	ELSE_IF_STATE_CHANGE_ON_EX_( 5, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpRollingBackFront, ESI_STANDUP_ROLLING_BACK_FRONT )
	ELSE_IF_STATE_CHANGE_ON_EX_( 6, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpRollingBackBack, ESI_STANDUP_ROLLING_BACK_BACK )
	ELSE_IF_STATE_CHANGE_ON_EX_( 7, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpAttackBack, ESI_STANDUP_ATTACK_BACK )
	else if( fInputStartTime < m_pXSkinAnimFuture->GetNowAnimationTime() )
#else SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	else if( 0.33f < m_pXSkinAnimFuture->GetNowAnimationTime() )
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	{
		if( true == m_InputData.oneZ )
		{	
			bChangeStandUpAttackBack		= true;
			bChangeStandUpRollingBackFront	= false;
			bChangeStandUpRollingBackBack	= false;
			bChangeStandUpBigAttackBack		= false;
		}
		else if ( true == m_InputData.oneX )
		{
			bChangeStandUpAttackBack		= false;
			bChangeStandUpRollingBackFront	= false;
			bChangeStandUpRollingBackBack	= false;
			bChangeStandUpBigAttackBack		= true;
		}
		// 우측 방향을 향하고 있는 경우에
		else if( true == m_FrameDataFuture.syncData.bIsRight )
		{
			// 우측 화살표 키보드를 누른 경우
			if( true == m_InputData.pureRight )
			{	
				// 앞으로 롤링
				bChangeStandUpAttackBack		= false;
				bChangeStandUpRollingBackBack	= false;
				bChangeStandUpRollingBackFront	= true;
				bChangeStandUpBigAttackBack		= false;

			}
			// 좌측 화살표 키보드를 누른 경우
			else if( true == m_InputData.pureLeft )
			{	
				// 뒤로 롤링
				bChangeStandUpAttackBack		= false;
				bChangeStandUpRollingBackFront	= false;
				bChangeStandUpRollingBackBack	= true;
				bChangeStandUpBigAttackBack		= false;

			}
		}
		// 좌측 방향을 향하고 있는 경우에
		else
		{
			// 우측 화살표 키보드를 누른 경우
			if( true == m_InputData.pureRight )
			{	
				// 뒤로 롤링
				bChangeStandUpAttackBack		= false;
				bChangeStandUpRollingBackFront	= false;
				bChangeStandUpRollingBackBack	= true;
				bChangeStandUpBigAttackBack		= false;

			}
			else if( true == m_InputData.pureLeft )
			{	
				// 앞을 롤링
				bChangeStandUpAttackBack		= false;
				bChangeStandUpRollingBackBack	= false;
				bChangeStandUpRollingBackFront	= true;
				bChangeStandUpBigAttackBack		= false;
			}
		}
	}

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_DAMAGE_AIR_DOWN_LANDING
void CX2GUEL::DamageAirDownLandingEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b532f0 ( a jump to 32b52b50 )
	DamageDownFrontEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	DamageDownFrontEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_STANDUP_ROLLING_FRONT_FRONT
void CX2GUEL::StandUpRollingFrontFrontEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b53300
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
			StateChangeDashIfPossible();
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX && true == m_bAbleQuickStandAttack, ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );

		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
			StateChange( USI_WAIT );
	}
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	// 상급 기사 숙련으로 인한 재빠른 기상 공격
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX && true == m_bAbleQuickStandAttack, ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED )	
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::StandUpRollingFrontFrontEnd()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b4f0d0
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateEnd();

	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_STANDUP_ROLLING_FRONT_BACK
void CX2GUEL::StandUpRollingFrontBackEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b53440
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
			StateChangeDashIfPossible();
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX && true == m_bAbleQuickStandAttack, ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );

		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
			StateChange( USI_WAIT );
	}
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	// 상급 기사 숙련으로 인한 재빠른 기상 공격
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX && true == m_bAbleQuickStandAttack, ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED )	
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::StandUpRollingFrontBackEnd()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b4f110
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_STANDUP_ROLLING_BACK_FRONT
void CX2GUEL::StandUpRollingBackFrontEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b53580
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
			StateChangeDashIfPossible();
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX && true == m_bAbleQuickStandAttack, ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else
			StateChange( USI_WAIT );
	}
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	// 상급 기사 숙련으로 인한 재빠른 기상 공격
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX && true == m_bAbleQuickStandAttack, ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED )	
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::StandUpRollingBackFrontEnd()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b4f150
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_STANDUP_ROLLING_BACK_BACK
void CX2GUEL::StandUpRollingBackBackEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b536c0
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
			StateChangeDashIfPossible();
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX && true == m_bAbleQuickStandAttack, ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else
			StateChange( USI_WAIT );
	}
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	// 상급 기사 숙련으로 인한 재빠른 기상 공격
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX && true == m_bAbleQuickStandAttack, ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED )	
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::StandUpRollingBackBackEnd()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b4f190
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_STANDUP_ATTACK_FRONT
void CX2GUEL::StandUpAttackFrontEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b53800
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::StandUpAttackFrontEnd()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b4f1d0
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_STANDUP_ATTACK_BACK
void CX2GUEL::StandUpAttackBackEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b53880
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::StandUpAttackBackEnd()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b4f210
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::StandUpAttackBackStateEndFuture()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b4f250
	m_FrameDataFuture.syncData.bIsRight = !m_FrameDataFuture.syncData.bIsRight;
	CommonStateEndFuture();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_FrameDataFuture.syncData.bIsRight = !m_FrameDataFuture.syncData.bIsRight;
	CommonStateEndFuture();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
//ESI_STANDUP_ATTACK_FRONT_NO_DOWN
void CX2GUEL::StandUpAttackFrontNoDownEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b53900
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::StandUpAttackFrontNoDownEnd()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b4f270
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_STANDUP_ATTACK_BACK_NO_DOWN
void CX2GUEL::StandUpAttackBackNoDownEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b53980
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::StandUpAttackBackNoDownEnd()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1b: June 32b4f2b0
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

//ESI_DASH_JUMP_POWER_LANDING
void CX2GUEL::DashJumpPowerLandingStartFuture()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b4f2f0
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x /= 2.0f;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x /= 2.0f;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashJumpPowerLandingStart()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b4f320
	CommonStateStart();
	CreateStepDust();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateStart();
	CreateStepDust();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashJumpPowerLandingEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b75000
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneRight == true || m_InputData.oneLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() / 6.0f )
	{
		if( m_InputData.oneZ == true )
		{
			StateChange( ESI_COMBO_Z );
		}
		else if( m_InputData.oneX == true )
		{
			StateChange( ESI_COMBO_X );
		}
		else if( SpecialAttackEventProcess() == true )
		{
		}
		else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else if( m_InputData.pureUp == true )
		{
			StateChange( USI_JUMP_READY );
		}
		else if( m_InputData.oneDown == true 
			&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, 
			&m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}

		else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneRight == true || m_InputData.oneLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() / 6.0f )
	{
		if( m_InputData.oneZ == true )
		{
			StateChange( ESI_COMBO_Z );
		}
		else if( m_InputData.oneX == true )
		{
			StateChange( ESI_COMBO_X );
		}
		else if( SpecialAttackEventProcess() == true )
		{
		}
		else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else if( m_InputData.pureUp == true )
		{
			StateChange( USI_JUMP_READY );
		}
		//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE( 10.f, ESSI_REVENGE_WAIT )
		else if( m_InputData.oneDown == true 
			&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, 
			&m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}

		else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ComboZEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d: June 32b53a00
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneZ == true, ESI_COMBO_ZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, ESI_COMBO_ZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ComboZZEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d: June 32b53d50
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneZ == true, ESI_COMBO_ZZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, ESI_COMBO_ZZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	
	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ComboZZZEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d: June 32b540a0 ( flags 2 / 3 / 4 test the unit class directly, as June )
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		m_InputData.oneX == true && CX2Unit::UC_ELESIS_CRIMSON_AVENGER == GetUnitClass(), ESI_COMBO_SDK_ZZZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneZ == true, ESI_COMBO_ZZZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 3, m_fEventTime[3].keyInputStart, m_fEventTime[3].keyInputEnd, m_fEventTime[3].stateChange,
		m_InputData.oneX == true && CX2Unit::UC_ELESIS_GRAND_MASTER == GetUnitClass(), ESI_COMBO_SGM_ZZZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 4, m_fEventTime[4].keyInputStart, m_fEventTime[4].keyInputEnd, m_fEventTime[4].stateChange,
		m_InputData.oneX == true && CX2Unit::UC_ELESIS_BLAZING_HEART == GetUnitClass(), ESI_COMBO_SBH_ZZZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneX == true, ESI_COMBO_ZZZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, ESI_COMBO_ZZZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, m_InputData.oneX == true, ESI_COMBO_ZZZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ComboZZZZEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d: June 32b54680 ( the 550 air speed is unconditional, March's sits under BALANCE_PATCH_20131107 )
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		m_PhysicParam.nowSpeed.x = 550.f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// Speed_Time 의 경우, End 에서 처리해주는 기능이 없어서
		// 해당 모션 중 라인 맵에서 떨어지면 Speed 값 자체를 550 으로 고정
		m_PhysicParam.nowSpeed.x = 550.f;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ComboZZZXEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d: June 32b54910
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ComboXEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d: June 32b54b90 ( flags 2 / 1 test the job line, March tested the class )
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		m_InputData.oneX == true && IsInJobLine( 3 ), ESI_COMBO_SDK_XX )		// job line 3 = Dark Knight / Crimson Avenger
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX == true, ESI_COMBO_XX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ == true && IsInJobLine( 2 ), ESI_COMBO_SBH_XZ )		// job line 2 = Pyro Knight / Blazing Heart
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneX == true, ESI_COMBO_XX )
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, m_InputData.oneZ == true && CX2Unit::UC_ELESIS_BLAZING_HEART == GetUnitClass() ,
										ESI_COMBO_SBH_XZ )
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ComboXXEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d: June 32b55040 ( flag 2 tests the job line, March tested the class )
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		m_InputData.pureDown == true && m_InputData.oneX == true && IsInJobLine( 1 ), ESI_COMBO_SGM_XX_DOWN_X_START )		// job line 1 = Saber Knight / Grand Master
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX == true, ESI_COMBO_XXX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ == true, ESI_COMBO_XXZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 2, 2, m_InputData.pureDown == true && m_InputData.oneX == true && 
		CX2Unit::UC_ELESIS_GRAND_MASTER == GetUnitClass(), ESI_COMBO_SGM_XX_DOWN_X_START )
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트	
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneX == true, ESI_COMBO_XXX )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, m_InputData.oneZ == true, ESI_COMBO_XXZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ComboXXXEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d: June 32b554e0
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ComboXXZEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d: June 32b55760
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::JumpAttackZEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b6e980
	if ( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			m_fCanNotJumpTime = 0.2f;
		}
		StateChange( USI_JUMP_LANDING );
	}
	else if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
		( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			m_fCanNotJumpTime = 0.2f;
		}
		StateChange( USI_JUMP_LANDING );
	}
	// 현재 진행 방향 쪽으로 키보드를 누르면 앞으로 좀 더 나아감
	else if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
		( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::JumpAttackZFrameMoveFuture()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b4f340
	if( m_InputData.pureDown == true )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	CommonFrameMoveFuture();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( m_InputData.pureDown == true )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	CommonFrameMoveFuture();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::JumpAttackXEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b6ea10
	if ( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			m_fCanNotJumpTime = 0.2f;
		}
		StateChange( USI_JUMP_LANDING );
	}
	else if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
		( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			m_fCanNotJumpTime = 0.2f;
		}
		StateChange( USI_JUMP_LANDING );
	}
	// 현재 진행 방향 쪽으로 키보드를 누르면 앞으로 좀 더 나아감
	else if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
		( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::JumpAttackXFrameMoveFuture()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b4f380
	if( m_InputData.pureDown == true )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	CommonFrameMoveFuture();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( m_InputData.pureDown == true )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	CommonFrameMoveFuture();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashComboZEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d2: June 32b559e0 ( flags 4 / 5 are the job line 3 arms, tested through IsInJobLine )
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 4, m_fEventTime[4].keyInputStart, m_fEventTime[4].keyInputEnd, m_fEventTime[4].stateChange,
		m_InputData.oneZ == true && IsInJobLine( 3 ), ESI_DASH_COMBO_SCA_ZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 5, m_fEventTime[5].keyInputStart, m_fEventTime[5].keyInputEnd, m_fEventTime[5].stateChange,
		m_InputData.oneX == true && IsInJobLine( 3 ), ESI_DASH_COMBO_SCA_ZX_START )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ == true && IsInJobLine( 1 ), ESI_DASH_COMBO_SSK_ZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		m_InputData.oneZ == true && IsInJobLine( 2 ), ESI_DASH_COMBO_SPK_ZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneZ == true, ESI_DASH_COMBO_ZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 3, m_fEventTime[3].keyInputStart, m_fEventTime[3].keyInputEnd, m_fEventTime[3].stateChange,
		m_InputData.oneX == true && IsInJobLine( 2 ), ESI_DASH_COMBO_SPK_ZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, m_InputData.oneZ == true && GetUnitClassLine( UCL_FIRST ) ,
		ESI_DASH_COMBO_SSK_ZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 2, 2, m_InputData.oneZ == true && GetUnitClassLine( UCL_SECOND ) ,
										ESI_DASH_COMBO_SPK_ZZ )
#else //SERV_ELESIS_SECOND_CLASS_CHANGE
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, m_InputData.oneZ == true && CX2Unit::UC_ELESIS_SABER_KNIGHT == GetUnitClass() ,
		ESI_DASH_COMBO_SSK_ZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 2, 2, m_InputData.oneZ == true && CX2Unit::UC_ELESIS_PYRO_KNIGHT == GetUnitClass() ,
										ESI_DASH_COMBO_SPK_ZZ )
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, ESI_DASH_COMBO_ZZ )
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 3, 3, true == m_InputData.oneX && GetUnitClassLine( UCL_SECOND ) ,
										ESI_DASH_COMBO_SPK_ZX )
#else //SERV_ELESIS_SECOND_CLASS_CHANGE
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 3, 3, true == m_InputData.oneX && CX2Unit::UC_ELESIS_PYRO_KNIGHT == GetUnitClass() ,
										ESI_DASH_COMBO_SPK_ZX )
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashComboZZEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d2: June 32b56090
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashComboXEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d2: June 32b562f0 ( flag 0 also needs 2.0 MP, no Enum entry; the class tests are compares as June )
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX == true && CX2Unit::UC_ELESIS_GRAND_MASTER == GetUnitClass() && GetNowMp() >= 2.0f, ESI_DASH_COMBO_SGM_XX_ATTACK )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ == true && CX2Unit::UC_ELESIS_CRIMSON_AVENGER == GetUnitClass(), ESI_DASH_COMBO_SCA_XZ_CHARGE )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashJumpComboZEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d2: June 32b56710 ( flags 1 / 2 / 3 are the job line arms )
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() &&
		true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneX == true && IsInJobLine( 1 ), ESI_DASH_JUMP_COMBO_SSK_ZZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		m_InputData.oneX == true && IsInJobLine( 2 ), ESI_DASH_JUMP_COMBO_SPK_ZX_READY )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 3, m_fEventTime[3].keyInputStart, m_fEventTime[3].keyInputEnd, m_fEventTime[3].stateChange,
		m_InputData.oneX == true && IsInJobLine( 3 ), ESI_DASH_JUMP_COMBO_SDK_ZX_CHARGE )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneZ == true, ESI_DASH_JUMP_COMBO_ZZ )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() &&
		true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, ESI_DASH_JUMP_COMBO_ZZ );

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashJumpComboZZEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d2: June 32b56aa0
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() &&
		true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneX == true && IsInJobLine( 1 ), ESI_DASH_JUMP_COMBO_SSK_ZZX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneZ == true, ESI_DASH_JUMP_COMBO_ZZZ )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() &&
		true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, ESI_DASH_JUMP_COMBO_ZZZ )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashJumpComboZZZEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d2: June 32b56c80
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() &&
		true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( true == IsOnSomethingFuture() )
		{
			StateChange( USI_DASH_JUMP_LANDING );
		};
	}

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashJumpComboXEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d2: June 32b56cc0 ( the class tests are compares as June: flag 1 Grand Master, flag 2 Blazing Heart, flags 3 / 4 as listed )
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 4, m_fEventTime[4].keyInputStart, m_fEventTime[4].keyInputEnd, m_fEventTime[4].stateChange,
		m_InputData.oneX == true && CX2Unit::UC_ELESIS_CRIMSON_AVENGER == GetUnitClass(), ESI_DASH_JUMP_COMBO_SDK_XX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneX == true && CX2Unit::UC_ELESIS_GRAND_MASTER == GetUnitClass(), ESI_DASH_JUMP_COMBO_SSK_XX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		m_InputData.oneZ == true && CX2Unit::UC_ELESIS_BLAZING_HEART == GetUnitClass(), ESI_DASH_JUMP_COMBO_SPK_XZ )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX == true, ESI_DASH_JUMP_COMBO_XX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 3, m_fEventTime[3].keyInputStart, m_fEventTime[3].keyInputEnd, m_fEventTime[3].stateChange,
		m_InputData.oneZ == true && CX2Unit::UC_ELESIS_GRAND_MASTER == GetUnitClass(), ESI_DASH_JUMP_COMBO_SGM_XZ )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, true == m_InputData.oneX && GetUnitClassLine( UCL_FIRST ),
		ESI_DASH_JUMP_COMBO_SSK_XX )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 2, 2, true == m_InputData.oneZ && GetUnitClassLine( UCL_SECOND ),
										ESI_DASH_JUMP_COMBO_SPK_XZ )
#else //SERV_ELESIS_SECOND_CLASS_CHANGE
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, true == m_InputData.oneX && CX2Unit::UC_ELESIS_SABER_KNIGHT == GetUnitClass() ,
		ESI_DASH_JUMP_COMBO_SSK_XX )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 2, 2, true == m_InputData.oneZ && CX2Unit::UC_ELESIS_PYRO_KNIGHT == GetUnitClass(),
										ESI_DASH_JUMP_COMBO_SPK_XZ )
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, true == m_InputData.oneX , ESI_DASH_JUMP_COMBO_XX )

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	ELSE_IF_STATE_CHANGE_ON_EX_( 3, m_fEventTime[3].keyInputStart, m_fEventTime[3].keyInputEnd, m_fEventTime[3].stateChange,
	m_InputData.oneZ == true && CX2Unit::UC_ELESIS_GRAND_MASTER == GetUnitClass(), ESI_DASH_JUMP_COMBO_SGM_XZ )
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashJumpComboXXEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d2: June 32b570d0
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( ESI_DASH_JUMP_COMBO_XX_LANDING );
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( ESI_DASH_JUMP_COMBO_XX_LANDING );
	}

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::DashJumpComboXXLandingEventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1d2: June 32b57100
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX == true && IsInJobLine( 1 ), ESI_DASH_JUMP_COMBO_SSK_XXX )
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
#ifdef ADD_CAN_NOT_ATTACK_JUMP_DOWN
void CX2GUEL::ESI_CAN_NOT_ATTACK_JUMP_DOWN_EventProcess()
{	
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b57240
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
	else if( (m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true ) &&
		GetEnableDash() == true )
	{
		StateChange( USI_DASH_JUMP );
	}
	else if( true == m_FrameDataFuture.unitCondition.bFootOnWall && 
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( ESI_WALL_LANDING );
	}
	else if ( 0.0f <= m_PhysicParam.nowSpeed.y )
	{
		StateChange( USI_JUMP_UP );
	}
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
	else if( (m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true ) &&
		GetEnableDash() == true )
	{
		StateChange( USI_DASH_JUMP );
	}
	else if( true == m_FrameDataFuture.unitCondition.bFootOnWall && 
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( ESI_WALL_LANDING );
	}
	else if ( 0.0f <= m_PhysicParam.nowSpeed.y )
	{
		StateChange( USI_JUMP_UP );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
#endif // ADD_CAN_NOT_ATTACK_JUMP_DOWN
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 스킬 스테이트 함수 - 노전직
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  닷지&슬래시
void CX2GUEL::ESI_SI_A_EL_DODGE_AND_SLASH_Init()
{
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"RingAlpha.dds");
	TextureReadyInBackground(L"Smoke.dds");
	TextureReadyInBackground(L"waldo_trock_specialAttackB_particle.dds");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
}
void CX2GUEL::ESI_SI_A_EL_DODGE_AND_SLASH_FrameMove()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	CommonFrameMove();	// char-elesis CH2a: June 32b4f410 ( the evasion buff is no longer a 0.001 s event here )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_EL_DODGE_AND_SLASH );
		if( NULL != pSkillTemplet )
		{	// 회피 버프 적용
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}
	}

	CommonFrameMove();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
void CX2GUEL::ESI_SI_A_EL_DODGE_AND_SLASH_StartFuture()
{
	CommonStateStartFuture();
	// 스킬키 재입력 체크를 위해 입력한 스킬키에 대한 정보 설정
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_kSkillSlotKey.Set( m_iNowSpecialAttack - 1 );	// char-elesis CH1p: June 32b72740 @32b7275e: the held key in June's slot space ( A 0-4, B 5-9 )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_sCurrentSkillKeyManager.InitCurrentSkillKeyManager( m_iNowSpecialAttack - 1 );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE

	// 재사용 횟수 초기화
	m_uiDodgeAndSlashReuseCount = m_uiDodgeAndSlashMaxReuseCount;
}
void CX2GUEL::ESI_SI_A_EL_DODGE_AND_SLASH_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	m_kSkillSlotKey.FollowSlotSwap();	// char-elesis CH2a: June 32b727f0
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if ( 0 < m_uiDodgeAndSlashReuseCount && true == m_kSkillSlotKey.IsKeyPressedOnce( m_InputData ) )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_EL_DODGE_AND_SLASH, true );
		const bool bBackKeyHeld = ( true == m_InputData.pureRight && false == m_FrameDataFuture.syncData.bIsRight )
			|| ( true == m_InputData.pureLeft && true == m_FrameDataFuture.syncData.bIsRight );
		if ( NULL != pSkillTemplet && 0 != pSkillTemplet->m_iDeleteSkillIDByUpgrade && true == bBackKeyHeld )	// only the enhanced skill steps back
			StateChange( ESI_SI_A_EL_DODGE_AND_SLASH_BACK );
		else
			StateChange( ESI_SI_A_EL_DODGE_AND_SLASH );
	}
	else if ( ( true == m_InputData.oneZ || true == m_InputData.oneX ) && m_uiDodgeAndSlashAttackCount < 1 )
	{
		++m_uiDodgeAndSlashAttackCount;
		StateChange( ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	// 스킬 슬롯 체인지 사용 여부 체크
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_kSkillSlotKey.FollowSlotSwap();	// char-elesis CH1p: June 32b727f0 @32b727fe: a slot swap moves the held key by 5, not March's 4
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_sCurrentSkillKeyManager.CheckSkillSlotSwap();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE

	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if ( m_uiDodgeAndSlashReuseCount > 0 &&
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
			  true == m_kSkillSlotKey.IsKeyPressedOnce( m_InputData ) )	// char-elesis CH1p: June 32b727f0 @32b72852
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
			  true == m_sCurrentSkillKeyManager.IsPressedCurrentSkillKey(m_InputData) )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	{
		StateChange( ESI_SI_A_EL_DODGE_AND_SLASH_SECOND );
	}
	else if ( true == m_InputData.oneZ || true == m_InputData.oneX )
	{
		StateChange( ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
void CX2GUEL::ESI_SI_A_EL_DODGE_AND_SLASH_SECOND_StartFuture()
{
	CommonStateStartFuture();

	// 재사용 횟수 줄이기
	--m_uiDodgeAndSlashReuseCount;
}
/** @function : ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z_EventProcess
	@brief : 닷지&슬래시 중 Z&X 공격
*/
void CX2GUEL::ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	else if ( 0 < m_uiDodgeAndSlashReuseCount && true == m_kSkillSlotKey.IsKeyPressedOnce( m_InputData ) )	// char-elesis CH2a: June 32b6eaa0
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_EL_DODGE_AND_SLASH, true );
		if ( NULL != pSkillTemplet && 0 != pSkillTemplet->m_iDeleteSkillIDByUpgrade )	// only the enhanced skill steps back
			StateChange( ESI_SI_A_EL_DODGE_AND_SLASH_BACK );
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}
// 도약
void CX2GUEL::ESI_SI_A_EL_LEAP_CRASH_Init()
{
	TextureReadyInBackground(L"Smoke.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
}
void CX2GUEL::ESI_SI_A_EL_LEAP_CRASH_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_SI_A_EL_LEAP_CRASH_JUMP );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2b2: June 32b57350 ( its memo branch to state 312 is dead: 31fca9e0 returns 0 )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	CommonEventProcess();
}
void CX2GUEL::ESI_SI_A_EL_LEAP_CRASH_JUMP_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( ESI_SI_A_EL_LEAP_CRASH_JUMP_LANDING );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2b2: June 32b57680 ( its memo branch to state 313 is dead: 31fca9e0 returns 0 )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	CommonEventProcess();
}
void CX2GUEL::ESI_SI_A_EL_LEAP_CRASH_JUMP_LANDING_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2b2: June 32b57960
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
// char-elesis CH2c: June 32b52460 ( Avatar of the Fire preloads nothing )
void CX2GUEL::ESI_SI_A_SPK_AVATAR_OF_THE_FIRE_Init()
{
}

// char-elesis CH2c: June 32b6b9c0 ( at the end the unit lands or falls; the walk / back-walk / dash cancels need the foot on a line, no Z-X cancel )
void CX2GUEL::ESI_SI_A_SPK_AVATAR_OF_THE_FIRE_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? USI_WAIT : USI_JUMP_DOWN );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH2c: June 32b7db80 ( the aura absorbs the fire debuffs of every living unit within range for 10 MP each, then the damage effect set plays; no team test )
void CX2GUEL::ESI_SI_A_SPK_AVATAR_OF_THE_FIRE_FrameMove()
{
	CommonFrameMove();

	if ( true == m_pXSkinAnim->EventTimerOneshot( AVATAR_OF_THE_FIRE_AURA_EVENT_TIME ) )
	{
		const CX2EffectSet::Handle hAura = g_pX2Game->GetEffectSet()->PlayEffectSet(
			0 == m_usBlazeAbsoluLevel ? L"EffectSet_A_SPK_Avatar_Of_TheFire" : L"EffectSet_A_SPK_Avatar_Of_TheFire_FireKing", this );
		vector<DELETE_CONDITION_EFFECT_SET> vecAuraDeleteCondition;
		vecAuraDeleteCondition.push_back( DCES_DAMAGE_REACT );
		AddEffectSetDeleteCondition( hAura, vecAuraDeleteCondition );

		float fSizeRate = 1.f;
		if ( true == TryGetEquippedSkillAbilityValue( CX2SkillTree::SA_SIZE_ABS, CX2SkillTree::RNSI_A_SPK_AVATAR_OF_THE_FIRE_UPGRADE, true, fSizeRate ) )
			g_pX2Game->GetEffectSet()->AddDamageEffectModifier( hAura, EFFECT_MODIFIER_TAG_SCALE, D3DXVECTOR3( fSizeRate, fSizeRate, fSizeRate ), false );

		int iAbsorbedCount = 0;
		for ( int i = 0; i < g_pX2Game->GetUnitNum(); ++i )
		{
			CX2GameUnit* pUnit = g_pX2Game->GetUnit( i );
			if ( NULL == pUnit || false == g_pX2Game->IsValidUnit( pUnit ) || CX2GameUnit::GUSI_DIE == pUnit->GetGameUnitState() || false == ( 0.f < pUnit->GetNowHp() ) )
				continue;
			if ( GetDistance( GetPos(), pUnit->GetPos() ) > AVATAR_OF_THE_FIRE_ABSORB_RANGE )
				continue;

			int iUnitAbsorbed = 0;
			if ( 0.f < pUnit->GetExtraDamagePack().m_Fire.m_fTime )
			{
				pUnit->AccessExtraDamagePack().Init( CX2DamageManager::EDT_FIRE );
				++iUnitAbsorbed;
			}
			if ( 0.f < pUnit->GetExtraDamagePack().m_EnchantBlaze.m_fTime )
			{
				pUnit->AccessExtraDamagePack().Init( CX2DamageManager::EDT_ENCHANT_BLAZE );
				++iUnitAbsorbed;
			}
			if ( true == pUnit->HaveThisBuff( BTI_DEBUFF_FIRE ) )
			{
				pUnit->EraseBuffTempletFromGameUnit( BTI_DEBUFF_FIRE );
				++iUnitAbsorbed;
			}
			if ( true == pUnit->HaveThisBuff( BTI_DEBUFF_UNEXTINGUISHABLE_FIRE ) )
			{
				iUnitAbsorbed += max( 1, static_cast<int>( pUnit->GetBuffAccumulationCountNowInFactor( BTI_DEBUFF_UNEXTINGUISHABLE_FIRE ) ) );
				pUnit->EraseBuffTempletFromGameUnit( BTI_DEBUFF_UNEXTINGUISHABLE_FIRE );
			}

			if ( 0 < iUnitAbsorbed )
			{
				CKTDGParticleSystem::CParticleEventSequence* pAbsorbSeq = g_pX2Game->GetMajorParticle()->CreateSequence(
					static_cast<CKTDGObject*>( this ), L"Avatar_of_the_Fire_Absorb_P01", pUnit->GetBonePos( L"Bip01" ) );
				if ( NULL != pAbsorbSeq )
					pAbsorbSeq->SetBlackHolePosition( GetBonePos( L"Bip01" ) );
			}
			iAbsorbedCount += iUnitAbsorbed;
		}

		const float fMpRefund = min( static_cast<float>( iAbsorbedCount ) * AVATAR_OF_THE_FIRE_MP_PER_ABSORBED_EFFECT, GetMaxMp() );
		if ( 0 < iAbsorbedCount )
			UpNowMpWithText( fMpRefund );
	}

	if ( true == m_pXSkinAnim->EventTimerOneshot( AVATAR_OF_THE_FIRE_DAMAGE_EVENT_TIME ) )
	{
		const CX2EffectSet::Handle hDamage = g_pX2Game->GetEffectSet()->PlayEffectSet(
			0 == m_usBlazeAbsoluLevel ? L"EffectSet_A_SPK_Avatar_of_the_Fire_Damage" : L"EffectSet_A_SPK_Avatar_of_the_Fire_Damage_FireKing", this );
		vector<DELETE_CONDITION_EFFECT_SET> vecDamageDeleteCondition;
		vecDamageDeleteCondition.push_back( DCES_NONE );
		AddEffectSetDeleteCondition( hDamage, vecDamageDeleteCondition );

		float fSizeRate = 1.f;
		if ( true == TryGetEquippedSkillAbilityValue( CX2SkillTree::SA_SIZE_ABS, CX2SkillTree::RNSI_A_SPK_AVATAR_OF_THE_FIRE_UPGRADE, true, fSizeRate ) )
			g_pX2Game->GetEffectSet()->AddDamageEffectModifier( hDamage, EFFECT_MODIFIER_TAG_SCALE, D3DXVECTOR3( fSizeRate, fSizeRate, fSizeRate ), false );
	}
}

// char-elesis CH2c: June 32b6f160 ( Swordfire in the air: lands at the end, a held direction keeps the walk speed )
void CX2GUEL::ESI_SI_SA_SPK_SWORDFIRE_FLY_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( true == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_LANDING );
		}
		else if ( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
			( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
		{
			m_PhysicParam.nowSpeed.x = GetWalkSpeed();
		}
	}
	CommonEventProcess();
}

// char-elesis CH2c: June 32b5e860 ( Swordfire on the ground: the usual skill end and cancels )
void CX2GUEL::ESI_SI_SA_SPK_SWORDFIRE_GROUND_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
// char-elesis CH2b2: June 32b70d30 ( a forward and upward speed burst at the start of the jump )
void CX2GUEL::ESI_SI_A_EL_LEAP_CRASH_JUMP_StateStartFuture()
{
	CommonStateStartFuture();

	TIME_SPEED timeSpeed;
	timeSpeed.vSpeed = D3DXVECTOR3( LEAP_CRASH_JUMP_SPEED_X, LEAP_CRASH_JUMP_SPEED_Y, 0.f );
	timeSpeed.iFlag = 1;	// no enum: 1 adds to the speed ( X2GameUnit.h TIME_SPEED )
	if ( 0.f < m_fDashElapsedTime )
		timeSpeed.vSpeed.x = LEAP_CRASH_JUMP_SPEED_X_AFTER_DASH;
	m_vecSpeedFactor.push_back( timeSpeed );
}

// char-elesis CH2b2: June 32b75210 ( the landing shock wave grows with the dash time before the jump )
void CX2GUEL::ESI_SI_A_EL_LEAP_CRASH_JUMP_LANDING_FrameMove()
{
	CommonFrameMove();

	if ( true == m_pXSkinAnim->EventTimerOneshot( LEAP_CRASH_LANDING_EVENT_TIME ) )
	{
		const float fScale = min( m_fDashElapsedTime * LEAP_CRASH_LANDING_SCALE_PER_DASH_TIME + 1.f, LEAP_CRASH_LANDING_SCALE_LIMIT );
		// June passes false for the hyper flag here ( JUNE_BUGS )
		const CX2EffectSet::Handle hEffect = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_EL_LEAP_CRASH_Landing", this, NULL,
			false, GetPowerRate() * fScale, -1.f, D3DXVECTOR3( fScale, fScale, fScale ) );

		float fSizeRate = 1.f;
		if ( true == TryGetEquippedSkillAbilityValue( CX2SkillTree::SA_SIZE_ABS, CX2SkillTree::RNSI_A_SSK_LEAP_CRASH_UPGRADE, true, fSizeRate ) )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::RNSI_A_SSK_LEAP_CRASH_UPGRADE );
			if ( NULL != pSkillTemplet && static_cast<size_t>( LEAP_CRASH_UPGRADE_NUMERICAL_INDEX ) < pSkillTemplet->m_vecNumericalData.size() )
			{
				CX2SkillTree::SkillTemplet::NumericalRow row;
				pSkillTemplet->GetNumericalRowByIndex( LEAP_CRASH_UPGRADE_NUMERICAL_INDEX, 1, row );
				fSizeRate = ( ( fScale - 1.f ) * ( row.m_fValue - 1.f ) + 1.f ) * fSizeRate;
			}
			g_pX2Game->GetEffectSet()->AddDamageEffectModifier( hEffect, EFFECT_MODIFIER_TAG_SCALE, D3DXVECTOR3( fSizeRate, fSizeRate, fSizeRate ), false );
		}
		else if ( true == TryGetEquippedSkillAbilityValue( CX2SkillTree::SA_SIZE_ABS, CX2SkillTree::RNSI_A_SSK_LEAP_CRASH, true, fSizeRate ) )
		{
			fSizeRate = ( fScale - 1.f ) * ( fSizeRate - 1.f ) + 1.f;
			g_pX2Game->GetEffectSet()->AddDamageEffectModifier( hEffect, EFFECT_MODIFIER_TAG_SCALE, D3DXVECTOR3( fSizeRate, fSizeRate, fSizeRate ), false );
		}
	}
}

// char-elesis CH2b2: June 32b72d70 ( the loop count restarts unless the next state is another Rushing Sword hit )
void CX2GUEL::ESI_SI_A_EL_RUSHING_SWORD_StateEndFuture()
{
	if ( ESI_SI_A_EL_RUSHING_SWORD_2nd != m_FrameDataFuture.unitCondition.nextState && ESI_SI_A_EL_RUSHING_SWORD_3rd != m_FrameDataFuture.unitCondition.nextState )
		m_iRushingSwordNowLoopValue = 0;
	CommonStateEndFuture();
}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
/** @function : ESI_SI_A_EL_RUSHING_SWORD_StartFuture
	@brief : 러싱 소드
			 2회 추가 공격에 대해서 모두 스테이트 를 분리 함.
			 각 스테이트별로 슈퍼아머를 다르게 설정하기 위한 분리.

*/
void CX2GUEL::ESI_SI_A_EL_RUSHING_SWORD_StartFuture()
{
	CommonStateStartFuture();
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	m_iRushingSwordNowLoopValue = 1;	// char-elesis CH2b2: June 32b72cc0 @32b72cd5 ( the count starts at 1 here; March's StateStart reset it to 0 )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	m_kSkillSlotKey.Set( m_iNowSpecialAttack - 1 );	// char-elesis CH1p: June 32b72cc0 @32b72d5c
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_sCurrentSkillKeyManager.InitCurrentSkillKeyManager( m_iNowSpecialAttack - 1 );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ESI_SI_A_EL_RUSHING_SWORD_Init()
{
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	TextureReadyInBackground(L"RingAlpha.dds");
	TextureReadyInBackground(L"Smoke.dds");
	TextureReadyInBackground(L"Whitecircle03.dds");
	TextureReadyInBackground(L"title_StarLight_Smash_spark.dds");
	TextureReadyInBackground(L"waldo_trock_specialAttackB_particle.dds");
}
void CX2GUEL::ESI_SI_A_EL_RUSHING_SWORD_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	// char-elesis CH2b2: June 32b85470 ( serves the 1st and the 2nd hit; the next hit's MP is June's skill-use value, the loop count one more for the enhanced skill )
	m_kSkillSlotKey.FollowSlotSwap();
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if ( m_fSkillCancelAfter >= 0.f && m_fSkillCancelAfter < m_pXSkinAnimFuture->GetNowAnimationTime() &&
		true == m_kSkillSlotKey.IsKeyPressedOnce( m_InputData ) )
	{
		const CX2SkillTree::SKILL_ID eSkillID = GetUpgradeSwappedSkillID( CX2SkillTree::RNSI_A_SSK_RUSHING_SWORD );
		const int iSkillLevel = max( 1, GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( eSkillID, false, 0, true ) );
		const float fMpConsume = GetActualMPConsume( eSkillID, iSkillLevel );
		if ( GetNowMp() < fMpConsume )
		{
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1),
				D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
		}
		else
		{
			if ( true == m_InputData.pureRight )
				m_FrameDataFuture.syncData.bIsRight = true;
			else if ( true == m_InputData.pureLeft )
				m_FrameDataFuture.syncData.bIsRight = false;

			UpNowMp( -fMpConsume );
			const int iLastLoopValue = m_iRushingSwordAddLoopValue + ( true == IsNowStateSkillUpgraded() ? 1 : 0 ) + 1;
			if ( iLastLoopValue >= m_iRushingSwordNowLoopValue )
				StateChange( ESI_SI_A_EL_RUSHING_SWORD_2nd );
			else
				StateChange( ESI_SI_A_EL_RUSHING_SWORD_3rd );
		}
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_kSkillSlotKey.FollowSlotSwap();	// char-elesis CH1p: June 32b85470 @32b854b9
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_sCurrentSkillKeyManager.CheckSkillSlotSwap();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE

	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	// 스크립트에서 SKILL_CANCEL_AFTER로 지정된 시간 이후, 스킬 키가 입력 되면 처리
	else if( m_fSkillCancelAfter >= 0.f && m_fSkillCancelAfter < m_pXSkinAnimFuture->GetNowAnimationTime() &&
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
		true == m_kSkillSlotKey.IsKeyPressedOnce( m_InputData ) )	// char-elesis CH1p: June 32b85470 @32b8551f
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
		true == m_sCurrentSkillKeyManager.IsPressedCurrentSkillKey(m_InputData) )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	{ 
		// MP체크
		if ( GetNowMp() < m_fMPConsume )		
		{
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == false)
			{
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1),
					D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
			}
#else
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1),
				D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
#endif ALWAYS_SCREEN_SHOT_TEST
		}
		else
		{
			if( true == m_InputData.pureRight )
				m_FrameDataFuture.syncData.bIsRight = true;
			else if( true == m_InputData.pureLeft )
				m_FrameDataFuture.syncData.bIsRight = false;

			UpNowMp( -m_fMPConsume );
			StateChange( ESI_SI_A_EL_RUSHING_SWORD_2nd );
		}
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

void CX2GUEL::ESI_SI_A_EL_RUSHING_SWORD_StateStart()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	CommonStateStart();	// char-elesis CH2b2: June 32b705b0 ( the same body as 32b704c0, now for all three hits )
	if ( 0 < m_iWaltzOfSwordDodgeAbility && WSS_DESTRUCTION == GetWayOfSwordState() )
		m_DamageData.damage.fPhysic *= GetSkillNumericalByName( L"ATK_REL", CX2SkillTree::RNSI_P_SGM_WALTZ_OF_SWORD, true );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	m_iRushingSwordNowLoopValue = 0;
	CommonStateStart();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
void CX2GUEL::ESI_SI_A_EL_RUSHING_SWORD_2nd_StartFuture()
{
	++m_iRushingSwordNowLoopValue;
	CommonStateStartFuture();
}


#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

void CX2GUEL::ESI_SI_A_EL_RUSHING_SWORD_2nd_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	ESI_SI_A_EL_RUSHING_SWORD_EventProcess();	// char-elesis CH2b2: June 32b85470 is the 2nd hit's EventProcess too
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_kSkillSlotKey.FollowSlotSwap();	// char-elesis CH1p: June 32b85470 @32b854b9 ( the 2nd state shares the function )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_sCurrentSkillKeyManager.CheckSkillSlotSwap();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE

	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	// 스크립트에서 SKILL_CANCEL_AFTER로 지정된 시간 이후, 스킬 키가 입력 되면 처리
	else if( m_fSkillCancelAfter >= 0.f && m_fSkillCancelAfter < m_pXSkinAnimFuture->GetNowAnimationTime() &&
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
		true == m_kSkillSlotKey.IsKeyPressedOnce( m_InputData ) )	// char-elesis CH1p: June 32b85470 @32b8551f
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
		true == m_sCurrentSkillKeyManager.IsPressedCurrentSkillKey(m_InputData) )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	{ 
		// MP체크
		if ( GetNowMp() < m_fMPConsume )
		{
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == false)
			{
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1),
					D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
			}
#else
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1),
				D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
#endif ALWAYS_SCREEN_SHOT_TEST
		}
		else
		{
			if( true == m_InputData.pureRight )
				m_FrameDataFuture.syncData.bIsRight = true;
			else if( true == m_InputData.pureLeft )
				m_FrameDataFuture.syncData.bIsRight = false;
			UpNowMp( -m_fMPConsume );

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
			if ( m_iRushingSwordAddLoopValue >= m_iRushingSwordNowLoopValue )
			{
				StateChange( ESI_SI_A_EL_RUSHING_SWORD_2nd );
			}
			else
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
			{
				StateChange( ESI_SI_A_EL_RUSHING_SWORD_3rd );
			}
		}
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
void CX2GUEL::ESI_SI_A_EL_RUSHING_SWORD_3rd_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2b2: June 32b57c60
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

		CommonEventProcess();
}
// 발차기
void CX2GUEL::ESI_SI_A_EL_KICK_Start()
{
	CommonStateStart();
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_EL_KICK );
	if( NULL != pSkillTemplet )
	{
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
		const int iSkillLevel = userSkillTree.GetSkillLevel( pSkillTemplet->m_eID );

		m_DamageData.fForceDownValue = -pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_ABS, iSkillLevel );

#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELESIS_MEMO3 ) == true )
			m_DamageData.fForceDownValue *= 1.1f;
#endif //ADD_MEMO_1ST_CLASS
	}
}
void CX2GUEL::ESI_SI_A_EL_KICK_Init()
{
	TextureReadyInBackground( L"ColorBallGray.dds" );
	TextureReadyInBackground( L"Condense_Pulse01.dds" );
}
void CX2GUEL::ESI_SI_A_EL_KICK_EventProcess()
{	
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
// 메가 슬래시
void CX2GUEL::ESI_SI_A_EL_MEGA_SLASH_Init()
{
	TextureReadyInBackground(L"EL_FIRE_01.dds");
	TextureReadyInBackground(L"EL_FIRE_02.dds");
	TextureReadyInBackground(L"EL_FIRE_04.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Explosion_Light01.dds");
	TextureReadyInBackground(L"Flame_REVOLVER.dds");
	TextureReadyInBackground(L"SA_EL_MEGA_SLASH_FIRE02.dds");
	TextureReadyInBackground(L"Whitecircle03.dds");
	XSkinMeshReadyInBackground(L"Mega_Slash_Fire_Dummy.x");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
}
void CX2GUEL::ESI_SI_A_EL_MEGA_SLASH_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
// 소닉 어썰트 - 스탭
void CX2GUEL::ESI_SI_A_EL_SONIC_ASSAULT_STAB_Init()
{
	TextureReadyInBackground(L"EL_FIRE_01.dds");
	TextureReadyInBackground(L"EL_FIRE_02.dds");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_04.dds");
	TextureReadyInBackground(L"EL_FIRE_07.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Explosion_Light01.dds");
	TextureReadyInBackground(L"Flame_REVOLVER.dds");
	TextureReadyInBackground(L"SA_EL_MEGA_SLASH_FIRE02.dds");
	TextureReadyInBackground(L"Whitecircle03.dds");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
}
void CX2GUEL::ESI_SI_A_EL_SONIC_ASSAULT_STAB_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
// 소닉 어썰트 - 스팅
void CX2GUEL::ESI_SI_A_EL_SONIC_ASSAULT_STING_Init()
{
	TextureReadyInBackground(L"EL_FIRE_01.dds");
	TextureReadyInBackground(L"EL_FIRE_02.dds");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_04.dds");
	TextureReadyInBackground(L"EL_FIRE_07.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Explosion_Light01.dds");
	TextureReadyInBackground(L"Flame_REVOLVER.dds");
	TextureReadyInBackground(L"SA_EL_MEGA_SLASH_FIRE02.dds");
	TextureReadyInBackground(L"Whitecircle03.dds");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	TextureReadyInBackground(L"Particle_Blur.dds");	// char-elesis CH2a: June 32b4f740
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
void CX2GUEL::ESI_SI_A_EL_SONIC_ASSAULT_STING_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2a: June 32b58b70
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}
// 와일드 쇼크
void CX2GUEL::ESI_SI_SA_EL_WILD_SHOCK_Init()
{
	TextureReadyInBackground(L"EL_FIRE_07.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Explosion_Light01.dds");
	TextureReadyInBackground(L"Flame_REVOLVER.dds");
	TextureReadyInBackground(L"Whitecircle03.dds");
	TextureReadyInBackground(L"EL_FIRE_01.dds");
	TextureReadyInBackground(L"EL_FIRE_02.dds");
	TextureReadyInBackground(L"EL_FIRE_04.dds");
	TextureReadyInBackground(L"SA_EL_MEGA_SLASH_FIRE02.dds");
	TextureReadyInBackground(L"SA_EL_Wild_Shock_WAVE.tga");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	XSkinMeshReadyInBackground(L"SI_SA_EL_WILD_SHOCK_ATTACKBOX.X");
}
void CX2GUEL::ESI_SI_SA_EL_WILD_SHOCK_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2a: June 32b58ea0
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}
// 파워 버스터
void CX2GUEL::ESI_SI_SA_EL_POWER_BURSTER_Init()
{
	TextureReadyInBackground(L"EL_FIRE_01.dds");
	TextureReadyInBackground(L"EL_FIRE_02.dds");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_07.dds");
	TextureReadyInBackground(L"EL_FIRE_10.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Explosion_Light01.dds");
	TextureReadyInBackground(L"Whitecircle03.dds");
	TextureReadyInBackground(L"Flame_REVOLVER.dds");
	TextureReadyInBackground(L"Particle_Blur.DDS");
	TextureReadyInBackground(L"SA_EL_MEGA_SLASH_FIRE02.dds");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	XSkinMeshReadyInBackground(L"SA_EL_POWER_BURSTER_FIRE01.X");
	XSkinMeshReadyInBackground(L"Effect_ElSword_FlameGagerLand.X");
	XSkinMeshReadyInBackground(L"Elsword_SI_SA_Phoenix_Talon_Mesh02.X");
}
void CX2GUEL::ESI_SI_SA_EL_POWER_BURSTER_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
// 메가 버스터
void CX2GUEL::ESI_SI_SA_EL_MEGA_BURSTER_Init()			
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	// char-elesis CH2a: June 32b4fa60 ( 38 preloads, March 21 )
	TextureReadyInBackground( L"AeroTornado04.dds" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
	TextureReadyInBackground( L"EL_FIRE_01.dds" );
	TextureReadyInBackground( L"EL_FIRE_02.dds" );
	TextureReadyInBackground( L"EL_FIRE_03.dds" );
	TextureReadyInBackground( L"EL_FIRE_04.dds" );
	TextureReadyInBackground( L"EL_FIRE_07.dds" );
	TextureReadyInBackground( L"EL_FIRE_10.dds" );
	TextureReadyInBackground( L"Flame_REVOLVER.dds" );
	TextureReadyInBackground( L"Explosion_Light02.dds" );
	TextureReadyInBackground( L"GroundShockWave02.dds" );
	TextureReadyInBackground( L"SA_EL_MEGA_SLASH_FIRE02.dds" );
	TextureReadyInBackground( L"Particle_Blur.DDS" );
	XSkinMeshReadyInBackground( L"Light_Sigong_Box_Open01.X" );
	XSkinMeshReadyInBackground( L"Mega_Slash_Trace_Dummy.x" );
	XSkinMeshReadyInBackground( L"NUI_CUTTYSARK_Dying_Landing_Effect_Hole.X" );
	XSkinMeshReadyInBackground( L"NUI_CUTTYSARK_Dying_Landing_Effect_Rock.X" );
	XSkinMeshReadyInBackground( L"Elsword_SI_SA_Phoenix_Talon_Mesh02.X" );
	XSkinMeshReadyInBackground( L"SA_EL_MEGA_BURSTER_FIRE_CIRCLE.X" );
	XSkinMeshReadyInBackground( L"SA_EL_MEGA_BURSTER_FIRE_Earth.X" );
	XSkinMeshReadyInBackground( L"CDC_AS_FIRE_EX_WIND01.x" );
	XSkinMeshReadyInBackground( L"Effect_ElSword_FlameGagerLand.X" );
	XSkinMeshReadyInBackground( L"SA_SPK_SWORDFIRE_PROJECTILE_ATTACKBOX.x" );
	XSkinMeshReadyInBackground( L"SI_SA_SBH_WALL_OF_PROMINENCE_01.X" );
	XMeshReadyInBackground( L"SA_SPK_BURST_WAVE_FIRE.Y" );
	TextureReadyInBackground( L"EL_FIRE_03.dds" );
	TextureReadyInBackground( L"GroundShockWave02.dds" );
	TextureReadyInBackground( L"elsword_SA_rageCutter_smogPoint.dds" );
	TextureReadyInBackground( L"EL_FIRE_04.dds" );
	TextureReadyInBackground( L"smoke02.dds" );
	TextureReadyInBackground( L"ALS_flame_002.dds" );
	TextureReadyInBackground( L"Arme_Critical.dds" );
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );
	TextureReadyInBackground( L"Arme_Damage_Ring.dds" );
	TextureReadyInBackground( L"star2.dds" );
	XMeshReadyInBackground( L"ADD_Effect_Ground_Explosion_01.Y" );
	XMeshReadyInBackground( L"LCG_M_Beam04.Y" );
	XSkinMeshReadyInBackground( L"RAINCOAT_CAT_DummyAttackBox_Up3_50x50x50.X" );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	TextureReadyInBackground(L"AeroTornado04.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"EL_FIRE_01.dds");
	TextureReadyInBackground(L"EL_FIRE_02.dds");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_04.dds");
	TextureReadyInBackground(L"EL_FIRE_07.dds");
	TextureReadyInBackground(L"EL_FIRE_10.dds");
	TextureReadyInBackground(L"Flame_REVOLVER.dds");
	TextureReadyInBackground(L"Explosion_Light02.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	TextureReadyInBackground(L"SA_EL_MEGA_SLASH_FIRE02.dds");
	TextureReadyInBackground(L"Particle_Blur.DDS");
	TextureReadyInBackground(L"Particle_Blur.dds");
	XSkinMeshReadyInBackground(L"Light_Sigong_Box_Open01.X");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	XSkinMeshReadyInBackground(L"NUI_CUTTYSARK_Dying_Landing_Effect_Hole.X");
	XSkinMeshReadyInBackground(L"NUI_CUTTYSARK_Dying_Landing_Effect_Rock.X");
	XSkinMeshReadyInBackground(L"Elsword_SI_SA_Phoenix_Talon_Mesh02.X");
	XSkinMeshReadyInBackground(L"SA_EL_MEGA_BURSTER_FIRE_CIRCLE.X");
	XSkinMeshReadyInBackground(L"SA_EL_MEGA_BURSTER_FIRE_Earth.X");
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
void CX2GUEL::ESI_SI_SA_EL_MEGA_BURSTER_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2a: June 32b594a0
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}
// 언리미티드 블레이드
void CX2GUEL::ESI_SI_SA_EL_UNLIMITED_BLADE_Init()
{
	TextureReadyInBackground(L"AeroTornado04.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"COMET_CRASHER_Explosion_02.tga");
	TextureReadyInBackground(L"ColorBallFire.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Explosion_Light01.dds");
	TextureReadyInBackground(L"Explosion_Light02.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	TextureReadyInBackground(L"Mesh_Raven_Event_AC_Upbody21_Effect.tga");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"Smoke.dds");
	TextureReadyInBackground(L"Whitecircle03.dds");
	XSkinMeshReadyInBackground(L"Effect_ElSword_FlameGagerLand.X");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	XSkinMeshReadyInBackground(L"elsword_SA_SwordBlasting_circle.X");
}
void CX2GUEL::ESI_SI_SA_EL_UNLIMITED_BLADE_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2a: June 32b597a0
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}
// 극기 - 강(强)
void CX2GUEL::ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_Init()
{
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"ColorBallGray.dds");
	XSkinMeshReadyInBackground(L"HummingWind_AttackBox.X");
}
void CX2GUEL::ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_EventProcess()
{
	// 극기 상태에서 피격시 활성화 되는 스테이트 
 	if( false == IsOnSomethingFuture() )
 	{
#ifdef ADD_CAN_NOT_ATTACK_JUMP_DOWN
		StateChange( ESI_CAN_NOT_ATTACK_JUMP_DOWN );
#else
		StateChange( GetJumpDownStateID() );
#endif // ADD_CAN_NOT_ATTACK_JUMP_DOWN
 		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
 	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( GetWaitStateID() );
	}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1k: June 32b59aa0 ( the walk / back-walk / dash / ZX cancels also test the line )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE

	CommonEventProcess();
}
void CX2GUEL::ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_StateStart()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1k: June 32b80840 ( Endurance Power and its memo state ): the effect set and the Punish buff factor are new
	m_fGroggyTime = 0.f;
	m_PhysicParam.nowSpeed.x = 0.0f;
	m_PhysicParam.nowSpeed.y = 0.0f;

	CureExtraDamage( CX2DamageManager::EDT_ENCHANT_SHOCK );
	CureExtraDamage( CX2DamageManager::EDT_FROZEN );
	CureExtraDamage( CX2DamageManager::EDT_ENTANGLE );
	CureExtraDamage( CX2DamageManager::EDT_DRYAD );
	CureExtraDamage( CX2DamageManager::EDT_STUN );
	CureExtraDamage( CX2DamageManager::EDT_Y_PRESSED );
	CureExtraDamage( CX2DamageManager::EDT_PANIC );

	EraseBuffTempletFromGameUnit( BTI_DEBUFF_STUN );
	EraseBuffTempletFromGameUnit( BTI_DEBUFF_PANIC );
	EraseBuffTempletFromGameUnit( BTI_DEBUFF_COLD );
	EraseBuffTempletFromGameUnit( BTI_DEBUFF_CONFUSION );
	EraseBuffTempletFromGameUnit( BTI_BUFF_ENDURANCE_POWER );

	if ( 0.f < m_fPunishEffectPowerRate )
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_EL_ENDURANCE_POWER_PUNISH", this, NULL, IsHyperState(), m_fPunishEffectPowerRate );

		const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::RNSI_A_EL_ENDURANCE_POWER, false, 0, true );
		const bool bPvpOrPve = ( NULL != g_pX2Game && true == g_pX2Game->GetIsNowPVPCorrectionGame() );	// June 322cc1a0: PVP or PVE
		SetBuffFactorToGameUnitByBuffFactorID( ( true == bPvpOrPve ) ? BFI_BUFF_PUNISH_PVP : BFI_BUFF_PUNISH, iSkillLevel, NULL, 0, false );
	}
	else
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_EL_ENDURANCE_POWER", this );
	}

	CommonStateStart();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_fGroggyTime = 0.f;
	m_PhysicParam.nowSpeed.x = 0.0f;
	m_PhysicParam.nowSpeed.y = 0.0f;

	CureExtraDamage( CX2DamageManager::EDT_ENCHANT_SHOCK );
	CureExtraDamage( CX2DamageManager::EDT_FROZEN );
	CureExtraDamage( CX2DamageManager::EDT_ENTANGLE );
	CureExtraDamage( CX2DamageManager::EDT_DRYAD );
	CureExtraDamage( CX2DamageManager::EDT_STUN );
	CureExtraDamage( CX2DamageManager::EDT_Y_PRESSED );
	CureExtraDamage( CX2DamageManager::EDT_PANIC );

	EraseBuffTempletFromGameUnit(BTI_DEBUFF_STUN);	
	EraseBuffTempletFromGameUnit(BTI_DEBUFF_PANIC);	
	EraseBuffTempletFromGameUnit(BTI_DEBUFF_COLD);	
	EraseBuffTempletFromGameUnit(BTI_DEBUFF_CONFUSION);	
//{{ Iruha : 2026-10-03 // June buff port Phase 2 (JUNE_BUFF_PORT_PHASES.md)
#ifdef SERV_IRUHADEV_JUNE_BUFF_ENUM
	EraseBuffTempletFromGameUnit(BTI_BUFF_ENDURANCE_POWER);
#else
	EraseBuffTempletFromGameUnit(BTI_BUFF_INDURANCE_POWER);	
#endif SERV_IRUHADEV_JUNE_BUFF_ENUM
//}}

	CommonStateStart();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
/** @function : DoStateEndurancePowerRage
	@brief : 극기, 섬멸에 의한 슈아 팅 스테이트로 전환			
			극기에 의한 스테이트 전환 시는 메모 적용 여부를 검사해야 함
*/
void CX2GUEL::DoStateEndurancePowerRage( bool bIsWayOfSword_ )
{
	if( true == bIsWayOfSword_ )
		StateChange( ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE, false );
	else
	{
#ifdef ADD_MEMO_1ST_CLASS
		// 메모 보유 시 
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELESIS_MEMO9 ) == true )
		{
			StateChange( ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_MEMO, false );
		}
		else
#endif // ADD_MEMO_1ST_CLASS
			StateChange( ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE, false );
	}
}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
// char-elesis CH1k: June 32b75c90 ( script id 207, the state DetonationStateChange enters; CH1m registers it ): the same reset as 205 / 206 and the plain effect set
void CX2GUEL::ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_DETONATION_StateStart()
{
	m_fGroggyTime = 0.f;
	m_PhysicParam.nowSpeed.x = 0.0f;
	m_PhysicParam.nowSpeed.y = 0.0f;

	CureExtraDamage( CX2DamageManager::EDT_ENCHANT_SHOCK );
	CureExtraDamage( CX2DamageManager::EDT_FROZEN );
	CureExtraDamage( CX2DamageManager::EDT_ENTANGLE );
	CureExtraDamage( CX2DamageManager::EDT_DRYAD );
	CureExtraDamage( CX2DamageManager::EDT_STUN );
	CureExtraDamage( CX2DamageManager::EDT_Y_PRESSED );
	CureExtraDamage( CX2DamageManager::EDT_PANIC );

	EraseBuffTempletFromGameUnit( BTI_DEBUFF_STUN );
	EraseBuffTempletFromGameUnit( BTI_DEBUFF_PANIC );
	EraseBuffTempletFromGameUnit( BTI_DEBUFF_COLD );
	EraseBuffTempletFromGameUnit( BTI_DEBUFF_CONFUSION );
	EraseBuffTempletFromGameUnit( BTI_BUFF_ENDURANCE_POWER );

	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_EL_ENDURANCE_POWER", this );

	CommonStateStart();
}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
void CX2GUEL::ProcessEndurancePowerSkill()
{	// 극기 - 강(强)
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1k: June 32b80c60: the buff factor by id and level ( the PvP-or-PvE variant in a PvP or PvE game )
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_EL_ENDURANCE_POWER, true );
	if ( NULL == pSkillTemplet )
		return;

	const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( pSkillTemplet->m_eID, false, 0, true );
	const bool bPvpOrPve = ( NULL != g_pX2Game && true == g_pX2Game->GetIsNowPVPCorrectionGame() );	// June 322cc1a0: PVP or PVE
	SetBuffFactorToGameUnitByBuffFactorID( ( true == bPvpOrPve ) ? BFI_BUFF_ENDURANCE_POWER_PVP : BFI_BUFF_ENDURANCE_POWER, iSkillLevel, NULL, 0, false );

	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_ENDURANCE", this );
	PlaySound( L"Elsword_Endurance.ogg" );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_EL_ENDURANCE_POWER );
	if( NULL != pSkillTemplet )
	{
		SetBuffFactorToGameUnit( pSkillTemplet, 0 );

		// 스킬 지속 시간 중 재 사용 했을 때 처리를 위해, 
		// 이펙트셋은 버프로 넣지 않고 따로 생성 함.
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_ENDURANCE", this ); 
		PlaySound( L"Elsword_Endurance.ogg" );	
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 스킬 스테이트 함수 - 1-1차 세이버 나이트
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CX2GUEL::ESI_DASH_COMBO_SSK_ZZ_EventProcess()			/// DASH_COMBO_ZZ
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1e: June 32b59e30
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneZ, ESI_DASH_COMBO_SSK_ZZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		true == m_InputData.oneX, ESI_DASH_COMBO_SSK_ZZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, true == m_InputData.oneZ, ESI_DASH_COMBO_SSK_ZZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, true == m_InputData.oneX, ESI_DASH_COMBO_SSK_ZZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ESI_DASH_COMBO_SSK_ZZZ_EventProcess()			/// DASH_COMBO_ZZZ
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1e: June 32b5a2a0
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ESI_DASH_COMBO_SSK_ZZX_EventProcess()			/// DASH_COMBO_ZZX
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1e: June 32b5a5a0
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
/** @function : ESI_DASH_JUMP_COMBO_SSK_XX_EventProcess
	@brief : 점프 상태에서 내려오면서 입력을 받고,
			 랜딩에서 바로 스테이트 변경시키기 위한 작업
*/
void CX2GUEL::ESI_DASH_JUMP_COMBO_SSK_XX_EventProcess()		/// DASH_JUMP_COMBO_XX
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1e: June 32b5a8a0 ( one body for this and ESI_DASH_JUMP_COMBO_SSK_ZZX; June has no cancels here )
	bool& bESI_DASH_JUMP_COMBO_SSK_XXX = m_FrameDataFuture.stateParam.bEventFlagList[0];
	bool& bESI_COMBO_Z = m_FrameDataFuture.stateParam.bEventFlagList[1];

	if( true == m_InputData.oneX )
		bESI_DASH_JUMP_COMBO_SSK_XXX = true;
	else if ( true == m_InputData.oneZ )
		bESI_COMBO_Z = true;

	if ( true == IsOnSomethingFuture() )
	{
		if( true == bESI_COMBO_Z )
			StateChange( ESI_COMBO_Z );
		else if( true == bESI_DASH_JUMP_COMBO_SSK_XXX )
			StateChange( ESI_DASH_JUMP_COMBO_SSK_XXX );
		else
			StateChange( ESI_DASH_JUMP_COMBO_XX_LANDING );
	}
	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	bool& bESI_DASH_JUMP_COMBO_SSK_XXX = m_FrameDataFuture.stateParam.bEventFlagList[0];
	bool& bESI_COMBO_Z = m_FrameDataFuture.stateParam.bEventFlagList[1];

	if( true == m_InputData.oneX )
		bESI_DASH_JUMP_COMBO_SSK_XXX = true;
	else if ( true == m_InputData.oneZ )
		bESI_COMBO_Z = true;

	if ( true == IsOnSomethingFuture() )
	{
		if( true == bESI_COMBO_Z )
			StateChange( ESI_COMBO_Z );
		else if( true == bESI_DASH_JUMP_COMBO_SSK_XXX )
			StateChange( ESI_DASH_JUMP_COMBO_SSK_XXX );
		else
			StateChange( ESI_DASH_JUMP_COMBO_XX_LANDING );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ESI_DASH_JUMP_COMBO_SSK_XXX_EventProcess()		/// DASH_JUMP_COMBO_XXX
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1e: June 32b5a910
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

		CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
// 크로스 슬래시
void CX2GUEL::ESI_SI_A_SSK_CROSS_SLASH_Init()
{
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"Mesh_Raven_Event_AC_Upbody21_Effect.tga");
	TextureReadyInBackground(L"Particle_Blur.DDS");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"WhitePoint.dds");
	TextureReadyInBackground(L"Whitecircle02.dds");
	TextureReadyInBackground(L"title_StarLight_Smash_spark.dds");
	XSkinMeshReadyInBackground(L"SA_SSK_EXTINCTION_END_FX_Blade.X");
	XSkinMeshReadyInBackground(L"Lire_SI_A_Low_Kick_Mesh02.X");
	XSkinMeshReadyInBackground(L"AirSlash02.X");
}
void CX2GUEL::ESI_SI_A_SSK_CROSS_SLASH_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}

// 파워 브레이크
void CX2GUEL::ESI_SI_A_SSK_POWER_BREAK_Init()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	TextureReadyInBackground(L"Arme_Ring2.dds");	// char-elesis CH2b2: June 32b4fdc0 ( 13 more names than March )
	TextureReadyInBackground(L"ColorBallFire.dds");
	TextureReadyInBackground(L"Arme_Critical.dds");
	TextureReadyInBackground(L"Arme_Damage_Center.dds");
	TextureReadyInBackground(L"Arme_Damage_Ring.dds");
	TextureReadyInBackground(L"Armor_Crack.dds");
	TextureReadyInBackground(L"HA_RRF_Reckless_Fiste_FX_Map01.tga");
	TextureReadyInBackground(L"PET_FIREFOX_MIHOEffect02.tga");
	TextureReadyInBackground(L"star2.dds");
	TextureReadyInBackground(L"WhitePoint.dds");
	TextureReadyInBackground(L"WhitePointSmall.dds");
	XMeshReadyInBackground(L"ADE_PE01.Y");
	XMeshReadyInBackground(L"ALS_SHAKE_CHAOS_02.Y");
	XSkinMeshReadyInBackground(L"aisha_SA_SCREWDRIVER_circle.X");
	XSkinMeshReadyInBackground(L"SI_A_SBH_SCALET_ROSE_attackSphere.X");
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"ColorBallFire.dds");
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
void CX2GUEL::ESI_SI_A_SSK_POWER_BREAK_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2b2: June 32b5af20
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}
// 헤비 스터너
void CX2GUEL::ESI_SI_A_SSK_HEAVY_STUNNER_Init()
{
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"ColorBallBlue.dds");
}
void CX2GUEL::ESI_SI_A_SSK_HEAVY_STUNNER_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
// 스파이럴 블래스트
void CX2GUEL::ESI_SI_SA_SSK_SPIRAL_BLAST_Init()	
{
	TextureReadyInBackground(L"ColorBallFire.dds");
	TextureReadyInBackground(L"EL_FIRE_01.dds");
	TextureReadyInBackground(L"EL_FIRE_02.dds");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_04.dds");
	TextureReadyInBackground(L"EL_FIRE_06.dds");
	TextureReadyInBackground(L"Flame_REVOLVER.dds");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	TextureReadyInBackground(L"Mesh_Elsword_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"SA_EL_MEGA_SLASH_FIRE02.dds");
	XSkinMeshReadyInBackground(L"SA_EL_SA_SSK_SPIRAL_BLAST_01.X");
	XSkinMeshReadyInBackground(L"SA_EL_SA_SSK_SPIRAL_BLAST_03.X");
	XSkinMeshReadyInBackground(L"SpiralBlast01.X");
}
void CX2GUEL::ESI_SI_SA_SSK_SPIRAL_BLAST_EventProcess()
{	
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2a: June 32b5b520
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}
// 세이버 - 익스팅션
#pragma region 세이버 - 익스팅션
void CX2GUEL::InitExtinction()
{
	// 충전 지속 시간 초기화
	m_fExtinctionRemainTime = 0.f;

	// 발사 각도 초기화
	m_fExtinctionDegreeZ = _CONST_EL_::BASE_EXTINCTION_DEGREE_Z;
}
void CX2GUEL::CreateExctionUIEffect()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		// 각도 조절 상태를 알려주기 위한 UI 이펙트 
		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hExctionBG )
		{
			// 각도기 배경UI 생성
			wstring wstrParticleName = L"SA_SSK_EXTINCTION_CHARGE_P14_";
			wstrParticleName += ( m_FrameDataFuture.syncData.bIsRight == true ) ? L"R" : L"L";
#ifdef ADD_MEMO_1ST_CLASS //김창한
			if( GetEqippedSkillMemo(CX2SkillTree::SMI_ELESIS_MEMO6) == true )
				wstrParticleName += L"_MEMO";
#endif //ADD_MEMO_1ST_CLASS
			m_hExctionBG = g_pData->GetGameMajorParticle()->CreateSequenceHandle( this,  wstrParticleName.c_str(), GetBonePos(L"Bip01") );	

			CKTDGParticleSystem::CParticleEventSequence* pSeq = 
				g_pData->GetGameMajorParticle()->GetInstanceSequence( m_hExctionBG );
			if( NULL != pSeq )
			{
				pSeq->CreateNewParticle(GetBonePos(L"Bip01"));
			}
		}
		// 각도 조절 상태를 알려주기 위한 UI 이펙트 
		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hExtinctionArrow )
		{
			// 각도기 UI 생성
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetGameMajorParticle()->CreateSequence( this,  L"SA_SSK_EXTINCTION_CHARGE_P15", GetBonePos(L"Bip01") );
            if ( pSeq != NULL )
            {
                m_hExtinctionArrow = pSeq->GetHandle();
                m_hExtinctionArrowParticle = pSeq->CreateNewParticleHandle(GetBonePos(L"Bip01"));
            }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_hExtinctionArrow = g_pData->GetGameMajorParticle()->CreateSequenceHandle( this,  L"SA_SSK_EXTINCTION_CHARGE_P15", GetBonePos(L"Bip01") );	
			if( m_pExtinctionArrow == NULL )
			{
				CKTDGParticleSystem::CParticleEventSequence* pSeq = 
					g_pData->GetGameMajorParticle()->GetInstanceSequence( m_hExtinctionArrow );
				if( NULL != pSeq )
				{
					m_pExtinctionArrow = pSeq->CreateNewParticle(GetBonePos(L"Bip01"));
				}
			}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		}
	}
}
void CX2GUEL::DestoryExctionUIEffect()
{
	g_pData->GetGameMajorParticle()->DestroyInstanceHandle( m_hExtinctionArrow );
	g_pData->GetGameMajorParticle()->DestroyInstanceHandle( m_hExctionBG );
	m_hExtinctionArrow = INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hExctionBG = INVALID_PARTICLE_SEQUENCE_HANDLE;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hExtinctionArrowParticle = INVALID_PARTICLE_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pExtinctionArrow = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}
void CX2GUEL::Extinction_FrameMoveFuture()
{
	CreateExctionUIEffect();
	// 각도 조절 상태를 알려주기 위한 UI 이펙트의 각도 조절
    CKTDGParticleSystem::CParticle* pParticle = NULL;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    pParticle = g_pX2Game->GetMajorParticle()->ValidateParticleHandle( m_hExtinctionArrowParticle );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    pParticle = m_pExtinctionArrow;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( NULL == pParticle )
		m_hExtinctionArrowParticle = INVALID_PARTICLE_HANDLE;	// char-elesis CH2b: June 32b75e80 @32b75ec5
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	if( NULL != pParticle )
	{
		D3DXVECTOR3 vRot(0, (m_FrameDataFuture.syncData.bIsRight == true ) ? 0.f : 180.f ,m_fExtinctionDegreeZ );
		pParticle->SetAxisRotateDegree( vRot );
		pParticle->SetRotate( vRot );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGParticleSystem::CParticleEventSequence* pSeq = pParticle->GetMasterSequence();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetGameMajorParticle()->GetInstanceSequence( m_hExtinctionArrow );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( NULL != pSeq )
		{
			pSeq->SetAddRotate( vRot );
			pSeq->SetAxisAngle( vRot );
		}

	}
}
// 세이버 - 익스팅션
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_START_Init()				
{
	TextureReadyInBackground(L"ColorBallFire.dds");
	TextureReadyInBackground(L"Mesh_Raven_Event_AC_Upbody21_Effect.tga");
	TextureReadyInBackground(L"Condense_Light01.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Explosion_Light01.dds");
	TextureReadyInBackground(L"Lire_Kick_Impact01.dds");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"WhitePoint.dds");
	TextureReadyInBackground(L"Whitecircle03.dds");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	XSkinMeshReadyInBackground(L"EXTINCTION_ATTACK_DUMMY.X");
	XSkinMeshReadyInBackground(L"SA_SSK_EXTINCTION_END_FX_Blade.X");
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	TextureReadyInBackground(L"ADD_CENTER_LIGHT01.dds");	// char-elesis CH2b: June 32b4ff80
	TextureReadyInBackground(L"ALS_SLICER_01.dds");
	TextureReadyInBackground(L"Arme_Critical.dds");
	TextureReadyInBackground(L"Arme_Damage_Center.dds");
	TextureReadyInBackground(L"Arme_Damage_Ring.dds");
	TextureReadyInBackground(L"star2.dds");
	XMeshReadyInBackground(L"SA_SSK_EXTINCTION_AIM_ARROW.Y");
	XMeshReadyInBackground(L"SSK_Extinction_Projectile01.Y");
	XMeshReadyInBackground(L"SSK_Extinction_Projectile02.Y");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_START_Start()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	m_fExtinctionRemainTime = 0.f;	// char-elesis CH2b: June 32b500a0 ( the aim starts level; March's base angle is -30 )
	m_fExtinctionDegreeZ = 0.f;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	InitExtinction();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	CommonStateStart();
}
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_START_StartFuture()
{
	// 익스팅션 충전 완료를 체크하기 위한 스킬 키 설정
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_kSkillSlotKey.Set( m_iNowSpecialAttack - 1 );	// char-elesis CH1p: June 32b5b870 @32b5b883
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_sCurrentSkillKeyManager.InitCurrentSkillKeyManager( m_iNowSpecialAttack - 1 );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	
#ifdef FIX_NOT_DRAWING_EXTINCTION_GUIDE_UI
	DestoryExctionUIEffect();	
#endif // FIX_NOT_DRAWING_EXTINCTION_GUIDE_UI

	CommonStateStartFuture();
}
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_START_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_SI_SA_SSK_EXTINCTION_CHARGE );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2b: June 32b5b8e0
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	CommonEventProcess();
}
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_CHARGE_FrameMove()
{
	m_fExtinctionRemainTime += m_fElapsedTime;

	CommonFrameMove();
}
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_CHARGE_FrameMoveFuture()
{
	// 이펙트 각도 조절
	Extinction_FrameMoveFuture();

	CommonFrameMoveFuture();
}
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_CHARGE_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_kSkillSlotKey.FollowSlotSwap();	// char-elesis CH1p: June 32b5bbe0 @32b5bbeb
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_sCurrentSkillKeyManager.CheckSkillSlotSwap();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE

	// 최대 충전 시간 초과 시 공격 모션으로 변경
	if( m_fExtinctionRemainTime >= _CONST_EL_::MAX_EXTINCTION_CHARGE_TIME )
	{
		StateChange( ESI_SI_SA_SSK_EXTINCTION_ATTACK );		
	}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	else if( true == m_kSkillSlotKey.IsKeyReleased( m_InputData ) )	// char-elesis CH1p: June 32b5bbe0 @32b5bc22
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	else if( true == m_sCurrentSkillKeyManager.IsUnpressedCurrentSkillKey( m_InputData ) )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	{
		StateChange( ESI_SI_SA_SSK_EXTINCTION_ATTACK );
	}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,	// char-elesis CH2b: June 32b5bbe0 @32b5bc40
		true == m_InputData.pureUp, ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.pureDown, ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, true == m_InputData.pureUp, ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 0, true == m_InputData.pureDown, ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2b: June 32b5bbe0
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}
/// 세이버 - 익스팅션 - Attack
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_ATTACK_FrameMove()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	// char-elesis CH2b: June 32b75f90
	if ( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
	{
		D3DXVECTOR3 vDamageScale( 1.f, 1.f, 1.f );
		if ( true == m_bIsApplyVictoriousSword )
		{
			const float fVictoriousSwordScale = ( true == HaveThisBuff( BTI_BUFF_SGM_VICTORIOUS_SWORD_UPGRADE ) )
				? EXTINCTION_ATTACK_SCALE_VICTORIOUS_SWORD_UPGRADE : EXTINCTION_ATTACK_SCALE_VICTORIOUS_SWORD;
			vDamageScale = D3DXVECTOR3( fVictoriousSwordScale, fVictoriousSwordScale, fVictoriousSwordScale );
		}
		float fEffectLifeTime = -1.f;
		if ( true == IsNowStateSkillUpgraded() )
		{
			vDamageScale *= GetLearnedSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE, GetNowStateSkillID() );
			fEffectLifeTime = GetLearnedSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, GetNowStateSkillID() );
		}
		const CX2EffectSet::Handle hEffect = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_SSK_EXTINCTION_DAMAGE", this, NULL,
			IsHyperState(), GetPowerRate(), fEffectLifeTime );
		if ( INVALID_EFFECTSET_HANDLE != hEffect )
		{
			if ( CX2EffectSet::EffectSetInstance* pEffectSet = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hEffect ) )
			{
				pEffectSet->SetEffectScale( vDamageScale, true );
				pEffectSet->SetLocalRotateDegree( D3DXVECTOR3( 0.f, 0.f, m_fExtinctionDegreeZ ) );
			}
		}
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 익스팅션 차지 데미지 삭제
		CX2EffectSet::Handle hEffect = 
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_SSK_EXTINCTION_DAMAGE", this ,
			NULL, IsHyperState(), GetPowerRate() );

#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 충전 시간에 따라 공격력 최대 20%증가.
		m_fExtinctionRemainTime = max( m_fExtinctionRemainTime, 0.f );

		float fMultiplePowerRateRel = ( min( m_fExtinctionRemainTime, 1.f ) * _CONST_EL_::LIMIT_EXTINCTION_POWER_RATE ) + 1.f;

		CX2EffectSet::Handle hEffect = 
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_SSK_EXTINCTION_DAMAGE", this ,
														NULL, IsHyperState(), GetPowerRate() * fMultiplePowerRateRel );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	

		if( INVALID_EFFECTSET_HANDLE != hEffect )
		{
			if ( CX2EffectSet::EffectSetInstance* pEffectSet = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hEffect ) )
			{	// 키 입력을 통해 조절한 각도로 이펙트 생성
				pEffectSet->SetLocalRotateDegree( D3DXVECTOR3(0.f, 0.f, m_fExtinctionDegreeZ ) ) ;
			}
		}
	}

#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	CommonFrameMove();
}
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_ATTACK_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() &&
		true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
	else if( false == IsOnSomethingFuture() )
	{
		// 현재 진행 방향 쪽으로 키보드를 누르면 앞으로 좀 더 나아감
		if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
			( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
		{
			m_PhysicParam.nowSpeed.x = GetWalkSpeed();
		}
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2b: June 32b6ee00
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_ATTACK_StartFuture() 
{
	DestoryExctionUIEffect();
	CommonStateStartFuture();
}
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP_FrameMove()	// 각도 조절
{
	m_fExtinctionRemainTime += m_fElapsedTime;

#ifdef ADD_MEMO_1ST_CLASS //김창한
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELESIS_MEMO6) == true )
	{
		if( m_fExtinctionDegreeZ < _CONST_EL_::MAX_EXTINCTION_DEGREE_Z + 30.f )
			m_fExtinctionDegreeZ += m_fElapsedTime * _CONST_EL_::EXTINCTION_DEGREE_Z_CONTROL;
		else
			m_fExtinctionDegreeZ = _CONST_EL_::MAX_EXTINCTION_DEGREE_Z + 30.f;
	}
	else
#endif //ADD_MEMO_1ST_CLASS
	{
		if( m_fExtinctionDegreeZ < _CONST_EL_::MAX_EXTINCTION_DEGREE_Z )
			m_fExtinctionDegreeZ += m_fElapsedTime * _CONST_EL_::EXTINCTION_DEGREE_Z_CONTROL;
		else
			m_fExtinctionDegreeZ = _CONST_EL_::MAX_EXTINCTION_DEGREE_Z;
	}

	CommonFrameMove();
}
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_kSkillSlotKey.FollowSlotSwap();	// char-elesis CH1p: June 32b5c0c0 @32b5c0cb
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_sCurrentSkillKeyManager.CheckSkillSlotSwap();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE

	// 최대 충전 시간 초과 시 공격 모션으로 변경
	if( m_fExtinctionRemainTime >= _CONST_EL_::MAX_EXTINCTION_CHARGE_TIME )
	{
		StateChange( ESI_SI_SA_SSK_EXTINCTION_ATTACK );		
	}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	else if( true == m_kSkillSlotKey.IsKeyReleased( m_InputData ) )	// char-elesis CH1p: June 32b5c0c0 @32b5c103
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	else if( true == m_sCurrentSkillKeyManager.IsUnpressedCurrentSkillKey( m_InputData ) )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	{
		StateChange( ESI_SI_SA_SSK_EXTINCTION_ATTACK );
	}
	// 방향키를 통해 발사 각도 조절
	else if( true == m_InputData.pureDown )
	{
		StateChange( ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2b: June 32b5c0c0
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP_FrameMoveFuture()
{
	// 이펙트 각도 조절
	Extinction_FrameMoveFuture();

	CommonFrameMoveFuture();
}
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN_FrameMove()	// 각도 조절
{
	m_fExtinctionRemainTime += m_fElapsedTime;

#ifdef ADD_MEMO_1ST_CLASS //김창한
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELESIS_MEMO6) == true )
	{
		if( m_fExtinctionDegreeZ > _CONST_EL_::MIN_EXTINCTION_DEGREE_Z - 30.f)
			m_fExtinctionDegreeZ -= m_fElapsedTime * _CONST_EL_::EXTINCTION_DEGREE_Z_CONTROL;
		else
			m_fExtinctionDegreeZ = _CONST_EL_::MIN_EXTINCTION_DEGREE_Z- 30.f; 
	}
	else
#endif //ADD_MEMO_1ST_CLASS
	{
		if( m_fExtinctionDegreeZ > _CONST_EL_::MIN_EXTINCTION_DEGREE_Z )
			m_fExtinctionDegreeZ -= m_fElapsedTime * _CONST_EL_::EXTINCTION_DEGREE_Z_CONTROL;
		else
			m_fExtinctionDegreeZ = _CONST_EL_::MIN_EXTINCTION_DEGREE_Z; 
	}

	CommonFrameMove();
}
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN_FrameMoveFuture()
{
	// 이펙트 각도 조절
	Extinction_FrameMoveFuture();

	CommonFrameMoveFuture();
}
void CX2GUEL::ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_kSkillSlotKey.FollowSlotSwap();	// char-elesis CH1p: June 32b5c400 @32b5c40b
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_sCurrentSkillKeyManager.CheckSkillSlotSwap();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE

	// 최대 충전 시간 초과 시 공격 모션으로 변경
	if( m_fExtinctionRemainTime >= _CONST_EL_::MAX_EXTINCTION_CHARGE_TIME )
	{
		StateChange( ESI_SI_SA_SSK_EXTINCTION_ATTACK );		
	}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	else if( true == m_kSkillSlotKey.IsKeyReleased( m_InputData ) )	// char-elesis CH1p: June 32b5c400 @32b5c443
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	else if( true == m_sCurrentSkillKeyManager.IsUnpressedCurrentSkillKey( m_InputData ) )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	{
		StateChange( ESI_SI_SA_SSK_EXTINCTION_ATTACK );
	}
	// 방향키를 통해 발사 각도 조절
	else if( true == m_InputData.pureUp )
	{
		StateChange( ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2b: June 32b5c400
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}
#pragma endregion 세이버 - 익스팅션
// 승리의 검
void CX2GUEL::ESI_SI_SA_SSK_VICTORIOUS_SWORD_Init()
{
	TextureReadyInBackground(L"AeroTornado04.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"ColorBallFire.dds");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_04.dds");
	TextureReadyInBackground(L"Explosion_Light02.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	XSkinMeshReadyInBackground(L"HummingWind_AttackBox.X");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	TextureReadyInBackground(L"Mesh_Raven_Event_AC_Upbody21_Effect.tga");
	TextureReadyInBackground(L"Particle_Blur.dds");
	XSkinMeshReadyInBackground(L"SA_SPK_INFERNAL_BLADE_Weapon_Dummy.x");
	TextureReadyInBackground(L"Whitecircle02.dds");
	TextureReadyInBackground(L"Whitecircle03.dds");
	TextureReadyInBackground(L"title_StarLight_Smash_spark.dds");
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	TextureReadyInBackground(L"ARA_SA_FallingDragon_circleSpark.dds");	// char-elesis CH3a: June 32b501d0 ( the names March's list lacks )
	TextureReadyInBackground(L"ARA_SA_FallingDragon_down.DDS");
	TextureReadyInBackground(L"Lire_Kick_Impact01.dds");
	TextureReadyInBackground(L"soket_earth_quake_smoke02.DDS");
	TextureReadyInBackground(L"WhitePoint.dds");
	XMeshReadyInBackground(L"eve_SA_photonFlare_lightLine.Y");
	XMeshReadyInBackground(L"Up_Light01_Mesh.Y");
	XSkinMeshReadyInBackground(L"Lire_SI_SA_Gungnir_Mesh05.X");
	XSkinMeshReadyInBackground(L"SA_AEM_ELEMENTAL_STORM_FIRE01.X");
	XSkinMeshReadyInBackground(L"SA_SGM_SWORD_OF_RELICS_Hand_M01.X");
	XSkinMeshReadyInBackground(L"SA_SSK_VICTORIOUS_SWORD_Weapon_Dummy.x");
	XSkinMeshReadyInBackground(L"VictoriousSword_UpgradeWeaponM01.X");
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
void CX2GUEL::ESI_SI_SA_SSK_VICTORIOUS_SWORD_EventProcess()
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
{	// char-elesis CH3a: June 32b5c740 ( the walk / back-walk / dash / ZX cancels need the foot on a line )
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
void CX2GUEL::ESI_SI_SA_SSK_VICTORIOUS_SWORD_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CX2EffectSet::Handle hHandle = INVALID_EFFECTSET_HANDLE;
		if( GetUnitClass() == CX2Unit::UC_ELESIS_SABER_KNIGHT )
			hHandle = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_SSK_VICTORIOUS_SWORD_FX_NEW", this );
		else
			hHandle = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_SGM_VICTORIOUS_SWORD_NEW", this );

		m_vecEffectSetToDeleteOnDamageReact.push_back(hHandle);
	}		

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( 0.533f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.533f ) == true && EventCheck( 0.533f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		/// 스킬 레벨 반환
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		// char-elesis CH3a: June 32b76320 @32b76634 ( the level of 28008; the buff factor follows the buffs held, not the class )
		const int iSkillLevel = max( GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::RNSI_SA_SGM_VICTORIOUS_SWORD, false, 0, true ), 1 );

		if( false == HaveThisBuff( BTI_BUFF_SGM_VICTORIOUS_SWORD ) && false == HaveThisBuff( BTI_BUFF_SGM_VICTORIOUS_SWORD_UPGRADE ) )
		{
			SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_SGM_VICTORIOUS_SWORD, iSkillLevel );
		}
		else
		{
			EraseBuffTempletFromGameUnit( BTI_BUFF_SGM_VICTORIOUS_SWORD );	// June 3229e160
			SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_SGM_VICTORIOUS_SWORD_UPGRADE, iSkillLevel );
		}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		const int iSkillLevel = max( GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_SSK_VICTORIOUS_SWORD ), 1 );

		if( GetUnitClass() == CX2Unit::UC_ELESIS_SABER_KNIGHT )
			SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_VICTORIOUS_SWORD, iSkillLevel );
		else
			SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_SGM_VICTORIOUS_SWORD, iSkillLevel );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	}

	CommonFrameMove();
}
// 저지먼트 드라이브
void CX2GUEL::ESI_SI_SA_SSK_JUDGEMENT_DRIVE_Init()
{
	TextureReadyInBackground(L"AeroTornado04.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"ColorBallFire.dds");
	TextureReadyInBackground(L"Condense_Pulse02.dds");
	XSkinMeshReadyInBackground(L"Elsword_Double_Slash_Mesh01.X");
	XSkinMeshReadyInBackground(L"Elsword_Double_Slash_Mesh02.X");
	TextureReadyInBackground(L"Explosion_Light02.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	XSkinMeshReadyInBackground(L"Lire_SI_SA_Gungnir_Mesh05.X");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	TextureReadyInBackground(L"Particle_Blur.dds");
	XSkinMeshReadyInBackground(L"aisha_active_energySpurt_circle.X");
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	TextureReadyInBackground(L"Arme_Critical.dds");	// char-elesis CH2b: June 32b50320
	TextureReadyInBackground(L"Arme_Damage_Center.dds");
	TextureReadyInBackground(L"Arme_Damage_Ring.dds");
	TextureReadyInBackground(L"Mesh_Raven_Event_AC_Upbody21_Effect.tga");
	TextureReadyInBackground(L"star2.dds");
	TextureReadyInBackground(L"Stat_Stun_B.tga");
	TextureReadyInBackground(L"WhitePoint.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
#ifdef ADD_MEMO_1ST_CLASS //김창한
void CX2GUEL::ESI_SI_SA_SSK_JUDGEMENT_DRIVE_StateStartFuture()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	CommonStateStartFuture();	// char-elesis CH2b: June 32b50430
	m_kSkillSlotKey.Set( m_iNowSpecialAttack - 1 );
	m_fDashJumpZXChargeTime = 0.f;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	CommonStateStartFuture();

	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELESIS_MEMO7 ) == true )
	{
		float fAniSpeed = m_pXSkinAnimFuture->GetPlaySpeed();
		m_pXSkinAnimFuture->SetPlaySpeed( fAniSpeed * 1.3f );
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
void CX2GUEL::ESI_SI_SA_SSK_JUDGEMENT_DRIVE_StateStart()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	CommonStateStart();	// char-elesis CH2b: June 32b50460
	m_fDashJumpZXChargeTime = 0.f;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	CommonStateStart();

	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELESIS_MEMO7 ) == true )
	{
		float fAniSpeed = m_pXSkinAnim->GetPlaySpeed();
		m_pXSkinAnim->SetPlaySpeed( fAniSpeed * 1.3f );
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

}
#endif //ADD_MEMO_1ST_CLASS
void CX2GUEL::ESI_SI_SA_SSK_JUDGEMENT_DRIVE_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	m_kSkillSlotKey.FollowSlotSwap();	// char-elesis CH2b: June 32b50480
	if ( m_fEventTime[0].stateChange + JUDGEMENT_DRIVE_HOLD_TIME_AFTER_EVENT < m_FrameDataFuture.unitCondition.fStateTime
		|| ( true == m_kSkillSlotKey.IsKeyReleased( m_InputData ) && m_fEventTime[0].stateChange < m_FrameDataFuture.unitCondition.fStateTime ) )
	{
		StateChange( ESI_SI_SA_SSK_JUDGEMENT_DRIVE_FINAL );
	}
	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
void CX2GUEL::ESI_SI_SA_SSK_JUDGEMENT_DRIVE_FrameMove()	// char-elesis CH2b: June 32b5cb40
{
	if ( m_fEventTime[0].stateChange < m_FrameDataNow.unitCondition.fStateTime )
	{
		const float fChargeLimit = ( true == IsNowStateSkillUpgraded() ) ? JUDGEMENT_DRIVE_CHARGE_LIMIT_UPGRADE : JUDGEMENT_DRIVE_CHARGE_LIMIT;
		m_fDashJumpZXChargeTime = min( m_fElapsedTime + m_fDashJumpZXChargeTime, fChargeLimit );	// the member the Dark Knight charge states use
	}
	CommonFrameMove();
}
void CX2GUEL::ESI_SI_SA_SSK_JUDGEMENT_DRIVE_FINAL_FrameMove()	// char-elesis CH2b: June 32b76710
{
	CommonFrameMove();
	if ( true == m_pXSkinAnim->EventTimerOneshot( JUDGEMENT_DRIVE_DAMAGE_EVENT_TIME ) )
	{
		if ( JUDGEMENT_DRIVE_MIN_CHARGE > m_fDashJumpZXChargeTime )
			m_fDashJumpZXChargeTime = 0.f;
		const float fPowerPerCharge = ( true == IsNowStateSkillUpgraded() ) ? JUDGEMENT_DRIVE_POWER_PER_CHARGE_UPGRADE : JUDGEMENT_DRIVE_POWER_PER_CHARGE;
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_SSK_JUDGEMENT_DAMAGE", this, NULL, false,
			GetPowerRate() * ( 1.f + m_fDashJumpZXChargeTime * fPowerPerCharge ), -1.f );
	}
}
void CX2GUEL::ESI_SI_SA_SSK_JUDGEMENT_DRIVE_FINAL_EventProcess()	// char-elesis CH2b: June 32b5cbd0
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 스킬 스테이트 함수 - 2-1차 파이로 나이트
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CX2GUEL::ESI_DASH_COMBO_SPK_ZX_EventProcess()			// ESI_DASH_COMBO_SPK_ZX
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1f: June 32b5ced0 ( also the body of ESI_COMBO_SBH_ZZZX; CH1m registers it )
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )	{		StateChange( USI_WAIT );	}	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )	WALK_CANCEL_AFTER( m_fWalkCancelAfter )	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ESI_DASH_COMBO_SPK_ZX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.233f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.233f ) == true && EventCheck( 0.233f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if(	true == FlushMp( _CONST_EL_::FIRE_BALL_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_EL_Dash_Combo_ZX", this, NULL, IsHyperState(), GetPowerRate()  * GetFireBallPowerRate() );
		else
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_EL_Dash_Combo_ZX_FAIL", this );
	}
	CommonFrameMove();
}
void CX2GUEL::ESI_DASH_COMBO_SPK_ZZ_EventProcess()			// DASH_COMBO_ZZ
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1f: June 32b5d1d0
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneZ, ESI_DASH_COMBO_SPK_ZZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, true == m_InputData.oneZ, ESI_DASH_COMBO_SPK_ZZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ESI_DASH_COMBO_SPK_ZZZ_EventProcess()			// DASH_COMBO_ZZZ
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1f: June 32b5d590
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ESI_DASH_COMBO_SPK_ZZZ_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.266f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.266f ) == true && EventCheck( 0.266f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if(	true == FlushMp( _CONST_EL_::FIRE_BALL_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_EL_Dash_Combo_ZZZ", this, NULL, IsHyperState(), GetPowerRate()  * GetFireBallPowerRate() );
		else
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_EL_Dash_Combo_ZZZ_FAIL", this );
	}
	CommonFrameMove();
}
void CX2GUEL::ESI_DASH_JUMP_COMBO_SPK_XZ_EventProcess()		// DASH_JUMP_COMBO_XZ
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1f: June 32b5d890 ( the Z press re-attacks while June's counter ( 2 from the start state ) lasts, also at animation end )
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneZ && 0 < m_uiSPKXZReattakCount, ESI_DASH_JUMP_COMBO_SPK_XZ_REATTACK )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_JUMP_DOWN );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT(0,0, true == m_InputData.oneZ && 0 < m_uiSPKXZReattakCount, ESI_DASH_JUMP_COMBO_SPK_XZ_REATTACK)
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

		CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ESI_DASH_JUMP_COMBO_SPK_XZ_FrameMove()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1f: June 32b770e0 ( the fireball damage effect of SERV_IRUHADEV_JUNE_EL_DASHJUMP_XZ_FIREBALL, as the state's own body )
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		if( true == IsEnoughMPForAttack( _CONST_EL_::SPK_DASH_JUMP_XZ_MP_COST, vBonePos, 0.f, D3DXVECTOR3( 0.f, 0.f, -45.f ) ) )
		{
			D3DXVECTOR3 vRotateDegree = GetRotateDegree();
			vRotateDegree.z = -45.f;

			const WCHAR* pwszDamageEffectName = L"DamageEffect_Elesis_SPK_DashJump_Combo_XZ";
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE
			if( true == m_bIsLearnUnextinguishableFire )
				pwszDamageEffectName = L"DamageEffect_Elesis_SBH_DashJump_Combo_XZ";
#endif SERV_ELESIS_SECOND_CLASS_CHANGE

			g_pX2Game->GetDamageEffect()->CreateInstance( this, pwszDamageEffectName, GetPowerRate() * GetFireBallPowerRate(),
				vBonePos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
		}
	}

	CommonFrameMove();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef SERV_IRUHADEV_JUNE_EL_DASHJUMP_XZ_FIREBALL
		// June2017 32b770e0 (this state's FrameMove; also the _REATTACK state's): no effect set. The fireball damage
		// effect is created directly at Dummy2_Lhand, never SetScale'd (scale 1), angle and move axis
		// ( dirDegree.x, dirDegree.y, -45 ), land pos unitCondition.landPosition.y (+0x80c4). June baked the old
		// effect set's x2 into its data (SPK_Fire_Ball_Damage_M01 Size 0.7 -> 1.4, passive particles 1 / 0.9 -> 2 / 1.8)
		// and the dive into this angle (mesh Velocity (700,-800,0) Rotate 135 -> (1063,0,0)), so the March effect set
		// drew June's data at 2x and aimed it 10 degrees up. MP: vslot 0x6f0 = IsEnoughMPForAttack (32007e90) with
		// ( 4, bone pos, 0, (0,0,-45) ); its not-enough-MP effect replaces EffectSet_A_EL_DashJump_Combo_XZ_FAIL.
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		if( true == IsEnoughMPForAttack( _CONST_EL_::SPK_DASH_JUMP_XZ_MP_COST, vBonePos, 0.f, D3DXVECTOR3( 0.f, 0.f, -45.f ) ) )
		{
			D3DXVECTOR3 vRotateDegree = GetRotateDegree();
			vRotateDegree.z = -45.f;

			const WCHAR* pwszDamageEffectName = L"DamageEffect_Elesis_SPK_DashJump_Combo_XZ";
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE
			if( true == m_bIsLearnUnextinguishableFire )
				pwszDamageEffectName = L"DamageEffect_Elesis_SBH_DashJump_Combo_XZ";
#endif SERV_ELESIS_SECOND_CLASS_CHANGE

			g_pX2Game->GetDamageEffect()->CreateInstance( this, pwszDamageEffectName, GetPowerRate() * GetFireBallPowerRate(),
				vBonePos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
		}
#else SERV_IRUHADEV_JUNE_EL_DASHJUMP_XZ_FIREBALL
		if(	true == FlushMp( _CONST_EL_::SPK_DASH_JUMP_XZ_MP_COST ) )
		{
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
			if( true == m_bIsLearnUnextinguishableFire )
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_SBH_DashJump_Combo_XZ", this, NULL, IsHyperState(), GetPowerRate() * GetFireBallPowerRate() );
			else
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_EL_DashJump_Combo_XZ", this, NULL, IsHyperState(), GetPowerRate() * GetFireBallPowerRate() );
		}
		else
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_EL_DashJump_Combo_XZ_FAIL", this );
#endif SERV_IRUHADEV_JUNE_EL_DASHJUMP_XZ_FIREBALL
	}

	CommonFrameMove();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
/** @function : ESI_DASH_JUMP_COMBO_SPK_ZX_StartFurture
	@brief : ESI_DASH_JUMP_COMBO_SPK_ZX 이후 추가 X공격(파이어볼) 구현을 위한 함수
*/
void CX2GUEL::ESI_DASH_JUMP_COMBO_SPK_XZ_StartFurture()
{
	m_bIsDashAttackPossible = false;
	m_uiSPKXZReattakCount = _CONST_EL_::MAX_DASH_JUMP_COMBO_SPK_XZ_REATTACK;
	CommonStateStartFuture();
}
/** @function : ESI_DASH_JUMP_COMBO_SPK_ZX_REATTACK_StartFurture
	@brief : ESI_DASH_JUMP_COMBO_SPK_ZX 이후 추가 X공격(파이어볼) 구현을 위한 함수
*/
void CX2GUEL::ESI_DASH_JUMP_COMBO_SPK_XZ_REATTACK_StartFurture()
{
	--m_uiSPKXZReattakCount;
	CommonStateStartFuture();
}
// 소드 이럽션
void CX2GUEL::ESI_SI_A_SPK_SWORD_ERUPTION_Init()
{
	XSkinMeshReadyInBackground(L"A_SPK_SWORD_ERUPTION_BLADE.x");
	XSkinMeshReadyInBackground(L"A_SPK_SWORD_ERUPTION_Square.x");
	TextureReadyInBackground(L"AeroTornado04.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"COMET_CRASHER_Explosion_02.tga");
	TextureReadyInBackground(L"EL_FIRE_01.dds");
	TextureReadyInBackground(L"EL_FIRE_02.dds");
	TextureReadyInBackground(L"EL_FIRE_04.dds");
	TextureReadyInBackground(L"EL_FIRE_07.dds");
	TextureReadyInBackground(L"Explosion_Light02.dds");
	TextureReadyInBackground(L"Flame_REVOLVER.dds");
	XSkinMeshReadyInBackground(L"Lire_SI_SA_Gungnir_Mesh05.X");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	TextureReadyInBackground(L"Particle_Blur.DDS");
}
void CX2GUEL::ESI_SI_A_SPK_SWORD_ERUPTION_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
// 대폭살
void CX2GUEL::ESI_SI_A_SPK_BIG_BURST_Init()					
{
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"ColorBallFire.dds");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_06.dds");
	TextureReadyInBackground(L"EL_FIRE_09.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
}
void CX2GUEL::ESI_SI_A_SPK_BIG_BURST_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2c: June 32b5df30
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}
// 버스트 라이징
void CX2GUEL::ESI_SI_SA_SPK_BURST_RISING_Init()		
{
	XSkinMeshReadyInBackground(L"A_SPK_SWORD_ERUPTION_BLADE.x");
	XSkinMeshReadyInBackground(L"A_SPK_SWORD_ERUPTION_Square.x");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"ColorBallFire.dds");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_06.dds");
	TextureReadyInBackground(L"EL_FIRE_09.dds");
	XSkinMeshReadyInBackground(L"Lire_SI_SA_Gungnir_Mesh05.X");
	XSkinMeshReadyInBackground(L"SA_EL_MEGA_BURSTER_FIRE_CIRCLE.X");
	XSkinMeshReadyInBackground(L"SA_EL_MEGA_BURSTER_FIRE_Earth.X");
}
void CX2GUEL::ESI_SI_SA_SPK_BURST_RISING_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
// 버스트 웨이브
void CX2GUEL::ESI_SI_SA_SPK_BURST_WAVE_Init()	
{
	TextureReadyInBackground(L"AeroTornado04.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"Condense_Pulse02.dds");
	TextureReadyInBackground(L"EL_FIRE_01.dds");
	TextureReadyInBackground(L"EL_FIRE_02.dds");
	TextureReadyInBackground(L"EL_FIRE_04.dds");
	TextureReadyInBackground(L"EL_FIRE_07.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Explosion_Light02.dds");
	TextureReadyInBackground(L"Flame_REVOLVER.dds");
	TextureReadyInBackground(L"Particle_Blur.DDS");
	TextureReadyInBackground(L"SA_EL_MEGA_SLASH_FIRE02.dds");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	TextureReadyInBackground(L"Mesh_Elsword_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"elsword_SA_rageCutter_smogPoint.dds");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	XSkinMeshReadyInBackground(L"DummyAttackBox_200x200x200.X");
}
void CX2GUEL::ESI_SI_SA_SPK_BURST_WAVE_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
// 소드 파이어
void CX2GUEL::ESI_SI_SA_SPK_SWORDFIRE_Init()			
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	TextureReadyInBackground(L"Blur_16.dds");	// char-elesis CH2c: June 32b507d0 ( March preloads nothing )
	TextureReadyInBackground(L"Rose_explosion_01.Tga");
	TextureReadyInBackground(L"secretVelder_perpit_fire_flame.dds");
	XMeshReadyInBackground(L"BumBleBee_C_SkillC_E1.Y");
	XSkinMeshReadyInBackground(L"Darkcloud_AttackBox01.x");
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

}
void CX2GUEL::ESI_SI_SA_SPK_SWORDFIRE_EventProcess()
{	
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	// char-elesis CH2c: June 32b5e830 ( the skill state only picks the ground or the air state )
	StateChange( true == m_FrameDataFuture.unitCondition.bFootOnLine ? ESI_SI_SA_SPK_SWORDFIRE_GROUND : ESI_SI_SA_SPK_SWORDFIRE_FLY );
	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_LANDING );
		}	
		else
		{
			// 현재 진행 방향 쪽으로 키보드를 누르면 앞으로 좀 더 나아감
			if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
				( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
			{
				m_PhysicParam.nowSpeed.x = GetWalkSpeed();
			}
		}
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
// 인페르날 블레이드
void CX2GUEL::ESI_SI_SA_SPK_INFERNAL_BLADE_Init()
{
	TextureReadyInBackground(L"EL_FIRE_01.dds");
	TextureReadyInBackground(L"EL_FIRE_02.dds");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_04.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Explosion_Light01.dds");
	TextureReadyInBackground(L"SA_EL_MEGA_SLASH_FIRE02.dds");
	XSkinMeshReadyInBackground(L"SA_SPK_INFERNAL_BLADE_Weapon_Dummy.x");
	TextureReadyInBackground(L"Whitecircle03.dds");
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	TextureReadyInBackground(L"EL_FIRE_10.dds");	// char-elesis CH2c: June 32b50820 ( 6 more names than March )
	TextureReadyInBackground(L"Field_Boss_Dark_Snake_Smoke01.dds");
	TextureReadyInBackground(L"SA_EL_MEGA_SLASH_FIRE01.dds");
	TextureReadyInBackground(L"Whitecircle03.dds");
	XMeshReadyInBackground(L"SPK_InfernalBlade_WeaponM01.Y");
	XSkinMeshReadyInBackground(L"SPK_InfernalBlade_WeaponM01.x");
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}
void CX2GUEL::ESI_SI_SA_SPK_INFERNAL_BLADE_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2c: June 32b5eb60
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	CommonEventProcess();
}
// 이터널 파이어
void CX2GUEL::ESI_SI_SA_SPK_ETERNAL_FIRE_Init()		
{
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_10.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	TextureReadyInBackground(L"Particle_Blur.DDS");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"SA_EL_MEGA_SLASH_FIRE02.dds");
	TextureReadyInBackground(L"state_potion_flare.DDS");
	TextureReadyInBackground(L"stone_AirShip.dds");
	XSkinMeshReadyInBackground(L"Elsword_SI_SA_Phoenix_Talon_Mesh02.X");
	XSkinMeshReadyInBackground(L"SA_SPK_ETERNAL_FIRE_Fire_Circle01.x");
	XSkinMeshReadyInBackground(L"SA_SPK_ETERNAL_FIRE_Fire_Circle02.x");
	XSkinMeshReadyInBackground(L"SA_SPK_ETERNAL_FIRE_Fire_Circle03.x");
}
void CX2GUEL::ESI_SI_SA_SPK_ETERNAL_FIRE_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

// 대시 점프 콤보 XZ
void CX2GUEL::ESI_DASH_JUMP_COMBO_SGM_XZ_Init()		
{
	XSkinMeshReadyInBackground(L"SGM_DASH_JUMP_COMBO_XZ_M01.X");
	XSkinMeshReadyInBackground(L"Ara_Dash_Combo_X_M01.X");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"GroundShockWave02.dds");
}

void CX2GUEL::ESI_DASH_JUMP_COMBO_SGM_XZ_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_DASH_JUMP_COMBO_SGM_XZ_LOOP );
	}
	CommonEventProcess();
}

// 대시 점프 콤보 XZ
void CX2GUEL::ESI_DASH_JUMP_COMBO_SGM_XZ_LOOP_EventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( ESI_DASH_JUMP_COMBO_SGM_XZ_DOWN );
	}
	CommonEventProcess();


}

// 대시 점프 콤보 XZ 다운
void CX2GUEL::ESI_DASH_JUMP_COMBO_SGM_XZ_DOWN_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1e: June 32b5f1c0
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}

	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

// 콤보 XX 다운 X 시작

void CX2GUEL::ESI_COMBO_SGM_XX_DOWN_X_START_Init()		
{
	XSkinMeshReadyInBackground(L"Elsword_SI_A_ES_WINDMILL_Mesh01.X");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	XSkinMeshReadyInBackground(L"Elsword_SI_A_ES_WINDMILL_Mesh02.X");
	XSkinMeshReadyInBackground(L"SA_SSK_EXTINCTION_END_FX_Blade.X");
	TextureReadyInBackground(L"title_StarLight_Smash_spark.dds");
}

void CX2GUEL::ESI_COMBO_SGM_XX_DOWN_X_START_FrameMove()		
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) == true )
#else	X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif	X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE	
	{

		if ( NULL != g_pX2Game->GetEffectSet() )
		{
			m_hXXDownXLoop = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SGM_XXX_Start",
				this, this, IsHyperState(), GetPowerRate() );
		}

		m_vecEffectSetToDeleteOnDie.push_back( m_hXXDownXLoop );
		m_vecEffectSetToDeleteOnCustomState.push_back( m_hXXDownXLoop );
	}
	CommonFrameMove();
}



void CX2GUEL::ESI_COMBO_SGM_XX_DOWN_X_START_StateStart()		
{
	CommonStateStart();
	m_iComboXXdownXNowLoopCount = 0; // 루프 횟수 초기화 ( 기본 애니메이션에서 4회 회전 )
	m_iComboXXdownXMaxLoopCount = 0;
	
}

void CX2GUEL::ESI_COMBO_SGM_XX_DOWN_X_START_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1e: June 32b72fe0 ( the loop count cap as March; June adds the forward / backward speed while the state plays )
	if ( m_InputData.oneX == true )
	{
		++m_iComboXXdownXMaxLoopCount;
		if ( m_iComboXXdownXMaxLoopCount >= _CONST_EL_::MAX_XX_DOWN_X_LOOP_COUNT )
			m_iComboXXdownXMaxLoopCount = _CONST_EL_::MAX_XX_DOWN_X_LOOP_COUNT;
	}

	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( m_iComboXXdownXMaxLoopCount > 0 )
			StateChange( ESI_COMBO_SGM_XX_DOWN_X_LOOP );
		else
			StateChange( ESI_COMBO_SGM_XX_DOWN_X_END );
	}
	else
	{
		if ( true == IsOnSomethingFuture() )
			m_PhysicParam.nowSpeed.y = 0.f;
		m_PhysicParam.nowSpeed.x = XX_DOWN_X_FORWARD_SPEED;
		if ( true == m_InputData.pureRight )
		{
			if ( false == GetIsRight() )
				m_PhysicParam.nowSpeed.x = XX_DOWN_X_BACKWARD_SPEED;
		}
		else if ( true == m_InputData.pureLeft )
		{
			if ( true == GetIsRight() )
				m_PhysicParam.nowSpeed.x = XX_DOWN_X_BACKWARD_SPEED;
		}
		else
			m_PhysicParam.nowSpeed.x = 0.f;
	}

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( m_InputData.oneX == true )
	{
		++m_iComboXXdownXMaxLoopCount;
		if ( m_iComboXXdownXMaxLoopCount >= _CONST_EL_::MAX_XX_DOWN_X_LOOP_COUNT )
			m_iComboXXdownXMaxLoopCount = _CONST_EL_::MAX_XX_DOWN_X_LOOP_COUNT;
	}

	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( m_iComboXXdownXMaxLoopCount > 0 )
			StateChange( ESI_COMBO_SGM_XX_DOWN_X_LOOP );
		else
			StateChange( ESI_COMBO_SGM_XX_DOWN_X_END );
	}

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

// 콤보 XX 다운 X 루프
void CX2GUEL::ESI_COMBO_SGM_XX_DOWN_X_LOOP_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1e: June 32b731a0 ( same speed block as the start state )
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		++m_iComboXXdownXNowLoopCount;

		if ( m_iComboXXdownXNowLoopCount < m_iComboXXdownXMaxLoopCount )
			StateChange( ESI_COMBO_SGM_XX_DOWN_X_LOOP );
		else
			StateChange( ESI_COMBO_SGM_XX_DOWN_X_END );
	}
	else
	{
		if ( true == IsOnSomethingFuture() )
			m_PhysicParam.nowSpeed.y = 0.f;
		m_PhysicParam.nowSpeed.x = XX_DOWN_X_FORWARD_SPEED;
		if ( true == m_InputData.pureRight )
		{
			if ( false == GetIsRight() )
				m_PhysicParam.nowSpeed.x = XX_DOWN_X_BACKWARD_SPEED;
		}
		else if ( true == m_InputData.pureLeft )
		{
			if ( true == GetIsRight() )
				m_PhysicParam.nowSpeed.x = XX_DOWN_X_BACKWARD_SPEED;
		}
		else
			m_PhysicParam.nowSpeed.x = 0.f;
	}
	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		++m_iComboXXdownXNowLoopCount;

		if ( m_iComboXXdownXNowLoopCount < m_iComboXXdownXMaxLoopCount )
			StateChange( ESI_COMBO_SGM_XX_DOWN_X_LOOP );
		else
			StateChange( ESI_COMBO_SGM_XX_DOWN_X_END );
	}
	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}


// 콤보 XX 다운 X 끝
void CX2GUEL::ESI_COMBO_SGM_XX_DOWN_X_END_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1e: June 32b5f4c0
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

// 재빠른 기상, 앞 구르기 공격
void CX2GUEL::ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a, CH1b review: June 32b5f7c0
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

// 재빠른 기상, 뒷 구르기 공격
void CX2GUEL::ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1a: June 32b5fac0
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}


// 도발 시작
void CX2GUEL::ESI_A_SGM_PROVOKE_Init()		
{
	TextureReadyInBackground(L"A_SGM_DRASTIC_CHARGE_Power_P03.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	XSkinMeshReadyInBackground(L"Lire_SI_SA_Gungnir_Mesh05.X");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"SGM_D_J_C_XZ_Landing2.dds");
	TextureReadyInBackground(L"title_StarLight_Smash_spark.dds");
	TextureReadyInBackground(L"Iron_Will_P01.DDS");
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	// char-elesis CH3a2: June 32b50a30 ( the 9 names March's list lacks )
	TextureReadyInBackground(L"Arme_Critical.dds");
	TextureReadyInBackground(L"Arme_Damage_Center.dds");
	TextureReadyInBackground(L"Arme_Damage_Ring.dds");
	TextureReadyInBackground(L"LD_Slicer_trace_01.dds");
	TextureReadyInBackground(L"star2.dds");
	TextureReadyInBackground(L"WhitePoint.dds");
	TextureReadyInBackground(L"WhitePointSmall.dds");
	XSkinMeshReadyInBackground(L"SGM_ProvokeReadyTM01.X");
	XSkinMeshReadyInBackground(L"SI_A_SBH_SCALET_ROSE_attackSphere.X");
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}

void CX2GUEL::ESI_A_SGM_PROVOKE_StateStart()		
{
	SetIsProvokeStateChange( true );
	CommonStateStart();
}

void CX2GUEL::ESI_A_SGM_PROVOKE_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	// char-elesis CH3a2: June 32b85aa0 ( a press of the skill key inside the key window spends 10 MP and arms the revenge, which starts at the state-change time or the end of the animation )
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if( true == m_FrameDataFuture.stateParam.bEventFlagList[0] &&
		( m_fEventTime[0].stateChange < m_pXSkinAnimFuture->GetNowAnimationTime() || true == m_pXSkinAnimFuture->IsAnimationEnd() ) )
	{
		StateChange( ESI_A_SGM_PROVOKE_REVENGE );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( true == m_kSkillSlotKey.IsKeyPressedOnce( m_InputData ) &&
		m_pXSkinAnimFuture->GetNowAnimationTime() < m_fEventTime[0].keyInputEnd && m_fEventTime[0].keyInputStart <= m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		if( PROVOKE_REVENGE_MP_CONSUME <= GetNowMp() )
		{
			UpNowMp( -PROVOKE_REVENGE_MP_CONSUME );
			m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		}
		else
		{
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1),
				D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
		}
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH3a2: June 32b85aa0 ( the cancels need the foot on a line )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}


void CX2GUEL::ESI_A_SGM_PROVOKE_StateEnd()
{
	SetIsProvokeStateChange( false );
	CommonStateEnd();
}


// 도발 성공 후 반격
void CX2GUEL::ESI_A_SGM_PROVOKE_REVENGE_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH3a2: June 32b5fb30 ( the cancels need the foot on a line )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}


// 드래스틱 차지 시작 
void CX2GUEL::ESI_A_SGM_DRASTIC_CHARGE_START_Init()		
{
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_04.dds");
	TextureReadyInBackground(L"Mesh_Elsword_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"A_SGM_DRASTIC_CHARGE_Power.dds");
	TextureReadyInBackground(L"A_SGM_DRASTIC_CHARGE_Power_P03.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	XSkinMeshReadyInBackground(L"SA_EL_SA_SSK_SPIRAL_BLAST_01.X");
	TextureReadyInBackground(L"SGM_D_J_C_XZ_Landing2.dds");
	TextureReadyInBackground(L"title_StarLight_Smash_spark.dds");

}

void CX2GUEL::ESI_A_SGM_DRASTIC_CHARGE_START_StateStart ()
{
	CommonStateStart();
	m_fDrasticChargeNowLoopTime = 0;
}

void CX2GUEL::ESI_A_SGM_DRASTIC_CHARGE_START_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( NULL != g_pX2Game->GetEffectSet() )
		{
			m_hDrasticChargeGroundFire = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_SGM_DRASTIC_CHARGE_GroundFire",
				this, this, IsHyperState(), GetPowerRate() );
			m_hDrasticChargeLoop = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_SGM_DRASTIC_CHARGE_LOOP",
				this, this, IsHyperState(), GetPowerRate() );
		}

		m_vecEffectSetToDeleteOnDie.push_back( m_hDrasticChargeLoop );
		m_vecEffectSetToDeleteOnCustomState.push_back( m_hDrasticChargeLoop );

		m_vecEffectSetToDeleteOnDie.push_back( m_hDrasticChargeGroundFire );
		m_vecEffectSetToDeleteOnCustomState.push_back( m_hDrasticChargeGroundFire );
	}
	CommonFrameMove();
}



void CX2GUEL::ESI_A_SGM_DRASTIC_CHARGE_START_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_A_SGM_DRASTIC_CHARGE_LOOP );
	}

	CommonEventProcess();
}

void CX2GUEL::ESI_A_SGM_DRASTIC_CHARGE_START_StartFurture()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_kSkillSlotKey.Set( m_iNowSpecialAttack - 1 );	// char-elesis CH1p: June 32b50be0 @32b50bf1
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_sCurrentSkillKeyManager.InitCurrentSkillKeyManager( m_iNowSpecialAttack - 1 );
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonStateStartFuture();
}


// 드래스틱 차지 루프
void CX2GUEL::ESI_A_SGM_DRASTIC_CHARGE_LOOP_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_kSkillSlotKey.FollowSlotSwap();	// char-elesis CH1p: June 32b5fe60 @32b5fe6d
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	m_sCurrentSkillKeyManager.CheckSkillSlotSwap();		// 스킬 슬롯이 변경되었는가?
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE

	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )									// Animation End 일 때
	{
		StateChange( ESI_A_SGM_DRASTIC_CHARGE_LOOP );									// Loop 로 진행
	}

	else if ( m_fDrasticChargeNowLoopTime >= _CONST_EL_::MAX_DRASTIC_CHARGE_LOOP_TIME )	/// 최대 시간이 지났을 경우
	{													
		StateChange( ESI_A_SGM_DRASTIC_CHARGE_END );										// End 로 진행
	}

	else if ( m_fDrasticChargeNowLoopTime >= _CONST_EL_::MIN_DRASTIC_CHARGE_LOOP_TIME )	// 최소 시간을 지나갔다면
	{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
		if ( true == m_kSkillSlotKey.IsKeyReleased( m_InputData ) ||	// char-elesis CH1p: June 32b5fe60 @32b5fedb
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
		if ( true == m_sCurrentSkillKeyManager.IsUnpressedCurrentSkillKey( m_InputData ) || 	// 키 입력이 입력되지 않았거나 
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
			 m_fDrasticChargeLoopDecreaseMPValue > GetNowMp() )								// 마나가 적을 경우
		{
			StateChange( ESI_A_SGM_DRASTIC_CHARGE_END );									// End 로 진행
		}								
	}
	
	CommonEventProcess();
}
void CX2GUEL::ESI_A_SGM_DRASTIC_CHARGE_LOOP_FrameMove()
{
	m_fDrasticChargeNowLoopTime = m_fDrasticChargeNowLoopTime + m_fElapsedTime;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		FlushMp( m_fDrasticChargeLoopDecreaseMPValue );
	}

	CommonFrameMove();
}

// 드래스틱 차지 끝 
void CX2GUEL::ESI_A_SGM_DRASTIC_CHARGE_END_StateStart()		
{
	CommonStateStart();
}

void CX2GUEL::ESI_A_SGM_DRASTIC_CHARGE_END_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH3a2: June 32b5ffc0 ( the cancels need the foot on a line )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}



// 스냅 크래셔, 공중에서 사용했다면 Loop, 아니라면 End 로 넘어감
void CX2GUEL::ESI_A_SGM_SNAP_CRASHER_START_Init()		
{
	XSkinMeshReadyInBackground(L"Ara_Dash_Combo_X_M01.X");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_100x50x50_front.X");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	TextureReadyInBackground(L"SGM_D_J_C_XZ_Landing2.dds");
	TextureReadyInBackground(L"title_StarLight_Smash_spark.dds");
	XSkinMeshReadyInBackground(L"SGM_DASH_JUMP_COMBO_XZ_M01.X");
	XSkinMeshReadyInBackground(L"JUGGERNAUT_BUSTER_ATTACK_BOX.X");
	TextureReadyInBackground(L"A_SGM_DRASTIC_CHARGE_Power_P01.dds");
	XSkinMeshReadyInBackground(L"BREAKING_FIST_Attack_Box.X");
}

void CX2GUEL::ESI_A_SGM_SNAP_CRASHER_START_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )			// 땅에서 떠있다면 Loop
			StateChange( ESI_A_SGM_SNAP_CRASHER_LOOP );
		else											// 붙어있다면 End
			StateChange( ESI_A_SGM_SNAP_CRASHER_END );
	}

	CommonEventProcess();
}

void CX2GUEL::ESI_A_SGM_SNAP_CRASHER_START_StateEnd()
{
	CommonStateEnd();

	if ( NULL != g_pX2Game->GetEffectSet() )
	{
		m_hSnapCreaherLoopAttack = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_SGM_SNAP_CRESHER_LOOP_ATTACK",
			this, this, IsHyperState(), GetPowerRate() );
	}
	m_vecEffectSetToDeleteOnCustomState.push_back ( m_hSnapCreaherLoopAttack );
	m_vecEffectSetToDeleteOnDie.push_back ( m_hSnapCreaherLoopAttack );
}


// 스냅 크래셔, 공중에서 사용 후 내려옴
void CX2GUEL::ESI_A_SGM_SNAP_CRASHER_LOOP_StateStart()		
{
	CommonStateStart();
	if ( NULL != g_pX2Game->GetEffectSet() )
	{
		m_hSnapCresherLoop = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_SGM_SNAP_CRESHER_LOOP",
			this, this, IsHyperState(), GetPowerRate() );
	}
	m_vecEffectSetToDeleteOnCustomState.push_back ( m_hSnapCresherLoop );
	m_vecEffectSetToDeleteOnDie.push_back ( m_hSnapCresherLoop );
}

void CX2GUEL::ESI_A_SGM_SNAP_CRASHER_LOOP_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( ESI_A_SGM_SNAP_CRASHER_END );
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_A_SGM_SNAP_CRASHER_LOOP );
	}
	CommonEventProcess();
}


// 스냅 크래셔, 공중에서 사용 후 내려와서 땅 찍기
void CX2GUEL::ESI_A_SGM_SNAP_CRASHER_END_StateStart()		
{
	if ( NULL != g_pX2Game->GetEffectSet() )
	{
		if ( INVALID_EFFECTSET_HANDLE != m_hSnapCresherLoop )
		{
			g_pX2Game->GetEffectSet()->StopEffectSet( m_hSnapCresherLoop );
			m_hSnapCresherLoop = INVALID_EFFECTSET_HANDLE;
		}

		if ( INVALID_EFFECTSET_HANDLE != m_hSnapCreaherLoopAttack )
		{
			g_pX2Game->GetEffectSet()->StopEffectSet( m_hSnapCreaherLoopAttack );
			m_hSnapCreaherLoopAttack = INVALID_EFFECTSET_HANDLE;
		}

	}
	CommonStateStart();
}

void CX2GUEL::ESI_A_SGM_SNAP_CRASHER_END_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}


// 소닉 블레이드

void CX2GUEL::ESI_SA_SGM_SONIC_BLADE_Init()		
{
	TextureReadyInBackground(L"A_SGM_DRASTIC_CHARGE_Power_P01.dds");
	TextureReadyInBackground(L"A_SGM_DRASTIC_CHARGE_Power_P02.dds");
	TextureReadyInBackground(L"A_SGM_SONIC_BLADE_Map02.dds");
	TextureReadyInBackground(L"SGM_D_J_C_XZ_Landing2.dds");
	TextureReadyInBackground(L"elsword_active_crecentCut_crecent.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"elsword_active_crecentCut_crecent.dds");
}

void CX2GUEL::ESI_SA_SGM_SONIC_BLADE_EventProcess()
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
{	// char-elesis CH3a: June 32b6f1f0 ( the walk / back-walk / dash / ZX cancels need the foot on a line )
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( true == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_LANDING );
		}
		else
		{
			if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
				( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
			{
				m_PhysicParam.nowSpeed.x = GetWalkSpeed();
			}
		}
	}

	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( true == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_LANDING );
		}

		else
		{
			// 현재 진행 방향 쪽으로 키보드를 누르면 앞으로 좀 더 나아감
			if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
				( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
			{
				m_PhysicParam.nowSpeed.x = GetWalkSpeed();
			}
		}
	}

	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}


// 크리티컬 다이브

#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
void CX2GUEL::ESI_SA_SGM_CRITICAL_DIVE_Init()		
{
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"ColorBallGray.dds");
	TextureReadyInBackground(L"RingAlpha.dds");
	TextureReadyInBackground(L"SGM_D_J_C_XZ_Landing2.dds");
	TextureReadyInBackground(L"title_StarLight_Smash_spark.dds");
	XSkinMeshReadyInBackground(L"A_SGM_CRITICAL_DIVE_Attack_Line.X");
	TextureReadyInBackground(L"waldo_trock_specialAttackB_particle.dds");
}

void CX2GUEL::ESI_SA_SGM_CRITICAL_DIVE_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	if ( m_pXSkinAnim->GetNowAnimationTime() > CRITICAL_DIVE_HIT_TIME && true == GetAttackSuccess( true ) )	// char-elesis CH2b: June 32b60650
	{
		StateChange( ESI_SA_SGM_CRITICAL_DIVE_HIT );
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		const CX2UserSkillTree::SkillSlotData* pSkillSlotData = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillSlot( GetUpgradeSwappedSkillID( CX2SkillTree::RNSI_SA_SSK_CRITICAL_DIVE ) );
		if ( NULL != pSkillSlotData )
		{
			const float fMpRefund = m_fSkillStartMpConsumed * CRITICAL_DIVE_MP_REFUND_RATE;
			m_fSkillStartMpConsumed = 0.f;
			UpNowMpWithText( fMpRefund );
		}
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	if( m_pXSkinAnim->GetNowAnimationTime() > 0.425f && GetAttackSuccess(true) == true )
	{
		StateChange( ESI_SA_SGM_CRITICAL_DIVE_HIT );
	}

	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		const CX2UserSkillTree::SkillSlotData* pSkillSlotData = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillSlot( CX2SkillTree::SI_SA_SGM_CRITICAL_DIVE );
		if( NULL != pSkillSlotData )
		{
			float fMPConsume = pSkillSlotData->m_fMPConsumption / 2.f;
			UpNowMp(fMPConsume);
			if( NULL != g_pData->GetPicCharBlue() )
			{
				WCHAR wszText[64] = L"";
				StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"MP+ %d", (int)fMPConsume );

				D3DXVECTOR3 pos = GetPos();
				pos.y += 50.f;

				g_pData->GetPicCharBlue()->DrawText( wszText, pos, GetDirVector(), CKTDGPicChar::AT_CENTER );
			}
		}
		
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
}


// 크리티컬 다이브 사용 중 적이 맞았을 때
void CX2GUEL::ESI_SA_SGM_CRITICAL_DIVE_HIT_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2b: June 32b609e0
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}


// 저거넛 버스터, 시작

void CX2GUEL::ESI_SA_SGM_JUGGERNAUT_BUSTER_START_Init()		
{
	TextureReadyInBackground(L"Arme_Ring2.dds");
	XSkinMeshReadyInBackground(L"JUGGERNAUT_BUSTER_END_TRACE.X");
	TextureReadyInBackground(L"RingAlpha.dds");
	TextureReadyInBackground(L"SGM_D_J_C_XZ_Landing2.dds");
	TextureReadyInBackground(L"title_StarLight_Smash_spark.dds");
	XSkinMeshReadyInBackground(L"JUGGERNAUT_BUSTER_ATTACK_BOX.X");
	XSkinMeshReadyInBackground(L"JUGGERNAUT_BUSTER_LOOP_TRACE2.X");
	XSkinMeshReadyInBackground(L"JUGGERNAUT_BUSTER_READY_TRACE2.X");
}
void CX2GUEL::ESI_SA_SGM_JUGGERNAUT_BUSTER_START_StateStart()		
{
	m_fJuggernautBusterNowLoopTime = 0;
	m_bJuggernautBusterIsStartDirectionRight = GetIsRight();
	CommonStateStart();
}
void CX2GUEL::ESI_SA_SGM_JUGGERNAUT_BUSTER_START_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP );
	}
	else
	{
		if( true == IsOnSomethingFuture() )
		{
			m_PhysicParam.nowSpeed.y = 0.f;	
		}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * JUGGERNAUT_BUSTER_START_SPEED_RATE;	// char-elesis CH3a: June 32b6f550 @32b6f5b4
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 0.5f;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		if( m_InputData.pureRight == true )
		{
			if ( GetIsRight() == false ) 
				m_PhysicParam.nowSpeed.x = m_PhysicParam.nowSpeed.x * -1;
		}
		else if( m_InputData.pureLeft == true )
		{
			if ( GetIsRight() == true ) 
				m_PhysicParam.nowSpeed.x = m_PhysicParam.nowSpeed.x * -1;
		}
		else
		{
			m_PhysicParam.nowSpeed.x = 0.0f;
		}
	}

	CommonEventProcess();
}


// 저거넛 버스터, 루프
void CX2GUEL::ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_EventProcess()
{
	if ( _CONST_EL_::MAX_JUGGERNAUT_BUSTER_LOOP_TIME < m_fJuggernautBusterNowLoopTime)
	{
		StateChange( ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST );
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP );
	}
	else
	{
		if( true == IsOnSomethingFuture() )
		{
			m_PhysicParam.nowSpeed.y = 0.f;	
		}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * JUGGERNAUT_BUSTER_LOOP_SPEED_RATE;	// char-elesis CH3a: June 32b6f620 @32b6f6aa
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 0.7f;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
		if( m_InputData.pureRight == true )
		{
			if ( GetIsRight() == false ) 
				m_PhysicParam.nowSpeed.x = m_PhysicParam.nowSpeed.x * -1;
		}
		else if( m_InputData.pureLeft == true )
		{
			if ( GetIsRight() == true ) 
				m_PhysicParam.nowSpeed.x = m_PhysicParam.nowSpeed.x * -1;
		}
		else
		{
			m_PhysicParam.nowSpeed.x = 0.0f;
		}
	}

	CommonEventProcess();
}

void CX2GUEL::ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_FrameMove()
{
	if ( false == m_FrameDataNow.syncData.bFrameStop )
		m_fJuggernautBusterNowLoopTime = m_fJuggernautBusterNowLoopTime + m_fElapsedTime;

	CommonFrameMove();
}


// 저거넛 버스터, 루프 끝
void CX2GUEL::ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_SA_SGM_JUGGERNAUT_BUSTER_END );
	}

	CommonEventProcess();
}

void CX2GUEL::ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST_StartEnd()
{
	m_FrameDataNow.syncData.bIsRight = m_bJuggernautBusterIsStartDirectionRight;
	CommonStateEnd();
}


// 저거넛 버스터, 끝
void CX2GUEL::ESI_SA_SGM_JUGGERNAUT_BUSTER_END_EventProcess()
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
{	// char-elesis CH3a: June 32b60d10 ( the walk / back-walk / dash / ZX cancels need the foot on a line )
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}


// 전장의 숨결

#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
void CX2GUEL::ESI_SA_SGM_WAR_PRELUDE_Init()		
{
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"ColorBallGray.dds");
	TextureReadyInBackground(L"Condense_Pulse01.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"Mesh_Elsword_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"Pulsewave01.dds");
	XSkinMeshReadyInBackground(L"SA_SGM_WAR_PRELUDE_AURA.X");
	TextureReadyInBackground(L"SA_SGM_WAR_PRELUDE_Map01.dds");
	TextureReadyInBackground(L"SGM_D_J_C_XZ_Landing2.dds");
}

void CX2GUEL::ESI_SA_SGM_WAR_PRELUDE_EventProcess()
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
{	// char-elesis CH3a: June 32b61010 ( the state goes at once to the air or the ground state; the air one needs the Dignity of Grand Master level )
	if( false == m_FrameDataFuture.unitCondition.bFootOnLine && m_usDignityOfGrandMasterLevel > 0 )
		StateChange( ESI_SA_SGM_WAR_PRELUDE_FLY );
	else
		StateChange( ESI_SA_SGM_WAR_PRELUDE_GROUND );

	CommonEventProcess();
}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}


// 소드 오브 렐릭스

#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
void CX2GUEL::ESI_HA_SGM_SWORD_OF_RELICS_Init()		
{
	TextureReadyInBackground(L"ARA_SA_FallingDragon_down.DDS");
	TextureReadyInBackground(L"AeroTornado04.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"BlackImage.dds");
	TextureReadyInBackground(L"ColorBallGray.dds");
	TextureReadyInBackground(L"Condense_Pulse01.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Explosion_Light02.dds");
	TextureReadyInBackground(L"GroundShockWave.dds");
	TextureReadyInBackground(L"HELLPUTT_COMET_CRASHER_Explosion_02.tga");
	XSkinMeshReadyInBackground(L"Lire_SI_SA_Gungnir_Mesh05.X");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	TextureReadyInBackground(L"Mesh_Elsword_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"Mesh_Raven_Event_AC_Upbody21_Effect.tga");
	TextureReadyInBackground(L"PROTECT_GUARD_PHYSIC_Attack_B_Slash.tga");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"Pulsewave01.dds");
	XSkinMeshReadyInBackground(L"SA_SGM_SWORD_OF_RELICS_Hand_M01.X");
	TextureReadyInBackground(L"SA_SGM_SWORD_OF_RELICS_MAGIC01.tga");
	TextureReadyInBackground(L"SA_SGM_SWORD_OF_RELICS_MAGIC02.tga");
	XSkinMeshReadyInBackground(L"SA_SGM_SWORD_OF_RELICS_SWORD_EX3.X");
	XSkinMeshReadyInBackground(L"SA_SGM_SWORD_OF_RELICS_SWORD_EX4.X");
	TextureReadyInBackground(L"SA_SGM_WAR_PRELUDE_Map01.dds");
	TextureReadyInBackground(L"SGM_D_J_C_XZ_Landing2.dds");
	TextureReadyInBackground(L"WhiteImage.dds");
	TextureReadyInBackground(L"WhitePoint.dds");
	TextureReadyInBackground(L"WhitePointSmall.dds");
	TextureReadyInBackground(L"stone_AirShip.dds");
	TextureReadyInBackground(L"title_StarLight_Smash_spark.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_100x50x50_tall2.x");
}

void CX2GUEL::ESI_HA_SGM_SWORD_OF_RELICS_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 스킬 스테이트 함수 - 2-2차 블레이징 하트
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 콤보 XZ
void CX2GUEL::ESI_COMBO_SBH_XZ_Init()
{
	TextureReadyInBackground(L"secretVelder_perpit_fire_flame.dds");
}
void CX2GUEL::ESI_COMBO_SBH_XZ_EventProcess()					
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1f2: June 32b61350 ( no class test on the Z press, JUNE_BUGS 632 )
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneZ, ESI_COMBO_SBH_XZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true && CX2Unit::UC_ELESIS_BLAZING_HEART == GetUnitClass() ,
		ESI_COMBO_SBH_XZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ESI_COMBO_SBH_XZ_FrameMove()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1f2: June 32b78390 ( 2 MP for the fireball effect set )
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.damage.fMagic *= GetFireBallPowerRate();
		if( true == FlushMp( XZ_FIRE_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SBH_Combo_XZ_Fire_NEW", this, NULL, IsHyperState(), GetPowerRate() * GetFireBallPowerRate() );
	}

	CommonFrameMove();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.damage.fMagic *= GetFireBallPowerRate();
	}

	CommonFrameMove();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
// 콤보 XZZ
void CX2GUEL::ESI_COMBO_SBH_XZZ_Init()
{
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	XSkinMeshReadyInBackground(L"SI_SA_SBH_WALL_OF_PROMINENCE_02.X");
	TextureReadyInBackground(L"star2.dds");
	TextureReadyInBackground(L"Arme_Damage_Ring.dds");
	TextureReadyInBackground(L"Arme_Damage_Center.dds");
	TextureReadyInBackground(L"Arme_Critical.dds");
}
void CX2GUEL::ESI_COMBO_SBH_XZZ_EventProcess()					
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1f2: June 32b61660 ( no class test on the Z press, JUNE_BUGS 632 )
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneZ, ESI_COMBO_SBH_XZZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true && CX2Unit::UC_ELESIS_BLAZING_HEART == GetUnitClass() ,
		ESI_COMBO_SBH_XZZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ESI_COMBO_SBH_XZZ_FrameMove()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1f2: June 32b785b0 ( the fireball effect set after the March one, 2 MP )
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Elesis_SBH_Combo_XZZ", this, NULL, IsHyperState(), GetPowerRate() * GetFireBallPowerRate() );
		if( true == FlushMp( XZ_FIRE_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SBH_Combo_XZZ_Fire_NEW", this, NULL, IsHyperState(), GetPowerRate() * GetFireBallPowerRate() );
	}

	CommonFrameMove();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Elesis_SBH_Combo_XZZ", this, NULL, IsHyperState(), GetPowerRate() * GetFireBallPowerRate() );
	}

	CommonFrameMove();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
// 콤보 XZZZ
void CX2GUEL::ESI_COMBO_SBH_XZZZ_Init()
{
	TextureReadyInBackground(L"secretVelder_perpit_fire_flame.dds");
}
void CX2GUEL::ESI_COMBO_SBH_XZZZ_EventProcess()					
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1f2: June 32b61970
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ESI_COMBO_SBH_XZZZ_FrameMove()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1f2: June 32b78920 ( 2 MP for the fireball effect set )
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.damage.fMagic *= GetFireBallPowerRate();
		if( true == FlushMp( XZ_FIRE_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SBH_Combo_XZZZ_Fire_NEW", this, NULL, IsHyperState(), GetPowerRate() * GetFireBallPowerRate() );
	}

	CommonFrameMove();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.damage.fMagic *= GetFireBallPowerRate();
	}

	CommonFrameMove();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
// 대시 콤보 X
void CX2GUEL::ESI_DASH_COMBO_SBH_X_Init()
{
	XSkinMeshReadyInBackground(L"C_SPK_Fire_Ball_FX.X");
	TextureReadyInBackground(L"secretVelder_perpit_fire_flame.dds");
	XSkinMeshReadyInBackground(L"EffectSet_SBH_FireBall01.X");
	XSkinMeshReadyInBackground(L"EffectSet_SBH_FireBall02.X");
	XSkinMeshReadyInBackground(L"EffectSet_SBH_FireBall03.X");
	TextureReadyInBackground(L"star2.dds");
	TextureReadyInBackground(L"Arme_Damage_Ring.dds");
	TextureReadyInBackground(L"Arme_Damage_Center.dds");
	TextureReadyInBackground(L"Arme_Critical.dds");
	TextureReadyInBackground(L"Arme_smoke.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.x");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_06.dds");
	TextureReadyInBackground(L"ColorBallFire.dds");
	XMeshReadyInBackground(L"Blur_Light_Mesh01.y");
}
void CX2GUEL::ESI_DASH_COMBO_SBH_X_EventProcess()				
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1f2: June 32b61bd0
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
			( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
		{
			StateChangeDashIfPossible();
		}
		else if ( true == IsOnSomethingFuture() )
		{
			StateChange( USI_DASH_JUMP_LANDING );
		}
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX, ESI_DASH_COMBO_SBH_XX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		true == m_InputData.oneZ, ESI_DASH_COMBO_SBH_XXXZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
			( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
		{
			StateChangeDashIfPossible();
		}
		else if ( true == IsOnSomethingFuture() )
		{
			StateChange( USI_DASH_JUMP_LANDING );
		}
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneX == true, ESI_DASH_COMBO_SBH_XX )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, m_InputData.oneZ == true, ESI_DASH_COMBO_SBH_XXXZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ESI_DASH_COMBO_SBH_X_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.135f ) == true  )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.135f ) == true && EventCheck( 0.135f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if(	true == FlushMp( _CONST_EL_::SPK_DASH_JUMP_XZ_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Elesis_SBH_Dash_Combo_X", this, NULL, IsHyperState(), GetPowerRate() * GetFireBallPowerRate() );
		else
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Elesis_SBH_Combo_XZZ_FAIL", this );
	}

	CommonFrameMove();
}
void CX2GUEL::ESI_DASH_COMBO_SBH_X_FrameMoveFuture()
{
	CommonFrameMoveFuture();

	m_PhysicParam.passiveSpeed.x = GetDashSpeed();
}

// 대시 콤보 XX	
void CX2GUEL::ESI_DASH_COMBO_SBH_XX_Init()
{
	XSkinMeshReadyInBackground(L"C_SPK_Fire_Ball_FX.X");
	TextureReadyInBackground(L"secretVelder_perpit_fire_flame.dds");
	XSkinMeshReadyInBackground(L"EffectSet_SBH_FireBall01.X");
	XSkinMeshReadyInBackground(L"EffectSet_SBH_FireBall02.X");
	XSkinMeshReadyInBackground(L"EffectSet_SBH_FireBall03.X");
	TextureReadyInBackground(L"star2.dds");
	TextureReadyInBackground(L"Arme_Damage_Ring.dds");
	TextureReadyInBackground(L"Arme_Damage_Center.dds");
	TextureReadyInBackground(L"Arme_Critical.dds");
	TextureReadyInBackground(L"Arme_smoke.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.x");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_06.dds");
	TextureReadyInBackground(L"ColorBallFire.dds");
	XMeshReadyInBackground(L"Blur_Light_Mesh01.y");
}
void CX2GUEL::ESI_DASH_COMBO_SBH_XX_EventProcess()				
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1f2: June 32b61fe0
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
			( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
		{
			StateChangeDashIfPossible();
		}
		else if ( true == IsOnSomethingFuture() )
		{
			StateChange( USI_DASH_JUMP_LANDING );
		}
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX, ESI_DASH_COMBO_SBH_XXX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		true == m_InputData.oneZ, ESI_DASH_COMBO_SBH_XXXZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
			( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
		{
			StateChangeDashIfPossible();
		}
		else if ( true == IsOnSomethingFuture() )
		{
			StateChange( USI_DASH_JUMP_LANDING );
		}
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneX == true, ESI_DASH_COMBO_SBH_XXX )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, m_InputData.oneZ == true, ESI_DASH_COMBO_SBH_XXXZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ESI_DASH_COMBO_SBH_XX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.363f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.363f ) == true && EventCheck( 0.363f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if(	true == FlushMp( _CONST_EL_::SPK_DASH_JUMP_XZ_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Elesis_SBH_Dash_Combo_XX", this, NULL, IsHyperState(), GetPowerRate() * GetFireBallPowerRate() );
		else
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Elesis_SBH_Combo_XZZ_FAIL", this );
	}

	CommonFrameMove();
}
void CX2GUEL::ESI_DASH_COMBO_SBH_XX_FrameMoveFuture()
{
	CommonFrameMoveFuture();

	m_PhysicParam.passiveSpeed.x = GetDashSpeed();
}

// 대시 콤보 XXX
void CX2GUEL::ESI_DASH_COMBO_SBH_XXX_Init()
{
	XSkinMeshReadyInBackground(L"C_SPK_Fire_Ball_FX.X");
	TextureReadyInBackground(L"secretVelder_perpit_fire_flame.dds");
	XSkinMeshReadyInBackground(L"EffectSet_SBH_FireBall01.X");
	XSkinMeshReadyInBackground(L"EffectSet_SBH_FireBall02.X");
	XSkinMeshReadyInBackground(L"EffectSet_SBH_FireBall03.X");
	TextureReadyInBackground(L"star2.dds");
	TextureReadyInBackground(L"Arme_Damage_Ring.dds");
	TextureReadyInBackground(L"Arme_Damage_Center.dds");
	TextureReadyInBackground(L"Arme_Critical.dds");
	TextureReadyInBackground(L"Arme_smoke.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.x");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_06.dds");
	TextureReadyInBackground(L"ColorBallFire.dds");
	XMeshReadyInBackground(L"Blur_Light_Mesh01.y");
}
void CX2GUEL::ESI_DASH_COMBO_SBH_XXX_EventProcess()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1f2: June 32b623f0 ( the Z press also fires at animation end; no cancels, as March )
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
			( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
		{
			StateChangeDashIfPossible();
		}
		else if ( true == IsOnSomethingFuture() )
		{
			StateChange( USI_DASH_JUMP_LANDING );
		}
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneZ, ESI_DASH_COMBO_SBH_XXXZ )

	CommonEventProcess();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
			( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
		{
			StateChangeDashIfPossible();
		}
		else if ( true == IsOnSomethingFuture() )
		{
			StateChange( USI_DASH_JUMP_LANDING );
		}
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, ESI_DASH_COMBO_SBH_XXXZ )
	/*SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )*/

	CommonEventProcess();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::ESI_DASH_COMBO_SBH_XXX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.492f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.492f ) == true && EventCheck( 0.492f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if(	true == FlushMp( _CONST_EL_::SPK_DASH_JUMP_XZ_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Elesis_SBH_Dash_Combo_XXX", this, NULL, IsHyperState(), GetPowerRate() * GetFireBallPowerRate() );
		else
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Elesis_SBH_Combo_XZZ_FAIL", this );
	}

	CommonFrameMove();
}
void CX2GUEL::ESI_DASH_COMBO_SBH_XXX_FrameMoveFuture()
{
	CommonFrameMoveFuture();

	m_PhysicParam.passiveSpeed.x = GetDashSpeed();
	m_fDashComboStartDelay = _CONST_EL_::SBM_DASH_COMBO_DELAY_TIME;
}

// 대시 콤보 X 중 Z
void CX2GUEL::ESI_DASH_COMBO_SBH_XXXZ_Init()
{
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"ColorBallFire.dds");
	TextureReadyInBackground(L"EL_FIRE_09.dds");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_06.dds");
	TextureReadyInBackground(L"star2.dds");
	TextureReadyInBackground(L"Arme_Damage_Ring.dds");
	TextureReadyInBackground(L"Arme_Damage_Center.dds");
	TextureReadyInBackground(L"Arme_Critical.dds");
}
void CX2GUEL::ESI_DASH_COMBO_SBH_XXXZ_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
			( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
		{
			StateChangeDashIfPossible();
		}
		else if ( true == IsOnSomethingFuture() )
		{
			StateChange( USI_DASH_JUMP_LANDING );
		}
		else if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}
	/*SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )*/

	CommonEventProcess();
}
void CX2GUEL::ESI_DASH_COMBO_SBH_XXXZ_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.27f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.27f ) == true && EventCheck( 0.27f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Elesis_SBH_Combo_XXXZ", this, NULL, IsHyperState(), GetPowerRate() * GetFireBallPowerRate() );
	}

	CommonFrameMove();
}
void CX2GUEL::ESI_DASH_COMBO_SBH_XXXZ_FrameMoveFuture()
{
	CommonFrameMoveFuture();

	m_PhysicParam.passiveSpeed.x = GetDashSpeed();
}

// 홍련의 검
void CX2GUEL::ESI_SI_A_SBH_FLAME_SWORD_Init()
{
	TextureReadyInBackground(L"Rose_explosion_01.Tga");
	TextureReadyInBackground(L"secretVelder_perpit_fire_flame.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"Arme_Critical.dds");
	TextureReadyInBackground(L"Rose_explosion_01.Tga");
}
void CX2GUEL::ESI_SI_A_SBH_FLAME_SWORD_EventProcess()			
{
	if( m_pXSkinAnim->GetNowAnimationTime() > 0.235f && GetAttackSuccess(true) == true )
	{
		StateChange( ESI_SI_A_SBH_FLAME_SWORD_FINISH );
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
void CX2GUEL::ESI_SI_A_SBH_FLAME_SWORD_FINISH_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}

// 불놀이
void CX2GUEL::ESI_SI_A_SBH_FIREWORKS_Init()
{
	TextureReadyInBackground(L"EL_FIRE_07.dds");
	TextureReadyInBackground(L"Flame_REVOLVER.dds");
	XSkinMeshReadyInBackground(L"RVC_SA_ignitionCrow_circle.X");
	TextureReadyInBackground(L"Rose_explosion_01.Tga");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.x");
	TextureReadyInBackground(L"secretVelder_perpit_fire_flame.dds");
	TextureReadyInBackground(L"DarkSmoke01.dds");
	TextureReadyInBackground(L"Arme_Critical.dds");
	TextureReadyInBackground(L"raven_beed_hit_circle.DDS");
	TextureReadyInBackground(L"Rose_explosion.Tga");
	XSkinMeshReadyInBackground(L"EffectSet_SA_SBH_BLAZING_DANCE_02.X");
}
void CX2GUEL::ESI_SI_A_SBH_FIREWORKS_EventProcess()				
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2c: June 32b62bf0
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}

// 불꽃 장미
void CX2GUEL::ESI_SI_A_SBH_SCALET_ROSE_Init()
{
	XSkinMeshReadyInBackground(L"RVC_SA_ignitionCrow_circle.X");
	TextureReadyInBackground(L"Rose_explosion_02.Tga");
	TextureReadyInBackground(L"WhitePointSmall.dds");
	XSkinMeshReadyInBackground(L"WindBlade_MagicSquare03.X");
	XSkinMeshReadyInBackground(L"SI_A_SBH_SCALET_ROSE_attackSphere.X");
	TextureReadyInBackground(L"Rose_explosion_01.Tga");
	TextureReadyInBackground(L"Rose_explosion.Tga");
	TextureReadyInBackground(L"DarkSmoke01.dds");
	TextureReadyInBackground(L"Arme_Critical.dds");
	TextureReadyInBackground(L"secretVelder_perpit_fire_flame.dds");
	TextureReadyInBackground(L"raven_beed_hit_circle.DDS");
}
void CX2GUEL::ESI_SI_A_SBH_SCALET_ROSE_EventProcess()			
{
	if( m_pXSkinAnim->GetNowAnimationTime() > 0.33f && m_pXSkinAnim->GetNowAnimationTime() < 0.465f )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_hSBHScaletRoseDamageEffect ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( CX2DamageEffect::CEffect* pEffect = m_pSBHScaletRoseDamageEffect != NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			if( pEffect->GetDamageTimeNow() > 0 )
			{
				StateChange( ESI_SI_A_SBH_SCALET_ROSE_SUCCESS );
			}
		}
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
void CX2GUEL::ESI_SI_A_SBH_SCALET_ROSE_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.25f ) == true  )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.25f ) == true && EventCheck( 0.25f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vDamageEffectPos = GetPos();
		vDamageEffectPos.y += 90.f;
		if(GetIsRight() == true)
		{
			vDamageEffectPos += (30.f * GetDirVector());
		}
		else
		{
			vDamageEffectPos -= (30.f * GetDirVector());
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_hSBHScaletRoseDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( this, L"DamageEffect_Elesis_A_SBH_SCALET_ROSE_Start", GetPowerRate(), vDamageEffectPos, GetRotateDegree(), GetRotateDegree() );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_pSBHScaletRoseDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_Elesis_A_SBH_SCALET_ROSE_Start", GetPowerRate(), vDamageEffectPos, GetRotateDegree(), GetRotateDegree() );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.465f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.465f ) == true && EventCheck( 0.465f , false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hSBHScaletRoseDamageEffect );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pSBHScaletRoseDamageEffect != NULL )
		{
			g_pX2Game->GetDamageEffect()->DestroyInstance( m_pSBHScaletRoseDamageEffect );
			m_pSBHScaletRoseDamageEffect = NULL;
		}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}

	CommonFrameMove();
}
void CX2GUEL::ESI_SI_A_SBH_SCALET_ROSE_StateEnd()
{
	CommonStateEnd();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hSBHScaletRoseDamageEffect );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pSBHScaletRoseDamageEffect != NULL && NULL != g_pX2Game->GetDamageEffect() )
	{
		g_pX2Game->GetDamageEffect()->DestroyInstance( m_pSBHScaletRoseDamageEffect );
		m_pSBHScaletRoseDamageEffect = NULL;
	}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
}
void CX2GUEL::ESI_SI_A_SBH_SCALET_ROSE_SUCCESS_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}

// 스칼딕 소드
void CX2GUEL::ESI_SI_SA_SBH_SCALDIC_SWORD_Init()
{
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.x");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	XSkinMeshReadyInBackground(L"P_EBS_BoostDash_circle.X");
	TextureReadyInBackground(L"Arme_Critical.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	XSkinMeshReadyInBackground(L"RVC_SA_DeadlyRaid_circleLine.X");
	TextureReadyInBackground(L"Rose_explosion_01.Tga");
	XMeshReadyInBackground(L"SCALDIC_SWORD_01.Y");
	XMeshReadyInBackground(L"SCALDIC_SWORD_03.Y");
	TextureReadyInBackground(L"secretVelder_perpit_fire_flame.dds");
}
void CX2GUEL::ESI_SI_SA_SBH_SCALDIC_SWORD_EventProcess()		
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )	// char-elesis CH2c: June 32b631f0
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL

	CommonEventProcess();
}

// 불꽃의 결계
void CX2GUEL::ESI_SI_SA_SBH_WALL_OF_PROMINENCE_Init()
{
	TextureReadyInBackground(L"GroundShockWave02.dds");
	TextureReadyInBackground(L"Rose_explosion_01.Tga");
	TextureReadyInBackground(L"WhitePointSmall.dds");
	TextureReadyInBackground(L"smoke02.dds");
	XSkinMeshReadyInBackground(L"BREAKING_FIST_Attack_Box.X");
	XSkinMeshReadyInBackground(L"DummyAttackBox_100x50x50_tall2.x");
	TextureReadyInBackground(L"secretVelder_perpit_fire_flame.dds");
	XSkinMeshReadyInBackground(L"SI_SA_SBH_WALL_OF_PROMINENCE_01.X");
}
void CX2GUEL::ESI_SI_SA_SBH_WALL_OF_PROMINENCE_EventProcess()	
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
void CX2GUEL::ESI_SI_SA_SBH_WALL_OF_PROMINENCE_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.454f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.454f ) == true && EventCheck( 0.454f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vHandPos = GetBonePos(L"Bip01");
		CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, 
			L"DamageEffect_Wall_of_Prominence_Projectile", GetPowerRate(), vHandPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		if ( NULL != pDamageEffect )
		{
			CX2DamageEffect::DamageEffectDataInLua& DamageEffectData = pDamageEffect->GetDamageEffectDataInLua();

			if( 1 < DamageEffectData.m_vecDieDamageEffect.size() )
			{
				DamageEffectData.m_vecDieDamageEffect[0]->fLifeTime = m_fWallOfProminenceLifeTime;
				DamageEffectData.m_vecDieDamageEffect[1]->fLifeTime = m_fWallOfProminenceLifeTime;
			}

			if( 1 < DamageEffectData.m_vecHitDamageEffect.size() )
			{
				DamageEffectData.m_vecHitDamageEffect[0]->fLifeTime = m_fWallOfProminenceLifeTime;
				DamageEffectData.m_vecHitDamageEffect[1]->fLifeTime = m_fWallOfProminenceLifeTime;
			}
		}
	}

	CommonFrameMove();
}

// 블레이즈 윙
void CX2GUEL::ESI_SI_SA_SBH_BLAZE_WING_Init()
{
	TextureReadyInBackground(L"Black_Light.dds");
	TextureReadyInBackground(L"Condense_Light01.dds");
	TextureReadyInBackground(L"Rose_explosion_01.Tga");
	TextureReadyInBackground(L"smoke02.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	XSkinMeshReadyInBackground(L"SI_SA_SBH_BLAZE_WING_02.X");
	TextureReadyInBackground(L"secretVelder_perpit_fire_flame.dds");
	TextureReadyInBackground(L"star2.dds");
	TextureReadyInBackground(L"Arme_Damage_Ring.dds");
	TextureReadyInBackground(L"Arme_Damage_Center.dds");
	TextureReadyInBackground(L"Arme_Critical.dds");
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZE_WING_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}

// 블레이징 댄스
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_START_Init()
{
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"Condense_Light01.dds");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	TextureReadyInBackground(L"PET_SILVERFOX_MIHOEffect02.tga");
	TextureReadyInBackground(L"raven_beed_hit_circle.DDS");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"Arme_Critical.dds");
	TextureReadyInBackground(L"Rose_explosion_01.Tga");
	TextureReadyInBackground(L"star2.dds");
	TextureReadyInBackground(L"Arme_Damage_Ring.dds");
	TextureReadyInBackground(L"Arme_Damage_Center.dds");
	TextureReadyInBackground(L"DarkSmoke01.dds");
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_START_StateStart()
{
	CommonStateStart();

	m_vecBlazingDanceTargetUid.clear();
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_START_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_SI_SA_SBH_BLAZING_DANCE_1ST );
	}

	CommonEventProcess();
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_1ST_StartFuture()
{
	CommonStateStartFuture();

	BlazingDanceFindTargetAndMove();
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_1ST_EventProcess()
{
	if( true == m_bIsThereBlazingDanceTarget && true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_SI_SA_SBH_BLAZING_DANCE_2ND );
	}
	else if( false == m_bIsThereBlazingDanceTarget )
	{
		StateChange( ESI_SI_SA_SBH_BLAZING_DANCE_FINISH );
	}

	CommonEventProcess();
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_2ND_StartFuture()
{
	CommonStateStartFuture();

	BlazingDanceFindTargetAndMove();
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_2ND_EventProcess()
{
	if( true == m_bIsThereBlazingDanceTarget && true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_SI_SA_SBH_BLAZING_DANCE_3RD );
	}
	else if( false == m_bIsThereBlazingDanceTarget )
	{
		StateChange( ESI_SI_SA_SBH_BLAZING_DANCE_FINISH );
	}

	CommonEventProcess();
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_3RD_StartFuture()
{
	CommonStateStartFuture();

	BlazingDanceFindTargetAndMove();
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_3RD_EventProcess()
{
	if( true == m_bIsThereBlazingDanceTarget && true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_SI_SA_SBH_BLAZING_DANCE_4TH );
	}
	else if( false == m_bIsThereBlazingDanceTarget )
	{
		StateChange( ESI_SI_SA_SBH_BLAZING_DANCE_FINISH );
	}

	CommonEventProcess();
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_4TH_StartFuture()
{
	CommonStateStartFuture();

	BlazingDanceFindTargetAndMove();
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_4TH_EventProcess()
{
	if( false == m_bIsThereBlazingDanceTarget || ( true == m_bIsThereBlazingDanceTarget && true == m_pXSkinAnimFuture->IsAnimationEnd() ) )
	{
		StateChange( ESI_SI_SA_SBH_BLAZING_DANCE_FINISH );
	}

	CommonEventProcess();
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_StartFuture()
{
	CommonStateStartFuture();

	m_bIsBlazingDanceLastAttack = true;
	BlazingDanceFindTargetAndMove();
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_StateEnd()
{
	m_bIsBlazingDanceLastAttack = false;
	CommonStateEnd();
}
void CX2GUEL::ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_FrameMoveFuture()
{
	CommonFrameMoveFuture();

	if( NULL != g_pX2Game->GetWorld() &&
		NULL != g_pX2Game->GetWorld()->GetLineMap() )
	{
		const CKTDGLineMap::LineData* pLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( GetLastTouchLineIndex() );
		if( NULL != pLineData )
		{
			D3DXVECTOR3 vFinalDestDir = m_vec3BlazingDanceTargetPos - GetPos();
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
			D3DXVECTOR3 vRightVec = pLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			bool bRight = ( fIsRight > 0.f ) ? true : false;
		
			m_FrameDataFuture.syncData.bIsRight = bRight;
		}
	}
}

// 에인션트 파이어
void CX2GUEL::ESI_SI_HA_SBH_ANCIENT_FIRE_Init()
{
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"ColorBallFire.dds");
	TextureReadyInBackground(L"Condense_Light01.dds");
	TextureReadyInBackground(L"Condense_Pulse01.dds");
	XSkinMeshReadyInBackground(L"EffectSet_SA_SBH_ANCIENT_FIRE01.X");
	XSkinMeshReadyInBackground(L"EffectSet_SA_SBH_ANCIENT_FIRE02.X");
	TextureReadyInBackground(L"EffectSet_SA_SBH_ANCIENT_FIRE_03.tga");
	XSkinMeshReadyInBackground(L"Lire_SI_SA_Gungnir_Mesh05.X");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
	TextureReadyInBackground(L"PET_SILVERFOX_MIHOEffect02.tga");
	XSkinMeshReadyInBackground(L"RVC_SA_DeadlyRaid_circleLine.X");
	TextureReadyInBackground(L"SA_SGM_WAR_PRELUDE_Map01.dds");
	TextureReadyInBackground(L"smoke02.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_100x50x50_tall2.x");
	TextureReadyInBackground(L"star1.dds");
	TextureReadyInBackground(L"Arme_Damage_Center.dds");
	TextureReadyInBackground(L"Arme_Critical.dds");
}
void CX2GUEL::ESI_SI_HA_SBH_ANCIENT_FIRE_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
void CX2GUEL::ESI_SI_HA_SBH_ANCIENT_FIRE_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.887f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.887f ) == true && EventCheck( 0.887f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );

		if ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_SA_SBH_ANCIENT_FIRE_Attack01_Pvp", GetPowerRate(), vBonePos, GetRotateDegree(), GetRotateDegree() );
		else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_SA_SBH_ANCIENT_FIRE_Attack01_Dungeon", GetPowerRate(), vBonePos, GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.177f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.177f ) == true && EventCheck( 2.177f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		SetLayer(XL_EFFECT_2);	
		m_bSetCustomRenderParam = true;
		m_RenderParam.color = 0xff000000;
		SetAlphaObject(true);
		m_fAncientFireRenderTime = 2.4f;
	}

	CommonFrameMove();
}


#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트








///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 패시브 함수
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** @function : ChivalryProc_FrameMove
	@brief : 패시브 스킬 - 기사의 숙명 처리 프레임 무브
			 - 해당 버프가 안 걸렸을 때만 체크하여 버프 걸기
*/

void CX2GUEL::ChivalryProc_FrameMove()
{
	// 기사의 숙명을 배우지 않았다면 처리하지 않기
	if( 0 == m_uiChivalryLevel )
		return;

	// 1초 간격으로 처리
	m_fChivalryBuffCheckTime += m_fElapsedTime;
	if( m_fChivalryBuffCheckTime < 1.f )
	{
		return;
	}

	m_fChivalryBuffCheckTime = 0.f;
	// 공격력 버프 적용 여부 체크
	{
		/// 김현철 // 2013-07-18
		// A 클라이언트가 이 함수의 호출자, B 클라이언트가 파티원인 경우
		// B 의 HP가 특정 % 이하로 내려가면 true를 리턴 해야 하나
		// A 클라이언트에서는 B가 파티원이지만
		// B 클라이언트에서는 B가 파티원이 아닌 자기 자신이기 때문에 따로 체크 해야 함
		// 또한 파티원이 아닌 유저의 클라이언트에서는 체크 자체가 안됨
		if( true == g_pX2Game->GetIsExsitancePartyMemberHPLessThanPer( _CONST_EL_::CHIVALRY_BUFF_CONDITION_HP_RATE, this ) )
		{
			// 공격력 버프 적용
			if( 0 < m_uiChivalryAttackBuffFactorID )
			SetBuffFactorToGameUnitByBuffFactorID( m_uiChivalryAttackBuffFactorID, m_uiChivalryLevel);
		}
	}

	// 방어력 버프 적용 여부 체크
	{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
		// char-elesis CH1j: June 32b7ac20 ( in a PvP game state the defence buff condition is 0.25 )
		if( ( CX2Main::XS_PVP_GAME == g_pMain->GetNowStateID() ? CHIVALRY_DEFENCE_HP_RATE_PVP : CHIVALRY_DEFENCE_HP_RATE ) >= ( GetNowHp() / GetMaxHp() ) )
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
		if(  _CONST_EL_::CHIVALRY_BUFF_CONDITION_HP_RATE >= (GetNowHp() / GetMaxHp()) )
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
		{
			// 방어력 버프 적용
			if( 0 < m_uiChivalryDefenceBuffFactorID )
				SetBuffFactorToGameUnitByBuffFactorID( m_uiChivalryDefenceBuffFactorID, m_uiChivalryLevel );
		}
	}
}

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
void CX2GUEL::StigmaOfFireProc_FrameMove()
{
	// 불꽃의 낙인이 충전되었는데 사용되지 않았다면 return
	// 스킬을 배우지 않았다면 사용되지 않을것이므로 스킬을 배웠는지의 체크도 가능.
	if( true == m_bIsStigmaOfFire )
		return;

	m_fStigmaOfFireCoolTime += m_fElapsedTime;
	if( m_fStigmaOfFireCoolTime < 0.5f )
	{
		m_bIsStigmaOfFire = false;
		return;
	}

	m_bIsStigmaOfFire = true;
}

void CX2GUEL::ApplyDebuffStigmaOfFire( const CX2DamageManager::DamageData& damageData_ )
{
	//불꽃의 낙인 스킬은 스폐셜, 하이퍼 액티브에는 반응하지 않는다. 0.5초의 쿨타임이 지나야 다시 사용 가능하다.
	if( CX2DamageManager::AT_NORMAL == damageData_.attackType && true == m_bIsStigmaOfFire && 0 < m_uiSkillLevelStigmaOfFire )
	{	
		if( null != damageData_.optrDefenderGameUnit )
		{
			CX2GameUnit* pDefenderUnit = damageData_.optrDefenderGameUnit.GetObservable();
			if( NULL != pDefenderUnit )
			{
				pDefenderUnit->SetBuffFactorToGameUnitByBuffFactorID(static_cast<UINT>(BFI_DEBUFF_STIGMA_OF_FIRE), m_uiSkillLevelStigmaOfFire );
				//불꽃의 낙인 버프의 중첩 횟수를 검사.
				//중첩이 5가되면 불꽃의 심판 버프를 걸어준다.
				vector<KBuffFactor> VecDebuffFactor;
				pDefenderUnit->GetBuffFactorOnlyDebuff( VecDebuffFactor );


				BOOST_FOREACH( const KBuffFactor& kFactor_, VecDebuffFactor )
				{
					if( kFactor_.m_BuffIdentity.m_eBuffTempletID == BTI_DEBUFF_STIGMA_OF_FIRE && kFactor_.m_ucAccumulationCountNow > 4 )
					{
						pDefenderUnit->SetBuffFactorToGameUnitByBuffFactorID(static_cast<UINT>(BFI_DEBUFF_JUDGEMENT_FIRE), m_uiSkillLevelStigmaOfFire );
						pDefenderUnit->SetBuffFactorToGameUnitByBuffFactorID(static_cast<UINT>(BFI_DEBUFF_JUDGEMENT_FIRE2), m_uiSkillLevelStigmaOfFire );
						g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_SBH_Judgement_Fire", GetPowerRate() * m_fStigmaOfFireDamageValue, pDefenderUnit->GetPos(), GetRotateDegree(), GetRotateDegree() );
					}
				}
			}
		}	

		m_bIsStigmaOfFire = false; 
		m_fStigmaOfFireCoolTime = 0.0f;
	}
}

void CX2GUEL::ApplyDebuffFlameSword( const CX2DamageManager::DamageData& damageData_ )
{
	//홍련의 검 스킬은 스폐셜, 하이퍼 액티브에는 반응하지 않는다.
	if( CX2DamageManager::AT_NORMAL == damageData_.attackType && true == m_bIsFrameSword )
	{	
		if( null != damageData_.optrDefenderGameUnit )
		{
			CX2GameUnit* pDefenderUnit = damageData_.optrDefenderGameUnit.GetObservable();
			if( NULL != pDefenderUnit )
			{
				pDefenderUnit->SetBuffFactorToGameUnitByBuffFactorID(static_cast<UINT>( BFI_DEBUFF_HIGH_FEVER ), m_uiSkillLevelFlameSword );
				pDefenderUnit->SetBuffFactorToGameUnitByBuffFactorID(static_cast<UINT>( BFI_DEBUFF_FLAME_SWORD ), m_uiSkillLevelFlameSword );
			}
		}	
	}
}

void CX2GUEL::FireBlossomsProc_FrameMove()
{
	// 스킬을 배우지 않았다면 return
	if( m_uiFireBlossomsSkillLevel < 1 )
		return;


	if( true == m_bIsFireBlossoms )
	{
		bool bUse = false;

		// 새롭게 지워줘야 할 BuffTempletID가 설정되었으면 해당 버프 삭제.
		if( m_eFireBlossomsSaveDebuffID != BTI_NONE )
		{
			EraseBuffTempletFromGameUnit( m_eFireBlossomsSaveDebuffID );
			bUse = true;		
		}
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
		// char-elesis CH1j: June 32b87520 ( the weapon enchant is read inline from the Blaze, Frozen and Poison timers; the Shock enchant is not looked at )
		else if( 0.f < m_ExtraDamagePack.m_EnchantBlaze.m_fTime || 0.f < m_ExtraDamagePack.m_EnchantFrozen.m_fTime || 0.f < m_ExtraDamagePack.m_EnchantPoison.m_fTime )
		{
			CX2DamageManager::EXTRA_DAMAGE_TYPE eEnchantType = CX2DamageManager::EDT_ENCHANT_BLAZE;
			if( m_ExtraDamagePack.m_EnchantBlaze.m_fTime < m_ExtraDamagePack.m_EnchantFrozen.m_fTime || m_ExtraDamagePack.m_EnchantBlaze.m_fTime < m_ExtraDamagePack.m_EnchantPoison.m_fTime )
				eEnchantType = ( m_ExtraDamagePack.m_EnchantFrozen.m_fTime < m_ExtraDamagePack.m_EnchantPoison.m_fTime ) ? CX2DamageManager::EDT_ENCHANT_POISON : CX2DamageManager::EDT_ENCHANT_FROZEN;
			AccessExtraDamagePack().Init( eEnchantType );
			bUse = true;
		}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
		else if( CX2DamageManager::EDT_NONE != GetApplyWeaponExtraDamageType() )
		{
			// BuffTempletID가 설정안된 상태라면 현재 내 유닛에 적용된 무기 속성 체크
			AccessExtraDamagePack().Init( GetApplyWeaponExtraDamageType() );
			bUse = true;
		}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
		else
		{
			// 새롭게 BuffTempletID가 설정되지 않았고, 무기속성도 걸리지 않았다면
			// 기존에 걸려있는 디버프 중 하나를 삭제
			vector<KBuffFactor> VecDebuffFactor;
			GetBuffFactorOnlyDebuff( VecDebuffFactor );

			if( false == m_vecBuffTempletPtr.empty() )
			{
				BUFF_TEMPLET_ID eEraseDebuff = BTI_NONE;

				BOOST_FOREACH( CX2BuffTempletPtr ptr, m_vecBuffTempletPtr )
				{
					if ( NULL != ptr && false == ptr->GetDidFinish() && ptr->IsDeBuff() && false == ptr->IsWorldBuff() )
					{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
						// 행동 불능 디버프에 대한 조건 추가
						if ( false == ptr->IsNonControlDebuff() )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
							eEraseDebuff = ptr->GetBuffTempletID();
					}
				}

				if( eEraseDebuff != BTI_NONE )
				{
					EraseBuffTempletFromGameUnit( eEraseDebuff );
					bUse = true;
				}
			}
		}
		

		if( true == bUse )
		{
			if( NULL != g_pData->GetPicCharBlue() )
				g_pData->GetPicCharBlue()->DrawText( L"RESIST", GetHeadBonePos(), GetDirVector(), CKTDGPicChar::AT_CENTER );

			m_eFireBlossomsSaveDebuffID = BTI_NONE;
			m_fFireBlossomsCoolTime = 0.f;
			m_bIsFireBlossoms = false;
			SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_FIRE_BLOSSOMS, m_uiFireBlossomsSkillLevel );	
		}
	}
	else
	{
		// 쿨타임을 체크
		m_fFireBlossomsCoolTime += m_fElapsedTime;
		m_bIsFireBlossoms =  m_fFireBlossomsCoolTime >= m_fFireBlossomsCheckTime ;
	}
}

void CX2GUEL::BlazingDanceFindTargetAndMove()
{
	// 내 유닛이 이동가능한 지점에 있는 타겟의 위치만 찾아 내 유닛을 이동시킴
	// 한번 타겟이 되었던 유닛은 우선순위가 뒤로 밀린다.

	m_bIsThereBlazingDanceTarget = false;
	CX2GameUnit* pDidTargetingUnit = NULL;
	const CKTDGLineMap::LineData* pDidLineData = NULL;
	if ( NULL != g_pX2Game->GetWorld() &&
				NULL != g_pX2Game->GetWorld()->GetLineMap() )
	{
		for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
		{
			CX2GameUnit* pUnit = g_pX2Game->GetUnit( i );
			bool IsDidTargeting = false;
			if( NULL != pUnit )
			{
#ifdef FIELD_BOSS_RAID // 유도 / 타겟 지정에 대한 예외
				if( true == pUnit->GetIsBosRaidNPC() )
					continue;
#endif // FIELD_BOSS_RAID

				const CKTDGLineMap::LineData* pLineData = 
					g_pX2Game->GetWorld()->GetLineMap()->GetLineData( pUnit->GetLastTouchLineIndex() );
			
				if( NULL == pLineData )
					continue;

				if( false == BlazindDanceCheckTargetPosition( pUnit, pLineData ) )
					continue;

				//이미 한번 타겟이 되었던 유닛이라면 일단 저장
				BOOST_FOREACH( UidType uTargetUid_, m_vecBlazingDanceTargetUid )
				{				
					if( uTargetUid_ == pUnit->GetUnitUID() )
					{
						IsDidTargeting = true;
						break;
					}
				}
				if( true == IsDidTargeting )
				{
					pDidTargetingUnit = pUnit;
					pDidLineData = pLineData;
					continue;
				}

				D3DXVECTOR3 MyPos = GetPos();
				D3DXVECTOR3 TargetPos = pUnit->GetPos();
				if( GetDistance3Sq( MyPos, TargetPos ) < 490000.0f )
				{
					m_FrameDataFuture.syncData.position		= TargetPos;
					m_bIsThereBlazingDanceTarget = true;

					m_vecBlazingDanceTargetUid.push_back( pUnit->GetUnitUID() ); //타겟된 유닛의 UID를 저장

					if( true == m_bIsBlazingDanceLastAttack ) //마지막 타격이라면 방향전환을 위해 타겟의 위치를 저장
					{
						m_vec3BlazingDanceTargetPos = TargetPos;
					}
					break;
				}
			}
		}
	}

	// 유닛을 이동시킬 타겟이 없다면 이미 한번 타겟이 되었던 유닛이 있는지 체크하고 해당 위치로 이동.
	if( false == m_bIsThereBlazingDanceTarget && NULL != pDidTargetingUnit && NULL != pDidLineData )
	{
		D3DXVECTOR3 MyPos = GetPos();
		D3DXVECTOR3 TargetPos = pDidTargetingUnit->GetPos();
		if( GetDistance3Sq( MyPos, TargetPos ) < 490000.0f )
		{
			bool bMove = true;

			if( false == BlazindDanceCheckTargetPosition( pDidTargetingUnit, pDidLineData ) )
				bMove = false;

			if( true == bMove )
			{
				m_FrameDataFuture.syncData.position		= TargetPos;
				m_bIsThereBlazingDanceTarget = true;

				if( true == m_bIsBlazingDanceLastAttack ) //마지막 타격이라면 방향전환을 위해 타겟의 위치를 저장
				{
					m_vec3BlazingDanceTargetPos = TargetPos;
				}
			}
		}
	}
}
const bool CX2GUEL::BlazindDanceCheckTargetPosition( CX2GameUnit* pUnit_, const CKTDGLineMap::LineData* pLineData_  )
{
	// 타겟의 상태, 위치가 내 유닛을 이동시키기에 적절한지 체크

	bool bValiedTargetPosition = true;

	
	if( pUnit_->GetTeam() == GetTeam() )	//같은 팀인지 체크
	{
		bValiedTargetPosition = false;
	}
	else if( pUnit_->GetInvincible() == true )	//무적상태인지 체크
	{
		bValiedTargetPosition = false;
	}
	else if( pUnit_->GetNowHp() <= 0.f )	//살아 있는지 체크
	{
		bValiedTargetPosition = false;
	}
	else
	{	
		D3DXVECTOR3 MyPos = GetPos();
		D3DXVECTOR3 TargetPos = pUnit_->GetPos();
		int			lineIndex = 0;
		

		if( NULL == pLineData_ )
		{
			bValiedTargetPosition = false;
		}
		else if( pLineData_->lineType != CKTDGLineMap::LT_NORMAL || pLineData_->bEnable == false ) //라인맵이 유효한지 체크
		{
			bValiedTargetPosition = false;	
		}
		else if( CX2GameUnit::GUT_NPC == pUnit_->GetGameUnitType() )  
		{
			
			CX2GUNPC* pMonster = static_cast<CX2GUNPC*>(pUnit_);

			
			if( NULL != pMonster &&							//공중 AI만 갈 수 있는 위치로 이동하는 문제를 위한 체크,
				NULL != pMonster-> GetNPCAI()&&
				true == pMonster->GetNPCAI()->GetAIFly() &&
				TargetPos.y < pLineData_->startPos.y )
			{
					bValiedTargetPosition = false;		
			}
			else	// 트랩 몬스터 인지 체크, 체크 몬스터인지 체크
			{
				if( CX2UnitManager::NCT_THING_TRAP == pMonster->GetNPCTemplet().m_ClassType ||
					CX2UnitManager::NCT_THING_CHECKER == pMonster->GetNPCTemplet().m_ClassType )
				bValiedTargetPosition = false;
			}
		}
		else if( g_pX2Game->GetWorld()->GetLineMap()->CanDown( MyPos, LINE_RADIUS, &TargetPos, &lineIndex, false ) == false ) //위치 아래에 착지할 수 있는 라인맵이 있는지 체크
		{
			bValiedTargetPosition = false;	
		}
	}

	return bValiedTargetPosition;
}

const bool CX2GUEL::GetUnitClassLine( UNIT_CLASS_LINE UnitClassLine_ )
{
	switch( UnitClassLine_ )
	{
	case CX2GUUser::UCL_FIRST:
		{
			switch( GetUnitClass() )
			{
			case CX2Unit::UC_ELESIS_SABER_KNIGHT:
			case CX2Unit::UC_ELESIS_GRAND_MASTER:
		
				return true;
				break;
			} 
		} break;

	case UCL_SECOND:
		{
			switch( GetUnitClass() )
			{
			case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
			case CX2Unit::UC_ELESIS_BLAZING_HEART:
		
				return true;
				break;
			}
		} break;
	}

	return false;
}


/* virtual */ void CX2GUEL::ChangeDamageData()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1o: June 32b732e0 ( vslot 0x83c ): gated on First Strike learned, not the class; PvP halves the damage bonus
	if ( true == IsFirstStrikeRelateState() && 0 < GetSkillLevelValue( CX2SkillTree::RNSI_P_SSK_FIRST_STRIKE ) )
	{
		CX2GUEL::CFirstStrikeData cFirstStrikeData = GetFirstStrikeData();
		float fAddStopTimeDef = cFirstStrikeData.GetFirstStrikeAddStopTimeDef();
		if ( ESI_JUMP_ATTACK_X == GetNowStateID() )
			fAddStopTimeDef = fAddStopTimeDef * 0.5f;

		m_DamageData.fStopTimeDef = m_DamageData.fStopTimeDef + fAddStopTimeDef;
		m_DamageData.fHitAddMP = m_DamageData.fHitAddMP * cFirstStrikeData.GetFirstStrikeAddIncreaseMPRate();

		float fAddDamageRate = cFirstStrikeData.GetFirstStrikeAddDamage();
		if ( CX2Main::XS_PVP_GAME == g_pMain->GetNowStateID() )
			fAddDamageRate = ( fAddDamageRate - 1.f ) * 0.5f + 1.f;

		m_DamageData.damage.fPhysic = m_DamageData.damage.fPhysic * fAddDamageRate;
	}
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	if ( true == IsFirstStrikeRelateState() && GetUnitClass() == CX2Unit::UC_ELESIS_GRAND_MASTER )
	{
		CX2GUEL::CFirstStrikeData cFirstStrikeData = GetFirstStrikeData();
		float fAddfStopTimeDef = cFirstStrikeData.GetFirstStrikeAddStopTimeDef();
		if ( GetNowStateID() == ESI_JUMP_ATTACK_X )	// 점프 X 커맨드에 대해서만 예외 처리, 경직 시간 0.5배
		{
			fAddfStopTimeDef = fAddfStopTimeDef * 0.5f;
		}
		m_DamageData.fStopTimeDef = m_DamageData.fStopTimeDef + fAddfStopTimeDef;	// 경직 시간 증가 ( 절대값 n초 추가 ) 
		m_DamageData.fHitAddMP = m_DamageData.fHitAddMP * cFirstStrikeData.GetFirstStrikeAddIncreaseMPRate();		// 마나 회수율 증가 ( 배율 증가 ) 		
		m_DamageData.damage.fPhysic = m_DamageData.damage.fPhysic * cFirstStrikeData.GetFirstStrikeAddDamage();		// 데미지 증가 ( 배율 증가 )
	}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}

void CX2GUEL::DrasticChargeProc_FrameMove()
{
	switch ( GetNowStateSkillID() )
	{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
		// char-elesis CH1j: June 32b7b5f0 ( the upgraded skill keeps the loop effects too )
		case CX2SkillTree::RNSI_A_SGM_DRASTIC_CHARGE :
		case CX2SkillTree::RNSI_A_SGM_DRASTIC_CHARGE_UPGRADE :
			break;
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
		case CX2SkillTree::SI_A_SGM_DRASTIC_CHARGE :
			break;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE

		default:
			{
				if ( NULL != g_pX2Game->GetEffectSet() )
				{
					if ( INVALID_EFFECTSET_HANDLE != m_hDrasticChargeLoop )
					{
						g_pX2Game->GetEffectSet()->StopEffectSet( m_hDrasticChargeLoop );
					}					
					if ( INVALID_EFFECTSET_HANDLE != m_hDrasticChargeGroundFire )
					{
						g_pX2Game->GetEffectSet()->StopEffectSet( m_hDrasticChargeGroundFire );
					}
				}
			}
			break;	
	}
}

void CX2GUEL::XX_DOWN_XProc_FrameMove()
{
	switch ( GetNowStateID() )
	{
	case ESI_COMBO_SGM_XX_DOWN_X_START :
	case ESI_COMBO_SGM_XX_DOWN_X_LOOP :
	case ESI_COMBO_SGM_XX_DOWN_X_END :
		break;

	default:
		{
			if ( NULL != g_pX2Game->GetEffectSet() )
			{
				if ( INVALID_EFFECTSET_HANDLE != m_hXXDownXLoop )
				{
					g_pX2Game->GetEffectSet()->StopEffectSet( m_hXXDownXLoop );
				}					
			}
		}
		break;	
	}
}

void CX2GUEL::SnapCrasherProc_FrameMove()
{
	switch ( GetNowStateSkillID() )
	{
	case CX2SkillTree::SI_A_SGM_SNAP_CRASHER :
		break;

	default:
		{
			if ( NULL != g_pX2Game->GetEffectSet() )
			{
				if ( INVALID_EFFECTSET_HANDLE != m_hSnapCresherLoop )
				{
					g_pX2Game->GetEffectSet()->StopEffectSet( m_hSnapCresherLoop );
				}			

				if ( INVALID_EFFECTSET_HANDLE != m_hSnapCreaherLoopAttack )
				{
					g_pX2Game->GetEffectSet()->StopEffectSet( m_hSnapCreaherLoopAttack );
				}

			}
		}
		break;	
	}
}

#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef MODIFY_SET_DAMAGE_DATA
/*virtual*/void CX2GUEL::SetDamageExceptionProcess( OUT CX2DamageManager::DamageData* pDamageData_ ,OUT float& fPowerRate)
{
	CX2GUUser::SetDamageExceptionProcess(pDamageData_, fPowerRate);

#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
	if( WSS_VIGOR == GetWayOfSwordState() )
	{
		switch( pDamageData_->m_WayofSwordType ) 
		{
		// 질풍 커맨드 다운수치 감소
		case CX2DamageManager::WST_VIGOR_NORMAL:
			{
				float fGaleForceDownDecreaseRate = _CONST_EL_::GALE_FORCE_DOWN_DECREASE_RATE;

				// 대전에서는 감소량 1/2
				if( NULL != g_pX2Game && CX2Game::GT_PVP == g_pX2Game->GetGameType())	
					fGaleForceDownDecreaseRate = _CONST_EL_::GALE_FORCE_DOWN_DECREASE_RATE_PVP;

				pDamageData_->fForceDownValue *= fGaleForceDownDecreaseRate;
			} break;
		default:
			break;
		}
	}
	else if( WSS_DESTRUCTION == GetWayOfSwordState() )
	{
		switch( pDamageData_->m_WayofSwordType ) 
		{
		// 섬멸 스킬, 커맨드 방어 무시 적용
		case CX2DamageManager::WST_DESTRUCTION_NORMAL:
		case CX2DamageManager::WST_DESTRUCTION_ACTIVE:
		case CX2DamageManager::WST_DESTRUCTION_SPECIAL:
			{
				pDamageData_->m_bIgnoreStateDefence = true;
			} break;
		default:
			break;
		}
	}
#endif // ELSWORD_WAY_OF_SWORD
}
#endif //MODIFY_SET_DAMAGE_DATA

#ifdef MODIFY_RIDING_PET_AWAKE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
namespace _CONST_EL_CH1C_
{
	// char-elesis CH1c: June 32011d20 / 31fd66b0 / 32011f50 ( the awakening effect-set family ); stand-in: not ported ( Stage 12 ), June's empty-name default
	void ElesisAwakeningEffectBegin()			{}
	bool ElesisAwakeningEffectSetPresent()		{ return false; }
	void ElesisAwakeningEffectPlay()			{}
}
using namespace _CONST_EL_CH1C_;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
void CX2GUEL::RidingHyperModeFrameMove()
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1c: June 32b81200
	CommonHyperModeFrameMove( 0.f, 0.1f, true );
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	CommonHyperModeFrameMove(0.f, 0.1f, true);
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
void CX2GUEL::CommonHyperModeFrameMove( float fTime1_, float fTime2_, bool bSound_ /*= false*/ )
{
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	// char-elesis CH1c: June 32b7b820, two independent timers; the trace only without the awakening set
	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );

	if( true == m_pXSkinAnim->EventTimerOneshot( fTime1_ ) )
	{
		ShowMinorParticleHyperModeChange();
	}

	if( true == m_pXSkinAnim->EventTimerOneshot( fTime2_ ) )
	{
		ElesisAwakeningEffectBegin();
		if( true == ElesisAwakeningEffectSetPresent() )
			ElesisAwakeningEffectPlay();

		ShowMinorParticleHyperModeShock();

		UpDownCrashCamera( 30.0f, 0.4f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

		if( false == ElesisAwakeningEffectSetPresent() )
			ShowMinorParticleHyperModeTrace();

		ApplyHyperModeBuff();

		if( true == bSound_ )
		{
			PlaySound( L"Energy.ogg" );
			PlaySound( L"Break.ogg" );
			PlaySound( L"Change.ogg" );
		}
	}

	CommonFrameMove();
#else SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( fTime1_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( fTime1_ ) == true && EventCheck(fTime1_, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		ShowMinorParticleHyperModeChange();
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( fTime2_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( fTime2_ ) == true && EventCheck(fTime2_, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		ShowMinorParticleHyperModeShock();

		UpDownCrashCamera( 30.0f, 0.4f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

		ShowMinorParticleHyperModeTrace();
		ApplyHyperModeBuff();

		if( true == bSound_ )
		{
			PlaySound( L"Energy.ogg" );
			PlaySound( L"Break.ogg" );
			PlaySound( L"Change.ogg" );
		}
	}
	CommonFrameMove();
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE
}
#endif //MODIFY_RIDING_PET_AWAKE





#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
// char-elesis CH1e: June 32b6ad40 ( Grand Master Z-Z-Z-X: the down-X start state )
void CX2GUEL::ESI_COMBO_SGM_ZZZX_EventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX == true && CX2Unit::UC_ELESIS_GRAND_MASTER == GetUnitClass(), ESI_COMBO_SGM_XX_DOWN_X_START )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1e: June 32b522d0 ( the first 2 MP of the dash X-X attack are taken on the future frame )
void CX2GUEL::ESI_DASH_COMBO_SGM_XX_ATTACK_StateStartFuture()
{
	CommonStateStartFuture();
	UpNowMp( -XX_ATTACK_MP_COST );
}

// char-elesis CH1e: June 32b86080 ( timer times 4 to 9 chain on the previous one: kept, JUNE_BUGS 629 )
void CX2GUEL::ESI_DASH_COMBO_SGM_XX_ATTACK_EventProcess()
{
	bool& bAttackKeyPressed = m_FrameDataFuture.stateParam.bEventFlagList[0];
	bool& bAttackKeyPressedAgain = m_FrameDataFuture.stateParam.bEventFlagList[1];

	if ( true == bAttackKeyPressed )
	{
		if ( true == m_InputData.oneX )
			bAttackKeyPressedAgain = true;
	}
	else if ( true == m_InputData.oneX )
	{
		bAttackKeyPressed = true;
	}

	float fEventTime = ( m_pXSkinAnimFuture->GetMaxAnimationTime() - XX_ATTACK_TIME_OFFSET ) / XX_ATTACK_TIME_DIVISOR;
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_DASH_COMBO_SGM_XX_END );
	}
	else
	{
		bool bTimerReached = m_pXSkinAnimFuture->EventTimerOneshot( fEventTime * 2.f + XX_ATTACK_TIME_OFFSET );
		for ( int iMultiplier = 3; iMultiplier <= 9 && false == bTimerReached; ++iMultiplier )
		{
			fEventTime = fEventTime * static_cast<float>( iMultiplier ) + XX_ATTACK_TIME_OFFSET;
			bTimerReached = m_pXSkinAnimFuture->EventTimerOneshot( fEventTime );
		}

		if ( true == bTimerReached )
		{
			if ( true == bAttackKeyPressed )
			{
				if ( GetNowMp() >= XX_ATTACK_MP_COST )
				{
					UpNowMp( -XX_ATTACK_MP_COST );
				}
				else
				{
					g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1),
						D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
					StateChange( ESI_DASH_COMBO_SGM_XX_END );
				}
			}
			else
			{
				StateChange( ESI_DASH_COMBO_SGM_XX_END );
			}

			if ( false == bAttackKeyPressedAgain )
				bAttackKeyPressed = false;
			bAttackKeyPressedAgain = false;
		}
	}

	CommonEventProcess();
}

// char-elesis CH1e: June 32b6b0a0
void CX2GUEL::ESI_DASH_COMBO_SGM_XX_END_EventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1e: June 32b6bc30 ( one start for ESI_P_SGM_ADVANCED_KNIGHT_MASTERY and _SMALL: half of the casts play the second guard animation )
void CX2GUEL::ESI_P_SGM_ADVANCED_KNIGHT_MASTERY_StateStart()
{
	CommonStateStart();
	if ( 0 == GetRandomInt( 0 ) % 2 )
		m_pXSkinAnim->ChangeAnim( L"SI_P_SGM_ADVANCED_KNIGHT_MASTERY_GUARD2", false, 0.f );
}

// char-elesis CH1e: June 32b6bc80
void CX2GUEL::ESI_P_SGM_ADVANCED_KNIGHT_MASTERY_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}

// char-elesis CH1f: June 32b52300 ( the dash-jump Z-X ready state: no dash attack, 5 X attacks )
void CX2GUEL::ESI_DASH_JUMP_COMBO_SPK_ZX_READY_StateStartFuture()
{
	m_bIsDashAttackPossible = false;
	m_uiSPKXZReattakCount = DASH_JUMP_ZX_LOOP_COUNT;
	CommonStateStartFuture();
}

// char-elesis CH1f: June 32b6b320
void CX2GUEL::ESI_DASH_JUMP_COMBO_SPK_ZX_READY_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
		StateChange( ESI_DASH_JUMP_COMBO_SPK_ZX_LOOP );
	CommonEventProcess();
}

// char-elesis CH1f: June 32b52320 ( each loop uses one of the X attacks )
void CX2GUEL::ESI_DASH_JUMP_COMBO_SPK_ZX_LOOP_StateStartFuture()
{
	--m_uiSPKXZReattakCount;
	CommonStateStartFuture();
}

// char-elesis CH1f: June 32b6b350 ( a second X press inside the key window loops again while X attacks are left )
void CX2GUEL::ESI_DASH_JUMP_COMBO_SPK_ZX_LOOP_EventProcess()
{
	bool& bLoopAgain = m_FrameDataFuture.stateParam.bEventFlagList[0];

	if ( true == bLoopAgain
		&& ( m_fEventTime[0].stateChange < m_pXSkinAnimFuture->GetNowAnimationTime() || true == m_pXSkinAnimFuture->IsAnimationEnd() ) )
	{
		StateChange( ESI_DASH_JUMP_COMBO_SPK_ZX_LOOP );
		bLoopAgain = false;
	}
	else if ( m_pXSkinAnimFuture->GetNowAnimationTime() < m_fEventTime[0].keyInputEnd
		&& m_fEventTime[0].keyInputStart <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& true == m_InputData.oneX && 0 < m_uiSPKXZReattakCount )
	{
		bLoopAgain = true;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() && true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	CommonEventProcess();
}

// char-elesis CH1f: June 32b7d2b0 ( the dash-jump XZ fireball with the Z-X damage effects, half the MP and a 0.1 s timer )
void CX2GUEL::ESI_DASH_JUMP_COMBO_SPK_ZX_LOOP_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( DASH_JUMP_ZX_LOOP_EVENT_TIME ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( DASH_JUMP_ZX_LOOP_EVENT_TIME ) == true && EventCheck( DASH_JUMP_ZX_LOOP_EVENT_TIME, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		if( true == IsEnoughMPForAttack( DASH_JUMP_ZX_LOOP_MP_COST, vBonePos, 0.f, D3DXVECTOR3( 0.f, 0.f, -45.f ) ) )
		{
			D3DXVECTOR3 vRotateDegree = GetRotateDegree();
			vRotateDegree.z = -45.f;

			const WCHAR* pwszDamageEffectName = L"DamageEffect_Elesis_SPK_DashJump_Combo_ZX";
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE
			if( true == m_bIsLearnUnextinguishableFire )
				pwszDamageEffectName = L"DamageEffect_Elesis_SBH_DashJump_Combo_ZX";
#endif SERV_ELESIS_SECOND_CLASS_CHANGE

			g_pX2Game->GetDamageEffect()->CreateInstance( this, pwszDamageEffectName, GetPowerRate() * GetFireBallPowerRate(),
				vBonePos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
		}
	}

	CommonFrameMove();
}

// char-elesis CH1g: June 32b519f0
void CX2GUEL::ESI_DASH_JUMP_COMBO_SDK_XXX_Init()
{
	XSkinMeshReadyInBackground( L"SA_EL_SA_SSK_SPIRAL_BLAST_01.X" );
	XMeshReadyInBackground( L"SCA_Projectile001.Y" );
}

// char-elesis CH1g: June 32b52330 ( the charge time restarts with the state )
void CX2GUEL::ESI_DASH_JUMP_COMBO_SDK_ZX_CHARGE_StateStart()
{
	CommonStateStart();
	m_fDashJumpZXChargeTime = 0.f;
}

// char-elesis CH1g: June 32b73bf0 ( the charge time and the class-skill float restart on the future frame )
void CX2GUEL::ESI_DASH_JUMP_COMBO_SDK_ZX_CHARGE_StateStartFuture()
{
	CommonStateStartFuture();
	m_fDashJumpZXChargeTime = 0.f;
	if ( !( static_cast<float>( m_FrameDataFuture.syncData.fClassSkillFloat ) == 0.f ) )
		SetClassSkillFloat( 0.f );
}

// char-elesis CH1g: June 32b52350 ( the charge time grows with the frame time up to its limit )
void CX2GUEL::ESI_DASH_JUMP_COMBO_SDK_ZX_CHARGE_FrameMove()
{
	CommonFrameMove();
	m_fDashJumpZXChargeTime += m_fElapsedTime;
	if ( DASH_JUMP_ZX_CHARGE_TIME_LIMIT <= m_fDashJumpZXChargeTime )
		m_fDashJumpZXChargeTime = DASH_JUMP_ZX_CHARGE_TIME_LIMIT;
}

// char-elesis CH1g: June 32b73c90 ( X released or held to the fire time: the facing key sets the aim, then the fire state )
void CX2GUEL::ESI_DASH_JUMP_COMBO_SDK_ZX_CHARGE_EventProcess()
{
	if ( DASH_JUMP_ZX_CHARGE_FIRE_TIME <= m_FrameDataFuture.unitCondition.fStateTime || false == m_InputData.pureX )
	{
		const bool bFacingKeyHeld = ( true == m_InputData.pureRight && true == GetIsRight() )
			|| ( true == m_InputData.pureLeft && false == GetIsRight() );
		const float fAimDegree = ( true == bFacingKeyHeld ) ? 0.f : DASH_JUMP_ZX_FIRE_DEGREE;
		if ( !( static_cast<float>( m_FrameDataFuture.syncData.fClassSkillFloat ) == fAimDegree ) )
			SetClassSkillFloat( fAimDegree );

		StateChange( ESI_DASH_JUMP_COMBO_SDK_ZX_FIRE );
	}
	CommonEventProcess();
}

// char-elesis CH1g: June 32b6b460
void CX2GUEL::ESI_DASH_JUMP_COMBO_SDK_ZX_FIRE_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() && true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
}

// char-elesis CH1g: June 32b7d550 ( the charged shot: 5 MP, power and size grow with the charge, the aim picks the effect set )
void CX2GUEL::ESI_DASH_JUMP_COMBO_SDK_ZX_FIRE_FrameMove()
{
	CommonFrameMove();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( DASH_JUMP_FIRE_EVENT_TIME ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( DASH_JUMP_FIRE_EVENT_TIME ) == true && EventCheck( DASH_JUMP_FIRE_EVENT_TIME, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const float fChargePowerRate = m_fDashJumpZXChargeTime + 1.f;
		const float fEffectScale = 1.f + m_fDashJumpZXChargeTime * DASH_JUMP_ZX_FIRE_SCALE_PER_CHARGE;
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		const bool bAimedDown = ( GetClassSkillFloat() < 0.f );

		if ( true == IsEnoughMPForAttack( DASH_JUMP_ZX_FIRE_MP_COST, vBonePos, 0.f,
			D3DXVECTOR3( 0.f, 0.f, ( true == bAimedDown ) ? DASH_JUMP_ZX_FIRE_DEGREE : 0.f ) ) )
		{
			const WCHAR* pwszEffectSetName = ( true == bAimedDown ) ? L"EffectSet_SDK_Dash_Jump_Combo_ZX" : L"EffectSet_SDK_Dash_Jump_Combo_ZX_Front";
			g_pX2Game->GetEffectSet()->PlayEffectSet( pwszEffectSetName, this, NULL, false, GetPowerRate() * fChargePowerRate, -1.f,
				D3DXVECTOR3( fEffectScale, fEffectScale, fEffectScale ) );
		}
	}
}

// char-elesis CH1g: June 32b64310 ( Dark Knight Z-Z-Z-X: the X press needs no class test )
void CX2GUEL::ESI_COMBO_SDK_ZZZX_EventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX, ESI_COMBO_SDK_ZZZXX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g: June 32b64660 ( the end of the Z-Z-Z-X-X chain )
void CX2GUEL::ESI_COMBO_SDK_ZZZXX_EventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g: June 32b648e0 ( the flags are set with no key window; the cancels after the on-line branch can fire only in the air )
void CX2GUEL::ESI_DASH_JUMP_COMBO_SDK_XX_EventProcess()
{
	bool& bToDashJumpComboXXX = m_FrameDataFuture.stateParam.bEventFlagList[0];
	bool& bToComboZ = m_FrameDataFuture.stateParam.bEventFlagList[1];

	if ( true == m_InputData.oneX )
		bToDashJumpComboXXX = true;
	else if ( true == m_InputData.oneZ )
		bToComboZ = true;

	if ( true == IsOnSomethingFuture() )
	{
		if ( true == bToComboZ )
			StateChange( ESI_COMBO_Z );
		else if ( true == bToDashJumpComboXXX )
			StateChange( ESI_DASH_JUMP_COMBO_SDK_XXX );
		else
			StateChange( ESI_DASH_JUMP_COMBO_XX_LANDING );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
}

// char-elesis CH1g: June 32b64bd0
void CX2GUEL::ESI_DASH_JUMP_COMBO_SDK_XXX_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
}

// char-elesis CH1g: June 32b7c0b0 ( the Uppercut effect set costs 3 MP; without it the not-enough-MP effect plays at the left hand )
void CX2GUEL::ESI_DASH_JUMP_COMBO_SDK_XXX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( DASH_JUMP_FIRE_EVENT_TIME ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( DASH_JUMP_FIRE_EVENT_TIME ) == true && EventCheck( DASH_JUMP_FIRE_EVENT_TIME, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( true == FlushMp( DASH_JUMP_XXX_FIRE_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_C_SCA_Dash_ZZZ_Uppercut", this );
		else
			CreateNotEnoughMPEffect( GetBonePos( L"Dummy2_Lhand" ), 0.f, 0.f, 0.f );
	}

	CommonFrameMove();
}

// char-elesis CH1g: June 32b64ee0 ( Dark Knight X-X: X goes on, Z jumps to the last hit; the Z flag has its own key window )
void CX2GUEL::ESI_COMBO_SDK_XX_EventProcess()
{
	IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX, ESI_COMBO_SDK_XXX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		true == m_InputData.oneZ, ESI_COMBO_SDK_XXXXXXX )
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g: June 32b652d0 ( both flags use the key window of event time 0 )
void CX2GUEL::ESI_COMBO_SDK_XXX_EventProcess()
{
	IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX, ESI_COMBO_SDK_XXXX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneZ, ESI_COMBO_SDK_XXXXXXX )
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g: June 32b656c0
void CX2GUEL::ESI_COMBO_SDK_XXXX_EventProcess()
{
	IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX, ESI_COMBO_SDK_XXXXX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneZ, ESI_COMBO_SDK_XXXXXXX )
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g: June 32b65ab0
void CX2GUEL::ESI_COMBO_SDK_XXXXX_EventProcess()
{
	IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX, ESI_COMBO_SDK_XXXXXX )
	ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 1, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneZ, ESI_COMBO_SDK_XXXXXXX )
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g: June 32b65ea0 ( one flag: X or Z goes to the last hit )
void CX2GUEL::ESI_COMBO_SDK_XXXXXX_EventProcess()
{
	IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX || true == m_InputData.oneZ, ESI_COMBO_SDK_XXXXXXX )
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g: June 32b66200 ( the last hit of the Dark Knight chains )
void CX2GUEL::ESI_COMBO_SDK_XXXXXXX_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
// char-elesis CH1g2: June 32b51a20 ( the Z-Z-Z loop count restarts with the Z-Z state )
void CX2GUEL::ESI_DASH_COMBO_SCA_ZZ_StateStart()
{
	CommonStateStart();
	m_usDashComboZZZLoopCount = 0;
}

// char-elesis CH1g2: June 32b51a40
void CX2GUEL::ESI_DASH_COMBO_SCA_ZZZ_START_Init()
{
	XSkinMeshReadyInBackground( L"SA_EL_SA_SSK_SPIRAL_BLAST_01.X" );
	XMeshReadyInBackground( L"SCA_Projectile001.Y" );
	XMeshReadyInBackground( L"Raven_Seven_Burst_MEMO_F01.Y" );
}

// char-elesis CH1g2: June 32b51a70 ( both loop states: one more loop counted )
void CX2GUEL::ESI_DASH_COMBO_SCA_ZZZ_LOOP_1_StateStart()
{
	CommonStateStart();
	++m_usDashComboZZZLoopCount;
}

// char-elesis CH1g2: June 32b51a90
void CX2GUEL::ESI_DASH_COMBO_SCA_ZX_START_StateStart()
{
	CommonStateStart();
}

// char-elesis CH1g2: June 32b52390 ( the charge time restarts with the state )
void CX2GUEL::ESI_DASH_COMBO_SCA_XZ_CHARGE_StateStart()
{
	CommonStateStart();
	m_fDashJumpZXChargeTime = 0.f;
}

// char-elesis CH1g2: June 32b523b0
void CX2GUEL::ESI_DASH_COMBO_SCA_XZ_CHARGE_StateStartFuture()
{
	CommonStateStartFuture();
	m_fDashJumpZXChargeTime = 0.f;
}

// char-elesis CH1g2: June 32b523d0 ( Z released after the first event time, or held to the fire time )
void CX2GUEL::ESI_DASH_COMBO_SCA_XZ_CHARGE_EventProcess()
{
	const float fStateTime = m_FrameDataFuture.unitCondition.fStateTime;
	if ( ( m_fEventTime[0].stateChange <= fStateTime && false == m_InputData.pureZ ) || DASH_COMBO_XZ_CHARGE_FIRE_TIME <= fStateTime )
		StateChange( ESI_DASH_COMBO_SCA_XZ_FIRE );

	CommonEventProcess();
}

// char-elesis CH1g2: June 32b52420 ( the charge time grows with the frame time up to its limit )
void CX2GUEL::ESI_DASH_COMBO_SCA_XZ_CHARGE_FrameMove()
{
	CommonFrameMove();
	m_fDashJumpZXChargeTime += m_fElapsedTime;
	if ( DASH_COMBO_XZ_CHARGE_TIME_LIMIT <= m_fDashJumpZXChargeTime )
		m_fDashJumpZXChargeTime = DASH_COMBO_XZ_CHARGE_TIME_LIMIT;
}

// char-elesis CH1g2: June 32b66480 ( Crimson Avenger dash Z-Z: Z goes on, no class test )
void CX2GUEL::ESI_DASH_COMBO_SCA_ZZ_EventProcess()
{
	IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneZ, ESI_DASH_COMBO_SCA_ZZZ_START )
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g2: June 32b667a0
void CX2GUEL::ESI_DASH_COMBO_SCA_ZZZ_START_EventProcess()
{
	IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneZ, ESI_DASH_COMBO_SCA_ZZZ_LOOP_1 )
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g2: June 32b66ac0 ( a Z press loops on while fewer than 4 loops were counted; the animation end finishes the combo )
void CX2GUEL::ESI_DASH_COMBO_SCA_ZZZ_LOOP_1_EventProcess()
{
	IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneZ && m_usDashComboZZZLoopCount < DASH_COMBO_ZZZ_LOOP_MAX_COUNT, ESI_DASH_COMBO_SCA_ZZZ_LOOP_2 )
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_DASH_COMBO_SCA_ZZZ_END );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g2: June 32b66df0
void CX2GUEL::ESI_DASH_COMBO_SCA_ZZZ_LOOP_2_EventProcess()
{
	IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneZ && m_usDashComboZZZLoopCount < DASH_COMBO_ZZZ_LOOP_MAX_COUNT, ESI_DASH_COMBO_SCA_ZZZ_LOOP_1 )
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_DASH_COMBO_SCA_ZZZ_END );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g2: June 32b67120 ( the end of the Z-Z-Z loop: wait, then the four cancels incl. Z-X )
void CX2GUEL::ESI_DASH_COMBO_SCA_ZZZ_END_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
}

// char-elesis CH1g2: June 32b67420 ( Crimson Avenger dash Z-X: X goes to the attack )
void CX2GUEL::ESI_DASH_COMBO_SCA_ZX_START_EventProcess()
{
	IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX, ESI_DASH_COMBO_SCA_ZX_ATTACK )
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g2: June 32b67740 ( no detonation while the state runs )
void CX2GUEL::ESI_DASH_COMBO_SCA_ZX_ATTACK_StateStart()
{
	CommonStateStart();
	SetNoDetonation( true );
}

// char-elesis CH1g2: June 32b67760
void CX2GUEL::ESI_DASH_COMBO_SCA_ZX_ATTACK_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
		StateChange( ESI_DASH_COMBO_SCA_ZX_LOOP );

	CommonEventProcess();
}

// char-elesis CH1g2: June 32b67790
void CX2GUEL::ESI_DASH_COMBO_SCA_ZX_ATTACK_StateEnd()
{
	CommonStateEnd();
	SetNoDetonation( false );
}

// char-elesis CH1g2: June 32b677b0 ( no detonation while the state runs )
void CX2GUEL::ESI_DASH_COMBO_SCA_ZX_LOOP_StateStart()
{
	CommonStateStart();
	SetNoDetonation( true );
}

// char-elesis CH1g2: June 32b677d0 ( the loop ends with its animation once X is released or the state time reaches its limit; the limit reads the present frame data )
void CX2GUEL::ESI_DASH_COMBO_SCA_ZX_LOOP_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( DASH_COMBO_ZX_LOOP_TIME_LIMIT <= m_FrameDataNow.unitCondition.fStateTime || false == m_InputData.pureX )
			StateChange( ESI_DASH_COMBO_SCA_ZX_END );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g2: June 32b67a50
void CX2GUEL::ESI_DASH_COMBO_SCA_ZX_LOOP_StateEnd()
{
	CommonStateEnd();
	SetNoDetonation( false );
}

// char-elesis CH1g2: June 32b67a70
void CX2GUEL::ESI_DASH_COMBO_SCA_ZX_END_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g2: June 32b6b760
void CX2GUEL::ESI_DASH_COMBO_SCA_XZ_FIRE_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )

	CommonEventProcess();
}

// char-elesis CH1g2: June 32b7c2e0 ( the Z-Z-Z effect set costs 3 MP; without it the not-enough-MP effect plays at the left hand )
void CX2GUEL::ESI_DASH_COMBO_SCA_ZZZ_START_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( DASH_COMBO_ZZZ_START_FIRE_TIME ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( DASH_COMBO_ZZZ_START_FIRE_TIME ) == true && EventCheck( DASH_COMBO_ZZZ_START_FIRE_TIME, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( true == FlushMp( DASH_COMBO_ZZZ_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_C_SCA_Dash_ZZZ", this );
		else
			CreateNotEnoughMPEffect( GetBonePos( L"Dummy2_Lhand" ), 0.f, 0.f, 0.f );
	}

	CommonFrameMove();
}

// char-elesis CH1g2: June 32b7c510 ( the Uppercut effect set at once, the Z-Z-Z one a quarter second later; 3 MP each )
void CX2GUEL::ESI_DASH_COMBO_SCA_ZZZ_LOOP_1_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( DASH_COMBO_ZZZ_LOOP_1_FIRST_FIRE_TIME ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( DASH_COMBO_ZZZ_LOOP_1_FIRST_FIRE_TIME ) == true && EventCheck( DASH_COMBO_ZZZ_LOOP_1_FIRST_FIRE_TIME, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( true == FlushMp( DASH_COMBO_ZZZ_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_C_SCA_Dash_ZZZ_Uppercut", this );
		else
			CreateNotEnoughMPEffect( GetBonePos( L"Dummy2_Lhand" ), 0.f, 0.f, 0.f );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( DASH_COMBO_ZZZ_LOOP_1_SECOND_FIRE_TIME ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( DASH_COMBO_ZZZ_LOOP_1_SECOND_FIRE_TIME ) == true && EventCheck( DASH_COMBO_ZZZ_LOOP_1_SECOND_FIRE_TIME, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( true == FlushMp( DASH_COMBO_ZZZ_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_C_SCA_Dash_ZZZ", this );
		else
			CreateNotEnoughMPEffect( GetBonePos( L"Dummy2_Lhand" ), 0.f, 0.f, 0.f );
	}

	CommonFrameMove();
}

// char-elesis CH1g2: June 32b7c940 ( the same two effect sets in the other order and at other times )
void CX2GUEL::ESI_DASH_COMBO_SCA_ZZZ_LOOP_2_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( DASH_COMBO_ZZZ_LOOP_2_FIRST_FIRE_TIME ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( DASH_COMBO_ZZZ_LOOP_2_FIRST_FIRE_TIME ) == true && EventCheck( DASH_COMBO_ZZZ_LOOP_2_FIRST_FIRE_TIME, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( true == FlushMp( DASH_COMBO_ZZZ_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_C_SCA_Dash_ZZZ", this );
		else
			CreateNotEnoughMPEffect( GetBonePos( L"Dummy2_Lhand" ), 0.f, 0.f, 0.f );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( DASH_COMBO_ZZZ_LOOP_2_SECOND_FIRE_TIME ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( DASH_COMBO_ZZZ_LOOP_2_SECOND_FIRE_TIME ) == true && EventCheck( DASH_COMBO_ZZZ_LOOP_2_SECOND_FIRE_TIME, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( true == FlushMp( DASH_COMBO_ZZZ_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_C_SCA_Dash_ZZZ_Uppercut", this );
		else
			CreateNotEnoughMPEffect( GetBonePos( L"Dummy2_Lhand" ), 0.f, 0.f, 0.f );
	}

	CommonFrameMove();
}

// char-elesis CH1g2: June 32b7cd70 ( the finishing effect set, 3 MP )
void CX2GUEL::ESI_DASH_COMBO_SCA_ZZZ_END_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( DASH_COMBO_ZZZ_END_FIRE_TIME ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( DASH_COMBO_ZZZ_END_FIRE_TIME ) == true && EventCheck( DASH_COMBO_ZZZ_END_FIRE_TIME, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( true == FlushMp( DASH_COMBO_ZZZ_MP_COST ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_C_SCA_Dash_ZZZ_Finish", this );
		else
			CreateNotEnoughMPEffect( GetBonePos( L"Dummy2_Lhand" ), 0.f, 0.f, 0.f );
	}

	CommonFrameMove();
}

// char-elesis CH1g2: June 32b7d920 ( the charged shot: 5 MP, damage, life time and size grow with the charge; it is placed at the unit, not at the hand )
void CX2GUEL::ESI_DASH_COMBO_SCA_XZ_FIRE_FrameMove()
{
	CommonFrameMove();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( DASH_COMBO_XZ_FIRE_EVENT_TIME ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( DASH_COMBO_XZ_FIRE_EVENT_TIME ) == true && EventCheck( DASH_COMBO_XZ_FIRE_EVENT_TIME, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const float fChargePowerRate = 1.f + m_fDashJumpZXChargeTime * DASH_COMBO_XZ_FIRE_POWER_PER_CHARGE;
		const float fEffectScale = 1.f + m_fDashJumpZXChargeTime * DASH_COMBO_XZ_FIRE_SCALE_PER_CHARGE;
		const float fLifeTimeRate = 1.f + m_fDashJumpZXChargeTime * DASH_COMBO_XZ_FIRE_LIFE_TIME_PER_CHARGE;
		const D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );

		if ( true == IsEnoughMPForAttack( DASH_COMBO_XZ_FIRE_MP_COST, vBonePos, 0.f, D3DXVECTOR3( 0.f, 0.f, 0.f ) ) )
		{
			CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_SCA_Dash_Combo_XZ",
				GetPowerRate() * fChargePowerRate, GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y,
				false, -1.f, 1.f, fLifeTimeRate );
			if ( NULL != pDamageEffect )
				pDamageEffect->SetScale( D3DXVECTOR3( fEffectScale, fEffectScale, fEffectScale ) );
		}
	}
}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE
#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
// char-elesis CH2a: June 32b572e0 ( the MP the start spent is kept only while Waltz of Sword's ability is set )
void CX2GUEL::ESI_SI_A_EL_DODGE_AND_SLASH_StateStart()
{
	const float fMpBeforeStart = GetNowMp();
	CommonStateStart();
	if ( 0 < m_iWaltzOfSwordDodgeAbility )
		m_fDodgeAndSlashConsumedMp = fMpBeforeStart - GetNowMp();
	else
		m_fDodgeAndSlashConsumedMp = 0.f;
}

// char-elesis CH2a: June 32b72600 ( leaving for any state but its own three clears the Z / X attack count and the re-use count )
void CX2GUEL::ESI_SI_A_EL_DODGE_AND_SLASH_StateEndFuture()
{
	const int iNextStateID = static_cast<USHORT>( m_FrameDataFuture.unitCondition.nextState );
	if ( ESI_SI_A_EL_DODGE_AND_SLASH != iNextStateID && ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z != iNextStateID && ESI_SI_A_EL_DODGE_AND_SLASH_BACK != iNextStateID )
	{
		m_uiDodgeAndSlashAttackCount = 0;
		m_uiDodgeAndSlashReuseCount = 0;
	}
	CommonStateEndFuture();
}

// char-elesis CH2a: June 32b704c0 ( Waltz of Sword in the destruction state raises the attack's physic damage )
void CX2GUEL::ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z_StateStart()
{
	CommonStateStart();
	if ( 0 < m_iWaltzOfSwordDodgeAbility && WSS_DESTRUCTION == GetWayOfSwordState() )
		m_DamageData.damage.fPhysic *= GetSkillNumericalByName( L"ATK_REL", CX2SkillTree::RNSI_P_SGM_WALTZ_OF_SWORD, true );
}

// char-elesis CH2a: June 32b4f7e0
void CX2GUEL::ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_READY_StateStart()
{
	CommonStateStart();
	m_fSonicAssaultStingChargeTime = 0.f;
	m_fDashJumpZXChargeTime = 0.f;	// shared with the Dark Knight dash-jump Z-X charge
}

// char-elesis CH2a: June 32b4f800
void CX2GUEL::ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_READY_StateStartFuture()
{
	CommonStateStartFuture();
	m_kSkillSlotKey.Set( m_iNowSpecialAttack - 1 );
}

// char-elesis CH2a: June 32b58e70
void CX2GUEL::ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_READY_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
		StateChange( ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_CHARGE );
	CommonEventProcess();
}

// char-elesis CH2a: June 32b4f830
void CX2GUEL::ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_CHARGE_StateStart()
{
	CommonStateStart();
}

// char-elesis CH2a: June 32b75580 ( the hold time grows; while MP lasts the charge grows and drains MP, and its cap plays the charged effect set once )
void CX2GUEL::ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_CHARGE_FrameMove()
{
	CommonFrameMove();
	m_fSonicAssaultStingChargeTime += m_fElapsedTime;

	const float fMpConsume = m_fElapsedTime * SONIC_ASSAULT_CHARGE_MP_COST_PER_SECOND;
	if ( fMpConsume <= GetNowMp() && m_fDashJumpZXChargeTime < SONIC_ASSAULT_CHARGE_TIME_LIMIT )
	{
		const float fCharge = m_fElapsedTime + m_fDashJumpZXChargeTime;
		m_fDashJumpZXChargeTime = ( SONIC_ASSAULT_CHARGE_TIME_LIMIT < fCharge ) ? SONIC_ASSAULT_CHARGE_TIME_LIMIT : fCharge;
		UpNowMp( -fMpConsume );
		if ( SONIC_ASSAULT_CHARGE_TIME_LIMIT <= m_fDashJumpZXChargeTime )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_EL_SONIC_ASSAULT_CHARGE", this );
	}
}

// char-elesis CH2a: June 32b4f840 ( released, or held to the hold time: fire )
void CX2GUEL::ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_CHARGE_EventProcess()
{
	m_kSkillSlotKey.FollowSlotSwap();
	if ( SONIC_ASSAULT_CHARGE_HOLD_TIME <= m_fSonicAssaultStingChargeTime || true == m_kSkillSlotKey.IsKeyReleased( m_InputData ) )
		StateChange( ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_FIRE );
	CommonEventProcess();
}

// char-elesis CH2a: June 32b4f8a0
void CX2GUEL::ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_FIRE_StateStart()
{
	CommonStateStart();
}

// char-elesis CH2a: June 32b70db0 ( the charge ( 0 to 0.5 ) scales the physic damage and the forward speed burst )
void CX2GUEL::ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_FIRE_FrameMove()
{
	CommonFrameMove();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( SONIC_ASSAULT_FIRE_EVENT_TIME ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( SONIC_ASSAULT_FIRE_EVENT_TIME ) == true && EventCheck( SONIC_ASSAULT_FIRE_EVENT_TIME, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const float fCharge = ( SONIC_ASSAULT_CHARGE_TIME_LIMIT < m_fDashJumpZXChargeTime ) ? SONIC_ASSAULT_CHARGE_TIME_LIMIT : m_fDashJumpZXChargeTime;
		const float fChargeRate = fCharge * 2.f;	// 0 to 1 over the charge

		TIME_SPEED timeSpeed;
		timeSpeed.vSpeed = D3DXVECTOR3( GetWalkSpeed() * SONIC_ASSAULT_FIRE_SPEED_RATE * ( SONIC_ASSAULT_FIRE_SPEED_PER_CHARGE_RATE * fChargeRate + 1.f ), 0.f, SONIC_ASSAULT_FIRE_SPEED_TIME );
		timeSpeed.iFlag = 2;	// no enum: 2 sets the speed ( X2GameUnit.h TIME_SPEED )
		m_vecSpeedFactor.push_back( timeSpeed );

		m_DamageData.damage.fPhysic *= ( fChargeRate + 1.f );
	}
}

// char-elesis CH2a: June 32b75760 ( the damage effect sets, the memo pair in the destruction state )
void CX2GUEL::ESI_SI_SA_EL_WILD_SHOCK_FrameMove()
{
	CommonFrameMove();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( WILD_SHOCK_DAMAGE_EVENT_TIME ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( WILD_SHOCK_DAMAGE_EVENT_TIME ) == true && EventCheck( WILD_SHOCK_DAMAGE_EVENT_TIME, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( WSS_DESTRUCTION == GetWayOfSwordState() )
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_EL_WILD_SHOCK_DAMAGE_MEMO", this, NULL, IsHyperState() );
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_EL_WILD_SHOCK_DAMAGE_Sub_MEMO", this, NULL, IsHyperState() );
		}
		else
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_EL_WILD_SHOCK_DAMAGE", this, NULL, IsHyperState() );
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_EL_WILD_SHOCK_DAMAGE_Sub", this, NULL, IsHyperState() );
		}
	}
}

// char-elesis CH2d: June 31fc7ac0 ( a pure skill key: A S D C Skill5 Q W E R SkillEx5 )
bool CX2GUEL::IsAnySkillKeyPressed() const
{
	return m_InputData.pureA == true || m_InputData.pureS == true || m_InputData.pureD == true || m_InputData.pureC == true
		|| m_InputData.pureSkill5 == true
		|| m_InputData.pureQ == true || m_InputData.pureW == true || m_InputData.pureE == true || m_InputData.pureR == true
		|| m_InputData.pureSkillEx5 == true;
}

// char-elesis CH2d: June 32b89470 ( the bleeding HP cost of the Dark Knight skills: the effect set, then the damage of the given hit data ( x rate, x hit count ) taken from HP, leaving 10% of max HP; true always )
bool CX2GUEL::ApplyBleedingHpCost( CX2DamageManager::DamageData* pDamageData_, float fDamageRate_, const USHORT usHitCount_ )
{
	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_B_EL_SDC_Bleeding", this );

	const float fHpRate = GetNowHp() / GetMaxHp();
	if ( fHpRate < BLEEDING_MIN_HP_RATE )
		return true;

	CX2DamageManager::DamageData kDamageData( *pDamageData_ );
	kDamageData.optrDefenderGameUnit = this;

#ifdef SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS
	if ( WSS_DESTRUCTION == GetWayOfSwordState() && CX2DamageManager::WST_DESTRUCTION_NORMAL <= pDamageData_->m_WayofSwordType && pDamageData_->m_WayofSwordType <= CX2DamageManager::WST_DESTRUCTION_SPECIAL )
		fDamageRate_ *= m_fDestructionBleedingDamageRate;
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS

	kDamageData.damage.fMagic *= fDamageRate_;
	kDamageData.damage.fPhysic *= fDamageRate_;

	const double dHitDamage = static_cast<double>( GetFinalDamage( &kDamageData ) ) * static_cast<double>( usHitCount_ );
	const double dLeftHp = static_cast<double>( GetNowHp() ) - static_cast<double>( GetMaxHp() ) * static_cast<double>( BLEEDING_KEEP_HP_RATE );
	float fHpCost = static_cast<float>( dHitDamage <= dLeftHp ? dHitDamage : dLeftHp );
	if ( true == HaveThisBuff( BTI_BUFF_BLOOD_RUNAWAY_PASSIVE ) )
		fHpCost = static_cast<float>( static_cast<double>( fHpCost ) * static_cast<double>( BLEEDING_BLOOD_RUNAWAY_COST_RATE ) );
	if ( 0.f < fHpCost )
		UpNowHp( -fHpCost, 0.f );
	return true;
}

// char-elesis CH2d: June 32b51aa0
void CX2GUEL::SI_SA_SDK_BRUTAL_CUTTER_Init()
{
	XSkinMeshReadyInBackground( L"Brutal_Cutter_Weapon_T.X" );
	XSkinMeshReadyInBackground( L"Brutal_Cutter_Weapon_W.X" );
	TextureReadyInBackground( L"EL_Base_Blood_M01.dds" );
	TextureReadyInBackground( L"ASL_HIT_P001.dds" );
	XMeshReadyInBackground( L"SCA_CRIMSON_BREAK_P01.Y" );
	XMeshReadyInBackground( L"ELK_WIND_SLICER_M02.Y" );
	TextureReadyInBackground( L"SDK_AIRWAVE_T01.dds" );
}

// char-elesis CH2d: June 32b51b00
void CX2GUEL::SI_SA_SDK_EJECTION_BUSTER_CHARGE_Init()
{
	TextureReadyInBackground( L"ASL_HIT_P001.dds" );
	XMeshReadyInBackground( L"Ejection_Buster_Oura.Y" );
	XMeshReadyInBackground( L"SCA_CRIMSON_BREAK_P01.Y" );
	XSkinMeshReadyInBackground( L"Ejection_Buster_Weapon.X" );
	XMeshReadyInBackground( L"SDK_Ejection_Buster_Blade01.Y" );
	XMeshReadyInBackground( L"Ejection_Buster_Oura4.Y" );
	XMeshReadyInBackground( L"SCA_CRIMSON_BREAK_PE02.Y" );
	TextureReadyInBackground( L"SDK_AIRWAVE_T01.dds" );
	XMeshReadyInBackground( L"Ejection_Buster_Oura5.Y" );
	XSkinMeshReadyInBackground( L"SDK_EJECTION_BUSTER_Attack01.X" );
	XMeshReadyInBackground( L"Ejection_Buster_Oura6.Y" );
}

// char-elesis CH2d: June 32b51c90
void CX2GUEL::SI_SA_SDK_BURNING_PHANTOM_READY_Init()
{
	XMeshReadyInBackground( L"SDK_BURNING_PHANTOM_P001.Y" );
	TextureReadyInBackground( L"WhiteImage.dds" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	XMeshReadyInBackground( L"SCA_CRIMSON_BREAK_P01.Y" );
	TextureReadyInBackground( L"SDK_AIRWAVE_T01.dds" );
	XSkinMeshReadyInBackground( L"SDK_BURNING_PHANTOM_StartDummy.X" );
}

// char-elesis CH2d: June 32b51fa0
void CX2GUEL::SI_A_SDK_INJECTION_STIGMA_Init()
{
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	TextureReadyInBackground( L"SDK_AIRWAVE_T01.dds" );
	XSkinMeshReadyInBackground( L"SDK_INJECTION_STIGMA_Blade01.X" );
	TextureReadyInBackground( L"ASL_HIT_P001.dds" );
	XMeshReadyInBackground( L"SCA_CRIMSON_BREAK_P01.Y" );
	XSkinMeshReadyInBackground( L"SDK_BURNING_PHANTOM_PT01.X" );
	XMeshReadyInBackground( L"SDK_BURNING_PHANTOM_Storm001.Y" );
}

// char-elesis CH2d: June 32b52000
void CX2GUEL::SI_A_SDK_DARK_WAVE_Init()
{
	TextureReadyInBackground( L"WhiteImage.dds" );
	TextureReadyInBackground( L"EL_Base_Blood_M01.dds" );
	TextureReadyInBackground( L"WhiteImage.dds" );
	XMeshReadyInBackground( L"ELK_WIND_SLICER_M02.Y" );
	XMeshReadyInBackground( L"Ejection_Buster_Oura5.Y" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
}

// char-elesis CH2d: June 32b52060
void CX2GUEL::SI_A_SCA_SHADE_SCRATCH_Init()
{
	TextureReadyInBackground( L"EL_Base_Blood_M01.dds" );
	XMeshReadyInBackground( L"SDK_BURNING_PHANTOM_P001.Y" );
	XMeshReadyInBackground( L"SDK_BURNING_PHANTOM_Storm001.Y" );
	XMeshReadyInBackground( L"ALS_SHAKE_CHAOS_02.Y" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	XMeshReadyInBackground( L"ELK_WIND_SLICER_M02.Y" );
	XMeshReadyInBackground( L"SCA_CRIMSON_BREAK_P01.Y" );
	TextureReadyInBackground( L"SDK_AIRWAVE_T01.dds" );
	TextureReadyInBackground( L"star2.dds" );
	XMeshReadyInBackground( L"Raven_Seven_Burst_MEMO_F01.Y" );
}

// char-elesis CH2d: June 32b89c10 ( 0.2 s: the HP cost raises the damage ( x1.3 in PvP / PvE, else x1.4 ), then the Brutal Cutter buff factor and its two effect sets )
void CX2GUEL::SI_SA_SDK_BRUTAL_CUTTER_FrameMove()
{
	if ( true == m_pXSkinAnim->EventTimerOneshot( BRUTAL_CUTTER_EVENT_TIME ) )
	{
		if ( true == ApplyBleedingHpCost( &m_DamageData, BLEEDING_HP_COST_DAMAGE_RATE, BRUTAL_CUTTER_BLEEDING_HIT_COUNT ) )
		{
			const float fDamageRate = ( true == g_pX2Game->GetIsNowPVPCorrectionGame() ) ? BLEEDING_DAMAGE_RATE_PVP_OR_PVE : BLEEDING_DAMAGE_RATE;
			m_DamageData.damage.fMagic *= fDamageRate;
			m_DamageData.damage.fPhysic *= fDamageRate;
		}

		const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::RNSI_SA_SDK_BRUTAL_CUTTER, false, 0, true );

		wstring wstrEffectSetFirst;
		wstring wstrEffectSetSecond;
		UINT uiBuffFactorId = BFI_BUFF_BRUTAL_CUTTER;
		if ( false == HaveThisBuff( BTI_BUFF_BRUTAL_CUTTER_MEMO ) && false == HaveThisBuff( BTI_BUFF_BRUTAL_CUTTER ) )
		{
			wstrEffectSetFirst = L"EffectSet_SI_SA_SDK_BRUTAL_CUTTER_FX1";
			wstrEffectSetSecond = L"EffectSet_SI_SA_SDK_BRUTAL_CUTTER_FX2";
		}
		else
		{
			EraseBuffTempletFromGameUnit( BTI_BUFF_BRUTAL_CUTTER );
			wstrEffectSetFirst = L"EffectSet_SA_SDK_BrutalCutterFX1_MEMO";
			wstrEffectSetSecond = L"EffectSet_SA_SDK_BrutalCutterFX2_MEMO";
			uiBuffFactorId = BFI_BUFF_BRUTAL_CUTTER_MEMO;
		}

		CX2BuffFactorPtr ptrBuffFactor = CX2BuffTempletManager::GetInstance()->GetBuffFactorPtr( uiBuffFactorId );
		if ( NULL != ptrBuffFactor )
		{
			CX2BuffFactorPtr ptrBuffFactorClone = ptrBuffFactor->GetClonePtr( iSkillLevel );
			if ( NULL != ptrBuffFactorClone )
			{
				const SkillMemoEntry* pMemoEntry = GetSkillMemoEntry( CX2SkillTree::RNSI_SA_SDK_BRUTAL_CUTTER );
				if ( NULL != pMemoEntry && CX2SkillTree::MI_CHECK_AND_KILL_1 == pMemoEntry->m_iType )
					RescaleBuffFactorTime( *ptrBuffFactorClone, true == pMemoEntry->m_vecValue.empty() ? 0.f : pMemoEntry->m_vecValue[0] );

				if ( true == IsUpgradedSkillLearned( GetNowStateSkillID() ) )
				{
					float fBuffTime = 1.f;
					const KBuffFinalizerFactor* pFirstTime = ptrBuffFactorClone->GetFirstFinalizer( BFT_TIME );
					if ( NULL != pFirstTime && 1 < pFirstTime->m_vecValues.size() )
						fBuffTime = pFirstTime->m_vecValues[1];

					float fEffectiveTime = 1.f;
					if ( true == TryGetAcquiredSkillAbilityValue( GetUnit(), CX2SkillTree::SA_EFFECTIVE_TIME, GetNowStateSkillID(), false, fEffectiveTime ) )
						ptrBuffFactorClone->ApplyExternalFactorToFinalizer( BFT_TIME, fEffectiveTime * fBuffTime, false );
				}

				ptrBuffFactorClone->SetGameUnitBuffFactor( this, -1, true );
				SetBuffFactorToGameUnit( *ptrBuffFactorClone );
			}
		}

		// char-elesis CH2d fix: June 32b89c10 @32b8a109 / @32b8a237 push each handle on the delete-on-state-end list
		m_vecEffectSetToDeleteOnStateEnd.push_back( g_pX2Game->GetEffectSet()->PlayEffectSet( wstrEffectSetFirst, this ) );
		m_vecEffectSetToDeleteOnStateEnd.push_back( g_pX2Game->GetEffectSet()->PlayEffectSet( wstrEffectSetSecond, this ) );
	}

	CommonFrameMove();
}

// char-elesis CH2d: June 32b67cd0
void CX2GUEL::SI_SA_SDK_BRUTAL_CUTTER_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == m_FrameDataFuture.unitCondition.bFootOnLine )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
	CommonEventProcess();
}

// char-elesis CH2d: June 32b51b90 ( the held key is June's slot 0-9; the charge time members are the Sonic Assault charge's )
void CX2GUEL::SI_SA_SDK_EJECTION_BUSTER_CHARGE_StateStartFuture()
{
	CommonStateStartFuture();
	m_kSkillSlotKey.Set( m_iNowSpecialAttack - 1 );
	m_fSonicAssaultStingChargeTime = 0.f;
	m_fDashJumpZXChargeTime = 0.f;
}

// char-elesis CH2d: June 32b51bc0
void CX2GUEL::SI_SA_SDK_EJECTION_BUSTER_CHARGE_StateStart()
{
	CommonStateStart();
	m_fSonicAssaultStingChargeTime = 0.f;
	m_fDashJumpZXChargeTime = 0.f;
}

// char-elesis CH2d: June 32b7cfa0
void CX2GUEL::SI_SA_SDK_EJECTION_BUSTER_CHARGE_FrameMove()
{
	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SI_SA_SDK_EJECTION_BUSTER_Charge_3", this );
	CommonFrameMove();
}

// char-elesis CH2d: June 32b51be0 ( the charge state lasts one frame )
void CX2GUEL::SI_SA_SDK_EJECTION_BUSTER_CHARGE_EventProcess()
{
	StateChange( SI_SA_SDK_EJECTION_BUSTER_ATTACK );
	CommonEventProcess();
}

// char-elesis CH2d: June 32b7d110
void CX2GUEL::SI_SA_SDK_EJECTION_BUSTER_ATTACK_FrameMove()
{
	if ( true == m_pXSkinAnim->EventTimerOneshot( EJECTION_BUSTER_ATTACK_EVENT_TIME ) )
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SI_SA_SDK_EJECTION_BUSTER_Attack_1500", this );

	CommonFrameMove();
}

// char-elesis CH2d: June 32b67ff0 ( the end of the animation lands only on a line; in the air the state goes on )
void CX2GUEL::SI_SA_SDK_EJECTION_BUSTER_ATTACK_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() && true == m_FrameDataFuture.unitCondition.bFootOnLine )
	{
		StateChange( USI_JUMP_LANDING );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
	CommonEventProcess();
}

// char-elesis CH2d: June 32b51cf0
void CX2GUEL::SI_SA_SDK_BURNING_PHANTOM_READY_StateStart()
{
	CommonStateStart();
	m_usBurningPhantomLoopCount = 0;
}

// char-elesis CH2d: June 32b68650
void CX2GUEL::SI_SA_SDK_BURNING_PHANTOM_READY_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
		StateChange( SI_SA_SDK_BURNING_PHANTOM_START );

	CommonEventProcess();
}

// char-elesis CH2d: June 32b68680
void CX2GUEL::SI_SA_SDK_BURNING_PHANTOM_START_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
		StateChange( SI_SA_SDK_BURNING_PHANTOM_LOOP );

	CommonEventProcess();
}

// char-elesis CH2d: June 32b73ae0 ( the shock debuff is cleared; a tick every 0.15 s, each tick above the 4th costs 10 MP; no CommonFrameMove once the count is past 9 )
void CX2GUEL::SI_SA_SDK_BURNING_PHANTOM_LOOP_FrameMove()
{
	if ( 0.f != m_ExtraDamagePack.m_EnchantShock.m_fTime )
		m_ExtraDamagePack.m_EnchantShock.m_fTime = 0.f;

	if ( m_usBurningPhantomLoopCount <= BURNING_PHANTOM_LAST_TICK )
	{
		const USHORT usTick = static_cast<USHORT>( static_cast<int>( static_cast<double>( m_FrameDataNow.unitCondition.fStateTime ) / static_cast<double>( BURNING_PHANTOM_TICK_TIME ) ) );
		if ( m_usBurningPhantomLoopCount < usTick )
		{
			m_usBurningPhantomLoopCount = usTick;
			if ( BURNING_PHANTOM_FIRST_TICK < usTick )
				UpNowMp( -BURNING_PHANTOM_MP_PER_TICK );
		}
		CommonFrameMove();
	}
}

// char-elesis CH2d: June 32b51d10 ( from the 4th tick the loop ends below 10 MP, from the 9th, or when no skill key is held )
void CX2GUEL::SI_SA_SDK_BURNING_PHANTOM_LOOP_EventProcess()
{
	if ( BURNING_PHANTOM_FIRST_TICK <= m_usBurningPhantomLoopCount )
	{
		if ( GetNowMp() < BURNING_PHANTOM_MIN_MP || BURNING_PHANTOM_LAST_TICK <= m_usBurningPhantomLoopCount || false == IsAnySkillKeyPressed() )
			StateChange( SI_SA_SDK_BURNING_PHANTOM_END );
	}
	CommonEventProcess();
}

// char-elesis CH2d: June 32b686b0
void CX2GUEL::SI_SA_SDK_BURNING_PHANTOM_END_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == m_FrameDataFuture.unitCondition.bFootOnLine )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
	CommonEventProcess();
}

// char-elesis CH2d: June 32b8b410 ( 0.01 s: half the physical attack x the skill's ability is taken from HP; 0.2 s: the dummy damage effect carries the bleeding cost, the effect set plays, the buff is replaced )
void CX2GUEL::SI_A_SDK_INJECTION_STIGMA_FrameMove()
{
	if ( true == m_pXSkinAnim->EventTimerOneshot( INJECTION_STIGMA_HP_COST_EVENT_TIME )
		&& NULL != GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_SDK_INJECTION_STIGMA, true ) )
	{
		float fHpCost = GetLearnedSkillAbilityValue( CX2SkillTree::SA_ATK_PHYSIC_REL, GetNowStateSkillID() ) * GetPhysicAttackValue();
		if ( true == HaveThisBuff( BTI_BUFF_BLOOD_RUNAWAY_PASSIVE ) )
			fHpCost = static_cast<float>( static_cast<double>( fHpCost ) * static_cast<double>( BLEEDING_BLOOD_RUNAWAY_COST_RATE ) );
		UpNowHp( -fHpCost, 0.f );
	}

	if ( true == m_pXSkinAnim->EventTimerOneshot( INJECTION_STIGMA_DAMAGE_EVENT_TIME )
		&& NULL != GetEquippedActiveSkillTemplet( CX2SkillTree::RNSI_A_SDK_INJECTION_STIGMA, true ) && NULL != GetUnit() )
	{
		CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_SI_A_SDK_INJECTION_STIGMA_DUMMY",
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree() );
		if ( NULL != pDamageEffect )
		{
			float fPowerRate = GetPowerRate();
			if ( true == ApplyBleedingHpCost( pDamageEffect->GetDamageDataPtr(), BLEEDING_HP_COST_DAMAGE_RATE, BLEEDING_SINGLE_HIT_COUNT ) )
				fPowerRate *= ( true == g_pX2Game->GetIsNowPVPCorrectionGame() ) ? BLEEDING_DAMAGE_RATE_PVP_OR_PVE : BLEEDING_DAMAGE_RATE;

			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SI_A_SDK_INJECTION_STIGMA", this, NULL, false, fPowerRate );
			g_pX2Game->GetDamageEffect()->DestroyInstance( pDamageEffect );
		}

		EraseBuffTempletFromGameUnit( BTI_BUFF_INJECTION_STIGMA );
		SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_INJECTION_STIGMA, 1, NULL, 0, false );
	}

	CommonFrameMove();
}

// char-elesis CH2d: June 32b69680
void CX2GUEL::SI_A_SDK_INJECTION_STIGMA_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == m_FrameDataFuture.unitCondition.bFootOnLine )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
	CommonEventProcess();
}

// char-elesis CH2d: June 32b8b850 ( 0.3 s: the dummy damage effect carries the bleeding cost, the pushing effect set plays; 0.8 s: the impact effect set, with the same damage rise )
void CX2GUEL::SI_A_SDK_DARK_WAVE_FrameMove()
{
	if ( true == m_pXSkinAnim->EventTimerOneshot( DARK_WAVE_DAMAGE_EVENT_TIME ) )
	{
		CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_A_SDK_DARK_WAVE_DUMMY",
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree() );
		if ( NULL != pDamageEffect )
		{
			float fPowerRate = GetPowerRate();
			if ( true == ApplyBleedingHpCost( pDamageEffect->GetDamageDataPtr(), BLEEDING_HP_COST_DAMAGE_RATE, BLEEDING_SINGLE_HIT_COUNT ) )
			{
				const float fDamageRate = ( true == g_pX2Game->GetIsNowPVPCorrectionGame() ) ? BLEEDING_DAMAGE_RATE_PVP_OR_PVE : BLEEDING_DAMAGE_RATE;
				m_FrameDataNow.stateParam.bEventFlagList[0] = true;
				fPowerRate *= fDamageRate;
			}

			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_SDK_DARK_WAVE_HIT_Pushing", this, NULL, false, fPowerRate );
			g_pX2Game->GetDamageEffect()->DestroyInstance( pDamageEffect );
		}
	}

	if ( true == m_pXSkinAnim->EventTimerOneshot( DARK_WAVE_IMPACT_EVENT_TIME ) )
	{
		float fPowerRate = GetPowerRate();
		if ( true == m_FrameDataNow.stateParam.bEventFlagList[0] )
			fPowerRate = ( ( true == g_pX2Game->GetIsNowPVPCorrectionGame() ) ? BLEEDING_DAMAGE_RATE_PVP_OR_PVE : BLEEDING_DAMAGE_RATE ) * fPowerRate;

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_SDK_DARK_WAVE_HIT_Impact", this, NULL, false, fPowerRate );
	}

	CommonFrameMove();
}

// char-elesis CH2d: June 32b699a0
void CX2GUEL::SI_A_SDK_DARK_WAVE_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == m_FrameDataFuture.unitCondition.bFootOnLine )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
	CommonEventProcess();
}

// char-elesis CH2d: June 32b69cc0 ( a hit after the key-input time goes on to the attack state )
void CX2GUEL::SI_A_SCA_SHADE_SCRATCH_EventProcess()
{
	if ( true == GetFrameData( true ).unitCondition.bHit && m_fEventTime[0].keyInputStart < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		StateChange( SI_A_SCA_SHADE_SCRATCH_ATTACK );
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == m_FrameDataFuture.unitCondition.bFootOnLine )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
	CommonEventProcess();
}

// char-elesis CH2d: June 32b6a020
void CX2GUEL::SI_A_SCA_SHADE_SCRATCH_ATTACK_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == m_FrameDataFuture.unitCondition.bFootOnLine )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )
	CommonEventProcess();
}

// char-elesis CH3a: June 32b6bcd0 ( the walk / back-walk / dash / ZX cancels need the foot on a line )
void CX2GUEL::ESI_SA_SGM_CROSS_SLASH_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
}

// char-elesis CH3a: June 32b6bfd0 ( the same instructions as Cross Slash's, compared one by one )
void CX2GUEL::ESI_SA_SGM_DIVINE_BLADE_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
}

// char-elesis CH3a: June 32b6dae0 ( the same instructions as Cross Slash's )
void CX2GUEL::ESI_SA_SGM_WAR_PRELUDE_GROUND_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
}

// char-elesis CH3a: June 32b6d7e0 ( Cross Slash's chain, but the end of the animation lands only on a line )
void CX2GUEL::ESI_SA_SGM_WAR_PRELUDE_FLY_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() && true == m_FrameDataFuture.unitCondition.bFootOnLine )
	{
		StateChange( USI_JUMP_LANDING );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
}

// char-elesis CH3a: June 32b6dde0
void CX2GUEL::ESI_SA_SGM_GIGA_BLAST_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_SA_SGM_GIGA_BLAST_CHARGE );
	}
	CommonEventProcess();
}

// char-elesis CH3a: June 32b6de10
void CX2GUEL::ESI_SA_SGM_GIGA_BLAST_CHARGE_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESI_SA_SGM_GIGA_BLAST_END );
	}
	CommonEventProcess();
}

// char-elesis CH3a: June 32b77de0 ( 0.8 s: the black hole effect set, the transcended memo's own; the handle is kept on the custom-state, die and skill-state-end lists )
void CX2GUEL::ESI_SA_SGM_JUGGERNAUT_BUSTER_START_FrameMove()
{
	CommonFrameMove();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( JUGGERNAUT_BUSTER_BLACK_HOLE_EVENT_TIME ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( JUGGERNAUT_BUSTER_BLACK_HOLE_EVENT_TIME ) == true && EventCheck( JUGGERNAUT_BUSTER_BLACK_HOLE_EVENT_TIME, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const int iMemoSkillID = GetTranscendenceMemoSkillID( CX2SkillTree::RNSI_SA_SGM_JUGGERNAUT_BUSTER );
		const bool bTranscendedMemo = ( CX2SkillTree::RNSI_SA_SGM_JUGGERNAUT_BUSTER_TRANS == iMemoSkillID || CX2SkillTree::RNSI_SA_SGM_JUGGERNAUT_BUSTER_TRANS_UPGRADE == iMemoSkillID );

		std::wstring wstrEffectSetName;
		if( true == bTranscendedMemo )
			wstrEffectSetName = ( true == m_bIsApplyVictoriousSword ? L"EffectSet_SGM_JuggernautBuster_BLACKHOLE_Trans_WithVictoriousSword" : L"EffectSet_SGM_JuggernautBuster_BLACKHOLE_Trans" );
		else
			wstrEffectSetName = ( true == m_bIsApplyVictoriousSword ? L"EffectSet_SGM_JuggernautBuster_BLACKHOLE_WithVictoriousSword" : L"EffectSet_SGM_JuggernautBuster_BLACKHOLE" );

		const CX2EffectSet::Handle hHandle = g_pX2Game->GetEffectSet()->PlayEffectSet( wstrEffectSetName, this );
		if( INVALID_EFFECTSET_HANDLE != hHandle )
		{
			m_vecEffectSetToDeleteOnCustomState.push_back( hHandle );
			m_vecEffectSetToDeleteOnDie.push_back( hHandle );
			m_vecEffectSetToDeleteOnSkillStateEnd.push_back( std::make_pair( hHandle, static_cast<int>( GetNowStateSkillID() ) ) );
		}
	}
}

// char-elesis CH3a: June 32b78170 ( 0.29 s: the finish effect set; the handle is deleted on death only )
void CX2GUEL::ESI_SA_SGM_JUGGERNAUT_BUSTER_END_FrameMove()
{
	CommonFrameMove();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( JUGGERNAUT_BUSTER_FINISH_EVENT_TIME ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( JUGGERNAUT_BUSTER_FINISH_EVENT_TIME ) == true && EventCheck( JUGGERNAUT_BUSTER_FINISH_EVENT_TIME, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const std::wstring wstrEffectSetName = ( true == m_bIsApplyVictoriousSword ? L"EffectSet_SGM_JuggernautBuster_Finish_WithVictoriousSword" : L"EffectSet_SGM_JuggernautBuster_Finish" );
		const CX2EffectSet::Handle hHandle = g_pX2Game->GetEffectSet()->PlayEffectSet( wstrEffectSetName, this );
		if( INVALID_EFFECTSET_HANDLE != hHandle )
			m_vecEffectSetToDeleteOnDie.push_back( hHandle );
	}
}

// char-elesis CH3a2: June 32b50b00 ( Provoke is a held-key skill state now: the skill key is remembered at the start )
void CX2GUEL::ESI_A_SGM_PROVOKE_StateStartFuture()
{
	CommonStateStartFuture();
	m_kSkillSlotKey.Set( m_iNowSpecialAttack - 1 );
}

// char-elesis CH3a2: June 32b6ced0 ( at the end of the animation, or once USEFUL_VALUE0 has passed off a line, the unit falls; the cancels need the foot on a line )
void CX2GUEL::ESI_A_SGM_UPPER_SLASH_EventProcess()
{
	if( ( GetUsefulValueFuture( 0, 0 ) <= m_pXSkinAnimFuture->GetNowAnimationTime() && false == m_FrameDataFuture.unitCondition.bFootOnLine ) || true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_JUMP_DOWN );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER_ON_LINE_( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER_ON_LINE_( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER_ON_LINE_( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX_ON_LINE_( m_fAttackCancelAfterEx )

	CommonEventProcess();
}
#endif SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL
#endif // NEW_CHARACTER_EL


