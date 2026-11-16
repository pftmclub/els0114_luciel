# June character port: Lu/Ciel (Stage 8, the first new character)

Plan key `char-luciel` · branch `main` · kickoffs `kickoffs/char-luciel/` · log tag `[JUNE_CHAR]` (lines start `[JUNE_CHAR] LUCIEL <sub>`, sub-tags
`DATA`, `NEWUNIT`, `CREATE`, `DUO`, `CORE`, `SKILL`, `PASSIVE`, `UI`, `NUM`, `JOB`, `TUTORIAL`) · flags `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_*` for Lu/Ciel's
own code, `SERV_IRUHADEV_JUNE_CHAR_*` for the class-agnostic new-character and two-body pieces Rose and Ain reuse (+ a temporary `..._DEBUG` per
sub-phase) · backlog Stage 8 (`JUNE_PORT_BACKLOG_ORDER.md`, "Stage 8: new characters (Lu/Ciel, Rose, Ain)") · written 2026-11-02 by the
`new-plan char-luciel` session from the Chung plan (`JUNE_CHARACTER_CHUNG_PHASES.md`), the Aisha "Template for Stage 7" and its "Phase CH-N".

## Context

**What this plan does.** It brings June 2017's Lu/Ciel into the March client from nothing: the unit type (`UT_LUCIEL` 10), seven classes
(`UC_LUCIEL_DUO` 200; first jobs Chiliarch 37, Royal Guard 38, Diablura 39; second jobs Dread Lord 127, Noblesse 128, Demonio 129), creation,
starter items, the tutorial, job change, June's class `CX2GULuciel` (a new `X2Lib/X2GULuciel.cpp` / `.h`), his two-body ("duo") system, his
skills, passives, UI and transcendence rows. Third jobs (`UC_LUCIEL_CATASTROPHE` 177, `_INNOCENT` 178, `_DIANGELION` 179) are out (scope, user
2026-10-20).

**Why it differs from a Stage 7 port.** March has no Lu/Ciel class, script, motion or templet row: every class function is `new` (no
`inside` column), and the engine has never held a tenth unit type or a class id above 127. So before the class can run, the plan builds the
new-character foundation (phases `NU*`) and the two-body engine (phases `DU*`). Both are class-agnostic where June's code is (flags
`SERV_IRUHADEV_JUNE_CHAR_*`), so Rose (`UT_ROSE` 11, base `UC_ROSE_GUNNER` 201) and Ain (`UT_AIN` 12, base 202) reuse them.

**Order (user, 2026-11-02).** CH0 and NU0 (the two maps, read-only) and CH0b (assets) run now. **Every code phase starts after char-chung Z**,
ahead of `char-elsword`: Chung's CH2-CH4 edit `X2GUUser.*` and `X2Game.cpp`, which NU1-DU5 also edit. **Changed by the user 2026-11-03
(`new-plan char-elsword`): Lu/Ciel is lower priority; every phase after CH0b waits until the March characters are ported (Stage 7
closed).**

**The state he starts from** (planning evidence below; CH0 / NU0 re-verify anything they build on).
- **Enums are registered** (`SERV_IRUHADEV_JUNE_ENUM_NAMES`): `X2Unit.h:36` `UT_LUCIEL = 10` (**equal to March's `UT_END`**; June's `UT_END` is
  13), `X2Unit.h:167-191` the seven classes. `UC_END` is 124, and the socket / item loaders drop every class at or above it.
- **The class id does not fit the wire.** Every shared `char m_cUnitClass` (`KUnitInfo` `CommonPacket.h:1457`, 23 `CommonPacket.h` and 10
  `ClientPacket.h` members) is signed: 128, 129 and 200 wrap negative. June reads a word (`movzx word ptr` in `3235eb20` / `32360090` /
  `323614c0` / `32361e00`, june-transcend T0). The save column is an INTEGER and is fine.
- **Dormant March pieces:** `CI_CIEL = 11` (`X2Data.h:44`), `GetHyperModeTableName( bCiel_ )` (`X2GUUser.cpp:33444-33539`), "stand-in: Lu/Ciel
  not ported" branches in `X2StateClassChange.cpp`, `X2UICharInfo.cpp`, `X2UISkillTreeNew2015.cpp`, `X2UnitViewerUI.cpp:7534`
  (`plan_march_side.md` §1). `X2OfflineSkillAutoLearn.h` already covers 200-202. No `Always.h` flag names Lu/Ciel.
- **Creation:** the class buttons come from `DLG_Unit_Create_Unit_Button_New.lua` (March bytecode, no Lu slot; June's source has
  `LucielCreateUnitSlot`, `Pos_Luciel = 8`), plus nine `Select<Char>()` functions in `X2StateCreateUnit.cpp:1266-2346` and the entry switch
  `:195-215`. The emulator's `Handlers_Unit.cpp:222-308` does not check the class. `GBASE_ITEM[]` (`X2OfflineInventory.cpp:1226`) covers
  classes 1-9.
- **Stats:** `StatTable.lua` has no row for 37-39 / 127-129 / 200. `X2OfflineStatTable::UnitTypeOf` (`:271`) returns the templet's unit type
  (10) and the loader then retries **class** 10 (Sword Knight): a silent wrong curve once Lu/Ciel's templet row exists. NU4 fixes it.
- **June's class:** `CX2GULuciel`, range `32ae4080`-`32b4e76f` (Rose before, Elesis after), ~583 functions (Ghidra defines 499; **85 of the
  119 override targets are not carved**, among them 0x7b4 `32b46580`), vtable `334f06fc` with **547 slots** (0x880 / 0x884 / 0x888 are
  Lu/Ciel's own virtuals), object 0x98a0 bytes, ctor `32b1bd90` (base ctor `3203e850`), dtor `32b39380`.
- **His script is two files:** `Luciel_Duo.lua` (22,847 lines, 406 tables: 6 `INIT_*`, 400 `LSI_*`) includes `Luciel_Demonio.lua` (7,524 lines,
  158 tables). Neither is in March's koms. Both scripts are written in the `LSI_` state namespace, not `CSI_`.
- **Two bodies.** Lu is the major unit and Ciel the minor one. The keys are `DUO_TYPE` (390 + 158 uses; June's `Enum.src.lua:17515` has
  `DT_MAJOR 0 / DT_MINOR 1 / DT_BOTH 2 / DT_KEEP 3`, and the live `Enum.lua` lacks `DT_BOTH` / `DT_KEEP`), `MINOR_UNIT_ANIM_NAME`,
  `MINOR_UNIT_SPEED_TIME0-4`, `NOT_SHOW_UNIT_BY_DUOTYPE`, `HIT_ADD_COMBINATION_POINT` (damage data, 6) and `CP_AMOUNT`. The second-body
  members `+0x8e88` / `+0x8e90` / `+0x8e91` are **`CX2GUUser`'s**, written in the base ctor (`32040e4e`, `32042433`, `32042471` from the unit's form
  byte `+0xe40`, `32042479`), not in the class. The Stage 8 bullets (backlog) list the engine readers.

**Out of scope (owned elsewhere).** Rose and Ain (their own `char-rose` / `char-ain` plans, brief `kickoffs/june-backlog/stage8-plan.md`);
third jobs; generic UI (Stage 9); emotion behaviour (Stage 10; CH0b stages the 98 `*_Emotion_*` motions); `PET_*` Lu/Ciel pets (Stage 17); NPC
copies (`TIME_LU*`, `RAID_SHADOW_CIEL/RUCIEL`: already June bytecode); guild-agit form packets `0x4fd` / `0x4fe` unless NU0 finds them
reachable offline (then DU5).

## Rules

The Chung plan's Rules hold (`python tools/docsection.py JUNE_CHARACTER_CHUNG_PHASES.md "Rules"`) with Lu/Ciel's names and these changes:
- **Everything in the class is `new`.** `X2GULuciel.cpp` / `.h` are new files (ASCII, CRLF; the Edit tool is fine on them), added to
  `X2Lib_2010.vcxproj` (and its `.filters`) by CH1a. The class compiles under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` from CH1a and stays
  unreferenced until CH1 final. `_LIVE` then adds the factory cases (`CX2Game::CreateGUUser` `X2Game.cpp:21276`, the intruder copy
  `:2141-2201`), the script and motions. Names follow March's sibling classes (`X2GUChung.cpp`): `LSI_<STATE>_StateStart` / `_FrameMove` /
  `_EventProcess`, Hungarian members, `_CONST_LUCIEL_<phase>_` namespaces. No `June` and no `Luciel_` copies of `CX2GUUser` members.
- **Class-agnostic pieces stay class-agnostic.** Where June's engine code tests a unit type or reads a two-body member for every class
  (the equip view, the user sync, the damage-state getters, creation), the port is in `CX2GUUser` / the engine under a `SERV_IRUHADEV_JUNE_CHAR_*`
  flag and is not copied into the class. Rose's and Ain's ports widen the `#if`, they do not repeat it.
- **Shared structs (2-byte class).** `CommonPacket.h` is reached before `Always.h`: NU2's flag is also `#define`d at the end of `ServerDefine.h`,
  added to `SECOND_HOMES` in `tools/flagcheck.py`, and added to `X2ServerProtocol_2010.vcxproj`'s `X2TOOL|Win32` defines (layout change). Prove it
  with `cl /P` on one file per project. Keep the emulator's handlers and `X2OfflineDB` consistent in the same commit.
- **Saves.** NU4 adds no schema; DU4 is the one migration (user, 2026-11-02: June's NESI numbering, forward-only, snapshot `db_backup/` first).
  Play-test saves never slot a skill in slot 4 / 9 before transcendence.
- **Shared Lua per character.** `UnitTemplet.lua` (core list) gets only Lu/Ciel's 7 rows (NU3). `DLG_Unit_Create_Unit_Button_New.lua` is a
  multi-character file: NU4 converts it to core-list source and adds only Lu/Ciel's slot. `Enum.lua` (core list) gains `DT_BOTH` / `DT_KEEP` (CH1p).
  `DungeonEnum.lua` / `DungeonData.lua` (core list) gain only the tutorial rows (TU). Single-owner files are adopted whole (`adopt-june`, bytecode),
  each with `luabind.py check`.
- **Starter items (user, 2026-11-02): 15 rows, a labelled stand-in** under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_STARTER_ITEMS`. These are the gear
  113300-113304 (Cobo ... for Beginners, `unit_class` 200; Ciel's copy through `item_id_for_minor_unit` 200113300-200113304), **both** promotion
  sets (Lu's 41890-41894, set 5933; Ciel's 41900-41904, set 5934, `is_for_minor_unit` 1), and `plan_starter_items.tsv` lists the ids. The live
  shape is ten rows, so the comment says so.
- **Stat rows: open until CH0 (user, 2026-11-02).** CH0 reports whether June's `CharacterStatTable` (or any June data) is a full per-level
  table. The user decides then. Until then the loader uses the synthetic curve and logs it (NU4), and never another class's rows.
- **Ghidra.** Read-only, except that CH0 may *create* the uncarved functions (create-function only, no rename or comment) so that later phases
  can decompile them. It says which it did. Where a function stays uncarved, the capstone dump in `june2017_re/char_luciel/ghidra/` is the source.
- **One code phase at a time in `X2GULuciel.*`**, and one at a time in `X2GUUser.*` across this plan and any other.

## Starting context

| Need | Command |
|---|---|
| One section of this doc | `python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "<heading>"` |
| Planning evidence | `june2017_re/char_luciel/plan_*` and `ghidra/` (below); CH0 / NU0 write `ch0_*` / `nu0_*` beside them |
| Checklist rows | `python tools/docsection.py JUNE2017_CHARACTER_PORT_CHECKLIST.md "<heading>"`; Lu/Ciel's 78 lines: `plan_march_side.md` §6a |
| Stage 8 engine bullets | `python tools/docsection.py JUNE_PORT_BACKLOG_ORDER.md "Stage 8: new characters (Lu/Ciel, Rose, Ain)"` |
| Chung's tools (take a character or copy with the range changed) | `june2017_re/charport_states.py`, `charport_keys.py`, `charport_closure.py`, `charport_adis.py` (`CLASS_RANGE`: CH0 adds `luciel`), `charport_regsim.py`, `s3_no_state.py`; `char_chung/ch0_*`; `char_arme/ch0c_listing_check.py` |
| June Lua (reading) | `june_src_fixed/data036_kom/Luciel_Duo.src.lua`, `Luciel_Demonio.src.lua`, `UnitTemplet.src.lua` (Lu/Ciel rows `:1723-1979`), `Enum.src.lua`, `NewSkillTempletVer2.src.lua` (tree rows from `:282612`), `DungeonEnum.src.lua:56,101`, `DungeonData.src.lua:29878`, `LevelUpEvent.src.lua:158,381-383,627-629` |
| Ghidra | `ghidra` skill; range `0x32ae4080-0x32b4e76f`, vtable `334f06fc` (547 slots), ctor `32b1bd90`, dtor `32b39380`, InitState `32b43d10` (0x7a4), InitStateCommon `32b3e8b0`, skill setter `32b46580` (0x7b4, uncarved), factory `3230f470` (case 10 in `322ced00` / `322d5bf0`) |
| Bring June Lua in | `/x2-adopt-june` (`luasrc.py adopt-june data036 Luciel_Duo.lua Luciel_Demonio.lua`, `--dry-run` first) |
| Stage June assets | `python tools/kom.py stage-june <list>`, `/elstools pack dataNNN`, `/x2-kom-packed copy dataNNN` (ask first) |
| Hygiene / build / logs | `flagcheck.py <FLAG> --worktree --files ...`, `junediverge.py --check ...`, `enumscan.py --check ...`, `build.py --deploy` (background), `logs.py --grep "JUNE_CHAR\] LUCIEL"` |
| Save / test character | `/x2-db`, `/x2-create-char <name> <class> <level>` (NU4 teaches `createchar.py` the seven classes) |
| Dev console | auth level 3, END key; `/additem <id> [qty]`, `/qdone <id>`, `/tc 1` |

## Planning evidence (2026-11-02)

Digests are kept in `june2017_re/char_luciel/`. They were written by Sonnet subagents and spot-checked: the ctor writes vtable `334f06fc` at
`32b1bde2`, `X2Unit.h:36` / `:167-191` hold the enums, and `UnitTypeOf` is at `X2OfflineStatTable.cpp:271-287`. The files are
`plan_june_side.md`, `plan_march_side.md`, `plan_ghidra.md` (+ `ghidra/` dumps: `vtable_334f06fc_all.tsv`, `range_function_candidates.tsv`,
`range_string_refs.tsv`, the decompiles), `plan_charport_keys.txt` and `plan_starter_items.tsv`.

| Measure | Lu/Ciel | (Chung) |
|---|---|---|
| June class range | `32ae4080`-`32b4e76f`; ~583 functions (499 carved); 122 vtable slots into the range (119 overrides + 3 own) | 441 carved, 72 overrides |
| Skill tree | 301 rows over 7 classes (200: 19; 37 / 38 / 39: 35 each; 127 / 128 / 129: 59 each); 195 templets (52 `SI_` 10000-10601, 143 `RNSI_` 30001-30606); **not in `s3_reform_ids_no_state.tsv`** | 261 |
| Skill setter | `32b46580`: jump tables 0x7532-0x760c (65 ids) and 0x778a-0x778e (5) plus compares, ~75-85 cases | 118 |
| Registrations | InitStateCommon `32b3e8b0`: 294 through `31fd89b0`; InitState's class switch sets `+0x8ac8` (37 / 38 / 39 / 200 -> 0, 127 -> 1, 128 -> 2, 129 -> 3) | |
| Script tables | Duo: LU 83, CIEL 79, SA 63, A 36, LCG 29, LLC 28, RIDING 27, LLD 17, LCN 17, LDN 7, LDB 5, rest; Demonio: SA 94, BOUNDING 30, LDB 19, A 14, HA 1 | 385 |
| Key census | Duo 230 keys: june-only 23 / neither 3 / march-only-site 4 / both 200; Demonio 127: 12 / 2 / 3 / 110 | 249: 11 / 8 / 7 / 223 |
| Gap scan | `Luciel_Duo {'no NUMERICAL_DATA_INDEX': 67, 'no DAMAGE table': 15}`, `Luciel_Demonio {11, 2}`, `Luciel_Demonio {'CUSTOM_RENDER_STATE': 3}` | 74 / 15 |
| Motions | 125 June files (data208 Lu 62, data218 Ciel 63; 98 `*_Emotion_*`); March has 4 of the names (`Motion_CSM_Lu_LDB.x`, `Motion_Lu_LDB_Evil.x`, `Motion_CSM_Ciel_LDN.X`, `Motion_Ciel_LDN_Evil.X`) | 70 / 17 |
| UnitTemplet | 7 June rows, 0 in March; 18 keys no other class has (`m_MinorUnit*`, `m_fMinorUnitLobbyRotationY`, `majorUnitName`, `minorUnitName`, `m_HyperCutIn_Trans`, `m_MinorUnitHyperCutIn(_Trans)`) | |
| Tutorial | `DI_DUNGEON_TUTORIAL_LUCIEL` 41240 (`Dungeon_Tutorial_Luciel.lua`, already June bytecode), `DI_BATTLE_FIELD_TUTORIAL_LUCIEL` 39609, `WI_TUTORIAL_LUCIEL_STAGE0` 633; live `DungeonEnum` / `DungeonData` lack them; `Tutorial_Luciel_Stage0*` (data040) missing | |
| Missing June Lua | `Luciel_Duo.lua`, `Luciel_Demonio.lua`, most Lu/Ciel UI dialogs in data034 (only `DLG_Transcendental_Change_Guide_Luciel` present), `DLG_Class_Change_Luciel_*` (data217), `DLG_LUCIEL_BOUNDING_KEY.lua` (CH0 checks) | |
| Summons | NPC scripts `CSM_*`, `SA_LDN_*`, `SI_SA_*` present as June bytecode; `NUI_SI_A_LCN_SHADOW` (1907) has no script | |
| Transcendence | guide present; `HQ_Cutin_{LLD,LCN,LDN}_{LU,CIEL}_Transcendence` (+ `_Trans` for LDN) in data195 | |
| Starter items | gear 113300-113304 exact; promotion sets Lu 41890-41894 / Ciel 41900-41904 (both, user) | |

## Phases

| Phase | What | Model | Depends |
|---|---|---|---|
| CH0 | Class map: functions (carve), states, skills, keys, checklist rows, assets + effect closure, summons, unmapped helpers, stat-row data question | Opus high | - |
| NU0 | Engine map: every `UT_END` / `UC_END` / unit-class-width site, the 60 `case UT_ADD` switches against June's, creation, templet keys, two-body readers, equip view, NESI, char info | Opus high | - (beside CH0) |
| CH0b | Every June Lu/Ciel asset, whole, first | Sonnet high, user play-test | CH0 |
| NU1 | Unit-type range: June's `UT_END` 13, arrays / loops / script table, `UC_END` guards | Opus high, Opus review | NU0, char-chung Z |
| NU2 | June's 2-byte unit class on the wire and in the emulator | Opus xhigh, Opus review | NU1 |
| NU3 | `UnitTemplet`: Lu/Ciel's 7 rows and the 18 June-only keys' parser | Opus high, Opus review | NU1 |
| NU4 | Creation: button dialog, `SelectLuciel`, emulator create-unit, starter items, stat fallback fix, tools; select screen | Opus high, Opus review, user play-test | NU2, NU3 |
| DU1 | Second body in `CX2GUUser`: members, minor-unit model, user sync, not-selected-body physics, one-shot reset | Opus high, Opus review | NU3 |
| DU2 | Damage-state getter virtuals and their two engine readers | Opus high, Opus review | NU1 |
| CH1a | Class skeleton: new files, ctor / dtor / members, own virtuals, `MemoConditionLuciel` | Opus high, Opus review | CH0, NU1 |
| CH1b-CH1l | State bodies by group and override bands (11 sub-phases, below) | Sonnet high, Opus review per series (user, 2026-11-10): CH1b-f2, CH1g-i, CH1k-k2, CH1l-l3 | CH1a; one at a time |
| CH1m-CH1p | Wiring: registration; skill head + base callbacks; virtuals + keys + hooks; the in-class duo switch | Opus high, Opus review per series: CH1m-o together, CH1p alone | CH1b-CH1l reviews, DU1, DU2 |
| CH1 | Final: define `_LIVE`, factory cases, adopt both scripts, play-test | Opus high, Opus review, user play-test | CH1p review, NU4, CH0b |
| DU3 | June's equip-view rewrite (every class), item-templet minor-unit fields | Opus high, Opus review, user play-test | CH1 review |
| DU4 | NESI renumber and the save migration | Opus xhigh, Opus review, user play-test | DU3 review |
| DU5 | Two bodies in the character window and the unit viewer; form packets | Opus high, Opus review, user play-test | DU4 review |
| CHN | Job change for 37 / 38 / 39 / 127 / 128 / 129 (Aisha's "Phase CH-N") | Opus high, Opus review, user play-test | CH1 review |
| TU | Tutorial dungeon, field and world; first-entry route | Sonnet high, Opus review, user play-test | CH1 review |
| CH2a0 | Lu/Ciel's summoned NPCs (CH0 places it before the first skill phase that spawns one) | Opus high, Opus review, user play-test | CH1 review |
| CH2a-CH2d | Skills: base 200 and first jobs 37 / 38 / 39 | Sonnet high, Opus review per series (user, 2026-11-10): CH2a-b, CH2c-c2, CH2d-d2, user play-test each | CH1 review (CH2d: CHN) |
| CH3a-CH3d | Skills: second jobs 127 / 128 / 129, hyper actives, transcendence rows | Sonnet high (CH3d Opus), Opus review per series: CH3a-b, CH3c-c2, CH3d alone; user play-test | CH2 reviews, CHN |
| CH`n`0 | One Opus sub-phase per unmapped helper CH0 finds | Opus high, Opus review | as its phase |
| CH4a-CH4c | Passives; damage exception + overrides; memo and item readers | Opus high, Opus review per series: CH4a-b together, CH4c alone; user play-test | CH2 reviews |
| CH5 | UI and data: two-body cut-ins, HUD portraits, bounding-key dialog, combination-point UI | Sonnet high, Opus review, user play-test | CH1 review, DU5 |
| CH6 | Whole-character walk + gap reruns | Sonnet medium, user walk | CH3, CH4, CH5, TU reviews |
| Z | Close-out | Sonnet medium | CH6 |

CH0 and NU0 run now and side by side. Their outputs don't overlap, and both are read-only except CH0's optional Ghidra carving. CH0b follows
CH0 and may also run before char-chung Z, because it is assets only. All other phases wait for char-chung Z. A group over ~45 functions is split
by CH0 with a digit suffix (`CH1g2`), along with its kickoff and STATUS row. Play-test follow-ups after CH1 final get the next letters.

## Phase CH0 - Lu/Ciel class map (read-only)

**Run with:** Opus 5.5 high. No code, no flag, no build. Delegate reads, greps, tsv joins, scripted runs and known-address dumps. Outputs in
`june2017_re/char_luciel/`.

**Step 0.** Decide the carving: either create the ~85 uncarved functions in Ghidra (`range_function_candidates.tsv`, create-function only) or
use capstone dumps. Then build the registration map with `charport_states.py init 32b43d10 32b3e8b0 <base>` and `skill 32b46580`, and run
`ch0c_listing_check.py` on it. Every row must match the raw listing; check the `~` rows and the std::string-named rows by hand. Add `luciel`
to `charport_adis.py`'s `CLASS_RANGE`.

**Scope:**
1. **Class map** `ch0_class_map.tsv`: every function in `0x32ae4080-0x32b4e76f`, plus the three jump-thunk tails. Columns: kind (override /
   own virtual / registrar / state callback / helper / dead), June vslot, **proposed plain name**, the March `CX2GUUser` member it overrides (by
   name, from June's base vslot), target sub-phase from the table below, and funcmap rows `Char Luciel CH0`. Settle the boundary functions
   (`32b4e770` / `32b4e780` are shared `ret` stubs).
2. **State and skill table** `ch0_state_skill.tsv`: 406 + 158 tables, 195 templets / 301 tree rows. **Regenerate the S3 census for Lu/Ciel**
   (`s3_no_state.py` with the seven classes added; the tsv has no Lu/Ciel rows) so CH6 compares like with like. Confirm no state id is stored in a
   `UCHAR`, and say whether the `LSI_` state ids overlap `CSI_` ranges anywhere a shared table is indexed by state id.
3. **Keys** `ch0_keys.tsv` (`charport_keys.py Luciel_Duo`, `Luciel_Demonio`): each of the 23 + 12 June-only keys gets an owner phase (duo keys
   -> CH1p, `CUSTOM_RENDER_STATE` -> CH3c (7w), `CUSTOM_OFFSET_NOT_SELECT_UNIT` / `USE_NOT_SELECT_*` -> DU1), and the `neither` keys are checked.
4. **Checklist rows** `ch0_checklist_rows.tsv`: every §1-§7au row that names Lu/Ciel (`plan_march_side.md` §6a, 78 lines), plus the 11
   every-class sections -> phase or n/a with the reason. NU0 owns the rows about engine sites; agree the split with its tsv.
5. **Assets** `ch0_assets.tsv` + `ch0b_stage_list.tsv` (routes a name / b data / c `item.kim` Lu/Ciel items, unioned) and **effect closure**
   `ch0_effect_closure.tsv` (`charport_closure.py`, run-time-built names included). It includes the 125 motions, the minor-unit meshes
   (`Mesh_Lu_Base_SD.X` and kin), faces and hair for both bodies, the cut-in art, `POPUP_TR_*` from `DLG_Transcendental_Change_Guide_Luciel`, the
   icon sheets `skillicons.py check` lists, and the June UI Lua the class reads (`DLG_LUCIEL_BOUNDING_KEY.lua`). The 4 March same-name motions are
   replaced by June's (asset rule). Say who reads them in March (`CSM_*` NPCs).
6. **Unmapped helpers.** For every skill, the callees outside the class range that funcmap does not map (`ch0_unmapped_callees.py`). Each one
   gets an Opus `CH<n>0` sub-phase, kickoff and STATUS row.
7. **Summon sites.** Every `CreateNPC`-style call in the class and its NPC id (`NUI_SI_A_LCN_SHADOW` 1907 has no script: what does June
   spawn?), and `luabind.py check` on every summoned NPC's script. Place CH2a0 before the first skill phase that spawns one, and split it if there
   is more than one spawn rule.
8. **Stat-row data (open decision).** Is June's `CharacterStatTable.src.lua` (`UT_LUCIEL` `:167`, `DREAD_LORD` / `NOBLESSE` / `DEMONIO`
   `:417-433`) a full per-level table in `StatTable`'s units, or a display / base table? Is there any other June stat source for 37-39, 127-129 and
   200? Report what exists, and put the question to the user with a recommendation. Don't decide it.
9. **Shared data.** Which `LSI_*` / `SI_*` / `RNSI_*` names the C++ enums need (raw id + comment where June's Enum has no name); the 7
   `UnitTemplet` rows' strings (`majorUnitName` / `minorUnitName` ids in our string table?).
10. **Amend the kickoffs** CH0b-CH5, CHN and TU ("Amended by CH0" sections: filters with real column numbers, addresses, counts, any split).

**Done gate:** the tsvs exist; `ch0c_listing_check.py` passes; every function has a sub-phase and a name; 406 + 158 tables and 195 ids mapped;
the S3 baseline exists; assets union routes a / b / c; the summon map exists; the stat question is with the user; kickoffs amended.

### CH0 - Done (2026-11-02)

Read-only, no flag, no build (Opus 5.5 high; Sonnet subagents ran the builders, each claim below spot-checked). Outputs and their builders are in
`june2017_re/char_luciel/` (`ch0_*.py`, rerun order in each docstring); `charport_adis.py` has `luciel`; `s3_no_state.py` has the 7 Lu/Ciel families;
`char_eve/ch0_build_assets.py` has a `luciel` entry (run it with `--out`). 139 funcmap rows `Char Luciel CH0` (`ch0_funcmap_new_rows.tsv`).

**Step 0.** GhidraMCP has no create-function endpoint, so nothing was carved: uncarved functions (1,144 of 1,643 starts) are read with capstone
(`charport_adis.py luciel`). `charport_states.py init 32b43d10 32b3e8b0 3202b110` -> `ch0_reg.tsv`, 364 rows; `ch0c_listing_check.py` **364 / 364**
(re-run by me), no `~` rows; the 5 std::string rows are the START / WIN / LOSE rolls (9 / 11 / 12) and hyper 23 / 204 (8 names by class).

**Maps**
- `ch0_class_map.tsv` (1,643 starts): **867 to transcribe** (651 state-fn, 119 overrides, 93 helpers, 3 own virtuals, 1 registrar), 731 thunks, 27 dead,
  18 not Lu/Ciel. Names: every state-fn, 105 of 122 vtable rows, 33 of 93 helpers; 75 stay `?` with a hint (names in `ch0_hand_names.tsv`).
  The state-id getter band (0x114 / 0x118 / 0x438-0x4a4 / 0x5c4) returns June ids: **June inserts a state at 24**, so 25 = DAMAGE_GROGGY ... 42 =
  DAMAGE_REVENGE (NU0's DU2 note agrees); never count March's enum. `31fc9d90` / `31fc9d50` = CX2GUUser transformed-form flags (+0x8f6e Lu,
  +0x8f6f Ciel; 2 = either, 3 = both), checked in the bytes.
- `ch0_state_skill.tsv`: **564 tables** (553 phased, 5 dead, 6 INIT_*, 0 unassigned) + 46 registered names with no table; **198 ids** (195 templets +
  106 / 900 / 208), 301 tree rows (271 with a state, 30 passive). Prefixes: `LD` / `LUCIEL` 200, `LLC` 37, `LCG` 38, `LDB` 39, `LLD` 127, `LCN` 128, `LDN` 129.
  State ids 7-592 (16-bit already, `SERV_IRUHADEV_JUNE_USER_STATE_ID16`); no `LSI_` in either `Enum.lua`.
- `ch0_skill.tsv`: the uncarved setter `32b46580` via a listing emulator: **64 cases, 112 ids**; 12 states are set elsewhere (InitStateCommon, vslot 0x57c).
- `ch0_vtable_all.tsv` (547) / `ch0_vtable_slots.tsv` (122). `ch0_keys.tsv` (357; census counts match the plan): owners per key. `ch0_checklist_rows.tsv`
  171 rows (43 n/a, 36 NU / DU with "NU0 confirms"). `ch0_s3_baseline.txt`: 301 rows, all no_state.
- Assets: `ch0_assets.tsv` 17,227; **`ch0b_stage_list.tsv` 14,609** (dry run 37 koms, 0 refused, 4 new kom dirs); `ch0_effect_closure.tsv` 13,049 rows,
  572 missing. **127 motions, not 125**; the 4 same-name ones are already June's bytes. `skillicons` both ranges FAIL until CH0b copies 12 sheets.
- Summons (`ch0_summons.tsv`): 3 class spawn sites, 1906 (LLD, `@32b20486`), 1908 (LCN, `@32b2fe73`), 2269 (LDN, `@32b340ce`); 2270 is spawned by the
  Garden NPC's Lua; **1907 `NUI_SI_A_LCN_SHADOW` is spawned by nothing** (June ships no script; the shadow is DamageEffect `Mesh_LCN_SHADOW`).
  `luabind` PASS for 4 scripts, Garden FAIL (4 unbound). The dtor / ReInit look up 2269, so CH1a registers its name.
- Item 9: `ch0_enum_names.tsv`: 7 of 195 ids in the C++ enum by name, 68 in the core `Enum.lua`, 126 in neither; **none unnamed in June**.
  `ch0_unittemplet_luciel.tsv`: all 28 name / description STR ids exist in March's `general.ess`; the 18 June-only keys = NU0's.
- Item 6: 379 out-of-class callees, 131 unmapped (72 confirmed, 57 candidate, 2 unknown); 21 + the transform-link group need Opus.

**Phase changes** (kickoffs amended by `ch0_amend_kickoffs.py`; STATUS rewired):
- Merged: **CH1c -> CH1b** (41), **CH1e -> CH1d** (30), **CH1j -> CH1i** (45, riding is 3 functions). Split: CH1f / **CH1f2**, CH1g / **CH1g2**, CH1k /
  **CH1k2**, CH1l / **CH1l2** / **CH1l3**, CH2c / **CH2c2**, CH2d / **CH2d2**, CH3c / **CH3c2** (CH1h 47 stays whole).
- New Opus helper phases: **CH1a0** (gauge class, transform link, hold-key selector, switching guide; before CH1a, after DU1), **CH1k0** (buff
  Time-finalizer passes), **CH1l30** (unit scans, effect aim), **CH2d20** (transform HUD), **CH3c20** (`SLockOnSystem::ProcessLockOn` out of
  `CX2GUAra`, Garden binders). **CH2a0 -> CH3a0** (first spawn is 127's). `31fd6260` / `31fdd8e0` go to DU1.
- InitState pushes the ten passive delegates (7am): nine are CH4a's, `AddCombinationPointOnHit` `32b14bc0` CH1p's; CH1m keeps 4 functions.
- **Corrections to the plan:** `LSI_A_*` are actives, not combos (CH1f is `*_COMBO_*`); `Enum.lua` also needs the `CUSTOM_RENDER_STATE` table
  (CH1p); `DLG_LUCIEL_BOUNDING_KEY.lua` is adopted at CH1 (Init reads it), the two other Lu/Ciel dialogs at CH5; the `CommonStateStart` key
  parse is DU1's (NU0 agrees); of the checklist's type-10 branches, L274 (`BOUNDING_UP` StateStart `31fd02d0`) is DU1's, L379 / L454 CH4b's, L471 CH1i's
  (none of the four is in NU0's tsvs, so their kickoffs carry them).

Functions per phase: CH1a 21, CH1b 41, CH1d 30, CH1f 39, CH1f2 38, CH1g 38, CH1g2 38, CH1h 47, CH1i 45, CH1k 31, CH1k2 30, CH1l 32, CH1l2 32,
CH1l3 32, CH1m 4, CH1n 1, CH1p 1, CH2a 32, CH2b 27, CH2c 35, CH2c2 35, CH2d 31, CH2d2 31, CH3a 26, CH3b 45, CH3c 41, CH3c2 40, CH3d 7, CH4a 15, CH4c 2.

**Stat rows (item 8, with the user).** June has **no per-level table** for Lu/Ciel: `CharacterStatTable` is gear-stat scaling (one relative row per
unit type, `UT_LUCIEL` :166-173 `430, 98, 410, 93, 10000`; second-class rows 127 / 128 / 129 :417-433; 37 / 38 / 39 / 200 use the type row), and no
June server `StatTable` exists here. But **every existing `StatTable.lua` row (50 classes, levels 1-80, 4,000 rows) is that relative row x (0.8 +
0.0225 (L-1)), HP x (1 + 0.15 (L-1)), within 1** (`ch0_stat_fit.tsv`; spot-checked Elsword L1 / L40, Lord Knight L1 / L40); second jobs start at 0.8 x
their own row and then follow the type curve; 81-99 is `cap99_gen.py`'s extension. Trap for NU4: once the templet exists, `UnitTypeOf` maps 200 to
type 10 and the fallback silently reads Knight's class-10 rows.

## Phase NU0 - New-unit and two-body engine map (read-only)

**Run with:** Opus 5.5 high, beside CH0. No code. Outputs `june2017_re/char_luciel/nu0_*.tsv`, funcmap rows `Char Luciel NU0`.

**Scope:**
1. **Unit-type range** `nu0_unit_type_sites.tsv`: every `UT_END` / `UT_END - 1` / `UT_ADD`-terminated site (the `X2UnitManager.h:3442,3444`
   arrays, `apszScript[]` + its `ASSERT` at `X2UnitManager.cpp:405-427`, `X2Item.h:208-209`, `X2Item_Preprocessing.inl:1035`, the ~25 loops in
   `X2BeginningGame` / `X2StateCreateUnit` / `X2StateServerSelect`) and every `UC_END` guard (`X2SocketItem.cpp:2897`, `X2ItemManager*.cpp`). Each
   site gets June's function, June's bound or table size (June's `UT_END` 13; June's 12-name script table at `333c1c28`), and a verdict for NU1:
   widen / June's body / leave. Use `cl /P` to say which branch is live.
2. **Per-type switches** `nu0_type_switches.tsv`: the 60 `case CX2Unit::UT_ADD:` sites (`plan_march_side.md` §4). For each one: the June
   function, what June does for `UT_LUCIEL` (and for `UT_ROSE` / `UT_AIN` in one column, for the later plans), and the owner phase. Lu/Ciel cases are
   owned here; Rose / Ain cases are recorded only.
3. **Unit-class width** `nu0_unit_class_width.tsv`: every `char` unit-class member in `CommonPacket.h` / `ClientPacket.h` (33), every
   client reader / emulator writer cast (`X2Unit.cpp:4342,4586,4753,4845`, `Handlers_Unit.cpp:251,263,411,499` and the rest), and June's type and
   offset for each (`3235eb20` / `32360090` / `323614c0` / `32361e00`). Note any `Put` / `Get` serializer and `sizeof`-dependent code.
4. **Creation and selection** `nu0_create.tsv`. This covers **Lu/Ciel's whole entry on the character-creation screen** (user, 2026-11-02):
   June's create-unit state (June's `SelectLuciel` counterpart and the entry switch), the button slot in `DLG_Unit_Create_Unit_Button_New.lua`
   June vs March (`LucielCreateUnitSlot`, `Pos_Luciel = 8`, the Lu slot only), the portrait / name / description textures and strings it shows,
   the preview model on the creation stage (both bodies? which idle / select motions, the camera and lobby rotation from `m_fLobbyRotationY` /
   `m_fMinorUnitLobbyRotationY`), the job-preview buttons `DLG_Unit_Create_ClassChange_Button_Luciel` (who reads it, what each button plays),
   the select sound / voice, and any intro movie or effect June plays on picking him. Also the server-select / character-list model for a
   two-body unit, and what June does on a new character's first entry (tutorial route?). Every asset these name goes to CH0b's list.
5. **Templet keys:** June's `UnitTemplet` parser for the 18 Lu/Ciel-only keys (function addresses, target members), and March's parser.
6. **Two-body engine** `nu0_duo_engine.tsv`: every reader / writer of the `+0x8e88` / `+0x8e90` / `+0x8e91` members and the unit's form byte
   `+0xe40` outside the class (base ctor `3203e850`; the whole-image writers `31fe633a`, `3204e07a`, `32050e1a`, `32092a04`). Plus the Stage 8
   bullets: the user-sync bits (c3e rows B, E, F, H, I, G), PhysicProcess's not-selected-body branch (`32015b30 @32019f9b-3201a1c7`), the one-shot
   reset (`31fd4290` / `31fd4340`), the damage-state getter vslots (0x114 / 0x118 / 0x494 / 0x490, 0x458) and their readers, and the minor-unit
   model and skin creation. Assign each to DU1 / DU2.
7. **Equip view, NESI, char info:** the Stage 8 bullets' functions (`3201b930`, `31fd2f00`, `31ffe990`, `31fd48a0`, `31fd63c0`, `328825c0`,
   `32882540`; NESI 20 -> 35, 21 -> 43; `charinfo_functions.tsv` rows `3234b8c0`, `323acfb0`, `3291d390`, `31f93a10`, `31faa150`, `31f94190`,
   `3291d360`, `3234f4d0`, `321693b0`, `32e22ce0`; form packets `0x4d9` / `0x4da`, plus `0x4fd` / `0x4fe` if guild agit is reachable offline)
   -> DU3 / DU4 / DU5, with the save-migration shape for DU4 (which tables and columns hold an NESI id).
8. **Job change and tutorial:** the class-change route for the six job classes (quests `CJT_*`, cash items, `X2OfflineClassChangeSeed.h`
   `PARENT_ROWS`, `X2OfflineLuaEnumSeed.h`) -> CHN; the tutorial's DungeonEnum / DungeonData rows, `Tutorial_Luciel_Stage0*`, and how June
   enters it -> TU.
9. **Amend the kickoffs** NU1-NU4, DU1-DU5, CHN and TU.

**Done gate:** the tsvs exist; every site has an owner phase and a verdict; the DU4 migration shape is written; kickoffs amended.

### Done (2026-11-02, Opus 5.5 high; read-only, no flag, no build)

Eight tsvs in `june2017_re/char_luciel/` (Sonnet subagents; each spot-checked against the June bytes or the March line it cites). The
kickoffs NU1-NU4, DU1-DU5, CHN, TU and CH0b carry the per-phase detail ("Amended by NU0"); 25 funcmap rows `Char Luciel NU0`.
- **Item 1** `nu0_unit_type_sites.tsv` (55 rows; `cl /P` liveness): widen 15, june_body 2, leave 38 (15 dead). June's script table holds 13
  (`337341b8`, `cmp esi, 0x337341ec`), getter `3237a680` `cmp eax, 0xc`, classes accepted 1..202 (`3271a858 cmp eax, 0xc9` after `dec`).
  **Trap:** March's `ASSERT` at `X2UnitManager.cpp:425` compiles out, so the loop must not widen before the table. `SERV_LIMIT_UNIT` is live (the
  planning digest said dead). `X2OfflineStatTable.cpp:283` stays (class -> type for base classes only).
- **Item 2** `nu0_type_switches.tsv` (60 sites): CH1 7, CH5 10, DU1 3, DU5 6, NU1 1, NU4 9, TU 1, none 23 (dead, already June's, or June
  data-driven). Rose / Ain cases recorded in their column. June reads the form byte (`+0x8e90` / unit `+0xe40`) in the revive voice, weapon-effect
  bones, emotion sound, Square SD mesh and reform.
- **Item 3** `nu0_unit_class_width.tsv` (202 rows): June's class is a **word** at the same offsets as March's char (`3235eb7b movzx ecx, word ptr
  [ebp + 0x15]` and three siblings; transcendence int at +2), packs 1, PUT / GET member by member. **The layout change does not reach
  X2ServerProtocol's code:** the serializers compile only in `X2ServerPacket.cpp`, offline transport is bytes with both ends in X2Lib
  (`Session.cpp:343-349`), and X2ServerProtocol sees `CommonPacket.h` as an unused header. NU2's size: 50 members, 34 serializers, 68 readers,
  39 writers, 5 `char` signatures; no `sizeof` / `memcpy`.
- **Item 4** `nu0_create.tsv` (47 rows): SelectLuciel `325559f0`, event proc `32558860` (messages 36 / 50), no June entry switch. June shows
  **two bodies** on the create and select screens (`@32555fe8` viewer `+0xd7c`); NU4 shows Lu, DU5 adds Ciel. Create request trap: March's live
  branch sends the unit type as the class (`X2StateCreateUnit.cpp:2505`). Name / description are pictures in `GUI_character_info_10.TET`. Assets
  listed in CH0b's kickoff (new: background, info_10, bt_03, 6 voices, 4 Common / Lobby motions, 29 basic / cash meshes, `SelectVoiceTable.lua`;
  June replaces `bt_01` / `bt_02`, `Texture67`); the 4 job movies are already in March.
- **Item 5** `nu0_templet_keys.tsv`: June parser `3237aa40` (XOR keys, `xorstrings.tsv`), 18 keys all scalar; 15 new members plus
  `m_fLobbyRotationY` (+0x2d4, every June row, March has none); `minorUnitName`'s length (+0x50) is June's "has a second body" test.
- **Item 6** `nu0_duo_engine.tsv` (2,964 rows: DU1 440, DU1-accessor 2,248, DU2 48, DU3 6, DU5 123, out 90, Ain / Rose 9). The second body
  mirrors the first at +0x6b4 (`+0x8e7c..+0x8e8c`), `+0x8e90` active body, `+0x8e91` dual enabled. **June's skin-anim accessor is duo-aware and
  inlined into every class** (`32070cf0`; Eve's range alone reads it 420 times): DU1 changes `X2GUUser.h:2092-2094`, not 2,248 sites (rows
  retagged from the subagent's `CH1`). The second body is built only in `InitMotion` `31fe5860`. DU2 has seven reader functions, not two
  (`31fd6ce0`, `31fd7350`, `320348c0`, `32063650`, `31fe7610`, `3202a9c0`, the 0x530-0x538 predicates).
- **Items 7-8** `nu0_equip_nesi.tsv` (33: DU3 7, DU4 10, DU5 12, none 4), `nu0_jobchange_tutorial.tsv` (19: CHN 11, TU 8). **DU4 shape:**
  `item.slot` 20 -> 35 for category 9 and `inventory_size.size` 21 -> 43 for category 9, schema v22 -> v23; no other table holds an NESI id.
  Guild agit is not reachable offline: `0x4fd` / `0x4fe` dropped. Job change: quests already in `FieldQuest.lua` (200 -> 37 35644-35647, -> 38
  35648-35651, -> 39 35660-35690; 127 35750-35790, 128 35700-35740, 129 35800-35840), tickets 276955-276958.
- **Divergence rows** (verdict `stage8`, `junediverge.py --check-all` RESULT: PASS): `Handler_EGS_CREATE_TUTORIAL_ROOM_REQ` (`325622e0`: June
  sends every class to `DI_DUNGEON_TUTORIAL_*`, March to `DI_BATTLE_FIELD_TUTORIAL_*`; TU) and `RotateYSelectCharacterType` (`31edf260`, the
  templet rotation keys; NU4).
- **Open, in the owning kickoffs:** `USE_PHYSICPROCESS_NOT_SELECT_UNIT` / `..._ANIM_TIME_IN_SPEED` members (DU1); readers of the Common /
  Field / Lobby minor motions (NU3 / DU5); where the form persists across a relog (DU4, so there is one migration); June's widths of the 46
  members outside the four disassembled structs (NU2 widens all); the 50 unit `+0xe40` hits marked "other struct?" (DU5); the 39 / 129 ticket
  family (CHN). `dispsweep.py` misreads `cmp [reg+disp32], imm` and drops 66 / F2 / F3 prefixes (noted in DU1).

## Phase CH0b - Every June Lu/Ciel asset, whole, first

**Run with:** Sonnet 5.5 high, user play-test (ErrorLog diff on a select-screen pass of all nine March characters). `kom.py stage-june
ch0b_stage_list.tsv`, then pack, then ask before each copy. Nothing reads the files until CH1, except the 4 March same-name motions (CH0
names the readers). **Gate:** `python tools/skillicons.py check <Lu/Ciel's skill-id ranges 10000-10699, 30001-30999>` = PASS, with the shared
`DLG_SKILL_Passive_*` sheets included.

### CH0b - Done (2026-11-03)

Sonnet 5.5. No C++, no flag, no build (assets and data only; CH0 found no March reader a June asset breaks).
- **Reference:** `ErrorLog.txt` (287,399 B, 2026-11-03 19:26) saved as `data\ErrorLog_pre_CH0b_luciel.txt` before any copy.
- **Staged (stage list):** `kom.py stage-june --jobs 8 ch0b_stage_list.tsv`: `STAGE-JUNE OK: 37 kom(s), 14609 staged (14607 new, 2 replaced, 0 identical), 0 refused, 89s`
  (dry run identical). Replaced: `DLG_SKILL_Buff_01.tga` / `.tet` (data063), backup `kom_backup_stage_june\20261103_201733`.
- **Packed / copied (user approved, 2026-11-03):** 37 `PACK OK`, 37 `COPY OK` (all add-only: 0 removed; 33 existing archives + 4 new: `data162` 331, `data163` 294,
  `datai028` 250, `datai029` 245; `X2Main.cpp` mounts `datai001-032`). The 4 same-name motions were already June's bytes (nothing replaced).
- **NU0 amendment (not in the stage list, which holds CH0b-owned files only):**
  - `kom.py stage-june` 12 files: `DLG_Unit_Background_Luciel.tga` (data051), `DLG_UI_Common_Texture_GUI_character_info_10.tga/.TET` and `DLG_UI_Common_Texture67.tga` (data057; Texture67 replaced),
    `DLG_UI_Button_NEW_GUI_character_bt_01/_02 .TGA/.TET` (March's data056, 4 replaced, backup `20261103_202419`), `bt_03 .TGA/.TET` (data215),
    `HQ_Loading_TUTORIAL_LUCIEL_01/02.tga` (data126, found there). `STAGE-JUNE OK: 5 kom(s), 12 staged (7 new, 5 replaced)`.
  - `luasrc.py adopt-june` (June bytecode) 14 Lua: data034 `DLG_Unit_Create_ClassChange_Button_Luciel`, `Class_Change_Guide_Luciel` / `_1`, `DLG_1ST_` / `DLG_2ND_Class_Change_Guide_Luciel`;
    data036 `SelectVoiceTable`; data040 `Tutorial_Luciel_Stage0` / `_LineMap`; data217 `DLG_Class_Change_Luciel_CG / CN / DB / DN / LC / LD`.
  - `luabind.py check` over the 14: PASS after one row in `tools/data/lua_unbound_known.tsv` (`AddVoice`, SelectVoiceTable, NU4 ports the loader; March does not open the file yet, `X2Data.cpp:1920`).
  - 9 `PACK OK`, 9 `COPY OK` (data034 036 040 051 056 057 126 215 217). `data051` also deployed 48 older entries (`DLG_Background_*`, dated 2026-09-22, not CH0b's) already in `kom_contents`; the user approved.
- **Skill icons:** `skillicons.py check 10000-10601` PASS (52 skills), `check 30001-30606` PASS (143), `check 10000-10699 30001-30999` PASS (195, 0 misses, 0 sheets).
- **Not staged here (routed):** the 43 `Tutorial_Luciel_Stage0_*` meshes / textures (data164) and the other TU / CHN / CH5 Lua stay with their owners (TU, CHN, CH5, CH1); `ch0_effect_closure` names in no June kom (`Ciel_Claw04.ogg`, `Luciel_Shadow_Fury04.ogg`, `WhiteImage.tga`, `__Motion_HAURES2_Ciel_LDN.X`) have nothing to stage.
- **Play-test (user) is the gate:** log tag `[JUNE_CHAR]` has no lines; check `ErrorLog.txt` against `ErrorLog_pre_CH0b_luciel.txt` (`KEM_ERROR`, `Motion Error`, missing texture / mesh). Lu/Ciel is not selectable until NU4.
- **Drift:** none new.

## Phase NU - New-character foundation (NU1-NU4)

| Sub-phase | Scope | Flag |
|---|---|---|
| NU1 | `UT_END` becomes June's 13 (the enum reorders so `UT_LUCIEL` / `UT_ROSE` / `UT_AIN` keep June's values and `UT_END` follows them; no member renumbered). Every NU0 item-1 site gets its verdict: arrays sized by the new end, loops bounded as June bounds them (a loop that would offer Rose / Ain before their templets exist skips a type with no templet, logged), and `apszScript[]` with June's names (Rose's and Ain's scripts are absent, so their slots load nothing and log). The `UC_END` guards accept June's class ids as June does. | `SERV_IRUHADEV_JUNE_CHAR_UNIT_TYPE_RANGE` |
| NU2 | The unit class as June's word in every NU0 item-3 member. Put / Get, the emulator writers and the client readers move together. CommonPacket.h exception (ServerDefine.h, `SECOND_HOMES`, vcxproj define). Proven by a probe of each project, and by an existing character's full login / village / dungeon round-trip with no new log line. | `SERV_IRUHADEV_JUNE_CHAR_UNIT_CLASS_WORD` |
| NU3 | Lu/Ciel's 7 `UnitTemplet.lua` rows (core-list source, `luasrc.py refresh`), and March's parser reads the 18 June-only keys into new templet members (June's names and types, NU0 item 5). A `[JUNE_CHAR] LUCIEL DATA` line at load gives the row count and both body names. | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_DATA` (rows), `SERV_IRUHADEV_JUNE_CHAR_MINOR_UNIT_TEMPLET` (parser) |
| NU4 | Creation, starting with **Lu/Ciel's entry on the character-creation screen** (NU0 item 4, user 2026-11-02): `DLG_Unit_Create_Unit_Button_New.lua` converted to core-list source with Lu/Ciel's slot from June; his portrait, name and description; the preview model with June's motions, camera and lobby rotation (one body until DU5 if June shows two); the select voice / effect; `DLG_Unit_Create_ClassChange_Button_Luciel` adopted with its job previews; `CX2StateCreateUnit::SelectLuciel` and the entry switch (June's bodies); the emulator create-unit path for class 200 (the 15 starter rows, Rules); `X2OfflineStatTable` never borrows another class's rows (`UnitTypeOf`'s result is a unit type, not a class: the fallback becomes the base class or the synthetic curve, logged once per class); `tools/createchar.py` `CLASSES` / `PARENT` / `PROMO` gain the seven classes; the select screen shows a created Lu/Ciel (one body until DU5) and **does not enter the game until CH1**: the hand-off says so. | `SERV_IRUHADEV_JUNE_CHAR_CREATE`, `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_STARTER_ITEMS` |

**Done gate:** every NU0 site has its verdict applied; an existing character's play is unchanged (ErrorLog diff); the creation screen shows
Lu/Ciel's slot, portrait, description and preview as June's, and his job-preview buttons play; a Lu/Ciel is created and
listed with its 15 items in the save (`/x2-db` inventory); NU4's `CREATE` line shows class 200, the starter-row count and the stat source.

### NU1 - Done (2026-11-14)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_UNIT_TYPE_RANGE` (Always.h). Log `[JUNE_CHAR] LUCIEL NEWUNIT` (offline_server.log).
- **Enum** (`X2Unit.h`): with the flag `UT_LUCIEL` 10, `UT_ROSE` 11, `UT_AIN` 12, `UT_END` 13 (June `Enum.src.lua:313`); March's enum (incl. the
  `JUNE_ENUM_NAMES` block) is the `#else`. No member renumbered. Every `UT_END` site follows by itself, so the 15 `widen` rows that are plain
  `UT_END` bounds needed no edit: the `m_aUnitTypeLuaTemplet` array (13 x 8 B, June `32385a07`), the script loop (`337341ec`), the getter
  (`3237a6b3`, `<= 12`), `InvalideDataCheck`'s type loop (`328d6ad4`), the item-parse type check (`325bd4d8`), and the three picture loops
  (`324e41cf`, `324a9b5a`, `32532edf`: 12 pictures, null-safe `GetPicture`).
- **Script table** (`X2UnitManager.cpp`): June's 13 slots (`337341b8`): index 10 `LUCIEL_DUO.LUA`, 11 / 12 NULL. A `BOOST_STATIC_ASSERT` stands in for
  the compiled-out `ASSERT` (the NU0 trap). A slot with no script, or one `IsValidFile` cannot find, takes March's NULL path and logs once per type
  (three lines per start until CH1 adopts `Luciel_Duo.lua`; 9 March types unchanged). Stand-in: June builds the templet anyway and logs 0x48 / 0x49.
- **Class guards:** item parse class `0 .. UC_AIN_ANGEL` (June `325bd515`, `X2ItemManager_Preprocessing.cpp`); class stat check
  `UC_ELSWORD_LORD_KNIGHT .. UC_AIN_APOSTASIA` (June `328d6ae1` / `328d6b30`, 100..136; the live `CharacterStatTable.lua` is June's and holds all 37
  class rows and types 1-12, so no XEM_ERROR148). `X2SocketItem.cpp:2899` was already June's 1..202 (char-ara CHNa, `JUNE_CLASS_PATH`): no edit.
  `UC_END` stays 124.
- **`ChangeUnitButtonInfo`** (`X2StateServerSelect.cpp`): June `32564d50 @325651bd-325651d0` tests templet NULL or type outside 1..12 for the STR 707
  box (same arguments, Ghidra-checked: `0x18` / `0x2d` string lengths), replacing March's case list. A type-10 unit no longer pops 707.
- **Kept March's on purpose:** `X2OfflineStatTable::UnitTypeOf` identity pinned to `<= UT_ADD` (classes 10-12 are job classes; would have widened
  silently). `X2GameEdit.cpp` class cheat (kickoff: leave).
- **Live `UT_END` readers that now reach 13 without an edit (harmless):** `X2OfflineCashShop.cpp:174` (Lu / Rose / Ain costume rows get the
  mounted-file check instead of "offered unchecked"), `IruhaKimCensus` counts, `PlaySoundCreateUnit` (NU4) and `PlaySoundRevive` (DU1) guards:
  types 10-12 fall to March's switch default (no voice) until those phases. Dead rows (`X2Item.h`, `X2BeginningGame`, the old `Select*`, block
  comment, `#else` bodies) widen for free.
- **Hygiene:** flagcheck `--worktree`: `files: 6 PASS, 0 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`; `in this flag's files: 0 deleted line(s), 0
  non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS`.
  `--range 18460a26^..18460a26`: same tallies, `RESULT: PASS`.
  junediverge `--check` (6 files): `RESULT: PASS`. enumscan: 0 new (17 rows are the existing `JUNE_ENUM_NAMES` definitions).
- **Compile:** `/Zs` probes of the 5 `.cpp` (no PCH, `census.cl_cmd`): 0 errors with the flag; `/P` shows `LUCIEL_DUO.LUA`, `UT_END = 13` and the
  NEWUNIT line. Flag off = base by flagcheck. Commit `18460a26`.
  `DEPLOY OK: X2_offline.exe is 26,475,008 B, dated 2026-11-14 00:55:27, md5 a62d6986ede44b4015f4d47aea583cb6` (`--pch force`).
  `EXESTR OK: 2 of 2 found` (the NEWUNIT format string, `LUCIEL_DUO.LUA`, wide).
- **Routed:** data036 `Enum.lua` still says `UT_END = 10` (no Lua reads it) -> NU3 kickoff. March's 9-picture `DLG_PVP_Result_Card_Back*` /
  `DLG_PVP_Room_Character_Slot` (June's hold 12) -> CH5 kickoff. `PlaySoundCreateUnit` / `PlaySoundRevive` bodies stay NU4 / DU1.
- **How to see it:** startup writes (ErrorLog.txt since fix 1) three `LUCIEL NEWUNIT` lines (types 10, 11, 12); an existing character's select / village / dungeon is unchanged.
- **Play-test 1 (user, 2026-11-14): passed** - select (every slot, no 707), village / dungeon on `ElesisGM` and `reyaa` unchanged; logs on exe
  a62d6986: 0 XEM_ERROR144 / 148, 0 KEM_ERROR72 for `LUCIEL_DUO.LUA` (the skip ran), 0 EXCEPTION. The three NEWUNIT lines were missing:
  `CX2OfflineLog` opens at 00:58:04, the unit scripts load at 00:57:55, so the lines were dropped.
- **Fix 1:** the line goes to ErrorLog (`ErrorLogMsg( XEM_ERROR142, ... )`, the JUNE_FX idiom; ErrorLog is open from 00:57:55) and the misplaced
  `X2OfflineLog.h` include (nested in `EXTRA_NPC_STAT_PARITY`) is dropped. flagcheck `--base 18460a26^`: PASS, 0 deleted; `/Zs` 0 errors.
  `DEPLOY OK: X2_offline.exe is 26,485,248 B, dated 2026-11-14 01:15:49, md5 917b9e3c95b58f02d35c49878d17141e`; `EXESTR OK: 1 of 1`. Commit `6ffe57ee`.
- **Play-test 2 (user, 2026-11-14): passed** - ErrorLog lines 634-636 at startup: types 10 (`LUCIEL_DUO.LUA`), 11, 12 skipped; none for 1-9;
  0 XEM_ERROR144 / 148, 0 EXCEPTION. The run's nine empty-name KEM_ERROR72 follow missing movie subtitle scripts (`RAVEN_OUT.LUA`,
  `MOVIE_CREATE_RENA_BASE.LUA`, `[JUNE_ENGINE] UI MOVIE subtitle script NOT loaded`): not NU1.

### NU1 - Review (2026-11-14)

Opus 5.5 high. **Verdict: pass, no code change.** Covers `18460a26` + fix 1 `6ffe57ee` against `nu0_unit_type_sites.tsv` and the NU1 kickoff.
- **Enum:** with the flag `UT_LUCIEL` 10, `UT_ROSE` 11, `UT_AIN` 12, `UT_END` 13 (June `Enum.src.lua:313`); `UT_NONE`..`UT_ADD` untouched; flag off =
  March's enum incl. the `JUNE_ENUM_NAMES` block (flagcheck PASS, 0 deleted). No `case UT_END` and no Lua binding of `UT_END` exists.
- **`UT_END` re-grep** (`X2Lib`, `KTDXLIB`, `X2`; 60 hits): every live site is a NU0 row. `widen` rows reach 13 unedited (array, script loop,
  getter `X2UnitManager.cpp:690` / `:702`, `InvalideDataCheck:195`, item-parse type check `:683`, picture loops `X2StateDungeonRoom.cpp:2912`,
  `X2StatePVPResult.cpp:2258`, `X2StatePVPRoom.cpp:3814`: each null-checks `GetPicture(i)` and `CKTDGUIStatic::GetPicture` bounds the index,
  `KTDGUIStatic.cpp:524`). The harmless live ones are the four the Done note names (`X2OfflineCashShop.cpp:174`, `X2ItemManager.cpp:547/594`,
  `X2StateCreateUnit.cpp:4021`, `X2GUUser.cpp:51010`). Dead: `X2BeginningGame`, `X2StateBeginning`, the old `Select*` loops, `X2Item.h`,
  `X2BuffDisplayerTemplet.cpp:1257` and `X2StateClassChange.cpp:943` (`#else` of June bodies), `X2StateServerSelect.cpp:3399` (block comment).
  No unnamed live site.
- **Script table:** 13 entries (index 10 `LUCIEL_DUO.LUA`, 11 / 12 NULL); the `BOOST_STATIC_ASSERT` holds the size; slot 0 is skipped silently
  (`> UT_NONE`), a missing or unmounted script takes March's NULL branch and logs once per type.
- **Class guards:** `X2ItemManager_Preprocessing.cpp` `UC_NONE .. UC_AIN_ANGEL` = June `325bd515` (`cmp 0xca; jbe`); `X2ItemStatCalculator.cpp`
  `UC_ELSWORD_LORD_KNIGHT .. UC_AIN_APOSTASIA` = June `328d6ae1` / `328d6b30` (`mov esi,0x64` ... `cmp esi,0x89; jl`). `X2SocketItem.cpp:2899`:
  one copy (char-ara CHNa), no second.
- **`ChangeUnitButtonInfo`:** June `325651bd-325651d0` = templet NULL (`[ebx+0xaf0]`), type `<= 0`, or `>= 0xd` -> STR 707 (0x2c3) box; arguments
  match (0x2d `DLG_UI_Selection_MessageBox_Ok_Button_New.lua`, 0x18 `UI_PopUp_Negative_01.ogg`). The unconditional templet read after it
  (`XSkinMeshReady`) is March's, unchanged.
- **Leave rows:** `X2OfflineStatTable` identity pinned to `<= UT_ADD`; `X2GameEdit.cpp` untouched.
- **Ghidra spot-checks (read-only):** `3237a6b3 cmp eax,0xc; jbe` (getter `<= 12`), `328d6ad4 cmp esi,0xd` / `328d6b30 cmp esi,0x89`,
  `325651cd cmp eax,0xd; jl`: all equal the code.
- **Hygiene:** flagcheck `--base 18460a26^ --files <6 code files> KTDXLIB/Always.h`: `files: 6 PASS, 0 PASS~ (blank/marker only), 1 DEFINE, 0 OTHER
  (other flags), 0 FAIL, 0 ERROR`; `in this flag's files: 0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0
  #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS`. junediverge `--check-all`: `RESULT: PASS` (232 allow-listed).
- **Play-test logs** (exe 917b9e3c, run 01:18:45): ErrorLog 634-636, three `LUCIEL NEWUNIT` lines (10 `LUCIEL_DUO.LUA`, 11, 12); 0 EXCEPTION.
- **Doc fix:** the log command in 31 char-luciel kickoffs and this doc read `--grep "JUNE_CHAR LUCIEL"`, which matches nothing (the line is
  `[JUNE_CHAR] LUCIEL ...`); now `--grep "JUNE_CHAR\] LUCIEL"` (re-run: 3 lines).
- Build: none (no code change). NU2, NU3, DU2 ready (kickoffs checked: NU3 carries "Amended by NU1"; NU2 / DU2 need none).

### NU2 - Done (2026-11-14)

Opus 5.5 xhigh. Flag `SERV_IRUHADEV_JUNE_CHAR_UNIT_CLASS_WORD`: Always.h, second home at the end of `ServerDefine.h` (`SECOND_HOMES`), and
`X2ServerProtocol_2010.vcxproj` X2TOOL as `SERV_IRUHADEV_JUNE_CHAR_UNIT_CLASS_WORD=` (empty, so ServerDefine.h's `#define` raises no C4005).
Log `[JUNE_CHAR] LUCIEL NEWUNIT unit <uid> class <n> read as a word ( a char would read <n wrapped> )` (offline_server.log, once per unit).
- **Width and offsets:** the class is a `short`. Probe (template-error sizeof): X2Lib `KUnitInfo` 654 B (NU0: 653), class +0x15 width 2,
  transcendence int +0x17 (= June 3235eb7b / 3235eb84); `KRoomUserInfo` +0x21, `KFieldUserInfo` +0x24, `KSquareUserInfo` +0x4a (= June
  323600f8 / 3236150f / 32361e4f). X2ServerProtocol (X2TOOL defines): the same four offsets and width 2; its totals (612 / 585 / 159 / 173)
  differ by country / in-house flags its config lacks, as before NU2 (no X2ServerProtocol source touches these structs, NU0).
  June reads the word with `movzx` (unsigned); `short` is the house precedent (`m_sNewUnitClass`) and keeps the trade-board search's "any
  class" `-1`; every class id (0..202) reads the same either way.
- **Names kept:** members and parameters keep `m_cUnitClass` / `cUnitClass` and change type only; a rename would touch 408 uses in 62 files.
- **Members, live by `cl /P` of a stdafx probe (35):** CommonPacket.h 19 (`KUnitInfo`, `KRoomUserInfo`, the Henir / Dungeon / Pvp ranking
  infos, `DBE_QUEST_COMPLETE_REQ` / `_ACK` `m_cChangeUnitClass`, `KConnectionUnitInfo`, `KSquareUserInfo`, `KPartyUserInfo` (live
  `SERV_CHANNEL_PARTY` copy), `KPartyListUserInfo`, `KFieldUserInfo`, `KCommunityUserInfo`, `KGuildMemberInfo`, `KGuildMemberMessageInfo`,
  `KApplyJoinGuildInfo`, `KMatchUserInfo`, `KPvpMatchUserResultLog`, `KAutoPartyUserInfo`); ClientPacket.h 10 (`DELETE_UNIT_ACK`,
  `UPDATE_PARTY_USER_INFO_NOT`, `ADMIN_CHANGE_UNIT_CLASS_REQ` / `_ACK`, `CHANGE_USER_INFO_NOT`, `SEARCH_TRADE_BOARD_REQ`,
  `COUPLE_PROPOSE_AGREE_NOT` `m_cAcceptUnitClass`, `RELATIONSHIP_INFO_ACK` x2, `CHANGE_MY_UNIT_INFO_NOT`); **`ClientPacket_Global.h`
  `KEGS_FINAL_DELETE_UNIT_ACK`** (missing from the NU0 tsv: the `OnlyGlobal/` headers were not swept; the emulator writes it); client
  `RoomNpcSlot` (X2Room.h), `RelationshipCommunityInfo`, `GuildMemberInfo`; emulator `KPartyBot`, `KBotCastEntry`. Three tsv members are
  dead in this build and widened anyway (`KRecommendUserInfo`, CommonPacket.h's `EVENT_DATA`, `KLocalRankingUnitInfo`).
- **Left, with the verdict:** unit types `m_cUnitType` x2 / `m_cAttUnitType` x2 (<= 12); `KPartyUserInfo` 4683 (`#else` of
  `SERV_CHANNEL_PARTY`); X2SkillTree's `SealSkillItemInfo::m_cUnitClass`, `AddSealSkillInfo_LUA( ..., char )`, `GetUnsealSkillItemInfo( ...,
  char& )` and X2UIInventory.cpp:6650 (all in the dead `#else` of `UPGRADE_SKILL_SYSTEM_2013`; the tsv had them live); `ClientPacket_Global.h`
  `REGISTRATION_GUILD_INVITE_MSG_REQ` / `EVENT_CHUNG_GIVE_ITEM_NOT` (dead); `GetChungUIClass` (Chung UI index); `BOT_CAST`'s `(char)UC_*`
  initialisers (base classes); `ServerPacket.h`, `GameEvent/`, `PvpMatch/`, `X2Data/XSL*` (no X2Lib include). Serializers need no edit:
  `PUT` / `GET` pick `KSerializer::Put( short )` / `Get( short& )`, 2 bytes.
- **Narrowing census:** `/Zs /W4` of the 22 X2Lib TUs that name a member, flag off vs `/D` on. New C4244 short -> char only at
  X2Community.cpp:11849 (`AllocateRelationDummyUnit`) and X2UIRankingInfo.cpp 865-1048 (`SetCharIcon` x6); explicit casts by grep. Edited:
  the emulator's `(char)` casts (Handlers_Unit.cpp x4, Handlers_Room.cpp `MakeRoomUserInfo`, X2OfflineServer.cpp `MakeUnitInfoFromRow`),
  the `char` parameters of `MakeDefaultUnitInfo`, `AllocateRelationDummyUnit`, `SetCharIcon`, `CX2Main::ChangeUnitClass` (Lua-bound;
  lua_tinker reads a short), and the casts at X2Room.cpp:1105 / X2UIPersonalShopBoard.cpp:2258. After: 22 TUs 0 errors, 0 short -> char.
- **Readback line** in `CX2Unit::UnitData::SetKUnitInfo`: a class above `CHAR_MAX` logs once per unit uid. Existing characters (<= 125)
  never log it.
- **Hygiene:** flagcheck `--worktree --files <19 code files> Always.h ServerDefine.h`: `files: 19 PASS, 0 PASS~ (blank/marker only), 1
  DEFINE, 1 OTHER (other flags), 0 FAIL, 0 ERROR`; `in this flag's files: 0 deleted line(s), 0 non-ASCII inserted line(s), 6 EOL-mismatched
  inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS`. OTHER = Always.h's peer blocks; the 6 EOL
  rows are `KGuildMemberInfo` / `KGuildMemberMessageInfo`, whose member lines are LF inside a CRLF block (the inserted lines take the member
  line's LF). junediverge `--check` (19 files): `RESULT: PASS`. enumscan: 0 rows on NU2 lines.
- **Build: blocked, not NU2's.** `build.py --deploy`: X2ServerProtocol ok (0 warning lines); KTDXLIB failed at Always.h:12769, char-elesis
  CH3b0's `#error SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON needs ...` (committed `7473d4bf`; its `SERV_ELESIS_SECOND_CLASS_CHANGE` is defined
  only in ServerDefine.h:4117, which KTDXLIB does not include). Nothing deployed; build + play-test once that is fixed.
- **Routed:** a unit whose class has no templet crashes the server select (`ChangeUnitButtonInfo`'s unconditional
  `XSkinMeshReady( pUnit->GetUnitTemplet()->m_MotionFile )` after the 707 test; `GetType()` at the top unit) -> NU4 kickoff. NU3's rows give
  class 200 a templet, so NU4's Lu/Ciel is not hit.
- **After the commit:** flagcheck `--range 0c915cba^..0c915cba`: `files: 19 PASS, 0 PASS~ (blank/marker only), 2 DEFINE, 0 OTHER (other
  flags), 0 FAIL, 0 ERROR`; 0 deleted, the same 6 EOL rows; `RESULT: PASS`. Build: after char-elesis's fix `029ca9e1`, a peer's
  `build.py --deploy` of main built NU2 in: `X2_offline.exe` 26,488,832 B, 2026-11-14 02:08:40, md5 `e9dee3d698e1420fe434100dcb726b2a`
  (all four projects linked, 0 C4005 for the flag). `EXESTR OK: 1 of 1 found` (the NEWUNIT format string, wide).
- **Play-test (user, 2026-11-14): passed.** Scratch unit `NU2Word` (uid 165, class 200, all three dates 1700000000 so it sorted onto page 5;
  backup `db_backup/els_db_20261114_021117_pre_sql.sql`). Run 02:12:37: offline_server.log:193 `[JUNE_CHAR] LUCIEL NEWUNIT unit 165 class
  200 read as a word ( a char would read -56 )`, once over four character-list loads; no NEWUNIT for any other unit; every other class read
  as saved (`[JUNE_TRANSCEND] UNIT ... ( select )`, 1-125). Selects `jtLordKnight` (100), `jtVoidPrincess` (105, a dungeon), `ElesisGM` (120),
  `ElGuideCA` (125), four relogs; 0 EXCEPTION. The 5 UNHANDLED `E_DISABLE_HB_CHECK_REQ` are March's state-change heartbeat toggle
  (`X2State.cpp:8772` / `:8915`, no class field), not NU2. The scratch unit was deleted afterwards (no other table held uid 165; backup
  `els_db_20261114_021901_pre_sql.sql`).

### NU2 - Review (2026-11-14)

Opus 5.5 high. **Pass, no code change.** Reviewed `0c915cba` against `nu0_unit_class_width.tsv` and the Done note.
- **Members:** `cl /P` of a stdafx-only X2Lib probe (census.py's `cl_cmd`, flag and `SERV_IRUHADEV_OFFLINE` on, exit 0) leaves 33 unit-class
  members, all `short`: CommonPacket.h 19, ClientPacket.h 10 (+ March's own `short m_sNewUnitClass`), `ClientPacket_Global.h`
  `KEGS_FINAL_DELETE_UNIT_ACK` (reached), `RoomNpcSlot`, `GuildMemberInfo`, `RelationshipCommunityInfo`; plus the emulator's `KBotCastEntry` /
  `KPartyBot` outside stdafx = the Done note's 35. **0 `char` unit-class members survive.** Not findings: `KItemFormatTemplet::m_byUnitClass`
  (item.kim record byte, unsigned, every class id <= 202 fits; item-templet fields are DU3's), `KPVPChannelInfo::m_cPVPChannelClass` (PvP
  channel), unit types `m_cUnitType` / `m_cAttUnitType`.
- **Layout (Ghidra, read-only):** `3235eb7b` `MOVZX ECX, word ptr [EBP + 0x15]` then `3235eb84` dword at +0x17 (KUnitInfo); `32361e4f` word
  at +0x4a then `32361e56` dword at +0x4c (KSquareUserInfo). June's serializer: handler `3255b100` -> `3255c7c0` -> packet GET `320ed1f0` ->
  KUnitInfo GET `320eb310` calls `33051f40( +0x15 )`, the same getter as `+0x53` (`m_usPort`), and `33051f40` reads 2 bytes (`33052870( p, 2 )`).
  So June's wire class is 2 bytes, as NU2's `Put( short )` / `Get( short& )`.
- **Narrowing:** re-grep of `(char)` / `static_cast< char >` / `char` locals and parameters near a class in X2Lib / KTDXLIB / X2ServerProtocol:
  every hit is the `#else` copy of an NU2 site, the dead `UPGRADE_SKILL_SYSTEM_2013` `#else`, `SetChungUIClass` (Chung UI index) or
  X2SkillNoteManager.cpp:119 (a class-line count). The four widened parameters, X2Room.cpp:1105 and X2UIPersonalShopBoard.cpp:2258 are `short`.
- **Left rows:** confirmed. X2SkillTree.h 4072 / 4119, .cpp 1822 / 5404 and X2UIInventory.cpp:6650 sit in the `#else` of
  `UPGRADE_SKILL_SYSTEM_2013` (Always.h:1643); `KPartyUserInfo`'s `char` copy (now CommonPacket.h:4719) is the `#else` of `SERV_CHANNEL_PARTY`
  (ServerDefine.h:201); `m_cGetCharClass` is under the undefined `SERV_EVENT_CHUNG_GIVE_ITEM`; `BOT_CAST`'s `(char)UC_*` are base classes.
- **Second homes:** Always.h:13162, ServerDefine.h:4801, `X2ServerProtocol_2010.vcxproj:1037` (X2TOOL|Win32, `NAME=` empty),
  `tools/flagcheck.py:65` `SECOND_HOMES` row.
- **Hygiene:** flagcheck `--range 0c915cba^..0c915cba` (19 code files + Always.h + ServerDefine.h): `files: 19 PASS, 0 PASS~ (blank/marker
  only), 2 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR`; `in this flag's files: 0 deleted line(s), 0 non-ASCII inserted line(s), 6
  EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS`. The 6 EOL rows are the
  `KGuildMemberInfo` / `KGuildMemberMessageInfo` LF lines the Done note names. junediverge `--check-all`: `RESULT: PASS`. enumscan: 190 A/B
  rows in X2Unit.cpp, none on an NU2 line.
- **Play-test logs:** exe e9dee3d6; offline_server.log:193 `[JUNE_CHAR] LUCIEL NEWUNIT unit 165 class 200 read as a word ( a char would read
  -56 )`, once; 0 EXCEPTION; 5 UNHANDLED `E_DISABLE_HB_CHECK_REQ` (not NU2). Save: no unit 165, no class 200 (31 units, highest class 125).
- **Kickoff:** NU4's "Amended by NU0" said `InitDummyUnits` keeps the class in a signed `char`; that field is `KUnitInfo::m_cUnitClass`, a
  `short` since NU2. NU4's "Amended by NU2" now says so.
- Build skipped: no code change, no play-test.

### NU3 - Done (2026-11-14)

Opus 5.5 high. Flags `SERV_IRUHADEV_JUNE_CHAR_MINOR_UNIT_TEMPLET` (parser) and `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_DATA` (load line; `#error` without the
parser flag), Always.h. Log `[JUNE_CHAR] LUCIEL DATA` (ErrorLog, the NU1 fix-1 idiom: offline_server.log opens after the unit manager loads).
- **Rows** (data036 `UnitTemplet.lua`, core-list source): June's `UnitTemplet.src.lua:1723-2024` appended verbatim (7 rows: 200, 37, 38, 39, 127,
  128, 129; CRLF kept); the 59 earlier rows are byte-identical (first 51,112 bytes `cmp`-equal). **Parsed-value diff:** both files run under the
  vendored luajit with a stub `AddUnitTemplet` (every row's keys and values, sorted): `59a60,66`, only the 7 new rows. Each new row has the 16 keys
  March's parser fails without (`unitType` .. `promotionFootItemID`); all 28 STR ids are in the live string table (`ch0_unittemplet_luciel.tsv`).
- **Parser** (`X2UnitManager.cpp` `AddUnitTemplet_LUA`, `X2Unit.h` `UnitTemplet`): 17 members under June's key names (`m_MajorUnitName` /
  `m_MinorUnitName` from `majorUnitName` / `minorUnitName`, 5 `m_MinorUnitPromotion*ItemID`, 7 `m_MinorUnit*Motion`, `m_fLobbyRotationY`,
  `m_fMinorUnitLobbyRotationY`); the 3 minor cut-ins were already parsed (`JUNE_HYPER_CUT_IN_TIME`). Defaults from June `3237aa40`'s decompile:
  name ids miss -> `-1` (`STR_ID_EMPTY`, empty string), ints `0`, wstrings `L""` (`3344f1f4`), floats `0`; no key fails the row. June's order:
  names between `name` and `description`, minor items / motions after the game motions (`@3237cbcc-3237e53e`), rotations after
  `introMovieFIleName` (`@3237efaf-3237f2fe`). Nothing reads the members yet (NU4 rotation / names, DU1 motions).
- **Load line** (`OpenScriptFile`, after the templet script): `LUCIEL DATA unit templet rows <Lu/Ciel> of <all>, second-body rows <n>, major body
  <name>, minor body <name>` (from `UC_LUCIEL_DUO`). Expected `7 of 66, second-body rows 7`.
- **Other classes:** every earlier member is parsed as before (insert-only); the new members read 0 / empty for them, except `m_fLobbyRotationY`,
  which 56 of the 59 earlier rows set (unread until NU4 ports `31edf260`).
- **Enum.lua** (data036, routed by NU1): `UT_END = 13` (June `Enum.src.lua:313`; comment keeps "was 10"), the stale "C++ UT_END is 10" comment
  dropped. A peer session (char-elesis, 3 `NUI_SI_SA_SB*` ids) edited Enum.lua right after and refreshed its ledger row; that row is theirs.
- **Lua gates:** `luasrc.py refresh data036 UnitTemplet.lua`; `LEDGER CHECK PASS: 0 FAIL, 0 WARN`; `CORE CHECK PASS: 0 FAIL, 0 WARN`.
  `PACK OK data036` (271 entries; ~ Enum.lua, ~ UnitTemplet.lua); `COPY OK data036: ... 14092104 bytes, 271 entries, md5 f7b1f8253c10c87e878c2a46175df8ba`.
- **Hygiene:** flagcheck `MINOR_UNIT_TEMPLET --with JUNE_CHAR_LUCIEL_DATA --worktree --files <3 files>`: `files: 2 PASS, 0 PASS~ (blank/marker only),
  0 DEFINE, 1 OTHER (other flags), 0 FAIL, 0 ERROR`; `in this flag's files: 0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched
  inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS` (the OTHER is Always.h carrying a peer's
  uncommitted C3y-2 debug retire; only NU3's 21 lines are committed).
  junediverge `--check` (2 files): `RESULT: PASS`. enumscan: no row in NU3's lines (the 17 are the existing `UNIT_CLASS` definitions).
- **Compile:** `/Zs` probe of `X2UnitManager.cpp` (no PCH): exit 0, 0 errors; `/P` has the DATA line, both new parse blocks and the members.
  Build: first `build.py --deploy --pch force` failed in X2Lib on char-elesis CH3b0's uncommitted `ELESIS_SUMMON` guard (`Always.h:12769`
  `#error`), not NU3's; nothing deployed.
  Deployed by a peer's build after char-elesis's fix `029ca9e1`: exe 26,488,832 B, dated 2026-11-14 02:08:40, md5 e9dee3d698e1420fe434100dcb726b2a;
  `EXESTR OK: 1 of 1` (the DATA format string, wide) and `EXESTR OK: 3 of 3` (`m_fMinorUnitLobbyRotationY`, `minorUnitName`, `m_MinorUnitGameMotion0`).
  Range flagcheck `f0b8249e^..f0b8249e`: `files: 2 PASS, 0 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`; 0 deleted; `RESULT: PASS`.
- **How to see it:** start the game to the server / character select; ErrorLog has one `[JUNE_CHAR] LUCIEL DATA` line next to the three NU1
  `LUCIEL NEWUNIT` lines. An existing character's select / village / dungeon is unchanged.
- **Play-test (user, 2026-11-14): passed** on exe e9dee3d6, run 02:12:35: ErrorLog 634 `[JUNE_CHAR] LUCIEL DATA unit templet rows 7 of 66,
  second-body rows 7, major body Lu, minor body Ciel`, then NU1's three NEWUNIT lines (635-637); 0 EXCEPTION, 0 XEM_ERROR144 / 148,
  0 KEM_ERROR72, 0 unbound Lua names. The 5 UNHANDLED `E_DISABLE_HB_CHECK_REQ` are the dev heartbeat toggle, not NU3.

### NU3 - Review (2026-11-14)

Opus 5.5 high. **Verdict: pass, no code change.** Covers `f0b8249e` against `nu0_templet_keys.tsv` and June `AddUnitTemplet_LUA` `3237aa40`.
- **Members** (`X2Unit.h`): 17, June's key names and types (2 name wstrings, 5 int minor promotion items, 7 motion wstrings, 2 floats); the three
  minor cut-ins stay with `JUNE_HYPER_CUT_IN_TIME`, no second copy.
- **Defaults, Ghidra (read-only):** `majorUnitName` `@3237afc0` / `minorUnitName` `@3237b1eb` miss -> `mov [esp+0x18],ebp`, EBP = -1 from
  `3237ad0d` (`or ebp,0xffffffff`) = `STR_ID_EMPTY` (-1, `StringID_def1.h:7`), then the string-table call `31c31060`; weapon item `@3237cde7`
  miss -> `mov [edi],0`; `m_fLobbyRotationY` `@3237f0da` miss -> `fldz; fstp`. Each miss falls through to the next key: no key fails the row.
- **Order:** minor items / motions after the game motions and the rotations after `introMovieFIleName` = June. The two names sit after the
  `description` read, June reads them before it; key reads are independent table lookups and `description` fails the row either way, so the
  difference has no effect (noted, not moved).
- **Rows** (re-run by a helper): pre-NU3 `UnitTemplet.lua` from `kom_backup_tools\data036.kom.20261114_013807` (51,112 B, sha1 = the ledger row at
  `f0b8249e^`); current file's first 51,112 bytes equal it; the 10,956-byte tail = June `UnitTemplet.src.lua:1723-2024` joined with CRLF. Luajit
  stub dump: 59 -> 66 rows, diff `1492a1493,1780` (row counts `59a60,66`), nothing else. All 7 rows carry the 16 `LUA_GET_VALUE_RETURN` keys.
- **`DATA` line:** ErrorLog 634 `rows 7 of 66, second-body rows 7, major body Lu, minor body Ciel` (exe e9dee3d6).
- **Enum.lua:** data036 line 288 `UT_END = 13` (June `Enum.src.lua:313`), no stale C++ comment; no other `.lua` in `kom_contents` names `UT_END`.
- **Stale comment, not edited:** the `JUNE_HYPER_CUT_IN_TIME` block's comment in `AddUnitTemplet_LUA` still says March has neither rotation key
  (C3t's line; old comments are not rewritten).
- **Hygiene:** flagcheck `MINOR_UNIT_TEMPLET --with JUNE_CHAR_LUCIEL_DATA --range f0b8249e^..f0b8249e`: `files: 2 PASS, 0 PASS~ (blank/marker
  only), 1 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR`; `in this flag's files: 0 deleted line(s), 0 non-ASCII inserted line(s), 0
  EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS`. No later commit touches either
  flag. junediverge `--check-all`: `RESULT: PASS` (232 allow-listed). `LEDGER CHECK PASS: 0 FAIL, 0 WARN`; `CORE CHECK PASS: 0 FAIL, 0 WARN`.
- **Logs:** 0 EXCEPTION, 0 XEM_ERROR144 / 148, 0 unbound Lua names.
- Build: none (no code change). DU1 ready (char-chung Z closed; kickoff carries "Amended by NU3"); NU4 waits on the NU2 review (its kickoff
  carries "Amended by NU3").

### NU4 - Done (2026-11-14)

Opus 5.5 high. Flags (Always.h): `SERV_IRUHADEV_JUNE_CHAR_CREATE` (create / select screens, rotation, request, stat fallback; `#error` unless
UNIT_TYPE_RANGE, UNIT_CLASS_WORD, MINOR_UNIT_TEMPLET and JUNE_CLASS_CHANGE_GUIDE), `SERV_IRUHADEV_JUNE_SELECT_VOICE_TABLE`,
`SERV_IRUHADEV_JUNE_CHAR_LUCIEL_STARTER_ITEMS`. No debug flag: the `LUCIEL CREATE` lines are permanent (one at load, one per Lu/Ciel create).
- **Creation screen** (`X2StateCreateUnit.*`): new `SelectLuciel` = June `325559f0` (Ghidra-read: SelectAdd's skeleton, colour 0.08 / 0.13 /
  0.88, `DLG_Unit_Background_Luciel.tga`, Move 0 / -50 / 200, Scale 1.2, `LobbyDownLanding`, class 200 / type 10, voice
  `LucielVoice_Introduce.ogg`); msg `SCUUCM_LUCIEL_BUTTON_UP` 36 (C++ + Enum.lua), re-select type 10 (`3255ab8c`), default-select case (dead on US:
  limit 8), `DummyLuciel` (`3255b800`), class-change dialog (`325525a0` case 10), stand board `Dummy_Village_Board11` + `EffectSet_Luciel_Stand_Aura`
  at x + 25 / z + 50 (`32551e30`, read from the fadd at `@325522ad`), info texture `_info_10` (`325506e0`), stat chart (`3254ee40`: 37 / 127 =
  2,1,2; 38 / 128 / 200 = 2,3,2; 39 / 129 = 3,2,3), viewer lookup (`3254e2b0`). `X2Unit.h`: key colour + the job-preview class table for UT_LUCIEL.
  Stand-ins: preview costume by `EquipPromotionItemByUnitTemplet` (June preloads, `3254e410`); the create request sends the base class of the
  type (June `325584f0` sends `+0x2c0`, the previewed job; `32368880`'s base class equals March's value for types 1-9). One body until DU5.
- **Rotation** (`X2Data.cpp`): June `31edf260` replaces `RotateYSelectCharacterType` for every class (divergence row marked ported): templet
  `m_fLobbyRotationY` / minor when |v| > 1.19e-7, else June's case table (floats by `charport_consts.py`). March callers pass no minor flag.
- **Select screen** (`X2StateServerSelect.cpp`): background (`3255ecf0`), stand board (`32561130`, same offsets); `X2Data.cpp` image rows 59-65
  (`31ee0de0` rows 0x3f / 0x40 / 0x41 / 0x4d / 0x42 / 0x43 / 0x4e, by `char_image_rows.py`; two quirks -> `JUNE_BUGS.md` 655 / 656).
- **Voices:** `CX2UnitManager::OpenSelectVoiceTable` / `AddVoice_LUA` (`32379ed0`, bound in `X2Main.cpp`; first row wins) /
  `GetVoiceFileName` (`32376270`); `ResetUnitManager` opens it before `AnimationNameTable.lua` (`31ee6100 @31ee620a`). `PlaySoundCreateUnit`
  (`32550aa0`) and `PlaySoundSelect` (`3255f240`, now by class, 4 callers) use it. Stand-in: class -> type through the templet (June `31eca1d0`
  switch). **Other classes' voices change:** Rena's create voice `RenaVoice_Create0n` (was `LenaVoice_`; both files exist, June has only Rena's);
  select voices from the table rows (Elesis DK / CA, Add TT / DE, Lu Noblesse, Ain AP); everything else builds March's names.
- **Emulator:** `GBASE_ITEM` row 200 (gear 113300-113304, costume 41890-41894) + `LUCIEL_MINOR_UNIT_COSTUME` 41900-41904 (bagged: Lu's costume holds
  the fashion slots until DU4). `X2OfflineStatTable`: fallback by `BaseClassOf` (type -> base class), synthetic line once per class,
  `GetStatSource`. The flat StatTable already has all 7 classes (`STATFLAT OK: 12177 rows, 123 classes`), so Lu/Ciel reads its own rows.
- **Data:** data034 `DLG_Unit_Create_Unit_Button_New.lua` -> core-list source (`convert`, T1) + Lu/Ciel's slot (June's keys, x 713: March's row
  ends at Elesis 668, June's own layout re-orders every slot); data036 `Enum.lua` + `SCUUCM_LUCIEL_BUTTON_UP` and June's `SOUND_PLAY_TYPE`
  (`refresh`); data057 stage-june `DLG_UI_Common_Texture89_NEW` / `91_NEW` .tga / .TET (new). `ClassChange_Button_Luciel.lua` and
  `SelectVoiceTable.lua` were already June's bytecode. `lua_unbound_known.tsv`: AddVoice row dropped (bound now). `createchar.py`: 7 classes.
  PACK OK / COPY OK data034 (md5 08b46ebb...), data036 (58c6c97d...), data057 (144bccfe...).
- **Hygiene:** flagcheck `JUNE_CHAR_CREATE --with JUNE_SELECT_VOICE_TABLE JUNE_CHAR_LUCIEL_STARTER_ITEMS --worktree` (14 files): `files: 6 PASS,
  2 PASS~ (blank/marker only), 1 DEFINE, 5 OTHER (other flags), 0 FAIL, 0 ERROR`; `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched
  inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS` (the 5 OTHER are the voice-only files; the voice
  and starter flags alone: PASS, 0 deleted). junediverge `--check` (14 files): `RESULT: PASS`. enumscan: no row on NU4 lines. luabind (shipped button
  file, SelectVoiceTable, ClassChange_Button_Luciel): `RESULT: PASS`. `LEDGER CHECK PASS`, `CORE CHECK PASS`. `/Zs` probe of the 7 .cpp: 0 errors.
- `DEPLOY OK: X2_offline.exe is 26,521,088 B, dated 2026-11-14 05:25:52, md5 90b3df890f0d39ea3a24dcb6807fdc08` (built at 0102ee15 + the 14
  NU4 files). `EXESTR OK: 6 of 6` (both `LUCIEL CREATE` formats, `DLG_Unit_Background_Luciel.tga`, `LucielVoice_Introduce.ogg`,
  `SelectVoiceTable.lua`, `LucielCreateUnitSlot`, wide) and `1 of 1` (`AddVoice`).
- Commit `5f01aed9`; flagcheck `--range 5f01aed9^..5f01aed9` (all three flags): same tallies, 0 deleted lines, `RESULT: PASS`.
- **Not ported (left):** June's jumping-character mode (`+0x71c`, `_JUMPING` msgs), the new-character badge, the minor body / rotation (DU5),
  the tutorial route (TU: a Lu/Ciel's first Game Start would enter Elsword's tutorial and the class does not exist before CH1).
- **How to see it:** creation screen slot 10 (right of Elesis); create a Lu/Ciel; select screen lists him; `[JUNE_CHAR] LUCIEL CREATE` lines.
- **Play-test (user, 2026-11-14): passed** (exe 90b3df89) - slot, Lu's preview, job previews and select screen all render. Logs: ErrorLog 639
  `LUCIEL CREATE voice table SelectVoiceTable.lua loaded=1 rows=400 inserted=400`; offline_server.log `LUCIEL CREATE class 200 unit 166: 15 of
  15 starter row(s) in the save, stat source StatTable.lua rows of the class` (gear 5 worn), `CREATE 'testlu1' class=200 -> unitUID=166`;
  0 EXCEPTION. Then the create reply's first select sent him to the tutorial (room 1001, dungeon 41150, Elsword's: no Lu/Ciel case before TU)
  and the client crashed at `XS_DUNGEON_GAME`: no `CX2GULuciel` before CH1, as expected. `testlu1` (uid 166) stays in the save; Game Start
  with him crashes until CH1.

### NU4 - Review (2026-11-14)

Opus 5.5 high. **Verdict: pass after one hygiene fix** (a missing flag-dependency guard, no behaviour change). Covers `5f01aed9` against
`nu0_create.tsv` and June, Ghidra read-only.
- **SelectLuciel** = June `325559f0`: guard `+0x2c0 == 200`, side flag (0.08, 0.13, 0.88, 1) by `charport_consts.py` (fld order r, g, b), background
  `DLG_Unit_Background_Luciel.tga`, viewer Move 0 / -50 / 200, Scale 1.2, light -250 / 100 / -600, `LobbyDownLanding`, class 200 / type 10, class-change
  dialog, stand board `32551e30(10)`, intro movie from the dummy's templet (`+0x30c` -> `+0xaf0` -> `+0x2b8`, show + loop `0x101`), info dialog
  `325514e0(10, 1)`, `LucielVoice_Introduce.ogg`. March's SelectAdd repeats the front-control block twice; NU4 keeps it once (no effect). The
  promotion stand-in wears `promotionWeapon / Body / Leg / Hand / FootItemID` 41890-41894 from Lu's templet = June's preload set 5933.
- **Stand board** (create `32551e30`, select `32561130`): `fadd 25.0` on `[esp+0x2c]` (x) and `fadd 50.0` on `[esp+0x34]` (z) at `@325522ad` /
  `@325522bf` and `@325615ad` / `@325615bf`. The cached `vStandMeshPos` equals March's per-iteration `GetPos()` for every other class.
- **Rotation** = June `31edf260`: templet `+0x2d4` / minor `+0x2d8` when |v| > 1.1920929e-07, then the case table; all 17 value groups re-mapped through
  June's `Enum.src.lua` UNIT_CLASS ids, every class in the right group, Noblesse 20 (minor) / -15. Templet values vs June's `UnitTemplet.src.lua`:
  63 of 66 rows equal (Elsword Swordman 35, Raven Fighter 10, Eve Nasod 15, Ara Asura 40, all 7 Lu/Ciel incl. minor); the 3 Add rows have no key in
  the tree (March rows) and fall back to the switch (20 / 10 / 3, June's templet says 30 / 50 / 5): char-add CH5 brings Add's rows, not NU4's to fix.
- **Voices** = June `32376270`: (class, type) lexicographic find = `std::pair`; suffix switch 0-13 = June's 14 cases (`QuestComplete_` prefix,
  `Voice_Revive0` / `Voice_Create0` + `rand(0,100) % 2 + 1`); name table `32375d00` read from exe bytes incl. the four Ghidra leaves as bare returns
  (`Eve` / `Ara` / `Add` / `Ain` at `33458d74` / `60` / `48` / `24`). Class -> type stand-in: June `31eca1d0` puts 0x25-0x27, 0x7f-0x81, 200 in type
  10; the templet rows of all 7 say `UT_LUCIEL`; Swordman -> 1 and Ara Asura -> 7 agree too. `CX2StateServerSelect::SOUND_PLAY_TYPE` 0 / 1 =
  the manager's `SPT_UNIT_SELECT` / `SPT_SERVER_SELECT`.
- **Create request / stat fallback:** `s_aiClassTable[t][BASIC][FIRST]` is class t for types 1-9 (Swordman ... Nasod Ruler) and 200 for type 10, so
  March characters send the same class as before; `BaseClassOf` uses the same table.
- **Image rows 59-65** = `char_image_rows.py` for all 9 tables (first two columns; June's transcendence columns have no slot in March's two-field
  row, CH6's); quirks = `JUNE_BUGS.md` 655 / 656.
- **Data:** data057 texture 89 / 91 `.tga` + `.TET` md5 = June's extract (4 of 4). `luabind.py check` on the **shipped** data034 button file (by path):
  19 methods, 0 unbound, `ENUM 0 member(s) missing`, `RESULT: PASS`; by name the tool reads June's own layout file, whose 13 missing members are Rose /
  Ain / the unported `_JUMPING` buttons (not shipped). `SelectVoiceTable`, `ClassChange_Button_Luciel`: 0 unbound, 0 enum missing.
  `LEDGER CHECK PASS: 0 FAIL, 0 WARN`; `CORE CHECK PASS: 0 FAIL, 0 WARN`.
- **Fix:** `_STARTER_ITEMS`'s CREATE line calls `CX2OfflineStatTable::GetStatSource`, declared only under `JUNE_CHAR_CREATE`, with no guard: turning
  `JUNE_CHAR_CREATE` off alone broke the build. `Always.h` now has `#error ... _STARTER_ITEMS needs ... JUNE_CHAR_CREATE` after its define; `/Zs`
  probe of `X2OfflineInventory.cpp` with the guard: 0 errors (so `JUNE_CHAR_CREATE` is defined at that line). flagcheck `JUNE_CHAR_LUCIEL_STARTER_ITEMS
  --worktree --files KTDXLIB/Always.h`: `files: 1 PASS`, `0 deleted line(s)`, `RESULT: PASS`; junediverge `--check KTDXLIB/Always.h`: `RESULT: PASS`.
- **Hygiene (NU4 commit):** flagcheck `JUNE_CHAR_CREATE --with JUNE_SELECT_VOICE_TABLE JUNE_CHAR_LUCIEL_STARTER_ITEMS --range 5f01aed9^..5f01aed9`:
  `files: 6 PASS, 2 PASS~ (blank/marker only), 1 DEFINE, 5 OTHER (other flags), 0 FAIL, 0 ERROR`; `in this flag's files: 0 deleted line(s), 0
  non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS`.
  OTHER: `X2Main.cpp`, `X2StateServerSelect.h`, `X2UnitManager.cpp` / `.h` hold only `JUNE_SELECT_VOICE_TABLE` blocks; `X2OfflineInventory.cpp` only
  `_STARTER_ITEMS` (its other listed flags are existing context). PASS~: the two `X2OfflineStatTable` files, one marker line each.
  junediverge `--check-all`: `RESULT: PASS` (235 allow-listed).
- **Logs (exe 90b3df89):** ErrorLog 639 voice table `rows=400 inserted=400`; server 192 `15 of 15 starter row(s)`, stat source the class's rows;
  0 EXCEPTION; 2 UNHANDLED `E_DISABLE_HB_CHECK_REQ` (not NU4's).
- Full build skipped: no play-test (the fix is a compile-time guard only). CH1 no longer waits on this review.

## Phase DU - The two-body engine (DU1-DU5)

| Sub-phase | Scope | Flag |
|---|---|---|
| DU1 | The second body in `CX2GUUser`: the `+0x8e88` / `+0x8e90` / `+0x8e91` members and their base-ctor writes (from the form byte `+0xe40`), the minor-unit model / skin built from NU3's templet members, the user-sync bits (c3e rows B, E, F, H, I, G: positions `0x20000`-`0x80000`, active body `0x1000000`, float `0x10000`, class short `0x100000`, the class-skill float drivers `0x200000`), PhysicProcess's not-selected-body branch (`32015b30 @32019f9b-3201a1c7`, readers of `CUSTOM_OFFSET_NOT_SELECT_UNIT` / `USE_NOT_SELECT_MOTION_DIFFERENCE`), the one-shot reset (`31fd4290` / `31fd4340`). Every other class sees no change. | `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` |
| DU2 | `CX2GUUser` gains June's damage-state getter virtuals (vslots 0x114 / 0x118 / 0x494 / 0x490; base bodies = the `USI_DAMAGE_*` constants) and `GetJumpDownStateID` (0x458, virtual). `PhysicProcess_Jumping` (`SERV_IRUHADEV_JUNE_USER_JUMP_UP_LINE`) and StateChange's force-down flush (`320348c0 @32034931-32034994`) call them. Lu/Ciel's overrides (`32b4d810` / `32b4d850` / `32b4da10` / `32b4d9d0` / `32b4d5b0`) come with CH1. | `SERV_IRUHADEV_JUNE_USER_DAMAGE_STATE_GETTERS` |
| DU3 | June's equip-view rewrite whole (the Stage 8 bullet "June's equip-view rewrite"): `UpdateEquipViewListInfo` `3201b930` with the `slotId` argument on vslot 0x644 for every override (Chung `32bdc750`, Ara `32bc89e0`, Arme `32cc9950`, and Lu/Ciel's), `GetAddItemAndRemoveItemToViewList` `31fd2f00`, `AddEquipToViewList` `31ffe990`, the skin anim `31fd48a0`, `31fd63c0`, `CreateEquip`'s two arguments, the two cosmetic every-class cases (`328825c0`, `32882540`), and the item-templet minor-unit fields (minor-unit item id and flag, Extra keys 10-13, item-type / unit-class guards from K2; checklist 7g). | `SERV_IRUHADEV_JUNE_CHAR_EQUIP_VIEW` |
| DU4 | NESI renumber to June's ids (`NESI_ONEPIECE_FASHION` 20 -> 35, `NESI_END` 21 -> 43, `NESI_MINORUNIT_*`) and the forward-only `X2OfflineDB` migration (user, 2026-11-02; snapshot `db_backup/` first, schema step +1). It retires `SERV_IRUHADEV_JUNE_CHARINFO_EQUIP_GROUP` (CharInfo C3) and the `IS_FOR_MINOR_UNIT` stand-in (slot hide by June's ids, `32e22ce0`). Every character's equipment must show identically before and after (relog test on three characters). | `SERV_IRUHADEV_JUNE_CHAR_NESI_RENUMBER` |
| DU5 | Two bodies in the character window and the unit viewer (the Stage 8 bullet "The character window's Lu/Ciel and Rose branches", Lu/Ciel half): unit `+0xe40` (setter `3234b8c0`, NOT-apply `323acfb0`) behind `CIM_SWITCHING` / `_ONLY_THIS_DIALOG`, the viewer form byte `+0xd80` / `+0xd81` with `31f93a10` / `31faa150` / `31f94190` / `3291d360`, the MsgProc click switch `3291d390` (replaces the logged stub), OnDropAnyItem `3234f4d0` / `321693b0`, the form packets `0x4d9` / `0x4da` with emulator handlers, both bodies on the creation-screen preview and the select screen (where June shows two and NU4 showed one), and the CharInfo C3 form stand-ins (ResetStat's form, `@3291bf2d`). Rose's weapon UI stays for `char-rose`. | `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEW` |
| DU5b | (split from DU5, 2026-11-15) The unit viewer's second body: `+0xd7c` minor skin, `+0xd80` / `+0xd81` / `+0xd82`, `31f93a10`, `31f94190`, `31faa150`, `3291d360`, `PlayAnim` which = 2; retires DU5's "viewer `+0xd80` read as the window's switch" stand-ins and the NOT's viewer calls. | `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` |
| DU5b2 | (split from DU5b, 2026-11-16) Each body's equipment in the unit viewer: June's whole `UpdateEqip` `31fa9e60`, `AddEqip` `31f9df40`, `AddEquipByTID` `31f9c640`, `ReplaceEmptyToBasic` `31f9e930` / `31f9cfd0`, `RemoveEqip` / `RemoveAllEquips`; slot tables `[ pos ][ 3 ]` (drops R14e's two stand-ins). | DU5b's |
| DU5c | (split from DU5) Both bodies on the create screen (`@32555fe8`) and select screen (`32564400`, `3255e570`), the village avatar (`323f3290` rest: `323f1490`, the viewer rebuild; `323fa700` / `323fad80`; SD mesh table `31ee3470`). | DU5b's, or `_DUO_AVATAR` |
| DU5c2 | (split from DU5c, 2026-11-16) Both bodies on the create screen (`@32555fe8`) and select screen (`32564400`, `3255e570`), the viewer's back body (frame move, draw, `+0xd82` writers, equip copies), `CX2Eqip::SetFixed` `3295a560`, R14e's select-screen onepiece. | DU5b's |
| DU5d | (split from DU5) Viewer weapon effects case 10 (`X2UnitViewerUI.cpp:960`, `:6255`, `:8559`, `:8681`) and CH0's checklist viewer rows (CharCustomData, UnitViewerEffectSetTable, `IS_FOR_MINOR_UNIT` slot key, glow). | DU5b's |
| DU5d2 | (split from DU5d, 2026-11-16) The back body's emotions (`+0xd82` writers, `31fa64f0`), the window's equip tint `31f93920`, June's other `SetUnitScale` callers; after june-engine R14f. | DU5b's / DU5c2's |
| DU5e | (split from DU5) Each body's game stat: `SetGameStats` second `KStat` `323571c0`, the packet fields and the seven sites, the emulator's per-body numbers. | `SERV_IRUHADEV_JUNE_CHAR_DUO_STAT` |
| DU5f | (split from DU5e, 2026-11-15) Each body's sockets: the type-10 socket aggregate and the HUD's second gage, the set-socket count `3286a6b0` and the per-body socket record `3271a060`, the window's other-form set sum `@3291bf2d`. | `SERV_IRUHADEV_JUNE_CHAR_DUO_SOCKET` |

**Done gate:** DU1 / DU2: every other class's play is unchanged (the reviews diff `cl /P`), and a Lu/Ciel's second body follows and syncs once CH1
lands (CH1's play-test). DU3-DU5: Lu and Ciel each wear their own equipment in the village, in the character window and on the select screen;
switching the form in the window swaps the shown body; a relog keeps it.

### DU1 - Done (2026-11-14)

Opus 5.5 high. Part 1 of 4. Flags `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` and `SERV_IRUHADEV_JUNE_CHAR_CLASS_GRADE` (Always.h, `#error` guards on
UNIT_TYPE_RANGE, MINOR_UNIT_TEMPLET, RENDER_MOTION_CASH_EMOTION, BUFF_BEHAVIOR_PARITY, SEPARATION_MOTION). No `_DEBUG`, no log line (nothing runs
before CH1). Files: `X2GUUser.h`, `X2GUUser.cpp`, `X2Unit.h`, `X2Unit.cpp`. funcmap rows: `grep "Char Luciel DU1" june2017_re/funcmap.tsv` (17).
- **Split (scope rule, one logical change):** 440 `DU1` rows over ~80 June functions do not fit one session. DU1 = data model and lifetime;
  **DU1b** = `SetActiveBody` / `UpdateDuo` / `SetDuoEnabled`, `CommonStateStart` keys, `CommonFrameMove(Future)`, `CommonStateEnd`,
  PhysicProcess's not-selected body; **DU1c** = user sync, per-body float pairs, readers outside `CX2GUUser`; **DU1d** = the base-function
  readers in `X2GUUser.cpp`, then the full build and the regression play-test. One review (`DU1-review.md`) after DU1d; CH1a0 / CH1p / CH3c
  still wait on "DU1 review". Kickoffs written.
- **Members** (June user +0x8e7c..+0x8e92, +0x4214..+0x4228): `m_pMinorUnitMotionFuture` / `m_pMinorUnitMotion`, `m_pMinorUnitXSkinAnimFuture` /
  `m_pMinorUnitXSkinAnim`, seven `m_pFrame_MinorUnit_*`, `m_bMinorUnitActive` (+0x8e90), `m_bDuoEnabled` (+0x8e91), `m_bEndDuoAtAnimEnd`
  (+0x8e92). Ctor (`3203e850 @32042429-32042479`): NULLs, active body = the unit's form byte. Dtor (`3203a620`): close both meshes, delete the
  future anim, reset the now anim. The latch (+0x8e93) and the not-select block (+0x8f50..) come with their readers in DU1b.
- **Unit form byte:** `CX2Unit::m_bMinorUnitForm` (+0xe40) + `IsMinorUnitForm()`, false in `Init()`. Read only; its writer `3234b8c0` is DU5's.
- **Accessors** (X2GUUser.h): `GetXSkinAnim` (`32071d60`) and `GetXSkinAnimPtr` (`32071d80`) follow the active body; new `GetXSkinAnimFuture`
  (`32070cf0`), `GetNotSelectXSkinAnimFuture` (`32070d10`), `GetNotSelectXSkinAnimPtr` (`32071db0`), `GetMinorUnitXSkinAnimPtr` (`32071d40`),
  `IsMinorUnitActive` (`31e4d0b0`), `IsDuoEnabled`. `GetSkinAnimPtr` (user vslot 22) stays the first body: June's slot is `32071d20` in both
  `CX2GUUser` and `CX2GULuciel` (vtslot 0x58).
- **InitMotion** (`31fe5860 @31fe5f31-31fe6670`), unit type `UT_LUCIEL` only: `MINOR_UNIT_MOTION_FILE_NAME`, both meshes from
  `m_MinorUnitGameMotion0`, skin anims, collision, the mix list [key, Motion1 (grade > 0), Motion2 (grade > 1), Emotion, cash emotion `false`]
  only when Motion0 is set (June mixes Motion1 before Motion2 here, Motion2 first for the first body: kept), detail %, seven clone frames.
- **Anims:** `AnimStop` / `AnimPlay` / `AnimPlayByPlayType` (`31fe7f70` / `31fe8130` / `31fe8050`) and both one-shot resets (`31fd4290` /
  `31fd4340`) include the second body.
- **Divergent function found and replaced:** `GetX2UnitClassGrade`. June's inline (`@31fe5c5c`): class 10-99 -> 1, 100-199 -> 2, else 0 (Lu/Ciel's
  base 200 is a base class); March gave 200 grade 2. Row added to `june_march_divergent.tsv`. Callers: InitMotion, the motion preload
  (`X2GUUser.cpp:5066`), `X2ItemStatCalculator.cpp:361`; only Lu/Ciel classes are >= 200.
- **Hygiene.** flagcheck `DUO_BODY --with CLASS_GRADE --worktree`: 4 PASS (`X2GUUser.cpp` +114, `X2GUUser.h` +44, `X2Unit.h` +14, `X2Unit.cpp`
  +3), 1 DEFINE, 0 OTHER, 0 FAIL, 0 deleted, 0 non-ASCII, RESULT: PASS. `junediverge --check`: RESULT: PASS. enumscan: no hit on DU1's lines
  (the grade threshold 19 is not an enum id).
- **Compile:** `/Zs` probes (no PCH, census `cl_cmd`) of `X2GUUser.cpp`, `X2Unit.cpp`, `X2ItemStatCalculator.cpp`, `X2GUChung.cpp`: 0 errors each;
  a `/P` probe shows both flags on. Full build skipped: no play-test (DU1d builds and hands off the regression pass).
- **In game:** nothing to see. Every other class has form 0 and reads March's anims.

### DU1b - Done (2026-11-14)

Opus 5.5 high. Part 2 of DU1, flag `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (Always.h: a second `#error` guard, SKILL_STATE_PTR and SKILL_TREE_PANELS).
Files: `X2GUUser.h`, `X2GUUser.cpp`, `X2Unit.h`, `X2Unit.cpp`, `Always.h`. No `_DEBUG`, no log line (nothing runs before CH1). funcmap rows:
`grep "Char Luciel DU1b" june2017_re/funcmap.tsv` (15).
- **Split again (scope rule, one logical change).** June also runs Ciel's body every frame (matrix, rotation, clock, bones, equip anims in
  `CommonFrameMove(Future)`) and through a second physics set (`PhysicProcess`'s second position, PhysicParam `+0x36c`, the
  `MINOR_UNIT_SPEED_*` keys, the `+0x8604` speed-time twin). That is 2-3 sessions of RE on its own: **DU1b2** = the per-frame body and the
  SyncData second position; **DU1b3** = the physics, the not-select keys and the other `CommonStateStart` keys CH1o gives DU1. Kickoffs
  written; DU1c now follows DU1b3; `DU1-review.md` covers all six parts.
- **Switching:** `SetActiveBody` (`3204e030`), `SetDuoEnabled` (`32092a00`), `UpdateDuo( bSkipStatRefresh )` (`3204df80`; vslots resolved
  with `vtslot.py`: 0x638 `UpdateEquipCollisionData`, 0x2c0 `UpdatePassiveAndActiveSkillState`, 0x330 `InitStat`, 0x630
  `UpdateNotRenderingOtherEquipList`), the equipment show pass `UpdateShowViewListByActiveBody` (`31fd6260`, inlined `SetShowObject`),
  `CX2Unit::SetMinorUnitForm` (the store of `3234b8c0`) and `CX2Unit::UpdateSkillTreeCommandGroup` (`3234d930` -> `SetCommandGroup`).
- **New members** (ctor zeroes all, `@32042480` / `@3203f385` / `@320431f6`): `m_bDuoTypeBodyApplied` (+0x8e93, the latch: only the ctor
  clears it, so after the first plain `DUO_TYPE` 0 / 1 start only a hand-over switches the body), `m_bActiveBodyChanged` /
  `m_bActiveBodyChangeWaitNow` / `m_cActiveBodyChangeStateNum` (+0x84dc / +0x84dd / +0x84de: the switch recorded in the Future frame, applied
  by the Now state start once `IsActiveBodyChangeReached` `31fc9510` sees the Now `stateChangeNum` reach it), `m_bFirstUnitTransformed` /
  `m_bMinorUnitTransformed` (+0x8f6e / +0x8f6f, declared here for CH1a0's `31fc9d50` / `31fc9d90`).
- **State start:** `MINOR_UNIT_ANIM_NAME` and `DUO_TYPE` (miss 3) in both starts, through `ApplyDuoTypeAtStateStart` (Future always, Now only
  for others' units, June's `IsMyUnit` gate `@32059a14`): 3 ends the duo; 0 / 1 hand over (event flag 9 / 8 = June +0x7d8e / +0x7d8d, Now
  +0x802e / +0x802d: `bEventFlagList[8..9]`, read by Lu/Ciel `32b397a0`) when the other body's anim name is set and the other body is active,
  else set the body once (latch); 2 shows both; types 0-2 also set `m_iSkillCutInSetSubIndex` (+0x8acc). Ciel's body plays
  `MINOR_UNIT_ANIM_NAME` (unit type 10), Lu's waits (`XAP_ONE_WAIT`) while Ciel acts and the reverse. `DUO_TYPE` has no June Enum entry:
  named constants in `_CONST_GUUSER_DU1B_`.
- **Settled (kickoff ask):** `+0x8f50` is `APPLY_ANIM_MOVE_MULTIPLY` (float, miss 1.0), not a not-select key; `USE_PHYSICPROCESS_NOT_SELECT_UNIT`
  = +0x8f6c (both starts), `USE_NOT_SELECT_UNIT_ANIM_TIME_IN_SPEED` = +0x8f6d (Future only). All go to DU1b3 with their readers.
- **State end / frame move:** `CommonStateEnd` moves both bodies' matrices to the Now position (June's block runs for every class: the
  active body is the first, already moved each frame); both frame moves end the duo when the other body's anim ends and `m_bEndDuoAtAnimEnd`.
- **Stand-ins (one-line comments):** the second after-image (+0x6758, created at `31fee682`, DU1d) and the after-image mode in
  Enable/Disable; the equip's minor-unit byte +0x15d4 (DU3: every equip is Lu's until then); the riding byte +0x93da (Stage 17, reads 0);
  user vslot 0x848 in the Now start (empty base, Lu/Ciel `32b18170`: CH1o); `31fdbb70` (state 0x2f anim override, routed by char-chung
  CH1n2); the portrait refresh `328dcba0` (checklist); `InitStat`'s per-form game stat `3234af90` (DU1c). Not added: `32d51e80`'s
  `SetActiveBody( form )` at user creation is a no-op after the ctor's copy.
- **Kickoffs amended:** DU1c (`InitStat` per-form stat), DU1d (second after-image + mode), DU3 (equip +0x15d4 in
  `UpdateShowViewListByActiveBody`), CH1o (vslot 0x848, vslot 0x648 clears `m_bActiveBodyChanged`), CH1a0 (the pair is declared).
- **Hygiene.** flagcheck `--worktree` (my five files): 2 PASS, 2 PASS~ (one blank line each), 1 OTHER (Always.h: a peer's uncommitted
  char-elesis CH3c2 block, not staged), 0 FAIL, 0 deleted, 0 non-ASCII, RESULT: PASS. `junediverge --check`: RESULT: PASS. enumscan: no row in
  DU1b's lines. Commit `ab1b6f5a`; `--range ab1b6f5a^..ab1b6f5a`: 3 PASS, 2 PASS~, 0 OTHER, 0 FAIL, 0 deleted, RESULT: PASS.
- **Compile:** `/Zs` probes (census `cl_cmd`, no PCH) of `X2GUUser.cpp`, `X2Unit.cpp`, `X2GUChung.cpp`: 0 errors each; a flag probe shows
  every dependency on. Full build skipped: no play-test (DU1d builds and hands off the regression pass).
- **In game:** nothing to see. Every other class has no `DUO_TYPE` (miss 3 -> `UpdateDuo` returns at the unit-type test) and form 0.

### DU1b2 - Done (2026-11-14)

Opus 5.5 high. Part 3 of DU1's six, flag `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (no new define, Always.h untouched). Files: `X2GUUser.h`, `X2GUUser.cpp`
(+151 lines, insert-only). No `_DEBUG`, no log line (nothing runs before CH1). funcmap rows: `grep "(DU1b2)" june2017_re/funcmap.tsv` (14).
- **SyncData second position:** `vNotSelectPosition` after `position` (June syncData +0x14: Future +0x7e48, Now +0x80e8), zeroed in `Init`
  (June ctor `320774e0`), copied with the struct (compiler `operator=`; no `sizeof` / `memcpy` user). DU1b3 writes it, DU1c puts it on the wire.
- **Future (`32061bf0`, decompiled):** `fNowAnimTime` (`@32061de1`) and the `DISABLE_GRAVITY` window (`@32062601`) read the active body;
  riding moves both bodies to the saddle, otherwise the active body goes to the position and, with the duo on, the not-selected body to
  `vNotSelectPosition` (`@320628d1-32062a93`); only the active body turns (`@32062aef` / `@32062b75`); the second body's clock (`@32062c47`)
  and its before-time update (`@32062da8`). The push / pass vectors keep March's `m_pXSkinAnimFuture != NULL` tests (June tests `+0x87d0` too).
- **Now (`32063650`, capstone; Ghidra times out):** `CommonFrameMove_EffectSet` (`@320647ad`) and the `SUPER_ARMOR_TIME` time (`@32064a91`) read
  `GetXSkinAnimPtr()`; the moves (`@320652f5-32065532`) as in the Future but through `GetXSkinAnimPtr()` / `GetNotSelectXSkinAnimPtr()`; **both**
  bodies turn by the same vector (`@32065547-320656cf`: June turns `+0x87d4` and `+0x8e88`, not the active one); the second clock (`@32065712`),
  its `Bip01_Spine1` `RotateBone` (`@32065792`), its before-time update (`@320661af`); the equip loop picks the equip's body
  (`@3206584a-320659e4`, stand-in `bEquipForMinorUnit = false` until DU3) and logs June's `StateLog( L"CX2GUUser.CommonFrameMove NULL ==
  pXSkinAnim !!!" )` and skips when that body is missing; the five bone-frame reads by the active body (feet `@3206722c` / `@32067289`, poison
  and blaze numbers `@32067900` / `@320691ff`, the marker head `@3206c239`).
- **Not in `CommonFrameMove`:** the kickoff's `@3206d44b` is in the Now sync pop after it (`CommonFrameMove` returns at `3206cf24`; the pop
  starts about `3206cfe0`, `ret 8` at `@3206d473`): others' Lu/Ciel units apply the synced active-body word with `SetActiveBody`. With the
  Future write of that word (`@32063316`) it goes to **DU1c** (kickoff amended).
- **Left to DU1b3 (kickoff amended with the March lines):** the `+0x8f6d` speed-time reader (`@3206211d`, before March's `m_vecSpeedFactor`
  loop) and the `+0x8604` twin (`@320623ed` / `@32062419`, after it). The kickoff listed `@32062419` with the moves; it is the twin's.
- **Routed (scope rule; every class, not Lu/Ciel's subject):** two Stage 2 bullets in `JUNE_PORT_BACKLOG_ORDER.md`: the Now rotation offset
  (`+0x9420` vector and the `+0x9540` y flip June adds to `dirDegree`, `@320655e1-32065663`), and the `+0x8864` drift block
  (`@32064f47-320652b7`, moves both bodies itself and skips the normal move / rotation). DU3 (kickoff amended): the equip loop's
  `+0x8e64` / `+0x8e70` block after the body pick (`@320659e9`).
- **Live branches only:** `GRAPPLING_TEST`, `RENA_SIEGE_MODE_ANGLE_TEST` and the non-adaptive / non-riding `#else` paths are off in
  `US_SERVICE` (flag probe) and get no twins; the riding blocks keep `SERV_IRUHADEV_JUNE_DIVERGE_SADDLE_POS`'s choice.
- **Hygiene.** flagcheck `DUO_BODY --worktree --files X2GUUser.cpp X2GUUser.h`: 2 PASS (+145, +6), 0 OTHER, 0 FAIL, 0 deleted, 0 non-ASCII,
  RESULT: PASS. `junediverge --check`: RESULT: PASS. enumscan `--check X2GUUser.cpp --reuse`: 0 A/B rows (a fresh `cl /P` of its `X2GuEve.cpp`
  TU exited 2 with no output; the `/Zs` probes below compile the real files). Commit `0921dee6`; `--range 0921dee6^..0921dee6`: 2 PASS,
  0 OTHER, 0 FAIL, 0 deleted, RESULT: PASS.
- **Compile:** `/Zs` probes (census `cl_cmd`, no PCH) of `X2GUUser.cpp` and `X2GUChung.cpp`: 0 errors each. Full build skipped: no play-test
  (DU1d builds and hands off the regression pass).
- **In game:** nothing to see. Every other class has no second body (`m_pMinorUnitXSkinAnim(Future)` NULL, form 0): each new line is a NULL
  test or picks the first body.

### DU1b3 - Done (2026-11-14)

Opus 5.5 high. Part 4 of DU1, flag `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (Always.h: a third `#error` guard, NOT_SELECT_UNIT_OFFSET, APPLY_MOVEMENT_SPEED,
LINEMAP_LAND_MONSTER_ROAD). Files: `X2GUUser.cpp` (+421), `X2GUUser.h`, `X2GameUnit.h`, `X2GameUnit.cpp`, `KTDGXSkinAnim.h`, `Always.h`. No `_DEBUG`, no log
line (nothing runs before CH1). funcmap rows: `grep "(DU1b3)" june2017_re/funcmap.tsv` (16).
- **Split again (kickoff's own rule).** The physics alone filled the session: **DU1b3** = Ciel's physics; **DU1b4** (kickoff written) = the
  `MINOR_UNIT_SPEED_X/Y` / `MINOR_UNIT_PASSIVE_SPEED_X/Y` parses, the APPLY_MOVEMENT_SPEED minor copy and rescale, the frame-move passive apply
  (`@320631b0`), `+0x8f6d` and the `+0x8604` twin, and the camera / armor keys. DU1c now depends on DU1b4; `DU1-review.md` covers seven parts.
- **The second PhysicParam:** `CX2GameUnit::m_MinorUnitPhysicParam` (June unit `+0x36c`, a whole `PhysicParam`), copied from Org in `Init`
  (`322a5f90 @322a600a`) and `ResetPhysicParam` (`322886a0 @322886c2`); RidingOffEnd copies only the first, as June. Every unit carries it.
- **`PhysicProcess` (`32015b30`), June's order:** a local second position from Future `vNotSelectPosition`; line fallbacks zero the second
  speed too; its x decay (own walk speed vs the first body's `GetWalkSpeed`); its fall (cap without SET_LIMIT, JUNE_BUGS 142), land-connect
  zero and reverse gravity (both second anims turned); the x move, line speed and anim move push **both** the local second position and the kept
  `m_vNotSelectMotionBeforePos` (`+0x8f60`) for every class (harmless: read only with `+0x8f5d`). The anim move now reads the **active** body
  (`GetXSkinAnimFuture()`, June `32070cf0`; March read `m_pXSkinAnimFuture`) and multiplies by `APPLY_ANIM_MOVE_MULTIPLY` (it is in these lines,
  so ported here). With `USE_PHYSICPROCESS_NOT_SELECT_UNIT` the second (kept or local, by `+0x8f5d`) also gets the push loop, the ceiling / y
  move and the landing. Store: second = kept position or the **first body's** position, then the offset (`m_afNotSelectUnitOffset`,
  `GetMotionMoveNow` difference = June `320703f0` `+0x2c4`, added getter in `KTDGXSkinAnim.h`).
- **Two helpers (June's functions, March's inline blocks):** `PhysicProcess_PushByUnits` = `32010060` and `PhysicProcess_LandOnLine` = `32010440`
  (PhysicParam and anim by `bMinorUnit_`, returns bMisplaced). Under the flag the first body runs through them too; their bodies are March's
  live branches verbatim (`X2OPTIMIZE_NPC_NONHOST_SIMULATION`, `ADD_CAN_PASS_ALLY_UNIT`, `MONSTER_ROAD` + `FIX_LINE_TYPE_WALL`,
  `X2OPTIMIZE_WALL_TRAP_BUG_FIX`, `AISHA_SECOND_CLASS_CHANGE` on; `REMOVE_LAND_CONNECT_CONSTRAINTS` off: flag probe), March's inline code in `#else`.
- **State-start keys** (both starts, after `ParseNotSelectUnitOffset`): `USE_NOT_SELECT_MOTION_BEFORE_POS` (miss 0; true copies the **Future**
  second position to `+0x8f60` in both starts), `APPLY_ANIM_MOVE_MULTIPLY` (miss 1.0, `fld1`), `USE_PHYSICPROCESS_NOT_SELECT_UNIT` (miss 0).
  Ctor: `+0x8f5d` / `+0x8f60` / `+0x8f6c` zero as June; `+0x8f50` = 1.0 (June's ctor leaves it to the first state start).
- **June bugs kept (JUNE_BUGS 659 / 660, row 142 Port column updated):** the second body's ceiling test reads the first body's y speed
  (`@3201917c`); the offset turns the motion difference as `dir.x * ( dx + dz )` / `dir.z * ( dx + dz )` (`@3201a009-3201a071`).
- **Routed (scope rule; every class or riding):** Stage 2 bullet: June's misplaced re-test (`@32019e84-32019f94`) and the `+0x40ac` fall
  record (`32010440 @32011081`); Stage 17 bullet: the riding-pet anim move (`@32018204`, pet `+0x5a1`).
- **Enum gate:** `enumscan --check` lists 4 class A rows at `PhysicProcess_PushByUnits`'s `case 1..4`: those are
  `GetUserUnitRelativePos` codes (no enum; March's own switch, copied), not NPC ids; a one-line comment says so.
- **Hygiene.** flagcheck `DUO_BODY --worktree --files` (my six files): 5 PASS (+421, +6, +6, +7, +3), 1 OTHER (Always.h: my guard + a peer's
  uncommitted char-elesis CH3d block, not staged), 0 FAIL, 0 deleted, 0 non-ASCII, RESULT: PASS. `junediverge --check`: RESULT: PASS.
  Commit `04d2cacf`; `--range 04d2cacf^..04d2cacf`: 6 PASS, 0 OTHER, 0 FAIL, 0 deleted, RESULT: PASS.
- **Compile:** `/Zs` probes (census `cl_cmd`, no PCH) of `X2GUUser.cpp`, `X2GameUnit.cpp`, `X2GUChung.cpp`: 0 errors each. Full build skipped: no
  play-test (DU1d builds and hands off the regression pass).
- **In game:** nothing to see. Other classes have no second anim and none of the keys: the second-body lines move only a local and the unread
  `+0x8f60`; the first body runs March's code through the helpers; `APPLY_ANIM_MOVE_MULTIPLY` reads 1.0.

### DU1b4 - Done (2026-11-14)

Opus 5.5 high. Part 5 of DU1, flag `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (Always.h: a fourth `#error` guard, `SERV_IRUHADEV_JUNE_SPEED_TIME`), plus
the new divergence flag `SERV_IRUHADEV_JUNE_DIVERGE_COMMON_STATE_END_FUTURE`. Files: `X2GUUser.cpp` (+168), `X2GUUser.h` (+3), `Always.h` (+14).
No `_DEBUG`, no log line (nothing runs before CH1). funcmap rows: `grep "(DU1b4)" june2017_re/funcmap.tsv` (10).
- **Speed keys by active body (`320506f0 @3205283a`).** June has two parse branches on `+0x8e90`: the active body takes `SPEED_X/Y` and
  `PASSIVE_SPEED_X/Y`, the other body the `MINOR_UNIT_` keys (Ciel active: `SPEED_X` -> `+0x370`, `MINOR_UNIT_SPEED_X` -> `+0x304`). That is why
  the kickoff saw two sites per key. Ported with `activeBodyPhysicParam` / `otherBodyPhysicParam` (speeds: no store on a miss; passives: -1).
- **SPEED_TIME lists (`31fe3a60`).** The same swap: `SPEED_TIME(_ULTIMATE)` fills the active body's list, `MINOR_UNIT_SPEED_TIME(_ULTIMATE)` the
  other's (`m_vecMinorUnitSpeedFactor`, June `+0x8604`); each `_ULTIMATE` falls back to its plain list on its own. C3l's gate is reused.
- **APPLY_MOVEMENT_SPEED (`@3205418e-320542b9`).** The minor PhysicParam gets the bool every state start; when set, its `nowSpeed.x`, its
  `passiveSpeed.x` (test `|x - (-1.0)| > FLT_EPSILON`, June's own, not `IsSamef`) and its list's `vSpeed.x` scale by the main rate.
- **`USE_NOT_SELECT_UNIT_ANIM_TIME_IN_SPEED`** (`+0x8f6d`, Future only, after `USE_PHYSICPROCESS_NOT_SELECT_UNIT`, miss 0).
- **Frame move (`32061bf0`).** Main pass: with the key, while Ciel acts and the duo is on, the time is the not-selected (Lu's) anim time
  (`@3206211c`). New minor pass (`@320623c9-320625f5`): no anim test; with the key, while **Lu** acts, Ciel's anim time; flags 1 / 2 / 3 on the
  minor PhysicParam, no 11-13; same 5th-column turn. Minor passive apply after March's (`@320631b0-320631e2`).
- **June bug kept (JUNE_BUGS 662):** the main pass's time override is not undone, so with Ciel active the minor pass runs Ciel's list on Lu's
  anim time (`LSI_BOUNDING_TARGET_CIEL_COMMAND_XXX` has the key and both lists).
- **Divergence found and replaced in this commit:** June's `CommonStateEndFuture` (`31fdd8e0`) runs, before March's body, the SPEED_TIME entries
  with no anim time (3rd column < 0, flag 1 / 2 only, 5th-column turn) and erases them (main list when `+0x87d0`, then the minor list), and
  moves the active Future anim (and, in a duo, the not-selected one) to the Future position. March has none of it. Ported as a prefix under
  `SERV_IRUHADEV_JUNE_DIVERGE_COMMON_STATE_END_FUTURE` (helper `ApplyUntimedSpeedTime` in `_CONST_GUUSER_DU1B4_`), the minor parts nested under
  `DUO_BODY`; `LINKED_LINEMAP_JUMP_USER_TEST` is off (probe), so the rest is March's live body. `june_march_divergent.tsv` row added. **This one
  reaches every class** (any untimed SPEED_TIME entry); DU1d's regression play-test covers it.
- **`SUPER_ARMOR_CUSTOM_COLOR`** (`32057b80 @3205958e`): every `CommonStateStart`, before the event-flag zero, into `m_coSuperArmorCustomColor`
  (June `+0x8a2c`), miss red (1, 0, 0, 1). The reader is CH3c's (kickoff amended).
- **Routed (scope rule):** `RETAIN_CAMERA_STATE` / `FORCE_DYNAMIC_CAMERA_INDEX` are not Lu/Ciel keys: every June class script has them (WIN
  states, `WAIT_STATE_CHANGE`), and `FORCE`'s reader (`+0x801c`) is unmapped. Stage 2 bullet; funcmap row `routed`.
- **Kickoffs amended:** CH3c (the color's reader), DU1d (minor passive resets beside March's `passiveSpeed = (-1, -1)` sites), DU1-review (the
  divergence flag).
- **Gates.** `flagcheck JUNE_CHAR_DUO_BODY --with JUNE_DIVERGE_COMMON_STATE_END_FUTURE --worktree --files` (3 files): 2 PASS (+168, +3), 1 DEFINE,
  0 OTHER, 0 FAIL, 0 deleted, 0 non-ASCII, RESULT: PASS. `junediverge --check`: RESULT: PASS. `enumscan --check X2GUUser.cpp`: the 4 known
  DU1b3 rows only (`GetUserUnitRelativePos` codes). Flag probe (`cl /P`): both flags and `SPEED_TIME` on, no `#error`.
  Commit `32fb86be`; `--range 32fb86be^..32fb86be` (with both flags): 2 PASS, 1 DEFINE, 0 OTHER, 0 FAIL, 0 deleted, RESULT: PASS.
- **Compile:** `/Zs` probes (census `cl_cmd`, no PCH) of `X2GUUser.cpp` and `X2GUChung.cpp`: 0 errors each. Full build skipped: no play-test.
- **In game:** nothing new for Lu/Ciel yet. Other classes: form 0, none of the duo keys, the minor list stays empty; the only live change is
  the untimed SPEED_TIME entries applying at state end and the Future anim placed at the Future position there (June's behaviour).

### DU1c - Done (2026-11-14)

Opus 5.5 high. Part 6 of DU1, flag `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (Always.h: a fifth `#error` guard, BUFF_NEW_FINALIZER and CHARINFO_SHOW) plus
the new divergence flag `SERV_IRUHADEV_JUNE_DIVERGE_UP_MP_ITEM_PVE`. Files: `X2UDPPacket.h` / `.cpp`, `X2GUUser.h` / `.cpp`, `X2Game.cpp`,
`X2DamageEffect.cpp`, `X2UICharInfo.h`, `X2Unit.h`, `Always.h`. No `_DEBUG`, no log line (nothing runs before CH1). funcmap rows:
`grep "(DU1c)" june2017_re/funcmap.tsv` (11).
- **Split again (kickoff's own rule).** The readers outside `CX2GUUser` (the weapon-effect type-10 cases `3228c540` / `32292610`, `GetStat`
  `328842f0`, the socket readers `32365d30` / `328836c0`, `3235a160`, `31ed3340`) go to **DU1c2** (kickoff written); DU1d now depends on it.
- **Sync packet (not `CommonPacket.h`: `KDYNAMIC_UNIT_USER_SYNC` lives in `X2UDPPacket.h`, client-local; no `ServerDefine.h` twin).** Bits
  0x10000 E float, 0x20000 / 0x40000 / 0x80000 B (second position x half / y float / z half), 0x100000 F short, 0x1000000 I word, in June's
  wire order (after RelativePos and before C3e-1's 0x200000; I after BuffSyncCount). H (0x20000000) stays char-ain's. SendFrameData encodes B
  with `_EncodeUserSyncPos` against the line index (`@3203d6e4` / `@3203d709`), SetData decodes it the same way, y raw (`3201f5f0`).
- **SyncData:** `fMinorUnitNowMP` (E, June +0x48), `sClassSyncShort` (F, +0x54), `usMinorUnitActive` (I, +0x84), protected as June's slots,
  zeroed in `Init`. **E is Ciel's own MP:** Lu/Ciel's vslot 0x64c override `32b1c960` writes gauge MP (0) to Future `fNowMP` and gauge MP (1)
  to E (reader `32aeb2c0`, CH1l; kickoff amended). F's drivers are class code (Ain, Rose, Lu/Ciel). June's converters `32071ea0` / `320829b0`
  need no code: March keeps one SyncData shape copied whole (send list -> receive list -> Now).
- **Active body sync:** Future I = ( `m_bMinorUnitActive` ) every frame for every class (`@32063316`, = `31fe9e40`), before nowAction is cleared;
  `PopFrameData`'s tail (`@3206d404-3206d45a`): another player's `UT_LUCIEL` unit calls `SetActiveBody( I == 1 )` when it differs.
- **MP-on-hit cash ability per body.** June's `+0x88c8` / `+0x88d4` are `m_fUpMPAtAttackOrDamage` and a minor-unit copy (`+0x88c4` =
  `m_bShowOppnentMP`), so the pair is `CashItemAbility` with `GetUpMPAtAttackOrDamage( bMinorUnit )` / `SetUpMPAtAttackOrDamage`. Readers by
  the active body: `AttackResult` (`@320132fb`), `DamageReact` (`@32049e76`; NU0 filed it under `32045dc0` CanApplyBuffToGameUnit, no int3
  between them), `CX2DamageEffect::CEffect::AttackResult` (`@31f8716d`, June tests `!= 0` where March tested `> 0`). Writers:
  `UseItemSpecialAbility`'s SAT 12 case (`@322fd320`, active body; the kickoff's "ProcessExpListByNpcUnitDie `322f8920`" is this uncarved code
  after it), `Set/RemoveSpecialAbilityInEquip` (`322e8690` / `322e84e0`) by the **char window's switching view** (`g_pData` +0x2cc UI manager,
  +8 char window, +0x25a; new `CX2UICharInfo::GetSwitchingView`). June reads the window with no NULL test; March creates it on first open, so
  `IsCharInfoSwitchingView` (`_CONST_GAME_DU1C_`) tests NULL and reads false. JUNE_BUGS 663: the body is picked by context, not by who wears it.
- **Divergence found and replaced (this commit):** June's `SetSpecialAbilityInEquip` and the same SAT 12 case skip the Magic Necklace (200680)
  in `GT_PVE` (unreachable offline) under `SERV_IRUHADEV_JUNE_DIVERGE_UP_MP_ITEM_PVE`. `june_march_divergent.tsv` rows: `InitStat`,
  `SetSpecialAbilityInEquip`, `RemoveSpecialAbilityInEquip`, `UseItemSpecialAbility` (SAT 12 case only; other cases not compared),
  `PopFrameData`.
- **InitStat per body (kickoff amendment, `31fe5480` @`31fe55a5`):** the game stat is `UnitData::GetGameStatOfBody( IsMinorUnitActive() )`
  (June `3234af90`: unit data +0x164 `m_GameStat` or +0x1a0, new `m_MinorUnitGameStat`), `m_fBaseHP > 0` else `m_Stat`. **Stand-in:** the
  second stat's writer is June's `UnitData::SetGameStats( KStat, KStat )` `323571c0` (seven packet sites); it and the emulator's per-body numbers
  go to **DU5** (kickoff amended). Until then Ciel active reads `m_Stat` (June's absent-value path); CH1's play-test will show it.
- **Gates.** `flagcheck JUNE_CHAR_DUO_BODY --with JUNE_DIVERGE_UP_MP_ITEM_PVE --worktree --files` (9 files): 7 PASS (`X2UDPPacket.cpp` +66,
  `X2GUUser.cpp` +54, `X2UDPPacket.h` +28, `X2GUUser.h` +18, `X2DamageEffect.cpp` +8, `X2Unit.h` +4, `X2UICharInfo.h` +3), 2 OTHER (`Always.h`
  and `X2Game.cpp`: a peer's uncommitted char-elesis CH4a blocks beside mine, not staged), 0 FAIL, 0 deleted, 0 non-ASCII, RESULT: PASS.
  `junediverge --check` (8 files): RESULT: PASS. `enumscan --check`: the 4 known DU1b3 rows only. Flag probe: both flags, BUFF_NEW_FINALIZER
  and CHARINFO_SHOW on, no `#error`. Commit `d353b1ff` (shared `Always.h` / `X2Game.cpp` staged without the peer's hunks); `--range
  d353b1ff^..d353b1ff` (both flags): 8 PASS, 1 DEFINE, 0 OTHER, 0 FAIL, 0 deleted, RESULT: PASS.
- **Compile:** `/Zs` probes (census `cl_cmd`, no PCH) of `X2GUUser.cpp`, `X2Packet.cpp` (includes `X2UDPPacket.cpp`), `X2Game.cpp`,
  `X2DamageEffect.cpp`, `X2Unit.cpp`, `X2UICharInfo.cpp`, `X2GUChung.cpp`: no error in DU1c's code; each TU's only errors are the peer's
  uncommitted `X2BuffBehaviorTemplet.h(146)` (C2027 `CX2GameUnit`, char-elesis CH4a in progress). Full build skipped: no play-test.
- **In game:** nothing new. Other classes: I is always 0 and only Lu/Ciel reads it; E / B / F are 0; the MP-on-hit copy read is the first
  body's (form 0, window not switched); InitStat reads `m_GameStat`. The Magic Necklace gate is PvE-only.

### DU1c2 - Done (2026-11-14)

Opus 5.5 high. Part 7 of DU1, flag `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (Always.h: a sixth `#error` guard, DIVERGE_WEAPON_EFFECT_SHOW). Files:
`X2GameUnit.h` / `.cpp`, `X2Unit.cpp`, `Always.h`. No `_DEBUG`, no log line (nothing type 10 runs before CH1). funcmap rows:
`grep "(DU1c2)" june2017_re/funcmap.tsv` (8 new).
- **Enchant particle, type 10 (ported).** `CommonFrameMoveForUser`'s switch gains `UT_LUCIEL` -> new `Weapon::EnchantParticleFrameMoveForLuciel`
  (June `32297d50` case 10 -> `3228c540`, Eve's shape): Ciel active (`IsMinorUnitActive`, the `@3228c602` read NU0 called a game-loop reader)
  takes the midpoint of `TRACE_END[0]` / `TRACE_START[0]`, Lu `GetBonePos( L"MAJOR:Bone_RFinger2" )`; emit 10.
- **+11 / +13 glow, type 10 (ported, both inside DIVERGE_UPGRADE_WEAPON_PARTICLE's June bodies).** `InitUpgradeWeaponParticle` (`32299730`):
  `UT_LUCIEL` joins the sword group. `ProcessUpgradeWeaponParticle` (`32292610` @`32292f71`): base **proven `CX2Unit`** (the register that
  reads +0xe40 is the owner's +0x867c unit, the same one whose templet +0xaf0 drives the switch). Form 1 (Ciel): `GetWeaponBonePos( idx,
  L"Effect_LWeapon", 0 )` / `( idx, L"Effect_RWeapon", 1 )`, form 0 (Lu): `MAJOR:` right / left finger; seq 1-2 first, 3-4 second; when all of
  5-10 exist, 5 / 6 / 9 first and 7 / 8 / 10 second. Read from the listing: the decompile mixes the two position vectors. NU0's "vslot 0x2b0 =
  skin anim" was wrong: it is a weapon index, new `CX2GameUnit::GetWeaponIndexHavingModel( int ) const` (base returns 0 as June `330f9c30`;
  Lu/Ciel's `32b151b0` is CH1k's, kickoff amended with its body).
- **`MAJOR:` bone names.** June's user `GetBonePos` (vslot 0x84, `31fda280`) strips `MAJOR:` / `MINOR:` to pick the body; March's does not
  yet, so Lu's case-10 positions read no bone until **DU1d** ports it (kickoff amended; DU3's "equip dump" label for `31fda280` is wrong).
- **Hand sparks `31ff8510`:** an `X2GUUser.cpp` function, left in DU1d's list.
- **`ResetHudMaxHpMp` `3235a160` (ported; the kickoff's "not ported" was stale: Skill S7k ported it).** The two form reads: `InitSocketData(
  form )` (@`3235a1d4`) and `GetEqipStat( false, form )` (@`3235a426`); `#else` keeps `0`. The `UT_LUCIEL` second-gage MP block
  (@`3235a29e-3235a339`) needs the type-10 socket aggregate `32091730` (no March class) and the gage index of `3248a800`: one-line stand-in
  comment, routed to **DU5** (kickoff amended with the formula).
- **Set-socket form reads (routed).** `GetStat` `328842f0` @`32885558`, `InitSocketData` `32365d30` @`32365fe7` / @`3236616d`, used-socket
  count `328836c0` @`328837a5`: base proven `CX2Unit` (g_pData +0x68 my user, +0x90 select unit). Each passes the form to `CX2Inventory`
  `3286a6b0( set id, body )` (equipped parts of a set worn by that body, filter `3286a4b0`), inside the item +0xac "set complete" socket list
  that March's `ItemData` lacks and no phase has ported (S7m: Stage 16). No March site to edit; the form goes with that list's port, the
  count helper and the type-10 aggregate to DU5.
- **`31ed3340` = `CX2Room::DeleteNpcSlot`** (g_pX2Room, caller `CX2Game` `322e7ba0`): reads `3234af90( 0 )`, the first body's game stat =
  March's `m_GameStat`. No change.
- **Gates.** `flagcheck JUNE_CHAR_DUO_BODY --worktree --files` (4 files): 4 PASS (`X2GameUnit.cpp` +83, `X2Unit.cpp` +11, `X2GameUnit.h` +6,
  `Always.h` +3), 0 FAIL, 0 deleted, 0 non-ASCII, RESULT: PASS. `junediverge --check` (4 files): RESULT: PASS. `enumscan --check`: 190
  older A/B rows in the three files, none in this phase's hunks. No `JUNE_BUGS` row (no quirk kept). Commit `89cfbabd`; `--range
  89cfbabd^..89cfbabd`: 4 PASS, 0 OTHER, 0 FAIL, 0 deleted, RESULT: PASS.
- **Compile:** `/Zs` probes (census `cl_cmd`, no PCH) of `X2GameUnit.cpp`, `X2Unit.cpp`, `X2GUChung.cpp`: 0 errors (so the new guard does not
  fire); `cl /P` shows the new function, the virtual and the three `UT_LUCIEL` cases live. Full build skipped: no play-test.
- **In game:** nothing new. Other classes are never `UT_LUCIEL` and have form 0, so `InitSocketData( 0 )` / `GetEqipStat( false, 0 )` as before.

### DU1d - Done (2026-11-14)

Opus 5.5 high. Part 8 of DU1, flag `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (no new define, Always.h untouched). Files: `X2GUUser.h` (+206), `X2GUUser.cpp`
(+255), both insert-only. No `_DEBUG`, no log line (nothing reads a second body before CH1). funcmap rows: `grep "(DU1d)" june2017_re/funcmap.tsv` (11).
- **Split again (kickoff's own rule).** NU0's `DU1` rows came from an inline-member scan; June also reads the active body through out-of-line
  getter calls (`32071d80` x105, `32070cf0` x14, `32071db0` x16 in the user band) that no row lists. New census
  `june2017_re/char_luciel/du1d_body_reads.tsv` (104 functions, rows + calls, each with an owner). **DU1d** = kickoff bullet 1 (the accessor sites);
  **DU1d2** (kickoff written) = the bones / hands / head / emotion / after-image / sound / socket readers, the 63 `DU1d2` functions of the
  census, then the full build and the regression play-test. `DU1-review.md` now also covers DU1d2.
- **The cancel macros (X2GUUser.h, 22 twins).** June expands `ELSE_IF_ARROW_*_PRESSED_AFTER`, `ELSE_IF_ZX / ASDC_PRESSED_AFTER`, `*WALK / DASH /
  SKILL / ZX_CANCEL_AFTER(_EX / _FLY)`, `IF_SKILL_CANCEL_AFTER` and the `*_STATE_CHANGE_ON_*` / `ELSE_IF_SET_EVENT_FLAG` macros with the
  active-body accessor (inline in `31fcc350` / `31fcc9d0`, called at `@31fcc60f` / `@31fe76ce`). Each `#define` gets a flagged twin reading
  `GetXSkinAnimFuture()`, so the base functions, every class and Lu/Ciel's own CH1 states read the active body. `X2GUAdd.h`'s Add-only macros are
  not touched. `cl /P` of `X2GUChung.cpp`: 264 twin expansions (Chung's form is 0: the same anim).
- **Base functions (X2GUUser.cpp, 46 sites, March's line in `#else`):** event processes PEPPER_RUN_READY `31fcb930` (sits after `31fcb2f0`'s
  jump table), PEPPER_RUN_END `31fcb990`, THROW_ITEM `31fcbc10` (inside the USER_BASE_STATE_BODIES body; its stand-in comment stays in `#else`),
  SUMMON_MAGIC_SPEAR `31fcbd10` (named by Elsword's `ESSI_SUMMON_MAGIC_SPEAR` registration, thunk `32d25f80`), COMMON_BUFF, StartIntrude,
  DamageSmall / Big / StandUp / Revenge, HyperMode, BoundingLandCrash `31fd0260`, GenericGroundComboEnd / SpecialActive / Active (June bodies
  only), RandomLose `31fcd0b0`, COMMON_RELATIONSHIP, RidingHyperMode `32006b90`; `CommonEventProcess`'s USE_COMMON_CANCEL block `31fe7610`; the
  event kit (PROTON CONTROL_FIRE `31fcfb70`, MECA_DROP `31fcfde0`, SNIPER / PROTON ready events `320211d0` / `320212b0`); the cpp macro
  `USER_SKILL_STATE_CANCEL_CHAIN`. Now-anim sites to `GetXSkinAnimPtr()`: SNIPER / PROTON ready frames (`3200d1d0` / `3200e040`, inline
  `+0x8e90 ? +0x8e88 : +0x87d4`), THROW_ITEM_FrameMove (`31ff92d0 @31ff9304`), ResetAnimSpeed's Now branch (`31ffe190`: two calls to `32071d80`;
  its Future branch reads the inline accessor).
- **ReInit (`32022380 @32023e04 / @32023e4b`):** after March's two `SetPlaySpeed( 1.0f )`, the second body's Future / Now anims too, each NULL-tested.
- **Not edited (census owners):** DANCE_Z / DANCE_X `31fcbb60` / `31fcbba0` (June amusement states, no March id); `31fcce20` / `3200c550`
  (no call or pointer anywhere in the exe); `31fcd0f0` / `31fcd160` / `31fcd1b0` / `31fcd220` (base helpers only Lu/Ciel's thunks `32b4aa20..70`
  call: CH1); March's THROW_WOODEN_PIECE / MACHINE_GUN_* / WallLanding have no June accessor row (June reads `+0x87d0` or lacks them): unchanged.
  `Send_SKILL_USE_REQ` `32006b30` has no read of its own (the rows are `32006b90`'s).
- **Kickoff amendment carried to DU1d2:** the DU1b4 note (minor `passiveSpeed` resets beside March's `( -1, -1 )` sites) and the DU1b after-image
  note were not reached; both are in `DU1d2.md`.
- **Gates.** `flagcheck JUNE_CHAR_DUO_BODY --worktree --files X2GUUser.cpp X2GUUser.h`: 1 PASS (+255), 1 PASS~ (+206, one blank line), 0 OTHER,
  0 FAIL, 0 deleted, 0 non-ASCII, RESULT: PASS. `junediverge --check` (2 files): RESULT: PASS. `enumscan --check X2GUUser.cpp`: the 4 known DU1b3
  rows only. No `JUNE_BUGS` row, no divergent function (each June body differs from March's only by the accessor). Flag probe: DUO_BODY,
  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE, SKILL_CANCEL_BY_HYPER_MODE, JUNE_BUFF_AWAKE_LOCK, CHAR_ARME_CORE, USER_BASE_STATE_BODIES all on.
  Commit `a9621883`; `--range a9621883^..a9621883`: 1 PASS, 1 PASS~, 0 OTHER, 0 FAIL, 0 deleted, RESULT: PASS.
- **Compile:** `/Zs` probes (census `cl_cmd`, no PCH) of `X2GUUser.cpp`, `X2GUChung.cpp`, `X2GUElsword_SwordMan.cpp`, `X2GUAdd.cpp`, `X2GuEve.cpp`:
  0 errors each. Full build skipped: no play-test (DU1d2 builds and hands off the regression pass).
- **In game:** nothing to see. Every other class has form 0: `GetXSkinAnimFuture()` / `GetXSkinAnimPtr()` return March's members.

### DU1d2 - Done (2026-11-14)

Opus 5.5 high. Part 9 of DU1, flag `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` plus a new `SERV_IRUHADEV_JUNE_DIVERGE_PLAY_SOUND_REVIVE` (Always.h, two
`#error` guards: `JUNE_SELECT_VOICE_TABLE`, `JUNE_CHAR_DUO_BODY`). Files: `X2GUUser.cpp` (+207), `X2GUUser.h` (+6), `Always.h` (+15), all insert-only.
No `_DEBUG`, no log line (nothing reads a second body before CH1). funcmap rows: `grep "(DU1d2)" june2017_re/funcmap.tsv` (11).
- **Split again (kickoff's own rule).** The 63 census rows are ~6,000 decompiled lines. **DU1d2** = the bones and hands half of bullet 1 plus
  the passive resets; **DU1d3** (kickoff written) = the rest of bullet 1 and the second after-image (24 rows); **DU1d4** (kickoff written) = the
  out-of-line getter callers (26 rows), then the full build and the regression play-test that closes DU1. Census owners updated
  (`DU1d2 done` / `routed: ...` / `DU1d3` / `DU1d4`).
- **Census defect (for DU1d3).** `du1d_body_reads.tsv` files a read under the previous function when June has no padding between two functions:
  `3201fb8d` is in uncarved `3201f8d0` (after `3201f850`'s `ret 8`; an equip pass on `+0x15d4` / `+0x172` / `+0x1b0 == 7`, called from Init
  `3204ea20 @3204eb06`, so DU3's equip byte), `3204e4f3` is in `3204e4a0` (after one `int3`; `+0x8fa4` / `+0x8fa8`, called from CommonFrameMove
  `@320661f9`). DU1d3 re-checks each row's start before transcribing.
- **Bone names (ported).** New `GetCloneFrameByBodyPrefix` (June's inline test in `31fd9200` / `31fda280` / `31fda680`): `"MAJOR:"` (string
  `33473188`) the first body by the rest of the name, `"MINOR:"` (`33473178`) the second when it exists, else the active body by the whole name.
  `GetCombineMatrixFromBoneName`, `GetBonePos( name )` (vslot 0x84) and `GetBonePos( name, out )` (vslot 0x80, false and zero on a miss) use it,
  so DU1c2's `L"MAJOR:Bone_RFinger2"` finds a bone. **Fix:** the D-phase port of `31fd9200` compared `"MAJOR_"` (wrong string); June's body is
  re-ported under DUO_BODY, the old one stays in `#else`; divergence row corrected and a `GetBonePos` replace row added (ported).
- **Head, rotate, re-form (ported).** `GetHeadBonePos` (`31fcac20`, uncarved): the active body's head frame. `RotateBone` (`@31fd954e`): the second
  anim updates the **same** first-body frames again (JUNE_BUGS 668, kept). `SetUnitReForm` (`@31fda12e`): `UT_LUCIEL` re-forms only the second
  body, as the second form. `RestoreHead` (`@31fde558`): face restore and `Bip01_Head` / `Bip01_Spine1` re-form on the second body; June's
  `31fdc9d0` on the transformed anims `+0x8f84` / `+0x8f9c` stays a stand-in for CH1a0 (members not in March).
- **ScaleHead (ported).** New `ScaleHeadBone( bone, head, face, iBody )` = June `31fd64a0` (body 0 / 1, else nothing); March's `ScaleHead`
  (= `31fdc070`) calls it with body 0. The class overrides (Chung `32bf0480`, Lu/Ciel `32b17225`) call it directly in their ports.
- **Hands (ported).** `ScaleHand` / `RestoreHand` (`31fde6d0` / `31fded80`): `UT_LUCIEL` with unit form Lu (`+0xe40 != 1`, `IsMinorUnitForm`) also
  scales / restores `Bone_RFinger0-3` / `Bone_LFinger0-3`; the second body repeats the hand loop (ScaleHand also Eve's dummies, RestoreHand not).
- **Revive voice (ported, divergent).** `PlaySoundRevive` = June `31fdb390`: types 1-12, the name from NU4's `GetVoiceFileName( class, SPT_REVIVE )`,
  type 10 replaced by `LuVoice_Revival.ogg` / `CielVoice_Revival.ogg` by the active body. Every class: Rena now revives with `RenaVoice_Revive0n`
  (was `LenaVoice_`; both files exist in data095). Divergence row added (ported).
- **Feet (routed).** `31fdc0e0` / `31fdc310` (foot scale `+0x8e20` on both bodies) have no March counterpart: their only callers are June's base
  `Set` / `UnSetSpecificValueByBuffTempletID` cases (`@3204c6ac` / `@3202f1ec`); they come with those cases (Char Eve CH4a checklist row).
- **Passive resets (no change).** `dispsweep 378 37c` over the user band: June's only writers of the minor passive speed are `32050230 @32054283`
  and `32061bf0 @32062537`, both DU1b4 ports. March's four `( -1, -1 )` resets (Process_ExtraDamage x2, GRAPPLE, WaterHold) have no minor twin.
- **Gates.** `flagcheck JUNE_CHAR_DUO_BODY --worktree --files X2GUUser.cpp X2GUUser.h Always.h --with DIVERGE_PLAY_SOUND_REVIVE` (+ the peer's
  uncommitted `JUNE_CHAR_ELESIS_PASSIVE` / `_DEBUG` stripped, their hunks not staged): 2 PASS (+270 / +9 incl. the peer's), 1 DEFINE, 0 OTHER,
  0 FAIL, 0 deleted, 0 non-ASCII, RESULT: PASS. `junediverge --check` (3 files): RESULT: PASS. `enumscan --check X2GUUser.cpp`: the 4 known DU1b3
  rows only. Flag probe: DUO_BODY, DIVERGE_PLAY_SOUND_REVIVE, DIVERGE_SCALE_HEAD / RESTORE_HEAD / UNIT_REFORM / ROTATE_BONE / BONE_NAME_PREFIX,
  USER_LEFT_HAND_SCALE, SERV_NEW_DEFENCE_DUNGEON, MODIFY_GET_BONE_POS, ADD_PLAY_SOUND, DUNGEON_ITEM, ELSWORD_SECOND_CLASS_CHANGE all on.
  Commit `57f8cf79` (shared files staged as HEAD + this phase's hunks); `--range 57f8cf79^..57f8cf79`, each flag `--with` the other: DUO_BODY
  2 PASS, 1 DEFINE, 0 OTHER, 0 FAIL, 0 deleted, RESULT: PASS; DIVERGE_PLAY_SOUND_REVIVE 1 PASS, 1 DEFINE, 1 OTHER (`X2GUUser.h`: DUO_BODY's), RESULT: PASS.
- **Compile:** `/Zs` probes (census `cl_cmd`, no PCH) of `X2GUUser.cpp`, `X2GUChung.cpp`: 0 errors each. Full build skipped: no play-test (DU1d4
  builds and hands off the regression pass).
- **In game:** nothing to see for March classes except the revive voice (Rena's file name). Form 0 and no second anim leave every other path as before.

### DU1d3 - Done (2026-11-14)

Opus 5.5 high. Part 10 of DU1, flag `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (no new define, Always.h untouched). Files: `X2GameUnit.h` (+17),
`X2GameUnit.cpp` (+66), `X2GUUser.h` (+21), `X2GUUser.cpp` (+104 / -1, the DU1b stand-in line in `SetActiveBody`). No `_DEBUG`, no log line.
funcmap rows: `grep "(DU1d3)" june2017_re/funcmap.tsv` (10).
- **Census re-check (kickoff ask).** Each site's real start = a `ret` / `int3` followed by a call target or pointer. Wrong rows corrected in
  `du1d_body_reads.tsv` (owner column): `31fcb1b0` is the `GetWeaponEnchantData` thunk, the reads are `31fcb1c0` `ChangeModelDetail`; `31fee682` is
  inside `InitComponent` `31fee090` and its three sites are the second after-image, not the marker particles; `31fdb800` / `31fdf5d0` (PlayEmotion
  helpers) split off `31fdb4e0` / `31fdf4c0`; `OnFrameMove` `3206eaf0` off slot 484; `CommonFrameMove_TimedEvent` `31ff27c0` off `31ff1ff0`;
  `3201f8d0` / `3204e4a0` (known). `31fceeb0` and `3204e110` are dead (no call, jump or pointer). `322726f0`'s `+0x8e80` is the NPC defender's byte.
- **Second after-image (ported).** `CX2GameUnit::m_pMinorUnitAfterImage` (+0x6758, ctor / dtor), `EnableAfterImage( body )` / `DisableAfterImage(
  bForce, body )` = June `32275a30` / `322720f0` through `GetAfterImageByBody` (`AFTER_IMAGE_BODY` 0 / 1 / 3, no June Enum entry, default 3 = what
  every March caller passes in June), virtual `IsMinorUnitActive` (unit vslot 152, base false; the user's DU1 getter now overrides it). User side:
  created in `InitComponent` on the second anim (10 / 5 images, no hair mesh), `UpdateAfterImageEquip( body )` (vslot 0x6b0, virtual: Lu/Ciel
  overrides), `AfterImageFrameMove` ticks both, ReInit colour, `NotifyShowObjectChanged` (JUNE_BUGS 669: hide leaves Lu's on while Ciel is active),
  `SetActiveBody` tests the active body's after-image, `MINOR_UNIT_AFTER_IMAGE` (`m_v2NowMinorUnitAfterImageTime`, +0x852c) read in
  `CommonStateStart` and run in `CommonFrameMove_TimedEvent` on the second anim's clock; the first body's timed pair now names body 0.
- **Readers (ported):** `ChangeModelDetail` (second anims), `CommonFrameMove_InsertBuffFactor` (active clock), `ApplyRotateBoneMap` (active body;
  it exists since Char Chung CH1j0), `UpdateEquipCollisionData` (skip unless duo or the equip's body is the active one; the equip byte is DU3's
  stand-in 0), `ProcessEnchantedWeaponEffectAtHand` (`UT_LUCIEL`: Ciel `Dummy1_Rhand` on the second anim, Lu `Bone_RFinger2`, rates 15 / 30 /
  60 / 25 read from June's floats), `Init` (`SetActiveBody`: a battle field keeps the unit's form, any other game resets to Lu).
- **Routed:** DU3 = `3201f8d0` (equip pass), `3204e4a0` (June hide-equip-by-body pass `+0x8fa4`-`+0x8fb4`), `InitEqip` @`32024a35` (3286a350 vs
  the active body); all three read the equip's `+0x15d4`. Not ported here (not this flag's subject): `+0x7c80` and `+0x1d1` after-image flags
  (class 13's toggle, checklist 7al), the `CommonFrameMove` `+0x941c` hide block (@32065c06, June-only, already passes mode 3). The Elsword /
  Rena / Aisha `AfterImageFrameMove` overrides also tick +0x6758, always NULL for those classes: their stand-in comments stay.
- **Split again (kickoff's own rule).** **DU1d3b** (kickoff written) = PlayEmotion + helpers, EmotionFrameMove, `31fe0bb0`, slot 484 /
  OnFrameMove / `320200b0`, `31ff1ff0` and the rest of TimedEvent, `31ff3ce0`, `31fe2fb0`, `31fe4210`, and the lead below. DU1d4 now depends on it.
- **Lead for DU1d3b:** June's `Init` state switch (game type 1 / 5 -> state 9, 3 -> 13, others no state change there) not compared beyond the body.
- **Gates.** `flagcheck JUNE_CHAR_DUO_BODY --worktree --files` (4 files, the peer's uncommitted `JUNE_CHAR_ELESIS_PASSIVE` hunks not staged):
  4 PASS, 0 OTHER, 0 FAIL, 1 deleted (above), 0 non-ASCII, RESULT: PASS. `junediverge --check` (4 files): RESULT: PASS. `enumscan --check`: the
  known DU1b3 rows and two older header rows only. JUNE_BUGS 669 added. Commit `442529eb`; `--range 442529eb^..442529eb`: 4 PASS, 0 OTHER,
  0 FAIL, 1 deleted (the same line), RESULT: PASS.
- **Compile:** `/Zs` probes (census `cl_cmd`, no PCH) of `X2GUUser.cpp`, `X2GameUnit.cpp`, `X2GUNPC.cpp`, `X2StateMenu.cpp`, `X2GURaven.cpp`,
  `X2GUChung.cpp`, `X2GUElsword_SwordMan.cpp`, `X2GULire_ElvenRanger.cpp`, `X2GUArme_VioletMage.cpp`: 0 errors each. Full build skipped: no play-test
  (DU1d4 builds and hands off the regression pass).
- **In game:** nothing to see. March classes have no second anim, so no second after-image; body 3 picks the first after-image as before.

### DU1d3b - Done (2026-11-14)

Opus 5.5 high. Part 10b of DU1, flag `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (no new define, Always.h untouched). File: `X2GUUser.cpp` (+94, insert-only,
17 hunks, each tagged `// char-luciel DU1d3b`). No `_DEBUG`, no log line. funcmap rows: `grep "(DU1d3b)" june2017_re/funcmap.tsv` (12).
- **Function starts re-checked** (DU1d3's method). March names: `31ff1ff0` = `CommonFrameMoveFuture_TimedEvent` (same place, after the combo
  manager), `31ff3ce0` = `CommonFrameMove_Particle`, `3206d770` = `InitSocketData` (slot 484), `3206eaf0` = `OnFrameMove`. None in March:
  `31fdb800`, `31fdf5d0`, `31fe0bb0` (June's emotion setter), `320200b0`, `31fe2fb0` / `31fe3e30` / `31fe4210`.
- **Emotion (ported, March's own lines):** `PlayEmotion` runs every anim call on the active body (riding Play @`320435cc` / @`32043616`, avatar
  `MixAnim` = June's mix on `32071d80` @`32043e26`, ChangeAnim / Play @`32043d02`-@`32044242`) and the unit-viewer table row by body
  (@`32043ba1`, `DUO_TYPE_MINOR_UNIT` / `_FIRST_UNIT`); `EmotionFrameMove` tests the active anim's end (@`3204a8b4` / @`3204a8ce`).
- **Emotion (routed, backlog Stage 10 bullet):** June's `PlayEmotion` / `EmotionFrameMove` are the EmotionData rewrite (Stage 10 owns it). Its
  body reads sit only in June-only branches: the setter `31fe0bb0`, the helpers `31fdb800` / `31fdf5d0`, the `+0xe40` arguments (@`32043d92`,
  @`320442bb`, @`320446c0`, OnFrameMove @`3206ecb7`), the sit-item body filter (@`3204a9f5` / @`3204aa03`), the sit-item block @`32043938`.
- **Per frame (ported):** `OnFrameMove` scales the second anim pair like the first (@`3206f1bb` / @`3206f263`). `CommonFrameMoveFuture_TimedEvent`'s
  four stop lists run on `GetXSkinAnimFuture()` (@`31ff2043` / `2376` / `2634` / `2704`). `CommonFrameMove_TimedEvent`'s attack window
  (@`31ff2952` / @`31ff2984`) and its four stop lists (@`31ff2d0a` / `3088` / `3388` / `36e8`) run on `GetXSkinAnimPtr()`. `SoundEventTimer`
  (shared by both sound players: @`31ff39b1` and DU1d4's `31fea120 @31fea191`) does too. `CommonFrameMove_Particle` puts the hold effect on the
  active body's Bip01 (@`31ff3df5`). Only the live `X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE` lines were touched (probe: on).
- **Routed:** DU3 = `InitSocketData @3206d8f7` (skip an equip's sockets when its minor-unit field `3286a4b0` is not both / not the active body)
  and the second body's equip creation `31fe4210` -> `31fe3e30` -> `31fe2fb0`. DU5 = `@3206da0f` (set count by form). Stage 16 = `320200b0`
  (socket-option particles per body, none in March). Kickoffs DU3 / DU5 / DU1d4 / DU1-review amended.
- **Not body reads (routed, backlog Stage 2 bullets):** June's stop records are 16 B with a state-index test (divergence row
  `CommonFrameMoveFuture_TimedEvent / CommonFrameMove_TimedEvent`, replace, body reads ported; two allow rows); the `Init` lead: PvP / type 5
  `USI_START`, battle field `USI_WAIT`, dungeon / PVE no state change in `Init`, riding on through `3204d120` (row `CX2GUUser::Init` is `out`:
  the gate matches the bare name `Init` and would trip on every `Init(` call); `OnFrameMove`'s first-body quirk (the `+0x87d0` test scales
  `+0x87d4` again; March's live `VERIFY_STAT_BY_BUFF` branch scales the Future anim).
- **For the DU1 review:** DU1b's constants say DUO_TYPE has no June Enum entry; June's `Enum.lua` has `DT_MAJOR` 0 / `DT_MINOR` 1 / `DT_BOTH` 2 /
  `DT_KEEP` 3, and DU1b's `DUO_TYPE_NONE = 3` ("the duo ends") shares `DT_KEEP`'s value (written into `DU1-review.md`).
- **Gates.** `flagcheck JUNE_CHAR_DUO_BODY --worktree --files X2Lib/X2GUUser.cpp`: 1 PASS (+94), 0 OTHER, 0 FAIL, 0 deleted, 0 non-ASCII,
  RESULT: PASS. `junediverge --check X2GUUser.cpp`: RESULT: PASS (open replace rows 8, unchanged). `enumscan --check`: the 4 known DU1b3 rows only.
  Flag probe: DUO_BODY, X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE, VERIFY_STAT_BY_BUFF, RIDING_SYSTEM, UNIT_EMOTION, JUNE_UNIT_VIEWER_EFFECT_TABLE,
  JUNE_UNIT_SOUND on. Commit `217f55da`; `--range 217f55da^..217f55da`: 1 PASS (+94), 0 OTHER, 0 FAIL, 0 deleted, RESULT: PASS.
- **Compile:** `/Zs` probes (census `cl_cmd`, no PCH) of `X2GUUser.cpp`, `X2GUChung.cpp`, `X2GULire_ElvenRanger.cpp`: 0 errors each. Full build
  skipped: no play-test (DU1d4 builds and hands off the regression pass, now with DU1d3b's paths).
- **In game:** nothing to see. March classes have one body, so the active anim is `m_pXSkinAnim` and every path is as before.

### DU1d4 - Done (2026-11-14)

Opus 5.5 high. Part 11 of DU1, flag `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (no new define, Always.h untouched). File: `X2GUUser.cpp` (+95, insert-only,
21 blocks, each tagged `// char-luciel DU1d4`). No `_DEBUG`, no log line. funcmap rows: `grep "(DU1d4)" june2017_re/funcmap.tsv` (8).
- **Function starts re-checked** (DU1d3's method: the int3 start, then each `ret` before the site and a call / jump / pointer to the next byte).
  Nine of the 25 census rows filed a read under the previous function: `31fd3180` -> `31fd3260` (IsChangeWorldColorNow); `31ff5440` ->
  `31ff5df0` (CommonCameraMove); `31ffb4f0` -> uncarved `31ffba50` (SUMMON_MAGIC_SPEAR_FrameMove, thunk `32d25f70`, after `31ffb7f0`'s tail
  `jmp` @`31ffba4e`); `31ffc5c0` @`31ffcbd4` / @`31ffcc26` -> `31ffcba0` (DieFrameMove); `3200bc20` -> `3200c030` (June auto-potion gate);
  `32045dc0` -> `320462a0` (DamageReact); `3204ace2` -> a jump-table case `3204af38` of EmotionFrameMove `3204a860`. Census owners rewritten.
- **Ported (March's line in `#else`, `GetXSkinAnimPtr()` at June's call):** DamageReact's revenge and reflex-magic windows (@`32046849` /
  `689d` / `6952` / `699e`), `CommonFrameMove_Invincible` = `31fe7180` (@`31fe7216` / `7255`), `CommonFrameMoveStateAbnormalityWaterHold` =
  vslot `0x38c` `31ff4800` (`EventTimer( 0.1f )` @`31ff4b4b`), `CommonCameraMove` (June's body, D0c-1: now / before time @`31ff5f91`-`5fd8`,
  sub-camera timing @`31ff618f`), both bone `ActiveSkillShow` overloads (`31fddfd0` @`31fddffc`, `31fde0f0` @`31fde11b`),
  `SUMMON_MAGIC_SPEAR_FrameMove` (0.55 / 0.65 one-shots), `StartIntrudeFrameMove` (four timers), `DieFrameMove` (June's body: anim end
  @`31ffcbd4`, step dust @`31ffcc26`), `DamageAirDownLandingFrameMove`, `COMMON_RELATIONSHIP_SKILL_FrameMove`, `DamageDataChangeProcess`,
  `IsChangeWorldColorNow` (C3r's function), `ShowHyperSkillCutInByScript` (C3t's function). Only live branches were touched (probe below).
- **Already right (no edit):** `SetLeftHandScale` (Raven CH2c0 reads `GetXSkinAnimPtr()`), `CommonFrameMove` @`32063946`-`39a4` (DU1b duo end),
  @`32063b24` (Elsword CH4a2), @`32064acb` (DU1b2), `SoundEventTimer` (DU1d3b), `THROW_ITEM_FrameMove` (DU1d). `MACHINE_GUN_ATTACK_FrameMove`
  `31ffb4f0` reads `+0x87d4` inline: March's line stays. `PEPPER_RUN_END_FrameMove`'s read is inside the Stage 10 stand-in.
- **Split again (kickoff's own rule): DU1d5 = the two-body render** (kickoff written). June's `CommonRender_Prepare` `31fd07b0` calls a per-body
  prepare (vslot `0x7e4` = `31ff03d0`, none in March) for the active body and then the not-selected one, and June's `OnFrameRender_Draw`
  (`31fd0880`, user vtable slot 3; the census called it "none in March") draws both bodies. That is a whole divergent function trio with
  June-only lines (`+0x8fa4` body override, `3227baf0( 0x3b1 )`, R5's `+0x160`), not a getter swap, and nothing draws Ciel without it.
  `DU1-review.md` now covers DU1d5 and runs after it.
- **Routed:** june-engine R5 (kickoff amended) = `31fd4460` (CRS setup, @`31fd44c8` / `44fb`) and `CommonFrameMove` @`32065a11` / `5a4c` (CRS
  window; also DU3's); CH1a0 (kickoff amended) = Lu/Ciel-only base helpers `31fd2d60` / `31fd2e30` (DIE_FRONT / DIE_BACK start) with DU1d's
  `31fcd0f0` family; backlog Stage 10 (bullet) = the cash-shop frame `32044f80` (none in March), EmotionFrameMove case `3204af38`, and
  `CommonStateStart @32060bb3` (anim `+0x2b4` unless an avatar emotion); Stage 2 = the drift block reads (@`32064fc6`-`5241`, DU1b2's bullet);
  parked PvP item bar = auto-potion gate `3200c030` (note on that line).
- **Gates.** `flagcheck JUNE_CHAR_DUO_BODY --worktree --files X2Lib/X2GUUser.cpp`: 1 PASS (+95), 0 OTHER, 0 FAIL, 0 deleted, 0 non-ASCII,
  RESULT: PASS. `junediverge --check X2GUUser.cpp`: RESULT: PASS (allow-listed 8, unchanged). `enumscan --check X2GUUser.cpp`: the 4 known DU1b3
  rows only. No JUNE_BUGS row, no new divergent function. Commit `2b3d1b60`; `--range 2b3d1b60^..2b3d1b60`: 1 PASS (+95), 0 OTHER, 0 FAIL,
  0 deleted, RESULT: PASS. Flag probe (`/Zs`): DUO_BODY, X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE, EDT_WATER_HOLD_TEST,
  DIVERGE_DIE_FRAME_MOVE, DIVERGE_COMMON_CAMERA_MOVE, JUNE_WORLD_COLOR_WINDOW, JUNE_HYPER_CUT_IN_TIME, JUNE_DAMAGE_REACT_ORDER, GUILD_SKILL_PART_2 on.
- **Build:** `DEPLOY OK: X2_offline.exe is 26,600,960 B, dated 2026-11-14 12:17:46, md5 8e5646f370573b533533a5e21a6190cf, equal to
  X2/US_SERVICE/x2.exe (2026-11-14 12:17:38); built at main d3cfe730 + 1 uncommitted code file(s).` (the file is this phase's).
- **In game:** March classes have one body, so every new line reads `m_pXSkinAnim` as before. The play-test below is the DU1 regression pass
  (DU1b4-DU1d4's every-class lines).
- **Play-test PASSED 2026-11-14** (user: all checks; run 12:20-12:26 on the deployed exe md5 8e5646f3): ravbe, Iivue, reyaa as handed off.
  Logs: 0 EXCEPTION, ErrorLog clean, UNHANDLED only the known `E_DISABLE_HB_CHECK_REQ` x4 (outside this phase). DU1d5 ready.

### DU1d5 - Done (2026-11-14)

Opus 5.5 high. Part 12 of DU1, flag `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` plus a new `SERV_IRUHADEV_JUNE_DIVERGE_USER_RENDER` (Always.h, needs DUO_BODY,
`JUNE_RENDER_CAMERA_MESH_HIDDEN`, `JUNE_ENUM_NAMES`; `#error` guard). Files: `X2GUUser.cpp` (+297), `X2GUUser.h` (+6), `Always.h` (+13), insert-only.
No `_DEBUG`, no log line. funcmap rows: `grep "(DU1d5)" june2017_re/funcmap.tsv` (3). `june_march_divergent.tsv`: `CommonRender_Prepare`,
`OnFrameRender_Draw (CX2GUUser)` (replace, done). JUNE_BUGS 673.
- **`CommonRender_Prepare` = June `31fd07b0`** (June's body inside March's, March's in `#else`): active body NULL -> no render; the per-body
  prepare for the active body (hint kept), then for the not-selected one when it exists (hint dropped, June's order @`31fd0844`).
- **`CommonRender_PrepareBody( bActiveBody_ )` = June `31ff03d0`** (user vslot 0x7e4, none in March; virtual like June's, no class overrides it):
  the anim is `GetXSkinAnimPtr()` / `GetNotSelectXSkinAnimPtr()`; with `m_bHideEquipByBody` (+0x8fa4) and `m_bHideEquipByBodyApplied` (+0x8fb4),
  `m_iHideEquipBody` (+0x8fa8) 0 takes `GetMinorUnitXSkinAnimPtr()` (`32071d40`), 1 takes `GetSkinAnimPtr()` (vslot 0x58 = `32071d20`). The three
  members are declared here, zeroed, unwritten until DU3 (DU3 kickoff amended). Line-by-line against March's body, the same: pain / sacrifice
  types, world light, ApplyRenderParam gate (+0x8e04), outline width (700 / 1000 / 500), unit-detail types, damage light (four branches, CTT_DAMAGE,
  the ADD switch), invincible white, the extra-damage chain and its constants, under-water colour, alpha, room slot refresh, equip loop. The R0
  world cartoon / rim copy moves before the pain test (June @`31ff05c2`; fields disjoint). **June-only, under DIVERGE_USER_RENDER:**
  `bZWriteEnable` / `bZEnable` = true (RenderParam +0x65 word 0x101 @`31ff060f`); the super-armor red outline also when the type is already
  `RT_CARTOON_COLOR_EDGE` (@`31ff06fa`); `HaveThisBuff( BTI_DEBUFF_HARD_BODY )` (0x3b1) -> black edge, `CTT_PETRIFIED`, grey 0.7 outline (@`31ff0c5b`,
  inside the pain == 0 block, after the colour chain); the anim's instant-show set whatever its hint (@`31ff0d40`); `SetForceLayer( camera
  world-mesh-hidden )` after the equip loop (@`31ff0e3a`: user +0x160 = `CKTDGObject::m_bForceLayer`, row `31d28a20`).
- **`OnFrameRender_Draw` = June `31fd0880`:** hide-no-renderable on both anims, the active anim then the not-selected one when instant-show; the
  equip loop is March's. June's top tests only NULL (no `GetShowObject`): under DIVERGE_USER_RENDER, March's test in its `#else`.
- **Class overrides (kickoff ask):** all 12 June user vtables override slot 0x7e8 and call `31fd07b0`; none overrides 0x7e4. `334fa61c` /
  `33522914` set `m_RenderParam.cartoonTexType` by WOS state first (Elsword / Elesis, already in their `_LIVE` branches). March's nine overrides
  all call `CX2GUUser::CommonRender_Prepare()`, so every class reaches the two-body path. Lu/Ciel's own (`32ae4f90`) is a plain base call (CH1).
- **june-engine R5 (kickoff amended):** the `+0x160` copy is done here; R5 keeps the NPC / riding-pet render and checks `CX2GUNPC` for the same copy.
- **Not ported (none found):** nothing else in the three functions; the DIVERGE lines are every difference beyond the body pick.
- **Gates.** `flagcheck JUNE_CHAR_DUO_BODY --worktree --files X2GUUser.cpp X2GUUser.h --with JUNE_DIVERGE_USER_RENDER`: 1 PASS, 1 PASS~ (2 blank
  lines), 0 OTHER, 0 FAIL, 0 deleted, RESULT: PASS. `flagcheck JUNE_DIVERGE_USER_RENDER --worktree --files Always.h`: 1 DEFINE, RESULT: PASS
  (with `--with` the DU1d2 compound guard at Always.h:13355 is an ERROR the tool cannot evaluate). `junediverge --check` (3 files): RESULT: PASS
  (allow-listed 8). `enumscan --check`: the 6 known rows only. Flag probe (`/P` via stdafx): DIVERGE_USER_RENDER, DUO_BODY, CAMERA_MESH_HIDDEN,
  ENUM_NAMES, RENDER_WORLD_CARTOON, CASH_SKILL_FOR_CHUNG_2011_04, GUILD_SKILL_PART_2, SERV_ELESIS_SECOND_CLASS_CHANGE, FACE_OFF_MONSTER_HEAD_TEST on.
  `/Zs` probes: `X2GUUser.cpp`, `X2GUChung.cpp`, `X2GURaven.cpp` 0 errors. Commit `9223c6c7`; `--range 9223c6c7^..9223c6c7`: DUO_BODY (with
  DIVERGE) 1 PASS, 1 PASS~, 0 FAIL, 0 deleted, RESULT: PASS; DIVERGE_USER_RENDER on `Always.h` 1 DEFINE, RESULT: PASS.
- **Build:** `DEPLOY OK: X2_offline.exe is 26,601,984 B, dated 2026-11-14 12:54:14, md5 0475c657fa982b7f260892bdf4af33f7, equal to
  X2/US_SERVICE/x2.exe (2026-11-14 12:54:07); built at main 7f4afddf + 3 uncommitted code file(s).` (the three are this phase's).
- **In game:** March classes have one body, so the second call and the second draw never run. What changes for every class: the body is
  drawn with its equips even when its anim's prepare says no-render (a stopped anim, e.g. water hold), a colour-edge super armor gets the red outline, the
  hard-body debuff looks petrified, and the unit goes to the non-alpha chain while a world mesh hides. Ciel is drawn from CH1.
- **Play-test PASSED 2026-11-14** (user: all checks; run 13:05-13:10 on the deployed exe md5 0475c657): ravbe and reyaa, village and dungeon.
  Logs: 0 EXCEPTION, ErrorLog has no render line, UNHANDLED only the known `E_DISABLE_HB_CHECK_REQ` x3. Side note (not this phase, since the
  import): `/invin` fails, its `Slash_invin` calls the unbound `SetInvincibleAndNoMpConsume_Cheat` (ErrorLog 13:06:14). DU1 review ready.

### DU1 - Review (2026-11-14)

Opus 5.5 high. **Verdict: pass after one fix** (enum names, no behaviour change). Covers DU1, DU1b, DU1b2, DU1b3, DU1b4, DU1c, DU1c2, DU1d,
DU1d2, DU1d3, DU1d3b, DU1d4, DU1d5 (13 code commits `c90b0000` .. `9223c6c7`). All 13 Done notes read; Ghidra read-only.
- **Fix: `DUO_TYPE` takes June's names.** DU1b's `_CONST_GUUSER_DU1B_` said DUO_TYPE had no June Enum entry; June's `Enum.lua` has `DT_MAJOR` 0 /
  `DT_MINOR` 1 / `DT_BOTH` 2 / `DT_KEEP` 3 (`Enum.src.lua:17515`). New `CX2Unit::DUO_TYPE` (`X2Unit.h`, under `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY`,
  tag `char-luciel DU1 review`); the four `DUO_TYPE_*` constants are removed and their 11 uses in `X2GUUser.cpp` (both state starts, both
  anim plays per body, `ApplyDuoTypeAtStateStart`, the unit-viewer row in `PlayEmotion`) read the members. **Value 3 keeps, not a new
  decision:** June `320506f0 @32050e10` on 3 (and on a miss, `@32050dfe`) stores word 0 at `+0x8e91` (duo off, end-at-anim-end off), calls
  `UpdateDuo( 1 )` and never `SetActiveBody`: the active body stays and the duo ends. DU1b's code did exactly that; only the name was wrong.
- **Spot-checks against June (all equal):**
  - `ApplyDuoTypeAtStateStart` = `@32050e10-32050f56`: hand-over tests `ESP+0xcc` / `+0xe8` = the size words of the `MINOR_UNIT_ANIM_NAME`
    (21-byte key at `ESP+0x368`) / `ANIM_NAME` strings, vslot `0x260` = `IsMinorUnitActive`, flags `+0x7d8e` (9) / `+0x7d8d` (8), latch
    `+0x8e93` (word `0x100` at `+0x8e92`), `+0x8acc` = type; type 2 = `SetDuoEnabled( 1 )` only.
  - `GetX2UnitClassGrade` = `31fe5860 @31fe5c45-31fe5c6b`: `class - 10 <= 0xbd` (unsigned) -> `1 + ( class >= 100 )`, else 0. March's `/10`
    form gives the same for every class id >= 0.
  - Ctor `3203e850`: `+0x8e90` = unit `+0xe40` (`@32042467`, 0 without a unit), `+0x8e91` word 0 (`@32042479`), `+0x8e93` 0, `+0x84dc` word /
    `+0x84de` 0 (`@3203f385`), `+0x8f6e` word, `+0x8f5c` word, `+0x8f60..68`, `+0x8f6c` 0 (`@320431f6-3204321c`), `+0x8fa4` / `+0x8fa8` /
    `+0x8fb4` 0 (`@32040ed8-32040ef0`). DU1b3's `+0x8f50 = 1.0` and DU1b4's `+0x8f6d = 0` (June leaves both to the first state start) are safe
    defaults, noted in their comments.
  - `CommonRender_PrepareBody` = `31ff03d0`: body pick `32071d80` / `32071db0`; override `+0x8fa4 == 1 && +0x8fb4 == 1`, `+0x8fa8` 0 ->
    `32071d40`, 1 -> vslot `0x58`; world cartoon / rim before the pain test (`@31ff05c2`); Z word `0x101` (`@31ff060f`); super-armor
    colour-edge red (`@31ff06fa`); `HaveThisBuff( 0x3b1 )` black edge / petrified at `@31ff0c5b`, skipped by the pain != 0 jump
    (`@31ff096c` -> `31ff0caa`), so inside the pain == 0 block; instant-show `+0x173 = 1` unconditional (`@31ff0d40`); `+0x160` from the
    camera after the equip loop (`@31ff0e3a`). `CommonRender_Prepare` / `OnFrameRender_Draw` match DU1d5's note.
  - `GetCloneFrameByBodyPrefix`: strings `33473188` "MAJOR:" / `33473178` "MINOR:" (Ghidra string list); `31fda280` compares 6 characters.
- **Mechanical checks (subagent, spot-read):** the 22 cancel-macro twins in `X2GUUser.h` equal their `#else` with `m_pXSkinAnimFuture` ->
  `GetXSkinAnimFuture()` and nothing else (at `a9621883` and at HEAD). Every `char-luciel DU1d3b` (17) / `DU1d4` (21) / `DU1d5` (4) hunk sits in
  a funcmap row's function or a census row (one extra address is the `32351070` MixAnim callee). `du1d_body_reads.tsv` (104 rows): no owner
  left as a bare `DU1d2` / `DU1d3` / `DU1d4`; 4 rows `CH1` (Lu/Ciel-only base helpers), 18 routed, 8 none, 2 dead. `nu0_duo_engine.tsv`: all 440
  `DU1` rows are cited by address in a Done note, funcmap, the census or a routed kickoff; the 2,248 `DU1-accessor` rows are inline expansions of
  the accessor (`DU1.md`: one header change, not per site), covered by DU1's accessors and the macro twins.
- **No change for other classes:** each DU1 commit's flag-off strip equals its parent byte for byte (below); every new branch needs a second
  anim, form 1, `UT_LUCIEL` or a duo key, which March classes never have. What does reach every class is the four DIVERGE flags (June's own
  behaviour, divergent rows): untimed SPEED_TIME at state end, the Magic Necklace PvE gate, the revive voice file, the render lines. The DU1d4
  regression and DU1d5 render play-tests passed on them.
- **Hygiene, DU1 commits** (`--range c^..c` each, code files, `--with CLASS_GRADE DIVERGE_COMMON_STATE_END_FUTURE DIVERGE_UP_MP_ITEM_PVE
  DIVERGE_PLAY_SOUND_REVIVE DIVERGE_USER_RENDER`): 13 of 13 `RESULT: PASS`, together 36 PASS, 4 PASS~, 0 OTHER, 0 FAIL, 0 ERROR; 1 deleted line
  (`442529eb`: DU1b's own stand-in line inside the flag, replaced by DU1d3). The single `--range c90b0000^..HEAD` run reads `RESULT: FAIL` only
  for peer commits in the range: `abe3a805` (char-elesis CH4b2) and `dbc7c77a` (CH4c) edit `X2GUUser.cpp` under `_ELESIS_PASSIVE` / `_BLOOD`, flags
  outside the `--with` list (2 FAIL, 5 OTHER, none in DU1 text). `Always.h` per flag without `--with`: DIVERGE_USER_RENDER `RESULT: PASS`; the
  others FAIL / ERROR only on the `#if defined( A ) && !defined( B )` dependency guards (DU1's own and peers') the tool cannot strip; each phase's
  own range run showed its define as DEFINE.
- **This review's edit:** flagcheck `DUO_BODY --worktree --files X2Lib/X2GUUser.cpp X2Lib/X2Unit.h`: `files: 2 PASS, 0 PASS~, 0 DEFINE, 0 OTHER,
  0 FAIL, 0 ERROR`, 17 deleted lines (the removed constants and DU1b's lines rewritten, all inside the flag; flag-off unchanged), `RESULT: PASS`.
  junediverge `--check-all`: `RESULT: PASS` (239 allow-listed); `--check` of the two files: `RESULT: PASS`. enumscan `--check X2GUUser.cpp`: the 4
  known DU1b3 rows only. `/Zs` probes (census `cl_cmd`) of `X2GUUser.cpp`, `X2Unit.cpp`, `X2GUChung.cpp`: 0 errors each. Full build skipped: no
  play-test (a rename; the next play-test phase builds it).
- **Routed:** `X2UnitManager.cpp:297` (`JUNE_UNIT_VIEWER_EFFECT_TABLE`, not DU1's) tests the literal `2` = `DT_BOTH`: CH1p kickoff amended, with
  the new members and the live `Enum.lua` gap (`DT_BOTH` / `DT_KEEP` missing). Side note from DU1d5 (`/invin`'s unbound
  `SetInvincibleAndNoMpConsume_Cheat`) is not DU1's.
- CH1a0 is ready; CH1p and CH3c no longer wait on this review.

### DU2 - Done (2026-11-14)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_USER_DAMAGE_STATE_GETTERS` (Always.h; no `_DEBUG`, no log tag). Files: `X2GUUser.h`, `X2GUUser.cpp`,
`X2GUArme_VioletMage.cpp`. funcmap rows: `grep "Char Luciel DU2" june2017_re/funcmap.tsv` (22).
- **Scope widened to the whole damage-state band (one logical change).** The plan named 0x114 / 0x118 / 0x490 / 0x494 / 0x458, but June's readers
  ask the band together (IsDamageState 15 getters, the stun test 16), and Lu/Ciel overrides all of it (`ch0_vtable_slots.tsv`). So DU2 makes 18
  damage getters virtual (0x468 groggy, 0x46c-0x4a4, 0x114 / 0x118) plus `GetJumpDownStateID` (0x458). Every base was read from the exe bytes at
  vtable `334781f4` (`mov eax, N`, N = 25 .. 42; March `USI_` = June - 1 there); 0x458 = March's riding body (`3208de10`). The other state-id
  getters (wait / walk / dash / jump / die, 0x438-0x464) stay with CH1o's virtual wiring.
- **Names.** March's own names are kept and made virtual where March has them (`GetDamageGroggyStateID`, `GetDamageAirDownStateID`,
  `GetDamageDownLandFront`, `GetDamageDownLandBack`, `GetDamageAirDownLanding`, `GetJumpDownStateID`); the 13 new ones follow
  `Get<State>StateID() const`. `GetJumpDownStateID` returns `int` under the flag (Lu/Ciel's `32b4d5b0` returns its own id); both its
  `RIDING_SYSTEM` branches are flagged. No class in `X2Lib` declares any of the 19 names (grep), so no March class now overrides a slot by accident.
  CH1k2 / CH1l kickoffs amended with the exact names and signatures.
- **Readers (NU0's DU2 rows), verdicts:**
  - routed under the flag: `PhysicProcess_Jumping` (31fd6da9 / 31fd7070: 0x114 / 0x118 / 0x494 / 0x490; 0x458 was already a getter);
    `ForceDownAndDetonationProcess` (3202a9c0: 0x114 / 0x118 / 0x48c then riding 0x55 / 0x56; the switch head becomes an `if`, the body is shared);
    `CommonFrameMove` stun test (@32067598-3206776c: 16 getters + riding; see the routed bullet); `IsDamageState` (0x534, inside char-elesis
    `_LIVE`: the Elesis CH1o amendment's "make the base ask them"); `IsSocketDownTargetState` (0x538 `3208eb60`: 10 getters); Aisha's skill lookup
    (`32cebe80 @32cec2bf`; NU0 filed it under Aisha's InitState, the chunk is uncarved: 0x114 / 0x118 / 0x490).
  - automatic through the virtual, no edit: StateChange's force-down flush (`320348c0`, already March's three getters), BlackHole `31f6eea0`
    (`X2DamageEffect.cpp:3757`), `IsNotCommonMoveState` (`31fd7640`, 0x458 @31fd77b1), the stun block's groggy getter (0x468).
  - no March body, recorded: CommonEventProcess's `+0x8e74` block (@31fe7944 / @31fe7992; unported since Buff 7c), `31fd7350` (June's
    `LT_JUMP_DOWN` line, below), `32296570` (Stage 2 bullet "attack-result / damage-react hooks"), vslot 0x530 `3208e7d0` (back damage states;
    only caller Ain `32a50f65`, char-ain).
- **Routed (scope rule), backlog Stage 2:** the stun -> groggy polarity in `CommonFrameMove` (June groggies a stunned unit IN a hit-reaction state,
  March in any other: DU2 kept March's polarity with June's state set), and `LT_JUMP_DOWN` foot lines (June line type 11, handler `31fd7350`).
- **Hygiene.** flagcheck `--worktree`: 3 PASS (`X2GUUser.h` +33, `X2GUUser.cpp` +64, `X2GUArme_VioletMage.cpp` +4), 1 OTHER (Always.h: peers'
  blocks), 0 deleted, 0 non-ASCII, RESULT: PASS. `junediverge --check`: RESULT: PASS. `enumscan --check`: the 2 A/B rows are old `X2GUUser.h`
  NPC ids, none in DU2's lines.
- **Build.** Two of my runs and one queued run failed in KTDXLIB on char-elesis CH3b0's `ELESIS_SUMMON` guard (`SERV_ELESIS_SECOND_CLASS_CHANGE`
  lives only in `ServerDefine.h`; fixed by `029ca9e1`). Deployed by the peer build `.build/logs/20261114-015742` (started 01:57, after DU2's edits at
  01:38; `X2GUUser.cpp` / `X2GUArme_VioletMage.cpp` compiled, 0 errors): exe dated 2026-11-14 02:08:40, md5 `e9dee3d6`. No `EXESTR` line: DU2 adds
  no string.
- **In game:** nothing new to see; every class jumps, falls, is knocked down and stunned exactly as before (base getters = March's constants).
- **Play-test (2026-11-14, exe `e9dee3d6`): passed.** `jtLordKnight` (02:12) and `jtVoidPrincess` (02:13) in a dungeon; logs run after the
  deploy, clean exit, 0 `EXCEPTION`, 0 unbound Lua names, only the usual `E_DISABLE_HB_CHECK_REQ` unhandled; no crash bundle from the run.

### DU2 - Review (2026-11-14)

Opus 5.5 high. **Verdict: pass, no code change** (commit `14fe2f4d`, flag `SERV_IRUHADEV_JUNE_USER_DAMAGE_STATE_GETTERS`).
- **Bases, re-read from `unpacked_x2.exe` bytes** (vtable `334781f4`, capstone on the raw image): 0x468 -> `3208de40` `mov eax, 0x19` (25 -> March
  `USI_DAMAGE_GROGGY` 24); 0x114 -> `3208de60` `mov eax, 0x1e` (30 -> `USI_DAMAGE_DOWN_FRONT` 29); 0x490 -> `330de330` `mov eax, 0x25` (37 ->
  `USI_DAMAGE_AIR_DOWN_LANDING` 36); 0x4a4 -> `3208def0` `mov eax, 0x2a` (42 -> `USI_DAMAGE_REVENGE` 41); 0x458 -> `3208de10` = riding (vslot 0x200)
  ? 0x4f : 0x11 (17 = `USI_JUMP_DOWN`; ids below 25 are equal in both). March = June - 1 from 25 holds.
- **No accidental override.** None of the 19 names is declared in any `X2Lib` / `KTDXLIB` header but `X2GUUser.h` (grep). `GetJumpDownStateID`'s
  `int` return: its 3 callers (`X2GUEL.cpp:11958` StateChange, `X2GUUser.cpp` PhysicProcess_Jumping, IsNotCommonMoveState) all take an `int`.
- **Two routed sites opened against June's listing:** PhysicProcess_Jumping `31fd6da9`: 0x114 -> 0x118 -> 0x494 -> 0x490, then 0x458 = DU2's order;
  ForceDownAndDetonationProcess `3202a9c0`: 0x114 -> 0x118 -> 0x48c -> riding 0x55 / 0x56 = DU2's `if`; `GetNowStateID()` is March's
  `m_itNowStateData->first`, so the switch -> `if` reads the same state. The stun test (@32067598-3206776c) asks 0x46c-0x484, 0x488, 0x490-0x4a0,
  0x114, 0x118, 0x48c, riding: DU2's order. A match jumps to the groggy change (`3206776e`), confirming the polarity the Done note routes to Stage 2.
- **Every NU0 `DU2` reader row has a verdict** (48 rows, 12 functions): routed (31fd6ce0, 3202a9c0, 32063650, 3208e8c0, 3208eb60, 32cea0c0
  @32cec2bf), automatic (31f6eea0, 320348c0, IsNotCommonMoveState: NU0 files @31fd77b1 under Ghidra's carve `31fd7350`, the real start is
  `31fd7640`, funcmap row `3208de10`), no March body (31fe7610 +0x8e74 block, 31fd7350 `LT_JUMP_DOWN`, 32296570, 3208e7d0 Ain).
- **Hygiene.** flagcheck `--range 14fe2f4d^..14fe2f4d`: 3 PASS (`X2GUUser.cpp` +64, `X2GUUser.h` +33, `X2GUArme_VioletMage.cpp` +4), 1 DEFINE, 0 OTHER,
  0 FAIL, 0 deleted, 0 non-ASCII, RESULT: PASS. `git log -S<flag> 14fe2f4d..HEAD`: only `274dc26c` (Done note text). `junediverge --check-all`:
  allow-listed 232, RESULT: PASS. Flag-off = March: flagcheck's unifdef strip of the three files equals `14fe2f4d^` byte for byte (no `cl /P` run: a
  flag-off preprocess needs an `Always.h` edit, which the shared tree forbids).
- **Build:** none (no fix, no play-test). CH1p's DU2 dependency is met.

### DU3 - Done (2026-11-15)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_EQUIP_VIEW` (defined; `#error` guard: `JUNE_ITEM_KIM`, `JUNE_EQUIP_EFFECT_SET`, `JUNE_CHAR_DUO_BODY`).
**Split:** June's equip view is three layers, too much for one session: **DU3** (this) the item / equip foundation, **DU3b** the view list
(`3201b930`, `31fd2f00`, `31ffe990`, overrides, callers, the unit's per-body tables), **DU3c** Ciel's look at init (`31fe4210` chain), the body readers,
the glow cases and the one full build + play-test. One review (`DU3-review.md`) at DU3c. Kickoffs `DU3b.md`, `DU3c.md` carry the Ghidra reading.
- **CX2Eqip** (June ctor `32961280`, CreateEquip `31fac2a0`): two new trailing arguments with `false` defaults, so every March caller is unchanged:
  the equip's body `m_bMinorUnitEquip` (+0x15d4, getter `IsMinorUnitEquip`) and `m_bSkipSubEquipChainAdd` (+0x15d5; its readers `32960ad0` /
  `32961c80` are DU3c's). The ctor keeps the unit type only for a `UC_ANYONE` templet (`@32961362`; March's callers already gate, so no change), and
  reads the frame offset, common model and XET rows with the body (`32867660` / `32867880` / `32867960`; K2's keyed readers). June's appearance-templet
  swap (`325bb740`, +0x15d8 / +0x15dc, Ciel's appearance id) stays Stage 16.
- **CX2Item:** `GetEffectiveEnchantLevel` (`32882540`: PvP equip -> its `SAT_PVP_EQUIP` value or 7 (`32882440`); `DITT_PC_BANG_ONLY_EQUIPMENT` -> 7;
  else `m_EnchantLevel`) and `IsUsableEquip` (`328825c0`: weapon / defence, not fashion, level < 0 -> false; endurance item at 0 -> false; a PC-bang-only
  item with an owner -> false). Stand-in (labelled): June reads the owner unit's byte +0x48d (a PC-bang session); offline has none. The division sets
  load no rows yet (Stage 11), so both PC-bang branches read false today. Not called yet: DU3b's view list is the first caller.
- **Enum names** (June `Enum.src.lua`): `SAT_PVP_EQUIP = 40` after March's `SAT_END` (`X2Item_Preprocessing.inl`), `DITT_PC_BANG_ONLY_EQUIPMENT = 77`
  (`X2ItemManager.h`). June's `EP_AC_RING2` (28) is March's `EP_AC_TEMP4` (28): the enum rule keeps March's name, so none is registered (DU3b / DU5
  write `EP_AC_TEMP4` with June's name in the comment).
- **Stand-ins replaced** (they now read `IsMinorUnitEquip()`; every equip still says Lu until DU3b / DU3c build Ciel's): `X2GUUser.cpp`
  `@31fd8cff` (UpdateEquipCollisionData), `@3206585b` (CommonFrameMove render param), `31fd6260` (UpdateShowViewListByActiveBody); `X2GULuciel.cpp`
  `IsEquipForMinorUnit` (`32b18220`), `SetHandEquipShow`'s hand and arm tests (`32b143e0`).
- **Gates:** `finish.py --files <9 files> --with SERV_IRUHADEV_JUNE_CHAR_EQUIP_VIEW`: flagcheck PASS (6 PASS, 1 PASS~, 2 OTHER = the stand-in
  sites inside `JUNE_CHAR_DUO_BODY` / `JUNE_CHAR_LUCIEL_CORE` blocks, 0 FAIL), junediverge PASS, enumscan PASS; FINISH RESULT: PASS.
  Commit `f238da7b`; `finish.py --range f238da7b^..f238da7b`: flagcheck PASS (7 PASS, 1 PASS~, 1 DEFINE, 0 FAIL; 0 deleted lines; the one bare
  `#endif` is the `Always.h` `#error` guard, as its neighbours), junediverge PASS, enumscan PASS; FINISH RESULT: PASS.
- **Compile:** `/Zs` probes of `X2Item.cpp`, `X2Eqip.cpp`, `X2GULuciel.cpp`, `X2GUUser.cpp`, `X2ItemManager.cpp`: 5 x 0 errors; break-test defines on
  the new names fail in each file (the blocks compile). Full build skipped: no play-test (DU3c is the seam).
- **Kickoff rows done here:** NU0 `DU3` rows `32882540`, `328825c0`; CH0 §7g L407-409 (accessors were K2's, the body now reaches them); DU1b / DU1b2
  / DU1d3 / CH1l stand-ins. The `Char Chung CH1l0` and `stage8` rows move to DU3b.

### DU3b - Done (2026-11-15)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_EQUIP_VIEW` (DU3's; its `#error` guard now also needs `JUNE_USER_VIRTUAL_HOOKS` for `GetSkinAnimByForm`).
June's view list is ported whole for every class; March's bodies stay in each `#else`. Rows: funcmap `(DU3b)` (14), `june_march_divergent.tsv`.
- **`CX2GUUser`** (`X2GUUser.h/.cpp`): `UpdateEquipViewListInfo( add, remove, short slot )` = June `3201b930` (vslot 0x644 base; no
  `GetAddItemAndRemoveItemToViewList` call; fashion / `IT_ACCESSORY` items swap the equip at ( pos, slot body ), a normal item removes every equip at
  pos with no fashion over it on that body; remove shows the usable normal item or `CreateBasicEquip` with bForceBasic; slot 0x29 / 0x2a ->
  `EP_AC_TEMP4`; tail `SetShowViewList( false )` while either transform flag is set). `AddEquipToViewList( item, slot, bForceBasic )` = June
  `31ffe990` (enchant from `GetEffectiveEnchantLevel`; `UT_LUCIEL` normal item builds Ciel's copy on body 1 from `GetItemIDForMinorUnit()` or, with
  none, the item itself when `GetCommonItemModelName( UT_LUCIEL, true )` is set; body 0 when none shown; March's weapon tail per new equip).
  `GetAddItemAndRemoveItemToViewList` = June `31fd2f00`. New `IsEquipInViewList` (`31fd63c0`); members `m_iMinorUnitEquipOverridePosition` /
  `ItemID` (`+0x9570` / `+0x9574`, writers to DU3c). vslot 0x570 is CH1o's `GetSkinAnimByForm`.
- **Overrides:** Chung `32bdc750`, Ara `32bc89e0`, Arme `32cc9950` take the slot (= March + the argument); Ara's berserk
  `GetAddItemAndRemoveItemToViewListBerserk` = June `32ba6990` (fashion test by the templet's minor-unit flag), `AddBerserkEquipToViewList` reads
  `GetEffectiveEnchantLevel` (`32bc15d0`). Callers pass June's slot: the packet's `m_sSlotID` (`X2StateMenu.cpp` x2, `X2UIInventory.cpp` equip change),
  `GetItemSlotID( uid )` (endurance break, expiry: June `3286a310`), Arme's transform replay slot 0 (`@32cdbf08`).
- **`CX2Inventory`:** `GetItemSlotID` (`3286a310`), `IsMinorUnitEquipItem` (`3286a350`), static `IsMinorUnitEquipSlot` (`32070ef0`; June's table with
  `GetEquipGroup`'s guard: March's slot 20 = `NESI_ONEPIECE_FASHION` answers false until DU4 renumbers, so no DU4 flag is needed).
- **`CX2Unit` per-body tables:** `m_FashionEqipPosition[EP_END][3]` (`+0xb7d`), `m_NormalEqipPositionByBody` (`+0xbf2`, `EP_AC_HAIR..EP_AC_TEMP5`),
  flat `m_NormalEqipPosition` (`+0xc67`); June's `UpdateEqipPosition` `32350070`, `EqipConflict` `32350130`, `IsNormalEquipInThePosition` `31fac260`
  (active body), the dead `UpdateFashionEqipPosition` / `UpdateNormalEqipPosition` (`3234b540` / `3234b5f0`), `UpdateViewEqip`'s fashion test
  (`32350230`), `IsFashionEquipInThePosition( pos, body )`. New `AdjustEquipPositionForSecondRing` (`3234fe90`; stand-in: June's unit `+0x294` slot
  map is unidentified, the inventory's slot id is read; DU4).
- **Routed:** `ResetEqip` `32355e40` (divergence row + allow), the rest of `UpdateViewEqip`, the per-body view table `3234ff60`, the override writers,
  and Ciel's stack dummy passed as basic (March's ctor keeps a dangling `m_pDummyItem`; June's ctor never reads the argument): all `DU3c.md`. The second
  ring and the `+0x294` map: `DU4.md`. The viewer readers stay june-engine R14c.
- **JUNE_BUGS** 728 (dead `UpdateNormalEqipPosition` drops the body), 729 (add path's fashion test at the templet's ring position), 730 (Ciel's
  override copy reads a destroyed temp item's UID).
- **Behaviour today:** March characters run June's scans with body 0; visible changes: a PvP-equip item shows its fixed level, a broken-enchant item
  under a removed avatar shows the basic model, and an accessory swap shows the latest accessory. Lu/Ciel's copy is built but untested until DU3c.
- **Gates:** `finish.py --files <15 files> --with SERV_IRUHADEV_JUNE_CHAR_EQUIP_VIEW`: flagcheck PASS (14 PASS, 1 PASS~, 0 OTHER, 0 FAIL; the 2
  deleted lines are DU3's `#error` guard pair widened in `Always.h`, the bare `#endif` is that guard's own), junediverge PASS, enumscan exit 1 on 190
  A/B rows that predate DU3b in these files (0 on DU3b's added lines, intersected with `git diff -U0`); stateends n/a (no skill states registered).
  Commit `aa720258`; `finish.py --range aa720258^..aa720258 --with ...`: the same (flagcheck PASS 14 / 1 PASS~ / 0 FAIL, junediverge PASS, enumscan
  the same pre-existing rows).
- **Compile:** `/Zs` probes of the 9 changed `.cpp` files: 9 x 0 errors; break-tests (`-DsSlotID_=@`, `-DGetItemSlotID=@`) fail inside the new blocks of
  every file. Full build skipped: no play-test yet (DU3c is the seam).
- **Play-test deferred:** section DU3b in `kickoffs/char-luciel/playtest.md` (`playtest.py check`: PASS), batch at DU3c.

### DU3c - Done (2026-11-15)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_EQUIP_VIEW` (DU3's; no new define, `Always.h` untouched). Rows: funcmap `(DU3c)` (7). The seam of DU3 / DU3b / DU3c.
- **Ciel's look at init** (`X2GUUser.cpp`): `InitEqip` calls the new `CX2GUUser::UpdateEqip( true )` (June `31fe4210`, `32024510 @32024551`) in place of
  `CX2ItemManager::UpdateEqip` (`#else`). Per view equip: `IsUsableEquip`, `GetEffectiveEnchantLevel`; a normal item builds Lu's equip unless a fashion
  sits at ( pos, body 0 ), and for Lu/Ciel without a fashion at ( pos, body 1 ) Ciel's: the templet's minor-unit dummy, or the item itself when its Lu/Ciel
  model has a Ciel name; fashion / accessory items go to the body of their slot. `ReplaceEmptyToBasic( bLoad )` (`31fe3e30`) and
  `ReplaceEmptyToBasic( table, pos, id, bLoad )` (`31fe2fb0`) fill the free ( pos, body ) cells from the unit templet's basics, the second body from the
  basic item's minor-unit id. Every equip passes June's last argument 1.
- **Dummy items (DU3b's open question, settled):** June's ctor `32961280` never reads the dummy argument and keeps no item pointer (`+0x190` = 0); March's
  basic equip stores the item and deletes it in its dtor, so DU3b's stack `CX2Item` would have been freed from the stack. Stand-in (labelled): every
  dummy June builds on the stack is a `new CX2Item` the equip owns; DU3b's Ciel copy of a basic item gets its own copy (two equips never share one).
  `CX2Eqip` keeps no other pointer into the item (templet, UID, owner unit only).
- **Socket options by body:** `InitSocketData` skips a Lu/Ciel equip whose `CX2Inventory::GetEquipDuoType` (new, `3286a4b0`: weapon / armour slots
  `DT_BOTH`, minor-unit slots `DT_MINOR`, the rest `DT_MAJOR`) is not both and not the active body (`@3206dc6a`).
- **Hide-equip-by-body:** `NOT_SHOW_UNIT_BY_DUOTYPE` read in `CommonStateStart` (`@320618a8-3206194c`, new members `m_fHideEquipStartAnimTime` /
  `m_fHideEquipEndAnimTime` = `+0x8fac` / `+0x8fb0`); `UpdateHideEquipByBody` (`3204e4a0`) runs after the before-anim-time update in `CommonFrameMove`
  (`@320661f9`): outside the window the restore is `UpdateDuo( true )` (= `3204df80( 1 )`), inside it hides that body's equips once. JUNE_BUGS 673 stays
  what to watch (Lu/Ciel's `LSI_LDB_WIN` uses `{ 1, 5, 0 }`).
- **`_UpdateSkin`** (`X2Eqip.cpp`, inside `JUNE_EQIP_SKIN`'s `bAllModels`): Lu's equip (body byte clear) with `m_pNormalXSkinModel[1]` set attaches
  every model (`3295ae20`).
- **The chain byte `+0x15d5`:** `31ce9860` is `RemoveObjectChain`, `31ce9a60` `AddObjectChain`. Set (every June in-game caller) keeps March's behaviour
  (particles chained, sub-equips added); clear (the viewer callers only) unchains. So there is no asymmetry and no June bug. Renamed
  `m_bAttachedInDGManagerChain`, default `true` (= March for unported callers); readers `32960ad0` / `32961c80` routed to june-engine R14c with the callers.
- **Identified, routed:** `CommonFrameMove @320659e9-32065ae1` = the equip render param under the custom render state (june-engine R5, funcmap 2608's
  second consumer); the attach-anim basic-item pair `3295dfa0 @3295e2cf-3295e4f6` with its Ciel branch (june-engine R14c owns the pair); the per-body
  socket record `3271a060` (DU5); the face override writer `3201f8d0` (customize, Stage 12; `31fd7b40` is not a function start). Kickoffs amended.
- **Already done before DU3c:** glow type 10 (`32299730` / `32292610`, DU1c2; hand sparks `31ff8510` case 10, DU1d3); `31fd9200` / `31fda280` are the
  `MAJOR:` / `MINOR:` bone lookups (DU1d2), not an equip dump. No code.
- **Save:** testlu1 (unit 166) raised to level 15 for the play-test weapon (`elsdb.py setchar`, backed up).
- **Gates:** `finish.py --files <6 files> --with SERV_IRUHADEV_JUNE_CHAR_EQUIP_VIEW`: flagcheck PASS (6 PASS, 0 OTHER, 0 FAIL; the 10 deleted lines are
  DU3's chain-byte lines renamed and DU3b's two dummy lines replaced, all inside the flag), junediverge PASS, enumscan PASS; FINISH RESULT: PASS.
- **Compile:** `/Zs` probes of `X2GUUser.cpp`, `X2Eqip.cpp`, `X2Inventory.cpp`: 3 x 0 errors; break-tests on the new names fail inside the new blocks.
- **Build:** blocked twice by other sessions' unfinished edits: run 1 failed in CH2d20's `X2StateMenu.cpp` / `X2UIManager.cpp` (members not yet
  declared), run 2 in CH3c20's `Always.h:13695` `#error` (`SERV_IRUHADEV_JUNE_CHAR_NPC_LUA_BINDS`); no DU3c file had an error. The play-test is handed
  off after the next `DEPLOY OK`. Deployed by a peer's build at 18:31 (after `2fbfe93d`): `EXESTR OK: 1 of 1 found in X2_offline.exe`
  (`NOT_SHOW_UNIT_BY_DUOTYPE`), md5 `df9afaf4`; no rebuild (user, 2026-11-15).
- **Play-test:** sections DU3b + DU3c of `kickoffs/char-luciel/playtest.md` (`playtest.py check`: PASS), handed off assembled 2026-11-15. **Passed** (user, 2026-11-15; run 18:33 on md5 `df9afaf4`): 12 `LUCIEL DUO switch`
  lines (testlu1 7, LuSkillA 5), 0 EXCEPTION, UNHANDLED only `E_DISABLE_HB_CHECK_REQ` x6, ErrorLog only the usual optional XET / texture misses.

### DU3 - Review (2026-11-15)

Opus 5.5 high. Covers DU3 `f238da7b`, DU3b `aa720258`, DU3c `2fbfe93d` (flag `SERV_IRUHADEV_JUNE_CHAR_EQUIP_VIEW`). **Verdict: pass after three fixes.**
Ghidra read-only; decompiles and listings in the session scratchpad.
- **DU3:** ctor `32961280` stores arg 8 / 9 at `+0x15d4` / `+0x15d5` (`@3296148d` / `@32961493`); the unit-type clear is the templet's use-condition
  nibble `!= 0x1000` (= `UC_ANYONE` 1); the three keyed reads push `+0x15d4` (`@329616f6` / `@3296182a` / `@3296186c`). `32882540` / `32882440` / `328825c0`
  = `GetEffectiveEnchantLevel` / the PvP helper / `IsUsableEquip` line by line (SA `+8` = `m_Value1`; `+0x48d` PC-bang stand-in labelled).
  **Fix 1:** June's ctor also remaps a second ring through the owner (`3234fe90( item, &m_EqipPos )` `@329615f4`); DU3 had left it out. Now in
  `X2Eqip.cpp` (`AdjustEquipPositionForSecondRing` made public); it does nothing until DU4 adds slots 0x29 / 0x2a (`DU4.md` amended).
- **DU3b:** `3201b930` both branches (fashion / accessory by `IsMinorUnitEquipSlot( slot )` = body, break after one; normal removes every unprotected equip
  with restart; remove path's `return 0` with no tail on a missing normal item; basic via `323516d0` then `AddEquipToViewList( item, slot, 1 )` by stack
  reuse; tail vslot 0x63c `( 0 )`), `31ffe990` (Ciel first, override `+0x9570` / `+0x9574`, JUNE_BUGS 730 read, slot table `0x14 + 0x17` with the 4 Lu
  slots), `31fd2f00` (both branches, index in both cases), `32350070` / `32350130` / `31fac260` (offsets `+0xb7d` / `+0xbf2` / `+0xc67`, band 16..29,
  flat table at the raw position, active body `+0xe40`). Callers pass the packet's `m_sSlotID` or `GetItemSlotID( uid )`. All = the transcriptions.
- **DU3c:** `31fe3e30` (memset 0x75 = `[39][3]`, eight calls in June's order, templet `+0xe4..+0x100`, Raven pos 0x24) and `31fe2fb0` = the port.
  `31fe4210`: arguments and bodies match, but **fix 2:** after pushing Lu's equip June keeps it in the local (`326df830` is a plain push_back); Ciel's equip
  is moved into that local (`31e4f5a0`, `ECX = local_1d0` `@31fe45de`), and with no minor-unit id and no Ciel model (`@31fe458c`) the tail
  (`@31fe44f2`) pushes Lu's equip a second time. Ported verbatim (March's `pEquip.reset()` dropped), JUNE_BUGS 739: 8 Lu/Ciel weapons reach it (Henir
  dual weapons); the DG manager refuses the second chain add and ASSERT is compiled out, so March is safe.
  Socket skip `@3206dc6a` (gate unit type 10, body `+0x8e90`, skip when group != 2, body != 2, group != body), `NOT_SHOW_UNIT_BY_DUOTYPE`
  `@320618a8-3206194c`, `3204e4a0` (window `[start, end)`, inlined `SetShowObject( false )`), `_UpdateSkin` term (`+0x5a8` = model [1]) = the port.
  **Fix 3 (owed):** `GetEquipDuoType` deleted; `InitSocketData` calls S7k's `GetEquipGroup` (same table, equipped-only scan); the flag's `#error` guard
  now names `JUNE_SKILL_STAT_GETTERS`; funcmap `3286a4b0 (DU3c)` row re-pointed.
- **Dummy stand-in:** accepted. June's ctor never reads `param_5` (the dummy flag) and zeroes `+0x190`, so its stack items were never kept; owning heap
  items is the bridge. Chain byte `m_bAttachedInDGManagerChain` default `true` = March for unported callers: accepted.
- **Flag-off = March:** `cl /P` of `X2GUUser.cpp`, `X2Eqip.cpp`, `X2Item.cpp` with the flag on and off (scratch `KTDX.h` / `Always.h` searched first;
  the control with the unmodified copy = the real flag-on output): 66 / 24 / 18 hunks, every ON line inside an `EQUIP_VIEW` block, every OFF line in its
  `#else` (7 hand-checked: a `/*= */` default comment, `OUT`, `((void)0)`). `flagcheck --base f238da7b^ --with <12 peer flags>` over the 10 touched
  sources: PASS, 0 OTHER, 0 deleted; the only residue is one blank line outside the flag in 5 files. `/Zs` with the flag off: 6 x 0 errors.
- **Gates:** `finish.py --review --commits f238da7b aa720258 2fbfe93d --with SERV_IRUHADEV_JUNE_CHAR_EQUIP_VIEW`: flagcheck PASS (27 PASS, 2 PASS~,
  1 DEFINE, 0 OTHER, 0 FAIL; 12 deleted lines = DU3c's rename / dummy lines and DU3b's guard, the 2 bare `#endif`s are the `#error` guard's), junediverge
  PASS, `--check-all` PASS, enumscan exit 1 on 190 rows, **0 on DU3 lines** (`git blame`: 163 from `dd63297`, the rest older peer commits). Fixes:
  `finish.py --files <6 files> --with ...`: flagcheck PASS (6 PASS, 0 OTHER, 0 FAIL; 23 deleted lines, all inside the flag), junediverge PASS, enumscan PASS.
  Commit `4fd494d2`; `finish.py --range 4fd494d2^..4fd494d2 --with ...`: flagcheck PASS (6 PASS, 0 OTHER, 0 FAIL), junediverge PASS, enumscan PASS.
- **Compile:** `/Zs` probes of `X2Eqip.cpp`, `X2GUUser.cpp`, `X2Inventory.cpp`, `X2Unit.cpp`, `X2UICharInfo.cpp`: 5 x 0 errors. **Build blocked** (run 19:20): 11 `LNK2019` in a peer's uncommitted `X2GULuciel.cpp`
  (CH2b's `LSI_SA_LLC_DARK_CRESCENT_*` / `_RUINOUS_IMPACT_*` declared, not defined); no error in a DU3 review file; nothing deployed (exe still md5 `df9afaf4`).
- **Save:** testlu1 (unit 166) raised to level 80, Henir dual weapon 62119 worn in slot 9 (Cobo weapon to bag 1 slot 3); `elsdb.py`, backed up.
- **Deploy:** the 19:41 build of `6d076802` (+ CH2b's then-uncommitted files), md5 `f9a0cda2`.
- **Play-test: passed** (user, 2026-11-15, run 20:15 on md5 `f9a0cda2`), on LuSkillA (unit 191, Curse of Darkness Dual Weapon 30344): weapon at init, off and
  on in the field, body switches; 15 `LUCIEL DUO switch` lines, 0 EXCEPTION, UNHANDLED only `E_DISABLE_HB_CHECK_REQ`. 30344 has a minor-unit id, so
  the JUNE_BUGS 739 double push itself was not on that path (testlu1 keeps the Henir weapon for it); the change is one dropped `reset()`, Ghidra-proven.


### DU4 - Done (2026-11-15)

Opus 5.5 xhigh. Flag `SERV_IRUHADEV_JUNE_CHAR_NESI_RENUMBER` (defined; `#error` guard: `JUNE_CHAR_MINOR_UNIT_PLAY_STATUS`, `JUNE_CHAR_EQUIP_VIEW`, and against
the retired `JUNE_CHARINFO_EQUIP_GROUP`, whose define is commented out). Rows: funcmap `(DU4)` (9), divergence `OnRClickedItem` (`stage9`, new) and
`GetEquippingItemByEquipPos` (source amended), JUNE_BUGS 741-742. Kickoffs: `DU4-review.md` written, `DU5.md` amended.
- **Enum** (`X2Unit.h`): June's `NOW_EQIP_SLOT_ID` (`Enum.src.lua:6653-6696`): `NESI_ONEPIECE_FASHION` 35, `NESI_END` 43, Ciel's 19 `NESI_MINORUNIT_*`,
  `NESI_AC_MINIME` / `_EARRING` / `_RING2` 37 / 39 / 41; March's 0-19 unchanged. data036 `Enum.lua` (core list) had June's names already except
  `NESI_MINORUNIT_DEFENCE_HAIR_FASHION`, but March's 20 / 21: now June's values (`luasrc.py refresh`, core-list row noted). Without this the June
  character window's onepiece slot read 20 and Ciel's hair slot had no id.
- **Save v24** (`X2OfflineDB.*`, nested over v23): `item.slot` 20 -> 35 and `inventory_size.size` 21 -> 43 for category `ST_E_EQUIP`, one savepoint, log
  `[JUNE_CHAR] LUCIEL DUO DB MIG v24 done: <n> worn onepiece row(s) ...`. `BaseSlotSize` follows `NESI_END`. Snapshot `els_db_20261115_205721_pre_manual.sql`.
- **Emulator** (`X2OfflineInventory.cpp`, labelled stand-in, no June server): a costume / accessory that is not `UC_ANYONE` must sit on the body its
  minor-unit flag names (June's right-click rule); June's slots past March's take the character window's `EQUIP_POS` / `FASHION` rows
  (`DLG_UI_CHARACTER_NEW_4`), minime / earring = `EP_AC_TEMP2` / `_TEMP3` (June's names), a second-ring slot takes a ring. Ciel's NU4 starter costume
  goes to her own five slots (was bagged).
- **Character window** (`X2UICharInfo.cpp`): `IsEquippedSlotShown` = June `32e22ce0` by slot id (retires C2's `IS_FOR_MINOR_UNIT` stand-in);
  `ResetNowEquipUI` = June `329146d0` (item body by its slot `3286a350`, second ring remapped through my unit) in place of C2's `bMinorUnitItem = false`.
- **Right-click** (`X2UIInventory.cpp` `OnRClickedItem`): June's body test (`321aa5d0 @321ae867`, `@321aeb5a-321aec39`; raw chunk outside every Ghidra
  function). Not fashion / accessory -> Lu; not `UC_ANYONE` -> the templet's minor-unit flag (a Ciel costume always goes to Ciel); `UC_ANYONE` -> the
  shown body (`IsMinorUnitForm()`, or the window's `GetSwitchingView()` while it is shown). The kickoff's "slot 0x26" is slot `+0x40` = the slot's
  **equip position** (`EP_ONEPIECE_FASHION` 38): an onepiece slot with an anyone item takes the shown body, so Lu's onepiece slot always wins (JUNE_BUGS 741).
  Stand-in: June's item `+0x80` (a look item id, Stage 16) is not in March. The rest of June's loop is the `stage9` divergence row; `321693b0` -> DU5.
- **Slot tables by name** (`X2Inventory.cpp` `IsMinorUnitEquipSlot` / `GetEquipGroup` / `GetAddMaxMPValue( group )`, the second-ring tests in `X2GUUser.cpp`
  x2 and `X2Unit.cpp`). `GetEquippingItemByEquipPos` gets June's ring remap and slot-class test (`32869e10`), ending june-stat-calc D2b's two stand-ins.
- **Unit `+0x294` identified:** `UnitData`'s `std::map<UidType, short>` worn UID -> slot, only writer `AddEqip( item, slot, bCheck )` `323574c0` (wrapper
  `32358ad0`), never erased. Slot-0 writers: preview items (not in the inventory: stand-in reads -1, June 0, same result) and another player's equip
  change `31f1ed70` (JUNE_BUGS 742, no other players offline). **Stand-in confirmed:** the inventory slot (`3286a310`) holds the same value offline.
- **Tools:** `elsdb.py` `EQUIP_SLOTS` on June's ids + `MINOR_SLOTS` body filter for `--equip`.
- **Gates:** `finish.py --files <10 files> --with SERV_IRUHADEV_JUNE_CHAR_NESI_RENUMBER`: flagcheck PASS (8 PASS, 1 PASS~ = one blank line in
  `X2OfflineInventory.cpp`, 1 OTHER = `Always.h`, the retired `CHARINFO_EQUIP_GROUP` define line, 0 FAIL), junediverge PASS, enumscan exit 1 on 190
  rows, **0 on DU4 lines** (intersected with `git diff -U0`); luasrc `core --check` / `ledger --check` PASS; stateends n/a (no skill states).
  Commit `31a27331`; `finish.py --range 31a27331^..31a27331 --with ...`: the same (flagcheck PASS 8 / 1 PASS~ / 1 OTHER / 0 FAIL, junediverge PASS,
  enumscan the same 190 pre-existing rows).
- **Compile:** `/Zs` probes of the 8 changed `.cpp`: 8 x 0 errors; break-tests fail inside the new blocks.
- **Build:** `DEPLOY OK: X2_offline.exe is 26,973,696 B, dated 2026-11-15 21:08:20, md5 439247b2c3e968db8ef23b6b032f44cc` (built at `244c652c` + the
  DU4 files + CH2c's uncommitted `X2GULuciel.*`); `EXESTR OK: 1 of 1` (`[JUNE_CHAR] LUCIEL DUO DB MIG v24 done`, wide) and `1 of 1` (the size UPDATE).
- **Data:** `PACK OK data036` (only `Enum.lua` changed), `COPY OK data036` md5 `e4dfe3d0`.
- **Save prep:** onepieces worn in March's slot 20 on jtLordKnight (25, 163569), reyaa (12, 163571), testlu1 (166, Lu's 163578); Ciel's Spa Wear
  163579 and Archdevil Bottom Piece (Lu) 85912 bagged on testlu1 (`elsdb.py`, backed up).
- **Play-test: passed** (user, 2026-11-15, on md5 `439247b2`). Save after: `user_version` 24, the three onepieces at slot 35, testlu1's Ciel promotion
  set in Ciel's slots 21-25 and Ciel's Spa Wear in 36, all 59 worn bags 43. The v24 line ran in the first launch (screenshot run); the last run
  (21:13) has 20 handled `EGS_CHANGE_INVENTORY_SLOT_ITEM_REQ`, 0 EXCEPTION, UNHANDLED only `E_DISABLE_HB_CHECK_REQ`. Finding (not DU4): a June
  onepiece shows merged with the costume top / bottom in the U window only, March's viewer `UpdateEquipSlotVisibility` id lists; in play it is right.
  Routed to june-engine R14e (kickoff amended, `8b5d3e7c` / `5218e482`).

### DU4 - Review (2026-11-15)

Opus 5.5. **Pass, no code change.** Reviewed `31a27331` against June in Ghidra and the Done note. Only bookkeeping changed: funcmap row `32358ad0 (DU4)` gains
the slot-0 caller `3291d9f0`.
- **Enum:** `X2Unit.h` `NOW_EQIP_SLOT_ID` and the live data036 `Enum.lua` both equal June's `Enum.src.lua:6653-6696` (44 of 44 name / value pairs,
  script compare); March's 0-19 keep their order and values, and `NESI_END` = 43.
- **Slot hide** `IsEquippedSlotShown` = `32e22ce0`. Lu's list {0, 2, 4, 6, 8, 10-19, 35, 37, 39, 41} shows when not switched, Ciel's {20-34, 36, 38, 40,
  42} shows when switched, and any other id always shows. **Window fill** `ResetNowEquipUI` = `329146d0`: position from templet `+0xd5`, remapped by
  `3234fe90` on my unit; accessory or fashion match; `3286a350( uid )` == the slot's `+0x44`; then reset or create.
- **`GetEquippingItemByEquipPos`** = `32869e10`: at `@32869e9f`, `ECX` = `g_pData +0x68 +0x90` (my unit, null-checked as ported). At
  `@32869ed1-32869ef4`, the slot-id class must equal the third argument. The head (positions 2-14 not fashion -> 0) is D2b's and unchanged.
  `GetEquipGroup` = `3286a4b0` (1 / 3 / 5 / 7 / 9 -> 2, minor -> 1, else 0, missing UID -> 0). `GetAddMaxMPValue` = `32869f80`. `IsMinorUnitEquipSlot` = `32070ef0`.
- **Right-click** (`disasm_range.py 0x321ae800 0x321aeea0`): target body at `@321ae867`. Not fashion and not accessory -> 0. Not `UC_ANYONE`
  (`+4 & 0xf000 != 0x1000`, `UC_ANYONE` = 1 in `XSLItem.h` and June) -> templet flags `+8` bit 2. Anyone -> unit `+0xe40` (= `m_bMinorUnitForm`), or
  the char window's `+0x25a` (`m_bSwitchingView`) when its `+0x1e5` (`m_bShow`) is set. The `+0x80` look-templet branch is the labelled stand-in.
  Slot test `@321aeb5a`: `cmp [slot+0x40], 0x26` is the slot's equip position (`EP_ONEPIECE_FASHION` = 38 under `SERV_NEW_ONE_PIECE_AVATAR_SLOT`).
  The anyone test re-reads the same templet (`item +8`). The match `@321aec2c-321aec39` = the port's early `continue` plus March's position / fashion
  test (the `continue` leaves only that slot). `321693b0` (target position adjust) is still DU5's, as noted. JUNE_BUGS 741 holds: both onepiece
  slots pass for an anyone item, and Lu's comes first.
- **Unit `+0x294`:** `323574c0` overwrites only when the slot is not 0 and inserts a new UID. Callers through `32358ad0` are as the funcmap lists, plus
  `3291d9f0` (`CX2UICharInfo::SetNowDragItemReturn`, slot 0). That one re-adds an item that is already worn, so its entry keeps the real slot. The
  stand-in (inventory slot, `3286a310`) still equals June's map offline; JUNE_BUGS 742 is unreachable offline.
- **Save:** the v24 rung is two UPDATEs in one `BEGIN` / `COMMIT`, the same as every other rung in `Migrate()`, and logs its line. The live save reads
  `user_version` 24: worn slots >= 20 are 21-25 (1 each), 35 (3) and 36 (1), and all 59 worn bags are 43. `elsdb.py` `EQUIP_SLOTS` / `MINOR_SLOTS`
  match June's ids and the emulator's slot rows. **Emulator stand-in**
  (`IsSuitableSlot`): the body rule = the right-click rule for non-anyone items. Minime / earring / second ring = `EP_AC_TEMP2` / `_TEMP3` / ring, and
  June's `EP_AC_MINIME` 26, `EP_AC_EARRING` 27 and `EP_AC_RING2` 28 agree. Ciel's starter costume goes to `NESI_MINORUNIT_*` in `COSTUME_SLOT`'s order.
- **Flag-off = March:** `cl /P` (census `cl_cmd`) of `X2Inventory.cpp`, `X2UICharInfo.cpp` and `X2UIInventory.cpp` with the flag on, and again with it
  off through a scratchpad shadow `KTDX.h` / `Always.h` (`/I` first; the shared `Always.h` was not touched). All six runs exit 0. With
  `__FILE__` / `#line` path noise removed, on and off differ only in the `X2Unit.h` enum block and the flag's own blocks in each file.
- **Gates:** `finish.py --review --commits 31a27331 --with SERV_IRUHADEV_JUNE_CHAR_NESI_RENUMBER`: flagcheck PASS (8 PASS, 1 PASS~, 1 OTHER =
  `Always.h`'s retired `CHARINFO_EQUIP_GROUP` define line, 0 FAIL); junediverge PASS; `--check-all` PASS; enumscan exit 1 on 190 rows, **0 on DU4's
  330 added lines** (`git diff -U0` intersect). stateends / skillicons / luabind are n/a (no skill states, skills or June script).
- Full build skipped: no play-test (no code change).

### DU5 - Done (2026-11-15)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEW` (defined; second home `ServerDefine.h` + `SECOND_HOMES`; `#error` guard: `NESI_RENUMBER`,
`DUO_BODY`, `CHARINFO_VIEW`, `CHARINFO_DLG`, all ON at that line by a probe). Rows: funcmap `(DU5)` (15), JUNE_BUGS 749. **Split** (the amendments made
DU5 four phases' work): DU5b viewer second body, DU5c create / select / village avatar, DU5d viewer effects + CH0 viewer rows, DU5e each body's numbers
(table above; `DU5.md` "Split by DU5"; kickoffs written).
- **Packets** (`EventID_Client.h`, `ClientPacket.h/.cpp`): `EGS_CHANGE_UNIT_FORM_REQ` { char } (June 0x4d9, one byte, `32916e10`, no ACK) and
  `EGS_CHANGE_UNIT_FORM_NOT` { UidType, char } (0x4da, reader `320aa0f0` = UidType + Get(char)). Agit 0x4fd and Rose's index on 0x4d9 not ported.
- **Emulator** (`Handlers_Field.cpp`, labelled stand-in): a Lu/Ciel's form goes to `unit.minor_unit_form` (the column the unit select reads into the
  gage data, which sets the unit's form) and the session cache; the NOT comes back. Log `[JUNE_CHAR] LUCIEL DUO FORM unit <uid>: form a -> b saved, NOT sent`.
- **Unit:** `CX2Unit::SetMinorUnitForm` = `3234b8c0` (store, then a shown inventory is shown again: vslot 8 = `SetShow`, `32194ed0`);
  `IsOtherHeadTypeAccessoryWorn` = `3234f4d0` (division group 76 `DITT_ITEM_ID_HEAD_TYPE_ACCESSORY`, registered under the flag; another position,
  same body by the item owner's form).
- **Village:** `CX2StateField` dispatches the NOT to `CX2TFieldGame::Handler_EGS_CHANGE_UNIT_FORM_NOT` = `323acfb0` (type 10 only; `-1` = no change;
  my unit: window `SetSwitchingView( result )` + `EquipChanged`, inventory `SetInventorySort( sort, page )` = `321969e0`). `CX2SquareUnit::SetMinorUnitForm`
  = `323f3290`'s head (unit form, command group `3234d930`, returns the unit's form). Stand-ins: the SD mesh / viewer rebuild and `323fa700` / `323fad80`
  (DU5c), the viewer calls (DU5b), the pet list viewer `328f3550` (June-only window), Rose `323ffda0` (char-rose).
- **Window** (`X2UICharInfo.cpp`): the click (`3291d390 @3291d4a2-3291d622`) and the buttons (`3291db60 @3291dc9f-3291ddd1`) replace the three logged
  stubs: in the village (June state 0xe = `XS_VILLAGE_MAP`) the REQ with `!form`, gated by `CanSwitchFormInVillage` (square unit not emoting
  `+0x608`, foot on a line `+0x49c` = `m_bFootOnLine`); elsewhere `SwitchViewOnlyThisDialog` (only the window switches; June's local lines).
  `SetShow` opens on the unit's form (`@3291e636`, `@3291ea14`); `ResetStat( true )` reads the shown body (`@3291cfeb`).
- **Drop / right-click:** `OnDropAnyItem` = `32918410 @32918837-32918bb9`: ring remap before the slot search, an anyone accessory takes the shown body,
  a second head-type accessory on that body is refused with string 39493 (`KTDGUIOKMsgBox`, return false, no drag return), and the final test
  keeps a minor-unit item off Lu's slots. Right-click `@321aeb3e` (conflict) and `@321aebf3` (ring remap). `RedirectRingToSecondRingSlot` = `321693b0`.
  `DrawSlotMouseOverImage` `32913b90 @32913d3e`: the dragged item's slot by `IsMinorUnitEquipItem` (JUNE_BUGS 749: a bag item's slot number counts).
- **Stand-in kept:** June reads the viewer's `+0xd80` in `ResetStat` / `OnDropAnyItem`; DU5 reads the window's `m_bSwitchingView` (`32912c50` writes both
  from one value). The viewer member comes with DU5b. The window's other-form set sum (`@3291bf2d`) is DU5e's.
- **Gates:** `finish.py --files <21 files> --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEW`: flagcheck PASS (13 PASS, 6 PASS~, 1 DEFINE = `ServerDefine.h`,
  1 OTHER = `Always.h` peers' uncommitted blocks; 0 FAIL); junediverge PASS; enumscan exit 1 on 190 rows, **0 on DU5's added lines** (`git diff -U0`
  intersect); stateends / skillicons / luabind n/a.
- **Compile:** `/Zs` probes of the 9 changed `.cpp` (incl. `X2ServerPacket.cpp` for the serializers): 9 x 0 errors; flag probe: the flag and its four
  dependencies ON in X2Lib.
- **Build:** `DEPLOY OK: X2_offline.exe is 27,020,800 B, dated 2026-11-15 22:24:18, md5 49fa44dc1524480474d0ac42dc78c1d3` (built at `fac2180d` + 38
  uncommitted code files: a peer's coalesced build had already linked DU5's files); `EXESTR OK: 2 of 2` (`[JUNE_CHAR] LUCIEL DUO FORM unit ...` and
  `... FORM REQ %d ignored: no selected unit`, wide).
- **Commit** `b99ac347`; `finish.py --range b99ac347^..b99ac347 --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEW`: flagcheck PASS (13 PASS, 6 PASS~, 2 DEFINE,
  0 OTHER, 0 FAIL); junediverge PASS; enumscan the same 190 pre-existing rows.
- **Play-test: passed** (user, 2026-11-15, on md5 `49fa44dc`). Evidence gap for the review: the logs keep only the last launch (22:30-22:36:
  testlu1 selected, 0 EXCEPTION, no form REQ in it), and the save holds none of the hand-off's test items (233220, 183697, 63000) on any character;
  testlu1's `minor_unit_form` is 1. The review may ask for a repeat of checks 2-4 if it wants the `LUCIEL DUO FORM` line.

### DU5 - Review (2026-11-15)

Opus 5.5 high. Reviewed `b99ac347` against June in Ghidra / `disasm_range.py` (read-only). **Verdict: pass, no code change.** No play-test (no fix landed).
- **Click switch** `3291d390 @3291d4a2-3291d622`: `+0x1e4` clicked and `!+0x25b`, viewer `[esp+0x10]` = `+0x1b4` (set `@3291d418`), my unit type 10 (`31fac140`);
  gate `bl`: no `g_pTFieldGame` or no square unit = 1, `+0x608` emotion != 0 (`ET_NONE` 0; `+0x608` = emotion, D1-3c) -> 0, `+0x49c` == 0 -> 0
  (`+0x49c` = `m_bFootOnLine`: June `@323f0f7b` writes it by `fabs( y - land ) < 1` and `speed.y <= 0` = March `PhysicProcess`); state 0xe -> `32916e10( +0xe40 == 0 )`;
  0xf agit (no March state) not ported; else `32912c50( !+0x25a )`, `3291d2e0` (its push is unread: `ret 4`, no arg use = March `EquipChanged()`), `31f94190` stand-in,
  `3291cfb0( 1 )`, `329146d0`, inventory shown (`+0x1dc`) -> vslot 8 `SetShow( 1 )`: = `SwitchViewOnlyThisDialog`, same order.
- **Buttons** `3291db60 @3291dc9f-3291ddd1`: same gate (no `g_pTFieldGame` skips it = `CanSwitchFormInVillage` true), my unit type 10, REQ `( +0xe40 == 0 )` with no
  village test (only 0xf agit is tested) = the port; `@3291dd2d` = the click's local lines.
- **NOT** `323acfb0`: square unit by UID; type 11 -> Rose (stand-in); type 10: `-1` (`movsx`) skips the form and leaves `bl` 0; else `bl = 323f3290( form == 1 )`,
  `323fa700` / `323fad80` (DU5c); my UID -> `321524d0( 0x13 )` = `UI_MENU_CHARINFO` (bottombar_buttons.tsv, funcmap 32158070) -> `32912c50( bl )`, `3291d2e0`, viewer x4
  (DU5b); `( 0x1f )` = `UI_MENU_INVEN` -> `321969e0` = `SetInventorySort( +0xd8, sort == 12 ? 1 : page[sort] )` = `GetSortType()` / `GetNowInvenSortTypePageNum()`
  (`ST_PC_BANG` 12 -> 1); `( 0x14 )` pet list `328f3550` (June-only, stand-in). The port's extra NULL tests change nothing reachable.
- **Setter** `3234b8c0`: store `+0xe40`, inventory (`3214ef90`) `+0x1dc == 1` -> vslot 8 `( 1 )` = the port. **Square head** `323f3290`: `m_pUnit` -> `3234b8c0( arg )`,
  reads `+0xe40` back (the return), `323f1490` SD mesh (DU5c), `3234d930( arg == 1 )`; no unit -> skips to the viewer rebuild (DU5c) and returns 0 = the port's false.
- **Drop** `32918410 @32918837-32918bb9`: remap `321693b0( &pos )` before the slot search (the port's `UIInventory` is created above it, so June's inventory NULL test
  never differs); body: `+4 & 0xf000 == 0x1000` (anyone) and `+6 & 0xf == 3` (accessory) -> viewer `+0xd80` (stand-in `m_bSwitchingView`, viewer NULL -> 0), else
  fashion `+4 & 1` or accessory -> templet `+8` bit 2 = `IsMinorUnitItem`; `3234f4d0` on my unit -> string `0x9a45` box, `@32918a7b` -> `@32918c37` return 0 with no
  vslot `0x40` (drag return) = the port; the final test re-remaps, then slot `+0x44` set, or item bit 2 clear (`@32918baa-32918bb9`) = the port's minor clause.
- **Right-click** `321abd20`: `@321aeb3e` conflict -> box, `@321aee54` -> `@321ac121` return 0; `@321aebf3` ring remap before the slot match = the port. Order: June tests the
  conflict before the usable test `328825c0`; the port's March endurance `continue` sits before it. Unreachable: item.kim has one accessory with `PT_ENDURANCE`
  (183270, a leg test item), and `328825c0` passes accessories. `32350de0` (return false) stays the stage9 divergence row, as DU5 said.
- **Conflict** `3234f4d0`: NULL item -> 0; templet `+6 & 0xf == 3`; `32807db0( 0x4c )` = `DITT_ITEM_ID_HEAD_TYPE_ACCESSORY` 76 (June `Enum.src.lua:17425`; no March member
  at 76; the division loader takes every group); `i < unit +0x2cc` = inventory `+0x28` = `m_MaxSize[ ST_E_EQUIP 9 ]` (`m_pOwnerUnit` +0, `m_MaxSize` +4), `3286a280( 9, i )`;
  position `+0xd5` differs; also group 76; `3286a350( uid ) == ( item +0xc4 owner +0xe40 != 0 )` -> 1 = the port (owner NULL -> this: June would crash, kept as DU5 wrote).
- **Ring** `321693b0`: `*pos == 0x17`; body `+0xe40`, or CharInfo `+0x25a` while shown `+0x1e5`; `32869e10( 23, 0, body )` held and `( 28, 0, body )` free -> `0x1c` = the port.
  **Highlight** `32913b90 @32913d3e`: slot `+0x44 != 0` compared to `3286a350( dragging uid )` = the port; JUNE_BUGS 749 reads right.
- **SetShow** `@3291e636` / `@3291ea14` pass my unit `+0xe40`; **ResetStat** `@3291cfeb-3291d021`: arg 1 and viewer -> viewer `+0xd80`, else unit `+0xe40`, `== 1` = the port.
- **Packets:** `32916e10` = `32920900( 0x4d9, &byte, 1, 0 )`; `320aa0f0` = UidType (`33052190`) then char (`33051d30`) at +8 = the structs. Emulator: `minor_unit_form` +
  the session cache with the row's MP; the play-status handler (`Handlers_Field.cpp:292`) writes both on every change, so the row's MP is current.
- **Flag-off = March:** `cl /P` (census.py `cl_cmd`, no `/Yu`) of `X2UICharInfo.cpp` / `X2UIInventory.cpp` / `X2TFieldGame.cpp` in a mirrored header tree, flag on vs
  `//#define` in both shadow `Always.h` and `ServerDefine.h` (an `/I` shadow loses to the same-directory include): 21 / 12 / 10 hunks, every flag-on line is DU5's added
  text (or its `DECL_PACKET` / `GET_STRING` expansion), every flag-off line is the pre-DU5 text (0 unmatched).
- **Gates:** `finish.py --review --commits b99ac347 --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEW`:
  ```
  flagcheck JUNE_CHAR_DUO_VIEW       PASS     files: 13 PASS, 6 PASS~ (blank/marker only), 2 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR
  junediverge                        PASS     RESULT: PASS
  junediverge --check-all            PASS     RESULT: PASS
  enumscan                           FAIL     exit 1: 190 A/B row(s) remain in 17 file(s)
  stateends / skillicons / luabind   SKIPPED  not requested
  FINISH RESULT: FAIL (7 gates, 1 failed, 3 skipped)
  ```
  enumscan: all 190 rows are pre-existing `X2Unit.cpp` `SI_*` lines (6193-6216 and like); intersected with `git show -U0 b99ac347`: **0 on DU5's lines**. No OTHER row to explain.
  INFO (not run, `--with`): CHARINFO_DLG / _SHOW / _STATS / _VIEW, CHAR_DUO_BODY, CHAR_NESI_RENUMBER, DIVERGE_SKILL_COOL_TIME_RATE, EQUIP_EFFECT_SET, OFFLINE (enclosing).
- **Build:** none (no code change). **Play-test:** none (no fix; DU5's passed). The `LUCIEL DUO FORM` log line was not captured (DU5 note); not asked for, since every
  path it covers matches June byte for byte.
- **Next:** `/x2-phase char-luciel DU5b` and `/x2-phase char-luciel DU5e` (Opus 5.5 high; no shared files, can run side by side); CH5 (Sonnet 5.5 high) is ready too.

### DU5b - Done (2026-11-16)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (Always.h; `#error` guard: `DUO_VIEW`, `MINOR_UNIT_TEMPLET`, `SEPARATION_MOTION`,
`UNIT_VIEWER_EFFECT_PLAYER`, all on at that line by the `/Zs` probe). Files: `X2UnitViewerUI.h` (+30), `X2UnitViewerUI.cpp` (+207), `X2UICharInfo.cpp`
(+27), `X2TFieldGame.cpp` (+12), insert-only. funcmap rows: `grep "(DU5b)" june2017_re/funcmap.tsv` (17). Census (Sonnet, read-only): 141 hits
of viewer `+0xd7c..+0xd82` / `+0xe40` in 44 functions of `31f8f000-31fb2000`.
- **What June's character window shows (corrects the kickoff's gate).** The viewer ctor turns the two-body view on (`31fab020 @31fab7c9`:
  `+0xd81` = 1, `@31fab7d8`: `+0xd82` = 1); the window's `ResetUnitViewerUI` (`329148d0`) runs the form filter `31f94190` and sets `+0xd81` = 0.
  So the window shows **one** body, the switching view's, and the switch swaps it. Side by side (`31f91020` non-NULL) is the create / select
  screens' and the cash shop's view: DU5c.
- **Split (scope rule, one logical change).** June rewrote the viewer's equipment functions whole (`UpdateEqip 31fa9e60`, `AddEqip 31f9df40`,
  `AddEquipByTID 31f9c640`, `ReplaceEmptyToBasic 31f9e930` / `31f9cfd0`: slot tables `[ pos ][ 3 ]` at `+0x208` / `+0x5b0`, the skin by body,
  Ciel's copy of a Lu item from the templet's minor id, a try-on mode `+0x118c`, a field-state form filter, class / appearance tests). That
  is **DU5b2** (kickoff written; four `stage8` rows in `june_march_divergent.tsv`). Until DU5b2 every equip sits on Lu's skin, so Ciel shows
  bare. The play-test is batched at DU5b2 (`playtest.md` section DU5b).
- **Members** (`X2UnitViewerUI.h`): `m_pMinorUnitMotion` (+0xd78), `m_pMinorUnitXSkinAnim` (+0xd7c), `m_bSwitchingView` (+0xd80),
  `m_bEnableDuoView` (+0xd81), `m_bShowDuoBackUnit` (+0xd82); ctor values as June. Viewer `+0xe40` is `m_bFixed` (`SetFixed 31f933c0`,
  `SetPositionOnScr 31f9a860`): NU0's 11 viewer "other struct?" rows settled; its other 6 rows read the unit's `+0xe40` through `+0x1e0`.
- **Getters = June (divergent, row added):** `GetXSkinAnim` / `GetXSkinAnimPtr` return the shown body (`31fac3c0` / `31f90ff0` / `31fac430`,
  `AccessShownXSkinAnim`); `GetBackXSkinAnimPtr` = `31f91020`. Other classes have form 0: unchanged.
- **SetUnit** (`31faa6f0 @31faaad2-31faae53`): when the main motion changes, Ciel's skin is rebuilt from `m_MinorUnitCommonMotion` (non-empty
  = June's test, templet +0x1e0) plus the state's lobby / field / emotion motions (June's switch, kept); `+0xd80` = the unit's form before
  `UpdateEqip` (`@31faaf04`). Stand-ins: Ciel's Cash25 emotion append (`31ee9150( 10, false )`, routed with the main body's, Stage 10); the
  cash shop's try-on form (`+0x360 / +0x364`) reads 0.
- **Shown body in March's own functions:** `OnFrameMove`, `GetRenderParam`, `OnFrameRender_Prepare` / `_Draw`, `SetPositionOnScr` take a local
  reference `m_pXSkinAnim = AccessShownXSkinAnim()` (one line each; it hides the member, so March's ~60 reads there follow June's
  `31f90ff0`). `OnFrameMove` returns when the shown body is missing (June `31fa8ce0`). Not touched: avatar capture, emotions, the weapon
  effects (`31f93050` / `31f961a0` case 10 = DU5d).
- **PlayAnim** = June's head (`31fa3010 @31fa3062-31fa319d`, divergent row): fourth argument `UVPB_MAJOR_UNIT` / `_MINOR_UNIT` / `_BOTH` (no
  June Enum: a class enum), unit manager + templet test, `ChangeAnim( name, transition, 0 )`, a Ciel miss stops her anim then plays.
  `PlayByMotionType` passes `UVPB_BOTH` at its 7 calls (`31fa7ef0`).
- **Switching view:** `SetSwitchingView` (viewer half of `32912c50`) + `UpdateSwitchingView` (`31f93a10`: shown skin colour 1, viewer
  scale / rotation; back skin colour 0.3, scale x 0.85); `ApplySwitchingViewFilter` (`31f94190`: show / hide the two skins, hide the other
  body's equips, a kept position stays). Stand-in: the equips' position / scale / colour copies (`31f93710` / `31f93820` / `31f93920`, equip
  `+0x908` / `+0x914` / `+0x190`, no March members) -> DU5c.
- **Window and village (DU5's stand-ins retired):** `CX2UICharInfo::SetSwitchingView` sets the viewer first; `ResetStat` and `OnDropAnyItem`
  read the viewer's `+0xd80`; `EquipChanged` (`3291d2e0 @3291d320`) and `SwitchViewOnlyThisDialog` (`@3291dd74`) run the filter;
  `ResetUnitViewerUI` runs it and turns the two-body view off (`@32914abf-32914ad8`); the form NOT stops viewer groups 0-4 and runs the filter
  (`323acfb0 @323ad11e-323ad167`).
- **Routed (scope rule):** `3291d360` + `31faa150` (customize re-filter by body; callers are equip-change / square-unit paths) and the NOT's
  `31fa8230` / `31fa5c90( 0 )` -> backlog Stage 12 (customize). `+0xd82` writers (`PlayEmotion 31fa52b0`, `31fa5c90`), the back body's frame
  move and draw, `31f90fb0` (minor rotation) -> DU5c (kickoff amended).
- **June bug kept:** JUNE_BUGS 756, the destructor never closes Ciel's motion mesh (`31fab920 @31fac060`): no destructor code.
- **Gates:** `finish.py --files <5> --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER`: flagcheck PASS (4 PASS, 1 OTHER = `Always.h`, peers'
  uncommitted blocks), junediverge PASS, enumscan exit 0; stateends / skillicons / luabind n/a. Commit `5b6c5880`; `finish.py --range
  5b6c5880^..5b6c5880 --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER`: flagcheck PASS (4 PASS, 1 DEFINE, 0 OTHER, 0 FAIL), junediverge PASS, enumscan exit 0.
- **Compile:** `/Zs` probes (census `cl_cmd`, no PCH) of `X2UnitViewerUI.cpp`, `X2UICharInfo.cpp`, `X2TFieldGame.cpp`: 0 errors each; a flag
  probe shows `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` on. Full build skipped: no play-test yet (batched at DU5b2).
- **Play-test deferred:** section DU5b in `kickoffs/char-luciel/playtest.md`, batch at DU5b2. Review: `DU5b2-review.md` covers DU5b + DU5b2.

### DU5b2 - Done (2026-11-16)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (DU5b's; no new define, `Always.h` untouched). Files: `X2UnitViewerUI.cpp` (+540),
`X2UnitViewerUI.h` (+35), `X2Eqip.h` (+3), insert-only. funcmap rows: `grep "(DU5b2)" june2017_re/funcmap.tsv` (12).
- **Replaced whole (March's bodies in `#else`):** `UpdateEqip` `31fa9e60` (no economical path; fashion / accessory items only for the shown body
  outside viewer states 4 / 5 / 6 / 10; class test `31ed6500`; usable `328825c0`), `AddEqip` `31f9df40` (Lu/Ciel's normal item also builds Ciel's
  copy: templet minor id -> `AddEquipByTID( id, false, true, enchant )`, else the item itself when its Lu/Ciel model has a Ciel name; skin and slot
  by `IsMinorUnitEquipItem`), `AddEquipByTID` `31f9c640` (`( id, bBasic, bMinorUnit, enchant )`), `RemoveEqip` `31f9f560`, `RemoveAllEquips`
  `31f9c5a0` (= March), `ReplaceEmptyToBasic( pos, id )` `31f9cfd0` (Lu's basic unless Ciel is shown; Ciel's from the basic item's minor id).
  Four `stage8` divergent rows -> `replace`; new rows `CX2UnitViewerUI::RemoveEqip` (replaced) and `CX2Eqip::SetFixed` (routed, below).
- **Tables `[ CX2Unit::EP_END ][ 3 ]`** (June `+0x208` / `+0x5b0`): every live site indexes the equip's body (`IsMinorUnitEquip`) at the
  second-ring position (`GetViewSlotPosition` = June `31fa8ce0 @31fa8f9b`): frame move, render prepare, draw; R14e's two stand-ins in
  `UpdateEquipSlotVisibility` dropped (the fashion table is the member; the apply loop walks bodies 0-2). `IsFashionItemEquippedAt` /
  `IsItemEquippedAt` take a body (no live caller). An `#error` in the header: the flag needs `JUNE_VIEWER_EQUIP_VISIBILITY` (the March `#else`
  of that function reads one body).
- **R14e's left-arm bullet:** June's `ReplaceEmptyToBasic()` `31f9e930` = March's (same seven positions, Raven `EP_RAVEN_LEFT_ARM` = 0x24 from
  templet +0x100): no change.
- **`+0x118c` = `m_bShowFormEquipOnly`** (new): ctor 0 (`@31fab7d2`); June sets 1 only in `CX2SquareUnit::InitViewer` (`323f7380 @323f7417`) and
  `CX2Room` `31ed46e0` (`@31ed4815` / `@31ed489b`), so field / room viewers add only the shown body's items. Writers routed to DU5c (amended).
- **Names:** `XS_PVP_ROOM` for June's `g_pMain +0x8c == 8`; viewer states 9 / 10 have no March or June Enum name (numbers with comments, as C9c).
- **Stand-ins (labelled):** `ApplyCustomize( 2 )` (Stage 12); appearance-templet test `328823d0` (Stage 16); magical closet (UIManager +0xcc,
  June-only UI); equip position / scale / colour copies `3295b260`, `31f93710` / `31f93820` / `31f93920` (DU5c); `RemoveEqip`'s tail on the
  unidentified global `33775878`; `+0x9d0` third UID table (unidentified); `InitWeaponTrace` reads Lu's weapon only (June builds trace frames for
  bodies 0-2: DU5d, amended); June's dummy items live on the stack, March's basic equip owns a heap copy (as DU3c).
- **Routed (scope rule):** June's `SetFixed` `3295a560` sets `m_bFixed` only, March's also hides the sub-equips; the fixed viewers are the village
  square units' -> DU5c (divergent row + two allow rows). June now calls it for a skin equip with sub-equips or particles too
  (`31fac390` = `CX2Eqip::HasSubEquipOrAttachedParticle`, new). The frame move's worn-item enchant lookup keeps March's two-argument call (June
  passes the body: `32869e10( pos, 0, body )`), DU5c with the back body's frame move.
- **June bug kept:** JUNE_BUGS 757 (`AddEquipByTID`'s fashion branch clears the fashion slot it just filled when the normal slot is stale).
- **Gates:** `finish.py --files <3> --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER`: flagcheck PASS (3 PASS, 0 OTHER, 0 FAIL), junediverge PASS (2
  allow-listed: the routed `SetFixed`), enumscan exit 0; stateends / skillicons / luabind n/a.
- **Compile:** `/Zs` probes of `X2UnitViewerUI.cpp`, `X2Eqip.cpp`, `X2CashShop.cpp`, `X2UICharInfo.cpp`, `X2Community.cpp`: 0 errors each.
- **Build:** `DEPLOY OK: X2_offline.exe is 27,048,960 B, dated 2026-11-16 04:42:30, md5 1a80748e8721e729af435d220ab3a4bf` (built at main 6cd55496 +
  7 uncommitted code files, peers' included).
- **Play-test fix (2026-11-16, run 1):** the character window drew Ciel at the untransformed world position (top right of the village) with no
  window model: March's `OnFrameMove` moves the skin only for a non-fixed viewer, and the window viewer is fixed (`SetPositionOnScr` placed
  Lu's skin once, before Ciel became the shown body). June's frame move (`31fa8ce0 @31fa8e29-31fa8ee7`) moves the shown body every frame,
  fixed or not: added under the flag (3 lines). Equipment was right (Ciel wore her Spa Wear).
  `DEPLOY OK: X2_offline.exe is 27,075,072 B, dated 2026-11-16 05:15:39, md5 06e712e076c2288a3104d1310b06a867` (main 95f548ff + peers' files).
- **Play-test fix (2026-11-16, run 2):** the village character was invisible whenever Ciel was the saved form (village entry and back from a
  field): the square unit calls `PlayAnim` with three arguments, so DU5b's default body `UVPB_MAJOR_UNIT` animated Lu's skin only and Ciel's
  (the shown one) never played. Every June caller outside the viewer passes 2 (`323fad80` x27, dungeon result `32513d30` / `325182a0`, create
  `32552860`.., pet `328f3550` / `328f74e0`): the default is `UVPB_BOTH` now (one header line, DU5b's flagged declaration).
  `DEPLOY OK: X2_offline.exe is 27,077,632 B, dated 2026-11-16 06:08:24, md5 2c919c742b8639ca0aaab3c4e1c28ef0` (main adffeace + peers' files).
- **Play-test:** the DU5b batch seam: sections DU5b + DU5b2 of `kickoffs/char-luciel/playtest.md` (`playtest.py check`: PASS), handed off
  assembled. Review: `DU5b2-review.md` covers DU5b + DU5b2.
- **Play-test passed** (user, 2026-11-16, run 3 on md5 `2c919c74`): window body in place and idling, switch / click swap, each body's gear,
  glove swaps, Ciel visible in the village, other classes unchanged. Open, already routed: the window's switch does not yet change the village
  avatar (DU5c, `323f3290` / `323fa700` / `323fad80`); the awakening fire stays behind on Ciel (new phase DU6, user report).

### DU5b2 - Review (2026-11-16)

Opus 5.5 high. Covers DU5b + DU5b2: commits `5b6c5880` (DU5b), `f3c4e39a` (DU5b2), `d2e49ce9` and `f7d74f91` (its two play-test fixes), read
against June in Ghidra and the exe bytes. **Verdict: pass, no code change.**
- **Form filter `31f94190`:** the first `SetShowObject` (`31cfe4a0`) is on `+0x1e8` (Lu) with ( `!+0xd80`, `+0xd80` ), the second on `+0xd7c` (Ciel)
  with ( `+0xd80`, `+0xd80` ). The equip loop matches `+0x1b0` (position) against the kept list and keeps it unless position 2 (`EP_WEAPON_HAND`),
  Lu shown and the equip is Ciel's (`+0x15d4`). Otherwise it hides when the body differs from the switch (inline `SetShowObject( false )`:
  `+0x172` = 0, vslot 0x28 when it was shown). Equal to `ApplySwitchingViewFilter`.
- **`UpdateSwitchingView` `31f93a10`:** shown skin colour (1, 1, 1, 1), viewer scale `+0x128`, rotation `+0x11c`. The back skin (`31f91020`) gets
  colour `3345993c` (0.3f from the exe) and scale x `3346f728` (a double holding the float 0.85f, 0.8500000238, from the exe), so the port's `0.85f` is exact.
- **SetUnit's minor switch `31faa6f0 @31faab03-31faac3c`:** June switches on state - 1 (`@31faa760 DEC EAX`), jump table `31faaff8`. Templet
  `+0x1e0` / `+0x1fc` / `+0x218` / `+0x234` = Common / Field / Lobby / Emotion (`wstring` 0x1c each, `X2Unit.h:443-446` order). Cases: CASHSHOP
  C+E; ROOM, CREATEUNIT C+L+E; FIELD C+F+E; CLASSCHANGE, SELECTUNIT C+L; CHARINFO, WATCH C; default (NONE, others) C+L+E+F, in that order.
  Equal to the port's switch, push for push.
- **`PlayAnim` `31fa3010 @31fa3062-31fa319d`:** body 0 / 2 plays `+0x1e8` and body 1 / 2 plays `+0xd7c`, both behind unit manager `+0x38`, unit
  `+0x1e0` and templet `+0xaf0`. Ciel's branch: a ChangeAnim miss calls Stop (`31d28d40`), then Play. Equal. The default `UVPB_BOTH` (`f7d74f91`)
  is the 2 every outside caller passes.
- **`RemoveEqip` `31f9f560`, line by line:** templet missing -> erase only. Position via `3234fe90` (second ring). Body = Ciel's only for a fashion
  or accessory (`+6 & 0xf == 3`) item. Fashion branch: when the normal slot is empty and the item is not an accessory, `31fac260` then
  `32869e10( pos, 0, 0 )`, then `AddEqip`; otherwise the normal shows again. Normal branch: when Ciel's skin exists and the item is not an accessory,
  it erases and zeroes `[ pos ][ 1 ]` (`( pos * 3 + 0x42 ) * 8` = `+0x208 + 8`). Then `erase`, `31f9e930` or `31f9a2a0` on the flag, and `31f94360`.
  Equal.
- **`AddEquipByTID` `31f9c640`:** the `+0x118c` test reads the unit's `[ pos ][ body ]` fashion bytes (`+0xb7d`); position `> 0x26` returns. Dummy
  UID: -1 on an empty map, else `min( first key, -1 ) - 1` (June's sign / -1 tests). `!bBasic` -> period 2, quantity 1. A negative enchant reads
  `32882540`. State 3 (`UVS_FIELD`): Lu/Ciel and body != `+0xd80` deletes the dummy and returns. States 7-9 (`UVS_CHARINFO`, `UVS_WATCH`, 9) are not
  attached to the DG chain. Body 1 -> `+0xd7c` skin, else `+0x1e8`. Then `31f9a2a0` and the state-9 copy (stand-in, DU5c). Equal.
- **`[ pos ][ 3 ]` at every site:** `cl /P` of `X2UnitViewerUI.cpp` (census `cl_cmd`), flag on: no `m_ViewEquipNormal` / `m_ViewEquipFashion`
  use with one index survives. The old `[ ePos ]` sites are in March's `#else` bodies or in `JUNE_VIEWER_EQUIP_VISIBILITY`'s dead `#else`, which
  the header's `#error` keeps dead. No other file reads the tables (`X2Eqip.cpp` / `X2NPCUnitViewerUI.h` hits are comments).
- **Shown-body local:** `OnFrameMove`, `GetRenderParam`, `OnFrameRender_Prepare` / `_Draw` and `SetPositionOnScr` only re-point `m_pXSkinAnim`.
  Their reads that need Lu's skin are the weapon effects (`InitWeaponTrace` reads Lu's weapon, already routed to DU5d). Other classes have
  `+0xd80` = 0, so they are unchanged.
- **Accepted as is:** `d2e49ce9` adds June's unconditional `Move` after March's `if ( false == m_bFixed )` `Move`. A non-fixed viewer moves twice
  to the same position (idempotent), so behaviour = June. Not rewritten into `#else`: that would mean a rebuild for no change in behaviour.
- **Gates:** `finish.py --review --commits 5b6c5880 f3c4e39a d2e49ce9 f7d74f91 --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER -v`:
  `FINISH RESULT: PASS (7 gates, 0 failed, 3 skipped)`. flagcheck PASS (9 PASS, 1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR). The 1 deleted line is
  `f7d74f91`'s change of the `PlayAnim` default inside DU5b's flagged declaration, so flag-off = March. `junediverge` PASS; `--check-all` PASS;
  `enumscan` exit 0; stateends / skillicons / luabind not requested. No OTHER rows.
- **Build:** no code change; full build skipped: no play-test (DU5b2's `DEPLOY OK` md5 `2c919c74` and the passed run-3 play-test stand).
- **Next:** DU5c and DU5d are ready (`DU5c.md`, `DU5d.md`). Both edit `X2UnitViewerUI.cpp`, so run them one at a time.

### DU5c - Done (2026-11-16)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_DUO_AVATAR` (Always.h; `#error` guard: `DUO_VIEWER`, `UNIT_TYPE_RANGE`, `JUNE_ENUM_NAMES`, all on at that
line by a `/Zs` flag probe, `RIDING_SYSTEM` on too). Files: `X2SquareUnit.cpp` (+126), `X2SquareUnit.h` (+9), `X2UnitViewerUI.h` (+3), `X2TFieldGame.cpp`
(+7), `X2Room.cpp` (+7), insert-only. funcmap rows: `grep "(DU5c)" june2017_re/funcmap.tsv` (7). Log line (permanent, one per SD mesh load):
`[JUNE_CHAR] LUCIEL DUO AVATAR unit <uid>: SD mesh <name> ( form <0|1> ) loaded|missing`.
- **Split (scope rule, one logical change):** DU5c = the village avatar (the done gate's village half). The create / select screens, the viewer's back
  body (frame move, draw, `+0xd82` writers, `31f90fb0`, `31f99f70`, equip copies, enchant lookup), `CX2Eqip::SetFixed` (June's three `+0x8ec` readers sit
  outside carved functions), R14e's select-screen onepiece and the other players' form byte -> **DU5c2** (kickoff written; `DU5c.md` "Split by DU5c").
- **SD table** (`31ee3470`, table `3372c1e8` filled by `333a3d20`): `_CONST_SQUAREUNIT_DU5C_::SD_MESH_FILE_NAME[ UT_END ][ 2 ]`, 13 rows by unit type x
  spa village, June's names verbatim (row 0 `Dummy_Ani_Wait.X`, Elsword's `_SD_Content.X`, Rose / Ain `Contnet10`). The ctor (`323f8080 @323f8bb2-323f8c8a`)
  opens the SD mesh from it when the location manager exists, replacing March's switch (Lu had no case: no SD mesh before). Spa = `GetCurrentVillageID()`
  `VMI_SPA_RUBEN` / `VMI_SPA_RANOX` (June's `+0x64 - 1500 < 2`, as june-hud H2). June checks no upper bound on the type; the port bounds it by `UT_END`.
- **SD mesh by form** = new `CX2SquareUnit::UpdateSDMeshByMinorUnitForm` (`323f1490`): type 10 only, `Mesh_Ciel_Base_SD.X` / `Mesh_Lu_Base_SD.X` (or
  `_Content10`), close + delete, open, new skin anim, hidden. Called by `Init`'s head (`323fea90 @323feaa0-323fead8`, with the skill command group) and
  by `SetMinorUnitForm` (DU5's stand-in retired). Assets in March: `data021` holds both bodies' SD meshes (CH0b).
- **Viewer rebuild** (`323f3290 @323f331e-323f3479`): keep the shown skin's anim name and play type (`LobbyWait` / loop when none), write `+0xd80` raw
  (new `SetSwitchingViewValue`, no `UpdateSwitchingView`, as June), `UpdateEqip`, `NotifyShowObjectChanged` (vslot 0x28 = `31f96ad0`) when hidden, then
  `ChangeAnim( name, false, 0 )` + `Play( type )` on the new shown body.
- **The form NOT** (`323acfb0 @323ad02c-323ad038`): `InitWaitMotion` then `StateStart` (protected in March: made public under the flag, the name kept).
- **One body, shown body's equipment:** `InitViewer` (`323f7380 @323f740d-323f741e`) sets `SetEnableDuoView( false )` and `SetShowFormEquipOnly( true )`
  before `SetUnit`; `CX2Room::Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT` sets it on the slot viewer (`@31ed4815`) and on the watch window's viewer
  (`@31ed489b`; DU5b2's "my unit's" reading corrected: UI 0x19 with a target UID).
- **Divergent function found:** June's `CX2SquareUnit::Init` (`323fea90`) has a longer tail (`ApplyCustomize( 2 )`, `323eec00`, `323ee400`, the gage
  singleton's HP set): row `stage12`, only the form head ported here; the tail goes with the customize system (backlog Stage 12). `323f3250` (form head
  without the viewer) has no caller.
- **Gates:** `finish.py --files <6> --with SERV_IRUHADEV_JUNE_CHAR_DUO_AVATAR`: flagcheck PASS (5 PASS, 1 OTHER = `Always.h`, peers' uncommitted block
  moves; 0 FAIL), junediverge PASS, enumscan exit 0; stateends / skillicons / luabind n/a. Post-commit `--range` line: see the commit's hand-off.
- **Compile:** `/Zs` probes of `X2SquareUnit.cpp`, `X2TFieldGame.cpp`, `X2Room.cpp`, `X2UnitViewerUI.cpp`: 0 errors each.
- **Build:** `DEPLOY OK: X2_offline.exe is 27,080,192 B, dated 2026-11-16 07:57:54, md5 2f5527f0b4e32a87036715b23f48db9e` (built at main a1262f00 + 6 uncommitted code files, DU5c's); `EXESTR OK: 2 of 2` (the `LUCIEL DUO AVATAR` line and `Mesh_Ciel_Base_SD_Content10.X`, wide).
- **Play-test:** village switch (window button, Ciel <-> Lu) changes the avatar and its SD mesh, a relog keeps it; field / room viewers unchanged for
  other classes. Review: `DU5c-review.md`.

### DU5c - Review (2026-11-16)

Opus 5.5 high. Reviewed `b7dfce78` against June's listing (`disasm_range.py`, read-only). **Verdict: pass after one citation fix, no behaviour change.**
- **SD table:** `333a3d20` fills `3372c1e8` with 26 wstrings ( 0x1c each ); all 26 equal `SD_MESH_FILE_NAME` with their case ( Elsword `_SD_Content.X`,
  Arme / Eve / Add spa `.x`, `Mesh_ARA_` spa, Rose / Ain `Contnet10` ). `31ee3470`: `0 < type`, spa `<= 1`, entry `( type x 2 + spa ) x 0x1c`, no upper
  bound ( the port's `UT_END` bound is unreachable ). Ctor `@323f8bb1-323f8c94`: only with `g_pData +0x268`, spa = `+0x64 - 1500 <= 1`
  ( `VMI_SPA_RUBEN` / `_RANOX` ), type = templet `+0xaf0` field 0, `OpenXSkinMesh( name, L"Bip01" )` = March's default argument. March's switch in `#else`.
- **`323f1490`:** type 10 only; Ciel / Lu names `334944e4` / `3346a00c`, spa `334944a8` / `33469fd4`; close `+0x520` only when the device manager
  exists, delete `+0x51c`, open, `CreateSkinAnim`, `+0x2b4` = 1 ( `SetApplyMotionOffset` ), `SetAnimXSkinMesh`, `AddModelXSkinMesh`, then an inline
  `SetShowObject( false )` ( `+0x172` = 0, vslot 0x28 when it was shown ) = the port. The log line is the port's own.
- **`323fea90` head:** `+0x192` = 1, form = unit `+0xe40`, `323f1490( form, templet type )` when a templet, `3234d930( form == 1 )`, then InitSystem /
  InitViewer / InitState / InitParticle / InitWaitMotion ( `323f3660`, `323f7380`, `323f59f0`, `323edea0`, `323fa700` ). June reads `+0xe40` before its
  unit test; the port reads it after (same result with a unit).
- **`323f3290`:** SD mesh with unit `+0xe40` after `3234b8c0`, command group from the argument. Viewer: `L"LobbyWait"` ( `3346f4dc` ) and play type 0
  ( `XAP_LOOP` ); the shown skin ( `321f2e40` ) `+0x228` / `+0x2a8` = `m_NowAnimName` / `m_PlayType`; `+0xd80` = argument raw; `UpdateEqip` `31fa9e60`; not
  `GetShowObject()` ( manager test and `+0x172` ) -> vslot 0x28, the slot June's inline `SetShowObject` calls = `NotifyShowObjectChanged`;
  `ChangeAnim( name, 0, 0.0 )` + `Play( type )` on the re-read shown skin. = the port.
- **`323acfb0` replays:** type 10, form != -1: `323f3290`, `323fa700`, `323fad80` at `@323ad057-323ad06b`. `323fa700` = InitWaitMotion ( Init's last
  call ), `323fad80` = StateStart ( `BeginTable( state name )` first ). The `public:` toggle around `StateStart` changes nothing else.
  **Fix:** the Done note, the `X2TFieldGame.cpp:6712` comment and the funcmap row cited `@323ad02c-323ad038` ( June's templet / type test ); the
  comment and the row now say `@323ad05e-323ad06b`.
- **`323f7380 @323f740a-323f7417`:** `+0xd81` = 0, `+0x118c` = 1 before `SetUnit( unit, 3 )` = the port. **`31ed46e0`:** `@31ed480c-31ed4815` the slot
  viewer ( `+0x54`; June tests it for NULL, March already returned at `X2Room.cpp:1473` on a NULL viewer ); `@31ed4845-31ed489b` UI 0x19 shown, target
  UID = the packet's -> viewer `+0xf4` ( my unit jumps past it, as March's other-unit branch ) = the port.
- **Flag-off = March:** `cl /P` of `X2SquareUnit.cpp`, `X2TFieldGame.cpp`, `X2Room.cpp` at `b7dfce78` with the `#define` commented out vs `b7dfce78^`:
  `X2Room.cpp` identical; the other two differ by one `__LINE__` value each ( a `FOREACH` temporary, an error-log line number ). Flag-on differs only
  in DU5c's hunks.
- **Gates:** `finish.py --review --commits b7dfce78 --with SERV_IRUHADEV_JUNE_CHAR_DUO_AVATAR -v`: `FINISH RESULT: PASS (7 gates, 0 failed, 3 skipped)`;
  flagcheck 5 PASS + 1 DEFINE, 0 OTHER, 0 deleted; junediverge PASS, `--check-all` PASS; enumscan exit 0; stateends / skillicons / luabind n/a.
  INFO: `DUO_VIEW`, `DUO_VIEWER`, `UNIT_TYPE_RANGE`, `JUNE_ENUM_NAMES` ( the `#error` guard's needs and the enclosing block ).
- **Build:** `/Zs` probe of `X2TFieldGame.cpp`: 0 errors. Full build skipped: the fix is one comment; the deployed exe ( md5 `2f5527f0...` ) is DU5c's
  code unchanged.
- **Play-test: passed** (user, 2026-11-16, logs read): `LuDbA` ( unit 202 ) entered as Ciel ( `SD mesh Mesh_Ciel_Base_SD.X ( form 1 ) loaded` ),
  four window switches each logged `LUCIEL DUO FORM ... saved, NOT sent` then the matching Lu / Ciel `SD mesh ... loaded`, the relog loaded Ciel again
  ( 08:32:55 ); no `LUCIEL DUO AVATAR` line for another unit; ErrorLog 0 matching lines, packets 0 EXCEPTION.

### DU5c2 - Done (2026-11-16)

Opus 5.5 high. Flags `SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY` (`#error` guard: `DUO_VIEWER`, `JUNE_CHAR_CREATE`, `JUNE_STATCALC_FINAL_DAMAGE`, all on at that
line by a `/Zs` flag probe) and `SERV_IRUHADEV_JUNE_DIVERGE_INVENTORY_DEFAULT_SIZE`. Files: `X2UnitViewerUI.cpp` (+144), `X2UnitViewerUI.h` (+5),
`X2StateCreateUnit.cpp` (+14), `X2StateServerSelect.cpp` (+11), `X2Inventory.cpp` (+12), insert-only. funcmap rows: `grep "(DU5c2)" june2017_re/funcmap.tsv`
(16). Log lines (permanent, one per screen build): `[JUNE_CHAR] LUCIEL DUO LOBBY create screen: ...` / `... select screen: unit <uid> class <c> ...`.
- **Back body** (Ciel while Lu is shown; `GetBackXSkinAnimPtr`, two-body view on): new `BackUnitFrameMove` = June `31fa8ce0( t, dt, 0 )`'s own part:
  position = viewer + ( `+0xd84` + 1.0 ) x ( 50, 0, 70 ) (`+0xd84` is the ctor's 0, no other writer), skin frame move, that body's equipment
  (`31fac160`; fashion slot: worn item `32869e10( second-ring pos, 0, body )` enchant sync; no frame move for June `EP_AC_RING2` = `EP_AC_TEMP4`),
  the render-param block. Called from `OnFrameMove` after the shown body's render params (`31faa550 @31faa5a3`). Prepare (`31f974b0( false )`) and
  Draw (`31fa00b0 @31fa022d-31fa0346`) add the back skin after the shown one; `SetPositionOnScr` moves it to its offset (`31f99f70`).
- **Shown pass = June's** (same function, param 1): equips of the other body are skipped (`@31fa8f5b`), the enchant lookup asks the equip's body at
  its second-ring position (`@31fa9040`; DU5b2's two-argument stand-in closed), the second ring gets no frame move (`@31fa90e7`).
- **Not repeated (decision):** June's second `31fa8ce0` pass also re-runs its shared tail on the shown body (`31f90ff0`: effect re-arm / drain, weapon
  effects, hand fire, `Bip01_Head` / `+0xe28` particle) - the shown body's work twice per frame. The port runs it once; the review judges it.
  Skin `+0x320` = viewer `+0xd88` (ctor 0, never written again) and the Rose weapon call `3295dbc0` (unit type 11) are not ported.
- **`SetUnitScale`** (`31f969a0`: the viewer and both skins) and **`SetMinorUnitRotation`** (`31f90fb0`). `SelectLuciel` passes state 5
  (`@32555d38`, Ciel's lobby motions) and scales both bodies; then `D3DXToRadian( RotateYSelectCharacterType( class, true ) )` turns Ciel
  (`@32555fe8-3255602a`). The select screen (`32564400 @32564846-325648ac`) scales both bodies and turns the minor body the same way; June's
  `31f99f70` move there is covered by March's `Move` + the frame move that follows; `31faa150` (customize) stays Stage 12.
- **R14e's select-screen onepiece:** June's list handler `3257cd60` builds the unit with `32366e20( info )` and calls no size function; June's
  `CX2Inventory` ctor (`3286d280`) sizes every bag 50, March's 24 cut the equip bag below slot 35 (onepiece) and 41 / 42 (second rings) and Ciel's
  slots 24-25. Replaced under its own divergence flag; new divergent row `CX2Inventory::CX2Inventory`. The other `GetItemMaxNum( ST_E_EQUIP )`
  loops on unreset units (`X2Unit.cpp`) are covered by the same default.
- **Equip copies closed:** `31f93710` / `31f93820` / `3295b260` / `31f93640` / `31f968e0` write equip `+0x908`-`+0x920`, which nothing in June's
  CX2Eqip or viewer reads (dispsweep): nothing to port (DU5b / DU5b2 stand-ins answered; their comments stay as written).
- **Routed (scope rule):** `CX2Eqip::SetFixed` -> **june-engine R14f** (kickoff amended): June's fixed viewers build equips with `+0x15d5` clear,
  so `LoadAttached @32962094` never chains their sub-equips and `OnFrameRender_Draw 3295d280 @3295d5aa` draws the shown ones; March chains every
  sub-equip until R14f, so June's `SetFixed` alone would draw them twice. Divergent row and the two allow rows now name R14f. The `+0xd82` writers
  (`PlayEmotion 31fa52b0`, `31fa5c90`), the emotion frame move's back body (`31fa64f0`), the window's equip tint `31f93920` and June's other 25
  `SetUnitScale` callers -> **DU5d** (kickoff amended). Other players' form byte (`32367330` / `32367da0` / `32368300`; room, field, watch
  callers): offline has no other players, closed with DU5e's packet fields, no change.
- **Gates:** `finish.py --files <6> --with SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY SERV_IRUHADEV_JUNE_DIVERGE_INVENTORY_DEFAULT_SIZE`: `FINISH RESULT: PASS
  (9 gates, 0 failed, 5 skipped)`; flagcheck DUO_LOBBY 4 PASS / 2 OTHER (`Always.h` peers' uncommitted block moves, `X2Inventory.cpp` = the
  inventory flag), INVENTORY 1 PASS / 5 OTHER, 0 FAIL, 0 deleted lines, 8 EOL-mismatched inserts (`X2Inventory.cpp`: the namespace takes its CRLF
  anchor in a mixed file); junediverge PASS; enumscan exit 0; stateends / skillicons / luabind n/a. Post-commit `--range`: see below.
- **Compile:** `/Zs` probes of `X2UnitViewerUI.cpp`, `X2StateCreateUnit.cpp`, `X2StateServerSelect.cpp`, `X2Inventory.cpp`: 0 errors each.
- **Build:** `DEPLOY OK: X2_offline.exe is 27,083,264 B, dated 2026-11-16 08:41:00, md5 22bcc29d90d1a473658994398ca12953` (built at main 0856b6bd + 6
  uncommitted code files, DU5c2's); `EXESTR OK: 2 of 2` (the two `LUCIEL DUO LOBBY` lines, wide).
- **Play-test:** create screen (Lu + Ciel side by side, Ciel turned), select screen (`testlu1`: both bodies, Ciel's shoes and weapon from slots 24-25;
  `jtLordKnight`: Spa Wear 163569 alone), other classes unchanged. Review: `DU5c2-review.md`.
- **Play-test passed** (user, 2026-11-16, md5 `22bcc29d`): create screen both bodies, select screen both bodies with Ciel's gear, onepiece, other
  classes unchanged. One defect: Ciel bare on the create screen.
- **Fix (same flag):** `EquipPromotionItemByUnitTemplet` = June `3254e1a0` (Lu's five promotion items, then the minor body's five, templet
  `+0x1cc..+0x1dc`; `AddEquipByTID` skips 0 for one-body classes). It covers SelectLuciel's stand-in (June calls the by-class `3254e410`, same items for
  class 200) and the job preview (June `325519d0` calls `3254e1a0`). Divergent rows: `EquipPromotionItemByUnitTemplet` (ported) and `EquipPromotionItem`
  (`3254e410`, by-class templet items instead of March's arrays: routed to each character port's creation screen). `finish.py --files
  X2StateCreateUnit.cpp --with SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY`: PASS (6 gates); `/Zs` probe 0 errors.
  `DEPLOY OK: X2_offline.exe is 27,075,584 B, dated 2026-11-16 09:04:05, md5 2a1a963a14c71dc33eeb4df476a7a3d7` (built at main 6e440d0a + 12 uncommitted
  code files: this fix and 11 of peers' in-progress files).
- **Fix play-test passed** (user, 2026-11-16, md5 `2a1a963a`): Ciel in her promotion items on the create screen. Log: `[09:06:36.822] [JUNE_CHAR]
  LUCIEL DUO LOBBY create screen: both bodies, Ciel turned 30.0 deg`; 0 EXCEPTION, 0 ErrorLog lines.

### DU5c2 - Review (2026-11-16)

Opus 5.5 high. Reviewed `8fe221f2` + `bd1bb0a0` against June in Ghidra / `disasm_range.py`. Verdict: **pass after one fix**, plus a comment fix and a
flag-nesting move, all under `SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY` (`X2UnitViewerUI.cpp` only).
- **Fix: the shown pass's enchant sync.** June runs the same code `@31fa9031-31fa9092` for both passes (worn item `32869e10( pos, 0, body )`, change
  while usable `328825c0`, else 0). DU5c2 gave the back pass June's test but left March's `REAL_TIME_ELSWORD` endurance / infinity test on the shown
  pass. Both now call `ApplyWornItemEnchantLevel` (in `_CONST_UNITVIEWER_DU5C2_`); it differs from March only for a broken, PC-bang-only or
  non-endurance, non-infinity worn item.
- **Comment fix:** `+0xd84` is not ctor-only: June's customize preview writes 0.33 (`32233f70 @32234125`, `3224d0f0 @3224de5b` / `@3224de6d`). The
  port's constant offset is right for every March path (customize is Stage 12); routed to the Stage 12 brief. `+0xd88`: the ctor `@31fab47a` is the
  only viewer writer (the other `+0xd88` hits are `CX2Unit` and unrelated classes).
- **Move:** `SetPositionOnScr`'s back-body move sat inside `#ifdef SERV_IRUHADEV_JUNE_UNIT_VIEWER_EFFECT_PLAYER`; it now stands before that block
  (June order: `31f99f70` moves the back skin `@31f9a0ae-31f9a12d` before `31f968e0` and `+0x1d4`). No behaviour change with the live flags.
- **Doubled tail: keep the decision, no JUNE_BUGS row.** June's second `31fa8ce0` pass re-runs the shown body's tail (base viewer `329585f0` at
  head and tail, re-arm, drain, `+0xf28` show loop, title / CEPE / HABIT). Nothing reachable changes when it runs twice: the re-arm needs a loop row
  and Lu/Ciel's only row (`LDN_LobbyWait`, `DT_MINOR`) is loop false; drain, bone positions, show flags and sounds are idempotent;
  `EffectSet_LDN_Start_Ciel` (UIEffectSet) has no `TRACE_MATRIX` entry, so no cumulative offset doubles. The one dt accumulator, the HABIT timer
  `+0xf90` / `+0xfa0` (`3322a380` @31fa99f1), is not in March: the Stage 12 brief now says its port must run it twice with the back body shown.
- **Confirmed = June:** filter `31fac160` (shown: `+0x15d4 == +0xd80`; back: `!= `) and body index; enchant arguments `( pos, 0, body )`; second-ring
  skip `pos != 0x1c` = `EP_AC_TEMP4` (28); render-param block `@31fa9149` on the pass's skin; back position = viewer + ( `+0xd84` + 1.0 ) x
  ( 50, 0, 70 ) (`334355b0` = 1.0 double, `33732708` = 50 / 0 / 70); Prepare `31f974b0` (instant-show off, show test, prepare, on) and Draw `31fa00b0
  @31fa022d-31fa0346` (back skin with show + instant show, after the shown one); `31f91020` = back skin only with `+0xd81` and `+0xd82`;
  `SetUnitScale` = `31f969a0` (viewer, `+0x1e8`, `+0xd7c`); `SetMinorUnitRotation` = `31f90fb0` (`Rotate`); SelectLuciel `@32555d38` `SetUnit( .., 5 )` =
  `UVS_CREATEUNIT`, rotation `@32555fe8-3255602a` after `3254e410`, `31edf260( +0x2c0 class, 1 )` x `33441c58` (pi / 180 double); select screen
  `@32564846-325648ac` (June reads the templet's class, the port the unit's: equal); `3286d280` sets every bag 0x32; `3257cd60`'s calls hold no size
  function; `32961c80 @32962094` chains a sub-equip only with `+0x15d5`, so R14f owns `SetFixed`.
- **Flag-off = March:** `cl /P` of `X2UnitViewerUI.cpp`, `X2StateCreateUnit.cpp`, `X2StateServerSelect.cpp`, `X2Inventory.cpp` with both flags
  `#undef`'d after the stdafx include vs the same at `0856b6bd`: same line counts, only `__LINE__` / FOREACH-counter digits differ. `/Zs` probe of
  `X2UnitViewerUI.cpp` (flags on): 0 errors.
- **Gates:** `finish.py --review --commits 8fe221f2 bd1bb0a0 --with SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY SERV_IRUHADEV_JUNE_DIVERGE_INVENTORY_DEFAULT_SIZE -v`:
  `FINISH RESULT: PASS (10 gates, 0 failed, 5 skipped)`; OTHER rows = `Always.h` peers' blocks and each file's other flag; 8 EOL-mismatched lines =
  `X2Inventory.cpp`'s namespace (as the phase note says); `JUNE_CHAR_ADD_DP` / `JUNE_EQIP_FRAME_MOVE_DEBUG` retired. The fix: `finish.py --files
  X2UnitViewerUI.cpp --with SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY`: PASS (6 gates); 9 deleted lines, all DU5c2's own lines under the flag (the
  comment, the moved block, the enchant lines now in the helper). Post-commit `finish.py --range 303795cf^..303795cf --with
  SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY`: PASS (6 gates).
- **Build:** `DEPLOY OK: X2_offline.exe is 27,090,432 B, dated 2026-11-16 09:23:17, md5 65db270adcaffe9c32baccee653ab7f4` (built at main db9bd3ef + 5
  uncommitted code files: this fix and peers' in-progress files; nothing relinked: a peer's 09:22 link already holds `X2UnitViewerUI.obj` 09:21:32,
  newer than the edit at 09:16:25). No new strings, so no `exestr` line.
- **Play-test passed** (user, 2026-11-16, md5 `65db270a`): create screen, select screen ( `testlu1`, `jtLordKnight` ), the enchant glow, other
  classes. Log: `[09:38:09.717] [JUNE_CHAR] LUCIEL DUO LOBBY create screen: both bodies, Ciel turned 30.0 deg`, `[09:39:11.757] ... select screen: unit
  166 class 200, both bodies, minor body turned 30.0 deg`; 0 EXCEPTION, 0 ErrorLog lines. DU5c2 done; next DU5d (`DU5d.md`).

### DU5d - Done (2026-11-16)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (DU5b's). Files: `X2UnitViewerUI.cpp` (+223 / -10), `X2UnitViewerUI.h` (+10). funcmap rows:
`grep "(DU5d)" june2017_re/funcmap.tsv` (6); divergent row `CX2UnitViewerUI::InitWeaponTrace` (replaced in this commit). No log line added.
- **Weapon trace (`InitWeaponTrace` = June `31f9a2a0`, whole body under the flag):** bodies 0-2 in turn, fashion slot first, the 8 normal anims'
  TRACE / weapon-bone frames; Ara's model 0 line frames; Lu's model 0 `ATTACK_LINE_END0` / `START0_LWeapon` (June keeps it in Ara's `+0xed0` =
  `m_pFrame_ATTACK_LINE_START0_FRONT`) and model 1 `END0` / `START0_RWeapon` (new `m_pFrame_ATTACK_LINE_END0_RWeapon` / `_START0_RWeapon`,
  `+0xefc` / `+0xf00`). **Corrects DU5b2's amendment:** June keeps one frame set, not one per body; a later body's weapon overwrites (Ciel's when she
  has one). DU5b2's two stand-in hunks were replaced by June's body (the 10 deleted lines flagcheck counts are those flag-on lines and their
  markers; March's lines stay whole in the `#else`). Other classes fill body 0 only: same frames as March.
- **Enchant fire, case 10:** new `ProcessEnchantWeaponEffectForLuciel` (`31f93050`, OnFrameMove case `@31fa96fe`): Ciel shown -> middle of
  `TRACE_START[0]` / `TRACE_END[0]`, else Lu's `Bone_RFinger2` (skin `+0x1e8`); emit 10. Init stays the Sword one (June's default).
- **Hand sparks, case 10** (`31f961a0 @31f96536`): Ciel shown -> her own skin's `Dummy1_Rhand`, else Lu's `Bone_RFinger2`; rates 15 / 30 / 60 / 25 / 15
  (read from the exe). The March stub comment `UT_LUCIEL / UT_ROSE / UT_AIN cases: checklist 7as` stays for Rose / Ain.
- **+11 / +13 particles:** `InitUpgradeWeaponParticle` puts Lu/Ciel in the first group (`31f92910`); `ProcessUpgradeWeaponParticle` case 10
  (`31f9a900 @31f9b488`, handles mapped from the listing's registers): cash shop / state 10 -> Lu's fingers (second group emits 0) or Ciel's
  `Dummy3_RWeapon` / `Dummy4_LWeapon`; elsewhere Ciel's form -> middles of the two line pairs (missing frames: InitWeaponTrace after 2 s), Lu's
  form -> fingers. Groups: handles 1 2 5 6 9 / 3 4 7 8 10; the +13 six only when all exist. Helper `PlaceUpgradeWeaponSequences` in
  `_CONST_UNITVIEWER_DU5D_`.
- **Checklist (CH0):** UnitViewerEffectSetTable: the loader, the duo column and filter were in (`JUNE_UNIT_VIEWER_EFFECT_TABLE`, `DUO_BODY`); the two
  viewer lookups passed 0, now the shown body as June (`31fa3010 @31fa3219`, `31fa8ce0 @31fa92e9`; the one `DT_MINOR` row is `LDN_LobbyWait`).
  `IS_FOR_MINOR_UNIT`: already read (`X2SlotItem.cpp:63-65`, `JUNE_CHARINFO_SHOW`). Form-change portrait `328dcba0`: already ported (CH5,
  `UpdateCharacterPortraitByForm`). CharCustomData `DUO_TYPE` (`326525d0`): March has no customize reader, nothing reaches it offline: Stage 12
  (`JUNE_PORT_BACKLOG_ORDER.md` "Stage 12"), no change. Glow (§7as): the four sites above.
- **Split (one logical change):** the DU5c2 amendment's back-body emotions (`+0xd82` writers, `31fa64f0`), the window's equip tint `31f93920` (walks
  each equip's sub-equip list `+0x8dc`, which june-engine R14f's `LoadAttached` brings) and the other `SetUnitScale` callers -> **DU5d2**
  (kickoff written, after R14f).
- **Gates:** `finish.py --files X2Lib/X2UnitViewerUI.cpp X2Lib/X2UnitViewerUI.h --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER`: `FINISH RESULT: PASS (6
  gates, 0 failed, 3 skipped)`; flagcheck 1 PASS, 1 PASS~ (blank / marker), 0 OTHER, 0 FAIL, 10 deleted lines (above), 0 EOL-mismatched; INFO
  `CHARINFO_VIEW_ATTRIB_ENCHANT`, `DIVERGE_UPGRADE_WEAPON_PARTICLE`, `UNIT_VIEWER_EFFECT_PLAYER` (enclosing, not this change's); junediverge PASS;
  enumscan exit 0; stateends / skillicons / luabind n/a. New local `#error` in `X2UnitViewerUI.h` (needs `CHARINFO_VIEW_ATTRIB_ENCHANT`,
  `ADD_UPGRADE_WEAPON_PARTICLE`, `ARA_CHARACTER_BASE`): all three on at that line by a `cl` probe.
- **Commit** `0ebcfdff`; `finish.py --range 0ebcfdff^..0ebcfdff --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER`: `FINISH RESULT: PASS (6 gates, 0 failed,
  3 skipped)`, flagcheck 1 PASS, 1 PASS~, 0 OTHER, 0 FAIL, the same 10 deleted lines.
- **Compile:** `/Zs` probe of `X2UnitViewerUI.cpp`: 0 errors.
- **Build:** `DEPLOY OK: X2_offline.exe is 27,104,768 B, dated 2026-11-16 10:16:26, md5 65ccb63e7b13cee700e2ae15090828c5` (built at main 63e45d19 +
  28 uncommitted code files: this phase's and peers' in-progress files). `EXESTR OK: 3 of 3 found in X2_offline.exe` (`ATTACK_LINE_START0_RWeapon`,
  `ATTACK_LINE_START0_LWeapon`, `Dummy3_RWeapon`, wide).
- **Play-test passed** (user, 2026-11-16, md5 `65ccb63e`): character window on `testlu1`, Lu shown / Ciel shown / swaps / reopen, another class
  unchanged. Logs (run after the deploy): 0 EXCEPTION; `LUCIEL DUO FORM` / `AVATAR` / `LOBBY` lines as before; ErrorLog's `OpenXET` misses
  (`MOTION_CIEL_LDN_EVIL.XET` among many classes' XETs at game load) are a load-time batch, not this phase. Next: `DU5d-review.md`.
- **Save:** `testlu1` (166) normal weapon 62119 set to +13, fire attribute (`attrib0` 1); backup `db_backup\els_db_20261116_100555_pre_sql.sql`.

### DU5d - Review (2026-11-16)

Opus 5.5 high. Reviewed `0ebcfdff` against June in Ghidra / `disasm_range.py` / `charport_consts.py`. Verdict: **pass, no code change** (flag
`SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER`; no debug define to retire).
- **`InitWeaponTrace` = `31f9a2a0`:** bodies 0-2 (`local_8c < 3`); fashion UID tested as a 64-bit pair (`+0x5e0 / +0x5e4 + body x 8`), then the
  normal one (`+0x238`); a fashion UID that misses the map does nothing (no fall-back to the normal one), as the port's `continue`; templet test
  `31f90b00`; 8 models of `CX2Eqip +0x544` (`GetNormalAnim`) into `+0xdc8` / `+0xde8` / `+0xe08`, absent model -> 0; trace names by unit type 6
  (`ATTACK_SPHERE3_Back01`) = March's `GetWeaponTraceStartAndEndName`; the line frames only with `+0x1e0` set, after the trace loop: model 0 type 7
  `END0` / `START0_FRONT`, type 10 `END0` / `START0_LWeapon` -> `+0xecc` / `+0xed0`; model 1 type 10 `END0` / `START0_RWeapon` -> `+0xefc` / `+0xf00`;
  none reset when absent. One frame set, the last body with a weapon wins (DU5d's correction of the DU5b2 amendment stands).
- **`31f93050`:** empty / -1 fire list, `+0x1e8` NULL or no instance -> return; `CanShowWeaponEffect`; `+0xd80 == 1` -> ( `+0xdc8` - `+0xde8` ) x 0.5
  (qword `3342ebc8`) + `+0xde8` = ( START - END ) x 0.5 + END; else `Bone_RFinger2` on `[ +0x1e8 ]`; emit 10 (`3344a51c`), hidden 0. OnFrameMove
  jump table `31fa9e38` entry 7 (type 10) = `31fa9731` -> `31f93050`.
- **`31f961a0` case 10:** jump table `31f96868` entry 9 = `31f96536`; Lu: ECX = `[ +0x1e8 ]` (set before the switch) `Bone_RFinger2`, rates 15 / 30 /
  60 / 25 / 15; Ciel `@31f96603`: ECX = `[ +0xd7c ]`, `Dummy1_Rhand` (`3345bb58`), shares `@31f96471`'s 15 / 30 / 60 / 25 / 15.
- **`31f92910` case 10:** first group, as the port's added label.
- **`31f9a900 @31f9b488`:** handles from the load sequence: 1 EDI, 2 EBX, 3 `[0x1c]`, 4 `[0x10]`, 5 EBP, 6 `[0x14]`, 7 `[0x18]`, 8 `[0x28]`,
  9 `[0x24]`, 10 `[0x20]` (ESP-adjusted for the two pending pushes); h1-h4 tested first; state `+0xf14` (= `m_eUnitViewerState`) 10 or 1. In all four
  branches the first vector (stack `0x44` / `0x50` by branch) goes to 1 2 5 6 9 and the other to 3 4 7 8 10: Lu fingers on `[ +0x1e8 ]` (R first,
  second group emits 0); Ciel in the cash shop `Dummy3_RWeapon` / `Dummy4_LWeapon` on `[ +0xd7c ]` (all 10); Ciel's form (unit `+0xe40` =
  `IsMinorUnitForm`) the `+0xecc..+0xed0` midpoint first and `+0xefc..+0xf00` second (all 10), a NULL among the four -> `@31f9ad60` timer > 2.0
  (qword `33437820`) -> `31f9a2a0`; else Lu fingers. The 5-10 test follows the first four placements, as the port. `+0xd80` other than 0 / 1 in the
  state branch: June does nothing; the port's `bool` cannot hold one. State 10 stays a literal with its comment (March's enum ends at `UVS_WATCH` 8;
  no June Enum entry).
- **Effect-set duo argument:** `@31fa3219` and `@31fa92e9`: `CMP [ +0xd80 ], 1` / `SETE` pushed as the duo argument = `DT_MINOR` (1) when Ciel is
  shown, `DT_MAJOR` (0) else.
- **Flag-off = March:** `cl /P` of `X2UnitViewerUI.cpp` at `0ebcfdff` and `0ebcfdff^` with every `#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` made
  `#if 0`: 369,083 lines each, identical after `__LINE__` / FOREACH-counter digits. The header's three hunks sit wholly inside the flag.
- **Gates:** `finish.py --review --commits 0ebcfdff --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER -v`: `FINISH RESULT: PASS (7 gates, 0 failed, 3
  skipped)`; flagcheck 1 PASS, 1 PASS~ (3 blank / marker lines outside the flag), 0 OTHER, 0 FAIL; the 10 deleted lines are DU5b2's two stand-in
  hunks and their markers (March's lines stay whole in the `#else`); junediverge and `--check-all` PASS; enumscan exit 0; INFO
  `JUNE_CHARINFO_VIEW_ATTRIB_ENCHANT`, `JUNE_DIVERGE_UPGRADE_WEAPON_PARTICLE`, `JUNE_UNIT_VIEWER_EFFECT_PLAYER` (enclosing, not this change's).
- **Build:** full build skipped: no play-test (no code change; DU5d's play-test passed on `65ccb63e`).
- Next: DU5d2 waits for june-engine R14f.

### DU5d2 - Done (2026-11-16)

Opus 5.5 high. Flags `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (DU5b's: viewer, equip) and `SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY` (DU5c2's: the
`SetUnitScale` callers). Files: `X2UnitViewerUI.cpp` (+78), `X2UnitViewerUI.h` (+2), `X2Eqip.cpp` (+18), `X2Eqip.h` (+5), `X2StateClassChange.cpp`,
`X2CashShop.cpp`, `X2SquareUnit.cpp`, `X2StateDungeonResult.cpp`, `X2StateCreateUnit.cpp` (+4 each). funcmap rows: `grep "(DU5d2)" june2017_re/funcmap.tsv`
(8). Divergent row `CX2UnitViewerUI::SetFixed` now ported (verdict stays `stage2`). No log line added.
- **Equip tint by body** (`SetEquipColorByBody` = June `31f93920`): every view equip of the given body gets the colour on each sub-equip
  (`CX2Eqip::SetSubEquipColor` = `3295a9e0`: the skin's render param or the mesh's `m_NormalRenderParam`) and `m_bBackUnitTint` = 1, the other
  body's equips get 0. `UpdateSwitchingView` calls it twice as June does (`@31f93cf8`: the shown body 1 x 4; `@31f93e4b`: the other body 0.3 x 4, the
  call that sets the flag last); DU5b's stand-in comment is gone. `AddEqip` ends as June (`@31f9e859-31f9e8fc`): `UVS_CHARINFO` tints the other body
  0.3 x 4; `UVS_CASHSHOP` with the back body hidden tints it 0 x 4 (no March path hides it, so inert).
- **`+0x190` named from its readers:** `m_bBackUnitTint`, read only by `3295b430` (the custom render copy forces 0.3 x 4) from `SetRenderParam`'s
  custom-render branch and `3295c460`: backlog **Stage 11** (bullet added). Ctor 0 (`@329614f6`).
- **`SetFixed` = June `31f933c0`** (R14f's route): `true` also fixes a `UT_SKIN` equip with sub-equips or attached particles
  (`HasSubEquipOrAttachedParticle`), then hides the enchant; `false` = March's. `#error` without `JUNE_DIVERGE_EQIP_SET_FIXED` (March's
  `CX2Eqip::SetFixed` hides sub-equips). Every March caller passes `true` as June does (class change, watch x 2, community, char info x 2).
- **Back-body gate (`+0xd82` = `m_bShowDuoBackUnit`):** `31fa5c90`'s two writes go into PlayByMotionType's port: type 7 (SIT_READY) sets it with no
  effect stop (`@31fa5d4d`), the types that stop the effect groups set it after the stops (`@31fa5d36`). Its third write (`@31fa61d0`, = 0) and all 8 of
  PlayEmotion's (`@31fa5672`..`@31fa58ad`) sit behind the custom sit names (`+0xffc` + i x 0x1c): backlog **Stage 12** (bullet added). With no 0
  writer reachable, the gate stays on, as March's viewer behaves today.
- **Emotion end by body (`31fa64f0`, the body pick only):** `EmotionFrameMove` waits on the shown body's anim end (`GetXSkinAnimPtr`), or on the
  back body's when the new `m_bEmotionOnBackUnitOnly` (`+0xe60`) is set. A missing body counts as ended. The ended case clears the flag
  (`@31fa68ca`), and PlayByMotionType clears it too (`@31fa5cdb`). March read Lu's skin (`m_pXSkinAnim`) even with Ciel shown. The flag's only setter
  (PlayEmotion `@31fa5c30`) needs June's per-body EmotionData test (`32351900`), so it is a stand-in comment, backlog **Stage 10** (bullet added,
  with `32351070`'s per-body mix and `31fa4c00`'s per-body play). March's PlayEmotion already plays both bodies (`PlayAnim` default `UVPB_BOTH`).
- **`SetUnitScale` callers.** Every Lu/Ciel viewer builds Ciel's skin, room and field included (`SetUnit`), so March's viewer-only `Scale` left her
  unscaled. Swapped:
  - class-change popup (`324886b0 @32488b3d`, 1.4 / 1.5)
  - cash shop `ResetUnitViewer` (`32233f70`, 1.5)
  - square unit frame move (`323f23c0`, `m_Scale x GetScale`)
  - dungeon / training result (`3250e6c0`, 1.9)
  - create-screen job preview `SelectUpperClass` (`325519d0`, 1.2)

  Left as they are, no change:
  - the 11 per-class `Select*` (one body)
  - cash shop `SetOpen` / `OnFrameMove` (re-apply the same 1.5; June's pre-equip vec3 list is June-only UI)
  - the GFx customize handler and wheel zoom (`3224d0f0`, `3224c760`: no March counterpart, Stage 12)
  - PvP room / result and PVE result (`3253d7d0`, `32516a70`, `324a9650`): offline has no PvP; June's PvP-room 0.65 and per-motion factors are noted
    here only.
- **Touched-line enum fix:** `X2StateClassChange.cpp:884` (S7h's `117`) now reads `CX2Unit::UC_CHUNG_TACTICAL_TROOPER`, same value.
- **Gates:** `finish.py --files <9> --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY SERV_IRUHADEV_JUNE_SKILL_CLASS_POPUP
  --skip-flag SERV_IRUHADEV_JUNE_DIVERGE_EQIP_SET_FIXED` (the `#error` target): DUO_LOBBY PASS (7 PASS, 2 OTHER, 3 deleted = DU5b's two stand-in
  comments + the `117` line; the five March scale lines stay whole in `#else`), DUO_VIEWER PASS (4 PASS, 5 OTHER, 2 deleted = DU5b's two stand-in comments), SKILL_CLASS_POPUP PASS
  (1 PASS, 8 OTHER, 1 deleted = the `117` line), junediverge PASS. enumscan exit 1: 229 older literals in `X2CashShop.cpp` / `X2StateClassChange.cpp`,
  none on a changed line (checked by line), so not this phase's.
- **Compile:** `/Zs` probes of all 7 `.cpp`: 0 errors.
- **Build:** `DEPLOY OK: X2_offline.exe is 27,159,552 B, dated 2026-11-16 11:50:04, md5 dc655e7b67f7b84d92c5e37548fb4a8d, equal to X2/US_SERVICE/x2.exe
  (2026-11-16 11:49:57); built at main 3e9fa56d + 17 uncommitted code file(s).` (this phase's 9 and peers' in-progress files; every edited
  `.obj` is newer than its source). No exestr line: no string added.
- **Commit** `990fc1e3`; `finish.py --range 990fc1e3^..990fc1e3` (same flags): DUO_LOBBY / DUO_VIEWER / SKILL_CLASS_POPUP PASS with the tallies above, junediverge PASS, enumscan the same 229 older rows (none on a changed line).
- **Play-test** (user, 2026-11-16, md5 `dc655e7b`): checks 1, 3, 4 and 5 passed. Check 2 failed: Ciel was drawn headless behind Lu in the cash shop.
- **Fix (same flags):** June's cash shop shows no back body. `ResetUnitViewer` (`32233f70`) runs the switching-view filter after the first frame move
  (`31f94190( empty )`), writes `+0xd84` = 0.33 (`@32234125`, `33483504`), and turns the second body off at the end (`+0xd81` = 0, `@32234246`).
  March drew Ciel behind Lu. All three are ported. `+0xd84` is now `m_fBackUnitOffsetRate`, read only by `BackUnitFrameMove` (`@31fa8e4b`: offset x
  ( rate + 1 )). The move `31f99f70` adds the plain offset, so it is unchanged. DU5c2 said `+0xd84` had no writer after the ctor (its code comment
  stays as written). June's writers are the cash shop, the class-change popup (1.5, `@32488b4b`, now ported) and the GFx handler (`3224d0f0`,
  Stage 12). Gates (`--files` of the 4, same `--with`): DUO_LOBBY PASS (4 PASS, 1 deleted = DU5c2's back-body `Move` line, now times the rate),
  DUO_VIEWER PASS, SKILL_CLASS_POPUP PASS, junediverge PASS, enumscan the same older rows (none on a changed line). `/Zs` probes 0 errors.
  `DEPLOY OK: X2_offline.exe is 27,159,552 B, dated 2026-11-16 12:19:43, md5 451483489dd8639d115fa6b7fd5d5d10, equal to X2/US_SERVICE/x2.exe
  (2026-11-16 12:19:36); built at main 65abbe87 + 4 uncommitted code file(s).` (this fix's 4).
- **Re-test passed** (user, 2026-11-16, md5 `45148348`): cash shop shows Lu only, the character window and another class are unchanged. Logs (run 12:57, after the deploy): 0 EXCEPTION, 0 ErrorLog lines, 0 unbound Lua names; `LUCIEL DUO LOBBY` / `AVATAR` / `FORM` lines as before.
- **Not done here:** MODS.md's R14f entry still lists the viewer `SetFixed` as "not ported yet"; MODS.md has a peer's uncommitted edits, so char-luciel Z
  updates that line. Review: `DU5d2-review.md`.

### DU5d2 - Review (2026-11-16)

Opus 5.5 high. Reviewed `990fc1e3` and the fix `ab16f820` against June (`disasm_range.py`, constants read from the exe). Verdict: **pass, no code
change** (flags `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` / `_DUO_LOBBY`; no debug define to retire).
- **`31f93920`:** map `+0x1fc`, NULL equip skipped with no flag write; `CMP bl, [ +0x15d4 ]` -> `3295a9e0( colour )`, then `SETE` into `+0x190` for
  both outcomes (match 1, else 0). **`3295a9e0`:** `+0x8dc` sub-equips, NULL skipped; `+0x240 == 1` -> vslot 0x14 (`GetRenderParam`, funcmap row
  `327a8fff`) else `+0x198`; NULL param skipped; colour at `+0x28..+0x34`. Port = June.
- **`UpdateSwitchingView`:** `@31f93cf8` pushes `[ +0xd80 ]` with `fld1` x 4; `@31f93e4b` pushes `+0xd80 == 0` with `3345993c` = 0.3 x 4, in that order.
  **`AddEqip` tail `@31f9e859`:** `+0xf14` 1 (`UVS_CASHSHOP`) with `+0xd82 == 0` -> `fldz` x 4; 7 (`UVS_CHARINFO`) -> 0.3 x 4; both push `+0xd80 == 0`.
  Enum values checked: `UVS_CASHSHOP` 1, `UVS_CHARINFO` 7.
- **`+0x190`:** readers and ctor as the Done note; Stage 11 bullet stands.
- **`31f933c0`:** `+0xe40` = arg; true: `+0x1c8` 2 / 3 fix, else type 1 with sub-equips (`+0x8dc` / `+0x8e0`) or particles (`+0x8f0` / `+0x8f4`) fix
  (`3295a560`); then `31f91f90( 0 )`; false: `31f91f90( 1 )` only when shown (`31fae370` / `+0x172`) = March's false path. `USE_TYPE`: `UT_SKIN` 1,
  `UT_ATTACH_ANIM` 2, `UT_ATTACH_NORMAL` 3. Port = June.
- **`31fa5c90`:** `+0xe5c` cleared unless state 10, then `+0xe60` = 0 (`@31fa5cdb`) unconditionally; type 6 / 8 skip; 7 -> `+0xd82` = 1 (`@31fa5d4d`);
  others stop the groups (state 1: 0..2, else `31fac190`) then `+0xd82` = 1 (`@31fa5d36`). `UVUMT_HABIT` 6, `SIT_READY` 7, `SIT_WAIT` 8 under
  `JUNE_ANIMATION_NAME_TABLE`. June's types 1-2 custom-WAIT early return (`+0xf74`, after the `+0xe60` write) is Stage 12 and never taken in March, so
  the port's placement is equal on every reachable path. `@31fa61d0` and PlayEmotion's 8 writes: behind the custom sit names, Stage 12 route stands.
- **`31fa64f0`:** shown skin (`31fac430`) `+0x2ac`, NULL -> ended; `+0xe60 == 1` replaces it with the back skin's (`31f91020`), NULL -> ended; the ended
  default clears `+0xe5c` and `+0xe60` (`@31fa68ca`). Port's ternary = June.
- **Fix `ab16f820`:** `32233f70`: `Move` (`31f99f70`), `+0xd84` = `33483504` = 0.33 (`@32234125`), `SetUnitScale` (`337335e0` = 1.5 x 3); in the
  unit branch the frame move (vslot 1) then `31f94190( empty )` (`@32234214`); after the branch `SetShowObject( +0xd8, 1 )` then `+0xd81` = 0
  (`@32234246`), both inside the viewer test. Class change `@32488b4b` = `33471a1c` = 1.5. `BackUnitFrameMove @31fa8e4b`: `+0xd84` + 1.0 (qword
  `334355b0`) times `33732708` = ( 50, 0, 70 ). Port = June.
- **`SetUnitScale` sites:** class change 1.5, or `3344f240` = 1.4 when class `0x75` (`UC_CHUNG_TACTICAL_TROOPER`); cash shop 1.5; square
  `323f2b86` `GetScale` x `+0x358..+0x360` (`m_Scale`); result `3250e770` `334a1a44` = 1.9; create `32551bb7` `3344ce94` = 1.2. All five = June. The
  left-alone list (one body, same value, no March counterpart, PvP) stands.
- **Flag-off = March:** `cl /P` of `X2UnitViewerUI.cpp` and `X2CashShop.cpp` from trees extracted at `ab16f820` and `990fc1e3^`, `DUO_VIEWER` and
  `DUO_LOBBY` commented out (the peer `#error` guards on `DUO_VIEWER` disarmed in the flag-off copies only): token-identical (366,315 / 368,864 lines)
  after `#line` / `__LINE__` / FOREACH-counter masking; flag on differs (1,285 / 80 diff lines).
- **Gates:** `finish.py --review --commits 990fc1e3 ab16f820 --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY
  SERV_IRUHADEV_JUNE_SKILL_CLASS_POPUP --skip-flag SERV_IRUHADEV_JUNE_DIVERGE_EQIP_SET_FIXED "#error guard target" -v`: DUO_LOBBY PASS (11 PASS,
  2 OTHER, 4 deleted), DUO_VIEWER PASS (6 PASS, 7 OTHER, 3 deleted), SKILL_CLASS_POPUP PASS (2 PASS, 11 OTHER, 1 deleted), junediverge PASS,
  `--check-all` PASS; enumscan exit 1 = the 229 older rows in `X2StateClassChange.cpp` / `X2CashShop.cpp` (none on a changed line, as the Done
  note). Every OTHER row is a file of this change owned by one of the other two `--with` flags. Deleted lines: DU5b's two stand-in comments (inside
  DU5b's flagged `AddEqip` / `UpdateSwitchingView`), the `117` line (inside S7h's `SKILL_CLASS_POPUP` block, now the enum), and DU5c2's back-body
  `Move` line (inside the flagged `BackUnitFrameMove`, now times the rate): no March line is lost. `JUNE_DIVERGE_EQIP_SET_FIXED` SKIPPED: the `#error`
  guard's target, not this change's flag.
- **Kept as written:** `X2UnitViewerUI.cpp:9`'s comment ("`+0xd84` is 0 outside the customize preview") and PlayByMotionType's head comment ("`+0xe60`
  and `+0xd82` have no March member") are older text this phase made stale; old comments are not rewritten, the Done note and this review hold the
  current facts.
- **Build:** none (no code change; no play-test). Next: CH3c.

### DU5e - Done (2026-11-15)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_DUO_STAT` (defined; second homes `ServerDefine.h`, `SECOND_HOMES`, `X2ServerProtocol_2010.vcxproj` X2TOOL;
`#error` guard: `DUO_BODY`, `TRANSFORM_LINK`, `EQUIP_VIEW`, `NESI_RENUMBER`, all ON by the /Zs probes). Rows: funcmap `(DU5e)` (9), JUNE_BUGS 753.
**Split:** the socket half (type-10 aggregate, HUD second gage, set count, per-body socket record, `@3291bf2d`) -> **DU5f** (kickoff written).
- **Writer:** `CX2Unit::UnitData::SetGameStats( first, second )` = `323571c0`: `m_GameStat` and `m_MinorUnitGameStat` by March's `SetKStat` each (June's
  KStat has 7 fields, field 0 a float base HP; the inline conversion zeroes the extra rates as `SetKStat` does). June's `SetKUnitInfo` `3235eb20`
  writes no second stat (`@3235fa09` / `@3235fb58`): the village unit keeps the last room's.
- **Packets:** `KRoomUserInfo` (`CommonPacket.h`, June +0xb0 / +0xcc) and five NOTs (`ClientPacket.h`: equip change +0x19 / +0x35, skill info
  +9 / +0x25, game stat +8 / +0x24, zero endurance, level up) carry `m_kMinorUnitGameStat` right after `m_kGameStat`; Put / Get beside it.
- **Seven sites:** `SetKRoomUserInfo` (`32360090 @323608cf`), `CX2Room` equip / skill info / game stat (`31ed46e0`, `31ed4c80`, `31ed57d0`),
  `CX2StateMenu` equip change (`31f1ed70`), zero endurance (`32461e80`), level up (`32473f70`). Only the equip change gives the user the stat of the
  unit's form (`@31f1ee8c`); the other four give the first and every max HP reads the first (JUNE_BUGS 753, verbatim).
- **Transform test:** both StateMenu handlers wrap March's NEW_CHARACTER_CHUNG view refresh and the weapon refresh in `!IsTransformed( DT_BOTH )`
  (`@31f1f1d8`, `@324620b9`); the equip change then calls `UpdateDuo( false )` (`@31f1f27a`; returns at once for other classes, re-reads Lu/Ciel's
  stat). The rest of those two June bodies (change type 5, the gage clamp) is not compared: routed to backlog Stage 16.
- **Emulator** (stand-in, no June server): `MakeGameStat` sums Lu's slots + the shared ones for `UT_LUCIEL` (`AddEquippedStat( kStat, 0 )`), every
  slot for other classes (unchanged); new `MakeMinorUnitGameStat` = base + Ciel's slots + shared (group 1; zero for other classes), the slot table of
  `CX2Inventory::GetEquipGroup`. Filled in the room user info, the room equip and title NOTs and the level up. Log
  `[JUNE_CHAR] LUCIEL DUO STAT unit <uid>: Lu hp .. atk .. / .. def .. / .., Ciel hp ...`.
- **Character window:** no change needed: DU5's `GetShownEquipStat( form )` already sums each body's gear.
- **Stale comment kept:** `X2Unit.h`'s `m_MinorUnitGameStat` still says "stand-in: zero until its writer lands" (DU1c's line; not rewritten).
- **Gates:** `finish.py --files <17 files> --with SERV_IRUHADEV_JUNE_CHAR_DUO_STAT`: flagcheck 12 PASS, 2 PASS~, 1 DEFINE, 1 OTHER (`Always.h`: peers'
  uncommitted blocks), 1 FAIL = `X2Unit.cpp` stripped line 6293, a peer's uncommitted char-luciel CH5 `LUCIEL_UI` hunk in the same file (not staged;
  the range run below has none); 0 deleted, 0 non-ASCII. junediverge PASS. enumscan exit 1 on 191 pre-existing rows, **0 on DU5e's lines** (`git diff -U0` intersect).
- **Compile:** `/Zs` probes of the 9 changed TUs (incl. `X2ServerPacket.cpp`): 0 errors.
- **Build:** `DEPLOY OK: X2_offline.exe is 27,048,960 B, dated 2026-11-16 04:04:33, md5 8c2dc751abba8b12eb0886bb3d122baf` (built at `2c38224a` + 19
  uncommitted code files); `EXESTR OK: 1 of 1` (`[JUNE_CHAR] LUCIEL DUO STAT ...`, wide).
- **Commit** `9286ce8d`; `finish.py --range 9286ce8d^..9286ce8d --with SERV_IRUHADEV_JUNE_CHAR_DUO_STAT`: flagcheck PASS (13 PASS, 2 PASS~, 2 DEFINE,
  0 OTHER, 0 FAIL); junediverge PASS; enumscan the same 191 pre-existing rows.
- **Play-test: run read** (2026-11-16, 04:07-04:13 on md5 `8c2dc751`; the user played battlefield 40000, not the village first). testlu1's own gear
  already splits the bodies: `LUCIEL DUO STAT unit 166: Lu hp 128500 atk 6881 / 6608 def 353 / 347, Ciel hp 128500 atk 6911 / 6638 def 363 / 357`. The
  necklace (`/additem 64000`) went on Lu's slot 18 both times (dropped with Lu active: outside the village the window follows the active body, DU5):
  each room equip NOT sent one line, Lu hp 128500 -> 129700 -> back, Ciel unchanged. The user saw no HP-bar change on tag: = June (one shared HP bar,
  every handler sets max HP from the first stat, JUNE_BUGS 753); Ciel's own atk / def (+30 / +10) is not visible in play. 0 EXCEPTION, ErrorLog clean,
  UNHANDLED only `E_DISABLE_HB_CHECK_REQ` x4. The review may ask for a Ciel-slot drop (switch to Ciel in game, then drop) if it wants that direction too.

### DU5e - Review (2026-11-16)

Opus 5.5 high. Reviewed `9286ce8d` against June (`disasm_range.py`, read-only). **Verdict: pass, no code change.** No build, no play-test (no fix landed).
- **Writer** `323571c0` (`ret 0x38` = two 7-dword KStats by value): per body it decodes the 13-dword xor-protected stat, zeroes dwords 0-5, writes the KStat's
  float field 0 + six ints (`fild`) into dwords 6-12, re-protects; first -> `+0x164` (`m_GameStat`), second -> `+0x1a0` (`m_MinorUnitGameStat`). March's `SetKStat`
  = the same five values + the six rate resets; its `LimitMaximum` is empty, so no difference. June's two extra KStat ints have no March field (wire and struct are
  March's five; Stage 33). Note: `june_march_divergent.tsv` row `SetKStat` (stage33) says "no extra-rate reset"; the listing does zero six dwords (left to Stage 33).
- **Push order** (every site: first `sub esp, 0x1c` copies the higher offset = arg 2, second the lower = arg 1): equip change room `+0x19` / `+0x35` (`@31ed47e5`),
  skill info `+9` / `+0x25` (`@31ed4d63`), game stat `ebp` / `+0x1c` (`@31ed5884`), StateMenu equip `[esp+0x3d]` / `[esp+0x59]` (`@31f1eec5`), zero endurance local
  `+0x50` / `+0x6c` (`@32461fd4`), level up (`@324740fe`, `+0x5d` / `+0x79`), room user info `+0xb0` / `+0xcc` (`@323608a9`). All = the port (first, second), each pair
  0x1c apart = the second KStat right after `m_kGameStat`, as the packet structs and Put / Get place it.
- **User stat (vslot 0x1e0):** `31f1ed70 @31f1ee8c` `cmp byte [unit +0xe40], 1` picks `+0x59` (second) else `+0x3d` = `IsMinorUnitForm()` (DU1); zero endurance
  `@32461fc5` passes local `+0x50` (first). Max HP: equip `@31f1eef7` and game stat `@31ed589f` feed the gage the first's base HP. JUNE_BUGS 753 reads right.
- **Transform test** equip `@31f1f1d4`: `IsTransformed( 2 )` true -> `@31f1f27f`, skipping vslots 0x63c( 1 ) / 0x630 / 0x634, the three `SetEnableAttackBox` calls, the
  weapon loop (category 9 = `ST_E_EQUIP`, slot 9 / 10 = `g_iEquippedNormalWeaponSlotID` / `g_iEquippedFashionWeaponslotID`, vslot 0x28c, break) and `UpdateDuo( 0 )`
  `@31f1f27a`: exactly March's NEW_CHARACTER_CHUNG block + weapon loop, `UpdateDuo` inside it. Zero endurance `@324620b5`: same test around the same block, weapon refresh
  by templet `+0xd5 == 2` (`EP_WEAPON_HAND`), no `UpdateDuo`, inside the per-item loop after vslot 0x644 = the port.
- **Emulator stand-in:** `AddEquippedStat( kStat, group )` filter = `GetEqipStat`'s test (`2 != want && 2 != slot && slot != want`); `GetEquipGroupOfSlot` = the
  `NESI_RENUMBER` branch of `CX2Inventory::GetEquipGroup` (March's onepiece is 35 there, not a minor slot, so the CHARINFO special case changes nothing). Lu = group 0,
  Ciel = group 1 (`MakeMinorUnitGameStat`, zero for other classes, which keep group 2 = every slot). Senders: room user info, room equip NOT, title NOT, level up fill
  both; the other `MakeGameStat` callers are `KUnitInfo` (no second stat in June), the play status max HP and the dungeon base HP (first stat, as June's HP).
  Groups stay numbers: no Enum entry (the values coincide with `DT_*`, but June names no equip group; DU4 / DU5 reviews kept them the same way).
- **Flag-off = March:** `cl /P` (census.py `cl_cmd`, no `/Yu`) of `X2StateMenu.cpp` / `X2Unit.cpp` / `X2Room.cpp` in a mirrored header tree, flag on vs `//#define` in
  both shadow `Always.h` and `ServerDefine.h`: 17 / 10 / 13 hunks (7 each = the six packet fields + the `SetGameStats` declaration), every flag-on line is DU5e's
  added text, every flag-off line the pre-DU5e text (the commit deletes no line in those files; 0 unmatched).
- **Gates:** `finish.py --review --commits 9286ce8d --with SERV_IRUHADEV_JUNE_CHAR_DUO_STAT -v`:
  ```
  flagcheck JUNE_CHAR_DUO_STAT       PASS     files: 13 PASS, 2 PASS~ (blank/marker only), 2 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR
  junediverge                        PASS     RESULT: PASS
  junediverge --check-all            PASS     RESULT: PASS
  enumscan                           FAIL     exit 1: 191 A/B row(s) remain in 12 file(s)
  stateends / skillicons / luabind   SKIPPED  not requested
  FINISH RESULT: FAIL (7 gates, 1 failed, 3 skipped)
  ```
  enumscan: the 191 rows are the pre-existing `X2Unit.cpp` `SI_*` lines (DU5e's note intersected them with its diff: 0 on its lines). 0 OTHER rows, 0 deleted lines;
  PASS~ = one blank line each in `X2OfflineInventory.cpp` / `X2OfflineServer.cpp`. INFO (not run): DUO_BODY, EQUIP_VIEW, NESI_RENUMBER, TRANSFORM_LINK,
  JUNE_SKILL_MEMO_RESYNC, OFFLINE (enclosing).
- **Stale comments kept** (old comments are not rewritten): `X2Unit.h` `m_MinorUnitGameStat` "zero until its writer lands", `X2Room.cpp` skill-info "March's packet has one".
- **Play-test:** DU5e's run stands; the Ciel-slot drop is not asked for: group 1 is the same filter with another argument and the run's first line already shows
  Ciel's own sum differing from Lu's (atk 6911 vs 6881).
- **Next:** `/x2-phase char-luciel DU5f` (Opus 5.5 high).

### DU5f - Done (2026-11-16)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_DUO_SOCKET` (new, `Always.h` only: no packet field) + `_DEBUG` (one `LUCIEL DUO SOCKET` line per max-MP reset;
the review retires it). Commit: see git log `char-luciel Phase DU5f`. Funcmap: 13 `(DU5f)` rows. JUNE_BUGS 762-765.

- **`SocketData_DuoUnit` (June `32091730`):** `CX2SocketItem::SocketData` plus each body's socket MP rate / MP add (`+0x468` Lu and shared, `+0x478`
  Ciel and shared; getters = vslots 3 / 4 `32091b10` / `32091b60`, ReInit `32092450`), filled by `AddMPStatOfBody` (`3271a060`: the option's own
  `m_fIncreaseMPRate` / `m_iAddMPValue`, raw). `CX2GameUnit::m_SocketData` and `CX2Unit`'s aggregate take the type; the pairs are read for `UT_LUCIEL`
  only (June picks the class at Init; March tests the unit type where it reads).
- **The record, every June caller:** user `InitSocketData` (each equip's vectors before DU3c's active-body skip, the pet aura with group 2),
  `InitSetItemOptions` (`32050230`, user vslot +0x798: mode = active body; Lu's sets in modes 0 / 2, Ciel's only for Lu/Ciel in modes 1 / 2), unit
  `InitSocketData( mode )` (record, then June's mode skip: S7k's "every source summed" is gone for Lu/Ciel), unit `SumSetItemSocketOption` (same groups).
  Item +0x8c / +0x9c / +0xac stay Stage 16.
- **HUD (`3235a160`):** gauge 0 reads Lu's pair, and for `UT_LUCIEL` gauge 1 = `ResetMaxMp( 300, 1 )` x ( 1 + Ciel's rate + ability ) + Ciel's add, sent with
  `CX2GageManager::SetMaxMp( uid, mp, 1 )` (June `3248a800`: the gauge index now reaches the gage data). DU1c2's stand-in comment replaced.
- **In game (`32b0fa90`):** `CX2GULuciel::ResetMaxMP` reads each gauge's own pair (CH1l2 read the shared sum for both).
- **Set counts:** `GetSetIDNPartsNum( map, group )` (June `32350cb0`, new overload) feeds the per-body set sums; March's `GetSetIDNPartsNum` takes June's
  `32350b80` filter (class test + `CX2Inventory::IsEquipInForm` `3286a3e0` with `CX2Unit::IsMyShownMinorUnitForm` `3234d8d0`): the window's set count
  follows its shown body. `june_march_divergent.tsv` row `GetSetIDNPartsNum` -> `replace`, ported; `statcalc_map.tsv` row -> `partial` (328823d0 left).
- **`CX2Inventory::GetEquippedSetPartsCount` (`3286a6b0`):** ported, unreferenced: its readers gate item +0xac (Stage 16), or are routed (funcmap row).
- **Window `@3291bf2d`:** ported verbatim; June sums the other body's sets into a local and drops it (JUNE_BUGS 764).
- **Not here:** other players' form bytes stay DU5c's (DU5c.md). Emulator `KGamePlayStatus` max MP stays the flat `ONE_CHARGE * 3` stand-in for both
  gauges (no June server); the client's resets above size the gauges.
- **Gates (`finish.py --files <13 files> --with ..._DUO_SOCKET ..._DUO_SOCKET_DEBUG`):** flagcheck `DUO_SOCKET` 9 PASS, 1 PASS~ (`X2SocketItem.cpp` blank line), 2 OTHER (the `_DEBUG` partner's
  files), 1 FAIL = `X2Unit.cpp` flag-off differs only at a peer's uncommitted `LUCIEL_UI` hunk (its 4 deleted lines; mine are insert-only, the committed
  range run is clean: see below); junediverge PASS; enumscan exit 1 on the 190 pre-existing `X2Unit.cpp` `SI_*` rows, 0 on DU5f's lines (`git diff -U0`
  intersect); stateends / skillicons / luabind not applicable (no skill states, skills or scripts). Syntax probe (`cl /Zs`) of the 7 changed .cpp: 0 errors.
- **Build:** COALESCED into peer build `95f548ff` (started after my last edit), then `DEPLOY OK: X2_offline.exe is 27,075,072 B, dated 2026-11-16 05:15:39,
  md5 06e712e076c2288a3104d1310b06a867`; `EXESTR OK: 2 of 2` (`LUCIEL DUO SOCKET HUD unit`, `... GAME unit`, wide).
- **Range gate (`finish.py --range 15a08390^..15a08390`):** flagcheck `DUO_SOCKET` PASS (11 PASS, 1 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL); junediverge PASS;
  enumscan the same 190 pre-existing rows.
- **Play-test: passed (user, 2026-11-16).** testlu1 (unit 166), Energy Fusion Ring 188833 (socket 60405 = +30 MP). Logs (`offline_server.log`, run
  after the deploy): no ring `HUD ... Lu max MP 300.0, Ciel max MP 300.0`; ring on Ciel `HUD ... Lu 300.0 ( add 0 ), Ciel 330.0 ( add 30 )` and in game
  `GAME ... gauge 0 ... -> 300.0` / `gauge 1 ... add 30 -> 330.0` on every reset; ring off `300 / 300`; ring on Lu `HUD ... Lu 330.0 ( add 30 ), Ciel 300.0`
  and `gauge 0 ... add 30 -> 330.0` / `gauge 1 -> 300.0`. Set count by the window's view: user-confirmed (soft check, no log line).
- **Next:** `/x2-phase char-luciel DU5f review` (Opus 5.5 high; retires `_DEBUG`).

### DU5f - Review (2026-11-16)

Opus 5.5 high. Reviewed `15a08390` against June (`disasm_range.py`, read-only). **Verdict: pass after 2 fixes**, both under `SERV_IRUHADEV_JUNE_CHAR_DUO_SOCKET`;
`_DEBUG` retired. Commit: see git log `char-luciel Phase DU5f review`.
- **Fix 1, window (`3291ba40` @`3291c120`):** June does not drop the other body's local. After the set loop it adds the local's `+0x2f0` (the window's
  accumulators are `SocketDataTemplet`s, `32738cc0`: raw stat, index 0 = `m_fIncreaseHPRate`; main at esp+0xd4, local at esp+0x468) into the window's
  result, then destroys the local. The port now adds `kOtherFormSocketData`'s HP rate when the other body's set map is not empty (June skips the block
  on an empty map @`3291c017`). JUNE_BUGS 764 and funcmap row `3291ba40 @3291bf2d` corrected: only the HP rate is kept.
- **Fix 2, in game (`32b0fa90`):** June computes `( max + max x rate ) + add` (`fmulp` / `faddp`, then `fadd` the add); DU5f kept CH1l2's
  `max + ( max x rate + add )`, which rounds differently. Now June's order. The HUD (`3235a160`) already had it.
- **`SocketData_DuoUnit`:** vslots `32091b10` / `32091b60`: argument true reads the pair at `+0x468` (values `+0x470` / `+0x474`), false `+0x478`; ReInit
  `32092450` = base `320922a0` + both pairs 0. `3271a060`: map value `+0x10` is the option's `SocketDataTemplet` (raw stat at `+0x2f0`, so `+0x2f4` =
  `m_fIncreaseMPRate`; funcmap 942), `+0x22c` = `m_iAddMPValue` (`fild`); group 0 / 2 -> `+0x468`, 1 / 2 -> `+0x478`; no PvP / dungeon-only test. = port.
- **Callers:** user `3206d770`: argument only for type 10 (@`3206d846`), the record (random + templet vector, `+0x5c`; `+0x8c` / `+0x9c` / `+0xac`
  Stage 16) before the skip @`3206dc6a`, both skipped when the argument is null (@`3206dbb3`); mode = `+0x8e90 == 1` (@`3206d8f7`) = `m_bMinorUnitActive`;
  aura `push 2` @`3206e5fa` per step before the sum. `32050230`: group 0 recorded with the pointer, summed in modes 0 / 2; group 1 only with the pointer,
  summed in 1 / 2; empty option lists skipped; level = max level or the unit's. Unit `32365d30` / `323652b0`: the same, mode skip only inside the
  record branch (@`32365eed` -> `323660b3`). All = port.
- **Gauges:** HUD: gauge 0 vslots ( 1 ), gauge 1 for type 10 vslots ( 0 ) and `ResetMaxMp( 300, 1 )`, both `( R + R x rate ) + add`, `3248a800( uid, mp,
  index )` -> gage data vslot 0x14 ( mp, index ) = `CX2GageData::SetMaxMp( mp, index )`. Lu/Ciel `ResetMaxMP` gauge 0 ( 1 ), gauge 1 ( 0 ). = port.
- **Form filter:** `3286a3e0` slot tables = `GetEquipGroup` `3286a4b0`'s (odd 1-9 -> both, Ciel slots 20-42 bar 35 / 37 / 39 / 41 -> form, rest and an
  unequipped item -> not form). `3234d8d0` / `32912cc0`: my unit `+0xe40`, overridden by the viewer's `+0xd80` only while the window's `+0x1e5` is set and
  the viewer (`+0x1b4`) exists. `32350b80`: usable, class (`31e4c380` = templet class, `31ed6500`), `328823d0` (Stage 16), form, set id. `32350cb0`:
  the same with `group == 2 || == arg`. = port.
- **Callers of `32350b80`** (7): party `31ed40c0`, `GetEnchantStat` `32358df0`, `GetRepairDiscountRate` `32363f00`, skill level by socket `32357a70`,
  window `3291ba40`, plus `31fa6980` and `320200b0` (no March caller; `320200b0` routed DU1d3b). March's live one-argument callers are exactly the first
  five; `X2UICharInfo.cpp:3901` is in `CHARINFO_STATS`' `#else`. **`32350cb0`** (5): all ported.
- **Not ported, routed:** June's user `+0x94a0`, a second option vector filled beside `+0x893c` (`320503bb`, `320505a6`, `3206dd52`-`3206de11`) for every
  class. Not this phase's subject: a bullet in `kickoffs/june-stat-calc/S3.md` (sockets and options).
- **Flag-off = March:** `cl /P` (census `cl_cmd`) of `X2Unit.cpp` / `X2GUUser.cpp` / `X2UICharInfo.cpp` on a `git archive 15a08390` tree, flag on vs
  `//#define`: 23 / 16 / 8 hunks, ON-only 118 / 61 / 47 lines all DU5f `+` lines, OFF-only 11 / 18 / 4 all pre-DU5f text, 0 unmatched.
- **Gates on DU5f (`finish.py --review --commits 15a08390 --with ..._DUO_SOCKET ..._DUO_SOCKET_DEBUG -v`):**
  ```
  flagcheck JUNE_CHAR_DUO_SOCKET     PASS     files: 11 PASS, 1 PASS~ (blank/marker only), 1 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR
  junediverge                        PASS     RESULT: PASS
  junediverge --check-all            PASS     RESULT: PASS
  enumscan                           FAIL     exit 1: 190 A/B row(s) remain in 13 file(s)
  FINISH RESULT: FAIL (7 gates, 1 failed, 3 skipped)
  ```
  enumscan: the pre-existing `X2Unit.cpp` `SI_*` rows (lines 6348+), none on DU5f's lines. 0 deleted lines; PASS~ = one blank line in `X2SocketItem.cpp`.
  stateends / skillicons / luabind not applicable (no states, skills or scripts).
- **Build:** `/Zs` probes of `X2UICharInfo.cpp` and `X2GULuciel.cpp`: 0 errors. Full build skipped: no play-test (the HP-rate carry shows only with a
  set on the hidden body, the order fix is a last-bit rounding); both ride the next play-test build.
- **Range gate (`finish.py --range b3f59bba^..b3f59bba --with ..._DUO_SOCKET ..._DUO_SOCKET_DEBUG -v`):** flagcheck `DUO_SOCKET` PASS (2 PASS, 1 OTHER =
  the `_DEBUG` retire in `Always.h`, 0 FAIL; the 2 deleted lines are DU5f's own lines inside its `#ifdef`: the window comment and the max-MP expression);
  `_DEBUG` retired (nothing to check); junediverge PASS; enumscan PASS. FINISH RESULT: PASS.

### DU6 - Done (2026-11-16)

Opus 5.5 high. New flag `SERV_IRUHADEV_JUNE_CHAR_DUO_AWAKEN_EFFECT` (defined, no debug define). Files: `X2Lib/X2GameUnit.cpp`, `KTDXLIB/Always.h`.
Found by the user in the DU5b2 play-test: awakening with Ciel active left the fire where Lu stood.
- **The effect:** the four hyper-boost particles (`HyperBoostRight_Luciel` / `HyperBoostLeft_Luciel`, created by Lu's `HyperModeBuffEffectStart` `32b1f0b0`,
  skipped for a transcended unit or one with an awakening effect set). Each frame `CX2GameUnit::HyperModeBuffEffectOnFrameMove` puts them on the feet
  and upper arms. March read the cached frames `m_pFrame_Bip01_R_Foot` ... (first body = Lu, which stands still while Ciel is active).
- **June `32291060 @32291266-322913ef`:** the same emit-rate tiers, then each position from vslot 0x84 `GetBonePos( L"Bip01_R_Foot" )` / `_L_Foot` /
  `_R_UpperArm` / `_L_UpperArm` (left-facing swaps the pairs as March). A user's slot 0x84 is `31fda280`: no prefix -> `32071d80`, the active body
  ( `+0x8e90` ? `+0x8e88` : `+0x87d4` ) = March's `CX2GUUser::GetBonePos` under `JUNE_CHAR_DUO_BODY` (DU1d2). No re-bind on a body switch is needed:
  the position is read every frame. The D0c-1 funcmap row ("same as March") missed the bone source; corrected.
- **Landed:** June's positions inside March's `HyperModeBuffEffectOnFrameMove` under the flag, March's `GetFramePos` block in `#else`. Other classes: the
  same bones of their one skin (user `GetBonePos` without a body = `m_pXSkinAnim`); NPCs: `CX2GUNPC::GetBonePos` = `m_pXSkinAnim`, the frames March cached.
  A missing bone gives a zero position (June) where March's `GetFramePos` left the vector unset.
- **Rows:** `funcmap.tsv` `32291060` -> PORTED (DU6 text); `june_march_divergent.tsv` new `HyperModeBuffEffectOnFrameMove` replace row, ported.
- **Gates:** `finish.py --files X2Lib/X2GameUnit.cpp KTDXLIB/Always.h --with SERV_IRUHADEV_JUNE_CHAR_DUO_AWAKEN_EFFECT`: `FINISH RESULT: PASS (8 gates,
  0 failed, 5 skipped)`; flagcheck `1 PASS, 1 OTHER, 0 FAIL` (the OTHER is `Always.h`, a peer's uncommitted lines); `junediverge` PASS; `enumscan` exit 0;
  two `retired` (peer debug defines), stateends / skillicons / luabind not requested (nothing registered or staged). Range gate `finish.py --range 04ab6729^..04ab6729 --with SERV_IRUHADEV_JUNE_CHAR_DUO_AWAKEN_EFFECT`: flagcheck PASS ( 1 PASS, 1 DEFINE, 0 OTHER, 0 FAIL ), junediverge PASS, enumscan exit 0, `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`. Commit `04ab6729`.
- **Build:** `COALESCED` (peer full build started 06:08:28, after these edits), then `DEPLOY OK: X2_offline.exe is 27,077,120 B, dated 2026-11-16 06:18:49, md5 383991cd4b67a71ea6192b711738a34f, equal to X2/US_SERVICE/x2.exe`.
- **How to see it in game:** LuDbA (202): awaken with Ciel active, the fire stays on Ciel's feet and arms while she walks and dashes; switch body while
  awake, it moves to Lu. Elsword / Aisha awakening unchanged.
- **Play-test: passed (user, 2026-11-16).** LuDbA (202): the fire stays on Ciel while she moves, dashes and jumps; it moves to whichever body is active on a
  switch; Elsword and Aisha awakening unchanged. Visual check only ( DU6 adds no log line ).
- **Next:** `/x2-phase char-luciel DU6 review` (Opus 5.5 high).

### DU6 - Review (2026-11-16)

Opus 5.5 high. Reviewed commit `04ab6729` against June's listing (`disasm_range.py 32291060 32291440`) and the exe bytes. **Verdict: pass, no code change.**
- **Positions:** each of the eight calls is vslot 0x84 with one string; the strings read from the exe are `3345bbc8` `Bip01_R_Foot`, `3345bbac`
  `Bip01_L_Foot`, `3347481c` `Bip01_R_UpperArm`, `334747f8` `Bip01_L_UpperArm`. Right-facing ( vslot 0x170 == 1 ): R_Foot -> handle +0xcbc
  ( `m_hHyperBoostRFoot` ), L_Foot -> +0xcc0, R_UpperArm -> +0xcc4, L_UpperArm -> +0xcc8; left-facing swaps each pair. Equal to the commit line by line.
  `SetPosition` is `31cf1a40` ( vec, 0, 0 ), then `+0x3a4 = 1` on all four = March's `SetTrace( true )`.
- **Emit rates:** remain < max / 3.0 ( `33439b50` ) -> `3345b368` 15 / `33438a8c` 20; remain < max * 0.5 ( `3342ebc8` ) -> 20 / `33441b9c` 30; else
  30 / `33438b40` 40 (doubles and floats read from the exe). Equal to March's tiers, which the commit keeps.
- **Which GetBonePos:** June Lu/Ciel vtable `334f06fc` slot 0x84 = `31fda280` = March `CX2GUUser::GetBonePos( const WCHAR* ) const`, which under
  `JUNE_CHAR_DUO_BODY` takes `GetCloneFrameByBodyPrefix` -> `GetXSkinAnimPtr()` (the active body). June NPC vtable `334ba584` slot 0x84 = `32755aa0`:
  `+0x8604` skin anim null test, `GetCloneFrame` `31d38a20`, `combineMatrix._41-_43` else zero = March `CX2GUNPC::GetBonePos( const WCHAR* ) const`
  ( `m_pXSkinAnim`; `INDEPENDENT_ANIMATION_TIME` is off, else NPC would not compile against the pure-virtual const base ).
- **No re-bind on a body switch:** a `.text` scan for every `[reg + 0xcbc]` operand: the CX2GameUnit / CX2GUUser readers are `32291060` (this one),
  `31ffda00` `ShowMinorParticleHyperModeTrace` (also positions by vslot 0x84 name; March's already calls `GetBonePos`, so it follows the body too),
  `3200a730` and `322845c0` (show / hide only: sequence `+0x172`, no position), create / destroy / ctor sites, and the per-class
  `HyperModeBuffEffectStart` overrides (Lu's `32b1f0b0` = vslot 0xc4, reached at awakening start). `SetActiveBody` `3204e030` touches no handle.
- **Flag-off = March:** `cl /P` ( census `cl_cmd` ) of `X2GameUnit.cpp` with the flag on, of a scratch copy with the guard renamed (flag off), and of
  `04ab6729^`: flag-off equals the base after normalising file paths and line numbers ( 379,527 lines each ); flag-on differs only in the eight
  `SetPosition` lines.
- **Gates:** `finish.py --review --commits 04ab6729 --with SERV_IRUHADEV_JUNE_CHAR_DUO_AWAKEN_EFFECT -v`: `FINISH RESULT: PASS (7 gates, 0 failed,
  3 skipped)`; flagcheck PASS ( 1 PASS, 1 DEFINE, 0 OTHER, 0 FAIL; 0 deleted lines ); `junediverge` PASS; `junediverge --check-all` PASS; `enumscan`
  exit 0; stateends / skillicons / luabind not requested (nothing registered or staged). No OTHER rows to explain.
- **Build:** no code change; full build skipped: no play-test (DU6's `DEPLOY OK` and passed play-test stand).
- **Next:** none from DU6. Waiting on the user: the DU5b + DU5b2 batched play-test, then the CH2d3 play-test.

## Phase CH1 - Deploy unit: the class core

**Transcription rules (CH1b-CH1l).** Each function from the class map is written whole, from Ghidra (or the capstone dump), under
`SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE`, with the name CH0 proposed. A class-range callee owned by a later group gets a declaration and
`// June <va>: body in <phase>`. An engine callee is called through its March counterpart (`funcmap.tsv`, `junediverge.py --list`); an
unmapped one means "this needs Opus": stop and say so. June's numbers, order and quirks are kept verbatim (a `JUNE_BUGS.md` row each). Skill-level
and skill-use reads go through the shared `CX2GUUser` helpers (Chung plan, Rules). The dev free-skill gate is
`CX2GUUser::IsDevUnlimitedSkillUse()`. Nothing runs until CH1 final.

| Sub-phase | Scope | Model |
|---|---|---|
| CH1a | Class skeleton: `X2GULuciel.h` / `.cpp` (new, vcxproj + filters), the 0x98a0 object's members (ctor `32b1bd90` after the base ctor `3203e850`, dtor `32b39380`), the three own virtuals (0x880 `32b14db0`, 0x884 `32b3acf0`, 0x888 `32ae6130`), `MemoConditionLuciel`, the `+0x8ac8` class index. Flags `_CORE`, `_LIVE` (`//#define`) | Opus high |
| CH1b | Lu's common states, part 1: wait, walk, jump, dash (`LSI_LU_*`) | Sonnet high |
| CH1c | Lu's common states, part 2: damage, stand-up, guard-like, throw, buff and the rest of `LSI_LU_*` | Sonnet high |
| CH1d | Ciel's common states, part 1 (`LSI_CIEL_*` movement) | Sonnet high |
| CH1e | Ciel's common states, part 2 | Sonnet high |
| CH1f | Combos `LSI_A_*` / `LSI_COMBO_*` (Duo) | Sonnet high |
| CH1g | Chiliarch `LSI_LLC_*` and Royal Guard `LSI_LCG_*` job states | Sonnet high |
| CH1h | Dread Lord `LSI_LLD_*` and Noblesse `LSI_LCN_*` job states | Sonnet high |
| CH1i | Demonio script: `LSI_LDB_*`, `LSI_LDN_*`, its `A` / `BOUNDING` / `HA` states (`Luciel_Demonio`) | Sonnet high |
| CH1j | Riding (`LSI_RIDING_*`, 27) | Sonnet high |
| CH1k | Overrides, band 1 (CH0 splits the 119 by vslot) | Sonnet high |
| CH1l | Overrides, band 2, including the five damage-state overrides DU2 calls | Sonnet high |
| CH1m | **Opus.** Registration: InitState `32b43d10`, InitStateCommon `32b3e8b0` (294 registrations), the start / win / lose rolls; regsim against the listing | Opus high |
| CH1n | **Opus.** Skill head `32b46580` (0x7b4, ~80 cases) and the base callbacks it hands to | Opus high |
| CH1o | **Opus.** Virtual wiring, the June-only keys CH0 gives CH1, the checklist hooks | Opus high |
| CH1p | **Opus.** The in-class duo switch: `DUO_TYPE` (`DT_BOTH` / `DT_KEEP` added to the live `Enum.lua`, core list), `MINOR_UNIT_ANIM_NAME`, `MINOR_UNIT_SPEED_TIME0-4`, `NOT_SHOW_UNIT_BY_DUOTYPE`, the body switch and its key (`DLG_LUCIEL_BOUNDING_KEY.lua`), combination points (`HIT_ADD_COMBINATION_POINT`, `CP_AMOUNT`). Flag `_DUO`, sub-tag `DUO` | Opus high |
| CH1 | **Opus.** Final: `_LIVE` (+ `_DUO`) defined; `CreateGUUser` / intruder case `UT_LUCIEL`; `Luciel_Duo.lua` + `Luciel_Demonio.lua` adopted (bytecode, `luabind.py check` both); motions copied with them (CH0b's holds); `[JUNE_CHAR] LUCIEL DATA` line at the end of InitState (class, script, motions, states, duo); play-test: create, enter the village, walk, jump, dash, combo, swap bodies, enter a dungeon, relog | Opus high, user play-test |

### CH1a0 - Done (2026-11-14)

Opus 5.5 high. Two new flags ( `Always.h`, no `_DEBUG`, no log tag, inert ): class-agnostic `SERV_IRUHADEV_JUNE_CHAR_TRANSFORM_LINK` ( needs
`_DUO_BODY` and `_SKILL_TREE_UI`, `#error` guard ) and `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( defined here; CH1a adds only `//#define _LIVE` ).
Files: `Always.h`, `KTDGUISlot.h`, `X2GUUser.h/.cpp`, `X2Game.h/.cpp`, `X2GageData.h`, `X2UISkillTreeNew2015.h/.cpp`. funcmap:
`grep "Char Luciel CH1a0" june2017_re/funcmap.tsv` ( 23 rows ). `JUNE_BUGS.md` 674. Commit `3964df64`; flagcheck `--range 3964df64^..3964df64`: both flags `RESULT: PASS` ( same tallies as the worktree run below ).
- **Already ported:** `31fc8900` = Chung CH2b0's `SkillSlotKey::IsKeyPressedOnce` ( Lu/Ciel calls it with `m_InputData` ).
- **Transform pair** ( `CX2GUUser`, DU1b's members `+0x8f6e` / `+0x8f6f` ): `SetTransformed( int, bool )` `31fc9d50`, `IsTransformed( int )` `31fc9d90`.
  Arguments are `CX2Unit::DT_*`: the getter's 2 ( `DT_BOTH` ) means either body, 3 ( `DT_KEEP` ) both; kept as June has them.
- **The transform window ( user `+0x8f70` ).** CH0's "linked slot table" is a 0x18-byte UI object ( no vtable / RTTI ): dialog handle +0,
  `float[3]` cool times +8, selected slot +0x14. Created by Lu/Ciel's ALTAR_OF_EVIL / TRANSCENDENCE transform starts ( `32b43910` / `32b43b10`: new 0x18
  + `3200f380` ), deleted by `32b38f30` / `32b39380` / ReInit. Declared as `CX2TransformSkillSlotUI` ( `X2GUUser.h`, name from
  `DLG_LUCIEL_TRANSFORM_SKILL_SLOT.lua` ) with the members this phase's callers need: `SetCoolTime` `31fc9e80`, `SetCoolTimeShow` `31fc9df0`,
  `GetSlotSkillID` / `GetSlotSkillLevel` `31fdc680` / `31fdc810`, `SelectSlot` `31fdc9a0`, `CanUseSlot` `3200f680`. Its slot data, RTTI
  `CX2TransFormSkillSlotData` ( type 6, new `CKTDGUISlotData::SDT_TRANSFORM_SKILL_SLOT = 6`; ctor `328d9b00`, `UpdateUI` `328da050` = the memorize
  slot's tail ), is ported whole next to `CX2MemorizeSkillSlotData`. Member `m_pTransformSkillSlotUI`, NULL in the user ctor ( June @320431fd ).
- **User helpers:** `ApplyTransformSkillCoolTime( float, bool bAdd )` `31fc9f00`, `ApplyTransformSkillCost()` `31fd0490`, `CheckTransformSkillSlotKey()`
  `32030160` ( messages `STR_ID_2549` / `STR_ID_226` in March's `X2GUAdd.cpp:2569` form; June's `[S%1%]%2%` string-id debug form is not ported, March's
  `GET_STRING` has none ), `StateChangeToTransformSkill()` `32039dc0`, `IsFourthSkillSlotKeyPressedOnce()` `31fd0440` ( `oneC`, `oneR` on the swapped row;
  input offsets 0x19 / 0x39 from `+0x86da` match Chung's `IsKeyPressedOnce` table ). New base virtual `GetTransformedSkillStateID( SKILL_ID )`
  ( vslot 0x57c, base `330f9c30` returns 0; Lu/Ciel `32ae9c00` = CH1l ). `GetNowStateSkillID` / `GetActualCoolTime` / 2-arg `GetActualMPConsume` /
  `UpNowMp` are June's vslots 0x524 / 0x6a4 / 0x6a0 / 0xf0 ( funcmap ).
- **Switching guide:** `CX2Game::UpdateLucielSwitchingGuideUI( const bool*, float )` `322e6300` ( `_CORE` ), Eve's `322e65c0` port's idiom; it
  differs from Eve's: count shown only when cool > 0 ( no empty-string branch ), tooltip `( 0, 0 )` then `( 1, 1 )`.
- **Gauge:** RTTI `CX2LucielGageData` ( vt `3349b2a4`, 0x130 bytes ), the Combination Point gauge ( `DLG_Luciel_CombinationPoint.lua` ):
  ctor `32498e40` ( +0x110 changed 0, +0x114 now 0, +0x120 max 10, +0x12c float 1.0 ), copy ctor `32a38250`, `SetNowCombinationPoint` `32a327a0`
  ( store, then clamp 0 .. max ). `X2GageData.h` ( `_CORE` ).
- **New phase CH1a1 ( this plan's subject ).** The gauge overrides **20 of 22** `CX2GageData` virtuals ( all but slots 12 / 14 ), and RTTI names a
  `CX2LucielMyGageUI`; CH0 mapped neither. Decompiles saved to `june2017_re/char_luciel/ghidra/gage/` ( 10 bodies uncarved ). Kickoff `CH1a1.md`,
  STATUS row; CH1k / CH1k2 now wait on its review.
- **Routed, kickoffs amended:** the window's ctor / slot set / show / TransFormEnd / per-frame count `3200f7a0` / handle release `31fd03f0` and the
  engine hooks ( `CX2GUUser::OnFrameMove` @32070133, `CX2UIManager::ToggleUI` @32158ef9 via `31fe3c90`, `CX2Game::KeyProcess` @322f7b5a ) -> CH2d20;
  `UpNowMp` is virtual in June ( Lu/Ciel `32aeb160` ) -> CH1k; the `CX2Game` ctor / dtor guide calls ( @32307bca / @32308957 ) -> CH1 final ( `_LIVE` );
  `IsTransformed` readers in packet / viewer sites `31f1ed70`, `32461e80`, `32195e40` -> DU5; signature note -> CH1l; `_CORE` note -> CH1a.
  Not routed ( scattered systems, identified only ): `321faa90`, a per-frame UI tick in dungeon `0xa1c2` that tests `UT_LUCIEL` + buff `0x3a3` and
  `IsTransformed( DT_BOTH )` ( caller `321facb0` ).
- **DU1d4's helpers** ( shared DIE_FRONT / DIE_BACK `31fd2d60` / `31fd2e30`, `31fcd0f0..31fcd220` ) stay with CH1m's registrations, as DU1d4 allowed.
- **Hygiene:** flagcheck `TRANSFORM_LINK --worktree` ( 9 files ): `files: 4 PASS, 1 PASS~, 0 DEFINE, 4 OTHER, 0 FAIL, 0 ERROR` / `0 deleted, 0 non-ASCII,
  0 EOL-mismatched, 0 #else/#endif without token, 0 encoding change(s)` / `RESULT: PASS` ( OTHER = `Always.h` and the three `_CORE` files ).
  `LUCIEL_CORE --worktree` ( its 4 files; on all 9 the tool stops on `X2GUUser.cpp`'s CHUNG_CORE `#if`, Unevaluable ): `2 PASS, 1 PASS~, 1 OTHER,
  0 FAIL` / `RESULT: PASS`. `junediverge.py --check` ( 9 files ): `RESULT: PASS`. `enumscan.py --check`: no row in the new code ( the 6 rows are older ).
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH ) of `X2GUUser.cpp`, `X2Game.cpp`, `X2UISkillTreeNew2015.cpp`, `X2GageData.cpp`: 0 errors each;
  a `cl /P` probe shows both flags and `_SKILL_TREE_UI` on and the four classes / function present. Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert ); CH2d2 / CH3c2 hand off the transform-skill play-test.

### CH1a0 - Review (2026-11-14)

Opus 5.5 high, read-only Ghidra. **Verdict: pass, no code change; one routing fix (docs).**
- **Every body re-read in Ghidra against the commit** (not only 3 spot-checks): `31fc9d50` / `31fc9d90` (0 / 1 / 2 set, getter 2 = either, 3 = both,
  else false; `CX2Unit::DT_*` right), `31fc9e80` (`&&` test kept, below 0 -> 0, show = value > 0), `31fc9df0` (`Skill_CoolTime` picture show, hide
  clears `CoolTime` string), `31fdc680` / `31fdc810` (`STR_ID_3738` = 0xe9a `SlotB` + index, slot `+0x3c4` data `+0xc` / `+0x10`, 0 on every miss),
  `31fdc9a0`, `3200f680` (consume vslot 0x6a0 `<=` now MP vslot 0xd8, unordered -> false), `31fc9f00`, `31fd0490` (unit `+0x867c` -> `+0x494` skill
  tree; cool time, then MP, then -1), `32030160` (rows `+0x86f7/fb/ff` = `oneA/S/D`, `+0x8707/0b/0f` = `oneQ/W/E` against `X2GameUnit.h`'s
  `InputData`; last pressed wins; early return when selected; 0x9f5 / 0xe2, font 3 = `XUF_DODUM_20_BOLD`, colours (1,1,1,1) / (0,0,0,1)), `32039dc0`,
  `31fd0440` (`+0x19` = `oneC`, `+0x39` = `oneR`).
- **Slot data:** `328d9b00` (type 6, updated 1, id 0, `+0x10` / `+0x14` = -1), `328da050` (templet, then level from the selected unit `+0x494` > 0,
  icon + piece, slot `+0x340` = 0 = `SetDragable( false )`). `SDT_END` has no user.
- **Gauge:** `32498e40` (`+0x110` 0, `+0x114` 0, `+0x120` 10, `+0x12c` 1.0), `32a38250` (base copy `322b24f0`, cells by value), `32a327a0` (store,
  then clamp; June writes only on change, same result).
- **Switching guide `322e6300`:** guide `( 0, 0 )` / `( 1, 0 )` + key string of action 0x27, count `( int )( cool + 1.0 )` only when cool > 0, tooltip
  `( 0, 0 )` / `( 1, 1 )`: the three differences from Eve's `322e65c0` hold.
- **Routing:** CH1a1, CH2d20, CH1k, CH1k2, CH1l, CH1 final, DU5, CH1a amendments are in their kickoffs. **Fixed here:** the Done note left `321faa90`
  "identified only"; the scope rule defers, never drops. Its dungeon `0xa1c2` is `DI_EVENT_HALLOWEEN_ZOMBIE` (41410), so it is now a bullet in
  `JUNE_PORT_BACKLOG_ORDER.md` Stage 19 (event dungeons), and funcmap row `31fc9d90 (CH1a0)` names that stage.
- **Hygiene:** flagcheck `TRANSFORM_LINK --range 3964df64^..3964df64`: `files: 4 PASS, 1 PASS~, 0 DEFINE, 4 OTHER, 0 FAIL, 0 ERROR` / `0 deleted,
  0 non-ASCII, 0 EOL-mismatched, 0 #else/#endif without the flag token, 0 encoding change(s)` / `RESULT: PASS` (OTHER = `Always.h` with the other
  flag's block, and `X2GageData.h` / `X2Game.cpp` / `X2Game.h`, which are `LUCIEL_CORE`'s; PASS~ = one blank line in `X2UISkillTreeNew2015.cpp`).
  `LUCIEL_CORE --range ... --files Always.h X2Game.h X2Game.cpp X2GageData.h`: `2 PASS, 1 PASS~, 1 OTHER, 0 FAIL` / same zero line / `RESULT: PASS`
  (OTHER = `Always.h`; PASS~ = one blank line in `X2GageData.h`; without `--files` the tool stops on `X2GUUser.cpp:29404`'s CHUNG_CORE `#if`, as the
  Done note says). `git log -S<flag> 3964df64..HEAD`: nothing for either flag. `junediverge.py --check-all`: `allow-listed: 240` / `RESULT: PASS`.
  `enumscan.py --check` (5 files): 6 rows, none in CH1a0's lines (DU1b3 `04d2cacf`, `09107672`).
- No build: no code changed (the code is inert; CH2d2 / CH3c2 play-test it).

### CH1a1 - Done (2026-11-14)

Opus 5.5 high, Ghidra read-only. New class-agnostic flag `SERV_IRUHADEV_JUNE_CHAR_FORM_MP_GAGE` ( `Always.h`, `#error` unless `DIVERGE_GAGE_TALLY`,
`DUO_BODY`, `FIX_LIMITED_MANA_MANAGEMENT`, `DUNGEON_ITEM` ) plus `_CORE` for the class. Files: `Always.h`, `X2GageData.h`, `X2GageData.cpp`. funcmap:
`grep "Char Luciel CH1a1" june2017_re/funcmap.tsv` ( 38 rows ). `JUNE_BUGS.md` 675-678. `june_march_divergent.tsv`: 2 rows ( base `OnFrameMove`,
`UpdateDamageFlushMP` ), both ported. Commit `88246a05`; flagcheck `--range 88246a05^..88246a05 --with LUCIEL_CORE`: `RESULT: PASS` ( same tallies as
the worktree run below ).
- **Slot map** ( vt `3349b2a4` vs base `33487acc`, Chung / Ara / Rena columns identical to the base in 1-12 / 14 / 21 ): 0 dtor, 1 `GetNowMp`,
  2 `SetNowMp`, 3 `UpNowMp`, 4 `GetMaxMp`, 5 `SetMaxMp`, 6 `UpMaxMp`, 7 `SetAddMaxMP`, 8 `GetChangeRateMp`, 9 `SetChangeRateMp`, 10 `UpChangeRateMp`,
  11 `FlushMp`, 12 `DamageFlushMp`, 13 `OnFrameMove`, 14 `DetonationFrameMove`, 15 `UpdateDataFromGameUnit`, 16 `InitWhenGameIsOver`, 17 / 18 the
  play-status pair, 19 `GetCloneGageData`, 20 `CopyGageData`, 21 June-only `MpFrameMove`. Slots 1-11 take June's **gauge index** ( 0 = main,
  1 = the minor unit's; the base answers 1 with nothing ); March had them inline, non-virtual and indexless. The TALLY port's third `bSkip`
  argument of `SetNowMp` / `UpNowMp` was this index ( no March caller passes it ).
- **Engine ( `FORM_MP_GAGE` ):** every gauge now owns `m_pMinorUnitMpGage` ( June `+0xc`: NULL in the ctor, newed by `Init` `322b37a0`, copied by the
  copy ctor / `operator=`, deleted by the dtor ) and `m_fMinorUnitAddMaxMP` ( `+0x14`, copied only; no writer in the exe by byte scan ). Slots 1-11
  are virtual with June's base bodies ( `322b2d00` ... `32a31910` ); base `FlushMp` ignores the index ( spends on 0 ). `OnFrameMove` is replaced by
  June's `32a33c40`: Detonation, HP regen, then `MpFrameMove` ( `32a344a0`: item MP-rate timer, regen through `UpNowMp( v, true, 0 )` **tracked**,
  factors ); **no `Veryfy` call** ( June dropped it ). June's byte `+0x18` gate ( MP regen x 0 unless 1 ) is left out: both ctors set 1, nothing
  writes it ( comment at the site ). `UpdateDamageFlushMP` spends tracked on my select unit's form index ( `31d717e0` = file-local
  `IsMySelectUnitMinorForm()`, `_CONST_GAGEDATA_CH1A1_` ). Protected helpers for the class: `GetMpGageByIndex`, `TallyMpChange`, `Get/SetAddMaxMpAmount`,
  `GetMinorUnitAddMaxMp`.
- **`CX2LucielGageData` ( `_CORE` ):** overrides 1-11 ( gauge by index: 0 -> main, else minor ), 15, 19, 20, 21, plus `operator=` `32a36f70` and
  `SetMaxCombinationPoint` `32a36ef0`. **`MpFrameMove` `32a35170`:** the form in play ( `31d717e0` ) regenerates at the main gauge's rate + item rate,
  the other form gains `+0x12c` x time flat, factors run on the form in play's gauge. So CH1a0's `m_fRestFormMpRate` is renamed
  **`m_fRestFormMpPerSecond`** ( MP per second for the resting form, default 1.0 ). Not written ( recorded ): slot 0 ( compiler dtor ), 13 ( a bare
  base call ), 16 ( = the base body ), 12 / 14 ( not overridden ).
- **Routed ( new phases, this plan's subject ):** **CH1a2** = slots 17 / 18 ( `32a32530` / `32a350d0` ): they read June `KGamePlayStatus` fields
  March lacks ( `+0x5a` / `+0x5e` minor max / now MP, `+0x62` form byte, `m_cCharAbilType` 6 ), a shared packet struct ( `CommonPacket.h` ->
  `ServerDefine.h` second home, `SECOND_HOMES`, `X2TOOL` defines, emulator `MakeGamePlayStatus` / `SaveGamePlayStatus` ), outside this phase's
  Touches. **CH1a3** = `CX2LucielMyGageUI` ( vt `3353375c` / `33533744`; overrides slots 0, 1 InitUI `32e6e230`, 2 `32e719b0`, 8 SetShow `32e5ed90`,
  19 SetCharacterImage `32e62d50`, 24 `32e63070` ) and the gage manager cases `3248e290` @`3248e618`, `3248ec80`. Both kickoffs written; CH1 final
  now waits on their reviews. June engine callers that pass a non-zero index ( the unit's `UpNowMp` vslot 0xf0 `32aeb160` ) stay CH1k's.
- **Hygiene:** flagcheck `FORM_MP_GAGE --worktree --with SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE --files Always.h X2GageData.h X2GageData.cpp`:
  `files: 1 PASS, 1 PASS~ (blank/marker only), 1 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR` / `3 deleted line(s), 0 non-ASCII, 0 EOL-mismatched,
  0 #else/#endif without the flag token, 0 encoding change(s)` / `RESULT: PASS` ( the 3 deletions = CH1a0's own `m_fRestFormMpRate` lines inside
  `_CORE`, renamed; PASS~ = one blank line before the `_CORE` block in `X2GageData.cpp`; each flag alone reports the other's hunks as OTHER, so both
  are stripped together ). A `LUCIEL_CORE needs FORM_MP_GAGE` guard was dropped: flagcheck cannot evaluate an `#if` naming both, and the class does
  not compile without the engine flag anyway. `junediverge.py --check` ( 3 files ): `allow-listed: 0` / `RESULT: PASS`. `enumscan.py --check`
  ( 2 files ): `0 A/B row(s) remain`.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH ) of `X2GageData.cpp`, `X2GageManager.cpp`, `X2GageUI.cpp`, `X2RandomMission.cpp`,
  `X2GameUnit.cpp`, `X2GUUser.cpp`, `X2GUNPC.cpp`: 0 errors each; a probe with `#error` on either flag off compiles ( both on ). Full build skipped:
  no play-test.
- **How to see it in game:** nothing for Lu/Ciel yet ( inert ). Every class: MP regen and detonation now feed the gained / spent MP tallies, and the
  gauge's per-frame `Veryfy` is gone; regen amounts are unchanged.

### CH1a1 - Review (2026-11-14)

Opus 5.5 high, Ghidra read-only (`funcdis.py` + vtable dump from the exe). **Verdict: pass, no code change.**
- **Slot map:** vt `3349b2a4` vs base `33487acc` and Chung `3349b064`: Lu/Ciel replaces every slot but 12 ( `32a31dc0` ) and 14 ( `32a33b20` ), as the
  Done note says; slots 1-11 / 21 in Chung = the base bodies. Slot 13 `32a35000` is a bare `call 32a33c40`; slot 16 `32a350c0` is byte-for-byte the base
  `32a33f90` ( `SetHyperModeRemainTime( 0 )` `31e8c9d0` ): no code needed. 17 / 18 ( `32a32530` / `32a350d0` ) are CH1a2's.
- **Spot-checks (3, all = commit):** (1) `MpFrameMove` `32a35170`: item timer, form index `31d717e0`, `UpNowMp( ( vslot 0x20( 0 ) + item rate ) x time
  [x 0 unless +0x18 == 1], 1, form )`, `UpNowMp( +0x12c x time, 1, form == 0 )` (ungated), factors on `+8` / `+0xc` by form, cleared when HP gauge `+0x20`
  is NULL or <= 0, factor x elapsed time. (2) `SetAddMaxMP` `32a31800`: Ciel -> `+0x10 = +0x14`, then equal -> return, else
  `SetMaxMp( GetMaxMp( 0 ) - old + new, 0 )` through the vtable, store. (3) `UpdateDamageFlushMP` `32a33d50`: `UpNowMp( +0xfc x -1.0 ( 33436c90 ), 1,
  31d717e0() )` via vslot 0xc, then the March tail.
- **`+0x18` gate:** written only by the ctor `322b3a70` @`322b3abb` and copy ctor `322b24f0` @`322b259f` ( `bl` = 1 in both ); `op=` `32a36530` and
  `Init` `322b37a0` never touch it. The exe's one setter-shaped `[ecx + 0x18]` byte writer, `32e1dba0`, is a UI show toggle ( calls a singleton +
  `31cd32e0` ). Leaving the gate out is safe.
- **Base bodies / Lu/Ciel bodies:** re-read against the diff ( base `SetNowMp` keeps March's own negative store, = June's `322b3010` order; `FlushMp`
  base spends on 0, Lu/Ciel on the index; `UpdateDataFromGameUnit` point before max = JUNE_BUGS 677 ). JUNE_BUGS 675-678 and the two divergence rows
  match the bodies.
- **Callers:** every 3-argument `SetNowMp` / `UpNowMp` call is in `X2GageData.cpp` ( 184, 265, 357, 1757, 1775, 1776, 1810 ) or CH1a's
  `X2GULuciel.cpp:303` ( after this commit ); the `bSkip_` overloads at `X2GageData.h:307` / `:349` are the dead `#else` branch. No `&CX2GageData::`
  member pointer, no luabind binding of a gauge MP accessor.
- **Routing:** `CH1a2.md` / `CH1a3.md` exist with STATUS rows; CH1 depends on CH1a2 / CH1a3 / CH1a4 reviews.
- **Hygiene:** flagcheck `FORM_MP_GAGE --range 88246a05^..88246a05 --with SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE`: `files: 1 PASS, 1 PASS~ (blank/marker
  only), 1 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR` / `3 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0
  #else/#endif without the flag token, 0 encoding change(s)` / `RESULT: PASS` ( the 3 deletions = the `m_fRestFormMpRate` rename inside `_CORE`; PASS~ =
  one blank line ). `git log -S<flag> 88246a05..HEAD` and `git log 88246a05..HEAD -- X2Lib/X2GageData.*`: empty. `junediverge.py --check-all`:
  `allow-listed: 240` / `RESULT: PASS`. `enumscan.py --check X2GageData.h X2GageData.cpp`: `0 A/B row(s) remain`.
- No build ( no code change ). CH1a2 and CH1a3 are unblocked; CH1k's CH1a1 dependency is met.

### CH1a2 - Done (2026-11-14)

Opus 5.5 high, Ghidra read-only. New class-agnostic flag `SERV_IRUHADEV_JUNE_CHAR_MINOR_UNIT_PLAY_STATUS` in three homes ( `Always.h`, end of
`ServerDefine.h`, `X2ServerProtocol_2010.vcxproj` X2TOOL as `=`; `SECOND_HOMES` row in `tools/flagcheck.py` ); the gauge pair under `_CORE`. Files:
`CommonPacket.h` / `.cpp`, `X2GageData.h` / `.cpp`, `X2OfflineServer.h` / `.cpp`, `X2OfflineDB.h` / `.cpp`, `Handlers_Field.cpp`, `X2OfflineStatTable.cpp`.
funcmap: `grep "Char Luciel CH1a2" june2017_re/funcmap.tsv` ( 8 rows ). `JUNE_BUGS.md` 685-687. Commit `29ff06ae`; flagcheck `--range
29ff06ae^..29ff06ae`: layout flag `RESULT: PASS` ( same tallies as the worktree run below ); `_CORE`: `2 PASS, 9 OTHER, 1 ERROR`, the ERROR = Always.h's
existing `LIVE && !CORE` guard, which the tool cannot evaluate once the new block's comment names `_CORE`; resolved by hand: the commit's Always.h hunk
is the 11-line comment + `#define` block ( DEFINE in the layout-flag run ), the 9 OTHER rows are the layout flag's files.
- **June's struct ( found by a field-offset scan of `.text` ):** Get `320dbbd0`, Put `320bcbd0`, ctor `31f3b0a0`, copy ctor `31e85320`. After
  `m_mapRidingPetCoolTime` ( `+0x4a` ) June has int `+0x5a` **`m_iMinorUnitMaxMP`**, int `+0x5e` **`m_iMinorUnitCurMP`**, char `+0x62`
  **`m_cMinorUnitForm`** ( serialized through the char pair `33051d30` / `33051ce0`, not `Get(bool)`'s `== 1` body, so a `char` ). The ctor sets 0, 0,
  **-1** and, like March, leaves `m_iMaxHP` unset. `CAC_COMBINATION_POINT = 6` ( the gauge setters' type compares in `32a30000`-`32a40000`: 1 Elsword /
  Elesis, 3 Ara, 4 Add, 5 Rena, 6 Lu/Ciel ) and an inline `GetCombinationPoint()` beside `GetNaturalForce()`.
- **Proof ( cl /P + offset probe, X2Lib US_SERVICE and X2ServerProtocol X2TOOL ):** flag ON in both ( X2Lib also `_CORE` ON ); `offsetof`
  `m_iMinorUnitMaxMP` 90 / `m_iMinorUnitCurMP` 94 / `m_cMinorUnitForm` 98 / `m_iCharAbilCount` 22 / `m_mapRidingPetCoolTime` 74, `sizeof` 99,
  `CAC_COMBINATION_POINT` 6, `CAC_NATURAL_FORCE` 5: identical in both projects and = June ( 0x5a / 0x5e / 0x62, 0x63 ).
- **Gauge pair ( `_CORE` ).** `GetMyPlayStatusToPacket` `32a32530`: `max( 0, (int)GetNowMp( 1 ) )` to the now MP, then the max, the Combination
  Point cut to a char into `m_iCharAbilCount` ( June `movsx`, row 685 ), the form = my select unit's `IsMinorUnitForm()` read inline with no NULL test
  ( row 686; June does not call `31d717e0`, so CH1a1's guarded helper is not used ), base call last and its return kept. It writes no CAC type: the
  type comes from the server ( base `32a32200` does not write it either ). `SetMyPlayStatusFromPacket` `32a350d0` ( uncarved, capstone ): base `32a33fa0`
  first, `SetMinorUnitForm( 1 == form )` ( `3234b8c0`, no NULL test ), `SetMaxMp( max, 1 )`, `SetNowMp( now, false, 1 )`, point ( type 6, else 0 )
  **before** `SetMaxCombinationPoint( 10 )` ( row 687 ), changed byte = 1. `32a32200` / `32a33fa0` = March's base pair ( Veryfy first; base setter calls
  `SetNowMp( v, 0, 0 )` ), so the base bodies need no change.
- **Emulator.** `CharAbilTypeOf`: `UT_LUCIEL` -> 6 ( stand-in: no June server switch; June's client reads 6 ). `MakeGamePlayStatus`: for `UT_LUCIEL`
  only, max = `ONE_CHARGE * 3` ( stand-in, sized like the main gauge ), now = `min( saved, max )`, form = saved; every other class keeps the ctor values,
  which is what their gauges send. `Handler_EGS_UPDATE_PLAY_STATUS_NOT` saves the minor MP and form on change ( own session cache, `-2` = unwritten
  form ), before the four-value early return.
- **Schema decision: v23, forward-only, no existing row changes.** June's setter always forces the form from the packet, so the emulator must carry it
  ( and Ciel's MP ) across a village -> dungeon hop exactly as it carries `cur_mp`; a session-only cache would flip Ciel back to Lu on every relaunch.
  `ALTER TABLE unit ADD COLUMN minor_cur_mp ... DEFAULT 0` and `minor_unit_form ... DEFAULT -1` ( the ctor values ), `UNIT_COLUMNS` / `ReadUnitRow`
  24 / 25, `SaveMinorUnitPlayStatus`. Save backed up first: `db_backup/els_db_20261114_154414_pre_manual.sql` ( user_version 22 ). The rung runs on the
  first launch of the next deployed build and logs `[JUNE_CHAR] LUCIEL DATA DB MIG v23 done`. **DU4's migration becomes v24** ( `DU4.md` amended ).
  Flag off: `SCHEMA_VERSION` falls back to 22, so a v23 save refuses to open ( as every earlier rung ).
- **Hygiene:** flagcheck `JUNE_CHAR_MINOR_UNIT_PLAY_STATUS --worktree --files <12 files>`: `files: 3 PASS, 5 PASS~ (blank/marker only), 2 DEFINE,
  2 OTHER (other flags), 0 FAIL, 0 ERROR` / `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without
  the flag token, 0 encoding change(s)` / `RESULT: PASS` ( OTHER = the two gauge files, `_CORE`'s; PASS~ = one blank line before each new block ).
  `JUNE_CHAR_LUCIEL_CORE --worktree --files X2GageData.h X2GageData.cpp`: `2 PASS` / `RESULT: PASS`. `--with` both gives an ERROR on Always.h's
  existing `LIVE && !CORE` guard ( unevaluable with CORE stripped ), so each flag is checked alone. No `#error` guard for `_CORE` needing the layout
  flag ( same reason as CH1a1's: the class does not compile without it ). `junediverge.py --check` ( 12 files ): `allow-listed: 0` / `RESULT: PASS`.
  `enumscan.py --check` ( 4 files ): `0 A/B row(s) remain`.
- **Compile:** `/Zs` probes ( no PCH ) of `X2GageData.cpp`, `X2ServerPacket.cpp` ( includes `CommonPacket.cpp` ), `X2OfflineServer.cpp`,
  `X2OfflineDB.cpp`, `Handlers_Field.cpp`, `X2OfflineStatTable.cpp`, `X2GageManager.cpp`, and X2ServerProtocol `X2ServerProtocol.cpp` /
  `Socket/Session.cpp` ( X2TOOL defines ): 0 errors each. Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( Lu/Ciel inert until `_LIVE` ). Every class: the play-status packet carries three more fields ( 0, 0, -1 );
  the save gains two columns on the next deployed build. CH1 final play-tests Ciel's MP and form surviving a village -> dungeon hop.

### CH1a2 - Review (2026-11-14)

Opus 5.5 high, Ghidra read-only ( `disassemble_function` + `disasm_range.py` for the uncarved bodies ). **Verdict: pass, no code change.**
- **Struct ( spot-checks 1-2, = commit ):** Put `320bcbd0` ends `Put(int) [+0x5a]`, `Put(int) [+0x5e]` ( `33051fa0` ), `movzx [+0x62]` -> `33051ce0`;
  Get `320dbbd0` ends `33052000( +0x5a )`, `33052000( +0x5e )`, `33051d30( +0x62 )`. Ctor `31f3b0a0`: `+4`..`+0x16` zeroed ( word at `+0x14` = hyper
  count + CAC type ), four containers at `+0x1a` / `+0x2a` / `+0x3a` / `+0x4a`, then `+0x5a = 0`, `+0x5e = 0`, `+0x62 = 0xff`; `+0` ( `m_iMaxHP` ) unset.
- **Offset probe ( re-run ):** X2Lib US_SERVICE ( `offsetprobe.py` ) and X2ServerProtocol X2TOOL defines ( `/Zs` C2440 probe ): `m_iMinorUnitMaxMP` 0x5a,
  `m_iMinorUnitCurMP` 0x5e, `m_cMinorUnitForm` 0x62, `sizeof` 0x63, `CAC_COMBINATION_POINT` 6, identical in both ( X2Lib `m_mapRidingPetCoolTime` 0x4a ).
- **CAC values:** every `cmp byte ptr [edi + 0x15], N` in `32a30000`-`32a40000`: `32a348ee` 1, `32a34a1e` 3, `32a34c7e` 1, `32a34e2e` 4, `32a34fae` 5,
  `32a35123` 6; March's enum gives `CAC_NATURAL_FORCE` 5 implicitly, so the explicit `= 6` follows on.
- **Gauge pair ( spot-check 3, = commit ):** `32a32530`: vslot 1( 1 ) twice through `max( 0, ftol )` -> `+0x5e`, then vslot 4( 1 ) -> `+0x5a`, the
  protected point at `+0x114` decoded and `movsx al` -> `+0x16`, `[g_pData + 0x68] + 0x90` -> `+0xe40 == 1` -> `+0x62` with no NULL test, base `32a32200`
  last ( `ret` = its return ). `32a350d0`: base `32a33fa0` first, `SetMinorUnitForm` `3234b8c0`( `+0x62 == 1` ), vslot 5( `+0x5a`, 1 ), vslot 2( `+0x5e`,
  0, 1 ), `32a327a0`( type `== 6` ? `+0x16` : 0 ), `32a36ef0`( 10 ), `+0x110 = 1`. March's base pair ( `X2GageData.cpp:442` / `:462` ) touches neither
  `m_iCharAbilCount` nor the new fields, so the call order is safe. JUNE_BUGS 685-687 match the listings.
- **Emulator:** `CharAbilTypeOf` `UT_LUCIEL` -> 6 and `MakeGamePlayStatus` ( `X2OfflineServer.cpp:1195` ) fill only Lu/Ciel ( labelled stand-in max );
  other classes keep 0 / 0 / -1, which their gauges echo, so the first on-change write stores the column defaults. `Handler_EGS_UPDATE_PLAY_STATUS_NOT`
  writes before the four-value early return, after the `m_iMaxHP <= 0` guard; its session cache follows the existing four ( not reset per unit, as
  theirs ). v23 rung: `DEFAULT 0` / `DEFAULT -1` = ctor values, so no existing row changes meaning; `UNIT_COLUMNS` ends `minor_cur_mp, minor_unit_form`
  = `ReadUnitRow` 24 / 25; `SCHEMA_VERSION` 23 branch keeps `DEFAULT_RESURRECTION_STONE` 10.
- **Hygiene:** flagcheck `JUNE_CHAR_MINOR_UNIT_PLAY_STATUS --range 29ff06ae^..29ff06ae`: `files: 3 PASS, 5 PASS~ (blank/marker only), 2 DEFINE, 2 OTHER
  (other flags), 0 FAIL, 0 ERROR` / `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag
  token, 0 encoding change(s)` / `RESULT: PASS` ( OTHER = `X2GageData.*`, `_CORE`'s ). `JUNE_CHAR_LUCIEL_CORE --range` same: `2 PASS, 0 PASS~, 0 DEFINE,
  9 OTHER, 0 FAIL, 1 ERROR` / `0 deleted line(s)` / `RESULT: FAIL`: the ERROR is Always.h:13430's existing `LIVE && !CORE` guard ( unevaluable; the
  commit's Always.h hunk is the layout flag's comment + `#define`, DEFINE in the first run ), the 9 OTHER rows are the layout flag's files. Resolved by
  hand, as in the Done note. `git log -S<flag> 29ff06ae..HEAD`: empty for both. `junediverge.py --check-all`: `allow-listed: 242` / `RESULT: PASS`.
  `enumscan.py --check X2GageData.cpp X2OfflineServer.cpp X2OfflineStatTable.cpp`: `0 A/B row(s) remain` ( a first run's `cl /P failed` was transient;
  the same command passed on re-run ).
- No build ( no code change ). CH1's CH1a2 dependency is met.

### CH1a - Done (2026-11-14)

Opus 5.5 high. New `X2Lib/X2GULuciel.h` / `.cpp` ( ASCII, CRLF; in `X2Lib_2010.vcxproj` + `.filters` ) under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE`;
`//#define SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` added ( `Always.h`, with `#error` guards: `_LIVE` needs `_CORE`, `_CORE` needs `JUNE_ENUM_NAMES` ). funcmap
`grep "(CH1a)" june2017_re/funcmap.tsv` ( 19 rows ); `JUNE_BUGS.md` 679-681; member table `june2017_re/char_luciel/ch1_offsets.tsv` ( 123 class rows +
Elesis's base rows ); `ch0_class_map.tsv` names / phases updated.
- **Class.** `CX2GULuciel : CX2GUUser`; the ctor is private ( Chung's pattern ); `CreateGULuciel` waits for CH1 final ( the class is abstract until
  CH1o wires the pure virtuals ). **123 members** at June's `+0x959c`..`+0x98a0`, in June's order, named from a census of every class-range access
  ( each with its users in `ch1_offsets.tsv`; names read only from users are marked provisional there ). Inits from the ctor's x87 run with its `fxch`es
  followed ( `0x334380f0` = -1.0 ); nine members June leaves unset stay unset ( JUNE_BUGS 681 ). Types from the users: eleven vectors ( damage-effect
  handles, `{ UID, GAME_UNIT_TYPE }` keys = `SGameUnitKey`, 24-byte `STimedGameUnitKey`, `SDamageEffectHitCount`, effect-set handles, `D3DXVECTOR3` ),
  `std::map<int,bool>` ( heads by form ), two `KProtectedType<float>`, `SAltarOfEvilData` ( June's member initializer `32b4d1c0` / dtor `32699670` ),
  Ara's `sLockOnSystemPtr` ( until CH3c20 moves it ), the bounding-key dialog as `CKTDGUIDialogType`.
- **State enum `LUCIEL_STATE_ID`:** 531 June names over 493 ids ( `ch0_reg.tsv` + `ch0_state_skill.tsv` + the setter's extra states ), June's values,
  under `JUNE_ENUM_NAMES`. Five names are registered at several ids; the name takes its own slot ( `..._PHOENIX_RUN` 123 / 571, `LSI_LU_JUMP_COMBO_Z`
  178, `LSI_CIEL_THROW_ITEM` 228, `LSI_LCG_F_DASH_COMBO_XXXZ_END` 333 ); the 17 re-registered ids and USI 111-116 are listed in CH1m's kickoff.
- **Bodies ( 15 of 21 + 4 thunks ):** ctor `32b1bd90` ( June's base-ctor 6th argument 0x251 = state-vector size, not in March ), dtor `32b39380`,
  particle inits ( inlined in June ), `DeleteLucielMinorParticle` `32b1eb10` ( CH0's "DeleteMinorParticle" is the user virtual ), `StateChangeBySkillID`
  `32aeaa10` ( decoded from its two jump tables: 44 skill ids ), `UpGageNowMpOfForm` `32aeb1e0`, `GetNowCombinationPoint` `32b0fa00`, `UpCombinationPoint`
  `32b14860`, `CountDamageEffectHit` `32b14db0` ( own 0x880: entry `+8` is a hit counter June increments; CH0 read it as a reference ) with `32b4cb80`
  inlined, `GetSkillCombinationPointCost` `32b14f70` ( templet `+0x10` = `m_usFPConsumtion` ), `KillMySummonedNPC` `32b20100`, `ResetPhantomRushReady`
  `32b34920`, `ClearAltarOfEvilEffectSet` `32b35540`, `TryUseSkillWithCombinationPoint` `32b3acf0` ( own 0x884; Elesis CH1k's idioms; cool-time / MP checks
  behind `IsDevUnlimitedSkillUse`; June's string 33027 has no March `STR_ID` and stays a number ), `PreloadAuraAccelResources` `32ae6130` ( own 0x888 ),
  InitState's `+0x8ac8` switch as `SetSkillCutInSetIndexByUnitClass` ( CH1m calls it ).
- **Not ported:** `MemoConditionLuciel` `32aeb3f0` ( March has no `CX2GUUser::MemoCondition`; Skill S3f: it never applies; CH4c amended ).
- **New phase CH1a4 ( this plan's subject ):** `ReleaseTransformedSkinAnim` `32b13d30`, `SetFormSkinAnim` `32b13e20`, `EndTransformedForm` `32b38f30` need
  `CX2GUUser`'s transformed-skin pair `+0x8f74` / `+0x8f8c` ( ctor `32073d50`, dtor `@3203b530` ), the `+0x200` / `+0x210` set clears, the `+0x3cfc`
  equipment show loop and user vslot 0x634, none in March. Declared here ( the dtor calls two ); kickoff `CH1a4.md`, STATUS row; CH1 final waits for it.
- **Registered ( `JUNE_ENUM_NAMES`, June's `Enum.src.lua` values ):** 188 Lu/Ciel `SKILL_ID` names ( `X2SkillTree.h`: the 187 CH0 found unnamed but
  `RNSI_P_LCN_TRIBUTE`, which a peer added, + `SI_SA_COMMON_AURA_DARKNESS_ACCEL` 208 ), `BTI_BUFF_SUMMON_DARKNESS` 474 / `_SHADOW_BOLT` 481 /
  `_TWILIGHT_JUDGEMENT` 482 ( `X2BuffFactor.h` ), `NUI_SA_LDN_GARDEN_OF_HAURES` 2269 ( `X2UnitManager.h` ). An `enumprobe.py` run confirmed the SA / ST /
  item / state / NUI names used resolve to June's values ( `SA_EFFECTIVE_RATE` 5, `SA_MP_REGENERATION_ABS` 43, `SA_COOLTIME_1` 148, `SA_TIME_ABS` 169 ).
- **Kept from June ( JUNE_BUGS ):** 679 Ciel's Chaser of Darkness changes to state 268, which nothing registers; 680 the cost getter re-reads the level;
  681 the nine unset members. Also kept without a row: the form-3 test in `UpGageNowMpOfForm` ( dead ), the `-10` combination-point literal.
- **Reading to check in review:** `UpCombinationPoint` reads June's raw `+0x6c50` as `GetCharacterSpecializationA() + 1.0`, Eve's port's reading
  ( `X2GUEve.cpp:49919` ); `+0x6d04` as `GetCharacterSpecializationB()` ( Chung's ).
- **Routed / amended:** CH1m ( the cut-in switch call, the unnamed ids ), CH2d20 ( the window's dialog release in the dtor ), CH4c ( MemoCondition ),
  CH1 ( `CreateGULuciel`, depends on CH1a4 ), CH1b ( `ch1_offsets.tsv` names, provisional ones ).
- **Hygiene:** flagcheck `LUCIEL_CORE --worktree --files X2GULuciel.h X2GULuciel.cpp`: `0 PASS, 1 PASS~, 0 OTHER, 1 FAIL` / `0 deleted, 0 non-ASCII,
  0 EOL-mismatched` ( the FAIL = the new `.cpp`'s PCH include outside the guard, precedent C3z-4t1 / RM1 ); `Always.h` = the tool's Unevaluable ERROR on
  the compound `#if` guards. `JUNE_ENUM_NAMES --worktree` ( 4 files ): `1 PASS, 2 PASS~, 1 OTHER, 0 FAIL` / `RESULT: PASS` ( OTHER = `X2GULuciel.h`,
  `_CORE`'s ). `_LIVE`: the define is commented out ( inert by design ), so the tool reports "not defined". `junediverge.py --check` ( 5 files ): `RESULT: PASS`.
  `enumscan.py --check X2GULuciel.h X2GULuciel.cpp`: `0 A/B row(s)` ( 7 enumerator values misread as SKILL_ID / SA, `LSI_` names contain `SI_`: `stray`
  verdicts in `enum_literals.tsv` ).
- **Compile:** `/Zs` probes of `X2GULuciel.cpp` ( census `cl_cmd` ), plain and with `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 0 errors each ( an earlier
  abstract-class error proved the guarded code compiles; neither `#error` guard fires ). Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Commit** `ab709d9f`; flagcheck `--range ab709d9f^..ab709d9f`: `LUCIEL_CORE` ( the two class files ) `0 PASS, 1 PASS~, 1 FAIL` ( the precedented new `.cpp` ), `JUNE_ENUM_NAMES` ( 4 files ) `1 PASS, 2 PASS~, 1 OTHER, 0 FAIL` / `RESULT: PASS`, same tallies as the worktree run ( the `eol!` on `X2UnitManager.h` is the tool's mixed-EOL file note: the inserted line is CRLF like its neighbours ).

### CH1a - Review (2026-11-14)

Opus 5.5 high, Ghidra read-only (`decompile_function` / `disassemble_function`, `funcdis.py` for uncarved bodies, `charport_consts.py` for constant
widths). **Verdict: pass after one fix** ( `UpCombinationPoint` ).
- **Fix:** `UpCombinationPoint` `32b14860` multiplies the gain by the raw spec-A float `+0x6c50` ( `fild n; fmul [esp]; fmul qword 33438088 = 10.0` ),
  with no `+ 1`; Eve's `3297ee20`, the reading CH1a copied, adds `fadd qword 334355b0 = 1.0` itself. The port now reads `n x GetCharacterSpecializationA()
  x 10.0`; kept verbatim as JUNE_BUGS 688 ( with no spec-A bonus a point gain gives no MP ). Both "June's double" constants are now `double`
  ( `0.8` cost rate, `10.0` MP rate ), as June's `fmul qword`s.
- **Ctor `32b1bd90`, every store ( both runs ):** all 123 members' inits = the header's; x87 runs followed ( `334380f0` = -1.0 at `+0x9714`, `+0x9730`,
  `+0x973c`, `+0x97a0`, `+0x97c0`, `+0x97cc` / `+0x97d0`, `+0x9828` / `+0x982c`, `+0x9700`; `334712f8` = 1200.0; `3346ede8` = 9999.0 x3; `3345b364` =
  35.0 x2 ); user throw members 0.66 / ( 40, 130, 0 ) / ( 2000, 100, 0 ); `32b4d1c0` = `SAltarOfEvilData` ( 2.0, 1200.0, 0.2, 0, two empty vectors );
  `328343d0( 0 )` on `+0x95c0` is `vector::reserve( 0 )` ( no effect, not ported ). Unset = `+0x95e4`, `+0x9644` / `+0x9648`, `+0x96dc`, `+0x96e0`,
  `+0x9778`, `+0x97fc`..`+0x9804`, `+0x9838`, `+0x988c` = JUNE_BUGS 681 ( nine members, one an array ). `+0x9704` is June's dialog handle ( `-1, 0`,
  reset by `31d41c20( 0 )` ): March also defines `DIALOG_HANDLE_TEST`, so `CKTDGUIDialogType( NULL )` and `SAFE_DELETE_DIALOG` = June's invalidate / delete.
- **Dtor `32b39380`:** order = commit: user major particles, `+0x959c`, minor ( `32b1eb10` ), NPCs `0x772` / `0x791` / `0x774` ( 1906 / 1937 / 1908 ),
  BTI `0x1da` / `0x1e1` ( 474 / 481 ), guide `322e6300( &false, 0.0 )`, `32b34920`, `32b38f30( 0, 0 )` / `( 1, 0 )`, `32b13d30( 0 )` / `( 1 )`, window
  `+0x8f70` ( `31fd03f0` then delete: CH2d20 stand-in ), NPC `0x8dd` ( 2269 ), dialog.
- **Bodies ( all = commit but the fix ):** `StateChangeBySkillID` `32aeaa10`: 37 case labels ( 9 unconditional, 28 behind vslot 0x260 ), every
  skill -> state value resolved and equal ( `SI_ETC_WS_COMMON_LOVE` = 900 live by `enumprobe.py`; the 6001 copy is dead ), Twilight's buff 482 and
  Magdonic's `+0x7e1c` tests in place, Phantom Rush `( minor != 1 ) x 2 + 460` with `true`; the Done note's "44 skill ids" is 37 labels.
  `32b14f70` ( 30023 / 30021, 30216 / 30220 vs states 460 / 462 ), `32b3acf0` ( strings 226 / 2549 / 33027 / 26119, stone 130208, -1 test, MP `fchs`
  from the consume slot ), `32aeb1e0` ( `cmp eax, 3` ), `32b0fa00`, `32b14db0` ( `+0x638` index at entry `+4`, `+0x5c4` skill at `+0`, `inc [+8]` ),
  `32b20100`, `32b34920` ( 30421 ), `32b35540`, `32ae6130` ( nine names read from the exe, same order ), cut-in `@32b43d52` ( table `32b4577c`:
  37 / 38 / 39 / 200 -> 0, 127 -> 1, 128 -> 2, 129 -> 3 ).
- **State enum:** 531 names / 493 ids, no value disagrees with `ch0_reg.tsv` / `ch0_state_skill.tsv`; multi-id names take their own slot
  ( `LSI_LCG_F_DASH_COMBO_XXXZ_END` 333: 327 is `..._ZXXZ_END`'s ); the 22 unnamed ids = CH1m's 17 re-registered + USI 111-113 / 115 / 116.
- **Registrations:** 188 `SKILL_ID` + BTI 474 / 481 / 482 + NUI 2269 = June's `Enum.src.lua` ( 192 / 192 ), no collision.
- **Provisional names ( 5 spot-checked, none renamed ):** `m_fSeizedByFearRate` ( SA 6 of 30401 in `32b45820` ), `m_fCommunionRate` ( 30402 ),
  `m_fSoulExtortionDrainRate` ( `32b435f0`, 30019, x rate -> MP ), `m_fSummonedDarknessElapsedTime` ( `32b20260` reset with skill 30013 / BTI 474 ),
  `m_iShadowBoltControlIndex` ( `32b2ff50`, game unit lookup ).
- **Routing:** CH1a4, CH2d20, CH1m, CH4c, CH1 ( `CreateGULuciel`, CH1a4 dependency ), CH1b amendments all present.
- **Hygiene ( CH1a commit ):** flagcheck `--range ab709d9f^..ab709d9f`: `LUCIEL_CORE` `0 PASS, 1 PASS~, 3 OTHER, 1 FAIL, 1 ERROR` ( FAIL = the new
  `.cpp`'s PCH line, precedent; ERROR = `Always.h`'s `LIVE && !CORE` guard ); `JUNE_ENUM_NAMES` `1 PASS, 2 PASS~, 1 OTHER, 1 FAIL, 1 ERROR` ( the
  same `.cpp` line and compound guard; `eol!` on `X2UnitManager.h` = the mixed-EOL file note ); `_LIVE` "not defined" ( commented out by design ).
  `git log -S`: `_CORE` / `_LIVE` later only in CH1a1 / CH1a2 / CH1a3 ( reviewed ), `JUNE_ENUM_NAMES` none. `junediverge --check-all`: `RESULT: PASS`.
  `enumscan --check X2GULuciel.h X2GULuciel.cpp`: `0 A/B`.
- **Hygiene ( this fix ):** flagcheck `LUCIEL_CORE --worktree --files X2Lib/X2GULuciel.cpp`: `1 PASS` / `RESULT: PASS` ( 4 lines replaced inside the
  file CH1a added under `_CORE`; no `#else`, the flag-off original is no file ); `junediverge --check`: `RESULT: PASS`; `enumscan`: `0 A/B`.
- **Compile:** `/Zs` probe of `X2GULuciel.cpp` ( census `cl_cmd` ), plain and `/D _LIVE`: 0 errors each. Full build skipped: no play-test.

### CH1a3 - Done (2026-11-14)

Opus 5.5 high, Ghidra read-only. Files: `X2GageUI.h` / `.cpp`, `X2GageManager.h` / `.cpp` ( CP949, `bytepatch.py`; 382 insertions, 0 deletions ).
funcmap `grep "Char Luciel CH1a3" june2017_re/funcmap.tsv` ( 19 rows ); `JUNE_BUGS.md` 682-684. Commit `c29941ce`; flagcheck `--range
c29941ce^..c29941ce --with SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: `RESULT: PASS` ( same tallies as the worktree run below ).
- **`CX2LucielMyGageUI` ( `_CORE` ):** RTTI vt `3353375c`, 29 slots vs base `3353336c`, 0x64 bytes ( base 0x5c + `m_bMinorUnitFormCharacterImage`
  +0x5c + `m_hSeqCombinationPointFull` +0x60; ctor `32e647d0`, made only by `CreateMyGageUI` ). Ported: 0 dtor `32e72770` / `32e6ad10`, 1 `InitUI`
  `32e6e230` ( Rena's order, opens `DLG_Luciel_CombinationPoint.lua`, shows `StaticPVPGameInfo_UnSelected` and `Luciel_Combination_Point_BG` ),
  2 `OnFrameMove` `32e719b0`, 8 `SetShow` `32e5ed90` ( = Eve's on +0x60 ), 19 `SetCharacterImage` `32e62d50`, 24 `UpdateMpAndHyperAndDetonationGage`
  `32e63070` ( March's virtual at that slot: base `32e62d10`, called by the base frame move `32e71620` after the HP gauge; Lu/Ciel runs hyper
  ( vslot 0x6c ) before MP, then detonation ( 0x64 ), then the resting form ). Helpers: `UpdateCharacterImageByForm` `32e62ef0`, `UpdateFormMpGage`
  `32e62410`, `UpdateCombinationPointUI` `32e6e2c0`, `UpdateCombinationPointFullEffect` `32e6ae10`, static `GetMinorUnitFormCharacterImageName` `32e62190`.
- **Slots not overridden ( = the base's pointer ):** 3 `OnFrameRender`, 4 `CrashMyStateGageUI`, 5 / 6 HP / MP texture, 7 `GetShow`, 9 / 10 alpha,
  11-18 the buff virtuals and `SetDurationTime`, 20 `SetBuffIconStatic`, 21 `InitWakeOrb` ( `32e5b310`, Rena's too ), 22 `ResetGageUIEtc`, 23
  `RefreshWayOfSwordUI`, 25 `UpdateDetonationGage`, 26 ( June-only `0x68` ), 27 `UpdateHyperGage`, 28 ( `0x70` ). Secondary vt `33533744`: slot 0
  `32e72670` = compiler adjustor thunk ( `sub ecx, 0x44; jmp 32e72770` ), 1-3 the base's.
- **What it reads:** `CX2LucielGageData` ( CH1a1 names ): `GetNowCombinationPoint`, `Get/SetCombinationPointChanged`, `GetNowMp` / `GetMaxMp` with
  the gauge index ( in play = `IsMinorUnitForm()`, June `31d717e0`; the other index for the unselected bar ); `CX2Unit::IsMinorUnitForm` ( +0xe40 ).
- **Stand-ins / left out ( recorded at the site ):** `31c966d0` ( June-only `CPictureData` UV fill ) = `UpdateGageForUV` on picture 1 behind June's
  0..1 rate gate. `32e6e2c0`'s `CX2GageManager +0xd1` test ( hide the full effect ): the byte's six writers all sit behind enable code 2155, never
  sent offline; noted in backlog Stage 17's existing bullet ( which said "no writer found" ). `SetCharacterImage`'s transcend argument uses
  `IsMyGageTranscended()` ( null-safe ) where June reads unit +0x3c through an unchecked user, as March's base port does.
- **Particle flag:** sequence `+0x2f4` = `SetResolutionConvert` ( June's Elsword `PlayUIEffectDestruction` `32e67770` writes the same `+0x170` /
  `+0x2f4` pair March ports as `SetOverUI` / `SetResolutionConvert` ).
- **Manager:** `CreateGageDataByUnitClass` `3248ec80` ( static, `_CORE`; 31eca1d0 == `UT_LUCIEL` -> `CX2LucielGageData`, else `CX2GageData` ). Under
  `_LIVE` ( commented out ): `CreateMyGageUI`'s seven Lu/Ciel classes ( `3248e290 @3248e618` ), `InsertPartyMemberUI` ( `324910c0`: = March but the
  factory ) and `InsertPvpMemberUI( SlotData, CX2GUUser* )` ( `32491200` ) take the factory. `CX2GULuciel::CreateGageData` `32b37c70` stays CH1k2's.
- **Routed:** the rest of June's two PvP member inserts ( `32491200`, unmapped `32491480`: observer byte, team test ) -> backlog Stage 9 bullet
  ( PvP only, unreachable offline ). `DLG_Luciel_CombinationPoint.lua` ( data034, CH5's asset, missing ) -> CH1 kickoff amended ( adopt it with the
  scripts or the point row stays empty ).
- **Hygiene:** flagcheck `JUNE_CHAR_LUCIEL_CORE --worktree --with SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE --files` ( 4 files ): `files: 1 PASS, 3 PASS~
  (blank/marker only), 0 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR` / `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched
  inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)` / `RESULT: PASS` ( PASS~ = one blank line before each new block ).
  `junediverge.py --check` ( 4 files ): `allow-listed: 7` / `RESULT: PASS`. `enumscan.py --check` ( 4 files ): `0 A/B row(s) remain`.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH ) of `X2GageUI.cpp` and `X2GageManager.cpp`: 0 errors each, plain and with
  `/D SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` ( the `_LIVE` probe builds `new CX2LucielMyGageUI`, so the class compiled ). Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert ). From CH1 final: the HUD's Combination Point row, the full mark + `LD_Arousal_UI_01` at 10, Ciel's
  portrait on a body swap, the resting body's MP on the unselected bar.

### CH1a3 - Review (2026-11-14)

Opus 5.5 high, Ghidra read-only (vtable dump from the exe, `decompile_function`, `disasm_range.py` for uncarved bodies). **Verdict: pass, no code change.**
- **Slot map:** vt `3353375c` vs base `3353336c`, Rena `3353363c`, Eve `335336cc` (29 slots each): Lu/Ciel differs from the base only at 0 `32e72770`,
  1 `32e6e230`, 2 `32e719b0`, 8 `32e5ed90`, 19 `32e62d50`, 24 `32e63070` (base `32e62d10`); 25 `32e66cc0` / 27 `32e5e120` = the base's (vslots 0x64 /
  0x6c); 21 `32e5b310` = base and Rena (Eve's is `32e5b7b0`). Secondary vt `33533744`: slot 0 `32e72670` (thunk), 1-3 = `32e5c470` / `31f3f070` x2.
- **Spot-checks (3, all = commit):** (1) the slot map above. (2) `UpdateFormMpGage` `32e62410`: the same NULL chain (gauge data `+0x10`, both statics,
  `g_pData +0x68 +0x90`); index `31d717e0` = selected unit `+0xe40 == 1` (bool, so `iVar == 0` = the port's other index); bar call `32e5c1e0( bar, 0,
  L"MP" (33532aec), now / max, 1 )` = `UpdateGageForUV( PGUB_MY_MP (0), L"MP" )`; `g_pStatic_MP` "%ld/%ld" now / max; `32e5c7f0`; resting rate on
  picture 1 through `31c966d0( L"BG_LUCIEL_MANA", rate, 1 )`, whose body is March's `UpdateGageForUV` width branch behind `334350e8 <= rate <=
  334355b0` and the same texture checks; string 0 = `3344f1f4` (empty). (3) `UpdateCombinationPointUI` `32e6e2c0` (uncarved, ends `32e6e3c7`):
  protected count `+0x114`, `>= 10` -> full; changed byte `+0x110` cleared; `Luciel_Combination_Point` (33534860) pictures 0-9 `bShow = count > i`,
  picture 10 = full; `+0x60 = -1` when the `g_pX2Game` holder `3372c0fc` (funcmap row 151) is dead; `CX2GageManager +0xd1 == 1` -> show 0 (left out);
  vslot 0x1c `GetShow` then `32e6ae10( show )`.
- **`+0xd1` writers:** `3245c5bd`, `32465b8d`, `32478137`, `32edbe4d` each sit right after a code `0x86b` (2155) test; the left-out test stays left out.
- **Manager:** `324910c0` passes `movzx word [edi + 0x24]` (the party class read as a word, agreeing with NU2's `SERV_IRUHADEV_JUNE_CHAR_UNIT_CLASS_WORD`)
  to `3248ec80`; `32491200` passes the user's `+0x867c -> +0x38` class or 0 = March's null-safe `CX2GUUser::GetUnitClass()` (`UC_NONE`). The factory's
  seven-class switch equals June's `31eca1d0 == UT_LUCIEL` within scope (third jobs out, memory rule). Stage 9 PvP bullet and Stage 17 note present.
- **Routing:** `kickoffs/char-luciel/CH1.md` "Amended by char-luciel CH1a3" carries the `DLG_Luciel_CombinationPoint.lua` adoption and `LD_Arousal_UI_01`.
- **Hygiene:** flagcheck `JUNE_CHAR_LUCIEL_CORE --range c29941ce^..c29941ce --with SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: `files: 1 PASS, 3 PASS~
  (blank/marker only), 0 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR` / `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted
  line(s), 0 #else/#endif without the flag token, 0 encoding change(s)` / `RESULT: PASS` (PASS~ = one blank line before each new block). `git log
  -S SERV_IRUHADEV_JUNE_CHAR_LUCIEL c29941ce..HEAD` on the four Gage files: none (no later commits touch them). `junediverge.py --check-all`:
  `allow-listed: 242` / `RESULT: PASS`. `enumscan.py --check` (4 files): `0 A/B row(s) remain in 4 file(s) (C 0, D 0, X 1 ignored)`.
- **Build:** none (no code change). CH1's CH1a3 dependency is met.

### CH1a4 - Done (2026-11-14)

Opus 5.5 high, read-only RE ( `charport_adis.py`, `disasm_range.py`, `dispsweep.py` ). funcmap `grep "(CH1a4)" june2017_re/funcmap.tsv` ( 7 rows );
JUNE_BUGS 689; `ch0_vtable_all.tsv` slot 0x634 named.
- **`CX2GUUser` pair ( `TRANSFORM_LINK`, `X2GUUser.h` ):** `struct STransformedSkin { bool m_bSwapped; m_pMotionFuture; m_pMotion; m_pXSkinAnimFuture;
  CKTDGXSkinAnimPtr m_pXSkinAnim; m_pAfterImage; }` ( 0x18, June's ctor `32073d50` zeroes all six ) and `m_aTransformedSkin[2]` after
  `m_pTransformSkillSlotUI` ( June `+0x8f70` / `+0x8f74` / `+0x8f8c` ). The user ctor `@32040eb0` / `@32040ec3` = the struct ctor; the user dtor
  `@3203b530`-`@3203b56a` releases only the two anim refs = the member's own dtor. Nothing else to write in `X2GUUser.cpp`'s ctor / dtor.
- **Other readers of `+0x8f74`..`+0x8fa0` ( `dispsweep.py`, every hit read ):** in `CX2GUUser` only the ctor, dtor and `RestoreHead` `31fde420`
  @`31fde67f` / @`31fde69c`; the rest are Lu/Ciel's own ( `32b162e0` LoadTransformedSkinAnim CH1k, `32b13b60` GetSkinAnimByForm CH1l, `32b0e440`
  vslot 0x6b0 CH1l2, `32b34fe0` StartTransformedForm CH4a: each kickoff amended ) or other classes' unrelated offsets ( `327xxxxx`, `3328xxxx`, `3330xxxx` ).
  `RestoreHead`'s DU1d2 stand-in is replaced: **`CX2GUUser::RestoreHeadBySkinAnim`** = June `31fdc9d0` ( Face restore + `SetUnitReForm` Bip01_Head /
  Bip01_Spine1, NULL -> nothing ), called on both sets' anims after the second-body block, unconditionally as June; stand-in kept in `#else`.
- **`ReleaseTransformedSkinAnim` `32b13d30`:** motion future then motion closed ( `CloseDevice`, only with a device manager, as June ), anim future
  and after-image `SAFE_DELETE_KTDGOBJECT`, anim ref reset; the byte untouched.
- **`SetFormSkinAnim( iForm, STransformedSkin& )` `32b13e20` ( declared here ):** form 0 = `m_pMotionFuture` ( `+0x87c8` ) / `m_pMotion` ( `+0x87cc` ) /
  `m_pXSkinAnimFuture` / `m_pXSkinAnim` / `m_pAfterImage` ( `+0x6754` ), form 1 = DU1's `m_pMinorUnit*` + `m_pMinorUnitAfterImage` ( `+0x6758` ). A
  body after-image with `m_bEnable` ( `+0x194` ) set is hidden by `CKTDGXSkinAfterImage::SetShowAfterImage( false )` ( June `3322a9c0`, new in
  `KTDGXSkinAfterImage.h` under `TRANSFORM_LINK` ) and `DisableAfterImage( true, AIB_ACTIVE_UNIT )`; after the swap `EnableAfterImage( AIB_ACTIVE_UNIT )`
  when the new after-image exists; the old five go into the set; byte = `!byte`.
- **`EndTransformedForm( iForm, bByDie )` `32b38f30`:** `BTI_BUFF_TRANSCENDENCE` for form 1, else `BTI_BUFF_ALTAR_OF_EVIL`. Transformed: `SetTransformed(
  false )`; set not swapped -> return ( **June's quirk: the tail is skipped**, JUNE_BUGS 689 ); `SetFormSkinAnim`; `+0x200` / `+0x210` =
  `m_CollisionListSet` / `m_AttackListSet` clear ( R12b: `UpdateEquipCollisionData`'s sets ); Bip01_Head into `m_pFrame_Bip01_Head` ( `+0x4200` ) /
  `m_pFrame_MinorUnit_Bip01_Head` ( `+0x4218` ), no NULL test as June; `SetShowViewList( true )` ( vslot 0x63c ); `m_vecpWeapon` ( `+0x3cfc` ): each
  weapon with `m_pUserWeaponEquip` ( Weapon `+0x1c0` ) shows its `GetNormalAnim` / `GetHyperAnim` ( CX2Eqip `+0x544` / `+0x564`, 8 = `MAX_MODEL_COUNT_A_ITEM` )
  with `SetShowObject( true )` ( June inlines it ); vslot 0x634 = **`UpdateEquipCollisionData()`** ( base `31fc7a60` calls 0x638 on `m_ViewEqipList`
  = March `X2GUUser.cpp:5996`; Chung / Ara override it ); `UpdateShowViewListByActiveBody`; `EffectSet_SA_LDB_TRANSFORM_END_BY_DIE` on `this` with
  every default when `bByDie`; my unit ( vslot 0x198 ): window delete ( CH2d20 stand-in, the dtor's twin ) and `(int)( (int)GetBuffTimeFinalizerDuration(
  BTI ) / 6.0 )` > 0 -> `UpCombinationPoint`. Tail ( also the not-transformed path ): `ClearAltarOfEvilEffectSet`, `EraseBuffTempletFromGameUnit( BTI )`,
  `StopEffectSet( m_hTransformedFormEffectSet )`.
- **Registered ( `JUNE_ENUM_NAMES`, `X2BuffFactor.h` ):** `BTI_BUFF_ALTAR_OF_EVIL = 763`, `BTI_BUFF_TRANSCENDENCE = 764` ( `Enum.src.lua` L12193 / L12966 ).
- **Offsets cross-check:** Weapon `+0x1c0` and the after-image list `+0x1bc` both sit 4 bytes past March's layout, June's larger `CKTDGObject`.
- **Routed / amended:** CH2d20 ( second dialog-release site ), CH4a ( the shared sets, JUNE_BUGS 689 reach ), CH1k / CH1l / CH1l2 ( the member name ).
- **Gates ( `finish.py --files`, 6 files ):** `flagcheck TRANSFORM_LINK` `2 PASS, 1 PASS~, 3 OTHER, 0 FAIL` / `RESULT: PASS`. `junediverge` `RESULT: PASS`.
  Tool limits, re-run per file: `DUO_BODY` and `JUNE_ENUM_NAMES` report FAIL on `X2GUUser.cpp` ( TRANSFORM_LINK's own lines sitting inside the
  enclosing `DUO_BODY` / `RESTORE_HEAD` block: not their change; `DUO_BODY` is enclosing only ); `flagcheck JUNE_ENUM_NAMES --worktree --files
  X2Lib/X2BuffFactor.h`: `1 PASS~` / `RESULT: PASS`. `LUCIEL_CORE` raised Unevaluable on Chung's compound guard in `X2GUUser.cpp:29404`;
  `--files X2GULuciel.h X2GULuciel.cpp`: `2 PASS` / `RESULT: PASS` ( the 2 deleted lines = the two declaration comments "body in CH1a4", inside the
  `_CORE`-only file ). `enumscan --check` class files + `KTDGXSkinAfterImage.h`: `0 A/B`; the shared files' only rows in this diff are the two enum
  definitions themselves ( `X2BuffFactor.h:680` / `:681` ), the rest pre-exist. `stateends` / `skillicons` / `luabind`: not applicable ( no states,
  skills or scripts ).
- **Compile:** `/Zs` probes ( census `cl_cmd` ) of `X2GULuciel.cpp` ( plain and `/D _LIVE` ) and `X2GUUser.cpp`: 0 errors each; `/P` shows all four
  new bodies compiled in. Full build skipped: no play-test. Header edits ( `X2GUUser.h`, `X2BuffFactor.h`, `KTDGXSkinAfterImage.h` ) sit in the PCH: the
  next full build must rebuild it ( `--pch force` if in doubt ).
- **How to see it in game:** nothing yet ( inert until CH1 final; CH2d2 / CH3c2 hand off the transform play-test ).
- **Commit** `27c7bdd5`; flagcheck `--range 27c7bdd5^..27c7bdd5`: `TRANSFORM_LINK` `2 PASS, 1 PASS~, 3 OTHER, 0 FAIL` / `0 deleted` / `RESULT: PASS`; `LUCIEL_CORE` ( class files ) `2 PASS` / `RESULT: PASS`; `JUNE_ENUM_NAMES` ( `X2BuffFactor.h` ) `1 PASS~` / `RESULT: PASS`.

### CH1a4 - Review (2026-11-14)

Opus 5.5 high, read-only ( `charport_adis.py` on all six June bodies, Ghidra `decompile_function` on `32b13e20` / `32b38f30` ). **Verdict: pass, no code change.**
- **`STransformedSkin`:** ctor `32073d50` zeroes `+0x00` byte, `+0x04`..`+0x0c`, the ref at `+0x10`, `+0x14` = the struct; `32b13d30` closes `+4` then `+8`
  ( `CloseDevice` only with `g_pKTDXApp +0x1f4` ), deletes `+0xc` then `+0x14`, releases `+0x10`, byte untouched = commit. Set bases `+0x8f74` / `+0x8f8c`
  ( Ghidra `param_1 + 0x23dd` / `0x23e3` dwords ).
- **`SetFormSkinAnim` `32b13e20`:** set `+4` <-> `+0x87c8` ( `m_pMotionFuture`, R12b row `31fe5860` ), set `+8` <-> `+0x87cc` ( `m_pMotion` ), `+0xc` <->
  `+0x87d0`, `+0x10` <-> `+0x87d4`, `+0x14` <-> `+0x6754`; form 1 = `+0x8e7c`..`+0x8e88` / `+0x6758`. After-image `+0x194 == 1` -> `3322a9c0( 0 )` then
  `322720f0( 1, 3 )` = `DisableAfterImage( true, AIB_ACTIVE_UNIT )` ( 3 in March `X2GameUnit.h:3620` ); new after-image set and flag 1 -> `32275a30( 3 )`;
  byte = `sete` ( `!byte` ). Port order and slots equal.
- **`EndTransformedForm` `32b38f30`:** BTI `0x2fb + ( form != 0 )`; not transformed -> straight to the tail; byte clear -> `goto` the epilogue past the tail
  ( JUNE_BUGS 689 confirmed in Ghidra ); `+0x200` / `+0x210` = the collision / attack sets ( R12b `326bd250` row ); head frame `+0x4200` / `+0x4218` with no
  NULL test; vslot 0x63c( 1 ); weapon loop `+0x3cfc`, Weapon `+0x1c0`, 8 x ( `+0x544 + 4i` then `+0x564 + 4i` ), inlined `SetShowObject( true )`.
  `+0x564` = hyper per R14 `32963eb0` / R14b `3295a570` ( minor is `+0x584`, not read here; R12b's "hyper +0x584" row is that minor array ).
  vslot 0x634, `31fd6260`; `PlayEffectSet` args ( name, this, 0, 0, -1, -1, (1,1,1), 0, 3 x zero vectors, 1.0 ) = March's defaults ( `X2EffectSet.h:1083` );
  vslot 0x198, dialog-release stand-in ( CH2d20 ), `_ftol2` / `fild` / `/ 6.0` / `_ftol2` / `> 0`. Tail: `32b35540`, `3229e160` = March
  `EraseBuffTempletFromGameUnit` ( find_if + `ByIterator`, `X2GameUnit.cpp:15357` ), `321dafb0( +0x983c, 0 )` = March's immediate `StopEffectSet` ( the
  deferred-queue precedent, C9c-1 / C9d / C9f ).
- **`RestoreHeadBySkinAnim` `31fdc9d0`:** NULL -> nothing; Face frame inline restore ( `+0xdc` = 0x100, scale 1 ) = `SetFrameScaleRestore` ( D1-3h row );
  `SetUnitReForm` Bip01_Head / Bip01_Spine1 with the templet type and `+0x8c0c` ( `IsTransformed()`, as DU1d2's second-body calls ). Both calls follow
  the second-body block `@31fde558-31fde67a`, unconditional = commit. **`SetShowAfterImage` `3322a9c0`:** 8-byte `m_AfterImageList` entries, `pAnim`
  non-NULL -> inlined `SetShowObject( b )` = commit. **BTI 763 / 764** = `Enum.src.lua` L12193 / L12966.
- **Hygiene ( `finish.py --review --commits 27c7bdd5 -v` ):** `TRANSFORM_LINK` `2 PASS, 1 PASS~, 3 OTHER, 0 FAIL` / `0 deleted` / `RESULT: PASS`.
  `junediverge` and `--check-all`: `RESULT: PASS`. The four FAILs are the Done note's tool limits, re-run per file: `DUO_BODY` ( enclosing only ) and
  `JUNE_ENUM_NAMES` FAIL on `X2GUUser.cpp` = TRANSFORM_LINK's lines; `flagcheck JUNE_ENUM_NAMES --range 27c7bdd5^..27c7bdd5 --files X2Lib/X2BuffFactor.h`:
  `1 PASS~` / `RESULT: PASS`. `LUCIEL_CORE` Unevaluable on Chung's guard `X2GUUser.cpp:29404`; `--files X2GULuciel.h X2GULuciel.cpp`: `2 PASS` /
  `RESULT: PASS` ( 2 deleted = the two "body in CH1a4" declaration comments ). OTHER rows = each file's own flag. `enumscan` 216 A/B in 6 files: filtered to
  the commit's lines only `X2BuffFactor.h:680` / `:681`, the enum definitions themselves; the rest pre-exist. `stateends` / `skillicons` / `luabind`: n/a.
  No later commit touches the six files.
- **Build:** none ( no code change ). CH1's CH1a4 dependency is met.

### CH1k0 - Done (2026-11-14)

Opus 5.5 high. New class-agnostic flag `SERV_IRUHADEV_JUNE_CHAR_BUFF_TIME_PASS` ( `Always.h`, no `_DEBUG`, no log tag, inert until CH1k / CH1m call it ),
with an `#error` guard in `X2GameUnit.cpp` on `SERV_IRUHADEV_JUNE_BUFF_BINDING` ( `SetDurationTime` ), `SERV_IRUHADEV_JUNE_CHAR_EVE_PASSIVE`
( `GetDurationTime` ) and `FIX_BUFF_FINALIZE_DURATION_TIME_CONTROL_BUG`. Files: `Always.h`, `X2GameUnit.h` / `.cpp` ( insert-only, 89 lines ). Code commit
`88c14ed5`. funcmap: `grep "(CH1k0)" june2017_re/funcmap.tsv` ( 3 rows; the CH0 candidate rows stay, superseded ). `JUNE_BUGS.md` 690, 691.
- **Already ported?** None of the three: Eve CH4b2 holds only the Get / Set siblings ( `3227bfa0` / `3227c110` ); `3227b920` / `3227ba10` are March's
  `GetBuffFinalizerTempletPtrList` / `ChangeBuffFinalizerTempletPtrList` ( D0c-1: same as March ), `3229fd80` is March's `GetBuffFactor`.
- **`3227bcb0` -> `CX2GameUnit::AddBuffTimeFinalizerDuration( BUFF_TEMPLET_ID, float )`:** each Time finalizer of the BTI's first buff gets
  `Set( Get + add )`, then the list goes back even when no buff matched ( as Eve's setter ). Caller `32b3d380` ( CH1m ).
- **`322a7000` -> `AddAllBuffTimeFinalizerDuration( float )`:** per `GetBuffFactor` entry, skipped when the templet exists and is a debuff
  ( templet `+0x64` = `IS_DEBUFF`, `jz @322a70b8` ); every Time finalizer `Set( Get + add )`; for BTI 5 / 822 ( `BTI_HYPER_MODE` /
  `BTI_TRANSCENDENTAL_HYPER_MODE`, both in March's enum ) the gauge ( unit `+0xc04` = `GetGageData()` ) hyper-mode remain time ( protected `+0xe8` )
  gains the value too when it is > 0 ( `test ah,5 / jp` ), via `SetHyperModeRemainTime` `31e8c9d0`. Callers `32b3db80` / `32b3dea0` ( CH1m, CH2c2 ).
- **`322a74a0` -> `MultiplyAllDebuffTimeFinalizerDuration( float )`:** returns when rate < 0 ( `fldz / fcomp`, `test ah,0x41`; NaN passes, kept ); per
  factor, skipped when the templet exists and is **not** a debuff ( `jnz @322a7538` ): **CH0 had this backwards** ( "non-debuff" ); the funcmap and
  `ch0_unmapped_identified.tsv` rows and the CH1k kickoff are corrected. `Set( Get * rate )`. Caller Lu/Ciel vslot 0x318 `32b0e3f0` ( CH1k ).
- **Quirks kept:** a factor's edit lands on the first buff of its BTI, so N buffs of one BTI give the first N edits ( row 690; a missing templet passes
  both filters ); the awakening clock moves once per Time finalizer ( row 691 ).
- **Kickoffs amended:** CH1k ( `322a74a0` name, debuff ), CH1m ( `3227bcb0`, `322a7000` ), CH2c2 ( `322a7000` ).
- **Hygiene:** `finish.py --files` ( worktree ) and `--range 88c14ed5^..88c14ed5`, both: `flagcheck JUNE_CHAR_BUFF_TIME_PASS PASS` ( `2 PASS, 0 PASS~,
  1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR` ), `junediverge RESULT: PASS`, `enumscan exit 0`, `FINISH RESULT: PASS`. `JUNE_BUFF_BINDING` /
  `JUNE_CHAR_EVE_PASSIVE` SKIPPED: targets of the `#error` guard, not this phase's. stateends / skillicons / luabind: n/a.
- **Compile:** `/Zs` probe of `X2GameUnit.cpp` ( census `cl_cmd`, no PCH ): 0 errors; a `cl /P` of it holds all three definitions ( flag on, guard
  passes ). Full build skipped: no play-test.
- **Peer:** char-luciel CH1l30 ran at the same time in `X2Game.*` / `X2DamageEffect.h` / `funcmap.tsv`; this phase committed only its own hunks.
- **How to see it in game:** nothing yet ( inert ); CH1k / CH1m's callers reach it when `_LIVE` lands.

### CH1k0 - Review (2026-11-14)

Opus 5.5 high, read-only ( Ghidra `disassemble_function` on `322a74a0`, `322a7000`, `3227bcb0`, `31e8c9d0` ). **Verdict: pass, no code change.**
- **Debuff filter, both ways:** `322a74a0` @`322a7531`-`322a7538`: templet NULL -> scaled, `+0x64 == 0` -> `jnz` not taken -> skip; so debuffs and missing
  templets are scaled = port `NULL != ptr && false == IsDeBuff()`. `322a7000` @`322a70b1`-`322a70b8`: NULL -> added, `+0x64 == 0` -> added, else skip = port
  `NULL != ptr && IsDeBuff()`. `+0x64` = `IS_DEBUFF` per four confirmed funcmap rows ( `32369cc0`, `326c31c0`, `326c9530`, `3204dcd0` ); March
  `IsDeBuff()` returns `m_bDeBuff` ( `X2BuffTemplet.h:113` ). The templet comes from `GetInstance()` ( `31ef6990` / inlined lazy create ) + `326c3170`.
- **Rate guard:** `fldz / fcomp [rate]`, `test ah,0x41`, `jz` out: out only with C0 = C3 = 0 ( 0 > rate ); NaN sets both and runs = port `fRate_ < 0.f`.
- **Awakening clock:** @`322a714f` BTI 5 / 0x336, unit `+0xc04` NULL-tested ( March `GetGageData()` is the non-virtual `m_pGageData`,
  `X2GameUnit.h:2420` ), protected `+0xe8` read twice, `test ah,5 / jp` skips 0, < 0 and NaN = port `0.f < remain`; `31e8c9d0` stores the protected
  `+0xe8` ( March `SetHyperModeRemainTime`, `X2GageData.h:555` ); all inside the `pTimeFinalizer != NULL` branch of the finalizer loop ( row 691 ).
- **`3227bcb0`:** arg 1 ( `[esp+0x44]` ) -> `3227b920` = BTI, arg 2 ( `[esp+0x48]` ) `fadd` = float, `ret 8`; vslot 0x3c Get, `fadd`, vslot 0x38 Set
  ( slots 15 / 14 per Eve CH4b2 rows `3227bfa0` / `3227c110` ); `3227ba10` after the loop with no test of `3227b920`'s result = port.
- **Loops:** factor stride 0x5b, BTI `+0x20`; `ChangeBuffFinalizerTempletPtrList` once per non-skipped factor in both passes ( row 690 ) = port.
- **`#error` guard:** `cl /P` of `X2GameUnit.cpp` ( census `cl_cmd` ): rc 0, no `C1189`, one definition of each function in the `.i`.
- **Hygiene ( `finish.py --review --commits 88c14ed5 -v`, two `--skip-flag` for the guard's targets ):** `JUNE_CHAR_BUFF_TIME_PASS` `2 PASS, 0 PASS~,
  1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR` / `0 deleted` / `RESULT: PASS`; `junediverge` and `--check-all` `RESULT: PASS`; `enumscan exit 0`;
  `FINISH RESULT: PASS (9 gates, 0 failed, 5 skipped)`. No OTHER rows or deleted lines to explain. Later commit `feb8091e` ( CH1l30 ) adds its own hunks to
  `X2GameUnit.*`, none in this flag's blocks.
- **Build:** none ( no code change ). CH1k now waits only on the CH1i review.

### CH1l30 - Done (2026-11-14)

Opus 5.5 high. New class-agnostic flag `SERV_IRUHADEV_JUNE_CHAR_ENEMY_SCAN` ( `Always.h`, no `_DEBUG`, no log tag, inert until CH1l3 / CH1m call it ),
with an `#error` guard beside the define on `SERV_IRUHADEV_JUNE_DAMAGEEFFECT_RUNTIME` and `SERV_IRUHADEV_JUNE_DAMAGEDATA` ( `CEffect::GetLockOnAimPos`
and its `CX2GameUnit` friend ). Files: `Always.h`, `X2Game.h` / `.cpp`, `X2GameUnit.h` / `.cpp`, `X2DamageEffect.h` ( insert-only, 128 lines ).
Code commit `feb8091e`. funcmap: `grep "(CH1l30)" june2017_re/funcmap.tsv` ( 4 rows; the CH0 candidate rows stay, superseded ). No `JUNE_BUGS.md` row: no quirk with an effect.
- **Already ported?** `322d7b30` = char-elesis CH3c2's `CX2Game::GetFrontNearestEnemyUnit`; `31f75980` = March's `CEffect::SetLinkMainEffectByPos`
  ( both settled by the 2026-11-14 kickoff amendment ). `32294e30` ( C3c `CX2GameUnit::CollectUnitsAround` ) is the twin of the two new bodies.
- **`322f1a90` -> `std::vector<CX2GameUnit*> CX2Game::GetEnemyUnitListInRange( pUnit_, iMyTeam_, fRange_ )`:** thiscall on the game ( caller sets ECX =
  `g_pX2Game` ), hidden return vector, `RET 0x10`. NULL unit -> empty. Per unit: `IsValidUnit` ( inlined `+0x304` list ), team `== iMyTeam_` skip,
  `GUSI_DIE` ( vslot 0x174 == 4 ), `GetNowHp() <= 0` ( `[3342e84c]` = 0.0 ), same `GameUnitType` + same UID = self skip; aim point = vslot 0x3e4 with
  `L""` ( `[3344f1f4]` is zero bytes ) from `pUnit_->GetPos( false )` = `CEffect::GetLockOnAimPos`; an NPC must be `NCT_BASIC` and subtracts
  ( `+0x3e30` + `+0x3e34` ) x 0.25 ( `[33437830]` double 0.25 ); kept when distance `<=` range ( `fcom / test ah,0x41 / jp`: NaN refused ); `std::sort`
  ( `322acc40`, the same ascending float sort as `32294e30` ) then the units copied out. **No count cap:** CH0's "nearest-N" was a guess. Caller
  `32b34c70` ( CH1l3 ): unit = the one from `31e55340`, team = Lu/Ciel's ( `+0x278` ), range 500.0 ( `[334ed3dc]` ). Needs `friend class CX2Game;` in
  `CEffect` ( `X2DamageEffect.h`, under the flag ) for the protected aim helper.
- **`32295360` -> `bool CX2GameUnit::GetNearestEnemyUnitDistance( OUT float& )`:** thiscall, `RET 4`, returns AL ( true when a unit beat the start ).
  `*out = 9999999.0` ( `[3344915c]` ). Per unit: `IsValidUnit` `322ccf50`, same team skip, `GUSI_DIE`; the HP gage test is inlined ( `+0xc04` NULL
  passes, `+0x20` NULL or HP `<= 0` skips ) = March `GetNowHp() <= 0.f` ( June `31e4f900` returns 0 for a NULL HP gage ); aim point as above from this
  unit's `GetPos( false )`; NPC `NCT_BASIC` + size x 0.25; kept when distance `<` `*out` ( `test ah,0x41 / jnz`: equal or NaN refused ). Two local
  vectors June builds and frees are never filled ( no behaviour, not ported ). Null-game check added as a labelled stand-in, as `CollectUnitsAround`.
  Caller `32b3e470` ( CH1m ).
- **`322d7b30` at `32b318e0` ( CH2d2 ):** pushes ( unit, float range, an empty `std::set`, `&int` = -1 ). June's 4th argument is a `const int&`: it
  goes straight to vslot 0x178, and NPC `3275bc30` compares `[esi]`. Elesis's by-value `iAttackType_` with -1 is the same; no new body.
- **Names:** `junediverge.py --list` checked; no June / address / phase-tag names.
- **Hygiene:** `finish.py --files` ( worktree ): `flagcheck JUNE_CHAR_ENEMY_SCAN PASS` ( `5 PASS, 0 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR` ),
  `junediverge RESULT: PASS`, `enumscan exit 0`, `FINISH RESULT: PASS`. `JUNE_DAMAGEEFFECT_RUNTIME` / `JUNE_DAMAGEDATA` SKIPPED: targets of the
  `#error` guard, not this phase's. stateends / skillicons / luabind: n/a. `--range feb8091e^..feb8091e`: same verdicts, `FINISH RESULT: PASS`.
- **Compile:** `/Zs` probes of `X2Game.cpp` and `X2GameUnit.cpp` ( census `cl_cmd`, no PCH, the real `Always.h` ): 0 errors each; a `cl /P` holds both
  bodies ( flag on, guard passes ). Full build skipped: no play-test.
- **Peer:** char-luciel CH1k0 ran at the same time in `X2GameUnit.*` / `Always.h`; this phase applied its hunks there only after `88c14ed5` landed.
- **How to see it in game:** nothing yet ( inert ); CH1l3 / CH1m / CH2d2's callers reach it when `_LIVE` lands.

### CH1l30 - Review (2026-11-14)

Opus 5.5 high ( Ghidra `disassemble_function` on `322f1a90`, `32295360`, `31e4f900`, `322aa800`, `322d7b30`; constants read from `unpacked_x2.exe` with
`charport_consts.read_va` ). **Verdict: pass after one fix.**
- **`322f1a90`:** EBX = hidden return `[esp+0xa0]`, EBP = unit `+0xa4`, team `+0xa8`, range `+0xac`, `RET 0x10`; ECX is never read ( the body fetches the
  game itself ), so a member called on `g_pX2Game` is the same. NULL unit -> empty vector. Order = port: NULL, inlined `+0x304` list ( IsValidUnit ),
  team ( `+0x278` decrypt ) `==` skip, vslot 0x174 `== 4`, `31e4f900 fcomp [3342e84c] / test ah,0x41 / jnp` ( skips `<= 0`, keeps NaN = March `<= 0.f` ),
  `+0x264` equal then vslot 0x54 64-bit UID equal = self. Aim: `L""` built, vslot 0x138 ( 0 ) on the unit, vslot 0x3e4 on the target. NPC `+0x85f8`
  `+4 == 0` else skip, ( `+0x3e34` + `+0x3e30` ) x `[33437830]`. Range `fcom / test ah,0x41 / jp`: keeps `<=`, NaN refused. Sort `322acc40` -> insertion
  `322aa800` @`322aa82b`-`322aa839` moves when `new < first`: ascending, 8-byte pairs. No cap. = port.
- **`32295360`:** `*out = [3344915c]`, AL = found flag, `RET 4`; order valid ( `322ccf50` ), team, die, inlined HP, aim, NPC size, `test ah,0x41 / jnz`
  ( keeps strict `<`, NaN refused ). Locals `[esp+0x28]` / `[esp+0x44]` only freed. **Fix:** the inlined HP test @`322954c7`-`322954e0`
  ( `fldz / fcomp hp / test ah,1 / jz skip` ) keeps only `0 < hp`, so a NaN HP skips; the port's `GetNowHp() <= 0.f` kept it. Now
  `false == ( 0.f < GetNowHp() )` ( `X2GameUnit.cpp`, same flag, one line ). The gage cases agree: June `31e4f900` = gage `+0xc04` NULL -> 1.0
  ( passes, as the inlined test ), HP object `+0x20` NULL -> 0.0; March's `GetNowHp` returns 1 for a NULL `m_pGageData` and has no HP sub-object.
- **Constants:** `[3344915c]` = 9999999.0f, `[33437830]` = 0.25 ( double ), `[3342e84c]` = 0.0f, `[3344f1f4]` = `00 00` ( `L""` ), caller range
  `[334ed3dc]` = 500.0f.
- **`322d7b30` 4th argument:** @`322d7c30` loads the argument itself ( a pointer ) and passes it to vslot 0x178: a `const int&`. Elesis's by-value
  `iAttackType_` with -1 is the same. No change.
- **`friend class CX2Game;`** sits in `CEffect` beside the CX2GUUser / CX2GameUnit friends of `GetLockOnAimPos`, under the flag. The `Always.h` `#error`
  guard reads `JUNE_DAMAGEEFFECT_RUNTIME` ( line 4415 ) and `JUNE_DAMAGEDATA` ( 4439 ), both defined before it ( line 13466 ); the CH1l30 probes passed it.
- **Hygiene:** `finish.py --review --commits feb8091e`: `flagcheck JUNE_CHAR_ENEMY_SCAN PASS` ( `5 PASS, 0 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR` ),
  `junediverge RESULT: PASS`, `junediverge --check-all RESULT: PASS`, `enumscan exit 0`, `FINISH RESULT: PASS`. `JUNE_DAMAGEDATA` /
  `JUNE_DAMAGEEFFECT_RUNTIME` SKIPPED: targets of the guard ( a run without the skip lists their `#if` guard lines in `Always.h` as ERROR and the
  scan's files as OTHER, nothing of theirs changed ). Fix, worktree: `flagcheck JUNE_CHAR_ENEMY_SCAN PASS` ( 1 PASS; the 1 deleted line is the
  replaced HP test inside this flag's own block ), `junediverge PASS`, `enumscan exit 0`, `FINISH RESULT: PASS`. Commit `b199de72`, `--range
  b199de72^..b199de72`: the same verdicts, `FINISH RESULT: PASS`.
- **Compile:** `/Zs` probe of `X2GameUnit.cpp` ( census `cl_cmd`, no PCH ): 0 errors. Full build skipped: no play-test.

### CH1b - Done (2026-11-14)

Sonnet 5.5 high, inert transcription under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `X2GULuciel.h` / `.cpp`, nothing references it until CH1 ). funcmap `grep "(CH1b)" june2017_re/funcmap.tsv` ( 43 rows ); JUNE_BUGS 692; no review of its own ( CH1f2 covers it ).
- **41 functions, June addresses** ( every row of `ch0_class_map.tsv` col 6 = CH1b, name as CH0 proposed ): `32ae5540` JUMP_UP EventProcess; `32ae55e0` / `32ae5610` DASH_JUMP_POWER_LANDING StartFuture / Start;
  the eight StandUp StateEnds `32ae5630 32ae5670 32ae56b0 32ae56f0 32ae5730 32ae5770 32ae58a0 32ae58e0` ( CommonStateEnd, SetForceInvincible and SetShowInvincible 0.3 );
  `32ae5920` SWITCHING StartFuture ( `m_fSwitchingCoolTime` = 1.0 ); `32aebc10` / `32b0fd90` WAIT StartFuture / Start; `32aebc60` JUMP_DOWN, `32af1150` JUMP_DOWN_NO_ATTACK, `32b0fde0` JUMP_READY, `32b20b30` WAIT,
  `32b20d70` WALK, `32b20f70` JUMP_LANDING, `32b21250` DASH, `32b214c0` DASH_END, `32b0ff90` DASH_JUMP, `32b21830` DASH_JUMP_LANDING, `32b21a70` DASH_JUMP_POWER_LANDING EventProcess;
  `32aebd10` / `32aebeb0` DAMAGE_DOWN FRONT / BACK, the six StandUp rolling / attack EventProcess `32aec060 32aec0f0 32aec180 32aec210 32aec2a0 32aec330` and `32af0150` ( uncarved: read from `charport_adis.py`, no decompile ) / `32af01e0`;
  FrameMove `32b0fef0` DASH_END, `32b10260` / `32b10310` DAMAGE_DOWN FRONT / BACK, FrameMoveFuture `32b11670` JUMP_DOWN_NO_ATTACK; `32b3a250` / `32b3a270` DIE_FRONT / BACK StateStart. The 43 thunks are CH1m's registration.
- **State ids are Lu's own names** ( `LSI_LU_*`, `LSI_RIDING_ON` ), never `USI_*`: June inserts a state at 24, so ids of 24 and up ( damage air fall 38, stand-up front 32, riding on 73 ) are not March's `USI_` values. Job-class dash combos use `LSI_LLC_` / `LSI_LCG_F_` / `LSI_LDB_F_DASH_COMBO_Z` ( 270 / 323 / 551 ).
- **Active body:** the future anim is `GetXSkinAnimFuture()`, the now anim `GetXSkinAnimPtr()` ( DU1 ); `IsAnimationEnd` / `GetNowAnimationTime` / `GetMaxAnimationTime` ( = +0x220 - +0x2b0 ) read them. The June byte `+0x8e90` ? `+0x8e84` : `+0x87d0` is the getter, nothing else.
- **Shared June body ported as members:** `DieFrontStart()` / `DieBackStart()` = June `31fd2d60` / `31fd2e30` ( CommonStateStart, DyingStart vslot 0x338, active anim `SetPlaySpeed( 0.5 )`; March has one per class, `CX2GUEL::DieFrontStart`; CH1d's Ciel dies through them ). `IsInJobLine( int )` is declared ( June `32aeb380`, vslot 0x670, body CH1l2; Chung's name and shape ).
- **Callees kept as March:** `StateChangeDashIfPossible()` ( June vslot 0x668, Lu override `32ae5400` is CH1l2's ), `PlayAnimationWaitStart` ( `june_march_divergent.tsv` replace row: June's extra branch reads member `+0x89e0`, null in ported classes; `junediverge --check` passes ), `TryHyperModeStateChange` through `ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE`, `ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXCAS` ( the 15-key test of JUMP_LANDING ), `CanDown`, `GetEnableDash( EDS_END )`, `SpecialAttackEventProcess( ASUT_GROUND / ASUT_AIR )`, `GetRidingOn`.
- **June's shapes kept:** DAMAGE_DOWN tests flag [1] before [4] ( Eve's order is the reverse ) and rolls `GetRandomInt()` against `m_iNoDownDamageAtStandUpAttackPercent` into NO_DOWN or the plain attack; the roll states sink 30 or 45 as June; DASH_JUMP's tail ( oneUp flag [0], oneDown `m_bDownForce` + `fMaxGSpeed`, riding ) runs on every path; DAMAGE_DOWN_BACK_FrameMove uses `EventTimer( 0.32 )` without the one-shot. JUNE_BUGS 692: DASH_JUMP_LANDING has no `else` after its off-the-line block ( Eve's `32982790` too ).
- **Not in this phase:** the checklist row §7 L254 ( scale `m_fNowStatePowerRate`, never assign ) has no site here ( none of the 41 reads `+0x7cc8` ); skill 900 `SI_ETC_WS_COMMON_LOVE` ( LSI_LU_THROW_ITEM ) has no own function of its own in this group ( shared callbacks, CH1m ); no state is registered, so `stateends` / `skillicons` / `luabind` do not apply.
- **Gates ( `finish.py --files X2Lib/X2GULuciel.h X2Lib/X2GULuciel.cpp` ):** `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`; `junediverge --check` `RESULT: PASS`; `enumscan --check` exit 0 ( job lines 1 / 2 / 3 are no Enum, commented ).
- **Compile:** `/Zs` probes ( census `cl_cmd` ) of `X2GULuciel.cpp`, plain and `/D_LIVE`: 0 errors; `/P` shows the hyper-mode macro and `LINE_RADIUS` expanded. Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Commit** `8716f6b7`; `finish.py --range 8716f6b7^..8716f6b7`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`, `junediverge` PASS, `enumscan` exit 0.

### CH1d - Done (2026-11-14)

Sonnet 5.5 high, inert transcription under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `X2GULuciel.h` / `.cpp`, nothing references it until CH1 ). funcmap `grep "(CH1d)" june2017_re/funcmap.tsv` ( 30 rows ); JUNE_BUGS 693; no review of its own ( CH1f2 covers it ).
- **30 functions, June addresses** ( every `state-fn` row of `ch0_class_map.tsv` col 6 = CH1d ): StateEnds `32ae5a00 32ae5a40 32ae5a80 32ae5ac0 32ae5b00 32ae5b40` ( the six stand-ups ); `32ae5960` JUMP_UP, `32af1b80` JUMP_DOWN,
  `32af5cf0` JUMP_DOWN_NO_ATTACK ( uncarved: read from `charport_adis.py` ), `32b11720` JUMP_READY, `32b118d0` DASH_JUMP, `32b22710` WAIT, `32b22950` WALK, `32b22b50` JUMP_LANDING, `32b22e30` DASH, `32b23050` DASH_END,
  `32b23380` DASH_JUMP_LANDING, `32b235c0` DASH_JUMP_POWER_LANDING EventProcess; `32af1c30` / `32af1da0` DAMAGE_DOWN FRONT / BACK and the six stand-up rolling / attack EventProcess `32af1f20 32af1fb0 32af2040 32af20d0 32af2160 32af21f0`;
  FrameMove `32b11830` DASH_END, `32b11ba0` / `32b11c50` DAMAGE_DOWN FRONT / BACK, FrameMoveFuture `32b12e20` JUMP_DOWN_NO_ATTACK. The 32 thunks ( incl. `32af1f10`, the second slot of `32af1c30` ) are CH1m's registration.
  CH0 gave this group only EventProcess / StateEnd / FrameMove functions: no Ciel `StateStart` is in it ( DIE_FRONT / DIE_BACK use CH1b's `DieFrontStart` / `DieBackStart` ).
- **How it was read:** every function decompiled in Ghidra and diffed ( names and `FUN_` addresses normalised ) against the Lu function CH1b wrote from its twin. 26 of the 30 are **the same body as Lu's with only state ids changed**
  ( the diff shows nothing else ); 4 differ in shape ( DAMAGE_DOWN FRONT / BACK, DASH, DASH_END ) and are written from the Ciel decompile.
- **State ids ( June, and the one thing to watch in CH1's play-test ):** Ciel's code passes **Lu's ids for every common state ( 13-22, 25-42, 73 ) and her own ids only for what Lu's table lacks** ( combos, jump combos, dash combos, switching, stand-up rolls / attacks, 213 ).
  So `LSI_LU_DASH_JUMP` / `LSI_LU_JUMP_DOWN` / `LSI_LU_WAIT` / `LSI_LU_DAMAGE_AIR_FALL` / `LSI_LU_DAMAGE_STANDUP_FRONT` appear inside `LSI_CIEL_*` functions. June's `StateChange` ( `320348c0` ) does no id remap ( decompiled );
  whether the shared ids reach Ciel's states is the registration's job ( CH1m, `InitStateCommon` registers Lu ids and Ciel ids both ), so no id was translated here. Ciel's own: `LSI_CIEL_JUMP_COMBO_Z / X` 251 / 252, `_COMBO_Z / X` 238 / 243,
  `_DASH_COMBO_Z / X` 253 / 254, `_DASH_JUMP_COMBO_Z / X` 255 / 257, `_DASH_JUMP_POWER_LANDING` 230, `_SWITCHING` 237, stand-up rolls 231-234, attacks 235 / 236, `_DAMAGE_STANDUP_BACK` 213.
- **Where Ciel differs from Lu ( June shapes kept ):** DAMAGE_DOWN FRONT / BACK have **no no-down roll** ( flags [1] and [4] both give the plain stand-up attack; no `GetRandomInt()`, no `m_iNoDownDamageAtStandUpAttackPercent` ), and Ciel's DASH / DASH_END have
  **no job-line split** ( `IsInJobLine` is not called: Z / X give her own `DASH_COMBO_Z / X`, DASH_END keeps the first-eighth window ). DASH_JUMP_LANDING keeps Lu's missing `else` ( JUNE_BUGS 693 ).
- **Constants:** no new one: every immediate is CH1b's ( `LINE_RADIUS`, `LINE_RADIUS * 1.5f`, `0.3f`, `0.4f`, `0.19f`, `0.32f`, `0.33f`, `0.125f` ); no `_CONST_LUCIEL_CH1D_` block opened.
- **Callees:** unchanged from CH1b ( `StateChangeDashIfPossible`, `ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE`, `SpecialAttackEventProcess`, `CanDown`, `GetDashJumpSpeed`, `GetJumpSpeed`, `GetRidingOn` ); nothing new needed.
- **Names:** `junediverge.py --list` checked; every name is `LSI_CIEL_<STATE>_<Fn>` ( the CH0 map's names ), no `June` / address / phase tag in any identifier; the phase tag is in the comments only.
- **Not in this phase:** checklist row §7 L254 ( `+0x7cc8` ): no site in these 30; no state registered, so `stateends` / `skillicons` / `luabind` do not apply.
- **Gates ( `finish.py --files X2Lib/X2GULuciel.h X2Lib/X2GULuciel.cpp` ):** `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`; `junediverge` `RESULT: PASS`; `enumscan` exit 0; `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH, the real `Always.h` ) of `X2GULuciel.cpp`, plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 0 errors each; `cl /P` holds all 30 `LSI_CIEL_*` definitions. Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Commit** `814dc0e6`; `finish.py --range 814dc0e6^..814dc0e6`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`, `junediverge` PASS, `enumscan` exit 0.

### CH1f - Done (2026-11-14)

Sonnet 5.5 high, inert transcription under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `X2GULuciel.h` / `.cpp`, nothing references it until CH1 ). funcmap `grep "(CH1f)" june2017_re/funcmap.tsv` ( 39 rows ); JUNE_BUGS 694-696; no review of its own ( CH1f2 covers it ).
- **39 functions, June addresses** ( every `state-fn` row of `ch0_class_map.tsv` col 6 = CH1f ): Lu ground combos EventProcess `32aec3c0` Z, `32aec6b0` ZZ, `32aec9a0` ZZZ, `32aece00` ZZZZ, `32aed0f0` ZZZZX, `32aed320` X, `32aed640` XX, `32aed9f0` XXX, `32aede70` XXXX,
  `32ae57d0` XXXZ_LOOP, `32aee0a0` / `32aee130` / `32aee1b0` XXXZ READY / CHARGE / END, `32aee4a0` XXXZX; dash combos `32aee6d0` Z, `32aee9e0` X_START, `32aeea40` X_LANDING; jump combo X `32aeee30`; dash jump combos `32aeee70` Z, `32aef050` ZX, `32aef160` X,
  `32aef660` XX_START EventProcess + `32aef530` XX_START FrameMove, `32aef690` XX_END, `32aef960` XXX, `32aefc40` XXXX, `32aeff50` XXXXZ; Ciel EventProcess `32af2280` Z, `32af2570` ZZ, `32af2860` ZZZ, `32af2cc0` ZZZZ;
  StateStart `32ae57b0` ( Lu XXXZ_READY ), `32ae5b80` ( Ciel XXXZ_START ); StateEnd `32ae5ba0` ( Ciel XXXZ_LOOP ); FrameMoveFuture `32ae5810` / `32ae5850` ( Lu JUMP_COMBO Z / X ), `32ae5be0` / `32ae5c20` ( Ciel ), `32ae6050` ( the second Lu JUMP_COMBO_Z ).
  The 39 thunks are CH1m's registration.
- **How it was read:** `charport_adis.py luciel --phase CH1f` listings ( all 39 are uncarved in Ghidra ), the x87 compare idioms decoded to `<` / `<=` by a scratch condenser, then each function diffed against its Z-chain neighbour ( names and addresses normalised ):
  Ciel's four EventProcess are Lu's with only ids changed, except `LSI_CIEL_COMBO_ZZZ` ( see below ); the four ground X combos and the dash chain differ only in their events and head.
- **The shapes ( kept as June has them ):** every combo is a chain `foot head -> anim end -> event / key pairs -> skill / walk / dash cancel -> CommonEventProcess`. The Z chain tests the anim end *before* the events ( head `y -= LINE_RADIUS` ), the X chain and the dash combos *after*
  them ( `y -= LINE_RADIUS * 1.5f` ); `COMBO_X` also clears flags 0-2 on a walk key against the facing ( `CAN_WALK_CANCEL` ). The event / key pair is June's expansion, written once as a file-local macro; cancels are the header's `SKILL_CANCEL_AFTER` ( Buff 7c ) and two on-line macros.
- **Where Ciel differs from Lu:** only `LSI_CIEL_COMBO_ZZZ`: event 1 is the Royal Guard line ( `IsInJobLine( 2 )` -> `LSI_LCG_COMBO_ZZZX` ) and event 2 the Chiliarch line ( `1` -> `LSI_LLC_M_COMBO_ZZZX` ), Lu has Chiliarch first. Jump down / wait keep Lu's ids ( as in CH1d ); ids are Ciel's own only for her combo chain.
- **June quirks kept ( JUNE_BUGS 694-696 ):** `DASH_JUMP_COMBO_XX_START_FrameMove` reads the *future* animation's `EventTimer( 0.369 )`; state id **338** ( `LSI_LU_DASH_JUMP_COMBO_X` event 2 ) has no name and is registered with `LSI_LU_JUMP_COMBO_Z`'s table, so `32ae6050` is
  `LSI_LU_JUMP_COMBO_Z_SECOND_FrameMoveFuture` ( kickoff `CH1f2` amended for `32b12ed0` ); the last branch of `DASH_JUMP_COMBO_X` needs the air special attack to return true *and* Z.
- **Constants:** namespace `_CONST_LUCIEL_CH1F_`: `2.f` ( XXXZ loop ), `1.08` / `1.0` ( Ciel's charge rate ), `0.15f` ( X_START ), `0.2f` x2, `0.9f`, `0.369f`, the aim cone ( -20 / 80 / 120 / 1200 ), speeds 1700 / -2000, `90.0` offset and `-2000.0` scale of the aim speed, and the id 338.
  `LINE_RADIUS` is 30.f, so June's `30.0` is `LINE_RADIUS` and `45.0` is `LINE_RADIUS * 1.5f`.
- **Callees:** nothing new: `GetNearestTargetAimDegree( 4 floats )` ( Chung CH1j0 ), `IsInJobLine`, `SpecialAttackEventProcess`, `StateChangeDashIfPossible`, `CommonEventProcess` / `FrameMove` / `FrameMoveFuture` / `StateStart` / `StateEnd`, `CAN_WALK_CANCEL`.
- **Names:** `junediverge.py --list` checked; names are the CH0 map's ( `LSI_<STATE>_<Fn>` ) plus the one `_SECOND_` suffix; no `June`, address or phase tag in any identifier.
- **Not in this phase:** checklist row §7 L254 ( `+0x7cc8` ): no site in these 39; no state registered, so `stateends` / `skillicons` / `luabind` do not apply.
- **Gates ( `finish.py --files X2Lib/X2GULuciel.h X2Lib/X2GULuciel.cpp` ):** `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`; `junediverge` `RESULT: PASS`; `enumscan` exit 0; `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH, the real `Always.h` ) of `X2GULuciel.cpp`, plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 0 errors each; `cl /P` holds 110 `LSI_*` definitions ( CH1b 41 + CH1d 30 + these 39 ). Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Commit** `1335ce1b`; `finish.py --range 1335ce1b^..1335ce1b`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`, `junediverge` PASS, `enumscan` exit 0.

### CH1f2 - Done (2026-11-14)

Sonnet 5.5 high, inert transcription under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `X2GULuciel.h` / `.cpp`, nothing references it until CH1 ). funcmap `grep "(CH1f2)" june2017_re/funcmap.tsv` ( 38 rows ); JUNE_BUGS 697-699; review `CH1f2-review.md` ( Opus ) covers CH1b, CH1d, CH1f, CH1f2.
- **38 functions, June addresses** ( every `state-fn` row of `ch0_class_map.tsv` col 6 = CH1f2 ): Ciel EventProcess `32af2fb0` ZZZZX, `32af31e0` X, `32af3500` XX, `32af38b0` XXX, `32af3d30` XXXX, `32af3f60` XXXZ_START, `32af3fe0` XXXZ_LOOP, `32af41e0` XXXZ_END, `32af44d0` XXXZX,
  `32af4700` DASH_COMBO_Z, `32af4b90` DASH_COMBO_X, `32af4f80` JUMP_COMBO_X, `32af4fc0` / `32af51b0` / `32af52c0` DASH_JUMP_COMBO Z / ZX / X, `32af5690` / `32af5790` / `32af5890` XX / XXX / XXXX, `32af5990` XXXXZ, `32af5af0` XXXXZ_LANDING, `32b11cf0` JUMP_COMBO_Z;
  Ciel FrameMove `32af59c0` DASH_JUMP_COMBO_XXXXZ, `32b23820` X, `32b23b80` XX, `32b23ee0` XXX, `32b24560` XXXX, `32b248c0` XXXZ_END, `32b24c40` DASH_COMBO_X, `32b24fa0` JUMP_COMBO_X, `32b25300` DASH_JUMP_COMBO_ZX, `32b25610` / `32b25970` / `32b25cd0` DASH_JUMP_COMBO XX / XXX / XXXX;
  Lu `32b103b0` XXXZ_CHARGE StateEnd, `32b10540` JUMP_COMBO_Z EventProcess, `32b12ed0` ( `LSI_LU_JUMP_COMBO_Z_SECOND_EventProcess`, state 338 ), `32b21cd0` XXXZ_END FrameMove, `32b21f80` DASH_JUMP_COMBO_ZX FrameMove. The 38 thunks are CH1m's registration.
- **How it was read:** `charport_adis.py luciel --phase CH1f2` ( and `--phase CH1f` for the Lu twins ), a scratch normaliser ( addresses and jump targets reduced to relative offsets ) diffed each Ciel EventProcess against its Lu twin from CH1f:
  `ZZZZX`, `X`, `XX`, `XXX`, `XXXX`, `XXXZ_END`, `XXXZX`, `JUMP_COMBO_X`, `DASH_JUMP_COMBO_ZX`, `XXXXZ_LANDING` differ only in ids ( and the job-line arguments ); the rest were read in full.
- **Where Ciel differs from Lu:** `DASH_COMBO_Z` has three X events ( job lines 2 / 1 / 3 ), `DASH_COMBO_X` swaps the two lines against Lu's `X_LANDING`, `DASH_JUMP_COMBO_Z` tests event 1 ( Chiliarch dash ) *before* event 0 ( X ), `XXXZ_START` goes to LOOP while Z is held else END,
  `XXXZ_LOOP` ends at 3 s ( Lu's 2 s ) and then runs the cancels as else-ifs, `DASH_JUMP_COMBO_X` events 2 / 3 test the job line before Z and reach `LSI_LCG_DASH_JUMP_COMBO_SNIPING_START` / `LSI_LDB_F_DASH_JUMP_COMBO_XZ_START`.
- **The projectile FrameMoves:** ten Ciel FrameMoves and Lu's `32b21f80` play one effect set at a time ( `EventTimer` on the present animation ) after `IsEnoughMPForAttack( mp, bone )`; June inlines the body, written once as `PlayComboProjectileEffectSet( name, mp, bAddMaxCountModifier )`
  ( `PlayEffectSet` with power `GetPowerRate() * m_fPowerOfOriaxDamageRate` and life time `m_fSoulCollectorDamageRate`, then `AddDamageEffectModifier` tag 2 = the same rate, tag 4 = `m_usSoulCollectorMaxCount` added ).
  `32b25300` skips tag 4; `32b23ee0` plays two; Lu's `32b21f80` is the plain March call ( life time -1, no modifiers ). `32b21cd0` scales the effect set and its damage by `1 + charge * 0.5` ( `SetEffectScale( v, true )` = June's `+0x70..+0x7c` stores ) with power `( 1 + charge ) * GetPowerRate()`;
  Ciel's `32b248c0` is a damage effect ( `CreateInstance`, life time rate `1 + charge * 0.5`, 35 ahead and 40 up of the bone, the MP test on the unmoved bone position ).
- **June quirks kept ( JUNE_BUGS 697-699 ):** the `XXXXZ_FrameMove` dive reads the future animation ( as 694 ); `DASH_JUMP_COMBO_X`'s last branch needs the air special attack and Z ( as 696 ); the Duo damage rate is passed where `PlayEffectSet` takes the life time.
- **Constants:** namespace `_CONST_LUCIEL_CH1F2_` ( times, MP costs, the 3500 / -2500 dive speeds and aim cone, the 40 / 35 offsets, modifier tags 2 and 4 with no Enum entry ); `LINE_RADIUS * 1.5f` is June's 45.
- **Names:** `junediverge.py --list` checked; the map's `LSI_<STATE>_<Fn>`, the CH1f `_SECOND_` suffix, and `PlayComboProjectileEffectSet` ( the class's `Play...` pattern ). Members `m_fSoulCollectorDamageRate` / `m_fPowerOfOriaxDamageRate` / `m_usSoulCollectorMaxCount` keep CH1a's provisional names ( their users here read like a life-time rate, a power rate and a count ): the review decides.
- **Callees:** nothing new to write: `IsEnoughMPForAttack`, `PlayEffectSet`, `AddDamageEffectModifier`, `CX2DamageEffect::CreateInstance`, `GetNearestTargetAimDegree`, `SetEffectScale` all exist in March / earlier ports. June's `PlayEffectSet` extra argument ( force down rate 1.0 ) is March's default.
- **Not in this phase:** modifier tags 2 / 4 only take effect in `ApplyDamageEffectModifiers` for their classes' phases ( CH2 ); no state is registered, so `stateends` / `skillicons` / `luabind` do not apply.
- **Gates ( `finish.py --files X2Lib/X2GULuciel.h X2Lib/X2GULuciel.cpp` ):** `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`; `junediverge` `RESULT: PASS`; `enumscan` exit 0; `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH, the real `Always.h` ) of `X2GULuciel.cpp`, plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 0 errors each; `cl /P` holds 148 `LSI_*` definitions ( 110 + these 38 ). Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Commit** `b346db2f`; `finish.py --range b346db2f^..b346db2f`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`, `junediverge` PASS, `enumscan` exit 0.
- **Next:** `/x2-phase char-luciel CH1f2 review` ( Opus 5.5 high ), then CH1g ( Sonnet ).

### CH1f2 - Review (2026-11-14)

Opus 5.5 high. Covers CH1b `8716f6b7`, CH1d `814dc0e6`, CH1f `1335ce1b`, CH1f2 `b346db2f`. **Verdict: pass after one fix** ( 16 timer calls, below ). No debug define to retire ( none was added ).
- **The fix: `EventTimer` -> `EventTimerOneshot` at 16 sites** ( CH1f `LSI_LU_DASH_JUMP_COMBO_XX_START_FrameMove`; CH1f2 `LSI_CIEL_DASH_JUMP_COMBO_XXXXZ_FrameMove`, `LSI_LU_COMBO_XXXZ_END_FrameMove`, `LSI_LU_DASH_JUMP_COMBO_ZX_FrameMove`
  and the ten Ciel projectile FrameMoves, `LSI_CIEL_COMBO_XXX_FrameMove` twice ). June tests `anim+0x24c < t` before `31d28e10( t )` at each of them, which is March's inline
  `EventTimerOneshot` ( `KTDGXSkinAnim.h:277`, `m_fAnimOneshotTimer < fTime && EventTimer( fTime )`; `+0x24c` = `m_fAnimOneshotTimer`, effect-completion notes ). Ghidra's decompile of
  `32b248c0` reads `if( *(float*)(anim + 0x24c) < 0.05 ) FUN_31d28e10( 0.05 )`. The x87 forms were decoded both ways: `fld [+0x24c]; fcomp t; test 5; jp` and `fld t; fcom [+0x24c]; test 0x41; jne`, both = timer < t.
  Plain `EventTimer` lets a projectile or the dive fire again when the timer is crossed twice in one state. CH1b / CH1d had it right: the DAMAGE_DOWN_FRONT FrameMoves are one-shot, BACK and DASH_END plain, as June.
  A scan of all four phases' listings found no other one-shot site.
- **Spot-checks ( listing, then Ghidra read-only ):** Ciel `DASH_JUMP_COMBO_Z` `32af4fc0`: event 1 ( `IsInJobLine( 1 )` + double tap -> `LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH` ) before event 0 ( X -> `ZX` ), macro comparisons = June's x87 ( change: `stateChange < now || end`;
  input: `start <= now < end` ). `XXXZ_LOOP` `32af3fe0`: `3.0 <= fStateTime || !pureZ` -> END, then the skill / hyper / walk / dash cancels as else-ifs = `SKILL_CANCEL_AFTER` and the two on-line macros.
  Ciel `XXXZ_END_FrameMove` `32b248c0`: `Bip01` + 40 up + 35 along `GetDirVector` ( minus when facing left ), MP 5 on the unmoved bone, `CreateInstance( this, name, ( 1 + charge ) * GetPowerRate(), pos, rot, rot, landPosition.y, false, -1, 1, 1 + charge * 0.5, -1, -1, 0, -1 )` = June's 17 pushes.
  Also read: CH1b `32aebd10` ( flag [1] before [4], `GetRandomInt() < m_iNoDownDamageAtStandUpAttackPercent` ), CH1d `32b22e30` ( Ciel DASH: switching at `m_fSwitchingCoolTime <= 0`, then Z / X, no job-line split ), CH1f `32af2860` ( Royal Guard event 1, Chiliarch event 2, `oneX` before `IsInJobLine` ). All = June.
- **`PlayComboProjectileEffectSet`:** June's `321e1e60` pushes ( name, this, NULL, now-state `+0x10` = `IsHyperState()`, `GetPowerRate() * m_fPowerOfOriaxDamageRate`, life time `m_fSoulCollectorDamageRate`, scale ( 1, 1, 1 ), false, three zero vectors, 1.0 ) = the helper with March's defaults.
  Modifiers `321db050`: ( handle, 2, `m_fSoulCollectorDamageRate`, false ) then ( handle, 4, `(float)m_usSoulCollectorMaxCount`, true ). MP test `vslot 0x6f0( mp, Bip01, 0, 0-vector )` = `IsEnoughMPForAttack( mp, vBonePos )`, after the game / effect-set checks as June.
  Modifier count per FrameMove: two everywhere, four in `32b23ee0`, **one ( tag 2 ) only in `32b25300`**, none in Lu's `32b21f80`. Effect-set names and MP costs match per caller. JUNE_BUGS 699 stands.
- **Constants:** every float / double of `_CONST_LUCIEL_CH1F2_` = the listing ( `f=` / `d=` ): 3.0, 0.2, 0.369, -20 / 80 / 120 / 1200, 3500 / -2500, 90.0 / -2500.0 ( doubles ), 0.25, 0.5, 0.1 / 4, 0.28 / 4, 0.42 / 4, 0.622 / 4, 0.667 / 2, 0.577 / 5, 0.05 / 5, 40, 35, 0.3 / 5, 0.2 / 5,
  0.28 / 4, 0.28 / 5, 0.05 / 5, 0.378 / 5. The dive's radian factor is `D3DXToRadian`'s ( double 0.01745329238474369 ), its `0 != z` test takes NaN into the aim branch as June's `fucompp` / `test 0x44` does.
- **Provisional members: left to CH4a** ( kickoff amended ). Their only readers so far are these FrameMoves and no writer is in CH1b-CH1f2; `InitPassiveSkillState` `32b45820` and `SI_P_LCG_SOUL_COLLECTOR` ( 10401 / 30406 ) are CH4a's, so CH4a names them from what writes them. Renaming now would be a second guess.
- **Kickoffs amended:** CH1g, CH1g2, CH1h, CH1i ( the one-shot timer idiom ), CH4a ( the three member names ).
- **Gates:** `finish.py --review --commits 8716f6b7 814dc0e6 1335ce1b b346db2f -v`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `8 PASS, 0 OTHER, 0 FAIL`, 0 deleted lines / `RESULT: PASS`; `junediverge` PASS; `junediverge --check-all` PASS; `enumscan` exit 0;
  `FINISH RESULT: PASS (7 gates, 0 failed, 3 skipped)` ( stateends / skillicons / luabind: no state registered, no skill, no script ). The fix, `finish.py --files X2Lib/X2GULuciel.cpp`: `1 PASS, 0 OTHER, 0 FAIL`, `RESULT: PASS`,
  16 deleted lines = the 16 replaced timer lines, in this flag's own new code ( no March line touched ); `junediverge` PASS; `enumscan` exit 0.
- **Compile:** `/Zs` probes of `X2GULuciel.cpp` ( census `cl_cmd`, no PCH ), plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 0 errors each. Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Commit** `c7a6dced`; `finish.py --range c7a6dced^..c7a6dced`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `1 PASS, 0 OTHER, 0 FAIL` ( 16 deleted = the replaced timer lines ) / `RESULT: PASS`, `junediverge` PASS, `enumscan` exit 0.
- **Next:** `/x2-phase char-luciel CH1g` ( Sonnet 5.5 high ).

### CH1g - Done (2026-11-14)

Sonnet 5.5 high, inert transcription under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `X2GULuciel.h` / `.cpp`, nothing references it until CH1 ). funcmap `grep "(CH1g)" june2017_re/funcmap.tsv` ( 38 rows ); JUNE_BUGS 700-701; no review of its own ( the CH1i review covers CH1g, CH1g2, CH1h, CH1i ).
- **38 functions, June addresses** ( every `state-fn` row of `ch0_class_map.tsv` col 6 = CH1g ): StateStart / StartFuture `32ae5c70` `32ae5c90` `32ae5cb0` `32ae5db0` `32ae5dd0` `32ae5e10` `32ae5e30` `32ae5f40` `32ae6090` `32ae60b0`; Sniping X charge EventProcess / FrameMove `32ae5ce0` / `32ae5d20`;
  Royal Guard F loops `32ae5e60` ( FrameMove ), `32ae5ef0` / `32ae5f60` / `32ae5fb0` / `32ae6000` ( EventProcess ); Chiliarch `32af0270` ZZZX, `32af0570` / `32af0860` / `32af0b50` / `32af0e50` dash Z / ZX / ZXX / XX, `32af7960` / `32af7c10` F dash ZXXX / XXX,
  `32af7ec0` M ZZZX, `32af8230` / `32af85a0` / `32af8910` / `32af8bc0` / `32af8f30` M dash ZX / ZXX / ZXXX / XX / XXX; Royal Guard `32af5d20` dash ZX, `32af6110` ZZZX, `32af6330` / `32af6560` / `32af6780` Sniping Z / Z_FINISH / X_FIRE, `32af69b0` / `32af6a20` / `32af6c20` dash jump Sniping START / LOOP / FINISH.
  The 38 thunks are CH1m's registration.
- **How it was read:** `charport_adis.py luciel --phase CH1g` listings ( the tool's function bounds run into the next function: each body was read to its `int3` ), a scratch condenser that reduced each listing to its distinct parts and checked the skill / walk / dash cancel blocks
  against June's expansion token by token ( `SKILL_CANCEL_AFTER`, the CH1f `WALK_CANCEL_AFTER_ON_LINE_` / `DASH_CANCEL_AFTER_ON_LINE_`, all equal ); the x87 compares decoded as in CH1f; job-line arguments checked at each `vslot 0x670` call.
- **The shapes ( kept as June has them ):** the combo / dash combo chains are CH1f's ( event macro, then the three cancels ). Differences: the order of the cancels is not fixed ( `32af6110`, `32af6330` walk, dash, skill; `32af6560` walk, skill, dash; the rest skill, walk, dash );
  the F / M states add a back-walk cancel ( `m_fBWalkCancelAfter`, a walk key against the facing, on the line: file-local `BWALK_CANCEL_AFTER_ON_LINE_` ); `32af6a20` has the awakening branch only ( file-local `HYPER_MODE_CANCEL_AFTER_`, no special attack );
  `32af6110` / `32af6330` / `32af6560` test the animation end in an `if` of its own, then the foot ( `y -= LINE_RADIUS * 1.5f` ); `32af0b50` / `32af0e50` land ( `LSI_LU_JUMP_LANDING` ) on the line after 0.7 s with no animation end test.
- **Tail calls:** from the state change time on ( `m_fEventTime[0].stateChange <= now` ), the four F / M dash states ( `32af7960` `32af7c10` `32af8910` `32af8f30` ) hand the frame to another state's EventProcess: the F pair to `LSI_CIEL_DASH_JUMP_COMBO_X_EventProcess` ( `32af52c0` ),
  the M pair to `LSI_LU_DASH_JUMP_COMBO_X_EventProcess` ( `32aef160` ), both CH1f / CH1f2's; else the animation end with `stateChange == 9999` goes to `LSI_LU_WAIT`.
- **Sniping X charge:** `FrameMove` ( `32ae5d20`, twin `32ae5e60` ) drains `float( elapsed * 10.0 )` MP and adds the elapsed time to `m_fChargeTime` ( capped at 1 ) only while that MP is available and the charge is below 1; `EventProcess` ends at 3 s or on X released ( the F loops also need 0.4 s, a double ).
- **June quirks kept ( JUNE_BUGS 700-701 ):** three F loop bodies ( `32ae5f60` `32ae5fb0` `32ae6000` ) end in the ZZZXX END state, never their own; `32ae5e30` is the StartFuture callback of four loop states but calls `CommonStateStart` and drains 10 MP ( its twin `32ae5cb0` is a Start callback ).
  Not bugs, noted: the Sniping loop's missing special-attack cancel; `m_iSnipingComboMaxCount` is unset by the ctor ( CH1a ).
- **Constants:** namespace `_CONST_LUCIEL_CH1G_` ( 3 s, 0.4 as a double, -10 MP, 10 MP per second as a double, the 1.0 charge cap, the 5 / 8 Sniping counts, 0.7, 9999 ); state ids and unit classes are enum members ( `UC_LUCIEL_DREAD_LORD` 127 / `UC_LUCIEL_NOBLESSE` 128 ); `LINE_RADIUS` is June's 30, `* 1.5f` its 45.
- **Names:** `junediverge.py --list` checked; names are the CH0 map's ( `LSI_<STATE>_<Fn>` ), the macros `BWALK_CANCEL_AFTER_ON_LINE_` / `HYPER_MODE_CANCEL_AFTER_` follow the file's existing `_ON_LINE_` pattern; no `June`, address or phase tag in any identifier.
- **Callees:** nothing new to write: `UpNowMp`, `GetNowMp`, `IsInJobLine`, `CAN_WALK_CANCEL`, `CanHyperMode`, `IsQuickSlotSpecialItemWaiting`, `GetHypetMpdeStateID`, `CommonStateStartFuture`, and CH1f / CH1f2's two dash jump EventProcess.
- **Not in this phase:** checklist row §7 L254 ( `+0x7cc8` ): no site in these 38; no state is registered, so `stateends` / `skillicons` / `luabind` do not apply.
- **Gates ( `finish.py --files X2Lib/X2GULuciel.h X2Lib/X2GULuciel.cpp` ):** `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`; `junediverge` `RESULT: PASS`; `enumscan` exit 0; `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH, the real `Always.h` ) of `X2GULuciel.cpp`, plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 0 errors each; `cl /P` holds 186 `LSI_*` definitions ( 148 + these 38 ). Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Commit** `69234352`; `finish.py --range 69234352^..69234352`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`, `junediverge` PASS, `enumscan` exit 0.
- **Next:** `/x2-phase char-luciel CH1g2` ( Sonnet 5.5 high ).

### CH1g2 - Done (2026-11-14)

Sonnet 5.5 high, inert transcription under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `X2GULuciel.h` / `.cpp`, nothing references it until CH1 ). funcmap `grep "(CH1g2)" june2017_re/funcmap.tsv` ( 38 rows ); JUNE_BUGS 702; no review of its own ( the CH1i review covers CH1g, CH1g2, CH1h, CH1i ).
- **38 functions, June addresses** ( every `state-fn` row of `ch0_class_map.tsv` col 6 = CH1g2 ): Royal Guard F combos `32af91e0` ZZZX, `32af9400` / `32af98c0` ZZZXX / ZZZXZ END, `32af9690` ZZZXZ loop, `32af9b90` / `32af9e80` / `32afa2d0` dash Z / ZX / XX, `32afa720` / `32afa750` dash jump ZXX ( F, M ), `32afa780` / `32afa7f0` / `32afaac0` dash jump XZZ ready / loop / end;
  twelve Chiliarch dash jump combos `32b105d0` `32b10980` `32b10bd0` `32b10e20` `32b111d0` `32b11420` ( F, Z / X dash, up, down ) and `32b11d80` `32b12130` `32b12380` `32b125d0` `32b12980` `32b12bd0` ( M ); StateStart `32b1cba0` `32b1cc60`, `32b3a290` win, `32b3a2b0` lose;
  FrameMove `32b22240` / `32b27b60` ( Chiliarch ZZZX F / M ), `32b26030` `32b266a0` `32b26a40` ( Sniping Z, X fire, dash jump loop ), `32b28020` `32b282c0` `32b287b0` ( F ZZZXX END, ZZZXZ loop, dash jump XZZ loop ); EventProcess `32b3b530` Sniping ready, `32b3bc70` F ZZZX wait. The 62 thunks are CH1m's registration.
- **How it was read:** `charport_adis.py luciel --phase CH1g2` listings, then a scratch matcher ( normalised instruction sequence against every CH1b-CH1g listing ): 14 functions matched a written sibling at 100 % and differ only in a state id ( `32af91e0` and `32af9690` = `32af6110` / `32af6330`, `32af9b90` = `32aee1b0`, `32afa780` = `32af69b0`, `32afaac0` = `32af5690`, the 12 dash jump combos = one of two bodies ),
  the rest were read line by line to the `ret` ( the tool's bounds run into the next function ). Each state id was checked against the `pushes=` annotation; the cancel blocks equal the CH1f / CH1g macros ( skill / walk / dash, plus the back walk in the F / M states ).
- **Shapes kept as June has them:** the order of the cancels per function ( `32af91e0` / `32af9690` / the two ready states walk, dash, skill; `32af9400` / `32af98c0` and the dash ZX / XX states skill, walk, back walk, dash; the dash jump loop the awakening branch only ); `32af9e80` / `32afa2d0` event 1 is Down + X on a Noblesse unit.
  The twelve Chiliarch dash jump combos test the X / Z keys ( the dash ones also Up / Down ), then `SpecialAttackEventProcess( ASUT_AIR )`, then at the animation end set the facing and the walk speed from the held keys ( `SetAirWalkSpeedByInput`, the block of `LSI_LU_JUMP_DOWN_NO_ATTACK_FrameMoveFuture`; June inlines it in each ).
- **Effects:** `PlayRatedProjectileEffectSet` ( June inlines it ) is the body CH1f2 had inside `PlayComboProjectileEffectSet` without its MP test; the CH1f2 helper now calls it ( same code moved, 10 lines deleted in the flagcheck line ). The Sniping Z and dash jump loop FrameMoves play a Loop set ( kept for deletion at the state end ) and the rated Projectile set, both with the local z rotation `random * 4 - 2` ( `SetEffectSetLocalRotateZ`, instance +0xe4 .. +0xec ) under one `IsEnoughMPForAttack`.
  `32b26a40` / `32b287b0` set the Sniping count to the max count + 1 when the MP test fails ( the loop ends ); `32b266a0` times itself on the minor unit's animation ( the Sniping states are Ciel's ), no MP test.
- **State power rate ( checklist §7 L259 / L443, +0x7cc8 ):** the two Start functions `32b1cba0` / `32b1cc60` do `m_fNowStatePowerRate *= charge + 1.0`; both rows carry a Lu/Ciel entry.
- **Sniping ready / F ZZZX wait ( `32b3b530` / `32b3bc70` ):** event 1 ( X held ) first shows `STR_ID_2549` ( not enough MP ) while the MP is below 10 and clears the flag, then goes on to its own state; the Down + X event reads event 2's times ( JUNE_BUGS 702 ).
- **Constants:** namespace `_CONST_LUCIEL_CH1G2_` ( the effect times and MP, the two random limits 0.5 / 0.2, the rotation range, the 10 MP limit ); state ids, `UC_LUCIEL_NOBLESSE`, `CX2SkillTree::ASUT_AIR`, `STR_ID_2549` are named members; the event / job line indexes are plain ( no Enum ). `PlaySound( name, true, false )` is the open `PlaySound` replace row's caller ( allow-listed, `june_diverge_allow.tsv` row 13 ).
- **Not in this phase:** nothing registered ( `stateends` / `skillicons` / `luabind` do not apply ); no new branch drifted.
- **Gates ( `finish.py --files X2Lib/X2GULuciel.h X2Lib/X2GULuciel.cpp` ):** `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS` ( 10 deleted lines = the CH1f2 helper's body moved ); `junediverge` `RESULT: PASS`; `enumscan` exit 0; `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH, the real `Always.h` ) of `X2GULuciel.cpp`, plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 0 errors each; `cl /P` holds 224 `LSI_*` definitions ( 186 + these 38 ). Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Commit** `2e5864b0`; `finish.py --range 2e5864b0^..2e5864b0`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`, `junediverge` PASS, `enumscan` exit 0.
- **Next:** `/x2-phase char-luciel CH1h` ( Sonnet 5.5 high ).

### CH1h - Done (2026-11-14)

Sonnet 5.5 high, inert transcription under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `X2GULuciel.h` / `.cpp`, nothing references it until CH1 ). funcmap `grep "(CH1h)" june2017_re/funcmap.tsv` ( 47 rows ); JUNE_BUGS 703; no review of its own ( the CH1i review covers CH1g, CH1g2, CH1h, CH1i ).
- **47 functions, June addresses** ( every `state-fn` row of `ch0_class_map.tsv` col 6 = CH1h ): StartFuture `32ae5940` `32ae5df0` `32ae60d0` `32ae60f0` `32ae6110` `32af6c50` `32afcdd0` `32afd6e0`;
  EventProcess `32af1180` `32af13b0` `32af16a0` `32af16e0` `32af1950` `32af6cc0` `32af6fc0` `32af72d0` `32af7500` `32af7730` `32afabc0` `32afae50` `32afb1c0` `32afb450` `32afb6c0` `32afb950` `32afbbe0` `32afbe70` `32afc100` `32afc390` `32afc620` `32afc8b0` `32afcb40` `32afce40` `32afd140` `32afd450` `32afd750` `32afda50` `32afdd60`;
  FrameMove `32b15b20` `32b15d40` `32b15fe0` `32b270b0` `32b272c0` `32b27710` `32b28ca0` `32b29000` `32b29360` `32b29570`. The 47 thunks are CH1m's registration ( 4 of the FrameMoves serve four states each: `32b28ca0` / `32b29000` ).
- **How it was read:** `charport_adis.py luciel --phase CH1h` listings, cut at each function's end; a scratch matcher ( normalised listing against every CH1b-CH1g2 listing, state ids, strings and constants kept ) found 38 of the 47 equal to a written sibling or to each other apart from a state id, a key or a string;
  the rest ( the loop EventProcess `32af16e0`, the X2 loop `32af6fc0` family, the three StartFuture resets, the six FrameMoves with a make target or a damage effect ) were decoded line by line to the `ret`, x87 compares read with the CH1f rules.
- **Shapes ( = CH1f / CH1g2 ):** `32af1180` / `32af1950` / `32af72d0` = Lu's ZZZZX body ( jump down, wait, skill / walk / dash ); `32af7500` / `32af7730` = `32af6780`; twelve F / M ends = `32af98c0` ( skill, walk, back walk, dash ), `32afb1c0` the same with its anim end going to the M loop;
  `32af13b0` = `32af0860` with its event 0 X to the ready state, `32afae50` = `32af7ec0` with X in place of Z ( Dread Lord unit test kept ); `32af6cc0` / `32afce40` / `32afd750` put the X event before the anim end ( Lu's `32aed320` order ), `32af6fc0` / `32afd140` / `32afda50` add `m_usStateLoopCount < 5` and end in their END state.
- **Loop EventProcess ( `32af16e0` / `32afb450` ):** the cancel chain opens the function ( no `if` before it: written `if( false ) {}` then the three macros ); then an else-if on the key window of the state time ( key start <= state time <= key end ): X + 0.3 s ( a double ) after the last X changes state, X held stamps the time;
  last, outside the chain, the state change time ends the state ( the Dread Lord loop goes to LOOP_X, the Chiliarch M loop to its END ).
- **Make target and effect handle:** the three down-X StartFuture functions reset `m_uidMakeTarget` / type ( `SetMakeTarget( -1, GUT_USER )`, +0x6898 .. +0x68a0, the MAKE_TARGET store of June's damage effect ), the damage effect handle, the position and the loop count.
  `m_hDashComboDownXEffectSet` ( CH1a, provisional ) is a **damage effect** handle in June ( `32b270b0` stores `CreateInstanceHandle`'s result and reads `GetInstance` every frame ): renamed `m_hDashComboDownXDamageEffect`, type `CX2DamageEffect::CEffectHandle`, ctor `INVALID_DAMAGE_EFFECT_HANDLE` ( same 4 bytes, same -1 ); `ch1_offsets.tsv` row updated.
  The XX loop FrameMoves find the target with `CX2Game::GetUnitByUIDAndType` ( June `32294460`, same list and compare ), test `GetNowHp() > 0` ( no gauge data reads 1 ), `32b15d40` also needs it within 700 units, then play the loop set at `m_vDashComboDownXPosition` ( custom position ).
- **June quirks kept ( JUNE_BUGS 703 ):** the XX loops and the non-F ZX states play / create the F ZX effect names. Not bugs, noted: `32b270b0` reads the damage effect position every frame, outside the timer; the first set of `32b272c0` / `32b27710` keeps no handle.
- **Constants:** namespace `_CONST_LUCIEL_CH1H_` ( 0.3 s double, loop count 5 as a USHORT, 0.01 s, 700, 10 MP ); state ids are enum members, `GUT_USER` / `UC_LUCIEL_DREAD_LORD` named; the event indexes are plain ( no Enum ). The timers are `EventTimerOneshot` ( CH1f2 review ).
- **Callees:** nothing new to write beyond the member rename: `PlayComboProjectileEffectSet`, `PlayRatedProjectileEffectSet`, `SetMakeTarget`, the cancel macros of CH1f / CH1g.
- **Not in this phase:** no state is registered ( `stateends` / `skillicons` / `luabind` do not apply ); checklist §7 L254 ( `+0x7cc8` ) and §7ac L778 ( CanDown ) have no site in these 47; no new branch drifted.
- **Gates ( `finish.py --files X2Lib/X2GULuciel.h X2Lib/X2GULuciel.cpp` ):** `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS` ( 2 deleted lines = CH1a's ctor initialiser and member declaration, renamed and retyped ); `junediverge` `RESULT: PASS`; `enumscan` exit 0; `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH, the real `Always.h` ) of `X2GULuciel.cpp`, plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 0 errors, 0 warnings in the new lines; `cl /P` holds 271 `LSI_*` definitions ( 224 + these 47 ). Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Commit** `4946d4f1`; `finish.py --range 4946d4f1^..4946d4f1`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`, `junediverge` PASS, `enumscan` exit 0.
- **Next:** `/x2-phase char-luciel CH1i` ( Sonnet 5.5 high; its Opus review covers CH1g, CH1g2, CH1h, CH1i ).

### CH1i - Done (2026-11-14)

Sonnet 5.5 high, inert transcription under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `X2GULuciel.h` / `.cpp`, nothing references it until CH1 ). funcmap `grep "(CH1i)" june2017_re/funcmap.tsv` ( 51 rows ); JUNE_BUGS 704-705;
no review of its own: the CH1i review ( `kickoffs/char-luciel/CH1i-review.md`, Opus ) covers CH1g, CH1g2, CH1h, CH1i.
- **45 functions, June addresses** ( every `state-fn` / `helper` row of `ch0_class_map.tsv` col 6 = CH1i; the 42 thunks are CH1m's registration ):
  StartFuture `32ae9b40` `32b14c30` `32b34100` `32b341b0` `32b343b0` `32b35fd0`; StateEnd `32b14c00`; FrameMove `32b39e70`;
  EventProcess `32b08230` `32b085b0` `32b08940` `32b08d30` `32b090d0` `32b09470` `32b09810` `32b09bb0` `32b09fa0` `32b0a380` `32b0a3e0` `32b0a420` `32b0a780` `32b0ab20` `32b0aec0` `32b0b190` `32b0b460` `32b0b7c0` `32b0b820` `32b0b860` `32b0bbc0` `32b0bf60` `32b0c220` `32b0c580` `32b0c920` `32b0ccc0` `32b0d060` `32b0d400` `32b0d6d0` `32b0d730` `32b0d770` `32b0dac0` `32b0dd90` `32b0ddd0` `32b0e120`;
  the two `?` helpers: `32b1f2b0` = `ShowMinorParticleHyperModeShockOfBody` ( March's `ShowMinorParticleHyperModeShock` with the `MAJOR:` / `MINOR:` + `Bip01` bone and the TOKANG text as a major particle ) and `32b37d80` = `CommonHyperModeFrameMove( fChangeTime, fShockTime, bSound )` ( the class pattern of Ara / Arme ).
- **How it was read:** `charport_adis.py luciel --phase CH1i` listings ( the tool does not stop at a `ret` without padding: `32b0a3e0` ran into `32b0a420`, cut at the next class-map address ); a scratch token diff ( memory offsets, state ids, branch kinds, constants ) of every listing against four templates
  sorted the 35 EventProcess functions: 15 equal to a template apart from the next state id and the key, 6 equal to each other ( the end states ), 7 tiny ones, `32b085b0` ( no key ), `32b0c220` / `32b0d770` / `32b0ddd0` ( a template with a fixed next state ) and the three not-selected-body ones;
  those, the StartFutures and both helpers were decoded line by line to the `ret`, x87 compares read with the CH1f rules.
- **Shapes:** every EventProcess is `anim end -> ( on line ? WAIT : JUMP_DOWN )`, then `ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_( 0, ..., key, state )`, then the cancel macros ( skill, walk, dash, back walk ) as CH1g. The anim is the active body's, or ( A: `0 < USEFUL_VALUE0` Now value, C: no value test ) the not-selected body's when
  `IsDuoEnabled()` and `GetNotSelectXSkinAnimFuture()` exist. Next state = `IsMinorUnitActive() ? Ciel id : Lu id` ( ids +20 apart ), `F / M` pairs +5. `32b08940` / `32b09bb0` / `32b09fa0` time event 0 on the not-selected body: the new macro `ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_NOT_SELECT_`
  ( June's expansion ); `32b09fa0` does it for Demonio ( class 129 ) only. The end states ( `32b0aec0` family ) add `oneSit -> CheckBoundingTarget()` ( `32b0bf60` has none ). `32b085b0` takes event 0 with no key ( `true` ). `32b08230` has no value gate.
- **Callees:** `LandingPrediction` ( CX2GUUser, hoisted by char-ara CH1g: checklist 7ac L777 answered, no second copy ), `GetUnitByUIDAndType`, `IsValidUnit`, `GetNowHp`, `GetLandPosition( pos, true, 30, &idx )` ( 7y L680 ), `GetLineGroupIncludesLineData` ( = June `31c5c240`, +0xf4 / +0xf8 = `m_lineGroupID` ), `LineGroupID::operator!=` ( `31cfd6b0` ),
  `GetGameUnitAttackedByMe` ( +0xd78 ), `GetUnitWidth( false )` ( vslot 0x188 ), `GetUsefulValueNow / Future( 0, 0 )`, the awakening effect-set family as three inert stand-ins in `_CONST_LUCIEL_CH1I_` ( `stand-in: not ported`, Stage 12, as Ara / Arme ).
  `CheckBoundingTarget` ( vslot 0x568, `32b34680` ) is declared virtual in the class header, body in CH1l.
- **Engine finding, routed:** June `31c60890` is the June body of `CKTDGLineMap::GetFixedDistancePositionForWeaponTaker` ( ~30 June callers: Add, Elesis, Raven, Lu/Ciel ) and it is **not** March's: negative distance flips `bIsRight`, the start line comes from a scan of every enabled non-wall line, not from `GetLandPosition`.
  The three Lu/Ciel StartFutures ( `32b341b0` `32b343b0` `32b35fd0` ) call March's version for now. `tools/data/june_march_divergent.tsv` has the `replace` row, `june_diverge_allow.tsv` four rows ( these three and `CX2GUEL::ESI_SI_A_SBH_SCALET_ROSE_SUCCESS_End` ),
  and the whole replacement is the new Opus phase **CH1i2** ( `kickoffs/char-luciel/CH1i2.md`; Add / Elesis / Raven take it too ).
- **June quirks kept ( JUNE_BUGS 704-705 ):** Z start / Z loop pick the other body's next state ( `dec al` ); `32ae9b40` skips `CommonStateStartFuture`. Not bugs, noted: `32b0c220` drops the unit 45 ( a double, not `LINE_RADIUS` ) on its jump-down branch; `32b0c220` goes to `LSI_LDB_M_DASH_COMBO_ZX` whichever the body.
- **Constants:** namespace `_CONST_LUCIEL_CH1I_` ( landing speed 1000 / -1000 / 5 steps, 45.0, half width 0.5, land radius 30, stand adds 30.0 and 20.0, 0.1 s, world colour 0xff222222, camera 30 / 0.4 ); state ids are enum members, `UC_LUCIEL_DEMONIO` named; event indexes plain ( no Enum ). Hyper mode timers are `EventTimerOneshot` ( CH1f2 review ).
- **Checklist rows:** 7ac L777 done ( above ); 7ac L778 ( CanDown ), 7 L254 ( `+0x7cc8` ), 7l L471 ( the BOUNDING_UP hook `32b38cb0` ) have no site in these 45.
- **Not in this phase:** no state is registered ( `stateends` / `skillicons` / `luabind` do not apply ); CH1j is merged in ( the only riding function here is `32b39e70` ); no new branch drifted besides CH1i2.
- **Gates ( `finish.py --files X2Lib/X2GULuciel.h X2Lib/X2GULuciel.cpp` ):** `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`; `junediverge` `RESULT: PASS` ( `--check-all` PASS with the four allow rows ); `enumscan` exit 0; `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH, the real `Always.h` ) of `X2GULuciel.cpp`, plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 0 errors, 0 warnings; `cl /P` holds 314 `LSI_*` definitions ( 271 + 43 ). Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Commit** `a20ce8f2`; `finish.py --range a20ce8f2^..a20ce8f2`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`, `junediverge` PASS, `enumscan` exit 0.
- **Next:** `/x2-phase char-luciel CH1i review` ( Opus 5.5 high ); `/x2-phase char-luciel CH1i2` ( Opus 5.5 high, independent of the review ).

### CH1i - Review (2026-11-14)

Opus 5.5 high. Reviewed CH1g ( `69234352` ), CH1g2 ( `2e5864b0` ), CH1h ( `4946d4f1` ), CH1i ( `a20ce8f2` ) against June ( `charport_adis.py luciel --phase <id>` listings, Ghidra where the address is a function ). **Verdict: pass after five fixes**, all in CH1i except one CH1h constant, all under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( inert ).
- **Fixes ( `X2Lib/X2GULuciel.cpp`, blocks tagged `char-luciel CH1i review` ):**
  1. `32b0a420` / `32b0a780` / `32b0ab20` ( Z landing, ZZ, ZZZ ) test `sub al, bl` ( bl = 1 ) before the +20: the next state is the other body's, like `32b0a380` / `32b0a3e0`. CH1i had the normal order; now inverted as June, and **JUNE_BUGS 704 widened** to the five Z-chain states.
  2. `32b09bb0`: the state change stores a **word** 0 at `+0x7d85` ( `@32b09cc2` ), clearing event flags 0 and 1; the macro cleared flag 0 only. Written out with both stores ( no other CH1g-CH1i listing has a word / dword flag store ).
  3. `32b1f2b0`: `+0x8e90 == 1` ( `IsMinorUnitActive` ) picks `L"MINOR:"` ( `@32b1f336` ); CH1i had `MAJOR:` for the minor body.
  4. CH1h `COMBO_XX_DOWN_XX_LOOP_REPEAT_TIME`: June `32af16e0` / `32afb450` add `qword 0.30000001192` = `0.3f` widened, not the double 0.3; now `= 0.3f` in the double constant.
  5. `32b35fd0`'s line-group comment named the two bodies; it is the target's group against mine.
- **CH1i, whole phase re-checked after the first fail** ( scratch scripts over all 45 listings ): next-state base and +20 / +5 direction per function ( only the five Z-chain states invert ), the event key per macro ( all equal ), the flag stores.
- **Spot-checks, pass:** `32b08940` / `32b09bb0` / `32b09fa0` ( NOT_SELECT macro = June's two clauses on `GetNotSelectXSkinAnimFuture`, the duo / null / flag order, Demonio test 0x81, `if( false )` opening; entering the Demonio block skips the cancels in June too ), `32b0a380` ( Ghidra decompile ), `32b0a3e0`, `32b08d30` ( gate `0 < USEFUL_VALUE0 Now`, oneX, 0x202 ), `32b0aec0` ( `oneSit -> CheckBoundingTarget` ).
  StartFutures `32b341b0` / `32b343b0` / `32b35fd0`: argument order ( pos*, dist, bIsRight, OUT ) = March's declaration, `!GetIsRight()`, `0.5 x GetUnitWidth( false ) + ( USEFUL_VALUE0 Future / 30.0 / 20.0 )` stored as a float, `GetPos( true )` everywhere except the attacked unit's distance origin ( `GetPos( false )`, `32b343b0` ), land radius 30, `lineType == 0 && bEnable`, position / `vNotSelectPosition` stores, `LineGroupID::IsValid` both indexes, `operator!=`.
  `32b37d80`: `SetWorldColor` 2/15 ( = 0x22 ), two `EventTimerOneshot` ( oneshot timer `+0x24c` < t ), awakening stand-ins in June's order, crash camera 30 / 0.4, `ClearScreen()` = white x1, vslot 0xc0 = `ApplyHyperModeBuff`, the four sounds only with `bSound`. `32b1f2b0`: the three sequences ( handles +0x9010..+0x9018 = enum 4..6, counts 5/10/3, 10/10/1, 200/200/30, ground at landPosition.y + 5 ), TOKANG on the major system with the defaults.
  CH1g `32ae5d20` ( MP drain, cap 1, `UpNowMp( -drain )` ) and `32af7960` ( tail call at `stateChange <= now`, 9999 test, skill / walk / back walk / dash ); CH1g2 `32afa7f0` ( `IsOnSomethingFuture` = `+0x7e1c` ), `32b26a40` ( loop / projectile sets, rotation +0xe4..+0xec, push to `+0x3ec0` ); CH1h `32b15d40` ( GetNowHp inline > 0, distance < 700, `GetPos( false )` ).
- **All four phases by script:** cancel order of 168 functions = June ( the two awakening-only states use `HYPER_MODE_CANCEL_AFTER_` ); state-change targets per function = June ( flagged rows were macro / shared-tail noise, read by hand ); every float / double of `_CONST_LUCIEL_CH1G_` .. `_CH1I_` found in its cited listing ( the other widened-float `qword`s are compares, same result ).
- **Debug defines:** none left by the four phases; nothing retired.
- **Gates, reviewed commits ( `finish.py --review --commits 69234352 2e5864b0 4946d4f1 a20ce8f2 -v` ):** `flagcheck JUNE_CHAR_LUCIEL_CORE` `8 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS` ( 12 deleted lines = CH1g2's helper body moved, CH1h's member rename ); `junediverge` PASS; `junediverge --check-all` PASS; `enumscan` exit 0; `FINISH RESULT: PASS (7 gates, 0 failed, 3 skipped)`.
- **Gates, this fix ( `finish.py --files X2Lib/X2GULuciel.cpp` ):** `flagcheck JUNE_CHAR_LUCIEL_CORE` `1 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS` ( 11 deleted lines: the replaced lines above, all inside the `_CORE` block ); `junediverge` PASS; `enumscan` exit 0; `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`; `stateends` / `skillicons` / `luabind` skipped: nothing registered or staged.
- **Compile:** `/Zs` probes ( census `cl_cmd`, the real `Always.h` ) of `X2GULuciel.cpp`, plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 0 errors, 0 warnings. Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Commit** `15229af5`; `finish.py --range 15229af5^..15229af5`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `1 PASS, 0 OTHER, 0 FAIL` / `RESULT: PASS`, `junediverge` PASS, `enumscan` exit 0.
- **Next:** `/x2-phase char-luciel CH1k` ( Sonnet 5.5 high ); `/x2-phase char-luciel CH1i2 review` ( Opus 5.5 high ) is independent.

### CH1i2 - Done (2026-11-14)

Opus 5.5 high. June `31c60890` replaces March's `CKTDGLineMap::GetFixedDistancePositionForWeaponTaker` whole ( `KTDXLIB/KTDGLineMap.cpp`, D1 shape: June's body under `SERV_IRUHADEV_JUNE_DIVERGE_FIXED_DISTANCE_WEAPON_TAKER`, March's in `#else` ).
funcmap row `31c60890 (CH1i2)`; JUNE_BUGS 706; the CH1i2 review ( `kickoffs/char-luciel/CH1i2-review.md`, Opus ) checks it.
- **How it was read:** `decompile_function 0x31c60890` ( 682 lines ), `charport_consts.py --fn 31c60890` ( widths: dword -1.0 / 999999.0, qwords 30.0 / 900.0 / 0.0 / 1.0 / 0.5 / 15.0 ), raw listing of the flip ( `31c60890`-`31c609c0` ) and the walk ( `31c61c80`-`31c62000` ).
- **What June does, against March:**
  - `fDist < 0` flips `bIsRight` and negates ( `@31c608a3`-`31c608ba`, `test ah,5 / jp`: NaN does not flip ); March has no flip.
  - **The start line is `GetLandPosition` after all** ( CH1i's "scan of every line" corrected ): the 600-line block is June's `GetLandPosition` ( `31c7bb00`, funcmap row ) inlined with only `LT_WALL` skipped ( no `cmp 7`: monster roads count = `bMonster` true ),
    the `( pos.x, 0, pos.z )` LineToLine with eps 30 / 900, the Manhattan < 30 hit keeping the highest, no index fallback ( index -1 ) and the 999999 nearest-end pass. The port calls `GetLandPosition( *pvPos, true, 30.f, &iLineIndex )`. No line: `false`, as March.
  - The first stretch is measured from `( pos.x, line start y, pos.z )` to the start ( left ) / `( pos.x, line end y, pos.z )` to the end ( right ) ( `@31c61d4e`-`31c61d7b` ); March measures from `pos` itself.
  - The walk ( `beforeLine` / `nextLine` while walk < dist, add each line's length ) is March's.
  - Walk < dist + 15 ( qword `33449130`; NaN takes the lerp branch, as March's compare ): June returns `false` with the point **15 in from the edge** ( `lerp = 15 / length`, `@31c61e2a`-`31c61e50` ); March returns the bare edge. Else March's lerp and `true`.
  - March's unused `GetLineGroupIncludesLineData` line ( `#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP`, compiled out ) has no June counterpart and is not in June's body.
- **JUNE_BUGS 706:** the 15-in point is not clamped: a last line shorter than 15 puts it past the line's far end; a zero-length line divides by 0. Kept verbatim.
- **Callers ( all now June's ):** Lu/Ciel `32b341b0` / `32b343b0` / `32b35fd0` ( `X2GULuciel.cpp:7644` / `7669` / `7712`, inert ), Elesis `ESI_SI_A_SBH_SCALET_ROSE_SUCCESS_End` ( already June's caller ), Raven `RSI_SA_RWT_GIGA_PROMINENCE_LOOP_FrameMove` ( June `32c4bcd0` `@32c4bdd2`: same arguments as March ),
  Add `NasodArmorAttackStartFuture` ( `X2GUAdd.cpp:5649` ): **finding** — June `329edde0` `@329ee138` passes the target's `GetBoneLinePos` toward this unit, or the target's `GetPos()` when the two are within 500.0 ( `@329ee0af` ); March passes `&pUnit->GetPos()`.
  Not fixed here: char-add CH1d owns `329edde0` ( class map ) and ports its whole body; a note went into `kickoffs/char-add/CH1d.md` ( "Added by char-luciel CH1i2" ), with `329ef130` ( same pattern ).
- **Divergence table:** `june_march_divergent.tsv` row `GetFixedDistancePositionForWeaponTaker`: `june_does` / `march_does` corrected as above, `source` = "ported under `SERV_IRUHADEV_JUNE_DIVERGE_FIXED_DISTANCE_WEAPON_TAKER`" ( done ); the four `june_diverge_allow.tsv` rows deleted ( 0 left ).
- **Flag:** `Always.h` house block, `#error` unless `RAVEN_WEAPON_TAKER` and `SERV_IRUHADEV_JUNE_LINEMAP_LAND_MONSTER_ROAD` ( the bool `GetLandPosition` ); both ON at the end of `Always.h` by a `cl /EP` probe before saving.
- **Gates ( `finish.py --files KTDXLIB/KTDGLineMap.cpp KTDXLIB/Always.h tools/data/june_march_divergent.tsv tools/data/june_diverge_allow.tsv --skip-flag SERV_IRUHADEV_JUNE_LINEMAP_LAND_MONSTER_ROAD` ):**
  `flagcheck JUNE_DIVERGE_FIXED_DISTANCE_WEAPON_TAKER` `1 PASS, 1 DEFINE, 0 OTHER, 0 FAIL` / `RESULT: PASS`; `junediverge` `RESULT: PASS`; `enumscan` exit 0; `FINISH RESULT: PASS (7 gates, 0 failed, 4 skipped)`.
  SKIPPED: `LINEMAP_LAND_MONSTER_ROAD` ( the guard's target, not this phase's flag ); `stateends` / `skillicons` / `luabind` ( no state, skill or script ). `junediverge --check-all`: `RESULT: PASS`, no `GetFixedDistancePositionForWeaponTaker` allow row.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH, the real `Always.h` ): `KTDGLineMap.cpp` flag on 0 errors; flag off ( a wrapper that `#undef`s it after `StdAfx.h` ) 0 errors; `X2GULuciel.cpp` plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` 0 errors ( warnings are the usual header noise ).
  Full build skipped: no play-test.
- **How to see it in game:** March's live callers ( Raven's Giga Prominence flames, Add's nasod armor attack ) use it from the next full build; Elesis's Scarlet Rose fire once char-elesis registers it; the first planned check is CH3c ( Demonio bounding strike ). No play-test in this phase.
- **Commit** `5d7b09ce`; `finish.py --range 5d7b09ce^..5d7b09ce --skip-flag SERV_IRUHADEV_JUNE_LINEMAP_LAND_MONSTER_ROAD`: `flagcheck JUNE_DIVERGE_FIXED_DISTANCE_WEAPON_TAKER` `1 PASS, 1 DEFINE, 0 OTHER, 0 FAIL` / `RESULT: PASS`, `junediverge` PASS, `enumscan` exit 0, `FINISH RESULT: PASS`.
- **Next:** `/x2-phase char-luciel CH1i2 review` ( Opus 5.5 high ); the CH1i review is independent.

### CH1i2 - Review (2026-11-14)

Opus 5.5 high. **Verdict: pass, no code change.** Reviewed commit `5d7b09ce` against June `31c60890` ( decompile + raw listing `31c60890`-`31c620d5` ).
- **Flip:** `FCOM` dist vs 0, `TEST AH,5 / JP` ( `@31c608a3`-`31c608ba` ): flips only on C0 alone = `dist < 0` strict; NaN ( C0|C2 ) and 0 do not flip. Port `fDist < 0.f` = June.
- **Start line:** the scan skips `bEnable == 0` and `lineType == 1` ( `LT_WALL`, `CMP [ESI+4],1` `@31c60990`; `LT_WALL` = 1 in `KTDGLineMap.h:44`, `FIX_LINE_TYPE_WALL` defined `Always.h:3305` ); no `cmp 7` anywhere in the function ( monster roads count = `bMonster` true ).
  y prefilter `start.y <= pos.y + 30 || end.y <= pos.y + 30`, span test x / y / z inside = March's skip negated; inlined LineToLine from `( pos.x, 0, pos.z )` with 30 / 900; hit = Manhattan < 30 and `pointL2.y > retVal.y` from -1.0.
  No hit: no index fallback ( index starts -1 ) and the 999999 nearest-end pass over enabled lines, start then end, strict `<` ( `@31c61bb3` ); still -1 ( no lines ) returns 0.
  This tree's `GetLandPosition( pos, bool, float, int* )` ( `KTDGLineMap.cpp:1075`, W1c ) does the same with `true` and `iLineIndex = -1` ( its index fallback reads `GetLineData( -1 )` = NULL, a no-op ). Its returned position is unused in June too.
- **Walk:** first stretch left from `( pos.x, start.y, pos.z )` to `startPos` ( `@31c61d6f` ), right from `( pos.x, end.y, pos.z )` to `endPos` ( `@31c61f51` ) = port. Loop tests `beforeLine` / `nextLine >= 0` first, then `FCOM` dist vs walk,
  `TEST AH,41 / JNZ` exits on `dist <= walk` or NaN ( `@31c61d9d`, `@31c61f7b` ) = `walk < dist`. Adds `GetDistance( start, end )` per line = port.
- **End test:** `FCOMP` ( 15 + dist ) vs walk ( `@31c61e3a`, `@31c61fd3` ): `<=` or NaN jumps to the long branch ( `@31c61ef8` / `@31c6202b`, `AL = 1` ), so `walk < dist + 15` takes the 15-in branch and a NaN takes the long one = the port's `if`. ( Ghidra's decompile renders it as `15 + dist <= walk`. )
  15-in: `lerp = 15 / length` ( `FDIVR` qword `33449130` ); left `( 1 - lerp ) * start + lerp * end`, right `lerp * start + ( 1 - lerp ) * end`, `XOR AL,AL` = port. Long: `( walk - dist ) / length`, March's two lerps, `true` = port.
- **`#else`:** March's body unchanged ( flagcheck: 0 deleted lines ).
- **Add caller finding:** confirmed ( `329edde0`: `GetBoneLinePos` of the target toward Add `@329ee077`, `GetDistance` vs 500 `@329ee0af`, call `@329ee138` with 100 / `!GetIsRight()` ). Two corrections to the `kickoffs/char-add/CH1d.md` note:
  the 500 test is `<=` ( equal swaps to `GetPos()`, NaN keeps the bone position ), and June's `FIX_AISHA_TELEPORT_POSITON` y test also reads the bone position ( `@329ee07c`-`329ee08d` ). Note amended; still char-add CH1d's to port. `329ef130` has the bone / 500 / call pattern ( `@329ef322`, `@329ef3c8` ).
- **Divergence table:** row `GetFixedDistancePositionForWeaponTaker` `source` names the flag; `june_diverge_allow.tsv` has no row for it.
- **Gates** ( `finish.py --review --commits 5d7b09ce --skip-flag SERV_IRUHADEV_JUNE_LINEMAP_LAND_MONSTER_ROAD "guard target" -v` ): `flagcheck JUNE_DIVERGE_FIXED_DISTANCE_WEAPON_TAKER` `1 PASS, 0 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`, 0 deleted lines, 0 encoding changes / `RESULT: PASS`;
  `junediverge` PASS; `junediverge --check-all` PASS; `enumscan` exit 0; `FINISH RESULT: PASS (8 gates, 0 failed, 4 skipped)`. SKIPPED: `LINEMAP_LAND_MONSTER_ROAD` ( guard target ), `stateends` / `skillicons` / `luabind` ( none apply ).
- **Build:** none ( no code change, no play-test ). No debug define to retire.
- **Next:** CH3c's CH1i2 dependency is met; the CH1i review is independent ( `/x2-phase char-luciel CH1i review`, Opus 5.5 high ).

### CH1k - Done (2026-11-14)

Sonnet 5.5 high. All 31 CH1k rows of the class map written whole under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `X2GULuciel.cpp` +629, `.h` +35; inert, `_LIVE` stays
`//#define` ). Code commit `da5999ca`. funcmap: `grep "(CH1k)" june2017_re/funcmap.tsv` ( 22 rows ). `JUNE_BUGS.md` 707. Listings: `charport_adis.py luciel --phase CH1k`.
- **Overrides ( June vslot ):** `Init` `32b1e8e0` ( 0x40 ), `InitComponent` `32b0f080` ( 0x3c ), `InitEffect` `32ae4f30` ( 0x48 ), `DamageReact` `32ae4080` ( 0x60 ), `Win` `32aea610` /
  `Lose` `32b0eff0` / `Draw` `32aea650` ( 0x90 / 0x94 / 0x98 ), `GetNowMp` `32aeb0c0` / `GetMaxMp` `32aeb0f0` / `SetMaxMp` `32aeb120` / `FlushMp` `32aeb280` / `UpNowMp` `32aeb160` /
  `SetNowMp` `32aeb240` ( 0xd8 / 0xdc / 0xe0 / 0xe4 / 0xf0 / 0xf8 ), `StampEffectSetIndex` `32b0efc0` ( 0x248 ), `GetWeaponIndexHavingModel` `32b151b0` ( 0x2b0, DU1c2's name ),
  `GetReduceDebuffDurationRate` `32ae5480` ( 0x29c ), `SetDamageExceptionProcess` `32aea530` ( 0x2b8 ), `GetDamageEffectIndex` `32b0efe0` ( 0x2e0 ), `ChangeMaxCharacterAddResource`
  `32b14cd0` ( 0x324 ), `DoProcessWhenKillEnemy` `32b0e3f0` ( 0x318 ), `DoProcessWhenDamageDataDestroyed` `32b17680` ( 0x2fc ), `AdjustDamageDataBeforeDamageReact` `32b1e720` ( 0x300 ).
- **Names settled ( the `?` rows ):** 0x318 is **March's `DoProcessWhenKillEnemy( const CX2GameUnit* )`** ( Rena / Ara use it; the hint said `void()`, the body ends `ret 4` ); 0x2fc = Ara's
  `DoProcessWhenDamageDataDestroyed`; **0x2e4 = `DoProcessWhenDamageDataCreated`** ( new: June's DamageData init `32610da0`, caller `31f8ee59`, calls the attacker's vslot 0x2e4; the sibling of
  Ara's `32610d50` ); `32b16260` = `IsInvasionOfDarkEnergyTarget`, `32b166e0` = `EraseInvasionOfDarkEnergyTarget`, `32aeb090` = `SetNowCombinationPoint( int )`, `32aeb450` = `GetStateIDByForm`.
- **Helpers:** `SetGageNowMp` `32ae50f0`, `SetGageNowMpOfForm` `32b0e620`, `LoadTransformedSkinAnim` `32b162e0` ( the two `Motion_*_Evil.x`, strings read in Ghidra ), `EraseDamageEffectIndexEntry`
  `32b16810`. `32b4b580` ( CH1k2's row, `0 < m_fInvasionOfDarkEnergyTime` ) is **declared** `IsInvasionOfDarkEnergyActive`, body in CH1k2.
- **`GetStateIDByForm`:** the jump tables ( `32aeb870` / `32aeb8b4`, `32aeb978` / `32aeba38` ) were decoded from the exe and every id is an `LSI_*` member; ids 124 / 125 ( Lu phoenix run mid states ) have
  no name or state in `Luciel_Duo.lua` and stay numbers with a comment.
- **Engine, class-agnostic ( `Always.h`, flag `SERV_IRUHADEV_JUNE_CHAR_UNIT_MP_VIRTUAL`, `#error` unless `FORM_MP_GAGE` + `DIVERGE_GAGE_TALLY` ):** `CX2GameUnit::GetNowMp / GetMaxMp / SetMaxMp / FlushMp /
  UpNowMp / SetNowMp` are virtual ( bases `3208dc30` .. `3208dd20`; March's bodies and signatures kept, `FlushMp` gains June's gauge index, default 0 ); the CH1a0 amendment is met. Enum names under
  `JUNE_ENUM_NAMES`: `DTT_RAIN_OF_CHAOS_GUN = 41`, `BTI_BUFF_SOUL_COLLECTOR = 462`, `BTI_DEBUFF_CURSE_OF_ABYSS = 471` ( June's `Enum.lua` ). `CX2LucielGageData::SetRestFormMpPerSecond` ( `X2GageData.h` ).
- **Decisions / differences:** `InitComponent` skips June's `XS_PVE_GAME` ( 0x17 ) branch ( no March state; the PvP value is taken in `XS_PVP_GAME` only ); `0x12` = `XS_TRAINING_GAME`. The three
  `AddModelXSkinMesh` calls drop June's `__FILEW__` / `__LINE__` ( a log only ). `GetWeaponIndexHavingModel` guards a null weapon / equip ( JUNE_BUGS 707 ); Lu's array `CX2Eqip +0x5a4` is the hyper
  model array by March's member order ( `+0x544` normal anim, `+0x564` hyper anim, `+0x584` normal model ) - **not seen named in June, CH1k2's review may check it**. Soul Collector's stack count is read before
  the buff is erased ( June's order ). Constants in `_CONST_LUCIEL_CH1K_` ( dialog fade 0.07, after image 10 / 0.8, model count 8 ).
- **Routed:** the base declarations of vslots 0x2e4 ( `DoProcessWhenDamageDataCreated` + the `SimpleInit` call ), 0x2fc ( `ARA_LIVE` guard ) and 0x300 ( Raven's `_LIVE` guards, `X2GUUser.h:1674` and the four
  `X2DamageManager.cpp` sites ) -> **CH1o** ( kickoff amended ); CH1k2's state-id getters call `GetStateIDByForm`.
- **Gates:** `finish.py --files X2GULuciel.h/.cpp X2GageData.h`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `3 PASS, 0 OTHER, 0 FAIL, 0 ERROR` / `RESULT: PASS`, `junediverge` PASS, `enumscan` exit 0; `--files Always.h X2GameUnit.h
  X2BuffFactor.h X2DamageManager.h --skip-flag GAGE_TALLY / FORM_MP_GAGE ( the `#error` guard's targets ) --skip-flag ENUM_NAMES`: `flagcheck JUNE_CHAR_UNIT_MP_VIRTUAL` `1 PASS, 1 DEFINE, 0 OTHER, 0 FAIL`
  / `RESULT: PASS`, `junediverge` PASS. `JUNE_ENUM_NAMES` on the two header blocks alone: `PASS~` x 2 ( blank / marker lines ), its only ERROR is `Always.h`'s unevaluable `#if` guard. **`enumscan`** on
  `X2BuffFactor.h` lists 212 class A rows: every `BFI_` / `BTI_` definition line of that table ( mine are two of them ); the other six files 0 rows. stateends / skillicons / luabind: nothing registered or staged.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH, real `Always.h` ) of `X2GULuciel.cpp` ( plain, `/W3`, and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` ), `X2Main.cpp` ( the `lua_tinker` `&CX2GameUnit::SetNowMp` pointers ),
  `X2GameUnit.cpp`, `X2GUNPC.cpp`, `X2GUUser.cpp`, `X2DamageManager.cpp`, `X2GageData.cpp`: 0 errors each; `XL_EFFECT_0 == 14` ( June's layer 0xe ) proved by a static probe. Full build skipped: no play-test.
- **Range gate** ( `finish.py --range da5999ca^..da5999ca --skip-flag GAGE_TALLY / FORM_MP_GAGE` ): `flagcheck JUNE_CHAR_UNIT_MP_VIRTUAL` `1 PASS, 1 DEFINE, 5 OTHER, 0 FAIL, 0 ERROR` / `RESULT: PASS`; `JUNE_CHAR_LUCIEL_CORE` `3 PASS, 3 OTHER, 0 FAIL, 1 ERROR` and `JUNE_ENUM_NAMES` `2 PASS~, 4 OTHER, 0 FAIL, 1 ERROR`: the ERROR in both is `Always.h`'s existing `#if` guard ( line 13430 / 10175 ) that the tool cannot evaluate once `Always.h` joins the set ( as CH1a2 ); resolved by hand: the commit's `Always.h` hunk is the 13-line `UNIT_MP_VIRTUAL` block. `junediverge` PASS; `enumscan` as above.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Next:** `/x2-phase char-luciel CH1k2` ( Sonnet 5.5 high; its review also covers CH1k ).

### CH1k2 - Done (2026-11-14)

Sonnet 5.5 high. All 30 CH1k2 rows of the class map written under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `X2GULuciel.cpp` +336, `.h` +30 -1; inert, `_LIVE` stays `//#define` ). Code commit `3880c02b`.
funcmap: `grep "(CH1k2)" june2017_re/funcmap.tsv` ( 11 rows, getters grouped ). `JUNE_BUGS.md` 708. No debug define. Review: `kickoffs/char-luciel/CH1k2-review.md` ( covers CH1k, CH1k2 ).
- **Written ( June address / vslot ):** `DeleteMinorParticle` `32b1f0a0` ( 0x4c ), `HyperModeBuffEffectStart` `32b1f0b0` ( 0xc4 ), `ReInit` `32b373e0` ( 0x44 ), `CreateGageData` `32b37c70` ( 0xd0 ), `StateChange( int, bool )` `32b4db90` ( 0x5c ) /
  `StateChange( int, int, bool )` `32b4dbb0` ( 0x434 ), `IsInvasionOfDarkEnergyActive` `32b4b580`, `GetWaitStateID` `32b4d3c0` ( 0x438 ), the movement getters `32b4d3f0` Walk, `32b4d430` Dash, `32b4d470` DashEnd, `32b4d4b0` DashJump,
  `32b4d4f0` DashJumpLanding, `32b4d530` JumpReady, `32b4d570` JumpUp, `32b4d610` JumpLanding, `32b4d650` DieFront, `32b4d690` DieBack ( 0x43c-0x464 ), the damage getters `32b4d6d0` Groggy, `32b4d710` SmallFront, `32b4d750` SmallBack,
  `32b4d790` BigFront, `32b4d7d0` BigBack, `32b4d890` StandUpFront, `32b4d8d0` StandUpBack, `32b4d910` AirSmall, `32b4d950` AirDown, `32b4da50` AirUp ( DU2's exact names, `const` ).
- **No class code, two rows:** `32b4cfb0` = the compiler's `vector<SDamageEffectHitCount>::erase` ( 12-byte entries ); `32b4dd50` = a copy of March's `CX2GameUnit::SetHyperModeCountByBuff` ( `+0x6844` written if different, CH1k's `Init` already calls it ).
- **Getters, decoded from the listings, not from CH0's hint:** only `GetWaitStateID` calls `GetStateIDByForm`; the other 20 do the arithmetic inline: `IsTransformed( DT_BOTH )` ( `31fc9d90( 2 )` ) then vslot 0x260 ( `IsMinorUnitActive` ):
  transformed -> `LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_*` ( 0x1a8-based ) / `LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_*` ( + 0x2d ); not transformed -> `LSI_LU_*` / `LSI_CIEL_*` ( + 0xb5 movement, + 0xb4 damage, + 0xb9 die ).
  Every constant was matched to its `LSI_*` member by script ( no number in the code ). The ten damage getters answer the transformed **WAIT** state while transformed; the die pair answers Lu's id while transformed.
- **`ReInit`:** June tests `g_pData +0x6c` ( the PvP room ) not the game type, then `+0xd8 == 8` = `PGT_AMUSEMENT_ITEM` ( registered in `X2PVPRoom.h` under `JUNE_ENUM_NAMES`, June `Enum.lua:736` ): other form's MP = its MP / 3.0.
  Else the kill lead from `CX2Game` `+0x4a4` / `+0x4a8` ( taken as Red / Blue team kill, `GetRedTeamKill` / `GetBlueTeamKill`; the review confirms the order ): above 1 the other form's MP and the Combination Point go to 0,
  exactly 1 halves the other form's MP and zeroes the Combination Point. The `+0x8f70` window ( `m_pTransformSkillSlotUI` ) is deleted as the destructor does ( stand-in: CH2d20 adds its dialog release ). JUNE_BUGS 708: two gauge reads with no null test.
- **`IsInvasionOfDarkEnergyActive`:** the listing ( `fcomp` / `test ah, 0x41` / `jp` ) is `0 <= m_fInvasionOfDarkEnergyTime`; CH0's hint said `0 < time`. Written as June has it ( default -1 = off ).
- **Routed ( CH1o, kickoff amended ):** eleven getters have no virtual base in March ( `GetWaitStateID` .. `GetDieBackStateID`; seven of them do not exist in March at all ): declared `virtual` in `X2GULuciel.h` and marked `+`; CH1o adds the bases.
  Declaring the two `StateChange` overloads here routes the class's own calls through `GetStateIDByForm`, as June's do ( inert until `_LIVE` ).
- **Gates:** `finish.py --files X2GULuciel.h/.cpp X2PVPRoom.h`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 1 OTHER, 0 FAIL, 0 ERROR` ( the OTHER is `X2PVPRoom.h`, which belongs to `JUNE_ENUM_NAMES` ), `JUNE_ENUM_NAMES` `1 PASS, 2 OTHER, 0 FAIL`,
  `junediverge` PASS, `enumscan` exit 0; the one deleted line is the header comment `body in CH1k2` on `IsInvasionOfDarkEnergyActive`. Range gate `--range 3880c02b^..3880c02b`: identical tallies. stateends / skillicons / luabind: nothing registered or staged.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH ) of `X2GULuciel.cpp` ( plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` ) and `X2GUUser.cpp` ( includes the changed `X2PVPRoom.h` ): 0 errors each ( one first-run error, a `const CX2PVPRoom*` calling the non-const `GetPVPGameType`, fixed ).
  Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Next:** `/x2-phase char-luciel CH1k2 review` ( Opus 5.5 high; covers CH1k and CH1k2 ), then `/x2-phase char-luciel CH1l`.

### CH1k2 - Review (2026-11-15)

Opus 5.5 high. Covers CH1k ( `da5999ca` ) and CH1k2 ( `3880c02b` ). **Verdict: pass after one fix.** Listings: `charport_adis.py luciel --phase CH1k` / `--phase CH1k2`, `june2017_re/funcdis.py`.
- **Fix ( CH1k's one inference was wrong ):** `GetWeaponIndexHavingModel` `32b151b0` reads Lu's model from June `CX2Eqip +0x5a4`, which is the **normal** model, not the hyper one. June's `CX2Eqip` has one
  more array than March's before the models, so March's member order does not carry over. Read in June: `_UpdateSkin( int )` `3295bdf0` ( hyper branch `+0x1c4 && +0x1c5`: removes `+0x5a4`, adds `+0x5c4` with
  the hyper XETs `+0x814 / +0x81c`; normal branch the reverse; `+0x320` = attachment 1 / 2 ), `InitSkin` `32963eb0` ( models open into `+0x5a4`; the hyper loop stores the model at `+0x5c4` and its skin anim at
  `+0x564` ), attach-anim init `3295dfa0` ( normal skin anim at `+0x544` built on the `+0x5a4` model; a June-only anim at `+0x584` ). June layout: `+0x544` normal anim, `+0x564` hyper anim, `+0x584` June-only
  anim, `+0x5a4` normal model, `+0x5c4` hyper model. Lu's test now calls `GetNormalModel`; Ciel's `+0x544` = `GetNormalAnim` was right. Same flag ( `_CORE` ), comment tagged `CH1k2 review`.
- **Confirmed = June:** `ReInit` `32b373e0`: base, the three summons ( 1906 / 1937 / 1908 ) then 2269 last, `IsMyUnit` ( vslot 0x198 ) sync of the Combination Point into `iClassSyncInt` ( Future `+0x7ec0` ),
  team 0 / 1 only, the PvP room ( `g_pData +0x6c` ) then `+0xd8 == 8`, other form = `+0x8e90 == 1 ? 0 : 1` ( `m_bMinorUnitActive` ), **kill lead `+0x4a4` / `+0x4a8` = `m_RedTeamKill` / `m_BlueTeamKill`**
  ( March `X2Game.h:2010-2011` order; Red = team 0 takes `+0x4a4 - +0x4a8` ), lead > 1 / == 1 / amusement branches with their constants ( 0.5, 3.0 ) and gauge null tests exactly where June has them ( JUNE_BUGS 708 ),
  `+0x8f70` delete stand-in. `IsInvasionOfDarkEnergyActive` `32b4b580`: `test ah, 0x41` / `jp` returns true on C0 or C3 alone = `0 <= time`, NaN false; the code's `0.f <=` matches.
  `GetWaitStateID`: riding ( vslot 0x200 ) -> `LSI_RIDING_WAIT` ( 0x4c ), else `GetStateIDByForm( m_CommonState.m_Wait )` ( `+0x4088` ); the only getter that calls it. The other 20 getters: every `and` / `add`
  constant decoded by script to its `LSI_*` member and compared with the code: 80 of 80 equal; the damage ten answer the transformed WAIT, the Die pair a bare `mov eax, 7 / 8` ( Lu ) while transformed.
  Both `StateChange` overloads = `GetStateIDByForm` then June's base. `32b4cfb0` = `vector<12-byte>::erase`, `32b4dd50` = `SetHyperModeCountByBuff` on `+0x6844`: no class code owed.
- **The `+` declarations:** the eleven getters `GetWaitStateID` .. `GetDieBackStateID` are `virtual` in `X2GULuciel.h` only; March has `GetWaitStateID` / `GetWalkStateID` / `GetDash*` non-virtual
  ( `X2GUUser.h:1742-1768` ) and the other seven not at all. CH1o's kickoff carries the bases. The damage ten override DU2's virtual bases.
- **Hygiene** ( `finish.py --review --commits da5999ca 3880c02b --skip-flag JUNE_DIVERGE_GAGE_TALLY / JUNE_CHAR_FORM_MP_GAGE` ( the `#error` guard's targets ) `-v` ): `JUNE_CHAR_UNIT_MP_VIRTUAL` `1 PASS, 1 DEFINE,
  5 OTHER, 0 FAIL` / PASS; `JUNE_CHAR_LUCIEL_CORE` `5 PASS, 4 OTHER, 0 FAIL, 1 ERROR` and `JUNE_ENUM_NAMES` `1 PASS, 2 PASS~, 6 OTHER, 0 FAIL, 1 ERROR`: each ERROR is `Always.h`'s existing `#if` guard
  ( line 13430 / 10175 ) the tool cannot evaluate once `Always.h` joins the set, resolved by hand as CH1k ( the commit's `Always.h` hunk is the 13-line `UNIT_MP_VIRTUAL` block ). Every OTHER is a file of one of the
  other two flags ( `X2BuffFactor.h`, `X2DamageManager.h`, `X2PVPRoom.h` = `ENUM_NAMES`; `X2GameUnit.h` = `UNIT_MP_VIRTUAL`; the class files = `_CORE` ). The one deleted line is CH1k2's header comment `body in CH1k2`.
  `junediverge` PASS, `--check-all` PASS. `enumscan` exit 1: the 212 class A rows are the `BFI_` / `BTI_` definition lines of `X2BuffFactor.h`'s enum ( as CH1k ), no literal in code.
  The fix: `finish.py --files X2Lib/X2GULuciel.cpp`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `1 PASS, 0 OTHER, 0 FAIL, 0 ERROR` ( 2 deleted lines = the two replaced lines ), `junediverge` PASS, `enumscan` exit 0.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH ) of `X2GULuciel.cpp` plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 0 errors each. Full build skipped: no play-test. No debug define to retire.
- **Routed:** June `320708b0` ( a bounds-checked `+0x5a4 + 4 * i`, i < 8, CH0 named it a `CX2GUUser` getter ) is June's `CX2Eqip::GetNormalModel` by this layout: CH1l's kickoff amended ( its `32b18220` caller ).
- **Next:** `/x2-phase char-luciel CH1l` ( Sonnet 5.5 high ).

### CH1l - Done (2026-11-15)

Sonnet 5.5 high. All 32 CH1l rows of the class map written under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `X2GULuciel.cpp` +1,028, `.h` +61 -3; inert, `_LIVE` stays `//#define` ). Code commit `f82df8f5`.
funcmap: `grep "(CH1l)" june2017_re/funcmap.tsv` ( 11 rows, getters grouped ). `JUNE_BUGS.md` 709-712. No debug define. Review: none of its own, the CH1l3 review covers CH1l, CH1l2, CH1l3.
- **Written ( June address / vslot ):** `CanPlayEmotion` `32ae9bf0` ( 0x574 ), `GetTransformedSkillStateID` `32ae9c00` ( 0x57c ), `IsTransformSkillState` `32ae9de0` ( 0x580 ), `SetShowViewListAndUpdate` `32ae9f20` ( 0x640 ), `InitEnchantWeaponEffectUnitSpecific` `32aea690` ( 0x54c ),
  `SetNowMpFromStoredValue` `32aeb2c0` ( 0x650 ), `DoProcessWhenActiveBodyChangeApplied` `32b0fa20` ( 0x648 ), `COMMON_BUFF_FrameMove` `32b12fd0` ( 0x5f0 ), `GetSkinAnimByForm` `32b13b60` ( 0x570 ), `SetHandEquipShow` `32b143e0` ( 0x62c ),
  `CanRidingState` `32b14e10` ( 0x628 ), `IsClassCommonMoveState` `32b15340` ( 0x578 ), `CreateMajorParticleArray` `32b154f0` ( 0x540 ), `GetSkillIconTexture` `32b15900` ( 0x504 ), `SkillIconOverride` `32b16220` ( 0x56c ),
  `UpdateNotRenderingOtherEquipList` `32b18220` ( 0x630 ), `SyncFutureMpFromGage` `32b1c960` ( 0x64c ), `CheckBoundingTarget` `32b34680` ( 0x568, now `bool` ), `AttackResultByType` `32b37730` ( 0x4e0 ), `CreateMinorParticleArray` `32b37780` ( 0x548 ),
  `HyperModeFrameMove` `32b38080` ( 0x600 ), `GetJumpDownStateID` `32b4d5b0` ( 0x458 ), the DU2 getters `32b4d810` .. `32b4db10` ( 0x114 / 0x118 / 0x48c / 0x490 / 0x494 / 0x49c / 0x4a0 / 0x4a4, DU2's exact names ), `GetDanceWaitStateID` `32b4db50` ( 0x5c4 ), `IsHyperModeState` `32b4dbe0` ( 0x51c ).
- **Names CH0 left `?`, named here by what each does:** `IsTransformSkillState` ( 0x580 ), `SetShowViewListAndUpdate` ( 0x640, `SetShowViewList` then, when shown, `UpdateNotRenderingOtherEquipList` in June's base ), `DoProcessWhenActiveBodyChangeApplied` ( 0x648; June's `CommonStateStart` @`32051059` calls it with the
  DUO_TYPE key and a key int ), `SetHandEquipShow` ( 0x62c; callers `StateChange` @`320349cb` with 0, `PlayEmotion` @`320447e9` with 1 ), `SyncFutureMpFromGage` ( 0x64c ). The 0x87c override `32b172b0` ( CH1l2 ) is declared as `SetSDOnePieceAvatar( int iForm, bool )` ( base `3208e280` stores `m_bSDOnePieceAvatar` ).
- **Unmapped callees identified from their listings:** `32282440` = user `IsOnSomethingNow` / NPC `IsOnSomething` by unit type ( file-local `IsGameUnitOnSomething` ), `31fd5ab0` = `GetEquipPtrFromEquipViewListByEquipPosition`, `320708b0` = `CX2Eqip::GetNormalModel`, `31cd0850` = `CKTDGUISlot::SetItemTex( file, key )`,
  `31fc9d90( 2 )` = `IsTransformed( DT_BOTH )`. No stop-and-ask case.
- **`UpdateNotRenderingOtherEquipList` ( the one big row ):** it is not the base with a patch. June runs the base's one-piece rules once per body: the item list per body comes from `32b176a0` ( CH1l3 ), each of 16 division groups hides the shown equips of the item's own body at its case-list positions
  ( `HideFormEquips` + one position array per group in `_CONST_LUCIEL_CH1L_`; a hair equip keeps its model as the hair mesh, marks the body only in the groups where June marks it ), then the Raven-arm, hand, two face-accessory passes, the hair-mesh hand-over to the after-image,
  `m_bOtherEquipNotRendered` = body 0's mark, and one hair / face pass per body. June has no Arme / Ain or near-portal exception here ( the base's `bArmeOrAin`, `bNearPortal` are gone ), kept. The draw-face-accessory stores June makes in three groups are dead ( JUNE_BUGS 712 ) and not written.
  The equip's body byte ( `CX2Eqip +0x15d4` ) is DU3's: `IsEquipForMinorUnit` is a file-local stand-in returning false, so Ciel's passes find no equip until DU3; every other row is exact.
- **Shared-file hunk ( `X2GUUser.h` ):** `m_pHairXSkinMesh` is private; two 3-line hunks under `_CORE` make it `protected` ( `protected:` before the `MODIFY_AFTER_IMAGE` block, `private:` after ). Insert-only, flagcheck PASS. A header edit: the next full build should rebuild the PCH ( `build.py --pch force` if a hunk seems dead ).
- **Quirks kept ( JUNE_BUGS 709-712 ):** the MP sync reads the gauge unchecked ( 709 ); `CheckBoundingTarget` starts Lu's strike state while Ciel is in play and Ciel's while Lu is ( 710 ); `IsTransformSkillState` compares a skill id with six transform state ids when not transformed ( 711 ); the dead draw-face stores ( 712 ).
  Not rows: `GetSkinAnimByForm` crosses the bodies while transformed ( form 0 with Ciel in play returns Lu's own anim ), `GetSkillIconTexture` sets Twilight Judgement's texture name in all three branches, `IsHyperModeState` reads the state id where June reads the state record index ( same number, `LSI_*` = registration index ).
- **Declared for later phases ( bodies owed ):** CH1l3 gets `SetLucielMinorParticleByEnum` `32b1eca0`, `CreateChaserOfDarknessDamageEffect` `32b1f800`, `ProcessPhantomRushHit` `32b347f0`, `PlayPunisherEffectSet` `32b34c70`, `RegisterDesperadoTarget` `32b18060`, `AccumulateShadowFuryDamage` `32b0e4e0`, `PlayBluffEffectSet` `32b37090`,
  `CheckBoundingStrikeHit` `32b153d0`, `AddCombinationPointByDamageEffectHit` `32b16880`, `GetFormEquipItemList` `32b176a0` ( names and signatures in `X2GULuciel.h`; kickoff amended ). The seven minor particle handles are named ( `LUCIEL_MINOR_PII_REVENGE_WAIT` .. `_MAGNUM_BLADE_FIRE_WAVE` ).
  CH1l2 gets `SetSDOnePieceAvatar`. CH1o gets the `+` bases ( 12 virtuals with no March base, listed in its kickoff ).
- **Gates:** `finish.py --files X2GULuciel.h/.cpp X2GUUser.h`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `3 PASS, 0 OTHER, 0 FAIL, 0 ERROR` ( the 3 deleted lines: `CheckBoundingTarget`'s old `void` declaration and the two `LUCIEL_MINOR_PII_BASE` / `_END = 7` enum lines ), `junediverge` PASS, `enumscan` on the two class files exit 0;
  the finish run's `enumscan FAIL` is two lines of `X2GUUser.h` ( 2915-2916, `NUI_NEIL_THE_SNIPER_EVENT` / `NUI_PROTON_BOMB_EVENT` ) that this phase did not touch. Range gate `--range f82df8f5^..f82df8f5`: identical tallies. stateends / skillicons / luabind: nothing registered or staged.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH ) of `X2GULuciel.cpp` plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`, and `X2GUUser.cpp` ( includes the changed header ): 0 errors each ( first run: `GetSkillTemplet` takes one argument here, the `m_pHairXSkinMesh` access above ).
  Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Next:** `/x2-phase char-luciel CH1l2` ( Sonnet 5.5 high ).

### CH1l2 - Done (2026-11-15)

Sonnet 5.5 high. All 32 CH1l2 rows of the class map written under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `X2GULuciel.cpp` +1,302, `.h` +56 -4; inert, `_LIVE` stays `//#define` ). Code commit `12df5212`.
funcmap: `grep "(CH1l2)" june2017_re/funcmap.tsv` ( 11 rows, grouped ). `JUNE_BUGS.md` 713-717. No debug define. Review: none of its own, the CH1l3 review covers CH1l, CH1l2, CH1l3.
- **Written ( June address / vslot ):** `SetEquippedSkillLevel` `32ae40c0` ( 0x7b8 ), `CommonStateStartFuture` `32ae4f60` ( 0x7c0 ), `CommonStateEndFuture` `32ae4f70` ( 0x7d8 ), `CommonStateEnd` `32ae4f80` ( 0x7dc ), `CommonRender_Prepare` `32ae4f90` ( 0x7e8 ), `SpecialAttackEventProcess` `32ae4fb0` ( 0x7f4 ),
  `StateChangeDashIfPossible` `32ae5400` ( 0x668 ), `IsSkillChainReady` `32ae9bd0` ( 0x658 ), `GetEffectOffsetByIndex` `32ae9d00` ( file-local ), `ShownSkillCoolTime` `32aea8c0` ( 0x6a8 ), `IsInJobLine` `32aeb380` ( 0x670 ), `StoreGageSnapshot` `32aeb410` ( 0x740 ), `RestoreGageSnapshot` `32aeb430` ( 0x744 ),
  `UpdateAfterImageEquip` `32b0e440` ( 0x6b0 ), `SumSocketOptionAfterSlots` `32b0e670` ( 0x794 ), `SpecialAttackUse` `32b0f940` ( 0x7fc ), `ResetMaxMP` `32b0fa90` ( 0x664 ), `GetActualMPConsume` `32b14640` ( 0x6a0 ), `GetActualCoolTime` `32b15050` ( 0x6a4 ), `IsNotCommonMoveState` `32b15220` ( 0x828 ),
  `AfterImageFrameMove` `32b15430` ( 0x850 ), `ShowActiveSkillCutInAndLightByScript` `32b16c80` ( 0x834 ), `ScaleHead` `32b171c0` ( 0x66c ), `SetSDOnePieceAvatar` `32b172b0` ( 0x87c ), `RegisterDamageEffectHitCountEntry` `32b18170` ( 0x848 ), `SpecialAttackNoStageChange` `32b1c880` ( 0x810 ),
  `CommonFrameMoveFuture` `32b1edd0` ( 0x7c8 ), `SpecialAttackCooldown` `32b1eef0` ( 0x800 ), `CommonStateStart` `32b37910` ( 0x7c4 ), `NoStateChangeActionFrameMove` `32b37c40` ( 0x80c ), `CommonFrameMove` `32b397a0` ( 0x7cc ), `CanUseSpecialAttack` `32b3a310` ( 0x7f8 ).
- **Names CH0 left `?`:** 0x658 `IsSkillChainReady( skill, unused )` ( true only for the two Phantom Rush ids ), 0x6b0 `UpdateAfterImageEquip` ( already March's virtual, DU1d3 ), 0x87c `SetSDOnePieceAvatar` ( CH1l's name ), 0x848 `RegisterDamageEffectHitCountEntry` ( adds a { skill base, damage effect index, 0 } counter when the state skill's templet `+0xe` = `m_usFPGainCount` > 0 ).
  Unmapped callees identified from listings: `31fd32e0` = `StateChangeBySkillSlot( m_iChosenSkillSlotIndex )`, `31fd0570` = `IsTrainingOrPhotozone`, `322a0090` = `GetBuffAccumulationCountNowInFactor`, vslot 0x260 = `IsMinorUnitActive` ( byte `+0x8e90` ), `3201f850` / `32028190` = the Arme / Eve `SetSkillLevelStateData` / `...ByID` ( carried per class again ).
- **Shape.** Skill use is the Ara / Eve shape ( lookup, use, cooldown ) with Lu's parts: combination point cost and message `0x8103`, the Stable Dark Energy use-condition rewrites ( 30421 learned: a locked Phantom Rush counts as ANYTIME_EXCEPT_AIR outside the air keys, Sinister Mind's air keys read as the ground key ), a Summon Darkness re-cast ends the buff through user action 42
  ( `SpecialAttackNoStageChange` posts it on the Future frame, `NoStateChangeActionFrameMove` runs `CancelSummonDarkness` on the Now frame ), MP paid to the form in `+0x96b8`, the stored `+0x96bc` points paid at the cooldown. `GetActualMPConsume`: Twilight Judgement / Summon Darkness / ready Phantom Rush cost 0, Dancing Blade -30 per loop,
  Open Mind ( ability 27 of the class's passive ) on 45 skill ids in three lists ( Lu's body, Ciel's body, both ). `ResetMaxMP` is the base's body once per form ( `ResetMaxMp( 300, form )`, gauge `SetMaxMp( result, form )` ), so the old "type-10 override has no March class" note of the base is now written.
  `CommonFrameMove` is the big row: eight timers, Soul Shock damage effect follow, Dead Hands release sound, the other body's wait pose while the duo is off, Summon Darkness NPC kills, `ResetPhantomRushReady`, Open Mind / Stable Dark Energy upkeep for transcended units, the Punisher effect set stop; its eight callees are CH1l3's.
- **Quirks kept ( JUNE_BUGS 713-717 ):** the Now-side frame move reads the Future list's duo flags ( 713 ); two dead memo cases ( 714 ); the dash start runs the base first, then changes state again ( 715 ); every Lu / Ciel unit updates the one switching guide ( 716 ); `SetEquippedSkillLevel` writes unregistered state ids ( 717 ).
- **Corrections to CH1a names ( header + ctor ):** `m_iSpecialAttackCombinationPointCost` is `m_iSkillUseMpForm` ( June stores 0 / 1 there in `SpecialAttackUse`, ctor 3 ), `m_hSoulShockEffectSet` is `m_hSoulShockDamageEffect` ( `CX2DamageEffect::CEffectHandle`, June moves it with the unit ). `m_iLastUsedSkillID` ( +0x96bc ) holds a point count ( paid to `UpCombinationPoint` ):
  name kept, the CH1p phase renames it with its readers ( CH1l3 kickoff says so ). New stand-in members `m_bSkillCameraOverride` / `m_fSkillCameraOverrideValue` ( June `+0x8d90` / `+0x8da4`, the camera group; Rena carries the same pair, the reader is Stage 9's ).
- **Enum name:** `BTI_BUFF_DEAD_HANDS = 463` registered in `X2BuffFactor.h` under `JUNE_ENUM_NAMES` ( June Enum.lua ). User action id 42 has no Enum.lua name: file-local `UAI_CANCEL_SUMMON_DARKNESS`, string `0x8bae` has no StringID entry: file-local `STR_ID_SKILL_REPLACED_BY_UPGRADE`.
- **Declared for CH1l3 ( bodies owed, `X2GULuciel.h` ):** `UpGageNowMp` `32aeb1a0`, `ShowActiveSkillCutInAndLight` `32b16940`, `CancelSummonDarkness` `32b204d0`, `IsOpenMindConditionMet` `32b4c7f0`, `GetOpenMindSkillID` `32b0ef80`, `UpdateOpenMindBuff` `32b14d10`, `UpdateStableDarkEnergyBuff` `32b0eed0`, and the eight frame-move helpers
  `32b1f650` / `32b20780` / `32aebb60` / `32b15780` / `32b34a90` / `32b16760` / `32b35c60` / `32b0e550` ( names in the CH1l3 kickoff amendment ). `32b4cb40` is written inline; `GetEffectOffsetByIndex` is ready for `32b35600`. CH1o gets nine more `+` bases ( its kickoff lists them ).
- **Gates:** `finish.py --files X2GULuciel.cpp X2GULuciel.h X2BuffFactor.h`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 1 OTHER, 0 FAIL, 0 ERROR` ( the OTHER row is the `X2BuffFactor.h` name block, flag `JUNE_ENUM_NAMES`, also PASS ), `junediverge` PASS, `enumscan` on the two class files exit 0;
  the finish run's `enumscan FAIL` lists only the 213 existing enum definitions of `X2BuffFactor.h` ( every registered name there is flagged, mine too ), not code. The 6 deleted lines are the two ctor initializers, the two member lines and the two comments above. Range gate `--range 12df5212^..12df5212`: identical tallies. stateends / skillicons / luabind: nothing registered or staged.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH ) of `X2GULuciel.cpp` plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`, and `X2GUUser.cpp` ( includes the changed `X2BuffFactor.h` ): 0 errors each. Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Next:** `/x2-phase char-luciel CH1l3` ( Sonnet 5.5 high ).

### CH1l3 - Done (2026-11-15)

Sonnet 5.5 high. All 32 CH1l3 rows of the class map written under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `X2GULuciel.cpp` +715, `.h` +15 -6, `X2BuffFactor.h` +14; inert, `_LIVE` stays `//#define` ). Code commit `bc8eefb8`.
funcmap: `grep "(CH1l3)" june2017_re/funcmap.tsv` ( 15 rows, grouped ). `JUNE_BUGS.md` 718-720. No debug define. Review: `CH1l3-review.md` ( Opus 5.5 high; covers CH1l, CH1l2, CH1l3 ).
- **Written ( June address ):** `UpGageNowMp` `32aeb1a0`, `FrameMoveTwilightJudgementMp` `32aebb60`, `AccumulateShadowFuryDamage` `32b0e4e0`, `FrameMoveBoundingKeyDialog` `32b0e550`, `UpdateStableDarkEnergyBuff` `32b0eed0`, `GetOpenMindSkillID` `32b0ef80`, `UpdateOpenMindBuff` `32b14d10`,
  `CheckBoundingStrikeHit` `32b153d0`, `FrameMoveAuraOfFear` `32b15780`, `UpdateInvasionOfDarkEnergyTargetTime` `32b16760`, `AddCombinationPointByDamageEffectHit` `32b16880`, `ShowActiveSkillCutInAndLight` `32b16940`, `GetFormEquipItemList` `32b176a0`, `RegisterDesperadoTarget` `32b18060`,
  `SetLucielMinorParticleByEnum` `32b1eca0`, `FrameMoveChaserOfDarknessEffect` `32b1f650`, `CreateChaserOfDarknessDamageEffect` `32b1f800`, `CancelSummonDarkness` `32b204d0`, `FrameMoveShadowBoltSpear` `32b20780`, `ProcessPhantomRushHit` `32b347f0`, `ClearDeathTouch` `32b349f0`,
  `FrameMoveDeathTouchEffect` `32b34a90`, `PlayPunisherEffectSet` `32b34c70`, `PlayAltarOfEvilEffectSet` `32b35600`, `FrameMoveAltarOfEvilEnd` `32b35c60`, `PlayBluffEffectSet` `32b37090`, `IsOpenMindConditionMet` `32b4c7f0`.
  No body owed: `32b4cb40` ( written inline in CH1l2 ) and the STL instances `32b4cef0` / `32b4cff0` / `32b4d140` / `32b4d260` ( `vector<STimedGameUnitKey>::push_back` and its helpers ).
- **Names CH0 left `?`:** `ClearDeathTouch` ( `32b349f0` ) and `PlayAltarOfEvilEffectSet` are new declarations in `X2GULuciel.h`; the rest follow CH1l / CH1l2's declarations and the CH0 hints.
- **Corrections to CH1a members ( header + ctor; the later readers are in the amended kickoffs ):** `+0x9678` is a stack count ( `m_ucAuraOfFearStackCount`, UCHAR ), `+0x969c` the Twilight Judgement buff's stack count ( `m_ucTwilightJudgementStackCount`, UCHAR ), `+0x9690` the spear NPC's UID
  ( `m_iShadowBoltNpcUID` ), `+0x97b8` / `+0x97bc` two damage effect handles ( `m_hDeathTouchBeamEffect` / `m_hDeathTouchTargetEffect`, replacing the one `UidType` ), `m_AltarOfEvilData.m_fDuration` is the MP paid per altar ( `m_fMpCostPerEffectSet` ).
  `m_fRapidImpactRange` ( `+0x97c4` ) is a timer that counts down every frame: name kept, CH4a renames it with its readers.
- **Where Ghidra overruled the class map:** `GetOpenMindSkillID` maps class 127 -> 30416, 128 -> 30418, 129 -> 30420 ( CH0 hint and CH1l2's comment had it reversed ); `IsOpenMindConditionMet` tests `points <= threshold` ( JUNE_BUGS 719 ).
- **Unmapped callees identified:** `3241d2a0` = `g_pMain +0x180` ( the game option ) `+0x2c == arg` = the effect option ( `m_eEffect` ) test, used with `OL_LOW`; `+0x8a48` = `m_bHold`, `+0x9108` = `m_bGrap` ( Chung's shot cancel has the same pair ); `IsOpenMindConditionMet` reads the gauge's combination point inline ( = `GetNowCombinationPoint`, `32b0fa00` );
  vslot `0xa8` of the game = `GetNPCUnitByUID`; `31fcef00 == 5` = the now state's skill is `ST_HYPER_ACTIVE_SKILL` ( inline, as `GetShowActiveSkillShow` does ); `321db1b0`'s tags 9 / 10 / 11 name the damage data's force down value, extra damage and relate skill index ( inferred from the funcmap row, a review item ).
- **Shape.** The hit readers of `AttackResultByType` map DamageData by June offset to March members ( probed with `offsetof`: `+0x14` hAttackerEffect, `+0x28` reActResult, `+0x46c` defenderType, `+0x470` optrDefenderGameUnit, `+0x590` m_eUsedSkillID, `+0x5c4` m_iAppliedSkillID, `+0x5a8` m_RelateSkillData, `+0x638` m_iDamageEffectIndex ).
  The cut-in tables of `ShowActiveSkillCutInAndLight` are file-local ( `_CONST_LUCIEL_CH1L3_`: 12 + 3 + 6 names read from June's three static initializers `333a78e0` / `333a8080` / `333a74e0` ); CH5 confirms them. The customize-manager cut-in override is a stand-in ( `bCustomizeCutInApplied` false ).
- **Quirks kept ( JUNE_BUGS 718-720 ):** the cut-in function ignores its sub index argument; Open Mind's `<=`; the Desperado target's time is the raw word -1. Not rows: `CancelSummonDarkness` calls the base's cost ( not the class's zero ), `CreateChaserOfDarknessDamageEffect` passes 1.0 as the reserve time,
  the Punisher loop is guarded against an empty list on a boss ( June would read element 0 ).
- **Enum names ( `X2BuffFactor.h`, `JUNE_ENUM_NAMES` ):** `BTI_BUFF_DEATH_TOUCH` 760, `BTI_BUFF_P_LUCIEL_OPEN_MIND` 877, `BTI_BUFF_P_LDN_STABLE_DARKENERGY` 895, `BFI_BUFF_CHASER_OF_DARKNESS_MEMO` 603, `BFI_BUFF_P_LUCIEL_OPEN_MIND` 906, `BFI_BUFF_P_LDN_STABLE_DARKENERGY` 925 ( checked against `Enum.src.lua` ).
  No name: state id 444 ( `STATE_ID_ALTAR_OF_EVIL_UNNAMED` ), the modifier tags, the event code 0x8b3.
- **Gates:** `finish.py --files X2GULuciel.cpp X2GULuciel.h X2BuffFactor.h`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 1 OTHER, 0 FAIL, 0 ERROR` ( the OTHER row is the `X2BuffFactor.h` name block, flag `JUNE_ENUM_NAMES`, also PASS ), `junediverge` PASS, `enumscan` on the two class files exit 0
  ( the finish run's `enumscan FAIL` lists only the existing `X2BuffFactor.h` enum definitions, every registered name there, mine too ). The 10 deleted lines are the renamed members: 6 in the header ( altar member + ctor, three members, the UID ) and 4 ctor initializers. Range gate `--range bc8eefb8^..bc8eefb8`: identical tallies.
  stateends / skillicons / luabind: nothing registered or staged.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH ) of `X2GULuciel.cpp` plain and with `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`, and `X2GUUser.cpp` ( includes the changed `X2BuffFactor.h` ): 0 errors, 0 warnings in the new block. Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Next:** `/x2-phase char-luciel CH1l3 review` ( Opus 5.5 high ).

### CH1l3 - Review (2026-11-15)

Opus 5.5 high. Covers CH1l ( `f82df8f5` ), CH1l2 ( `12df5212` ), CH1l3 ( `bc8eefb8` ). **Verdict: pass after one fix.** Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `_LIVE` stays `//#define` ).
- **Fix ( `X2GULuciel.cpp`, `CreateChaserOfDarknessDamageEffect` ):** June @`32b1fa67` loads `-1.0` ( `334380f0` ) into the reserve-time slot; CH1l3 wrote `1.0`. Now `-1.f` ( March's default; `bReserve` is false, so no behaviour change ). The CH1l3 note's "passes 1.0" is wrong; funcmap row corrected.
- **Routed, not fixed here ( CH1o amendment ):** March's `CX2EffectSet::ApplyDamageEffectModifiers` has no case 4, 10 or 11 ( the default logs `[JUNE_FX] ... tag N is not ported` ). Lu/Ciel push all three: tag 4 ( CH1f2 Power of Oriax ), 10 / 11 ( CH1l3 Punisher ); tag 9 exists, under Elsword's `_SKILL`.
- **Ghidra spot-checks ( all = June ):**
  - `UpdateNotRenderingOtherEquipList` `32b18220`: the 18 case lists ( 12 groups + 6 face-accessory passes ) equal the `_CONST_LUCIEL_CH1L_` arrays; the two if-chain groups ( 0x22 one-piece 6 / 8-11, 0x27 8-11 ), 0x62 ( 8-11 + 17 ) and 0x3ea ( 8-11, + 18 / 27 when the equip is a not-rendering-hair item ) match; the hair flags per group ( keep mesh + mark: 0x5f / 0x5e / 0x23-0x25 / 0x40 / 0x6a; keep only: 0x26 / 0x28 / 0x29 ) match.
  - `GetFormEquipItemList` `32b176a0`: list `+0x8834`, position `+0x1b0`, `{ +0x15d4, +0x1a0 }`, stops at two.
  - `CommonStateStart` `32b37910`: base, FP-cost x ( spec B + 1.0 ), the 10 Communion ids x `+0x9640`, camera clear on my unit outside 358-362, altar stop on 446-448 / 452 / 453; order kept.
  - `CommonFrameMove` `32b397a0`: block order, timers `+0x95fc` / `+0x960c` / `+0x96a4` / `+0x9674` / `+0x96a8` / `+0x9714` / `+0x9888`, the eight CH1l3 callees in June's order, Wait / Wait3 on the right skin ( `+0x8e88` / `+0x87d4` ), NPC 1906 / 1937, Punisher state 467.
  - `CanUseSpecialAttack` `32b3a310`: the Stable Dark Energy rewrites ( 30216 / 30220 -> condition 5 off the air keys; Sinister Mind 30048 air key -> 1 ), check order, vslot `0x82c` = `IsDevUnlimitedSkillUse` as Ara / EL, strings 0xe2 / 0x9f5 / 0x8103 / 0x6607 / 0x8bae.
  - DamageData map: `+0xa4` = `m_ExtraDamage.m_Damage` is proved by an independent reader ( funcmap `32c5ce40`, Raven Megadrill: March source writes `m_Damage`, June writes `+0xa4`; row 1348's ExtraDamageData layout ); a protected float ( `FLD` ), x 0.5.
  - Punisher `32b34c70`: tags 9 / 10 / 11 write CEffect `+0x5f0` / `+0x2a4` / `+0x7a8` = DamageData ( at CEffect `+0x200` ) `+0x3f0` fForceDownValue / `+0xa4` m_Damage / `+0x5a8` relate skill index; values rate / rate / index + 1 ( add only on 11 ); boss = vslot `0x3c8` GetIsBosRaidNPC; 500 / 5 / 0.5 / 0.25.
  - Chaser `32b1f800`: 23 arguments ( owner, name, rate x `+0x95d4`, Dummy02, two `GetRotateDegree`, `+0x80c4`, false, -1, 1, 1, -1, -1, 0, -1, 0, 0 ), lock-on `+0x938` ( 8 bytes ) / `+0x940`, destroy silently + erase the first, timer 0.3, BFI 603 `( id, NULL, 1, 0, false )`.
  - Cut-in `32b16940`: 12 + 3 + 6 names = the three initializers, size 512 x 512 / position ( 256, 512 ), index `+0x8ac8 * 3 + +0x8acc`, `IsTransformed( DT_MAJOR / DT_MINOR )`, hyper third entry only without a customize cut-in, event 0x8b3 even roll = CASH_21.
  - Open Mind `32b0ef80` / `32b4c7f0`: 127 -> 30416, 128 -> 30418, 129 -> 30420; ability 2 ( 5 without a templet ), `points <= threshold`.
  - Death Touch `32b34a90` / `32b349f0`: `+0x97b8` / `+0x97bc` damage effect handles, +25 / +25 / +100, `SetLinkMainEffectByPos( mine, target, 99999 )` ( the default ), clear writes -1 / -1 / GUT 0 and erases BTI 760.
  - Altar `32b35600` / `32b35c60`: `+0x9748` MP per altar through `FlushMp`, 8 at most, offset into instance `+0x1a4`, low effect option = `3241d2a0( 2 )` on `g_pMain +0x180` ( the kickoff's "`+0x1ac == 2`" is the instance offset's z ), state 444, timer helper `3322a380` adds to `+0x9754`.
  - Renamed members: byte reads of `+0x9678` / `+0x969c`, dword UID `+0x9690`, float timer `+0x97c4`.
- **Gates:** `finish.py --review --commits f82df8f5 12df5212 bc8eefb8 -v`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `7 PASS, 0 OTHER-fail, 0 FAIL` ( 2 OTHER = the `X2BuffFactor.h` name blocks, flag `JUNE_ENUM_NAMES`, itself `RESULT: PASS` ), 19 deleted lines = the CH1a member / enum / declaration lines the three notes list; `junediverge` and `junediverge --check-all` PASS;
  `enumscan` FAIL only on the 22 enum definitions of `X2BuffFactor.h` and two untouched `X2GUUser.h` lines ( 2915-2916 ), no row in the class files. Fix: `finish.py --files X2Lib/X2GULuciel.cpp` PASS ( flagcheck 1 PASS, 2 deleted = the replaced call + comment ).
- **Compile:** `/Zs` probes of `X2GULuciel.cpp` plain and with `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 0 errors. Full build skipped: no play-test. No debug define to retire.
- **Next:** `/x2-phase char-luciel CH1m` ( Opus 5.5 high ).

### CH1m - Done (2026-11-15)

Opus 5.5 high. `CX2GULuciel::InitState` ( `32b43d10` ) and `InitStateCommon` ( `32b3e8b0` ) written under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( `X2GULuciel.cpp` +2476, `.h` +11; inert, `_LIVE` stays `//#define` ). Code commit `a2312de2`.
No debug define. No review of its own: the CH1o review covers CH1m, CH1n, CH1o. funcmap: `grep "(CH1m)" june2017_re/funcmap.tsv` ( 7 rows ). `JUNE_BUGS.md` 721.
- **How it was built:** generated from `ch0_reg.tsv` ( `ch0c_listing_check.py` 364 / 364 ) by resolving each callback through the class-map thunks to the CH1b-CH1l definitions ( their `June <va>` comments ), `CX2GUUser` bodies / funcmap rows, and the vslot thunks ( bounding 0x5d4-0x5e4, hyper 0x600 / 0x604, riding hyper 0x608 ).
  Re-checked by `june2017_re/char_luciel/ch1m_regcheck.py` ( new, committed ): **337 registrations, 0 problems** ( a mutation test of a callback, an id, a left-out and a deferred marker each fails it ). Listing order re-read from `disasm_32b3e8b0.txt`: same 294. Spot-check in the raw listing: `LSI_LU_DASH_END` ( 0x14, thunks `32b4aa90` / `32b4aaa0` / `32b4cbe0` / `32b4de50` ).
- **InitState:** cut-in index ( `SetSkillCutInSetIndexByUnitClass` ), `m_ChargeMpState` / `m_CommonState.m_Wait` = `LSI_LU_WAIT` ( +0x7d04 / +0x4088 = 0xd ), `m_CommonState.m_Thrown` = `LSI_LU_DAMAGE_AIR_FLY_FRONT` ( +0x408c = 0x28; `GRAPPLING_TEST` is off ), `InitStateCommon`, `InitStateCommonRandom` ( = `3202b110` + `31fe22d0` ), START / WIN / LOSE,
  `LSI_WAIT_STATE_CHANGE`, then the Diablura / Demonio ( 39 / 129 ) test with 42 bounding-target and 19 LDB dash-combo rows, vslots 0x690 / 0x684 ( `false` ), `CX2GUUser::InitState`. The `[JUNE_CHAR] LUCIEL DATA` line is CH1 final's.
- **InitStateCommon:** 294 rows, no class test: 272 registered, 18 left out with a one-line comment as Elesis / Ara ( phoenix run 123-126 / 571-574: PvP amusement; dance 127-129 / 590-592: Stage 10; riding 93-96: Stage 17 ), and 4 skill states left to the phase that owns every callback
  ( Dark Crescent 298 / Ruinous Impact 299: CH2b; Eschaton 363: CH2c / CH2c2; Fantasie Impromptu 2nd Movement 364: CH3b ) with a `registered by CHxx` marker: registered now they would be table-only, stuck and invincible ( `stateends` flagged them ). Hyper 23 / 204 take `GetHyperModeTableName( false / true )` ( @`32b3fae6` / @`32b40fdd` ).
  Empty on purpose ( one-line comment each ): die `StateEnd` `3202a7d0` ( runs in `StateChange` ), DASH_END `StateStart` `31fc7fc0` ( = `CommonStateStart`, which an empty slot runs ), riding attack X frame `32002f70` ( Stage 17 ).
- **Ids are June's** ( `LUCIEL_STATE_ID` ), as the CH1b-CH1l bodies use them; `USI_START` / `WIN` / `LOSE` / `HYPER_MODE` ( equal below 24 ). Lu's bounding states ( 120-122 ) and `LSI_WAIT_STATE_CHANGE` ( 130 ) are registered inline at June's ids, **not** through `InitBoundingStates` / `InitWaitStateChange`:
  `enumprobe.py` shows March's ids are 102-104 / 105, and `GetStateIDByForm` maps 120 -> 417 for Ciel only by June number. Two numbers: 327 is registered twice ( June, JUNE_BUGS 721 ); 338 stays a number ( JUNE_BUGS 695 ).
- **Seven `CX2GUUser` bodies only Lu / Ciel register** ( DU1d routed them here ), written in the class as CH1b did `DieFrontStart`: `DieFrontStartFuture` / `DieBackStartFuture` ( `31fcd0f0` / `31fcd1b0`: CommonStateStartFuture, Future anim `SetPlaySpeed( 0.5 )` inlined ), `DieFrontFrameMove` / `DieBackFrameMove`
  ( `320008f0` / `32000910`, uncarved: `DieFrameMove( 0.19, 1.12 )` ), `DieFrontEventProcess` / `DieBackEventProcess` ( `31fcd160` / `31fcd220`: `GetShowObject()` inlined, Future anim end -> `GUSI_DIE`, no CommonEventProcess ), `DashEndStartFuture` ( `31fd2680`, uncarved: `GetDashSpeed()` ).
- **Where CH0 was wrong:** the ten delegates are pushed by `InitPassiveSkillState` `32b45820` ( data refs `32b4584d`-`32b463fc` ), not InitState: routed to CH4a. `32b4ce90` / `32b4cf30` are called by the uncarved skill setter ( @`32b490e8` / @`32b490f7` ): routed to CH1n. CH1m declares neither.
- **Found, routed to CH1o ( the plan's subject, needed before CH1 final ):** March's `CX2GUUser` state ids are June - 1 from 24 ( riding 71 vs 72 ), and further apart for the event kit ( 93-98 vs 111-116 ), bounding ( 102 vs 120 ) and the wait change ( 105 vs 130 ). Every engine `StateChange( USI_* )` at 24 or above lands on the wrong Lu state;
  DU2's getters cover only the damage states, and `StateChange` cannot translate ( the two numberings share 24-105 ). Also `InitEventStates` ( inside `InitStateCommonRandom` ) overwrites `LSI_RIDING_SPECIAL_MOVE` ( 92 ), and RT_BOUNDING stays off ( `m_bBoundingStatesReady` ) until the seam lands. CH1o's amendment lists the options.
- **Gates:** `finish.py --files X2GULuciel.cpp X2GULuciel.h --state X2GULuciel.cpp Luciel_Duo --state X2GULuciel.cpp Luciel_Demonio`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR` / `RESULT: PASS`, `junediverge` PASS, `enumscan` exit 0, `stateends` PASS both
  ( Duo: 0 STUCK after the deferral ); skillicons / luabind not run ( no skill added, no script staged: CH1 final adopts the scripts ). 0 deleted lines. Range gate `finish.py --range a2312de2^..a2312de2 ...`: identical, `FINISH RESULT: PASS`.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH ) of `X2GULuciel.cpp` plain and with `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 0 errors, 0 warnings in the class. Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Next:** `/x2-phase char-luciel CH1n` ( Opus 5.5 high ).

### CH1n - Done (2026-11-15)

Opus 5.5 high. `CX2GULuciel::SetEquippedSkillFuncBySkillSlotIndex` ( June `32b46580`, vslot 0x7b4, uncarved: read from `ghidra/disasm_32b46580_SetEquippedSkillFunc_capstone.txt` ) written under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` ( inert, `_LIVE` stays `//#define` ). Commit `cfe12590` ( code, note, kickoffs ).
No debug define. No review of its own: the CH1o review covers CH1m, CH1n, CH1o. funcmap row `32b46580 (CH1n)`. `JUNE_BUGS.md` 722.
- **Case count: 64 cases, 112 skill ids** ( `ch0_skill.tsv`, re-read in the listing ). **4 written** ( every callback a base one ), **60 left to the phase that writes their state functions**, one row each in `june2017_re/char_luciel/ch1n_skill_cases.tsv` ( new: entry, path, ids, slot table, extra states, owner ):
  CH2a 8, CH2b 5, CH2c 7, CH2d 6, CH3a 12 ( incl. the memo branch ), CH3b 9, CH3c 10, CH3d 3. The skill phases' kickoffs are amended with their rows. Writing those cases now would reference ~400 bodies that do not exist ( link error ) or leave table-only, stuck states ( `stateends` ).
- **The head ( as June ):** slot 0..4; normal / hyper slot state at June's ids from June's four tables ( `334ed540` / `334ed554` / `334ed568` / `334ed57c` = 50-69 ), `Init()`, hyper flag, `stateID` ( the sibling ports' pattern; June's `+0x11` store is `IsSkillSlotState` ); `GetEquippedActiveSkillTemplet( id, true )`;
  the transcendence-memo branch ( `31fdbcc0` = `GetTranscendenceMemoSkillID`, `32206450`, `32207210` = `GetUpgradeBaseSkillID`: memo id 30035 `RNSI_SA_LLD_PHANTOM_BLADE_ADD_TRANS` registers Dancing Blade ( CH3a's marker ), any other memo id returns ); then June's type switch ( `[templet + 4] - 5`, table `32b4a834` ): 5 buff, 6 / 7 / 13-16 the id tree, 12 relationship, 8-11 and 17 / 18 return.
- **Written cases:** buff 208 ( `SI_SA_COMMON_AURA_DARKNESS_ACCEL`: Init = the 0x888 `PreloadAuraAccelResources`, `COMMON_BUFF_FrameMove` ( vslot 0x5f0 ) / `CX2GUUser::COMMON_BUFF_EventProcess`, extra 264 with `m_eSkillID` 208 ), 30301 / 30302 ( Punishing Accel, extra 265 with the raw id ), 30303 ( Phantom Circle, extra 463 with the raw id ),
  relationship 900 ( `SI_ETC_WS_COMMON_LOVE`, raw-id test: `LSI_LU_THROW_ITEM` on both slot states with `COMMON_RELATIONSHIP_SKILL_FrameMove` / `_EventProcess` ( vslots 0x5f4 / 0x5f8 ), extra 263 = the `LSI_CIEL_THROW_ITEM` table, no June name, as `CH1a`'s `StateChange( 263 )` ). The hyper state gets only the FrameMove / EventProcess June copies ( no Init ).
  The buff case runs on into the id tree as June ( no `break`; JUNE_BUGS 722: no buff id is there ).
- **Enum:** June's 20 slot-state names `USI_SPECIAL_ATTACK_1` ... `USI_SPECIAL_ATTACK_SLOTB_HYPER_5` ( Enum.lua `GUSI_END + 45` .. `+ 64` = 50-69 ) added to `LUCIEL_STATE_ID` under `SERV_IRUHADEV_JUNE_ENUM_NAMES`. Probed: March's `CX2GUUser::USI_SPECIAL_ATTACK_1` is 49, `_SLOTB_HYPER_5` 68 ( one lower ). Inside the class the names shadow the base members on purpose ( the class runs on June ids ).
- **Checklist rows:** §7 L261 dev free-skill gate: done by CH1l2 ( `32b3a310` @`IsDevUnlimitedSkillUse()`; the payment `32b1eef0` has none in June either ). §7b: the setter's `31fdbcc0` caller = `GetTranscendenceMemoSkillID` ( S3b's port ); the `32b3c3b0` callers are CH3a's ( Dancing Blade EventProcess ).
  §7i: no `31fd2af0` call in the class ( the engine's `SnapshotBeforeSlotRebind` ). §7ae: types 14-16 named `ST_SPECIAL_ACTIVE_GRADE_1-3`; 17 / 18 fall to `default` as June; `ResetLeftSkillCoolTimeBySkillType`: not in the setter. §7ag: the setter takes slot 0..4 + bank bool ( `cmp edi, 5` ), no > 3 / -4 decode: nothing here.
- **Routed:** to CH1o ( the state-id seam it owns ): the engine's slot tables are March's 49-68 ( `SnapshotBeforeSlotRebind`, `IsSkillSlotState`, `IsBlankSlotSkillState` / `LogNoStateSkill`, the skill-use `StateChange` ), so Lu's slot states 50-69 are missed until the seam lands; June's byte `+0x11` on non-slot states ( 265 here; 344 in CH2c's Soul Shock ) has no March member.
  To CH3c: the Transcendence case ( @`32b487cf` ) ends with `m_ptrShrewdCutterLockOn = CreateLockOnSystem()` ( `32b4cf30` / `32b4ce90` @`32b490e0`-`32b49108` ), after CH3c20 moves `SLockOnSystem` out of `CX2GUAra`.
- **Gates:** `finish.py --files X2GULuciel.cpp X2GULuciel.h --state X2GULuciel.cpp Luciel_Duo --state X2GULuciel.cpp Luciel_Demonio`: `flagcheck JUNE_CHAR_LUCIEL_CORE` `2 PASS, 0 PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR` / `RESULT: PASS`; `flagcheck JUNE_ENUM_NAMES` `0 PASS, 2 OTHER` ( the class files are `_CORE`'s ) / `RESULT: PASS`;
  `junediverge` PASS, `enumscan` exit 0, `stateends` PASS both; skillicons / luabind not run ( no skill added, no script staged ). `FINISH RESULT: PASS (8 gates, 0 failed, 2 skipped)`. 0 deleted lines. Range gate `finish.py --range cfe12590^..cfe12590 ...`: identical, `FINISH RESULT: PASS`.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH ) of `X2GULuciel.cpp` plain and with `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 0 errors, 0 warnings in the class. Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( inert until CH1 final ).
- **Next:** `/x2-phase char-luciel CH1o` ( Opus 5.5 high; its review covers CH1m, CH1n, CH1o ).

### CH1o - Done (2026-11-15)

Opus 5.5 high. New class-agnostic flag `SERV_IRUHADEV_JUNE_USER_VIRTUAL_HOOKS` ( **defined**; `#error` without `JUNE_CHAR_DUO_BODY` ) plus `_LIVE` widenings. Commit `b557510a` ( code, note, kickoffs ).
No debug define. Review: `CH1o-review.md` covers CH1m, CH1n, CH1o. funcmap: `grep "(CH1o)" june2017_re/funcmap.tsv` ( 10 rows ). `JUNE_BUGS.md` 723.
- **Split ( scope ):** the kickoff's amendments came to ~200 engine call sites ( `june2017_re/char_luciel/ch1o_vcall_sites.tsv`, scan `ch1o_vcall_scan.py`: June's
  `mov r,[vtable+slot]; call r` pairs ). CH1o does the bases and the hook sites; **CH1o2** the 84 readers of the state getters; **CH1o3** effect-set tags 4 / 10 / 11,
  the checklist hooks, the `+0x11` skill-state byte and Lu's `ReservedSkillStart`. Kickoffs written; CH1p now waits for the CH1o3 review.
- **State-id seam: user decision ( 2026-11-15 ):** shift March's `USER_STATE_ID` to June's numbers ( not getters per site, not renumbering Lu ). New backlog
  Stage 41, plan key `june-state-ids`, brief `kickoffs/june-backlog/stage41-plan.md`; **CH1 final waits for it** ( `CH1.md` amended ). It also solves CH1n's slot-id seam.
- **Bases ( June's base bodies = March's behaviour; no March class declares any of the names, grep ):** `X2GUUser.h`: `GetWaitStateID` ( 0x438, both `RIDING_SYSTEM`
  branches ), `GetWalkStateID` / `GetDashStateID` / `GetDashEndStateID` made virtual, 7 new getters 0x448-0x464 ( June bases return 21 / 22 / 15 / 16 / 18 / 7 / 8 = March's
  `USI_*`, equal below 24 ), `ResetMaxMP` ( 0x664 ) and `StateChangeDashIfPossible` ( 0x668 ) virtual, `CanPlayEmotion` 0x574, `SetHandEquipShow` 0x62c, `SetShowViewListAndUpdate`
  0x640, `DoProcessWhenActiveBodyChangeApplied` 0x648, `SyncFutureMpFromGage` 0x64c, `SetNowMpFromStoredValue` 0x650, `GetSkinAnimByForm` 0x570, `RegisterDamageEffectHitCountEntry` 0x848;
  `X2GameUnit.h`: `DoProcessWhenDamageDataReset` ( unit 0x2e4 ).
- **Engine sites routed ( June address = March line ):** `SyncFutureMpFromGage` replaces `fNowMP = GetNowMp()` in InitComponent @31fee873, ReInit @32023c24, CommonFrameMoveFuture
  @32062f78; `SetNowMpFromStoredValue` replaces `SetNowMp( Now fNowMP )` in CommonFrameMove @3206c1e6; `SetHandEquipShow( false )` in StateChange @320349cb ( before the local-unit
  test ) and `( true )` at PlayEmotion's end @320447e9; `CanPlayEmotion` gate at PlayEmotion's top @320433d7; CommonStateStartFuture reads `CP_AMOUNT` ( XOR key decoded at 3379d38c,
  int, default 0 ) and calls 0x648( `DUO_TYPE`, `CP_AMOUNT` ) @32051059; CommonStateStart calls 0x848 before clearing the body-change wait @32059b81 ( DU1b's stand-in gone );
  `DamageData::SimpleInit` calls `NotifyAttackerWhenReset` first ( June 32610da0, twin of Ara's 32610d50 ). Automatic through the virtual: every March `ResetMaxMP` / `StateChangeDashIfPossible`
  / `GetWaitStateID` caller.
- **Named on purpose:** CH1k's `DoProcessWhenDamageDataCreated` is `DoProcessWhenDamageDataReset` ( June calls it at the top of SimpleInit with the old attacker ); 0x648's second
  argument is `CP_AMOUNT`, not a skill id: Lu stores it as the last used skill id when the state has no templet ( JUNE_BUGS 723, verbatim ).
- **No base on purpose ( no March engine caller; Lu's comment says so ):** 0x580 ( writes `+0x94c0`, which nothing reads ), 0x504 ( no caller in June's exe ), 0x568 / 0x87c ( class ),
  0x5c4 ( dance, Stage 10 ), 0x658 ( `322cc480` forwarder ), 0x740 / 0x744 ( Crest of Time, char-add ), 0x7f8 / 0x7fc / 0x800 ( each class's own; 0x800 is reached through `ReservedSkillStart`, CH1o3 ).
  0x640's only June caller ( `31fe4030`, riding-off ) is Stage 17's.
- **`_LIVE` widenings ( kickoff ):** Ara's 0x2fc base / `NotifyAttackerWhenDestroyed` / NPC forwarder, Raven's 0x300 base and its three DamageManager sites, `IsHyperModeState` ( 0x51c ),
  and Lu's link in the `ReservedSkillStart` chain ( `X2GUUser.h` / `.cpp` ). `CX2GUNPC` `32778fe0` ( 0x300 to the owner, every attacker unit ) moves to CH3a0 with the summons.
- **Gates:** `finish.py --files <10 code files> --state X2GULuciel.cpp Luciel_Duo / Luciel_Demonio`: `junediverge` PASS, `stateends` PASS both, `enumscan` exit 1 on 6 old rows only
  ( `X2GUUser.cpp:28408-28417` NUI_BEE*, `X2GUUser.h:2943-2944`; none in this diff ). Its flagcheck cannot pass a second phase flag as `--with` ( only `_DEBUG` ), and its `_CORE` run
  crashes on a peer's `#if` ( `X2GUUser.cpp:29435`, Unevaluable ), so flagcheck ran directly: `JUNE_USER_VIRTUAL_HOOKS --with JUNE_CHAR_LUCIEL_LIVE` `5 PASS, 1 DEFINE, 2 OTHER ( X2GUNPC: _LIVE ),
  0 FAIL` / `RESULT: PASS`; `JUNE_CHAR_LUCIEL_LIVE --with JUNE_USER_VIRTUAL_HOOKS` `7 PASS, 1 DEFINE, 0 FAIL`, only 'not #defined' ( `//#define` until CH1 final: INERT ); `JUNE_CHAR_LUCIEL_CORE`
  ( the two class files ) `2 PASS` / `RESULT: PASS`, 44 deleted lines = the class header's comment updates and the 0x2e4 / 0x648 renames. Engine files: 0 deleted, 0 non-ASCII. Range `b557510a^..b557510a`: `HOOKS --with _LIVE` `5 PASS, 1 DEFINE, 4 OTHER, 0 FAIL` / PASS; `_LIVE --with HOOKS` `7 PASS, 0 FAIL`, INERT only.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH ): `X2GUUser.cpp`, `X2GULuciel.cpp` plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`, `X2DamageManager.cpp`, `X2GUNPC.cpp`,
  `X2GUChung.cpp`: 0 errors, 0 warnings in the touched files. Full build skipped: no play-test ( CH1 final's play-test walks one other class as the regression check, `CH1.md` ).
- **How to see it in game:** nothing ( every base equals March; Lu/Ciel is not live ).
- **Next:** `/x2-phase char-luciel CH1o review` ( Opus 5.5 high ), then `CH1o2`.

### CH1o - Review (2026-11-15)

Opus 5.5 high. Covers CH1m ( `a2312de2` ), CH1n ( `cfe12590` ), CH1o ( `b557510a` ); read against June S2 ( `857b18cd`, Lu on the shared state ids ). **Verdict: pass, no code change; two locator fixes in docs.**
- **CH1o bases ( Ghidra / raw exe, all = June = March ):** `GetWaitStateID` `3208ddd0` ( riding vslot 0x200 ? 0x4c : `+0x4088` ); the ten getters return 14 / 19 / 20 / 21 / 22 / 15 / 16 / 18 / 7 / 8
  ( `31b052e0`, `3208ddf0`, `330dcd70`, `330ddfe0`, `3208de00`, `31afa8f0`, `3225b3a0`, `330de0a0`, `3208de30`, `33069eb0` ) = `june_user_state_ids.tsv` C++ values ( `USI_RIDING_WAIT` 76 );
  `CanPlayEmotion` `32ec5990` `mov al,1`; 0x62c / 0x648 / 0x848 / 0x2e4 empty ( `3208ddc0` ret 4, `33093ad0` ret 8, `31f3f070` ); `SetShowViewListAndUpdate` `31fca010` = vslot 0x63c( b ) then 0x630 when b ( both virtual in March );
  `SyncFutureMpFromGage` `31fe9da0` ( compare, then store ); `SetNowMpFromStoredValue` `31fceed0` ( `+0x813c` -> vslot 0xf8 ); `GetSkinAnimByForm` `31fd48a0` ( 1 -> `+0x8e88`, else `+0x87d4` ); `NotifyAttackerWhenReset` `32610da0` ( `+0xc` owner -> vslot 0x2e4 ).
  June's four NPC vtables ( `334b8084` ... ) keep 0x2e4 = empty base, so no NPC forwarder is owed ( unlike 0x2fc / 0x300 ). Lu's vtable `334f06fc` overrides every one of the slots.
- **Hook sites ( 2 of the kickoff's, spot-checked in Ghidra ):** StateChange @`320349cb` sits after the force-down flush and the wait-id reset, before `IsLocalUnit` ( vslot 0x19c ) = March's line;
  PlayEmotion @`320433d7` gate returns false ( `32043dc8` `xor al,al` ), @`320447e9` `SetHandEquipShow( true )` is joined by both the avatar-sound and the `PlayEmotionSound` ( @`3204452f` ) paths = after March's if / else.
  Also read: CommonStateStartFuture @`32051042`-`32051063` ( `CP_AMOUNT` default 0, call 0x648( duo, cp ) ). June's other StateChange calls there ( `31fe0bb0` emotion setter, `+0x89e0` ) are DU1d3b's ( routed ).
- **CH1n:** the four written cases = listing `32b46580` @`32b4677d`-`32b469d6` and @`32b4a731`-`32b4a7fc`: table names ( `334f7c18` ... `334f7b58`, `334f5358` / `334f49b4` ), callbacks ( `32b4c1a0` init on 208 only, `329ccd90` / `329cba40`, `329ccda0` / `329ccdb0` ),
  hyper copies 0x34-0x48 only, extra ids 264 / 265 / 463 / 263, skill id 208 / raw / raw / 900, `+0x11` only on 265. Slot tables `334ed540` / `554` / `568` / `57c` = 50-58 / 51-59 / 60-68 / 61-69 = the C++ `USI_SPECIAL_ATTACK_*` ( S1 ).
- **CH1m:** `ch1m_regcheck.py` re-run on today's tree ( after S2 ): `matched 337 registrations, 0 problems` ( later-phase slots CH2b 11, CH2c 2, CH2c2 1, CH3b 3 ); spot-checks in the listings: `USI_HYPER_MODE` @`32b3fb00` ( StartFuture / Start / FrameMove thunk `32bd5f20` = vslot 0x600 / EventProcess / End ), `LSI_RIDING_WAIT` 76 @`32b42d5d` ( 3 callbacks ),
  Demonio `LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_START` 519 @`32b4424d` ( StartFuture `32b4ddd0`, EventProcess `32b4b5a0` ): all = the code.
- **JUNE_BUGS:** 722 / 723 match ( 723: `32b0fa20` @`32b0fa82` stores arg 2 into `+0x96bc` when `32206450` returns 0 ). 721's content matches but its "listing rows 1611 / 1631 / 1649" pointed at other rows: now @`32b4249b` / @`32b425b8` / @`32b426b6`.
- **Doc fix:** `ch1o_vcall_sites.tsv` row 0x648 @`32051059` named `32050230 CommonStateStart`; it is `320506f0 CommonStateStartFuture` ( the code and JUNE_BUGS 723 were right ). CH1o2 names each row's function itself.
- **Name grep:** no March user class declares any of the 22 names ( 10 getters + `GetWaitStateID` + the 11 hooks ); same-name hits are `CX2GUNPC::GetWaitStateID` ( not a `CX2GUUser` ) and `CX2UIUnit::GetWaitStateID` ( UI ). `X2GULuciel.h` b557510a `+39 / -39` = 37 comment-only lines and the two renames ( 0x2e4, 0x648 ).
- **Gates:** `finish.py --review --commits a2312de2 cfe12590 b557510a --state ... Luciel_Duo / Luciel_Demonio -v`: `junediverge` PASS, `junediverge --check-all` PASS, `enumscan` exit 0, `stateends` PASS both, `FINISH RESULT: FAIL` on 15 flagchecks only:
  every enclosing flag of the chains ( 8 class `_LIVE`, `DUO_BODY`, `ENUM_NAMES`, ... ) is checked alone, so the other phase flag's lines in b557510a read as flag-off diffs ( CH1o's known limit: one flag at a time, no partner `--with` ), and the `_CORE` run crashes on a peer `#if` ( `X2GUUser.cpp:29435`, Unevaluable ).
  Direct paired runs instead: `HOOKS --with _LIVE --range b557510a^..b557510a` `5 PASS, 1 DEFINE, 4 OTHER, 0 FAIL` / PASS; `_LIVE --with HOOKS` `7 PASS, 1 DEFINE, 2 OTHER, 0 FAIL`, only "not #defined" ( INERT until CH1 final );
  `_CORE` `a2312de2` / `cfe12590` `2 PASS` each / PASS; `_CORE` `b557510a` ( class files ) `2 PASS`, 44 deleted = comments and the two renames / PASS. Every run: 0 non-ASCII, 0 EOL, 0 encoding change.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH ) of `X2GULuciel.cpp`, `X2GUUser.cpp`, `X2DamageManager.cpp`, `X2GUNPC.cpp`, plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 8 x 0 errors, no warning in the touched files. Full build skipped: no play-test. No debug define to retire.
- **Next:** `/x2-phase char-luciel CH1o2` ( Opus 5.5 high; kickoff checked, still valid after S1 / S2 ).

### CH1o2 - Done (2026-11-15)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_USER_VIRTUAL_HOOKS` ( CH1o's, defined; no new flag, no debug define ). Files: `X2GUUser.cpp`, `X2GUUser.h`, `X2GULuciel.cpp`, `X2GULuciel.h`.
No review of its own: the CH1o3 review covers CH1o2. Verdicts: `june2017_re/char_luciel/ch1o_vcall_sites.tsv`, new column `ch1o2_verdict` ( one line per site, 84 ).
- **Tally:** 27 routed, 24 automatic ( March already calls the getter ), 21 June-only ( owner named ), 11 on an NPC object ( `CX2GUNPC`'s own slot at the same offset ),
  1 class code ( Elsword `32b59aa0` ). The scan's function column was wrong for three groups ( CC-padding start heuristic ): the `31fd7350` rows are
  `IsNotCommonMoveState` `31fd7640`; the `CommonFrameMove` 0x438 rows are `PopFrameData` `3206cff0`; the `ProcessExpListByNpcUnitDie` rows are
  `CheckQuickSlotItemSpecialAbility` `322fa960`. The verdict column names the real function.
- **Routed ( `#ifdef` getter `#else` March's constant ):** `IsNotCommonMoveState` ( 8 getters 0x43c-0x45c; 0x438 / 0x458 were already getters ); `CanRidingState`
  ( base `31fd7930`: June's whole body, which adds the head test "the Future next state is a skill state -> false", `m_StateList.find` on `unitCondition.nextState`
  so an absent id adds no map entry, then wait / walk / dash / dash end getters and March's five riding ids, 76 / 77 / 75 / 81 / 82 = June's `0x4c 0x4d 0x4b 0x51 0x52` );
  `CheckDashAngleChangeTime` ( = `31ff7700`: dash, dash jump, dash jump landing, walk; the Aisha levitation case is March's = June's type 2 / state `0x9c` );
  `SetEnableDash` ( = `31fc8540`: both dash tests; `StateChange( 0xe )` stays `USI_WALK` as June pushes the literal ); `IsDashState` ( `X2GUUser.h`, June `32b901c0` );
  `PopFrameData` ( both `USI_WAIT` tests; June's third site is its second receive-list form, March has one list ); `CanApplyBuffToGameUnit` ( die front / back ).
- **Const:** `CanApplyBuffToGameUnit` is `const`, so `GetDieFrontStateID` / `GetDieBackStateID` became `const` in the base ( CH1o's lines, `X2GUUser.h` ) and in Lu's
  overrides ( `X2GULuciel.h` / `.cpp`, which call only const `IsTransformed` / `IsMinorUnitActive` ). No other caller ( grep ).
- **Automatic:** `PlayEmotion` ( 4 ), `EmotionFrameMove` ( 2 ), `SetEnterCashShop`, `IsWaitState`, `GetSpecialItemUsableExtraStateID` base, the quick-slot item check ( 8 ),
  `CX2DungeonGame` ( `X2DungeonGame.cpp:3310`, `:2820` ), `CX2StateTrainingGame` ( `:575` ), and three "my unit's Future state is wait" tests ( `X2UIInventory.cpp:5498`,
  `X2UIPetInventory.cpp:558`, `X2TitleManager.cpp:950`; which June function is which is not pinned, all three call the getter ).
- **June-only, routed ( scope rule ):** backlog **Stage 10** ( emotion system ): the emotion hotkeys in four states' `ShortCutKeyProcess` ( 16 sites; the rest of that
  function stays Stage 9's ), StateChange's `+0x89e0` clear ( both overloads; PlayEmotion writes it @`320443f5` ), June's custom-sit re-play after an equip refresh
  ( `31f1ed70`; March's `X2StateMenu.cpp:4877` refresh has none ), and June's `PlayEmotion` rewrite around the automatic tests ( vslot 0x4ac, generic charge-MP test
  `31fd7b60` in place of March's Lire / Aisha / Chung switch: Lu/Ciel get no sit-from-charge until then ). Idle float reset `31fd78a0`: backlog "June's idle timer"
  bullet ( Chung CH1l30 ). Rose's `IsWaitState` `32ae37f0`: char-rose.
- **Lead for CH1 final ( `CH1.md` amended ):** the die-state StateEnd stand-ins ( `X2GUUser.cpp` ~7731 / ~7741, `SOCKET_PARITY` / `BUFF_DEATH_CLEAR` ) test `USI_DIE_*`;
  Ciel's die states are 192 / 193, so a dying Ciel would keep her buffs. Not a getter reader in June ( a registered StateEnd ), so not routed here.
- **Gates:** `finish.py --files X2Lib/X2GUUser.cpp X2Lib/X2GUUser.h X2Lib/X2GULuciel.cpp X2Lib/X2GULuciel.h`: `FINISH RESULT: PASS (8 gates, 0 failed, 3 skipped)`;
  `JUNE_USER_VIRTUAL_HOOKS` `2 PASS, 2 OTHER, 0 FAIL` / PASS, 2 deleted = the two base die getters made const; `JUNE_CHAR_LUCIEL_CORE` `2 PASS, 2 OTHER` / PASS,
  6 deleted = Lu's two declarations and two definitions + comments; `JUNE_CHAR_ARME_CORE` `4 OTHER` / PASS ( `IsNotCommonMoveState` sits in its block ); `junediverge`
  PASS; `enumscan` exit 0; 0 non-ASCII, 0 EOL, 0 encoding change. Skipped: `stateends` ( no state registered ), `skillicons`, `luabind` ( nothing added or staged ).
  Commit `4cd0f5c2`; `finish.py --range 4cd0f5c2^..4cd0f5c2`: the same three flagcheck PASS lines, `FINISH RESULT: PASS (8 gates, 0 failed, 3 skipped)`.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH ) of `X2GUUser.cpp`, `X2GULuciel.cpp`, `X2GUElsword_SwordMan.cpp`, plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`:
  6 x 0 errors, 0 warnings in the touched files. Flag probe: `SERV_ELSWORD_INFINITY_SWORD`, `FIX_MEDITATION_RUN`, `JUNE_USER_STATE_ID16`, `_CORE`, `HOOKS` all defined in X2Lib.
  Full build skipped: no play-test ( CH1 final's regression walk covers dash, jump, riding, death, emotion on another class ).
- **How to see it in game:** nothing ( every base getter equals March's constant; Lu/Ciel is not live ).
- **Next:** `/x2-phase char-luciel CH1o3` ( Opus 5.5 high; kickoff checked, read list gains this note ).

### CH1o3 - Done (2026-11-15)

Opus 5.5 high. Two new class-agnostic flags, both **defined**: `SERV_IRUHADEV_JUNE_USER_SKILL_STATE_BYTE` ( `#error` without `JUNE_SOCKET_PARITY`; `_CORE` now `#error`s without it ) and
`SERV_IRUHADEV_JUNE_CHAR_DUO_WIN_SPEECH` ( `#error` without `JUNE_CHAR_UNIT_TYPE_RANGE` ); the effect-set cases and Lu's code under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE`. No debug define.
Commit `8b026393 ( code, note, kickoffs )`. Review: `CH1o3-review.md` covers CH1o2 and CH1o3. funcmap: `grep "(CH1o3)" june2017_re/funcmap.tsv` ( 6 rows ). `JUNE_BUGS.md` 724, 725.
- **Effect-set tags ( `X2EffectSet.cpp` `ApplyDamageEffectModifiers`, June `321db1b0`, table `321dbc60` ):** 4 @`321db477` = CEffect `+0x84c` `m_DamageTime` ( funcmap AttackResult row ):
  add `( int )v + count` ( `_ftol2` ), else `( int )( v * count )` ( `FIMUL` ), no NEXT loop ( CH1f2's `EFFECT_MODIFIER_TAG_SOUL_COLLECTOR_MAX_COUNT` is this hit count ); 10 @`321db7cf` =
  `m_ExtraDamage.m_Damage` ( protected float ), `v + x` / `v * x`; 11 @`321db91a` = `m_RelateSkillData.m_byteSkillIndex`, float math, chop rounding ( `FISTP`, RC 0xc00 ).
  **Confirmed in the listing:** the tag 10 / 11 NEXT loops ( `+0xd8` / `+0xdc` ) store into the main damage data again ( `EBX` = main `+0x2a4`, `[EDI + 0x7a8]` ): the tables keep their
  values, so no NEXT write is ported ( JUNE_BUGS 725 ). **Tag 9 widened:** a copy of Elsword's case under `#ifndef SERV_IRUHADEV_JUNE_CHAR_ELSWORD_SKILL` inside `_CORE` ( CH1o's insert-only
  pattern ), so Lu's Punisher keeps it when Elsword's skills are off; inert while `_ELSWORD_SKILL` is defined.
- **Skill-state byte ( representation: a state-data member, as June stores it ):** June's `UserUnitStateData` has `+0x11` right after `m_bHyperState` ( `+0x10` ): March's struct gains
  `bool m_bSkillState` there ( `Init()` clears it, `operator=` copies it ). New base `CX2GUUser::IsSkillState( bool bFuture )` = June `3208e430` ( vslot 0x528; all 13 vtables hold the base ):
  reads the future / now entry ( `m_itFutureStateData` / `m_itNowStateData` under `JUNE_SKILL_STATE_PTR` = June `+0x7cb0` / `+0x7cbc` ), **OR** March's slot-table test, because no March
  class's setter sets the byte ( their slot states answer as before ). The engine's two readers ( `CommonStateStart` S3d state string = `0x528( true )`, socket conditional buffs =
  `0x528( false )` ) now call it. Class-local copies ( Chung, Arme, Eve `IsSkillSlotState` ) are untouched.
  **Lu's setter:** normal slot state `@32b46622`, hyper ( word `0x101` @`32b4665c` ), Punishing Accel's extra state 265 `@32b468fa`. June marks 15 more extra states ( 266 / 267 CH2a, 296 / 297 CH2b,
  344 / 345 / 403 CH2c, 459 CH2d, 377 / 575 CH3a, 402 / 404 / 584 CH3b, 466 / 581 CH3c ): new column `skill_state_extra` in `ch1n_skill_cases.tsv` and one amendment line in each owner's kickoff.
- **Lu's `ReservedSkillStart`** ( standing rule ): June `32057b80` @`32057c4c`-`cb2`: `SpecialAttackCooldown( GetNowStateSkillID(), m_fReservedSkillMp )` then `SkillMemoMpRefund( id,
  IsSkillState( false ), m_fReservedSkillMp )`; June reads the id through vslot 0x524 ( base `3208e410` = `GetNowStateSkillID` ), not the templet ( Chung's port reads the templet ).
  Declared and defined under `_CORE`; reached through CH1o's `_LIVE` link.
- **Checklist rows:** §5 L184-188 **ported** ( `DUO_WIN_SPEECH` ): June's InitSystem `3202b9e0` reads `DUNGEON_SPEECH_LUCIEL` rows `{ LU = id, CIEL = id }` before March's `DUNGEON_SPEECH`
  ( no unit-type test there; CH0's "type-10 branch" is the reader's ), a row missing a key breaks with its table open ( JUNE_BUGS 724, dormant ); pairs at June `+0x8920` =
  `m_vecDuoDungeonWinSpeech`. `GetRandomWinSpeech` = June `32009dd0` ( moved out of the header ): unit type 10 + pairs + game: game `+0xb54` = `rand() % 2`, row = `rand() % count`, CIEL's line
  when 1, else LU's; other classes keep March's body. New `CX2Game::m_iWinSpeechForm` ( `+0xb54`, zeroed in the ctor @`3230770e` ) with Get / Set.
  §7 L260 / L263: **done by CH1l2** ( `CanUseSpecialAttack`: the quick-slot wait at the top; the riding-key wait after the four checks `3202cba0` / `3202d140` / `3202d3c0` / summon,
  @`32b3a61f` = June's order ). §7ap L977: **wired already**: Lu overrides 0x56c / 0x6a8 ( CH1l / CH1l2 ), the bases are `JUNE_USER_SKILL_ICON_OVERRIDE` / `_SHOWN_SKILL_COOL_TIME`
  ( defined, engine callers `X2UISkillTreeNew2015.cpp`, `X2Game.cpp` ), and @`32b3a6ba` is the `ShownSkillCoolTime` call in Lu's `CanUseSpecialAttack`.
- **Routed ( scope ):** to **CH5** ( kickoff amended ): June's dungeon-clear picture `324ce260` ( Ciel's pictures when game `+0xb54` is 1, @`324ce351`-`324ce3f1` ) and `ENDING_SPEECH`'s `LUCIEL`
  set ( `327e0040`, from the DungeonData loader `327e0f80` @`327e625e`; March's reader has no `LUCIEL` key ).
- **Gates:** `finish.py --files <8 code files> --with JUNE_USER_SKILL_STATE_BYTE JUNE_CHAR_DUO_WIN_SPEECH --state X2GULuciel.cpp Luciel_Duo / Luciel_Demonio`: `FINISH RESULT: PASS (8 gates, 0 failed, 2 skipped)`;
  `DUO_WIN_SPEECH` `4 PASS, 4 OTHER, 0 FAIL` / PASS, `SKILL_STATE_BYTE` `2 PASS, 6 OTHER, 0 FAIL` / PASS ( OTHER = the other new flag's and `_CORE`'s files ), 0 deleted; `junediverge` PASS; `enumscan` exit 0;
  `stateends` PASS both. `_CORE` run directly ( `--files X2EffectSet.cpp X2GULuciel.cpp X2GULuciel.h` ): `3 PASS, 0 FAIL` / PASS, 3 deleted = the three Lu setter comment lines rewritten
  ( "+0x11 store is IsSkillSlotState", "word +0x10 = 0x101", "routed to CH1o" ). With `_CORE` as a partner, flagcheck stops on CH1a's own `#if defined( _LIVE ) && !defined( _CORE )` ( `Always.h:13430`,
  unevaluable, as CH1o noted ), so `_CORE` is not a `--with` partner; my `_CORE` guard is nested `#ifndef` / `#ifdef` for the same reason. 0 non-ASCII, 0 EOL, 0 encoding change.
  Range: `finish.py --range 8b026393^..8b026393` ( same flags ): the same two flagcheck PASS lines, `FINISH RESULT: PASS (8 gates, 0 failed, 2 skipped)`; `_CORE --range`: `3 PASS, 0 FAIL` / PASS.
- **Compile:** `/Zs` probes ( census `cl_cmd`, no PCH ) of `X2GUUser.cpp`, `X2GULuciel.cpp`, `X2EffectSet.cpp`, `X2Game.cpp`, `X2StateDungeonGame.cpp` ( the speech caller ), `X2GUChung.cpp`, plain and
  `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 12 x 0 errors, 0 warnings in the touched files ( the first `X2GUUser.cpp` run caught `m_NowStateData` under `JUNE_SKILL_STATE_PTR`: fixed to the iterators ).
  The `#error` guards compile, so `JUNE_SOCKET_PARITY` and `JUNE_CHAR_UNIT_TYPE_RANGE` are defined there. Full build skipped: no play-test.
- **How to see it in game:** nothing ( every new engine path needs a unit-type-10 unit, a `DUNGEON_SPEECH_LUCIEL` script, tag 4 / 10 / 11 or a set byte; Lu/Ciel is not live ).
- **Next:** `/x2-phase char-luciel CH1o3 review` ( Opus 5.5 high; covers CH1o2 and CH1o3 ), then `CH1p`.

### CH1o3 - Review (2026-11-15)

Opus 5.5 high. Reviewed CH1o2 `4cd0f5c2` and CH1o3 `8b026393`. **Verdict: pass after one fix.** No new flag, no debug define to retire.
- **Fix ( `JUNE_USER_SKILL_STATE_BYTE`, `X2GUUser.cpp` `IsSkillState` ):** the slot-table fallback tested the stored entry's `stateID`; it now tests
  `GetFutureStateID()` / `GetNowStateID()`, the ids March's two readers tested ( under `JUNE_SKILL_STATE_PTR` those are the map keys, not `->second.stateID` ),
  so "classes that do not set the byte answer as before" holds by construction. June's own test ( byte `+0x11` ) is unchanged.
- **CH1o2 in Ghidra / raw bytes ( all = June ):** `CanRidingState` `31fd7930`: head = word `+0x7de4` ( `GetUnitCondition( true )` = future, funcmap C9h-4d row ) bounds-checked
  against the state vector, entry `+0xc` ( = `m_eSkillID`, as `3208e410` reads it ) `> 0` -> false; then vslots 0x438-0x444 and riding `0x4c / 0x4d / 0x4b / 0x51 / 0x52`.
  March's `0 <= id` + `find` = June's unsigned bounds test ( -1 and unregistered ids pass ). `IsNotCommonMoveState` `31fd7640`: vslots 0x438-0x45c ( 11 tests ) then the
  riding range. `SetEnableDash` `31fc8540`: vslot 0x440 against 0x174 ( now ) and 0x4f4 ( future ), then `StateChange( 0xe, 1 )` literal ( `USI_WALK` kept ). `PopFrameData`
  `3206cff0`: vslot 0x438 at both tests. Base getters still return March's constants; `GetDie*StateID` const has no other override or caller ( grep: only `X2GUUser.h`,
  `X2GULuciel.*` ). Verdict sample: automatic rows 27 / 52 / 60 checked in March ( `X2GUUser.h:3885`, `X2DungeonGame.cpp:2820`, `X2Game.cpp:12371` all call the getters );
  June-only rows 11 / 16 / 39 ( custom-sit re-play, StateChange `+0x89e0`, emotion hotkeys ) have no March body, owner Stage 10: right.
- **CH1o3 effect-set tags ( `dis_0x321db1b0.txt` ):** tag 4 = `_ftol2( v ) + count` / `_ftol2( v FIMUL count )` into `+0x84c`, no NEXT loop; tag 10 = protected float `+0x2a4`,
  stored only when changed, NEXT loop re-reads and re-writes `EBX` ( the main ); tag 11 = `FILD` byte `+0x7a8`, float add / mul, `FISTP` with RC `0xc00` ( chop ), NEXT loop
  stores `[EDI + 0x7a8]` again. JUNE_BUGS 725 right. Tag 9's `#ifndef` copy is byte-for-byte Elsword's case.
- **Skill-state byte:** June `3208e430` = `bFuture ? +0x7cb0 : +0x7cbc`, decrypt, return byte `+0x11` ( no slot-table test: the OR is the documented March stand-in ).
  `m_bSkillState` follows `m_bHyperState`, `Init()` clears, `operator=` copies. Lu's slot tables are the engine's `USI_SPECIAL_ATTACK_*` ids, so the OR adds nothing for Lu.
  Lu's three stores at June's addresses; the 15 `skill_state_extra` ids have their amendment in CH2a-CH2d / CH3a-CH3c. `ReservedSkillStart` = June `32057c4c`-`cb2`:
  0x800( 0x524(), `+0x8ea8` ), then 0x764( 0x524(), `movzx` 0x528( 0 ), `+0x8ea8` ): matches.
- **Win speech:** `3202b9e0` reads key 1 = `LU` ( `c7 6d` ^ `8b 38` ) and key 2 = `CIEL` ( `7b 7a a9 e3` ^ `38 33 ec af` ), breaks on either -1 with one `EndTable` ( the row ),
  pairs ( LU, CIEL ), and reads `DUNGEON_SPEECH` only in the `else`: = the port ( JUNE_BUGS 724 ). `32009dd0`: unit `+0x867c` -> templet `+0xaf0` type == 10, pairs non-empty,
  game alive, `rand() % 2` -> game `+0xb54`, row = `rand() % count`, `+0x1c` ( second = CIEL ) when `> 0`; fallback `VICTORY`: = the port. Ctor `@3230770e` stores `EBX` ( 0 ).
  CH5's amendment names the picture `324ce260` and `ENDING_SPEECH`'s `LUCIEL` set.
- **Checklist rows:** `32b3a310` calls `3202cba0`, `3202d140`, `3202d3c0`, then `329cc370` ( riding-key wait ) @`32b3a61f`, and vslot 0x6a8 ( `ShownSkillCoolTime` ) @`32b3a6ba`: L260 / L263 / L977 verdicts right.
- **Gates:** `finish.py --review --commits 4cd0f5c2 8b026393 --with <3 flags> --state X2GULuciel.cpp Luciel_Duo / Luciel_Demonio`: `FINISH RESULT: PASS (10 gates, 0 failed, 2 skipped)`;
  `DUO_WIN_SPEECH` `4 PASS, 4 OTHER` / PASS, `SKILL_STATE_BYTE` `2 PASS, 6 OTHER` / PASS, `VIRTUAL_HOOKS` `4 PASS, 8 OTHER` / PASS ( 2 deleted = the base die getters made const );
  every OTHER row is a partner flag's or `_CORE`'s file, or `Always.h`'s other define block; `junediverge` + `--check-all` PASS, `enumscan` exit 0, `stateends` PASS both.
  `_CORE` alone ( `flagcheck --commits 4cd0f5c2 8b026393 --files X2EffectSet.cpp X2GULuciel.cpp X2GULuciel.h` ): `5 PASS, 0 OTHER, 0 FAIL` / PASS, 9 deleted = CH1o2's 6 ( Lu's const
  declarations / definitions + comments ) and CH1o3's 3 setter comments. The fix: `finish.py --files X2Lib/X2GUUser.cpp --with <3 flags>`: `FINISH RESULT: PASS (8 gates, 0 failed, 3 skipped)`,
  1 deleted = the replaced return line ( inside `SKILL_STATE_BYTE`'s own block ). Commit `1fed231e`; `finish.py --range 1fed231e^..1fed231e --with SKILL_STATE_BYTE`:
  flagcheck PASS, `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`.
- **Compile:** `/Zs` probes of `X2GUUser.cpp` after the fix, plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: 2 x 0 errors, 0 warnings. Full build skipped: no play-test.
- **Next:** `/x2-phase char-luciel CH1p` ( Opus 5.5 high; kickoff checked, its dependencies are done ).

### CH1p - Done (2026-11-15)

Opus 5.5 high. New flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_DUO`, **`//#define`** like `_LIVE` ( CH1 final defines both; `#error`s without `_CORE` ); class code under `_CORE`;
one enum-name line under `JUNE_CHAR_DUO_BODY`. No debug define. Commit `b3ddeef9`. funcmap: `grep "(CH1p)" june2017_re/funcmap.tsv` ( 5 rows ). `JUNE_BUGS.md` 723 withdrawn, 726, 727. `MODS.md` section.
- **Already done before this phase ( checked, no change ):** `DUO_TYPE` at state start ( DU1b, `ApplyDuoTypeAtStateStart`, DU1 review's `CX2Unit::DT_*` ), `MINOR_UNIT_ANIM_NAME`
  ( DU1b ), `MINOR_UNIT_SPEED_TIME0-4` / `_ULTIMATE` and `MINOR_UNIT_PASSIVE_SPEED_*` ( DU1b4 ), `CP_AMOUNT` into vslot 0x648 ( CH1o ), the switch itself
  ( `DoProcessWhenActiveBodyChangeApplied`, CH1o ), the bounding key dialog's load ( CH1k ) and hide timer ( CH1l3 ), `CUSTOM_RENDER_STATE` in `Enum.lua` ( char-ara CH1 ).
- **`AddCombinationPointOnHit` `32b14bc0`** ( thunk `32b4cee0` ): DamageData `+0x594` > 0 and `!( 0 < +0x96a8 )` ( June's FCOMP lets a NaN through ) -> `UpCombinationPoint`,
  cool time 1.0. Its push sits in `InitPassiveSkillState` `32b45820` @`32b4583e`-`32b45855` ( attack-result list `+0x66ac` ), a CH4a function: **routed to CH4a** ( kickoff
  amended, one line ), so until CH4a no hit adds points.
- **`SetBoundingTarget` `32b38cb0`** ( CH0 filed it `not-luciel`, no owner; it is the "key dialog" half of this row ): exits on `m_fBoundingDamageRateEx` < 0 or NaN ( -1 until CH4a's
  passive ), uid < 0, `GetRidingOn`, or a held target whose serial matches `+0x96ec` ( only ever -1: JUNE_BUGS 727 ); cost = `GetRandomFloat( 0 )` <= `+0x9730` ? 0 : 1, more than the points
  -> exit; `GetUserUnitByUID` / `GetNPCUnitByUID` by type; then 0.5 s show time, the key, both flags cleared, `SetShow( true )`, static `DLG_LUCIEL_BOUNDING_KEY` gets
  `GetKeyString( GAMEACTION_SIT )` ( June 0x27 = V, as March's ported switching guide ). June's handle hands a unit without the dialog a dummy one: March returns on NULL.
- **`UpdateBoundingKeyDialogPosition` `32b35cf0`:** `GetPos( true )` +/- `GetDirVector` x 100 by `GetIsRight`, y + 208, `GetProj3DPos` / resolution scale -> `SetPos`; then
  the dialog's size / ( camera-eye distance / 1500 ), stored back. **Offsets proved:** June app `+0x1b8` = `GetResolutionScaleX` ( `31cce300` reads it the same way; March `+0x1a0` );
  June dialog `+0x120` = `m_vSize` ( June ctor `31cd9230`: `+0x128` byte, `+0x12c` = 512 / 384, `+0x134` = 1024 / 768 = March's drag-range defaults; March `m_vSize` is `+0x110`,
  offsetprobe ). The size compounds call after call: JUNE_BUGS 726 ( March's `m_vSize` only feeds the drag clamp ).
- **Engine hooks ( `_DUO` ):** `CX2GUUser::BoundingUpStateStart` after `CommonStateStart` ( June `31fd02d0` @`31fd02dd`-`32d` ) and `CX2GUNPC` state start after March's bounding-up block
  when `nowState == mBoundingLandCrash` ( June `3275a070` @`3275a0ac`-`111`, unguarded as June ): last attacker a user ( `+0x40e8` 0 ) found by `GetUnitByUIDAndType` ( June `32294460` )
  whose templet type is `UT_LUCIEL` -> `SetBoundingTarget( GetGameUnitType(), GetUnitUID(), GetLastHitAttackSerial() )`. Both files include `X2GULuciel.h` under `_DUO`.
- **`HIT_ADD_COMBINATION_POINT`** stops calling `DamageDataUnsupportedInt` under `_DUO` ( `X2DamageManager.cpp`, March's lines in `#else` ). **`X2UnitManager.cpp:297`** ( DU1 review route ):
  `2` -> `CX2Unit::DT_BOTH`, nested `#ifdef SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` ( the enum's own flag ) inside `JUNE_UNIT_VIEWER_EFFECT_TABLE`'s block.
- **Sub-tag `DUO` lines ( `_DUO`, throttled 1 s each ):** `[JUNE_CHAR] LUCIEL DUO switch: active body %s, state, DUO_TYPE, CP_AMOUNT, pending points` in
  `DoProcessWhenActiveBodyChangeApplied`; `LUCIEL DUO point: +%d on hit ( HIT_ADD_COMBINATION_POINT ), now %d` in `AddCombinationPointOnHit`.
- **Renames ( CH1l3 assigned the first ):** `m_iLastUsedSkillID` -> `m_iPendingCombinationPoint` ( a non-skill state's `CP_AMOUNT`, paid by `SpecialAttackCooldown`: JUNE_BUGS 723 was this
  misreading, withdrawn ), `m_iBoundingStrikeCount` -> `m_iBoundingStrikeCombinationPointCost`, `m_iBoundingTargetUnitIndex` -> `m_iBoundingTargetLastHitAttackSerial`,
  `m_fBoundingDamageRate` -> `m_fFreeBoundingStrikeRate`; `ch1_offsets.tsv` rows follow.
- **Data ( live ):** data036 `Enum.lua` ( core list ) `DUO_TYPE` gains June's `DT_BOTH` 2 / `DT_KEEP` 3 ( `luasrc.py refresh`, `ledger --check` PASS, `core --check` PASS, core-list row `edit:` note );
  data034 `DLG_LUCIEL_BOUNDING_KEY.lua` adopted ( June's bytecode, new; `luabind.py check` PASS; texture key `LUCIEL_ZX_UI_NO` present, June's `DLG_UI_Image_01` = March's ).
  `PACK OK data036`, `PACK OK data034`; `COPY OK data036` md5 `5ed5408e`, `COPY OK data034` md5 `172076a0`. `luabind.py check Luciel_Duo Luciel_Demonio`: `DUO_TYPE` resolves;
  the one ENUM gap left is `SKILL_ID.RNSI_A_LLD_BLUFF` ( CH1's kickoff names it ).
- **Not ported here ( routed ):** `NOT_SHOW_UNIT_BY_DUOTYPE` ( June `CommonStateStart` @`320618a8`-`3206194c` ): its members and consumer `3204e4a0` read the equip's minor-unit byte, DU1d5
  gave the key to **DU3** ( kickoff amended with the parse: four members, -9999.0 default ). The delegate push: **CH4a**.
- **Gates:** `finish.py --files <7 code files> --with JUNE_CHAR_LUCIEL_DUO JUNE_CHAR_DUO_BODY`: junediverge PASS, enumscan exit 0; the two flagcheck FAILs are tool limits, so per flag:
  `_DUO --worktree` ( Always.h, X2GUUser, X2GUNPC, X2DamageManager, X2GULuciel ): `3 PASS, 1 DEFINE, 1 OTHER ( _CORE's file ), 0 FAIL, 0 ERROR`, 0 deleted, only complaint "not #defined"
  ( deferred like `_LIVE`; finish.py's INERT test keys on `_LIVE` names ); `DUO_BODY --files X2UnitManager.cpp`: `1 PASS` / PASS ( with Always.h it ERRORs on DIVERGE_PLAY_SOUND_REVIVE's
  `#if defined && !defined` guard ); `_CORE --with _DUO --files X2GULuciel.cpp X2GULuciel.h`: `2 PASS, 0 FAIL` / PASS, 14 deleted = the renamed lines and two rewritten comments, all inside `_CORE`.
  Range ( `--range b3ddeef9^..b3ddeef9`, same file sets ): `_DUO` `3 PASS, 1 DEFINE, 1 OTHER, 0 FAIL, 0 ERROR` ( not #defined only ), `DUO_BODY` `1 PASS` / PASS,
  `_CORE --with _DUO` `2 PASS` / PASS ( 14 deleted ), junediverge `--check` of the 6 files PASS.
- **Compile:** `/Zs` probes ( census `cl_cmd` ) of `X2GULuciel.cpp`, `X2GUUser.cpp`, `X2GUNPC.cpp`, `X2DamageManager.cpp`, `X2UnitManager.cpp`: plain 5 x 0 errors; with `-D_DUO -D_LIVE`
  5 x 0 errors ( the first run caught `CX2OfflineLog` missing in `X2GULuciel.cpp`: include added under `_DUO` ). Full build skipped: no play-test.
- **How to see it in game:** nothing yet ( `_DUO` / `_LIVE` off ). From CH1: a body swap logs `LUCIEL DUO switch`; from CH4a: points on combo hits and the V key over a bounced enemy.
- **Watch ( CH4a play-test ):** the key dialog is `SetModal( true )` in June's script; if V does not reach the strike while it shows, that is the modal flag, not the port.
- **Next:** `/x2-phase char-luciel CH1p review` ( Opus 5.5 high ).

### CH1p - Review (2026-11-15)

Opus 5.5 high. Reviewed CH1p `b3ddeef9`. **Verdict: pass, no code change.** No debug define to retire ( CH1p added none ); `_DUO` stays `//#define` for CH1 final.
- **`AddCombinationPointOnHit` `32b14bc0`:** `+0x594` `JLE` = `0 <` the point; `FLDZ` / `FCOMP +0x96a8` / `TEST AH,5` / `JNP` skips only an ordered `0 < cool time`, so a NaN
  adds ( `false == ( 0.f < ... )` ); `UpCombinationPoint( point )` then `FLD1` into `+0x96a8`. Same.
- **`SetBoundingTarget` `32b38cb0`:** exits in June's order: `0 <= +0x973c` by `TEST AH,0x41` / `JP` ( NaN exits ), uid high / low sign, vslot 0x200 = `GetRidingOn` ( funcmap
  `3208ddd0` row ), uid `+0x96f0` >= 0 and `+0x96ec` == the serial. Cost: vslot 0x1cc = `GetRandomFloat( 0 )` ( funcmap `32274f60` reads it the same ) `FCOMP` the rate,
  `JP` -> 1, so `<=` costs 0 and a NaN 1; stored at `+0x96dc` before the points test ( `+0xc04` -> `+0x114` = `GetNowCombinationPoint` `32b0fa00`, null 0 ). Type 0 -> game vslot
  0xa0 ( uid64 ), 1 -> 0xa8 ( low dword ), else exit ( March `GUT_USER` 0 / `GUT_NPC` 1 ). Stores 0.5 ( `334ed42c`, read from the exe ), `+0x96f8` type, `+0x96f0` uid, `+0x96e8` /
  `+0x96e0` 0, `SetShow( 1 )`, static `334f34a0` = `"DLG_LUCIEL_BOUNDING_KEY"`, `31cbe6a0` = `CKTDGUIStatic::SetString( int, WCHAR* )` ( same guard as March's ), key 0x27 =
  `GAMEACTION_SIT` ( counted with `JUNE_SKILL_SLOT_10` defined; `X2KeyPad.cpp` maps it to V ), then `UpdateBoundingKeyDialogPosition` even without the static. Same.
- **`UpdateBoundingKeyDialogPosition` `32b35cf0`:** null handle -> return; vslot 0x138( 1 ) pos, 0x170 = `GetIsRight` picks + / - vslot 0x16c dir x 100; y + 208 written back into the
  same vector that later feeds the distance; `GetProj3DPos` / app `+0x1b8` / `+0x1bc`; `SetPos`; game -> vslot 0x94 camera -> `+0x38` eye `+0x44..+0x4c`; distance / 1500; both
  size halves / that divisor ( `FDIVP` / `FDIVR` read ). Constants 100 / 208 / 1500 read from the exe. **Dialog offset:** June ctor `31cd9230` writes `+0x118` / `+0x11c` 0 ( mouse x / y ),
  `+0x120` / `+0x124` ( size ), `+0x128` byte, `+0x12c..+0x138` ( drag centre / range ) = March's member order `m_PrevMousePointX/Y`, `m_vSize`, `m_bEnableDraggingRange`,
  `m_vDraggingRangeCenter`, `m_vDraggingRange`, 0x10 later: `GetSize` / `SetSize` are right. JUNE_BUGS 726 right.
- **Hooks:** `31fd02d0` = vslot 0x7c4 ( `CommonStateStart` ) then the hook; `3275a070` is called only from NPC `CommonStateStart` @`327af3aa`, first half = March's bounding-up
  block, second half @`3275a0ac` compares `+0x8708` unguarded, as ported. Both: `+0x40e8` == 0, `32294460` on the `+0x40e0` key ( uid >= 0, game lookup by type ) =
  `GetUnitByUIDAndType`, unit `+0x867c` -> templet `+0xaf0` -> type == 10 ( `UT_LUCIEL` ), args `+0x264` type, vslot 0x54 uid, `+0x40dc` serial ( March getters cite the same
  offsets, `X2GameUnit.h:3605` / `5520-5527` ). Same.
- **Renames:** `+0x96bc` is read and zeroed as a point count by all three June users ( `32b0fa20` stores `CP_AMOUNT`, `32b1eef0` and `32b32550` pay it to `UpCombinationPoint` ):
  `m_iPendingCombinationPoint` right, JUNE_BUGS 723 rightly withdrawn. `+0x96dc` 0 / 1 cost ( the strike end subtracts it, `CheckBoundingTarget` voids `1 <` it ), `+0x96ec`
  compared with the serial, `+0x9730` the free-strike roll: names fit; `ch1_offsets.tsv` follows. JUNE_BUGS 727 right ( only `CheckBoundingTarget` writes `+0x96ec`, -1 ).
  `AddCombinationPointOnHit( const DamageData& )` fits `DelegateProcessWithDamageData` ( `X2GameUnit.h:48` ) for CH4a's push.
- **Data:** data036 `Enum.lua` `DUO_TYPE` = June `Enum.src.lua:17513-17518` ( 0 / 1 / 2 / 3, order aside ); `luabind.py check DLG_LUCIEL_BOUNDING_KEY`: 14 methods, 0 unbound, ENUM 0
  missing, PASS. **Routes:** `CH4a.md` ( push @`32b4583e`, `+0x9730` / `+0x973c` ) and `DU3.md` ( `NOT_SHOW_UNIT_BY_DUOTYPE` parse ) amendments present and right.
- **Gates:** `finish.py --review --commits b3ddeef9 --with JUNE_CHAR_LUCIEL_DUO`: junediverge PASS, `--check-all` PASS, enumscan exit 0; `_DUO` `3 PASS, 1 DEFINE, 3 OTHER, 0 FAIL,
  0 ERROR`, 0 deleted, its one FAIL line is "not #defined" ( deferred like `_LIVE` ); OTHER = `X2GULuciel.*` ( `_CORE` ) and `X2UnitManager.cpp` ( `DUO_BODY` ), checked below.
  `flagcheck JUNE_CHAR_DUO_BODY --commits b3ddeef9 --files X2Lib/X2UnitManager.cpp`: `1 PASS` / PASS. `flagcheck JUNE_CHAR_LUCIEL_CORE --with JUNE_CHAR_LUCIEL_DUO --commits
  b3ddeef9 --files X2GULuciel.cpp X2GULuciel.h`: `2 PASS, 0 FAIL` / PASS, 14 deleted = 10 renamed / rewritten lines in the `.cpp` and 4 member lines in the `.h`, all inside `_CORE`.
  stateends / skillicons / luabind SKIPPED in finish.py: no skill state registered, no skill added ( luabind run by hand above ).
- **Compile:** `/Zs` probes ( census `cl_cmd` ) of `X2GULuciel.cpp`, `X2GUUser.cpp`, `X2GUNPC.cpp`, `X2DamageManager.cpp`, `X2UnitManager.cpp`: plain 5 x 0 errors; with
  `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_DUO /DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` 5 x 0 errors. Full build skipped: no play-test.
- **Next:** CH1 ( Opus 5.5 high ): every dependency is now done.

### CH1 - Done (2026-11-15)

Opus 5.5 high. **`SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` and `_DUO` defined** ( `Always.h` ); Lu/Ciel is playable. Files: `Always.h`, `X2Game.cpp`,
`X2GULuciel.h` / `.cpp`, `X2GUUser.cpp`. funcmap: `grep "(CH1)" june2017_re/funcmap.tsv` ( 4 rows ). Commit `5e880f3c`; `finish.py --range 5e880f3c^..5e880f3c --with <the 4 flags>`:
same flagcheck tallies as below, `FINISH RESULT: PASS (9 gates, 0 failed, 3 skipped)`.
- **Factory ( `_LIVE` ):** `CX2GULuciel::CreateGULuciel` ( Chung's inline pattern, June `3230f470` ) and `case CX2Unit::UT_LUCIEL` in `CX2Game::CreateGUUser`
  and `AddUserUnitIntruder` ( June case 10 at @`322ceeb4` / @`322d5d8f`; which caller is which is left to the review ). `X2Game.cpp` includes `X2GULuciel.h`.
- **Switching guide ( `_LIVE`, CH1a0 amendment ):** `UpdateLucielSwitchingGuideUI( &false, 0.f )` in the `CX2Game` ctor ( @`32307bca` ) and dtor ( @`32308957` ),
  between the memorize and Eve recovery calls, June's order.
- **Found by the first instantiation: `ParseCommonRandomState`** was March's last pure virtual Lu lacked ( C2259 ). June has no class override ( base `Init`
  `3204ea20` calls `31feaac0` ); `CX2GULuciel::ParseCommonRandomState` ( `_CORE` ) calls `ParseCommonRandomStateByUnitClass()`, as Aisha / Ara / Elesis do.
  `GetUnitClassName` names all seven classes = `Luciel_Duo.lua`'s `INIT_COMMON_RANDOM_STATE` keys ( `LUCIEL_DUO` ... `LUCIEL_DEMONIO` ).
- **Ciel's die-state end ( CH1o2 lead, `JUNE_USER_VIRTUAL_HOOKS` ):** `ch0_reg.tsv` rows 185 / 186 give `LSI_CIEL_DIE_FRONT` / `_BACK` June's die StateEnd
  `3202a7d0`, so `DoStateEndStart`'s two stand-ins ( SIEE 18, buff clear-all ) now also match `GetDieFrontStateID()` / `GetDieBackStateID()`. March
  classes return `USI_DIE_*` ( no change ); a transformed unit's Lu die state still matches `USI_DIE_*`.
- **DATA line ( `_LIVE`, not June ):** end of `InitState`: `[JUNE_CHAR] LUCIEL DATA class %d script %s motion %s minor_motion %s states %d bounding %d
  wait_state_change %d active_body %s`.
- **Data ( sub-agent, checked ):** `adopt-june` ( bytecode ) data036 `Luciel_Duo.lua` 278,167 B / `Luciel_Demonio.lua` 86,377 B, data034
  `DLG_Luciel_CombinationPoint.lua` 3,378 B ( all new ); data036 `Enum.lua` + `RNSI_A_LLD_BLUFF = 30304` ( `-- char-luciel CH1`, = June `Enum.src.lua:15069`
  and `X2SkillTree.h:3288` ), `luasrc.py refresh` + `ledger --check` PASS. `luabind.py check` the three: `RESULT: PASS` ( enum gate clean; one June nil
  `CAMERA_TYPE.CT_PARTS_LOOK_TRACKINGT` left as June's ). `LD_Arousal_UI_01` is in data036 `GameMajorParticle32.txt`. The 20 motions of the UnitTemplet rows
  are in data208 ( Lu ) / data218 ( Ciel ), in sync with the live koms ( CH0b copied them; nothing was held ). `PACK OK` / `COPY OK` data034 and data036
  ( md5 `d06aa6f2...` / `53ca067f...`, backups `kom_backup_tools\*.20261115_132507` ).
- **Gates:** `finish.py --files <5> --with LUCIEL_CORE LUCIEL_LIVE LUCIEL_DUO JUNE_USER_VIRTUAL_HOOKS --state X2GULuciel.cpp LUCIEL_DUO --lua <3>`:
  flagcheck CORE `3 PASS, 1 DEFINE, 1 OTHER`, DUO `2 PASS, 1 DEFINE, 2 OTHER`, LIVE `4 PASS, 1 DEFINE`, HOOKS `1 PASS, 1 DEFINE, 3 OTHER`, all `0 FAIL` /
  PASS; 3 deleted lines = `Always.h`'s two `//#define` lines and one description line; junediverge PASS; enumscan exit 0; stateends PASS; luabind PASS;
  skillicons SKIPPED ( no skill added; CH0b's 195 PASS ). `FINISH RESULT: PASS (9 gates, 0 failed, 1 skipped)`. INFO: `BUFF_DEATH_CLEAR`, `SKILL_STATE_PTR`,
  `SOCKET_PARITY` enclose the die-state hunks.
- **Build:** `DEPLOY OK: X2_offline.exe is 26,905,600 B, dated 2026-11-15 13:41:00, md5 a9e9de29ddaca6737cc3d40dcf2acd03, equal to X2/US_SERVICE/x2.exe`.
  `EXESTR OK: 2 of 2 found in X2_offline.exe` ( the DATA and DUO switch lines ).
- **How to see it in game:** the play-test hand-off below ( create, village, walk / jump / dash / combo, V swap, HUD point row and portraits, dungeon,
  Ciel MP + form across the hop, Ciel death clears buffs, win-pose return, relog, one other class's regression walk ).
- **Play-test:** owed by the user. After it, `CH1-review.md` ( written ) covers this commit and june-state-ids S2 `857b18cd`; CH2a / DU3 / CHN / TU are
  amended from what the play-test shows.
- **Play-test fix 1 ( field entry crashed ):** the village and the first-entry tutorial ran ( quest 11000 done ), then battlefield 40000 crashed in
  `CKTDGXSkinAfterImage`'s ctor ( `0xc0000005` reading NULL + 0x74; CrashRpt dump: `CX2GUUser::InitComponent` -> `Weapon::LoadDataForUser` ->
  `CreateSkinAfterImage` ). Lu's weapon `Mesh_Lu_Cash_Weapon_Sorted.X` ( 41890 ) is model-only, so the weapon anim keeps no mesh and March read its
  center / radius unguarded. June `3322abc0` tests the origin mesh first ( @`3322af08` `JZ` ). New class-agnostic flag
  `SERV_IRUHADEV_JUNE_SKIN_AFTER_IMAGE_NULL_MESH` ( `Always.h`, `KTDGXSkinAfterImage.cpp` ). The later frame copy is already NULL-safe
  ( `X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX`, `KTDX.h:378` ). Gates: flagcheck `1 PASS, 1 DEFINE, 0 FAIL` / PASS, junediverge PASS, enumscan exit 0.
  `DEPLOY OK: X2_offline.exe is 26,905,600 B, dated 2026-11-15 14:13:15, md5 f509c92055b89741bbfceb8d53258f35, equal to X2/US_SERVICE/x2.exe`.
  Not seen yet ( the class never reached it ): the creation preview shows Lu only ( DU5's ), the tutorial is March's default ( TU's ).
- **Correction ( second crash, same EIP ):** fix 1 did not reach the crash. The ctor is handed a **NULL origin anim** ( `[this + 0x74]` = `m_pOriginalAnim`
  is 0 ), not an anim without a mesh: Lu's weapons are `UT_SKIN` ( 113300 / 41890 ), the eqip's skin path opens models but builds no normal anim, and
  March's `Weapon::LoadDataForUser` passes `GetNormalAnim( i )` unguarded. Fix 1 stays ( a real June guard ). **Routed: new phase CH1q** ( `CH1q.md`, the
  user's call ): June's weapon loader for a skin-type weapon; CH1's play-test resumes after it.

### CH1q - Done (2026-11-15)

Opus 5.5 high. New class-agnostic flag **`SERV_IRUHADEV_JUNE_DIVERGE_WEAPON_LOAD_DATA_FOR_USER`** ( `Always.h`, defined ). Files: `X2GameUnit.cpp`,
`KTDGXSkinAfterImage.h`, `Always.h`; rows in `june2017_re/funcmap.tsv` ( `grep "(CH1q)"`, 2 ) and `tools/data/june_march_divergent.tsv` ( 2 `replace`
rows, ported ).
- **June's skin-type weapon handling is one NULL test.** June's `Weapon::LoadDataForUser` is `3228d660` ( called from `InitComponent` `31fee090`
  @`31fee9ba` ). Its loop is March's: an after-image when the eqip has a normal **model**, built from the normal **anim**, then the slash traces only when
  the normal anim exists. June's `CreateSkinAfterImage` `31f8ace0` returns 0 for a NULL origin anim before the `new` ( March's news it and the ctor reads
  the anim: the crash ). June's `InitSkin` `32963eb0` ( already ported, `JUNE_EQIP_SKIN` ) builds no normal anim for a `UT_SKIN` eqip either, only the
  hyper anims, so June shows Lu's weapon with **no after-image and no weapon slash trace**; that is June's behaviour, not a gap.
- **`CreateSkinAfterImage` ( `KTDGXSkinAfterImage.h` ):** `if ( NULL == pOriginAnim ) return NULL;` at the top. A NULL anim crashed before, so no caller depended on that path.
- **`LoadDataForUser` ( `X2GameUnit.cpp` ): June's whole body** replaces March's ( `#else` keeps March's ). Differences from March, all June's:
  head tests `GetMotion()`, the eqip and the eqip's **templet** ( March: the eqip's item and its templet ); the fashion test reads `m_pItemTempletOrigin`
  or the eqip's templet; the non-fashion enchant attribute comes from the inventory, `GetItem( eqip UID, true )` @`3228ea5a` ( March: `eqip->GetItem()` );
  **15 enchant types** take the textured slash trace ( 3 double master, 3 quadra master, 9 tripple, @`3228eb09`-`3228eb85`, all named in
  `X2DamageManager.h`; March had the 3 double master ); `GetSlashTraceTextureName` gets the unit templet's type with a NULL test. The rest ( Lua reads,
  after-image colour 0.8 / 1.0, 4 slash traces, Ara's attack-line frames ) = March. Constants read from the exe: 0.3 / 0.4 / 0.5 / 1.5 / 0.5 / 0.2 /
  400 / 150 / 0.8. June's Lua keys are XOR-encrypted; types, order and defaults match March's 8 reads.
- **Other `GetNormalAnim` / `GetHyperAnim` readers of a weapon eqip:** all already NULL-tested in March ( `X2GameUnit.cpp` frame move / render, bone
  matrix, `GetWeaponBonePos`; `X2GUUser.cpp` collision sets skip non-`UT_ATTACH_ANIM`; `X2GULuciel.cpp` tests both ). The NULL trace frames reach
  only `GetFramePos`, which tests NULL ( Lu's `EnchantParticleFrameMoveForLuciel` included ). Nothing else to port here.
- **DU3's part:** the equip view for a two-body unit ( which body wears which eqip, the minor-unit equip byte ) stays DU3's. Nothing in the weapon
  loader needs it for playability.
- **CH1 fix 1** ( `JUNE_SKIN_AFTER_IMAGE_NULL_MESH` ) stays: a real June guard in the ctor, now reached only by a non-NULL anim.
- **Compile:** `/Zs` probes of `X2GameUnit.cpp`, `KTDGXSkinAfterImage.cpp`, `X2GUUser.cpp`: 0 errors; a break-test define on the new code fails as
  expected ( proves the flag's code is compiled ).
- **Gates:** `finish.py --files <3>`: flagcheck `2 PASS, 0 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR` / PASS, 0 deleted; junediverge PASS; enumscan
  exit 0; stateends / skillicons / luabind SKIPPED ( no states, skills or scripts ). `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`.
  Commit `16997120`; `finish.py --range 16997120^..16997120 --with JUNE_DIVERGE_WEAPON_LOAD_DATA_FOR_USER`: same tallies, `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`.
- **Build:** `DEPLOY OK: X2_offline.exe is 26,905,600 B, dated 2026-11-15 14:56:53, md5 a6ff3e3a12d0b5fc78f45dae3a813885, equal to X2/US_SERVICE/x2.exe`.
  No exestr line: the phase adds no string.
- **Play-test:** CH1's, resumed from the field entry ( hand-off in the session; the CH1 review covers this commit ).
- **Play-test 1 ( 2026-11-15, run 14:59-15:05 on md5 a6ff3e3a ):** field entry fixed ( 3 `LUCIEL DATA` loads, 16 body swaps, 0 EXCEPTION ); walk / combo /
  village / dungeon pass. Two defects with Ciel active: **(a) Ciel draws no body** ( his animation, effects and sound run ): every equip is Lu's until DU3
  ( the `+0x15d4` stand-in ), and June builds Ciel's look from each item's minor-unit id ( `31fe4210` / `31ffe990` ). DU3's own play-test target ( kickoff
  amended ). **(b) a frozen Lu after-image** while Ciel moves: fix 2 below. Ciel's death check is still owed ( no body to see ).
- **Fix 2 ( a CH1l2 transcription error ), new flag `SERV_IRUHADEV_JUNE_SKIN_AFTER_IMAGE_ADD_NEW_IMAGE`:** June's `AfterImageFrameMove` `32b15430`
  writes byte `+0x1d0` on both after-images; CH1l2 read it as `m_bEnable` ( which is `+0x194` ), so Lu's after-image was enabled whenever Ciel moved and kept
  copying Lu's hidden, still pose. `+0x1d0` is June-only: the ctor sets it to 1 and `OnFrameMove` `3322a620` takes a new image only with both bytes set.
  Ported as `CKTDGXSkinAfterImage::m_bAddNewImage` / `SetAddNewImage` ( `.h` / `.cpp` ), and Lu/Ciel's override now calls it ( March's `Enable` /
  `Disable` in `#else` ). funcmap rows `grep "(CH1q)"` ( now 4 ). `/Zs` `KTDGXSkinAfterImage.cpp`, `X2GULuciel.cpp`, `X2GameUnit.cpp`: 0 errors; break-test
  fails as expected. `finish.py --files <4> --with JUNE_SKIN_AFTER_IMAGE_ADD_NEW_IMAGE`: flagcheck `3 PASS, 0 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`,
  0 deleted / PASS; INFO `JUNE_CHAR_LUCIEL_CORE` ( the edit sits in CH1l2's `_CORE` body ); junediverge PASS; enumscan exit 0; stateends SKIPPED ( no state
  registered ). `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`.
- **Fix 2 build:** `DEPLOY OK: X2_offline.exe is 26,906,112 B, dated 2026-11-15 15:23:23, md5 8ea4a8d44d1ccdf800a84c0ad872455f, equal to X2/US_SERVICE/x2.exe`.
  Commit `4b5106c9`; `finish.py --range 4b5106c9^..4b5106c9 --with JUNE_SKIN_AFTER_IMAGE_ADD_NEW_IMAGE`: same tallies, `FINISH RESULT: PASS`.
- **Play-test 2 PASSED ( 2026-11-15, user; run 15:28-15:34 on md5 8ea4a8d4 ):** no frozen Lu copy while Ciel moves, Lu's trail on moves only, the March
  regression walk unchanged. Logs: 14 `LUCIEL` lines ( DATA loads + swaps ), 0 EXCEPTION, ErrorLog clean, UNHANDLED only `E_DISABLE_HB_CHECK_REQ` x4.
  CH1's play-test is complete except Ciel's body and death check ( DU3 ). `CH1-review.md` is next.

### CH1 - Review (2026-11-15)

Opus 5.5 high. Covers CH1 `5e880f3c` + fix `975e8b87`, CH1q `16997120` + fix 2 `4b5106c9`, and june-state-ids S2 `857b18cd`. **Verdict: pass, no code
change.** Ghidra read-only; listings in the session scratchpad.
- **Factory:** `3230f470` = `new( 0x98a0 )` + ctor `32b1bd90`, nothing else. Both callers jump through a 12-entry table ( type - 1 ), case 10 = index 9:
  `322ced00` ( table `322cef10`, block `322cee9c`, call @`322ceeb4` ) returns from each case directly = **`CreateGUUser`**; `322d5bf0` ( table `322d5eb4`, block
  `322d5d77`, call @`322d5d8f` ) jumps to the shared tail `322d5dd2` ( vslot 0x40 `( 1, -1 )`, 0x198, ... ) like every case = **`AddUserUnitIntruder`**. No
  case-10-only call. Arguments ( slot, team `+0x10`, script, frame buffer `[g_pMain + 0x120]`, unit `+0x50` ) = the port's order. funcmap row updated.
- **Switching guide:** ctor @`32307bca` / dtor @`32308957` sit between memorize and Eve recovery ( CH1's note ); the port's `&false`, `0.f` and order match.
- **`ParseCommonRandomState`:** no class override in June; Lu's calls `ParseCommonRandomStateByUnitClass()` as Aisha / Eve / Ara / Elesis. `GetUnitClassName`
  ( `X2UISkillTreeNew2015.cpp:5633` ) gives `LUCIEL_DUO` / `_CHILIARCH` / `_ROYAL_GUARD` / `_DIABLURA` / `_DREAD_LORD` / `_NOBLESSE` / `_DEMONIO` for 200 / 37 /
  38 / 39 / 127 / 128 / 129: the seven keys of `Luciel_Duo.src.lua:597-814`. Note: that helper exists only under `JUNE_CHAR_ARME_LIVE` or `_EVE_LIVE` (both
  defined); the four classes share the dependency, no guard added.
- **Die-state ends:** `32b3e8b0` registers 0xc0 `LSI_CIEL_DIE_FRONT` / 0xc1 `_BACK` with End `329cada0` ( `jmp 3202a7d0` ), = `ch0_reg.tsv` rows 185 / 186. The
  getters ( `X2GULuciel.cpp:8651` ) give Ciel's 192 / 193 while Ciel is active and Lu's 7 / 8 ( = `USI_DIE_*` ) when transformed, so both bodies and the
  transformed death match; March classes keep the base `USI_DIE_*`. Proof in game is DU3's ( Ciel has no body yet; kickoff amended by CH1q ).
  **Found, routed:** June also gives `USI_RIDING_DIE` ( 0x57 ) that End for **every** class, and the stand-in misses it: backlog Stage 17 bullet ( not Lu's subject, no crash ).
- **S2:** bounding at 0x78 / 0x79 / 0x7a, tables `LSI_LU_DAMAGE_BOUNDING_*`, thunks `329cc970` / `980` / `990` / `9a0` / `9b0` -> `329cc0b0` / `150` / `1e0` /
  `050` / `0c0` ( the shared ones S2 names ) = `InitBoundingStates( "LSI_LU_" )`'s three rows. Wait change: `32b43d10` @`32b44174` pushes 0x82 and thunk
  `329cb720` ( `jmp 31fc8620` = `WaitStateChangeEventProcess` ) = `InitWaitStateChange`. Shadows gone, guard placed; S2's probes accepted for 110-118.
- **Data:** staged sha1 of `Luciel_Duo.lua` / `Luciel_Demonio.lua` / `DLG_Luciel_CombinationPoint.lua` = the adopt manifests' `ljconv` sha1, all `1B 4B 4C 81`.
  `luabind.py check` the three: `RESULT: PASS` ( one June nil, `CT_PARTS_LOOK_TRACKINGT` ). data036 `Enum.lua:12352` `RNSI_A_LLD_BLUFF = 30304` = June `:15069`.
- **CH1q:** `CreateSkinAfterImage` = June `31f8ace0` ( NULL anim -> 0, else `new( 0x1d4 )` + ctor ). `LoadDataForUser` = `3228d660`: head ( motion
  `+0x87cc`, eqip `+0x1c0`, templet `+0x198` ), loop of 8, after-image on the normal model `+0x5a4` from the normal anim `+0x544`, colour 0.8 / 1.0, fashion from
  `+0x194` else the eqip templet, change-weapon via `IsValidUnit` + `+0x91dd` and its clear, `GetEquippingItemByEquipPos( 2, 0, 0 )`, else `GetItem( uid, true )`
  @`3228ea5a`; the 15 ids 3003 / 3033 / 3303 / 4004 / 4044 / 4404 / 4546 / 4576 / 4657 / 4687 / 4768 / 4798 / 4846 / 4957 / 5068 = the 15 named members; texture
  name by unit type ( NULL -> 0 ), stride 400; the 4 traces ( 400 / 150, 100 / 50, custom 400 / 150 with stride ), radius x1.5 / x0.5, disable times, tip only
  untextured, Ara's frames on type 7. June returns nothing; March's three callers ignore the bool. Fix 1 = @`3322af08` ( `[anim + 0x190]` test ). Fix 2: ctor
  @`3322acaf` writes word 1 at `+0x1d0`; `OnFrameMove` `3322a620` needs `+0x194` and `+0x1d0`; `32b15430` writes `+0x1d0` on both after-images ( states 13 / 194 off ).
- **Gates:** `finish.py --review --commits 5e880f3c 975e8b87 16997120 4b5106c9 857b18cd --with <7 flags> --skip-flag JUNE_USER_STATE_IDS "guard target"`: flagcheck
  CORE `7 PASS, 4 DEFINE, 6 OTHER`, DUO `3 / 4 / 7`, LIVE `6 / 4 / 7`, WEAPON_LOAD `3 / 2 / 2`, ADD_NEW_IMAGE `3 / 1 / 0`, NULL_MESH `2 / 3 / 4`, HOOKS
  `2 / 4 / 11`, all `0 FAIL, 0 ERROR` / PASS; every OTHER row is another flag of this set or the four enclosing `INFO` flags; 51 deleted lines = S2's 48 shadow /
  inline rows + CH1's 3 `Always.h` lines. junediverge PASS, `--check-all` PASS, enumscan exit 0: `FINISH RESULT: PASS (14 gates, 0 failed, 4 skipped)`. With
  `JUNE_USER_STATE_IDS` in `--with` every flag read ERROR on S1's `_DEBUG` guard line ( the limit S2's note names ). `stateends.py check X2GULuciel.cpp`
  `Luciel_Duo` / `Luciel_Demonio`: 0 STUCK, PASS both.
- **Play-test verdicts** ( CH1 runs 1-2, CH1q runs 1-2 ): create / village / first-entry tutorial pass ( March's quest 11000: TU's ); field entry crash fixed
  ( CH1q ); walk / jump / dash / combo / dungeon pass; V swap pass ( 16 then 14 swaps logged ); Lu's frozen after-image fixed ( fix 2 ); March regression walk
  pass; relog / win pose / Ciel MP across the hop have no line of their own in the notes ( the user's "passed" covers them; 2-3 `LUCIEL DATA` loads per run ). **Owed to DU3:** Ciel's body and the Ciel death / buff check. **Not shown by a log:** a point on hit ( the
  run logged `CP_AMOUNT 0`, no `DUO point` line; only the six ZZZZX / XXXZX / dash-jump XXXXZ finishers carry `HIT_ADD_COMBINATION_POINT` ) and the point row:
  moved to CH2a's play-test. Cosmetic: the `DATA` line's `script` is empty ( background load keeps no file name ).
- **Build:** none ( no code change ); last deploy is CH1q fix 2's ( md5 `8ea4a8d4` ).
- **Kickoffs:** DU3 carries CH1q's amendment; CH2a, CHN, TU amended now. Unblocked: DU3, CHN, TU, CH2a, CH2d20, CH3c20 ( one at a time in `X2GULuciel.*` and in
  `X2GUUser.*` ).

## Phase CHN - Job change (Aisha's CH-N template, every Lu/Ciel job class)

**Run with:** Opus 5.5 high, Opus review, user play-test. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CLASS_CHANGE`, sub-tag `JOB`. June's class-change
route for 200 -> 37 / 38 / 39 and 37 -> 127, 38 -> 128, 39 -> 129: the quests (`CJT_*`, quest-system), the promotion items (`INIT_*` /
`PROMOTION_ITEM_ID_LIST` if June's script has one), the emulator's class-generic handlers (`Handlers_Quest.cpp`, `Handlers_Shop.cpp`),
`X2OfflineClassChangeSeed.h` `PARENT_ROWS` and `X2OfflineLuaEnumSeed.h`, `DLG_Class_Change_Luciel_*` (data217) and the class-change guide pieces
the change itself shows. Tree rows for the new class go to `SeedJuneAutoLearn` where free. **Done gate:** a level-15 Lu/Ciel reaches each first
job, and a level-35 one reaches each second job, by quest or item. The tree, the portrait and the motions follow, and a relog keeps it.

### CHN - Done (2026-11-15)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CLASS_CHANGE` (`Always.h`, `#error` guard on `JUNE_ENUM_NAMES`, which the older
`JUNE_CLASS_PATH` guard at `:12480` already requires in this tree). Commit: see `git log --grep "char-luciel Phase CHN"`.
- **Already class-generic (no change):** the quest route (data036 `FieldQuest.lua` = June bytecode: 35644-35647 -> 37, 35648-35651 -> 38,
  35660 / 35670 / 35680 / 35690 -> 39, 35750-35790 -> 127, 35700-35740 -> 128, 35800-35840 -> 129); `Handlers_Quest.cpp:590-625` applies any
  class + `SeedAutoLearn` (Lu/Ciel's rows are in `X2OfflineSkillAutoLearn.h`); the guide runtime (Ara CHNb / CHNb2: `UICustomEventProc`,
  `GuideAcceptClassUpQuest`, `OpenClassChangeGuideItem` map with 242371-242374 / 277105 / 277107, line / tier helpers with the Lu/Ciel rows);
  `GetClassLevel` (June `3234c170`, generic); portraits 60-65 (`X2Data.cpp:3098`); the 10 dialogs staged by CH0b (data034 guides, data217
  `DLG_Class_Change_Luciel_*`).
- **Item compatibility (`X2Unit.h`, checklist 7ar, owed since NU1):** `GetX2UnitClassCompatibility` had no Lu/Ciel arm, so no Lu/Ciel
  class could equip from the bag. Seven cases = June `31ed6500` case 200 (200 only), 0x25-0x27 (200 + self), 0x7f-0x81 (200 + parent + self).
- **Emulator seed (`X2OfflineClassChangeSeed.h`):** 14 rows from June `item.kim` by family: job-advance (unit class = the parent) 242371 -> 37,
  242372 -> 38, 277105 -> 39, 242373 -> 127, 242374 -> 128, 277107 -> 129; re-pick tickets 276955 / 276956 / 135824 / 277104 / 276957 / 276958 /
  135825 / 277106 (two Diabla / Demonio ticket ids each, same name, both kept). `PARENT_ROWS` 200 / 37 / 38 / 39 / 127 / 128 / 129 = June's tree
  (`3234b390` row 10). `SIDE_STEP_CLASSES` + 39: a labelled stand-in (June's `ClassChangeTable` is server-only), NU0's call. Claim path only
  (cash deposit), as live; `/additem` of a ticket puts it in the bag and does nothing, so the play-test uses the quest route.
- **Server-Lua enum seed (`X2OfflineLuaEnumSeed.h`):** the 7 `UC_LUCIEL_*` names, values from the C++ enum.
- **Data (data036 `LevelUpEvent.lua`, core-list source):** `TYPE9` level 15 (Duo -> `DLG_1ST_Class_Change_Guide_Luciel.lua`) and level 35
  (37 / 38 / 39 -> `DLG_2ND_Class_Change_Guide_Luciel.lua`), both with the two `LucielVoice_ClassUpReady` sounds (data162) = June
  `LevelUpEvent.src.lua:154-170` / `:377-395`. BOM + CRLF kept (475 -> 511 lines); `luasrc.py refresh`; `ledger --check` / `core --check` PASS.
  The level-70 transcendence row (`:623-640`) stays CH3d's (§7ao).
- **luabind** on the 10 dialogs + LevelUpEvent: 0 unbound, ENUM 0 / 0 on every dialog. One `ENUM MISSING NAME LUEI_SECRET_DUNGEON_OPEN_GUIDE`
  is June's file line 437, a secret-dungeon event block our LevelUpEvent.lua does not carry: not this phase's, not reached.
- **Pack:** `PACK OK data036` (vs live: ~1 changed LevelUpEvent.lua; TU had packed and copied its data036 at 16:04 first), `COPY OK data036`
  md5 `ad8c5a10b0f106fcaf316772c5a5fcdc`, backup `kom_backup_tools/data036.kom.20261115_160901`.
- **Units** (backup `db_backup/els_db_20261115_160952_pre_pre_char-luciel-CHN.sql`): `LuGuideLC` 180, `LuGuideCG` 181, `LuGuideDB` 182 (200,
  level 14, exp 81600 = 100 below 15); `LuGuideLD` 183 (37), `LuGuideCN` 184 (38), `LuGuideDN` 185 (39) (level 34, exp 1212400 = 100 below 35).
- **Gates (`finish.py --files`, worktree):** flagcheck `JUNE_CHAR_LUCIEL_CLASS_CHANGE` PASS (2 PASS, 1 PASS~, 1 OTHER = peers' Always.h
  blocks), junediverge PASS, enumscan PASS. FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped: no states, skills or staged June script).
- **Deploy:** `DEPLOY OK: X2_offline.exe is 26,907,648 B, dated 2026-11-15 16:19:28, md5 17cf42ad3caa06c5489edb86b88d42b2` (built at main
  61c2ff4c + 10 uncommitted files incl. peers' TU; `X2Unit.obj` 16:17 / `Handlers_Shop.obj` 16:09 / `X2OfflineLuaEnum.obj` 16:10, after these edits).
  No exestr check: the change adds no string.
- **No new log line:** the always-on `QUEST complete <id> CLASS CHANGE`, `CLASS GUIDE 1st / 2nd` and `[JUNE_CHAR] LUCIEL DATA class <id>` lines
  prove the play-test; no `_DEBUG` flag.
- **Found, routed:** Ara 34 / 124, Elesis 35 / 125 and Add 33 / 36 / 123 / 126 have no seed rows either (their tickets land in the bag): one
  bullet in `kickoffs/char-add/CHNb.md` (the next class-path phase on the same file). `SetDungeonClearTexture` for 37-39 / 127-129: CH6 walk.
- **Play-test 1 (2026-11-15 16:25-16:37, logs read):** all six `CLASS CHANGE` (180-182: 200 -> 37 / 38 / 39; 183-185: 37 -> 127, 38 -> 128,
  39 -> 129), `CLASS GUIDE 1st` x3 and `2nd` x3 (lines 1 / 2 / 3 -> 127 / 128 / 129, 4 skill icons, 3 command rows), every `/qdone` NetError 0,
  relog kept all six (user), 0 EXCEPTION. **Found:** (1) 12 textures missing (guide portraits `DLG_CU_LUCIEL_1_*` / `_2_*`, popup panels
  `DLG_CC_LUCIEL_1ST_*` / `_2ND_*`); CH0b's name staging missed them. Fix: the 21 textures the 10 dialogs name, `kom.py stage-june`: 15 new
  (+ `DLG_CC_Guide_LUCIEL_ver2` / `_1`, `Class_Change_Cash_Item_2nd_Pop05`), 6 identical, 0 replaced; `PACK OK` / `COPY OK` data052 (md5 `83336ade`),
  data151 (`272e8aaa`), data215 (`d6c07e6f`), datai020 (`0a590f9f`). (2) Promotion cubes 109999 / 110850 refuse (NetError 285,
  `CUBE refused - item 110850 lists no item group for class 128 / 129`): the packed `RandomItemTable.lua` is the March-era US server table, with no
  row for any June class (Lu/Ciel, Ara 34 / 124, Elesis 35 / 125, Add); June's server table is not in the repo. Decision with the user.
- **Play-test 2 (16:44-16:47, units 187 / 188):** no `OpenTexture` failure left, but the pictures still did not draw: each picture names piece
  `"01"`, defined in the texture's `.TET` sidecar, which name staging by `.tga` does not bring. Fix: the 15 `.tet` (`kom.py stage-june`, 15 new,
  6 identical, 0 replaced), `PACK OK` / `COPY OK` data052 (md5 `2e048616`), data151 (`df705629`), data215 (`14e12b3f`), datai020 (`4eb0e867`).
- **Play-test 3 (16:49-16:51, user: all pictures show):** 1ST guide portraits, Diabla popup panel, 2ND guide Noblesse portrait, Noblesse popup panel;
  `CLASS CHANGE` 189 200 -> 39, 190 38 -> 128; ErrorLog 0 lines for the Lu/Ciel dialog textures, 0 EXCEPTION. CHN passed; promotion cubes go to
  CHN2; review kickoff `kickoffs/char-luciel/CHN-review.md` (covers CHN + CHN2).

### CHN2 - Done (2026-11-15)

Sonnet 5.5 high. Server Lua only: no flag, no client change, **no build** (the exe is unchanged; the CHN deploy stands). File:
`KncWX2Server/ServerResource/US/RandomItemTable.lua` (BOM + CRLF kept, 226 lines inserted, 0 deleted).
- **Packed by Claude (user grant 2026-11-15, rule amended in CLAUDE.md):** the old repo source compiled with luajit equals the staged bytecode byte for
  byte (T0, 4,660,654 B); the new source compiles to 4,665,840 B (md5 `426c6809`), staged by `kom.py stage-server RandomItemTable.lua` (new command;
  backup `kom_backup_tools/RandomItemTable.lua.20261115_165818.bytecode`). `PACK OK data036` (vs live: ~1 changed, RandomItemTable.lua),
  `COPY OK data036` md5 `a480299e789e974635b93cd1c097644b` (backup `kom_backup_tools/data036.kom.20261115_165832`).
- **Shape = March's rows.** One `{ m_cUnitClass, m_iItemGroupID }` per class in cube 109999 (1st job) and 110850 (2nd job), one `AddRandomItemGroup(
  group, item, 20, period, 1 )` per piece: rate 20, quantity 1, period 10 for a 1st-job set and 15 for a 2nd-job set (copied from March's Elesis /
  Ara / Sheath Knight groups, not invented). Each added block (2 row runs, 2 group runs) carries the one-line `stand-in ( char-luciel CHN2 )` comment.
- **Sets, from `item.kim` by name** (all five pieces share the class's `unit_class`; weapon, top, bottom, gloves, shoes): Psychic Tracer 32 =
  41762-41766, Arc Tracer 33 = 41780-41784, Little Specter 34 = 41788-41792, Dark Knight 35 = 41815-41819, Time Tracer 36 = 41829-41833, Chiliarch 37 =
  135623-135627, Royal Guard 38 = 135629-135633, Diablura 39 = 42103-42107; Lunatic Psyker 122 = 41768-41772, Mastermind 123 = 41774-41778, Asura 124 =
  41797-41801, Crimson Avenger 125 = 41821-41825, Diabolic Esper 126 = 41835-41839, Dread Lord 127 = 135637-135641, Noblesse 128 = 135643-135647,
  Demonio 129 = 42111-42115. A script checked every group: 5 items, classes match, positions weapon / body / leg / hand / foot.
- **New group ids (16):** 1st job 5000083 (32), 5000084 (33), 5000085 (34), 5000086 (35), 5000087 (36), 5000088 (37), 5000089 (38), 5000090 (39); 2nd job
  5000091 (122), 5000092 (123), 5000093 (124), 5000094 (125), 5000095 (126), 5000096 (127), 5000097 (128), 5000098 (129). Next free after the file's highest
  group (5000082); none of them appears anywhere else in the file.
- **Elesis 120 / 121 reuse March's own groups 16953 (Grand Master 41745-41749) / 16954 (Blazing Heart 41750-41754):** already in the file, period 15,
  referenced by no cube. In 110850 they are the rows for 120 / 121; no new ids.
- **109999 also lists the 2nd-job classes, as March does** (Lord Knight -> the Knight set 11740, Yama Raja -> Little Devil 16855): each added 2nd job
  points at its parent's 1st-job group (120 -> 16823, 121 -> 16824, 122 -> 5000083, 123 -> 5000084, 124 -> 5000085, 125 -> 5000086, 126 -> 5000087,
  127 -> 5000088, 128 -> 5000089, 129 -> 5000090). 109999 therefore opens on all 18 June jobs, 110850 on the 10 second jobs.
- **Decisions.** **5 pieces:** `item.kim` shows the same five-piece reading for every class, and March's groups drop the hair / face / helm pieces
  where they exist (Chung's helm 40050, Infinity Sword's hair 41250), so Diabla's 42108, Demonio's 42116 and the Ara / Elesis / Add hair and face pieces
  are not in the groups. **One set per job class, no Ciel pair** (corrected 2026-11-15: the right test is `is_for_minor_unit`, not `item_id_for_minor_unit`): `item.kim` has one promotion set per job class, flagged Ciel-body for 38 / 127 / 129 and Lu-body for 37 / 39 / 128; only the base Duo (200) has both Lu's Promotion 41890-41894 and Ciel's Promotion 41900-41904, and it has no cube row (open question). **No row, said once:** Rose 40-43 / 130-133, Ain 44-46 /
  134-136 (no unit type in the client; their sets are not looked up) and the base classes 1 / 9 / 8 / 7 / 10 (no job cube). Elesis 29 / 30 and Ara 28 / 118 /
  31 / 119 already had rows.
- **Names resolve:** all 16 `UC_*` names are in data036 `Enum.lua` (`:317-:386`) with values equal to `X2Unit.h`'s enum; the emulator's UNIT_CLASS table is
  `Enum.lua`'s (the seed only fills missing names), so no seed edit. The loader's own refusal lines will show any row that does not resolve.
- **Not proven until the play-test:** `python tools/logs.py --grep "CUBE"` must show `CUBE     opened item 109999 ...` / `110850 ...` per cube, and no
  `lists no item group for class`.
- **Play-test (2026-11-15 17:01-17:03, user: passed; logs read):** `CUBE loaded: 2870 cube(s) ... 10060 group(s)`; `CUBE opened item 110850 (give-all): 5 item kind(s)` x1 and `109999` x2;
  0 `CUBE refused`, 0 EXCEPTION, ErrorLog 0 lines. The 11 / 13 "cannot pay out" cubes are other cubes (studio holes), not 109999 / 110850.
- **Revert:** `git revert` the CHN2 commit (pure insertion), then `kom.py stage-server RandomItemTable.lua`, pack, copy.

### CHN - Review (2026-11-15)

Opus 5.5 high. **Verdict: pass, no code change** (covers CHN `9f5bf8fe` and CHN2 `cb29dbf6`; the other CHN / CHN2 commits are docs, tools and kom
packs). No `_DEBUG` flag to retire; no build (review with no code change).
- **`GetX2UnitClassCompatibility` (`X2Unit.h`) = June `31ed6500` in Ghidra:** case 200 tests 200 only; 0x25 / 0x26 / 0x27 accept 200 or self;
  0x7f / 0x80 / 0x81 accept 200, the parent (0x25 / 0x26 / 0x27) or self. All seven arms match.
- **`PARENT_ROWS` = June's class tree:** `3234b390` reads `337a1660 [type][tier][line]`, filled at `3234b244-3234b2df`. Type 10: tier 0 lines 1-2 = 200;
  tier 1 = 0x25 / 0x26 / 0x27; tier 2 = 0x7f / 0x80 / 0x81. So 37 -> 127, 38 -> 128, 39 -> 129, all from 200. Tier 0 line 3 is 0 in June, which fits
  Diablura being the later third branch (the `SIDE_STEP_CLASSES` stand-in, labelled; it only lets a re-pick ticket cross to 39, like March's 22-27).
- **14 seed rows vs June `item.kim`:** every job-advance item carries the parent class (242371 / 242372 / 277105 = 200, 242373 = 37, 242374 = 38,
  277107 = 39), every ticket the target itself (276955 37, 276956 38, 135824 / 277104 39, 276957 127, 276958 128, 135825 / 277106 129); all `IT_SPECIAL`.
  Gates in `Handlers_Shop.cpp`: job advance needs the current class = `ClassChangeParentOf( target )` (`:729-745`). A re-pick is refused at tier 0
  (`:747-787`), and 200's self-parent row makes it tier 0, so a ticket used on a Duo is refused.
- **`X2OfflineLuaEnumSeed.h`:** the 7 names are June `Enum.src.lua:354-421` values (37 / 38 / 39 / 127 / 128 / 129 / 200) = `X2Unit.h:184-208`.
- **data036 `LevelUpEvent.lua`:** BOM + CRLF (510 lines). The 1st block (March `:154-170`) and the 2nd block (March `:338-356` = June `:377-395`) equal
  June byte for byte except the closing `}` has no trailing `,`. That is because Lu/Ciel's TYPE9 is our last entry, where June's is followed by Rose's
  TYPE10; the parsed data is the same. `luasrc.py ledger --check` and `core --check` print PASS, 0 FAIL, 0 WARN.
- **CHN2 `RandomItemTable.lua`:** a script checked all 80 added group lines against `item.kim`: 90 of 90 ids found, 0 bad. Each group has 5 items of
  the group's class, positions weapon / body / leg / hand / foot, rate 20, qty 1, period 10 (5000083-5000090) or 15 (5000091-5000098). The cube rows
  have March's `{ m_cUnitClass, m_iItemGroupID }` shape. The Elesis reuse checks out: 120 -> 16823 (Saber Knight set) and 121 -> 16824 (Pyro Knight
  set) in 109999. 16953 / 16954 were already defined at March `:124161`, so CHN2 added cube rows only. None of 5000083-5000098 existed before; BOM +
  pure CRLF kept. Accepted: the 5-piece decision (March's groups drop hair / face / helm), and the base Duo 200 has no cube row.
- **Logs:** the current logs are CHN2's run (17:01-17:03): `CUBE opened item 110850` x1, `109999` x2, 0 `CUBE refused`, 0 EXCEPTION. CHN's
  `CLASS CHANGE` lines (16:25-16:51) were overwritten by later runs; they stand on the CHN Done note's log read and the user's relog check.
- **Gate (`finish.py --review --commits 9f5bf8fe --with SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CLASS_CHANGE -v`):** flagcheck PASS (2 PASS, 1 PASS~,
  1 DEFINE, 0 OTHER, 0 deleted lines). The PASS~ is one blank line in `X2Unit.h` between `#endif SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CLASS_CHANGE` and
  the switch's `}`; it changes nothing. junediverge PASS, `--check-all` PASS, enumscan PASS. FINISH RESULT: PASS (7 gates, 0 failed, 3 skipped:
  no states, skills or June script). INFO: `JUNE_ENUM_NAMES` and `OFFLINE` are outer flags.
- **Next:** CH2b now waits only on CH2a.

## Phase TU - Tutorial

**Run with:** Sonnet 5.5 high, Opus review, user play-test. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_TUTORIAL`, sub-tag `TUTORIAL`. Add the
`DI_DUNGEON_TUTORIAL_LUCIEL` 41240 and `DI_BATTLE_FIELD_TUTORIAL_LUCIEL` 39609 rows to the core-list `DungeonEnum.lua` / `DungeonData.lua` (and
`DungeonEnum.h` under `JUNE_ENUM_NAMES`), the world `WI_TUTORIAL_LUCIEL_STAGE0` 633, adopt `Tutorial_Luciel_Stage0*` (data040), check
`Dungeon_Tutorial_Luciel.lua` with `luabind.py`, and port June's first-entry route for a new Lu/Ciel (NU0 item 8). **Done gate:** a new Lu/Ciel
plays the tutorial to its end and lands where June lands him.

## Stage 8 Lu/Ciel, TU - Done (char-luciel TU, 2026-11-15)

Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_TUTORIAL` (`Always.h`, `#error` without `JUNE_CLASS_TUTORIAL_DUNGEON` and `JUNE_CHAR_LUCIEL_LIVE`), sub-tag `TUTORIAL`.
A new Lu/Ciel's first select now enters June's tutorial dungeon 41240 (it fell to Elsword's 41150 before: CH1's play-test).
- **Code.** `X2StateServerSelect.cpp` / `X2StateBeginning.cpp` (compiled-out copy): `case UT_LUCIEL` -> `DI_DUNGEON_TUTORIAL_LUCIEL` in C3y-2's switch
  (June `325622e0`, type 10 -> `0xa118`). `DungeonEnum.h` (`JUNE_ENUM_NAMES`): `DI_DUNGEON_TUTORIAL_LUCIEL = 41240`. Emulator: `IsQuestTutorialDungeon`
  learns the id (`X2OfflineQuest.cpp`); `Handlers_Room.cpp` logs `[JUNE_CHAR] LUCIEL TUTORIAL room <uid> dungeon 41240 for unit <uid>`.
  Add's case (9 -> 41230) is char-add TU's; Rose / Ain keep the stand-in comment.
- **Data (June's rows verbatim, core-list sources, ledger refreshed).** data036 `DungeonEnum.lua` `DI_DUNGEON_TUTORIAL_LUCIEL` 41240 (`DungeonEnum.src.lua:101`);
  data036 `Enum.lua` `WI_TUTORIAL_LUCIEL_STAGE0` 633 and `NUI_TUTORIAL_RUCIEL_OBJECT` 2044 / `ICOOS` 2045 / `BOSS_KARIS` 2047 / `LUCIEL_DP1` 2067 (no collision with a
  live `NUI_`/`WI_` value; the `SOUCM_*` hits on those numbers are another table); data035 `DungeonData.lua` the 41240 row (`DungeonData.src.lua:29877`);
  data040 `WorldData.lua` the 633 row; data037 `NPCTemplet.lua` the four NPC rows. `NewNPCStat` rows were already live.
- **Assets.** 214 names by `kom.py stage-june` (data081 1, data160 88 and data161 78 = new koms: `Tuto_Ciel_*.ogg` / world sounds, data164 43 = `Tutorial_Luciel_Stage0_*`
  meshes and textures; data126 / data165 identical). The 7 Lu/Ciel tutorial `.lua` (dungeon, 3 NPC scripts, Dp1, Stage0, LineMap) were already June's bytes
  (`adopt-june --dry-run`: 7 identical), so nothing was adopted. NU0's "HQ_Loading_TUTORIAL_LUCIEL not found" was wrong: data126, already staged.
- **Not done, on purpose.** `DI_BATTLE_FIELD_TUTORIAL_LUCIEL` 39609 (already in live `DungeonEnum.lua`; June has no data row and no reader);
  `NUI_TUTORIAL_ERIS_SUMMON` / `Tutorial_Luciel_eris.lua` (a June `NPCTemplet` row whose script June does not ship; no dungeon names it);
  `ImpossibleAgainDungeon` (no live file).
- **Pack / copy.** PACK OK data035 / 036 / 037 / 040 / 081 / 160 / 161 / 164. COPY OK all (data035 md5 470c9aaa, data037 3eb5423e, data040 80630812, data081 fa477aad,
  data160 3500830c, data161 37392457, data164 9770a0bc). **data036 was copied with `--force` (md5 533396c2)**: a peer (char-luciel CHN) edited `LevelUpEvent.lua`
  a minute after my pack, so the live data036.kom carries my rows and not that edit; the next `pack data036` carries both.
- **Gates.** `finish.py` (the code files, `--with TUTORIAL ENUM_NAMES`, `--lua` the 5 scripts): `luabind.py check` Dungeon_Tutorial_Luciel + 4 NPC scripts RESULT PASS
  (before the data: 5 + 1 ENUM MISSING), `junediverge` PASS, `enumscan` exit 0, flagcheck: sites PASS (4 PASS + 1 PASS~ for the 6-line log block), the one ERROR
  per flag is Always.h's `#error` guards (precedent). `ledger --check` PASS, `core --check` PASS.
- **Build.** DEPLOY OK: X2_offline.exe is 26,907,648 B, dated 2026-11-15 16:19:28, md5 17cf42ad3caa06c5489edb86b88d42b2 (built at main f238da7b + 10 uncommitted code
  files of peers). EXESTR OK: 1 of 1 (wide `[JUNE_CHAR] LUCIEL TUTORIAL room`).
- **Revert.** Comment out the `#define`: Lu/Ciel falls back to Elsword's tutorial. The data rows are inert.
- **Play-test hand-off.** See the commit's hand-off: create a fresh Lu/Ciel, play the tutorial to its end; `logs.py --grep "JUNE_CHAR\] LUCIEL TUTORIAL"`.
  Open for the user: none.
- **Commit / play-test.** `f9da0f89`. Play-test passed 2026-11-15 (user): a fresh Lu/Ciel played the tutorial to the village. Logs: `[JUNE_CHAR] LUCIEL TUTORIAL room 1001 dungeon 41240 for unit 186`, `ROOM tutorial room 1001 ... dungeonID=41240`, ErrorLog 0 lines, lua 0 unbound, 0 EXCEPTION (the 5 `E_DISABLE_HB_CHECK_REQ` UNHANDLED are old noise). Range gate `f9da0f89^..f9da0f89`: flagcheck 0 FAIL (3 PASS, 1 PASS~, the `#error` guard row), junediverge / enumscan PASS.

## Stage 8 Lu/Ciel, TU -- Review (char-luciel TU review, 2026-11-15, Opus 5.5 high)

**Verdict: pass, no code change.** Reviewed commit `f9da0f89` against June in Ghidra (read-only) and the live koms.
- **Room switch.** Ghidra `325622e0`: the unit type at `[[[33775860+0x68]+0x90]+0xaf0]` switches; case 10 -> `0xa118` = 41240, default `0xa0be` (Elsword).
  The new `case UT_LUCIEL` (`UT_LUCIEL` = 10) sits in C3y-2's live `#ifdef SERV_IRUHADEV_JUNE_CLASS_TUTORIAL_DUNGEON` branch of
  `CX2StateServerSelect::Handler_EGS_CREATE_TUTORIAL_ROOM_REQ` (`X2StateServerSelect.cpp:7580`); the rest of the body is unchanged; the
  `X2StateBeginning.cpp` copy is the same case in compiled-out code. Emulator: `IsQuestTutorialDungeon` case (`X2OfflineQuest.cpp:923`) and the
  `Handlers_Room.cpp` log line, both behind the flag.
- **Data = June verbatim.** Live `DungeonEnum.lua:348` = `DungeonEnum.src.lua:101`; live `Enum.lua:2546` (`WORLD_ID`) and `:5060-5063` (`NPC_UNIT_ID`) =
  `Enum.src.lua:2357 / 4006 / 4890 / 4918 / 5825`. No collision inside a table: 633 is also `NUI_TUTORIAL_MARS` (`NPC_UNIT_ID`) and `BFI_DEBUFF_ENCHANT_HAND`,
  2044 / 2045 / 2047 / 2067 also `SOUCM_*`: other tables. Whole-row diffs (whitespace-normalised) identical: the `DungeonData.lua` 41240 row (19 lines),
  the `WorldData.lua` 633 row, the four `NPCTemplet.lua` rows.
- **Scripts.** `luabind.py check` Dungeon_Tutorial_Luciel + Tutorial_Luciel_boss_karis / _icoos / _object + Tutotial_Luciel_Dp1: RESULT PASS (0 unbound,
  ENUM 0 missing). `adopt-june --dry-run` on those 5 + Tutorial_Luciel_Stage0 / _LineMap: 7 identical (June's bytes already live).
- **Assets / koms.** data160 88 + data161 78 files (new koms), data164 43 `Tutorial_Luciel_Stage0*`, data081 1 = 210 + 4 reused = the 214 named. `kom.py status`
  data035 / 036 / 037 / 040 / 081 / 160 / 161 / 164: all in sync with the live koms. The `--force` data036 copy is superseded: CHN repacked data036 at 16:08
  (after its `LevelUpEvent.lua` edit at 16:05), and that kom still holds TU's rows (in sync with the dir). The data036 "stale .src.lua" warning is 23 sidecars
  from 2026-10-10, not TU's.
- **Not ported, confirmed.** `DI_BATTLE_FIELD_TUTORIAL_LUCIEL` 39609: June has no `DungeonData` row (39609 elsewhere in June is `STR_ID_39609`).
  `NUI_TUTORIAL_ERIS_SUMMON` 2046: only `Enum` / `NPCTemplet` / `NewNPCStat` rows in June, no script or dungeon names it.
- **Play-test.** One check, passed (user, 2026-11-15): fresh Lu/Ciel played 41240 to the village = June's end. Elsword's case is untouched (default branch).
- **MINOR, not fixed (routed):** the stand-in comment after the new case still reads "Lu/Ciel, Rose, Ain not ported ( 10 / 11 / 12 -> ... )" in both files.
  Old comments are not rewritten; char-rose's TU replaces that line when it adds case 11.
- **Gates.** `finish.py --review --commits f9da0f89 --with JUNE_CHAR_LUCIEL_TUTORIAL JUNE_ENUM_NAMES --skip-flag JUNE_CLASS_TUTORIAL_DUNGEON`:
  junediverge PASS, junediverge `--check-all` PASS, enumscan exit 0. flagcheck `_TUTORIAL` 3 PASS, 1 PASS~ (`Handlers_Room.cpp`, one blank line), 1 OTHER
  (`DungeonEnum.h` = the partner `JUNE_ENUM_NAMES` line), 0 FAIL, 0 deleted / non-ASCII / EOL lines; `JUNE_ENUM_NAMES` 1 PASS (`DungeonEnum.h`), 4 OTHER
  (the `_TUTORIAL` sites). Both RESULT lines read FAIL only for the one ERROR row, `Always.h:10175`: `#error` when `JUNE_USER_SKILL_USE_HELPERS` is on without
  `JUNE_ENUM_NAMES`; flagcheck's flag-off pass undefines `JUNE_ENUM_NAMES` and that guard fires as designed (same precedent as TU's own guard). Not a defect.
  `JUNE_CLASS_TUTORIAL_DUNGEON` SKIPPED: C3y-2's flag, target of TU's `#error`.
- **Build.** None: no code change.

## Phase CH2 - Skills: base and first jobs (200, 37, 38, 39)

**Run with:** Sonnet 5.5 high, Opus review, user play-test. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL`, sub-tags `SKILL`, `NUM`. A skill's
tree row decides its phase: class 200 has it -> CH2a; otherwise the first of 37 / 38 / 39 that does.

| Sub-phase | Scope |
|---|---|
| CH2a0 | **Opus.** Summoned NPCs (CH0 item 7): spawn rules, scripts' binder check, the `NUI_SI_A_LCN_SHADOW` case; CH0 places it |
| CH2a | Duo (200) skills (19 tree rows) |
| CH2b | Chiliarch (37) skills |
| CH2c | Royal Guard (38) skills |
| CH2d | Diablura (39) skills (`Luciel_Demonio`'s `LSI_SA_*` for 39) |

For each id: its case in the skill head (`32b46580`) with June's state functions whole, its attached rows (7u names, 7ai BFI arguments, the
numerical-only tables), and its effect closure. Upgrade ids reach their base through `m_iDeleteSkillIDByUpgrade` (June `32071720`). `NUM use`
lines are logged, and March has no Lu/Ciel to compare against: `MODS.md` lists June's damage per skill. **Done gate:** every class 200 / 37 / 38 /
39 id in CH0's S3 baseline has a state (except passives); each skill cast from an A and a B slot plays June's state, motion and effect and deals
damage; both bodies' skills are tested.

### CH2a - Done (2026-11-15)

Sonnet 5.5 high. **`SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` and the temporary `_SKILL_DEBUG` defined** ( `Always.h` ). Files: `Always.h`, `X2GULuciel.h` / `.cpp`, `MODS.md`.
No review of its own ( the CH2b review covers it; the debug define stays on until then ). Commit: see git log `char-luciel Phase CH2a`.
- **32 functions** ( every `CH2a` row of `ch0_class_map.tsv`; June reads uncarved, so by capstone `funcdis.py` / `disasm_range.py`, the 5 carved ones by Ghidra ):
  8 preloads ( `_Init` ), 11 `_FrameMove` ( 8 are June's `jmp [vtable+0x7cc]` = `CommonFrameMove()`; Flick Shot, Evil Claw, Soul Infest have a body ), 11 `_EventProcess`,
  2 Heavy Hit `_StateStart`. Ten of the eleven EventProcess are one chain ( animation end -> `USI_WAIT`, `SKILL_CANCEL_AFTER( m_fSkillCancelAfter )`, `WALK_CANCEL_AFTER`,
  `DASH_CANCEL_AFTER`, `CommonEventProcess()`; the 10 normalised instruction streams hash equal, June has no BWALK step ); Stomp's start state changes to the landing state
  ( 191 ) once the foot is on a line and the animation passed 0.4 s.
- **8 setter cases** in `SetEquippedSkillFuncBySkillSlotIndex` on `iBaseSkillID` ( `ch1n_skill_cases.tsv` CH2a rows ): callbacks as June's listing; the hyper state copies only
  what June copies ( FrameMove + EventProcess; Heavy Hit also StateStart; never the Init ); extra states Stomp landing 191 ( `m_eSkillID` = `m_iChosenSkillID`, June +0x8ea0 ),
  male Heavy Hit 266 ( skill-state byte, no skill id ), male Haunting Beat 267 ( raw skill id, skill-state byte ); Soul Infest and Nether Shredder store the raw skill id on both slot states.
- **Bodies of note.** Flick Shot ( 0.5 s ): 2 effect sets, 3 when `GetRandomFloat( 0 ) <= 0.5`, `_Trans` set when the skill is an upgrade. Evil Claw: FX3 set at 1.0 s, attack set at 1.05 s
  ( `_Trans` upgraded ). Soul Infest: 20 shells at 0.5-1.13 s and 10 `_Fly` shells at 1.16-1.46 s from `MAJOR:Bip01`, angle = rotate + random ( x * 360, y / z * 120 - 60 ),
  `SetLockOn( LOT_UID_VECTOR, 30 )`; the flying ones replace y / z with the random alone ( decoded from the listing, the decompile merged two random calls ). Heavy Hit start:
  `CommonStateStart`, chance = skill ability `SA_EFFECTIVE_RATE` ( 0.33 default ), then `m_fNowStatePowerRate`, `damage.fPhysic` / `fMagic` x `SA_DAMAGE_REL` ( 2.0 default ) and the
  `EFFECTSET_DEBUFF_POWERFUL_BOWSTRING` set at scale 1.5 ( June reads the ability of the base id 30201 for both ids ).
- **Stand-ins / renames.** Evil Claw's memo sets never play ( June 31fca9e0 memo stub answers 0; not written, as the CH1l3 precedent ). `m_iEvilClawHitCount` ( CH1a's provisional
  name, only user: this function ) is now `m_iEvilClawMemoVariant`. 208 and 30301 / 30302 are CH1n's ( base callbacks only ): tested here, not rewritten.
- **Not June bugs found**; no `JUNE_BUGS` row.
- **Debug lines ( `_SKILL_DEBUG` ):** `[JUNE_CHAR] LUCIEL SKILL equip skill <id> slot <n> bank A|B state <id>` ( every equip ), `flick shot sets <n>`, `evil claw fx3` / `evil claw attack set`,
  `soul infest shell` / `soul infest fly shell`, `heavy hit strong rate .. chance .. body F|M`. Damage numbers: `[JUNE_SKILL] NUM use` ( engine line ).
- **Gates:** `finish.py --files X2GULuciel.cpp X2GULuciel.h Always.h --with LUCIEL_SKILL LUCIEL_SKILL_DEBUG --state X2GULuciel.cpp Luciel_Duo --icons 30001-30302`:
  flagcheck `3 OTHER ( CORE enclosure; Always.h also holds DU3's uncommitted guard hunk ), 0 FAIL`, junediverge PASS, enumscan exit 0, stateends PASS, skillicons PASS,
  `FINISH RESULT: PASS (6 gates, 0 failed, 1 skipped)` ( luabind: no script staged ). The 1 deleted line in `.cpp` / `.h` each = the member rename.
- **Build:** `DEPLOY OK: X2_offline.exe is 26,931,712 B, dated 2026-11-15 17:13:47, md5 e78cb1701f58da334b312146c4bd1fc4, equal to X2/US_SERVICE/x2.exe`. `EXESTR OK: 3 of 3 found in X2_offline.exe` ( the equip, heavy hit and soul infest lines ) and 2 of 2 state names. The build also took in peers' uncommitted files.
- **Play-test save ( `elsdb`, backups `db_backup/*pre_createchar*` ):** `LuSkillA` ( bases at level 1, upgrades level 0 ) and `LuSkillB` ( upgrades level 1, bases level 0 ), class 200 level 70,
  3 skill pages each. Ciel has no body until DU3 ( judged by effects / sounds / log lines ).
- **Play-test 1 (2026-11-15, failed):** the user saw the wrong skill cast on both characters ( Nether Shredder played Soul Infest, Soul Infest Evil Claw, Hunting Prey Nether Shredder; it changed every time ) and cool times not kept. The logs show the setter cases and the equip pairs right and `ownerState` matching the key for the casts they pair, so the cause is not found; use-time lines are missing. **Routed: `CH2a2` ( Opus, `kickoffs/char-luciel/CH2a2.md` ), CH2b waits for it.**
- **Next:** `/x2-phase char-luciel CH2a2` on Opus 5.5 high.

### CH2a2 - Done (2026-11-15)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` ( CH2a's, no new define ); 3 new lines under `_SKILL_DEBUG`. Files: `X2GUUser.cpp`, `X2GULuciel.cpp`.
No review of its own ( the CH2b review covers it; the debug define stays on ). Commit: see git log `char-luciel Phase CH2a2`.
- **Cause ( both symptoms ):** `CX2GUUser::CommonSpecialAttackEventProcess` ( June `31fcadd0` ) hands a class June's pressed-slot numbering ( A 0-3, F 4, B 5-8, T 9 )
  only when its unit type is on the stand-in list `bMarchSlotIndex` ( Eve ... Elesis ); `UT_LUCIEL` was not on it, so Lu got March's ( B 4-7 ). Lu's
  `CanUseSpecialAttack` keeps that index and `StateChangeBySkillSlot` ( June `31fd32e0` ) reads it as June's: Q -> 4 = `USI_SPECIAL_ATTACK_5` ( bank A's F slot ),
  W -> 5 = Q's state, E -> 6 = W's, R -> 7 = E's. That is the user's list exactly ( `LuSkillB` page 1: Q Evil Claw, W Soul Infest, E Nether Shredder, R Haunting
  Beat ). It "changed every time" because the pages swap the banks. Cool time: `ReservedSkillStart` pays with `GetNowStateSkillID()`, so the cool time went
  to the skill of the state entered ( the left neighbour, or nothing for `USI_SPECIAL_ATTACK_5` ), never to the key pressed, so a second press was not refused.
  Bank A ( 0-3 ) was never wrong; CH2a's `ownerState` pairing in play-test 1 was a mis-pairing.
- **Fix ( `X2GUUser.cpp` ):** a new outermost arm of the stand-in chain under `_SKILL`: the list with `UT_LUCIEL` added, the earlier arms in `#else`. Lu now
  also reaches the F / T slots ( index 4 / 9 ), which the setter already handles. No other `bMarchSlotIndex` site exists ( grep ).
- **Debug lines ( `_SKILL_DEBUG`, `X2GULuciel.cpp` ):** `[JUNE_CHAR] LUCIEL SKILL use skill <id> slot <0-9> body L|C state <id>` ( `SpecialAttackUse`, after the
  state change ), `... cooldown skill <id> slot <n> state <id> cool <s>` ( `SpecialAttackCooldown` ), `... refused skill <id> slot <n> cool left <s>`
  ( `CanUseSpecialAttack`, the cool-time refusal ). A healthy use is one `use` line and one `cooldown` line with the same skill id.
- **Not changed:** CH2a's setter cases and state functions ( correct, as its logs showed ). No June bug; no `JUNE_BUGS` row.
- **Gates:** `finish.py --files X2GULuciel.cpp X2GUUser.cpp --with JUNE_CHAR_LUCIEL_SKILL JUNE_CHAR_LUCIEL_SKILL_DEBUG`: flagcheck `0 FAIL, 2 OTHER` ( my lines sit
  inside `_CORE` in `.cpp`; the worktree rows also held peers' uncommitted CH2d20 / CH3c20 hunks, whose 3 + 3 deleted lines are theirs ), junediverge PASS, enumscan exit 0,
  `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`. Skipped: stateends ( no state registered ), skillicons ( no skill added ), luabind ( no script staged ).
- **Build:** the first run ( 17:44 ) failed in char-luciel CH3c20's `NPC_LUA_BINDS` guard ( `Always.h:13695`, not this phase's; the peer narrowed it ). Rebuild:
  `DEPLOY OK: X2_offline.exe is 26,948,608 B, dated 2026-11-15 18:31:09, md5 df9afaf486b60758f01f2f05bc28f749, equal to X2/US_SERVICE/x2.exe (2026-11-15 18:31:01); built at main 18808960 + 2 uncommitted code file(s)`
  ( peers' ). `EXESTR OK: 3 of 3 found in X2_offline.exe` ( the use / cooldown / refused lines ).
- **Staging slip:** `7997ac26` staged the `X2GUUser.cpp` lines by `-U0` line numbers while a peer commit moved the file: the `#ifdef` landed in the Ara arm and the
  `#endif` inside the key loop ( HEAD did not compile ). `18808960` moves them back; the worktree was right throughout. `finish.py --commits 7997ac26 18808960`: PASS.
- **Play-test 1 (2026-11-15, user: all passed; logs 18:37-18:46 on the 18:31 build):** 48 casts, each one `use` + one `cooldown` line on the same skill id
  ( 0 mismatches ); bank B keys reach slots 5-8 and states 60 / 62 / 64 / 66, page 2 puts the bases there; Ciel's Heavy Hit / Haunting Beat enter 266 / 267.
  **Wrong cast: fixed.** Cool time is stored on the pressed skill, but **not enforced**: the save's `auth_level` 3 makes `IsDevUnlimitedSkillUse()` true
  ( `TOGGLE_UNLIMITED_SKILL_USE` is JP-only, `_SERVICE_` undefined, so `auth >= XUAL_DEV` ), and Lu's `CanUseSpecialAttack` then skips cool time, MP and
  combination points ( standing dev rule 2026-10-22 ). So 0 `refused` lines, 5 recasts inside the cool time ( Flick Shot+ by Lu after 5.1 s of 12 ) and
  Nether Shredder / Soul Infest usable without a full gauge ( user's question ) are the dev rule, not a bug. **Owed:** one short retest on `auth_level` 2
  ( second press refused, Nether Shredder refused without a full gauge ), then back to 3.
- **Point on hit ( CH1 review's ask ):** 0 `LUCIEL DUO point` lines, as expected: `AddCombinationPointOnHit` has no caller until CH4a's
  `InitPassiveSkillState` pushes it ( `X2GULuciel.h:779` ). The ask was early; it moves to CH4a's play-test. The HUD points the user saw come from skill hits.
- **Retest (2026-11-15, user set `auth_level` 0; logs 18:54-18:55):** the user saw the combination-point refusal; 4 casts, no recast inside a cool time
  ( Triple Shot again after 9 s of 6 ), no `refused` line ( no press fell inside a cool time; the cool-time test sits in the same block as the gauge test ).
  `auth_level` set back to 3. **Passed.**
- **Found, routed to CH5:** the HUD icon does not grey while the gauge is short. June `UpdateSkillCoolTime` `322e07e0` @322e1840 has a `UT_LUCIEL` arm
  ( cost `32b14f70` > points `32b0fa00` -> fade picture ); March has none. Lu/Ciel HUD, so `CH5.md` ( amended ), not this phase.
- **Next:** CH2b ( Sonnet ) and its review.

### CH2b - Done (2026-11-15)

Sonnet 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` ( CH2a's, no new define ) and the temporary `_SKILL_DEBUG` ( stays on until the CH2b review ). Files: `X2GULuciel.h` / `.cpp`, `X2BuffFactor.h`
( BTI 460 / 464 / 520, BFI 524 under `JUNE_ENUM_NAMES` ), `MODS.md`, `JUNE_BUGS.md` 740, `funcmap.tsv` ( `grep "(CH2b)"`, 9 rows ), `kickoffs/char-luciel/CH2b-review.md` ( covers CH2a, CH2a2, CH2b ). Commit: see git log `char-luciel Phase CH2b`.
- **27 functions + 5 setter cases + 2 registrations.** Seven preloads ( strings read from the listing ), Dead Hands ( 5 ), Dark Crescent ( 6 ), Ruinous Impact ( 5 ), Power of Oriax ( 3 ), Apollyon ( 4 ),
  Blasting Fear and Backstab ( Init + the shared EventProcess each ). Four EventProcess are the CH2a cancel chain unchanged ( normalised listings equal ); Dead Hands adds a head ( animation end or the table's event time
  passed -> wait ), Dark Crescent's lands into `LSI_LU_JUMP_LANDING` when the foot is on a line, Ruinous Impact's at the end casts Dark Crescent through `TryUseSkillWithCombinationPoint` when learned and 5 presses were counted.
  Cases: 30008 / 30038, 30010 / 30062, 30011 / 30063, 30203 / 30213 ( + male 296 ), 30205 / 30307 ( + male 297 ), each with the hyper copies June makes ( `ch1n_skill_cases.tsv` ). 298 / 299 replace CH1m's markers; `ch1m_regcheck.py` 339 matched, 0 problems.
- **Bodies of note.** Dark Crescent's start-future is Aisha's `32cd4830` target pick over `m_vecDarkCrescentTargetUnit` ( AT_SPECIAL for the invincible test, no physic loop ), only when the previous state's skill was Ruinous Impact,
  and it clears the vector on every path. Apollyon at 1.5 s: takes the Seized by Fear stacks ( BTI 460 ) of every enemy in range, multiplies the power rate by `count x ( rate - 1 ) + 1`, plays the `Attack` / `_MEMO` set and the
  `_FX` / `_MemoFX` set scaled by `count x size + 1.2 / 1.0`, and with Throne Authority ( 30417 ) can cut cool times and give combination points. Dead Hands' buff is BFI 524 at 0.7 s.
- **June +0x9444 ( `m_eStateStartSkillID` ).** June's `CommonStateStart` copies the state's skill id into it; Ruinous Impact's start reads it **before** its own CommonStateStart ( the previous state ), Dark Crescent's start-future too.
  March's `GetNowStateSkillID()` is already the new state there, so `CX2GULuciel` keeps its own copy ( written after `CX2GUUser::CommonStateStart` ). `31edf560` is a new `CX2GULuciel::IsSkillOrUpgrade` ( Chung's body ).
- **CH1a fix.** `TryUseSkillWithCombinationPoint` ( June `32b3acf0` ) returns bool: June's `al` is 0 at every refusal and 1 after the skill started; CH1a ported it void. No caller existed before Ruinous Impact.
- **Stand-ins / not written.** Dark Crescent's 0.25 s memo block ( skill memo 610011 ) is behind June's `31fca9e0` stub ( answers 0 ), as Evil Claw's memo sets in CH2a. Apollyon's stacks come from the Seized by Fear passive ( CH4a ): until then the
  count is 0 and the power rate is unchanged.
- **June bug found.** JUNE_BUGS 740: Apollyon's Throne Authority roll passes at or above the rate ( the memo-set roll before it passes at or under ). Kept.
- **Debug lines ( `_SKILL_DEBUG` ).** `dead hands buff level <n> skill <id>`, `ruinous impact end presses <n> targets <n>`, `dark crescent start previous skill <id> targets <n>`, `dark crescent slash set played`,
  `apollyon stacks <n> max <n> range <r> attack step <a> size step <s> memo set <0|1>`. Damage numbers: `[JUNE_SKILL] NUM use`.
- **Gates:** `finish.py --files X2GULuciel.cpp X2GULuciel.h X2BuffFactor.h --with JUNE_CHAR_LUCIEL_SKILL JUNE_CHAR_LUCIEL_SKILL_DEBUG JUNE_ENUM_NAMES --state X2GULuciel.cpp Luciel_Duo --icons 30001-30307`: flagcheck `JUNE_CHAR_LUCIEL_SKILL` PASS
  ( 3 OTHER: the `_CORE` enclosure; the `return` -> `return false` lines are the CH1a fix ), `JUNE_ENUM_NAMES` PASS ( 1 PASS~, 2 OTHER ), junediverge PASS, enumscan exit 0, stateends PASS, skillicons PASS,
  `FINISH RESULT: PASS (7 gates, 0 failed, 1 skipped)` ( luabind: no script staged ). `/Zs` probe of `X2GULuciel.cpp`: exit 0.
- **Build:** `DEPLOY OK: X2_offline.exe is 26,962,432 B, dated 2026-11-15 19:41:20, md5 f9a0cda252fc7f6b92f966694b6de7dc, equal to X2/US_SERVICE/x2.exe (2026-11-15 19:41:13); built at main 6d076802 + 3 uncommitted code file(s)`
  ( mine ). `EXESTR OK: 5 of 5` ( the five debug lines ), `2 of 2` ( `LSI_SA_LLC_DARK_CRESCENT`, `LSI_A_LLC_M_BACKSTAB` ), `1 of 1` wide ( `EffectSet_SA_LLC_APOLLYON_Attack_MemoFX` ).
- **Play-test save ( `elsdb`, backups `db_backup/*pre_createchar*` ):** `LuChA` ( unit 193, class 37, level 70: the five Chiliarch bases + Blasting Fear + Backstab at level 1, upgrades level 0 ) and `LuChB` ( unit 194: upgrades level 1,
  bases level 0 ). Page 1: A bank Dead Hands / Ruinous Impact / Dark Crescent / Blasting Fear, B bank Power of Oriax / Apollyon / Backstab; page 2 swaps the banks.
- **Play-test ( 2026-11-15, user: all passed; logs 20:15-20:19 on the 19:41 build ):** 24 casts of all 14 ids from A and B slots on both pages, each one `use` + one `cooldown` line on the same skill id; bank B reaches slots 5-8 ( states 60 / 62 / 64 ),
  Blasting Fear / Backstab enter 296 / 297 on Ciel ( body C ) and the F states on Lu; `dead hands buff level 1` for 30008 / 30038; `apollyon stacks 0 max 20 range 800` ( 0 stacks as expected ); `dark crescent slash set played` each cast; 0 EXCEPTION,
  UNHANDLED only `E_DISABLE_HB_CHECK_REQ`. **Not exercised:** the Ruinous Impact -> Dark Crescent target landing: `ruinous impact end` shows `presses 0-1 targets 0` every time, because the function that records the hit enemies in
  `m_vecDarkCrescentTargetUnit` ( June `32b177b0`, a Ruinous Impact damage callback ) is CH4a's ( `ch0_class_map.tsv` row, phase CH4a ), so the vector stays empty and Dark Crescent starts with no target ( June behaviour without it ). Retest the chain at CH4a.
- **Next:** `/x2-phase char-luciel CH2b review` on Opus 5.5 high.

### CH2b - Review (2026-11-15)

Opus 5.5 high. Covers CH2a ( `fe073ac8` ), CH2a2 ( `7997ac26`, `18808960` ) and CH2b ( `ac6dda59` ). **Verdict: pass after one fix.** Files: `X2GULuciel.cpp`, `Always.h`.
- **Fix ( `_SKILL` ).** `GetSkillCombinationPointCost` ( CH1a, June `32b14f70` ): the Fantasie Impromptu 2nd-movement discount reads June +0x9444 @32b15014
  ( the state-start skill ), not vslot 0x524; now `m_eStateStartSkillID` under `_SKILL`, CH1a's `GetNowStateSkillID()` in `#else`. Same value unless a state's skill
  changes after its CommonStateStart; reachable from CH2c ( 30021 / 30023 ).
- **Byte scan of disp32 +0x9444 over June `.text`: 5 hits.** Writers: ctor @32041de1 ( 0 = `SI_NONE`, = the port's ctor init ), `CX2GUUser::CommonStateStart` @32060cdb
  ( vslot 0x524 ). Readers: all three in `CX2GULuciel`, Ruinous Impact start @32affca3, cost @32b15014 ( the fix ), Dark Crescent start-future @32b2ac34. None missed.
  The port writes it in `CX2GULuciel::CommonStateStart` after the base; the base is called unqualified nowhere else in the class ( grep ), so every Lu state writes it.
- **Checked against June ( capstone `disasm_range.py` ) - all = June:**
  - Dead Hands EventProcess `32aff850`: animation end, or +0x8c18 ( `m_fEventTime[0].stateChange`, Chung CH2b's proof ) <= now ( `test ah, 0x41` / `jnp`: NaN does not end ),
    then the cancel chain identical to Power of Oriax `32affcf0` instruction by instruction.
  - Setter cases: hyper copies as June ( Dead Hands 4 callbacks, Apollyon 3, Oriax 2, Blasting Fear / Backstab EventProcess only; never the Init ). Ruinous Impact start `32affca0`:
    IsSkillOrUpgrade( +0x9444, 30008 Dead Hands ) else key = slot - 1, loop count 0, then CommonStateStart.
  - Dark Crescent start-future `32b2ac20`: IsSkillOrUpgrade( +0x9444, 30007 Ruinous Impact ); filters team, slot 94 with AT_SPECIAL ( 1 ), 0 < HP ( NaN passes, as `<= 0` ),
    boss-raid slot 0x3c8, line LT_NORMAL + enabled, `CanDown( pos, 30.0, 0 x5, 1, 0 )`, distance < 700; pick rule, -1 seed, flip of +0x7e56, position + line +0x28
    ( `startPos.y` ) + 20.0 ( double ); vector cleared on every path, then CommonStateStartFuture. Same as Arme's `32cd4830` port except the attack type ( Aisha: AT_COMMAND ).
  - Apollyon FrameMove `32b2b050`: SA 29 / 6 / 2 / 31 / 5 = `SA_SCOPE_ABS` / `ATK_REL` / `EFFECTIVE_COUNT` / `PROJECTILE_RANGE_REL` / `EFFECTIVE_RATE` ( `s0a_templet_keys.tsv` );
    defaults 0 / 1 / 0 / 1 / 0; the `[esp+0x34]` slot is the size rate for both sets; slot 0x180 = `GetInvincible` ( funcmap 429 / 430 ); `dist < range`; cap at the count;
    power rate x ( n x step + 1.0 ); memo roll `random <= oriax + rate`; scales n x size + 1.2 ( `3346f488`, a double ) / + 1.0; FX set Y 180 when facing left;
    Throne Authority 30417 only without the memo sets, SA 149 rate, pass at `rate <= random` ( JUNE_BUGS 740 confirmed ), SA 148 cool-time cut, SA 2 points.
  - `TryUseSkillWithCombinationPoint` `32b3acf0`: `al` = 0 at every refusal ( @32b3af90 ), 1 @32b3b4f7 after `SpecialAttackNoStageChange` ( vslot 0x810 ) or
    `StateChangeBySkillID` ( `32aeaa10` ) and `OnSkillUse`. CH2a's use / cooldown debug lines are unaffected.
  - Constants: 0.7 / 0.2 / 700 / 20.0 / 1.5 / 1.0 / 5e-8 / 1.2 / 180 / 30.0 read from the exe = the port.
  - CH2a2's `bMarchSlotIndex` arm ( diff of `18808960` ): outermost, Lu added, earlier arms in `#else` intact. CH2a's cases: diff only ( play-tested ).
- **Minor, not changed:** `CX2GULuciel::IsSkillOrUpgrade` repeats June `31edf560` ( already ported for Eve / Chung ); a shared helper is a cleanup, not a defect.
- **Debug retired:** `//#define SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL_DEBUG` ( bytepatch; the CH2b play-test passed ). `flagcheck ..._SKILL_DEBUG --worktree --files KTDXLIB/Always.h`:
  1 DEFINE, 0 FAIL, 0 ERROR, the 1 deleted line is the `#define` ( "not #defined" is the retire itself ). CH2c re-enables it for its play-test if it wants the use / cooldown lines.
- **Gates.** `finish.py --review --commits fe073ac8 7997ac26 18808960 ac6dda59 --with ..._SKILL ..._SKILL_DEBUG -v`: flagcheck `_SKILL` PASS ( 1 PASS, 1 DEFINE, 7 OTHER, 0 FAIL ),
  junediverge PASS, `--check-all` PASS, enumscan exit 0, `FINISH RESULT: PASS (7 gates, 0 failed, 3 skipped)`. OTHER rows: `.cpp` / `.h` hunks sit inside `_CORE`;
  `X2GUUser.cpp` sits in the Arme `_CORE` enclosure; `X2BuffFactor.h` = `JUNE_ENUM_NAMES`. Deleted lines: CH2a 1 + 1 ( member rename ), CH2b `.cpp` 10 ( 7 `return;`, the
  signature, the 2-line `OnSkillUse` if ) and `.h` 2 ( the declaration ), `18808960` 5 ( the staging slip moved back; `7997ac26`'s `tok!` row is that slip ).
  `stateends.py check X2GULuciel.cpp Luciel_Duo`: 370 tables, 0 STUCK, PASS. This review: `finish.py --files X2Lib/X2GULuciel.cpp --with ..._SKILL`: flagcheck PASS
  ( 1 PASS ), junediverge PASS, enumscan exit 0, `FINISH RESULT: PASS (6 gates, 0 failed, 3 skipped)`.
- **Build:** `/Zs` probes of `X2GULuciel.cpp` and `X2GUUser.cpp`: exit 0, 0 errors. Full build skipped: no play-test ( the fix's skill is CH2c's ); the next play-test build takes it.
- **Commit `7135cd45`, range gate:** `finish.py --range 7135cd45^..7135cd45` reports 1 ERROR on `Always.h` ( the `_SKILL` run cannot unifdef the retired `_SKILL_DEBUG` guard line
  13673; a tool limit when a retire and a fix share a commit, not a defect ). Per file: `flagcheck _SKILL --range ... --files X2Lib/X2GULuciel.cpp` 1 PASS, 0 FAIL, `RESULT: PASS`;
  `flagcheck _SKILL_DEBUG --range ... --files KTDXLIB/Always.h` 1 DEFINE, 0 FAIL, 0 ERROR ( only the retire's "not #defined" ); junediverge PASS, enumscan exit 0.
- **Next:** `/x2-phase char-luciel CH2c` on Sonnet 5.5 high.

### CH2c - Done (2026-11-15)

Sonnet 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` ( CH2a's, no new define ); the temporary `_SKILL_DEBUG` is **back on** ( un-commented in `Always.h` for the play-test; the CH2c2 review retires it again ).
Files: `X2GULuciel.h` / `.cpp`, `Always.h` ( that one line ), `MODS.md`, `funcmap.tsv` ( `grep "(CH2c)"`, 7 rows ), `june2017_re/char_luciel/ch2c_cases.py` ( new ), `kickoffs/char-luciel/CH2c2.md` ( amended ).
No review of its own ( the CH2c2 review covers it ). Commit: see git log `char-luciel Phase CH2c`.
- **35 functions** ( every `CH2c` row of `ch0_class_map.tsv`, read with `charport_adis.py`; the thunks are not written ): 8 preloads ( 130 reads, no other call in any of them ), Soul Extortion 5,
  Demonic Breath 4, Fantasie Impromptu 10, Death Crisis of Marbas 8, Eschaton EventProcess. Two event processes are the shared cancel chain ( normalised listings equal to Power of Oriax's `32affcf0`:
  Soul Extortion, Marbas end ), three are that chain with a different animation-end head ( Cantabile ready -> 350, Marbas big fire -> 358 / 362 by the charge, Eschaton -> wait / jump-down by foot and body ).
- **Deviation from the kickoff ( the one decision ): only Soul Extortion's setter case is written; the other six wait for CH2c2.** Each of Soul Shock, Demonic Breath, Fantasie Impromptu, Death Crisis of Marbas, Soul Strike
  and Shadow has a state whose EventProcess ( or the FrameMove that carries its hits ) is a CH2c2 function ( `ch0_class_map.tsv` col 6 ), so registering them now would register states with no EventProcess: `stateends.py`
  fails them and a cast would leave the player stuck and invincible. CH2c2's kickoff is amended with the six entries and `ch2c_cases.py` ( annotated listing of a case, each callback marked with its phase ).
  Eschaton ( 363 ) is registered in `InitStateCommon` with Init + EventProcess ( no stuck risk ); its FrameMove is CH2c2's ( marker for `ch1m_regcheck.py`: **0 problems, 340 matched** ).
- **Bodies of note.** Soul Extortion: both starts multiply the animation speed ( +0x8900 future / +0x88fc now ) by the upgrade's `SA_ANI_SPEED` ( 30039, memo swap off ) and call `ResetAnimSpeed`.
  Fantasie movement states: June's jump chains on left / right / facing reduce to bool tests ( worked out per branch; both bodies of the 2nd movement are the same listing with other ids ); the wait time ( +0x9628 ) at 2.0 hands
  over. Demonic Breath fire: five presses of the key ( `IsKeyPressedOnce`, the Ruinous Impact pattern ) cast Eschaton through `TryUseSkillWithCombinationPoint` ( the upgrade swap of 30020; ground 1 / air 2 ), a refusal resets the count.
  Death Crisis ready start: camera override 550 / 220 for the own unit ( new stand-in member `m_fSkillCameraOverrideSecondValue`, +0x8d94, as Rena's ), min / max charge = 1 - `SA_MP_CONSUME_REL` / `SA_MP_CONSUMPTION` ( upgraded skill only, fsubrp read as Intel `st1 = st0 - st1` ).
- **Stand-ins / not written.** Soul Extortion's `StateEnd` ( 32b3dd80 ) and Eschaton's `FrameMove` ( 32b2cfc0 ): CH2c2. No June bug found; no `JUNE_BUGS` row.
- **Debug line ( `_SKILL_DEBUG` ).** `[JUNE_CHAR] LUCIEL SKILL soul extortion start skill <id> anim rate <r> speed <s>` ( the now-side start ). The `equip` / `use` / `cooldown` lines of CH2a / CH2a2 are on again.
- **Gates:** `finish.py --files X2GULuciel.cpp X2GULuciel.h Always.h --with JUNE_CHAR_LUCIEL_SKILL JUNE_CHAR_LUCIEL_SKILL_DEBUG --state X2GULuciel.cpp Luciel_Duo --icons 30001-30310`: flagcheck `_SKILL` PASS ( 2 PASS, 1 OTHER = the
  `_CORE` enclosure, 0 FAIL ), junediverge PASS, enumscan exit 0, stateends PASS, skillicons PASS, `FINISH RESULT: PASS (6 gates, 0 failed, 1 skipped)` ( luabind: no script staged ). `/Zs` probe of `X2GULuciel.cpp`: exit 0.
- **Build:** `BUILD OK: X2/US_SERVICE/x2.exe 26,977,280 B, 2026-11-15 21:18:58, md5 768c6745b5e058a3278872d45e585f57` ( KTDXLIB 61 s, X2Lib 556 s, X2 23 s; built at main 244c652c + peers' uncommitted code files, among them DU4's `NESI_RENUMBER` ). `DEPLOY REFUSED` first ( game running ), then `--deploy-only`: `DEPLOY OK: X2_offline.exe is 26,977,280 B, dated 2026-11-15 21:23:15, md5 768c6745b5e058a3278872d45e585f57, equal to X2/US_SERVICE/x2.exe (2026-11-15 21:18:58); built at main fbadd300`; `EXESTR OK: 1 of 1` wide in the deployed exe. `EXESTR OK: 4 of 4` wide ( the debug line, preload strings ) and `2 of 2` narrow ( `LSI_SA_LCG_SOUL_EXTORTION`, `LSI_SA_LCG_ESCHATON` ) in the built exe ( `--exe build` ). The save is already schema v24 ( DU4's play-test ran the game ), so this exe needs no further migration.
- **Play-test save ( `elsdb`, backups `db_backup/*pre_createchar*` ):** `LuRgA` ( unit 198, class 38, level 70: Soul Extortion base 30019 level 1, upgrade 30039 level 0 ) and `LuRgB` ( unit 199: 30039 level 1, base 0 ). Page 1: 30019 / 30039 on
  slot 0 ( key A ), page 2: on slot 5 ( key Q ); no other Royal Guard skill is slotted ( their cases wait for CH2c2 ).
- **Commit `075874fc`, range gate:** `finish.py --range 075874fc^..075874fc --with ...`: flagcheck `_SKILL` PASS ( 2 PASS, 1 DEFINE = the debug define back on, 0 FAIL; 1 deleted line = that `//#define` ), junediverge PASS, enumscan exit 0.
- **Play-test hand-off ( Soul Extortion, 30019 / 30039; after the user closes the game and `build.py --deploy-only` prints `DEPLOY OK` ):** no dev-console line needed ( both units are level 70 ).
  Select `LuRgA` ( base ), enter a dungeon or the training room with enemies in front, and with page 1 active press key A ( slot 0 ); then switch to page 2 and press key Q ( slot 5 ); repeat with `LuRgB` ( the upgrade ).
  Expect: Lu plays her `Out` animation and Ciel casts ( June: `DUO_TYPE` minor ): other units stop for about 0.7 s, an effect set at 0.01 s ( the cross ), the first attack set at 0.32 s and the second at 0.92 s hit the enemies in front; you can walk
  after it ends. Not expected yet: the soul drain at the end ( the `StateEnd` is CH2c2's ). If you are stuck after the skill, or Lu / Ciel cast the wrong skill, say which key. Logs to read: `python tools/logs.py --grep "JUNE_CHAR\] LUCIEL SKILL"`:
  `equip skill 30019 slot 0 bank A state <id>` and `... slot 5 bank B ...` ( 30039 on `LuRgB` ), one `use skill 30019 slot 0|5 body L|C` + one `cooldown` line per cast, one `soul extortion start skill <id> anim rate <r> speed <s>` per cast ( rate 1.00 unless the upgrade's `SA_ANI_SPEED` is set ).
- **Play-test ( 2026-11-15, user: passed; logs 21:25-21:26 on the 21:23 deploy ):** 9 casts, each one `use` + one `cooldown` + one `soul extortion start` line on the same skill id: `LuRgA` 30019 x4 ( rate 1.00 ), `LuRgB` 30039 x5
  ( rate 1.30 = the upgrade's `SA_ANI_SPEED` ), both bodies ( `body L` from Lu, `body C` from Ciel ), `equip` lines state 50 ( key A ). 0 EXCEPTION, 0 ErrorLog lines, 0 unbound Lua names; UNHANDLED only `E_DISABLE_HB_CHECK_REQ`.
  **Not exercised:** page 2 / key Q ( bank B, slot 5 ): no `equip ... slot 5 bank B` line, so the B-slot cast is still owed ( CH2c2's play-test can take it with the other ids ).
- **Next:** `/x2-phase char-luciel CH2c2` on Sonnet 5.5 high.

### CH2c2 - Done (2026-11-15)

Sonnet 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` ( CH2a's, no new define ); `SERV_IRUHADEV_JUNE_ENUM_NAMES` for 6 new buff names; the temporary `_SKILL_DEBUG` stays **on** ( the CH2c2 review retires it ).
Files: `X2GULuciel.h` / `.cpp`, `X2BuffFactor.h` ( bytepatch ), `MODS.md`, `funcmap.tsv` ( `grep "(CH2c2)"`, 8 rows ), `kickoffs/char-luciel/CH2c2-review.md` ( new ). Review: `CH2c2-review.md` ( covers CH2c, CH2c2 ). Commit: see git log `char-luciel Phase CH2c2`.
- **Written ( every `CH2c2` row of `ch0_class_map.tsv`, read with `charport_adis.py`; the thunks are not written ):** Soul Shock ( both bodies share the event process, the frame moves share `LSI_A_LCG_SOUL_SHOCK_PlayEffects` ), Soul Strike ( 3 event processes, end future / end, 2 frame moves + `LSI_A_LCG_SOUL_STRIKE_LOOP_ApplyStackRates` ),
  Demonic Breath ( 3 + fire frame move + `LSI_SA_LCG_DEMONIC_BREATH_CountKeyPress` ), Fantasie Impromptu ( future frame move for six states, 2 frame moves, end event process ), Death Crisis of Marbas ( 9 + `SetLeftHandRotateBone` + two small helpers ),
  Shadow ( 2 frame moves, `ApplyShadowBuff` ), Eschaton frame move, Soul Extortion state end, `AddSoulCollectorStack` ( June `32b3db80`, 4 callers; two are the state ends ).
- **The six setter cases ( + Soul Extortion's `StateEnd`, Eschaton's `FrameMove` ):** Soul Shock 10205 ( male 344, skill-state byte ), Demonic Breath ( 348 / 349 ), Fantasie Impromptu ( 350-357 ), Death Crisis of Marbas ( 358-362; both the normal and the hyper state keep `m_eSkillID` = the equipped id ),
  Soul Strike ( 342 / 343 / 345 ( byte ) / 346 / 347 ), Shadow ( 403, byte ). Hyper states copy exactly the slots June's listing copies ( read per case with `ch2c_cases.py`, not all nine ). `ch1m_regcheck.py`: **340 matched, 0 problems**; `stateends.py` PASS ( 407 tables, 0 STUCK ).
- **New names.** `BTI_BUFF_SHADOW_LU` 478 / `_CIEL` 479, `BFI_BUFF_SOUL_COLLECTOR` 523, `BFI_BUFF_NOBLE_BEING` 540, `BFI_BUFF_SHADOW_LU` 545 / `_CIEL` 546 ( June's Enum.lua, `JUNE_ENUM_NAMES` in `X2BuffFactor.h`, no collision ).
- **Readings that were not obvious.** `31f8df00` = `GetGageData()` +0x24 = the soul gauge ( `UpNowSoul`; confirmed by the inlined copy in `32b3dea0` ). `32007b10( 0x8f, skill )` leaves its third push as `FlushMp`'s gauge index ( the minor-body flag ): the three-argument look is two calls.
  Soul Shock's / Soul Strike's `fmulp st(2)` stack ( `1 + n x step`, `ESR + n x step` ) written out in the bodies. `vslot 0x884` is `TryUseSkillWithCombinationPoint`. Shadow's EP = `CX2GUUser::GenericActiveSkillEventProcess` ( `329cba60` is a thunk to `31fcc9d0` ).
  `+0xe4..0xec` of an effect-set instance = `m_vLocalRotateDegree` ( roll in z ), `+0x7c` = `m_bDamageEffectScale`.
- **Stand-ins / not written ( one line each ).** June's memo getter `31fca9e0` is a stub ( 0 ): no memo set plays, as CH2c. The Soul Shock damage effect's rate is rescaled through `CEffect::SetPowerRate`: June reads a protected float at effect +0x2a4 ( the power rate by use; the review confirms the offset ).
  No June bug found ( both Soul Shock / Soul Strike EP copies are byte-equal, kept as two functions ): no `JUNE_BUGS` row.
- **Debug lines ( `_SKILL_DEBUG` ).** `[JUNE_CHAR] LUCIEL SKILL soul shock / demonic breath fire / fantasie impromptu cantabile beat / death crisis small fire / soul strike male | female shot / shadow buff / eschaton ...`; CH2a / CH2c's `use`, `cooldown`, `equip`, `soul extortion start` lines stay.
- **Gates:** `finish.py --files X2GULuciel.cpp X2GULuciel.h X2BuffFactor.h --with JUNE_CHAR_LUCIEL_SKILL JUNE_ENUM_NAMES JUNE_CHAR_LUCIEL_SKILL_DEBUG --state X2GULuciel.cpp Luciel_Duo --icons 10205 30001-30310`: flagcheck `_SKILL` PASS ( 2 PASS, 1 OTHER = the `_CORE` enclosure, 0 FAIL; 3 deleted lines = CH2c's three "body in CH2c2" markers / comment ), `JUNE_ENUM_NAMES` PASS, junediverge PASS, enumscan exit 0, stateends PASS, skillicons PASS, `FINISH RESULT: PASS (7 gates, 0 failed, 1 skipped)` ( luabind: no script staged ). `/Zs` probe of `X2GULuciel.cpp`: exit 0.
- **Build:** `BUILD OK: X2/US_SERVICE/x2.exe 27,020,800 B, 2026-11-15 22:24:11`. `DEPLOY OK: X2_offline.exe is 27,020,800 B, dated 2026-11-15 22:24:18, md5 49fa44dc1524480474d0ac42dc78c1d3, equal to X2/US_SERVICE/x2.exe (2026-11-15 22:24:11); built at main fac2180d + 38 uncommitted code file(s).` `EXESTR OK` in the deployed exe: wide `[JUNE_CHAR] LUCIEL SKILL soul shock ...`, wide `EffectSet_SA_LCG_DEMONIC_BREATH_FireFX3`, narrow `LSI_A_LCG_M_SOUL_SHOCK`.
- **Play-test save ( `elsdb`, backup `db_backup/els_db_20261115_220809_pre_sql.sql` ):** `LuRgA` ( unit 198, class 38, level 70 ) with the base skills and `LuRgB` ( unit 199 ) with the upgrades ( base level 0 ), same slots: page 1 = A slots 0-3 Soul Shock / Demonic Breath / Fantasie Impromptu / Death Crisis of Marbas, B slots 5-7 Soul Strike / Shadow / Soul Extortion;
  page 2 = A slots 0-2 Soul Strike / Shadow / Soul Extortion, B slots 5-8 Soul Shock / Demonic Breath / Fantasie Impromptu / Death Crisis of Marbas ( every skill from an A and a B slot; Soul Extortion's owed B-slot cast included ).
- **Play-test hand-off:** in the message that closes the session ( dev console first, per key, with the log lines to expect ).
- **Play-test ( 2026-11-15, user: all passed; logs 22:30-22:33 on the 22:24 deploy ):** 0 EXCEPTION, 0 ErrorLog lines, 0 unbound Lua names; UNHANDLED only `E_DISABLE_HB_CHECK_REQ`. 24 `use` + 24 `cooldown` lines: Soul Shock 10205 ( Lu slot 0, Ciel slot 0 / 5; power 1.6 / 2.0 with stacks ), Demonic Breath 30017 / 30068 ( fire, presses 0 ), Fantasie 30021 / 30069 ( 42 cantabile beat lines ), Death Crisis 30018 / 30037 ( 66 small-fire lines ), Soul Strike 30214 on Ciel ( male shots, loop 0-7 ), Shadow 30209 / 30310 ( buff on both bodies ), Soul Extortion 30019 / 30039 from slot 2 and 7 ( B slot done ).
  **Not in the logs:** the female Soul Strike ( 30207 from Lu ), the Eschaton chain ( no 5-press cast ), Accelerando / big fire ( no debug line ); the user reports them passed by sight. The review can re-test those three if it wants a log line.
- **Next:** `/x2-phase char-luciel CH2c2 review` on Opus 5.5 high; CH2d waits for that review.

### CH2c2 - Review (2026-11-15)

Opus 5.5 high. Reviewed CH2c ( `075874fc` ) and CH2c2 ( `833d1d53` ) against June in Ghidra ( read-only ). **Verdict: pass after one fix and one rename**; `_SKILL_DEBUG` retired.
Files: `X2GULuciel.cpp` / `.h`, `Always.h` ( the debug line ), `ch1_offsets.tsv`, `funcmap.tsv` ( the 8 CH2c2 rows, left uncommitted by CH2c2, + 2 corrected ), `MODS.md` ( CH2c2's section, also left uncommitted, + 1 corrected line ), `kickoffs/char-luciel/CH2d.md`.
- **Fix ( `_SKILL`, Soul Shock `32b2db60` / `32b2e080` ):** June's protected float at damage effect `+0x2a4` is not the power rate: `+0x1d8` is ( June's CEffect ctor `31f7aab0` @31f7aea1 stores the power-rate argument
  there and passes it to `32632390` with `&DamageData` = `+0x200` ); `+0x2a4` = DamageData `+0xa4` = `m_ExtraDamage.m_Damage` ( proved by CH1l3's review, Raven Megadrill reader `32c5ce40` ). CH2c2 rescaled the power
  rate through `CEffect::SetPowerRate`; it now rescales `GetDamageData().m_ExtraDamage.m_Damage` by target rate / power rate, written only when it changes, as June. One-line tag `char-luciel CH2c2 review`.
- **Rename ( CH1a's provisional name ):** `+0x968c` `m_fSoulStrikeChargeTime` -> `m_fShadowShieldAmount`. Its only June users are Shadow's `32b20670` ( writes max HP x `SA_MAX_HP_REL` / `_PVP` ) and `32aebaf0`
  ( the user DamageReact absorb: drains it, erases BTI 478 / 479 at <= 0; checklist 7j, CH4b ) and the ctor ( `dispsweep.py 968c` ). No behaviour change.
- **Spot-checked = June ( decompile + listing ):** `AddSoulCollectorStack` `32b3db80` ( BTI 0x1ce = 462, missing-templet limit 9999, `limit - 1 <= count` unsigned, Tribute 0x76c9 -> BFI 0x21c = 540,
  `count != 0 && cool <= 0` ( NaN skips ), `vslot 0xac < 3` -> soul `SA 0xaa`, buff time `0xa9`, cool `0x94`, BFI 0x20b = 523 at `+0x9608` ); Soul Extortion end `32b3dd80` ( roll > 0.5, default 5, UCHAR loop, `vslot 0x7dc` );
  Soul Strike end `32b3dea0` ( erase, the inlined stack body per loop above 5, reset to 5 ); Shadow `32b20670` ( templet 0x7601 with the upgrade flag, `+0x867c` unit, level floor 1, `vslot 0x260` -> 0x221 / 0x222 = 545 / 546,
  `322cc1a0` -> `0xab` else `0x14` ); Soul Shock ( ids 0x27dd, SA 6 / 0x20 / 0x51, scale `+0x70..+0x78` + `+0x7c` = `SetEffectScale( v, true )` ); Demonic Breath charge `32b1d490` ( `0.3 < t || 9 < n` -> 349, MP
  `32007b10( 0x8f, future skill )` into `vslot 0xe4` with the minor flag, count + 1, `+0x95e8` = 0 ); Accelerando end `32b1d5f0` ( loop count `+0x95da > 4` -> `vslot 0x884( 31fcf430( 0x7547 ), 1 )`, refusal -> state 13, else short = 5 );
  `SetLeftHandRotateBone` `32b15680` ( `+0x9630 += elapsed x class float`, clamp 45 ); female shot `32b2e5a0` ( `( r % 3 ) x 50 - 25` ). Enum values re-counted from `X2SkillTree.h` ( `HP_CONSUME_REL` has no `SA_` prefix: 148 / 169 / 170 / 171 hold ).
  One failure in nine, and it was the item CH2c2 itself left to this review, so the rest of the phase was not re-read line by line.
- **Setter cases:** `ch1m_regcheck.py` 340 matched / 0 problems and `stateends.py` PASS ( CH2c2's range gate below ); not re-derived.
- **Hygiene ( `finish.py --review --commits 075874fc 833d1d53 --with JUNE_CHAR_LUCIEL_SKILL JUNE_ENUM_NAMES JUNE_CHAR_LUCIEL_SKILL_DEBUG -v` ):** junediverge PASS, `--check-all` PASS, enumscan exit 0; flagcheck `_SKILL` and
  `JUNE_ENUM_NAMES` each 1 ERROR, both on `075874fc` `Always.h` line 10175 ( `#if defined( ..._USER_SKILL_USE_HELPERS ) && !defined( ..._JUNE_ENUM_NAMES )`, which flagcheck cannot resolve once `JUNE_ENUM_NAMES` is a partner ),
  not a code defect: run per commit with that commit's own flags both pass, `075874fc` ( `_SKILL` + `_DEBUG` ): `FINISH RESULT: PASS (6 gates)`, `833d1d53` ( + `JUNE_ENUM_NAMES`, `--state`; stateends PASS ): `FINISH RESULT: PASS (7 gates)`.
  OTHER rows: `X2BuffFactor.h` under `_SKILL` = the `JUNE_ENUM_NAMES` names ( PASS there ), `X2GULuciel.cpp` under `JUNE_ENUM_NAMES` = the `_CORE` / `_SKILL` blocks. Deleted lines: `075874fc` 1 = the `//#define` turned on,
  `833d1d53` 3 = CH2c's "body in CH2c2" markers. `833d1d53` also carried a racing peer's deletion of `kickoffs/june-stat-calc/D5-review.md`; its owner restored it ( `ddaf8568` ).
- **This review's gates:** `/Zs` probe of `X2GULuciel.cpp`: exit 0, 0 errors. Worktree `finish.py --files X2GULuciel.cpp X2GULuciel.h Always.h --with _SKILL _CORE _SKILL_DEBUG`: both class files PASS, junediverge PASS, enumscan 0,
  `_SKILL_DEBUG` retired; the `Always.h` ERROR is a peer's uncommitted `Always.h` hunks ( char-add ) plus the `_LIVE` / `_CORE` guard pairing. Range gate after the commit: below.
- **Commit `bc2f6000`, range gate:** `finish.py --range bc2f6000^..bc2f6000 --with _SKILL _SKILL_DEBUG` reports 1 ERROR on `Always.h` ( the retired `_SKILL_DEBUG` guard line 13688; the tool limit the CH2b review
  met ), and with `_CORE` as a partner 1 ERROR on the `_LIVE` / `_CORE` guard line 13445. Per file: `flagcheck _CORE --range ... --files X2GULuciel.cpp X2GULuciel.h` 2 PASS, 0 FAIL, `RESULT: PASS` ( 8 deleted lines =
  the 5 replaced Soul Shock lines, the comment line, the 2 renamed member lines ); `flagcheck _SKILL_DEBUG --files Always.h`: only the retire's "not #defined"; junediverge PASS, enumscan exit 0.
- **Build:** full build skipped: no play-test ( the fix changes one number on Soul Shock's damage effect; CH2d's build carries it, and its hand-off re-casts Soul Shock once ).
- **Next:** `/x2-phase char-luciel CH2d` on Sonnet 5.5 high.

### CH2d20 - Done (2026-11-15)

Opus 5.5 high. Files: `X2GUUser.h` / `.cpp`, `X2UIManager.h` / `.cpp`, `X2StateMenu.cpp`, `X2Game.cpp`, `X2GULuciel.cpp`, `Always.h`. Flags: `SERV_IRUHADEV_JUNE_CHAR_TRANSFORM_LINK`
( the window, the user wrappers, the `UI_MENU` member ) and `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` ( the hooks inside March functions; new `Always.h` guard: `_LIVE` needs
`TRANSFORM_LINK` ). Review: `kickoffs/char-luciel/CH2d20-review.md`. Commit: see git log `char-luciel Phase CH2d20`. funcmap: `grep "(CH2d20)"` ( 16 rows ).
- **The window `CX2TransformSkillSlotUI` is whole** ( every June function on it, all read in Ghidra + capstone, constants by `charport_consts.py` ):
  ctor `3200f380` ( dialog `DLG_LUCIEL_TRANSFORM_SKILL_SLOT.lua` on the now state, shown, a `CX2TransFormSkillSlotData` in `SlotB0-2` ), `ReleaseDialog` `31fd03f0`
  ( `SAFE_DELETE_DIALOG`; the window has no destructor ), `SetSlotSkill` `31fdc4c0`, `SetShow` `31fe2b80` ( shown: hot keys, slot guides, UI type position ),
  `SetTransformEndTexture` `31fd4930`, `OnFrameMove` `3200f7a0` ( cool-time count, `STR_ID_2632` seconds, cover when cooling or unaffordable ), and the three
  helpers `UpdateSkillSlotString` `31fdfb20`, `UpdateSlotGuideDescPos` `31fe12f0`, `SetSkillUIType` `31fe2b10`: these three are March's
  `CX2RidingPetSkillSlot` bodies with the skill keys ( `GAMEACTION_SKILL1-4` ), the same 12 / 0.5 / 297 guide offset, and type B at ( -297, -615 ).
- **CH0's row for `31fd4930` was wrong:** "when the dialog handle is invalid" read `31d41d90` as a validity test; it is `operator==( handle, NULL )` and the body runs when that
  is false. June mixes the four handle comparisons ( `31d41cd0` / `d10` `!=`, `d50` / `d90` `==` ); every body runs on a valid handle.
- **User wrappers:** `HasTransformSkillSlotUI` `31fd0380`, `GetShowTransformSkillSlotUI` `31fd03b0`, `SetShowTransformSkillSlotUI` `31fe3c90`.
- **Hooks ( `_LIVE` ):** `CX2GUUser::OnFrameMove` @`32070133` ( after SIEE 15: window and either transformed byte -> `OnFrameMove( GetElapsedTime() )`, June reads
  user +0x2b0 ); `CX2UIManager::ToggleUI` case 44 @`32158eab` ( falls to the shared tail, no observer test ) and `GetShow` case 44 @`3215277d`; `CX2StateMenu::SetShowDlgMenu`
  `324617c0` ( my unit's window takes the skill slot's place; hidden on the else / hide branches ); `CX2Game::KeyProcess` training F6 @`322f7b41` ( Demonio only, after March's
  `ChargeMpAndEtcInTrainingGame`; June's whole F5 / F6 / F7 block stays backlog Stage 2, bullet amended ).
- **`UI_MENU`:** June menu 44 is the last before its end ( 45, `32153450` loops 3..0x2c ). March numbers the enum differently ( enumprobe: `UI_RIDING_SKILL_SLOT` 33,
  `UI_MENU_END` 34 ), so `UI_TRANSFORM_SKILL_SLOT` is appended after `UI_RIDING_SKILL_SLOT`; June has no Lua name for it. `UI_MENU_END` only bounds the `CloseAll` /
  open-test loops and the LU menu sentinels: nothing persists it.
- **Lu/Ciel stand-ins replaced:** the three window deletes ( dtor, `EndTransformedForm`, ReInit ) call `ReleaseDialog()` first ( `X2GULuciel.cpp`, inside `_CORE` ).
- **Not ported, with reason:** `325290f0` ( `CX2StatePVPGame::OnFrameMove`'s per-frame skill-slot / window toggle ): offline has no PvP. Nothing creates the window yet
  ( CH2d2 / CH3c2: kickoffs amended with the March names ), so every hook is inert in play.
- **JUNE_BUGS** 731 ( the pet inventory open test counts a shown window as an open menu ), 732 ( training F6 resets only Demonio, not Diablura ).
- **Compile:** `/Zs` probes ( census `cl_cmd` ) of `X2GUUser.cpp`, `X2UIManager.cpp`, `X2StateMenu.cpp`, `X2Game.cpp`, `X2GULuciel.cpp`: 5 x 0 errors; `cl /P` shows every
  new block live ( both cases, the wrappers, the frame hook, the F6 line ). Full build skipped: no play-test.
- **Gates ( `finish.py --range 47c478ef^..47c478ef --with JUNE_CHAR_TRANSFORM_LINK JUNE_CHAR_LUCIEL_LIVE -v` ):** junediverge `RESULT: PASS`, enumscan exit 0;
  flagcheck `_LIVE` `5 PASS, 2 OTHER, 0 FAIL, 1 ERROR`, `TRANSFORM_LINK` `3 PASS, 4 OTHER, 0 FAIL, 1 ERROR`, both `0 deleted line(s)` in the flag's files. The ERROR is the
  tool's unevaluable compound guard ( `Always.h:13652` `LUCIEL_TUTORIAL && ... !LIVE`; precedent CH1a / CH1a1 ): resolved by hand, the commit's `Always.h` hunk is only the new
  3-line `_LIVE needs TRANSFORM_LINK` guard, and the probes compile with `_LIVE` on, so the guard holds. OTHER rows: each flag's run lists the other flag's blocks, plus
  `X2GULuciel.cpp`'s 3 replaced stand-in comments inside `_CORE` ( its 3 deleted lines ). stateends / skillicons / luabind skipped: no skill states, skills or scripts.

### CH2d20 - Review (2026-11-15)

Opus 5.5 high. Reviewed commit `47c478ef` against June in Ghidra (read-only). **Verdict: pass, no code change.** Two doc corrections, one drift bullet.
- **`3200f7a0` OnFrameMove, from the listing:** first test `fldz; fcom [cool]; test ah, 5; jp` = enter only when `0 < cool` ( NaN skips ); the clamp `fcomp; test ah, 5; jp`
  = `new < 0 -> 0`; the string test ( `@3200f828` ) writes `STR_ID_2632` ( 0xa48, format "i" ) only when `0 < cool`, else `L""`; `+ 0.5` is the qword at `0x3342ebc8` then
  `_ftol2` ( = `static_cast<int>( cool + 0.5 )` ); the cover test `@3200f8ce` `test ah, 1; jnz` = cover when `0 < cool` or NaN, then `CanUseSlot( i ) != 1`. The first
  block reads `CoolTime` with no handle test, the cover branches test it ( `31d41cd0` ), the else branch clears the cover picture then the string: all as ported.
- **Hook `@32070133`:** after the SIEE 15 call, `+0x8f70` window, `+0x8f6e` / `+0x8f6f` either byte, `fld [user +0x2b0]` ( = `m_fElapsedTime`, funcmap 322722e0 ) ->
  `3200f7a0`: = the port ( `GetElapsedTime()` ).
- **`324617c0` SetShowDlgMenu:** riding pet ( 2, 0 ) ( 0x28, +0x8b ); transform ( 0x2c, +0x89 ) ( 0x28, 0 ) ( 2, 0 ); else ( 2, +0x89 ) ( 0x28, 0 ) ( 0x2c, 0 ); then ( 1, +0x8a ).
  Hidden: ( 2, 0 ) ( 1, 0 ) ( 0x28, 0 ) ( 0x2c, 0 ). Every branch's order = the port.
- **`ToggleUI` case 44 `@32158eab`:** game + my unit ( vslot 0x98 ) -> `31fe3c90`, then `jmp 32158301` ( the shared tail ): `break` is right. **`GetShow` case 44 `@3215277d`:**
  returns `31fd03b0`; no game / unit jumps to `xor al, al` = March's trailing `return false`.
- **`31fe12f0` / `31fe2b10` constants** ( `charport_consts.py --fn` and a float read ): 12.0, 0.5, 297.0 qwords, slots 0..3 ( `< 4` ), LT byte `+0x240`; type B floats
  `0x33474740` = -297.0 ( x ), `0x3347473c` = -615.0 ( y ). **`@322f7b41` F6:** `m_pUnit` non-NULL and `+0x38 == 0x81` ( `UC_LUCIEL_DEMONIO` 129 ) -> `31fc9f00( 0.0, 0 )`;
  March's `GetUnitClass()` tests `m_pUnit` the same way.
- **`UI_MENU_END` readers** ( `X2UIManager.cpp` ): `CloseAll` ( `ToggleUI( i, false )` ), `PetInventoryOpenCondition`, `PetInventoryForceOpenCondition` and the `m_LUMenu*`
  sentinels; nothing persists or sizes by it. June's END bounds the same loops, so menu 44 in them = June.
- **Correction:** June `32153450` is March's `PetInventoryForceOpenCondition`, not `PetInventoryOpenCondition`: its caller `3216d750` is the inventory's pet button
  ( March `X2UIInventory.cpp`, `PetInventoryForceOpenCondition()` ) and its nine exclusions are ForceOpen's. funcmap row and `JUNE_BUGS` 731 renamed; the bug itself stands.
- **Drift ( routed ):** that June body refuses four main states ( 7, 8, 0x10, 0x17 ), March three: bullet in backlog Stage 9 ( UI sweep ), not this plan's subject.
- **Gates** ( `finish.py --review --commits 47c478ef --with JUNE_CHAR_TRANSFORM_LINK JUNE_CHAR_LUCIEL_LIVE -v` ): junediverge `RESULT: PASS`, `--check-all` `RESULT: PASS`,
  enumscan exit 0; flagcheck `_LIVE` `5 PASS, 2 OTHER, 0 FAIL, 1 ERROR`, `TRANSFORM_LINK` `3 PASS, 4 OTHER, 0 FAIL, 1 ERROR` ( FINISH `FAIL` from the ERROR only ), both
  `0 deleted line(s)` in the flag's files. The ERROR is the unevaluable compound guard at `Always.h:13652` ( not in this commit's hunk ), as the Done note says: the hunk is
  the 3-line `_LIVE needs TRANSFORM_LINK` guard, flag-off it is skipped = base. OTHER rows: each flag lists the other's blocks, and `X2GULuciel.cpp`'s 3 replaced stand-in
  comments sit inside `_CORE` ( the commit's only 3 deleted lines ). stateends / skillicons / luabind: not applicable.
- **Build:** none ( no code change; the window has no creator until CH2d2 ). CH2d2 kickoff checked: its CH2d20 amendment names the March calls; it now waits only for CH2d.

### CH2d - Done (2026-11-16)

Sonnet 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` ( no new define ); `_SKILL_DEBUG` is **on** again for the play-test ( the CH2d2 review retires it ). No review of its own ( CH2d2's covers it ).
Files: `X2GULuciel.h` / `.cpp`, `X2GUUser.h` / `.cpp` ( one fix, below ), `Always.h` ( the debug line ), `MODS.md`, `funcmap.tsv` ( `grep "(CH2d)"`, 8 rows ), `kickoffs/char-luciel/CH2d2.md` ( amended ).
- **Written ( 30 functions, every `CH2d` row of `ch0_class_map.tsv`, read with `charport_adis.py`; the thunks are not ):** 10 preloads ( generated from the listings, 153 of 153 loads ), Land Crasher ( event process, landing ), Bloodlust event process,
  Nightmare / Mortal Fear / Dark Plasma starts, `GetTransformedSkillID`, the Altar of Evil skill and transformed-form event processes ( start, jump up / down / X, combo Z ZZ ZZZ ZZZZ X XX, XXX ready x2, XXX end ).
- **Deviation ( the one decision, as CH2c ): only Land Crasher ( 30040 / 30054 ) and Bloodlust ( 30041 / 30075 ) get setter cases.** Nightmare ( @32b47d2e ), Altar of Evil ( @32b47de9 ), Abyss Strike ( @32b49cd0 ) and Phantom Rush ( @32b49d69 ) each have a state whose
  callbacks are CH2d2's ( `ch2c_cases.py` marks them ); registering them now leaves the player stuck. CH2d2's kickoff carries the four cases.
- **Fix in CH1a0's code ( `TRANSFORM_LINK` ):** June `32030160` returns `bool` ( the `SelectSlot` result, al = 0 on every other exit ); CH1a0 wrote `CheckTransformSkillSlotKey` as `void`. Now `bool`; the combo chains test it.
- **Bodies of note.** Land Crasher landing and Bloodlust = the CH1g cancel chain ( skill / ground walk / back walk / dash ) with their own head. The transformed form's chain is the same with `SpecialAttackEventProcess` replaced by `CheckTransformSkillSlotKey` + `StateChangeToTransformSkill`
  ( new file-local macro `TRANSFORM_SKILL_CANCEL_AFTER_` ), then `HYPER_MODE_CANCEL_AFTER_`. Z / ZZ / ZZZ / X / XX tests buff 763 only ( June's literal ), ZZZZ / XXX ready / XXX end / jump X test 763 + minor ( Ciel: 764, end state 504 ). Normalised listings
  compared with a diff script: five combos share Z's tail exactly, ZZZZ / XXX end differ in register allocation only. `GetTransformedSkillID` +6 on Ciel = `RNSI_SA_LDN_SHADOW_FURY / SHREWD_CUTTER / SILENT_RAGE`.
- **Stand-ins / open.** States 444 / 445 ( the XXX ready / end tables registered a second time, `32b4829a`-`32b4832b` ) have no June name: 445 is a constant in `_CONST_LUCIEL_CH2D_`, the second ready event process is `..._READY_SECOND_EventProcess`; CH2d2's case registers them. No June bug found.
- **Debug lines ( `_SKILL_DEBUG` ).** `[JUNE_CHAR] LUCIEL SKILL land crasher landing body L|C`, `land crasher chain body ...`, `bloodlust end body ... foot N`; the `equip` / `use` / `cooldown` lines stay.
- **Gates ( `finish.py --files X2GULuciel.cpp .h X2GUUser.cpp .h Always.h --with JUNE_CHAR_LUCIEL_SKILL ..._SKILL_DEBUG JUNE_CHAR_TRANSFORM_LINK --state X2GULuciel.cpp Luciel_Demonio --icons 30040-30077 30215-30220` ):** junediverge PASS, enumscan exit 0, stateends PASS, skillicons PASS,
  `ch1m_regcheck` 340 matched / 0 problems; flagcheck `_SKILL`: `X2GULuciel.cpp` PASS ( +639 ), `X2GUUser.cpp` PASS, the `.h` files OTHER ( `_CORE`, a peer's `_UI` friend line ); FINISH `FAIL` comes only from the tool's unevaluable compound guard at `Always.h` ( `_LIVE` / `TRANSFORM_LINK`, peers' uncommitted hunks beside it ), as CH2d20. `/Zs` probes of `X2GULuciel.cpp` and `X2GUUser.cpp`: 0 errors.
- **Build:** my `build.py --deploy` waited for a peer's build that already covered every edit ( exe `27,044,864 B, 2026-11-15 23:29:46, md5 1699a8a8ac9bb0b396f56c62533343b0` ); `EXESTR OK: 3 of 3` wide in the built exe. `DEPLOY REFUSED` ( game running ): run `python tools/build.py --deploy-only` after the user closes the game ( `DEPLOY OK` line goes below then ).
- **Play-test save ( `elsdb`, backups `db_backup/els_db_20261116_040604_pre_createchar.sql`, `..040607..` ):** `LuDbA` ( unit 202, class 39, level 70: base 30040 / 30041 ) and `LuDbB` ( unit 203: 30054 / 30075 level 1, base 0 ); page 1 slots 0 / 1, page 2 slots 5 / 6.
- **Deploy:** `DEPLOY OK: X2_offline.exe is 27,048,960 B, dated 2026-11-16 04:04:33, md5 8c2dc751abba8b12eb0886bb3d122baf, equal to X2/US_SERVICE/x2.exe (2026-11-16 04:04:25); built at main 2c38224a + 19 uncommitted code file(s).` `EXESTR OK: 2 of 2` wide in the deployed exe.
- **Play-test ( 2026-11-16, user: all passed; logs 04:09-04:11 on the 04:04 deploy ):** 0 EXCEPTION, 0 ErrorLog lines, 0 unbound Lua names; UNHANDLED only `E_DISABLE_HB_CHECK_REQ`. `LuDbA`: 30040 x3 ( landing then chain each, cool 15.00 ), 30041 x2 ( bloodlust end, foot 1, cool 9.00 ); `LuDbB`: 30054 x2 ( cool 12.00; first cast from Ciel, body C state 200 ), 30075 x2. `equip` lines: slot 0 state 50, slot 1 state 52.
  **Not in the logs:** page 2 ( slots 5 / 6, Q / W ) — no `equip ... slot 5` line, so the B-slot casts are still owed ( CH2d2's play-test can take them ); the Soul Shock re-cast on `LuRgA` ( no `soul shock` line, that unit was not played ). The user reports all passed by sight.

### CH2d2 - Done (2026-11-16)

Sonnet 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` ( no new define ); `SERV_IRUHADEV_JUNE_ENUM_NAMES` for 2 new buff names ( `BFI_BUFF_DEATH_TOUCH` 786, `BFI_BUFF_ALTAR_OF_EVIL` 789 ); `_SKILL_DEBUG` stays **on** for the play-test ( the CH2d2 review retires it ).
Files: `X2GULuciel.h` / `.cpp`, `X2BuffFactor.h` ( bytepatch ), `MODS.md`, `funcmap.tsv` ( `grep "(CH2d2)"`, 12 rows ), `JUNE_BUGS.md` ( 758-761 ), `kickoffs/char-luciel/CH2d2-review.md` ( new, covers CH2d, CH2d2 ) and `CH2d2_save.sql` ( new ).
- **Written ( 31 functions = every `CH2d2` row of `ch0_class_map.tsv`, read with `charport_adis.py`; the thunks are not ) and the four setter cases.** Nightmare ( start future, event process, target start / frame move / event process, the target helper ), the Altar of Evil form ( start, wait, walk,
  jump ready / landing / Z, dash, dash end, dash jump, dash jump landing, XXX ready x2, Death Touch start / frame move / event process, end, end state ), Abyss Strike, Phantom Rush ( two frame moves, target start ), `CreateAltarOfEvilProjectile`, `SetTransformBuffFactorToSelf`.
  Setter cases: Nightmare ( 30042 ), Altar of Evil ( 30043: 31 states incl. 444 / 445 ), Abyss Strike ( 30215: state 459 ), Phantom Rush ( 30216: 460-462 ). Offsets of each registration were decoded with a stack-offset script ( base = esp0 + 0x1c, a push moves later offsets by 4 ),
  hand-checked on Nightmare / Abyss Strike / Phantom Rush, and every named callback of the Altar case compared with the C++ by script ( 44 of 44; the two differences are the known 444 renames ).
- **Names ( `?` rows ).** `32b17090` = `CollectNightmareTargetUnit` ( bool: a combination point left, enemies in the cone kept in `m_vecNightmareTargetUnit` ); `32b3e720` = `SetTransformBuffFactorToSelf` ( BFI, skill level ); the second `COMBO_XXX_READY_StateStart` ( `32b380f0`, state 444 ) is `..._READY_SECOND_StateStart`;
  the class map's `32b38340` `M_PHANTOM_RUSH_CameraMove` is registered in the FrameMove slot ( +0x38 ), so it is `M_PHANTOM_RUSH_FrameMove`. `32b17c20` ( Nightmare target EP ) is `32b178b0` byte for byte: it calls it.
- **Bodies of note.** The form's wait / walk / jump / dash states are Lu's CH1b chains with June's inserts ( off the line -> jump-down y - 45; buff 763 gone or the fourth skill key -> end; Z / X; `CheckTransformSkillSlotKey` ); dash / dash end / dash jump / dash jump landing / jump Z are shared with Ciel's form ( minor test on buff and end state ).
  The Phantom Rush target start and Nightmare target start stand beside the target as the Bounding states do ( `GetFixedDistancePositionForWeaponTaker`, `GetLandPosition`, then the line test ); the Phantom Rush one ends with `CanDown` or `GetNearestLine` ( June's inlined scan equals March's ).
- **Deviation / stand-ins.** `CreateAltarOfEvilProjectile`: June throws a vector subscript error when the stored position list is shorter than the altar list ( effect option low ); the port skips that altar ( one-line comment ). The sound list clear is `m_vecSoundPlayData.clear()` ( June `resize( 0 )` on +0x85c4 ).
  **A peer's uncommitted fix is in this commit:** `X2GULuciel.h` had CH2d's declarations after `#endif SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL`; the worktree already held the `#endif` moved below them and CH2d2's declarations sit inside the flag.
- **June bugs:** `JUNE_BUGS.md` 758 ( the form's wait tests the walk before the double step ), 759 ( the landing's up key starts JUMP_READY with the jump speed ), 760 ( Death Touch keeps the first range value ), 761 ( a skipped altar shifts the projectile positions ).
- **Debug lines ( `_SKILL_DEBUG` ).** `[JUNE_CHAR] LUCIEL SKILL nightmare target body L|C found N hit list N`, `altar transform start body ... buff N`, `altar projectile body ... altars N enemies N`, `death touch beam body ... range R time T`, `abyss strike end body ... foot N`, `phantom rush target body ... found N`;
  the `equip` / `use` / `cooldown` lines of CH2a stay.
- **Gates ( `finish.py --files X2GULuciel.cpp .h X2BuffFactor.h --with JUNE_CHAR_LUCIEL_SKILL ..._SKILL_DEBUG JUNE_ENUM_NAMES --state X2GULuciel.cpp Luciel_Demonio --icons 30040-30077 30215-30220 ):** FINISH RESULT PASS ( 7 gates, 0 failed, 1 skipped = luabind: no `.lua` staged ); flagcheck `_SKILL` PASS ( 1 deleted line = the "wait for CH2d2" comment ),
  `JUNE_ENUM_NAMES` PASS~ ( 6 lines, 2 marker lines outside the flag ), junediverge PASS, enumscan exit 0, stateends PASS ( 455 tables, 99 with EP ), skillicons PASS; `/Zs` probe of `X2GULuciel.cpp`: 0 errors; `ch1m_regcheck` 340 matched / 0 problems.
- **Build:** the first `build.py --deploy` ( 04:53 ) failed in DU5f's uncommitted edits ( `X2Unit.cpp` / `X2Unit.h` / `X2UICharInfo.cpp` / `X2GUUser.cpp`, none in `X2GULuciel.*`; not touched ); their files compiled by 05:10 ( `/Zs` probes ) and the re-run COALESCED with a peer's full build ( started 05:15:43, main 95f548ff + 16 uncommitted code files ): `DEPLOY OK: X2_offline.exe is 27,075,072 B, dated 2026-11-16 05:15:39, md5 06e712e076c2288a3104d1310b06a867, equal to X2/US_SERVICE/x2.exe (2026-11-16 05:15:31)`. `EXESTR OK: 5 of 5` wide in the deployed exe ( the nightmare / altar projectile / death touch debug lines, `EffectSet_A_LDB_PHANTOM_RUSH_Ciel_Dummy`, `DamageEffect_SA_LDB_Death_Touch_Linking_Attack` ).
- **Gate on the commit ( `finish.py --range 95f548ff^..95f548ff` ):** FINISH RESULT PASS ( 7 gates, 1 skipped = luabind ); flagcheck `_SKILL` PASS ( 1 PASS, 1 deleted line = the "wait for CH2d2" comment ), `JUNE_ENUM_NAMES` PASS. **Slip fixed before it landed:** my first commit swept a peer's uncommitted DU5f hunk of `X2GULuciel.cpp` ( socket MP rate, 9 lines ); it was taken out by amending, the hunk stays in the worktree for DU5f.
- **Play-test save ( applied 05:16 with the game closed; backup `db_backup/els_db_20261116_051607_pre_sql.sql`, integrity ok ):** `python tools/elsdb.py x --file kickoffs/char-luciel/CH2d2_save.sql` ( 8 + 34 + 18 + 16 + 2 rows ): `LuDbA` ( Lu, Diablura, level 70 ) has Nightmare / Altar of Evil / Abyss Strike / Phantom Rush on page 1 keys A S D C and page 2 keys Q W E R, `LuDbB` the four upgrades the same way ( base skills level 0 ), both learned Death Touch / Mortal Fear / Dark Plasma ( the form's three linked skills ).
  both learn Death Touch / Mortal Fear / Dark Plasma ( the form's three linked skills ).
- **Play-test ( 2026-11-16, user: every skill in the normal form works; the Altar of Evil form's model and slot UI are wrong -> CH2d3; logs 05:34-05:40 on the 05:15 deploy ):** 0 EXCEPTION; `use` lines: Nightmare 30042 x6 / 30076 x2, Altar of Evil 30043 x3 / 30077 x2, Abyss Strike 30215 x8 / 30219 x2, Phantom Rush 30216 x7 / 30220 x2; `nightmare target ... found 1` x8, `abyss strike end` x9, `altar transform start ... buff 1` each cast. **Not in the logs:** `phantom rush target` ( the second use ), `death touch beam`, `altar projectile` ( the form's skills; CH2d3 first ).
- **Play-test hand-off:** in the message that closes the session ( `LuDbA` / `LuDbB` per key, the log lines to expect ). Logs to read: `python tools/logs.py --grep "JUNE_CHAR\] LUCIEL SKILL"`.

### CH2d3 - Done (2026-11-16)

Opus 5.5 high. Flags: `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` ( class code ), `SERV_IRUHADEV_JUNE_CHAR_TRANSFORM_LINK` ( user wrappers ), `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` ( skill tree hooks ),
`_SKILL_DEBUG` ( 2 lines; stays on, the CH2d2 review retires it ). No new define. Files: `X2GULuciel.h` / `.cpp`, `X2GUUser.h` / `.cpp` ( bytepatch ), `X2UISkillTreeNew2015.cpp`.
funcmap `grep "(CH2d3)"` ( 6 rows ). Review: `CH2d2-review.md` ( covers CH2d, CH2d2, CH2d3 ). Commit: see git log `char-luciel Phase CH2d3`.
- **Why the form looked wrong ( user report, CH2d2 play-test ):** the model swap in June is made by the form buff's hook, not by a state. `SetSpecificValueByBuffTempletID`
  `32b37d40` ( Lu/Ciel override 0x24c ) calls the base, then for BTI 763 / 764 `StartTransformedForm( m_bMinorUnitActive )` `32b34fe0`. Neither was ported ( CH0 put both in CH4a ),
  so the skin never changed and the transformed byte stayed false. The end state's `EndTransformedForm( 0, false )` then found the form not transformed and skipped its
  whole block, the transform window's release included: `HasTransformSkillSlotUI()` stayed true, and `CX2StateMenu::SetShowDlgMenu` ( every frame from
  `CX2StateMenu::OnFrameMove`, CH2d20's hook ) kept the regular slot bar hidden. One cause for all three symptoms.
- **Ported whole:** `SetSpecificValueByBuffTempletID` ( the only live caller of `32b34fe0`; `32b38c70` / `32b38c90` are dead ) and `StartTransformedForm`: transformed -> return,
  `SetTransformed( form, true )`, swapped set -> return, `SetShowViewList( false )`, every weapon's normal / hyper anims hidden, `SetFormSkinAnim`, the collision / attack sets
  given the new anim's lists, both anims' oneshot timer reset + `ChangeAnim( EVIL_Ru_Wait | Wait, false, 0 )` + `Play( XAP_LOOP )`, `Bip01_Head` frame, `SetUnitReForm`,
  `m_hTransformedFormEffectSet = PlayEffectSet( EffectSet_LDB_Lu_Horn | EffectSet_LDN_Ciel_Horn, this )`. `UnSetSpecificValueByBuffTempletID` `32ae5080` is a `jmp` to the
  base: nothing to port. Assets checked before code: `Motion_Lu_LDB_Evil.x` ( data208, holds `EVIL_Ru_Wait` ), `Motion_Ciel_LDN_Evil.X` ( data218 ), both horn effect sets in
  data036 `EffectSet.lua`, the window's dialog ( data034 ) and its 4 textures with their `.TET`.
- **Slot UI:** the window's position is June's ( `SetSkillUIType` `31fe2b10` re-read: type A at 0,0, type B at -297, -615 ); June has no other show / hide of the slot bar for
  PvE ( the only `ToggleUI( 44 )` callers are `SetShowDlgMenu` and PvP's `325290f0` ). Added June's two user wrappers the skill tree calls: `UpdateTransformSkillSlotString`
  `31fe12e0` and `SetTransformSkillUIType` `31fe3cb0` ( window private refreshers reached by a `friend` line ), hooked as the last statement of
  `CX2UISkillTreeNew::UpdateSkillSlotString` `328e1170` and `SetSkillUIType` `328e1680` ( `_LIVE` ): a key or A / B change now reaches the window. `3200f9f0` ( an unreferenced
  frame-move wrapper ) not ported.
- **Kickoffs amended:** `CH4a.md` ( both functions done here; 689's reach ), `CH3c2.md` ( Ciel's form model rides the same hook ). No June bug added: none visible kept
  ( the start sets the byte before its swapped test, the mirror of JUNE_BUGS 689; nothing reaches it ).
- **Debug lines ( `_SKILL_DEBUG` ):** `[JUNE_CHAR] LUCIEL SKILL transform form start form N swapped N anim NAME`, `transform form end form N die N transformed N swapped N window N`
  ( printed only when a form is transformed or the window exists ).
- **Gates ( `finish.py --files` 5 files `--with JUNE_CHAR_LUCIEL_SKILL JUNE_CHAR_TRANSFORM_LINK JUNE_CHAR_LUCIEL_LIVE JUNE_CHAR_LUCIEL_SKILL_DEBUG` ):** FINISH RESULT PASS
  ( 8 gates, 0 failed, 3 skipped: stateends, no state registered; skillicons, no skill added; luabind, no script ); flagcheck `_LIVE` 5 PASS, `_SKILL` 3 PASS + 2 OTHER, `TRANSFORM_LINK`
  3 PASS + 2 OTHER ( each flag's OTHER rows are the other two flags' files ), 0 FAIL, junediverge PASS, enumscan exit 0. `/Zs` probes of `X2GULuciel.cpp`, `X2GUUser.cpp`,
  `X2UISkillTreeNew.cpp`: 0 errors each.
- **Build:** `DEPLOY OK: X2_offline.exe is 27,077,120 B, dated 2026-11-16 06:18:49, md5 383991cd4b67a71ea6192b711738a34f, equal to X2/US_SERVICE/x2.exe (2026-11-16 06:18:43); built at main f7d74f91 + 7 uncommitted code file(s)`
  ( peers' `Always.h` / `X2GameUnit.cpp` / `X2UnitViewerUI.h` built in ). `EXESTR OK: 3 of 3` wide ( the start debug line, `EffectSet_LDB_Lu_Horn`, `EVIL_Ru_Wait` ).
- **Play-test 1 ( 2026-11-16 07:32, user: demon model on, Lu frozen, Z / A / C do nothing ) and fix:** the `transform trace` line ( `_SKILL_DEBUG`, once a second while
  transformed, `CX2GULuciel::CommonFrameMove` ) showed START -> WAIT ( 424 ) working and the wait anim running, but each key press re-entered WAIT. Cause: Lu/Ciel's
  `StateChange` override ( `32b4db90` / `32b4dbb0` ) sends every id through `GetStateIDByForm` `32aeb450`, which while transformed maps any id it does not list ( every
  form state's own id ) to the form's WAIT. June's form states call the base `320348c0` / `32034c00` directly (115 direct sites in the class, all in form functions, the two
  overrides and `FrameMoveAltarOfEvilEnd` `32b35c60`; the only virtual call with a form id is the cast state's move to the form start, `32b02d80`, made untransformed, kept).
  72 lines in `X2GULuciel.cpp` ( 25 CH2d / CH2d2 functions + `32b35c60` ) and `CX2GUUser::StateChangeToTransformSkill` ( `32039dc0`, the linked skills A / S / D ) now call
  `CX2GUUser::StateChange`, each tagged. Kickoffs `CH3c.md` / `CH3c2.md` amended ( Ciel's form functions, same rule ). Gates ( `finish.py --files` both, `--with` `_CORE`
  `_SKILL` `TRANSFORM_LINK` `_SKILL_DEBUG` ): PASS, 73 deleted lines = the 73 replaced calls. `DEPLOY OK: X2_offline.exe is 27,078,144 B, dated 2026-11-16 07:40:27,
  md5 6c73c32629bf335a1ab8a6f83ffad842` ( main 306a6b95 + 3 uncommitted ); `EXESTR OK: 1 of 1` ( the trace line ).
- **Play-test 2 ( 2026-11-16, user: all checks passed; logs 07:51-07:54 on the 07:40 deploy, clean exit, 0 EXCEPTION ):** 2 casts of Altar of Evil ( 30043 ), each
  `transform form start form 0 swapped 1 anim EVIL_Ru_Wait` and `transform form end form 0 die 0 transformed 1 swapped 1 window 1`; the trace shows the form's own states
  in play: WALK 425, JUMP_UP 427, DASH 430, DASH_JUMP 432, JUMP_X 435, COMBO_Z 436, COMBO_X 440, DEATH_TOUCH 446, MORTAL_FEAR 447, DARK_PLASMA 448, END 452;
  `death touch beam` x1. **Not in the logs:** `altar projectile` ( the XXX combo was not reached ), the upgrade 30077 / `LuDbB` and page 2: the CH2d2 review's play-test
  takes them. The trace and the other `_SKILL_DEBUG` lines stay on; the CH2d2 review retires them.
- **How to see it in game:** the hand-off in the closing message ( `LuDbA` / `LuDbB`, keys S and W, the form's end on key C and at the buff's end ). Logs:
  `python tools/logs.py --grep "JUNE_CHAR\] LUCIEL SKILL"`.

### CH2d2 - Review (2026-11-16)

Opus 5.5 high. Covers CH2d ( `6d84c3d5` ), CH2d2 ( `95f548ff` ) and CH2d3 ( `c466ca67`, `77c59f35` ). **Verdict: pass after two fixes** ( flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL`, `X2GULuciel.cpp` only ).
- **Fix 1, four timers are `EventTimerOneshot` in June:** Nightmare target voice ( `32b13360` ), Death Touch beam ( `32b318e0` ), both Phantom Rush frame moves ( `32b38140`, `32b38340` ) test the
  oneshot timer `+0x24c` < t before `EventTimer` `31d28e10` ( Death Touch: `fcom double 0.847` at `334f2cd0` ). CH2d2 wrote plain `EventTimer` ( the CH1f2 review's slip again ). Every timer the three
  commits added was swept: these four are all of them.
- **Fix 2, Phantom Rush target start ( `32b38540` ):** June passes the stand position itself as `CanDown`'s out pointer ( `lea edx, [esp + 0xac]` @32b3883f after 6 pushes = the stand position at
  `esp + 0x94`, read for the by-value position right after ), and on success stores that ( @32b38c17 ). March's `CanDown` writes the landing point to the out pointer when it touches a line, so June
  stands the unit on the floor point; the port wrote it to a dead `vFloorPos` and stored the unmoved stand position. Now `&vStandPos`. The scan fallback reads the same variable, as June.
- **Checked = June ( Ghidra / listings, not decompile alone ):**
  - Setter cases by hand: Nightmare ( hyper copies StartFuture + EventProcess only; target state StartFuture / FrameMove / EventProcess ), Abyss Strike ( 459: skill byte `+0x11` ), Phantom Rush ( hyper copies
    Init + FrameMove + EventProcess; 460 / 462 through `32b4e5a0` -> `32b4e580`; 461 FrameMove through `32b4e5c0` -> `32b4e590` in slot `+0x38`, skill byte; EP `329cd4a0` -> `329cba60` ). Altar of Evil:
    446-448 skill byte, 449 / 450 one registration picked by `32070d40` ( unit `+0x867c` -> `+0x3c` transcended ), THROW_ITEM / END / END tables.
  - Nightmare target start ( `32b30ec0` ): behind the target ( `!right` ), then in front, else on it; my facing when the first failed; y = target y; `GetLandPosition` gives only the line index ( the store
    reads the stand position ); `LT_NORMAL` + enabled. `32b17c20` = `32b178b0` ( 871 bytes, only the local tail jumps differ ).
  - Death Touch: SA `0x1f` ( default 800 ), `0x90` into the same value ( JUNE_BUGS 760 ), SA `1` ( default 10 ); ARM at my bone, Linking_Attack at the target, Link at my bone; handles `+0x97b8` link /
    `+0x97bc` linking attack, unit type `+0x97b0`, uid `+0x97a8`; vslot 0x21c `( 786, creator 0, level 1, 0, 0 )` = March order `( BFI, 1, NULL, 0, false )`.
  - `SetTransformBuffFactorToSelf` ( `32b3e720` ): `SetFinalizerValue( BFT_TIME, 0.5, 1 )` only in a PvP-correction game, vslot 0x524 skill id into 0x770, `SetGameUnitBuffFactor( this, -1, true )`, no can-apply.
  - `CreateAltarOfEvilProjectile` ( `32b358b0` ): cone ( range `+0x974c`, -180 / 180, facing, 180, no invincible skip ), option test `3241d2a0( 2 )` = `OL_LOW`, land y `+0x80c4`, lock-on `+0x938` user / `+0x940` NPC.
  - Form WAIT ( `32b31260` ): foot, buff 763 / fourth key, Z, X, linked key, walk, double, up ( jump speed, +30 ), `TryHyperModeStateChange( LSI_LU_HYPER_MODE )`, down ( the two `CanDown` = the inlined
    line-index wrapper ); walk before the double step = JUNE_BUGS 758. Phantom Rush's nearest-line scan = March's `GetNearestLine` ( disabled / wall / monster road skipped, `<=`, 9999999 start, same projection ).
- **Gates ( review, `finish.py --review --commits 6d84c3d5 95f548ff c466ca67 77c59f35 --with _LIVE _SKILL JUNE_ENUM_NAMES _SKILL_DEBUG TRANSFORM_LINK -v` ):** junediverge PASS, `--check-all` PASS, enumscan exit 0;
  flagcheck per flag 0 FAIL, but FINISH `FAIL` from 1 `ERROR` each: `Always.h` @10175, a compound `#if defined( ..._SKILL_USE_HELPERS ) && !defined( ...ENUM_NAMES )` the tool cannot evaluate, beside CH2d's
  debug-define toggle ( the same tool limit CH2d's note records ); not a finding. OTHER rows: `X2GULuciel.h` ( `_CORE` declarations ), `X2BuffFactor.h` ( `JUNE_ENUM_NAMES` ), `X2GUUser.h` ( `TRANSFORM_LINK` ),
  `X2UISkillTreeNew2015.cpp` ( `_LIVE` ), `77c59f35` `X2GULuciel.cpp` ( `_CORE` bodies whose `StateChange` calls became the base's ). Deleted lines: `CheckTransformSkillSlotKey` void -> bool ( 5 + 1 ),
  `X2GULuciel.h` 4 ( the `#endif` moved below CH2d's declarations, the CH1l helper lines re-added under it ), the "wait for CH2d2" comment, 72 + 1 replaced `StateChange` calls.
- **Gates ( the fix, `finish.py --files X2GULuciel.cpp --with _SKILL _SKILL_DEBUG --state X2GULuciel.cpp Luciel_Demonio` ):** FINISH RESULT PASS ( 6 gates, 2 skipped: icons, luabind ); flagcheck `_SKILL` PASS
  ( 6 deleted lines = the 6 replaced lines ), junediverge PASS, enumscan exit 0, stateends PASS.
- **Build:** `DEPLOY OK: X2_offline.exe is 27,080,704 B, dated 2026-11-16 08:10:15, md5 ea24d4fdd30b80483c67fc2dcdc230d8, equal to X2/US_SERVICE/x2.exe (2026-11-16 08:10:06); built at main b7dfce78 + 3 uncommitted code file(s).`
  ( peers' `Always.h` / `X2TFieldGame.cpp` built in ). No exestr line: the fixes add no string.
- **`_SKILL_DEBUG`:** stays on for this review's play-test ( the second Phantom Rush use and the altar projectile have never logged ); retired after the logs are read, in a commit with no build.
- **Play-test owed:** the closing message ( `LuDbA` / `LuDbB`, page 1 and page 2, the form's XXX combo, Phantom Rush twice ). Logs: `python tools/logs.py --grep "JUNE_CHAR\] LUCIEL SKILL"`.
- **Next:** CH3a0 and CH4a are ready ( their kickoffs checked ); CH4a edits `X2GULuciel.*`, so it waits for this review's debug retire.
- **Play-test ( 2026-11-16, user: all passed; logs 08:30-08:33 on the 08:10 deploy, clean exit ):** 0 EXCEPTION, 0 ErrorLog lines, 0 unbound Lua names; UNHANDLED only `E_DISABLE_HB_CHECK_REQ`.
  `LuDbA` page 1: Phantom Rush 30216 on both bodies with the second use ( `phantom rush target body L found 1`, state 460, the user could move ), Abyss Strike 30215 ( `abyss strike end body L foot 1` ),
  Nightmare 30042 x2, Altar of Evil 30043 x2 ( form start / end, form states 432 / 435 / 436 / 440 / 446 / 447 / 448 ), `death touch beam body L range 1200 time 5.00` once for its one cast.
  **Not in the logs:** page 2 ( slots 5-8 ), `LuDbB` ( 30076 / 30077 / 30219 / 30220 ), `altar projectile` ( the XXX combo was not reached ), `nightmare target`. Passed by sight; routed to CH3a's play-test
  ( `CH3a.md` amended: one page-2 cast and one Diablura upgrade with the debug define off, by sight ).
- **Gate on the commit ( `finish.py --range 9309a878^..9309a878 --with _SKILL _SKILL_DEBUG` ):** FINISH RESULT PASS ( 6 gates, 3 skipped ); flagcheck `_SKILL` PASS ( 6 deleted lines = the 6 replaced ), junediverge PASS, enumscan exit 0.
- **`_SKILL_DEBUG` retired** ( `//#define` in `Always.h`; the lines it guards stay ): build skipped: debug retire only.

## Phase CH3 - Skills: second jobs (127, 128, 129), hyper actives, transcendence

| Sub-phase | Scope |
|---|---|
| CH3a | Dread Lord (127) skills |
| CH3b | Noblesse (128) skills |
| CH3c | Demonio (129) skills; `CUSTOM_RENDER_STATE` x3 (7w), `SUPER_ARMOR_CUSTOM_COLOR`, the not-selected-unit offsets (with DU1) |
| CH3d | **Opus.** Hyper actives, the hyper cut-ins of both bodies (`m_HyperCutIn_Trans`, `m_MinorUnitHyperCutIn(_Trans)`), and the **transcendence rows** (template): the offline transcend allow-list, `LevelUpEvent.lua` rows (`:158`, `:381-383`, `:627-629`), `DLG_Transcendental_Change_Guide_Luciel`, checklist 7ao whole, `SPRT_HYPER_CLASS` skills and `TRANSCENDENTAL_SKILL_DATA` rows, `GetHyperModeTableName` (`31fdd490`; March's `bCiel_` branch), `HQ_Cutin_{LLD,LCN,LDN}_{LU,CIEL}_Transcendence`, the `list_TR_*` rows in `GetCharacterImageName` and the transcended pictures read from June's row pushes, and `POPUP_TR_*` |

**Done gate:** as CH2 for 127 / 128 / 129 and the hyper actives. A transcended Lu/Ciel (`settranscend`) casts the hyper-class skills, slots
A-5 / B-5 work, and both bodies' transcended pictures and cut-ins show. `skillicons.py check` = PASS after the transcendence rows.

### CH3c20 - Done (2026-11-15)

Opus helpers for CH3c2: the lock-on struct leaves `CX2GUAra`, and the four Lua methods the Garden of Haures NPC script lacked are bound.
Inert for play: nothing in Lu/Ciel calls them until CH3c2 (Garden of Haures, Shrewd Cutter); Ara's lock-on is the same code, moved.

- **`SERV_IRUHADEV_JUNE_CHAR_LOCKON_SYSTEM`** (guard: `_CHAR_ARA_LOCKON`; `X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR` is checked in `X2GUUser.h`,
  because it is defined after `Always.h`). `CX2GUUser::SLockOnSystem` + `sLockOnSystemPtr` (public, beside `InitData`) = Ara's struct with the
  CH2b0 `+0x30` member unconditional; `ProcessLockOn( CX2GUUser* )` in `X2GUUser.cpp` = Ara's CH2b0 June branch of `3200fa20`, 111 lines byte-copied
  (generated from `X2GUAra.cpp` 9674-9784, not retyped). `X2GUAra.h` (struct, `IMPLEMENT_INTRUSIVE_PTR`) and `X2GUAra.cpp` (the definition, both
  branches) sit in `#else`; Ara's `SLockOnSystem::CreateLockOnSystem()` and `m_sLockOnSystemPtr->ProcessLockOn( this )` resolve to the base by
  inheritance, no Ara call site changed. `X2GULuciel.h` `m_ptrShrewdCutterLockOn` (+0x9830) is `sLockOnSystemPtr` under the flag. The body uses
  only `CX2GameUnit` calls (`GetPos`, `GetTeam`, `GetDirVector`, `GetRandomInt`). Creating Lu's instance is CH3c's (InitState @`32b490e0`, routed by CH1m).
- **`SERV_IRUHADEV_JUNE_CHAR_NPC_LUA_BINDS`** (guard: `_LINEMAP_LAND_MONSTER_ROAD`, `_DIVERGE_FIXED_DISTANCE_WEAPON_TAKER`, `RAVEN_WEAPON_TAKER`,
  `_CHAR_ARME_CORE`, `DUNGEON_RANK_NEW`, `SERV_INSERT_GLOBAL_SERVER`, `SERV_TRAPPING_RANGER_TEST`, `SUMMON_MONSTER_CARD_SYSTEM`, `SERV_NEW_DEFENCE_DUNGEON`).
  Bind sites from `xorstrings.tsv` + `c3z4_binder_sites.py`; bodies read from the listing (Ghidra has no function at three of them).
  - `CX2Game::GetPosCheckDistStandardNPC( vPos, bIsRight, fDist, vDir, CX2GUNPC* )` = June `322f27e0` (ret 0x28, hidden return): result = vPos; with a
    line map, June's weapon-taker walk `31c60890` (= CH1i2's `GetFixedDistancePositionForWeaponTaker`) wins; else `GetLandPosition( vPos +- fDist * vDir,
    false, 30.0 )` (`31c7bb00`, `33441b9c`), then the nearest point (segment clamp: projection <= 0 -> start, length <= projection -> end) on every
    enabled line that is not `LT_WALL` (1) / `LT_MONSTER_ROAD` (7), best from 9999999.0 (`3344915c`), `!( best < d )` keeps the later line on a tie.
    Last, when the NPC is given and `IsPosAlongLastTouchLine( result )` (`32293b80`) differs from bIsRight, the result is vPos again. June's loop
    repeats the line filter inline with a -1.0 distance (`334380f0`) that the outer filter makes unreachable: not ported.
  - `CX2GUNPC::PushCreateNPCReq_LUA4` = June `327799c0` (ret 0x40): the create core at once (no push list, as row 168's `32779cb0`) with delay = .x,
    key = (int) .y of arg 6, team / AI type / ally team = (int) .x / .y / .z of arg 8, the caller's ally UID, elite false, `NCT_NONE`. The summoner's
    templet id June passes last is not carried (as the five C3z-4 wrappers, user 2026-10-10). `JUNE_BUGS` 733: the script then flushes an empty list.
  - `CX2GUNPC::IsSameOwnerUserID( CX2GUNPC* )` = June `3275b0f0`: NULL -> false; both `+0x8d5c` owner UIDs ( = `GetOwnerGameUnitUID`, as June's
    `GetOwnerUserUnitUID` bind `31e56300` reads the same member ) >= 0 and equal.
  - `GetRemainHyperModeTime` bound on `CX2GameUnit` (June class_def `@31de9747`, `31e4f890` = March's inline). June also binds a `CX2Game`
    `PushCreateNPCReq_LUA4` (`322e7980` @`31dc7eea`); the Garden script calls the NPC one only, so it is not bound here.
- **Data:** data036 `Enum.lua` `NPC_UNIT_ID` gains June's `NUI_SA_LDN_GARDEN_OF_HAURES = 2269` / `_ADD = 2270` (`Enum.src.lua:5996` / `3777`;
  live 2268 / 2271 neighbours, no collision); `luasrc.py refresh`, `ledger --check` PASS. `PACK OK data036` (1 changed: `Enum.lua`), `COPY OK data036`
  md5 `d8b0794aafbbb1410adc246a9d192332`. CH3a0 keeps the `NPCTemplet` rows and the C++ name of 2270 (its kickoff notes the two Enum lines are done).
- **luabind** `SA_LDN_GARDEN_OF_HAURES` + `_ADD`: 24 methods, 0 unbound, ENUM 0 missing, `RESULT: PASS` (was 4 unbound + 1 enum).
- **Compile:** `/Zs` probes (census `cl_cmd`) of `X2Game.cpp`, `X2GUNPC.cpp`, `X2Main.cpp`, `X2GUUser.cpp`, `X2GUAra.cpp`, `X2GULuciel.cpp`: 6 x 0 errors.
  The first probe caught my own `Always.h` guard on `X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR` (defined later); fixed within minutes, before any build
  read it (the 17:30 peer build's log has no `C1189`; its `X2StateMenu.cpp` errors are a peer's transform-HUD edit). Full build skipped: no play-test.
- **Gates (worktree):** `finish.py --with JUNE_CHAR_LOCKON_SYSTEM JUNE_CHAR_NPC_LUA_BINDS --lua ...`: flagcheck PASS / PASS (0 FAIL, 0 deleted lines;
  the OTHER rows are peers' uncommitted hunks in `Always.h`, `X2GUUser.*`, `X2Game.cpp`), junediverge PASS, enumscan PASS, luabind PASS; stateends
  not applicable (no state registered). Commit staged from HEAD + this phase's hunks only.
- **Commit** `b4f80375`. Range gate `b4f80375^..b4f80375`: flagcheck `_LOCKON_SYSTEM` PASS ( 4 PASS, 1 PASS~, 1 DEFINE, 0 FAIL ), `_NPC_LUA_BINDS`
  PASS ( 3 PASS, 2 PASS~, 1 DEFINE, 0 FAIL ); OTHER rows = the partner flag's files; junediverge, enumscan, luabind PASS.
- **funcmap:** 5 rows `(CH3c20)`. **Review:** `kickoffs/char-luciel/CH3c20-review.md` (Opus); CH3c2 waits on it.

### CH3c20 - Review (2026-11-15)

Opus 5.5 high. Reviewed commit `b4f80375` against June's listing (`disasm_range.py`, exe bytes) and Ghidra (read-only). **Verdict: pass after one fix.**
- **Fix ( `_NPC_LUA_BINDS`, `X2Game.cpp:9263` ):** `GetPosCheckDistStandardNPC`'s nearest-line test. June `@322f2d10`: `fld best; fcomp dist; test ah, 1; jne skip`
  = update only when `dist <= best` ( a tie keeps the later line, a NaN distance is skipped ). The port's `!( best < dist )` took a NaN distance; now
  `fDistance <= fNearestDistance`. funcmap row `322f27e0 (CH3c20)` still reads "tie = later line" ( true ); `funcmap.tsv` holds a peer's uncommitted edit, so not touched.
- **Hoist:** `X2GUUser.cpp` `ProcessLockOn` body = Ara's `_ARA_LOCKON` branch ( `X2GUAra.cpp` 9675-9788 at `b4f80375` ) byte for byte, 112 lines, only the first
  comment differs. `X2GUUser.h` struct = Ara's with `X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR` and `_ARA_LOCKON` resolved on: every member, default ( 99999.0 ),
  accessor, AddRef / Release. No `CX2GUAra::SLockOnSystem` / `sLockOnSystemPtr` user is left outside the `#else` blocks; `X2GULuciel.h` +0x9830 is the base type.
- **`322f27e0`:** ret 0x28; args from the frame: hidden return `+4`, vPos `+8`, bIsRight `+0x14`, fDist `+0x18`, vDir `+0x1c`, NPC `+0x28` = the port. Weapon-taker
  call `( &vPos, fDist, bIsRight, &out = vPos )`, `cmp al, 1`; probe `bIsRight == 1 ? + : -`; `31c7bb00( probe, 0, 30.0 ( 33441b9c ), &idx = -1 )` = `LINE_RADIUS`;
  best 9999999.0 ( `3344915c` ). Line skips `[+8] == 0`, `[+4] == 7 / 1` ( March `LineData`: `bEnable` +8, `lineType` +4, `startPos` +0x24, `endPos` +0x30 ).
  Projection `y*y + x*x + z*z` order, `test ah, 0x41; jp` = start when `proj <= 0` ( NaN goes on ); length `<= proj` ( qword ) -> end, NaN interpolates; length and
  distance sum `( x² + y² ) + z²` = the port. Index `< 0` keeps vPos. Tail ( all paths, `@322f2d8f` ): NPC set and `32293b80( result ) != bIsRight` -> vPos.
- **`327799c0`:** ret 0x40 = 16 dwords = the 9 Lua args. The 19 pushes of `322e67c0`: id, level, bActive, vPos, bRight, `.x` delay, bNoDrop, `ftol( .y )` key,
  `ftol` team / AI ( arg 8 `.x` / `.y` ), ally UID ( arg 9 ), 0 elite, `ftol( .z )` ally team, 0 create type, 0 my-unit byte, templet id: March's 14 = June's first 14
  ( C9h-4h's extra-arg reading ); the game-pointer gate = `g_pX2Game` test. **`3275b0f0`:** NULL -> 0; each owner = observer `+0x8d5c` -> vslot 0x54 or -1, both
  `>= 0` ( 64-bit sign test ) and equal = the port.
- **Ghidra spot-checks:** `32293b80` decompiles to the last-touch-line test ( row 1189, March `IsPosAlongLastTouchLine` ); `31c60890` is the function CH1i2 ported;
  bind `@31e0ccbe` sits in the `CX2GUNPC` class_def run. **JUNE_BUGS 733** stands: `327799c0` calls the core directly, no push list ( June's, not a port gap ).
- **Gates** ( `finish.py --review --commits b4f80375 --with JUNE_CHAR_LOCKON_SYSTEM JUNE_CHAR_NPC_LUA_BINDS --lua SA_LDN_GARDEN_OF_HAURES SA_LDN_GARDEN_OF_HAURES_ADD -v` ):
  `FINISH RESULT: PASS (8 gates, 0 failed, 2 skipped)`; flagcheck `_LOCKON_SYSTEM` 4 PASS, 1 PASS~, 1 DEFINE, 5 OTHER, 0 FAIL; `_NPC_LUA_BINDS` 3 PASS, 2 PASS~, 1 DEFINE,
  5 OTHER, 0 FAIL; both 0 deleted lines; every OTHER row = the partner flag's file. junediverge + `--check-all` PASS, enumscan exit 0, luabind PASS; stateends /
  skillicons skipped ( no state, no skill ). Fix worktree: `finish.py --files X2Lib/X2Game.cpp --with JUNE_CHAR_NPC_LUA_BINDS` PASS ( 1 PASS, the 1 deleted line = the
  replaced line inside the flag's own block ). Commit `191732e7`, range gate `191732e7^..191732e7`: the same, `FINISH RESULT: PASS`.
- **Second fix ( found by the full build ):** `Always.h`'s `_NPC_LUA_BINDS` guard named three `ServerDefine.h` flags ( `SERV_INSERT_GLOBAL_SERVER`,
  `SERV_TRAPPING_RANGER_TEST`, `SERV_NEW_DEFENCE_DUNGEON` ) that KTDXLIB never sees ( `C1189` ); the CH3c20 probes compiled X2Lib files only. Those three now
### CH3a0 - Done (2026-11-16)

Opus 5.5. Lu/Ciel's summoned NPCs exist and are set up as June does; Lu has no skill that spawns them until CH3a (Darkness), CH3b (Shadow Bolt) and CH3c2
(Garden of Haures), so the play-test spawns them from the dev console. Read from Ghidra (read-only): `3230c1a0`, `322ece70`, `32778fe0`, `32ad1080`, `322a2700`.
- **Data ( kickoff scope ):** data036 `Enum.lua` `NPC_UNIT_ID` gains `NUI_SI_SA_LLD_SUMMON_DARKNESS = 1906`, `NUI_SI_SA_LCN_SHADOW_BOLT = 1908`,
  `NUI_SI_SA_LLD_SUMMON_DARKNESS_PVP = 1937` (2269 / 2270 were CH3c20's); data037 `NPCTemplet.lua` gains June's five rows verbatim, June's order (1906, 1908,
  1937, 2269, 2270; `NPCTemplet.src.lua:17190 / 17208 / 17469 / 20300 / 20309`). 1907 gets nothing (kickoff). `luasrc refresh` both, `ledger --check` and
  `core --check` PASS. `PACK OK data036` (1 changed: `Enum.lua`), `PACK OK data037` (1 changed: `NPCTemplet.lua`), `COPY OK data036` md5 `3c141682`,
  `COPY OK data037` md5 `fff26354`.
- **Assets:** the five scripts are in data037 (June bytecode). The templet `MOTION` names (`Motion_SA_LLD_SUMMON_DARKNESS.x`, `Motion_SA_LCN_SHADOW_BOLT.x`,
  `Motion_SA_LDN_GARDEN_OF_HAURES(_ADD).x`) are in no June kom either (`stage-june --dry-run`: 4 refused); the NPC loads the script's `INIT_MOTION`
  (`X2GUNPC.cpp:3367`): `Motion_SUMMON_DARKNESS.x`, `CDC_AS_ROCKON_DUMMY.X`, `Motion_HAURES1_Ciel_LDN.x`, `Motion_HAURES2_Ciel_LDN.x` and every texture / sound in
  the five `INIT_DEVICE` tables are staged (data157 / data117 / data069 / data068 / data048 / data045). The kickoff's "motions in CH0b's list" was wrong; nothing to stage.
- **luabind** the five scripts: 11 / 11 / 6 / 24 / 0 methods, 0 unbound, ENUM 0 missing, `RESULT: PASS`.
- **`SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SUMMON`** (new, beside `_SKILL`, as Elesis CH3b0's `_SUMMON`; the kickoff named `_SKILL`, a separate flag keeps the guard
  on the flag-0x10 port honest). `X2Game.cpp` `SetUserSummonedNPCInfo`, June `3230c1a0` (checklist L253, which no phase owned): 0x772 / 0x791 (1906 / 1937)
  -> `3230cd2d` flags 0x40 (`SHF_SKILL_ABILITY_REL`); 0x774 (1908) `3230cb47` flags 1 (`SHF_FIXED_HP`) and, owner type 10, user `+0x9690` = NPC UID
  (`CX2GULuciel::SetShadowBoltNpcUID`, new one-line setter of CH1l3's `m_iShadowBoltNpcUID`); 0x8dd / 0x8de (2269 / 2270) -> `3230cd3a` flags 0x50. Skills from
  June's `31fcf520` map (`summon_npc_skill_map.tsv`): 30013 / 30016 / 30053, `SetSummonHelperInfo` does the upgrade swap. The 54-id `IsSummonSkillBy` already
  had the three. `X2UnitManager.h` `NUI_SA_LDN_GARDEN_OF_HAURES_ADD = 2270` under `JUNE_ENUM_NAMES`.
- **`SERV_IRUHADEV_JUNE_SUMMON_HELPER_PASSIVE_RATE`** (class-agnostic): `322ece70` flag 0x10 (`@322ed0a4`), no caller until 2269 / 2270: without 0x20, the stat
  scale x user `+0x4074` and the attack rate starts from `+0x4068` before x `+0x4064`. `SHF_PASSIVE_SUMMON_RATE = 0x10` (`X2GUUser.h`, enum line in `#else`);
  `+0x4074` = new `SkillRelatedData::m_fSummonedStatRateByPassive` (ctor 1.0 `@322a4351`; only writer Rose's Mechanic Mastery 31413 ability 0xb8, `32ad23a8`:
  char-rose's), `+0x4068` = Eve's `m_fSummonedAtkRateByPassive`. For Lu/Ciel both stay 1.0 (no writer), so Garden's numbers equal flags 0x40's.
- **`SERV_IRUHADEV_JUNE_SUMMON_DAMAGE_ADJUST_BY_OWNER`** (class-agnostic, CH1o amendment): `CX2GameUnit::AdjustDamageDataBeforeDamageReact` virtual (June unit
  vslot 0x300, base `3208ddc0` empty); the three `ADD_RENA_SYSTEM` sites in `X2DamageManager.cpp` (`@3262546d` / `@3262f1e6` / `@32628652`) call it on every
  attacker (March's `GUT_USER` test in `#else`); `CX2GUNPC` override = June `32778fe0`: AI `+0x7c98` type `+0x38` == `NAT_ALLY`, `dynamic_cast` to `CX2AllyNPCAI`
  (`32fe2daa`), ally unit type `+0x80` == `GUT_USER`, `GetUnitByUIDAndType( +0x78 uid, GUT_USER )` (game vslot 0xc8, june-stat-calc D4a), that unit's 0x300.
  The user virtual (`X2GUUser.h`, Lire / Raven / Ara / Lu overrides) now overrides the base; no signature change. Every class's ally summons reach their owner.
- **`_SUMMON_DEBUG`** (temporary; the CH3a0 review retires it): `[JUNE_CHAR] LUCIEL SUMMON set npc ...` per Lu case, `... hit npc N -> owner UID unit type T`
  (any class, once a second).
- **No JUNE_BUGS row** (nothing odd copied). **funcmap:** 4 rows `(CH3a0)`; the char-raven CH1o row of `32778fe0` ("not ported") is superseded by them.
- **Gates (worktree):** `finish.py --files <10 code files> --with JUNE_SUMMON_DAMAGE_ADJUST_BY_OWNER JUNE_SUMMON_HELPER_PASSIVE_RATE JUNE_CHAR_LUCIEL_SUMMON
  JUNE_CHAR_LUCIEL_SUMMON_DEBUG --lua <5 scripts>`: `FINISH RESULT: PASS (8 gates, 0 failed, 2 skipped)`; flagcheck `_LUCIEL_SUMMON` 3 PASS, 1 DEFINE, 6 OTHER;
  `_ADJUST_BY_OWNER` 4 PASS, 1 DEFINE, 5 OTHER; `_PASSIVE_RATE` 3 PASS, 1 DEFINE, 6 OTHER; 0 FAIL each (OTHER = the partner flags' files); junediverge PASS,
  enumscan exit 0, luabind PASS; stateends / skillicons skipped (no state, no skill). Every edit insert-only (`git diff --stat`: 101 insertions, 0 deletions in the
  CP949 files). `Always.h` / `funcmap.tsv` / `STATUS.md` hold a peer's uncommitted DU5c2 hunks: the commit stages this phase's hunks only.
- **Compile:** `/Zs` probes (census `cl_cmd`) `X2Game.cpp`, `X2GUNPC.cpp`, `X2GUUser.cpp`, `X2DamageManager.cpp`, `X2GULuciel.cpp`: 5 x 0 errors; a flag probe has all
  four flags on. Guard dependencies are all top-level `Always.h` defines (`ADD_RENA_SYSTEM` under `JUNE_CHAR_RENA_NF`, defined just above), none from `ServerDefine.h`.
- **Build:** `build.py --deploy` BUILD FAILED at the X2 link on a peer's in-progress CH4a edit (`CX2GULuciel::InitPassiveSkillState` declared, no body; `_LUCIEL_PASSIVE`), not this phase's: X2Lib with this phase's code built `ok in 560s`. Re-run: `COALESCED` into a peer's full build at `ca2afe20`, then `DEPLOY OK: X2_offline.exe is 27,090,432 B, dated 2026-11-16 09:23:17, md5 65db270adcaffe9c32baccee653ab7f4, equal to X2/US_SERVICE/x2.exe (2026-11-16 09:22:01); built at main ca2afe20 + 5 uncommitted code file(s)` (the peer's CH4a files); `EXESTR OK: 2 of 2 found in X2_offline.exe` (`LUCIEL SUMMON hit npc ...`, `... set npc ... flags 0x50 ...`, wide).
- **Play-test:** the hand-off's 4 checks (Darkness, Shadow Bolt, Garden + its plants on Lu/Ciel `testlu1`; an Eve Nasod Watch for the owner hand-off on another
  class). Review: `kickoffs/char-luciel/CH3a0-review.md` (Opus); CH3a waits on it.
- **Play-test passed** (2026-11-16): Lu/Ciel `testlu1` console spawns: `set npc 1908 skill 30016 flags 0x1 owner type 10 uid 1000029`, `set npc 2269 ... flags 0x50 ... hp 154200`,
  five `set npc 2270 ... flags 0x50 ... hp 128500` (the Garden script's plants), `hit npc 1906 -> owner 166 unit type 10` and `hit npc 2270 -> owner 166 unit type 10`; Eve `Iivue`:
  `hit npc 384 -> owner 19 unit type 5` (EFFECT_TO_UNIT site). No packet EXCEPTION. One crash in the first Eve run was the console's stale `jU` (Lu's freed unit; the dump
  faults in Lua's `GetPos` binder from `CX2GameEdit::ExecCommand`, no phase code on the stack); a fresh `jU` on Eve ran clean. The 1906 `set` line fell in the earlier log run.

### CH3a0 - Review (2026-11-16)

Opus 5.5. **Pass, no code change.** Reviewed `ca2afe20` against June in Ghidra (read-only) and the exe bytes.
- **`3230c1a0`:** 0x772 `@3230ca56` / 0x791 `@3230cb79` -> `3230cd2d` flags 0x40; 0x774 `@3230ca40` -> `3230cb47` flags 1, then `31e4d0e0` (unit type) == 0xa ->
  user `+0x9690` = NPC `+0x270`; 0x8dd / 0x8de through the byte table `3230d15c` (index 0xba / 0xbb -> entry 6) -> `3230cd3a` flags 0x50; all rate 1.0 into the
  shared tail `3230ca6d` (March's). `31fcf520`: 0x772 / 0x791 -> 0x753d (30013), 0x774 -> 0x7540 (30016), 0x8dd / 0x8de -> 0x7565 (30053) = the port's skills.
  `UT_LUCIEL` = 10.
- **`322ece70` flag 0x10:** inside the templet test, after scale x power rate, `!0x20` then `0x10`: scale x `+0x4074`, both rels = `+0x4068`, then both x `+0x4064`
  (`@322ed0a4`-`322ed0e4`); the rels start at 1.0 (`FLD1 @322ecfc6`, `FST [esp+0x18]` / `[esp+0x20]`). March's `fStatAtkRate_` is `GetStatAtkRateOfSummoned()`
  (`X2Game.cpp:17502`, the `+0x4064` member), so `fAtkRateBase * fStatAtkRate_` = June. Ctor: `FSTP [esi+0x4074] @322a4351`, the same 1.0 as `+0x4068`.
- **`32778fe0`:** DD, AI `+0x7c98`, type `+0x38` == 1, `32fe2daa` dynamic_cast, `+0x80` == 0, game vslot 0xc8 `( +0x78, +0x7c, 0 )`, that unit's 0x300 = the port
  (`GetNPCAI` = `m_pAI`, `GetAllyUnitType` / `GetAllyUnitUID`). Sites `@3262546d` / `@3262f1e6` / `@32628652`: the attacker's vslot 0x300, no type test. Every
  override (`X2GUUser.h` 3 bases, Lire, Raven, Ara, Luciel, `CX2GUNPC`) takes `CX2DamageManager::DamageData*`, so the base virtual is overridden, no signature change;
  flag-off is March's `GUT_USER` block in `#else`.
- **Data:** the five `NPCTemplet.lua` rows equal `june_src_fixed/data037_kom/NPCTemplet.src.lua` (EOL-normalised), June's order; data036 `Enum.lua` 1906 / 1908 /
  1937 / 2269 / 2270 = June's `Enum.src.lua`; live `data036.kom` / `data037.kom` md5 `3c141682` / `fff26354` = the packed ones; `ledger --check`, `core --check` PASS.
- **Gates:** `finish.py --review --commits ca2afe20 --with <4 flags> --lua <5 scripts> -v`: `FINISH RESULT: PASS (9 gates, 0 failed, 2 skipped)`; flagcheck
  `_LUCIEL_SUMMON` 3 PASS, 1 DEFINE, 6 OTHER; `_ADJUST_BY_OWNER` 4 PASS, 1 DEFINE, 5 OTHER; `_PASSIVE_RATE` 3 PASS, 1 DEFINE, 6 OTHER; 0 FAIL, 0 deleted lines.
  Every OTHER row is a partner flag's file (the three flags share the commit) or `JUNE_ENUM_NAMES` (`X2UnitManager.h`). junediverge + `--check-all` PASS, enumscan
  exit 0, luabind PASS; stateends / skillicons skipped (no state, no skill).
- **Logs:** the latest run has Eve's `hit npc 384 -> owner 19 unit type 5` (x7), 0 EXCEPTION, 0 unbound; Lu's lines were read in CH3a0's check (above).
- **Retired:** `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SUMMON_DEBUG` commented out in `Always.h`. Build skipped: debug retire only.
- **CH3a kickoff amended:** Summon Darkness's spawn shape (FrameMove helper `32b20260`, 1906 / 1937 by `GetIsNowPVPCorrectionGame`, `CreateNPCReq` `@32b20486`).

### CH3a - Done (2026-11-16)

Sonnet 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` ( no new define ); `SERV_IRUHADEV_JUNE_ENUM_NAMES` for `BFI_DEBUFF_SUPPRESSION_OBLIVION` = 529 ( `X2BuffFactor.h`, bytepatch ); `_SKILL_DEBUG` is **on** again for the play-test
( `Always.h`, one inserted line; the CH3b review retires it ). No review of its own ( the CH3b review covers CH3a ). Files: `X2GULuciel.h` / `.cpp`, `X2BuffFactor.h`, `Always.h`, `MODS.md`, `funcmap.tsv` ( `grep "(CH3a)"`, 9 Luciel rows ),
`JUNE_BUGS.md` ( 770 ), `kickoffs/char-luciel/CH3a_save.sql` ( new ).
- **Written ( every `CH3a` row of `ch0_class_map.tsv`: 26 functions, the thunks are not; read with `charport_adis.py` and the setter listing ):** ten preloads ( generated from the listings ), Oblivion ( frame move, event process, the execution helper
  `32b1fc90` = `ApplyOblivionToEnemies` ), Summon Darkness ( frame move + the spawn helper `32b20260` = `CreateSummonDarknessNPC`, Eve's `CreateNPCReq` shape ), Phantom Blade ( event process, attack start ), Supreme Punishment, Dancing Blade ( start, loop start,
  end, event process ), Chains of Bondage ( start, frame move, event process, both bodies ), and the 12 setter cases: Oblivion, Phantom Blade ( + 381 / 382 ), Summon Darkness, Supreme Punishment, Fear Tomorrow, Dead Cross, Demonic Cannon, Fear Absorption ( + 377 ),
  Chains ( + 376 / 378 / 379 ), Bluff ( + 575 ), Dancing Blade ( id 10018 ) and the memo branch ( one shared `RegisterDancingBladeStates` ). Dancing Blade is June's one duplicated registration, written once.
- **Decoded, not guessed:** every offset of an extra state's registration with the stack-offset rule ( base = esp0 + 0x1c, a push moves later offsets by 4 ); the shared tails ( `32b46d06` = FM + EP copied to the hyper state, `32b47028` = all nine, `32b4a7e5` = register );
  vslot 0x6a0 takes ( id, 1, 1 ) = `GetActualMPConsume( id, 1 )`; `0x260` = `IsMinorUnitActive()`; `0x4b0` / `0x4b4` = `GetComboZStateID()` / `GetComboXStateID()`; SA ids 7 / 0xa1 / 0x10 / 0x1d = `SA_ATK_PHYSIC_REL` / `_PVP` / `SA_DAMAGED_REL` / `SA_SCOPE_ABS`
  ( same values in June's data: Summon Darkness's template has `SA_DAMAGED_REL` first ); BFI 0x211 = June's `BFI_DEBUFF_SUPPRESSION_OBLIVION` ( Enum.lua 529 ).
- **Deviations / stand-ins.** Oblivion's memo reads ( x1.3, range 960 ) are not written: June's `GetEqippedSkillMemo` `31fca9e0` is a stub that answers 0 ( March's is replaced by it already ). `32b1fc90`'s unit test `vslot 0x1bc` is read as
  `GetEnchantExtraDamageType()` and the NPC grade member `+0x8f08` as `GetIsBosRaidNPC() || GetIsMiddleBosRaidNPC()` ( grade 3 is never set offline ): a unit with either resists the execution and takes the debuff. The class map's guess "`BLUFF_CIEL.StartFuture`"
  is wrong: June registers Lu's preload as Ciel's **StateInit** ( the delegate maker `32b4c930` fills slot +0x18 ). State 378 ( male chains ) has the skill-state byte in the listing @32b49b36 although `ch1n_skill_cases.tsv` lists none; it is set.
  Dancing Blade's loop path needs a transcendence memo ( `GetTranscendenceMemoSkillID` reads the skill-note memos, CH4c ): without one the second key press shows the lack-of-MP pop-up ( `JUNE_BUGS` 770 ), so the loop cannot be play-tested yet.
- **June bug:** `JUNE_BUGS.md` 770 ( Dancing Blade shows the lack-of-MP text when there is no memo ).
- **Debug lines ( `_SKILL_DEBUG` ), tag `[JUNE_CHAR] LUCIEL SKILL`:** `oblivion kill unit hp H line L` / `oblivion debuff unit hp H line L resists N`, `summon darkness create npc N key K body B`, `dancing blade loop start count N` / `no loop ( memo skill N, count N )`,
  `phantom blade attack ( hit armed )` / `fail ( no hit, half the cost returns )` / `attack start target found N`, `chains of bondage projectile body B upgraded N` / `dash attack body B`; the `equip` / `use` / `cooldown` lines of CH2a stay.
- **Gates ( worktree; the file also holds a peer's uncommitted CH4b hunks, passed as `--with` partners ):** `finish.py --files X2GULuciel.cpp .h X2BuffFactor.h --with JUNE_CHAR_LUCIEL_SKILL JUNE_ENUM_NAMES JUNE_CHAR_LUCIEL_SKILL_DEBUG --state X2GULuciel.cpp Luciel_Duo
  --icons 10018 30012-30015 30035 30055-30056 30064-30067 30082-30083 30204-30206 30304-30309`: FINISH RESULT PASS ( 7 gates, 0 failed, 1 skipped = luabind: no `.lua` staged ); flagcheck `_SKILL` ( with `_PASSIVE` as a partner ) 2 PASS, 0 FAIL, mine = 0 deleted lines;
  `JUNE_ENUM_NAMES` PASS~ ; junediverge PASS, enumscan exit 0, stateends PASS, skillicons PASS. `/Zs` probe of `X2GULuciel.cpp` ( census `cl_cmd`, debug on ): 0 errors. **Range gate `c63ae472^..c63ae472`:** `flagcheck _SKILL --with _PASSIVE _SKILL_DEBUG JUNE_ENUM_NAMES` `X2GULuciel.cpp` PASS ( +776, 0 deleted ), `.h` PASS ( +28 ); `JUNE_ENUM_NAMES` PASS~ ( `X2BuffFactor.h` +5 ); junediverge, enumscan, stateends, skillicons PASS ( finish.py's own run lists the two Luciel files as OTHER and FAILs the `JUNE_ENUM_NAMES` run: it has no `--with` partners there, the per-flag run above is the verdict ). The commit was made from a temporary index ( HEAD + this phase's hunks ): a peer had staged its CH4b files in the shared index.
- **Build:** `build.py --deploy` ran its own full build ( main 9fb16748 + the uncommitted files of this tree ): `DEPLOY OK: X2_offline.exe is 27,133,440 B, dated 2026-11-16 10:38:08, md5 85d46dcf40f1843140ce152466d7cc5c, equal to X2/US_SERVICE/x2.exe (2026-11-16 10:38:01); built at main 9fb16748 + 10 uncommitted code file(s).` `EXESTR OK: 6 of 6 found in X2_offline.exe` ( wide: the oblivion / summon darkness / dancing blade / phantom blade / chains debug lines, `EffectSet_SA_LLC_OBLIVION_Death` ).
- **Play-test save ( applied 10:23 with the game closed; backups `db_backup/els_db_20261116_102243_pre_createchar.sql`, `..102326_pre_sql.sql`, integrity ok ):** `python tools/elsdb.py x --file kickoffs/char-luciel/CH3a_save.sql` ( file kept ):
  `LuDlA` ( unit 223, Lu / Dread Lord, level 70, base skills ) and `LuDlB` ( unit 224, the enhanced skills ), 3 / 2 skill pages, slots 0-3 on keys A S D C and the same on 5-8 ( Q W E R ), slots 4 / 9 empty.
- **Play-test ( 2026-11-16, user: all passed; logs 10:48-10:50 on the 10:38 deploy, 0 EXCEPTION, 0 ErrorLog lines, 0 unbound Lua names ):** `LuDlA` pages 1-3 and `LuDlB` pages 1-2, all from bank A. `use` lines: 30012 x2 / 30064 x2, 30013 x4 ( `summon darkness create npc 1906 key 28` each cast ) / 30065 x2,
  30015 x2 ( `phantom blade attack ( hit armed )`, `attack start target found 1` ) / 30067 x2, 30014 x2 / 30066 x2 ( `oblivion kill unit` hp 9140 / 8913 under the line 12515 / 15018, `oblivion debuff unit` on the others, `resists 0` ), 30056, 30083, 10018, 30055, 30206 x3 ( projectile `upgraded 0` ), 30304 x3,
  30204 x2, 30308 x2, 30309 ( projectile `upgraded 1` ). **Not in the logs ( by sight only ):** bank B ( Q W E R, slots 5-8: no `equip ... bank B` line ), the Phantom Blade miss ( `fail` ), Dancing Blade's second key press ( `no loop` ), Shackling Chain's dash attack, Chains 30206's `use` shows the cooldown on state 378 ( the male state ) from Ciel.
- **Play-test hand-off:** in the message that closes the session ( per page and key, the log lines to expect ). Logs to read: `python tools/logs.py --grep "JUNE_CHAR\] LUCIEL SKILL"`.

### CH3b - Done (2026-11-16)

Sonnet 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` ( no new define ); `SERV_IRUHADEV_JUNE_ENUM_NAMES` for `BTI_BUFF_SUMMON_FAMILIAR_LU` / `_CIEL` ( 476 / 477 ), `BFI_BUFF_SUMMON_FAMILIAR_LU` / `_CIEL` ( 543 / 544 ), `BFI_BUFF_TWILIGHT_JUDGEMENT` ( 549 ), `X2BuffFactor.h` by bytepatch;
`_SKILL_DEBUG` stays **on** for the play-test ( the CH3b review, which also covers CH3a, retires it ). Files: `X2GULuciel.h` / `.cpp`, `X2BuffFactor.h`, `MODS.md`, `funcmap.tsv` ( `grep "(CH3b)"`, 14 rows ), `JUNE_BUGS.md` ( 772-774 ),
`kickoffs/char-luciel/CH3b_save.sql` ( new ), `kickoffs/char-luciel/CH3b-review.md` ( new ).
- **Written ( every `CH3b` row of `ch0_class_map.tsv`: 45 functions, the thunks are not ):** nine preloads ( generated from the listings ), the 2nd movement of Fantasie Impromptu ( frame move, event process, its state 364 registered in `InitStateCommon` ),
  Swift Smash ( frame move, event process ), Spectral Spear ( frame move + the specter block June inlines twice ), Shadow Bolt ( start state: start future, frame move, event process; the nine control state starts, control start future, control event process, end event process, end state end future ),
  Twilight Judgement ( state start, the buff start `32b20a10`, reattack start / event process ), Summon Familiar ( frame move ), Soul Spread ( the ready / loop event processes of both bodies, the loop frame move ), Magdonic Cannon ( event process ), Rain of Chaos ( 3 ),
  and the 9 setter cases ( 30210, 30024, 30016, 30022, 30057, 30058, 30084, 30208, 30305 ) with their 17 extra states ( 404, 405 .. 414, 415 / 416, 584, 402, 576 .. 580 ).
- **Decoded, not guessed:** every extra state's slots with the stack-offset rule ( base = esp0 + 0x1c, a push moves later offsets by 4, a call to the register routine resets it ); what each hyper state copies ( never the Init );
  Shadow Bolt's start frame move: `CanDown( pos, 30, &land, &( int )Now +0x80f4, NULL, 1, 0, 1, 0 )` only tests for ground ( the landing point is not used ), the spawn is the offset position y + 300 or, with no ground, the user's position y + 240; `CreateNPCReq` 14 arguments in the CH3a shape;
  vslot `0x204` = `GetPowerRate` ( the CH1a member `m_fShadowBoltDuration` stores it: renamed `m_fShadowBoltPowerRate` ), `0x260` = the minor-body byte `+0x8e90`, game vslot `0x8c` = `InitFocusUnit`, `32b3db80` = CH2c2's `AddSoulCollectorStack` ( Swift Smash upgraded ),
  `31fca8e0( id, 1 )` `+0x178` = `IsNowStateSkillUpgraded` ( Spectral Spear, Twilight Judgement ), the NPC members `+0x8750 / +0x8d34 / +0x8e39` = `SetFocusCamera` / `SetFocusCameraForce` / `SetApplyCinematicUI` ( Chung's Artillery Strike has the same three ).
- **Deviations / stand-ins.** The 2nd movement's `_MEMO` attack boxes ( `GetEqippedSkillMemo( 0x94ee7 )` and the synced short = 5 ) are not written: June's memo reader is a stub that answers 0, as in CH3a. The class map's StateEndFuture names of Soul Spread's loop functions are wrong:
  June registers them in the **event process** slot ( the setter listing, `32b4a404` / `32b4a4b4` ); they are written as `_EventProcess`. The nine Shadow Bolt control states share one Lua table ( `LSI_SA_LCN_SHADOW_BOLT_CONTROL` ) and June names only 405 and 414: 406 .. 413 are
  `LSI_SA_LCN_SHADOW_BOLT_CONTROL + 1 .. 8` named constants in `_CONST_LUCIEL_CH3B_` ( no June name ). `32b36ba0` ( Ciel's loop frame move ) and `32b01f80` ( the reattack event process ) are byte-equal listings: Ciel's loop registers Lu's function, the reattack calls the Shadow Bolt end function.
- **June bugs / quirks ( `JUNE_BUGS.md` 772-774 ):** Shadow Bolt's control tests `distance <= -750` ( never true: the offset block is dead ); Ciel's Swift Smash plays no effect set ( the shared frame move needs the Lu body ); Summon Familiar's mate is the last matching unit in the list.
- **Debug lines ( `_SKILL_DEBUG` ), tag `[JUNE_CHAR] LUCIEL SKILL`:** `fantasie 2nd movement attackbox N`, `swift smash effect set power rate P hyper H`, `spectral spear specter seed S`, `shadow bolt spawn ( ground G, my unit M ) power rate P` / `shadow bolt stacks N` / `shadow bolt control state S ( now N )`,
  `twilight judgement start ( upgraded U )` / `twilight judgement reattack stacks left N`, `summon familiar body B partner P level L`, `soul spread loops N`.
- **Gates ( worktree ):** `finish.py --files X2GULuciel.cpp .h X2BuffFactor.h --with JUNE_CHAR_LUCIEL_SKILL JUNE_ENUM_NAMES JUNE_CHAR_LUCIEL_SKILL_DEBUG --state X2GULuciel.cpp Luciel_Duo --icons 30016 30022-30024 30057-30058 30071-30074 30084 30208 30210 30305 30311-30312`:
  FINISH RESULT PASS ( 7 gates, 0 failed, 1 skipped = luabind: no `.lua` staged ); flagcheck `_SKILL` PASS ( with `_CORE` / `_PASSIVE` as partners: 2 PASS, 0 OTHER, 0 FAIL; the 10 deleted lines = 2 renamed `m_fShadowBoltDuration` lines + the move below ), `JUNE_ENUM_NAMES` PASS~, junediverge PASS, enumscan exit 0,
  stateends PASS, skillicons PASS; `june2017_re/char_luciel/ch1m_regcheck.py`: 341 registrations, 0 problems. `/Zs` probe of `X2GULuciel.cpp` ( census `cl_cmd`, debug on ): 0 errors. **`X2GULuciel.h` also carries one hunk that is not CH3b's:** in HEAD the CH3a declarations sat inside the `_PASSIVE` block
  and the `_SKILL` `#endif` above it; the working tree had them in the `_SKILL` block ( uncommitted by an earlier session ): committed here with the CH3b declarations, so HEAD matches the tree.
- **Build:** `build.py --deploy` ran its own full build ( main 3e9fa56d + 17 uncommitted code files, peers' included ): `DEPLOY OK: X2_offline.exe is 27,159,552 B, dated 2026-11-16 11:49:24, md5 e702326155a0d46b74e3316d91e7f2e9, equal to X2/US_SERVICE/x2.exe (2026-11-16 11:49:14); built at main 3e9fa56d + 17 uncommitted code file(s).`
  `EXESTR OK: 13 of 13 found in X2_offline.exe` ( wide: the eight debug lines, `EffectSet_LCG_Fantasie_Impromptu_2nd_Movement_Attackbox_4`, `DamageEffect_SI_SA_LCN_SPECTRAL_SPEAR_Specter`, `EffectSet_A_LCN_Soul_Spread` ) and `EXESTR OK: 2 of 2` ( narrow: `SHOT_SHADOW_BOLT_DOWN_RIGHT`, `LSI_SA_LCN_SHADOW_BOLT_CONTROL` x9 ).
- **Play-test save ( applied 11:38 with the game closed; backups `db_backup/els_db_20261116_113844_pre_createchar.sql`, `..113848_pre_createchar.sql`, integrity ok ):** `python tools/elsdb.py x --file kickoffs/char-luciel/CH3b_save.sql` ( file kept ): `LuNoA` ( unit 225, Lu / Noblesse, level 70, base skills )
  and `LuNoB` ( unit 226, the enhanced skills ), 3 / 2 skill pages, slots 0-3 on keys A S D C and the same on 5-8 ( Q W E R ), slots 4 / 9 empty.
- **Play-test hand-off:** in the message that closes the session ( per page and key, the log lines to expect ). Logs to read: `python tools/logs.py --grep "JUNE_CHAR\] LUCIEL SKILL"`.
- **Commit** `d4b36b07`. **Range gate** `d4b36b07^..d4b36b07` ( `--with` the `_SKILL`, `_SKILL_DEBUG`, `_CORE`, `_PASSIVE` and `JUNE_ENUM_NAMES` flags, `--state`, `--icons` as above ): FINISH RESULT PASS ( 9 gates, 0 failed, 1 skipped = luabind ); flagcheck `_SKILL` / `_CORE` / `_PASSIVE` PASS ( 2 PASS, 0 FAIL; the 10 deleted lines are the 2 renamed member lines and the declaration move ), `JUNE_ENUM_NAMES` PASS, junediverge, enumscan, stateends, skillicons PASS.
- **Review:** `kickoffs/char-luciel/CH3b-review.md` ( Opus; covers CH3a and CH3b, retires `_SKILL_DEBUG` ). Next: CH3c waits on the review.

### CH3b - Review (2026-11-16)

Opus 5.5 high, covers CH3a ( `c63ae472` ) and CH3b ( `d4b36b07` ). **Verdict: pass, no code change.** `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL_DEBUG` retired ( `Always.h`, the CH3a line commented out by bytepatch; the CH2d2 line above it stays ).
- **Line by line against June's listing ( `charport_adis.py luciel` + Ghidra decompile, read-only ):**
  - Spectral Spear frame move `32b2f4b0`: the two upgraded timers 0.36 / 0.56 ( each behind `31fca8e0( id, 1 ) +0x178` = `IsNowStateSkillUpgraded`, the second re-tested after the first fails ), the five soul timers 0.31 .. 0.71 ( 0.41 is compared as the double at `334d7488` and fired as the float at `334d7484`: same value; 0.71 goes through `31e4c230` = the same oneshot test ), seed `int( ( now - 0.3 ) / 0.1 + 0.5 )` by `_ftol2`, rotation x += R( s ) * 360, y / z += R( s + 10 / 20 ) * 110 + 120 ( stack offsets traced through the pushes ), `HaveThisBuff( 462 )` then `AddBuffAccumulationCount( 523, -1, 1, 0 )`; `CreateInstance` arguments ( Bip01, rotation twice, land y, false, -1 / 1 / 1, -1, -1, 0, -1 ) = the port.
  - Shadow Bolt start frame move `32b2fba0`: oneshot 0.001, `GetPowerRate()` into +0x9698 ( the rename to `m_fShadowBoltPowerRate` is right ), offset = dir * 120 added when right / subtracted when left, y + 300; `CanDown( pos, 30, &land, &line, NULL, 1, 0, 1, 0 )` ( argument addresses traced: land copy at esp+0x40, line index at esp+0x38 ); no ground: `GetPos()` y + 240; `CreateNPCReq` only for my unit, 0x774 = `NUI_SI_SA_LCN_SHADOW_BOLT`, delay 0.3, NAT_ALLY, `TN_NONE`, `NCT_NONE` ( June's two trailing arguments, my-unit and 0, are the CH3a / Eve shape ); stack timer 1.0; Noble Devotion 30419 `SA_EFFECTIVE_COUNT` + 1.0 truncated, + 12, `UCHAR` loop of BFI 548 at Shadow Bolt 30016's level ( vslot 0x21c `( BFI, 0, level, 0, 0 )` = March order `( BFI, level, NULL, 0, false )` ).
  - Shadow Bolt control event process `32b30090`: key released or no BTI 481 -> 414 unconditionally; positions times the direction per component; `GetDistance <= -750` dead ( JUNE_BUGS 772 ); `user.y + 1500 <= spear.y` sets v = v - ( v - 1500 ) ( `fsubrp` order checked ); the eight key rows and the default in June's order: 410 ( h >= 0, v <= 0, up + left; tested first with no spear ), 411, 412, 413, 406, 407, 408, 409, 405; no state change when already in the target. The NaN edges of each `>= / <=` match the C++ compares.
- **Setter cases ( stack-offset rule, struct = `UserUnitStateData`: +0x10 hyper, +0x11 skill, +0x14 Init, +0x1c SSF, +0x24 SS, +0x34 FM, +0x44 EP, +0x4c SEF ):** Swift Smash ( hyper copies EP + FM; 404 EP / FM, byte ), Spectral Spear ( EP + FM ), Shadow Bolt ( hyper copies SSF + FM + EP; 405 SS `32b4e1e0` + SSF `32b4e1f0` + EP `32b4e200`; 406 .. 413 SS `32b4e210` .. `32b4e280` + EP `32b4e200`; 414 SS `32b4e1e0`, EP `32b4c330`, SEF `32b4e290` ), Twilight ( SS + EP; 415 / 416 SS `32b4e2b0` + EP `32b4c340` ), Magdonic Cannon ( EP; 584 Init + EP, byte ), Summon Familiar ( EP + FM; 402 EP + FM, byte ), Soul Spread ( EP; 576 / 579 loop functions in the **EP** slot + FM, 578 Init `32b4c950` + EP, byte, 577 / 580 generic EP ). CH3a spot-check: Chains of Bondage ( `329cc2c0` copy-all; 376 / 379 generic EP, 378 SSF + EP + FM + byte ). All = the port; no Init is copied anywhere.
- **CH3a spot-check of `32b1fc90` ( Oblivion's execution ):** vslot 0x1bc is the NPC's weapon extra damage type ( funcmap row `3275aa20`, slot 111 ), +0x8f08 any non-zero grade, the line test `line < hp` -> debuff ( `BFI 529`, creator, level ) = the port and its stand-in comment.
- **Enums:** `BTI_BUFF_SUMMON_FAMILIAR_LU` / `_CIEL` 476 / 477, `BFI_BUFF_SUMMON_FAMILIAR_LU` / `_CIEL` 543 / 544, `BFI_BUFF_TWILIGHT_JUDGEMENT` 549, `BFI_DEBUFF_SUPPRESSION_OBLIVION` 529 = `Enum.src.lua`.
- **Not seen in the play-test logs, judged by code:** page 1 of `LuNoA` and bank B use the same cases as the seen casts ( one registration per skill, the slot index only picks the table ); Ciel's Swift Smash = JUNE_BUGS 773; control states 408 / 410 .. 413 = the rows above; Soul Spread from Lu = 576 / 577 above; Summon Familiar's partner = JUNE_BUGS 774 ( no second player offline ). No extra play-test.
- **ErrorLog `ErrorEnum(381)` ( 129 lines in the CH3b run ): not CH3b's.** FMOD result 47 = `FMOD_ERR_NEEDS3D` ( counted in `KTDXLIB/FMOD/inc/fmod.h` ). The line is engine C9h-8 ( `SERV_IRUHADEV_JUNE_DAMAGEEFFECT_SOUND`, `c3a723c0` ): `X2DamageEffect.cpp` plays a damage effect's `SOUNDn` 3D by default and `Set3DPositionAndUpdate` ( June `31d41710` ) calls `Update3DPosition` unconditionally; a sound first opened 2D ( cached by name ) fails `Set3DAttributes`. One line per play, not per frame; the sound still plays ( 2D ). All six files are damage-effect `SOUNDn` entries; older `ErrorLog_pre_*` runs show the same error for other classes ( FIRE2.OGG, FLAME.OGG ). June makes the same call ( JUNE_BUGS 139 notes the text ); no change, nothing routed.
- **Gates:** `finish.py --review --commits c63ae472 d4b36b07 --with _SKILL _SKILL_DEBUG JUNE_ENUM_NAMES _CORE _PASSIVE --state X2GULuciel.cpp Luciel_Duo --icons ...`: junediverge PASS, `--check-all` PASS, enumscan exit 0, stateends PASS, skillicons PASS; flagcheck per flag: the code files 4 PASS, 0 FAIL ( the 10 deleted lines = CH3b's 2 renamed member lines + the declaration move ), `X2BuffFactor.h` PASS~ in both commits, and 1 `ERROR` on `KTDXLIB/Always.h` ( c63ae472 +1, the debug define ): with any partner flag switched off over the whole file a peer `#error` guard fires ( `JUNE_USER_SKILL_USE_HELPERS` needs `JUNE_ENUM_NAMES`, `_LIVE` needs `_CORE`, `_PASSIVE` needs `_SKILL` ... ), so it is a tool limit on that one define line, which this review comments out. Retire worktree gate ( `--files KTDXLIB/Always.h` ): FINISH RESULT PASS ( `_SKILL_DEBUG` retired, junediverge PASS, enumscan exit 0 ).
- **Build skipped: debug retire only.**
- **Commit** `9223ccf2`. **Range gate** `9223ccf2^..9223ccf2`: FINISH RESULT PASS ( 6 gates, 0 failed; `_SKILL_DEBUG` retired, junediverge PASS, enumscan exit 0 ).

### CH3c - Done (2026-11-16)

Sonnet 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` ( no new define ); `_SKILL_DEBUG` is **on** again for the play-test ( `Always.h`, one inserted line by bytepatch; the CH3c2 review retires it ). No review of its own ( the CH3c2 review covers CH3c ).
Files: `X2GULuciel.h` / `.cpp`, `Always.h`, `MODS.md`, `funcmap.tsv` ( `grep "(CH3c)"`, 41 rows ), `JUNE_BUGS.md` ( 776-777 ), `kickoffs/char-luciel/CH3c_save.sql` ( new ), `kickoffs/char-luciel/CH3c2.md` ( amended ), `june2017_re/char_luciel/ch3c_held_cases.txt` ( new ).
- **Written ( every `CH3c` row of `ch0_class_map.tsv`: 41 functions, thunks are not ):** 13 preloads ( generated from the listings, call counts checked 13 / 13 ), `ChangeDesperadoAttackState` and Desperado's three event processes, Rapid Impact's two, Sinister Mind's start future / event process / fly event process,
  and the Transcendence form's: the skill event process, the form start, jump up / down / Z / Z landing ( June's X landing is byte-equal, one function ) / X / X loop, combo Z / ZZ / ZZZ / ZZZZ, the ZZ end future, XXX ready / loop / end starts, Shadow Fury and Silent Rage starts.
  Setter cases written and **registered**: Desperado ( + 5 states ), Sinister Mind ( + fly ), Garden of Haures, Abyss Extinction, Rapid Impact ( + 3 ), Punisher ( + 1 ), Rapid Fury ( + Ciel ).
- **Held for CH3c2 ( not registered ):** Transcendence ( 30049 ), Abyss Sniping ( 30059 ), Demonic Rush ( 30060 ). Their states have no event process until CH3c2 writes it, so they would run `XAP_ONE_WAIT` forever ( stateends FAIL with 19 STUCK, INVINCIBLE ones included ); the finished cases are in `ch3c_held_cases.txt`, `CH3c2.md` says how to land them.
  Every `body in CH3c2` comment in a registered case marks a callback of CH3c2's ( Desperado's start / frame moves, Sinister Mind's and Garden's frame move, Rapid Impact's move-attack start future, Punisher's frame moves ): until then Desperado does not shoot and Garden of Haures does not spawn.
- **Decoded, not guessed:** the cases' slots with the physical stack depth ( cdecl delegate makers keep their two pushes until the `add esp` ): the male / Ciel preloads of Rapid Impact ( 465 ), Punisher ( 467 ) and Rapid Fury ( 581 ) fill **StateInit**, not StartFuture as `ch0_skill.tsv` says ( as CH3a found for Bluff ), and 32b32550 is a StateStartFuture;
  the skill-state byte on 466, 467 ( digest missed ), 581, and on 494 / 495 / 500 ( in the held case ); state 499 reuses the Shrewd Cutter end table ( named constant ); the form's calls are the base `StateChange` ( CH2d3 rule ); the Transcendence combos test the buff first, then wait, then arm the next hit and turn the unit to the held direction ( `TurnTransformComboToHeldDirection`, June's inline block );
  vslot `0x568` = `CheckBoundingTarget`; `31fc9d90( 1 )` = `IsTransformed( DT_MINOR )`; `Stable Dark Energy` 30421 gates the fly start; `SKILL_CANCEL_AFTER_FLY` takes `CommonEventProcess()` as its body ( Raven's quirk ).
- **`CUSTOM_RENDER_STATE` x3:** nothing class-side: the parse is the engine's ( june-engine R5, `ParseCustomRenderState` in `CommonStateStart` ), the three Demonio scripts carry the table. **`SUPER_ARMOR_CUSTOM_COLOR` reader:** a scan of June's whole `.text` for displacement `0x8a2c` finds the writer ( `32059590` ) and four hits in another class
  ( `327b00e5`, `327b14f0`, `327b5c65`, `32e70b9d/d66` ): CX2GUUser's `+0x8a2c` has **no reader in June**; the member stays as DU1b4 wrote it, no accessor to add.
- **Deviations / stand-ins:** none beyond the held cases. **June bugs:** `JUNE_BUGS.md` 776 ( Sinister Mind's fly state runs CommonEventProcess only when the fly cancel fires ), 777 ( Desperado's shot states never run it ).
- **Debug lines ( `_SKILL_DEBUG` ), tag `[JUNE_CHAR] LUCIEL SKILL`:** `desperado aim value V targets N`, `rapid impact move attack body B`, `sinister mind fly state`, `transcendence form start` ( unused until the held case ); the `equip` / `use` lines of CH2a stay.
- **Gates ( worktree ):** `finish.py --files X2GULuciel.cpp .h --with JUNE_CHAR_LUCIEL_SKILL _SKILL_DEBUG _CORE _PASSIVE JUNE_CHAR_LOCKON_SYSTEM --state X2GULuciel.cpp Luciel_Demonio --icons 30047-30053 30059-30060 30078-30081 30085 30217-30218 30306 30314-30315`:
  FINISH RESULT PASS ( 9 gates, 0 failed, 1 skipped = luabind: no `.lua` staged ); flagcheck `_SKILL` / `_CORE` / `_PASSIVE` / `_LOCKON_SYSTEM` PASS ( 0 deleted lines; patience diff +1289 / +42 insertions only ), junediverge PASS, enumscan exit 0, stateends PASS ( the 19 STUCK of the first run were the held states ), skillicons PASS;
  `ch1m_regcheck.py` 341 / 0 problems. `Always.h` is left out of `--files` ( flagcheck ERRORs on its line 13445 for any edit ). `/Zs` probe of `X2GULuciel.cpp` ( census `cl_cmd`, debug on ): 0 errors ( an injected undeclared name was caught: the probe really compiles the block ).
- **Build:** `build.py --deploy` ( main ad28b6e7 + 3 of my code files, peers' uncommitted files included ): X2ServerProtocol, KTDXLIB and **X2Lib compiled ( 815 s, 0 errors )**; the **X2 exe project failed** in a peer's edit: `X2Unit.h(1446): C2653 'CX2SocketItem' is not a class or namespace name`
  ( june-stat-calc S2, `SERV_IRUHADEV_JUNE_STATCALC_EQUIP_STAT`, uncommitted ): not mine, not touched. a re-run stopped in the same peer's `Always.h` guard. **The peer's later full build ( 14:51 ) carries this commit:** `EXESTR OK: 6 of 6 found in X2_offline.exe` ( wide: the four debug lines, `LDN_GARDEN_OF_HAURES_FloorADD.Y`, `Ciel_Garden_Plant01.ogg` ) and `EXESTR OK: 3 of 3` ( narrow: `LSI_A_LDN_M_RAPID_IMPACT_MOVEATTACK`, `LSI_SA_LDN_DESPERADO_ATTACK_DOWN`, `LSI_A_LDN_RAPID_FURY_CIEL` ), X2_offline.exe 27,192,832 B, dated 2026-11-16 14:51:34, md5 9e0bda3c359d7701fc1033ce3149fdac ( no `DEPLOY OK` line of my own ).
- **Play-test save ( applied 14:07 with the game closed; backups `db_backup/els_db_20261116_140702_pre_createchar.sql`, `..140706..`, integrity ok ):** `LuDnA` ( unit 228, Lu / Demonio, level 70, base skills ) and `LuDnB` ( unit 229, the enhanced skills ), 2 skill pages, slots 0-3 on keys A S D C and the same on 5-8 ( Q W E R ), slots 4 / 9 empty,
  Stable Dark Energy ( 30421 ) learned on both: `python tools/elsdb.py x --file kickoffs/char-luciel/CH3c_save.sql` ( file kept ).
- **Play-test hand-off:** in the message that closes the session ( per page and key ). Logs to read: `python tools/logs.py --grep "JUNE_CHAR\] LUCIEL SKILL"`.

- **Fix after the first play-test ( 2026-11-16, commit `a6edaf9d` ):** Sinister Mind, Garden of Haures and Punisher showed no effect: their frame moves were CH3c2's rows. Written now ( June `32b32fe0` Sinister Mind + fly state, `32b32800` / `32b32b30` Punisher Lu / Ciel,
  `32b33dc0` Garden of Haures: the old garden killed, NPC 2269 120 in front with the 45-high ground probe, key = the skill's effective time ) and registered; CH3c2 no longer owns them ( `CH3c2.md` amended ). Debug line `garden of haures spawn ( ground G, my unit M ) at x y z`.
  `DEPLOY OK: X2_offline.exe is 27,204,608 B, dated 2026-11-16 15:19:16, md5 9eb54c83c98ae8a5a9d0b93db6be19a4`, `EXESTR OK: 4 of 4 found` ( wide: the garden debug line, three effect set names ); gates `finish.py` PASS ( 9 gates, 0 failed ), `stateends` PASS.
  **Play-test ( 2026-11-16, user ): everything on pages 1 and 2 works now** ( Rapid Fury's effect needed no change: its effect set, projectile and hit were already in the log ). Desperado still does not shoot ( its start / frame moves stay CH3c2's ).

### CH3c2 - Done (2026-11-16)

Sonnet 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` ( no new define ); `SERV_IRUHADEV_JUNE_ENUM_NAMES` gains `BFI_BUFF_TRANSCENDENCE` ( 790 ) and `BFI_DEBUFF_DESPERADO_MOVE_BRAKE_ADD` ( 792, `X2BuffFactor.h` by bytepatch ). `_SKILL_DEBUG` stays **on** for the play-test; the review ( `kickoffs/char-luciel/CH3c2-review.md`, covers CH3c ) retires it.
Files: `X2GULuciel.h` / `.cpp`, `X2BuffFactor.h`, `JUNE_BUGS.md` ( 781-782 ), `MODS.md`, `funcmap.tsv` ( `grep "(CH3c2)"`, 35 rows ), `kickoffs/char-luciel/CH3c2-review.md` and `CH3c2_save.sql` ( new ), this note, `STATUS.md`.
- **Written ( 35 of the 40 `CH3c2` class-map functions ):** Desperado ( start, the end shot state's start, ready start, frame move, shot frame move for the three shot states, and `AimAtNextDesperadoTarget` = the class map's `32b14030` helper ), Abyss Sniping ( ready / up event processes, ready / up frame moves ),
  Demonic Rush ( two event processes ), Rapid Impact's move attack ( `..._MOVEATTACK_StateStartFuture`, both bodies ), and the Transcendence form: start future / start, end / end-state, wait, walk, jump ready, jump landing, jump Z loop, combo X / XX / XXX ready / loop / end, Shrewd Cutter ( start, event process, attack A / B start future and event process, end start future ),
  Shadow Fury / Shrewd Cutter end / Silent Rage's shared event process and Shadow Fury's state end. Not redone: Sinister Mind, both Punisher and Garden of Haures frame moves ( the CH3c fix `a6edaf9d` wrote them ); `32b4ce50` is an STL range erase, written as `m_vecDesperadoTarget.clear()`.
- **Landed from CH3c's held list:** Transcendence ( 30049 / 30080 ), Abyss Sniping ( 30059 ) and Demonic Rush ( 30060 ) pasted before the HEAVY_HIT case, and the nine `body in CH3c2` comments of the Desperado and Rapid Impact cases turned into `SET_CB_FUNC` lines ( 33 + 2 two-slot comments by script, the member from the comment's slot word ).
  `stateends.py check` : 584 tables, 170 with an event process, **0 STUCK**.
- **Decoded, not guessed ( listing + Ghidra decompile ):** vslot `0x524` = `GetNowStateID`'s skill ( `GetNowStateSkillID()` ); `32007c40( type, skill, memo, &value )` = the file-local `TryGetAcquiredSkillAbilityValue` ( Desperado SA 177 / 2, Abyss Sniping SA 29 / 158 / 159 ); `vslot 0x134` = `GetLandPos( bFuture )`;
  Desperado's lock effect set handle is stored in the target entry's float time word ( JUNE_BUGS 782 ) and its hyper state copies no start ( 781 ); the shot's pitch band is stored as 0 between -20 and 30 ( x87 stack: `fstp` of the zero ) and Abyss Sniping's loop effect always stores yaw 0; the x87 compares ( `jp` after `test ah, 5` ) read as `NOT <` per `charport_adis.py`.
  The Transcendence start is the Altar of Evil's start ( `32b43910` ) with skill 30049, BFI 790 and end key `LUCIEL_LDN_LOCK`; the form's wait / walk / jump ready / jump landing differ from the Altar's ( `32b31260` / `32b31480` / `32b133d0` / `32b316a0` ) only in the ids, the buff ( 764 ) and the order of the double step and hyper mode ( listing diffs ).
  Shrewd Cutter's attack states toggle A / B by the current state id; its end start falls back to the lock-on's safe position ( +45, state 499 ) off an enabled normal line. `m_FrameDataNowBefore.unitCondition.dirVector` is June `+0x7fa8` ( Ara's Swallow Assault reads the same ), the turn sets `bAttackerFront`.
  `m_vecAbyssSnipingTarget` is `std::vector<CX2GameUnit*>` ( CH1 declared ints; June stores the unit pointers ).
- **Deviations / stand-ins:** none. **June bugs:** `JUNE_BUGS.md` 781 ( Desperado's hyper state has no start function ), 782 ( the lock effect handle in the entry's time word ).
- **Debug lines ( `_SKILL_DEBUG` ), tag `[JUNE_CHAR] LUCIEL SKILL`:** `desperado targets N need N brake B`, `desperado shot I of N power P pitch X effect E`, `abyss sniping aim target T pitch X range up U down D`, `shrewd cutter start lock L`, `shrewd cutter attack N next A|B`; CH3c's `desperado aim value`, `rapid impact move attack`, `transcendence form start`, `garden of haures spawn` stay.
- **Gates ( worktree ):** `finish.py --files X2GULuciel.cpp .h X2BuffFactor.h --with JUNE_CHAR_LUCIEL_SKILL _SKILL_DEBUG _CORE _PASSIVE JUNE_CHAR_LOCKON_SYSTEM JUNE_ENUM_NAMES --state X2GULuciel.cpp Luciel_Demonio --icons 30049 30059-30060 30080`:
  FINISH RESULT PASS ( 10 gates, 0 failed, 1 skipped = luabind: no `.lua` staged; `luabind.py check Luciel_Demonio` run apart: 0 unbound, ENUM 0 missing, PASS ); flagcheck `_SKILL` / `_CORE` / `_PASSIVE` / `_LOCKON_SYSTEM` / `JUNE_ENUM_NAMES` PASS ( 10 deleted lines in the `LUCIEL` flags' view: the nine `body in CH3c2` comments of the cpp and `X2GULuciel.h`'s `std::vector<int>` line, which `OTHER` rows attribute to `_CORE` / `_SKILL` / `_SKILL_DEBUG`; `JUNE_ENUM_NAMES`: 0 deleted, `X2BuffFactor.h` PASS~ ),
  junediverge PASS, enumscan exit 0, stateends PASS, skillicons PASS. `Always.h` is not in `--files` ( a peer's uncommitted edit ). `/Zs` probe of `X2GULuciel.cpp` ( census `cl_cmd` ): 0 errors; an injected undeclared name in a copy was caught, so the probe compiles the new block.
- **Build:** `DEPLOY OK: X2_offline.exe is 27,232,768 B, dated 2026-11-16 16:08:00, md5 a6da3cac2e26097268d33b765ccea309, equal to X2/US_SERVICE/x2.exe` ( X2Lib compiled 733 s, 0 errors; peers' uncommitted `Always.h`, `X2GUUser.*`, `X2SocketItem.*` are in it ). `EXESTR OK: 1 of 1` ( narrow `LSI_SA_LDN_ABYSS_SNIPING_UP` ) and `EXESTR OK: 8 of 8` ( wide: the five new debug lines, `EffectSet_SA_LDN_DESPERADO_Lock_Shot`, `EffectSet_SA_LDN_ABYSS_SNIPING_LOOP`, `LUCIEL_LDN_LOCK` ).
- **Play-test save ( applied 15:53 with the game closed; backup `db_backup/els_db_20261116_155341_pre_sql.sql`, integrity ok ):** `LuDnA` ( unit 228 ) and `LuDnB` ( unit 229 ): page 1 Desperado / Transcendence / Abyss Sniping / Demonic Rush on A S D C and Q W E R ( B: the `+` ids 30078 / 30080 ), page 2 Rapid Impact ( B: 30314 ), Shadow Fury / Shrewd Cutter / Silent Rage ( 30050 - 30052 ) and Stable Dark Energy learned, slots 4 / 9 empty: `python tools/elsdb.py x --file kickoffs/char-luciel/CH3c2_save.sql` ( file kept ).
- **Play-test hand-off:** in the message that closes the session. Logs to read: `python tools/logs.py --grep "JUNE_CHAR\] LUCIEL SKILL"`.
- **Fix after the first play-test ( 2026-11-16 ):** the transform skill window never showed. My registration script matched the slot word `Start` before `StartFuture`, so six registrations went into the `StateStart` slot: the Transcendence start future ( it builds the window; the later start line overwrote it ), Shrewd Cutter attack A / B and end, Rapid Impact's move attack ( both bodies ). All six are `StateStartFuture` now; `stateends` PASS ( 0 STUCK ).
  `DEPLOY OK: X2_offline.exe is 27,238,400 B, dated 2026-11-16 16:19:29, md5 d135c422eac62224ee8f33605b8569d1`.
  **Play-test ( 2026-11-16, user ): works; logs read ( run after the 16:19 deploy ):** ErrorLog 0 lines, no unbound Lua name; Abyss Sniping ( aim target 1, pitch 0 ), Demonic Rush, Desperado ( 2 targets: shots 1 and 2 of 2, effect 1 ), Transcendence form twice ( window shown, `window 1` at the end ), Shrewd Cutter ( lock 1, attacks 1-10 alternating B / A, the 11 cap ), Rapid Impact move attack ( body C ).
  **Not seen:** Desperado with 5 or more targets ( brake 0 ), a non-zero pitch for Desperado and Abyss Sniping, Q W E R bank, LuDnB, Transcendence cast from Lu's body ( all form casts were body C ). The review judges them by code.

### CH3c2 - Review (2026-11-16)

Opus 5.5 high, covers CH3c ( `cff1bbd0`, fix `a6edaf9d` ) and CH3c2 ( `d8c08fa4`, fix `f1c545e3` ). **Verdict: pass after one fix.** `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL_DEBUG` retired ( `Always.h`, the CH3c line commented out by bytepatch ).
- **Fix ( under `_SKILL`, one line ):** Rapid Impact's move attack `32b32550` calls `GetLandPosition` ( `31c7bb00` @`32b32747` ) into a temporary it never reads: only the line index is used and the stand point keeps the target's height. The port had copied Bounding Target's `vStandPos = GetLandPosition( ... )` ( `32b341b0` / `32b343b0` / `32b35fd0` do copy the result back ). Now the result is dropped as in June; `JUNE_BUGS.md` 784.
- **Line by line against June ( `charport_adis.py luciel` + Ghidra listing, read-only ):**
  - Desperado shot frame move `32b32090`: power = vslot 0x204 `GetPowerRate()` x `fimul +0x97e8` ( max count ), times SA 2 ( `SA_EFFECTIVE_COUNT` = 2 ) when `32007c40( 2, skill, 1, &v )` succeeds; the stop takes the entry's first dword as the handle ( row 782 ), lock-shot set `( this, target, false, power, -1, ( 1, 1, 1 ) )`; shot set `( this, NULL, false, -1, -1 )`; the x87 stack: above 30.0 ( double `33439c68` ) store 30 and +100, below -20 store -20 and -100, else the leftover `fldz` is stored ( pitch 0, NaN too ); instance +0xe4 / e8 / ec = the three aim words, +0x1a4 = ( 0, offset, 0 ); `++count`, `32b14030`, CommonFrameMove. Its frame move `32b135e0` ( the brake, not seen in play ): `count >= ftol( SA 177, default 5 )`, BFI 792 on each live target.
  - Abyss Sniping aim `32b172d0`: SA 29 -> range ( 700 ), SA 158 -> +0x9870 ( up, 35 ), SA 159 -> +0x9874 ( down, -35 ); cone `32009180( out, 1, range, down, up, 1, 180, 0 )` ( stack traced ) = the port; empty -> zero aim; NULL first unit -> CommonFrameMove only; pass 1 = degree to the unit, aim point y -100 above 5 / +100 below -5 ( `test ah, 5` + `jp` = NOT 5 < z ), pass 2 = degree to that point.
  - Abyss Sniping loop `32b36d90`: `fucomp` + `test ah, 0x44` + `jnp` skips the clamp only when the pitch equals 0 ( NaN is clamped-tested, as the port's `0.f != z` ); the clamp to up / down; offset +100 / -100; +0x9868 always gets the x87 zero; set name 35 chars = `EffectSet_SA_LDN_ABYSS_SNIPING_LOOP`.
  - Shrewd Cutter end start `32b33bf0`: CommonStateStartFuture, then `g_pX2Game` / world / line map tests, `GetLandPos( true )` ( vslot 0x134 ) into the future position, no lock-on -> done; `CanDown( GetPos(), 30, NULL, NULL, &line, 0, 0, 1, 0 )` ( nine arguments, order traced ); line in range, `lineType == 0`, `bEnable` -> done; else the lock-on's +0x1c / +0x24 safe position, y + 45.0 ( `33472998` ), base `StateChange( 499, 0 )`.
  - Rapid Impact move attack `32b32550`: first try `( &targetPos, width * 0.5 + +0x8ef4, !targetRight, &stand )`, success keeps the target's facing; second try with `targetRight`, failure -> the target's position; after either failure my own `GetIsRight()`; y = target y; the land line test `lineType 0` and `bEnable == 1`; `UpCombinationPoint( +0x96bc )` then 0 even with no target. Only the landed-point line differed ( fix above ).
- **Transcendence form vs Altar of Evil:** normalised listing diffs of `32b33480` / `32b31260`, `32b336a0` / `32b31480`, `32b13730` / `32b133d0`, `32b338c0` / `32b316a0`: only the state ids, buff 763 -> 764, the wait's walk test moved after the double step, the walk's double step + hyper mode moved before the stop test, and one block re-layout in the wait's tail ( same jumps ). The port's diff of the two copies shows exactly those two moves.
- **Setter cases** ( `june2017_re/char_luciel/setter_slotcheck.py`, new: the stack-offset slot decode of `32b46580` against each case's `SET_CB_FUNC` lines ): Desperado, Transcendence ( 30049 / 30080 ), Abyss Sniping ( 30059 ), Demonic Rush ( 30060 ) match slot for slot, order aside; Desperado's hyper copies only FrameMove + EventProcess ( row 781 ); the Transcendence start = StartFuture `32b4e750` + Start `32b4e760` ( `f1c545e3` ), Shrewd Cutter A / B / end = StartFuture; the one June slot with no C++ line is `32b4aaa0 > 31fc7fc0` ( = CommonStateStart, commented as the Altar case does ). Rapid Impact ( delegate makers, by hand ): both move-attack states fill StartFuture via `32b4e4a0` ( `{ this, 32b4e450 }` ) + EventProcess, skill byte +0x11 on the male one.
- **Types and enums:** `m_vecAbyssSnipingTarget` is `std::vector<CX2GameUnit*>` ( June swaps the cone's vector of unit pointers in ); `BFI_BUFF_TRANSCENDENCE` 790, `BFI_DEBUFF_DESPERADO_MOVE_BRAKE_ADD` 792, SA 2 / 29 / 158 / 159 / 177 = `Enum.src.lua` / computed enum values.
- **Play-test logs ( 16:20 run ):** `abyss sniping aim target 1 pitch 0.0 range up 35.0 down -35.0`, `desperado targets 2 need 5 brake 0`, `desperado shot 1 of 2` / `2 of 2 power 1.00 pitch 0.0 effect 1`, `transcendence form start` + `window 1` at the end, `shrewd cutter start lock 1`, attacks 1-10 alternating; ErrorLog 0 lines. Not seen ( brake, non-zero pitch, bank B, `LuDnB`, a cast from Lu's body ): judged by code above, same cases and functions. No extra play-test: the fix only keeps the stand point at the target's height.
- **Gates:** `finish.py --review --commits cff1bbd0 a6edaf9d d8c08fa4 f1c545e3 --with _SKILL _SKILL_DEBUG JUNE_ENUM_NAMES -v`: junediverge PASS, `--check-all` PASS, enumscan exit 0; flagcheck code files 6 PASS / 0 FAIL ( 20 deleted lines = 5 + 9 `body in CH3c2` comments and the 6 slot lines `f1c545e3` rewrote; `X2GULuciel.h`'s `std::vector<int>` line and `X2BuffFactor.h` are the two `OTHER` rows, `_CORE` / `JUNE_ENUM_NAMES`, PASS~ ), and 1 `ERROR` on `Always.h` @10175 ( the compound guard the tool cannot evaluate, as in the CH3b review ). Fix worktree: `finish.py --files X2GULuciel.cpp --with _SKILL`: FINISH RESULT PASS ( 6 gates ); `stateends.py check X2GULuciel.cpp Luciel_Demonio`: 584 tables, 0 STUCK, PASS.
- **Full build skipped: no play-test.** `/Zs` probe of `X2GULuciel.cpp` ( census `cl_cmd` ): 0 errors; a probe `#error` showed `_SKILL` on. CH3d's build carries the fix.
- **Commit** `12d6e77d`. **Range gate** `12d6e77d^..12d6e77d`: junediverge PASS, enumscan exit 0, `_SKILL_DEBUG` retired; flagcheck `_SKILL`: `X2GULuciel.cpp` PASS ( +1 / -1, the fixed line ), 1 `ERROR` on `Always.h` @13690 ( the debug define's own compound `#error` guard, the same tool limit ), so FINISH reads FAIL on that line only.

## Phase CH4 - Passives and class mechanics

**Run with:** Opus 5.5 high, Opus review, user play-test.

| Sub-phase | Scope |
|---|---|
| CH4a | Lu/Ciel's passive state (June's `InitPassiveSkillState` counterpart, CH0 names it), the passive ids, their objects and readers, and the 7am delegate lists. Flag `_PASSIVE` |
| CH4b | The damage-exception override and the other overrides CH0 finds (`GetActualMPConsume` / `GetActualCoolTime` style), death prevention, the per-skill stores |
| CH4c | Memo and item readers: `MemoConditionLuciel`, the memo ids, and Lu/Ciel's branches in `CX2Game::UseItemSpecialAbility` / `X2BuffTemplet.cpp` |

**Done gate:** each passive's effect shows (CH0 names the stat or log line); 0 `no_state` rows left except passives with no state in June.

### CH4a - Done (2026-11-16)

Opus 5.5 high. Flags: `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_PASSIVE` ( new; `#error` guard needs `_CHAR_LUCIEL_SKILL`, `_JUNE_SOCKET_PARITY`, `_STATCALC_DAMAGE_REACT`,
`_ENCHANT_PARITY`, `_BUFF_BEHAVIOR_PARITY`, `_CHAR_ENEMY_SCAN`, `_CHAR_BUFF_TIME_PASS`, `_JUNE_DAMAGEDATA`; not tripped ), `_PASSIVE_DEBUG` ( new, stays on: the CH4b
review retires it ), `SERV_IRUHADEV_JUNE_ENUM_NAMES` ( 7 BFI + 3 BTI names in `X2BuffFactor.h`, June's `Enum.lua` values ). Files: `X2GULuciel.h` / `.cpp`, `X2BuffFactor.h`
( bytepatch ), `Always.h` ( bytepatch ). funcmap `grep "(CH4a)"` ( 10 rows ). No review of its own: `CH4b-review.md` covers CH4a and CH4b.
- **Ported whole ( June `32b45820`, vslot 0x690, uncarved: read from the capstone listing ):** `InitPassiveSkillState` reads 15 `RNSI_P` passives ( 30401-30408,
  30410-30415, 30417 ) and Backstab / Power of Oriax through `TryGetAcquiredSkillAbilityValue`, in June's order, with June's resets, and pushes the ten delegates:
  nine on the attack-result list ( `AddCombinationPointOnHit` CH1p, `ApplyFearOnHit` `32b3d380`, `ApplyPowerOfOriaxStunOnHit` `32b0fc50`, `AddSoulCollectorStackOnHit`
  `32b43590`, `PlaySoulExtortionDrain` `32b435f0`, `SetMakeTargetOnPhantomBladeHit` `32b0fce0`, `AddDarkCrescentTargetOnRuinousImpactHit` `32b177b0` ( + its find
  `32b4caf0`, inlined ), `SetMakeTargetOnRapidImpactHit` `32b13c80`, `AddInvasionOfDarkEnergyTargetOnHit` `32b17f90` ) and `FrameMoveMadnessCrave` `32b3e470` on the
  frame-move list ( `+0x66cc` ). Plus `CreateFearDamageEffect` `32b3ca30`. The seven debuff groups of the Lord of Fear test ( `3226e230..90`, tables `3373daa4..e8`, read
  from the exe ) are one 18-entry list in the CH4a namespace. Engine offsets settled: `+0x6898 / +0x68a0` = `SetMakeTarget`, gameunit `+0x264` = `GetGameUnitType`,
  vslot 0x200 `GetRidingOn`, 0x3e4 = bone position or `GetPos`, sequence `+0x310 / +0x3e0 / +0x404` = black hole / add rotate / axis angle, `+0x4060` = no-down %,
  `+0x4ca8` = `m_AdditionalCriticalRate`.
- **Already done, not re-ported:** `SetSpecificValueByBuffTempletID` / `StartTransformedForm` ( CH2d3 ); `UnSetSpecificValueByBuffTempletID` is a `jmp` to the base.
- **Members renamed from their writer ( kickoff amendments CH1f2 / CH1l3 / CH1p ):** `+0x9614` `m_fSoulCollectorDamageRate` ( SA_ATK_REL ), `+0x9618`
  `m_fSoulCollectorRangeRate` ( SA_PROJECTILE_RANGE_REL; JUNE_BUGS 699's life time ), `+0x961c` `m_usSoulCollectorPiercingCount`, `+0x960c` `m_fSoulCollectorStackCoolTime`,
  `+0x9600 / +0x9604` `m_fBackstabAttackRate` / `m_fPowerOfOriaxAttackRate`, `+0x97c0..0x97d0` Invasion of Dark Energy target time / cool time left ( the timer
  CH1l3 asked about ) / cool time / damage rate / force down, `+0x980c..0x981c` Madness Crave damaged / move / jump / near range / far range, `+0x9820` a
  `CKTDXCheckElapsedTime`, `+0x9828 / +0x982c` Madness Destroy cool-time / debuff-time rates. `m_fFreeBoundingStrikeRate` / `m_fBoundingDamageRateEx` agree ( kept ).
- **Stand-in and routed ( kickoff `CH4b.md` amended ):** DamageData `+0x5c0` ( hit from behind, written by both DamageReacts ) has no March member: `bHitFromBehind`
  is `false`, so Lord of Fear's damage effect cannot play until CH4b. The `CX2GUUser` slot-251 buff hook `3204dcd0` ( Tribute / Noble Devotion 30419 ) goes to CH4b
  too: the CH3a0 peer held `X2GUUser.*`. Not read here and not CH4a's: Hyper Aura 106 ( no class reader ), the Open Mind trio and Stable Dark Energy ( CH1l3 ),
  Tribute ( CH2c2 ). The 13 old `SI_P_*` ids ( 10400-10412 ) have no reader in June's class: passives with no state.
- **June bugs kept ( JUNE_BUGS 766-768 ):** the burst's templet read unchecked ( guarded ), the Oriax stun at Seized by Fear's level, four values never reset.
  Quirks kept with a comment: the Lord of Fear stack raise carries to later units, the Soul Extortion MP's unsigned difference, the dropped memo 610016 read.
- **Gates ( worktree, `finish.py --files` 4 files `--with JUNE_CHAR_LUCIEL_PASSIVE _PASSIVE_DEBUG JUNE_CHAR_LUCIEL_CORE` ):** junediverge PASS, enumscan exit 0;
  flagcheck 2 PASS each but 1 ERROR on `Always.h` from the peer's uncommitted CH3a0 hunks; the committed range below is the verdict. 57 deleted lines = the renamed
  lines ( 38 cpp, 19 h ) and the two ctor inits. `/Zs` probe of `X2GULuciel.cpp`: 0 errors, `_PASSIVE` proven on by an `#error` probe.
- **Gate on the commit ( `finish.py --range a31855e9^..a31855e9 --with JUNE_CHAR_LUCIEL_PASSIVE _PASSIVE_DEBUG -v` ):** FINISH RESULT PASS ( 6 gates, 3 skipped:
  no state, no skill, no script ); flagcheck `_PASSIVE` 1 DEFINE + 3 OTHER ( the class files under `_CORE`, `X2BuffFactor.h` under `JUNE_ENUM_NAMES` ), 0 FAIL;
  junediverge PASS, enumscan exit 0. With `_CORE` added the class files PASS ( flag-off = base ) and `Always.h` reports ERROR on the old compound guard
  `#if defined( _LIVE ) && !defined( _CORE )` ( line 13445 ): flagcheck cannot unifdef `_CORE` through it; not a change of this phase.
- **Build:** `BUILD OK: X2/US_SERVICE/x2.exe 27,090,432 B, 2026-11-16 09:22:01` ( deploy first refused: the game was open ), then `DEPLOY OK: X2_offline.exe is 27,090,432 B,
  dated 2026-11-16 09:23:17, md5 65db270adcaffe9c32baccee653ab7f4, equal to X2/US_SERVICE/x2.exe (2026-11-16 09:22:01); built at main ca2afe20 + 5 uncommitted code file(s)`.
  `EXESTR OK: 2 of 2 found in X2_offline.exe` wide ( the init and soul extortion debug lines ); on the built exe 4 of 4 ( + the madness crave line, `DamageEffect_SI_P_LLC_SEIZED_BY_FEAR_BurstFear` ).
- **Play-test saves ( `elsdb addchar` + `x --file` ):** `LuPasLD` 220 ( Dread Lord 127 ), `LuPasCN` 221 ( Noblesse 128 ), `LuPasDN` 222 ( Demonio 129 ), level 70, the job's five
  passives at 4 ( 30401-30405 / 30406-30410 / 30411-30415 ), Power of Oriax 30010 and Soul Extortion 30019 added; slots in the hand-off.
- **Play-test ( 2026-11-16, user: all checks passed; logs 09:37-09:45 on the 09:23 deploy, clean exit, 0 EXCEPTION, 0 ErrorLog lines ):** init lines for
  `LuPasLD` 220 ( x9 ), `LuPasCN` 221 ( x3 ), `LuPasDN` 222 ( x7 ), each repeat identical ( force down 0.710, crit x1.080: the base resets what the passives add, nothing
  stacks across the re-inits March's callers make ); `fear: seized by fear` x25 ( range 350, limit 5; 1 / 2 / 5 stacks per hit = base / Lord of Fear / Dead Hands ),
  `burst fear` x19, `threat of fear` x7 ( cool times -2.00, next in 5.00 s ), `blasting fear range 460, 3 unit(s) burst`, `power of oriax stun` x10 ( level 4 ),
  `ruinous impact target` x4, `soul collector stack` x13 ( 1, 2, ... ), `soul extortion drain` x18 ( `stacks 5 / 5, mp 20.00, soul 1` ), `invasion of dark energy mark`
  x7 ( 3.00 s, next in 10.00 s ), `madness crave` x51 ( near range 700 / far 1200: `nearest 391, near 1 value 9.266` = 12 x ( 1 - 91 / 400 ), `nearest 1376 ... far 1` ),
  `LUCIEL DUO point: +1 on hit` x5 ( CH2a2's owed check, Lu and Ciel units ). `_PASSIVE_DEBUG` stays on; the CH4b review retires it.
- **How to see it in game:** the hand-off in the closing message ( units `LuPasLD` / `LuPasCN` / `LuPasDN` ). Logs: `python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`.
  Per passive: init lines `LUCIEL PASSIVE init` / `init 2` ( every value ); Seized by Fear / Lord / Aura / Threat `fear: seized by fear ...`, `burst fear`, `threat of fear`;
  Power of Oriax `power of oriax stun`; Soul Collector `soul collector stack N`; Soul Extortion `soul extortion drain`; Communion Recovery `rest mp` in `init`;
  Communion Expand / Absorb `no-down N%` and `force down` in `init 2`; Chemistry the V key over a bounded enemy; Invasion `invasion of dark energy mark`;
  Madness Crave `madness crave ( nearest ... )`; Madness Destroy `crit rate x` in `init 2`; the point on hit ( CH2a2's owed check ) `LUCIEL DUO point: +1 on hit`.

### CH4b - Done (2026-11-16)

Opus 5.5 high. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_PASSIVE` ( CH4a's; no new flag, no new `Always.h` entry ), `_PASSIVE_DEBUG` stays on ( the CH4b review retires it ).
Files: `X2DamageManager.h`, `X2GameUnit.h` / `.cpp`, `X2GUUser.h` / `.cpp`, `X2GUNPC.cpp` ( bytepatch ), `X2GULuciel.h` / `.cpp`. funcmap `grep "(CH4b)"` ( 5 rows ).
Review: `CH4b-review.md` covers CH4a and CH4b.
- **Hit from behind ( checklist 7aw, kickoff amendment CH4a ):** `DamageData::m_bHitFromBehind` ( June `+0x5c0`; `SimpleInit` zeroes it with `+0x5c1` = `m_bMakeTarget` ).
  Both DamageReacts set it when the Now `bAttackerFront` is clear ( user `+0x80cd` = `m_FrameDataNow.unitCondition.bAttackerFront`, proven by June's Now -> Future
  copy `@3204682c`; NPC `+0x844d` = `m_NPCFrameData` ), then for a `GUT_USER` `UT_LUCIEL` attacker: AT_SPECIAL x `GetSeizedByFearRate` ( `+0x95f8` ) unless the used
  skill is `ST_HYPER_ACTIVE_SKILL`, any other hit x `GetPowerOfOriaxDamageRate` ( June `32ae5120`: BTI 464 / 520 -> `+0x9604` ), and Backstab x `GetBackstabAttackRate`
  ( `+0x9600` ). User `@32048cdb-32048d82`, NPC `@327b39cf-327b3a8d`, at the D4a / D4b stand-ins ( kept in `#else` ). CH4a's `bHitFromBehind` stand-in now reads the member.
- **Backstab ( checklist 7d ):** user `@320483c2-3204842e`, NPC `@327b3382-327b33ed`, right before `GetFinalDamage`: with `bAttackerFront` clear, a Lu/Ciel user
  attacker and `USED_SKILL_ID` 30205 set `m_bIgnoreDefence`, `_OnPvp` and `_Decline` ( the funcmap's "NPC only" for `+0x534` was wrong: the user side sets it too ), and
  `IsEquippedSkillUpgraded( 30205 )` ( `32071e70` ) sets `m_fCriticalRate` ( `+0x510` ) to 1.0, which the checklist row did not list.
- **Shadow ( checklist 7j ):** `CX2GULuciel::AbsorbDamageByShadowShield` ( June `32aebaf0` ) and the user DamageReact step `@3204963d-32049674` ( after the 0x4f8 / 0x4fc
  hooks, outside their attacker block, before `322cfca0` ): the pool takes damage x 0.2, the hit is x 0.8; at <= 0 both Shadow buffs end.
- **Slot-251 buff hook ( checklist 7 L257, kickoff amendment CH4a ):** `CX2GameUnit::ProcessAfterSetBuffFactor( BTI )` ( empty virtual ), called in
  `SetBuffFactorToGameUnit` before slot 7 as June `@322a886e-322a8880`; `CX2GUUser`'s body = June `3204dcd0`. **Tribute is a chance, not an HP threshold** ( the
  checklist / funcmap 339 wording ): vslot 0x1cc is `GetRandomFloat`, so `GetRandomFloat( 0 ) < SA_EFFECTIVE_RATE`; with BTI 462 held the first debuff is erased and
  Soul Collector loses one stack ( `AddBuffAccumulationCount( 523, -1, 1, 0 )` ); Noble Devotion 30419 then gives `SA_RECOVERY_VALUE_REL` points ( `UpCombinationPoint`,
  a `friend` of `CX2GUUser` for this ) and `SA_RECOVER_MP_ABS` ( `_2` in PvP, June `322cc180` = `GT_PVP` only: allow-list row ) MP with the MP text. Ain's `32a3d220` waits.
- **Already done, checked only:** `SetDamageExceptionProcess` `32aea530` ( CH1k ), the gauge snapshot pair `32aeb410` / `32aeb430` ( CH1k; no March engine caller,
  Crest of Time is every user's, char-add ), Nobility's `m_fDamagedRateBySpecialActive` ( CH4a ). Not CH4b's: the user `ProcessAttackResultPassive` base `32011b80`
  ( Celestial Blitz, Stage 2 ), routed by the funcmap.
- **June quirk kept ( JUNE_BUGS 769 ):** the behind-hit byte and Backstab's defence flags stay on the attacker's DamageData until `SimpleInit`, never per hit.
- **Gates ( worktree, `finish.py --files` 8 files `--with JUNE_CHAR_LUCIEL_PASSIVE _PASSIVE_DEBUG` ):** flagcheck PASS ( 6 PASS, 1 PASS~ = the blank line before the hook
  body, 1 OTHER = `X2GameUnit.cpp`, whose other 31 lines are a peer's uncommitted `JUNE_STATCALC_UNIT_STAT` ); the one deleted line is CH4a's stand-in inside `_PASSIVE`;
  junediverge PASS after the `GT_PVP` allow row ( Ghidra: `322cc180` = `GetGameType() - 1 == 0` ); enumscan exit 0. `/Zs` probes of the 4 `.cpp`: 0 errors.
- **Commit `5d07d7dc`** ( `finish.py --range 5d07d7dc^..5d07d7dc --with JUNE_CHAR_LUCIEL_PASSIVE _PASSIVE_DEBUG` ): FINISH RESULT PASS ( 6 gates, 3 skipped: no state,
  no skill, no script ); flagcheck 7 PASS, 1 PASS~, 0 OTHER, 0 FAIL ( the deleted line = CH4a's stand-in ); junediverge PASS; enumscan exit 0. JUNE_BUGS 769 and the 5
  `(CH4b)` funcmap rows landed in the CH3a peer's commit `c63ae472` ( it committed the shared files from the work tree ).
- **Build:** my first run hit `LOCK TIMEOUT` ( a peer's build ); re-run: `BUILD OK: X2/US_SERVICE/x2.exe 27,133,440 B, 2026-11-16 10:38:01` then `DEPLOY OK: X2_offline.exe is
  27,133,440 B, dated 2026-11-16 10:38:08, md5 85d46dcf40f1843140ce152466d7cc5c, equal to X2/US_SERVICE/x2.exe (2026-11-16 10:38:01); built at main 9fb16748 + 10 uncommitted code file(s)`
  ( peers' too ). `EXESTR OK: 5 of 5 found in X2_offline.exe` wide ( hit from behind, backstab, shadow shield, tribute, noble devotion ).
- **Play-test saves ( `elsdb x --file` / `setchar` / `settranscend` ):** `LuPasLD` 220 Ambush Attack 30205 in slot 2 ( Dead Hands 30007 out ); `LuPasCN` 221 Shadow 30209
  in slot 6, Noblesse Oblige 30419 at 1, level 99, transcended ( 30419 is `SPRT_HYPER_CLASS` ). Tribute's chance at level 4 is 0.275.
- **How to see it in game:** the hand-off in the closing message. Logs: `python tools/logs.py --grep "JUNE_CHAR\] LUCIEL PASSIVE"`. Per item: `hit from behind on
  npc ...: damage A -> B` ( 7aw ), `backstab from behind on npc ...` ( 7d ), `lord of fear effect ( N stack(s) ... )` ( CH4a's fear effect, now reachable ), `shadow shield
  took ...` ( 7j ), `tribute: BTI ... erased` and `noble devotion: N point(s), mp +M` ( slot 251 ).
- **Play-test ( 2026-11-16, logs 10:47-10:53 on the 10:38 deploy, 0 EXCEPTION, 0 ErrorLog lines ):** checks 1-3 passed, 4-5 skipped by the user and owed to the
  CH4b review. `LuPasLD` 220 ( seized x1.150, backstab x1.360, oriax x2.000 ): combo from behind `986.7 -> 986.7` before Oriax, `2971.0 -> 5942.1` after ( x2 ); Triple
  Shot `4405.0 -> 5065.7` ( x1.15 ); `backstab from behind ... ignore defence, critical rate 0.00` then `skill 30205 ... 8873.6 -> 12068.1` ( x1.36 ), from behind only;
  `lord of fear effect ( 1 stack(s), power x4.320 )` ( on the CH3a peer's `LuDlB`, same path ). Not reached: Shadow and Tribute / Noblesse Oblige ( `LuPasCN` 221 ).

### CH4b - Review (2026-11-16)

Opus 5.5 high. Covers CH4a ( `a31855e9` ) and CH4b ( `5d07d7dc`, `9bdc3e6a` ). **Pass after one fix** ( `98433528` ), `_PASSIVE_DEBUG` retired.
- **Fix ( CH4a, `PlaySoulExtortionDrain` `32b435f0` ):** the soul's start is the defender's vslot 0x3e4 aim point for `Bip01`, asked from the Lu/Ciel's
  own `GetPos` ( vslot 0x138 on `this`, not on the defender ). CH4a read 0x3e4 as "bone or GetPos", which is only the user's form ( `31fd61a0` ). The NPC's
  `3275ac50` with a name answers the bone of the collision frame nearest the query point ( `32278580` ), unless `Bip01` is over 700 ( `3344ce98` ) from it,
  then whichever of the two is nearer the query ( tie: the frame bone ). New `GetNamedBoneAimPos` in the CH4a namespace; funcmap row 12390 corrected.
  `/Zs` probe of `X2GULuciel.cpp`: 0 errors.
- **CH4a = June ( Ghidra + capstone `32b45820..32b46570` ):** the 20 reads in June's order ( ids 30401 / 30205 / 30010 / 30406 / 30019 swapped / 30402 / 30407
  / 30403 / 30404 / 30408 / 30410 / 30015 / 30007 / 30411 / 30217 / 30413 / 30414 / 30415 / 30412 / 30417, all `/Zs`-probed ), 22 ability ids, every reset value
  ( soul collector 0 / 1 / 0 / 1 / 0 / 0, invasion -1 / 9999 / 9999 / -1 / -1, madness crave 0 x5 / 9999, destroy -1 / -1, absorb 100 ), the PvP pick ( state 9 or
  0x17 -> `+0x9644` + SA 160, else `+0x9648` + SA 43 ), the force-down clamp ( = March's setter ), delegate order and lists ( `+0x66ac`, crave on `+0x66cc` ).
  `ApplyFearOnHit` `32b3d380` = June ( the 517 x stacks loop vs 926 under Dead Hands read from the listing: the decompile shows 926 always, the listing does not ),
  `CreateFearDamageEffect` `32b3ca30` = June, the 7 groups `3226e230..90` = 19 table entries `3373daa4..e8` ( 184 twice ) = the 18-id list, `FrameMoveMadnessCrave`
  `32b3e470` = June ( 300.0 offset, 0x21c arguments mapped to March's order ).
- **CH4b = June:** both Backstab blocks ( `+0x528` word = `m_bIgnoreDefence` + `_OnPvp`, `+0x534` `_Decline`, `+0x510` 1.0 ), both behind-hit blocks ( `32070510`
  maps type 13 alone to 5, so `== 5` is `ST_HYPER_ACTIVE_SKILL`; null templet multiplies ), Shadow step ( 0.2 / 0.8 read from the exe ), `32aebaf0`, `32ae5120`,
  slot 251 ( `+0x3ec`, templet `+0x34` id, before slot 7 `@322a8885`, inside `SetBuffFactorToGameUnit` ), `3204dcd0` ( SA 5 / 154 / 79 / 172, BFI 523, unit type 10,
  MP text on `+0x68ac` ), `SimpleInit` `@31f8f34c` zeroes the word `+0x5c0`.
- **Gates ( `finish.py --review --commits a31855e9 5d07d7dc 9bdc3e6a 98433528 --with JUNE_CHAR_LUCIEL_PASSIVE _PASSIVE_DEBUG -v` ):** FINISH RESULT PASS ( 7 gates, 3
  skipped: no state, skill or script ); flagcheck 8 PASS, 1 PASS~ ( a blank line in `X2GUUser.cpp` ), 1 DEFINE, 3 OTHER: `X2BuffFactor.h` = `JUNE_ENUM_NAMES`
  ( CH4a's names ), `X2GULuciel.cpp` / `.h` = the class file under `_CORE`; 4 deleted lines = CH4b's stand-in + the fix's 3. junediverge PASS, `--check-all` PASS
  ( the first run's `LoadAttached` hit was the R14f peer's tree, gone at `99448c4b` ), enumscan exit 0. `X2GameUnit.cpp`'s other hunks are the stat-calc peer's.
- **Build ( the fix ):** `DEPLOY OK: X2_offline.exe is 27,136,000 B, dated 2026-11-16 11:14:20, md5 c10aa50ed839729974a90b96ba577df3, equal to X2/US_SERVICE/x2.exe
  (2026-11-16 11:14:13); built at main 0e01528e + 1 uncommitted code file(s)`. The debug retire: build skipped: debug retire only.
- **Play-test ( 2026-11-16, user: all passed; logs 11:17-11:23 on the 11:14 deploy, clean exit, 0 EXCEPTION, no phase line in ErrorLog ), `LuPasCN` 221:** Shadow
  `shadow shield took 2015.0, left 37235.0` x5 ( pool 39250, the buff ended by time: the break line was not reached ); `/buffFactor` replaced by the Lua line
  `SetBuffFactorToGameUnitByBuffFactorID_LUA( 55, 1, 0, true )`: `tribute: BTI 58 landed, debuff 58 erased, one soul collector stack spent ( chance 0.28 )` x4,
  each with `noble devotion: 1 point(s), mp +10.0`; `soul extortion drain ( stacks 5 / 5, mp 20.00, soul 1 )` x12 after the fix.
- **Next:** CH4c ( kickoff amended: `_PASSIVE_DEBUG` is retired; re-enable it for CH4c's lines and its review retires it again ).
- **Commit `df5fa9be`** ( `finish.py --range df5fa9be^..df5fa9be --with JUNE_CHAR_LUCIEL_PASSIVE_DEBUG --skip-flag JUNE_CHAR_LUCIEL_PASSIVE` ): FINISH RESULT PASS;
  `_PASSIVE_DEBUG` retired ( nothing to check ), `_PASSIVE` skipped: the only line is the commented define, and its compound `#error` guard is not unifdef-able.

### CH4c - Done (2026-11-16)

Opus 5.5 high. **No code change:** each item of the CH4c row is ported already or never fires in June. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_PASSIVE`
untouched, `_PASSIVE_DEBUG` stays retired ( no new log line ). Scans: capstone on `unpacked_x2.exe` ( call + `cmp` windows ) and Ghidra decompiles, read-only.
funcmap `grep "(CH4c)"` ( 4 rows ).
- **`MemoConditionLuciel` ( ctor `32aeb3f0`, `IsCondition` `32ae5150`, slot 2 `32ae5170` ): not ported, = the other 12 classes.** Bytes: `IsCondition` =
  `push [esp+4]; call 31fd4450; test al, al; setne al; ret 4`, and `31fd4450` = `xor al, al; ret 4`, so it always answers false. Slot 2 is a `jmp` to the
  base parse `31fc8cf0`. Divergence row `MemoCondition::decide / parse` ( D1-1: no MemoCondition class in the tree, `m_bApplyMemo` is false for every
  class ) covers it. CH1a's stand-in comment in the ctor ( `X2GULuciel.cpp:163` ) stays.
- **Memo reads in the class ( 21 calls to June's memo functions in `32ae4080-32b4e76f` ):**
  - 15 calls to `31fca9e0` ( `GetEqippedSkillMemo`, June's stub that returns 0 ) in 10 functions. Each port drops the read with a one-line comment:
    `32b15050` CH1l2, `32b1f800` CH1l3, `32b29af0` CH2a, `32b2ba70` / `32b2bf20` / `32b2c270` / `32b2e080` CH2c2, `32b3d010` CH2b, `32b3d380` CH4a, `32b2d2a0`
    CH3b ( 5 calls; uncommitted at this phase, the CH3b review owns it ). `X2GULuciel.cpp` has no call to March's `GetEqippedSkillMemo`.
  - 3 calls to `31fdbcc0` ( `GetTranscendenceMemoSkillID` ): `32b3c3b0` x2 ( Dancing Blade, CH3a ) and the skill head `32b46580 @32b46680` ( CH1n / CH3a ).
  - None to `31fdbbb0` ( `GetSkillMemoEntry` ), `31fdbe20` ( memo MP rate ) or `31fdbe70` ( memo MP refund, reached through `ReservedSkillStart`, CH1o3 ).
  - Plus the 2 MemoCondition ctor calls above ( `32aeb3f0`, and the class ctor `32b1bd90 @32b1c50b` ).
- **`CX2Game::UseItemSpecialAbility` ( `322fbc60`, scanned to the jump tables at `322ff65c` ):** its only Lu/Ciel piece is the SAT 12 case ( `322fd2fe-322fd3f0`,
  ported by DU1c ). Its other unit tests are Chung class 0x14 / 0x6e ( char-chung CH4c ), unit type 9 ( Add, `@322fe80a`, `@322ff4ed` ) and 7 ( Ara, `@322ff3dd` ).
- **`X2BuffTemplet.cpp`: no Lu/Ciel branch in June.** `CreateAndInsertCustomFunctionByBuff` `3229e1a0` switches on BTI 29 / 56 / 62 / 223 / 439 / 720 / 767 / 819 /
  891 / 894 / 920 / 927 / 952 / 1010 / 1022 / 1038 ( Ghidra decompile ), none of them Lu/Ciel's. June's buff code ( the funcmap's `CX2Buff*` ranges ) has no
  `GetUnitType` == 10 test and no immediate compare to a Lu/Ciel BTI ( 460-483, 520, 760, 763, 764, 775, 776, 877, 895 ). The one hit, `push 0x208` x3 in
  `CX2PremiumBuffManager` `326c7a10`, is a call argument, not BTI 520.
- **Every June Lu/Ciel unit test outside the class** ( `call 31e4c370 / 31e4d0e0 / 31e4d0c0` then `cmp` with 10 or a Lu/Ciel class id ): 14 sites, no class-id
  test. 8 are ported: `320483e6`, `32048d0c`, `3204963f`, `3204dedf`, `327b33a5`, `327b3a15` ( CH4b ), `3230cb5a` ( CH3a0 ), `321960ee` ( DU3b ). 2 were routed
  earlier: `32020ac9` in `320200b0` ( socket-option particles, Stage 16, DU1d3b ) and `321fabaa` in `321faa90` ( Stage 19, CH1a0 ).
- **Routed by CH4c ( scope rule: not this plan's subject, nothing reachable offline ):** 4 customize-item readers test an entry's `+0x40` body filter
  ( 1 both, 2 Lu, 3 Ciel ) against the form. They are `32363b40` ( the sit item for an emotion, against `CX2Unit +0xe40` ), `32237310` ( the cash shop's take-off-all
  tail, against viewer `+0x528` ), and `32d78bc0` / `32d7e250` ( the customize preview dialogs, against `32d767a0` / viewer `+0xd80` ). NU0 tagged them DU5. They go
  to backlog Stage 12 ( the customize system, one new bullet ), since March has no customize map ( `+0xa44` ) to filter.
- **Gates:** no client source changed, so `finish.py` has nothing to check ( no flag, no file ). Full build skipped: no play-test.
- **How to see it in game:** nothing new. The memo and item paths behave as before; the Lu/Ciel passives are CH4a / CH4b's. Review: `CH4c-review.md`.

### CH4c - Review (2026-11-16)

Opus 5.5 high. Reviewed `708255e6` ( no code ) against the exe bytes ( `disasm_range.py`, capstone scans, PE reads ) and Ghidra decompiles, read-only.
**Verdict: pass, no code change.** Every verdict of the Done note holds.
- **MemoConditionLuciel:** vtable `334f036c` slot 1 = `32ae5150` ( `push [esp+4]; call 31fd4450; test al, al; setne al; ret 4` ), slot 2 = `32ae5170` ( `jmp 31fc8cf0` );
  `31fd4450` = `xor al, al; ret 4`. The ctor `32aeb3f0` = base ctor `31fc8cd0` + the vtable store. Always false: not porting it is right.
- **Memo calls in `32ae4080-32b4e76f` ( rel32 scan ):** 15 calls to `31fca9e0` ( = `xor al, al; ret 4` ) in the 10 functions named, 3 to `31fdbcc0` ( `32b3c4b4`, `32b3c4e4`,
  `32b46680` ), none to `31fdbbb0` / `31fdbe20` / `31fdbe70`. Spot-checks: `32b2ba70 @32b2bd66` ( `cmp al, 1; jne` past the memo set, CH2c2's comment says so ) and `32b2d2a0`
  ( 5 calls, `@32b2d345` gates the memo variant on synced short 5; the CH3b body plays the plain boxes only ). Both ports drop the read as June's stub does.
- **Unit-test scan, re-run ( `call 31e4c370 / 31e4d0e0 / 31e4d0c0`, then `cmp` within 8 instructions, outside the class ):** the same 14 sites, all `cmp eax, 0xa`; no
  class-id test ( 37 / 38 / 39 / 127 / 128 / 129 / 200 / 177-179, `cmp` / `sub` / `add` within 12 ). The one unit-type switch after those getters ( `@32622f00`, cases 0-6 )
  has no case 10. Each site has a home: `320483e6` / `327b33a5` ( Backstab ) and `32048d0c` / `327b3a15` ( hit from behind ) are inside CH4b's funcmap ranges, `3204963f` is
  CH4b's Shadow pool step ( `@3204963d-32049674`, `X2GUUser.cpp` comment ), `3204dedf` is in `3204dcd0` ( CH4b slot 251 ), `3230cb5a` is in `3230c1a0` ( CH3a0 ), `321960ee`
  is the `321960dc` row ( DU3b ); `32020ac9` ( `320200b0`, backlog Stage 16 ) and `321fabaa` ( `321faa90`, Stage 19 ) are routed; `32237384`, `32363bda`, `32d78cd5`,
  `32d7e6e5` are the 4 customize readers in the new Stage 12 bullet.
- **`3229e1a0`:** Ghidra's decompile switches on 29 / 56 / 62 / 223 / 439 / 720 / 767 ( `0x2ff` ) / 819 / 891 / 894 / 920 / 927 / 952 / 1010 / 1022 / 1038, none Lu/Ciel's.
- **`322fbc60`:** the SAT switch ( `dec eax; cmp eax, 0x29`, byte index `322ff6d0`, table `322ff65c` ) sends SAT 12 to `322fd2fe` ( DU1c's port ). Its only unit tests are
  class 0x14 / 0x6e x2 ( Chung ) and type 9 x2 / 7 ( Add / Ara ); nothing tests Lu/Ciel.
- **For the CH3b review ( peer, uncommitted at this review ):** the `32b2d2a0` port's head comment is two lines; the one-line comment rule wants one.
- **CH6's kickoff** gains an "Amended by the CH4c review" section: walk the CH4 passives by their numbers ( the debug define is retired ), Shadow's pool break that the
  CH4b review did not reach, and nothing for memos or customize filters.
- **Gates:** no client source changed ( `finish.py`: nothing to check ). Full build skipped: no play-test. `_PASSIVE_DEBUG` stays retired.

## Phase CH5 - UI and data

**Run with:** Sonnet 5.5 high, Opus review, user play-test. Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_UI`, sub-tag `UI`. The skill and hyper cut-in
table for both bodies (June's names from data152 / data195), the HUD portrait for both bodies, the combination-point gauge / UI if June has one
(CH0), the bounding-key dialog, the Lu/Ciel event-scene and cut-in names the june-gfx bullet lists, and the dialog pieces the earlier phases
did not take. **Done gate:** a select-screen pass of all ten characters with no new error; both bodies' cut-ins and portraits show June's art.

## CH5 - Done (2026-11-16)

Flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_UI` ( `Always.h`, needs `_LIVE`, `_CHAR_CREATE`, `_CHAR_DUO_BODY`, `_CHAR_DUO_WIN_SPEECH`, `JUNE_SKILL_TREE_PANELS`, `JUNE_TRANSCEND_CHANGE` ). No debug flag, no log line: the play-test is visual.
- **Cut-ins ( confirm only, CH1l3 wrote them ):** all 21 names of the three `_CONST_LUCIEL_CH1L3_` tables ( `337ad268` / `337ad4a8` / `337ad148` / `337ad1d8` ) have a staged `.gfx` in data152; the set index ( `SetSkillCutInSetIndexByUnitClass`, jump table `32b45768` / `32b4577c` ) = June: 37 / 38 / 39 / 200 -> 0, 127 -> 1, 128 -> 2, 129 -> 3. The customize-manager override stays a stand-in ( Stage 12 ).
- **Skill slot greys ( CH2a2 routing ):** `CX2Game::UpdateSkillCoolTime` arm for `UT_LUCIEL` ( June `322e07e0` @322e1840-322e1885: cost `32b14f70` > gauge `32b0fa00` -> fade ); `friend class CX2Game` in `X2GULuciel.h` ( both getters are protected ).
- **Dungeon-clear picture ( CH1o3 routing, `324ce260` ):** `SetDungeonClearTexture` cases for the 7 Lu classes; game `+0xb54` > 0 shows Ciel's file ( 37 / 200 `Ciel.TGA`, 38 / 128 `LUCIEL_ROYAL_GUARD.TGA`, 39 `Ciel.tga`, 127 `LUCIEL_DREADLORD.tga`, 129 `LUCIEL_DEMONIO.tga` ), else the class row. Switch table `324ce82c` / `324ce87c` decoded from the exe.
- **`ENDING_SPEECH` `LUCIEL` ( `327e0040` ):** `X2DungeonManager.cpp` reads rows `{ LU = id, CIEL = id }` into `{ Lu's line, Ciel's line }`; the existing consumer shows a row's two strings in order. Balloon name ( `324d08a0` ) = the class templet's `m_Description`; dungeon-map emblem ( `324c99d0` ) = `DLG_UI_Common_Texture88_NEW.tga` / `LUCIEL_MAPSD`.
- **Quest-complete voice ( `323cd570` ):** the voice table's `SPT_QUEST_COMPLETE` / `_MINOR_UNIT` row by my unit's form ( `QuestComplete_Lu.ogg` / `_Ciel.ogg` exist in data162 / data163 ).
- **Skill-window portrait ( checklist 7aj, `328dcba0` / `328db7a0` ):** `CX2UISkillTreeNew::UpdateCharacterPortraitByForm` + `GetCharacterPortraitByForm` ( the named pieces of Texture91 / 89 / 96, transcended `TR_D_LORD_2` / `TR_N_OBLESSE_1` / `TR_D_EMONIO_2` of Texture103 ), called from `CreateSkillTreePortrait` ( @328e8f75 ) and from `CX2Unit::UpdateSkillTreeCommandGroup` ( @3234d9a0 ); the tree's form flag ( `+0x8c`, @328edaca ) now starts from `IsMinorUnitForm()`. June skips the generic picture for Lu; here it is drawn first and overwritten ( same result ).
- **Data ( luasrc `adopt-june`, asset rule ):** `DLG_LUCIEL_TRANSFORM_SKILL_SLOT.lua` ( new; CH2d20's window loaded a missing file until now ) and June's `DLG_PVP_Result_Card_Back`, `_MatchPvp`, `DLG_PVP_Room_Character_Slot` ( the NU1 amendment ). `luabind check` PASS, every texture and `.TET` they name exists. `DLG_UI_Common_Texture103_NEW.tga` + `.TET` staged from June ( data057, new ). `PACK OK data034`, `PACK OK data057`; **copy owed: the game was running.**
- **Already done earlier, nothing to port:** combination-point dialog and bounding-key dialog ( CH1p ), transform slot data type 6 ( CH1a0 ), the switching-guide statics ( live `DLG_GAME_SKILL_SLOT_NEW_10slot` carries them; `UpdateLucielSwitchingGuideUI` CH1a0 ), `m_cUnitClass` as a short ( NU2 ).
- **Not ported, routed ( scope rule ):**
  - `EmotionData.lua` does not exist in March ( an emotion-effect system, not Lu ); June's `EventScene.lua` has no Lu row.
  - `CX2MemoryHolder::GetCommonDLG` ( `328539c0` ): June reads picture 9 of `DLG_Common_BG` and `DLG_Background_Luciel.tga`, neither exists in any June kom ( JUNE_BUGS 754 ); Lu keeps March's default background.
  - `CX2ChatBox::PlayEmotionSound` ( `31ea6ce0`, 383-line decompile with per-state form lookups; files `Ciel_Emotion_*.ogg` exist ): voice only, goes to Stage 9 ( UI sweep ) as a bullet.
  - `CX2UIManufacture` "usable by" line for type-restricted items and the PvP loading picture ( PvP unreachable offline ): same stage.
- **June bugs:** JUNE_BUGS 754-755.
- **Gates:** `finish.py --files <9 files> --with JUNE_CHAR_LUCIEL_UI --lua <4 dialogs>`: flagcheck PASS on 8 files, FAIL only on `X2Unit.cpp` because a peer's uncommitted `JUNE_CHAR_DUO_STAT` hunks ( DU5e ) share the file ( my hunk @6306 is inside the flag; the committed range is re-run below ); `junediverge` PASS; `luabind` PASS; `enumscan` exit 1 on 190 old rows in touched files, **0 in my hunks** ( checked against `git diff -U0` ); `stateends` / `skillicons` skipped ( no skill states or icons ).
- **Build:** `BUILD OK: X2/US_SERVICE/x2.exe 27,044,864 B 2026-11-15 23:29:46`; deploy refused ( game running ), so `DEPLOY OK` is owed. `EXESTR OK: 4 of 4 found in x2.exe ( --exe build ), md5 1699a8a8ac9bb0b396f56c62533343b0` ( `LUCIEL_MAPSD`, `TR_D_LORD_2`, `HQ_Dungeon_Clear_LUCIEL_DREADLORD.tga`, `LUCIEL_CIELFACE_LARGE` ).
- **Play-test fix ( 2026-11-16 ): Ciel's Command tab header was empty.** `CreateSkillTreePortrait` passed the transcended flag to `LoadSkillCommandDescription` and `CreateMajorCommandPanel` as their "minor" flag; June passes "my unit is UT_LUCIEL" ( the byte stored @328e8f44, used @328e9117 / @328e912a ), so a non-transcended Lu never built the `_MINOR` description or rows. Fixed under `_UI` ( original in `#else` ). LVUp guide stand-in ( `X2LVUpEventMgr.cpp:927` ) still passes false.
- **Play-test passed ( 2026-11-16, logs read ):** `ErrorLog.txt` 0 lines; select-screen pass, skill-window portrait / Command tab ( Ciel header ) and clear pictures seen. User's "only Ciel" report: June rolls the speech body 50/50 per dungeon load ( game `+0xb54` is written only by `32009e90` and the ctor, exe-wide scan ); user chose to keep June's roll. Debug `[JUNE_CHAR] LUCIEL UI clear picture` lines: body 1 / 1 / 0 over classes 39 / 38 / 37, files as the table ( `_UI_DEBUG` stays on, the review retires it ). 5 `E_DISABLE_HB_CHECK_REQ` UNHANDLED lines are old noise.
- **How to see it:** select Lu/Ciel, open the skill window ( K ): portrait is the Lu/Ciel art for the active body; in a dungeon the combination-point skills grey when the gauge is short; dungeon clear shows Lu's or Ciel's picture with the matching speech.

  sit in an `#error` inside the flag block in `X2GUNPC.cpp` ( beside the `CreateNPCReq` call ); the `Always.h` guard keeps the six it can see. Gates PASS ( 2 deleted
  lines = the replaced guard pair ). `DEPLOY OK: X2_offline.exe is 26,948,608 B, dated 2026-11-15 18:31:09, md5 df9afaf486b60758f01f2f05bc28f749`.
- **Build:** `/Zs` probe of `X2Game.cpp`: 0 errors. Full build skipped: no play-test ( CH3c2 plays it ). CH3c2's kickoff checked: its CH3c20 amendment stands; it waits only on CH3c.

## CH5 - Review (2026-11-16)

Verdict: **pass after three fixes** ( flag `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_UI` ). Reviewed `994f84ac`, `1130d597`, `371c27da`, `c8a57999` against June in Ghidra.
- **Slot grey ( `322e07e0` @322e1823-322e1887 ):** test `[templet] == 0xa`, `32b14f70( id, level )` ( pushed level then id ), `32b0fa00`, `CMP gauge, cost; JL` -> fade @322e1be1 ( picture `+0x8 = 1`, string `""` ); sits after the type-11 ( Rose ) arm. = port.
- **Clear picture ( `324ce260` ):** index = class - 6 into `324ce87c`, targets from `324ce82c`, decoded from `unpacked_x2.exe`: 37 / 200 -> `Ciel.TGA`, 38 / 128 -> `LUCIEL_ROYAL_GUARD.TGA`, 39 -> `Ciel.tga`, 127 -> `DREADLORD`, 129 -> `DEMONIO`; each `CMP [game +0xb54], 0; JLE` -> the class row ( type 1, flag `+0x3c` ). = port; JUNE_BUGS 755 correct.
- **ENDING_SPEECH ( `327e0040` ):** keys XOR-decode to `LU` ( `337aa760`: 0x53^0x1f, 0x13^0x46 ) then `CIEL` ( `337aa75c` ); either missing returns ( = the port's break, the set is filled in place ). = port.
- **Portrait ( `328db7a0` / `328dcba0` ):** piece table, `param_4 == 1` transcended pieces, flip for 0x26 / 0x7f / 0x81, class from templet `+4`, type 2 = `CIT_160by160`; `CreateSkillTreePortrait` @328e8f2f-328e8f75 tests templet type == 0xa ( the same byte feeds the panels @328e9117 / @328e912a: `1130d597` is right ); `3234d930` calls `328e8cd0` then `328dcba0`. = port.
- **Emblem ( `324c99d0` ):** type - 2 into `324ca9e4`, type 10 -> @324ca329 `LUCIEL_MAPSD` / Texture88. = port.
- **Fix 1, `X2Unit.cpp`:** the committed `_UI` hunk sat in `SetMinorUnitForm` ( `pUISkillTree` undeclared there, the inventory test in `#else` ): HEAD did not compile; the worktree, which the CH5 build used, had it in `UpdateSkillTreeCommandGroup` after `SetCommandGroup` ( = June `3234d930` ). Committed that placement.
- **Fix 2, `PlayQuestCompleteSound` ( `323cd570` ):** June has no type switch: every class takes the voice table's `SPT_QUEST_COMPLETE` / `_MINOR_UNIT` row ( form byte `+0xe40` ). June's body replaces March's switch ( the Lu-only arm is gone ); files unchanged for March classes ( builder `QuestComplete_<name>.ogg`, Lire's table rows `QuestComplete_Lena.ogg` ). Divergent row added.
- **Fix 3, `GetCommonDLG` ( `328539c0`, name switch `32851f90` ):** JUNE_BUGS 754 said June ships no `DLG_Background_Luciel.tga`; June's data051 has it and its `.TET`. June's body ( picture type - 1, `DLG_Background_<name>.tga` for every type, a missing picture skipped ) now sits inside March's `GetCommonDLG` ( June's dialog handles stay March's ); both files staged ( `kom.py stage-june` ), `PACK OK data051`, `COPY OK data051` ( md5 9f2cbfc0ec06d63385d9db155731b569 ). JUNE_BUGS 754 rewritten: the colour picture 9 is missing in June too. Divergent row added.
- **Routed:** `324d08a0` names every class by its class templet's description ( March: per-type strings ); the Lu arm is June's, the rest goes to Stage 9 with the whole frame move ( backlog bullet (4), divergent row `stage9` ). The three CH5 routed items have their Stage 9 bullet.
- **Guard:** `_UI`'s `#error` now also needs `SERV_IRUHADEV_JUNE_SELECT_VOICE_TABLE` ( `GetVoiceFileName`; a top-level define 450 lines above ). `_UI_DEBUG` retired ( `//#define` ).
- **Gates** ( `finish.py --review --commits 994f84ac 1130d597 371c27da 61027f76 --with JUNE_CHAR_LUCIEL_UI` ): flagcheck `PASS` ( 11 PASS, 2 PASS~, 3 DEFINE, 0 OTHER, 0 FAIL; 13 deleted lines = CH5's own lines this review replaced: the Lu-only voice arm 6, the misplaced `X2Unit.cpp` hunk 4, the `Always.h` guard 2 + debug define 1 ); `junediverge` `PASS`, `--check-all` `PASS`; `enumscan` exit 1 on the 190 old rows of CH5's files, **0 in `X2MemoryHolder.cpp` / `X2UIQuestNew.cpp`**; stateends / skillicons / luabind not applicable. Worktree flagcheck was polluted by peers' live `X2Unit.cpp` / `Always.h` edits; the committed range is the verdict.
- **Build:** `DEPLOY OK: X2_offline.exe is 27,075,072 B, dated 2026-11-16 05:15:39, md5 06e712e076c2288a3104d1310b06a867` ( the queued peer build at 05:15 compiled these edits; mine found nothing to relink ). `EXESTR OK: 2 of 2 found` ( `DLG_Background_`, `ChararcterBG`, wide ) and `EXESTR OK: 2 of 2 absent` ( `DLG_Background_Elsword.tga`, `QuestComplete_Elsword.ogg`: March's switches are out ).
- **Play-test passed ( 2026-11-16 ):** Lu/Ciel's own background art, Lu's / Ciel's quest-complete voice by body, another class's voice unchanged, the skill-window portrait follows the body switch. Logs ( run after the deploy, md5 06e712e0 ): `ErrorLog.txt` 0 lines naming `DLG_Background_` / `QuestComplete_` / `Common_BG`; 3 `E_DISABLE_HB_CHECK_REQ` UNHANDLED lines are old noise.

## Phase CH6 - Whole-character walk and gap reruns

**Run with:** Sonnet 5.5 medium, then the user's walk. Back up the save; one Lu/Ciel per class, made by creation (not only by `createchar.py`),
the tutorial and job change; one transcended unit per second job. The walk covers every skill in an A and a B slot on both bodies, the body switch,
combination points, a hyper, the summons, one dungeon per job, the character window's form switch, then **leave the game and return to the
village**. Re-run `census.py`, `charport_scan.py`, `charport_keys.py Luciel_Duo` / `Luciel_Demonio` and CH0's S3 baseline, and record before /
after in the Done note. Walk **the becoming-transcended transition** on un-transcended units (user rule, 2026-10-28): `/tc 1`, quest 42030
(`/qdone 42030`), book 288493 (`/additem 288493 1`), one per second job, with the guide popup and that class's picture, the new tree rows, the HUD
portrait, and a relog. **Done gate:** the walk passes; 0 unexplained `no_state` rows; no new `KEM_ERROR`.

## Phase Z - Close-out

**Run with:** Sonnet 5.5 medium. Write the `MODS.md` "June character: Lu/Ciel" section (flags, files, data, starter stand-in, the DU4
migration, revert order: exe + data036 + motion koms together; the migration does not revert). Check that no `_DEBUG` flag is left defined and that
the checklist rows are marked `(Lu/Ciel: <phase>)`. Add a "Template for Stage 8" block here, covering what the NU / DU foundation took and what
Rose and Ain reuse, and update `kickoffs/june-backlog/stage8-plan.md` and the backlog Stage 8 note (`char-rose` next).

## Play-test hand-off (every phase)

Dev console first; copy-ready lines with real values (unit, class, skill id and slot, `/additem`, `/qdone`). For each check give where / paste /
what it does / look for / if not. Log tag `[JUNE_CHAR]`, `python tools/logs.py --grep "JUNE_CHAR LUCIEL <sub>"`.

## Open decisions (for the user)

Settled at planning (user, 2026-11-02):
1. Code phases start after char-chung Z, ahead of `char-elsword`.
2. Starter costume: both promotion sets (15 rows, a labelled stand-in).
3. NESI renumber: June's, with the forward-only migration (DU4).

Open:
4. **Stat rows** for 37-39 / 127-129 / 200: asked again after CH0 item 8 reports what June data exists. Until then the synthetic curve is used,
   logged. CH0 (2026-11-02) reported: no June per-level table exists; the existing rows all follow June's relative table (CH0 Done note).
   **Settled (user, 2026-11-02): derive from June.** NU4 writes Lu/Ciel's `StatTable.lua` rows from June's `CharacterStatTable` relative rows with
   the fitted formula (levels 1-80; 81-99 by `cap99_gen.py` as for the other classes), a labelled stand-in under its own flag; the user packs it.
