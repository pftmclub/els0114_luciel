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
   `stand-in:` line (memory `design-missing-june-pieces`). The user approves the design before S3 writes it. Live-DB procedures come first:
   S0 asks the user for any `%spa%` procedure (CLAUDE.md live-DB rule) and transcribes what comes back verbatim.
4. **The spa's code event is switched on from the save.** June builds the door lists only when `CEI_SPA_ON_OFF` (31,
   `CodeScriptEnum_KR.lua`) is on. Offline nothing sends code events, so S2 adds a save-DB code-event table that the emulator sends at login
   (standing rule "Game events are switched on from the save DB"). The table is general, the first row is 31 = on, and `tools/elsdb.py` switches
   rows. The rest of June's code-event logic stays June's.
5. **Data through the tools.** Core-list Lua (`DLG_Map_Village`, `DLG_Map_House`, `NPCTemplet`, `WorldData`, `MapData`, `Enum`) gets June's
   spa rows merged as BOM source (`luasrc.py`). Other June files arrive by `luasrc.py adopt-june` / `kom.py stage-june`, byte-exact. You pack and
   copy without asking. No new `.lua`.
6. **Scope.** Spa avatars (`DITT_SPA_AVATAR_*`) are Stage 11's. The spa event menu icon (`EventButtonIconInfo` -> `DLG_UI_Menu_Spa_Event.lua`)
   is Stage 9's, unless S0 shows it is the only way in. Spa achievements (`ACT_SPA_GAME_WIN`, `BTI_ALWAYS_SPA_BUFF_*`) are Stage 31's. The snow
   worlds (`dataFileName_CodeEvent`) stay off unless S0 finds a reason. Whatever else S0 finds goes through the scope rule (CLAUDE.md).
7. **One debug flag:** `SERV_IRUHADEV_JUNE_SPA_DEBUG`. S2 turns it on, Z retires it. Lines go to `offline_server.log` through
   `CX2OfflineLog::Server( L"[JUNE_SPA] ..." )`, throttled to about one a second.
8. Standing rules apply unchanged: branch `main`, explicit pathspecs, CP949 sources through `bytepatch.py`, no hardcoded enum literals (spa
   ids are in June's `Enum.lua`: use the names), one-line comments with the phase tag (`// june-spa S2: June 323b2e80`), `junediverge.py`,
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
- **Code-event switch (S2).** `X2OfflineDB` migration adds `code_events( event_id INTEGER PRIMARY KEY, enabled INTEGER NOT NULL, note TEXT )`
  seeded with `( 31, 1, 'CEI_SPA_ON_OFF' )`. At login, after the unit list, the emulator sends `EGS_UPDATE_CODE_EVENT_NOT` with the enabled
  ids in June's shape (the client handler `31f00970` is already partly ported, C9h-2d). `tools/elsdb.py code-event list | on <id> | off <id>`.
  Flag `SERV_IRUHADEV_OFFLINE_CODE_EVENTS` (emulator + DB). Save backed up before the migration; migrations are forward-only.
- **Emulator spa answer (S3).** S0 writes the options. The default S0 should test against June: on 0x637 with type `SBT_STRONG`, reply 0x638
  ok and grant `BTI_SPA_STRONG_BUFF` (1200 s from `BuffTemplet`) through the item-buff path. Then 0x639 or nothing while the buff runs.
  The user picks before S3.
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
- NPC / UI luas: `UI_SPA_PPORU`, `UI_MUL_MAI`, `UI_MULSAYRENA`, `UI_NPC_SHY_WISP`, `UI_WISP`, `UI_NPC_BILLBOARD_SPA` (adopt-june; find
  `UI_SPA_PPORU`'s kom first). `DLG_Destination_byButton` / `DLG_UI_Field_Title`: June's spa entries are present or merged.
- Assets: `Motion_Spa_Bboru.x`, `Motion_Wisp_EVENT*.xet`, the three sounds, loading images `HQ_LoadingSpa_Ruben_01/02.tga` (and Ranox's if
  missing), every `.TET` beside a staged UI texture (memory `stage-tet-with-every-ui-texture`), `SPA_PPORU_*.DDS`, `Alarm_SpaPporu.ogg`.
  Gates: an asset closure over the four spa world luas and the spa NPC / UI luas prints 0 missing; `luabind.py check` passes for every staged
  NPC / UI lua (ENUM and STATE gates too); `luasrc.py core --check` and `ledger --check` pass.
- Pack and copy every touched kom.

**Run with.** Sonnet 5.5 / high (data work). Review: inside S2's review (Opus). **Hand-off:** batch at S2 (regression section: Ruben, Ranox and
one other village still load, the Ruben sign and the spa houses appear, nothing new in `ErrorLog.txt`).

**Read list.** "Rules", "Starting context", "Phase S1 - Spa data and assets", S0's Done note.

**Done gate.** The gates above PASS (paste them), `PACK OK` / `COPY OK` per kom, `MODS.md` entry, the replaced files listed in the Done note.
Full build skipped: no play-test yet (batch at S2).

## Phase S2 - Code-event switch, entering and leaving the spa

**Scope.** Flags `SERV_IRUHADEV_OFFLINE_CODE_EVENTS`, `SERV_IRUHADEV_JUNE_SPA_ENTRY`, `SERV_IRUHADEV_JUNE_SPA_DEBUG`:
- the code-event table, its migration, the login send and the `elsdb.py` command ("Design"); `IsEnableCode( 31 )` reads true offline;
- TFG ctor: the two lists (`324f2ca0`, `324f2d30`) under June's code-event test, as June has them;
- OnFrameMove: entrance prompt (`323ab220`), exit prompt (`323ab390`), reset (`323ab4f0`); the key handler `323b2e80` (return position and
  line) and the exit `323a8a60`, with every reader of the stored return fields;
- March's Ruben omissions: `X2LocationManager.cpp:1974`, `X2Main.cpp` world index, `X2FieldMapBase.cpp`, `LoadFieldImage`, the offline enum seed;
  `VMI_SPA_RUBEN` registered where C++ lacks it (`JUNE_ENUM_NAMES` pattern);
- emulator: state change into 1500 / 1501 and back, and the login-inside-spa answer from "Decisions";
- the other rows S0 assigns here.
`+0x624` stays null (S4) and the soak timer stays out (S3).

**Run with.** Opus 5.5 / high. Own review (`S2-review.md`, covers S1's data). **Hand-off:** now. This is the seam: `build.py --deploy`, then
`playtest.py assemble june-spa S1 S2` in full.

**Read list.** "Rules", "Starting context", "Design", "Decisions", "Phase S2 - ...", S0's and S1's Done notes, the map rows marked S2.

**Done gate.** Play-test: in Ruben and in Ranox, walk to the door, the prompt shows, the key enters the spa (loading image, BGM, world, NPCs),
both exits lead back to the door's position. A relog inside a spa follows the decision. `elsdb.py code-event off 31`, relog: the doors do
nothing (June's behaviour with the event off). Log lines `door`, `enter`, the emulator's `FIELD` lines. `exestr.py` for the debug tag.

## Phase S3 - Soaking, the spa notice, effect and buff

**Scope.** Flag `SERV_IRUHADEV_JUNE_SPA_BUFF` (+ `_DEBUG`):
- `323ac9d0` type pick; the soak timers and the 0x637 send in OnFrameMove; 0x638 / 0x639 in `EventProcess` (and `323ac910` / `323ac990` if
  they are separate vslots); the spa block of `323aea40` (notice, `EffectSet_SPA_Buff` play and stop, handle `+0x484`);
- the three events' structs in `KncWX2Server/Common` and their `Put` / `Get`, client side behind the flag (`CommonPacket.h`: second home in
  `ServerDefine.h` + `SECOND_HOMES`);
- the emulator handler as the user decided (stand-in flag `SERV_IRUHADEV_OFFLINE_SPA_BUFF`, logged `stand-in:` line), the buff through the
  item-buff path; the buff's templet and factor rows checked live in March's data (S1 adds them if missing);
- `BFI_SPA_GROW_BUFF` / `BTI_*` names registered where C++ lacks them.

**Run with.** Opus 5.5 / high. Own review (`S3-review.md`). **Hand-off:** now.

**Done gate.** Play-test in each spa: stand in a pool line -> after June's soak time the notice (string 43476 for `SBT_STRONG`) and the
effect show, the buff icon appears with 20:00, leaving the line stops the effect, a dungeon run shows the buff applied (and the PvE rule
June's `NOT_USE_IN_PVE` gives). Log lines `type`, `send 0x637`, `recv 0x638`, `effect`, `emu grant`.

## Phase S4 - The Pporu game

**Scope.** Flag `SERV_IRUHADEV_JUNE_SPA_PPORU` (+ `_DEBUG`): the `+0x624` object as June builds it in the ctor (1500 / 1501 only), its tick
`32ec7240`, its OnFrameMove step `32ec7620` / `32ec7380`, `32ec7280`, the render hook `32ec7820` from `323aea40`, its UI events `32ec8330` /
`32ec7fa0`, its sound and textures, the house-40147 NPC click, and any packet and emulator reply S0 found. A new class in the field game's
own naming pattern (S0 names it from June's RTTI if present). The dtor `323b8ba0` frees it.

**Run with.** Opus 5.5 / high. Own review (`S4-review.md`). **Hand-off:** now.

**Done gate.** Play-test: in each spa, the Pporu NPC is there, the click game runs to success (textures and the alarm sound), leaving and
re-entering the spa does not crash or leak (log lines `pporu`).

## Phase S5 - The wisp NPC's rock-scissors-paper (only if S0 keeps it here)

**Scope.** Flag `SERV_IRUHADEV_JUNE_SPA_RPS`: the inner wisp NPC's `ROCKSCISSORSPAPER_NPC` path (`DLG_UI_RockScissorsPaper.lua`), its
packets and the emulator's answer (a stand-in designed in S0, user-approved), June's rewards as data says. If S0 routes rock-scissors-paper to
its own plan, this phase is deleted from STATUS and the NPC keeps June's talk menu only.

**Run with.** Opus 5.5 / high. Own review (`S5-review.md`). **Hand-off:** now.

**Done gate.** Play-test: talk to the wisp in each spa, play a round, see June's win / lose / draw UI and the reward the stand-in gives.

## Phase Z - Close

`SERV_IRUHADEV_JUNE_SPA_DEBUG` commented out (no build: debug retire only); `MODS.md` complete for every flag of this plan; the map has no
`absent` / `mixed` row without a routed owner; `junediverge.py --check-all` PASS; backlog Stage 24 marked closed with its routed items;
memory note updated; the STATUS block moved to `STATUS_ARCHIVE.md` with one *Closed plans* row. **Run with** Sonnet 5.5 / medium.

## Decisions

(S0 writes the options here; the user answers before S2 / S3 start.)

## Play-test hand-off

Dev console first (END key, auth level 3). Copy-ready lines. Village warps go through the console's warp command or the map; a unit for
Ranox must meet `MapData`'s level 65 (`/x2-create-char` or `elsdb.py`). Per check: where, what to press, what to look for, what a failure looks
like, and the `[JUNE_SPA]` lines to expect. Read with `python tools/logs.py --tag JUNE_SPA` plus the emulator's `FIELD` lines.
