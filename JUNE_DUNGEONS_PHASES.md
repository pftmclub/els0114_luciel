# June dungeons - phases

Created 2026-10-11 (user): the dungeon / world / map-entry ports that `june-effect` (C3y, C3z) collected are not effect-engine work,
so they run as their own plan. Engine fixes the dungeons need (NPC/game queries, intro skip, SimpleDLG) stay in
`JUNE2017_EFFECT_COMPLETION_PHASES.md`.

## Context

- Goal: every June dungeon and world that March lacks or has half of is playable offline, with June's rows, worlds, NPCs, meshes,
  map buttons and entry rules. Offline client only (shared-header changes flag-gated, server desync not a concern).
- **Phase IDs were kept** (C3y-1b ... C3z-5b, C3z-4t) so commit messages, JUNE_BUGS rows, census files and `funcmap.tsv` tags still match.
  New phases of this plan (for example the ones C3y-5 writes) take the next free ID in the same style, for example `C3y-5a`.
- Done phases that stay in june-effect as history: C3x (world load, 170 map files), C3y (census), C3y-1a (Hamel secret),
  C3z-1 (map assets), C3z-2 (old dungeon NPCs), C3z-3 (registry parity), C3z-5 (DungeonData keys), C3z-6 (relative level, poison).
- Commit message prefix: `June dungeons Phase <id>: <what>`. Log tag stays `JUNE_FX <id>` (existing code prints it).
- Where the text lives: every moved phase's task text and every earlier Done note is still in `JUNE2017_EFFECT_COMPLETION_PHASES.md`
  (section names below). **New Done notes go into this file**, under `## <id> - Done`, so the old doc stops growing.

## Rules

- All of CLAUDE.md and the standing memory rules: one `SERV_IRUHADEV_JUNE_*` flag per logical change in `KTDXLIB/Always.h`, original
  code in `#else`; June bugs kept verbatim with a `JUNE_BUGS.md` row; no new `.lua` files; June `.lua` arrives through
  `luasrc.py adopt-june`, core-list rows added by the phase that edits them; a missing June asset or key is a finding, never a hand patch.
- **Opus phases delegate the mundane work to subagents** (x2-phase skill, "When the phase is Opus").
- The port halves of C3y-1b / 1c / 2 / 3 / 4 share the core registries: one at a time.
- W3 packs `data040`, which those port halves also pack (`WorldData.lua`): one at a time with them.

## Starting context

- Tools, not hand commands: `python tools/build.py --deploy`, `flagcheck.py`, `logs.py --grep "JUNE_FX <id>"`, `kom.py`, `luasrc.py`.
- Censuses and gates: `june2017_re/c3y_unreached_dungeon_census.tsv` (+ `_world_` and `c3y_june_only_dungeon_ids.tsv`),
  `dungeon_npc_parity.tsv`, `dungeon_map_parity.py`, `dungeon_registry_census.py`, `c3z5_key_readers.tsv`.
- Method for a dungeon: DungeonEnum / DungeonData rows (data036 / data035, core) -> worlds (Enum `WORLD_ID`, data040 WorldData, files)
  -> NPC ids (`Enum.lua` + data037 `NPCTemplet.lua`, stat tables) -> meshes (`npc_mesh_census.py`) -> map button (June's icon file
  by `adopt-june`) -> emulator check. C3z-2's and C3y-1a's Done notes show it done.

## Phases

| Phase | Scope | Text lives in (JUNE2017_EFFECT_COMPLETION_PHASES.md) |
|---|---|---|
| C3y-1b | Henir Space's 5 missing boss worlds + the other Sander secret worlds (13 worlds); play-test + review owed | "C3y-1b - port: ..." and "C3y-1b - Done" |
| C3y-1c | the Sander secret dungeon (41141, 32 NPCs, map button, BEHEMOTHSOUL effect data) | "C3y-1c - port: the Sander secret dungeon" |
| C3y-2 | June's class tutorial dungeons replace March's battle-field tutorials (9 classes; route known since 2026-11-13: same entry `325622e0`, new ids; kickoff re-scoped to a port) | "C3y-2 - identify, then port: ..." + `kickoffs/june-dungeons/C3y-2.md` |
| C3y-3 | Bestera 1-4 + `DI_BESTERA_START` | "C3y-3 - identify, then port: Bestera 1-4" |
| C3y-4 | Space of Thought 1-3 + `_DUMMY`, the Special-map line | "C3y-4 - port: Space of Thought 1-3" |
| C3y-5 | identify June's route into the Hero tier (read-only), then write the port phases | "C3y-5 - identify: ..." |
| C3z-1b | June's data101 map assets (Velder / Henir); waits for june-asset-port P0 | kickoff `C3z-1b.md`; "C3z-1 - Done" |
| C3z-4t | June's `CX2TutorialGuideManager` (the 9 Battle Field tutorial dungeons via `benders_actor.lua`); scope first: report the system's size before porting. Done 2026-11-13: 4 port phases | kickoff `C3z-4t.md`; "C3z-4t - Done (scope)" below |
| C3z-4t1 | the guide manager core, its loader, its Lua bindings and the key-hint dialogs (unblocks C3y-2's scripts) | kickoff `C3z-4t1.md`; `june2017_re/c3z4t_scope.tsv` |
| C3z-4t2 | the GFx guide movie (`CX2TutorialGuideAdapter`, `tutorial_guide.gfx`); review covers t1 + t2 | kickoff `C3z-4t2.md` |
| C3z-4t3 | the game-side gates (ESC, UI hiding, shortcut keys, custom / server UI events, quest reward byte, deferred quest scene, input mask) | kickoff `C3z-4t3.md` |
| C3z-4t4 | the village / field / quest walkthrough steps 2xx-9xx (field entry, quest handlers, party and local-map UI); review covers t3 + t4 | kickoff `C3z-4t4.md` |
| C3z-5b | the DungeonData readers C3z-5 routed (DCC bits, GetLastClearDungeonID, m_fExtraValue, June-only-row keys) | kickoff `C3z-5b.md`; "C3z-5 - Done" |
| C3z-8 | June's boss-raid systems (`CX2BossRaidLongTimeEvent` enter / state UI, `CX2NewBossRaidManager` + raid phases, June's `CX2BossRaidManager`); scope first, report size and offline reach, then write port phases. Routed from skill-system-core S7o (user 2026-10-19) | kickoff `C3z-8.md`; `JUNE_SKILL_SYSTEM_PHASES.md` "Phase S7o - Done" |
| W3 | June's village world Lua: census every village world script + line map in data040, stage the closure, adopt June's (ljconv), pack, ask, copy. Moved from june-engine by its scope audit (2026-10-20), same ID; Sonnet 5.5 high | `JUNE_ENGINE_REMAINDER_PHASES.md` "Phase W3 - June village world Lua"; kickoff `W3.md` |
| W3b | full investigation of EVERY village and rest area against June after W3 (user 2026-11-12: Elder / Besma NPCs and layout do not match, textures missing): layout, NPC positions, textures, then the unambiguous fixes; Opus 5.5 high | kickoff `W3b.md`; "Phase W3b" below |
| W3c | June's field graph (village / rest / field links, BattleFieldData, MapData BEFORE / AFTER); identify, user decision, port | kickoff `W3c.md`; "Phase W3c" below |
| W3d | June's village NPC roster, services and talk (houses June lists that March does not, June-only houses / places, service + talk keys) | kickoff `W3d.md`; "Phase W3d" below |

Run with: Opus 5.5 high (C3y-5: xhigh). Kickoffs: `kickoffs/june-dungeons/<id>.md`. Board: `STATUS.md`, block `june-dungeons`.

## Phase W3b - every village and rest area against June (user 2026-11-12, after W3)

**Run with:** Opus 5.5 high, then a user play-test. Read-only census first; fixes only where the cause is proven. No code flag unless the census
finds a C++ gap (that becomes its own flagged phase, never an improvised edit). Log tag `[JUNE_ENGINE] WVIL`.

**Why.** W3 (Done note at the end of this file; MODS.md "June villages and rest areas..." and "data101 restored...") adopted June's world scripts and
line maps for villages and rest areas, staged 43 assets, restored `data101` and moved some NPC rows. The user then reported, travelling through
Elder and Besma: NPC positions and village layout do not match, some textures are missing. W3's check was too narrow, so this phase re-checks
EVERY village and rest area, not only those two: Ruben, Elder, Besma, Altera, Peita, Velder, Hamel, Sander, Ranox, Elysion and the rest areas
Elder, Besma, Altera, Peita, Velder, Hamel, Sander, Ranox, Elysion, Atlas Station (1008), plus the Arcade rest.

**Evidence already in hand (verified 2026-11-12, `ErrorLog.txt` of the run at 8:48):**
- `OpenTexture` failures for Besma village: `BESMA_VILLAGE_BRIDGE_FLOOR03`, `_ALCHEMIST_ROOF_01`, `_ALCHEMIST_BUILDING_01`, `_ALCHEMIST_COLUMN_01`,
  `_ALCHEMIST_OBJECT01`, `_ALCHEMIST_OBJECT02`, `_DISPLAY`, `_ACC_OBJECT01` (all `.dds`). March's live koms lack them, June's have them. Also
  `FIELD_ELDER_ROADTILE2.DDS`, `FIELD_DRAGON_ROAD_ROAD02_1B.DDS`, `RUBEN_BANK_CLOSE`, `RUBEN_TECHHOUSE_GANPAN`, `RUBEN_VILLAGE_HERB01` / `02` (check
  whether each is new since W3). `OpenXET` failures are noise (every mesh logs one).
- Likely cause of the textures: W3's closure followed Lua -> meshes March lacks or differs on, but never opened meshes March already had unchanged
  whose textures March lacks (June's new objects, for example the Besma Alchemist buildings). Real closure = Lua -> every mesh -> every texture
  inside the mesh (inflate `.Y` with `june2017_re/fxdata.xfile_text(lenient=True)`; method in `june2017_re/npc_mesh_census.py`: `jbytes`, `tokens`, `resolve`).
- `DLG_Map_Village.lua` `START_POS` is still March's: Elder StartPosId 20 x = -8041 (June -4780), id 22 x = -8499 (June -5071); June's Elder world is
  laid out differently, so spawn / warp-in positions no longer fit. June uses the keys `OtherPlaceID` / `OtherPlaceStartLineIndex` where March's file
  uses `BattleFieldId` / `BattleFieldStartLineIndex` (deliberate Atlas A5 choice; reader `X2Lib/X2LocationManager.cpp` ~455-490); the meaning differs
  for some rows (Besma rest 311: June `OtherPlaceID = VMI_BESMA` line 41, March `BattleFieldId = BESMA_FIELD_01` line 2; Altera 62 line 314 vs 2; Peita 72 line 312 vs 2).
- W3 synced only `COMMON_NPC_LOCATION` rows in `DLG_Map_House.lua` (9 moved, 29 added). Other per-house keys that decide where an NPC stands or where
  its mark / join area is were not diffed.
- Village `HouseIDList`s lack June's `HI_ADAMS_HOUSE`, `HI_MU`, `HI_GUILD_MARK`, `HI_EVENT_*` (those house blocks exist in March; `HI_SPA_SIGN_RUBEN`
  and `HI_PANE_PEITA_HOUSE` do not).

**Steps.**
1. **Census, all places** (`june2017_re/w3b_village_census.tsv`, one row per village / rest area / check). For each place: world script and its
   seasonal variants, line map, `WorldData.lua` row, `Village[...]` block (every key, not only `START_POS` and `HouseIDList`), every house block of its
   `HouseIDList` diffed key by key (June `june_src_fixed/data034_kom/DLG_Map_House.src.lua` and `DLG_Map_Village.src.lua` vs the staged March files),
   the minimap / map-window Lua (`DLG_Map_Village_*_Back/Front/House_Button.lua`, `DLG_Map_Local*.lua`, `MapData.lua`), and the full texture closure
   above, compared by NAME and by BYTES against March (staged and live; `data101` is restored, the staged dir equals live bar 2 empty entries).
2. **Prove the causes** before changing anything: read the log lines, diff tables and the C++ readers (`X2TFieldGame.cpp` `AddFieldNpc` ~3140-3157,
   `X2LocationManager.cpp` `VillageParsing`). June's meaning of `OtherPlaceID` / `OtherPlaceStartLineIndex` is read in Ghidra (skill `ghidra`) where the
   March reader cannot carry it.
3. **Fix what is unambiguous:** stage every June texture / mesh the closure needs (`kom.py stage-june`), June's value for every per-house and per-village
   key March's reader supports (core-list BOM source: edit, `luasrc.py refresh data034 <file>`, `ledger --check`, `core --check`), pack and copy the koms
   (`/elstools pack`, `/x2-kom-packed copy`, no ask).
4. **Route the rest:** a June key or semantic March's C++ cannot read, the missing houses, `HouseIDList` additions whose NPC assets are absent: each gets
   its own bullet in the plan or a new phase with the evidence, deferred and never dropped.
- **Gate:** census at 0 unexplained rows; the texture closure of every adopted world script at 0 missing; `ledger --check` and `core --check` PASS;
  `kom.py pack` of each touched kom shows only the intended changes against live; the Done note lists what is routed.
- **Play-test (user):** unit on the existing offline save; walk each village from the door to every edge, then each rest area: NPCs stand where their
  house mark says, nothing floats or sinks, no pink / black textures, spawn and warp-in points sit on the ground. `python tools/logs.py --grep "OpenTexture"`
  shows none of the names above.

## Phase W3c - June's field graph (routed by W3b, 2026-11-12)

**Run with:** Opus 5.5 high; identify, then a user decision, then the port. June links Elder - Besma - Altera - Peita differently: Besma's east
portal leads to the Besma rest area (line 310), and the Besma rest area leads to Besma (line 41), Peita (line 108) and Altera (line 60) instead of
Besma fields 1-3. W3b found it in `DLG_Map_Village.lua` START_POS (Besma 42 / 43, Altera 62 / 63 / March-only 67, Peita 72, Besma rest 311 / 313 /
315; `w3b_village_census.tsv` rows `routed W3c`). It goes with June's `BattleFieldData.lua` portals and world ids and `MapData.lua` `AddMapData`
BEFORE / AFTER. March's reader already carries village / rest ids as `BattleFieldId` (`X2StateField.cpp` ranges 20000-20010 / 1000-1008), so this
is a data port. **Decision (user, 2026-11-12): June's route**, after the W3b play-test showed the half-installed route (boards, Altera's
bottom path, Elder rest sign; detail in `W3c.md`). Kickoff `W3c.md`.

## Phase W3d - June's village NPC roster, services and talk (routed by W3b, 2026-11-12)

**Run with:** Opus 5.5 high, then a user play-test. The houses June lists that March does not (`HI_GUILD_MARK`, `HI_ADAMS_HOUSE`, `HI_MU`,
`HI_EVENT_GUILD_EDAN_VILAGE`, `HI_EVENT_BLESS_OF_SOLACE` in every village; June-only `HI_PANE_PEITA_HOUSE`, `HI_SPA_SIGN_RUBEN`; Ruben gate `HI_APINK`;
Ruben drops `HI_POSTBOX`), the 18 June-only houses and 3 June-only places (spa Ruben / Ranox, guild agit), and the NPC service and talk keys June
changes in ~40 houses. Each needs its assets, its `AddFieldNpc` gate and its offline system checked before June's value goes in. Kickoff `W3d.md`.

## Phase W3e - June's field worlds and field rows (routed by W3c, 2026-11-12)

**Run with:** Opus 5.5 high; identify, port what is safe, then a user play-test. W3c ported June's field graph and the route fields' assets that
do not move the ground. Left: (1) Ruben field 1 is rebuilt in June (`Field_Ruben_Stage0_Linemap.lua` 53 lines vs 116, 10 meshes, BattleFieldData
portal line 115 -> 51, 21 -> 10 monsters), but March's server spawn table `Ruben_Field_00.lua` walks lines up to 109 and June's copy is not in the tree;
(2) June's line maps for Velder field 1, Sander field 4, Lanox fields 1-4 and the Sander field 3 sign `field_sander_stage2_Guidealpa.Y`, each checked
against its spawn table and portal lines first; (3) BattleFieldData columns that are not the graph: `REQUIRE_LEVEL` on 26 rows (no March reader),
Elder fields 1 / 2 middle bosses (June has none), `MAX_JOIN_USER_COUNT`, the Velder ship `REQUIRE_DUNGEON`. Boss-raid parts (third portals in Hamel /
Sander fields, `RAID_FIELD_PORTAL_POSITION_INDEX`, the Nasod raid row, `raid_ssky.dds`) stay with C3z-8. Kickoff `W3e.md`; evidence
`june2017_re/w3c_field_closure.tsv`.

## Phase W3f - March's route on June's villages (user 2026-11-12, after W3c)

**Run with:** Opus 5.5 high, then a user play-test. **User decision (2026-11-12): June's villages and placement stay, but March's route and
March's sign boards come back, so no field is cut off.** It replaces W3c's route choice and, for the route's boards and line maps only, the
"June's same-name asset replaces March's" rule. Undo W3c's route rows (each carries `-- june-dungeons W3c` with March's value): data034
`DLG_Map_Village.lua` (Besma 42 / 43, Altera 62 / 63, Peita 72, Besma rest 311 / 313 / 315, restore Altera 67) and data036 `BattleFieldData.lua`
(Elder field 1's far portal back to the Elder rest 300). Then put March's copy back for every route board June re-labelled, from the stage backups
(`kom_backup_stage_june/20261112_*`): candidates `Besma_Village_Alpha_Text.Y`, `Field_Peita_Villeage_sign.Y`, `Field_Elder_StageRest_alpaguide.Y`,
`Field_Besma_Rest_G01_Navi.Y`, `Altera_Village_Typo.Y`, and whatever texture each names; and March's `LINEMAP_Altera_Village.lua` (backup
`20261112_083532`) for the bottom walkway to Altera field 3, once it is shown to fit June's Altera world. Keep the W3c field-change ACK fix,
the Lanox rest MapData row and the Altera / Peita field assets. Kickoff `W3f.md`.

## Order (recommendation, 2026-10-11)

1. C3y-1b play-test (user) and review; 2. C3y-1c; 3. C3y-2 / 3 / 4 one at a time (identify halves can run earlier);
4. C3z-5b best before or with them; C3z-4t (scope first) pairs with C3y-2, the class tutorials; 5. C3y-5 any time (read-only); 6. C3z-1b when P0 has restored data101.
7. C3z-8 (added 2026-10-19): its scope half is read-only and runs with anything; port halves wait for the user's look at the scope and, where they need June's
   `BattleFieldData.lua`, for atlas A6b.
8. W3 (added 2026-10-20, from june-engine): any time no other session packs `data040`.

## W3 - Done (2026-11-12, Sonnet 5.5)

Scope grew by the user's request: village / rest-area world scripts and assets to June, Lanox rest NPCs, field-name banner, Sander last-field sign.
- Census: 74 village / rest world scripts + line maps + variants; 13 differed. `june2017_re/w3_village_assets.tsv` = the staged closure (43 files); ~65 `data101` Velder meshes left for C3z-1b.
- `data034` `DLG_Map_House.lua`: 9 positions to June, 29 rows added (Lanox rest 4). `ledger --check` and `core --check` PASS.
- `data057` Fieldname sheets 1 and 6 and `data135` / `data030` sign meshes: June's.
- PACK OK + COPY OK: data024 026 027 028 029 030 031 034 040 057 110 135. Full build skipped: no code change.
- **Left:** `DLG_Map_Village.lua` `HouseIDList` / `START_POS` (June's rest areas arrive from the village; Adams / Mu / guild houses).
- **data101 (follow-up, same day):** dir restored byte-exact from the live kom (1,205 files), packed, copied (md5 3d798391); the Velder village meshes already match June except `Velder_village_ADplane.Y` (staged). Two 0-byte live entries (`Velder_Field_001/003.dds`) dropped. MODS.md has the detail.
- **Play-test owed:** every village loads, NPCs stand on the ground; Lanox rest area shows Hellen / Postbox / Billboard / Glave; entering Lanox field 1 shows the Lanox banner; Sander field 4 ends with the Lanox board.

## W3b - Done (2026-11-12, Opus 5.5)

Data only: no client source, no flag (flagcheck / junediverge not applicable; full build skipped: no code). Census `june2017_re/w3b_village_census.tsv`
(249 rows, 0 unexplained; `python june2017_re/w3b_village_census.py`), closure `june2017_re/w3b_asset_closure.tsv` (`w3b_asset_closure.py`: every
`Village[]` world incl. gates, lounges, rest areas, Atlas Station 1008, plus the Arcade and Peita rests; WorldData row -> every world `.lua` and variant ->
line map, meshes, sounds -> textures / XET / TET inside each mesh, June's copy, compared by bytes; `.lua` against ljconv(June)). New readers
`june2017_re/luatable.py` (data-only Lua tables).
- **Causes proven.** (1) Elder / Besma NPCs: W3 adopted June's worlds but six houses keep their spot in `NPCMeshPos` (no `COMMON_NPC_LOCATION`):
  Richang / Stella stood at y 3910 under June's Besma at y ~5500; Echo / Hofmann / Lenphad / Luichel at March's Elder spots. (2) Elder spawn points 20 / 22
  were March's x. (3) Textures: the closure W3 ran skipped meshes March already had and the NPC models; 52 June files were missing or older.
- **Fixed (data034 core source, `luasrc.py refresh`, `ledger --check` PASS, `core --check` PASS):** `NPCMeshPos` of the six houses, `JoinArea` of Grail /
  Hofmann / Richang, Ariel event house `NPCAniTex`; Elder StartPosId 20 / 22 x; Hamel lounge 87 -> 88 (March duplicated Velder lounge's id).
- **Staged:** `w3b_stage_assets.tsv` (52: 31 + 1 NPC model / XET / face files in data013, 6 data023, 2 data024, 4 data026, 2 data029, 3 data054, 2 new
  data056, 1 data110), `adopt-june` Elysion_Village_Festival / _XMAS (new; `luabind.py check` PASS). After the copy the closure shows 0 differ, 0 missing
  (2 `missing-both`: `Ruben_Dungeon_Gate.lua`, `Motion_NPC_Elsion_Hugo.XET`, June lacks them too). June's NPC models keep March's animation names
  (Wait01-03 / Talk / Bye); Billboard and Postbox become KSM, which March loads.
- **PACK OK + COPY OK:** data013 (md5 16766c83), 023 (a1b10494), 024 (dc4ca09b), 026 (d477b250), 029 (afbe34e5), 034 (fffab949), 040 (8ee149cd),
  054 (1aefda2f), 056 (dcb9947f), 110 (a684e35e). Each pack showed only the intended entries against live.
- **Log textures (run 2026-11-12 8:48):** `RUBEN_BANK_CLOSE`, `RUBEN_TECHHOUSE_GANPAN`, `RUBEN_VILLAGE_HERB01/02`, `FIELD_ELDER_ROADTILE2` are named by
  June's own meshes and June ships no such file (parity, kept). **Open:** the Besma Alchemist / Display / Acc-object / Bridge_Floor03 / Road02_1b textures exist only in June's data028, yet neither June's
  Besma meshes, Besma script nor March's old G01 name them; the full scan after the copies (every .x / .y / .lua / .tet / .xet of all 258 June koms
  and every live kom) found no file naming them. The play-test's `OpenTexture` grep decides whether a follow-up is needed.
- **Kept on purpose:** START_POS rows that differ only by key name (June `OtherPlaceID` = March `BattleFieldId`, atlas A5); `COMMON_NPC_LOCATION` row order.
- **Routed:** W3c (June's field graph: Besma 42 / 43, Altera 62 / 63 / 67, Peita 72, Besma rest 311 / 313 / 315 lead elsewhere in June, with
  BattleFieldData and MapData BEFORE / AFTER; a design decision); W3d (village NPC roster: Adams, Mu, guild mark, EDAN / Bless of Solace, Pane, spa sign,
  Ruben gate Apink, Ruben drops Postbox; 18 June-only houses, 3 June-only places; service and talk keys in ~40 houses; `DLG_Map_House_1/_2`);
  Stage 9 bullet (region-map dialogs `DLG_Map_Local_*`, MapData `AddLocalMapInfo`). The village minimap dialogs already equal June's.
- **Play-test (user, 2026-11-12):** Besma alchemist house still untextured; boards at Altera end / Peita start / Elder rest end wrong or blank;
  Altera bottom portal dead; Cobo list without Elder / Altera rest. **Follow-up (same day):** `fxdata.xfile_text(lenient)` stopped at the first
  MSZIP block zlib rejects (June's `Besma_Village_G01.Y` block 29 of 55), hiding half the mesh; it now pads that block and reads on. The rerun found
  the 9 Besma textures (`Alchemist_Roof_01` / `_building_01` / `_Column_01` / `_Object01` / `_02`, `Display`, `Acc_Object01`, `Bridge_Floor03`,
  `Field_Dragon_Road_Road02_1b`), staged new in data028 (backup `20261112_095755`), PACK OK + COPY OK data028 (md5 64674884). Closure 5,760 rows, 0
  differ / missing; census 0 unexplained. Boards and the Altera portal are June's route half-installed: user chose June's route -> W3c. Cobo list
  equals June's (June's dialog has no Elder / Altera rest rows).

## W3c - Done (2026-11-12, Opus 5.5)

Data only: no client source, no flag (flagcheck / junediverge not applicable; full build skipped: no code, the deployed exe is unchanged).
User decision taken before the phase: June's route. Graph read from June's `DLG_Map_Village` / `BattleFieldData` / `MapData` (`june_src_fixed`).
- **June's route (now live).** Ruben - Ruben field 1 - Elder - Elder field 1 - Besma - Besma rest - { Peita, Altera }; Peita - Peita fields 2 / 3 -
  Velder as before. Besma east (43) -> Besma rest StartPos 310; Besma rest 311 -> Besma 41, 313 -> Peita 108, 315 -> Altera 60; Altera 62 / 63 ->
  Besma rest 314; Peita 72 -> Besma rest 312; Besma west (42) -> Elder field 1 line 2; Elder field 1's far portal -> Besma StartPos 40 (was the
  Elder rest 300). March's Altera StartPos 67 (bottom walkway -> Altera field 3) removed: June has no row and its line map drops that walkway.
- **Edited (core-list source, `luasrc.py refresh`, `ledger --check` PASS, `core --check` PASS):** data034 `DLG_Map_Village.lua` (8 rows + 67 removed,
  March key names kept, a one-line `-- june-dungeons W3c` comment with March's value on each); data036 `BattleFieldData.lua` (Elder field 1 portal 2);
  data036 `MapData.lua` (June's Lanox rest `AddMapData`, MapData.lua:331-342; `AddMapData` is a no-op offline, `X2OfflineMapData.cpp:155`).
  The 20 shared `AddMapData` rows already equalled June's (BEFORE / AFTER included); the Cobo zone / cost functions equal June's but for the
  June-only `VMI_SPA_*` / `VMI_GUILD_AGIT` ids (-> W3d). After the edit every START_POS route equals June's bar Ruben 5 (spa, W3d).
- **Reader check.** Village / rest targets take `EGS_STATE_CHANGE_FIELD_REQ` (`X2StateField.cpp:2139`: 20000-20010 / 1000-1008; Besma 20002, Altera
  20003, Peita 20005, Besma rest 1001), and the "line index" there is a StartPosId (`GetStartPosLoc`, `X2StateField.cpp:2150`).
- **June quirk kept:** Besma rest 313 -> Peita StartPos 108, which neither June nor March defines; `GetStartPosLoc` returns (0, 0, 0) in June too
  (`324f1fd0`, funcmap). `JUNE_BUGS.md` #623.
- **Stranded by June's route (as in June):** Elder field 2 + Elder rest (only reach each other), Besma fields 1-3, Peita field 1, Altera fields 1-3,
  Altera rest; Cobo cannot reach them either (June's `DLG_Destination_byButton`, live since atlas A5, has no Elder / Altera rest row). Quest check (live
  `FieldQuest` / `FieldSubQuest` = June's, adopt-june dry run "identical"): 63 June sub-quests name those fields; only quest 13000 "[Field] Under
  Cover of Night" (Elder field 2) uses any, and nothing leads to it (June chains 12070 -> 13010); no save holds it (`unit_quest`). No dungeon entry
  sits in a field. So nothing reachable is stranded.
- **Field assets.** New `june2017_re/w3c_field_closure.py` (W3b's closure over every BattleFieldData world + line map): 5,199 rows, 23 differ,
  1 missing. Staged the route fields whose ground does not move: `w3c_stage_assets.tsv` (Altera field 1: `Field_Altera_Stage0_Flying_Boat_E` /
  `_Grass_D` / `_Grass_E` / `_Sky_Land_BB.Y`, data032; Peita field 2: `field_Peita_stage1_bluegate.dds`, data030); backup
  `kom_backup_stage_june/20261112_102612`.
- **Not ported, routed W3e:** Ruben field 1's June rebuild (line map 53 lines vs 116, 10 meshes, portal line 51) because March's server spawn table
  `Ruben_Field_00.lua` walks lines up to 109 and June's copy is not in the tree, so Ruben field 1 stays March's whole (its link to Elder is the same);
  June's Velder 1 / Sander 4 / Lanox 1-4 line maps and Sander 3 sign; BattleFieldData's non-graph columns (`REQUIRE_LEVEL`, Elder middle bosses,
  join counts, Velder ship gate). Boss-raid portals / rows -> C3z-8.
- **Elder rest end sign:** `Field_Elder_StageRest_alpaguide.Y` and its texture `Field_Elder_Type1.dds` ("Twin Watchtower", the Elder field 2 board)
  are byte-equal to June's (W3b closure `same`), so what shows there is June's look.
- **Census:** `w3b_village_census.py` re-run: 248 rows, 0 unexplained, 0 `routed W3c`.
- **PACK OK + COPY OK:** data030 (md5 d61905ba), data032 (604d6989), data034 (3c785a26), data036 (ff70b7d4); each pack changed only the intended
  entries. Revert: copy back `kom_backup_tools/data0{30,32,34,36}.kom.20261112_1029*`, restore the staged files from the stage backup, and undo the
  commented rows in the three Lua (`luasrc.py refresh` each).
- **Follow-up (W3c play-test, same day): code fix `SERV_IRUHADEV_JUNE_FIELD_CHANGE_ACK`.** Besma east -> Dicey Sky Road (the Besma rest, 1001) and
  Dicey Sky Road -> Besma drew only the sky and froze the unit; a full load into the rest was fine. Cause: the portal sets
  `m_bNowMovingToBattleField` (`X2StateField.cpp:2164`, June +0x22f) and `CX2StateField::OnFrameMove` returns early while it is set (`:277`); March's
  `Handler_EGS_STATE_CHANGE_FIELD_ACK` never clears it, because March's village portals only led to fields. June `32509660` clears it and sets
  `m_bEnableShortCutKey` (+0xc4, from March's layout probe) first (@325096ac / @325096b2); both are now at the top of March's handler. The rest of
  June's body equals March's: `SetVillageName` is inlined, and E1d already ported the failure-path setter `323aa1b0( -1, 3.0 )`. **Not ported:** June's
  third error branch `0x654` (quest-gated entry: reads a 4th / 5th ACK int as the quest id, string 36141, `UI_PopUp_Negative_01.ogg` box). It is the
  server's reply to the quest gate the user chose not to stand in (2026-10-19, `JUNE_QUEST_SYSTEM_PHASES.md`), so the emulator never sends it; March
  has no matching NetError. Divergence row added (`june_march_divergent.tsv`). flagcheck `--worktree` PASS (X2StateField.cpp PASS, Always.h OTHER
  = a peer's lines), junediverge PASS. My own run was blocked by char-ara CH4b's in-progress `Always.h` guard; char-ara's build `.build/logs/20261112-110130` started after this edit (10:58), compiled X2StateField.cpp with 0 errors: DEPLOY OK 2026-11-12 11:12:37, md5 66ee9277.
- **Play-test (user, 2026-11-12, exe md5 66ee9277): passed.** In-place portals draw the new place and the unit moves. Logs: portals 43 -> 1001, 315 -> 20003, 62 / 63 -> 1001, 313 -> 20005, 74 -> 40010 fired; every village / rest move one `EGS_STATE_CHANGE_FIELD_REQ` + ACK; 0 EXCEPTION, no "isepi wrong". User then chose March's route on June's villages -> W3f.

## W3f - Done (2026-11-12, Opus 5.5)

Data only: no client source, no flag (flagcheck / junediverge not applicable; full build skipped: no code, the deployed exe is unchanged; the
W3c fix `SERV_IRUHADEV_JUNE_FIELD_CHANGE_ACK` stays on). User decision (2026-11-12): June's villages, March's route and boards.
- **Route back to March (core-list source, `luasrc.py refresh` each, `ledger --check` PASS, `core --check` PASS).** data034 `DLG_Map_Village.lua`:
  the 8 `-- june-dungeons W3c` rows take March's target and line from their comment (Besma 42 -> Elder field 2 line 2, 43 -> Besma field 1 line 1;
  Altera 62 -> Besma field 3 line 2, 63 -> Altera field 1 line 1; Peita 72 -> Peita field 1 line 2; Besma rest 311 -> Besma field 1 line 2,
  313 -> Besma field 2 line 1, 315 -> Besma field 3 line 1), comments gone; Altera StartPos 67 restored (36174, 409, -193 -> Altera field 3 line 2).
  The file now equals the pre-W3c backup (`kom_backup_stage_june/20261112_w3b_data034`) bar W3b's 3 own position rows. data036
  `BattleFieldData.lua`: Elder field 1 portal 2 -> `POSITION_INDEX_TO_MOVE = 300`, `VMI_BATTLE_FIELD_ELDER_REST_00` (line 153 unchanged).
- **Boards.** Each board mesh's UV faces painted onto its textures (scratch renderer over the binary `.x`; textures from kom_contents). All five
  route boards were June's (staged by W3 / W3b) and June's names June's route, so March's copy is back, byte-exact:

  | Board | March names | June named | Restored from |
  |---|---|---|---|
  | data028 `Besma_Village_Alpha_Text.Y` | Bethagara Falls (Lv 21-22), Twin Watchtower | Dicey Sky Road, Wally's Memorial Bridge | `20261112_083749` |
  | data030 `Field_Peita_Villeage_sign.Y` | Pilgrim's Gateway, Pilgrim's Site, Feita | Dicey Sky Road, Pilgrim's Site, Feita | `20261112_083431` |
  | data027 `Field_Elder_StageRest_alpaguide.Y` | Wally's Memorial Bridge, Wally's Castle Gateway, Twin Watchtower | Bethma Village, Wally's Memorial Bridge | `20261112_083749` |
  | data029 `Field_Besma_Rest_G01_Navi.Y` | Bethma, Altera, Feita, Bethagara Falls, Toretugera, Leikiki, Pilgrim's Gateway + Lv | Bethma, Altera, Feita | `20261112_091758` |
  | data031 `Altera_Village_Typo.Y` | Toretugera Canyon (Lv 25-26), Graveyard of Purification | Dicey Sky Road | `20261112_083749` |

  Textures stay June's: the two W3b replaced (`Ruben_Dungeon_Gate_Font.dds`, `ruben_build_typo.dds`) keep March's layout (only "LV 1~9" and the
  coin differ), so March's UVs land on the same words; every other texture these meshes name is March's in both. Other replaced meshes checked for
  board textures: only `Besma_Village_Alpha_01/05/06.Y` (house-name sheet, not route) and Sander 3 (off route). June copies backed up to
  `kom_backup_stage_june/20261112_w3f_june_boards`.
- **Altera bottom walkway.** June's `LINEMAP_Altera_Village.lua` drops March's lines 28-30 (34583,480 -> 36264,409) and also trims line 3's end
  (34600 -> 34584, `CAN_DOWN = FALSE` dropped), moves line 26's start (30971 -> 30991), turns two `-2` links into `-1` and sets land height
  140 -> 0. Reported; **user chose March's whole** (2026-11-12): restored byte-exact from `20261112_083532/data040_kom` (March bytecode, md5 45112997).
  Altera field 3's way back (StartPos 68, 35812,409) sits on that walkway.
- **Census.** `w3b_asset_closure.py` re-run (5,760 rows: 5,752 same, 6 differs = the 5 boards, 2 missing-both); `w3b_village_census.py` (`GRAPH`
  note now "kept W3f: user hybrid", new `KEPT_MARCH_W3F` notes): 255 rows, 0 unexplained. Both closures (`w3b_asset_closure.py`,
  `w3c_field_closure.py`) now leave the kept-March names out of their `need_stage` lists, so a later `stage-june` run cannot undo W3f. The line
  map is not in the village closure (no world Lua names it by a `.lua` token); checked by hand above.
- **PACK OK + COPY OK** (each pack changed only its one entry): data027 (md5 1f3fad0b), data028 (ff3be268), data029 (7af719ae), data030
  (be0f60b1), data031 (9ad20a2e), data034 (fffab949), data036 (50f47529), data040 (0524a741). Revert: copy back
  `kom_backup_tools/data0{27..31,34,36,40}.kom.20261112_11452*`; in kom_contents restore the 6 files from `20261112_w3f_june_boards` and re-apply W3c's
  rows (its Done note), `luasrc.py refresh` each.
- **Kept:** the field-change ACK fix, the Lanox rest `AddMapData` row, W3c's staged Altera field 1 / Peita field 2 assets, Ruben field 1 March's.
  W3e's kickoff notes that every field is reachable again.
- **How to see it:** the play-test hand-off below (STATUS row). Log tag `[JUNE_ENGINE] WVIL`; the portal moves show as
  `EGS_JOIN_BATTLE_FIELD_REQ` (into a field) and `EGS_STATE_CHANGE_FIELD_REQ` (into a village or rest) in `offline_packets.log`.
- **Play-test 1 (user, 2026-11-12): every portal works.** Saved logs `24965799\logs_backup_w3f_20261112_120604`: 0 EXCEPTION; Elder field 1 ->
  Elder rest (1000) -> Elder field 2 -> Besma, Besma field 1 / 3, Altera fields 1 / 3 fired; Altera field 3 -> Altera landed on line 30 (the
  walkway). 2 UNHANDLED = `E_DISABLE_HB_CHECK_REQ` (old, not this phase). Two asks, done as a follow-up the same day:
  - **Walkway board missing (text only).** The signpost is a mesh of `Altera_Village_G02.Y` (395 verts, `Field_Altera_waysign.dds`, x 35793-36075);
    June's G02 drops it. Vertex diff March vs June G02: only the signpost + the step at x 34882-34979 (March) vs a crate at x 35000-35167 closing
    the ledge (June); the rest is the same geometry re-cut (120 vs 212 meshes), March also naming 6 textures June re-textured (all present).
    Restored March's G02 byte-exact (md5 baf1759e; June's copy in `20261112_w3f_june_boards`), matching March's line map.
  - **Cobo Express (B) lacked the Elder / Altera rests.** June's `DLG_Destination_byButton.lua` (datai014, core list) has no `Village_1000` /
    `Village_1002`; March's did (backup `20261009_141115`). March's two static blocks inserted at March's positions with a `-- june-dungeons W3f`
    line; `X2StateMenu::CreateWarpDest` (June `324627f0`, atlas A5) already adds a row when the control exists, and MapData's Cobo zones give
    1000 -> 3, 1002 -> 7 (`Handlers_Social.cpp` warp handler prices them). The Peita rest (1003) was in neither list.
  - `luasrc.py refresh` datai014; ledger / core check PASS; G02 added to `KEPT_MARCH` / `KEPT_MARCH_W3F`. PACK OK + COPY OK data031 (md5 1f20d388) + datai014 (cf326d55) (one entry
    each). MODS.md entry rewritten as a "MERGE MOD" note (user ask).
- **Re-test 2 (user, 2026-11-12):** walkway signpost passed. Cobo rows showed empty: my insert had copied each March block only up to its first
  blank line (control created, no picture / string). Replaced with March's whole blocks (icon `Icon_Elder` / `Icon_Altera`, `STR_ID_19920` /
  `STR_ID_19922`, red like the Besma rest); refresh, ledger / core check PASS; PACK OK + COPY OK datai014 (md5 d279d73a). Re-test owed: Cobo only.
- **Re-test 3 (user, 2026-11-12): passed.** Cobo rows show icon + name; logs: `button warp 20003 -> 1000, cost=21780` and `1000 -> 1002,
  cost=27720`, each followed by `entering mapID=1000 / 1002 (warp item)`; 0 EXCEPTION. W3f done. (Unrelated, not this phase: `ChainQuestInfo.lua`
  calls the unbound `AddGroupChainQuest`.)

## C3y-2 - Stopped (2026-11-13, Opus 5.5): blocked on C3z-4t

No code, no flag, no build, no pack. The kickoff's stop rule applied ("if the exit or the latch needs June's
`CX2TutorialGuideManager`, stop and report"). User 2026-11-13: stop here, C3z-4t first, then C3y-2 ports in one go.

- **June's handler, confirmed.** `CX2StateServerSelect::Handler_EGS_CREATE_TUTORIAL_ROOM_REQ` `325622e0`, dumped whole: unit type 2-12
  -> 41160 / 41170 / 41190 / 41180 / 41290 / 41210 / 41220 / 41230 / 41240 / 41250 / 41420, default 41150 (Elsword). That is the
  kickoff table exactly. Every other room field is set the way March sets it, then send 0x5c and wait for 0x5d. **Elesis has no early
  `return`**: she gets 41220. It is June's only tutorial-room builder (the `L"tutorial"` string has one xref, `32562322`), so June has
  no `CX2StateBeginning` copy. March's copy (`X2StateBeginning.cpp:4057`) still sends the old `DI_TUTORIAL_*` 30030-30035. Callers:
  `325712b0` (inventory ACK) and `325672c0` (a popup path). funcmap row `325622e0` updated.
- **Why it stops: the scripts can't be finished without the guide manager.** `luabind.py check` over the 9 `Dungeon_Tutorial_*` gives
  RESULT: FAIL with 12 unbound names, the same set in every script. `CreateCurtain` / `DeleteCurtain` appear only in Lena's and Raven's.
  - 7 are the manager's: `GetTutorialGuideManager` (CX2Game), `ShowInputGuideUI`, `HideInputGuideUI`, `CheckEventTriggerNPCPosX`,
    `CheckAndResetInputAlarm`, `OnBeginGuideNo`, `GetNowGuideNo`.
  - 5 are not: `PopTalkBox` (CX2Game), `IsOnSomethingNow` (CX2GUUser), `ShowJumpGuide` (CX2DungeonGame), and `CreateCurtain` /
    `DeleteCurtain` (CX2DungeonSubStage).
  - Enum gate: 117 failures. In every script: June's `INPUT_GUIDE_STEP` table (IGS_MOVE / DASH / ATTACK / SKILL / AWAKE), the
    `NUI_TUTORIAL_*` ids and the `WI_TUTORIAL_*_STAGE0` world ids.
  - Softlock, Elsword's script: sub-stage 0 clears on `CC_KILL_ALL_ACTIVE_NPC`, and its only active NPC is `NUI_TUTORIAL_DUMY2`.
    DUMY2 is killed only by `RF_STAGE0_SUB0_TRIGGER5`, whose condition is `GetTutorialGuideManager():CheckEventTriggerNPCPosX(DUMY2)
    < 3`. Sub-stage 2 waits on `GetNowGuideNo() == 102`. With March's binder the player cannot get past sub-stage 0, so pointing new
    characters at 41150-41230 now would trap them. March's battle-field tutorials stay live (8 classes; Add keeps CH0c's skip).
- **Routed.**
  - C3z-4t's kickoff now carries the 7 manager names, `INPUT_GUIDE_STEP`, and these 9 scripts as its play-test material.
  - C3y-2's kickoff now depends on C3z-4t and carries the 5 non-manager bindings, which stay its own. Its task is otherwise unchanged.
  - char-add TU and char-luciel TU already wait on C3y-2, so they now wait on C3z-4t too. No edit needed.

## C3z-4t - Done (scope) (2026-11-13, Opus 5.5 high): 4 port phases, T1 first

Read-only: no code, no flag, no build, no pack. Function map: `june2017_re/c3z4t_scope.tsv` (every manager function, its external callers, the
phase that ports it); funcmap rows tagged `C3z-4t`. User 2026-11-13 (after the size report): 4 phases, T1 first; C3y-2 waits for T1 + T2.

- **What the system is.** June's new-player walkthrough, not a tutorial-dungeon helper. `CX2TutorialGuideManager` (RTTI `.?AVCX2TutorialGuideManager@@`,
  vtable `33471a4c`) is a member of `CX2Main` at `+0x8c4` (`31d70b00`, never nil; ctor `31fc3a40` from `CX2Main` ctor `31e46300`, dtor `31fbe320`,
  frame move `31fc6510` from `31d90300`). About 55 functions in `31fbda90`-`31fc6650` (~36 KB of code, 21.7 KB of it the table loader `31fbe4a0`).
- **Four parts.**
  1. *Manager:* a guide state machine (`+0x14`: 0 idle, 1 running, 2 waiting on an event NPC, 4 waiting on a UI event, 6 waiting on a step).
     It reads June's `TutorialGuideInfo.lua` (data036, June-only, not in March): `TUTORIAL_GUIDE_INFO.GUIDE_SCENE`, 24 scenes. Scenes 101-103 are
     the tutorial dungeon; 2xx is the Ruben village; 4xx-5xx are quests and the party / local-map UI; 7xx is a failed dungeon entry; 9xx is
     the holy-animal quest. A scene has about 35 keys: conditions (going / new / complete quest, field, dungeon, UI state), key limits,
     start / end timers, next / replay, a GFx movie number and the UI custom event it waits for. `31fc4530` blocks guides above level 7, or
     when any character on the account is level 10+.
  2. *GFx adapter:* `CX2TutorialGuideAdapter` + `CallbackHandler` (`329656d0`-`32965d90`, about 12 functions). It registers
     `tutorialGuide.showGuide` / `hideGuide` on the GFx manager's movie (`g_pKTDXApp+0x1c8 +0x98`) and takes the `tutorialGudie` callback.
     `tutorial_guide.gfx` is already staged (GFx GA, data194). March's template is the R1 result adapter (`X2UIDungeonResultAdapter.*`,
     `CKTDGGFXResultBridge`).
  3. *Key-hint dialogs:* `ShowInputGuideUI` `31fc4fa0` (`DLG_UI_Tutorial_Input_Guide.lua`) and the side guide `31fc46d0`
     (`DLG_UI_Tutorial_Input_Side_Guide.lua`). Both are June-only data034 files. Blink and per-step key checks: `31fc5570` / `31fc52f0`.
  4. *Game hooks:* about 30 June functions call in (`c3z4t_scope.tsv`, "game hook" rows and the callers column). These are ESC, SetShowUI,
     the state frame move, StateChange, the CX2State custom / server UI events, the quest handlers, PartyUI, CashShop, the Field /
     BattleField ctors, `CX2Game::CheckAndWarningBusyStateNow`, DungeonGame release and the CX2GUUser input mask.
- **Lua surface.** 9 methods on the manager (binder `31d95080`, order from the `PUSH`es @31e3a452-31e3b29b): `CheckEventTriggerNPCPosX`
  `31fc3e80`, `CheckEventTriggerNPCPos` `31fc3f80`, `ShowInputGuideUI` `31fc4fa0`, `HideInputGuideUI` `31fbdcf0`, `GetNowGuideNo` `31fbdcb0`,
  `CheckAndResetInputAlarm` `31fbdcc0`, `CheckToNeedTutorialGuide` `31fbde90`, `OnBeginGuide` `31fc6320`, `OnBeginGuideNo` `31fc5e20` (body `31fc5d70`).
  Plus `CX2Game::GetTutorialGuideManager` `322cc010`. Six of these entry points are not split as functions in Ghidra: disassemble them by
  address. Users: the 12 June `Dungeon_Tutorial_*.lua`, 12 tutorial boss / NPC scripts (data037; `OnBeginGuideNo(102)` x10, `(103)` x1)
  and `Benders_Actor.lua` (March's battle-field tutorials: `CheckToNeedTutorialGuide`, `OnBeginGuideNo(101)`). Enum: `INPUT_GUIDE_STEP`
  (`Enum.src.lua:17590`).
- **Why C3y-2 waits** (agrees with "C3y-2 - Stopped" above). The class tutorial scripts clear sub-stage 0 only through `CheckEventTriggerNPCPosX`,
  and sub-stage 2 only on `GetNowGuideNo() == 102`. So T1 (bindings, state, dialogs) is the hard need. T2 makes scene 101's key limit visible
  and lets it finish the way June's does.
- **Phases written:** C3z-4t1 (manager core + loader + bindings + dialogs + data, small play-test on March's battle-field tutorial: Benders
  death -> `OnBeginGuideNo(101)`), C3z-4t2 (GFx adapter, play-test, review of t1 + t2), C3z-4t3 (game-side gates, incl. the four owed items
  from C3p-2 and Q5b), C3z-4t4 (walkthrough steps 2xx-9xx, fresh-character walk, review of t3 + t4). Kickoffs in `kickoffs/june-dungeons/`.
- **Routed elsewhere:** none. The per-class input-mask notes in the checklist (section 7: Eve `32978e80`) are served by T3's base `31fcea10` port.

## C3z-4t1 - Done (2026-11-13, Opus 5.5 high): the tutorial guide manager, its table, Lua and key-hint dialogs

Flag `SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE` (+ `_DEBUG`: begin / step / end / refuse lines, the C3z-4t2 review retires it). Log tag `JUNE_FX C3z-4t`
(offline_server.log). Map: `june2017_re/c3z4t_scope.tsv`; every function carries its June address.

- **Class.** New `X2Lib/X2TutorialGuideManager.h/.cpp` (in `X2Lib_2010.vcxproj`): June's whole manager except the GFx movie and the outside
  callers. Embedded in `CX2Main` (`m_TutorialGuideManager`, June +0x8c4; `GetTutorialGuideManager()` = `31d70b00`), ticked after
  `CheckClearMovieEnd` (`31d90300 @31d9143c`), loaded in `CX2StateStartUp::LoadData` after the game option (`3259bb20 @3259bbe2`), exit
  hook in `CX2DungeonGame::Release` beside the key restore (`327d2160 @327d21ae` -> `OnExitTutorial` = `31fc3cc0`).
- **Table.** `LoadTutorialGuideInfo` (`31fbe4a0`): 52 keys transcribed from the disassembly (key -> scene offset -> type -> default, read off
  the XOR'd strings and each reader's store); the 0x9c B scene is `GuideScene` with June's offsets in comments. `SERVER_EVENT_TYPE 1` stores
  June's event id 0x101 (t3 maps it). Repeated `GUIDE_NO` keeps the first row (std::map insert, as June).
- **State machine.** begin `31fc5d70`, start mode `31fbdc40`, `CanBeginGuide` `31fc4530` (level > 7 / any account unit 10+ outside a dungeon,
  verbatim), step `31fc4060`, end `31fc4c40`, event NPC / camera wait `31fc4d80` / `31fc4e30`, key wait `31fc6130`, timers `31fc6490` /
  `31fc6050`, finish / next / replay `31fc5ec0` / `31fc5f80` / `31fc6360` / `31fc6450`. The step's allowed-key list (ESC + F2) is March's
  `KTDIDevice` list (DIVERGE_GET_KEY_STATE) with two new setters (`31d25490` / `31d254e0`).
- **Lua.** `CX2TutorialGuideManager` class + 9 methods (binder `31d95080 @31e3a0ab`), `CX2Game::GetTutorialGuideManager` (`322cc010`).
  `luabind.py check Benders_Actor Dungeon_Tutorial_Elsword`: Benders 0 unbound; Dungeon_Tutorial_Elsword's 3 left are C3y-2's
  (`PopTalkBox`, `IsOnSomethingNow`, `ShowJumpGuide`), its 9 ENUM rows (`NUI_TUTORIAL_*`, `WI_TUTORIAL_ELSWORD_STAGE0`) too.
- **Key-hint dialogs.** `ShowInputGuideUI` `31fc4fa0`, side guide `31fc46d0`, hide / fade `31fbdcf0` / `31fc52f0`, blink and key pictures
  `31fbdd30` / `31fc4960` / `31fc5570`, `CheckEventTriggerNPCPos(X)` `31fc3f80` / `31fc3e80`, `CheckAndResetInputAlarm` `31fbdcc0`.
- **Data (PACK OK / COPY OK data034, data036, data057, data081).** New June bytecode: data036 `TutorialGuideInfo.lua`, data034
  `DLG_UI_Tutorial_Input_Guide.lua` / `_Side_Guide.lua` (records `june_adopt_20261113_091510/091515.tsv`); new assets data057
  `DLG_UI_Common_Texture_Tutorial_01.tga/.TET`, data081 `Tuto_Ui_Notice01.ogg`; none replaced. Core data036 `Enum.lua`: June's
  `INPUT_GUIDE_STEP` after `MEMO_ID`; ledger / core check PASS.
- **Stand-ins (one-line comments in the code).** GFx adapter `+0xb8` and show / hide (`32965a60` / `329659b0`) are t2. Scene 101 needs no
  stand-in: June's frame stop writes only `CX2Game +0x2d8` (no June reader) and `g_pTFieldGame +0x12c` (= `SetEnableKeyProcess`), so
  nothing freezes; the scene ends on the skill key (`GA_SKILL1` / `GA_SKILL4`) or ESC. June-only members with no March counterpart,
  skipped and routed to t3: dialog manager `+0x111` (quest reward gate, already t3), `g_pMain+0x3f5` zoom lock (+ reader `3245a8f0`),
  `CX2StateMenu +0x43d` (msg 122 in a guild), `CX2State` vslot `0xb0`, the now-state's `+0x378`, party manager `+0x8a`, the `+0xc4`
  start blocker (no June writer found). Likely, not proven: unit vslot `0x170` = `GetIsRight` (`3208dff0` reads `+0x80f6` beside
  GetPos's `+0x80dc`); `g_pMain+0x1cd` = `m_bPlayGuide`.
- **Hygiene.** flagcheck `--worktree` (9 files): 6 PASS, 2 FAIL = the new files' `#include "StdAfx.h"` / `#pragma once` (house
  precedent), 1 ERROR = the `_DEBUG` `#error` guard; 0 deleted lines, 0 non-ASCII, 0 encoding changes. junediverge `RESULT: PASS`;
  enumscan 0 A/B. Deps checked by `cl` probe: DIVERGE_GET_KEY_STATE, TUTORIAL_MODE, INIT_FOCUS_UNIT, GFX_MANAGER defined.
- **Build.** DEPLOY OK: X2_offline.exe is 26,407,936 B, dated 2026-11-13 10:02:09, md5 2763c264. EXESTR OK 3 of 3 (wide: the load and begin
  lines, `DLG_UI_Tutorial_Input_Guide.lua`) and 2 of 2 (`GetTutorialGuideManager`, `CheckEventTriggerNPCPosX`).
- **The account quirk and the play-test.** Every unit type in the save has a level 10+ character, so `CheckToNeedTutorialGuide` (Benders,
  `31fbde90`) returns false for any new character and `CanBeginGuide` would refuse outside a dungeon anyway. The play-test drives scene 101
  from the console inside a dungeon (level checks skipped there, as June). t4 owns the fresh-account walk.
- **Play-test (2026-11-13, user): pass.** Console scene 101 in a dungeon, twice: `begin 101` / `step 101: gfx 2, key limit 1, stop frame 1` /
  `end 101` (ended by the skill key after 4 s and 26 s); nothing on screen, as expected until t2's movie. Key-hint boxes (`ShowInputGuideUI`
  0 / 2, `HideInputGuideUI`) showed and faded. No `can't find` for the guide names. The start-up `load 24 scene(s)` line is lost:
  `LoadData` runs before offline_server.log opens (scene 101 was found, so the table loaded); the C3z-4t2 review moves that line.

## C3z-4t2 - Done (2026-11-13, Opus 5.5 high): the tutorial guide's GFx movie (CX2TutorialGuideAdapter)

Flag `SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_GFX` (`#error` without `JUNE_TUTORIAL_GUIDE`, `JUNE_GFX_MANAGER`, `JUNE_GFX_EVENTSCENE`, `OFFLINE`;
all four defined at the guard by a `cl /P` probe). Show / hide / callback lines under t1's `_DEBUG`. Log tag `JUNE_FX C3z-4t`.
Funcmap rows `32965bd0`, `32965a60 / 329659b0`, `32965a90`, `31fbdb30 / 31fbdb60`.

- **Adapter.** New `X2Lib/X2TutorialGuideAdapter.h/.cpp` (vcxproj entries): June's `CX2TutorialGuideAdapter` (vtable `334c9c60`): ctor
  `32965bd0` (+4 = the manager, no June reader; +8 = main.gfx's data-binding adapter), show `32965a60`, hide `329659b0`, dtor `329659e0`.
  The Scaleform half is `CKTDGGFXTutorialGuideBridge` in `KTDGGFXManager.*` (R1's pattern): listener `tutorialGuide` (`32965d60`),
  bindings `tutorialGuide.showGuide` (Int value object `334c909c`, GetValue `32948fe0` = VT_Int, new binding kind `JGBK_INT`) and
  `tutorialGuide.hideGuide` (trigger `33471e28`). Show sets the value only when the binding exists, then notifies either way (as June).
- **Callback.** `32965a90` answers the AS call `tutorialGudie` (June's spelling) with true; its 0xc B function object (vtable `334c9c74`)
  is released at once (`31ab4990`), so nothing follows. The string is in neither main.gfx nor tutorial_guide.gfx, so it never fires;
  the port logs it only. vslot 3 is `331a4950` (return false), the listener default.
- **Manager seam.** `Load()` makes the adapter after the table (`31fc3c20`, even without a root movie: it then binds nothing and logs
  `gfx adapter unbound`). `ShowGuideMovie` / `HideGuideMovie` call it; t1 held no key limit or scene end for t2, so no stand-in retires.
  vslots `31fbdb30` / `31fbdb60` = `OnReleaseGfxUI` / `OnRecreateGfxUI`: June calls them indirectly (no xref) and builds its root movie
  once (`31c456e0` from OnCreateDevice), so they have no March caller. The scene count is logged once at the first `BeginGuideScene`
  (t1's start-up line runs before offline_server.log opens).
- **June bug kept (JUNE_BUGS 643).** The manager dtor `31fbe320` never deletes the adapter; kept (also the safe order at exit).
- **Movie check.** `tutorial_guide.gfx` (data194, CFX) names the AS3 class `gfxAS3.tutorialGuide.TutorialGuide`, both binding names and
  the `timeLineNo*` labels of `GFX_GUIDE_NO`; main.gfx loads it as `WIDGET_TUTORIAL_GUIDE` at start-up (G2 play-test). Scene 101's
  `GFX_UI_NO` = `timeLineNoTutorial_1` = 2. June drops a binding the AS asks for before the C++ name exists (`31d1f0a0`), and so does the
  port; `Load` runs in the StartUp ctor, before the frame loop, and a show with 0 observers logs `gfx show %d not seen`.
- **Hygiene.** flagcheck `--worktree` (8 files): 1 PASS, 3 PASS~ (blank/marker lines), 1 DEFINE, 2 FAIL = the new files'
  `#include "StdAfx.h"` / `#pragma once` (t1 precedent); 0 deleted lines, 0 non-ASCII, 0 encoding changes. junediverge `RESULT: PASS`;
  enumscan 0 A/B.
- **Build.** DEPLOY OK: X2_offline.exe is 26,419,200 B, dated 2026-11-13 10:43:12, md5 b4108783. EXESTR OK 3 of 3 (wide: `gfx show %d`,
  `gfx hide`, `(logged at the first begin)`) and 2 of 2 (`tutorialGuide.showGuide`, `tutorialGudie`).
- **Play-test (2026-11-13, user): pass.** Console scene 101 in a dungeon (md5 b4108783), offline_server.log 10:53:10-10:53:12: `load 24
  scene(s) ... (logged at the first begin)`, `begin 101`, `gfx show 2: 1 observer(s) on tutorialGuide.showGuide` (the widget bound),
  `step 101: gfx 2, key limit 1, stop frame 1`, `end 101`, `gfx hide`; no callback line (none expected). 0 EXCEPTION. No review of its
  own: `C3z-4t2-review.md` covers t1 + t2.

## C3z-4t2 - Review (2026-11-13, Opus 5.5 high): pass, t1 + t2 match June; `_DEBUG` retired

Verdict: **pass, no transcription error.** The only code change is `//#define SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG` (Always.h, bytepatch).

- **t1 table (`31fbe4a0`).** All 51 value reads are in June's order with June's getter types (25 bool `32fe9e40`, 22 int `32fe9fd0`, 4 float
  `32fea370`) after the 3 `BeginTable`s. Six spot checks match the key, the default and the scene offset (stack scene at ESP+0x54):
  `GUIDE_NO` +0x00 / 0, `FIRST_GUIDE` +0x20 / false, `START_TIME` +0x24 / 0.0, `SERVER_EVENT_TYPE` 1 -> +0x54 = 0x101, `CUSTOM_EVENT_MSG`
  +0x58 / -10 (key decoded from the XOR bytes at `3379c8b4`), `GFX_UI_NO` +0x78, `REPLASE_GUIDE` +0x80, `REPLAY_GUIDE_NO` +0x98 / 0.
- **`CanBeginGuide` `31fc4530`.** The order is skip byte +0xd0, last custom event +0xd8, then the level blocks only outside a dungeon (unit level
  > 7, account 10+ `31fbdf40`), then F2 off +0xc8, state +0x14 unless high priority, the dungeon test, party member / not leader
  (`32430b80` / `32431330` +0x2c), dungeon match +0x70 (the +0x8a stand-in), and quick-quest +0x79d for condition 1. All of it matches.
- **Step `31fc4060` / end `31fc4c40`.** Every write matches the port's order and its scene offsets (+0x3c zoom, +0x21 skill type A, +0x20
  first guide, +0x4d stop frame, +0x28 / +0x2c inventory, +0x78 gfx, +0x3d / +0x40 MP, +0x34 key limit -> state 3, +0x4e any key).
  The end resets +0xb4 / +0xbc / +0xc0-0xc2 / state, hides the movie, and sets +0xd8 = -20. The 3 June-only bytes are the routed stand-ins.
- **Key wait `31fc6130`.** ESC (`31d24fb0(1)`) -> `AbortGuide` `31fc5f80`. Action ids 0x25/0x26/0x23/0x24, 0/1, 2/5 and 0x1c match March's
  `GA_LEFT/RIGHT/UP/DOWN`, `GA_ATTACK_FAST/STRONG`, `GA_SKILL1/SKILL4` and `GA_AWAKE`. `31fc7be0` matches too: IsMyUnit, count + 0, state 0x17.
- **Lua binder `31d95080`.** The 9 PUSHes run `31fc3e80, 31fc3f80, 31fc4fa0, 31fbdcf0, 31fbdcb0` (`mov eax,[ecx+0x18]` = GetNowGuideNo),
  `31fbdcc0, 31fbde90, 31fc6320, 31fc5e20` (`jmp 31fc5d70`). They match the port's names and order.
- **t2.** The ctor `32965bd0` adds the listener and then 2 bindings only when +8 != 0: an Int object `334c909c` with +8 = 0, and an 8 B trigger
  `33471e28`. Show `32965a60` sets +8 only when the binding is found and notifies either way. Hide `329659b0` and the dtor `329659e0`
  (listener + 2 removes) match. The callback `32965a90` compares with `tutorialGudie` and allocs the 0xc B object (`334c9c74`). It calls
  `31ab4990` (InterlockedDecrement -> delete at 0) and returns 1, else 0. `31fc3c20` builds the adapter with +8 = 0 when there is no root
  movie. `31fbdb30` deletes it through vslot 0, and `31fbdb60` overwrites it without a delete. Dtor `31fbe320` never touches +0xb8, so
  JUNE_BUGS 643 is right.
- **Load line.** offline_server.log has `[10:53:10.605] JUNE_FX C3z-4t load 24 scene(s) from TutorialGuideInfo.lua (logged at the first
  begin)` once, before `begin 101`. That run also logged show / step / end / hide and 0 EXCEPTION.
- **Hygiene.** flagcheck `JUNE_TUTORIAL_GUIDE --with _DEBUG` per commit:
  - `7f27edc3`: 6 PASS, 1 DEFINE, 2 FAIL (the new files' line 1).
  - `451b9568`: 2 PASS, 2 OTHER (the `_GFX` hunks), 2 FAIL (line 1), 1 ERROR (the `_GFX` `#error` guard).
  - Both: 0 deleted, 0 non-ASCII, 0 encoding changes.
  - `JUNE_TUTORIAL_GUIDE_GFX --range 451b9568^..451b9568`: 1 PASS, 3 PASS~, 1 DEFINE, 2 FAIL (line 1). All of these are house precedent.
  - No later commit touches these files or flags. 7f27edc3's 2 deleted lines are the STATUS row and the Enum.lua ledger refresh (June's
    `INPUT_GUIDE_STEP`), both intended.
  - The retire's `--worktree` gives "define NOT FOUND" (expected for a retired debug flag) and 1 replaced line.
  - junediverge `--check-all`: allow-listed 225, `RESULT: PASS`. enumscan: 0 A/B in 9 files.
- **Build.** Skipped: debug retire only (the queued build was cancelled; rule amended 2026-11-13: no build for a debug retire). The always-on lines stay: `load failed`,
  `gfx adapter unbound`, `gfx show %d not seen`, and the first-begin count.
- **Next.** C3z-4t3 is ready (`kickoffs/june-dungeons/C3z-4t3.md` checked). C3y-2 is unblocked (t1 + t2 done) and runs alongside t3.

## C3z-4t3 - Done (2026-11-13, Opus 5.5 high): the tutorial guide's game-side gates

Flag `SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_GATES` (`#error` without `JUNE_TUTORIAL_GUIDE` / `JUNE_GFX_MANAGER`) + `_DEBUG` (gate lines, once a
second per gate; the C3z-4t4 review retires it). Log tag `JUNE_FX C3z-4t gate <name>` (offline_server.log). Funcmap rows tagged `(C3z-4t3)`.

- **ESC** (C3p-2's owed item): `HandleMsgByESC` eats ESC while a guide runs and the scene has no `ANY_KEY_ALLOW` (`@31f1e1e8-31f1e212`), first
  inside the key test, before the result-UI mode (C3p-2's insertion point). C3p's `SKIP_START` comment now names GFx `closeToTopZorder`, not the chat box.
- **Dialog manager `+0x111`** (Q5b's owed item; t1 stand-in retired): `CKTDGUIDialogManager::m_bDialogCreateBlocked`, set to `!DIALOG_CREATE_ALLOW`
  by the step (`31fc42ba`), cleared by the end (`31fc4d70`). June has two readers and both are ported: `OpenQuestRewardWindow` returns before its
  sound (`323d2cb1`), and `KTDGUIOKMsgBox` returns an empty dialog (`31d7d9cc`; the C3z-4t3 audit: March's handle type never dereferences NULL,
  the 7 unguarded `->` sites stay safe). Scene 101 has no `DIALOG_CREATE_ALLOW`, so both are blocked during it.
- **StateChange** (Q5b's owed item): `StopGuide` (`31fc6010`) on every state change, after the relationship reset (`@31d88e27`). Village block (case 14):
  a new field game during a guide gets `SetEnableKeyProcess( false )` (`+0x12c`), and a running guide calls `SetShowQuickSlotByGuide` (`31fbdfa0`, now
  public) instead of the deferred quest scene. Battle field: no deferred scene while a guide runs. Like June, the test sits at the two call sites,
  not inside `PlayDeferredQuestScene` (the kickoff said the player; June's player has no test).
- **UI events:** `CX2TutorialGuideManager::UICustomEventProc` (`31fc63f0`) runs in `CX2State::MsgProc`'s custom-event case after `m_bIgnoreAllUIEvent`
  (June `+0xb5`); `UIServerEventProc` (`31fc63d0`) first in `CX2State::UIServerEventProc`. `SERVER_EVENT_TYPE 1` is now `EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK`
  (June 0x101: `32916d90` sends 0x100 / waits 0x101). 0x7156 / 0x7157 = `STUCM_SKILLTREESLOT_MOUSEIN` / `_MOUSEOUT`.
- **UI hiding:** `SetShowUI` hides the skill and riding slots in the tutorial while the input step is below `IGS_SKILL` (June's `INPUT_GUIDE_STEP`,
  now a manager enum), and in the dungeon state the key-hint dialogs follow the UI (`31fbddf0`). `OnFrameMove`: no shortcut keys while a stop-frame
  guide runs (`31fbdab0`).
- **Busy / shop / local map:** no busy warning during a guide (`@3230238f`); closing the cash shop during a guide shows GFx UI 6 again, every other
  open / close hides it (`@3223d698`); the party UI's local map opens Ruben during a guide (`@322b5a3a`).
- **Input mask:** `CX2GUUser::InitInpuDataProcess` (vslot `0x6fc`, `31fcea10`) masks held keys in the tutorial by input step through
  `InputData::Init_ByInputGuideStep` (`320708f0`; June's groups are March's key order; Lu / Ciel keeps S at the skill step). Eve's override
  (`32978e80`) and other class overrides stay their char ports' (checklist section 7).
- **t1's June-only members:** zoom lock `g_pMain+0x3f5` = `CX2Main::m_bLockCameraZoom` with its reader `CameraZoomInAndOutKeyProcess` (`3245a8f0`);
  party manager `+0x8a` = `m_bAutoPartyMatchFound`, set by the auto-party match popup (`3243d4e0`, beside `StopGuide`), cleared by the close for
  reasons 02 / 03 (and June-only 0x73f) with `BeginReplayGuide` (`3243c260`), read by `CanBeginGuide`. Event ids from June's dispatcher `3243ec20`
  (893 / 895 = March's `AUTO_PARTY_MAKING_SUCCESS_NOT` / `AUTO_PARTY_CLOSE_NOT`, two apart in both); close reasons by NetError value (0x40f-0x412).
- **Routed, not ported (scope rule):** (1) `CX2StateMenu +0x43d` (read by June's bottom-bar helpers `3245a9d0` / `3245c098`), the now-state `+0x378`
  (read by the menu dialog `324617c0` and `32388448-323895a2`), and `CX2State` vslot `0xb0` (field `3245c560`: a June UI at `+0x38c`, gauge `+0xd1`):
  June-only menu / HUD bytes, stand-ins kept, bullet in Stage 9. (2) The rest of June's two auto-party handlers (`32461a70` / `324639a0`, the
  0x40f / 0x410 rework): Stage 28 (updated). (3) `31d72cc0` in `KTDGUIOKMsgBox` (a June-only `g_pData+0x290` system, also SetShowUI's early
  return): Stage 9. Offline the auto-party handlers never fire (no matching server).
- **Hygiene.** flagcheck `--worktree --files` (17 files): 11 PASS, 1 PASS~, 5 OTHER (X2Game.cpp / X2Main.cpp: the peer's
  `JUNE_TUTORIAL_LUA_BIND` hunks; X2State.cpp: the ordered comment fix, 1 replaced line; the manager's .cpp / .h: nested in t1's flags), 0 FAIL,
  0 deleted lines, 0 non-ASCII, 0 encoding changes, `RESULT: PASS`. junediverge `--check` (17 files) `RESULT: PASS`. enumscan: 0 A/B on added lines.
- **Build.** First run failed in this phase's own edit (`X2CashShop.cpp` lacked `KTDGGFXManager.h`; include added under the flag). DEPLOY OK:
  X2_offline.exe is 26,430,464 B, dated 2026-11-13 11:51:19, md5 62a1a621 (built with the peer C3y-2 / CH2a uncommitted files in). EXESTR OK 6 of 6
  (wide: the gate format line, `esc eaten`, `quest reward window blocked`, `ok box blocked`, `skill slots hidden`, `input mask`).
- **Commit.** The code, this note and the STATUS row went in under C3y-2's commit `52951ffe` (an index race between the two sessions;
  `e57133b6`'s message says so). Range flagcheck `52951ffe^..52951ffe`: 13 PASS, 1 PASS~, 16 OTHER (C3y-2's files), 1 ERROR = the `_DEBUG`
  `#error` guard line (t1 precedent), 0 deleted lines, 0 encoding changes.
- **Play-test (2026-11-13, user): pass.** Exe md5 62a1a621, console scene 101 in a dungeon. ESC during the movie closed the guide and the
  system menu did not open. ESC pressed after the key-limit window is taken by the guide's own abort (`31fc6130`, which ticks before the
  state's `HandleMsgByESC`, as in June), so no `esc eaten` line. Begin + OK box in one line: `gate ok box blocked: guide 101` and no box.
  The tutorial-dungeon slot check was skipped (C3y-2's dungeons are not playable yet: C3y-2b). No review of its own: `C3z-4t4-review.md`
  covers t3 + t4 and retires `_GATES_DEBUG`.

## C3z-4t4 - Done (2026-11-13, Opus 5.5 high): the tutorial guide's walkthrough scenes (village, field, quest, party)

Flag `SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_WALK` (`#error` without `JUNE_TUTORIAL_GUIDE`) + play-test switch `_WALK_DEBUG`. t1's `_DEBUG` is on
again (begin / step / end / refuse lines). The C3z-4t4 review retires all three debug defines and `_GATES_DEBUG`. Log tag `JUNE_FX C3z-4t`
(offline_server.log). Funcmap rows tagged `(C3z-4t4)`.

- **Manager (`X2TutorialGuideManager.*`).** `OnBeginGuideByNewQuest` (`31fc5920`, CONDITION_NEW_QUEST_ID; a PRIORITY_HIGH scene over a running one
  reserves it at `+0xcc` and ends it), `OnBeginGuideByLevelUp` (`31fc59f0`, GOING + USE_LEVLE_UP_REPEAT), `OnBeginGuideByCompleteQuest` (`31fc5ae0`),
  `OnBeginGuideByGoingQuest` + `GetGuideNoByGoingQuest` (`31fc5c20` / `31fbe080`: per id the first scene whose field id is 0 or matches, PRIORITY_HIGH
  to the front, the runner-up reserved), `OnOpenClickWaitUI` (`31fc4c30`), `OnAbortGuide` (`31fc5f80`, for Stage 28's caller). Each begin logs
  `begin N ( by new quest / level up / complete quest / going quest )`. `CX2QuestManager::QuestInst::GetFirstNotCompleteSubQuest` = `31f31990`.
  The ids passed are sub-quest ids; the tutorial sub-quests share their quest's id (FieldSubQuest.lua 11005 / 11010), so the scenes match.
- **Hooks.** Field ctor `32509d30` and battle-field ctor `324bbe50`: a complete unit quest begins its scene and stops the scan, then the going
  quests' first sub-quests begin by going quest with the village id (location manager `+0x64`) or the battle-field id (`g_pData +0x134`, ported as
  `GetBattleFieldIdWhereIam`: likely; only scene 201 tests the id). Battle field NPC-load REQ `324b90b0`: `OnBeginGuideWhenNPCLoaded` once per state
  (`m_bTutorialGuideNPCLoaded`, June `+0x5ca`). `CX2State::Handler_EGS_UPDATE_QUEST_NOT` (`31f09b50` and `323e08c0`, merged in Q4e) and
  `CX2UIQuestNew::Handler_EGS_NEW_QUEST_NOT` (`323e4cb0` @`323e5294`, last before the return) -> by new quest. The quick quest's instant-complete
  button (`323e3cab`) -> `OnBeginGuideByGame`. `CX2InformerQuest::IsNowInfoChange` (`32f17780` @`32f17917`) -> by complete quest.
  `CX2Game::Handler_EGS_UPDATE_UNIT_INFO_NOT` (`322e5b40`): a level-up (level before Reset < the packet's) -> the first unit quest's first sub-quest
  by level-up (JUNE_BUGS 647: only the lowest quest id is looked at). `CX2PartyUI::OpenPartyDLG` (`322c2930` @`322c2cbe`) -> `OnOpenClickWaitUI`.
- **UI events (kickoff item 3): nothing to change.** The 8 messages the scenes wait for (UQNCM_EXIT / _QUESTCLEAR_OK / _QUICK_TO_DETAIL /
  _QUEST_INSTANTLY_COMPLETE, PUCM_PARTY_QUICK_PARTY / _PARTY_GAME_START / _SELECT_DUNGEON_MENU, LMUCM_DUNGEON_CLICKED) have the same value in the live
  data036 `Enum.lua` and in C++, are sent by DLG lua buttons, and reach t3's hook in `CX2State::MsgProc` before the owner UI's `UICustomEventProc`.
- **Test reach (kickoff item 4, user 2026-11-13: "dev switch, test only").** `_WALK_DEBUG` skips June's account level-10 refusal (`31fbdf40`) with a
  `dev: account level 10+ test skipped` line; the unit's own level-7 test stays. The review turns it off (June's behaviour returns).
- **Not ported here.** `31d87420` (`OnBeginGuideByGame` at a scene's end) was ported by the concurrent C3y-2b as `CX2Main::OnSceneEnded`
  (`1fc6edcc` / `39e785ea`); t4 relies on it for the WAIT scenes 211 / 402 / 516 / 911. Routed (scope rule): June's `IsNowInfoChange` rework and
  the June-only push handler `323e05f0` (0x27b) -> Stage 4; June's `OpenPartyDLG` body and the auto-party close `322c3fa0` (only caller of
  `31fc5f80` from outside) -> Stage 28. Offline the auto-party handlers and 0x27b never fire.
- **Hygiene.** flagcheck `--worktree --files` (12 files): 11 PASS, 1 PASS~ (one blank line after the new manager block), 0 FAIL, 0 deleted lines,
  0 non-ASCII, 0 encoding changes, `RESULT: PASS`. junediverge `--check` (12 files) `RESULT: PASS`. enumscan: 3 A/B rows, all old lines far from the
  edits (X2QuestManager.cpp:6356 / 6357, X2State.cpp:9326), 0 on added lines.
- **Build.** DEPLOY OK: X2_offline.exe is 26,437,632 B, dated 2026-11-13 13:48:24, md5 f2899679 (built at main 1fc6edcc + 20 uncommitted code files,
  the Elesis CH2b session's included; the first run at 13:19 failed only in that session's half-saved X2GUEL.cpp / X2GUUser.cpp). EXESTR OK 4 of 4
  (wide: `begin %d ( by %s )`, `dev: account level %d+ test skipped`, `complete quest`, `going quest`).
- **Commit.** `d393113b`. It was written from a tree read before C3y-2b's `2652e5de` landed 8 s earlier, so it dropped that commit's hunks;
  C3y-2b re-landed them in `fd841055` (identical to the restore this session had built). Range flagcheck `d393113b^..d393113b --files` (the 13 C3z-4t4
  files): 11 PASS, 1 PASS~, 1 OTHER (Always.h: t1's `_DEBUG` define switched on), 0 FAIL, 0 ERROR, 0 deleted lines, `RESULT: PASS`.
- **Play-test owed** (hand-off below the STATUS row): a new level-1 character through Ruben, scenes 211 / 202 / 204, 201 on re-entry, 503-505 at
  quest 11010, 411 when 11010 completes; one `begin` / `step` / `end` line per scene.

## C3z-4t4 - Play-test (2026-11-13, user): pass, quest highlight 30 px off -> C3z-4t4b

Exe md5 f2899679, a new level-1 character. offline_server.log: `begin` / `step` / `end` for 211 -> 202 -> 204, 503 -> 504 -> 505, 411 and
516 -> 507 -> 508 (the `by new quest` / `by complete quest` begins, every one after the `dev: account level 10+ test skipped` line); 101 / 102 in the
tutorial dungeon; ErrorLog.txt clean. Not seen: 201 (village re-entry with 11005 going; not walked). The user saw the quest scenes' highlight sit
off the quick-quest list. Cause (this session, June `323d0ca0` @`323d18c5`-`323d190c`): June's ctor places the quick-quest dialogs at y
`173 + 22 * i`, March's at `143 + 22 * i` (`X2UIQuestNew.cpp:359`); the GFx timelines are drawn for June's. Fix phase C3z-4t4b (kickoff written).

## C3z-4t4b - Done (2026-11-13, Opus 5.5 high): the quick-quest list at June's y, under the tutorial's quest highlight

Flag `SERV_IRUHADEV_JUNE_QUICK_QUEST_POS` (no dependencies). One site: the `CX2UIQuestNew` ctor's quick-quest `SetPos`
(`X2UIQuestNew.cpp:359`) is y `173 + 22 * i` under the flag, March's `143 + 22 * i` in `#else`. Review: `C3z-4t4-review.md` item 3 covers it.

- **June's values, read from `unpacked_x2.exe`** (ctor `323d0ca0` @`323d18c5`-`323d190c`): x float `33493cd8` = 810.0, step double `33474d68` =
  22.0, base double `33493cd0` = 173.0. March's `g_fQuickQuestMinimizeSizeY` (`X2UIQuestNew.h:11`) is 22.f, so only the base differs.
- **No other absolute quick-quest position (kickoff item 1).** March: `ExpandUIQuickQuest` / `MinimizeUIQuickQuest` (:7953 / :7981 now) move the
  slots below by `+-(fExpandSizeY + 4)` from their current y; the complete particle (:9158) reads the dialog's y. June: `UpdateQuickQuestDLG`
  `323dd640` calls no `SetPos` (`31cd2060`); `MinimizeAllUIQuickQuest` / `ExpandAllUIQuickQuest` `323c96c0` @`323c9793` / `323c98c0` @`323c9b56`
  are relative (GetPos `+0x30/+0x34` -/+ the step); `SetUIQuickQuestSlotInfo` `323da7a0` @`323da895` restores the pos saved at slot `+0x560`, which
  only the 2017 event slot setters `323db830` / `323dc470` write (they also shift relative: `+0x560` + double `33487bb0`); those are Stage 19's and
  never run offline.
- **Rest of June's ctor (kickoff item 2): untouched.** Nothing else in it moves what scenes 211 / 202 / 411 point at; 411's instant-complete
  button is a control of the same dialog, so it moves with it.
- **Other right-side dialogs (kickoff item 3): no code offset.** The quick-quest show checkboxes (`m_pCheckListQuickQuest` /
  `m_pCheckDetailQuickQuest`) are controls of the quest list / detail dialogs, placed by `SetPosition`'s caller as before. `X2PartyUI.cpp` sets no
  absolute position on the party window or the bottom menu (`DLG_UI_PARTY_BOTTON_NEW*.lua`; positions come from the dialog files); 503-505 were
  walked in the C3z-4t4 play-test with no offset reported. Nothing routed.
- **Hygiene.** flagcheck `--worktree --files` (2): X2UIQuestNew.cpp PASS (+5, the plain `#else` is the one tokenless line), Always.h OTHER (a
  peer's uncommitted Elesis block move; this phase's 8 lines appended at the end), 0 deleted lines, 0 non-ASCII, 0 encoding changes, `RESULT: PASS`.
  junediverge `--check` (2 files) `RESULT: PASS`. enumscan: 0 A/B rows.
- **Build.** DEPLOY OK: X2_offline.exe is 26,437,632 B, dated 2026-11-13 14:18:11, md5 8cc8dc3d (built at main 76a33020 + 12 uncommitted code files, peers' included). No new strings, so no exestr check.
- **Commit.** `b29b62bc` (temp-index commit: Always.h carried a peer's uncommitted block move). Range flagcheck `b29b62bc^..b29b62bc --files` (2):
  1 PASS, 1 DEFINE, 0 FAIL, 0 deleted lines, `RESULT: PASS`.

## C3z-4t4b - Play-test (2026-11-13, user): pass

Exe md5 13b50b83 (main 84aca201, includes `b29b62bc`), a new level-1 character. User: the highlight sits on the quick-quest entry in 211 and on the
instant-complete button in 411. offline_server.log: `begin` / `step` / `end` for 101, 102, 211 ( by new quest ) -> 202 -> 204, 503 -> 504 -> 505,
411 ( by complete quest ), 516 -> 507 -> 508; then `refuse: unit level above 7 outside a dungeon` for 11050 at level 8 (June's own test). ErrorLog.txt:
0 `C3z-4t` lines. Not seen: 201 (11005 was done before Ruben was left); user: left to the C3z-4t4 review's hand-off.

## C3z-4t4 - Review (2026-11-13, Opus 5.5 high): pass, t3 + t4 + t4b match June; the three `_DEBUG` defines retired

Covers C3z-4t3 (`52951ffe`), C3z-4t4 (`d393113b`) and C3z-4t4b (`b29b62bc`). No transcription error found; no code changed but `Always.h`.

- **t3, against the disassembly: match.** ESC `31f1e1a0` @`31f1e1e8`-`31f1e212` (31fbda90 running, 31fbdb20 = `[+0x66] != 1` = scene `+0x4e`
  ANY_KEY_ALLOW, eaten -> return false). Dialog manager `+0x111`: writer `31fc42ba` (scene `+0x74` DIALOG_CREATE_ALLOW == 1 -> 0, else 1), clear at
  EndGuide's tail (`31fc4c40`, beside `+0xd8 = -20`), readers `323d2cb1` (after the -1 test, before the sound) and `31d7d9cc` (June returns an empty
  handle {-1, 0}, the port NULL; the t3 audit stands). StateChange `@31d88e27` StopGuide; village `@31d8943f` (`g_pTFieldGame +0x12c = 0` after
  AddUnit) and `@31d8946f`-`31d89493` (31fbdfa0, then a jump past the deferred scene); battle field `@31d896aa`. `31fc63f0`: `+0xd8` first, state 0 ->
  false, not 4 -> true, `+0x70/74/78` -> 31fc6360 EndGuideStep, `+0x80/84/88`, `+0x7c`, 0x7156 / 0x7157 (= the port's scene fields `+0x58`-`+0x70`);
  its call site `31efa9c0` @`31efaac9` after `+0xb5`, eaten -> true. `31fc63d0` (state 4 and `+0x6c` = scene `+0x54`) first in `31f2ab80` @`31f2abe1`.
  SetShowUI `32154820`: the `0x16a` tutorial and `+0xe4 < 3` terms zero `+0x89` / `+0x8b` in both riding branches = the port's two setters. Input mask:
  `31fcea10` (tutorial only; unit type from `CX2Unit +0xaf0`) and `320708f0`, mapped to March's InputData byte offsets (`+0x10` Z ... `+0x48` LCtrl,
  `+0x4c` Sit = June `+0x8726` - `+0x86da`, `+0x50` / `+0x54` the slot-10 keys): every case, the fall-throughs, the Lu / Ciel (type 10) skill step and the
  tail match. Zoom `3245a8f0` (`+0x3f5` -> key taken, no zoom). Party `+0x8a`: `3243d4e0` @`3243d751` / `@3243d771`; `3243c260` reasons 0x411 / 0x412 /
  0x73f reach `@3243c646` (`+0x8a = 0`, 31fc6020) with no branch out. ShortCutKeyProcess `@31f0d998`: June's third term 31fbde80 is IsPlayingTutorial
  again, so the port's two terms are exact. Cash shop `@3223d698`, busy `@3230238f`, local map `@322b5a3a` (guide or 0 -> 0x2710 = March's fallback).
- **t4, against the disassembly: match.** `31fc5920` / `31fc59f0` / `31fc5ae0` match scene `+0x14` / `+0x10` + `+0x22` / `+0x18`, call `31fc4530( 1,
  prio == 2, +0x08 )` / `( 1, 0, +0x08 )` / `( 1, 0, +0x08 )`; PRIORITY_HIGH over a running guide reserves `+0xcc = +0x18` and calls `31fc4c40` (=
  EndGuide). `31fc5c20` / `31fbe080`: per id the first scene with field 0 or equal, PRIORITY_HIGH push_front, front returned, the next reserved even if
  the first cannot begin, `31fc4530( 0, 0, +0x08 )`. `31fc4c30` (state 6 -> step). `31f31990` (8-byte sub-quests, `+6` = success). Field ctor
  `@3250a047`-`3250a289` (complete quest -> 31fc5ae0 then jump to 31fc5c20 with the vector so far; village id `g_pData +0x268 +0x64`).
  **Battle-field id now confirmed:** `g_pData +0x124` is the battle-field manager (`32481fa0( g_pData +0x124 )` is March's
  `GetIsBossRaidCurrentField`: state 0x15, then `GetIsBossRaidFieldByFieldID( [this +0x10] )`), so `+0x134` = `m_uiBattleFieldIdWhereIam` = the port's
  `GetBattleFieldIdWhereIam`; funcmap row updated. `324b90b0` @`324b9119`-`324b9134` (31fc5e70 once, `+0x5ca`). `32f17780` @`32f17917` after `+0x28` /
  `+0x2c`. `31f09b50` @`31f09c83`-`31f09ce5` (after `32331a30` UpdateUnitQuest, sub-quest id). `323e4cb0` @`323e5294` (the packet's quest id
  `local_48[0]`). `323e3cab` (31fc5e30 after `323d2c60`). `322e5b40` (quest map copy, `begin()`, `+0xaf8` written by Reset `@323627da`; JUNE_BUGS 647
  stands). `322c2930` @`322c2cbe` (dungeon side after `+0x146 = 1`).
- **t4b:** `323d18c5`-`323d190c` read again: `810.0f`, `22.0`, `173.0`, `SetPos` `31cd2060`. Match.
- **Routed bullets exist:** Stage 4 "The quest informer's June rework" (`32f17780`, `323e05f0`); Stage 28 (`322c3fa0`, `3243bed0` / `3243c260`).
- **Hygiene.** flagcheck `_GATES --range 52951ffe^..52951ffe --with _GATES_DEBUG`: 13 PASS, 3 PASS~, 1 DEFINE, 14 OTHER (C3y-2's files), 0 FAIL,
  0 ERROR, 0 deleted lines, 0 encoding changes, `RESULT: PASS`. `_WALK --range d393113b^..d393113b --files` (13): 11 PASS, 1 PASS~, 1 OTHER (Always.h:
  t1's `_DEBUG` re-enabled, the 2 "deleted" lines are its old commented define + comment), 0 FAIL, `RESULT: PASS`. `JUNE_QUICK_QUEST_POS --range
  b29b62bc^..b29b62bc`: 1 PASS (the plain `#else`), 1 DEFINE, `RESULT: PASS`. junediverge `--check-all`: `RESULT: FAIL`, 4 hits, all `GT_PVP` in
  `X2GUEL.cpp` (5736 / 5766 / 6465 / 6957, char-elesis `b907661f` / `6f9608ea`): not this review's files, reported to that plan, not edited here.
  enumscan (12 files): 3 A/B rows, all old lines (X2QuestManager.cpp:6356 / 6357, X2State.cpp:9326), 0 on added lines.
- **Debug retired** (`KTDXLIB/Always.h`): `_GUIDE_DEBUG`, `_GATES_DEBUG`, `_WALK_DEBUG` commented out. With `_WALK_DEBUG` off the walkthrough again
  shows only on an account with no character at level 10+, as in June. Build skipped: debug retire only. The deployed exe (md5 b68f1fcc) still has all
  three on, which the scene-201 walk below uses.
- **Owed (user):** the scene-201 walk. The first hand-off ("leave Ruben and come back") was wrong: 11005's one sub-quest is VISIT_FIELD 40000, so
  leaving to the field finishes it. Corrected: hand in 11000 before visiting the field, then ESC -> Character Select -> same character ->
  `begin 201 ( by going quest )`.
- **Found in the walk (user, 2026-11-13), routed to C3z-4t4c:** 11005 completed in Ruben on accept. The emulator's `EGS_STATE_CHANGE_FIELD_REQ`
  never leaves the room (the real server does, `GSUserField.cpp`), so `QuestCheckHereAndNow` ran `OnVisitField( 40000 )` from the stale battle-field
  room (offline_server.log 15:14:58.704). Not a tutorial-guide defect; kickoff `C3z-4t4c.md`.

## C3z-4t4c - Done (2026-11-13, Sonnet 5.5 high): the offline server leaves the room on a field change

Flag `SERV_IRUHADEV_OFFLINE_LEAVE_ROOM_ON_FIELD_CHANGE` (`KTDXLIB/Always.h`, no `_DEBUG`: the `ROOM     leaving room` line is the evidence).
No review of its own (one emulator handler, transcribed from the real server).

- **`Handler_EGS_STATE_CHANGE_FIELD_REQ`** (`Handlers_Field.cpp`): ACK, buff list, then (when `m_kRoom.m_bActive`) the room leave, in the order of
  `GSUserField.cpp:211-221, 276-334`. Reason: `RT_BATTLE_FIELD` -> `NOT_LEAVE_ROOM_REASON_30` with the warp item, else `_27`; every other room
  type (square, training centre, dungeon) -> `_01`. The client's room-type enum has no wedding hall, so the server's `RT_WEDDING_HALL` arm has no
  offline counterpart. No `EGS_LEAVE_ROOM_ACK` (the client never asked).
- **Shared tail** `CX2OfflineServer::LeaveActiveRoom( kSes, iReason )` (`Handlers_Room.cpp`, declared in `X2OfflineServer.h`): the `ROOM     leaving
  room` log line, `CX2OfflineBuff::OnLeaveRoom`, `m_kRoom.Clear()`, `S_FIELD_MAP`; returns "buff list changed" so each caller sends
  `SendUpdateBuffInfo` after its own ACK. `Handler_EGS_LEAVE_ROOM_REQ` now calls it (its old tail sits in the flag's `#else`).
- **Reader audit** (clearing on a field change is safe because the real server clears at the same point):
  - `Handlers_Quest.cpp:143-150` `QuestCheckHereAndNow`: unchanged, now falls through to `OnVisitVillage( m_iLastPos )` in a village (the defect).
    `:738`, `:1142`, `:1244-1247`, `Handlers_Inventory.cpp:3976`, `Handlers_Skill.cpp:213`, `Handlers_Field.cpp:380` all test `m_bActive` first.
  - Dungeon result / continue-end: the client sends `EGS_STATE_CHANGE_FIELD_REQ` after the result is closed, so every result-time reader has run.
    A late `EGS_LEAVE_ROOM_REQ` on a cleared room only logs `RoomUID 0`, skips the buff call (`m_bActive` guard) and ACKs, as before.
  - Training centre `Handlers_Room.cpp:798` ("Not m_kRoom.Clear()"): it keeps the room for the 5 s after `EGS_END_TC_GAME_ACK` because the client keeps
    sending sync packets; the field change comes after those, and the server leaves `RT_TRAININGCENTER` there too.
  - `X2OfflineServer.cpp:181` reconnect note: about `m_nSelectedUnitUID`, not the room; `m_kRoom` is a server member, not a session one. Unaffected.
  - Field -> field portals use `EGS_JOIN_BATTLE_FIELD_REQ` (`X2StateBattleField.cpp:1238`), whose `OpenRoom` clears the room itself; only
    `PMT_MOVE_TO_VILLAGE` (`:1245`) sends the field change.
- **Hygiene.** flagcheck `--worktree --files` (4): 1 PASS, 2 PASS~ (1 marker line each), 1 DEFINE, 0 FAIL, `RESULT: PASS`. junediverge `--check`
  (3 files): `RESULT: PASS`. enumscan: 0 rows (reasons are `NetError::NOT_LEAVE_ROOM_REASON_*`, room type `CX2Room::RT_BATTLE_FIELD`).
- **Build.** `DEPLOY OK: X2_offline.exe is 26,441,216 B, dated 2026-11-13 15:54:08, md5 bd2b85980ddcf429ebc0cb3604ce1b59`, built at main `07fece2b` + the 4 files above.

### Play-test hand-off (gate): C3z-4t4c

Character: a new level-1 character of any March class (tutorial dungeon, then Ruben).
1. Tutorial dungeon -> result -> back in Ruben. Look for: `ROOM     leaving room ...` on the village entry (the reason is the number of `NOT_LEAVE_ROOM_REASON_01`).
2. Ruben -> field 40000 (portal) -> back to Ruben. Look for: `ROOM     leaving room ...` on the return, with a different reason number than step 1 (`NOT_LEAVE_ROOM_REASON_27`).
3. In Ruben hand in 11000. Expected: 11005 is accepted and **not** completed; no `VISIT_FIELD done` line while in the village.
4. Go to the field. Expected: `11005 sub 11005 VISIT_FIELD done (field 40000)` on field entry.
5. Optional, the review's owed scene-201 walk: hand in 11000 before the field, ESC -> Character Select -> same character; 201 should begin
   (the `begin 201 ( by going quest )` line needs `_GUIDE_DEBUG`, retired, so judge by the guide appearing on screen).
If not: `python tools/logs.py --grep "ROOM     leaving|VISIT_FIELD done|VISIT_VILLAGE done|C3z-4t begin"`; say "check logs" after.

## C3z-4t4c - Play-test (2026-11-13, user): pass (logs)

Two characters (unit 159, 160), both runs after the deploy. Village entry: `ROOM     leaving room 1001 (reason=115)` / `1003 (reason=115)` =
`NOT_LEAVE_ROOM_REASON_01`. Return from field 40000: `leaving room 1002 / 1004 (reason=941)` = `_27`. 11000 handed in -> 11005 accepted with no
`VISIT_FIELD done` in the village (16:14:07, 16:18:15); `11005 sub 11005 VISIT_FIELD done (field 40000)` only on field entry (16:14:48, 16:18:23).
0 EXCEPTION. Scene-201 walk: not judged from logs (its debug line is retired); the C3z-4t4 row still owes it. Unrelated, not this phase:
`AddGroupChainQuest` unbound in a June script; `QUEST_11000_END` / `_11005_END` `.GFX` / `.SWF` missing from the kom.

## C3y-2 - Done (2026-11-13, Opus 5.5 high): June's class tutorial dungeons for the March classes

A new March character's first select now enters June's class tutorial dungeon (`DI_DUNGEON_TUTORIAL_<class>`, 41150-41290) in place of
the battle-field tutorial. Flags (`KTDXLIB/Always.h`): `SERV_IRUHADEV_JUNE_CLASS_TUTORIAL_DUNGEON` (+ `_DEBUG`, `#error`s without
`JUNE_ENUM_NAMES`, `JUNE_TUTORIAL_GUIDE`, `JUNE_TUTORIAL_LUA_BIND`), `SERV_IRUHADEV_JUNE_TUTORIAL_LUA_BIND` (needs `_PLAY_GUIDE`),
`SERV_IRUHADEV_JUNE_TUTORIAL_PLAY_GUIDE`. funcmap rows tagged C3y-2; MODS.md "the March classes' June tutorial dungeons".

- **Room request** (`325622e0`, re-checked): June's switch replaces March's in `CX2StateServerSelect::Handler_EGS_CREATE_TUTORIAL_ROOM_REQ`
  (types 2-9 -> Aisha 41160, Lena 41170, Raven 41190, Eve 41180, Chung 41290, Ara 41210, Elesis 41220 with no early return, Add 41230;
  default Elsword 41150); Lu/Ciel / Rose / Ain are a one-line stand-in. The `CX2StateBeginning` copy gets the same switch, but it is
  compiled out (`ELSWORD_NEW_BEGINNING` is commented at `InHouse3.h:486`), so only server select logs. Divergence row 408 -> replaced.
  `DungeonEnum.h`: the 9 ids under `JUNE_ENUM_NAMES`. Add's first-select skip (CH0c) is untouched: char-add TU retires it.
- **The 5 unbound names, whole from June:** `PopTalkBox` `32305f00` -> new `CX2Game::PopTalkBox( uid, string id, bUser )` (talk box on
  `m_pTalkBoxMgr`; the scripts pass 2 args, so the box goes to an NPC owner and is dropped: JUNE_BUGS 644, kept); `IsOnSomethingNow`
  `31e4d0a0` = `m_FrameDataNow.unitCondition.bFootOnLine` (+0x80bc, the twin of Future's +0x7e1c); `ShowJumpGuide` `327ca2c0` ->
  `CX2DungeonGame::ShowJumpGuide( f )` -> the play guide; `CreateCurtain` / `DeleteCurtain` `327ed0a0` / `327ed240` = March's bodies, bound.
- **Play guide (`X2PlayGuide.*`), what ShowJumpGuide needs:** June's jump guide (create `3289e030`: UI-major `JumpGuide`, layer 12,
  hidden; show `3289e270`: `Tuto_Ui_Jump01.ogg` when it was hidden; follow the head 80 px up `3289cfe0`; frame-move `3289e370` hides it
  with a 2.0 s time once elapsed; delete `3289d1d0` beside the input guide in `CX2DungeonGame::Release` @`327d25f3` and
  `X2StateDungeonGame.cpp:6902` (`324ce714` @`324cea74`); hide on tutorial exit `327d2160` @`327d21c7`). June's `OnFrameMove_Tutorial`
  `3289e180` / `SetPostionInputGuide` `3289df90` replace March's (place only, never create, 80 px, no quest-guide particle): divergence
  rows added. June's whole `CX2PlayGuide::OnFrameMove` `3289e490` differs beyond the tutorial (state 7-8 skip, picture layout):
  only the `3289e370` call is in; the rest is a divergence row + allow row routed to Stage 9.
- **Emulator:** `IsQuestTutorialDungeon` learns the 9 ids; `IsEventDropDungeon` / `IsSkillUseCountedDungeon` learn Elsword's (they named
  only his). Stages load from the client's DungeonData (`Handlers_Room.cpp:2286`), so the rows below are what "stage 0 out of range" needs.
- **Data (subagent, verified by the gates):** core source data036 `DungeonEnum.lua` (+9), `Enum.lua` (+9 `WI_TUTORIAL_*_STAGE0`, +41
  `NUI_TUTORIAL_*`, no value collisions), data035 `DungeonData.lua` (June's 9 rows), data037 `NPCTemplet.lua` (41 rows; NewNPCStat rows
  already there now resolve), data040 `WorldData.lua` (9 rows keyed by `worldID`; June reuses the name `WI_TUTORIAL_EL_STAGE0` on a school
  row). 18 world scripts by `adopt-june` (`june_adopt_20261113_112144.tsv`); 416 assets by `stage-june`, 0 replaced, 47 identical; 8
  `tutorial_new_*.ogg` copied to the game dir's loose `data\music\` (June keeps them loose, C3y-1b precedent). ledger / core check PASS.
  `dungeon_registry_census.py`: SAME 250, STR_ID missing none. `luabind.py check` over the 9 scripts: RESULT PASS (0 unbound, ENUM 0).
  Still missing in June too: 2 stage BGMs, 3 textures, `HQ_BOSS_NAME_TUTORIAL_BOSS_SKA.dds`, 10 NPC sounds, 3 NPCTemplet MOTION names
  (unit scripts load their own meshes) - JUNE_BUGS 645 with the 4 nil NPC names.
- **Pack / copy:** PACK OK + COPY OK data035 3f0fc5c0, data036 991bcd6f, data037 2ce7045b, data040 24f03ec0, data078 861dc440, data080
  8e24690e, data081 8491c0a0, data082 6d58afc3, data083 bf76d90d, data084 87080081, data108 15704f65, data126 31db9eb7, data133 f1219677,
  data144 e885e6a7, data149 e9552cec, data164 3f1e542f, data165 b1500b88, data166 175e2d81.
- **Hygiene.** flagcheck `--worktree` (16 files): every site PASS (CLASS_TUTORIAL_DUNGEON 4, LUA_BIND 4, PLAY_GUIDE 3, the X2Main /
  ServerSelect / DungeonEnum / DungeonGame hunks counted under their own flags), 0 FAIL, 0 deleted lines, 0 non-ASCII, 0 EOL mismatch;
  1 ERROR per flag = the `#error` dependency guards in Always.h (precedent). junediverge `RESULT: PASS`; enumscan: 2 old rows outside
  these hunks (`X2GUUser.h:2564-2565`), 0 new.
- **Build.** First run failed only in C3z-4t3's then-unfinished files (X2GUUser / X2CashShop / camera zoom; 0 errors in C3y-2's); after their fix: DEPLOY OK: X2_offline.exe is 26,430,464 B, dated 2026-11-13 11:51:19, md5 62a1a6211612ef8d77162e5087f6fc12 (built at main 5b50392e + 34 uncommitted code files, shared with C3z-4t3). EXESTR OK: 3 of 3 (wide: the room line, Tuto_Ui_Jump01.ogg, JumpGuide) and 5 of 5 (the Lua names), md5 62a1a621.
- **How to see it:** create a new Elsword (and an Elesis); the first select loads `Ruben Forest` (Elsword) / `Ruben Training Ground`
  (Elesis), the guide hints run, the stage clears to the village. offline_server.log `JUNE_FX C3y-2 room 41150` / `41220`.
- **Range flagcheck** (`52951ffe^..e57133b6`, C3y-2 files): CLASS_TUTORIAL_DUNGEON 4 PASS, LUA_BIND 5 PASS, PLAY_GUIDE 3 PASS, 0 FAIL, 0 deleted; 1 ERROR each = the Always.h `#error` guards. 52951ffe also carries C3z-4t3's staged files (index race; e57133b6 restored C3y-2's lost hunks).
- **Debug define stays on** until the review. Next: user play-test, then `C3y-2-review.md`.

## C3y-2 - Play-test (2026-11-13): failed -> C3y-2b

New Elsword (`JUNE_FX C3y-2 room 41150 ( server select, unit type 1 )`) and new Elesis (`room 41220 ... unit type 8`) both reach June's
tutorial worlds (no `stage 0 out of range`, 0 EXCEPTION). The user saw: the old tutorial loading screen for a second before June's; the
opening slide shot (`PF_STAGE0_SUB0_SHOT0`) not skippable with ESC / right click; the character cannot move. Cause of the lock: the scripts
call `PlayEventScene` / `IsRecentlyPlayedEventScene` as CX2DungeonGame methods (June binds them there, `327ccad0` / `327ccb20`); March
binds them only as globals, so `CannotInput( 9999 )` is never released (ErrorLog x1 / x1114). `luabind.py` missed it (name-only match).
Also logged: OpenXET failures for the tutorial worlds and missing Ruben textures. All routed to `kickoffs/june-dungeons/C3y-2b.md`.

## C3y-2b - Done (2026-11-13, Opus 5.5 high): the tutorial dungeons' first play-test fixed

Flags (`KTDXLIB/Always.h`): C3y-2's `SERV_IRUHADEV_JUNE_TUTORIAL_LUA_BIND` (missed piece; new `#error` without `JUNE_GFX_EVENTSCENE`), new
`SERV_IRUHADEV_JUNE_SCENE_END_HOOK` and `SERV_IRUHADEV_JUNE_TUTORIAL_ROOM_LOADING`. Debug lines under C3y-2's `_DEBUG` (the review retires it).
funcmap rows tagged C3y-2b; MODS.md "tutorial dungeon fixes".

- **Input lock (fixed).** June binds `PlayEventScene` / `IsRecentlyPlayedEventScene` only as `CX2DungeonGame` methods: `class_def` helpers
  `31e7b2d0` @`31de31c0` / `31e7b420` @`31de344f` on class table `33777990` (= `ShowJumpGuide`'s); the bodies `327ccad0` / `327ccb20` ignore
  `this` and call `3226b130` / `3226a130` (xrefs @`327ccb0c` / @`327ccb58`). New members call March's `Gfx*EventScene_LUA`; March's two globals
  (GFx G3 misread them as globals) are in the `#else`. No live Lua calls them as globals. C3z-4t3's `input mask: guide 0, value -1` line is
  ruled out: step -1 is `case -1: return;` in `Init_ByInputGuideStep` (`X2GUUser.cpp`), no mask.
- **Gate gap (closed).** `tools/luabind.py check` reports `METHOD BOUND ONLY AS GLOBAL` (a `x:Name(` whose name is only `def`'d) and takes
  `--receiver CLASS --receiver-var slot1` (class_def on CLASS or a `class_inh` base). Before the fix all 9 scripts FAIL on the two names;
  after: 9 x `0 unbound, 0 only-global, 0 off-receiver`, RESULT PASS (also with `--receiver CX2DungeonGame --receiver-var slot1`).
- **Story skip (fixed).** June's `CX2SlideShot::OnFrameMove` `325c6b50` = March's (Enter / left click skip one shot; ESC / right click in the
  tutorial only drop the frame). The whole-story skip is June's `CX2SlideShot::MsgProc` `325c74b0` (showing && ESC key-down or right
  button-up -> scene end + `31d87420`), called by `CX2Main::MsgProc` @`31d8e5ce` after the two scenes' MsgProc: March had none.
- **Scene-end hook.** `CX2Main::OnSceneEnded` = June `31d87420`: the guide manager's `OnBeginGuideByGame` (C3z-4t's, no caller until now), a
  stand-in for `CX2UIQuestNew` `323ca2a0` (deferred REQ 0x36a, Stage 9 bullet), and dungeon 41470 (`DI_EVENT_2017_TREASURE`) start delay 0.
  Callers ported: the MsgProc skip, Lua `ScenEnd` -> `ScenEnd_LUA` (`325c74a0`: scene end + hook; every tutorial's last shot calls it) and
  the GFx scene end @`3226be30`. June's `CX2SlideShot::ScenEnd` `325c7140` adds two UI steps: divergence row + allow row -> Stage 9.
- **Loading flash (fixed).** `CX2StateDungeonRoom::UISetting`'s tutorial / field-dungeon branch set `HQ_Loading_RUBEN_PROLOG_01/02.tga`;
  June's `324e6130` @`324e79a3`-`324e7a71` sets no picture there (only the exiting branch and `324e93b0` do). The dialog's own pictures stay:
  June's `DLG_PVP_Game_Loading_State.lua` adopted (data034, bytecode; same 5 controls as March's, black bars; luabind PASS).
- **Assets (nothing to stage).** Full closure of the 18 world scripts + 41 NPC rows (meshes, textures inside `.X` / `.Y`, mesh-derived XETs):
  976 names June has, all already in March (935 assets byte-identical, `stage-june --dry-run` 0 new / 0 replaced); the logged textures,
  world layers and 301 XETs are missing in June too: JUNE_BUGS 646.
- **Pack / copy:** PACK OK + COPY OK data034 md5 d2b06cfb.
- **Hygiene.** flagcheck `--worktree --files` (9 files): SCENE_END_HOOK 4 PASS, TUTORIAL_ROOM_LOADING 1 PASS, TUTORIAL_LUA_BIND 2 PASS (+ the
  X2Main hunks counted under SCENE_END_HOOK); 0 FAIL, 0 non-ASCII, 0 EOL mismatch; 1 ERROR = the `#error` guard (precedent); LUA_BIND's 19
  deleted lines are a peer's uncommitted `Always.h` block reorder, not committed here (see the range line). junediverge `RESULT: PASS`
  (allow-listed 7, selftest PASS); enumscan 0 A/B rows.
- **Build.** DEPLOY OK: X2_offline.exe is 26,430,464 B, dated 2026-11-13 13:10:58, md5 c49b3858237554964c663bf8bfd63500 (built at main a6fa91e8 + 9 uncommitted code files, all C3y-2b's). EXESTR OK 1 of 1 (wide: the scene-end line) and 3 of 3 (PlayEventScene, IsRecentlyPlayedEventScene, ScenEnd), md5 c49b3858.
- **Range flagcheck** (`39e785ea^..39e785ea`): SCENE_END_HOOK 4 PASS, TUTORIAL_ROOM_LOADING 1 PASS, TUTORIAL_LUA_BIND 2 PASS; 0 FAIL, 0 deleted
  lines; 1 ERROR = the Always.h `#error` guard (precedent). The peer's `Always.h` / funcmap reorder was left out of the commit.
- **How to see it:** the C3y-2 play-test again (hand-off in the session). offline_server.log `JUNE_FX C3y-2 room 4115x` and
  `JUNE_FX C3y-2b scene end: guide resume, dungeon 41150`; ErrorLog no `can't find 'IsRecentlyPlayedEventScene'` / `'PlayEventScene'`.

### C3y-2b - play-test 1 (2026-11-13) and fix 2

Play-test (new Elsword 41150, new Elesis 41220): no input lock, story skip and scene-end hook work (`JUNE_FX C3y-2b scene end: guide resume,
dungeon 41220` x3). Three reports: images missing; the guide shows A / C while the skills sit in A / S; the boss softlocks (it never attacks,
so no awakening orb, no guide 102, boss stays undamageable).
- **Softlock (fixed).** All 11 tutorial boss scripts' `State_Change_to_Attack` reads `slot2:GetNowStateSkillID()` on the user: unbound in
  March (ErrorLog x1114), the condition dies, the boss never attacks. June binds it on CX2GUUser @`31e14b58` (vslot 0x524 thunk `31e4e760`);
  bound to March's `CX2GUUser::GetNowStateSkillID`. Also unbound: `SetLineForce` (the `Tutotial_*_Dp1` direct NPCs), June `31fff2d0` @`31e14ca3`:
  new `CX2GUUser::SetLineForce` (line centre `31c6ad50`, Future dirDegree / land position / position, Now position, both line indices). Both
  under `SERV_IRUHADEV_JUNE_TUTORIAL_LUA_BIND`. C3y-2's gate only covered the 9 dungeon scripts: `luabind.py check` over the 63 data037
  `Tuto*` scripts now has 0 unbound (3 ENUM misses are Rose / Lu-Ciel rows, out of scope).
- **Images (staged).** The tutorial's GFx scenes load (the 135 lines are the kom probe before the loose `data\eventscene\*.swf`). Missing were
  particle / UI textures the census never followed: a particle census of the tutorial scripts, effect sets, `X2PlayGuide.cpp` sequences and
  guide dialogs staged 83 June files (41 new, 42 replaced; backup `kom_backup_stage_june\20261113_133408`): `HQ_WhiteBack.tga`,
  `inform_dungeonGate_signal*.dds` (3), `HQ_KOGLogoWinter2015.tga`, `PerturbationMap.png`, 4 Ain portal files, 72 `DLG_UI_Guide_*` images (41
  replacing March's same-name files), `DLG_UI_Common_Texture03.tga`. Missing in June too: JUNE_BUGS 646 (extended). PACK OK + COPY OK data045
  8c87300b, data048 29161d23, data056 af4783a1, data057 46eaff38, data064 404f081a, data212 6fad0702 (data045 / data064 also carried earlier
  staged entries: ~31 / ~164 changed vs live).
- **A / C prompt (open).** The new characters' two skills sit in slots 0 / 1 (A / S), as the client places learned skills; guide 101 is
  June's GFx movie `timeLineNoTutorial_1` (`FIX_SKILL_TYPE_A`, skill keys allowed). Whether June's movie names the skill-key range (A..C) or
  expects a skill on C is not settled: asked the user.
- **Hygiene.** flagcheck `--worktree` TUTORIAL_LUA_BIND: X2Main.cpp PASS; X2GUUser.cpp FAIL / .h OTHER are a peer's uncommitted
  `JUNE_CHAR_ELESIS_SKILL` hunks (not committed here; see the range line). junediverge `RESULT: PASS`; enumscan 0 A/B.
- **Build.** DEPLOY OK: X2_offline.exe is 26,437,632 B, dated 2026-11-13 13:48:24, md5 f2899679c89d8a4e066018905a035295 (built at main
  1fc6edcc + 20 uncommitted code files, shared with char-elesis CH2b). EXESTR OK 2 of 2 (GetNowStateSkillID, SetLineForce), md5 f2899679.

### C3y-2b - play-test 2 (2026-11-13) and fix 3

Play-test 2 (new Elsword): the boss attacks, guide 102 runs, the tutorial clears to the village (`Quest_11000_End`). Reports: the skill
step points at A and C while the skills sit on A / S; the sub-stage 2 Banthus dialog replays on left click and shows no Banthus portrait; and
(caused by fix 1's adoption of June's `DLG_PVP_Game_Loading_State.lua`) every loading screen went black with only the tip.
- **Loading screens (fixed in code, June's dialog kept).** June's dialog adds a full-screen black picture 3; June's readers handle it and March's
  did not: dungeon loading `324cb020` @`324cc025` (Hero art on Hero dungeons, else hidden; new `CX2DungeonManager::IsHeroHellDungeon` `32931c20`,
  ids `DI_HERO_HELL_DUNGEON_START` / `_END`, `DI_HERO_DUMMY_HELL` registered in `DungeonEnum.h` under `JUNE_ENUM_NAMES`), battle field
  `323b0f20` @`323b122d`, field `32d544e0` @`32d54730`, `EnterTutorial` `324e93b0` @`324e9514` (hide). The tutorial-exit room branch keeps
  June's black (JUNE_BUGS 648). Flag `SERV_IRUHADEV_JUNE_LOADING_HERO_PICTURE`. March's dialog was briefly restored, then June's re-adopted
  (backup `kom_backup_stage_june\20261113_140854`); PACK OK + COPY OK data034 d2b06cfb.
- **A / C skill step (stand-in).** June's `OnFrameMoveInputGuideKey` `31fc5570` watches `GA_SKILL1` / `GA_SKILL4` and the input-guide picture
  shows A and C, so June's starter skills sit on slots 0 and 3; June's server placement is unavailable. `SERV_IRUHADEV_JUNE_STARTER_SKILL_SLOTS`:
  `CX2OfflineSkill::SeedDefaultSkills` fills A, C, S, D. Existing characters keep their slots (create new ones to test).
- **Sub-stage 2 dialog (routed: C3y-2c).** The scenes are SWF-only (`Tutorial_Elsworld_03` / `_04`, no XML rows in March or June, same
  `eventscene` files as June); `tutorial_elsworld_03.swf` holds no Banthus symbol. The play-test logs were overwritten by another session's
  run, so `kickoffs/june-dungeons/C3y-2c.md` re-runs it with a bridge trace.
- **Hygiene.** flagcheck `--worktree`: LOADING_HERO_PICTURE 6 PASS, STARTER_SKILL_SLOTS 1 PASS, JUNE_ENUM_NAMES (DungeonEnum.h) PASS; 0 FAIL,
  0 deleted. junediverge `RESULT: PASS`; enumscan 0 A/B (dungeon ids written by name).
- **Build.** DEPLOY OK: X2_offline.exe is 26,438,144 B, dated 2026-11-13 14:28:46, md5 894df4f9ff44d8bbc704e061d7d8a902 (built at main b29b62bc +
  11 uncommitted code files). EXESTR OK 1 of 1 (wide `HQ_LoadingDungeon_Hero_Hell.tga`), md5 894df4f9.

### C3y-2b - play-test 3 (2026-11-13) and fix 4

Play-test 3 (new Elsword `elstut4`, unit 156; exe md5 13b50b83, a peer build carrying fix 3): the tutorial runs end to end (guide 101, 102,
clear, `Quest_11000_End`), 0 unbound names, no loading-image error line; but the starter skills still sat on slots 0 / 1. Fix 3 had patched
`SeedDefaultSkills`, which this build never reaches at creation: under `SERV_IRUHADEV_JUNE_SKILL_REFORM_AUTOLEARN` the create, class-change
and skill-tree-init paths call `SeedAutoLearn` (its free-slot search). Fix 4 puts the A, C, S, D order into `SeedAutoLearn` (same flag
`SERV_IRUHADEV_JUNE_STARTER_SKILL_SLOTS`); the `SeedDefaultSkills` hunk stays (flag-off AUTOLEARN path). flagcheck `--worktree` 1 PASS (+13),
junediverge PASS. BUILD OK x2.exe 26,439,680 B 14:47:36; DEPLOY OK (deploy-only): X2_offline.exe 26,439,680 B, 2026-11-13 14:48:54, md5 b68f1fcc18893888d6b2fc08ea8854c5. `Tutorial_Elsworld_03` played
14:42:00 and ended forced 14:42:10 (the user's ESC after the click loop): C3y-2c's re-run still needs its trace.

### C3y-2b - play-test 4 (2026-11-13): passed

New Elsword `elstut5` (unit 157, exe md5 b68f1fcc): starter skills on slots 0 / 3 (A / C), the skill step advances, loading screens show
their pictures, the boss attacks and takes damage after the awakening guide, the tutorial clears to the village; ErrorLog has no unbound
name from this phase (`AddGroupChainQuest` predates it). Open: the sub-stage 2 Banthus dialog (C3y-2c), then the C3y-2 review (covers 2b / 2c).

### C3y-2c - trace run (2026-11-13, Opus 5.5 high): the tutorial's GFx scenes, before the fix

Reports (user, C3y-2b play-tests 2 / 3 plus this session): in sub-stage 2 a click on the last line of `Tutorial_Elsworld_03` / `_04` replays the
line instead of closing (C3y-2b play-test 3: `_03` played 14:42:00, ended forced 14:42:10 after the click loop); one left click on a line skips
several lines; after the boss dies a scene shows only black until ESC / right click, then the quest dialog shows; no Banthus portrait.
- **SWF read (AS3 disassembled, `abcdis.py` in the session scratchpad).** Every scene's `EventScene` class: `config` (first ENTER_FRAME) starts the
  typing and `createBinding( "EventScene.nextScene", nextEventScene )`; `nextEventScene`: 1st call shows the whole line, 2nd calls
  `playScene( _sceneNum )`: frame label `lastScene` -> `GameDataFunction.endEventScene()`, else `gotoAndPlay( n + 1 )` and retype
  (`_sceneNum` starts at 1). `clearBindings` on REMOVED_FROM_STAGE deletes both bindings; a stage KEY_DOWN listener (Enter / ESC) is never
  removed. `_03`: 15 frames, `lastScene` on 15; `_04`: 1 frame labelled `lastScene`. `Tutorial_Back.swf` has no script (text GFX_ID_6557 "Close").
- **Ruled out.** The Lua triggers are `ONE_TIME` and March honours it (`CX2DungeonSubStage::ProcessTrigger`); Enter is edge-locked
  (`CKTDIDevice::GetKeyState`); June's frame move `3226c120` = March's; Scaleform compares method closures by object + vtable index, so
  `deleteBinding` matches; `currentFrameLabel` stores and reads 0-based frames.
- **Banthus portrait: June shows none, not a bug.** Neither SWF has a Banthus symbol: `_03` speakers are Berthe (GFX_ID_1034, `T_NPC_Peita_PNG`),
  Elsword, Aisha, Rena (the bandit is named in Rena's 5113); `_04` is Elsword's one line 5176. No fix and no JUNE_BUGS row (its rules exclude a
  design that is merely different).
- **Trace.** `SERV_IRUHADEV_JUNE_GFX_SCENE_TRACE_DEBUG` (temporary, the C3y-2 review retires it), tag `[JUNE_GFX] SCENE TRACE`: clicks / Enter,
  FireNextScene and the observer serials one fire reaches, createBinding / deleteBinding (match or miss), every AS bridge call (id, instance,
  arg), `gameDataEventScene` member types, Play / queue / End with the state, `loadUserWidget` 0x2711 / 0x2712 with whether main.gfx's root
  value is registered (`CKTDGGFXUI::Invoke` does nothing until it is), the scene object's ctor / dtor ("DELETED WHILE PLAYING").
- **Hygiene.** flagcheck `--worktree` (3 files): 1 PASS, 1 PASS~ (a blank line), 1 DEFINE, 0 FAIL, 0 deleted; junediverge `RESULT: PASS`.
- **Build.** DEPLOY OK: X2_offline.exe is 26,462,720 B, dated 2026-11-13 16:27:38, md5 4263c1f75e4cbf50eb87d6ed7576dec4 (built at main 46ba840f + 7
  uncommitted code files, 4 of them a peer's char-elesis work). EXESTR OK 2 of 2 (`[JUNE_GFX] SCENE TRACE `, `TRACE input msg 0x%x`), md5 4263c1f7.
- **Next:** the user's trace run (new Elsword, the whole tutorial), "check logs", then the fix under its own flag in this phase.
