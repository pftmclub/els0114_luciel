#include "stdafx.h"
#include "X2OfflineServer.h"

//////////////////////////////////////////////////////////////////////////
// Author: Iruha
// Date: 2026-09-01
// Description: Offline mode - phase 3. Entering a village.
//
//              Two request/reply pairs, in this order:
//
//                EGS_STATE_CHANGE_FIELD_REQ / _ACK
//                  Sent from CX2StateServerSelect::Handler_EGS_STATE_CHANGE_
//                  FIELD_REQ (X2StateServerSelect.cpp:5934). Its ACK
//                  (:6022) is the only thing that sends
//                  XGM_STATE_CHANGE -> XS_VILLAGE_MAP, i.e. it is what
//                  actually leaves character select for the village. There is
//                  no separate EGS_STATE_CHANGE_VILLAGE_MAP_* event.
//
//                EGS_FIELD_LOADING_COMPLETE_REQ / _ACK
//                  Sent once the village world has loaded
//                  (X2StateField.cpp:1407). Carries the position the client
//                  spawned at; the real server stashes it in KGSUser::
//                  m_kLastPos (GSUserField.cpp:410) for the next login.
//
//              Both are waited on with AddServerPacket, so both must be
//              answered. Neither reply carries any state the client does not
//              already have - the village, its NPCs and its line map all come
//              out of the client's own .kom data, and the field user list is a
//              server *push* that simply never arrives offline, which is
//              correct: there is nobody else in the village.
//
//              Then four more, all found by running the village rather than by
//              reading the code:
//
//                EGS_OPTION_UPDATE_REQ / _ACK
//                  Sent from the CX2StateField constructor
//                  (X2StateField.cpp:131), i.e. once per village entry, and
//                  waited on.
//
//                EGS_UPDATE_PLAY_STATUS_NOT
//                  The client's live HP/MP/hyper, pushed every ~3s by
//                  CX2State::CheckAndSendingPlayStatus. This is the write half
//                  of what MakeGamePlayStatus reads back at character select.
//
//                EGS_FIELD_UNIT_SYNC_DATA_NOT
//                  Position sync, pushed every ~2s while the player moves. The
//                  real server broadcasts it to the rest of the field; offline
//                  there is nobody to broadcast to, but it is the only place
//                  the client reports where it is standing, so it is what makes
//                  "log back in where you left off" actually work.
//
//              EGS_JOIN_BATTLE_FIELD_REQ - the village portal out to a field -
//              was refused here in phase 3 and now lives in Handlers_Room.cpp:
//              in this build the fields outside a village are rooms, so
//              reaching one is room emulation rather than field entry.
//////////////////////////////////////////////////////////////////////////

#ifdef SERV_IRUHADEV_OFFLINE

//////////////////////////////////////////////////////////////////////////

bool CX2OfflineServer::Handler_EGS_STATE_CHANGE_FIELD_REQ( KOfflineSession& kSes, const KEvent& kEvent )
{
	KEGS_STATE_CHANGE_FIELD_REQ kReq;
	if( false == ReadReq( kEvent, kReq ) )
		return false;

	KEGS_STATE_CHANGE_FIELD_ACK kAck;
	kAck.m_iOK				= NetError::NET_OK;
	kAck.m_iLimitLevel		= 0;
	kAck.m_iRequireDungeonID= 0;

	// This ACK must never carry an error. Every failure path in
	// CX2StateServerSelect::Handler_EGS_STATE_CHANGE_FIELD_ACK
	// (X2StateServerSelect.cpp:6074) ends in
	// Handler_EGS_STATE_CHANGE_FIELD_REQ( true ), i.e. it immediately re-sends
	// the request - so refusing here is not "the client shows an error", it is
	// an unbounded request/reply loop. Anything that looks wrong is logged and
	// allowed through instead.
	//
	// Nor is there a CheckEnterTheVillage(). The real server gates villages on
	// level and on having cleared a dungeon (ERR_FIELD_05 / ERR_FIELD_06);
	// offline the client is trusted, and it only asks for a village its own UI
	// already lets the player reach. Its own
	// Handler_EGS_STATE_CHANGE_FIELD_REQ has additionally rewritten the map to
	// VMI_RUBEN for a character with EXP 0 or one whose stored map has no
	// village templet, so an unusable ID does not get this far.
	kSes.m_eState = S_FIELD_MAP;

	if( 0 != kSes.m_nSelectedUnitUID )
	{
		CX2OfflineDB::Instance()->SaveLastPosition( kSes.m_nSelectedUnitUID, kReq.m_iMapID );

		CX2OfflineLog::Server( L"FIELD    unitUID=%I64d entering mapID=%d%s",
			(__int64)kSes.m_nSelectedUnitUID, kReq.m_iMapID,
			( true == kReq.m_bUseWarpItem ) ? L" (warp item)" : L"" );
	}
	else
	{
		CX2OfflineLog::Server( L"FIELD    WARNING mapID=%d with no character selected on this session"
			L" - allowed through, nothing persisted", kReq.m_iMapID );
	}

#ifdef SERV_IRUHADEV_OFFLINE_LEAVE_ROOM_ON_FIELD_CHANGE
	// C3z-4t4c: the ACK, then the buff list, then the room leave - GSUserField.cpp:211-221, 276-334
	const bool bAckSent = Reply( kSes, EGS_STATE_CHANGE_FIELD_ACK, kAck );
#ifdef SERV_IRUHADEV_OFFLINE_ITEM_BUFF
	SendUpdateBuffInfo( kSes, true );
#endif SERV_IRUHADEV_OFFLINE_ITEM_BUFF

	if( true == m_kRoom.m_bActive )
	{
		// the battle field leaves with the warp-item reason when one was used; every other room type is reason 01
		const int iLeaveReason = ( (char)CX2Room::RT_BATTLE_FIELD != m_kRoom.m_kInfo.m_RoomType )
			? (int)NetError::NOT_LEAVE_ROOM_REASON_01
			: ( true == kReq.m_bUseWarpItem ) ? (int)NetError::NOT_LEAVE_ROOM_REASON_30 : (int)NetError::NOT_LEAVE_ROOM_REASON_27;

		const bool bBuffChanged = LeaveActiveRoom( kSes, iLeaveReason );
#ifdef SERV_IRUHADEV_OFFLINE_ITEM_BUFF
		if( true == bBuffChanged )
			SendUpdateBuffInfo( kSes );
#endif SERV_IRUHADEV_OFFLINE_ITEM_BUFF
	}
	return bAckSent;
#else // SERV_IRUHADEV_OFFLINE_LEAVE_ROOM_ON_FIELD_CHANGE
#ifdef SERV_IRUHADEV_OFFLINE_ITEM_BUFF
	// GSUserField.cpp:215-221: after the ACK, the whole buff list, only if there is one.
	const bool bAckSent = Reply( kSes, EGS_STATE_CHANGE_FIELD_ACK, kAck );
	SendUpdateBuffInfo( kSes, true );
	return bAckSent;
#else // SERV_IRUHADEV_OFFLINE_ITEM_BUFF
	return Reply( kSes, EGS_STATE_CHANGE_FIELD_ACK, kAck );
#endif SERV_IRUHADEV_OFFLINE_ITEM_BUFF
#endif SERV_IRUHADEV_OFFLINE_LEAVE_ROOM_ON_FIELD_CHANGE
}

//////////////////////////////////////////////////////////////////////////

bool CX2OfflineServer::Handler_EGS_FIELD_LOADING_COMPLETE_REQ( KOfflineSession& kSes, const KEvent& kEvent )
{
	KEGS_FIELD_LOADING_COMPLETE_REQ kReq;
	if( false == ReadReq( kEvent, kReq ) )
		return false;

	// SERV_EVENT_RETURN_USER_MARK and SERV_EVENT_GUIDELINE_POPUP are both off in
	// this build, so this ACK really is just KPacketOK - no returning-user mark,
	// no first-visit guide popup to decide about.
	KEGS_FIELD_LOADING_COMPLETE_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	if( 0 != kSes.m_nSelectedUnitUID )
	{
		CX2OfflineDB::Instance()->SaveLastFieldPos( kSes.m_nSelectedUnitUID,
			(int)kReq.m_ucLastTouchLineIndex, (int)kReq.m_usLastPosValue );

		CX2OfflineLog::Server( L"FIELD    unitUID=%I64d loaded, line=%d pos=%d",
			(__int64)kSes.m_nSelectedUnitUID,
			(int)kReq.m_ucLastTouchLineIndex, (int)kReq.m_usLastPosValue );
	}

	Reply( kSes, EGS_FIELD_LOADING_COMPLETE_ACK, kAck );

	//////////////////////////////////////////////////////////////////////////
	// Author: Iruha
	// Date: 2026-09-05
	// Description: Phase 28. This handler runs on every village entry within a
	// session, not only the first one after login (a dungeon/battlefield uses
	// a separate completion packet - EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_REQ,
	// Handlers_Room.cpp). SendPendingPetRestore is a no-op after its first
	// call - kSes.m_bPetRestorePending only gets set once, by Handler_EGS_
	// SELECT_UNIT_REQ - so calling it unconditionally here does not re-spawn
	// the pet on ordinary village-to-village travel. See the phase 28 section
	// of OFFLINE_MODE_PHASE9_PLAN.md's Trap.
	SendPendingPetRestore( kSes );
	//////////////////////////////////////////////////////////////////////////

#ifdef SERV_IRUHADEV_OFFLINE_ITEM_BUFF
	// ERM_JOIN_FIELD_ACK (GSUserField.cpp:533-546): joining a village grants
	// WARM_ENERGE_OF_EL (122), and that LoginServer round trip ends in
	// SendUpdateBuffInfo - the copy the client actually takes, because by then
	// it is in the village. The copy sent after EGS_STATE_CHANGE_FIELD_ACK
	// (GSUserField.cpp:215) arrives while the old dungeon/field game still
	// exists, and CX2PremiumBuffManager's village handler drops it (play-test
	// 2026-10-05: no icons until the next send). Offline grants no 122 - a live
	// global village buff, not an item buff - so the resend is made directly,
	// and only when there is something to show.
#ifdef SERV_IRUHADEV_OFFLINE_VILLAGE_BUFF
	// With the village buff ported, the live sequence itself: grant 122, and its
	// round trip ends in SendUpdateBuffInfo (always, as the ACK always sends).
	if( true == CX2OfflineBuff::Instance()->OnJoinVillage( kSes.m_nSelectedUnitUID ) )
		SendUpdateBuffInfo( kSes );
	else
		SendUpdateBuffInfo( kSes, true );
#else // SERV_IRUHADEV_OFFLINE_VILLAGE_BUFF
	SendUpdateBuffInfo( kSes, true );
#endif SERV_IRUHADEV_OFFLINE_VILLAGE_BUFF
#endif SERV_IRUHADEV_OFFLINE_ITEM_BUFF

	// Phase 6: standing in a village finishes a SQT_VISIT_VILLAGE step, and it
	// is also where a character that levelled elsewhere is told which title
	// missions it has grown into. Done here rather than at
	// EGS_STATE_CHANGE_FIELD_REQ because the client is only ready to draw a
	// quest update once the world is loaded, and the village is only *entered*
	// at that point - the state-change request is issued from character select,
	// before the map even exists.
	//
	// kReq carries no map ID, so the village is read back off the row that
	// EGS_STATE_CHANGE_FIELD_REQ just wrote a moment ago.
	{
		KOfflineUnitRow kRow;
		if( true == CX2OfflineDB::Instance()->LoadUnit( kSes.m_nSelectedUnitUID, kRow ) )
			QuestOnEnterVillage( kSes, kRow.m_iLastPos );
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

bool CX2OfflineServer::Handler_EGS_OPTION_UPDATE_REQ( KOfflineSession& kSes, const KEvent& kEvent )
{
	KEGS_OPTION_UPDATE_REQ kReq;
	if( false == ReadReq( kEvent, kReq ) )
		return false;

	// m_kFieldOption is the client's graphics detail level, which the real
	// server uses to decide how many other players to put in a field sector.
	// There is nobody else in the field, so there is nothing to do with it - but
	// CX2State::Handler_EGS_OPTION_UPDATE_REQ arms AddServerPacket, so it has to
	// be answered.
	KEGS_OPTION_UPDATE_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	return Reply( kSes, EGS_OPTION_UPDATE_ACK, kAck );
}

//////////////////////////////////////////////////////////////////////////

bool CX2OfflineServer::Handler_EGS_UPDATE_PLAY_STATUS_NOT( KOfflineSession& kSes, const KEvent& kEvent )
{
	KEGS_UPDATE_PLAY_STATUS_NOT kReq;
	if( false == ReadReq( kEvent, kReq ) )
		return false;

	// No reply: this is a push, and the real server does not answer it either.
	if( 0 == kSes.m_nSelectedUnitUID )
		return true;

	// Placed before the early-outs below on purpose: this has to run on every
	// tick, not only the ones where a gauge moved. This packet's fixed
	// three-second cadence is the only regular beat a dungeon run has, and
	// PushRemainingPlayTime rides it to keep CX2Game::m_AllowFrameCount topped up
	// - without which the client stops reading the keyboard about seven seconds
	// into every dungeon. See PushRemainingPlayTime for the full story.
	PushRemainingPlayTime( kSes );

#ifdef SERV_IRUHADEV_OFFLINE_SECRET_POISON
	// June effect C3z-6: KDungeonRoom::Tick's poison check (DungeonRoom.cpp:167-186) rides the same beat.
	TickSecretPoison( kSes );
#endif SERV_IRUHADEV_OFFLINE_SECRET_POISON

#ifdef SERV_IRUHADEV_OFFLINE_DUNGEON_CONTINUE_END
	// KDungeonRoom::Tick's CheckEndPlay (DungeonRoom.cpp:250-253) rides the same beat, so a 15 s timeout lands within 3 s of it.
	TickDungeonContinueEnd( kSes );
#endif SERV_IRUHADEV_OFFLINE_DUNGEON_CONTINUE_END

	// ...and the auto-party queue's, for the same reason. This packet is the
	// only regular beat the village has - CX2StateMenu::OnFrameMove pushes it
	// every three seconds through m_TimerForSendingPlayStatus - so the fake
	// matchmaking wait is counted out on it rather than on a timer the
	// emulator does not have. See AUTO_PARTY_QUEUE_MS for what that costs.
	TickAutoPartyMatch( kSes );

#ifdef SERV_IRUHADEV_OFFLINE_ITEM_BUFF
	// ...and the premium buffs' expiry (KGSUser::CheckBuffPeriod, 1 s live).
	CheckBuffPeriod( kSes );
#endif SERV_IRUHADEV_OFFLINE_ITEM_BUFF

#ifdef SERV_IRUHADEV_OFFLINE_SPA_SOAK_TIMER
	// june-spa S3c: the spa soak countdown rides the same beat, so the buff lands up to 3 s after the notice reaches 0
	TickSpaSoak( kSes );
#endif SERV_IRUHADEV_OFFLINE_SPA_SOAK_TIMER
	// Note this one carries a bare KGamePlayStatus, not the
	// KGamePlayStatusContainer that EGS_SELECT_UNIT_4_NOT uses.
	const KGamePlayStatus& kStatus = kReq.m_kGamePlayStatus;

	// A status the client could not build at all comes through as max HP 0
	// (CX2GageManager::GetMyPlayStatusToPacket gates the send on returning true,
	// but a gage set that exists with nothing in it still reports zeroes).
	// Writing that would overwrite a good saved HP with 0.
	if( kStatus.m_iMaxHP <= 0 )
		return true;

#ifdef SERV_IRUHADEV_JUNE_CHAR_MINOR_UNIT_PLAY_STATUS
	// char-luciel CH1a2: the minor unit's MP and the form, written on change like the four below
	if( kStatus.m_iMinorUnitCurMP != kSes.m_iSavedMinorUnitMP || (int)kStatus.m_cMinorUnitForm != kSes.m_iSavedMinorUnitForm )
	{
		CX2OfflineDB::Instance()->SaveMinorUnitPlayStatus( kSes.m_nSelectedUnitUID, kStatus.m_iMinorUnitCurMP, (int)kStatus.m_cMinorUnitForm );
		kSes.m_iSavedMinorUnitMP	= kStatus.m_iMinorUnitCurMP;
		kSes.m_iSavedMinorUnitForm	= (int)kStatus.m_cMinorUnitForm;
	}
#endif SERV_IRUHADEV_JUNE_CHAR_MINOR_UNIT_PLAY_STATUS

	// Fires every three seconds for the whole session, so only write when a
	// value actually moved. The cache lives on the session, which means one
	// redundant write after a reconnect - cheaper than the alternative of
	// tracking it globally and getting it wrong when the character changes.
	if( kStatus.m_iCurHP		== kSes.m_iSavedHP &&
		kStatus.m_iCurMP		== kSes.m_iSavedMP &&
		kStatus.m_iCurHyperGage	== kSes.m_iSavedHyper &&
		kStatus.m_iCharAbilCount== kSes.m_iSavedAbil )
	{
		return true;
	}

	CX2OfflineDB::Instance()->SaveGamePlayStatus( kSes.m_nSelectedUnitUID,
		kStatus.m_iCurHP, kStatus.m_iCurMP, kStatus.m_iCurHyperGage, kStatus.m_iCharAbilCount );

	kSes.m_iSavedHP		= kStatus.m_iCurHP;
	kSes.m_iSavedMP		= kStatus.m_iCurMP;
	kSes.m_iSavedHyper	= kStatus.m_iCurHyperGage;
	kSes.m_iSavedAbil	= kStatus.m_iCharAbilCount;

	return true;
}

//////////////////////////////////////////////////////////////////////////

bool CX2OfflineServer::Handler_EGS_FIELD_UNIT_SYNC_DATA_NOT( KOfflineSession& kSes, const KEvent& kEvent )
{
	KEGS_FIELD_UNIT_SYNC_DATA_NOT kReq;
	if( false == ReadReq( kEvent, kReq ) )
		return false;

	// No reply. The real server turns this into EGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT
	// for everyone else in the sector; offline there is nobody, so the broadcast
	// half is correctly absent.
	//
	// What is worth keeping is the position. Without it, last_line_index and
	// last_pos_value are only ever written once, by
	// EGS_FIELD_LOADING_COMPLETE_REQ - which reports where the character
	// *spawned*, so the stored position never actually changes and "log back in
	// where you left off" silently does nothing. This is the only packet that
	// reports where the character is standing.
	if( 0 == kSes.m_nSelectedUnitUID )
		return true;

	CX2OfflineDB::Instance()->SaveLastFieldPos( kSes.m_nSelectedUnitUID,
		(int)kReq.m_ucLastTouchLineIndex, (int)kReq.m_usLastPosValue );

	return true;
}

//////////////////////////////////////////////////////////////////////////

bool CX2OfflineServer::Handler_EGS_UPDATE_BATTLE_FIELD_USER_POS_NOT( KOfflineSession& kSes, const KEvent& kEvent )
{
	KEGS_UPDATE_BATTLE_FIELD_USER_POS_NOT kReq;
	if( false == ReadReq( kEvent, kReq ) )
		return false;

	// This was an explicit-ignore stub, on the reasoning that the real server
	// only rebroadcast it and there is nobody to rebroadcast to. Half right: it
	// is also the ONLY packet that reports where the character is standing in a
	// battlefield, and the login path needs that.
	//
	// The client's own re-entry code is what makes it matter
	// (X2StateServerSelect.cpp:4444-4450). On login it looks at the map it last
	// saw and, when that is a field rather than a village, re-joins the field
	// with the stored coordinates:
	//
	//     if ( VMI_BATTLE_FIELD_RUBEN_FIELD_01 <= pUnitData->m_nMapID &&
	//          VMI_BATTLE_FIELD_END            >  pUnitData->m_nMapID )
	//         Handler_EGS_JOIN_BATTLE_FIELD_REQ( pUnitData->m_nMapID,
	//                                            pUnitData->m_ucLastTouchLineIndex,
	//                                            pUnitData->m_usLastPosValue );
	//
	// All three come out of KUnitInfo::m_kLastPos, i.e. out of unit.last_pos,
	// last_line_index and last_pos_value. The battlefield join already stores the
	// field's ID in last_pos, so that branch was being taken correctly - but the
	// line and position still held whatever the *village* last wrote, because
	// EGS_FIELD_UNIT_SYNC_DATA_NOT is sent by CX2TFieldGame (the village) and
	// never by a field. Re-entering therefore placed the character at a village
	// line index inside a field, which is precisely the out-of-bounds spawn this
	// is meant to fix.
	//
	// One shared pair of columns for both map kinds is the studio's design, not a
	// shortcut: the server keeps a single KGSUser::m_kLastPos and overwrites it
	// from whichever place the player is in. The two encodings of m_usLastPosValue
	// differ - a village writes floatToHalf( ratio along the line )
	// (X2StateField.cpp:1400) and a field writes sqrtf( distance ) * 100
	// (X2BattleFieldGame.cpp:1031) - and that is safe because last_pos says which
	// map it belongs to, so the reader always knows which one it is looking at.
	if( 0 == kSes.m_nSelectedUnitUID )
		return true;

	// And this is the trigger for the field's monsters, which is the whole reason
	// it lives on this packet rather than on the join ACK. The client builds this
	// push out of GetMyUnit()->GetLastTouchLineIndex()
	// (X2BattleFieldGame.cpp:1014), so its arrival is proof that
	// CX2StateBattleField exists and the player's unit is in the world - which is
	// exactly the condition Handler_EGS_BATTLE_FIELD_NPC_LOAD_NOT branches on
	// (X2StateBattleField.cpp:1141). Sending the monsters from the join ACK
	// instead would race the XGM_STATE_CHANGE that creates that state.
	PushFieldNpcs( kSes );

	// ...and the field's heartbeat. This push is the closest thing a field has to
	// a clock - it arrives as the player moves - so respawns and the middle boss
	// ride it. A player standing perfectly still stops receiving it and the field
	// stops refilling, which is a real limitation but a harmless one: nothing is
	// being killed either.
	TickField( kSes );

	// KGSUser's own guard (GSUserBattleField.cpp:1032): a position for a map the
	// player is not on is stale, and writing it would move them somewhere they
	// have never been. Offline the current field is the room's.
	if( false == m_kRoom.m_bActive ||
		kReq.m_kLastPos.m_iMapID != m_kRoom.m_kInfo.m_iBattleFieldID )
	{
		return true;
	}

	CX2OfflineDB::Instance()->SaveLastFieldPos( kSes.m_nSelectedUnitUID,
		(int)kReq.m_kLastPos.m_ucLastTouchLineIndex,
		(int)kReq.m_kLastPos.m_usLastPosValue );

	return true;
}

#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_VIEW
// char-luciel DU5: June 0x4d9; stand-in ( no June server ): the form goes to unit.minor_unit_form ( the select reads it ) and 0x4da answers
bool CX2OfflineServer::Handler_EGS_CHANGE_UNIT_FORM_REQ( KOfflineSession& kSes, const KEvent& kEvent )
{
	KEGS_CHANGE_UNIT_FORM_REQ kReq;
	if( false == ReadReq( kEvent, kReq ) )
		return false;

	KOfflineUnitRow kRow;
	if( 0 == kSes.m_nSelectedUnitUID || false == CX2OfflineDB::Instance()->LoadUnit( kSes.m_nSelectedUnitUID, kRow ) )
	{
		CX2OfflineLog::Server( L"[JUNE_CHAR] LUCIEL DUO FORM REQ %d ignored: no selected unit", (int)kReq.m_cMinorUnitForm );
		return true;
	}

	if( CX2Unit::UT_LUCIEL != CX2OfflineStatTable::UnitTypeOf( kRow.m_iUnitClass ) )
	{
		CX2OfflineLog::Server( L"[JUNE_CHAR] LUCIEL DUO FORM REQ %d ignored: unit %I64d class %d is not Lu/Ciel", (int)kReq.m_cMinorUnitForm, (__int64)kRow.m_nUnitUID, kRow.m_iUnitClass );
		return true;
	}

	const int iMinorUnitForm = ( 1 == kReq.m_cMinorUnitForm ) ? 1 : 0;
	CX2OfflineDB::Instance()->SaveMinorUnitPlayStatus( kRow.m_nUnitUID, kRow.m_iMinorUnitCurMP, iMinorUnitForm );
	kSes.m_iSavedMinorUnitMP	= kRow.m_iMinorUnitCurMP;
	kSes.m_iSavedMinorUnitForm	= iMinorUnitForm;

	KEGS_CHANGE_UNIT_FORM_NOT kNot;
	kNot.m_iUnitUID			= kRow.m_nUnitUID;
	kNot.m_cMinorUnitForm	= (char)iMinorUnitForm;
	CX2OfflineLog::Server( L"[JUNE_CHAR] LUCIEL DUO FORM unit %I64d: form %d -> %d saved, NOT sent", (__int64)kRow.m_nUnitUID, kRow.m_iMinorUnitForm, iMinorUnitForm );
	return Reply( kSes, EGS_CHANGE_UNIT_FORM_NOT, kNot );
}
#endif SERV_IRUHADEV_JUNE_CHAR_DUO_VIEW

#ifdef SERV_IRUHADEV_OFFLINE_SPA_BUFF
// june-spa S3a: stand-in for June's missing server answer to 0x637 ( "Decisions" 1: refresh on every entry; 0x639 is never sent )
bool CX2OfflineServer::Handler_EGS_SPA_SOAK_REQ( KOfflineSession& kSes, const KEvent& kEvent )
{
	KEGS_SPA_SOAK_REQ kReq;
	if( false == ReadReq( kEvent, kReq ) )
		return false;

#ifdef SERV_IRUHADEV_OFFLINE_SPA_SOAK_TIMER
	// june-spa S3c: the user's soak countdown replaces the grant on entry below
	return OnSpaSoakReq( kSes, kReq.m_bInPool, (int)kReq.m_cSpaBuffType );
#else SERV_IRUHADEV_OFFLINE_SPA_SOAK_TIMER

	const int iSpaBuffType = (int)kReq.m_cSpaBuffType;
	if( false == kReq.m_bInPool )
	{
		CX2OfflineLog::Server( L"[JUNE_SPA] emu soak REQ out of the pool, type=%d: no reply", iSpaBuffType );
		return true;
	}

	int iBuffID = 0;
	switch( iSpaBuffType )
	{
	case CX2LocationManager::SBT_GROW:		iBuffID = BTI_SPA_GROW_BUFF;	break;
	case CX2LocationManager::SBT_STRONG:	iBuffID = BTI_SPA_STRONG_BUFF;	break;
	default:
		CX2OfflineLog::Server( L"[JUNE_SPA] emu soak REQ type=%d is no SPA_BUFF_TYPE: ignored", iSpaBuffType );
		return true;
	}

	if( 0 == kSes.m_nSelectedUnitUID )
	{
		CX2OfflineLog::Server( L"[JUNE_SPA] emu soak REQ type=%d ignored: no selected unit", iSpaBuffType );
		return true;
	}

	__int64 tBuffEndTime = 0;
	if( false == CX2OfflineBuff::Instance()->OnSoakSpa( kSes.m_nSelectedUnitUID, iBuffID, tBuffEndTime ) )
	{
		CX2OfflineLog::Server( L"[JUNE_SPA] emu buff=%d not granted ( see the BUFF line above ): no reply", iBuffID );
		return true;
	}

	CX2OfflineLog::Server( L"[JUNE_SPA] emu grant buff=%d sec=%I64d stand-in: refreshed on every pool entry ( type=%d, end=%I64d )",
		iBuffID, tBuffEndTime - _time64( NULL ), iSpaBuffType, tBuffEndTime );

	KEGS_SPA_SOAK_ACK kAck;
	kAck.m_iOK			= NetError::NET_OK;
	kAck.m_bSoaking		= true;
	kAck.m_tBuffEndTime	= tBuffEndTime;
	Reply( kSes, EGS_SPA_SOAK_ACK, kAck );

	SendUpdateBuffInfo( kSes );
	return true;
#endif SERV_IRUHADEV_OFFLINE_SPA_SOAK_TIMER
}

#ifdef SERV_IRUHADEV_OFFLINE_SPA_SOAK_TIMER
namespace _CONST_OFFLINE_SERVER_S3C_
{
	const int SPA_SOAK_SECONDS = 60;	// june-spa S3c stand-in: soak time that earns the spa buff ( user 2026-11-17; June's server value is lost )
}
using namespace _CONST_OFFLINE_SERVER_S3C_;

// june-spa S3c: { 1, type } starts or resumes the countdown and answers its end, { 0, -1 } pauses it; seconds soaked are kept
bool CX2OfflineServer::OnSpaSoakReq( KOfflineSession& kSes, bool bInPool, int iSpaBuffType )
{
	if( 0 == kSes.m_nSelectedUnitUID )
	{
		CX2OfflineLog::Server( L"[JUNE_SPA] emu soak REQ in=%d type=%d ignored: no selected unit", (int)bInPool, iSpaBuffType );
		return true;
	}

	if( kSes.m_nSpaSoakUnitUID != kSes.m_nSelectedUnitUID )
	{
		kSes.m_nSpaSoakUnitUID	= kSes.m_nSelectedUnitUID;
		kSes.m_iSpaSoakBuffID	= 0;
		kSes.m_iSpaSoakLeftSec	= SPA_SOAK_SECONDS;
	}

	const __int64 tNow = _time64( NULL );
	bool bPaused = false;
	if( 0 != kSes.m_iSpaSoakBuffID )
	{
		const __int64 tLeft = kSes.m_tSpaSoakGrantAt - tNow;
		kSes.m_iSpaSoakLeftSec	= ( tLeft > 0 ) ? (int)tLeft : 0;
		kSes.m_iSpaSoakBuffID	= 0;
		bPaused = true;
	}

	if( false == bInPool )
	{
		if( true == bPaused )
			CX2OfflineLog::Server( L"[JUNE_SPA] emu soak pause left=%d sec stand-in: the countdown resumes on the next pool entry", kSes.m_iSpaSoakLeftSec );
		return true;
	}

	int iBuffID = 0;
	switch( iSpaBuffType )
	{
	case CX2LocationManager::SBT_GROW:		iBuffID = BTI_SPA_GROW_BUFF;	break;
	case CX2LocationManager::SBT_STRONG:	iBuffID = BTI_SPA_STRONG_BUFF;	break;
	default:
		CX2OfflineLog::Server( L"[JUNE_SPA] emu soak REQ type=%d is no SPA_BUFF_TYPE: ignored", iSpaBuffType );
		return true;
	}

	kSes.m_iSpaSoakBuffID	= iBuffID;
	kSes.m_tSpaSoakGrantAt	= tNow + kSes.m_iSpaSoakLeftSec;
	CX2OfflineLog::Server( L"[JUNE_SPA] emu soak start buff=%d left=%d sec stand-in: %d s in the pool earns the buff ( end=%I64d )",
		iBuffID, kSes.m_iSpaSoakLeftSec, SPA_SOAK_SECONDS, kSes.m_tSpaSoakGrantAt );

	KEGS_SPA_SOAK_ACK kAck;
	kAck.m_iOK			= NetError::NET_OK;
	kAck.m_bSoaking		= true;
	kAck.m_tBuffEndTime	= kSes.m_tSpaSoakGrantAt;
	return Reply( kSes, EGS_SPA_SOAK_ACK, kAck );
}

// june-spa S3c: at the countdown's end the buff is (re)granted and the next countdown starts, so staying in tops the buff up
void CX2OfflineServer::TickSpaSoak( KOfflineSession& kSes )
{
	if( 0 == kSes.m_iSpaSoakBuffID || kSes.m_nSpaSoakUnitUID != kSes.m_nSelectedUnitUID )
		return;

	const __int64 tNow = _time64( NULL );
	if( tNow < kSes.m_tSpaSoakGrantAt )
		return;

	const int iBuffID = kSes.m_iSpaSoakBuffID;
	__int64 tBuffEndTime = 0;
	if( true == CX2OfflineBuff::Instance()->OnSoakSpa( kSes.m_nSelectedUnitUID, iBuffID, tBuffEndTime ) )
	{
		CX2OfflineLog::Server( L"[JUNE_SPA] emu grant buff=%d sec=%I64d stand-in: earned by %d s in the pool", iBuffID, tBuffEndTime - tNow, SPA_SOAK_SECONDS );
		SendUpdateBuffInfo( kSes );
	}
	else
	{
		CX2OfflineLog::Server( L"[JUNE_SPA] emu buff=%d not granted ( see the BUFF line above ); the countdown restarts", iBuffID );
	}

	kSes.m_iSpaSoakLeftSec	= SPA_SOAK_SECONDS;
	kSes.m_tSpaSoakGrantAt	= tNow + SPA_SOAK_SECONDS;
	KEGS_SPA_BUFF_END_TIME_NOT kNot;
	kNot.m_tBuffEndTime = kSes.m_tSpaSoakGrantAt;
	CX2OfflineLog::Server( L"[JUNE_SPA] emu soak next countdown end=%I64d ( 0x639 )", kNot.m_tBuffEndTime );
	Reply( kSes, EGS_SPA_BUFF_END_TIME_NOT, kNot );
}
#endif SERV_IRUHADEV_OFFLINE_SPA_SOAK_TIMER
#endif SERV_IRUHADEV_OFFLINE_SPA_BUFF

#endif SERV_IRUHADEV_OFFLINE
