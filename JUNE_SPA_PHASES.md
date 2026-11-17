# June spa villages - phased plan (`june-spa`)

Backlog Stage 24 (`JUNE_PORT_BACKLOG_ORDER.md`, "Stage 24: June spa villages (atlas E1d, 2026-10-17)"). Planned 2026-11-16 at the user's
request ("port the spa mode from June's"). Branch `main`. Kickoffs `kickoffs/june-spa/`. Log tag `[JUNE_SPA]`. Flags `SERV_IRUHADEV_JUNE_SPA_*`.

## Context

June 2017 has two spa villages, `VMI_SPA_RUBEN` 1500 and `VMI_SPA_RANOX` 1501. A door in Ruben (start pos 5) and one in Ranox (start pos 146)
lead in; two exit points in each spa lead back. Inside, the line the player stands on (the pool) picks a spa type from the village's `SPA_INFO`;
after a soak timer the client asks the server, and the server's reply shows the spa notice, plays `EffectSet_SPA_Buff` on the character and
(server side) grants the 20-minute spa buff (`BTI_SPA_STRONG_BUFF`, factor `BFI_SPA_STRONG_BUFF`). Each spa also has a "Pporu" NPC with a small
click game (field-game member `+0x624`), a wisp NPC with rock-scissors-paper, and a billboard.

March has none of this. Atlas E1d (2026-10-17) already landed the parser half: `VillageParsing` reads `IsSpaEnter` / `IsSpaOut` / `SPA_INFO`,
and June's link filter keeps spa rows out of the portal list. On today's data, walking up to the Ranox door does nothing (user, E1d play-test).

**Goal:** a player walks into either spa, soaks, gets June's notice, effect and buff, can play the Pporu game, and walks back out.

## Rules

1. **June wins, whole bodies.** Every June function this plan touches goes in whole, inside March's own function under the phase flag with
   March's body in `#else`. A June function March lacks gets a name in the class's own pattern (CLAUDE.md, no `June` in names).
2. **The map decides.** S0 writes `june2017_re/spa/spa_map.tsv`. Every later phase ports only rows in it, and a phase that finds a new June
   function adds the row first. A row is `June` only after a read of the exe in this plan.
3. **No server, so a designed stand-in for the server's answer.** June's server logic for the spa packets is gone (no spa code in
   `KncWX2Server`, no spa row in `ServerResource/US`). The emulator answers the way S0's design says, under its own flag, with a logged
   `stand-in:` line (memory `design-missing-june-pieces`). The user approves the design before S3a writes it. There is no live DB to pull
   procedures from (user, 2026-11-16; CLAUDE.md rule rewritten the same day).
4. **The spa's code event is switched on from the save.** June builds the door lists only when `CEI_SPA_ON_OFF` (31,
   `CodeScriptEnum_KR.lua`) is on. Offline nothing sends code events, so S2a adds a save-DB code-event table that the emulator sends at login
   (standing rule "Game events are switched on from the save DB"). The table is general, the first row is 31 = on, and `tools/elsdb.py` switches
   rows. The rest of June's code-event logic stays June's.
5. **Data through the tools.** Core-list Lua (`DLG_Map_Village`, `DLG_Map_House`, `NPCTemplet`, `WorldData`, `MapData`, `Enum`) gets June's
   spa rows merged as BOM source (`luasrc.py`). Other June files arrive by `luasrc.py adopt-june` / `kom.py stage-june`, byte-exact. You pack and
   copy without asking. No new `.lua`.
6. **Scope.** Spa avatars (`DITT_SPA_AVATAR_*`) are Stage 11's. The spa event menu icon (`EventButtonIconInfo` -> `DLG_UI_Menu_Spa_Event.lua`)
   is Stage 9's, unless S0 shows it is the only way in. Spa achievements (`ACT_SPA_GAME_WIN`, `BTI_ALWAYS_SPA_BUFF_*`) are Stage 31's. The snow
   worlds (`dataFileName_CodeEvent`) stay off unless S0 finds a reason. Whatever else S0 finds goes through the scope rule (CLAUDE.md).
7. **One debug flag:** `SERV_IRUHADEV_JUNE_SPA_DEBUG`. S2a turns it on, Z retires it. Lines go to `offline_server.log` through
   `CX2OfflineLog::Server( L"[JUNE_SPA] ..." )`, throttled to about one a second.
8. Standing rules apply unchanged: branch `main`, explicit pathspecs, CP949 sources through `bytepatch.py`, no hardcoded enum literals (spa
   ids are in June's `Enum.lua`: use the names), one-line comments with the phase tag (`// june-spa S2b: June 323b2e80`), `junediverge.py`,
   June bugs verbatim with a `JUNE_BUGS.md` row, `tools/finish.py` at every finish.

## Starting context

- **March code already in place (Atlas E1d and others).**
  - `X2Lib/X2LocationManager.cpp:416-441`: `SPA_INFO` parser. `:477-478`: `IsSpaEnter` / `IsSpaOut`. `:604`: templet `m_vecSpaInfo`.
  - `X2Lib/X2LocationManager.cpp:1747-1754`: link filter. `:1974-1978`: `VMI_SPA_RUBEN` deliberately omitted.
  - `X2Lib/X2LocationManager.h:135-153`: `HI_SPA_*` 40141-40147. `:459-484`: `struct SpaInfo`. `:556-557`: start-pos flags.
  - `X2Lib/X2TFieldGame.cpp:1937`: the comment that `+0x624` stays null. `:3387`: the `HI_SPA_SIGN_*` test. `:5105` / `:5307`: Ranox-only cases
    (loading image and one more).
  - `X2Lib/X2Main.cpp:14376-14380`: world-index switch, Ruben (19) omitted. `X2Lib/X2FieldMapBase.cpp:450-644`: Ranox only.
    `X2Lib/X2GageUI.cpp:1083`, `X2Lib/X2SquareUnit.cpp:42`: both spa ids tested.
  - `X2Lib/X2BuffFactor.h:847`: `BFI_SPA_STRONG_BUFF` 949 (no `BFI_SPA_GROW_BUFF`). `X2Lib/X2PremiumBuffManager.cpp:758`.
  - `KncWX2Server/Common/Enum/Enum.h:233-236`: `VMI_SPA_RANOX` / `VMI_SPA_RUBEN`. `X2Lib/Offline/X2OfflineLuaEnumSeed.h:556`: Ranox only.
  - Emulator: `X2Lib/Offline/Handlers_Field.cpp:62` `Handler_EGS_STATE_CHANGE_FIELD_REQ` trusts any map id and saves it as the last position.
    The item-buff path (`SERV_IRUHADEV_OFFLINE_ITEM_BUFF`, `SendUpdateBuffInfo`) already grants and syncs buffs.
- **June code, found by the planning session** (leads, S0 confirms each):

  | June | What |
  |---|---|
  | `323bad00` @323bb5c9-323bb61f | TFG ctor: `IsEnableCode( 31 )`, then entrance list `324f2ca0` -> `+0x324`, exit list `324f2d30` -> `+0x334` |
  | `323bad00` @323bb4fc-323bb549 | ctor: in village 1500 / 1501 only, `new` 0x20-byte Pporu object (ctor `32ec76e0`) -> `+0x624` |
  | `323bad00` @323bc7d4-323bc7fe | ctor: spa state init `+0x460` .. `+0x484` (`+0x474` = -1 type, `+0x484` = -1 effect handle) |
  | `323b5970` (OnFrameMove) | near an entrance (`+0x324`, distance <= `3346bda8`) -> `323ab220`; in a spa: Pporu tick `32ec7240`, exit list (`+0x334`) -> `323ab390`, exit key `323a8a60` when `+0x461`, type pick `323ac9d0`, soak timers `+0x46c` / `+0x470` against `334355b0`, sends event **0x637** { flag, type } through `323c15e0`, clears `+0x462` / `+0x463` |
  | `323ab220` / `323ab390` / `323ab4f0` | entrance / exit prompt particle (`+0x3a0`), flags `+0x460` / `+0x461`, and their reset |
  | `323b2e80` @323b2ee9 | key handler: near an entrance -> store return pos and line (`g_pData+0x268` `+0x88`..`+0x94`) -> state change into the spa |
  | `323a8a60` | exit: input `31c488c0( 0 )`, close two UIs, `31f12630( 0 )` (= `Handler_EGS_STATE_CHANGE_FIELD_REQ`) |
  | `323ac9d0` | my unit's line (`+0x3d4`) in a `SPA_INFO` `LineIdx` list -> type `+0x474`; changed -> `+0x464`, none -> `+0x463` |
  | `323bda90` (EventProcess) | `+0x624` -> `32ec8330`; event **0x638** { ok, 8 bytes } -> `+0x478` / `+0x47c`, `+0x462` = 1; event **0x639** -> `+0x478` / `+0x47c` (twins `323ac910` / `323ac990`; readers `323be4c0` / `323be550`) |
  | `323aea40` (TFG vslot, render / notice) | in a spa with `+0x462` and no change: notice string 43074 (type 0) / 43476 (type 1), `EffectSet_SPA_Buff` -> handle `+0x484` (stopped by `321dafb0` otherwise); for house `HI_SPA_PPORU` NPCs (`+0x5b0` == 40147, `+0x5fa`) -> `32ec7820` |
  | `32ec7280` / `32ec7380` / `32ec7620` / `32ec7820` / `32ec7fa0` | Pporu object: `Alarm_SpaPporu.ogg`, `SPA_PPORU_Normal` / `_Click` / `_Success.DDS`, UI event `32ec8330` -> `32ec7fa0` |
  | `323ad5f0` | `LoadFieldImage`: `HQ_LoadingSpa_Ruben_01/02.tga`, `HQ_LoadingSpa_Ranox_01/02.tga` (March has Ranox only) |
  | `32501bb0` @325024de | local-map loader: `SpaData { ID }` (reader not found yet) |

  The 0x637 / 0x638 / 0x639 event names are not in the exe's strings and not in March's protocol: S0 names them from June's handler and
  struct shapes (or keeps them as numbers with a one-line comment, the rule for an id with no name).
- **June data** (census 2026-11-16; `june_src_fixed/` paths):
  - `data034_kom/DLG_Map_Village.src.lua:4424-4501` `Village[VMI_SPA_RUBEN]` and `:4502-4571` `Village[VMI_SPA_RANOX]`. Keys: `NAME`
    43064 / 43065, `BaseLocalStateID`, `HouseIDList` {inner NPC, `HI_BILLBOARD`, `HI_SPA_PPORU`}, `WORLD_ID` 725 / 726, `CAMERA {1800,160,300}`,
    `SPA_INFO {{Type = SBT_STRONG, LineIdx = ...}}`, five `START_POS` each (410-414 / 420-424; two `IsSpaOut`, one `IsWarp`).
  - Doors: Ruben `:59-67` (start pos 5, `IsSpaEnter`, `OtherPlaceID = VMI_SPA_RUBEN`) **missing in March**. Ranox `:980-987` (146) is in
    March. Signs: `HI_SPA_SIGN_RUBEN` (Ruben `HouseIDList`) missing, `HI_SPA_SIGN_RANOX` present.
  - `DLG_Map_House.src.lua`: 9119 `HI_SPA_SIGN_RUBEN`, 9201 `HI_SPA_INNER_NPC_RUBEN` (`ROCKSCISSORSPAPER_NPC`), 9246 `_RANOX`, 9366
    `HI_SPA_PPORU` (with `COMMON_NPC_LOCATION` rows for both spa worlds). March lacks 40141 / 40143 / 40144 / 40147.
  - `DLG_Map_House_NPC_RelationData.src.lua:863-875`: the spa billboard rows. `NPCTemplet.src.lua:29408-29412`: `NUI_SPA_PPORU`
    (`UI_SPA_PPORU.lua`, `Motion_Spa_Bboru.x`). `MapData.src.lua:382-405`: `AddMapData` for both (level 10 / 65 - 99, `DI_RUBEN_SWAMP_NORMAL`).
  - `data040_kom/WorldData.src.lua:3594-3603`: `WI_SPA_RUBEN` (`Spa_Ruben.lua`, snow `Spa_Ruben_Snow.lua`), `WI_SPA_RANOX` (`Spa_Ranox.lua`,
    `Spa_Ranox_Snow.lua`). World luas set `Spa_*_LineMap.lua` and BGM `LI_SPA_Ruben001.ogg` / `LI_SPA_Ranox001.ogg`.
  - Enum (`data036_kom/Enum.src.lua`): `VMI_SPA_RUBEN` 1500, `WI_SPA_*` 725 / 726, `HI_SPA_*`, `NUI_SPA_PPORU` 10196, `SPA_BUFF_TYPE`
    (`SBT_GROW` 0, `SBT_STRONG` 1), `BTI_SPA_GROW_BUFF` 918 / `_STRONG_` 919, `BFI_SPA_GROW_BUFF` 948 / `_STRONG_` 949,
    `UI_SPA_CUSTOM_MSG.USCM_ENTER` 75000. March lacks `VMI_SPA_RUBEN`, `HI_SPA_SIGN_RUBEN`, `HI_SPA_INNER_NPC_*`, `HI_SPA_PPORU`,
    `NUI_SPA_PPORU`, `NUI_WISP_EVENT`, `NUI_SHY_WISP_EVENT`.
  - Buffs: `BuffTemplet.src.lua:1324` / `:1336` (1200 s; STRONG is `NOT_USE_IN_PVE`, dungeon / battle field), `BattleFieldBuffFactor.src.lua`
    130473 / 130557. March's `UIEffectSet.lua` already has `EffectSet_SPA_Buff`.
  - `DLG_Map_Local_Ranox.lua` (March data217) already has `SpaData`; March's `DLG_Map_Local_Ruben.src.lua:36-38` has the ID commented out
    (June UI parity Phase 3).
  - UI: `DLG_Destination_byButton` `Village_1500` / `_1501`, `DLG_UI_Field_Title` `SPA_RUBEN` / `SPA_RANOX`, NPC UIs `UI_MUL_MAI`,
    `UI_MULSAYRENA`, `UI_NPC_SHY_WISP`, `UI_WISP`, `UI_NPC_BILLBOARD_SPA`, `UI_SPA_PPORU` (location unknown).
  - Assets: 80 of the 85 files the four spa world luas name are already in March (meshes in data023, textures). Missing: the six world /
    line-map luas, `Motion_Wisp_EVENT*.xet` (datai027), `Motion_Spa_Bboru.x`, the five NPC / event UI luas, and three sounds
    (`Amb_Ruben_Spring_Bird04.ogg`, `LI_SPA_*001.ogg`; check loose `data\` first).
- **Tools.** Ghidra (skill `ghidra`, read-only), `june2017_re/dispsweep.py <hex offsets>`, `funcdis.py`, `disasm_range.py`; `luasrc.py`,
  `kom.py stage-june / pack / copy`, `luabind.py check`, `tools/elsdb.py`, `tools/finish.py`, `tools/build.py --deploy`, `tools/logs.py --tag
  JUNE_SPA`, `tools/exestr.py`, `tools/playtest.py`. Never read this doc whole: `docsection.py`.
- **Neighbours.** `X2TFieldGame.*` is shared with Stage 38 (`june-field`, planning only today) and char ports that hook field NPCs: one
  session at a time in that file. data034 `DLG_Map_Village` / `DLG_Map_House` are shared with june-dungeons W3d (the village NPC roster, which
  lists "spa sign"): one packer at a time. W3d keeps the village roster; this plan owns every spa row (1500 / 1501, the doors, the 4014x houses).

## Design

- **Map** `june2017_re/spa/spa_map.tsv`: `june_va`, `what`, `march` (file:line or `-`), `state` (`June` / `March` / `mixed` / `absent` /
  `route:<stage>`), `phase`, `readers`. funcmap rows tagged `Spa S24`.
- **Code-event switch (S2a).** `X2OfflineDB` migration adds `code_events( event_id INTEGER PRIMARY KEY, enabled INTEGER NOT NULL, note TEXT )`
  seeded with `( 31, 1, 'CEI_SPA_ON_OFF' )`. At login, after the unit list, the emulator sends `EGS_UPDATE_CODE_EVENT_NOT` with the enabled
  ids in June's shape (the client handler `31f00970` is already partly ported, C9h-2d). `tools/elsdb.py code-event list | on <id> | off <id>`.
  Flag `SERV_IRUHADEV_OFFLINE_CODE_EVENTS` (emulator + DB). Save backed up before the migration; migrations are forward-only.
- **Emulator spa answer (S3a).** Decided 2026-11-16 ("Decisions" 1): refresh on every pool entry; grant the type's buff for its `BuffTemplet`
  period, reply 0x638 with the end time; no 0x639.
- **Debug lines.** `[JUNE_SPA] door near=<0/1> pos=<id>`, `[JUNE_SPA] enter village=<id> return=<village>/<line>`,
  `[JUNE_SPA] type <old> -> <new> line=<n>`, `[JUNE_SPA] send 0x637 flag=<b> type=<n>`, `[JUNE_SPA] recv 0x638 ok=<b>`,
  `[JUNE_SPA] effect play/stop handle=<h>`, `[JUNE_SPA] emu grant buff=<BTI> sec=<n> stand-in`, `[JUNE_SPA] pporu <event>`.

## Measurement (planning session, 2026-11-16)

What the planning session read: Ghidra string search for spa names (`HQ_LoadingSpa_*`, `EffectSet_SPA_Buff`, `SPA_INFO`, `SpaData`,
`SPA_PPORU_*`, `Alarm_SpaPporu.ogg`), their xrefs, decompiles of `323aea40`, `323bda90`, `323b5970` (spa block), `323ac910`, `323ac990`,
`323ac9d0`, `323a8a60`, `323ab220`, `32ec7240`, the ctor listing @323bb4fc-323bb61f, and one `dispsweep.py` over TFG offsets `0x324`-`0x624`
restricted to `32390000-323d0000`. The data census is summarised in "Starting context". Not read yet (S0): the Pporu class body, 0x637's
send struct, 0x638 / 0x639 struct readers, `323b2e80` after the store, `SpaData`'s reader, the rock-scissors-paper NPC path, other
readers of `IsEnableCode( 31 )` outside the TFG ctor, and whether any non-TFG code (square, local map, party, UI) reads the spa ids.

## Phase S0 - Spa map and server design (read-only)

**Scope.** Write `spa_map.tsv` for every June function in the spa system and its March counterpart:
- every row in the "June code" table, read in the exe, plus everything they call that March lacks;
- every reader of TFG `+0x324`, `+0x334`, `+0x3a0`, `+0x460`-`+0x484`, `+0x624` (`dispsweep.py`, all hits, not only TFG's range);
- events 0x637 / 0x638 / 0x639: struct layouts (`323c15e0` arguments, `323be4c0`, `323be550`), June names if any, and every other
  handler of them (state menu, square, `CX2State` base);
- `IsEnableCode( 31 )` readers and `dataFileName_CodeEvent`'s code event;
- the Pporu object (ctor `32ec76e0`, all its methods, its UI file, any packet it sends), the wisp NPC's rock-scissors-paper
  (`ROCKSCISSORSPAPER_NPC`, `DLG_UI_RockScissorsPaper.lua`): is it only the spa's? Then it is S5, otherwise it is routed;
- `SpaData`'s reader (`32501bb0` store -> who reads it), `LoadFieldImage` spa cases, `X2Main` world-index, `X2FieldMapBase`, local-map /
  destination UI paths for 1500 / 1501;
- login with a saved position inside a spa: what June's client does with it (the server's answer is a design row).

Then: (1) the **emulator design** for 0x637 (options, each with June evidence, a recommendation) and the login-inside-spa answer, written to
"Decisions" for the user; (2) the **live-DB ask**: `SELECT OBJECT_NAME(object_id), definition FROM sys.sql_modules WHERE definition LIKE '%spa%'
AND OBJECT_NAME(object_id) LIKE 'gup_%'` (the user pastes the result; transcribe verbatim; "none" is an answer); (3) size S2-S5: split any
phase that holds more than about a dozen `absent` / `mixed` rows, amend kickoffs and STATUS rows; route drift (scope rule).

**Run with.** Opus 5.5 / xhigh; subagents do the greps, sweeps and raw Ghidra pulls. No review of its own (S2's review re-checks the rows it uses).

**Read list.** "Context", "Rules", "Starting context", "Design", "Measurement (planning session, 2026-11-16)".

**Done gate.** Every "June code" row and every sweep hit has a map row with a state, a phase and readers. Event structs written field by
field. "Decisions" holds the emulator options and the live-DB ask. Kickoffs S2-S5 amended. No client code, so: full build skipped, no play-test.

## Phase S0 - Done

2026-11-16, Opus 5.5 / xhigh. Read-only: no client code, so full build skipped (no play-test) and no gate applies (`finish.py` takes code files).
- **Map** `june2017_re/spa/spa_map.tsv`, 106 rows: every "June code" row read in the exe; the sweep hits by function (`not-spa` rows grouped);
  the spa-id readers outside TFG (immediate scan for 1500 / 1501 / `-1500` and houses 40141 / 40142 / 40147); RPS leads; emulator design rows.
  funcmap: 46 rows tagged `Spa S24`.
- **Sweeps.** `dispsweep.py` on +0x324..+0x340, +0x3a0, +0x460..+0x488, +0x624: 10,061 hits; 47 functions in the TFG range, 29 others that
  load `g_pTFieldGame`. A base-register scan finds no spa-offset hit outside TFG with `g_pTFieldGame` as its base: the spa state is private to
  the field game. The ctor's ebp-based hits (ebp = this there) were re-read from the locals file.
- **Events, field by field** (KSerializer type tags: 2 char, 5 int, 8 `__int64`, 0xc bool). June's packet names are not in the exe (no `EGS_`
  string table): the names are S0's, from the shapes; S3a appends them to March's id list under the flag (wire ids are March ordinals).
  - **0x637** `EGS_SPA_SOAK_REQ` C->S `{ bool m_bInPool; char m_cSpaBuffType }` (`323bf360` / `320acab0`), `KSession::SendPacket` (`323c15e0`:
    dest 0x202, logging on, no ack awaited). In a pool `{ 1, type }` after 1.0 s, once per entry (held while the end time is > 0 and the type is
    unchanged). Out of a pool `{ 0, -1 }` once a second for the first 3 s. TFG dtor in a spa: `{ 0, -1 }`.
  - **0x638** `EGS_SPA_SOAK_ACK` S->C `{ int m_iOK; bool m_bSoaking; __int64 m_tBuffEndTime }` (`320acb70`): `IsValidPacket( m_iOK )` and the
    bool -> end time to +0x478, notice on (+0x462).
  - **0x639** `EGS_SPA_BUFF_END_TIME_NOT` S->C `{ __int64 m_tBuffEndTime }` (`320acbe0`): replaces the end time only (0 lets the client ask again).
  - The end time is absolute: `323ac3e0` shows `mktime( localtime64( end ) ) - GetServerCurrentTime()` as M:SS. No other handler: no compare
    or push of 0x637-0x639 outside TFG; `CX2StateField` (`3250d2f0`) forwards to TFG `EventProcess`.
- **Entry.** Prompt (`323ab220`) within 150 of an `IsSpaEnter` start pos; the key handler stores the return position and opens an OK / Cancel box
  (STR 43070, OK = `USCM_ENTER` 75000); talking to sign 40141 / 40142 opens the same box. `USCM_ENTER` (`323b4040`) refuses level < 10
  (STR 43071), else asks for 1500 (from Ruben) or 1501 (from Ranox). **Exit:** up key near an `IsSpaOut` start pos ->
  `Handler_EGS_STATE_CHANGE_FIELD_REQ( 0 )`, which in a village sends 1500 to Ruben (2569, 60, 150) line 5 and 1501 to Ranox (-4100, 790, 266)
  line 31. The stored door position is not read there.
- **SpaData.** The local-map loader passes `SpaData.ID` to the templet and calls `VillageParsing( id )` (`32501bb0` @32502f9e). March reads no
  `SpaData`, so the spa villages' templets are never parsed today (S2a's first row).
- **Pporu** (`+0x624`, 0x20 bytes, no vtable or RTTI): NPC 40147 starts hidden, appears every 480-720 s (`rand() % 240 + 480`) with the "Start"
  anim and `Alarm_SpaPporu.ogg`; clicking it while it plays "Wait" sends June's shop-buy 0xe6 `{ 164548: 1 }` (Hard-boiled Egg, 20000 ED; the
  client checks bag space and ED first) and takes the 0xe7 ACK as a purchase. June's Pporu sells the egg, and the emulator's
  `Handler_EGS_BUY_ED_ITEM_REQ` already charges the price: no stand-in. Dev cheat: Lua `SetAppearPPoruCheat` (TFG +0x620, auth level > 2).
- **Rock-scissors-paper** is spa-only (`ROCKSCISSORSPAPER_NPC` only on houses 40143 / 40144; `AddRockScissorsPaperData` only for
  `NUI_SHY_WISP_EVENT` / `NUI_SAYRENA_EVENT`): S5 stays. Data (`CustomEventScript.lua`): 10000 ED a try, reward 164571 x1, valid 0.4-0.6 s, 3 s
  rounds, 11 s cool time. Packets 0x651 / 0x652 and 0x653 / 0x654; UI class around 328d0000-328d2d00.
- **Code event 31** readers: TFG ctor (door lists), village NPC creation (signs; March has it), `CX2StateMenu::CreateWarpDest` (both spa warp
  rows). The snow worlds use code event 46 (`X2WorldManager.cpp:111`): they stay off. The spa event icon is code event 29 and not the only way
  in: Stage 9.
- **Dead in June, not ported:** `323ac910`, `323ac990`, `323ad4c0`, `323b1e40`, `323aa1f0`, `3259beb0`, `325d2c60` (no caller, no xref).
- **Also found.** `UI_SPA_PPORU.lua` is named by June's `NPCTemplet` but is in no June kom (Pporu has `bCanTalkNpc = FALSE`): nothing to stage.
  Lua `ResetSpaRestExp` sends 0x64f with no payload, an admin reset of a server-only spa rest EXP with no June values: `route:none`.
- **Sizing.** S2 held 32 absent / mixed rows, S3 18, S4 23. Split: S2a plumbing (15 rows, mostly one-line Ruben cases), S2b doors (17, five of
  them a few lines), S3a wire + emulator (6), S3b client soak (12), S4a Pporu class (14 small methods), S4b hooks (9). One review per pair (at
  S2b, S3b, S4b). Seam S2b: S1 and S2a batch into it. S3a and S4a are inert (probe build; S3b / S4b test them).
- **Routed** (bullets added): ESC in a spa (`31f11c66`), the riding-pet stamina picture in a spa (`321f5e30` / `3294b030`) and the spa event
  icon -> Stage 9; Rose's Valkyrie gauge forced shown in a spa (`32e65970`) -> Stage 8 (char-rose).
- **Decisions** answered below. The live-DB ask is void: the user has no live-DB access anymore, and CLAUDE.md's rule was rewritten the same day.
- Rule note for S2a: March's `CEI_*` value 31 is `CEI_ALLOW_ELESIS_FIRST_CLASS_OLD_AND_NEW_YEAR_EVENT` (`ScriptID_Code.h:88`), a 2014 event;
  June's 31 is `CEI_SPA_ON_OFF`. The meaning differs, so S2a registers June's name beside it under the flag and uses it.

## Phase S1 - Spa data and assets

**Scope.** Everything the spa needs on disk, no C++:
- `Enum.lua` (data036, core): the missing spa members, June values (list in "Starting context").
- `DLG_Map_Village.lua` (data034, core): both `Village[...]` rows from June, the Ruben door (start pos 5) and `HI_SPA_SIGN_RUBEN` in Ruben's
  `HouseIDList`, with March key names where the file's note says so.
- `DLG_Map_House.lua` (+ `_1` / `_2` if they carry the rows; core): houses 40141, 40143, 40144, 40147.
- `DLG_Map_House_NPC_RelationData.lua`: the spa billboard rows (core-list it if it must be merged; adopt June's if whole-file is safe).
- `NPCTemplet.lua` (data037, core): `NUI_SPA_PPORU` plus the wisp NPC rows. `MapData.lua` (data036, core): both `AddMapData` blocks.
- `WorldData.lua` (data040, core): rows 725 / 726. World luas and line maps (adopt-june), the snow variants staged too (byte-exact rule) but not
  switched on.
- `DLG_Map_Local_Ruben.src.lua`: restore `SpaData.ID = VMI_SPA_RUBEN` (comment it as a June-parity restore).
- NPC / UI luas: `UI_MUL_MAI`, `UI_MULSAYRENA`, `UI_NPC_SHY_WISP`, `UI_WISP`, `UI_NPC_BILLBOARD_SPA` (adopt-june, data039). `UI_SPA_PPORU` is in
  no June kom (S0): nothing to stage. `DLG_Destination_byButton` / `DLG_UI_Field_Title`: June's spa entries are present or merged.
- Assets: `Motion_Spa_Bboru.x`, `Motion_Wisp_EVENT*.xet`, the three sounds, loading images `HQ_LoadingSpa_Ruben_01/02.tga` (and Ranox's if
  missing), every `.TET` beside a staged UI texture (memory `stage-tet-with-every-ui-texture`), `SPA_PPORU_*.DDS`, `Alarm_SpaPporu.ogg`.
  Gates: an asset closure over the four spa world luas and the spa NPC / UI luas prints 0 missing; `luabind.py check` passes for every staged
  NPC / UI lua (ENUM and STATE gates too); `luasrc.py core --check` and `ledger --check` pass.
- Pack and copy every touched kom.

**Run with.** Sonnet 5.5 / high (data work). Review: inside S2b's review (Opus). **Hand-off:** batch at S2b (regression section: Ruben, Ranox and
one other village still load, the Ruben sign and the spa houses appear, nothing new in `ErrorLog.txt`).

**Read list.** "Rules", "Starting context", "Phase S1 - Spa data and assets", S0's Done note.

**Done gate.** The gates above PASS (paste them), `PACK OK` / `COPY OK` per kom, `MODS.md` entry, the replaced files listed in the Done note.
Full build skipped: no play-test yet (batch at S2b).

## Phase S1 - Done

2026-11-16, Sonnet 5.5 / high. Data only: no client source, so no flag, `finish.py` / `flagcheck` / `junediverge` do not apply, and the full build is
skipped (no play-test yet; the S1 regression section is in `kickoffs/june-spa/playtest.md`, batch at S2b).
- **Core-list Lua merged (BOM source, June's rows verbatim).**
  - `Enum.lua` (data036): `NUI_WISP_EVENT` 10191, `NUI_SAYRENA_EVENT` 10192, `NUI_SHY_WISP_EVENT` 10193, `NUI_SPA_PPORU` 10196, `HI_SPA_SIGN_RUBEN` 40141,
    `HI_SPA_INNER_NPC_RUBEN` 40143, `HI_SPA_INNER_NPC_RANOX` 40144, `HI_SPA_PPORU` 40147, `VMI_SPA_RUBEN` 1500, table `UI_SPA_CUSTOM_MSG` (`USCM_ENTER` 75000). No value collides.
  - `DLG_Map_Village.lua` (data034): `Village[1500]` / `[1501]`, Ruben's door (start pos 5, `IsSpaEnter`) and `HI_SPA_SIGN_RUBEN` in Ruben's house list. June's
    `OtherPlaceID` / `OtherPlaceStartLineIndex` are kept verbatim on both doors (user, 2026-11-16: follow June; Ranox's door 146 is as it was). March's `VillageParsing` reads only
    `BattleFieldId` / `BattleFieldStartLineIndex`, so the destination is not stored yet: **S2a makes the parser read June's two keys** (`@32500ac4` / `@32500be0`), under its flag.
  - `DLG_Map_House.lua` (data034; the only house file March loads): 40141, 40143, 40144, 40147. `_1` / `_2` are not read. The file's one bare-LF line (:1108) is untouched.
  - `NPCTemplet.lua` (data037): the wisp, Sirena, shy wisp and Pporu rows (Mei and Meiseu's row was there). `MapData.lua` (data036): both `AddMapData` and Cobo zones 1 (Ruben spa) / 15
    (Ranox spa), as June. `WorldData.lua` (data040): `WI_SPA_RUBEN` / `WI_SPA_RANOX` with their snow variants (the snow file is read only under code event 46, which stays off).
  - `DLG_Map_Local_Ruben.lua` (data034): converted from its sidecar (T0, sidecar folded away), then `SpaData.ID = VMI_SPA_RUBEN` restored, as a June-parity line.
  - Rows in `tools/data/lua_core_list.tsv` updated for all seven; `DLG_Map_Local_Ruben.lua` added to the list by `convert`.
- **Adopted as June's bytecode (`adopt-june`, run record `june2017_re/june_adopt_20261116_212244.tsv`).** data040: the four spa world luas and the two line maps; data039: `UI_MULSAYRENA`,
  `UI_NPC_SHY_WISP`, `UI_WISP`. Byte-equal to June's already (not touched): `UI_MUL_MAI`, `UI_NPC_BILLBOARD_SPA`, `DLG_Map_House_NPC_RelationData` (it already holds the spa billboard rows;
  no core-list row needed), `DLG_UI_Field_Title`; `DLG_Destination_byButton` (core, June's whole file) already has `Village_1500` / `_1501`.
- **Assets.** Closure over the four spa world luas, both line maps, the five NPC / UI luas, the five spa meshes and the loading images / `SPA_PPORU_*.DDS` / `Alarm_SpaPporu.ogg`,
  walking every mesh's and `.xet`'s embedded textures: 106 names, **0 missing except two June does not have** (below). The files the plan listed as missing were mostly there already (June asset
  port): meshes, the loading images (Ruben's two as well), the `SPA_PPORU` pictures with their `.TET`, the alarm sound, `Amb_Ruben_Spring_Bird0*`. Only `li_spa_ruben001.ogg` was absent: it is loose
  music, copied with `kom.py copy-loose --dest music` (`COPY-LOOSE OK`; the Ranox one was already identical). `Motion_Wisp_EVENT*.xet` is not named by any spa file (it is `Wisp_EVENT.lua`'s,
  another NPC): not staged.
- **June has two dangling names (JUNE_BUGS #785, #786), kept:** `Amb_Ruben_Spring_Bird04.ogg` (June's file is `Bird04..ogg`) and `NUI_SPA_PPORU`'s `Motion_Spa_Bboru.x` / `UI_SPA_PPORU.lua`.
- **Gates.**
  - `luabind.py check` on the 11 NPC / UI / world / line-map luas: 0 unbound methods, ENUM 0 missing, `RESULT: PASS` (the STATE gate has no class script here).
  - `luasrc.py core --check`: `CORE CHECK PASS: 0 FAIL, 0 WARN`. `luasrc.py ledger --check`: `LEDGER CHECK PASS: 0 FAIL, 0 WARN`.
  - Load test (vendored luajit, Enum + DungeonEnum + `DLG_Map_Village.lua` + `DLG_Map_House.lua` as packed text): `Village[1500]` / `[1501]` with 5 start positions each, Ruben's door 5 -> 1500, the five spa
    houses present, no nil house id.
- **Packed and copied.** `COPY OK data034` (md5 946358ce), `data036` (00b2c16b), `data037` (b921ccc7), `data039` (35ac9a01), `data040` (5f251c2c); each `PACK OK`. data039 +3 files, data040 +6.
  Existing files replaced (backups in `kom_backup_tools\`): data034 `DLG_Map_House.lua`, `DLG_Map_Village.lua`, `DLG_Map_Local_Ruben.lua` (bytecode -> BOM source); data036 `Enum.lua`, `MapData.lua`; data037 `NPCTemplet.lua`; data040 `WorldData.lua`.
- **How to see it in game.** Nothing new is visible: the spa signs and door prompts need code event 31 (S2a's save table) and the door code (S2b). The S1 section is a regression check.
- **Left for S2a (noticed, in scope there).** `STR_ID_43064` ... `43091` exist in `StringID_def.lua` but their texts live in the client string table: S2b's first look at the OK box must read them. `Village[1500]` rows
  are parsed by `VillageParsing` only when `SpaData.ID` is read (S2a's first row). No open question for the user.

## Phase S2a - Code events, spa village plumbing, Ruben omissions

**Scope.** Flags `SERV_IRUHADEV_OFFLINE_CODE_EVENTS`, `SERV_IRUHADEV_JUNE_SPA_ENTRY`, `SERV_IRUHADEV_JUNE_SPA_DEBUG`; the map rows marked S2a:
- the code-event table, its migration, the login send and the `elsdb.py` command ("Design"); `IsEnableCode( 31 )` reads true offline;
  `CEI_SPA_ON_OFF` registered beside March's 31 (S0's rule note);
- `SpaData` in the local-map loader (`32501bb0`: templet arg + `VillageParsing`), so 1500 / 1501 get templets;
- `VillageParsing` reads June's `OtherPlaceID` / `OtherPlaceStartLineIndex` keys (S1 left them verbatim on the Ruben and Ranox spa doors; March's parser reads only the `BattleField*` names);
- March's Ruben omissions and the spa cases of the shared functions: `IsValidWarpField`, `WarpByCommand` 19, both `CreateWarpDest`s (with the
  code-event 31 gate), the warp-destination level-10 gate, `LoadFieldImage`, `LoadUI`'s loading-screen clause, `OpenFieldName`,
  `X2FieldMapBase` (two), the minimap channel combo, the offline enum seed; `VMI_SPA_RUBEN` registered where C++ lacks it (`JUNE_ENUM_NAMES`);
- emulator: last position as "Decisions" 2 says (saved like any village).
No door code (S2b), `+0x624` null (S4b), no soak (S3b).

**Run with.** Opus 5.5 / high. Review: in S2b's. **Hand-off:** batch at S2b (warp into both spas by console command; code event on / off).

**Read list.** "Rules", "Starting context", "Design", "Decisions", "Phase S2a - ...", S0's and S1's Done notes, the map rows marked S2a.

**Done gate.** `finish.py` PASS; probe build 0 errors (full build skipped: batch at S2b); section S2a in `playtest.md` (`playtest.py check` PASS).

## Phase S2a - Done

2026-11-17, Opus 5.5 / high. Flags `SERV_IRUHADEV_OFFLINE_CODE_EVENTS`, `SERV_IRUHADEV_JUNE_SPA_ENTRY`, `SERV_IRUHADEV_JUNE_SPA_DEBUG` (on; Z retires it).
- **Code events.** Save schema v25 (`X2OfflineDB`): `code_events( event_id PK, enabled, note )`, seeded `( 31, 1, 'CEI_SPA_ON_OFF' )`; save backed up
  first (`db_backup/els_db_20261117_034207_pre_june-spa-S2a-pre-v25.sql`). `LoadEnabledCodeEvents` -> `EGS_UPDATE_CODE_EVENT_NOT` (June's shape; C9h-2d's map
  + two empty sets) at the end of `PushServerSelectDefaults`, i.e. after both unit-list ACKs. June's handler `31f00970` re-read: map, two sets,
  `32344880` (ported, E1c), vslot `+0x100` (empty in `CX2State`; the menu-icon override is Stage 9, C9h-2e). `tools/elsdb.py code-event list | on <id> | off
  <id>` (takes effect at the next character list). `CEI_SPA_ON_OFF = 31` beside March's `CEI_ALLOW_ELESIS_FIRST_CLASS_OLD_AND_NEW_YEAR_EVENT`
  (`ScriptID_Code.h`, under `_CODE_EVENTS` because the seed names it; `_SPA_ENTRY` requires it). March's two 31 readers (`X2MemoryHolder.cpp:1049`
  / `:1213`) sit in the 2013 / 2014 new-year dialog, shown only under code events nothing seeds: no visible effect. The flag-on handler already drops
  March's `UpdateNewYear2014Event` calls (as June).
- **SpaData** (`32501bb0` @325024dc / @32502f9e): read after `BattleFieldRestData`, reset per local map, passed to the templet (`m_eSpaID`, June `+0x14`,
  ctor `32505910`), `VillageParsing( id )` when not -1 / 0. June's ctor also resets all five ids per map; March resets three (its rest id leaks to the next
  map without a `BattleFieldRestData` table): not changed, out of scope.
- **Door keys.** `VillageParsing` reads `OtherPlaceID` -> `m_uiBattleFieldId` (@32500887) and `OtherPlaceStartLineIndex` -> `m_usBattleFieldStartLineIndex`
  (@32500a7f), miss 0 in June; here they read after March's `BattleField*` names and keep those values when absent, because this file's other rows were
  renamed to March's keys by Atlas A5. (The kickoff's @32500ac4 / @32500be0 are the next keys, `WaitingTime` / `InternalPlaceStartPosID`.)
- **Shared functions, June's spa cases:** `IsValidWarpField` 1500; `WarpByCommand` 19; `CX2StateMenu::CreateWarpDest` first rows 1500 and 1501 (code
  event 31 on both, 1501 also `LMI_RANOX` level, @32462a4a-@32462b23); `CX2UIInventory::CreateWarpDest` 1500 first, no gate (@32173376; June's
  `DLG_Destination.lua` has no `Village_1500`, so it never shows: June's data); warp-destination gate (@32479a98): 1500 and level < 10 -> OK box string 43071
  at (250, 300), in June's state 14 = `XS_VILLAGE_MAP` (S0's "0xf" is June's guild-agit state; a warp from a battle field is not gated, as June);
  `LoadFieldImage` Ruben pictures; `LoadUI` (@323b0f52): the early return is skipped when entering a spa from a non-spa village (previous `+0x68`, new
  getter `GetOldVillageID`); `OpenFieldName` 29; both `X2FieldMapBase` functions; minimap channel combo (@3226440d); offline enum seed `VMI_SPA_RUBEN`.
  `VMI_SPA_RUBEN` was already registered (`Enum.h`, `JUNE_ENUM_NAMES`).
- **Emulator, last position ("Decisions" 2):** no change. `Handler_EGS_STATE_CHANGE_FIELD_REQ` saves any map id; login (`X2StateServerSelect.cpp:7330`)
  falls back to Ruben only when `GetVillageMapTemplet` is null, which `SpaData` now fills for 1500 / 1501.
- **Not done here:** `X2TFieldGame.cpp:3388` still tests the literal 31 for the signs (Atlas E1d's line): S2b touches that ctor and switches it to
  `CEI_SPA_ON_OFF`. June's guild agit (village 25000, state 15) seen in the loader tail, the Cobo list and the warp case -> backlog Stage 38 bullet.
- **Gates.** `finish.py --with` the three flags: flagcheck `JUNE_SPA_ENTRY` PASS (9 PASS, 1 DEFINE, 4 OTHER), `OFFLINE_CODE_EVENTS` PASS (1 PASS, 3 PASS~,
  1 DEFINE, 9 OTHER), `JUNE_SPA_DEBUG` PASS by hand (3 OTHER: its lines sit inside the other two flags' blocks), junediverge `RESULT: PASS`, enumscan exit 0;
  `FINISH RESULT: PASS`. CP949 files through `bytepatch.py`: 152 inserted lines, 0 deleted. Commit `d33805a3`: `finish.py --range d33805a3^..d33805a3`
  `FINISH RESULT: PASS` (same tallies).
- **Build.** Full build skipped: no play-test yet (batch at S2b). `/Zs` probe of the 10 changed `.cpp` (stdafx context): 0 errors, all three flags on.
- **Play-test:** deferred, section S2a in `playtest.md` (`playtest.py check` PASS), batch at S2b.
- **Open questions:** none.

## Phase S2b - Spa doors: entering and leaving

**Scope.** Flags `SERV_IRUHADEV_JUNE_SPA_ENTRY` (+ `_DEBUG`); the map rows marked S2b, June's code whole:
- TFG ctor: the two lists (`324f2ca0`, `324f2d30`) under June's code-event test; dtor frees them;
- OnFrameMove: the per-frame prompt reset, the entrance loop, the spa-village exit loop and exit key; the spa-sign talk flag in the NPC loop;
- the prompts `323ab220` / `323ab390` / `323ab4f0` (and its two callers), the door branch of the key handler `323b2e80`, `USCM_ENTER` in
  `323b4040`, the sign cases of `323b3690`, the exit `323a8a60`, and the spa return of `Handler_EGS_STATE_CHANGE_FIELD_REQ( 0 )` (June's body
  whole; its June-only states get labelled stand-ins).
Leave the Pporu lines (S4b) and the soak (S3b) out.

**Run with.** Opus 5.5 / high. Own review (`S2b-review.md`, covers S1 and S2a). **Hand-off:** now. **Seam:** `build.py --deploy`, then
`playtest.py assemble june-spa S1 S2a S2b` in full.

**Read list.** "Rules", "Starting context", "Design", "Decisions", "Phase S2b - ...", S0's, S1's and S2a's Done notes, the map rows marked S2b.

**Done gate.** Play-test: in Ruben and in Ranox, walk to the door, the prompt shows, the key opens June's box, OK enters the spa (loading image,
BGM, world, NPCs); a level-9 unit gets June's refusal; talking to the sign opens the same box; both exits lead to June's fixed spot by the door
(Ruben line 5, Ranox line 31). A relog inside a spa loads the spa. `elsdb.py code-event off 31`, relog: no doors, no signs, no spa warp rows.
Log lines `door`, `enter`, the emulator's `FIELD` lines. `exestr.py` for the debug tag.

## Phase S2b - Done

2026-11-17, Opus 5.5 / high. Flags `SERV_IRUHADEV_JUNE_SPA_ENTRY` (S2a's, description extended), `SERV_IRUHADEV_JUNE_SPA_DEBUG`. Map rows S2b: all `ported S2b`.
- **Lists.** `CX2LocationManager::GetVecSpaEnterPos` / `GetVecSpaOutPos` (`324f2ca0` / `324f2d30`: this village's start positions with `m_bIsSpaEnter` /
  `m_bIsSpaOut`). TFG ctor fills `m_vecSpaEnterPos` / `m_vecSpaOutPos` only when `IsEnableCode( CEI_SPA_ON_OFF )` (@323bb5b5); dtor clears them;
  `m_bJoinSpa` / `m_bExitSpa` (+0x460 / +0x461) start false. The sign test at `AddFieldNpc` now reads `CEI_SPA_ON_OFF` (`#else` keeps Atlas's `31`).
- **Prompts.** `ShowJoinSpa` / `ShowExitSpa` (`323ab220` / `323ab390`): the market arrow particle (`m_hMarketInArea`, +0x3a0) at pos + 270 y,
  `Get_In_*` drawn 60 lower, `Get_Out_*` not, frames by `m_fMarketTime` as March's market; hidden while dialogs are hidden. `HideJoinSpa` (`323ab4f0`)
  resets the **entrance prompt only** (S0's row said both; corrected in the map); callers `CX2CashShop::SetOpen` open path (@3223d97f) and
  `CX2StateField::SetShowStateDLG`'s hide branch (`32508a90` @32508e3e; S0's "CX2StateField slot").
- **OnFrameMove** (@323b6759-323b6aa4), inside March's `IsNearPortalToBattleField() == false` block after the market loop, as June: entrance reset,
  exit reset, entrance loop (<= 150, first hit), and in 1500 / 1501 the exit loop then `ExitSpa` while `m_bExitSpa`. The Pporu tick is S4b's. NPC loop:
  a spa sign 40141 / 40142 is in reach while `m_bJoinSpa` (@323b7346); Atlas's comment is kept in `#else` with a one-line replacement.
- **Enter.** `TalkNpc()` (`323b2e80` @323b2ee9): by a door, `OpenSpaEnterMsgBox` (SetLastPos / line, name 43064 Ruben / 43065 Ranox, box 43070 at
  (250, 300), OK = `USCM_ENTER`), then March's flow continues (June also runs its NPC loop after the box). `TalkNpc( int )` cases 40141 / 40142 open the
  same box and return true. `USCM_ENTER` (75000, enum `UI_SPA_CUSTOM_MSG` in `X2TFieldGame.h`): deletes the box; level < 10 -> OK box 43071; else
  Ruben -> REQ( `VMI_SPA_RUBEN` ), Ranox -> REQ( `VMI_SPA_RANOX` ) (warp branch, the spa's `IsWarp` position).
- **Exit.** `ExitSpa` (`323a8a60`): the key is action 0 = `GA_ATTACK_FAST` (Z, the village talk key), not "up" as S0 wrote; once per move
  (`m_bNowMovingToBattleField`, +0x22f; `friend class CX2TFieldGame` in `X2StateField.h`), closes the user-list popup, the user menu and all UI, shortcut
  keys off, `CX2State::Handler_EGS_STATE_CHANGE_FIELD_REQ( 0 )`.
- **`Handler_EGS_STATE_CHANGE_FIELD_REQ`** (`31f12630`) June's whole body (new `replace` row, done): false while the ACK is awaited (`31d70e00`); by
  state: `XS_VILLAGE_MAP` in 1500 -> Ruben (2569, 60, 150) line 5, 1501 -> Ranox (-4100, 790, 266) line 31 (`.rdata` 3346bda8-3346bdb8 / 33458fa0),
  any other village -> 0 -> Ruben's login position; `XS_WEDDING_GAME` (June 0x16) -> current village + last pos; default -> the before-dungeon village,
  else the current village's **login position** (`GetLoginPos`, `324f2dc0`; March used the last position); templet miss -> Ruben's login position.
  Stand-ins (labelled): June's state 15 `XS_GUILD_AGIT_GAME` left out (no March state); the tutorial branch tests the unit-select game-start flag
  `g_pData +0x5b8` (set in `32578bc0` case 10) instead of `m_EXP <= 0`: not ported, March's test kept. Both routed to Stage 38. Every other caller in a
  village passes a map id; only the spa exit sends 0.
- **Divergence rows** (`june_march_divergent.tsv`): `Handler_EGS_STATE_CHANGE_FIELD_REQ` replace (done); `TalkNpc`, `CX2TFieldGame::UICustomEventProc`,
  `SetShowStateDLG` as `stage38` (June rewrote them; only the spa pieces are ported here, kickoff scope). Backlog Stage 38 bullet added. funcmap +8 rows.
- **Debug** (`_DEBUG`): `door lists village= enter= exit=` (each TFG), `door near= pos=` (on change, at most 1/s), `enter village= return=`, `exit to= from=`.
- **Gates.** `finish.py --with SERV_IRUHADEV_JUNE_SPA_ENTRY SERV_IRUHADEV_JUNE_SPA_DEBUG`: flagcheck `JUNE_SPA_ENTRY` PASS (8 PASS, 1 DEFINE, 0 OTHER;
  `ELYSION_VILLAGE` INFO: the wrapped comment and the sign test sit in its block), junediverge `RESULT: PASS`, enumscan exit 1 on **pre-existing** lines
  only (X2CashShop.cpp class tables 13936-14379, X2State.cpp:9434 `SI_ETC_WS_COMMON_LOVE`; none in S2b's hunks). 447 inserted lines, 0 deleted (bytepatch).
- **Build.** `DEPLOY OK: X2_offline.exe is 27,246,592 B, dated 2026-11-17 07:05:07, md5 246d12fc982940ef538d068347235891` (main 6e10cb8c + 9 code files).
  `EXESTR OK: 3 of 3 found` (`door near=`, `exit to=`, `enter village=`, wide).
- **Range gate.** `finish.py --range c81726b0^..c81726b0`: flagcheck PASS (8 PASS, 1 DEFINE), junediverge PASS, enumscan the same pre-existing rows.
- **Play-test:** seam: S1 + S2a + S2b assembled (`playtest.py check` PASS); **passed 2026-11-17** (user). JUNE_BUGS #787: the Ruben door also opens the mailbox (June's layout and loop). Open questions: none.

## Phase S2b - Review

2026-11-17, Opus 5.5 / high. Covers S1 (`1d593cb8`, `a82d1f72`), S2a (`d33805a3`) and S2b (`c81726b0`). **Verdict: pass, no code change.**
- **S1 (data).** Merged rows equal June's (brace-matched, comments / whitespace ignored): `Village[1500]` / `[1501]`, Ruben door 5, Ranox door,
  `HI_SPA_SIGN_RUBEN` last in Ruben's house list, houses 40141-40144 / 40147, the four NPCTemplet rows, both `AddMapData` + Cobo zones 1 / 15, `WI_SPA_*`, the
  Enum values (no collisions), `SpaData`. Ruben's other two differences (`HI_POSTBOX` for June's `HI_ADAMS_HOUSE`, start pos 4's March key names) predate S1.
  Adopted files: `adopt-june --dry-run` 13 of 13 identical; the 9 in the run record match its `shipped_sha1`. `core --check` / `ledger --check` PASS, `luabind`
  on the 11 luas `RESULT: PASS` (ENUM 0 missing); `kom.py status` 131 in sync. data034 is now md5 144c4621 (a later repack; S1's note says 946358ce).
- **S2a.** Migration v25 follows the ladder's own rung idiom (`BEGIN` / `ROLLBACK` / `COMMIT` inside `Migrate()`, as v16-v24), `INSERT OR IGNORE` seed, forward
  only. Login send: map of enabled ids + C9h-2d's two empty sets, after both unit-list ACKs; `elsdb.py code-event` writes through `Writer` (backup first).
  `SpaData` read after `BattleFieldRestData`, reset per local map, templet arg, `VillageParsing` skipped for -1 and 0 (as @32502f9e). Door keys
  `OtherPlaceID` / `OtherPlaceStartLineIndex` default to March's names' values: correct for this file's mixed key names. Spa cases (warp field, warp
  command 19, both warp lists, level-10 gate, field image, `LoadUI`, field name 29, field map, minimap) each match their map row.
- **S2b.** Ctor lists under `IsEnableCode( CEI_SPA_ON_OFF )`, dtor clears, flags start false; distance 150.0 (`3346bda8`); `USCM_ENTER` 75000; enter box
  43070 with name 43064 / 43065 at (250, 300); level < 10 -> 43071; the sign cases 40141 / 40142; the exit and the REQ, below.
- **Ghidra spot-checks (3 of 3 pass).**
  1. `31f12630`, re-decompiled: early return on `31d70e00( 0x220 )`; tutorial test `+0x16c`, instance `+0xf8`, `g_pData +0x5b8 == 0` (the labelled stand-in);
     state 0x16 wedding = current village + last pos / line; state 0xe = 1500 -> 20000 line 5, 1501 -> 20008 line 31, any other village -> 0 -> templet
     miss -> Ruben login pos; default = before-dungeon village, else `GetLoginPos`; warp branch `324f30d0` / `324f1e10` / `32151680`; `.rdata` read from
     the exe: (2569.0, 60.0, 150.0), (-4100.0, 790.0, 266.0), box (250.0, 300.0), string 0x221 = 545. Equal to the port. Not ported (stand-in's tail):
     June clears `+0x5b8` at the end.
  2. `323ab220` / `323ab4f0`: y + 270.0 (double `33491e08`), drawn 60.0 lower (`33457f50`), Up at <= 0.5 (`3343777c`), Down to 1.0, reset after; `+0x460`
     = bShow. `323ab4f0` hides and clears `+0x460` only when set. Equal.
  3. `323a8a60`: state `+0x22f` latch, `31c488c0( 0 )` = `GetPureActionState( GAMEACTION_ATTACK_FAST )` (enum value 0), closes `+0x32c` / `+0x2e0`, `32151680`,
     state `+0xc4` = 0, `31f12630( 0 )`. Equal.
- **S2b's calls, confirmed:** exit key action 0 (`GA_ATTACK_FAST`); `HideJoinSpa` = entrance only; the two REQ stand-ins are labelled `stand-in:` in code and
  routed (backlog Stage 38 bullets "Guild agit" and "June's village talk / UI bodies"); `june_march_divergent.tsv` rows 549 (`replace`, done) and 550-552
  (`stage38`) present. June's "other village -> Ruben" branch has no offline reach besides the spa exit: the only other zero-argument callers in a
  village are the two `EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT` handlers (server pushes, never sent offline).
- **Fixed (bookkeeping):** `funcmap.tsv` rows `323ab220`, `323ab390`, `323ab4f0`, `323a8a60` still carried S0's reading ("none in March", "hide both
  prompts, clears +0x461", "up key"); renamed to the ported functions and corrected. No code changed.
- **Gates.** `finish.py --review --commits d33805a3 c81726b0 --with OFFLINE_CODE_EVENTS JUNE_SPA_ENTRY JUNE_SPA_DEBUG -v`: flagcheck `JUNE_SPA_ENTRY`
  PASS (17 PASS, 2 DEFINE, 4 OTHER, 0 deleted), `OFFLINE_CODE_EVENTS` PASS (1 PASS, 3 PASS~, 2 DEFINE, 17 OTHER, 0 deleted), junediverge PASS,
  `--check-all` PASS; enumscan exit 1 (119 rows in 19 files): `git blame` on every row, **none** from `d33805a3` / `c81726b0` (pre-existing, e.g.
  `X2CashShop.cpp` class tables, `X2State.cpp:9434`), so `FINISH RESULT: FAIL` is not this plan's.
  OTHER rows: `JUNE_SPA_ENTRY`'s four are `OFFLINE_CODE_EVENTS`' files (`ScriptID_Code.h`, `Handlers_Unit.cpp`, `X2OfflineDB.*`); `OFFLINE_CODE_EVENTS`'
  17 are `JUNE_SPA_ENTRY`'s files. PASS~ = the one blank line after each block. `JUNE_SPA_DEBUG` lines sit inside the other two flags' blocks.
- **Build.** None: no code change (full build skipped: no play-test; S2b's `DEPLOY OK` 246d12fc stands, play-test passed 2026-11-17).
- **Debug:** `SERV_IRUHADEV_JUNE_SPA_DEBUG` stays on (Rule 7: Z retires it). Next: S3a (`kickoffs/june-spa/S3a.md` checked, current). Open questions: none.

## Phase S3a - Spa packets and the emulator's buff answer

**Scope.** Flags `SERV_IRUHADEV_JUNE_SPA_BUFF`, `SERV_IRUHADEV_OFFLINE_SPA_BUFF` (stand-in); the map rows marked S3a:
- events 0x637 / 0x638 / 0x639 as S0's Done note writes them, appended to March's id list (`EGS_SPA_SOAK_REQ`, `EGS_SPA_SOAK_ACK`,
  `EGS_SPA_BUFF_END_TIME_NOT`), structs and `Put` / `Get` in `KncWX2Server/Common` behind the flag (`CommonPacket.h`: second home in
  `ServerDefine.h` + `SECOND_HOMES`);
- `BTI_SPA_GROW_BUFF` 918 / `BTI_SPA_STRONG_BUFF` 919 / `BFI_SPA_GROW_BUFF` 948 registered where C++ lacks them; the buff templet and factor rows
  checked live in data036;
- the emulator's 0x637 handler as "Decisions" 1 says, through the buff path (`CX2OfflineBuff`), one logged `stand-in:` line per grant.
Inert on the client (nothing sends 0x637 until S3b).

**Run with.** Opus 5.5 / high. Review: in S3b's. **Hand-off:** none of its own (S3b's play-test runs it).

**Read list.** "Rules", "Design", "Decisions", "Phase S3a - ...", S0's Done note, the map rows marked S3a.

**Done gate.** `finish.py` PASS; probe build 0 errors (full build skipped: no play-test).

## Phase S3a - Done

2026-11-17, Opus 5.5 / high. Flags `SERV_IRUHADEV_JUNE_SPA_BUFF` (client + shared packets; second home `ServerDefine.h`, `SECOND_HOMES` row in
`tools/flagcheck.py`) and `SERV_IRUHADEV_OFFLINE_SPA_BUFF` (emulator stand-in), both on, each with an `#error` guard (`cl` probe: all deps on).
- **Events.** `EGS_SPA_SOAK_REQ` / `EGS_SPA_SOAK_ACK` / `EGS_SPA_BUFF_END_TIME_NOT` appended after `ADMIN_CHEATS` in `EventID_Client.h` (June 0x637 /
  0x638 / 0x639; the wire ids are March ordinals, as every earlier appended event). Structs and `Put` / `Get` in `ClientPacket.h` / `.cpp`, field order
  as S0 read them: REQ `{ bool m_bInPool; char m_cSpaBuffType }`, ACK `{ int m_iOK; bool m_bSoaking; __int64 m_tBuffEndTime }`, NOT
  `{ __int64 m_tBuffEndTime }`. Not in `CommonPacket.h`, so no X2TOOL define.
- **Enums.** `BTI_SPA_GROW_BUFF` 918, `BTI_SPA_STRONG_BUFF` 919, `BFI_SPA_GROW_BUFF` 948 under `JUNE_ENUM_NAMES` in `X2BuffFactor.h` (no March member at those
  values; `BFI_SPA_STRONG_BUFF` 949 was already there, `JUNE_BUFF_PREMIUM`). June's `SPA_BUFF_TYPE` (`SBT_GROW` 0, `SBT_STRONG` 1, `Enum.src.lua:17985`)
  was in no C++ enum: `CX2LocationManager::SPA_BUFF_TYPE` beside `SpaInfo` (whose `m_iType` holds it), under `JUNE_SPA_BUFF`.
- **Data, live in data036** (`kom_contents/data036_kom`): `BuffTemplet.lua` carries both rows (918: 1200 s, PvP / dungeon / field, factor 948; 919:
  1200 s, dungeon / field, factor 949, `NOT_USE_IN_PVE`), `BattleFieldBuffTemplet.lua` both BTIs, `BattleFieldBuffFactor.lua` both factors (948 -> 918,
  949 -> 919), so `ActivateBuff`'s factor check passes. No pack needed.
- **Emulator** (`Handlers_Field.cpp` `Handler_EGS_SPA_SOAK_REQ`, dispatch in `X2OfflineServer.cpp`): `{ 0, * }` -> `[JUNE_SPA] emu soak REQ out of the pool`,
  no reply; a type that is not `SBT_*` -> logged, ignored; `{ 1, SBT_GROW / SBT_STRONG }` -> `CX2OfflineBuff::OnSoakSpa` (new, `Handlers_Inventory.cpp`):
  `DeactivateBuff` then `ActivateBuff( now )` (March's `ActivateBuff` refuses an active buff, so the refresh ends it first), end time read back from
  `unit_buff`; then `[JUNE_SPA] emu grant buff=<BTI> sec=<n> stand-in: ...`, `EGS_SPA_SOAK_ACK { NET_OK, true, end }`, `SendUpdateBuffInfo`. A failed
  grant logs the `BUFF` reason plus `emu buff=<id> not granted` and sends nothing. `EGS_SPA_BUFF_END_TIME_NOT` is never sent ("Decisions" 1). The
  per-packet savepoint makes the end-and-reactivate one transaction.
- **Map:** the 6 S3a rows -> `ported S3a` (`323c15e0` stays `March`: `KSession::SendPacket`, used by S3b).
- **Gates.** `finish.py --with SERV_IRUHADEV_JUNE_SPA_BUFF SERV_IRUHADEV_OFFLINE_SPA_BUFF` (worktree): flagcheck `JUNE_SPA_BUFF` PASS (2 PASS, 2 PASS~,
  2 DEFINE, 6 OTHER, 0 FAIL), `OFFLINE_SPA_BUFF` PASS (3 PASS, 2 PASS~, 1 DEFINE, 6 OTHER), junediverge PASS, enumscan PASS; `FINISH RESULT: PASS`.
  OTHER rows: each flag's rows are the other flag's files, plus `X2BuffFactor.h` (`JUNE_ENUM_NAMES`, insert-only: 11 lines, 0 deleted). Adding
  `JUNE_ENUM_NAMES` to `--with` errors on peers' `!defined( SERV_IRUHADEV_JUNE_ENUM_NAMES )` guards in `Always.h`, so it stays INFO. `PASS~`: the blank
  line before each new block. CP949 files through `bytepatch.py`: 125 inserted lines, 0 deleted. Commit `35844836`: `finish.py --range
  35844836^..35844836` `FINISH RESULT: PASS` (same tallies).
- **Build.** Full build skipped: no play-test. `/Zs` probe (X2Lib context) of `X2ServerPacket.cpp` (includes `ClientPacket.cpp`), `Handlers_Field.cpp`,
  `Handlers_Inventory.cpp`, `X2OfflineServer.cpp`, `X2OfflineEventName.cpp` and a flag probe: 0 errors, all four flags ON (`JUNE_SPA_BUFF`,
  `OFFLINE_SPA_BUFF`, `JUNE_ENUM_NAMES`, `OFFLINE_ITEM_BUFF`).
- **Play-test:** none of its own; S3b's sends the REQ (expected line `[JUNE_SPA] emu grant buff=919 sec=1200 stand-in`).
- **Open questions:** none. Review: in S3b's.

## Phase S3b - Soaking, the spa notice and effect

**Scope.** Flags `SERV_IRUHADEV_JUNE_SPA_BUFF` (+ `_DEBUG`); the map rows marked S3b, June's code whole: the ctor's spa state init and the
notice font (`+0x480`), `323ac9d0`, the soak timers and sends in OnFrameMove, 0x638 / 0x639 in `EventProcess`, the spa block of `323aea40`
(notice, `323ac3e0`, `EffectSet_SPA_Buff` play and stop), the dtor's leave send. The dead twins (`323ac910`, `323ac990`, `323ad4c0`) stay out
unless a caller turns up.

**Run with.** Opus 5.5 / high. Own review (`S3b-review.md`, covers S3a). **Hand-off:** now.

**Read list.** "Rules", "Starting context", "Design", "Decisions", "Phase S3b - ...", S0's, S2b's and S3a's Done notes, the map rows marked S3b.

**Done gate.** Play-test in each spa: stand in a pool line -> after 1 s the notice (STR 43476 for `SBT_STRONG`) with M:SS left and the effect
show, the buff icon appears with 20:00; step out and back in -> the buff restarts at 20:00 ("Decisions" 1); leaving the line stops the effect;
a dungeon run shows the buff applied (and June's `NOT_USE_IN_PVE` rule). Log lines `type`, `send 0x637`, `recv 0x638`, `effect`, `emu grant`.

## Phase S3b - Done

2026-11-17, Opus 5.5 / high. Flags `SERV_IRUHADEV_JUNE_SPA_BUFF` (S3a's), `SERV_IRUHADEV_JUNE_SPA_DEBUG`. One file pair: `X2TFieldGame.cpp` / `.h`, 236 lines
inserted, 0 deleted (`bytepatch.py`). Map: the 9 live S3b rows -> `ported S3b`; the dead twins `323ac910` / `323ac990` / `323ad4c0` stay out (no caller).
- **State** (TFG +0x462..+0x484): `m_bShowSpaNotice`, `m_bOutOfSpaPool`, `m_bSpaBuffTypeChanged`, `m_fSpaSoakTime`, `m_fSpaLeaveSendTime`, `m_fSpaLeaveTime`,
  `m_iSpaBuffType` (-1), `m_tSpaBuffEndTime` (-1), `m_pFontForSpaNotice`, `m_hSpaBuffEffect` (invalid), set in the ctor as June (@323baf69, @323bc7c3).
  **Font:** June keeps its own member (+0x480 = `GetUKFont( 2 )` @323bb335, beside +0x120 = `m_pFontForUnitName`, also font 2), so S3b adds one:
  `m_pFontForSpaNotice = GetUKFont( XUF_DODUM_15_BOLD )`; the dtor clears it beside `m_pFontForUnitName` (@323b8c0b).
- **`UpdateSpaBuffType`** (`323ac9d0`): the current village templet's `m_vecSpaInfo` row whose `LineIdx` holds `GetLastTouchLineIndex()` (+0x3d4, as S2b);
  the last match wins; changed -> type-changed flag; none -> out-of-pool flag.
- **Soak loop** (OnFrameMove @323b6aa9-323b6cd2), inside S2b's spa-village block after `ExitSpa` (June's village test jumps past it, @323b6956):
  in a pool, the timer is held at 0 while the end time is > 0 and neither flag is set; >= 1.0 s (double @334355b0) -> `EGS_SPA_SOAK_REQ { 1, type }`,
  both flags and the timer cleared; leave timer 0. Out of a pool: both timers += dt; send timer >= 1.0 and leave time <= 3.0 (double @33439b50, `fcomp`
  + `test ah, 0x41` + `jp`) -> `{ 0, -1 }`, notice off. Floats as June (`fstp dword` each add). Send = `CX2ServerProtocol::SendPacket` (June's inlined
  template: logging on, no compress; its extra `+0x2c` byte test is June's protocol class, not this plan's).
- **Packets:** March's `CX2TFieldGame::UIServerEventProc` is June's `323bda90` (the funcmap's "EventProcess", corrected: field-NPC loop, then the
  switch; `CX2StateField` 3250d2f0 forwards to it). 0x638 (@323bdcc9): invalid `m_iOK` -> false; else if `m_bSoaking` -> end time, notice on; true.
  0x639 (@323bdc9b): end time only; true. Written inline in the cases, as June compiled them (the out-of-line twins are dead).
- **Render** (`323aea40` @323aebea-323aef25) at the end of March's `OnFrameRender` not-loading branch, after the talk box (June's order): in 1500 / 1501
  with my unit, notice on and both flags clear -> STR 43074 (`SBT_GROW`) / 43476 (`SBT_STRONG`) with `AppendSpaBuffLeftTime`; drawn unless dialogs are
  hidden: `OutTextMultiline( 512, 150, ..., 0xffffff80, FS_SHELL, 0xff000000, 1.0, NULL, DT_CENTER )` (31c3ae80, args read off the pushes);
  `EffectSet_SPA_Buff` on my unit into the handle while it is invalid. Otherwise the handle is stopped. June's pass bool has no March counterpart here
  (March's `OnFrameRender` takes none): drawn once per frame.
- **`AppendSpaBuffLeftTime`** (`323ac3e0`): `_localtime64_s( end )`, `_mktime32`, `_difftime32( end, (time32)GetServerCurrentTime() )`, then
  `M : 0S` / `M : SS` (separator `L" : "` @33459168, pad `L"0"` @33458f54). JUNE_BUGS 788 (no clamp below zero, reached past 20 min since the emulator
  sends no 0x639) and 789 (an unset `tm` when the conversion fails; latent).
- **Dtor** (@323b936d-323b9431), after the relation-templet cleanup (June's order): stop the effect; in 1500 / 1501 send `{ 0, -1 }`. One addition: a
  `GetServerProtocol()` null test (March's shutdown order is not June's), commented.
- **Strings** (live `General.ess`): 43074 "@1 left before getting the 'Ventus's Blessing' buff ...", 43476 "@1 left before getting the 'Denif's
  Blessing' buff ...". Both spas are `SBT_STRONG`, so 43476 shows. The wording reads as a wait **until** the buff, while "Decisions" 1 grants it at
  once; noted for the user, not changed (the stand-in is approved).
- **Debug** (`_DEBUG`): `type <old> -> <new> line=` (on change, 1/s), `send 0x637 flag= type=` (+ ` (dtor)`), `recv 0x638 ok= soaking= end=`, `recv 0x639
  end=`, `effect play/stop handle=`.
- **Gates.** `finish.py --files X2Lib/X2TFieldGame.cpp X2Lib/X2TFieldGame.h --with SERV_IRUHADEV_JUNE_SPA_BUFF SERV_IRUHADEV_JUNE_SPA_DEBUG`: flagcheck
  `JUNE_SPA_BUFF` PASS (2 PASS, 0 OTHER, 0 FAIL), junediverge `RESULT: PASS`, enumscan PASS; `FINISH RESULT: PASS`. `JUNE_SPA_ENTRY` INFO (S2b's block
  around the soak loop). `OFFLINE_SPA_BUFF` (the kickoff's `--with`) is on no S3b line: dropped. `/Zs` probe of `X2TFieldGame.cpp`: 0 errors.
- **Build.** `DEPLOY OK: X2_offline.exe is 27,255,296 B, dated 2026-11-17 08:36:36, md5 0d5f427e0a72209ef08e880c8f5d3b74` (main d54bb444 + the 2 S3b files).
  `EXESTR OK: 4 of 4 found` (`send 0x637`, `recv 0x638`, `effect play`, `EffectSet_SPA_Buff`; wide).
- **Range gate.** `finish.py --range ad7b358e^..ad7b358e`: flagcheck `JUNE_SPA_BUFF` PASS (2 PASS, 0 OTHER), junediverge PASS, enumscan PASS.
- **funcmap** +1 row (`323bda90 (S3b)`). Play-test: hand-off now (below). Review: `S3b-review.md` (covers S3a).
- **Play-test passed 2026-11-17** (user, both spas, dungeon). Log: `type -1 -> 1 line=8`, `send 0x637 flag=1 type=1`, `emu grant buff=919 sec=1200`,
  `recv 0x638 ok=0 soaking=1`, `effect play` / `effect stop`, up to three `flag=0 type=-1` sends after leaving, the `(dtor)` send. The user then asked
  for a 1-minute soak timer: S3c.

## Phase S3c - The user's soak timer (stand-in)

**Scope.** Emulator only, flag `SERV_IRUHADEV_OFFLINE_SPA_SOAK_TIMER` ("Decisions" 1 as amended 2026-11-17). Asked for by the user after S3b's play-test,
so it has no kickoff of its own; S3b's review covers it. **Run with.** Opus 5.5 / high (same session as S3b).

## Phase S3c - Done

2026-11-17, Opus 5.5 / high. `X2Lib/Offline/Handlers_Field.cpp`, `X2OfflineServer.h`, `KTDXLIB/Always.h`; 132 lines inserted, 0 deleted (`bytepatch.py`).
- **Design (user):** 60 s in a pool earns the buff; stepping out pauses the countdown and keeps the seconds soaked; at 0 the buff is (re)granted for
  its `BuffTemplet` period (`BTI_SPA_STRONG_BUFF` 1200 s) and the next 60 s starts, so staying in tops the buff up every minute. June's server value is
  lost: `SPA_SOAK_SECONDS = 60` is labelled `stand-in` (`_CONST_OFFLINE_SERVER_S3C_`).
- **REQ** (`Handler_EGS_SPA_SOAK_REQ` -> `OnSpaSoakReq`; S3a's grant on entry in `#else`): a running countdown is banked first (`m_iSpaSoakLeftSec` =
  seconds to the end, 0 floor); `{ 0, -1 }` -> `emu soak pause left=` (only when it was running; no reply, as S3a); `{ 1, SBT_* }` -> end = now + left,
  `emu soak start buff= left=`, `EGS_SPA_SOAK_ACK { NET_OK, true, end }`. A new selected unit restarts at 60 s. Progress lives in the session
  (`KOfflineSession` +4 members), not the save: a restart of the game starts at 60 s again.
- **Tick** (`TickSpaSoak`, on `EGS_UPDATE_PLAY_STATUS_NOT`'s 3 s beat, before the max-HP early-out): end reached -> `OnSoakSpa` (S3a's end-and-reactivate),
  `emu grant buff= sec=`, `SendUpdateBuffInfo`; then end = now + 60 and `EGS_SPA_BUFF_END_TIME_NOT` (0x639; its first use), `emu soak next countdown`.
  The client's 0x639 case (S3b) replaces the end time only, so the notice restarts at `1 : 00`. The grant lands up to 3 s after the notice reads `0 : 00`
  (it may show `0 : 0-1` .. `0 : 0-3` meanwhile, JUNE_BUGS 788's no-clamp display).
- **Gates.** `finish.py --files KTDXLIB/Always.h X2Lib/Offline/Handlers_Field.cpp X2Lib/Offline/X2OfflineServer.h --with SERV_IRUHADEV_OFFLINE_SPA_SOAK_TIMER`:
  flagcheck PASS (1 PASS, 1 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL), junediverge PASS, enumscan PASS. `/Zs` probe of `Handlers_Field.cpp`: 0 errors.
- **Build.** `DEPLOY OK: X2_offline.exe is 27,256,832 B, dated 2026-11-17 09:00:56, md5 22a18bfd44f151361ac10325b596ca21` (main 2ef4c11e + the 3 S3c files).
  `EXESTR OK: 3 of 3 found` (`emu soak start`, `emu soak pause`, `emu soak next countdown`; wide). Play-test: hand-off now.
- **Range gate.** `finish.py --range d91db863^..d91db863`: flagcheck PASS (1 PASS, 1 PASS~, 1 DEFINE, 0 OTHER), junediverge PASS, enumscan PASS.
- **Play-test passed 2026-11-17** (user). Log: `emu soak start buff=919 left=60`, `pause left=47`, `start left=47` (resumed), `emu grant buff=919
  sec=1200 stand-in: earned by 60 s in the pool` 47 s after re-entry, `next countdown` + `recv 0x639`, `pause left=56`; effect play / stop on each
  entry / exit; no spa `UNHANDLED` / `EXCEPTION`.

## Phase S3b - Review

2026-11-17, Opus 5.5 / high. Covers S3a `35844836`, S3b `ad7b358e`, S3c `d91db863`. Verdict: **pass after one transcription fix** (below).
- **Ghidra spot checks** (June listings, not the map): (1) OnFrameMove soak `323b6aa9-323b6cd2`: hold test (64-bit end > 0, `+0x464 != 1`,
  `+0x463 != 1`), `>= 1.0` (`test ah, 1; jnz` skip), leave `<= 3.0` (`test ah, 0x41; jp` skip), flags / timers cleared after the send even when the
  session is down, `+0x470 = 0` on both in-pool paths, REQ bytes `{ 1, type }` / `0xff00`: match. (2) `UIServerEventProc` `323bdc82-323bdd22`
  (0x5a2 + 0x96 = 0x638, next = 0x639): IsValidPacket fail -> false; bool -> end + notice on; 0x639 end only: match. (3) Render `323aebea-323aef25`:
  type 0 -> 0xa842 (43074), 1 -> 0xa9d4 (43476); `GetHideDialog` skip; `OutTextMultiline` args (512, 150, text, 0xffffff80, `FS_SHELL` 2,
  0xff000000, 1.0, NULL, `DT_CENTER`, then June's -1 / 1.0 / 1.0 / false = March's defaults); effect on `m_pMyUnit` while -1, stop in the else
  inside the village block: match. **(4) `AppendSpaBuffLeftTime` `323ac3e0`: fail.** After `_difftime32` June runs `fldz; fcom; test ah, 0x41;
  jnz` / `fstp st1` (@323ac458-323ac46c): a negative time left becomes 0 before `_ftol2`. S3b ported it with no clamp and filed JUNE_BUGS 788.
- **Whole-phase re-check** after the fail: `UpdateSpaBuffType` `323ac9d0` (byte line index, last match wins, flag order), dtor `323b936d-323b9431`
  (stop, village test, `{ 0, -1 }`), ctor init: match. Put / Get order (`320acab0` bool, char; `320acb70` int, bool, int64; `320acbe0` int64)
  as the map; ACK zero-init before the read as June.
- **Fix** (`SERV_IRUHADEV_JUNE_SPA_BUFF`, `X2TFieldGame.cpp`, inside S3b's own new function, +5 / -2 via `bytepatch.py`): `dLeftSec < 0.0` -> 0
  (NaN kept, as June's unordered `jnz`); the function's comment corrected. JUNE_BUGS 788 withdrawn (row kept, marked); `spa_map.tsv` / `funcmap.tsv`
  323ac3e0 rows and "Decisions" 1 note the clamp. The S3b / S3c Done notes' "below zero" lines are history: the notice now reads `0 : 00` until the
  grant (S3c) or a new entry.
- **Emulator** (S3a / S3c): matches "Decisions" 1 as amended: 60 s soak, `{ 0, -1 }` pauses and banks the seconds, grant + 0x639 per minute on the
  3 s beat, `stand-in:` on every grant / start / pause line; S3a's grant on entry intact in `#else SERV_IRUHADEV_OFFLINE_SPA_SOAK_TIMER`.
- **`#else` paths:** every S3a / S3b / S3c client block is insert-only (no March line replaced); flag off = March.
- **Gates.** `finish.py --review --commits 35844836 ad7b358e d91db863 --with JUNE_SPA_BUFF OFFLINE_SPA_BUFF OFFLINE_SPA_SOAK_TIMER JUNE_SPA_DEBUG -v`:
  `JUNE_SPA_BUFF` PASS (4 PASS, 2 PASS~, 3 DEFINE, 8 OTHER), `OFFLINE_SPA_BUFF` PASS (5 PASS, 2 PASS~, 2 DEFINE, 6 OTHER), `OFFLINE_SPA_SOAK_TIMER`
  PASS (2 PASS, 1 DEFINE), 0 deleted lines, junediverge + `--check-all` PASS, enumscan PASS; `FINISH RESULT: PASS`. OTHER rows: each flag's rows
  are the sibling flags' files (packets vs emulator vs timer) and `X2BuffFactor.h` (`JUNE_ENUM_NAMES`, insert-only). INFO: `JUNE_ENUM_NAMES`,
  `JUNE_SPA_ENTRY`, `OFFLINE`, `OFFLINE_ITEM_BUFF` (enclosing guards, not this plan's change). Fix: `finish.py --files X2Lib/X2TFieldGame.cpp
  --with SERV_IRUHADEV_JUNE_SPA_BUFF`: PASS (1 PASS; 2 deleted lines, both S3b's own lines inside the flag), junediverge PASS, enumscan PASS.
- **Debug:** `SERV_IRUHADEV_JUNE_SPA_DEBUG` stays on (S4b / S5 log through it; Z retires it).
- **Build.** `DEPLOY OK: X2_offline.exe is 27,262,976 B, dated 2026-11-17 09:22:50, md5 805c9326b9d369ca1a76b2d2262b745a` (main a6d9c677 + the fixed
  `X2TFieldGame.cpp`). No new strings (exestr not needed).
- **Range gate.** `finish.py --range 860cf809^..860cf809 --with SERV_IRUHADEV_JUNE_SPA_BUFF`: flagcheck PASS (1 PASS, 2 deleted lines inside the flag),
  junediverge PASS, enumscan PASS. STATUS rows for this review were committed inside bba7cf09 (a peer's whole-file commit).
- Play-test: hand-off now (the notice at the end of a countdown).

## Phase S4a - The Pporu class

**Scope.** Flag `SERV_IRUHADEV_JUNE_SPA_PPORU`; the map rows marked S4a: the `+0x624` object (`32ec76e0` .. `32ec8350`, 14 methods) as a new
class in the field game's naming pattern (no RTTI in June: name it from what it does), its own files in `X2Lib_2010.vcxproj` and filters. The
catch request sends March's `EGS_BUY_ED_ITEM_REQ` (June's 0xe6 has its shape) and the ACK path follows June's `32ec7fa0`. Inert: nothing
creates it until S4b.

**Run with.** Opus 5.5 / high. Review: in S4b's. **Hand-off:** none of its own.

**Read list.** "Rules", "Phase S4a - ...", S0's Done note, the map rows marked S4a.

**Done gate.** `finish.py` PASS; probe build 0 errors (full build skipped: no play-test).

## Phase S4b - Pporu in the field game

**Scope.** Flags `SERV_IRUHADEV_JUNE_SPA_PPORU` (+ `_DEBUG`); the map rows marked S4b: ctor creation (1500 / 1501), the tick and the NPC-loop
steps in OnFrameMove, the key-handler and join-NPC skips, `EventProcess` forwarding, the render hook, the hidden start in add-field-NPC, the
dtor delete, and the Lua `SetAppearPPoruCheat` bind; textures, the alarm sound and the click particle checked staged (S1).

**Run with.** Opus 5.5 / high. Own review (`S4b-review.md`, covers S4a). **Hand-off:** now.

**Read list.** "Rules", "Starting context", "Decisions", "Phase S4b - ...", S0's and S4a's Done notes, the map rows marked S4b.

**Done gate.** Play-test in each spa: the cheat makes Pporu appear (alarm, "Start" anim, on-screen notice); clicking it during "Wait" buys the
Hard-boiled Egg (164548) for 20000 ED; with too little ED June's refusal shows; leaving and re-entering the spa does not crash or leak (log
lines `pporu`).

## Phase S5 - The spa NPCs' rock-scissors-paper

**Scope.** Flag `SERV_IRUHADEV_JUNE_SPA_RPS`. Spa-only (S0): houses 40143 / 40144, NPCs `NUI_SHY_WISP_EVENT` (Ruben) / `NUI_SAYRENA_EVENT`
(Ranox). First map the RPS class (UI around 328d0000-328d2d00, `CX2CustomEventManager::AddRockScissorsPaperData` `32681590`, the
`NMUM_ROCK_SCISSORS_PAPER` button, packets 0x651 / 0x652 and 0x653 / 0x654 with their structs) into `spa_map.tsv`; if it holds more than about a
dozen absent / mixed rows, split this phase before porting. Data: `CustomEventScript.lua`'s two `AddRockScissorsPaperData` rows and
`DLG_UI_RockScissorsPaper.lua` (data034), through the tools. The emulator answer is a stand-in built only from those rows (ED cost, reward
item and quantity), written to "Decisions" and approved by the user before it is coded.

**Run with.** Opus 5.5 / high. Own review (`S5-review.md`). **Hand-off:** now.

**Done gate.** Play-test: talk to the NPC in each spa, play a round, see June's win / lose / draw UI, the 10000 ED cost and the reward the
stand-in gives.

## Phase Z - Close

`SERV_IRUHADEV_JUNE_SPA_DEBUG` commented out (no build: debug retire only); `MODS.md` complete for every flag of this plan; the map has no
`absent` / `mixed` row without a routed owner; `junediverge.py --check-all` PASS; backlog Stage 24 marked closed with its routed items;
memory note updated; the STATUS block moved to `STATUS_ARCHIVE.md` with one *Closed plans* row. **Run with** Sonnet 5.5 / medium.

## Decisions

Written by S0 and answered by the user on 2026-11-16 (`AskUserQuestion`).

1. **The emulator's answer to 0x637** (no June server code). June evidence: the client sends `{ 1, type }` 1.0 s after a pool line is entered
   (and again only after the type changes or the player left a pool); `{ 0, -1 }` once a second for 3 s after leaving and from the TFG dtor;
   0x638 `{ m_iOK, bool, end time }` turns the notice and effect on, the notice counting down `end - server now` as M:SS; 0x639 only replaces the
   end time (0 or less makes the client ask again). Both spas' `SPA_INFO` rows are `SBT_STRONG` -> `BTI_SPA_STRONG_BUFF` 919 (1200 s,
   `URT_DUNGEON` / `URT_BATTLE_FIELD`, `NOT_USE_IN_PVE`); `SBT_GROW` -> `BTI_SPA_GROW_BUFF` 918 has no factor behaviour (`BBT_EMPTY`).
   Options: (a) grant when inactive, answer the current end time when active, push 0x639 `{ 0 }` at expiry while in a spa (recommended);
   (b) grant once per pool entry, no 0x639; (c) refresh on every entry.
   **Answer: (c) refresh on every entry.** On `{ 1, type }` with type `SBT_GROW` / `SBT_STRONG`: grant (or re-grant) that type's buff for its
   `BuffTemplet` period from now, reply 0x638 `{ NET_OK, true, now + period }`, sync the buff list; log `stand-in:` with the buff id and seconds.
   `{ 0, -1 }`: logged, no reply. Other types: logged, ignored. 0x639 is never sent, so a player who stands in the pool past the 20 minutes
   sees June's notice count below zero (June's client has no clamp) until they step out and back in. (S3b review: June does clamp; it reads `0 : 00`.)
   **Amended 2026-11-17 by the user after the S3b play-test (S3c, `SERV_IRUHADEV_OFFLINE_SPA_SOAK_TIMER`):** the countdown is the soak time that
   *earns* the buff (as STR 43476 reads, "@1 left before getting ..."), 60 s instead of 20 minutes; stepping out pauses it and stepping back in
   continues it; at 0 the buff is (re)granted for its `BuffTemplet` period and 0x639 starts the next 60 s. S3a's grant on entry stays in `#else`.
2. **Login with a saved position inside a spa.** June's client has no spa rule at login (it loads the saved village once the templet exists,
   which S2a's `SpaData` read gives), and June's exit needs no stored data (fixed spots by each door). Options: allow it (recommended) / never
   save a spa as last position. **Answer: allow it.** The emulator keeps saving 1500 / 1501 like any village. With code event 31 off, June
   builds no exit points, so a unit saved inside a spa can only leave by warp (June's behaviour with the event off).
3. **Live-DB ask** (`%spa%` procedures). **Answer: the user has no live-DB access anymore.** The ask is void; CLAUDE.md's live-DB rule was
   rewritten the same day (no pulls; repo transcriptions, else a user-approved stand-in). Rule 3 above follows it.
4. **Other S0 calls (no question needed).** Pporu: June's client buys the egg through the shop packet, so the existing buy handler answers it
   (no stand-in). Rock-scissors-paper: spa-only, so S5 stays; its stand-in is designed in S5 from June's `CustomEventScript` rows and asked
   then. Snow worlds: code event 46, not seeded: off. Spa event menu icon: code event 29, not the only way in: Stage 9.

## Play-test hand-off

Dev console first (END key, auth level 3). Copy-ready lines. Village warps go through the console's warp command or the map. June's client
gates spa entry on level 10 only (`MapData`'s 65 for Ranox is a server check the emulator does not run); test the refusal with a level-9 unit. Per check: where, what to press, what to look for, what a failure looks
like, and the `[JUNE_SPA]` lines to expect. Read with `python tools/logs.py --tag JUNE_SPA` plus the emulator's `FIELD` lines.
