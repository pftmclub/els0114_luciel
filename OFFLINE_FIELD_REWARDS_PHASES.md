# Offline field and dungeon rewards - phased plan (`field-rewards`)

Backlog Stage 23 (`JUNE_PORT_BACKLOG_ORDER.md`, "Stage 23: offline field rewards stand-in"). Planned 2026-11-12 from the brief
`kickoffs/june-backlog/stage23-plan.md`. Plan key `field-rewards`, branch `main`, kickoffs `kickoffs/field-rewards/`.

## Context

The offline emulator pays EXP, ED and items only from the studio's server tables: `DropTable.lua` (`AddMonsterDropInfo`,
`AddBattleFieldNpcDropInfo`, `AddToGroup`) and `StaticDropTable.lua`, read by `X2Lib/Offline/X2OfflineDropTable.cpp`. Those tables are
March 2014's. Every dungeon and field June added after them has no row, so a kill there logs
`GAME -> NOTE monster N has no reward row for key K ... grants nothing` and pays nothing; the 30 % clear bonus is 30 % of 0.

There is no June server, so June's real rows do not exist anywhere. **User decision (2026-11-12):** build a proper drop table for that
content from what the client does ship: `item.kim` (which items exist, their type, grade and level) and each dungeon's entry list
(`BOSS_DROP_ITEM` in `DungeonData.lua`), split into regular-monster drops and boss drops, at fair rates.

This plan does that as a **labelled stand-in**: generated rows, appended to the two server Lua files inside one marked block, switched
by one flag, with every rate copied from the nearest real band of the studio's own table (Sander) and every item taken from June data.

## Rules

1. **Nothing is invented silently.** Every generated row traces to two cited sources: an item pool (item.kim ids, or a dungeon's
   `BOSS_DROP_ITEM`) and a rate row (`tools/data/drop_rate_sheet.tsv`, each line naming the studio row it was copied from). A value with
   no source is a `decision` for the user, written in the sheet as `source = user <date>`.
2. **Rows are generated, never hand-typed.** `tools/droptable.py gen` writes the block; a hand edit inside the block fails `check`.
3. **The studio's rows are never changed.** The block is appended; it never repeats a `(key, MonsterID)` pair or a group id the studio
   file already has (the loader's maps would overwrite it). Group ids come from the reserved range **20000-20999** (free in the server
   file today: its ids jump from 10250 to 28260).
4. **Specific keys only.** A stand-in row is keyed by its own dungeon id + difficulty or its own field id, never the wildcard key 0.
5. **No new `.lua` file** (CLAUDE.md). The rows live in the existing `KncWX2Server/ServerResource/US/DropTable.lua` and
   `StaticDropTable.lua`, both UTF-8 with BOM and CRLF: keep that byte-exact, ASCII only inside the block.
6. **Server Lua is the user's to pack** (CLAUDE.md server-Lua rule). A phase names the two files in its hand-off and proves the load from
   the log once they are packed; it never writes a loose copy into the game directory.
7. **Flag-off means the studio's table exactly.** The block is wrapped in `if DROP_STANDIN then ... end`; only the flagged client
   publishes that global, so commenting out the define removes every stand-in row with no repack.
8. **Rates are pre-boost.** `SERV_IRUHADEV_OFFLINE_DROP_BOOST` repeats the draw for every row, studio or stand-in. No stand-in rate is
   raised or lowered to allow for it.
9. Standing rules apply unchanged: one branch `main`, explicit pathspecs, no hardcoded enum literals in code (ids in a `.tsv` are data),
   no `June` in names, one-line comments, live DB over `DataBase/`.

## Starting context

- **Loader:** `X2Lib/Offline/X2OfflineDropTable.cpp`: `ReadDropBlock` (:296, fields `DungeonID` / `BattleFieldID`, `MonsterID`, `Exp`,
  `ED`, `EDProperty`, `DropItemList`, `DropGroupList`), `ReadStaticBlock` (:447), `AddToGroup_LUA` (:532), `Decide` (:550, one weighted
  pick over the accumulated list, roll 0.00-99.99), `GetNpcItemDrop` (:581, specific key then key 0), `GetNpcReward` (:743).
  `Exp` is a gate: above 0 the kill pays `NpcExpTable[npc level]` (rows 81-99 exist since cap 99). ED pays `10 + ED * npc level`.
  Henir and extra-stage rows are read and discarded on purpose (:426, :437).
- **Consumer:** `X2Lib/Offline/Handlers_Room.cpp` `Handler_EGS_NPC_UNIT_DIE_REQ` (~:3200-3360, key = dungeon id + difficulty, or the
  field id), item drop ~:3628-3680, static drop ~:3869. Existing log lines: `DROP     monster %d dropped item %d`, `DROP     static
  drop for key %d`, `GAME     -> NOTE monster %d has no reward row for key %d`.
- **Tools:** `tools/kim.py` (item.kim), `tools/docsection.py`, `tools/logs.py`, `tools/build.py --deploy`, `tools/flagcheck.py`,
  `tools/junediverge.py`, `tools/exestr.py`. Dungeon scripts are walked by `june2017_re/dungeon_npc_dump.lua` and the registry by
  `june2017_re/dungeon_registry_dump.lua`, both run through `elstools/vendor/luajit/luajit.exe` (see `dungeon_npc_census.py`,
  `dungeon_registry_census.py` for how). Never hand-type msbuild; never read this doc in full, pull sections with `docsection.py`.
- **Data:** live registry `kom_contents/data035_kom/DungeonData.lua` (BOM source, core list) and `data036_kom/DungeonEnum.lua`,
  `BattleFieldData.lua`, `Enum.lua`; field monster tables `june_src_fixed/data036_kom/<Region>_Field_0N.src.lua`; June dungeon scripts
  `june_src_fixed/data035_kom/Dungeon_*.src.lua`.
- **New in this plan:** `tools/droptable.py` (`census`, `pools`, `gen`, `check`, `simulate`), `tools/data/drop_census.tsv`,
  `tools/data/drop_item_pools.tsv`, `tools/data/drop_rate_sheet.tsv`.
- **Flag family:** `SERV_IRUHADEV_OFFLINE_DROP_STANDIN` (+ `_DEBUG`). **Log tag:** `[DROP_STANDIN]` (`offline_server.log`).

## Measurement (planning session, 2026-11-12)

**Server tables.** `DropTable.lua`: 2,127 `AddMonsterDropInfo` rows over 162 dungeon keys, 397 `AddBattleFieldNpcDropInfo` rows over
fields 40000-40025 plus key 0, 495 groups (max id 30080). `StaticDropTable.lua`: 144 dungeon keys, fields 40000-40025.

**Dungeons.** The live client registers 272 dungeon ids; 153 have server rows, 119 have none:

| Group | Keys | NPC level | Count | In scope |
|---|---|---|---|---|
| Lanox: Burned Forest, Ashes Village, Geyser, Gateway, Fire Temple, Final Hall | 33270-33322 | 70-76 | 18 | R3 |
| Elysion: Atlas Development, Atlas City, Tower | 33330-33352 | 80-84 | 9 | R4 |
| Elysion: Sky Crossroads, Adrian, Soless, Memory | 33870-33902 | 83-89 | 12 | R4 |
| Secret: Hamel common / hell, Ranox hell | 33260, 33261, 41271 | 70-99 | 3 | R5 |
| Henir | 38100 | 99 | 1 | no: the loader discards Henir rows by design (R1 decision 4) |
| Defence | 38300-38302 | - | 3 | no (R1 decision 4) |
| Retired secret N/H/E (Wally Lab, Besma, Altera), El Forest Gate, arcade, events | - | - | about 23 | no: retired or event content |
| Training, tutorial, test | - | 0 | about 50 | no: no reward by design |

Key 41270 (`UNUSED_DUNGEON_ID.DI_RANOX_SECRET_COMMON`) has 5 server rows; R0 checks whether they are that dungeon's monsters or a
March dungeon that owned the id.

**Heroic dungeons.** June registers 191 `DI_*_HERO_*` rows (53 dungeons, ids from 100000, NPC level 85-87). The live client registers
none of them, so there is nothing to key rows on yet (R7 waits for the dungeons).

**Fields.** No row for Lanox `VMI_BATTLE_FIELD_RANOX_FIELD_01..04` (40029-40032) or Elysion `..._ELYSION_FIELD_01..03` (40035-40037);
40038 is an enum value with no field row. Monsters, from the field tables:

| Field | Monsters (`NUI_` prefix dropped) |
|---|---|
| 40029 | `LOW_MAG_NOM_FD`, `LOW_MAG_STONE_FD`, `LOW_MAG_CHARGER_FD`, `LOW_RAGE_BABYFIG_FD`, `LOW_RAGE_DRYAD_FD`, `LOW_SMITH_*_FD` (5), bosses `LOW_RAGE_FATHERFIG_FD_BOSS`, `LOW_SMITH_ALUMINUM_FD_BOSS` |
| 40030 | `LOW_GEYSER_*_FD` (7), boss `LOW_GEYSER_ALUMINUM_FD_BOSS` |
| 40031 | `LOW_MOUNT_*_FD` (6), boss `LOW_MOUNT_BERSERKER_BOSS` |
| 40032 | `LOW_FINALHALL_*_FD` (5), boss `LOW_FINALHALL_IGNIA_BOSS` |
| 40035 | `ELSION_F_BERMEVA`, `_PIMPIM`, `_PLINGCHU_G`, `_PLINGCHU_R`, `_PYURITA`, `_SHILEDPIM`, `_DEKALPYURITA`, `ELSION_CHEST` |
| 40036 | `ELSION_F_DECEPTOR`, `_DEFENDER`, `_LIBYA`, `_PROTO`, `_PROVER`, `_PUTER`, `_TEMPLAR`, `_HIGHTTEM`, `_NEWDEFENDER` |
| 40037 | `ELYSION_F_HERNA_BOW`, `_FEAR`, `_LANCE`, `_ORB`, `_SHIELD`, bosses `ELYSION_F_BOSS_ORB`, `_BOSS_SHIELD`, `ELYSION_NEW_CHEST` |

The raid field (`Raid_Field_00`, `NUI_FIELD_BOSS_IRON_ELTRION`) is not in this plan: raid rewards are their own system.

**Real data found.**

1. **Boss items: June's own dungeon-entry lists.** Every gap dungeon's `BOSS_DROP_ITEM` names its boss drops (`COMMON` plus one list per
   character): 121 distinct ids across the gap dungeons, all 121 present in item.kim. Lanox: three boss accessories per dungeon
   (`Kenta's`, `Sdeing's`, `Sirena's`, `Ignia's`, `Ifritan's`, `Scar's`, ids 30329-30365) and character weapons (`Crystallized ... of
   Fire`, `... of the Steel Anvil` L72; `Red / Blue Lava Demon` L78). Elysion: accessories 30426-30434, 30563-30576 and weapons `Atlas
   Dicetium` L82, `Elysion's Hernacyd` L86. Secret: materials 133537 / 133535 + set gloves / shoes (common), 154066 `Barrier's Fragment`
   + `Sacrificed Adventurer's` pieces 700009-700014 (hell). Lu/Ciel, Rose and Ain weapons are in the lists.
2. **The studio's shape for exactly this.** Behemoth Heart (33250-33252, the last real dungeon) shows how entry items become rows: the
   final boss (1411) carries groups 2127 / 2128 / 2129 = that dungeon's `BOSS_DROP_ITEM.COMMON` 30272 / 30273 / 30274 at 0.6 / 3 / 0.6
   (Normal) and 1.1 / 5.4 / 1.1 (Expert); stage bosses (1387-1389, 1409) carry two character boss weapons at 0.5 each (0.9 Expert).
3. **Regular-monster gear: the region ladder in item.kim.** Ids `10LL0C0G` (level, character, grade): Sander L64 `Demon Raider`, L68
   `Demon Subjugator` are what Sander's rows drop (groups 181-192). The same ladder continues: L72 `Radiant Fire`, L76 `Exploding
   Flame`, L80 `Steel Demon Soldier`, L82 `Atlas Commander` (ids `10801C2G`), L87 `Elysion Guard's` (`10851C2G`), each in NORMAL /
   Intermediate RARE / Advanced ELITE / Exceptional UNIQUE.
4. **Dungeon sets.** Sander's bosses drop `Fallen Behemoth` / `Demon Guard` (RARE, groups 1160-1174). Counterparts exist: `Hidden Flare`
   L72 (117561..), `Roaring Berserker` L76 (117679..), `Enchanted Diceon` L84 (117744..), `Hernacyd Guard` L89 (117907..).
5. **Consumables and craft materials.** Static rows drop the four common cooking / alchemy materials (99809 `Whole Grain Flour`, 99810
   `Ruve Herb`, 99811 `Aqua`, 99812 `Stella's Powder Mix`) plus one region pair (Sander: 99823 `Crumbly Meat`, 99824 `Cactus Stem`).
   item.kim has no later pair (99825 onward are El Gems), so R1 looks for Lanox / Elysion materials by recipe and name. Boxes (NPC 20,
   29) drop food and potion groups 6002-6008. Stones: group 4005 (Enhancement Stone Lv.6) and 4015 (Magic Stone); Lv.7-9 stones exist.

## Design: the stand-in

**Monster roles** (R0 assigns one per spawned NPC, from the script, never by name alone):

| Role | How R0 tells | Rate template (Normal; studio row it is copied from) |
|---|---|---|
| `mob` | an `NPC_GROUP` entry with none of the marks below | 33250 x 1382: Exp 1, ED 16, EDProperty 100; region NORMAL weapon 0.4, RARE weapon 0.08, NORMAL armour 0.4, RARE top / bottom / gloves / shoes 0.4 each, enhancement stones 0.6, magic stones 0.6 |
| `mob-minor` | summoned or `REWARD_RATE` below 1 | 33250 x 1415: every rate above halved |
| `mob-card` | a mob whose monster card exists in item.kim | `mob` + its card at 0.3 (33250 x 1378) |
| `stage-boss` | `GAGE_BAR` / `MONSTER_GRADE`, not the last stage | 33250 x 1387: ED 17; RARE weapon 0.53, ELITE weapon 1.23, RARE armour piece 0.11 each, ELITE armour piece 2.52 each, stone bundle (group 4090) 20, its character boss weapons 0.5 each, dungeon-set pieces 1 / 1.5 |
| `final-boss` | `HAVE_BOSS_GAGE` / `SHOW_BOSS_NAME` in the last stage, or the `CLEAR_COND` id | 33250 x 1411: the `stage-boss` gear lines; `Ancient Jewelry Box` 113276 at 0.07; entry `COMMON` items at 0.6 / 3 / 0.6; dungeon-set weapons 2 each |
| `box` | NPC 20 / 29 and June's chests | 33250 x 20: food and potion groups 30 / 30 / 20 / 20 |
| `none` | `NO_DROP`, `DROP = FALSE`, gates, traps, cut-scene actors | no row |
| `field-mob` | field table entry with `DROP = TRUE` | 40024 x 5143: 0.28 / 0.06 / 0.28 / 0.28 x4 / 0.6 / 0.6, ED 16 |
| `field-boss` | field `_BOSS` entry | 40024 x 5124: Exp 1, ED 17, no items (R1 decision 3) |

Hard and Expert use the studio's own multipliers, read from 33251 / 33252 against 33250 (Expert boss lines are x1.8).

**Item pools** (R1 fixes each list in `drop_item_pools.tsv`):

| Pool | Lanox | Elysion |
|---|---|---|
| Region gear, by dungeon level | L72 `Radiant Fire` (lv 70-73), L76 `Exploding Flame` (lv 74-76) | L80 `Steel Demon Soldier`, L82 `Atlas Commander`, L87 `Elysion Guard's` (split by level in R1) |
| Dungeon set (bosses) | `Hidden Flare` L72, `Roaring Berserker` L76 | `Enchanted Diceon` L84, `Hernacyd Guard` L89 |
| Boss items | the dungeon's own `BOSS_DROP_ITEM` | the dungeon's own `BOSS_DROP_ITEM` |
| Stones | enhancement + magic stone groups (level: R1 decision 2) | same |
| Static (craft materials) | 99809-99812 + the region pair R1 finds, else Sander's pair | same |

Secret dungeons (R5) copy the rate rows of the secret dungeons the studio table does have (33030 / 33031 and siblings), with each
dungeon's own entry items. Heroic dungeons (R7) have no studio template; their sheet is a decision of that phase.

**Fairness gate.** `droptable.py simulate <key>` computes the expected items per full clear, by pool, from the census spawn counts. A
generated dungeon passes when each pool is within 15 % of the same figure for Behemoth Heart at the same difficulty, and a field when
its per-kill expectation equals Sander Field 03's. The run prints both the one-draw and the `OFFLINE_DROP_BOOST` figures.

**Mechanism.** The generated block in each server file:

```lua
--{{ Iruha : <date> // field-rewards stand-in, generated by tools/droptable.py (OFFLINE_FIELD_REWARDS_PHASES.md) - do not hand-edit
if DROP_STANDIN then
DropTable:AddToGroup( 20000, 10720100, 12.5 )
DropTable:AddMonsterDropInfo
{
	DungeonID = 33270,
	MonsterID = 1500,
	StandIn = 1,
	...
}
end
--}}
```

Under `SERV_IRUHADEV_OFFLINE_DROP_STANDIN` the loader publishes `DROP_STANDIN` as the number 1 before the two files run (the way
`CX2OfflineLuaEnum::Publish` publishes `True`), reads the optional `StandIn` field, and logs one summary line. With the define
commented out the global is nil and Lua skips the block.

## Phase R0 - Census: every spawned monster against the reward tables

**Scope.** Build `tools/droptable.py census` and write `tools/data/drop_census.tsv`: one row per (dungeon key incl. difficulty or field
id) x spawned NPC, with NPC id and name, NPC level, stage / sub-stage, spawn count, role (table above) and the marks that decided it,
and the server-row state (`specific`, `wildcard`, `none`). Covers every dungeon the live registry registers and every field in
`BattleFieldData.lua`. Read-only: no client code, no Lua edit.

**Run with.** Sonnet 5.5 / high.

**Read list.** "Starting context", "Measurement (planning session, 2026-11-12)", "Design: the stand-in".

**Done gate.**
- Totals reproduce the planning numbers: 272 registered dungeons, 153 with rows; fields 40029-40032 and 40035-40037 with none.
- Every NPC in an in-scope key has a role; the `none` list and any `unknown` are listed in the Done note for R1.
- Key 41270's 5 server rows are identified (that dungeon's monsters or not).
- A second sheet section lists NPCs with state `none` inside dungeons that do have rows (R6's input; this is june-dungeons C3z-2b's
  question, answered here).
- Full build skipped: no play-test.

### Phase R0 - Done (2026-11-16)

**What landed.** `python tools/droptable.py census` writes `tools/data/drop_census.tsv` (12,090 rows: 10,882 dungeon, 1,208 field; one per key x
stage x sub-stage x NPC: level, spawn count, role, the marks that decided it, state, `gap`). `census --gaps` prints the second section,
`summary` the in-scope groups from the sheet, `explain <key>` one key. The server tables are read from the staged data036 files (the ones the
loader mounts) and cross-checked against the loose `KncWX2Server/ServerResource/US` copies: same 2,127 monster + 397 field-NPC rows and 144
static keys. Two additive dump modes, old output proven unchanged (md5 of `calls` on 3 files, default `dungeon_npc_dump.lua` on 3 scripts):
`dungeon_npc_dump.lua ... marks` (entry marks, `NPC_REWARD_RATE`) and `lua_registry_dump.lua deep` (table rows; `DropTable:` calls). No
client code, no Lua data edit, no flag. Full build skipped: no play-test. `flagcheck` / `junediverge` / `finish.py`: no client source changed.

**Totals against the plan.**
- Fields: 34 in `BattleFieldData.lua`; no row of their own for 40029-40032 and 40035-40037 (as planned) and 40026 (the raid field, out of scope).
- Dungeons: the live registry holds **282** ids (284 `AddDungeonData` rows, 2 commented out), not 272; **154** have a row under their own key
  (plan 153). The 1 extra with rows is 41270 (registered through `UNUSED_DUNGEON_ID`, the plan's groups leave it out). The 9 further
  ids are all no-reward-by-design content: the gap groups reproduce exactly (Lanox 18, Elysion 9 + 12, Hamel common / hell + Ranox hell 3,
  Henir 1, Defence 3 = 46) and the remainder is 82 against the plan's "about 73" (tests, tutorials, training, retired secrets, events,
  arcade); the Lu/Ciel tutorial 41240 (TU, 2026-11-15) is one of them, the other 8 are not traced.
- **Key 41270's 5 rows are not Ranox's monsters.** They are Halloween event monsters 4014-4018 (`EVENT_GLITER_ZOMBIE`, `_HALLOWEEN_LINKER`,
  `_PUMPKIN_MAGICIAN`, `_PUMPKIN_MIMIC`, `_GANGSHI_PPORU`); `Dungeon_Ranox_Secret_Hell.lua` spawns none of them. 41270 and 41271 both register
  that script (common and hell), so **R5 covers 41270 as well**, and its 5 stale rows stay as they are or are replaced (R5 decides).
- Behemoth Heart 33250 reproduces the plan's template from the marks alone: stage-boss 1387 / 1388 / 1389 / 1409, final-boss 1411
  (`HAVE_BOSS_GAGE` + `SHOW_BOSS_NAME` + `MONSTER_GRADE` + the clear-cond kill target), `CHEST` 20 a box.
- NPC levels: Lanox 70-76, Elysion 80-84 and 83-89, as planned. The four secret dungeons (33260, 33261, 41270, 41271) have no `LEVEL` or `m_NPCLevel` and are
  party-scaled: the sheet gives the band (99) and marks `level=band relative`; R5 settles what that means for the rows.

**Roles** (rows): mob 6568, none 2215, field-mob 989, box 907, mob-minor 582, stage-boss 431, final-boss 245, field-boss 153, **unknown 0**.
In scope, per `summary`: Lanox dungeons 591 rows (18 keys), Lanox fields 89, Elysion dungeons 1,117 (21 keys), Elysion fields 104, secret 499
(4 keys); every one has a role.

**For R1.**
- `none` NPCs (no row wanted) are gates, traps, dummies, scene actors and `NO_DROP` bosses of event cut-scenes: `droptable.py summary`
  lists each with its mark. Boxes in scope are `CHEST` 20 (every dungeon) and `ELSION_CHEST` 2176 (9 Elysion keys, 1 field).
- **`mob-card` is left to R1:** it needs an item.kim lookup (does a card exist for the NPC), not a script mark; R0 gives `mob`. A mob a unit
  script summons never appears in the stage entries, so the plan's "summoned" half of `mob-minor` is not derived (the `REWARD_RATE` below 1 half is).
- Field bosses follow the script, not the `_BOSS` name: 40035 `Scout Dekal Fyuta` 2135 and 40036 `Tube Processing Defender` 2162 / `Onlooker
  High Templar` 2165 sit in `IS_BOSS_MONSTER_GROUP` groups, so they are `field-boss` (the plan's field table listed them as plain mobs).
- `count` is the spawn number per run (a rate-form group counts its largest group; a `SUB_NPC` entry counts its `SUB_NPC_RATE`).
- Scripts carry `NPC_REWARD_RATE` as repeated keys, so only the last row of a sub-stage survives the run; the sheet marks `REWARD_RATE=` from
  what executes. Nothing in the offline path reads it.

**Second section (R6's input; june-dungeons C3z-2b's question).** `gap = R6` marks 676 rows (mob 643, stage-boss 14, mob-minor 11,
final-boss 8; 100 distinct NPC ids) in 61 dungeon keys that have rows for other NPCs: Druid Forest (6 NPCs), Altera air ship / below tunnel /
plain / core, Peita under garden, Velder bridge of hope / gate, Hamel waterway / frozen water temple / hall, Sander dry sander / Garpai rock /
den of Trock / Santilus ship (8) / Behemoth Heart (1), the Elder / Besma / Altera / Velder secret common and hell dungeons, the tree-day event.

**How to see it.** `python tools/droptable.py census` (8 s; same bytes on a re-run), `... summary`, `... census --gaps`.
Next: `/x2-phase field-rewards R1` on Opus 5.5 / high (R2 can run beside it).

## Phase R1 - Item pools and the rate sheet (user approves)

**Scope.** Write `tools/data/drop_item_pools.tsv` (pool, region, level band, item id, name, type, grade, source) with `droptable.py
pools`, and `tools/data/drop_rate_sheet.tsv` (role x pool x difficulty -> probability, `source` = the studio row). Read the Hard and
Expert rows (33251, 33252), the Sander dungeon static rows and the secret-dungeon rows to fill the multipliers. Find the Lanox /
Elysion craft materials and monster cards in item.kim by recipe and name. Run `simulate` on one Lanox and one Elysion dungeon.

**Run with.** Opus 5.5 / high. Then the phase stops in state `decision`.

**Read list.** "Rules", "Measurement (planning session, 2026-11-12)", "Design: the stand-in"; R0's Done note.

**Decisions put to the user, each with a default:**
1. Boss weapons: class-blind like the studio's rows (default, no code), or only the player's own character (an emulator filter under
   its own flag; a solo player otherwise sees mostly other characters' weapons).
2. Enhancement-stone level for Lanox and for Elysion, if item.kim / `EnchantItemInfo` give no level rule (default: what the data says;
   otherwise the user picks).
3. Field bosses: EXP and ED only, as Sander's (default), or the nearest dungeon `stage-boss` gear lines.
4. Henir and the Defence dungeon stay out (default).

**Done gate.** Both sheets committed; every pool item resolves in item.kim; every rate line has a `source`; the `simulate` figures and
the four decisions are in the Done note and the hand-off; STATUS row `decision`. Full build skipped: no play-test.

### Phase R1 - Done (2026-11-16)

**What landed.** `tools/droptable.py pools` / `rates` / `simulate` and the two `gen` inputs, written straight in R2's columns (no adapter: `gen --only lanox
--dry-run` reads them: 174 dungeon rows over 18 keys, 33 field rows, 22 static rows, 28 groups, no row over 100 %; Elysion 238 / 25 / 24 / 41). No client
code, no Lua, no flag; full build skipped: no play-test. `finish.py --files tools/droptable.py --no-flag ...`: `FINISH RESULT: PASS (4 gates, 0 failed, 4 skipped)`.
- `tools/data/drop_item_pools.tsv`, 639 rows / 447 items, every one usable in the staged item.kim (no fashion, cash, PC-bang, "Cannot use" or event text; 0 left
  out). Region gear: the ladder blocks L72 / L76 (Lanox) and L80 / L82 / L87 (Elysion), 12 classes each, NORMAL / RARE / ELITE; band = the tier nearest the
  key's NPC level, a tie up (Lanox 0-73 / 74-99, Elysion 0-80 / 81-84 / 85-99). Sets: Hidden Flare 3900 + Roaring Berserker 4015, Enchanted Diceon 4521 +
  Hernacyd Guard 5212 (12 weapons each). Boss items: each key's `BOSS_DROP_ITEM` (COMMON by position, the class lists as `boss-class`), secret keys too.
  Cards: 25 in-scope NPCs whose templet NAME names a `Monster Card` (`Volcanic Flare` 78948 matches no NPC). Static: 99809-99812 + Sander's 99823 / 99824
  (item.kim has no Lanox / Elysion "Cooking Good"; `Star Fruit` 700018 is not one) + `Gorgeous Jeweled Box` 111069; BH's `Demon's Runic Core` 117303 is
  left out ("cannot be used anymore").
- `tools/data/drop_rate_sheet.tsv`, 577 lines; every value read from the row its `source` names (`rates --check`: YES). Templates: `mob` 33250-33252 x 1382,
  `mob-minor` x 1415, `mob-card` x 1378 (0.3), `stage-boss` x 1387, `final-boss` x 1411, `box` x 20, `field-mob` 40024 x 5143, `field-boss` x 5124, field box
  40024 x 20, statics 33250-33252 / 40024. Hard / Expert are the studio's own rows (boss lines x1.5 / x1.8, 0.6 -> 1.1 on Expert; gear and stones unchanged).
  Unequal-weight studio groups (4005, 4015, 4090, 6002, 6004) are written as one `item` line per member at outer x weight / 100 (same odds: one lottery,
  one case); equal ones stay `group` lines. Stones: Lv.6 -> the tier's level from the stones' own item.kim text (Lv.7 71-81, Lv.8 81-90).
- Placement differences from BH, on purpose: the class weapons (BH: 2 per stage boss at 0.5) are one `boss-class` line on the final boss (same total per
  clear; a role line reaches every NPC of the role); BH's 1409 set weapons at 0.5 are not copied (no role tells 1409 apart); both sets' bottoms / shoes ride one
  `set-bottom` / `set-shoes` line each. Secret dungeons: `ref` lines (the Velder secret rows 33190 / 33191 x 459, 460, 944 / 980, 948 / 984), which `gen` ignores;
  R5 turns them into lines with each dungeon's entry items.

**Decisions (user 2026-11-16, `decision` lines in the sheet).** 1 boss weapons class-blind (no code). 2 stone level: the data answers it, no choice asked.
3 field bosses EXP + ED only. 4 Henir / Defence stay out (default kept, not asked). Asked in addition: 5 both region sets on every boss (as BH's two sets);
6 set tops / gloves added on stage bosses at Karuso Village's 33230-33232 x 1215 values (0.6 / 1.0, Hard 1.0 / 1.5, Expert 1.2 / 1.8; source `user 2026-11-16`).

**Simulate.** The plan's per-clear 15 % gate measures dungeon size and boss count, not rates (BH has 6 boss kills a clear, Burned Forest Normal 1), so the gate
is per role: items per kill of each role against the same role in BH (same difficulty) or Sander Field 03; per-clear figures are printed beside it.
| key | mob | mob-minor | stage-boss | final-boss | box | per clear, one draw (x3 boost) |
|---|---|---|---|---|---|---|
| 33270 Burned Forest N | 0.99 | 1.16 | - | 1.00 | 1.00 | gear 2.75 (8.25), stone 1.47, card 0.23, set 0.04, boss 0.04 |
| 33272 Burned Forest E | 0.99 | 1.16 | 1.17 | 1.13 | 1.00 | gear 2.87 (8.62), stone 1.67, set 0.22, boss 0.14 |
| 33880 Adrian N | 0.95 | 1.00 | 1.09 | 1.00 | - | gear 1.62 (4.87), stone 1.07, card 0.02, set 0.12 |
| 33882 Adrian E | 0.95 | 1.00 | 1.17 | 1.08 | - | gear 1.62, stone 1.07, set 0.22, boss 0.11 |
| 40029 Lanox field 1 | field-mob 1.00, field-boss 0 = 0 | | | | - | gear 0.64 (1.93), stone 0.44 |
| 40035 Elysion field 1 | field-mob 1.00, field-boss 0 = 0 | | | | 1.00 | gear 0.80 (2.40), stone 0.55 |
All 46 Lanox / Elysion keys: 168 role checks pass; the over-15 % ones are explained: Expert stage bosses 1.17 = decision 6's tops / gloves; `mob-minor` 1.16
(6 keys, Burned Forest / Ashes Village) = `gen` adds the full 0.3 `mob-card` line to a carded minor, while BH's halved rows carry no card.

**For R3 / R5.** CHEST 20 already has the studio wildcard row 0 x 20 (33 / 33 / 34); `gen` gives it a specific row in BH's shape (30 / 30 / 20 / 20), ELSION_CHEST
2176 has none today. Census `mob` NPCs with an internal name (dummies / triggers, up to 8 spawns a key, Elysion mostly) get rows; harmless if never killed.
NPC 29 `CHEST_MONSTER` in the secret dungeons is census `mob`, but the studio treats it as a chest (0 x 29 = food 100 %): R5 leaves it to the wildcard.

## Phase R2 - Loader flag and generator

**Scope.**
- `KTDXLIB/Always.h`: `SERV_IRUHADEV_OFFLINE_DROP_STANDIN` and `SERV_IRUHADEV_OFFLINE_DROP_STANDIN_DEBUG`.
- `X2OfflineDropTable.cpp/.h`: publish `DROP_STANDIN` before the drop scripts run; read `StandIn` in `ReadDropBlock` / `ReadStaticBlock`
  and keep it on the row; count stand-in rows; one line
  `[DROP_STANDIN] loaded: %d dungeon row(s) over %d key(s), %d field row(s) over %d field(s), %d static row(s), %d group(s)`; when the
  count is 0, `[DROP_STANDIN] no stand-in rows - pack KncWX2Server/ServerResource/US/DropTable.lua and StaticDropTable.lua into
  data036.kom`.
- `Handlers_Room.cpp` under `_DEBUG`: a throttled per-kill line `[DROP_STANDIN] key %d npc %d -> exp %d ed %d item(s) %d` when the row
  is a stand-in.
- `tools/droptable.py gen` (census + pools + sheet -> the block, CRLF, ASCII, deterministic order, `--dry-run`, `--only <region>`) and
  `check` (block equals a fresh `gen`; no pair or group id shared with the studio rows; group ids inside 20000-20999; every item in
  item.kim; BOM and CRLF intact; outer probabilities of a row sum to 100 or less).

**Run with.** Sonnet 5.5 / high; review Opus 5.5 / medium (`R2-review.md`).

**Read list.** "Rules", "Starting context", "Design: the stand-in".

**Done gate.** `flagcheck` PASS for both flags, `junediverge --check` PASS, one-file probe build with 0 errors; `gen --dry-run` prints
row counts and `check` passes on the dry output; no row is written to the server Lua yet. Full build skipped: no play-test.

### Phase R3 - Done (2026-11-16)

**What landed.** `droptable.py gen --only lanox`: 174 dungeon rows (18 keys), 33 field rows (4 fields), 22 static rows, 28 groups, written into the marked block of
`KncWX2Server/ServerResource/US/DropTable.lua` and `StaticDropTable.lua` (BOM + CRLF intact, +2,798 insertions, 0 deletions). `check`: rows 207, static 22, groups 28, PASS. No client code changed, no new flag.

**Fairness gate (`simulate`, 18 keys + 4 fields).** `mob` 0.99, `final-boss` 1.00-1.13, `box` 1.00, fields `field-mob` 1.00 (all four = Sander Field 03). 12 role rows exceed 15 %, all as R1's note explains:
`mob-minor` 1.16 (6 keys: Burned Forest / Ashes Village; `gen` adds the 0.3 card line) and Expert / Hard `stage-boss` 1.15-1.17 (decision 6 tops / gloves).

**Pack.** `stage-server` DropTable.lua md5 134ccb9d (928,406 B), StaticDropTable.lua md5 00fb42b5 (43,683 B); `PACK OK data036` (273 entries, 2 changed); `COPY OK data036` md5 76f3fee5e8a15fd286845e1a731a8e8e.
**Build.** `DEPLOY OK: X2_offline.exe ... md5 b525d2f18f70b977cf430339d5627662` (coalesced into a peer's full build). `EXESTR OK: 1 of 1 found` for `[DROP_STANDIN]`.
**Gates.** `finish.py --no-flag "data-only"`: PASS (no client file changed).

**How to see it.** Log tag `DROP_STANDIN`: expect `loaded:` 174 / 33 / 22 / 28 (per the loader's wording), per-kill lines, and no `no reward row` for keys 33270-33322 / 40029-40032.
**Play-test PASSED 2026-11-16 (user accepted the one-kill result).** `loaded:` 174 / 33 / 22 / 28; field 40029 mobs EXP 12591 / ED 2712; 33272 mobs EXP 9111 / ED 3486; boss 1686 EXP 109332 / ED 44424, items 130047 + 70058 + 99811; `no reward row` empty. No Kenta entry-list item on the one kill (0.6-3 %), accepted; fields 40030-40032 not walked. The debug define stays on until the R6 review.

### Phase R4 - Done (2026-11-16)

**What landed.** `gen` for Lanox + Elysion together (`--keys` over both regions: `--only elysion` alone rewrites the block and drops R3's rows, so every later rows phase regenerates the union). Elysion adds 238 dungeon rows (21 keys), 25 field rows (3 fields), 24 static rows, 39 groups; the block now holds 412 / 39 keys, 58 / 7 fields, 46 static, 67 groups. BOM + CRLF intact, insertions only. `droptable.py check` PASS (rows 470, static 46, groups 67). Tool fix: `block_scope` returned an unhashable set, so `check` crashed on a `--keys` block (now a frozenset; `tools/droptable.py`, one line).

**Fairness gate (`simulate`, 21 keys + 3 fields).** `mob` 0.94-1.00, `mob-minor` 1.00, `final-boss` 1.00-1.06, `box` 1.00, fields `field-mob` 1.00 (= Sander Field 03). Only Expert `stage-boss` is above 15 %: 1.17 on 33332 / 33342 / 33352 / 33872 / 33882 / 33892 / 33902 (R1 decision 6, tops / gloves, as R3). 35 further keys in the scope list have no census spawns (no dungeon rows).

**Pack.** `stage-server` DropTable.lua md5 a470fcfb (1,049,706 B), StaticDropTable.lua md5 279a740c (49,188 B); `PACK OK data036` (2 changed); `COPY OK data036` md5 61e982139fac9600d541cf00bdfd2780.
**Build.** Full build skipped: no code change; deployed exe is R3's (`DEPLOY OK` md5 b525d2f1, `EXESTR OK` for `[DROP_STANDIN]` from R3). Gates: `finish.py --no-flag "data-only"`.

**How to see it.** Log: `[DROP_STANDIN] loaded: 412 dungeon row(s) over 39 key(s), 58 field row(s) over 7 field(s), 46 static row(s), 67 group(s)`; per-kill lines; no `no reward row` for 33330-33352 / 33870-33902 / 40035-40037.
**Play-test:** pending.

### Phase R2 - Done (2026-11-16)

**What landed.** Flags `SERV_IRUHADEV_OFFLINE_DROP_STANDIN` and `_DEBUG` (end of `Always.h`; `_DEBUG` is on for R3's play-test and retires in
the R6 review). `X2OfflineDropTable.cpp/.h`: `EnsureLoaded` publishes `DROP_STANDIN` as the number 1 before `DropTable.lua` runs; `ReadDropBlock` /
`ReadStaticBlock` read the optional `StandIn` int; the row keeps `m_bStandIn`; counters per dungeon row / key, field row / field, static row, and
groups counted by id in 20000-20999 (named namespace `_CONST_DROPTABLE_STANDIN_R2_`); `IsStandInRow` for the handler. The summary line and the "no
stand-in rows - pack ..." line are as the phase section words them. `Handlers_Room.cpp`: under `_DEBUG` one `[DROP_STANDIN] key %d npc %d -> exp %d
ed %d item(s) %d` line per kill from a stand-in row, throttled to one per second per NPC id (item count = what the monster row drew, handed from
`PushNpcDrop` through a file-local counter). `tools/droptable.py gen` and `check`. No server Lua written, no `.lua` created, no pack.

**Generator contract (R1 writes these two files to this shape; `gen_blocks` has the column text).**
- `drop_item_pools.tsv`: `pool region band key npc item_id name type grade source` (band `lo-hi` of the dungeon's level or `*`; key = dungeon /
  field id, difficulty-less base id or exact key, or `*`; npc = NPC id or `*`, for monster cards).
- `drop_rate_sheet.tsv`: `kind region role diff pool exp ed ed_property prob inner source`; kind `param` (row's Exp / ED / EDProperty per region x role x
  diff; **no param line = no row for that role**, printed as a WARNING with the NPC count), `item` (each pool item in `DropItemList` at `prob`), `group`
  (the pool becomes a group, `inner` each or 100 / n, named in `DropGroupList` at `prob`), `static` (pool items in the key's static row at `prob`).
  Role `mob-card` adds to a `mob` / `mob-minor` whose pool rows name the NPC. Probabilities are written exactly as the sheet spells them.
- One row per (key, NPC); a NPC with several census roles takes the first of final-boss, stage-boss, field-boss, mob, field-mob, mob-minor, box
  (18 such NPCs in scope); `none` / `unknown` roles never get a row. A (key, NPC) pair or static key the studio rows already hold is skipped
  (read from the studio text of the loose server files with the block cut out, never from the census `state`, so a re-gen after packing is stable).
- Groups are deduplicated by content and numbered from 20000; every `AddToGroup` precedes the rows that name it. `--only lanox|elysion|secret|all`,
  `--keys`, `--dry-run`, `--out-dir DIR` (writes the two resulting files there, server files untouched). The block header line records the scope,
  so `check` regenerates exactly it. Without `--dry-run` the block is replaced or appended with BOM and CRLF kept; an empty result removes it.

**Proof.**
- Real pools and sheet do not exist yet (R1), so `gen --dry-run` on the real inputs prints 0 rows and the note. The logic is proved on a throwaway fixture
  outside the repo (7 pool rows, 13 sheet lines, real census, `--only lanox`): `GEN scope=lanox: 156 dungeon row(s) over 18 key(s), 33 field row(s) over
  4 field(s), 22 static row(s), 2 group(s)`; `check` on its `--out-dir` files: `CHECK rows 189, static rows 22, groups in block 2 ... RESULT: PASS`.
- `check` fails on each of: a hand edit of one row, a group id outside 20000-20999, a missing BOM, three bare LF lines, a studio pair repeated, a
  non-ASCII byte in the block, an item not in item.kim, outer probabilities over 100 (8 of 8 negatives). Write path on copies: gen, regen (same md5),
  gen with empty inputs (md5 equals the original studio file). The real `DropTable.lua` / `StaticDropTable.lua` are untouched (same md5).
- Lua: the fixture output run in LuaJIT against a counting stub: `DROP_STANDIN` unset gives the studio counts exactly (2127 monster, 397 field, 1623
  groups, 144 + 26 static); set adds 156 + 33 rows, 22 static, 4 `AddToGroup` and every added row carries `StandIn = 1`. That is the flag-off proof for rule 7.
- Flags are defined at the end of `Always.h` (a `cl /Zs` probe including `stdafx.h` with `#ifndef` -> `#error` for OFFLINE, DROP_STANDIN and `_DEBUG`: 0 errors).

**Hygiene.**
- `finish.py --files <4 files> --with ...DROP_STANDIN ...DROP_STANDIN_DEBUG`: `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`; flagcheck PASS (3 files
  PASS~ blank / marker only, `Always.h` OTHER = peers' defines), `junediverge --check` `RESULT: PASS`, enumscan exit 0. `flagcheck ..._DEBUG --worktree` PASS (0 deleted lines).
  `flagcheck ...DROP_STANDIN` alone flags the `_DEBUG` guard `#if` as an ERROR; that is the pair, run as `--with` it passes. stateends / skillicons / luabind not applicable.
- One-file probe build (`cl /Zs`, same defines as the build): `X2OfflineDropTable.cpp` 0 errors. `Handlers_Room.cpp` shows 3 errors, all `X2Unit.h(1446)`
  `CX2SocketItem` (june-stat-calc S2's uncommitted `IsShowDungeonOnly` declaration, a peer's edit, flag `JUNE_STATCALC_EQUIP_STAT`); the same 3 appear on the
  untouched `git show HEAD:` copy and none points at a line of this phase. Not mine to fix (shared-builds rule 4): R3's full build needs that peer fixed.
- Full build skipped: no play-test. No `exestr` line (no build).

**How to see it.** Nothing in game yet: with no stand-in rows packed the load log gains `[DROP_STANDIN] no stand-in rows - pack ...`; R3 writes the rows.
`python tools/droptable.py gen --dry-run`, `... check`.

**For R1 / R3 (open: the two formats differ).** R1 was editing `tools/droptable.py` in the same tree while R2 ran (uncommitted `pools` / `rates` / `simulate`,
not part of this commit). Its columns are not the contract above: pools `pool region band item_id name type grade level unit_class weight source` (no `key` / `npc`
columns, `region` is a region or a dungeon / NPC id, `weight` = inner probability), rates `role pool diff prob per source` (`per` = row / item / class-item / set /
card; Exp / ED / EDProperty are lines of pool `exp` / `ed` / `edproperty`). `gen` reads the files named by `--pools` / `--sheet` and stops with `lacks column(s)` on
any other shape, so nothing is misread. Whoever commits second reconciles in `gen_inputs` / `gen_blocks` (one adapter, the group / tier / card rules R1 settles are
not guessed here); R3 cannot run `gen --only lanox` before that, and the sheet needs `static` lines. Review: `kickoffs/field-rewards/R2-review.md` (Opus 5.5 / medium).

### Phase R2 -- Review (2026-11-16)

**Verdict: pass, no code change.** Commit reviewed: `c3b7e545`.
- **Flag-off = the studio's table.** Every client hunk is insert-only under `#ifdef SERV_IRUHADEV_OFFLINE_DROP_STANDIN` (`X2OfflineDropTable.cpp/.h`: the
  global, the two `StandIn` reads, `m_bStandIn`, the counters, the summary line, `IsStandInRow`) or `_DEBUG` (`Handlers_Room.cpp`: the namespace, the counter
  reset / set, the kill line). With the define commented out nothing is published, no field is read, no line is written. The Lua side of rule 7 (nil global
  skips the block, studio counts exact) is R2's LuaJIT stub proof; nothing in the client changes it.
- **`DROP_STANDIN`** is `lua_pushnumber( 1 )` + `lua_setglobal`, after the binder `decl` and before `RunScript( SCRIPT_NPC_EXP / DROP_TABLE )` and the later
  `SCRIPT_STATIC_DROP`: one state, published before both files.
- **Counters.** `StandIn` is read in `ReadDropBlock` and `ReadStaticBlock`; both stand-in counters sit after the real `++m_iMonsterRows` / `++m_iStaticRows`, so
  every early return (bad key, no reward of any kind, disabled block) counts nothing. Static rows are last-wins in the loader (`mapTarget[ iKey ] =`); a stand-in key
  equal to a studio key is what `check` refuses, so "a static row that loses to a studio key" cannot reach the loader. Groups are counted by id in 20000-20999 only.
  `IsStandInRow` looks up the same specific-then-0 order as `GetNpcReward`. The kill line's throttle is one per second per NPC id; the item count is reset before
  `PushNpcDrop` and set after `GetNpcItemDrop`.
- **Generator vs Rules 1-8.** Read in `gen_blocks` / `cmd_check`: studio pairs and static keys are taken from the server text with the block cut out and skipped,
  group ids numbered from 20000 and checked inside 20000-20999, block ASCII-checked, BOM and CRLF kept, no `.lua` created, a role with no `param` line is a
  WARNING with no row.
- **R1's files vs `gen`: closed, no adapter.** R1 (done, not yet committed at review time) wrote `drop_item_pools.tsv` (639 rows) and `drop_rate_sheet.tsv` (577
  lines) in `gen`'s own columns. Re-run on them: `gen --only lanox --dry-run --out-dir <scratch>` -> `174 dungeon row(s) over 18 key(s), 33 field row(s) over 4
  field(s), 22 static row(s), 28 group(s)`, studio pairs left alone 0, mixed-role NPCs 6; `check` on that output `CHECK rows 207, static rows 22, groups in block 28
  ... RESULT: PASS`; negative (one group id 20000 -> 21000): `RESULT: FAIL (2)` (block differs, id outside range). Server files untouched.
- **Swept-in peer hunk (finding, no action).** `c3b7e545` carries june-stat-calc S2's `#ifdef SERV_IRUHADEV_JUNE_STATCALC_EQUIP_STAT` hunk in `Handlers_Room.cpp`
  (`MakeRoomUserInfo`, the 4-argument `AddEquippedWeaponRange`). At `c3b7e545` that flag is not defined, so the committed tree takes the `#else` (March's 3-argument
  call) and compiles; S2's commit (`Always.h` define + `X2OfflineInventory.*`) completes it. S2's range gate will not see that hunk: its review reads it here.

**Hygiene.** `finish.py --review --commits c3b7e545 --with ...DROP_STANDIN ...DROP_STANDIN_DEBUG -v`: flagcheck `OFFLINE_DROP_STANDIN` PASS (3 PASS~ blank / marker
only, 1 DEFINE, 0 OTHER, 0 deleted lines, 0 non-ASCII, 0 EOL mismatch); `junediverge --check` `RESULT: PASS`; enumscan exit 0; INFO line = S2's / the enclosing
`OFFLINE` / `OFFLINE_MONSTER_GRADE` flags. `junediverge --check-all` `RESULT: FAIL, 1 hit`: `X2Lib/X2Item.cpp:272 IsSocketStatInGame GT_PVP`, a peer's uncommitted
june-stat-calc S2 edit (not in `HEAD`), not this phase's; S2 owns it. `flagcheck ..._DEBUG --range c3b7e545^..c3b7e545` `RESULT: PASS` (4 OTHER = the
`DROP_STANDIN` / `OFFLINE` lines of the same commit, 0 deleted). stateends / skillicons / luabind not applicable. Build skipped: no code change, no play-test.

**R3 kickoff amended:** the server-Lua rule of 2026-11-15 (R3 stages, packs and copies `DropTable.lua` / `StaticDropTable.lua` itself) and R1's files committed first.

## Phase R3 - Lanox: six dungeons and four fields

**Scope.** `gen --only lanox`: keys 33270-33322 (18), fields 40029-40032, their static rows and groups. `check` and `simulate` every
key. Build and deploy. Hand the two server Lua files to the user to pack.

**Run with.** Sonnet 5.5 / medium. Reviewed with R4-R6 in `R6-review.md`.

**Read list.** "Rules", "Design: the stand-in", "Play-test hand-off"; R1's and R2's Done notes.

**Done gate.** `check` PASS; fairness gate PASS for all 18 keys and 4 fields (figures in the Done note); `DEPLOY OK`; `EXESTR OK` for
`[DROP_STANDIN]`; after the user's pack, the `[DROP_STANDIN] loaded:` counts equal `gen`'s; play-test: no `no reward row` line for a
Lanox key, EXP and ED rise per kill, a boss item from the entry list drops on a forced run.

## Phase R4 - Elysion: seven dungeons and three fields

**Scope.** `gen --only elysion`: keys 33330-33352 and 33870-33902 (21), fields 40035-40037 (chests take the `box` role), static rows.

**Run with.** Sonnet 5.5 / medium. Reviewed in `R6-review.md`.

**Read list.** As R3, plus R3's Done note.

**Done gate.** As R3, for the Elysion keys.

## Phase R5 - Secret dungeons

**Scope.** Hamel common / hell (33260, 33261) and Ranox hell (41271), plus 41270 if R0 found its rows are not that dungeon's. Rates from
the studio's own secret rows (33030 / 33031 and siblings), items from each dungeon's entry list. Report, without changing, any secret
dungeon whose studio rows name monsters the June script no longer spawns (R6 covers the reverse).

**Run with.** Sonnet 5.5 / high. Reviewed in `R6-review.md`.

**Read list.** "Rules", "Design: the stand-in", "Play-test hand-off"; R1's Done note.

**Done gate.** As R3, for the secret keys.

## Phase R6 - June-only monsters in dungeons that have rows

**Scope.** R0's second sheet: NPCs June added to old dungeons (the 142 of june-dungeons C3z-2, and any others) that have no specific and
no wildcard row. Each gets a row copied from a same-role sibling in the same dungeon and difficulty (source = that row). This phase
replaces june-dungeons C3z-2b.

**Run with.** Sonnet 5.5 / high; review Opus 5.5 / high (`R6-review.md`, covering R3-R6: `check`, `simulate`, `flagcheck --range`,
`junediverge --check-all`, and it retires `_DEBUG`).

**Read list.** "Rules", "Design: the stand-in", "Play-test hand-off"; R0's Done note.

**Done gate.** Census state `none` is 0 for every in-scope key; `check` PASS; one play-test smoke in one old dungeon with a June-only
monster.

## Phase R7 - Heroic dungeons

**Scope.** Blocked until the live registry holds June's `DI_*_HERO_*` rows (june-dungeons; none of the 191 is registered today). Then:
re-run `census`, build the heroic pools from their entry lists (`Heroic Gear Piece` 162392, `Heroic Gear Chest` 162393, the per-dungeon
ids 1868xx / 1885xx), propose a rate sheet (no studio template exists; nearest is secret hell) for the user's yes, `gen --only heroic`.

**Run with.** Opus 5.5 / high.

**Read list.** "Rules", "Measurement (planning session, 2026-11-12)", "Design: the stand-in"; R5's Done note.

**Done gate.** As R3, for the heroic keys, after the user approves the heroic sheet.

## Phase Z - Close

**Scope.** `MODS.md` entry (both flags, the block, how to revert: comment out the define); backlog Stage 23 marked done; the
`droptable.py` row in `tools/README.md`; STATUS block archived. Runs when R6's review is done; R7 stays as one open row if still blocked.

**Run with.** Sonnet 5.5 / medium.

**Read list.** Every Done note of this plan.

**Done gate.** `check` PASS on the committed files; `STATUS_ARCHIVE.md` holds the block. Full build skipped: no play-test.

## Play-test hand-off

Each rows phase (R3-R6) hands over, copy-ready:
1. **Pack first.** The user XOR-encrypts and packs `KncWX2Server/ServerResource/US/DropTable.lua` and `StaticDropTable.lua` into
   `data036.kom`. Until then the log says `[DROP_STANDIN] no stand-in rows`.
2. **Character.** A unit inside the region's level band (Lanox 70-76, Elysion 80-89), made or levelled with `/x2-create-char` /
   `/x2-db`; the hand-off names the unit and the exact command.
3. **Where.** One field and one dungeon of the phase, with the full dev-console line to get there (field warp ids: atlas A6 Done note,
   `FieldWarpByCommand`), `/invin 0`, and the stage command for the boss room.
4. **Look for.** EXP and ED on every kill; items on the floor within a few dozen kills; at the boss, an item from the dungeon's entry
   list (the hand-off lists the ids and names).
5. **If not.** Which log line is missing and what it means.
6. **Log check.** `python tools/logs.py --tag DROP_STANDIN`, plus `--grep "no reward row"` scoped to the phase's keys: the first must
   show the `loaded:` line with `gen`'s counts and per-kill lines, the second must be empty for those keys.
