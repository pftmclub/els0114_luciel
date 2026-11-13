# June dungeons - C3z-4t4c - the offline server leaves the room on a field change

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase june-dungeons C3z-4t4c` |
| Plan doc | `JUNE_DUNGEONS_PHASES.md` (branch `main`) |
| Depends on | C3z-4t4 review (done 2026-11-13, `f9d757f8`) |
| Flag | `SERV_IRUHADEV_OFFLINE_LEAVE_ROOM_ON_FIELD_CHANGE` (no `_DEBUG`: the existing `ROOM` line is the evidence) |
| Log tag | offline_server.log `ROOM     leaving room` and `QUEST` lines (`python tools/logs.py --grep "ROOM     leaving|VISIT_FIELD done|VISIT_VILLAGE done|C3z-4t begin"`) |
| Review | none (one emulator handler, transcribed from the real server); the play-test is the gate |

## Read first

```sh
python tools/docsection.py JUNE_DUNGEONS_PHASES.md "C3z-4t4 - Review"
```

Code: `X2Lib/Offline/Handlers_Field.cpp` `Handler_EGS_STATE_CHANGE_FIELD_REQ` (:62); `X2Lib/Offline/Handlers_Room.cpp`
`Handler_EGS_LEAVE_ROOM_REQ` (:1469, ends with the buff `OnLeaveRoom`, `m_kRoom.Clear()`, `S_FIELD_MAP`); `X2Lib/Offline/Handlers_Quest.cpp`
`QuestCheckHereAndNow` (:119). Real server: `KncWX2Server/GameServer/GSUserField.cpp` `IMPL_ON_FUNC( EGS_STATE_CHANGE_FIELD_REQ )` (:42, the
`S_ROOM` switch near its end, ~:276-334) and `_IMPL_ON_FUNC( ERM_LEAVE_ROOM_FOR_ENTER_THE_VILLAGE_ACK )` (:596, buff `OnLeaveDungeonRoom` /
`OnLeaveBattleField`). The server sources are CP949: read them, never edit them.

## The defect (found 2026-11-13 by the user during the C3z-4t4 review's walk)

Walking from a battle field to a village (portal `PMT_MOVE_TO_VILLAGE`), and returning to the village after a dungeon result, the client sends
only `EGS_STATE_CHANGE_FIELD_REQ`, no `EGS_LEAVE_ROOM_REQ` (offline_packets.log 2026-11-13 15:14:45; no `ROOM leaving room` line). The real
server leaves the room inside that handler when the user is in `S_ROOM` (battle field: `ERM_LEAVE_ROOM_REQ` reason `NOT_LEAVE_ROOM_REASON_27`,
or `_30` with a warp item; square, training centre and the default case their own requests). The emulator's handler sets `S_FIELD_MAP` but
leaves `m_kRoom` active, so every later "where is the player" test still sees the field or dungeon just left. Seen: the user visited field 40000,
returned to Ruben, handed in 11000; the after-quest auto-open accepted 11005 and `QuestCheckHereAndNow` ran `OnVisitField( 40000 )` from the stale
room, so `11005 sub 11005 VISIT_FIELD done (field 40000)` was logged in the village (offline_server.log 15:14:58.704). Any quest accepted in a
village gets visit credit for the last field or dungeon; other `m_kRoom.m_bActive` readers (buffs, inventory room type, skill, continue) see the
stale room too.

## Task

1. In `Handler_EGS_STATE_CHANGE_FIELD_REQ`, under the flag, before the ACK: when `m_kRoom.m_bActive`, leave the room as the real server does for
   that room type, reusing what `Handler_EGS_LEAVE_ROOM_REQ` already does (the room-type buff end through `CX2OfflineBuff::OnLeaveRoom` under
   `SERV_IRUHADEV_OFFLINE_ITEM_BUFF`, the `ROOM leaving room %I64d (reason=%d)` line with the server's reason value from NetError, then
   `m_kRoom.Clear()`). Factor the shared tail into one helper rather than copying it; keep the buff update ordered as the server does (ACK, then
   the buff list). Do not send `EGS_LEAVE_ROOM_ACK` (the client never asked).
2. Audit every `m_kRoom` reader that could run after a field change and expect the old room (grep `m_kRoom` in `X2Lib/Offline`): the training
   centre note at `Handlers_Room.cpp:798` ("Not m_kRoom.Clear()"), the dungeon result / continue-end paths, the reconnect note at
   `X2OfflineServer.cpp:181`. Say per reader why clearing on a field change is safe, or keep it working.
3. No other behaviour change. `QuestCheckHereAndNow` stays as it is: with the room cleared it falls through to `OnVisitVillage( m_iLastPos )`,
   which is already right.

## Play-test hand-off (gate)

A new level-1 character (any March class): the tutorial dungeon into Ruben -> `ROOM leaving room` on the village entry after the result; go to the
Ruben field and back -> `ROOM leaving room ... (reason=<27's value>)`; hand in 11000 in the village -> 11005 accepted and **not** done (no
`VISIT_FIELD done` line in the village); go to the field -> `11005 sub 11005 VISIT_FIELD done (field 40000)` on field entry. The same run can
take the C3z-4t4 review's owed scene-201 walk: hand in 11000 before visiting the field, ESC -> Character Select -> same character ->
`begin 201 ( by going quest )` (needs a build with `SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG` on for that line; it was retired, so either switch it
on for this build and off again in the Done commit, or judge 201 by the guide appearing on screen).

No hardcoded enum literals (user, 2026-10-26; CLAUDE.md): the leave reasons are `NetError::NOT_LEAVE_ROOM_REASON_*` members, room types
`CX2Room::RT_*`. No `June` prefix (user, 2026-10-28; CLAUDE.md). Offline-only (CLAUDE.md): no server-sync questions.
