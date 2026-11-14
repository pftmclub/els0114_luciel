# June character port: Elesis (Stage 7, seventh character after the pilot; CH-N reused from Ara)

Plan key `char-elesis` · branch `main` · kickoffs `kickoffs/char-elesis/` · log tag `[JUNE_CHAR]` (lines start `[JUNE_CHAR] ELESIS <sub>`, sub-tags
`DATA`, `CORE`, `WOS`, `CLASS`, `SUMMON`, `SKILL`, `PASSIVE`, `BLOOD`, `UI`, `NUM`) · flags `SERV_IRUHADEV_JUNE_CHAR_ELESIS_*` for Elesis's class code,
`SERV_IRUHADEV_JUNE_CLASS_PATH` (Ara's CHNa) for the class-generic class-path fixes (+ a temporary `..._DEBUG` per sub-phase) · backlog Stage 7
(`JUNE_PORT_BACKLOG_ORDER.md`, brief `kickoffs/june-backlog/stage7-plan.md`) · written 2026-11-11 by the `new-plan char-elesis` session from the Ara
plan (`JUNE_CHARACTER_ARA_PHASES.md`, the most recent worked example and the first CH-N run) and the Elsword plan's Way of the Sword phases.

## Context

**What this plan does.** It moves Elesis (unit type `UT_ELESIS`, script `ELESIS_KNIGHT.lua`, class `CX2GUEL` in `X2Lib/X2GUEL.cpp` / `X2GUEl.h`,
seven classes up to transcendence: Knight 8, Saber Knight 29, Pyro Knight 30, **Dark Knight 35**, Grand Master 120, Blazing Heart 121, **Crimson
Avenger 125**) from her March kit to her June 2017 kit, end to end: June script, skill states for June's tree, numerical damage, motions, class-only
engine hooks (`JUNE2017_CHARACTER_PORT_CHECKLIST.md` §1-§7au), her class side of the Way of the Sword, her four summons, the Dark Knight / Crimson
Avenger blood hooks, UI pieces (the creation screen and the class-change dialogs included), data, her transcendence rows, and **her June-only class
path Dark Knight -> Crimson Avenger (CH-N, reused from Ara)**.

**Why Elesis is seventh (stage7-plan, 2026-10-22).** After Ara, so CH-N is reused, not written. The brief's premises hold with corrections:
the gap scan line is in "Planning evidence"; "the first live reserved-buff path (7ai)" is hers at CH1 final (Ara reaches the same path at CH2d /
CH3b); and **she summons** (the brief did not say so): four NPC ids, one of them already in March.

**Her size.** June's class spans `0x32b4e770`-`0x32b90b6f` (Lu/Ciel's thunk run ends `32b4e76f`; Ara starts `32b90b70`): **274** carved functions,
**409** thunks in two tables (T1 `32b8ecf0`-`32b900d0` 319 entries, T2 `32b905c0`-`32b90b50` 90), whose targets are her state functions: **234 of
the 409 targets have no Ghidra function**, as do **33 of her 66 vtable overrides** (list in `plan_june_side_binary.md` §2). June's script has
**305** state tables (283 `ESI_`, 22 bare `SI_`), 60 of them on the Dark Knight / Crimson Avenger line, and **261** skill-tree rows over the seven
classes (Ara 288). Expect Ara's size. **About half her functions are read from the raw listing only** (`charport_adis.py`, which every
transcription phase runs anyway): CH0 marks each class-map row `carved` / `listing-only`, and the CH1 reviews read every listing-only function.

**Written June-body-inside-March from the start (CLAUDE.md, 2026-10-28).** A June body goes inside March's function of the same name under `_LIVE`,
March's in `#else`, proved by a one-file probe compile that defines `_LIVE`. A June function March's class lacks is written under `_CORE` with a
plain name in the class's pattern.

**Scope: June 2017 up to transcendence (user, 2026-10-20).** June's Enum names three Elesis third jobs (`UC_ELESIS_EMPIRE_SWORD` 171,
`_FLAME_LORD` 172, `_BLOODY_QUEEN` 173) but no other table has data for them: out of scope. Transcendence is live (Stage 6), so her transcendence
rows (120 / 121 / 125) are in CH3d and CH6.

**Order.** char-ara is at CH1q2 (running 2026-11-11). **CH0 runs now** (read-only, alongside anything); **CH0b after char-ara CH6's walk** (it packs
and copies koms the walk uses); **every code phase after char-ara Z** (one character's code at a time: `Always.h`, `X2GUUser.*` are shared), and CHNa
builds on Ara's `SERV_IRUHADEV_JUNE_CLASS_PATH`. Lu/Ciel stays parked until Stage 7 closes (user, 2026-11-03); Add follows Elesis.

**The state she starts from.**
- **Class code (March, live).** `X2GUEL.cpp` 9,794 lines (9,531 CRLF, 263 LF, 7 bare CR), `X2GUEl.h` 1,069 (260 CRLF, 809 LF), CP949. The whole
  class sits inside `#ifdef NEW_CHARACTER_EL` (`.cpp:4-9792`, `.h:4-1070`); no function outside it. **Every Elesis macro is ON** under `US_SERVICE`
  (`NEW_CHARACTER_EL`, `SERV_ELESIS_SECOND_CLASS_CHANGE` with about 3,240 lines, ...; `cl /P` probe, `plan_march_side.md` §2): **no dormant
  system**. `SERV_NEW_CHARACTER_EL` is ON too (`ServerDefine.h:3732`), so the emulator gives her `CAC_WSP` like Elsword; the comments that say
  "off" at `X2OfflineStatTable.cpp:292-294`, `X2OfflineClassChangeSeed.h:65-93` and `Handlers_Shop.cpp:440` are stale.
- **Way of the Sword (live March code; the shared half is already June's).** char-elsword CH1q / CH1q2 ported the shared pieces under
  `SERV_IRUHADEV_JUNE_CHAR_ELSWORD_WOS` (live): the gain rate `ApplyWayOfSwordPointGainRate`, `GetDestRageWSPGain_Elesis`,
  `CalcVigorMpUpOnAttack_Elesis`, `CX2Game::UpdateElswordSkillSlotEffect`. **Still March, hers:** the class body (`X2GUEL.cpp:3682-4046`: Get / Set
  state and point `:3684-3714`, `Start_SwordmanVigor` `:3716`, `Start_SwordmanDestruction` `:3740`, `WayOfSwordStateChange` `:3769`,
  `GetFinalDamage_WayOfSword` `:4004`; `_CONST_EL_::GALE_*` `:24-31` against June's `WayOfSword.lua` `Const_Elesis_VigorUse*` /
  `DestRageWSPGain_Elesis`), `CX2ElesisGageData` (`InitWhenGameIsOver` still resets the state, `X2GageData.cpp:1129`; June `32a34c60` does not) and
  `CX2ElesisMyGageUI` (no `RefreshWayOfSwordUI`, June vslot 0x5c `32e69e70`; `InitUI` order, June `32e6e060`). June's class side: vslots 0x674 /
  0x678 / 0x67c `32b52790` / `32b527e0` / `32b52800`, Start / End Vigor `32b74760` / `32b74930`, Start / End Destruction `32b7f0b0` / `32b74bf0`
  (both Ends also erase the `..._DARK_KNIGHT` buffs), `WayOfSwordStateChange` `32b7f370`, the state-end hooks `32b74f80` / `32b74fc0`,
  `GetFinalDamage_WayOfSword` 0x74 `32b74ed0`, `SetCriticalPercent_WayOfSword` 0x78 `32b4ef90`, `CreateGageData` 0xd0 `32b7ef90`, the gain-rate
  getters 0x720 / 0x724 / 0x72c (`+0x9714` / `+0x9718` / `+0x9720`; two stand-ins wait on 0x720: `X2DamageEffect.cpp:5543`, `X2GUUser.cpp:14837`),
  the Crest of Time pair 0x740 / 0x744 `32b6ac80` / `32b6acb0`. Phase **CH1q**, as Elsword's.
- **Summons (June 5 sites, 4 NPC ids; March 1).** `NUI_ELESIS_FIRE_BLOSSOM` 1567 (March `X2GUEL.cpp:3667`; June `32b7a058` in `32b79db0` and
  `32b746e8` in vslot 0x250 `32b74500`), `NUI_SI_SA_SBH_WALL_OF_PROMINENCE` 1635 and `_FIREKING` 1971 (`32b7a4f5` / `32b7a442` in `32b7a0a0`),
  `NUI_SI_SA_SCA_BLOOD_CUTTER` 1641 (`32b8b3b7` inside the uncarved state function `32b8b0c0`); `CanUseSpecialAttack` calls
  `CheckSummonSpecialAttack` `3202daf0`; the Fire Blossom script reads `GetRelatedSkillLevel` (the `+0x8e74` write the pilot omitted). Chung, Rena
  and Eve each needed an Opus spawn phase: hers is **CH3b0**. The same vslot pair 0x24c / 0x250 (`32b89260` / `32b74500`) holds the two writers of
  `m_fCreateEffectSetRate` (`+0x8e50`) char-elsword CH4a2 left to her: until CH3b0, `EffectSet_Elesis_SBH_Blaze_Wing_Soul_Ignition` plays at rate 0.
- **Dark Knight / Crimson Avenger blood hooks (June-only).** The hit hooks 0x4f8 `32b7beb0`, 0x4fc `32b898a0`, 0x734 `32b7bad0` (blood sucking,
  bleeding; base stubs, inert until her port: checklist l.454-455), Blood Runaway `32b87880` (BFI 462, BTI 426) and `32b734d0`, `32b7e3b0`, the
  Madness chain `32b6ff20`, the transcendence berserker chain `32b7e9a0` (BFI 913), the per-frame list 0x730 `32b816a0` over `+0x9700`. Phase
  **CH4c** (Opus), after the Crimson Avenger skills.
- **Skill system (Stage 3).** `s3_reform_ids_no_state.tsv` has her five March classes only (no 35 / 125 rows: template recipe defect 2). The offline
  auto-learn already has rows for all seven (8: 6, 29 / 30 / 35: 11 each, 120 / 121 / 125: 21 each).
- **Effect engine (June's).** `DamageEffect.lua` / `EffectSet.lua` ship merged; no `c10a` alpha site and no `march-consumer` `c11a` row is hers.
- **Motions.** June 82 Elesis names, March 9 class files (data033): 6 same-name files differ in size (`Motion_EL_Common.x` 1.4 -> 6.1 MB, `_Field`,
  `_Emotion`, `_PK`, `_GM`, `_BH`), 19 same size, **57 June-only** (`Motion_EL_DK.x`, `MOTION_EL_CA.x` in June data206, `Motion_Standard_El.X`, 54
  emotions). Whether June drops a name March's script plays decides if the 6 deploy in CH0b or with CH1 final (7ad): CH0 answers.
- **June-only class path (CH-N).** `X2Unit.h` names `UC_ELESIS_DARK_KNIGHT` 35 (`:165`) and `UC_ELESIS_CRIMSON_AVENGER` 125 (`:179`) after `UC_END`
  (= 124), so 125 fails every `< UC_END` test until Ara's CHNa lands the class-generic bounds (`X2SocketItem.cpp:2897` -> 202, `GetClassLevel`,
  the optional log sites). **Elesis's own:** `GetX2UnitClassCompatibility` arms (`X2Unit.h:2057-2123`), the third-line helpers
  (`X2Unit.h:2685-2750`, assert-only today), the gauge create (`X2GageManager.cpp:194-209` has no 35 / 125 arm while `CX2GUEL::CreateGageData`
  `:1970` and `GetELGageData` cast to `CX2ElesisGageData`: **Ara's crash, hers too**), `X2Data.cpp:2872-2892` picture rows, the lobby particles
  (`X2UnitViewerUI.cpp:630` / `:2414` / `:2530`), `X2ComboTree.cpp:302`, the class switches inside `X2GUEL.cpp` (`:213`, `:249`, `:411`, `:2747`,
  `:9581`), `tools/createchar.py`. June's route: Dark Knight quests **35569-35572** (`CJT_JOB_THIRD`, from 8 at level 15), Crimson Avenger
  **35573-35577** (from 35 at level 35); the staged `FieldQuest.lua` already has them (titles matched; CH0 confirms the ids and rewards).
  `DLG_Class_Change_Elesis_DK` / `_CA` and `DLG_1ST` / `2ND_Class_Change_Guide_Elesis.lua` are June-only.
- **Stat rows.** `StatTable.lua` has 99 rows each for 8, 29 and 30 only (29 and 30 equal 8 at every level). **Grand Master and Blazing Heart have no
  rows**: `X2OfflineStatTable.cpp` (`:355-369`) falls back to her unit type's rows, silently, so 120 / 121 play on class 8's numbers today, and 35 /
  125 would too once their `UnitTemplet` rows exist. Open decision 1 (answered 2026-11-11): CHNa writes rows for 35 / 120 / 121 / 125 by Ara's rule.
- **Creation screen (user, 2026-11-11).** March's `DLG_Unit_Create_ClassChange_Button_Elesis.lua` (1,923 B) has two buttons (Saber Knight, Pyro
  Knight); June's (5,284 B) has six. The six `SCUUCM_*_CLASS_CHANGE_BUTTON_UP` handlers are generic (`X2StateCreateUnit.cpp:778-808`,
  `SelectUpperClass` `:3397`) and March's line helpers already carry SK -> GM and PK -> BH, so **Grand Master and Blazing Heart appear with June's
  dialog (CH5)**; the third line (Dark Knight, Crimson Avenger) needs the helper arms and the stat-chart arms (**CHNa**).
- **Class-change dialog at level 15 (user, 2026-11-11).** March's `LevelUpEvent.lua` opens the old `Class_Change_Guide_Elesis.lua`; June's row for
  class 8 opens `DLG_1ST_Class_Change_Guide_Elesis.lua` (three paths), the level-35 row adds 35 and opens `DLG_2ND_...` (**CHNb**; CH0b stages them).
- **Hooks waiting on the class port.** 35 checklist rows name her and 27 class-generic rows apply (`plan_checklist_rows.md`): the pressed-index
  set (`X2GUUser.cpp:26778-26797` says "Elesis, Add not ported"; her `CountUsedSkill` `X2GUEL.cpp:3157`), `SetDamageExceptionProcess` 0x2b8
  `32b80f20` with the June-only 28416 / 28417 part (`@32b81008-32b811ef`), the delegate pushes in `InitPassiveSkillState` `32b88300` (7), 0x428
  `32b52260`, the class float writer `32b73bf0` / reader `32b7d550` (l.496, state unnamed), BFI 910 / 913 callers `32b7e96c` / `32b7ebae`,
  `32b90420` returning June state `0xbb` (March's `0xbb` is another state: by name, never the number), her tutorial test `32b74065`, the three BFI
  names missing from June's `Enum.lua` (`JUNE_BUGS` 279). **Not hers:** the Crest of Time bases and the death-prevention gate (char-add), the
  training F7 key `322ed400` (backlog Stage 2); funcmap l.1420's `32b45e0c` is in Lu/Ciel's range, and `32b901c0` `IsDashState` is a shared base.
- **Her own March patch.** `SERV_IRUHADEV_JUNE_EL_DASHJUMP_XZ_FIREBALL` (`X2GUEL.cpp:7257-7292`, June `32b770e0`): the phase that owns that state
  writes June's whole function under `_LIVE`; the old block stays in the `#else` and Z records the flag as superseded.

**Out of scope (owned elsewhere).** Third jobs; new characters (Stage 8); UI that is not hers (Stage 9); tutorials and training dungeons
(june-dungeons); NPC / boss copies of Elesis; emotion motions' behaviour (Stage 10; CH0b stages the 54 files); `FORCE_DYNAMIC_CAMERA_INDEX` x15 /
`RETAIN_CAMERA_STATE` (Stage 9); `IMMUNITY_EDT_LIST_AT_THIS_STATE` x4 (backlog Stage 2); PvP-only paths (no PvP offline); stat and damage math
(Stage 33); the jumping-event dialogs (`DLG_UI_JUMPING_*`).

## Rules

The Ara plan's Rules hold (`python tools/docsection.py JUNE_CHARACTER_ARA_PHASES.md "Rules"`), with Elesis's names and these changes:
- **Standing rules** (`JUNE_PORT_BACKLOG_ORDER.md`, "Standing rules"): scope rule, no partial June updates, June wins, June bugs verbatim with a
  `JUNE_BUGS.md` row, cite June addresses, dev free-skill gate (`CX2GUUser::IsDevUnlimitedSkillUse()`) in her skill-use check.
- **`ReservedSkillStart` (user, 2026-11-05).** March's `CX2GUEL` has no override. CH1k writes `CX2GUEL::ReservedSkillStart` =
  `SpecialAttackCooldown( skillID, m_fReservedSkillMp )` then `SkillMemoMpRefund( ... )`, widening the base hook chain in `X2GUUser.h` / `.cpp` with
  her `_LIVE` flag (copy Ara's); CH1 final's play-test casts each skill twice (cool time shows, MP drops).
- **Slots by role, never by Ara's offset.** Elesis overrides 66 slots, Ara 77, 46 in common. She does **not** override 0x4e0
  (`AttackResultByType`), 0x780, 0x794, 0x68c (`InitAquiredActiveSkillState`), 0xc0 / 0xc8, 0x6a8, 0x3ac, 0x9c / 0xa0: no Ara hook is carried over
  by slot number. Every override's role is confirmed from its body (CH0 item 1).
- **Listing-only functions.** Ghidra stays read-only: a function with no Ghidra entry is read with `charport_adis.py elesis <va>` (CH0 adds
  `elesis` to its `CLASS_RANGE` as `(0x32b4e770, 0x32b90b6f)`), never by creating functions. A Sonnet phase whose function has no decompile and a
  control flow the listing does not make plain says "this needs Opus" for that function and goes on with the rest.
- **June's ids in passive code.** Every `SI_*` id March's `InitPassiveSkillState` / skill code reads is checked against June's `RNSI_*` id (March
  8000s, June 28000s). CH0 lists them; CH4a fixes what CH1 did not.
- **The checklist is the work list.** Every Elesis row in §1-§7au is ported by a phase here or marked n/a by CH0 with the reason. The phase that
  ports a row appends `(Elesis: CHn)` to it in place.
- **One deploy unit.** June's `ELESIS_KNIGHT.lua`, her June motions (held for CH1 if CH0 finds a dropped name) and the C++ that reads them go live
  together in CH1 final, behind `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (`//#define` in `Always.h` until then). Between phases she stays playable on
  her March kit. Classes 35 / 125 are playable only from CHNa on.
- **Flags:** `_CORE` (June functions March lacks, compiled, unreferenced), `_LIVE` (June bodies inside March's functions, the registration, the
  switch), `_WOS` (her Way of the Sword class side and gauge twins, CH1q; defined only with `_LIVE`), `_CLASS` (her 35 / 125 rows, CHNa / CHNb),
  `_SUMMON` (CH3b0), `_SKILL`, `_TRANSCEND`, `_PASSIVE`, `_BLOOD` (CH4c), `_UI`, `_ASSET` (only if a June asset breaks a March reader), and a
  temporary `_DEBUG` per sub-phase, retired in its review. The class-generic class-path fixes are Ara's `SERV_IRUHADEV_JUNE_CLASS_PATH`: widen
  it, never copy it. New namespaces are `_CONST_EL_<phase>_` (March has `_CONST_EL_` at `X2GUEL.cpp:6-53`, used qualified).
- **Names (CLAUDE.md, 2026-10-28).** No `June` in any identifier, no Ghidra address or phase tag in a name; a one-line `// char-elesis CH1n` tag on
  every edited block (CLAUDE.md, 2026-11-09). `python tools/junediverge.py --list` before transcribing; `--check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h
  <other files>` = `RESULT: PASS` at every finish. Rows that name her today: 417, 420, 421, 424, 427 (Way of the Sword) and 365 (class cases).
- **Shared `CX2GUUser` helpers, never copied into the class:** `GetSkillLevel` / `GetLearnedSkillAbilityValue`
  (`SERV_IRUHADEV_JUNE_USER_SKILL_LEVEL_HELPERS`); the seven skill-use helpers and their fields (`SERV_IRUHADEV_JUNE_USER_SKILL_USE_HELPERS`);
  the user state key `CREATE_EFFECT_SET` (`SERV_IRUHADEV_JUNE_USER_CREATE_EFFECT_SET`); Elsword's Way of the Sword helpers; the earlier
  characters' `CX2GUUser` bases: a phase that needs one under its own flag widens the `#if` instead of copying it.
- **No hardcoded enum literals** (CLAUDE.md, 2026-10-26): every June id by name; `enumscan.py --check X2Lib/X2GUEL.cpp` stays 0. March's C++ enum
  `EL_STATE_ID` (`X2GUEl.h:9-226`, 130 members, no `ESI_END`) takes June's missing names with June's ids. June's `Enum.lua` names **no** Elesis
  state, so every name is resolved from the exe's state-name strings (as Rena's `ch0_state_enum_names`); June's `USI_END` is 152 (March 100). The
  NPC names for 1635 / 1641 / 1971 are registered in `X2UnitManager.h` under `SERV_IRUHADEV_JUNE_ENUM_NAMES` (CH3b0).
- **Shared Lua per character.** `UnitTemplet.lua` is core-list source: CH5 replaces her 5 rows and adds June's 35 / 125 rows. Her class script is
  adopted whole (`adopt-june`, bytecode unless a phase must edit it) and passes `python tools/luabind.py check ELESIS_KNIGHT`; so do the four
  summon scripts (CH3b0). March's `ELESIS_KNIGHT.lua` (157,500 B bytecode, not in the ledger) is replaced, not converted.
- **Assets first, whole** (CH0b), byte-exact through `kom.py stage-june`; pack and copy yourself (CLAUDE.md, 2026-11-10).
- **CP949.** `X2GUEL.cpp` / `X2GUEl.h` (mixed CRLF / LF: match the neighbouring lines), `X2GUUser.*`, `X2Gage*.*`, `X2Unit.h`,
  `X2StateCreateUnit.*`, `X2DamageManager.*` are CP949: `bytepatch.py` only; `cl /P` decides every `#ifdef` branch.
- **Damage numbers.** Record March vs June damage per changed skill in `MODS.md`.
- **Saves.** Units 20 `wewe` (8 lv7), 40 `SaberT` (29 lv70), 21 `eresisu` (30 lv77), 29 `ElesisGM` (120 lv80), 30 `ElesisBH` (121 lv80), all
  transcendence 0; no 35 / 125. Every March class has a unit, so CH0b creates none; CHNa creates 35 / 125 once the class loads. Play-test saves
  never slot a skill in slot 4 / 9 before transcendence, and learn only the ids of the bank being tested.
- **Offline reach (Raven lesson 4).** Each phase's hand-off names only checks the offline client can reach; a June path that only a server or PvP
  drives is a stand-in or a routed item, decided in the phase, not found in CH6.
- **One code phase at a time in `X2GUEL.*`** (CH1-CH4). CH5 and CHNa / CHNb may run beside CH2-CH4 when their Touches do not overlap.

## Starting context

| Need | Command |
|---|---|
| One section of this doc | `python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "<heading>"` (`--no-sub` to skip Done notes) |
| Checklist rows | `python tools/docsection.py JUNE2017_CHARACTER_PORT_CHECKLIST.md "<heading>"`; her rows with line numbers and everything other plans routed to her: `june2017_re/char_elesis/plan_checklist_rows.md` |
| Planning evidence | `june2017_re/char_elesis/plan_*` (below); CH0 writes `ch0_*` beside them |
| Character tools | `june2017_re/charport_states.py`, `charport_keys.py`, `charport_closure.py`, `charport_adis.py` (CH0 adds `elesis`), `charport_regsim.py`, `s3_no_state.py`; Ara's builders `june2017_re/char_ara/ch0_*.py` (copy into `char_elesis/` with her range); `char_arme/ch0c_listing_check.py`; `char_eve/ch0_build_assets.py` (CH0 adds `elesis`) |
| June Lua (reading) | `june_src_fixed/data036_kom/ELESIS_KNIGHT.src.lua`, `UnitTemplet.src.lua`, `Enum.src.lua`, `CharacterStatTable.src.lua`, `FieldQuest.src.lua`, `LevelUpEvent.src.lua`, `WayOfSword.src.lua`; `june_src_fixed/data034_kom/DLG_Unit_Create_ClassChange_Button_Elesis.src.lua`, `DLG_1ST` / `2ND_Class_Change_Guide_Elesis.src.lua` |
| Ghidra | `ghidra` skill; her range `0x32b4e770-0x32b90b6f`, vtable `334fa61c` (544 slots, 66 overrides, 33 uncarved), RTTI `.?AVCX2GUEL@@` (type descriptor `3373d5e4`), ctor `32b70ee0` (vtable write `32b70f3e`), dtor `32b70990`, factory `32b72430`, `MemoConditionElesis` vtable `334fa4f4` (xrefs `32b72400`, `32b642f8`), gauge data vtable `3349b184`, gauge UI vtable `3349ac34`; read-only in CH0 and reviews |
| Gap scan | `cd june2017_re && python census.py && python charport_scan.py <short scratch dir>` (slow; revert the regenerated `census.tsv` and `JUNE2017_EFFECT_KEY_MATRIX.md` after, and only those), `python charport_keys.py ELESIS_KNIGHT` |
| Bring June Lua in | `/x2-adopt-june` (`luasrc.py adopt-june data036 ELESIS_KNIGHT.lua`, `--dry-run` first) |
| Stage June assets | `python tools/kom.py stage-june <list>`, `/elstools pack dataNNN`, `/x2-kom-packed copy dataNNN` |
| Edit a CP949 source | `python tools/bytepatch.py show / find / apply` (x2-byte-edit) |
| Hygiene / build / logs | `flagcheck.py <FLAG> --worktree --files ...`, `junediverge.py --check ...`, `enumscan.py --check ...`, `build.py --deploy` (background), `logs.py --grep "JUNE_CHAR ELESIS"` |
| Save / test character | `/x2-db` (`tools/elsdb.py setchar`, `settranscend <unit> 1`), `/x2-create-char <name> <class> <level>` |
| Dev console | auth level 3, END key; items `/additem <id> [qty]`, quests `/qdone <id>`, `/tc 1` |

## Planning evidence (2026-11-11)

Kept in `june2017_re/char_elesis/` (digests by Sonnet subagents, spot-checked: three uncarved thunk targets and one carved override in Ghidra, the
35 / 125 enum lines, the two-button March dialog, the `LevelUpEvent` rows, the Way of the Sword dialog sizes; CH0 re-verifies anything it builds
on): `plan_charport_scan_20261111.txt`, `plan_charport_keys.txt` / `.tsv`, `plan_march_side.md`, `plan_june_side_binary.md`,
`plan_june_side_data.md`, `plan_numerical_rows.tsv`, `plan_checklist_rows.md`.

Scans re-run 2026-11-11: `census.py` 26 PASS, 0 FAIL, 0 `to-implement` rows (implemented 653, june-ignores 305, handed-off 8; regenerated
`census.tsv` / matrix reverted); `charport_scan.py` (scratch `C:/tmp/cps_elesis`, seeded from Ara's finished scratch after a 258-file `cmp` because
the fresh decompile hung on `ManufactureItem.lua`; output identical to Ara's 2026-11-10 run):
`ELESIS_KNIGHT {'no NUMERICAL_DATA_INDEX': 69, 'no DAMAGE table': 20}` (the brief's gap 20 holds), no ENUM line, no KEYS line;
`s3_no_state.py --family X2GUEL.cpp --preprocess`: `rows=179 no_state=174 has_state=5` (only id 900 has a state, once per class; 8: 15, 29 / 30:
29 each, 120 / 121: 53 each). **The S3 tsv has no rows for 35 / 125** (template recipe defect 2): CH0 item 12 appends them.

| Measure | Elesis | (Ara) |
|---|---|---|
| June class range | `32b4e770`-`32b90b6f`, 274 carved; thunk tables T1 319 + T2 90 = 409 thunks, 409 distinct targets, **234 uncarved**; 66 vtable overrides (65 in range + the folded stub `32c69f70`), **33 uncarved**; 0x2b8, 0x6a0, 0x6a4, 0x428, 0xd0, 0x29c, 0x740, 0x744 overridden; 0x4e0, 0x780, 0x794, 0x68c, 0xc0 / 0xc8, 0x6a8, 0x3ac, 0x9c / 0xa0 not | 540 + 32, 77 |
| June skill-tree rows | 261 over seven classes (8: 15; 29 / 30 / 35: 29 each; 120 / 121 / 125: 53 each), **127** distinct ids (106 .. 28605; 29 / 30 / 35: `RNSI_SA` 16, `RNSI_A` 10, `RNSI_P` 2, `SI_ETC` 1; 120 / 121 / 125: `RNSI_SA` 27, `RNSI_A` 15, `RNSI_P` 8, `RNSI_HA` 2, `SI_ETC` 1); 0 unnamed; the S3 tsv lacks 35 / 125 | 288 / 139 |
| June script | `ELESIS_KNIGHT.src.lua` 20,161 lines: 305 states (283 `ESI_` + 22 bare `SI_`, all Dark Knight / Crimson Avenger skills) + 6 `INIT_*` (`MAX_HYPER_MODE_TIME` 30, `HYPER_MODE_COUNT` 1, `MP_CHARGE_RATE` 130; no way-of-sword init key; **no `PROMOTION_ITEM_ID_LIST`**: the promotion items are in `UnitTemplet`); groups DASH 50, SI 48, RIDING 27, COMBO 25, SA 23, DAMAGE 21, A 8, JUMP 6, STANDUP 6, PEPPER 5, QUICK 4, DANCE 3; by job tag: common 116, EL 19, SSK 20, SGM 38, SPK 19, SBH 31, SDK 29, SCA 30, hyper 3 (EGM / EBH / ECA) | 331 |
| Script keys | `DAMAGE_DATA` 79 (+8 `_NEXT`), `AT_COMMAND` 60, `APPLIED_SKILL_ID` 33, `TIME_STOP_IMMUNE_STATE` 3, `SHOW_HYPER_CUT_IN_TIME` 3, `SHOW_SKILL_CUT_IN_TIME` 43, `CUSTOM_RENDER_STATE` 0, `EFFECT_SET_LIST` 11 / `_EX` 103, `CREATE_EFFECT_SET` 46; class keys `WAY_OF_SWORD_TYPE` x78 (vigor 31, destruction 47; shared with Elsword), `EFFECT_SET_LIST_EX_NO_TRANS` x4, `_EX_TRANS_1` x4, `_EX_APPLIED_SKILL` x8, `EFFECT_SET_LIST_ULTIMATE_EX_APPLIED_SKILL` x1, `RELATIVE_SKILL_LEARN_CHECK` x8, `IGNORE_STATE_DEFENCE` x2, `IGNORE_DEFENCE_DECLINE` x2; 4 `MEMO` state tables | 54 / 36 / 16 |
| Key census (`charport_keys.py ELESIS_KNIGHT`) | 248 keys: both 234; june-only **9** (six `BBT_*` / `BFT_*` damage-data keys with no reader, the camera pair (Stage 9), `IMMUNITY_EDT` (Stage 2)); neither 1 (`HEAD_IK` x9); march-only-site 4 (CH0) | 250 / 14 / 7 / 4 |
| Numerical blocks | 142 `c1a` rows keyed to 48 ids (28000-28602), all `ok` | 160 / 59 |
| Motions | June 82 / March 9 class files; 6 differ in size, 19 same size, 57 June-only (`DK`, `CA`, `Standard`, 54 emotions); names dropped: CH0 | 75 / 17 |
| Alpha-tag sites (7c) / March-only templets (7u) | **0** / **0** | 0 / 0 |
| Summons | 5 sites, 4 NPC ids (1567 in March already; 1635, 1971, 1641 June-only) | none |
| Class mechanics | Way of the Sword class side: live March code, June's twins (CH1q); blood hooks: June-only (CH4c) | force power, Specter |
| March class | `.cpp` 9,794 lines, `.h` 1,069 (`EL_STATE_ID` 130 members: common 35, Knight 17, endurance 2, SSK 16, SPK 12, SGM 26, SBH 22; no state id in a `UCHAR`); `_CONST_EL_` `:6-53`, ctor `:83`, Init `:201`, InitState `:247-310` (cut-in switch `:249-273`), InitComponent `:311`, InitPassiveSkillState `:350-659`, `SetEquippedSkillFuncBySkillSlotIndex` `:660-1551` (42 `case SI_`), SetEquippedSkillLevel `:1552-1684`, CommonFrameMove `:1704`, SpecialAttackEventProcess `:1799`, GetActualMPConsume `:1844`, GetActualCoolTime `:1929`, CreateGageData `:1970`, InitStateCommon `:2004-2743`, InitStateByUnitClass `:2744-2970`, CanUseSpecialAttack `:3012`, StateChangeSpecialAttack `:3151`, ReInit `:3248`, DamageReact `:3308`, HyperModeFrameMove `:3504`, the Fire Blossom spawn `:3667`, Way of the Sword `:3682-4046`, SetDamageExceptionProcess `:9702`; no `ReservedSkillStart`, `InitAquiredActiveSkillState`, `AttackResultByType` | 17,140 |
| June class | InitState `32b87a60` (0x7a4, wrapper) -> parts `32b81be0` (111 names) + `32b864f0` (57), base `31fd8f00`; `SetEquippedSkillFuncBySkillSlotIndex` `32b8c470` (0x7b4, four jump-table switches: 28031.., 5.., 28000.. with 55 targets, 28601..); SetEquippedSkillLevel `32b4e790` (0x7b8, uncarved, 22 id blocks); InitComponent `32b81710`; InitPassiveSkillState `32b88300` (0x690); SpecialAttackEventProcess `32b4ee20` (0x7f4, uncarved) + CanUseSpecialAttack `32b84a50` (0x7f8), 0x7fc `32b52590`, 0x800 `32b73e60`; NoStateChangeActionFrameMove `32b818b0` (+ Endurance `32b80c60`); DamageReact `32b73ff0`; HyperModeBuffEffectStart `32b74300` (0xc4, uncarved); ReInit `32b81900` (uncarved); Init `32b52560` (uncarved); InitEffect `32b4ef40`; CommonFrameMove `32b88070`; HyperModeFrameMove `32b7f090` -> `32b7b820`; the June-only hook 0x83c `32b732e0` | |
| Stat rows | `StatTable.lua` 1-99 for 8 / 29 / 30 (level 1: 360, 296, 80, 66, 11250); **none for 120 / 121 / 35 / 125** (type fallback, silent); June `CharacterStatTable` relative rows: `UT_ELESIS` `450, 100, 370, 83, 11250`, Grand Master `500, 112, 400, 90, 12250`, Blazing Heart `490, 110, 430, 92, 11750`, Crimson Avenger `510, 102, 410, 97, 12000` | 1-99 for 5 |
| Save | units 20 (8 lv7), 40 (29 lv70), 21 (30 lv77), 29 (120 lv80), 30 (121 lv80); transcendence 0; **no 35 / 125** | |
| Cut-ins | the SGM / SBH `.dds` are in March; March's table names `HQ_CutIn_Elesis_Pyro_Knight.dds` / `_Saber_Knight.dds`, which exist nowhere; `HQ_CutIn_Elesis_SCA.dds` is June-only (data044); `HQ_Cutin_EGM / EBH / ECA_Transcendence` `.gfx` already in March; June's `UnitTemplet` adds `m_fLobbyRotationY` (30 / 25 / 1 / 5 / 15 / 1 / 10) and `m_HyperCutIn` for 120 / 121 / 125 | |
| Class path | quests 35569-35572 (to 35; reward items 109999 + 293720) and 35573-35577 (to 125; 110850 + 293721); promotion items 41815-41820 (35) and 41821-41827 (125) from `UnitTemplet`; 35 uses `Motion_EL_DK.x`, 125 `Motion_EL_DK.x` + `MOTION_EL_CA.x`; both intro movies `Movie_Create_Elesis_CA.avi` (loose, already equal to June's) | 35560-35568 |
| Transcendence | `DLG_Transcendental_Change_Guide_Elesis.lua` (same size both sides) names `POPUP_TR_G_MASTER` / `_B_HEART` / `_C_AVENGER`: **none in March** (June data215, + `.TET`); `TranscendCheck` (`Handlers_Quest.cpp`) has no Elesis class | |

## Phases

| Phase | What | Model | Depends |
|---|---|---|---|
| CH0 | Elesis map: class map (every thunk target and uncarved override included), state / skill table, checklist rows, assets + effect closure, keys, registration listing check, unmapped helpers, the Way of the Sword map, the summon map, the blood-hook map, the CH-N map (35 / 125), the motion question | Opus high | - (runs now) |
| CH0b | Every June Elesis asset, whole, first | Sonnet high, user play-test | CH0, char-ara CH6 walk |
| CH1a-CH1k | Class core transcription by state group and vslot band (12 sub-phases, below) | Sonnet high; Opus review every 2-4 phases | CH0, char-ara Z; one at a time |
| CH1m-CH1p | Wiring behind `_LIVE` (registration; base callbacks + slot head; virtuals; 7ag + hooks + June-only keys) | Opus high, Opus review each | the CH1a-CH1k reviews |
| CH1q | Way of the Sword: her class side and the gauge twins, flag `_WOS` | Opus high, Opus review | CH1p review |
| CH1 | Final: define `_LIVE`, adopt the script, copy any held motion koms, play-test on 8 / 29 / 30 / 120 / 121 | Opus high, Opus review, user play-test | CH1q review, CH0b |
| CH5 | UI and data: her 5 `UnitTemplet` rows June's + the 35 / 125 rows added, cut-in table, **the creation screen (Grand Master, Blazing Heart)** | Sonnet high, Opus review, user play-test | CH1 review |
| CHNa | CH-N (a): 35 / 125 load and play: her class arms, gauge create, stat rows (Open decision 1), units, **the creation screen's third line** | Opus high, Opus review, user play-test | CH5 review, char-ara CHNa review |
| CHNb | CH-N (b): the job-change route to 35 and 125 and **the level-15 / level-35 class-change dialogs** | Opus high, Opus review, user play-test | CHNa review |
| CH2a-CH2d | Skills: Knight + first jobs (8, 29, 30, 35) | Sonnet high; one Opus review per stage at CH2d; user play-test | CH1 review (CH2d: CHNb review) |
| CH3a-CH3d | Skills: second jobs (120, 121, 125), hyper actives, transcendence rows | Sonnet high (CH3d Opus); one Opus review at CH3d; user play-test | CH2d review (CH3c: CHNb review) |
| CH3b0 | Summons: the four NPC spawns and vslots 0x24c / 0x250 (the `m_fCreateEffectSetRate` writers), flag `_SUMMON` | Opus high, Opus review | CH3a; before CH3b |
| CH`n`0 | One Opus sub-phase per unmapped helper CH0 finds, before the Sonnet phase that needs it | Opus high, Opus review | as its phase |
| CH4a-CH4c | Passives; overrides + memo; the Dark Knight / Crimson Avenger blood hooks | Opus high; one Opus review at CH4c; user play-test | CH3d review |
| CH6 | Whole-character walk and gap reruns | Sonnet medium, user walk | CH4c review, CH5 review, CHNb review |
| Z | Close-out | Sonnet medium | CH6 |

CH1-CH4 all edit `X2GUEL.*`: one at a time. CH5, CHNa and CHNb touch `UnitTemplet.lua`, `X2Unit.h`, `X2GageManager.cpp`, `X2StateCreateUnit.*`, the
emulator and the quest Lua: they may run beside a CH2 phase when the Touches columns do not overlap. If CH0's counts put a group over ~45
functions (or over ~25 where most rows are listing-only), CH0 splits it with a digit suffix (`CH1e2`) and writes the kickoff and STATUS row; it
does not merge groups. Play-test follow-ups after CH1 final get the next letters (`CH1r`).

**Review cadence (user, 2026-11-10).** CH1a-CH1k are inert transcriptions in one class file: reviewed in runs (CH1b covers CH1a-CH1b, CH1d2 covers
CH1c-CH1d2, CH1g covers CH1e-CH1g, CH1k covers CH1h-CH1k). CH1m-CH1q, CH1 final, CH5, CHNa, CHNb and CH3b0 keep their own reviews (shared
code). CH2a-CH2d have one review at CH2d, CH3a-CH3d one at CH3d, CH4a-CH4c one at CH4c: the earlier phases say "no review of its own" and keep
their `_DEBUG` define on; the stage review retires them and gives a verdict line to each incidental shared-file hunk.

## Phase CH0 - Elesis map (read-only)

**Run with:** Opus 5.5 high. No code, no flag, no build. Ghidra read-only. Delegate reads, greps, tsv joins and known-address dumps to
subagents. Outputs in `june2017_re/char_elesis/`.

**Step 0 (the pilot's recipe fixes).** Build the registration map with `charport_states.py init 32b87a60 32b81be0 32b864f0 <base 31fd8f00 and the
line registrars it calls>` and `skill 32b8c470` (June's 0x7a4 `32b87a60` is a wrapper; its two parts register 111 and 57 names; `3202b110` and
`31fe22d0` are the base event states and the START / WIN / LOSE roll), then run `ch0c_listing_check.py` on it: every row must match the raw
listing; all `~` rows and std::string-named rows by hand. Map all four switches in `32b8c470` case by case (the planning resolved them by range
ends only) and the 22 id blocks of `SetEquippedSkillLevel` `32b4e790`. She has **no** second skill switch (0x68c is the base's): confirm. Ara's
builders are copied into `char_elesis/` with her range: say which. Add `elesis` to `charport_adis.py`'s `CLASS_RANGE`; confirm it reads the 234
uncarved thunk targets and the 33 uncarved overrides.

**Scope** (Ara's CH0 items, with Elesis's numbers):
1. **Class map** `ch0_class_map.tsv`: every function in `0x32b4e770-0x32b90b6f`, **every one of the 409 thunk targets and the 33 uncarved
   overrides included**, March pair, verdict, target sub-phase, the **`inside` / `new`** column (propose the plain name for every `new` row) and a
   **`carved` / `listing-only`** column. Funcmap rows `Char Elesis CH0`. Confirm the role of each override by its body: the planning left 0x428,
   0x558, 0x670, 0x720-0x72c, 0x730, 0x83c, 0x534, 0x4ac as guesses, and the base of 0x4f8 / 0x4fc / 0x734 is the shared stub `33093ad0`.
2. **State and skill table** `ch0_state_skill.tsv`: 311 / 311 script tables, every skill id; dead-data and March-only states listed; nothing in
   `X2GUEL.*` stores a state id in a `UCHAR` (7l): confirm. Map June's tags to the jobs (`SSK` / `SGM` Saber Knight / Grand Master, `SPK` / `SBH`
   Pyro Knight / Blazing Heart, `SDK` / `SCA` Dark Knight / Crimson Avenger, `EL` Knight) and say what the 22 bare `SI_` tables and the `QUICK`
   group are and which phase each goes to; list June's ids for every state name `EL_STATE_ID` lacks or numbers differently (from the exe strings).
3. **Keys** `ch0_keys.tsv`: `EFFECT_SET_LIST_EX_NO_TRANS` / `_EX_TRANS_1` / `_EX_APPLIED_SKILL` / `EFFECT_SET_LIST_ULTIMATE_EX_APPLIED_SKILL`,
   `RELATIVE_SKILL_LEARN_CHECK`, `IGNORE_STATE_DEFENCE`, `IGNORE_DEFENCE_DECLINE`, `WAY_OF_SWORD_TYPE`, `CREATE_EFFECT_SET` get an owner (class or
   engine; an engine reader with no March counterpart is a `CH<n>0` or a routed item); `PLAY_TYPE` / `TRANSITION` read by `32b6e0c0`; the neither
   key and the 4 march-only-site keys; `MAX_HYPER_MODE_TIME` / `HYPER_MODE_COUNT` / `MP_CHARGE_RATE` get their readers; June's
   `WayOfSword.lua` `Const_Elesis_*` keys: does June's code read them (Elsword's 14 extra keys had no reader).
4. **Checklist rows** `ch0_checklist_rows.tsv`: every §1-§7au row -> phase or n/a with reason (start from `plan_checklist_rows.md` Parts A1-A3 and
   its Part B routed items; correct funcmap l.1420's `32b45e0c` (Lu/Ciel's range) and the 0x7f4 vtable address `334fae14` -> `334fae10`).
5. **Assets** `ch0_assets.tsv` + `ch0b_stage_list.tsv` (routes a name / b data / c `item.kim` Elesis items incl. the 35 / 125 promotion ids
   41815-41827 and costume ids, unioned; `ch0_build_assets.py` gains `elesis`) and **effect closure** `ch0_effect_closure.tsv`
   (`charport_closure.py`, run-time-built names included; the 62 June-only effect names her code pushes are in `plan_june_side_binary.md` §6).
   **The motion kom:** name the March kom for June's data206 motions. **The motion question:** for each of the 6 differing class motions, the
   animation names March's script or class code plays that June's file lacks; if any, those files are held for CH1 final
   (`ch1_held_motions.tsv`), else CH0b copies them; the six March animations already missing (`SI_A_EL_LEAP_CRASH_*`, `SI_A_EL_RUSHING_SWORD`,
   `SI_SA_SSK_SPIRAL_BLAST`, `_VICTORIOUS_SWORD`) and the KEM 433 frame warning on her part meshes get their June files. Stage
   `POPUP_TR_G_MASTER` / `_B_HEART` / `_C_AVENGER` (+ `.TET`), `DLG_1ST` / `2ND_Class_Change_Guide_Elesis.lua`, `DLG_Class_Change_Elesis_SK` / `_PK`
   / `_GM` / `_BH` / `_DK` / `_CA` (June data217), `Class_Change_Guide_Elesis.lua`, `DLG_Unit_Create_ClassChange_Button_Elesis.lua` (owner CH5),
   `HQ_CutIn_Elesis_SCA.dds`, the four summon NPCs' scripts, meshes and templet rows (`npc_mesh_census.py`), the cut-in art CH5 needs.
6. **Unmapped helpers.** Per skill, the callees outside the class range funcmap does not map (`ch0_unmapped_callees.py`). Each gets an Opus
   `CH<n>0` sub-phase before the Sonnet phase that needs it (kickoff + STATUS row written by CH0). CH3b0 (summons) is planned already.
7. **Way of the Sword map** `ch0_wos.tsv`: every March Way of the Sword function of the class, the gauge data and the gauge UI paired with June's
   (start from `char_elsword/ch0_wos.tsv` rows `elesis` and `shared_with_elesis=yes`, and Elsword's CH1q / CH1q2 Done notes: what June changed for
   him it changed in her twins `32a34c60`, `32e6e060`, `32e69e70`); the three gain getters 0x720 / 0x724 / 0x72c with their writers and what
   `+0x971c` scales in `32b74ed0`; the `..._DARK_KNIGHT` buff ids; `_CONST_EL_::GALE_*` against June's constants; the emulator's `CAC_WSP` answer.
   Sized for CH1q; split if over ~45 functions (CH1q2 for the gauge side).
8. **Summon map** `ch0_summon.tsv`: the five call sites with their arguments, each NPC's script / templet row / mesh presence in March, the
   `GetRelatedSkillLevel` path and the `+0x8e74` write, `CheckSummonSpecialAttack` `3202daf0`, whether June's spawn rules differ from March's
   `:3667` (count limits, owner link, position), and the whole bodies of 0x24c `32b89260` / 0x250 `32b74500`. Sized for CH3b0.
9. **Blood-hook map** `ch0_blood.tsv`: 0x4f8 / 0x4fc / 0x734 and what calls them in the base (the caller sites and arguments), `32b87880`,
   `32b734d0`, `32b7e3b0`, `32b6ff20`, `32b7e9a0`, `32b7e740`, 0x730 `32b816a0` + `32b81220` and the list at `+0x9700`, the members `+0x9748` /
   `+0x974c` / `+0x9760` named from their readers, the skills that own each. Sized for CH4c.
10. **CH-N map** `ch0_classn.tsv`: every client and emulator site with arms for 8 / 29 / 30 / 120 / 121 and none for 35 / 125 (start from
   `plan_march_side.md` §5), each marked `covered by Ara CHNa` (check what her CHNa landed under `SERV_IRUHADEV_JUNE_CLASS_PATH`; if it has not
   run, what its kickoff takes) or `Elesis`, each with June's behaviour; the quest route (35569-35577 decoded from the staged `FieldQuest.lua`,
   the reward path in `Handlers_Quest.cpp`); the `LevelUpEvent.lua` rows (level 15, level 35, transcendence); `CorrelateUnitClassTemplet`,
   `AnimationNameTable` lobby names, the creation-screen sites (`plan_march_side.md` §10); the promotion and reward items in the live `item.kim`
   (`kim.py`). Sized for CHNa / CHNb.
11. **Shared data.** Which state names the C++ enum needs; every `SI_*` id March's passive / skill code reads with its June `RNSI_*` id; her 7
   `UnitTemplet` rows June vs March (are the string ids in our string table?); the dash-jump fireball state's phase.
12. **S3 census baseline:** `s3_no_state.py --family X2GUEL.cpp --preprocess` with the 35 / 125 rows (`--append-missing`, as Ara's CH0) so CH6
   compares like with like.
13. **Amend the kickoffs** CH0b-CH5, CHNa, CHNb ("Amended by CH0" sections: filters with real column numbers, addresses, counts, any split).

**Done gate:** the tsvs exist; `ch0c_listing_check.py` passes on every registration row; every Elesis checklist row has a phase or n/a; 311 / 311
tables and every skill id mapped; every thunk target and uncarved override has a row; assets union routes a / b / c and the motion kom is named;
the motion question answered; the Way of the Sword, summon, blood-hook and CH-N maps exist; kickoffs amended; open questions listed.

### CH0 - Done (2026-11-12)

Read-only: no flag, no build, no code. Outputs in `june2017_re/char_elesis/` (tables in the tsvs; per-phase detail in each kickoff's "Amended by CH0").

**Tooling.** Ara's builders copied with Elesis's range / vtable / registrars: `ch0_build_vtable.py`, `ch0_skill_listing_emu.py` (four switches + two compares,
follows `329cc2c0`), `ch0_march_live_states.py`, `ch0_build_state_skill.py` (her line / class rules, the CH0 splits), `ch0_build_state_enum_names.py`,
`ch0_build_keys.py`, `ch0_build_class_map.py` (+ `ch0_make_hand_names.py`, `ch0_hand_phases.tsv`, `ch0_hand_names.tsv`), `ch0_unmapped_callees.py`,
`ch0_passive_ids.py`, `ch0_build_enum_names.py`, `ch0_build_unittemplet.py`, `ch0_numtables.py`, `ch0_build_checklist_rows.py`, `ch0_amend_kickoffs.py`,
`ch0_funcmap_rows.py`. `charport_adis.py` gained `elesis`; `char_eve/ch0_build_assets.py` gained `elesis` and an optional `held_motions` key (Ara's outputs
byte-identical through the new script); `s3_no_state.py` `TSV_MISSING_CLASSES` gained `X2GUEL.cpp`: `--append-missing` appended 82 rows (35: 29, 125: 53),
the old `s3_reform_ids_no_state.tsv` is a byte prefix of the new one.

**Step 0.** `charport_states.py init 32b87a60 32b81be0 32b864f0 3202b110` -> `ch0_reg.tsv` (177: 111 + 57 + wrapper 4 + base 5; `31fe22d0` / `31fd8f00`
register nothing); `ch0c_listing_check.py` **177 / 177**; hand rows: three `~` rows in `32b864f0` (ids 321 / 326 / 329, shared tail @32b87203), roll ids
9 / 11 / 12, hyper id 23 (`31fdd490`), id 130. `32b8c470`: (a) 31 entries from 28031, (b) types 5..16, (c) 28000..28236 (237 entries, 55 targets),
(d) 28601..28605, compares 28600 / 900 (`ch0_skill_switches.tsv` 287 rows, `ch0_skill_listing_rows.tsv` 167). **No second switch** (0x68c = base
`3208ddc0`). `32b4e790`: 22 blocks, each vslot 0x7a8 per state (`ch0_setskilllevel_blocks.tsv`).

**Maps.**
- `ch0_class_map.tsv`: 687 starts (274 carved, 413 listing-only): state-fn 406, thunk 106, override 65, helper 56, dead 47, not-Elesis 5, registrar 2.
  **409 thunk slots = 402 jmp thunks (every target has a row) + 7 delegate adapters** (`push edx; call; ret`, pushed by `InitPassiveSkillState`);
  **66 overrides = 65 in range (32 uncarved) + `32c69f70`**. Per phase: CH1a 33, CH1b 23, CH1c 2, CH1d 9, CH1d2 9, CH1e 21, CH1f 14, CH1f2 25,
  CH1g 18, CH1g2 27, CH1h 24, CH1i 4, CH1j 14, CH1k 15, CH1m 3, CH1n 1, CH1o 5, CH1q 18, CH2a 28, CH2b 28, CH2b2 15, CH2c 15, CH2d 27, CH3a 24,
  CH3a2 16, CH3b0 4, CH3b 24, CH3b2 11, CH3c 36, CH3d 7, CH4a 12, CH4b 6, CH4c 11. 45 `new` helpers keep `?` with a hint (the owning phase names them).
- Overrides named by body: 0x29c `GetReduceDebuffDurationRate`, 0x428 `GetDefenceRateBySkill`, 0x4ac returns 0xbb = id 187 `ESI_SWORD_WAIT`, 0x670
  `IsInJobLine` (line 3 = 35 / 125), 0x720 / 0x724 / 0x72c plain getters of `+0x9714` / `+0x9718` / `+0x9720`, 0x740 / 0x744 snapshot pair, 0x4f8 /
  0x4fc = the C10e hooks, 0x734 = defender-side Blood Revenge; 0x534 (base `3208e8c0`, June-only) moved to CH1o; 0x558 / 0x83c named in CH1h / CH1o.
- `ch0_state_skill.tsv`: **311 / 311** (299 mapped, 6 dead data, 6 `INIT_*`), **127 / 127 ids**; 13 March-only states; no state id in a `UCHAR` (7l).
  The 22 bare `SI_` tables are Dark Knight / Crimson Avenger skill states (11 CH2d, 10 CH3c, 1 CH3d); QUICK = 4 `ESI_QUICK_STANDUP_ATTACK_*` (CH1a).
  Endurance Power (28201 / 28218) has no switch state: its states (ids 205 / 206 / 207) go to CH1k. `ch0_state_enum_names.tsv`: 134 of 213 names
  missing in `EL_STATE_ID`, 37 shared ones off by 1-2 after the USI_END rebase (register by June's id).
- `ch0_keys.tsv` 248 (both 234, june-only 9, neither 1, march-only-site 4), every key owned. June does not read `WayOfSword.lua`'s `Const_Elesis_*`.
- `ch0_checklist_rows.tsv`: 89 rows (all 67 planning rows of Parts A1-A3), 74 phase, 15 n/a; 20 are the numerical-only tables (state phases); `3201ad10` has no caller in her range; `32b77b30` (+0x3f00 writer) is dead code.
- `ch0_passive_ids.tsv`: 55 March `SI_*` reads, 41 differ (8000s vs 28000s), 12 cross-line. `ch0_enum_names.tsv`: 119 of 127 ids need registering.
  `ch0_unittemplet_elesis.tsv`: all string ids present. S3 baseline `ch0_s3_baseline.txt`: 261 rows, 254 no_state.
- **Way of the Sword** (`ch0_wos.tsv`, 84 rows): 28 code-bearing functions, **CH1q not split**; June's class side adds the Dark Knight buffs (BFI 471 /
  472 / 473, BTI 434 / 435: names to register), scales Destruction by `+0x971c`, and the gain getters come from Dark Force 28410. Emulator `CAC_WSP` OK.
- **Summons** (`ch0_summon.tsv`): five sites, all behind IsMyUnit; `CheckSummonSpecialAttack` and the `+0x8e74` writer are base code already
  ported / shared; NPC 1635 / 1641 / 1971 lack `NPCTemplet` rows, `Enum.lua` names and `X2UnitManager.h` members (CH3b0).
- **Blood hooks** (`ch0_blood.tsv`, 30 rows): CH4c = 0x4f8 (Dark Force HP intake), 0x4fc (Judgment), 0x734 (Blood Revenge; the base virtual and
  the 0x730 / 0x734 user DamageReact calls are missing in March), Blood Runaway, Madness, the transcendence berserker chain, the Shadow Edge list.
- **CH-N** (`ch0_classn.tsv` 49 rows, `ch0_classn_route.tsv`): 7 covered by Ara CHNa, 3 by Ara CHNb, 7 Elesis CHNa, 3 CHNb, 3 CH5; quests 35569-35577
  confirmed, `Handlers_Quest.cpp` already grants any class.

**Assets.** `ch0_assets.tsv` 11,328 rows; **`ch0b_stage_list.tsv` 6,452**: `STAGE-JUNE DRY RUN: 28 kom(s), 6452 staged (6314 new, 138 replaced, 0
identical), 0 refused`; new kom **data206** (57 motions); class motions -> data033. **Motion question:** `MOTION_EL_GM.x` and `Motion_EL_BH.x` drop
five names each that March's script plays: **held for CH1** (`ch1_held_motions.tsv`); `_Common`, `_Field`, `_Emotion`, `_PK` go now. The four missing
March animations are renamed by June (`SI_A_SSK_LEAP_CRASH_*`, `SI_A_SSK_RUSHING_SWORD`, `SI_SA_EL_SPIRAL_BLAST`, `SI_SA_SGM_VICTORIOUS_SWORD`).
Closure 6,856 rows (96 missing files, 92 in the list, 4 in no June kom). Icons: 75 misses on 12 files, all in the list. Enum gate (`luabind.py check
ELESIS_KNIGHT`): 4 `ENUM MISSING NAME` (CH1 adds them), 0 STATE, 0 unbound.

**Item 6.** `ch0_unmapped_identified.tsv` 22 rows: 13 same, 5 june-only, 2 differs, 2 unknown; **no `CH<n>0` phase**: each falls in an Opus phase
already (`32008a20` CH3b0; `3227c200`, `32489b20` CH4a; `32358df0` CH4b; `322d4a60`, `322d4ba0`, `322cf1b0` CH4c; `3208e8c0` CH1o) or is a pure-key
test CH2d writes (`31fc7ac0`, as `ARME_ANY_SKILL_KEY`). `funcmap.tsv`: 96 rows `Char Elesis CH0`; rows 1061 (vtable `334fae10`) and 1420 (`32b45e0c`
is Lu/Ciel's) corrected.

**Splits and kickoffs.** CH1f -> CH1f / CH1f2 (SPK / SBH), CH1g -> CH1g / CH1g2 (SDK / SCA; the review moves to CH1g2, covering CH1e-CH1g2), CH2b ->
CH2b / CH2b2, CH3a -> CH3a / CH3a2, CH3b -> CH3b / CH3b2 (special actives / actives). Every kickoff CH0b-CH6 has "Amended by CH0".

**Plan-doc inconsistencies.** Stat rows are moot (the flat `StatTable.lua`, e37c9211, has 35 / 120 / 121 / 125); CHNa lines moved (`X2Unit.h:2085-2151`,
`:2731-2802`, `X2GageManager.cpp:205-220`, `X2Data.cpp:2925-2936`); `X2ComboTree.cpp:302` and the lobby particles are dead / need nothing; the five
`X2GUEL.cpp` class switches are whole-body ports (CH1m / CH5 / CH4a / CH1e-g / CH1o), not CHNa edits; only `GetThirdLine...` gains 35 / 125; 0x4f8 is
Dark Force's HP intake, not a WoS damage effect (checklist row corrected in the tsv); 0x730 is the Shadow Edge list and `32b7e740` the Grand Master
aura, not blood hooks; `Motion_EL.x` / `_SK.x` / `_Lobby.x` are June's in the live data033 already; the plan's CH1 table says the CH1g review covers
CH1e-CH1g (now CH1g2). **Drift:** none. **Open for the user:** none.

## Phase CH0b - Every June Elesis asset, whole, first

**Run with:** Sonnet 5.5 high, then a user play-test. No C++ unless CH0 found a March reader a June asset breaks (then
`SERV_IRUHADEV_JUNE_CHAR_ELESIS_ASSET`). Sub-tag `DATA`. After char-ara CH6's walk.

**Scope:** every non-Lua row of `ch0b_stage_list.tsv`: motions (to the kom CH0 named; held ones packed and kept for CH1 final), body / face /
hair, equipment (35 / 125 promotion sets included), textures, effect meshes, sounds, cut-in art, portraits, `POPUP_TR_*`, the summon NPCs' meshes,
the 54 emotion motions; the class-change dialogs and guides (`adopt-june`, `luabind.py check`; `LevelUpEvent.lua` is CHNb's). Stage byte-exact
(`kom.py stage-june`), pack each kom, copy each (CLAUDE.md, 2026-11-10). Save `data\ErrorLog_pre_CH0b_elesis.txt` first. Gate: `python
tools/skillicons.py check <Elesis's skill-id range from CH0>` = PASS (stage what it lists; the shared `DLG_SKILL_Passive_*` sheets included; a
June-side gap is a `JUNE_BUGS.md` row).

**Done gate:** the stage list re-runs 0 `missing` / `march-copy`; in game one Elesis per March job tier, village and dungeon, no new `KEM_ERROR` /
`Motion Error` / missing texture or mesh against the pre-CH0b log (the KEM 433 frame warning gone); a costume, a weapon and three skills look right.

### CH0b - Done (2026-11-12)

Sonnet 5.5. No C++, no flag (`_ASSET` not needed: CH0 found no March reader a June asset breaks, and the play-test checks that). Full build skipped: no code change. **Copied (CLAUDE.md 2026-11-10, no ask):** 30 `COPY OK`; `kom.py status`: 0 need a pack, 0 need a copy.
- **Reference:** `ErrorLog.txt` (307,614 B) saved as `data\ErrorLog_pre_CH0b_elesis.txt` before staging.
- **Staged:** `kom.py stage-june --jobs 8 june2017_re/char_elesis/ch0b_stage_list.tsv`: `STAGE-JUNE OK: 28 kom(s), 6452 staged (6314 new, 138 replaced, 0 identical), 0 refused, 34s` (the dry run matched CH0). The 138 replaced March files are backed up under `kom_backup_stage_june/20261112_144313` and listed in `june2017_re/char_elesis/ch0b_replaced.tsv` (data021 1, data033 4 = `Motion_EL_Common` / `_Field` / `_Emotion` / `_PK`, data044 1, data052 9, data063 8, data130 30, data143 80, data145 5). New kom **data206** (57 June-only motions). Held, not staged (`ch1_held_motions.tsv`): `MOTION_EL_GM.x`, `Motion_EL_BH.x`; March's copies stay for CH1 final.
- **Lua:** `adopt-june data034` (`DLG_1ST_` / `DLG_2ND_Class_Change_Guide_Elesis` new, `Class_Change_Guide_Elesis` replaced) and `adopt-june data217` (`DLG_Class_Change_Elesis_SK` / `_PK` / `_GM` / `_BH` replace March's in data034; `_DK` / `_CA` new in data217): both `ADOPT-JUNE OK (bytecode)`, 9 staged, 0 refused (backups `20261112_144330` / `_144334`). `luabind.py check` over the 9: 0 unbound, ENUM 0 missing each; `luasrc.py ledger --check` PASS (0 FAIL, 0 WARN); `core --check` PASS. Not touched, as the kickoff says: `LevelUpEvent.lua` (CHNb), `DLG_Unit_Create_ClassChange_Button_Elesis.lua` (CH5).
- **Packed:** `kom.py pack` x30 (28 + data034 + data217), 30 `PACK OK`, 0 removed entries in any (`vs live: ... -0 removed`).
- **Done-gate re-run:** `stage-june --dry-run` over the same list: `6452 staged (0 new, 0 replaced, 6452 identical), 0 refused`. 0 `missing` / `march-copy`.
- **Skill icons:** `skillicons.py check 106 900 28000-28064 28200-28236 28400-28420 28600-28605`: was 75 misses on 12 files; now 131 skills, 0 misses, 2 known June gaps, `RESULT: PASS`. Gap: 28208 / 28229 (`SI_A_SPK_AVATAR_OF_THE_FIRE`, tree picture) is absent from June's own `DLG_Elesis_SKILL_ScreenShot_02.TET` (staged sheet identical to June's; June has 15 pieces, none of them it): `JUNE_BUGS.md` row 625 + two `skillicons_june_gaps.tsv` rows.
- **Not staged, June's own gaps (CH0):** `HQ_CutIn_Ara_normal.tga`, `HQ_CutIn_Ara_Rage.tga`, `ELISIS_RIDING_RAMIEL_SWORD_TRACE.DDS`, `SA_EL_MEGA_SLASH_TRACE` exist in no June kom; nothing to stage.
- **Save:** untouched. Every March class has a unit (20 `wewe` 8 lv7, 40 `SaberT` 29 lv70, 21 `eresisu` 30 lv77, 29 `ElesisGM` 120 lv80, 30 `ElesisBH` 121 lv80); 35 / 125 wait for CHNa.
- **Side note, not acted on:** `kom.py pack` reported ~118 changed entries in data145 and ~119 in datai008 against the live kom although this stage replaced only 5 / 0 files there: the live koms were older than `kom_contents` for those files (an earlier plan's staging, never copied), now in sync. Nothing was removed.
- **MODS.md:** "Stage 7 Elesis, CH0b" section with the backups and the revert.
- **Play-test (user) is the gate** (below). Log tag `[JUNE_CHAR]` has no lines (no code); the check is `ErrorLog.txt` against `ErrorLog_pre_CH0b_elesis.txt` (`KEM_ERROR`, `Motion Error`, missing texture / mesh, and whether the KEM 433 frame warning on her part meshes is gone). Her class script and combat motions are still March's, so she plays as before; her June meshes / textures / sounds / icons load.
- **Logs checked (user play-test passed, run 14:52-14:55, after the copy):** `ErrorLog.txt` (1,476 lines) against `ErrorLog_pre_CH0b_elesis.txt` (1,769 lines): **0** `KEM_ERROR`, **0** `Motion Error`, 0 `EXCEPTION`, 0 `UNHANDLED`; no missing Elesis texture or mesh. The rest is other characters' and village lines (`JUNE_FX PET_PREVIEW_LOOKUP`, `JUNE_ENGINE`, `JUNE_RESULT`, `OpenXET` sidecars), not this stage's.
  - **Open, routed to CH1 final (as Ara's CH0b):** 21 `ErrorEnum(433)` `Anim Frame : 79, Mesh Frame : 80` in one burst at 14:52:52 on `MESH_EL_BASIC_FACE_SORTED.X` and the five `MESH_EL_SPK_CASH_*` / `_BASE_HAIR_SORTED.X` part meshes. The pre-log has none because it never loaded her body, so it is not a regression; the done gate's "warning gone" is **not met**: June's part meshes (80 frames) still play March's 79-frame motion. CH1 final (June's motions and script) re-runs the check; a line still present there becomes a `JUNE_BUGS.md` row.
- **Drift:** none new.

## Phase CH1 - Deploy unit: June script, motions and the class core

**Groups** (CH0's map fills the counts and the exact function lists; the kickoffs carry the filters):

| Sub-phase | Run with | Scope |
|---|---|---|
| CH1a | Sonnet high | Movement and slot-head states: wait + wait-state-change, walk, jump (6), dash basics, throw item, common buff (`COMMON_BUFF_FrameMove` 0x5f0 `32b6def0`), hyper + the three job hyper states, charge, the `QUICK` group (4) |
| CH1b | Sonnet high, Opus review (covers CH1a-CH1b) | Reactions: damage (21 `ESI_DAMAGE*`), stand-up (6), die, start, win / lose / draw blocks (0x90 / 0x94 / 0x98), wall, amusement, frozen, transformed |
| CH1c | Sonnet high | Riding (27 `ESI_RIDING*`), pepper run (5), dance (3) |
| CH1d | Sonnet high | Ground combos with no job tag (`ESI_COMBO*`) |
| CH1d2 | Sonnet high, Opus review (covers CH1c-CH1d2) | Dash and dash / jump combos with no job tag (`ESI_DASH*`), incl. the dash-jump XZ fireball `32b770e0` if CH0 puts it here |
| CH1e | Sonnet high | Saber Knight / Grand Master lines (`SSK`, `SGM` combo and dash-combo states, and the non-skill states CH0 assigns to them) |
| CH1f | Sonnet high | Pyro Knight / Blazing Heart lines (`SPK`, `SBH`) |
| CH1g | Sonnet high, Opus review (covers CH1e-CH1g) | Dark Knight / Crimson Avenger lines (`SDK`, `SCA`; all June-only: every row is `new`, and every state name is a new `EL_STATE_ID` member) |
| CH1h | Sonnet high | Overrides below vslot 0x600: ctor `32b70ee0` / dtor `32b70990` offset map, Init `32b52560`, ReInit `32b81900`, InitEffect `32b4ef40`, InitComponent `32b81710` (its Way of the Sword calls under `_WOS`), DamageReact `32b73ff0` (her tutorial test `32b74065`), HyperModeBuffEffectStart 0xc4 `32b74300`, the particle arrays 0x540 / 0x548 / 0x54c, the state-id getters 0x4ac / 0x4b0 / 0x4b4 (by name, never the number), DetonationStateChange 0x4d4 `32b73e10`, 0x534 `32b90430`, 0x558 `32b6fb80`; the hit hooks 0x4f8 / 0x4fc / 0x734 only as declarations (CH4c) |
| CH1i | Sonnet high | Overrides 0x600-0x7bc: HyperModeFrameMove `32b7f090` -> `32b7b820`, CanRidingState 0x628 `32b70130`, 0x670 `32b64290`, SetEquippedSkillLevel `32b4e790` (22 id blocks); InitPassiveSkillState `32b88300`, 0x6a0 / 0x6a4, 0x29c, 0x2b8 only as declarations (CH4) |
| CH1j | Sonnet high | Common* overrides 0x7c0-0x7e8 (`32b70180`, `32b70220`, `32b724b0`, `32b88070`, `32b4edc0`; 0x7d8 / 0x7dc are tail jumps to the base) and the helpers only CommonFrameMove reaches (`32b6e050`, `32b7ac20`, the effect-set stops `32b7b5f0` / `32b7b6c0` / `32b7b760`, the debuff cleanse `32b87520`, the aimed effect mesh `32b813d0`, `32b7e580`); its Way of the Sword call under `_WOS`, its blood calls under `_BLOOD` |
| CH1k | Sonnet high, Opus review (covers CH1h-CH1k) | Skill-use overrides from 0x7f0 (the seven `CX2GUUser` skill-use helpers are called, never copied): SpecialAttackEventProcess `32b4ee20`, CanUseSpecialAttack `32b84a50` (its `CheckSummonSpecialAttack` call under `_SUMMON`), 0x7fc `32b52590`, 0x800 `32b73e60`, NoStateChangeActionFrameMove `32b818b0` + Endurance `32b80c60` / `32b80840`, SpecialAttackNoStageChange 0x810 `32b72550`, IsValideSkillRidingOn 0x818 `32b4ef10`, 0x828 `32b70290`, 0x834 `32b70d10` -> `32b70af0`, and **`ReservedSkillStart`** (Rules) |
| CH1m | Opus high, Opus review | Registration: June's InitState wrapper `32b87a60` and its parts `32b81be0` / `32b864f0` against March's `InitState` / `InitStateCommon` / `InitStateByUnitClass`, the hyper-mode table names (`GetHyperModeTableName` `31fdd490`), START / WIN / LOSE roll (`31fe22d0`), base event states (`3202b110`), `ESI_WAIT_STATE_CHANGE`, the June-only `EL_STATE_ID` members CH1a-CH1k declared, the `_LIVE` gate |
| CH1n | Opus high, Opus review | The `CX2GUUser` base callbacks she registers (Ara's CH1n triage first: only rows earlier characters did not cover); `SetEquippedSkillFuncBySkillSlotIndex` `32b8c470` head (bounds, slot tables, the 7i rule, the pre-switches; June drops the snapshot call and sets `+0x11`: `plan_checklist_rows.md` item 14) + the dev free-skill gate |
| CH1o | Opus high, Opus review | The virtuals under `_LIVE`; June-only and changed user virtuals she overrides (0x83c `32b732e0`, 0x670, the rest CH0 gives this phase); `SetSkillLevelStateData` if she has one in June; gaps the CH1a-CH1k reviews routed |
| CH1p | Opus high, Opus review | 7ag pressed-index space (`UT_ELESIS` joins June's set at `X2GUUser.cpp:26778-26797`; her decode, `CountUsedSkill` `X2GUEL.cpp:3157`, every reader together); hooks 7h / 7l / 7o / 7p / 7x; the June-only script keys CH0 gives an owner here (the four `EFFECT_SET_LIST_*` variants, `RELATIVE_SKILL_LEARN_CHECK`, `IGNORE_STATE_DEFENCE`, `IGNORE_DEFENCE_DECLINE`); the reserved-buff path (7ai, checklist l.870-871) checked for her ids |
| CH1q | Opus high, Opus review | Way of the Sword (`ch0_wos.tsv`): June's bodies inside every March function of the class that differs (Start / End Vigor, Start / End Destruction with the Dark Knight buff ids, `WayOfSwordStateChange` `32b7f370`, the state-end hooks, `GetFinalDamage_WayOfSword` `32b74ed0`, `SetCriticalPercent_WayOfSword` `32b4ef90`, the gain getters 0x720 / 0x724 / 0x72c replacing the two stand-ins), `CX2ElesisGageData::InitWhenGameIsOver` `32a34c60`, `CX2ElesisMyGageUI` `InitUI` `32e6e060` and `RefreshWayOfSwordUI` `32e69e70`, the snapshot pair 0x740 / 0x744 as `StoreGageSnapshot` / `RestoreGageSnapshot` (the Crest bases stay unported, as Elsword's); divergence rows 417 / 420 / 421 / 424 / 427 settled; sub-tag `WOS` |
| CH1 | Opus high, Opus review, user play-test | Define `_LIVE` (+ `_WOS`); adopt `ELESIS_KNIGHT.lua`; copy CH0b's held motion koms (if any); pack, copy; the play-test on 8 / 29 / 30 / 120 / 121, incl. the `[JUNE_SKILL] SBUF` lines of the reserved-buff path (Endurance Power, Flame Sword) |

**Transcription rules (CH1a-CH1k):** as Ara's (`JUNE_CHARACTER_ARA_PHASES.md`, "Phase CH1 - Deploy unit: June script, motions and the class
core", "Transcription rules"), with Elesis's flags and her class range `0x32b4e770`-`0x32b90b6f`: `inside` rows inside March's function under
`SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` with March's in `#else` and a one-file probe compile; `new` rows under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE`; a
later group's callee declared with `// June <va>: body in <phase>`; an unmapped engine callee is "this needs Opus" (stop). June's numbers and
quirks verbatim; `m_fNowStatePowerRate` (7i) with June's operator; June's BFI arguments (7ai). **Way of the Sword, summon and blood lines inside a
function a CH1a-CH1k phase transcribes** are written under `_WOS` / `_SUMMON` / `_BLOOD` (an `#ifdef` inside the `_LIVE` body) when they call a June
piece CH1q / CH3b0 / CH4c has not written yet; March's live Way of the Sword calls stay March's until CH1q. **Cancel macros (CH1b review):** only
`SKILL_CANCEL_AFTER` carries June's expansion; where the listing tests `+0x7e1c` (`bFootOnLine`) before the keys, a walk / back-walk / dash / ZX cancel
is the file-local `WALK_` / `BWALK_` / `DASH_CANCEL_AFTER_ON_LINE_` / `ZX_CANCEL_AFTER_EX_ON_LINE_` (top of `X2GUEL.cpp`), else March's macro.

**Done gate (CH1 final):** Elesis (8 / 29 / 30 / 120 / 121) loads June's script and motions; `[JUNE_CHAR] ELESIS DATA` logs the load; no new
`KEM_ERROR` / `Motion Error` / missing-animation line against a pre-CH1 run; combos, dash, jump, damage and riding play June's animations; the Way
of the Sword gauge fills both ways, vigor and destruction start and end as June, and the state survives a stage change; each skill cast twice
shows its cool time and spends MP; `charport_scan.py` / `charport_keys.py ELESIS_KNIGHT` show nothing new.

### CH1a - Done (2026-11-12)

All 33 class-map rows of phase CH1a are written, all `inside`: June's body in March's own `CX2GUEL` function under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`, March's in `#else`, tag `// char-elesis CH1a: June <va>` in each block. 18 were `listing-only` (no Ghidra function) and were read with `charport_adis.py elesis --phase CH1a`; none needed "this needs Opus". No `_CORE` (no `new` row), no `_WOS` / `_SUMMON` / `_BLOOD` line: March's live `GetWayOfSwordState()` calls stay March's.
`Always.h` gets `_LIVE` as `//#define` with one guard (needs `SERV_IRUHADEV_JUNE_BUFF_AWAKE_LOCK`, as Ara). New: `june2017_re/char_elesis/ch1_offsets.tsv` (Ara's class-agnostic rows + `+0x95d0` / `+0x9618` / `+0x96e4`); 33 funcmap rows `Char Elesis CH1a`; checklist L259 marked (none of these states writes the state power rate). No `JUNE_BUGS` row: nothing here is a June defect.

| June | `CX2GUEL::` | June | `CX2GUEL::` |
|---|---|---|---|
| `32b4f000` `32b529b0` `32b7f820` | Wait StartFuture / Start / EventProcess | `32b7fad0` | Walk EventProcess |
| `32b6e570` `32b4f030` `32b529e0` `32b7fd00` | JumpReady / JumpUp / JumpDown / JumpLanding EventProcess | `32b80020` | Dash EventProcess |
| `32b6e620` `32b52ac0` `32b802a0` | DashEnd StartFuture / FrameMove / EventProcess | `32b6e660` `32b805d0` | DashJump / DashJumpLanding EventProcess |
| `32b4f2f0` `32b4f320` `32b75000` | DashJumpPowerLanding StartFuture / Start / EventProcess | `32b6e980` `32b4f340` `32b6ea10` `32b4f380` | JumpAttackZ EventProcess / FrameMoveFuture, JumpAttackX EventProcess / FrameMoveFuture |
| `32b53800` `32b4f1d0` `32b53880` `32b4f210` `32b4f250` | StandUpAttack Front EventProcess / End, Back EventProcess / End / StateEndFuture (the four `QUICK` states register the same callbacks) | `32b700d0` `32b70100` `32b7f580` | SwordWait StartFuture / Start / EventProcess |
| `32b6e0c0` | PlayAnimationSwordWaitStart | `32b57240` `32b5f7c0` `32b5fac0` | CAN_NOT_ATTACK_JUMP_DOWN EventProcess, QUICK_STANDUP_ATTACK_FRONT / BACK_ADVANCED EventProcess |

**Where June differs from March** (the other 31 are March's logic written out whole, each checked against the raw listing):
- **`32b6e0c0` PlayAnimationSwordWaitStart:** the fatal threshold is 0.15 instead of 0.25 while `m_uiChivalryLevel` (`+0x95d0`, the level of `RNSI_P_SSK_CHIVALRY` 28400, written by `InitPassiveSkillState` `32b88300` @`32b889dd`) is above 0. The class map called this row `new`; March already has the function, so it is `inside` (CLAUDE.md "March has the function").
- **`32b6e660` DashJumpEventProcess:** a branch before the single-key ones for **both direction keys held**: `( oneRight && facing left ) || ( oneLeft && facing right )` clamps `nowSpeed.y` to `GetJumpSpeed()` and goes to JUMP_UP (speed > 0) or JUMP_DOWN; otherwise nothing, and the riding arm is skipped. March has no such branch (both keys fall into its `pureRight` arm).
- **`32b5f7c0` QUICK_STANDUP_ATTACK_FRONT_ADVANCED:** the five cancel macros (skill / walk / bwalk / dash / ZX_EX) already carry June's expansion (`SERV_IRUHADEV_JUNE_BUFF_AWAKE_LOCK`, earlier phases), so the body is March's text.
- **Form only:** `GetMaxAnimationTime()` already is `max - transition` (June's `+0x220 - +0x2b0`); `GetWalkSpeed()` etc. are the XOR triples x `m_fRunJumpSpeedRate`; the `#ifdef RIDING_SYSTEM` / `ELSWORD_WAY_OF_SWORD` / `SERV_ELESIS_SECOND_CLASS_CHANGE` wrappers are dropped in June's copy (all three are defined); Blazing Heart is `GetUnitClass() == UC_ELESIS_BLAZING_HEART` (June reads `GetUnit()->+0x38 == 0x79`).
- **Not in the CH1a range:** `ESI_WAIT_STATE_CHANGE`, `ESI_COMMON_BUFF`, `ESI_THROW_ITEM` and the hyper / hyper-job states use base callbacks (nothing to write; registration is CH1m). **`COMMON_BUFF_FrameMove` `32b6def0` is class-map phase CH1h**, not CH1a (the kickoff text lists it; the map and the 33-row count do not).

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --worktree --files X2Lib/X2GUEL.cpp KTDXLIB/Always.h`: `files: 1 PASS, 0 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`; `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: FAIL` only because `_LIVE` is commented out (by design). `git diff --patience --numstat`: 950 / 0 in `X2GUEL.cpp`, 14 / 0 in `Always.h`; `bytes>=0x80` 13,134 before and after.
- One-file `/Zs` probe of `X2GUEL.cpp` (`census.cl_cmd`, no PCH): 0 errors with `_LIVE` off and with `/D SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (`/P`: `0.15f : 0.25f` 1 hit and the both-keys branch 1 hit with the flag, 0 and 0 without).
- `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h KTDXLIB/Always.h`: `allow-listed: 0`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp`: 0 A/B rows.
- Full build skipped: no play-test (`_LIVE` is off; nothing registers a June body), so no `DEPLOY OK` / `exestr` line.

**Nothing to play-test.** Review: none of its own, the CH1b review covers CH1a-CH1b (no `_DEBUG` define here). Next: `/x2-phase char-elesis CH1b` (Sonnet 5.5 high).

### CH1b - Done (2026-11-12)

All 23 class-map rows of phase CH1b are written, all `inside`: June's body in March's own `CX2GUEL` function under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`, March's in `#else`, tag `// char-elesis CH1b: June <va>` in each block. 7 were `listing-only` (no Ghidra function: the two Die FrameMoves, the four DamageDown FrameMove / EventProcess and AirDownLanding); all 23 were read from the raw listing with `charport_adis.py elesis --phase CH1b`, and none needed "this needs Opus". No `_CORE` (no `new` row), no `_WOS` / `_SUMMON` / `_BLOOD` line. `_LIVE` stays `//#define` (CH1a defined it). New: one file-local macro in `X2GUEL.cpp` (under `_LIVE`, before `s_SkillCutInSet`); `ch1_offsets.tsv` `+0x9638` `m_bAbleQuickStand` / `+0x9639` `m_bAbleQuickStandAttack`; 23 funcmap rows `Char Elesis CH1b`. No checklist row to mark (no state here writes the state power rate and none calls the buff factor); no `JUNE_BUGS` row (no June defect, only changes).

| June | `CX2GUEL::` | June | `CX2GUEL::` |
|---|---|---|---|
| `32b52850` `32b4efc0` `32b528c0` | DieFront Start / FrameMove / EventProcess (also `ESI_RIDING_DIE`) | `32b52900` `32b4efe0` `32b52970` | DieBack Start / FrameMove / EventProcess |
| `32b52b00` `32b52b50` | DamageDownFront FrameMove / EventProcess | `32b52f00` `32b52f40` | DamageDownBack FrameMove / EventProcess |
| `32b532f0` | DamageAirDownLanding EventProcess (a jump to `32b52b50`) | `32b53300` `32b53440` `32b53580` `32b536c0` | StandUpRolling FrontFront / FrontBack / BackFront / BackBack EventProcess |
| `32b4f0d0` `32b4f110` `32b4f150` `32b4f190` | StandUpRolling FrontFront / FrontBack / BackFront / BackBack End | `32b53900` `32b53980` `32b4f270` `32b4f2b0` | StandUpAttack Front / Back NoDown EventProcess / End |

**Where June differs from March** (the other 15 are March's logic written out whole, each read from the raw listing):
- **DamageDownFront / Back EventProcess `32b52b50` / `32b52f40`:**
  - Flags 5 / 6 / 7 (the quick-stand arms) also fire on animation end: `stateChange < now || IsAnimationEnd()`, with the time tests before the key test. This is June's expansion of `ELSE_IF_STATE_CHANGE_ON_EX_`, written as the file-local `ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_` (Ara's, Elesis's own `_LIVE` guard). The animation-end branch above already catches the same condition, so the extra clause never decides (kept as June has it). The inlined alternate-future-animation choice (`+0x8e90` / `+0x8e84`, Luciel's second body) and the accessor `32070cf0` are read as `m_pXSkinAnimFuture` (precedent: Ara / Rena).
  - **No `GetRandomInt()` roll on the X key.** March: `GetRandomInt() < m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent` picks `ESI_QUICK_STANDUP_ATTACK_*`, else `ESI_STANDUP_ATTACK_*`. June's animation-end arm pushes `ESI_QUICK_STANDUP_ATTACK_FRONT` / `_BACK` (`0xb8` / `0xb9`) whenever the X flag is set (no `0x1c8` virtual call in the listing). Written as June has it. **Routed to CH4a:** with the roll gone, check whether `m_iNoDownDamageAtStandUpAttackPercent` still has a June writer in `InitPassiveSkillState` `32b88300`.
  - The key window start is `m_bAbleQuickStand ? keyInputStart : 0.33` (Ara's is 0.66); flags 5 / 6 / 7 of the Front state all test `bChangeStandUpRollingFrontFront` (as March), Back tests `RollingBackFront` / `RollingBackBack` / `AttackBack`.
- **StandUpRolling EventProcess (4):** flag 0 (X in the key window while `m_bAbleQuickStandAttack` -> `ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED` `0xf3` for FrontFront / BackFront, `_FRONT_ADVANCED` `0xf4` for FrontBack / BackBack) also fires on animation end; the `y -= LINE_RADIUS` (Front pair) / `x 1.5` (Back pair) and the dash / WAIT choice are March's. June's copy drops March's `SERV_ELESIS_SECOND_CLASS_CHANGE` wrapper (as June does everywhere).
- **= March:** both Die triples (`DieFrameMove( 0.19, 1.12 )`, `DyingStart` vslot `0x338`, play speed 0.5, `GetShowObject()` inlined), DamageDown FrameMoves (Front: `EventTimerOneshot( 0.19 )` = March's `X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE` branch, defined; Back: `EventTimer( 0.32 )`), AirDownLanding, the four Rolling Ends and the NoDown pair (animation end first, then JUMP_DOWN `y -= LINE_RADIUS x 1.5` / dash / WAIT; Ends: invincibility 0.3 / 0.3).
- **Ids:** by March's names (`USI_*`, `ESI_*`, `GUSI_DIE`); June's numbers differ for `ESI_*` (ch0 `DIFF` rows) and registration is CH1m. Spot-checked against the listings: `USI_DAMAGE_AIR_FALL` `0x26` = 5 + 33, `USI_JUMP_DOWN` `0x11`, `USI_WAIT` `0xd`, `ESI_STANDUP_*` `0x9a`-`0x9f` (June 154-159).
- **Not in this phase though the kickoff text lists them:** the win / lose / draw blocks (`32b52650` / `32b6de60` / `32b52690`) are class-map phase **CH1h**; start, wall, amusement, frozen and transformed use base callbacks (nothing in the class range; CH1m registers them). The class map and the 23-row count govern, as in CH1a.

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --worktree --files X2Lib/X2GUEL.cpp`: `files: 1 PASS, 0 PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`; `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: FAIL` only because `_LIVE` is commented out (by design). `git diff --numstat`: 414 / 0; `bytes>=0x80` 13,134 before and after.
- One-file `/Zs` probe of `X2GUEL.cpp` (`census.cl_cmd`, no PCH): 0 errors with `_LIVE` off and with `/D SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (`/P` with the flag: 10 `ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_` expansions = 3 + 3 + 4, 0 without).
- `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `allow-listed: 0`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp`: 0 A/B rows.
- Full build skipped: no play-test (`_LIVE` is off; nothing registers a June body), so no `DEPLOY OK` / `exestr` line.

**Nothing to play-test.** Review: `kickoffs/char-elesis/CH1b-review.md` (Opus 5.5 high, covers CH1a-CH1b; no `_DEBUG` define here). Next: `/x2-phase char-elesis CH1b review`, then CH1c.

### CH1b - Review (2026-11-12; covers CH1a-CH1b)

Verdict: **pass after one fix.** All 56 functions are `inside` under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (still `//#define`); the spot-checks below were read against the raw listing (`charport_adis.py elesis <va>`), branch by branch, `fcomp` / `test ah` direction included.

**Fixed in place (CH1a, `32b5f7c0` QUICK_STANDUP_ATTACK_FRONT_ADVANCED EventProcess).** The CH1a note said all five cancel macros already carry June's expansion. Only `SKILL_CANCEL_AFTER` does (`SERV_IRUHADEV_JUNE_BUFF_AWAKE_LOCK`). June's walk (`+0x8c98`), back-walk (`+0x8c94`), dash (`+0x8c9c`) and Z / X (`+0x8c88`) cancels also need `bFootOnLine` (`+0x7e1c`) before the key test (@`32b5f901` / `32b5f962` / `32b5f9c4` / `32b5fa28` / `32b5fa7d`), and the Z / X arms are two separate `else if`s. Ara's file-local `_ON_LINE_` macros (char-ara CH1d / CH1e / CH2d) were copied to the top of `X2GUEL.cpp` under `_LIVE` (tag `// char-elesis CH1b review`), and the June body now uses them. It was the only cancel-macro site in CH1a-CH1b. The plan's transcription rules now say so for CH1c-CH1k.

**Checked = June:**
- DamageDownFront / Back EventProcess `32b52b50` / `32b52f40`: the animation-end arm's order (flag 1 attack, flag 4 quick attack `0xb8` / `0xb9`, flag 2, flag 3, stand-up); the X key sets flag 4 with no `GetRandomInt` (no `0x1c8` call); flags 5 / 6 / 7 = `flag && ( stateChange < now || IsAnimationEnd() )`, then `keyInputStart <= now < keyInputEnd && m_bAbleQuickStand && <flag>`; Front tests flag 2 for all three, Back tests flags 2 / 3 / 1; the key window `now > ( m_bAbleQuickStand ? keyInputStart : 0.33 )`; the facing / pure-key flag writes (word stores read byte by byte); the air-fall arm `y -= 30`.
- StandUpRolling EventProcess `32b53300` / `32b53440` / `32b53580` / `32b536c0`: flag 0 -> `0xf3` / `0xf4` / `0xf3` / `0xf4`; `y -= 30` / `30` / `45` / `45`; condition `oneX && m_bAbleQuickStandAttack`; the animation-end clause.
- DamageDown FrameMoves: Front `EventTimerOneshot( 0.19 )` (the inlined `m_fAnimOneshotTimer < t` test at `+0x24c`), Back `EventTimer( 0.32 )`.
- PlayAnimationSwordWaitStart `32b6e0c0`: `m_uiChivalryLevel > 0` (unsigned `jbe`) -> 0.15 else 0.25; fatal when `maxHP x threshold >= nowHP`; the `Wait_W0` (2) / `Wait_W1` (-2) / `Wait` arms and both transition branches.
- DashJumpEventProcess `32b6e660`: the both-keys branch (clamp to `GetJumpSpeed()`, JUMP_UP if > 0 else JUMP_DOWN, the riding arm skipped), the wall landing, dash-jump Z / X on `m_bIsDashAttackPossible`, `oneUp && nowSpeed.y <= 0`, `oneDown` -> `2 x fMaxGSpeed`.
- JumpAttackZ / X EventProcess and FrameMoveFuture, Dash `32b80020` (Blazing Heart X only while `m_fDashComboStartDelay < 0`, the four DASH_END arms, CanDown -45), DashEnd StartFuture / FrameMove / EventProcess (the 0.3 x `GetMaxAnimationTime()` split, Blazing Heart X), QUICK_STANDUP_ATTACK_BACK_ADVANCED.
- `ch1_offsets.tsv` CH1a / CH1b rows (`+0x95d0`, `+0x9618`, `+0x9638`, `+0x9639`, `+0x96e4`, `+0x7d86`-`+0x7d8c`, `+0x8c10`-`+0x8c24`, `+0x4060`) agree with the listings.

**CH4a lead (CH1b):** June's `InitPassiveSkillState` `32b88300` writes `+0x95d0` (@`32b883b3`, `32b889dd`) and `+0x9638` / `+0x9639` (@`32b883bb`, `32b88617`) but never `+0x4060`. The only writer of `m_iNoDownDamageAtStandUpAttackPercent` is the shared base (Get Chance, `X2GUUser.cpp:6443`). Her class needs no writer; the line is in `kickoffs/char-elesis/CH4a.md`.

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --range 34269173^..27f88883`: `files: 2 PASS, 0 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`; `0 deleted`, `0 non-ASCII`, `0 EOL-mismatched`, `0 #else/#endif without the flag token`, `0 encoding change(s)`; `RESULT: FAIL` only for the commented-out `#define`.
- The fix: `--worktree --files X2Lib/X2GUEL.cpp`: `1 PASS, 0 OTHER, 0 FAIL`; its 5 deleted lines are CH1a's own lines inside the flag block (the tag comment and the 4 macro calls). `--base 34269173^ X2Lib/X2GUEL.cpp`: `1 PASS`, `0 deleted line(s)`. `bytes>=0x80` 13,134 before and after, CRLF matched.
- `junediverge.py --check-all`: `allow-listed: 206`, `RESULT: PASS`; `--check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `allow-listed: 0`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp`: 0 A/B rows.
- One-file `/Zs` probe of `X2GUEL.cpp` (`census.cl_cmd`, no PCH), after the fix: 0 errors with `_LIVE` off and with `/D SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`.
- Full build skipped: no play-test.

No `JUNE_BUGS` row, no `_DEBUG` define to retire. Next: `/x2-phase char-elesis CH1c` (Sonnet 5.5 high).

### CH1c - Done (2026-11-12)

The class map gives phase CH1c **2 rows**, both written, both `inside` (June body in March's own `CX2GUEL` function under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`, March's in `#else`, tag `// char-elesis CH1c`). No `_CORE` (no `new` row remains), no `_WOS` / `_SUMMON` / `_BLOOD` line. `_LIVE` stays `//#define`. 2 funcmap rows `Char Elesis CH1c`; `JUNE_BUGS.md` row 626; no checklist row to mark (no state writes the state power rate or calls the buff factor).

| June | `CX2GUEL::` | map column | what |
|---|---|---|---|
| `32b81200` | RidingHyperModeFrameMove | `inside`, listing-only | `CommonHyperModeFrameMove( 0, 0.1, true )`, = March (read from `charport_adis.py elesis`) |
| `32b7b820` | CommonHyperModeFrameMove | CH0 said `new`, `?` | **`inside`**: March has it (`X2GUEL.cpp:11133`, same three arguments, `ret 0xc`); Elsword's CH1c made the same correction |

**The other 35 states of the kickoff list are base callbacks.** Riding (27 `ESI_RIDING*`), pepper run (5), dance (3) and the three phoenix-run states have no function in her range: June's thunks `329cb180`-`329cc960` jump to `CX2GUUser` functions that March's `InitState` registers already, so nothing is transcribed (registration is CH1m). `ESI_RIDING_DIE` uses CH1b's DieFront functions; `ESI_RIDING_HYPER_MODE`'s Start / EventProcess / End are base callbacks too.

**Where `32b7b820` differs from March** (read from the raw listing):
- **Awakening effect-set family:** June calls `32011d20` (begin), `31fd66b0` (present test) and `32011f50` (play, only when present) before the shock particle, and plays the trace `31ffda00` only when the set is absent. File-local stand-ins in `_CONST_EL_CH1C_` ( `ElesisAwakeningEffectBegin` / `SetPresent` = false / `Play`; "not ported ( Stage 12 ), June's empty-name default" ), as Chung / Rena / Raven / Elsword / Eve / Ara. With the stand-in the trace still plays, as March.
- **= March:** world colour `0xff222222` (inlined `SetWorldColor`), **two independent `EventTimerOneshot` timers** (a plain `if` for each, no `else if`, no third timer: the listing falls through from the first block), `ShowMinorParticleHyperModeChange` / `Shock` with the default bone `Dummy1_Rhand`, `UpDownCrashCamera( 30, 0.4 )`, `ClearScreen` (inlined), `ApplyHyperModeBuff` (vslot 0xc0), the three sounds when `bSound`, tail vslot 0x7cc `CommonFrameMove`.
- **Caller:** the ordinary `HyperModeFrameMove` `32b7f090` (CH1i) calls the same helper (`CommonHyperModeFrameMove( 0.05, 0.83 )` in March), so the awakening stand-in applies to every Elesis awakening once `_LIVE` is on.
- **`JUNE_BUGS` 626:** `ESI_RIDING_DAMAGE_FRONT` is defined twice in her script (lines 6945 / 6951, second wins) and `ESI_RIDING_DAMAGE_BACK` has no table; kept verbatim (CH1 adopts the script as is).

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --worktree --files X2Lib/X2GUEL.cpp`: `files: 1 PASS, 0 PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`; `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: FAIL` only because `_LIVE` is `//#define` (by design). `git diff --numstat`: 51 / 0; `bytes>=0x80` 13,134 before and after.
- One-file `/Zs` probe of `X2GUEL.cpp` (`census.cl_cmd`, no PCH): 0 errors with `_LIVE` off and with `/D SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`; the `/P` output has `ElesisAwakeningEffectBegin` 2 times with the flag on, 0 off.
- `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `allow-listed: 0`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp`: 0 A/B rows.
- Full build skipped: no play-test (`_LIVE` is off), so no `DEPLOY OK` / `exestr` line.

**Nothing to play-test.** Review: none of its own, the CH1d2 review covers CH1c-CH1d2 (no `_DEBUG` define here). Next: `/x2-phase char-elesis CH1d` (Sonnet 5.5 high).

### CH1d - Done (2026-11-12)

All 9 class-map rows of phase CH1d are written: 9 `inside` (June body in March's own `CX2GUEL` function under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`, March's in `#else`, tag `// char-elesis CH1d: June <va>`), all 9 `listing-only` (read with `charport_adis.py elesis --phase CH1d`; none needed "this needs Opus"). `_LIVE` stays `//#define`. **First `_CORE` of this plan:** `Always.h` gets `SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE` (defined, with the `_LIVE needs _CORE` guard; `/D ..._LIVE` compiles, so the guard sees `_CORE` defined) for what June uses and March lacks: four `EL_STATE_ID` names appended with June's script ids in the comment (`ESI_COMBO_SDK_ZZZX` 283, `SDK_XX` 287, `SGM_ZZZX` 322, `SBH_ZZZX` 327; CH1m maps them, CH1e / CH1f2 / CH1g own the states) and `bool IsInJobLine( int )` declared (vslot 0x670 `32b64290`, a `new` row of CH1i; same as Ara / Chung / Elsword). No `_WOS` / `_SUMMON` / `_BLOOD` line. 9 funcmap rows `Char Elesis CH1d`; `JUNE_BUGS.md` row 627; no checklist row is assigned to CH1d and no state here writes the state power rate or calls the buff factor.

| June | `CX2GUEL::` | June | `CX2GUEL::` |
|---|---|---|---|
| `32b53a00` `32b53d50` `32b540a0` `32b54680` `32b54910` | ComboZ / ZZ / ZZZ / ZZZZ / ZZZX EventProcess | `32b54b90` `32b55040` `32b554e0` `32b55760` | ComboX / XX / XXX / XXZ EventProcess |

**Where June differs from March** (every function read branch by branch from the raw listing):
- **All nine:** the walk (`+0x8c98`), back-walk (`+0x8c94`) and dash (`+0x8c9c`) cancels also test `bFootOnLine` (`+0x7e1c`), so they use CH1b review's file-local `_ON_LINE_` macros; `SKILL_CANCEL_AFTER` is the shared June macro. Flags that fire on a time test (Z, ZZ, ZZZ, X, XX) also fire on animation end: CH1b's `ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_`, with the time tests before the key test.
- **ZZZ `32b540a0`:** chain order flags 2 / 0 / 3 / 4 / 1 (`m_fEventTime[2..4]` and the flag slots `+0x7d87` / `+0x7d88` / `+0x7d89` are the base arrays). New arms: flag 2 X + class 125 -> `ESI_COMBO_SDK_ZZZX` (`0x11b`), flag 3 X + class 120 -> `ESI_COMBO_SGM_ZZZX` (`0x142`), flag 4 X + class 121 -> `ESI_COMBO_SBH_ZZZX` (`0x147`); flag 0 Z -> `ZZZZ`, flag 1 X -> `ZZZX` as March. The tests are class compares (`GetUnitClass()`, March's own accessor, null-safe like June's inline `GetUnit()` test), not job lines.
- **ComboX `32b54b90`:** order flags 2 / 0 / 1. Flag 2: X + `IsInJobLine( 3 )` -> `ESI_COMBO_SDK_XX` (`0x11f`, new). Flag 1 (Z -> `ESI_COMBO_SBH_XZ`) tests `IsInJobLine( 2 )` where March tested class 121.
- **ComboXX `32b55040`:** flag 2 (down + X -> `ESI_COMBO_SGM_XX_DOWN_X_START`) tests `IsInJobLine( 1 )` where March tested class 120. June has no `SERV_ELESIS_SECOND_CLASS_CHANGE` wrapper.
- **`IsInJobLine` meaning** (read from `32b64290`, body CH1i): line 1 = classes 29 / 120, line 2 = 30 / 121, line 3 = 35 / 125. So once `_LIVE` is on, Saber Knight also takes down + X in XX and Pyro Knight takes Z in X toward the Blazing Heart states. **Leads:** CH1e checks `ESI_COMBO_SGM_XX_DOWN_X_START` for a Saber Knight class in the script, CH1f / CH1f2 `ESI_COMBO_SBH_XZ` for Pyro Knight; CH1 final's play-test covers both on 29 / 30.
- **= March:** ZZZZ (the 550 air speed is unconditional in June; March's sits under `BALANCE_PATCH_20131107`, which is defined), ZZZX, XXX, XXZ (and Z / ZZ apart from the two points above). The not-on-line arm is `StateChange( USI_JUMP_DOWN )` + `y -= LINE_RADIUS` (30), the animation-end arm `USI_WAIT`; ComboX tests animation end first, as March.
- **`JUNE_BUGS` 627:** ZZZ's `SDK_ZZZX` arm tests class 125 only, so a Dark Knight (35) takes the plain ZZZX; Crimson Avenger has no `SCA_*` combo states, so it may be intended. Kept verbatim.
- **Not found to differ:** the Z-family state ids by name (`ESI_*`; June's numbers differ, registration is CH1m). No branch drifted out of scope.

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --with ..._CORE --worktree --files X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h KTDXLIB/Always.h`: `files: 1 PASS, 0 PASS~, 1 DEFINE, 1 OTHER (X2GUEl.h, belongs to _CORE), 0 FAIL, 0 ERROR`; `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: FAIL` only because `_LIVE` is `//#define` (by design). Same with `_CORE` as the flag (`--with ..._LIVE`): `define: KTDXLIB/Always.h:12637`, `files: 1 PASS, 1 DEFINE, 1 OTHER`, `RESULT: PASS`. `git diff --numstat`: `X2GUEL.cpp` 198 / 0, `X2GUEl.h` 9 / 0, `Always.h` 11 / 0; `bytes>=0x80` 13,134 before and after.
- One-file `/Zs` probe of `X2GUEL.cpp` (`census.cl_cmd`, no PCH): 0 errors with `_LIVE` off and with `/D SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`. `/P` branch check: with the flag the four new state names appear 2 times (enum + body) instead of 1, `IsInJobLine` 5 instead of 2 (+3 calls); `ESI_COMBO_SGM_XX_DOWN_X_START` / `ESI_COMBO_SBH_XZ` count the same (June's use replaces March's).
- `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `allow-listed: 0`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: 0 A/B rows.
- Full build skipped: no play-test (`_LIVE` is off), so no `DEPLOY OK` / `exestr` line.

**Nothing to play-test.** Review: none of its own, the CH1d2 review covers CH1c-CH1d2 (no `_DEBUG` define here). Next: `/x2-phase char-elesis CH1d2` (Sonnet 5.5 high).

### CH1d2 - Done (2026-11-12)

All 9 class-map rows of phase CH1d2 are written: 9 `inside` (June body in March's own `CX2GUEL` function under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`, March's in `#else`, tag `// char-elesis CH1d2: June <va>`), 3 `listing-only` (the three ground-dash combos, read with `charport_adis.py elesis --phase CH1d2`; none needed "this needs Opus") and 6 carved. `_LIVE` stays `//#define`; no new `Always.h` define. The dash-jump XZ fireball `32b770e0` is CH1f's (CH0). Eight state names join `EL_STATE_ID` under `_CORE`, June's script id in the comment (CH1m maps, the named phase owns the state): `ESI_DASH_COMBO_SCA_ZZ` 299 / `SCA_ZX_START` 304 (CH1g2), `ESI_DASH_COMBO_SGM_XX_ATTACK` 323 (CH1e), `ESI_DASH_COMBO_SCA_XZ_CHARGE` 330, `ESI_DASH_JUMP_COMBO_SDK_XX` 285 and `SDK_ZX_CHARGE` 328 (CH1g), `ESI_DASH_JUMP_COMBO_SSK_ZZX` 321 (CH1e), `ESI_DASH_JUMP_COMBO_SPK_ZX_READY` 325 (CH1f). 9 funcmap rows `Char Elesis CH1d2`; `JUNE_BUGS.md` row 628; no checklist row is assigned to CH1d2, no state writes the state power rate or calls the buff factor, no `_WOS` / `_SUMMON` / `_BLOOD` line.

| June | `CX2GUEL::` | June | `CX2GUEL::` |
|---|---|---|---|
| `32b559e0` `32b56090` `32b562f0` | DashComboZ / ZZ / X EventProcess (listing-only) | `32b56710` `32b56aa0` `32b56c80` | DashJumpComboZ / ZZ / ZZZ EventProcess |
| `32b56cc0` `32b570d0` `32b57100` | DashJumpComboX / XX / XXLanding EventProcess | | |

**Where June differs from March** (every function read branch by branch from the raw listing):
- **DashComboZ `32b559e0`:** chain order flags 4 / 5 / 1 / 2 / 0 / 3. New: flag 4 (Z) -> `SCA_ZZ` and flag 5 (X) -> `SCA_ZX_START`, both `IsInJobLine( 3 )`. The three March arms test job lines through `IsInJobLine` ( flag 1 Z -> `SSK_ZZ`: line 1; flag 2 Z -> `SPK_ZZ` and flag 3 X -> `SPK_ZX`: line 2 ) where March used `GetUnitClassLine( UCL_FIRST / UCL_SECOND )`; the `SERV_ELESIS_SECOND_CLASS_CHANGE` split is gone. Flags that fire on a time test also fire on animation end; the walk / back-walk / dash cancels test the line (`_ON_LINE_`).
- **DashComboZZ `32b56090`:** = March + `_ON_LINE_` cancels. **DashComboX `32b562f0`:** two new arms, flag 0 X -> `SGM_XX_ATTACK` (class 120 and `GetNowMp() >= 2.0`, the 2.0 has no Enum entry) and flag 1 Z -> `SCA_XZ_CHARGE` (class 125); March had none; `_ON_LINE_` cancels, animation-end flags.
- **DashJumpComboZ `32b56710`:** order flags 1 / 2 / 3 / 0. New: X + `IsInJobLine( 1 / 2 / 3 )` -> `SSK_ZZX` / `SPK_ZX_READY` / `SDK_ZX_CHARGE`; flag 0 Z -> `ZZ` as March. **DashJumpComboZZ `32b56aa0`:** order 1 / 0, new flag 1 X + `IsInJobLine( 1 )` -> `SSK_ZZX`, flag 0 Z -> `ZZZ`. Neither has cancels (as March). June reaches these through the animation-end flags now too.
- **DashJumpComboX `32b56cc0`:** order flags 4 / 1 / 2 / 0 / 3, the class tests are **compares**, not job lines: flag 4 X + class 125 -> `SDK_XX`, flag 1 X + class 120 -> `SSK_XX`, flag 2 Z + class 121 -> `SPK_XZ`, flag 0 X -> `XX`, flag 3 Z + class 120 -> `SGM_XZ`. March tested the first / second class line for `SSK_XX` / `SPK_XZ`, so a Saber Knight (29) and a Pyro Knight (30) lose those two arms. **JUNE_BUGS 628**, kept verbatim.
- **DashJumpComboXXLanding `32b57100`:** new flag 0 X + `IsInJobLine( 1 )` -> `SSK_XXX` (animation end also fires it) before the animation-end `WAIT`; the not-on-line arm is `JUMP_DOWN`, `y -= 45` (`LINE_RADIUS * 1.5f`), as March.
- **= March:** DashJumpComboZZZ `32b56c80` (animation end and `bFootOnLine` -> `DASH_JUMP_LANDING`), DashJumpComboXX `32b570d0` (`bFootOnLine` -> `XX_LANDING`). `IsOnSomethingFuture()` stands for `bFootOnLine` (`+0x7e1c`, CH1b review).
- **Leads:** CH1e checks that the script's `ESI_DASH_COMBO_SSK_ZZ` / `ESI_DASH_JUMP_COMBO_SSK_ZZX` tables serve Saber Knight (29) too ( both reached through line 1 ); CH1f the same for `ESI_DASH_COMBO_SPK_ZZ` / `_SPK_ZX` / `ESI_DASH_JUMP_COMBO_SPK_ZX_READY` on Pyro Knight (30). CH1 final's play-test presses dash + Z, Z, Z / X and dash-jump + X on 29 / 30.

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --with ..._CORE --worktree --files X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `files: 1 PASS, 0 PASS~, 0 DEFINE, 1 OTHER (X2GUEl.h, belongs to _CORE), 0 FAIL, 0 ERROR`; `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: FAIL` only because `_LIVE` is `//#define` (by design). With `_CORE` as the flag (`--with ..._LIVE`): `define: KTDXLIB/Always.h:12637`, `files: 1 PASS, 1 OTHER`, `RESULT: PASS`. `git diff --numstat`: `X2GUEL.cpp` 150 / 0, `X2GUEl.h` 8 / 0; `bytes>=0x80` 13,134 before and after, LF-only lines 296 before and after.
- One-file probe of `X2GUEL.cpp` (`census.cl_cmd`, `/c /Zs /EHsc`, no PCH): exit 0, 0 errors with `_LIVE` off and with `/D SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`. `/P` branch check: each of the eight new state names appears 1 time off, 2 times on (`ESI_DASH_JUMP_COMBO_SSK_ZZX` 3: two arms); `IsInJobLine` 2 off, 15 on; `GetNowMp() >= 2.0f` 0 off, 1 on.
- `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `allow-listed: 0`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: 0 A/B rows.
- Full build skipped: no play-test (`_LIVE` is off), so no `DEPLOY OK` / `exestr` line.

**Nothing to play-test.** Review: `kickoffs/char-elesis/CH1d2-review.md` (Opus 5.5 high, covers CH1c-CH1d2; no `_DEBUG` define exists). Next: `/x2-phase char-elesis CH1d2 review` on Opus 5.5 high.

### CH1d2 - Review (2026-11-12; covers CH1c-CH1d2)

**Verdict: pass, no code change.** All 20 functions (CH1c 2, CH1d 9, CH1d2 9) were read against `charport_adis.py elesis <va>`; every transcription equals June. One bookkeeping fix: `june2017_re/char_elesis/ch1_offsets.tsv` gains 14 `field` rows (`+0x7d88` / `+0x7d89` = `bEventFlagList[3]` / `[4]`, `+0x8c28`-`+0x8c54` = `m_fEventTime[2..5]`), so later listings name the flag slots and times the dash / combo chains read (they printed bare before; the existing rows were right).

| June | checked | result |
|---|---|---|
| `32b559e0` DashComboZ | flag order 4 / 5 / 1 / 2 / 0 / 3 (slots `+0x7d89` `+0x7d8a` `+0x7d86` `+0x7d87` `+0x7d85` `+0x7d88`), times `+0x8c40` / `+0x8c4c` / `+0x8c1c` / `+0x8c28` / `+0x8c10` / `+0x8c34` (index x 12), keys Z / X / Z / Z / Z / X, `IsInJobLine` (vslot 0x670) 3 / 3 / 1 (`push ebx`, ebx = 1) / 2 / none / 2, targets 0x12b / 0x130 / 0xd0 / 0xe1 / 0xb0 / 0xe0; the job-line call only in the key arm; the shared June `SKILL_CANCEL_AFTER` (line test, hyper arm, quick-slot wait) and the three `_ON_LINE_` cancels | = June |
| `32b562f0` DashComboX | flag 0: X, `GetUnit()` non-null, class `0x78`, vslot 0xd8 `GetNowMp()` `fcomp 2.0` / `test ah, 1` / `jne` skip (so MP >= 2.0); flag 1: Z, class `0x7d`; targets 0x143 / 0x14a | = June |
| `32b56cc0` DashJumpComboX | landing on `bFootOnLine == 1` first; order 4 / 1 / 2 / 0 / 3, keys X / X / Z / X / Z, compares `0x7d` / `0x78` / `0x79` / none / `0x78`; targets 0x11d / 0xd3 / 0xe3 / 0xb6 / 0xed; tail `jmp` CommonEventProcess, no cancels | = June; `JUNE_BUGS` 628 stands beside 627 |
| `32b56710` / `32b56aa0` | else-if chains (each `StateChange` jumps to the tail); order 1 / 2 / 3 / 0 and 1 / 0; `IsInJobLine` 1 / 2 / 3 and 1; targets 0x141 / 0x145 / 0x148 / 0xb3 and 0x141 / 0xb4 | = June |
| `32b57100` XXLanding | `bFootOnLine == 0` -> `JUMP_DOWN` + `y -= 45.0` (double); flag 0 X + `IsInJobLine( 1 )` -> 0xd4; then animation end -> `WAIT` | = June |
| `32b56c80` / `32b570d0` / `32b56090` | landing on animation end and line / XX landing / cancels only | = June |
| CH1d nine | ZZZ order 2 / 0 / 3 / 4 / 1 with compares `0x7d` / - / `0x78` / `0x79` / - (targets 0x11b / 0xa7 / 0x142 / 0x147 / 0xa8); ComboX order 2 / 0 / 1 with `IsInJobLine` 3 / - / 2 and animation end tested before the line; ComboXX flag 2 `pureDown` + X + `IsInJobLine( 1 )`; ZZZZ's 550 after the `y -= 30`; animation-end counts per function (Z / ZZ 2, ZZZ 6, X / XX 4, XXX / XXZ 1) match the macro use; every cancel tests `bFootOnLine` | = June |
| CH1c | `32b81200` = `CommonHyperModeFrameMove( 0, 0.1, true )`; `32b7b820`: world colour 0.1333 x 3 / 1.0 = `0xff222222`, two independent one-shot timers, `32011d20` / `31fd66b0` / `32011f50` before the shock, the trace only when `31fd66b0` is false, camera 30 / 0.4, three sounds on `bSound`, vslot 0x7cc tail; the `_CONST_EL_CH1C_` stand-ins sit inside `MODIFY_RIDING_PET_AWAKE` with both callers | = June |
| `EL_STATE_ID` | the 12 `_CORE` names (CH1d 283 / 287 / 322 / 327, CH1d2 299 / 304 / 323 / 330 / 321 / 325 / 285 / 328) against `ch0_state_skill.tsv` registrations | all ids match |

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --with ..._CORE --range 57c3cd0e^..25ae9270`: `files: 3 PASS, 0 PASS~, 1 DEFINE, 2 OTHER (other flags), 0 FAIL, 0 ERROR`; `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: FAIL` only because `_LIVE` is `//#define` (`Always.h:12624`, by design; `_CORE` defined at 12637, both `#error` guards present). The two OTHER rows are `X2GUEl.h`'s `_CORE` hunks.
- `junediverge.py --check-all`: `allow-listed: 206`, `RESULT: PASS`.
- One-file `cl /Zs /Y-` probe of `X2GUEL.cpp` (US_SERVICE defines and includes): exit 0, 0 errors with `_LIVE` off and with `/D SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`; the `/EP` output grows 15,923,614 -> 15,935,824 bytes with the flag.
- No code changed, no `_DEBUG` define exists to retire. Full build skipped: no play-test.

**Nothing to play-test.** Next: `/x2-phase char-elesis CH1e` (Sonnet 5.5 high); its kickoff already carries the CH1d / CH1d2 leads.

### CH1e - Done (2026-11-12)

All 21 class-map rows of phase CH1e are handled: 15 `inside` (9 with a June body under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`, March's in `#else`, tag `// char-elesis CH1e: June <va>`; 6 read and found `= March`, no edit), 6 `new` (plain names under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE`), 10 `listing-only` (read with `charport_adis.py elesis --phase CH1e`; none needed "this needs Opus") and 11 carved. `_LIVE` stays `//#define`; no new `Always.h` define. Three state names join `EL_STATE_ID` under `_CORE`, June's script id in the comment (CH1m maps): `ESI_DASH_COMBO_SGM_XX_END` 324, `ESI_P_SGM_ADVANCED_KNIGHT_MASTERY` 332 and `_SMALL` 333. 21 funcmap rows `Char Elesis CH1e`; `ch1_offsets.tsv` +3 rows (`+0x9620` / `+0x962c` the two loop counters, `+0x9688` `m_hXXDownXLoop`); `JUNE_BUGS.md` row 629. No checklist row is assigned to CH1e, no state writes the state power rate or calls the buff factor, no `_WOS` / `_SUMMON` / `_BLOOD` line.

| June | `CX2GUEL::` | June | `CX2GUEL::` |
|---|---|---|---|
| `32b59e30` `32b5a2a0` `32b5a5a0` | `ESI_DASH_COMBO_SSK_ZZ` / `ZZZ` / `ZZX` EventProcess | `32b5a8a0` `32b5a910` | `ESI_DASH_JUMP_COMBO_SSK_XX` / `XXX` EventProcess |
| `32b5f1c0` `32b5f4c0` | `ESI_DASH_JUMP_COMBO_SGM_XZ_DOWN` / `ESI_COMBO_SGM_XX_DOWN_X_END` EventProcess | `32b72fe0` `32b731a0` | `ESI_COMBO_SGM_XX_DOWN_X_START` / `_LOOP` EventProcess |
| `32b6ad40` `32b6b0a0` `32b522d0` `32b86080` | new: `ESI_COMBO_SGM_ZZZX_EventProcess`, `ESI_DASH_COMBO_SGM_XX_END_EventProcess`, `ESI_DASH_COMBO_SGM_XX_ATTACK_StateStartFuture` / `_EventProcess` | `32b6bc30` `32b6bc80` | new: `ESI_P_SGM_ADVANCED_KNIGHT_MASTERY_StateStart` / `_EventProcess` ( one body for `_SMALL` too ) |

**Where June differs from March** (every function read branch by branch from the raw listing):
- **SSK_ZZ `32b59e30`:** flags 0 ( Z -> `SSK_ZZZ` ) and 1 ( X -> `SSK_ZZX` ) also fire on animation end; **ZZZ / ZZX / XXX / `SGM_XZ_DOWN` / `XX_DOWN_X_END`** = March + the `_ON_LINE_` walk / back-walk / dash cancels and `ZX_CANCEL_AFTER_EX_ON_LINE_` ( new file-local macro use; vslots 0x4b0 / 0x4b4 = `GetComboZStateID` / `GetComboXStateID` ).
- **`32b5a8a0`:** one body for `SSK_XX` and the June-only `SSK_ZZX` ( id 321 ): March's `SSK_XX` body **without** the five cancels. March's `ESI_DASH_JUMP_COMBO_SSK_XX_EventProcess` takes it ( CH1m registers both ids on it ).
- **`XX_DOWN_X_START` / `_LOOP` EventProcess:** June adds, while the animation plays ( the `else` of the end test ), a speed block: on the line `nowSpeed.y = 0`; `nowSpeed.x = 700`, `-500` when the held direction key is against the facing, `0` with no direction key. June's two loop counters are `KProtectedType<int>` ( `+0x9620` Now, `+0x962c` Max ); March's are plain ints, used as March ( stand-in: the type only guards memory edits ).
- **= March (read, no edit):** `SGM_XZ_Init` `32b509a0` and `X_START_Init` `32b509e0` ( the same preload lists ), `SGM_XZ_EventProcess` `32b5f160`, `SGM_XZ_LOOP` `32b5f190`, `X_START_StateStart` `32b72ed0`, `X_START_FrameMove` `32b77380` ( `EventTimerOneshot( 0.01 )` + `EffectSet_SGM_XXX_Start` + the two delete-on lists ).
- **New states:** `ESI_COMBO_SGM_ZZZX` = jump-down ( `y -= 30` ) / WAIT, flag 0 ( X, class 120 ) -> `ESI_COMBO_SGM_XX_DOWN_X_START`, SKILL / `_ON_LINE_` cancels. `ESI_DASH_COMBO_SGM_XX_END` = the same without a flag. `ESI_DASH_COMBO_SGM_XX_ATTACK`: `StateStartFuture` takes 2 MP; the EventProcess keeps X ( flag 0 ) and a second X ( flag 1 ), on a timer tick spends 2 MP and goes on while X was pressed ( not enough MP: `STR_ID_2549` message and `ESI_DASH_COMBO_SGM_XX_END` ), else ends; the time chain is **JUNE_BUGS 629** ( verbatim ). `ESI_P_SGM_ADVANCED_KNIGHT_MASTERY`: half of the starts play `SI_P_SGM_ADVANCED_KNIGHT_MASTERY_GUARD2`; the end falls with `y -= 45` off the line.
- **Shared support:** the `_CORE` functions use the flag-chain / on-line macros, so Ara's second macro block ( `#ifdef _CORE` / `#ifndef _LIVE` ) is repeated for Elesis ( without the ZX macro ); the Grand Master numbers live in `_CONST_EL_CH1E_` ( `XX_ATTACK_MP_COST` 2, `XX_DOWN_X_FORWARD_SPEED` 700, `XX_DOWN_X_BACKWARD_SPEED` -500, the timer offset 0.3 / divisor 10 ); DashComboX's literal `2.0f` ( CH1d2 ) is not rewritten.
- **Leads closed:** CH1d / CH1d2's "does the script serve Saber Knight (29)": the `SSK_*` / `SGM_XX_DOWN_X_*` / `SGM_ZZZX` tables are plain script tables with no class key ( `ELESIS_KNIGHT.lua` `10415` / `10585` / `12819` / `12990` ); which classes June registers them for is `InitStateByUnitClass` `32b864f0` ( class switch on `unit class - 0x1d` ), so **CH1m** reads the registration of ids 208 / 240 for class 29 ( lead ). Ids 332 / 333 are registered in the same function.
- **Not found to differ / drift:** none out of scope. The Advanced Knight Mastery passive that reaches these two states is CH4.

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --with ..._CORE --worktree --files X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `files: 1 PASS, 0 PASS~, 0 DEFINE, 1 OTHER (X2GUEl.h, belongs to _CORE), 0 FAIL, 0 ERROR`; `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: FAIL` only because `_LIVE` is `//#define` (by design). With `_CORE` as the flag ( `--with ..._LIVE` ): `define: KTDXLIB/Always.h:12637`, `files: 2 PASS`, `RESULT: PASS`. `git diff --numstat`: `X2GUEL.cpp` 363 / 0, `X2GUEl.h` 11 / 0; `bytes>=0x80` 13,134 before and after.
- One-file `/Zs` probe of `X2GUEL.cpp` ( `census.cl_cmd`, no PCH ): 0 errors with `_LIVE` off and with `-DSERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`. `/P` branch check: `XX_DOWN_X_FORWARD_SPEED` 1 off, 3 on; `ZX_CANCEL_AFTER_EX_ON_LINE_` expansions 0 off, 7 on; the six new function names appear in both ( `_CORE` ).
- `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `allow-listed: 0`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: 0 A/B rows.
- Full build skipped: no play-test (`_LIVE` is off), so no `DEPLOY OK` / `exestr` line.

**Nothing to play-test.** Review: none of its own, the CH1g2 review covers CH1e-CH1g2 ( no `_DEBUG` define here ). Next: `/x2-phase char-elesis CH1f` ( Sonnet 5.5 high ).

### CH1f - Done (2026-11-12)

All 14 class-map rows of phase CH1f are handled: 9 `inside` (5 with a June body under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`, March's in `#else`, tag `// char-elesis CH1f: June <va>`; 4 read and found `= March`, no edit), 5 `new` (plain names under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE`), 8 `listing-only` (read with `charport_adis.py elesis --phase CH1f`; none needed "this needs Opus") and 6 carved. `_LIVE` stays `//#define`; no new `Always.h` define. One state name joins `EL_STATE_ID` under `_CORE`: `ESI_DASH_JUMP_COMBO_SPK_ZX_LOOP` (June script id 326, CH1m maps). 14 funcmap rows `Char Elesis CH1f`; `ch1_offsets.tsv` +1 row (`+0x961c` `m_uiSPKXZReattakCount`); `JUNE_BUGS.md` row 631. No checklist row is assigned to CH1f, no state writes the state power rate or calls the buff factor, no `_WOS` / `_SUMMON` / `_BLOOD` line. The five script states `ESI_SPK_START` / `_WIN` / `_WIN2` / `_LOSE` / `_LOSE2` in `ch0_state_skill.tsv` run shared functions outside the Elesis range: nothing to transcribe (CH1m).

| June | `CX2GUEL::` | June | `CX2GUEL::` |
|---|---|---|---|
| `32b5ced0` `32b5d1d0` `32b5d590` `32b5d890` | `ESI_DASH_COMBO_SPK_ZX` / `ZZ` / `ZZZ`, `ESI_DASH_JUMP_COMBO_SPK_XZ` EventProcess (June bodies) | `32b770e0` | `ESI_DASH_JUMP_COMBO_SPK_XZ_FrameMove` (June body) |
| `32b50500` `32b50520` `32b76a60` `32b76da0` | `= March`: `SPK_XZ_StartFurture`, `SPK_XZ_REATTACK_StartFurture`, `SPK_ZX_FrameMove`, `SPK_ZZZ_FrameMove` | `32b52300` `32b52320` `32b6b320` `32b6b350` `32b7d2b0` | new: `ESI_DASH_JUMP_COMBO_SPK_ZX_READY_StateStartFuture` / `_EventProcess`, `ESI_DASH_JUMP_COMBO_SPK_ZX_LOOP_StateStartFuture` / `_EventProcess` / `_FrameMove` |

**Where June differs from March** (every function read branch by branch from the raw listing):
- **`SPK_ZX` / `ZZZ` EventProcess:** = March + the `_ON_LINE_` walk / back-walk / dash cancels and `ZX_CANCEL_AFTER_EX_ON_LINE_` (the two listings are byte-equal apart from jump targets). `32b5ced0` is also the body of `ESI_COMBO_SBH_ZZZX` and `32b76a60` its FrameMove (June reuses both; March's `SPK_ZX_*` functions take them, CH1m registers `SBH_ZZZX` on them, CH1f2 needs nothing more for it).
- **`SPK_ZZ` EventProcess `32b5d1d0`:** flag 0 ( Z -> `SPK_ZZZ` ) also fires on animation end ( `ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_` ); + the `_ON_LINE_` cancels.
- **`SPK_XZ` EventProcess `32b5d890`** ( also the `_REATTACK` state ): the Z re-attack ( flag 0, `0 < m_uiSPKXZReattakCount` ) also fires on animation end and uses the key window of `m_fEventTime[0]`; + the `_ON_LINE_` cancels. The landing and jump-down arms are March's.
- **`SPK_XZ` FrameMove `32b770e0`:** June creates the fireball damage effect itself ( `DamageEffect_Elesis_SPK` / `_SBH_DashJump_Combo_XZ`, 4 MP via `IsEnoughMPForAttack`, 0.2 s ): the body of the earlier `SERV_IRUHADEV_JUNE_EL_DASHJUMP_XZ_FIREBALL` block, now the state's own under `_LIVE`; that block stays inside March's `#else` (CH0 amendment; the flag stays defined).
- **New states:** Z-X path of the Pyro Knight line: `ZX_READY` sets the X-attack counter to 5 on the future frame ( `_CONST_EL_CH1F_::DASH_JUMP_ZX_LOOP_COUNT`; `XZ` sets 2 ) and goes to `ZX_LOOP` at animation end; `ZX_LOOP` spends one count per start, loops again on a second X inside the key window while counts remain, lands on the line, and its FrameMove is the XZ fireball with 2 MP, a 0.1 s timer and the `_ZX` damage effects ( **JUNE_BUGS 631**: no jump-down arm ).
- **Leads closed:** CH1d / CH1d2's "Pyro Knight ( 30 ) path through the script's tables": `ESI_DASH_COMBO_SPK_ZZ` / `_ZX` / `_ZZZ`, `ESI_DASH_JUMP_COMBO_SPK_XZ` / `_ZX_READY` / `_ZX_LOOP` and `ESI_COMBO_SBH_ZZZX` are plain tables in `ELESIS_KNIGHT.src.lua` with no class key (lines 11646 / 11704 / 11738 / 11769 / 11816 / 11838 / 14465); which classes reach them is the C++ side already read ( `IsInJobLine( 2 )`, CH1d / CH1d2 ) plus the registration ( CH1m ). Nothing else found to differ; no drift.

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE --with ..._LIVE --worktree --files X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `files: 2 PASS, 0 PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`; `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS`. With `_LIVE` as the flag: `X2GUEL.cpp` PASS, `X2GUEl.h` OTHER ( belongs to `_CORE` ), `RESULT: FAIL` only because `_LIVE` is `//#define` ( by design ). `git diff --numstat`: `X2GUEL.cpp` 180 / 0, `X2GUEl.h` 6 / 0; `bytes>=0x80` 13,134 before and after.
- One-file `/Zs` probe of `X2GUEL.cpp` ( `census.cl_cmd`, no PCH ): 0 errors with `_LIVE` off and with `-DSERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`. `/P` branch check: the five `new` names appear in both; `m_fEventTime[0].stateChange` 31 off, 33 on ( the ZZ and XZ flag chains ).
- `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `allow-listed: 0`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: 0 A/B rows.
- Full build skipped: no play-test (`_LIVE` is off), so no `DEPLOY OK` / `exestr` line.

**Nothing to play-test.** Review: none of its own, the CH1g2 review covers CH1e-CH1g2 ( no `_DEBUG` define here ). Next: `/x2-phase char-elesis CH1f2` ( Sonnet 5.5 high ).

### CH1f2 - Done (2026-11-12)

All 25 class-map rows of phase CH1f2 are handled: 25 `inside` (9 with a June body under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`, March's in `#else`, tag `// char-elesis CH1f2: June <va>`; 16 read and found `= March`, no edit), 0 `new`, 16 `listing-only` (read with `charport_adis.py elesis --phase CH1f2`; none needed "this needs Opus") and 9 carved. `_LIVE` stays `//#define`; no new `Always.h` define, no new `EL_STATE_ID` name (`ESI_COMBO_SBH_ZZZX` runs CH1f's `32b5ced0` / `32b76a60`). One constant namespace `_CONST_EL_CH1F2_` ( `XZ_FIRE_MP_COST` = 2 ) under `_CORE`. 25 funcmap rows `Char Elesis CH1f2`; `JUNE_BUGS.md` row 632; `ch1_offsets.tsv` needs no row. No checklist row is assigned to CH1f2, no state writes the state power rate or calls the buff factor, no `_WOS` / `_SUMMON` / `_BLOOD` line. The five script states `ESI_SBH_START` / `_WIN` / `_WIN2` / `_LOSE` / `_LOSE2` run shared functions outside the Elesis range: nothing to transcribe (CH1m); `ESI_COMBO_SBH_ZZZX` is CH1f's.

| June | `CX2GUEL::` | June | `CX2GUEL::` |
|---|---|---|---|
| `32b61350` `32b61660` `32b61970` | `ESI_COMBO_SBH_XZ` / `XZZ` / `XZZZ` EventProcess (June bodies) | `32b61bd0` `32b61fe0` `32b623f0` | `ESI_DASH_COMBO_SBH_X` / `XX` / `XXX` EventProcess (June bodies) |
| `32b78390` `32b785b0` `32b78920` | `ESI_COMBO_SBH_XZ` / `XZZ` / `XZZZ` FrameMove (June bodies) | `32b62520` | `= March`: `SBH_XXXZ` EventProcess |
| `32b51070` `32b51090` `32b510f0` `32b51110` `32b511d0` `32b51290` `32b51350` | `= March`: the seven `_Init` preload lists | `32b6f710` `32b6f750` `32b6f790` `32b6f7e0` | `= March`: the four dash `_FrameMoveFuture` |
| `32b78b40` `32b78e80` `32b791c0` `32b79500` | `= March`: the four dash `_FrameMove` | | |

**Where June differs from March** (every function read branch by branch from the raw listing):
- **`SBH_XZ` / `XZZ` EventProcess:** the Z press ( flag 0 ) tests only `oneZ`: March also required the Blazing Heart class. The flag also fires on animation end; + the `_ON_LINE_` walk / back-walk / dash cancels. The entrance `ESI_COMBO_SBH_XZ` is `IsInJobLine( 2 )` ( classes 30 / 121, CH1d ), so a Pyro Knight gets the whole X Z Z Z chain: **JUNE_BUGS 632**, kept verbatim. `SBH_XZZZ` = March + the `_ON_LINE_` cancels.
- **Dash X / XX EventProcess:** flags 0 ( X ) and 1 ( Z ) also fire on animation end; + `_ON_LINE_` cancels; the dash end ( dash / `DASH_JUMP_LANDING` / `WAIT` ) is March's. **`XXX`:** flag 0 ( Z -> `XXXZ` ) fires on animation end; no cancels ( March's are commented out ). **`XXXZ`** = March ( `JUMP_DOWN` with `y -= 30` is `LINE_RADIUS`; June's constant is the double 30.0 ).
- **`SBH_XZ` / `XZZ` / `XZZZ` FrameMove:** June adds the new fireball effect sets: `XZ` and `XZZZ` multiply `fMagic` by the fire ball rate ( as March ), then `FlushMp( 2 )` ( vslot `0xe4`, arg 2 unused ) plays `EffectSet_SBH_Combo_XZ_Fire_NEW` / `_XZZZ_Fire_NEW` ( hyper flag, `GetPowerRate() * GetFireBallPowerRate()` ); `XZZ` plays March's `EffectSet_Elesis_SBH_Combo_XZZ` first, then the 2 MP `EffectSet_SBH_Combo_XZZ_Fire_NEW`. A failed `FlushMp` plays nothing. The `*_Fire_NEW` sets and their `DamageEffect_SBH_Combo_XZ_Fire_NEW_*` rows are in June's `EffectSet` / `DamageEffect` data ( staged by CH0b ).
- **= March (read, no edit):** the seven Init preload lists ( compared by string and call kind: equal ), the four dash FrameMoveFutures ( `CommonFrameMoveFuture`, `passiveSpeed.x = GetDashSpeed()`; `XXX` also `m_fDashComboStartDelay` = 0.5 = `SBM_DASH_COMBO_DELAY_TIME` ), the four dash FrameMoves ( 0.135 / 0.363 / 0.492 / 0.27 s; MP 4 = `SPK_DASH_JUMP_XZ_MP_COST`; the effect-set names compared against the exe strings ).
- **Leads closed:** CH1d's "SBH_XZ through the script's tables": the states are plain tables with no class key ( CH1f's finding ), so JUNE_BUGS 632 is reached on a Pyro Knight. Nothing out of scope found; no drift.

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE --with ..._LIVE --worktree --files X2Lib/X2GUEL.cpp`: `files: 1 PASS`, `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`, `RESULT: PASS`. With `_LIVE` as the flag: `1 PASS`, `RESULT: FAIL` only because `_LIVE` is `//#define` ( by design ). `git diff --numstat`: `X2GUEL.cpp` 178 / 0; `bytes>=0x80` 13,134 before and after; LF-only lines 326 before and after.
- One-file `/Zs` probe of `X2GUEL.cpp` ( `census.cl_cmd`, no PCH ): 0 errors with `_LIVE` off and with `/DSERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`. `/P` region check ( the ten SBH functions ): `Fire_NEW` 0 off, 3 on; `XZ_FIRE_MP_COST` 0 off, 3 on; `FlushMp(` 3 off, 6 on.
- `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `allow-listed: 0`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: 0 A/B rows.
- Full build skipped: no play-test (`_LIVE` is off), so no `DEPLOY OK` / `exestr` line.

**Nothing to play-test.** Review: none of its own, the CH1g2 review covers CH1e-CH1g2 ( no `_DEBUG` define here ). Next: `/x2-phase char-elesis CH1g` ( Sonnet 5.5 high ).

### CH1g - Done (2026-11-12)

All 18 class-map rows of phase CH1g are handled: 0 `inside`, 18 `new` (plain names under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE`, tag `// char-elesis CH1g: June <va>`), 13 `listing-only` (read with `charport_adis.py elesis --phase CH1g`; none needed "this needs Opus") and 5 carved. `_LIVE` stays `//#define`; no new `Always.h` define. Eight state names join `EL_STATE_ID` under `_CORE`, June's script id in the comment (CH1m maps): `ESI_COMBO_SDK_ZZZXX` 284, `ESI_DASH_JUMP_COMBO_SDK_XXX` 286, `ESI_COMBO_SDK_XXX` .. `_XXXXXXX` 288-292, `ESI_DASH_JUMP_COMBO_SDK_ZX_FIRE` 329. One new member `m_fDashJumpZXChargeTime` ( June `+0x96f4`, `_CORE`; `ch1_offsets.tsv` +1 row ). 18 funcmap rows `Char Elesis CH1g`; `JUNE_BUGS.md` row 633. The five script states `ESI_SDK_START` / `_WIN` / `_WIN2` / `_LOSE` / `_LOSE2` run shared functions outside the Elesis range: nothing to transcribe (CH1m). Checklist row 7n L499 ( the class-skill float writer `32b73bf0` / reader `32b7d550` ) is written here; no state writes the state power rate or calls the buff factor, no `_WOS` / `_SUMMON` / `_BLOOD` line.

| June | `CX2GUEL::` | June | `CX2GUEL::` |
|---|---|---|---|
| `32b64310` `32b64660` | `ESI_COMBO_SDK_ZZZX` / `ZZZXX` EventProcess | `32b648e0` `32b64bd0` | `ESI_DASH_JUMP_COMBO_SDK_XX` / `XXX` EventProcess |
| `32b64ee0` `32b652d0` `32b656c0` `32b65ab0` | `ESI_COMBO_SDK_XX` / `XXX` / `XXXX` / `XXXXX` EventProcess | `32b65ea0` `32b66200` | `ESI_COMBO_SDK_XXXXXX` / `XXXXXXX` EventProcess |
| `32b519f0` `32b7c0b0` | `ESI_DASH_JUMP_COMBO_SDK_XXX` `_Init` / `_FrameMove` | `32b52330` `32b73bf0` `32b52350` `32b73c90` | `ESI_DASH_JUMP_COMBO_SDK_ZX_CHARGE` `_StateStart` / `_StateStartFuture` / `_FrameMove` / `_EventProcess` |
| `32b6b460` `32b7d550` | `ESI_DASH_JUMP_COMBO_SDK_ZX_FIRE` `_EventProcess` / `_FrameMove` | | |

**What June does** (every function read branch by branch from the raw listing; all are new states, so there is no March body to diff):
- **Ground combo chain:** `SDK_ZZZX` ( X, no class test, to `SDK_ZZZXX` ) and `ZZZXX` ( end ) = jump-down ( `y -= LINE_RADIUS` ) before the animation-end WAIT, then the `_ON_LINE_` cancels. `SDK_XX` .. `SDK_XXXXX`: flag 0 ( X ) goes up one hit and flag 1 ( Z ) jumps to the last hit `SDK_XXXXXXX`; both flags also fire on animation end; `XX` reads its Z window from event time 1, `XXX` / `XXXX` / `XXXXX` from event time 0; here WAIT comes before the jump-down. `SDK_XXXXXX` has one flag ( X or Z ) to the last hit; `SDK_XXXXXXX` has no flag. No Z-X cancel in any of them.
- **Dash-jump X-X-X:** `SDK_XX` sets flag 0 / 1 on `oneX` / `oneZ` with no key window and, on the line, goes to `ESI_COMBO_Z` / `SDK_XXX` / `ESI_DASH_JUMP_COMBO_XX_LANDING`; the cancels after it are dead ( **JUNE_BUGS 633**, kept ). `SDK_XXX` = the Saber Knight `XXX` body ( animation end JUMP_DOWN / WAIT, `_ON_LINE_` cancels incl. Z-X ); its FrameMove ticks at 0.01 s, `FlushMp( 3 )` plays `EffectSet_C_SCA_Dash_ZZZ_Uppercut`, else the not-enough-MP effect at `Dummy2_Lhand` ( 0, 0, 0 ).
- **Dash-jump Z-X charge and fire:** `ZX_CHARGE` start ( both frames ) zeroes `m_fDashJumpZXChargeTime`, the future start also the class-skill float; FrameMove adds the frame time up to 1.0; EventProcess fires at state time 3.0 or when X is released: the class-skill float becomes 0 with the facing key held, else -45 ( set only when different ), then `ZX_FIRE`. `ZX_FIRE` EventProcess: animation end on the line -> `USI_JUMP_LANDING`, then the cancels incl. Z-X. FrameMove ( 0.01 s ): `IsEnoughMPForAttack( 5 )` at `Dummy2_Lhand` ( rotation -45 when aimed down ), then `EffectSet_SDK_Dash_Jump_Combo_ZX` ( float < 0 ) or `_ZX_Front`, power `GetPowerRate() x ( charge + 1 )`, scale `1 + charge x 0.5` on all three axes, no hyper flag.
- **Shared support:** `X2GUEL.cpp` gains `IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_` ( the flag-chain macro that starts with `if` ) in both macro blocks and `ZX_CANCEL_AFTER_EX_ON_LINE_` in the `_CORE` / `#ifndef _LIVE` block; the numbers sit in `_CONST_EL_CH1G_`. `GetClassSkillFloat()` / `SetClassSkillFloat()` ( engine C3e-1 ) carry the class-skill float; no engine edit.
- **Leads closed:** CH1d / CH1d2's "`SDK_*` states of line 3 are plain script tables" ( no class key; registration is CH1m ). Nothing out of scope found; no drift.

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE --with ..._LIVE --worktree --files X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `files: 2 PASS, 0 PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`; `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS`. `git diff --numstat`: `X2GUEL.cpp` 370 / 0, `X2GUEl.h` 29 / 0; `bytes>=0x80` 13,134 and 5,494 before and after; `X2GUEL.cpp` LF-only lines 326 before and after.
- One-file `/Zs` probe of `X2GUEL.cpp` ( `census.cl_cmd`, no PCH ): 0 errors with `_LIVE` off and with `/DSERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`. `/P` check: `ESI_COMBO_SDK_XXXXXXX_EventProcess()` 2 off / 2 on, `EffectSet_SDK_Dash_Jump_Combo_ZX_Front` 1 / 1, `m_fDashJumpZXChargeTime` 9 / 9 ( `_CORE` is always on ).
- `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `allow-listed: 0`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: 0 A/B rows.
- Full build skipped: no play-test (`_LIVE` is off), so no `DEPLOY OK` / `exestr` line.

**Nothing to play-test.** Review: none of its own, the CH1g2 review covers CH1e-CH1g2 ( no `_DEBUG` define here ). Next: `/x2-phase char-elesis CH1g2` ( Sonnet 5.5 high ).

### CH1g2 - Done (2026-11-12)

All 27 class-map rows of phase CH1g2 are handled: 0 `inside`, 27 `new` (plain names under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE`, tag `// char-elesis CH1g2: June <va>`), 11 `listing-only` (read with `charport_adis.py elesis --phase CH1g2`; none needed "this needs Opus") and 16 carved. `_LIVE` stays `//#define`; no new `Always.h` define. Eight state names join `EL_STATE_ID` under `_CORE`, June's script id in the comment (CH1m maps): `ESI_DASH_COMBO_SCA_ZZZ_START` 300, `_ZZZ_LOOP_1` 301, `_ZZZ_LOOP_2` 302, `_ZZZ_END` 303, `_ZX_ATTACK` 305, `_ZX_LOOP` 306, `_ZX_END` 307, `_XZ_FIRE` 331 (`_ZZ` 299, `_ZX_START` 304 and `_XZ_CHARGE` 330 came from CH1d2). One new member `m_usDashComboZZZLoopCount` ( June `+0x9758`, a word; `_CORE`; `ch1_offsets.tsv` +1 row ); the numbers sit in `_CONST_EL_CH1G2_`. 27 funcmap rows `Char Elesis CH1g2`. No `JUNE_BUGS.md` row: nothing here looked unintended ( the points below are June's design, kept ). The five script states `ESI_SCA_START` / `_WIN` / `_WIN2` / `_LOSE` / `_LOSE2` run shared functions outside the Elesis range: nothing to transcribe (CH1m). No state writes the state power rate or calls the buff factor; no `_WOS` / `_SUMMON` / `_BLOOD` line; no checklist row is assigned to this phase.

| June | `CX2GUEL::ESI_DASH_COMBO_SCA_` | June | `CX2GUEL::ESI_DASH_COMBO_SCA_` |
|---|---|---|---|
| `32b51a20` `32b51a70` `32b51a90` | `ZZ` / `ZZZ_LOOP_1` ( both loops ) / `ZX_START` `_StateStart` | `32b51a40` | `ZZZ_START_Init` ( three preloads ) |
| `32b66480` `32b667a0` `32b66ac0` `32b66df0` `32b67120` | `ZZ` / `ZZZ_START` / `ZZZ_LOOP_1` / `ZZZ_LOOP_2` / `ZZZ_END` EventProcess | `32b67420` `32b67760` `32b677d0` `32b67a70` `32b6b760` | `ZX_START` / `ZX_ATTACK` / `ZX_LOOP` / `ZX_END` / `XZ_FIRE` EventProcess |
| `32b67740` `32b67790` `32b677b0` `32b67a50` | `ZX_ATTACK` / `ZX_LOOP` `_StateStart` / `_StateEnd` | `32b52390` `32b523b0` `32b523d0` `32b52420` | `XZ_CHARGE` `_StateStart` / `_StateStartFuture` / `_EventProcess` / `_FrameMove` |
| `32b7c2e0` `32b7c510` `32b7c940` `32b7cd70` | `ZZZ_START` / `ZZZ_LOOP_1` / `ZZZ_LOOP_2` / `ZZZ_END` FrameMove | `32b7d920` | `XZ_FIRE_FrameMove` |

**What June does** (every function read branch by branch from the raw listing; all are new states, so there is no March body to diff):
- **Dash Z-Z-Z chain:** `ZZ` / `ZZZ_START` / `ZZZ_LOOP_1` / `ZZZ_LOOP_2` EventProcess = the flag-chain macro ( flag 0, event time 0, the Z press with no class test ) then animation end -> `USI_WAIT`, then `SKILL_CANCEL_AFTER` and the on-line walk / back-walk / dash cancels. Targets: `ZZ` -> `ZZZ_START` -> `ZZZ_LOOP_1` <-> `ZZZ_LOOP_2`. The two loop states also test the loop count: the Z press loops on only while the count is below 4, and their animation end goes to `ZZZ_END` ( not WAIT ). The count is reset by `ZZ`'s StateStart and grows by one in each loop state's StateStart. `ZZZ_END` has no flag: animation end -> WAIT and the four cancels incl. Z-X ( `ZX_CANCEL_AFTER_EX_ON_LINE_` ).
- **Dash Z-X chain:** `ZX_START` EventProcess = `ZZ`'s with X -> `ZX_ATTACK`; `ZX_ATTACK` ( animation end -> `ZX_LOOP` ) and `ZX_LOOP` set `SetNoDetonation( true )` in StateStart and false in StateEnd. `ZX_LOOP` ends with its animation into `ZX_END` once the **present** frame's state time reaches 2.0 or X is released ( every other state here reads the future frame ), then the on-line cancels. `ZX_END` and `XZ_FIRE` EventProcess = animation end -> WAIT and the on-line cancels.
- **FrameMove effects:** `ZZZ_START` at 0.43 s, `ZZZ_END` at 0.37 s, `ZZZ_LOOP_1` at 0.01 / 0.25 s and `ZZZ_LOOP_2` at 0.08 / 0.31 s each spend 3 MP ( `FlushMp` ) for `EffectSet_C_SCA_Dash_ZZZ` / `_Finish` / `_Uppercut` ( LOOP_1 plays the Uppercut first, LOOP_2 last ), else the not-enough-MP effect at `Dummy2_Lhand`; `CommonFrameMove` comes last.
- **Dash X-Z charge / fire:** `XZ_CHARGE` restarts the charge time ( `+0x96f4`, CH1g's member ) in both starts, FrameMove adds the frame time up to 3.0 ( 1.0 for the Dark Knight's ); EventProcess fires at future state time >= event time 0 with Z released, or >= 3.1. `XZ_FIRE` FrameMove ( `CommonFrameMove` first ): at 0.01 s `IsEnoughMPForAttack( 5 )` at the hand, then `DamageEffect_SCA_Dash_Combo_XZ` placed at `GetPos()` ( not the hand ): power `GetPowerRate() x ( 1 + 0.5 charge )`, life time rate `1 + 0.333 charge`, `SetScale` `1 + 0.133 charge` on all three axes ( `CreateInstance` argument map checked against the 23-argument call: reserve false / -1, force-down rate 1 ).
- **Leads closed:** CH1d2's "`ESI_DASH_COMBO_SCA_*` states through the script's tables" ( plain tables, no class key; registration is CH1m ). Nothing out of scope found; no drift.

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE --with ..._LIVE --worktree --files X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `files: 2 PASS, 0 PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`; `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS`. `git diff --numstat`: `X2GUEL.cpp` 393 / 0, `X2GUEl.h` 36 / 0; `bytes>=0x80` 13,134 and 5,494 before and after; `X2GUEL.cpp` LF-only lines 326 before and after.
- One-file `/Zs` probe of `X2GUEL.cpp` ( `census.cl_cmd`, no PCH ): 0 errors with `_LIVE` off and with `/DSERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`. `/P` check ( `_CORE` is always on ): `ESI_DASH_COMBO_SCA_XZ_FIRE_FrameMove()` 2 off / 2 on, `DamageEffect_SCA_Dash_Combo_XZ` 1 / 1, `m_usDashComboZZZLoopCount` 6 / 6.
- `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `allow-listed: 0`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: 0 A/B rows.
- Full build skipped: no play-test (`_LIVE` is off), so no `DEPLOY OK` / `exestr` line.

**Nothing to play-test.** Review: `kickoffs/char-elesis/CH1g2-review.md` ( Opus 5.5 high; covers CH1e-CH1g2; no `_DEBUG` define here ). Next: `/x2-phase char-elesis CH1g2 review`.

### CH1g2 - Review (2026-11-12; covers CH1e-CH1g2)

**Verdict: pass, no code change.** All 105 class-map rows (CH1e 21, CH1f 14, CH1f2 25, CH1g 18, CH1g2 27) were read against `charport_adis.py elesis --phase <p>`; every listing-only row (carved `n`) was read branch by branch from the raw listing, the carved rows for targets, key tests, constants and names. Every transcription equals June; every `inside` row's `#else` is March's body, untouched. Two bookkeeping fixes, no source edit: checklist row 7n L499 gains its `(Elesis: CH1g ...)` tag (the aim writer is `32b73c90`, `32b73bf0` only resets the float), and `ch1_offsets.tsv` row `0x96f4` now names CH1g2's use (Crimson Avenger dash X-Z, 0 to 3). The `X2GUEl.h:1083` comment ("0 to 1") is left as written (CP949, comment only).

| Spot check (kickoff) | June | result |
|---|---|---|
| Z-Z-Z loops `32b66ac0` / `32b66df0` | word `+0x9758`: `ZZ` start writes 0 (`32b51a20`), `32b51a70` `inc word` ( both loop starts ), Z test `cmp word, 4` / `jae`; flag 0 -> `0x12e` / `0x12d`, animation end -> `0x12f`; key window `start <= t < end`, flag fires on `change < t` or animation end | = port |
| Z-X loop `32b677d0` | inside animation end: `fld 2.0 / fcomp [+0x8088]` ( Now state time ) `jnp` -> change, else `pureX` -> skip; target `0x133`; animation end with X held skips the cancels ( the port's `else if` chain hangs off the `if` ) | = port |
| X-Z charge `32b523d0` / `32b52420` | `stateChange <= future t` with `pureZ == 0`, or `3.1 <= t` -> `0x14b`; FrameMove `CommonFrameMove` first, `charge + elapsed`, clamp at the double 3.0 | = port |
| X-Z fire `32b7d920` | `CommonFrameMove` first, 0.01 s tick; `IsEnoughMPForAttack( 5, Dummy2_Lhand, 0, 0 )` ( vslot 0x6f0 ); `CreateInstance` 23 args: power `GetPowerRate() x ( 1 + 0.5 c )`, pos `GetPos()`, angle / move axis `GetRotateDegree()`, land `+0x80c4`, bReserve false, reserve -1, force-down 1, life time `1 + 0.333 c`, hit count -1, gap -1, mesh 0, hit-add MP -1, two false; `SetScale( 1 + 0.133 c )` x3 | = port |
| Z-Z-Z FrameMoves `32b7c2e0` / `32b7c510` / `32b7c940` / `32b7cd70` | 0.43 `ZZZ`; 0.01 `Uppercut` then 0.25 `ZZZ`; 0.08 `ZZZ` then 0.31 `Uppercut`; 0.37 `Finish`; `FlushMp( 3 )` ( vslot 0xe4 ), `PlayEffectSet` default arguments ( hyper false, -1 / -1, scale 1 ), else `CreateNotEnoughMPEffect( Dummy2_Lhand, 0, 0, 0 )`; `CommonFrameMove` last | = port |
| `EL_STATE_ID` names | CH1g2's 8 ( 300-303 / 305-307 / 331 ) and CH1e-CH1g's 12 equal `ch0_state_skill.tsv` names and ids; the listing pushes ( `0x12c`-`0x133`, `0x14b` ) agree; C++ values are appended, CH1m registers by script name | ok |
| `ch1_offsets.tsv` `0x9758` / `0x96f4` | word count; charge float ( row widened, above ) | ok |
| Macros, namespaces | `IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_` and `ZX_CANCEL_AFTER_EX_ON_LINE_` identical in the `_LIVE` and `_CORE` / `#ifndef _LIVE` blocks and match June's compare directions; `_CONST_EL_CH1E_` .. `_CH1G2_` values right, all used | ok |
| JUNE_BUGS 629 / 631 / 632 / 633 | each row says what the code does ( 629 the SGM XX attack time chain, 631 no jump-down arm in `SPK_ZX_LOOP`, 632 Pyro Knight reaches `SBH_XZ` Z, 633 dead cancels after `SDK_XX` ) | ok |

Notes: June's flag chains read the alternate future animation ( `+0x8e90` / `+0x8e84`, Luciel's second body ); the macros use `m_pXSkinAnimFuture`, right for Elesis. The June event-timer test is the inlined `EventTimerOneshot` the port's `X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE` idiom writes. No drift found; no `_DEBUG` define to retire.

**Gates.**
- `flagcheck ... --range ae70cf92^..HEAD` stopped in the tool ( `Unevaluable: line 27167`, a char-chung `#if` in a peer commit ), so it was run per commit over the five that touch `X2GUEL.*` ( `ae70cf92`, `fe45d2e0`, `5fc51cf0`, `b6a7f9ba`, `40185d05` ). `_CORE` with `_LIVE`: `RESULT: PASS` in all five ( define `KTDXLIB/Always.h:12637`; `files:` 2 PASS, 1 PASS in `5fc51cf0`; 0 PASS~ / OTHER / FAIL / ERROR ). `_LIVE` with `_CORE`: `X2GUEL.cpp` PASS in all five, `X2GUEl.h` OTHER ( belongs to `_CORE` ), `RESULT: FAIL` only for `define: NOT FOUND` ( `_LIVE` is `//#define`, by design ). Every run: `0 deleted, 0 non-ASCII inserted, 0 EOL-mismatched, 0 #else/#endif without the flag token, 0 encoding change(s)`.
- `junediverge.py --check-all`: `allow-listed: 207`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `0 A/B row(s) remain in 2 file(s)`.
- One-file `/Zs` probe of `X2GUEL.cpp` ( `census.cl_cmd`, no PCH ): 0 errors as-is and with `-DSERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`.
- Full build skipped: no play-test.

**Nothing to play-test.** Next: `/x2-phase char-elesis CH1h` ( Sonnet 5.5 high ).

### CH1h - Done (2026-11-12)

All 24 class-map rows of phase CH1h are accounted for: **8 written, 14 = March (no code), 2 routed**. Kickoff list items outside the 24 rows: 0x4b0 `32c69f70` = March, 0x4ac written, 0x4f8 / 0x4fc declared, 0x734 and 0x534 stay with CH4c / CH1o. `_LIVE` stays `//#define`. Two new flags in `Always.h` ( both `//#define`, both "needs `_LIVE`" ): `SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS` ( June's InitComponent / ReInit blocks, the gain getter ) and `_BLOOD` ( ReInit's Shadow Edge clear, the Madness doubling, two hit-hook declarations ). Every block carries `// char-elesis CH1h`; the `_WOS` / `_BLOOD` pieces sit inside the `_LIVE` body with March's lines in the `#else`, so `_LIVE` alone still runs March's Way of the Sword.

| June | Function | Status |
|---|---|---|
| `32b73e10` `32b73ff0` `32b74300` `32b81710` `32b81900` | `DetonationStateChange` / `DamageReact` / `HyperModeBuffEffectStart` / `InitComponent` / `ReInit` | `inside`, carved ( ReInit, DamageReact, HyperModeBuffEffectStart listing-only ) |
| `32b6fb80` | `GetHpTier` ( vslot 0x558 ) | `new`, listing-only, `_CORE` ( same body as CH4c's `32b6fa50` ) |
| `32b6fc40` | `GetReduceDebuffDurationRate` ( vslot 0x29c ) | `new`, listing-only, virtual under `_LIVE` ( the base is called by the buff finalizer, so not `_CORE` ) |
| `32b90420` | `GetSpecialItemUsableExtraStateID` ( vslot 0x4ac ) | `new`, listing-only, under `_LIVE` + `JUNE_BUFF_QUICKSLOT_ITEM`, returns `ESI_SWORD_WAIT` ( June 187 ) |
| `32b4ef40` `32b4ef70` `32b52560` `32b52650` `32b52690` `32b6de60` `32b6def0` `32b70300` `32b70330` `32b90410` ( + `32c69f70` ) | InitEffect, CreateMinorParticleArray, Init, Win, Draw, Lose, COMMON_BUFF_FrameMove, InitEnchantWeaponEffectUnitSpecific, CreateMajorParticleArray, GetComboXStateID ( GetComboZStateID ) | **= March**: Win / Lose / Draw are `GetWinStateID()` / `GetLoseStateID()` by name ( June 75 / 76 / 11 / 12 are `USI_RIDING_WAIT_HABIT` / `_RIDING_WAIT` / `USI_WIN` / `USI_LOSE` in June's numbering, March's differ ); COMMON_BUFF is June's inline `EventTimerOneshot` pair |
| `32b527b0` `32b6e500` | `SetWayOfSwordState`, `CanSpecialReactProcess` | **= March** ( map says `new`; March has both ) |
| `32b70990` `32b70ee0` `32b90520` `32b900e0` | dtor, ctor, scalar deleting dtor, `std::copy` | **= March** + offset map ( below ); `32b900e0` is ReInit's vector erase |
| `32b81300` `32b7beb0` `32b898a0` | Shadow Edge record remove, the two attacker hit hooks | routed to CH4c: declared under `_BLOOD` ( `ClearShadowEdgeRecords` stands for ReInit's loop + clear ) |

**What June does differently from March** ( each checked branch by branch from `charport_adis.py elesis --phase CH1h` ):
- **DamageReact:** a null `DamageData` returns first. The Destruction detonation also needs `damageType != DT_BUFF` and `!g_pMain->GetIsPlayingTutorial()` ( `+0x16a` ). Endurance Power picks `ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_MEMO` when the equipped `RNSI_A_EL_ENDURANCE_POWER` ( 28201 ) templet has `m_iDeleteSkillIDByUpgrade` set ( the upgrade ), else the plain state, both `StateChange( id, true )` ( March asks `GetEqippedSkillMemo` and passes false ). `DamageReactStateChange( -1, -1 )`. The gain is `Calc... x GetWayOfSwordPointMultiplier()` ( CH1q's vslot 0x720 ) and doubled while `m_bIsMadnessActive` with a Madness level ( CH4c's members, declared here ). Provoke revenge plays `EffectSet_CSI_CIP_IRON_WILL` first and skips `DT_BUFF` hits.
- **DetonationStateChange:** `StateChange( ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_DETONATION, true )` ( June script id 207, its Start `32b75c90` is CH1k's; the member joins `EL_STATE_ID` under `_CORE`; June names the table `ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE` for 205 and 207, so the C++ name is mine ) instead of March's `DoStateEndurancePowerRage( true )`.
- **InitComponent:** PvP **or PvE** ( game type 1 or 4 ) resets point and state; afterwards `RefreshWayOfSwordUI` on the gauge UI. **ReInit:** the point moves 1.0 toward the centre first; only game type 1 resets the losing team, every other game ( dungeons too ) ends the mode and enters the TOWARD state ( March restarts it ); the sync field is stored after both branches; the Shadow Edge list is cleared for every unit ( `_BLOOD` ).
- **HyperModeBuffEffectStart:** the four hyper boost handles are skipped for a transcended unit ( `m_iUnitClassTran` ) or an awakening effect set ( stand-in `ElesisAwakeningEffectSetPresent`, false ).
- **GetReduceDebuffDurationRate:** Chivalry learned and HP at or under 15 % of the max ( `GetMaxHp() * 0.15f` as a double ): 0.0 for 22 listed debuffs ( June's six tables, read from the exe, plus six single ids ), else 1.0. Seven BTI names join `X2BuffFactor.h` under `JUNE_ENUM_NAMES` ( 172 / 184 / 254 / 304 / 319 / 449 / 785 ), and `RNSI_A_EL_ENDURANCE_POWER` joins `X2SkillTree.h`.
- **Constructor / destructor ( = March ):** every plain store was read with an x87 stack trace ( the declared members hold June's values: -1 handles, 1.0 / 0.0 / 30.0 floats, `m_bIsDashAttackPossible` 1, the throw offsets 0.66 / ( 40, 130, 0 ) / ( 2000, 100, 0 ) ). `ch1_offsets.tsv` +103 rows: named `m_iPreComboCount`, `m_bIsActiveEndurancePower`, `m_bIsProvokeStateChange`, `m_vecBlazingDanceTargetUid`, the Madness pair, the Shadow Edge vector and the throw members; nine KProtectedType members ( 12 B ) and every other store as `unnamed_<offset>`. The `MemoConditionElesis` allocation ( `+0x7d10`, March has no `MemoCondition` ) is CH4b's. No `JUNE_BUGS.md` row: nothing here looked unintended.

**Leads for later phases:** CH1k / CH1m: state 207 needs its registration ( `ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_DETONATION` = script id 207, Init / EventProcess shared with 205, Start `32b75c90` plays only `EffectSet_EL_ENDURANCE_POWER` ). CH1o: `GetHpTier` becomes the vslot 0x558 override of a new user virtual. CH1q: write `GetWayOfSwordPointMultiplier` ( +0x9714 ); `ch0_wos.tsv` row 21's "two unidentified tests" are `DT_BUFF` and the tutorial flag. CH4c: `ClearShadowEdgeRecords`, the record type, the two hook bodies, `m_usMadnessLevel` / `m_bIsMadnessActive` writers; `ch1_offsets.tsv` rows `+0x9700` / `+0x97aa` / `+0x97ac` are named for it.

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE --with ..._LIVE ..._WOS ..._BLOOD --worktree --files X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `files: 2 PASS, 0 PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`, `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`, `RESULT: PASS` ( `.cpp` +338, `.h` +22 ). `flagcheck SERV_IRUHADEV_JUNE_ENUM_NAMES --worktree --files X2Lib/X2BuffFactor.h X2Lib/X2SkillTree.h`: `2 PASS~`, `RESULT: PASS`. `Always.h` +24 ( the two `//#define` blocks; `_WOS` shows `1 DEFINE` / "not #defined" as `_LIVE` does until CH1 final ).
- One-file `/Zs` probes of `X2GUEL.cpp` ( `census.cl_cmd`, no PCH ): 0 errors off, with `_LIVE`, `_LIVE + _WOS`, `_LIVE + _BLOOD` and `_LIVE + _WOS + _BLOOD`. `cl /P` off / `_LIVE` / all three: `DoStateEndurancePowerRage` 4 / 2 / 2, `CSI_CIP_IRON_WILL` 0 / 1 / 1, `GetIsNowPVPCorrectionGame` 1 / 1 / 2, `ClearShadowEdgeRecords` 0 / 0 / 2, `GetWayOfSwordPointMultiplier` 0 / 0 / 2, `m_bIsMadnessActive` 0 / 0 / 3. Enum values the bodies rely on checked by a constant probe ( `DT_BUFF` 6, `WSS_*`, `GT_PVP`, `TN_RED` / `TN_BLUE`, the seven damage-impact particle ids ).
- `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h X2Lib/X2BuffFactor.h X2Lib/X2SkillTree.h`: `allow-listed: 0`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `0 A/B row(s) remain in 2 file(s)`.
- `bytes>=0x80` before / after: `X2GUEL.cpp` 13,134 / 13,134, `X2GUEl.h` 5,494 / 5,494; LF-only lines `X2GUEL.cpp` 326 / 326.
- Full build skipped: no play-test ( `_LIVE` is off ), so no `DEPLOY OK` / `exestr` line. No `_DEBUG` define exists.

**Nothing to play-test.** Review: none of its own, the CH1k review covers CH1h-CH1k. Next: `/x2-phase char-elesis CH1i` ( Sonnet 5.5 high ).

### CH1i - Done (2026-11-12)

**All 4 class-map rows of phase CH1i accounted for: 2 written ( 1 `inside` under `_LIVE`, 1 `new` under `_CORE` ), 2 = March ( no code ).** The five declaration-only functions ( InitPassiveSkillState `32b88300`, 0x6a0 `32b7ebe0`, 0x6a4 `32b7ee10`, 0x29c, 0x2b8 ) belong to CH4a / CH4b ( 0x29c is CH1h's ); no declaration added here. `_LIVE` stays `//#define`; no new flag. funcmap rows `Char Elesis CH1i` ( 4 ). No `JUNE_BUGS` row: every difference below is June's own state list, nothing that looked unintended.

| June | Function | Result |
|---|---|---|
| `32b4e790` | `SetEquippedSkillLevel` ( 0x7b8, listing-only ) | **inside.** 22 blocks in June's order ( `ch0_setskilllevel_blocks.tsv` ), each `bChangeAll_ \|\| id`, `GetEquippedActiveSkillTemplet( id, true )`, null skips the block, then `SetEquippedSkillLevelStateData` per state ( June's vslot 0x7a8 ) in June's order. Ids are June's `RNSI_` names ( 21 registered in `X2SkillTree.h` under `JUNE_ENUM_NAMES`, values read from `Enum.src.lua`; `RNSI_A_EL_ENDURANCE_POWER` is CH1h's ) |
| `32b64290` | `IsInJobLine` ( 0x670 ) | **new**, `_CORE`. No unit false; line 1 = classes 29 / 120, 2 = 30 / 121, 3 = 35 / 125 ( by `UC_ELESIS_*` names ), else false. Declared and called since CH1d |
| `32b7f090` | `HyperModeFrameMove` ( 0x600 ) | **= March.** `CommonHyperModeFrameMove( 0.05, 0.83, false )`; the callee `32b7b820` is CH1c's |
| `32b70130` | `CanRidingState` ( 0x628 ) | **= March.** Base `31fd7930`, else now state == 187 = `ESI_SWORD_WAIT` ( by name ) |

**What June does differently from March in the 22 blocks:** June no longer sets `ESI_SI_A_EL_DODGE_AND_SLASH_SECOND`, `ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_MEMO`, `ESI_SI_SA_SBH_BLAZING_DANCE_2ND` / `_3RD` / `_4TH`, and has no Snap Crasher block; it adds Sonic Assault: Sting ( memo ready / charge / fire ), Dodge and Slash back, Ejection Buster, Brandish Breaker, Burning Phantom, Dread Weapon, Shade Scratch, Swordfire fly / ground, Judgement Drive final, Blazing Dance back / front, War Prelude fly / ground, Giga Blast. The `true` argument swaps in the upgraded skill's templet.

**Name mapping.** June script ids 220 / 221 carry one name ( `ESI_SI_SA_SSK_EXTINCTION_DEGREE` ) and sit where March has `_DEGREE_UP` / `_DEGREE_DOWN`: March's two members are used. June id 260 ( no registration name ) is March's `ESI_SA_SGM_WAR_PRELUDE` ( the skill's own state, the first state of block 21 before FLY / GROUND; its rebased March value is 259, one off like most members in `ch0_state_enum_names.tsv` ). **22 June-only state members** added to the tail of `EL_STATE_ID` under `_CORE` ( `X2GUEl.h`, each with its June script id and owning phase ): CH2a 311 / 318 / 319 / 320; CH2b 223; CH2c 233 / 234; CH2d 293 / 296 / 297 / 298 / 308; CH3a 334 / 335 / 336 / 337; CH3b 279 / 280; CH3c 294 / 295 / 309 / 310. C++ values follow the enum; registration is by script name ( CH1m ). `SI_SA_SDK_*` / `SI_A_SCA_*` names are June's own, as in the exe's name strings.

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE --with ..._LIVE ..._WOS ..._BLOOD --worktree --files X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `.cpp` PASS +244, `.h` PASS +22; `files: 2 PASS, 0 PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`; `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS`. `flagcheck SERV_IRUHADEV_JUNE_ENUM_NAMES --worktree --files X2Lib/X2SkillTree.h`: `PASS~` ( +26 ), `RESULT: PASS`. `Always.h` untouched.
- One-file `/Zs /EHsc` probes of `X2GUEL.cpp` ( `census.cl_cmd`, no PCH ): 0 errors with `_LIVE` off, with `_LIVE`, and with `_LIVE + _WOS + _BLOOD`. `cl /P` off / `_LIVE`: `RNSI_SA_SGM_GIGA_BLAST` 1 / 3, `ESI_SI_A_EL_DODGE_AND_SLASH_BACK` 1 / 2. First compile caught a duplicate `ESI_SA_SGM_WAR_PRELUDE` ( March has it ); the member was dropped.
- `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h X2Lib/X2SkillTree.h`: `allow-listed: 0`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `0 A/B row(s) remain in 2 file(s)`.
- `bytes>=0x80` before / after: `X2GUEL.cpp` 13,134 / 13,134, `X2GUEl.h` 5,494 / 5,494, `X2SkillTree.h` 23,310 / 23,310. LF-only lines: `X2GUEL.cpp` 326 -> 549 ( the 223-line June body sits after the LF-ended `{` of the function, as flagcheck's neighbour rule asks ).
- Full build skipped: no play-test ( `_LIVE` is off ), so no `DEPLOY OK` / `exestr` line. No `_DEBUG` define; every new block carries `// char-elesis CH1i`.

**Routed.** CH1o: `SetSkillLevelStateData`'s `+ 1.0` term ( checklist row 289 notes it ), the vslot 0x670 `IsInJobLine` decision. CH2a-CH3c: each owns the states listed above. The `X2GUEl.h:489` comment "body in CH1i" is stale ( the body is written ); left, comments are not rewritten.

**Nothing to play-test.** Review: none of its own, the CH1k review covers CH1h-CH1k. Next: `/x2-phase char-elesis CH1j` ( Sonnet 5.5 high ).

### CH1j - Done (2026-11-12)

**All 14 class-map rows of phase CH1j accounted for: 7 written ( all `inside`, `_LIVE` ), 7 = March ( no code ).** No function was genuinely `new`: the 7 helper rows CH0 left as `new` are March namesakes ( `ch0_class_map.tsv` rows corrected ). `_LIVE` stays `//#define`. New flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE` ( `//#define`, needs `_LIVE`, Always.h ). funcmap rows `Char Elesis CH1j` ( 20 ); `JUNE_BUGS` 635.

| June | Function | Result |
|---|---|---|
| `32b70180` | CommonStateStartFuture ( 0x7c0 ) | **inside.** Base first; `m_bIsDashAttackPossible` = now state is neither `ESI_DASH_JUMP_COMBO_SPK_XZ` nor `_XZ_REATTACK` ( June ids 227 / 228 ); in `ESI_SI_SA_SPK_SWORDFIRE_FLY` / `_GROUND` / `ESI_SI_SA_SPK_ETERNAL_FIRE` ( 233 / 234 / 236 ) with Blaze Absolu learned ( new `m_usBlazeAbsoluLevel`, +0x97a4 ) `ResetAnimSpeed( m_fAnimSpeed, true, true )` |
| `32b70220` | CommonStateStart ( 0x7c4 ) | **inside.** Base first, the same anim reset ( bFuture = true: JUNE_BUGS 635 ) |
| `32b88070` | CommonFrameMove ( 0x7cc ) | **inside.** June's order: base, `WayOfSwordStateChange`, Gale, Chivalry, Drastic / XX-down-X / Snap Crasher stops, Stigma, Fire Blossoms, dash combo delay, Ancient Fire; then the new part: `BloodRunawayProc_FrameMove` ( `32b734d0` ) and `ShadowEdgeRecordFrameMove` ( `32b813d0` ), dash time `m_fDashElapsedTime` ( +0x977c; grows in a dash state, zero otherwise except in Leap Crash 28204 / 28226 ), passive MP rate ( `m_fAdvancedKnightMasteryMPValue` +0x9780 over max( 1, MP ) into `SetAdditionalMPChangeRateByPassive` ), `m_fDashChargeTime` ( +0x9790; grows while dashing, runs down by the frame time after ), `32b7e580` / `32b7e740`, `32b7e9a0`, tail `32b6ff20` |
| `32b4edc0` | CommonRender_Prepare ( 0x7e8 ) | **inside.** Vigor `CTT_SKYBLUE`, Destruction `CTT_SCARLET` on `m_RenderParam.cartoonTexType` only ( March also set the render type and outline colour ), then the base |
| `32b7ac20` | ChivalryProc_FrameMove | **inside, differs:** the defence condition is 0.25 when `g_pMain` is in `XS_PVP_GAME` ( +0x8c == 9 ), else 0.5; the attack half = March ( 0.5 ) |
| `32b7b5f0` | DrasticChargeProc_FrameMove | **inside, differs:** `RNSI_A_SGM_DRASTIC_CHARGE` ( 28206 ) and `_UPGRADE` ( 28228 ) keep the loop effects; March tested 8215 only |
| `32b87520` | FireBlossomsProc_FrameMove | **inside, differs:** the enchant is read inline from the Blaze / Frozen / Poison timers ( longest wins, ties Blaze then Frozen ), Shock not read; rest = March |
| `32b4eda0` `32b4edb0` `32b724b0` | CommonStateEndFuture / CommonStateEnd / CommonFrameMoveFuture | **= March** ( base tail jumps; Way of the Sword state into `m_CannonBallCount` ) |
| `32b6e050` `32b7b6c0` `32b7b760` `32b901c0` | Gale / XX-down-X / Snap Crasher / `IsDashState` | **= March** ( states 240-242 = the three `ESI_COMBO_SGM_XX_DOWN_X_*`, skill 8216 ) |

**Corrections to CH0's map.** `32b734d0` is the Blood Runaway tick ( skill 28414 ), not Fire Blossoms ( that is `32b87520` ): now a `new` row `BloodRunawayProc_FrameMove`, CH4a. `32b7e580` is the Grand Master transcendence tick ( skill 28415 ), a passive: moved CH4c -> CH4a as `GrandMasterTranscendenceProc_FrameMove`. Both, with `ApplyGrandMasterAura`, are declared under `_PASSIVE`; `ShadowEdgeRecordFrameMove`, `TranscendenceBerserkerFrameMove`, `MadnessFrameMove` under `_BLOOD` ( `// June <va>: body in <phase>` ). CH4a / CH4c kickoffs carry an "Amended by CH1j" section. `WayOfSwordStateChange` and `GaleBuffFrameMove` are called by name outside `_WOS`: they are March functions, CH1q replaces the first's body in place.

**Names.** `X2SkillTree.h` ( `JUNE_ENUM_NAMES` ): `RNSI_A_SSK_LEAP_CRASH_UPGRADE` 28226, `RNSI_A_SGM_DRASTIC_CHARGE_UPGRADE` 28228 ( values from `Enum.src.lua` ). `X2GUEl.h` ( `_CORE` ): 4 members with constructor values, see the list above. `_CONST_EL_CH1J_`: the two defence rates.

**Gates.**
- `flagcheck ..._CORE --with _LIVE _WOS _BLOOD _PASSIVE --worktree --files X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `.cpp` PASS +151, `.h` PASS +12; `files: 2 PASS, 0 FAIL, 0 ERROR`; `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched, 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS`. `X2SkillTree.h` ( `JUNE_ENUM_NAMES` ) PASS +2. `Always.h`: +12 lines ( the `_PASSIVE` block ), verdict OTHER.
- One-file `/c /Zs /EHsc` probes of `X2GUEL.cpp`: 0 errors with `_LIVE` off, with `_LIVE`, and with `_LIVE + _WOS + _BLOOD + _PASSIVE`. `cl /P` off / `_LIVE`: `m_usBlazeAbsoluLevel` 2 / 4, `RNSI_A_SGM_DRASTIC_CHARGE_UPGRADE` 1 / 2, `m_fDashChargeTime` 2 / 7. All flags the bodies test resolved ON by `cl /P` ( `BALANCE_PATCH_20131107`, `SERV_ELESIS_SECOND_CLASS_CHANGE`, `SERV_ELSWORD_INFINITY_SWORD`, `ELSWORD_WAY_OF_SWORD` ).
- `junediverge.py --check` on the three class files: `allow-listed: 0`, `RESULT: PASS`. `enumscan.py --check`: `0 A/B row(s)`.
- `bytes>=0x80` before / after: `X2GUEL.cpp` 13,134 / 13,134, `X2GUEl.h` 5,494 / 5,494, `X2SkillTree.h` 23,310 / 23,310, `Always.h` 38,738 / 38,738; pure insertions ( `git diff --numstat` 151 / 12 / 2, 0 deleted ).
- Full build skipped: no play-test ( `_LIVE` is off ), so no `DEPLOY OK` / `exestr` line. No `_DEBUG` define; every new block carries `// char-elesis CH1j`.

**Routed.** CH4a: the three `_PASSIVE` bodies and the writers of `m_usBlazeAbsoluLevel` ( `32b88cdd` ), `m_fAdvancedKnightMasteryMPValue` ( `32b88689`, ability 0x2b of 28404 ), `m_fDashChargeTime`'s thresholds. CH1k: the readers of `m_fDashElapsedTime` ( `32b70d72`, `32b7527e` ) sit in its range.

**Nothing to play-test.** Review: none of its own, the CH1k review covers CH1h-CH1k. Next: `/x2-phase char-elesis CH1k` ( Sonnet 5.5 high ).

### CH1k - Done (2026-11-12)

**All 15 class-map rows of phase CH1k plus `ReservedSkillStart` accounted for: 11 written `inside` ( `_LIVE` ), 3 `new` ( 2 `_CORE`, `IsNotCommonMoveState` `_LIVE` ), 2 = March ( `32b70d10`, `32b4fd00` ), 1 `_LIVE` hook ( `ReservedSkillStart` ).** `_LIVE` stays `//#define`. New flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON` ( `//#define`, needs `_LIVE`: the `CheckSummonSpecialAttack` call of her lookup, as the kickoff asks ). funcmap rows `Char Elesis CH1k` ( 16 ); `JUNE_BUGS` 636 / 637; checklist rows L217 / L265 / L266 / L874 / L910 appended `(Elesis: CH1k)`; 6 CH0 map rows corrected ( see below ).

| June | Function | Result |
|---|---|---|
| `32b4ee20` | SpecialAttackEventProcess ( 0x7f4 ) | **inside.** Lookup ( 0x7f8 ), use ( 0x7fc ), `Send_SKILL_USE_REQ`; use condition 3 / 5 ( anytime ) gives `m_iChosenSkillID`, slot and `m_fReservedSkillMp` back; `m_iNowSpecialAttack` = slot + 1; a failed lookup restores only the skill id |
| `32b84a50` | CanUseSpecialAttack ( 0x7f8 ) | **inside.** Quick-slot wait, unit, slot, templet-mismatch text `0x8bae`, level, **War Prelude ( 28009 / 28047 ) is a ground skill while `+0x9796` ( Dignity of Grand Master 28416 level, `m_usDignityOfGrandMasterLevel` ) is 0**, use condition, summon ( `_SUMMON` ), PVP-official, not-PvE, not-dungeon, riding-key wait, `IsValideSkillRidingOn`, MP = `GetActualMPConsume`, dev gate around cool time ( 226 ) / MP ( reserved + chosen, 2549 ) / hyper stone ( 26119 ); then **Injection Stigma 28213: HP must stay above ability `SA_ATK_PHYSIC_REL` x physical attack ( x 0.2 with buff 666 ), text 2627**, outside the dev gate. No chain block, no FP, `ShownSkillCoolTime` is not called ( unlike Ara ) |
| `32b52590` | StateChangeSpecialAttack ( 0x7fc ) | **inside.** Signature takes the skill id under `_LIVE`. No-stage-change: cooldown ( 0x800 ) + `SkillMemoMpRefund` + chosen MP = 0; else reserve ( 0x804 ), Sonic Assault Sting upgrade ( 28029 ) enters `ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_READY` ( 318 ), else `StateChangeBySkillSlot`; processed flag |
| `32b73e60` | SpecialAttackCooldown ( 0x800 ) | **new `_CORE`.** Templet / unit tests first, slot used, `UpNowMp`, finality stone ( `IsTrainingOrPhotozone`, no Infinity branch ), cool time, the Unlimited Blade memo block ( dead, JUNE_BUGS 637 ), `ApplySkillCoolTimeRateByBuff`, `OnSkillUse` |
| `32057b80` | ReservedSkillStart | **`_LIVE`.** As Ara's; `X2GUUser.h` / `.cpp` hook chain widened with `ELESIS_LIVE` ( 7 `#ifndef`s ) |
| `32b72550` `32b4ef10` | SpecialAttackNoStageChange, IsValideSkillRidingOn | **inside.** Endurance Power ids 8207 ( March ) / 28201 / 28218 |
| `32b70290` | IsNotCommonMoveState ( 0x828 ) | **new `_LIVE`.** Base test, less `ESI_SWORD_WAIT` ( future ) and `ESI_DASH_JUMP_POWER_LANDING` ( now ) |
| `32b70d10` `32b70af0` | ShowActiveSkillCutInAndLightByScript, ...AndLight | `32b70d10` = March ( jumps with the same arguments ); `32b70af0` **inside** March's `ShowActiveSkillCutInAndLight`: `EventTimerOneshot` gate ( +0x24c is `m_fAnimOneshotTimer` ), event 0x8b3 cut-ins `HQ_CutIn_Elesis_CASH_21` ( even `rand()` ) / `_22` ( ctor order at `333a82f0` / `333a8393` ); cut-in item override `31fd8a50` not ported ( Stage 12 ) |
| `32b818b0` `32b80c60` | NoStateChangeActionFrameMove ( 0x80c ), ProcessEndurancePowerSkill | **inside.** Base `32008e40` first; Endurance Power action: BFI 232 / 423 ( PvP or PvE, `GetIsNowPVPCorrectionGame` ) at the equipped templet's level ( 7ai arguments ), effect set, sound, then the name strings of 28201 and 28218 |
| `32b80840` `32b75c90` `32b59aa0` `32b4fd00` | Endurance Power states ( 205 / 206 / 207 ) | `32b80840` **inside** ( Start of 205 / 206: March's reset, then `+0x9754` > 0 plays `EffectSet_EL_ENDURANCE_POWER_PUNISH` with it as power rate and sets BFI 479 / 480, else `EffectSet_EL_ENDURANCE_POWER` ); `32b75c90` **new `_CORE`** `ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_DETONATION_StateStart` ( id 207: reset + plain effect set ); `32b59aa0` **inside** ( the walk / back-walk / dash / ZX cancels carry the line test, `_ON_LINE_` macros ); `32b4fd00` = March |

**Corrections to CH0's map.** `32b52590` / `32b73e60` / `32b84a50` / `32b70af0` / `32b80c60` / `32b75c90` rows now name their March pair or plain name ( `ch0_class_map.tsv` ).

**Names / members.** `X2SkillTree.h` ( `JUNE_ENUM_NAMES` ): `RNSI_SA_EL_SONIC_ASSAULT_STING_UPGRADE` 28029, `RNSI_SA_SGM_WAR_PRELUDE_UPGRADE` 28047, `RNSI_A_SDK_INJECTION_STIGMA` 28213, `RNSI_A_EL_ENDURANCE_POWER_UPGRADE` 28218. `X2BuffFactor.h`: BTI 393 / 666, BFI 232 / 420 / 423 / 479 / 480 ( June names ). `X2GUEl.h` ( `_CORE` ): `m_usDignityOfGrandMasterLevel` ( +0x9796 ), `m_fPunishEffectPowerRate` ( +0x9754 ), ctor 0; both written by CH4a ( `InitPassiveSkillState` @32b88a3c / the 28413 block ). `_CONST_EL_CH1K_`: `PushInfoText`, `IsTrainingOrPhotozone`, the 0.2 rate. **Stand-in:** `m_fMPConsume = m_fChosenSkillMp` in the lookup, because March's Rushing Sword states read it until CH2a. State ids `ESI_SWORD_WAIT` / `ESI_DASH_JUMP_POWER_LANDING` compare by name ( script ids 187 / 162 ).

**Routed.** CH1p: the pressed-index decode ( her lookup has none; `CommonSpecialAttackEventProcess` hands March's index ). CH4a: the writers of `+0x9794` / `+0x9796` ( `32b88a30` / `32b88a3c` ) and `+0x9754`. CH2a: Rushing Sword's states read `m_fMPConsume` ( drop the stand-in ). CH1m: register state 207 ( Init / EventProcess = the 205 pair ).

**Gates.**
- `flagcheck JUNE_CHAR_ELESIS_CORE --with _LIVE _SUMMON JUNE_ENUM_NAMES --worktree --files X2GUEL.cpp X2GUEl.h X2SkillTree.h X2BuffFactor.h`: `.cpp` PASS +415, `.h` PASS +14, the two enum headers OTHER ( enum flag ); `0 deleted line(s), 0 non-ASCII inserted, 0 EOL-mismatched, 0 #else/#endif without the flag token, 0 encoding change(s)`, `RESULT: PASS`. `X2GUUser.cpp` / `.h` ( `_LIVE`, `--files` ): PASS +17 / +17 ( `RESULT: FAIL` only for the commented-out `#define` ). `JUNE_ENUM_NAMES` on `X2SkillTree.h` / `X2BuffFactor.h`: 2 PASS~, `RESULT: PASS`. `Always.h`: +24 lines ( `_SUMMON` block, guards ), a pure appended block.
- One-file `/c /Zs /EHsc` probes ( `census.cl_cmd`, no PCH ): `X2GUEL.cpp` 0 errors with `_LIVE` off, with `_LIVE`, and with `_LIVE + _WOS + _BLOOD + _PASSIVE + _SUMMON`; `X2GUUser.cpp` 0 errors off and with `_LIVE`. `cl /P` off / `_LIVE` / `_LIVE + _SUMMON`: `s_SkillCutInSetEvent8b3` 0 / 2 / 2, `CX2GUEL::IsNotCommonMoveState` 0 / 1 / 1, `CheckSummonSpecialAttack` 2 / 1 / 2, `ReservedSkillStart` ( `X2GUUser.cpp` ) 9 / 10.
- `junediverge.py --check` ( the four class / user files ): `allow-listed: 7`, `RESULT: PASS` ( `X2GUEL.cpp` / `.h` alone: `allow-listed: 0` ). `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `0 A/B row(s)`.
- `bytes>=0x80`: `X2GUEL.cpp` 13,134 before / after, `X2GUEl.h` 5,494, `X2GUUser.*` unchanged; pure insertions.
- Full build skipped: no play-test ( `_LIVE` is off ), so no `DEPLOY OK` / `exestr` line. No `_DEBUG` define; every new block carries `// char-elesis CH1k`.

**Nothing to play-test.** Next: `/x2-phase char-elesis CH1k review` ( Opus 5.5 high, covers CH1h-CH1k ), then CH1m.

### CH1k - Review (2026-11-12; covers CH1h-CH1k)

**Verdict: pass, no code change.** All 57 class-map rows ( CH1h 24, CH1i 4, CH1j 14, CH1k 15 ) plus `ReservedSkillStart` were read against `charport_adis.py elesis --phase <p>`. Every written listing-only row ( carved `n` ) was read branch by branch from the raw listing: ReInit, HyperModeBuffEffectStart, GetHpTier, GetReduceDebuffDurationRate, the 0x4ac getter, SetEquippedSkillLevel, IsInJobLine, SpecialAttackEventProcess, the use `32b52590`, IsValideSkillRidingOn, SpecialAttackNoStageChange and the Endurance Power EventProcess. So were the "= March" listing-only rows ( Init, Lose, the enchant / particle arrays, GetComboXStateID, the three tail jumps, HyperModeFrameMove, CommonFrameMoveFuture ). Every transcription equals June, and every `inside` row's `#else` is March's body.

| Spot check (kickoff) | June | result |
|---|---|---|
| Lookup `32b84a50` | quick-slot wait `0 < +0x8d78`, unit, `CommonSpecialAttackEventProcess`, slot, templet ( swap 1 ), mismatch text `0x8bae`, `m_iChosenSkillID` written before the level ( `GetSkillLevel( id, 0, 0, 1 )`, floor 1 ) and the use condition, `word +0x9796 < 1` and id `0x6d69` / `0x6d8f` -> condition 1 = `ASUT_GROUND`, summon `3202daf0`, `3202cba0` / `3202d140` / `3202d3c0`, riding-key wait, vslot 0x818, MP = vslot 0x6a0 ( id, level, 1 ) | = port |
| Dev gate | vslot 0x82c `jne` skips all three: cool time `fcomp 0.0` / `test 0x41` ( fails when left > 0, text 226 ), MP `reserved + chosen > now` ( 2549 ), type `0xd` = `ST_HYPER_ACTIVE_SKILL` + not training / photo zone + no stone 130208 + no Infinity essence ( 26119 ) | = port |
| Injection Stigma | outside the gate: `31edf560( id, 0x6e35 )`, ability 7 = `SA_ATK_PHYSIC_REL` x `+0x4424`, buff `0x29a` = `BTI_BUFF_BLOOD_RUNAWAY_PASSIVE` x double 0.2f, fails when `now HP <= cost` ( text `0xa43` = 2627 ); success writes skill id and pressed slot | = port |
| `32b4ee20` / `32b52590` / `32b73e60` | saved MP / slot / id restored for condition 3 or 5 after either outcome, `m_iNowSpecialAttack = slot + 1`, failure restores the id only; use: vslot 0x810, cooldown ( templet id ) + refund ( argument id, 1, MP ), MP 0, else reserve, 28029 -> state `0x13e` ( false ), slot change; cooldown: `IsMyUnit`, templet / unit, slot mark, `UpNowMp( -mp )`, stone `GetItemByTID`, `SetSkillCoolTimeLeft( id, 0x6a4( templet, level, 0 ), 1 )`, 8003 / memo 608025 / BTI 393 -> BFI 420 ( raw `420, 0, 1, 0, 0` ), `3235a710`, `31fcd3d0` | = port |
| `32b80c60` / `32b80840` / `32b75c90` / `32b59aa0` | BFI `0x1a7` / `0xe8` and `0x1e0` / `0x1df` on `322cc1a0`, raw `( BFI, 0, level, 0, 0 )` = C++ `( BFI, level, NULL, 0, false )`; the Punish set passes hyper ( state byte +0x10 ), power `+0x9754`, life time -1, and the plain sets use every default ( hyper 0, -1 / -1 ); id 207 = 205's reset + plain set; walk / back-walk / dash / ZX cancels all test `bFootOnLine` ( `+0x7e1c` ) | = port |
| `32b70af0` | `m_fAnimOneshotTimer < t` then `EventTimer( t )` ( = `EventTimerOneshot` ), `GetShowCutInAndChangeWorldColor` before `GetShowActiveSkillShow`, set index x 0x90 ( cut-in 0 ), event 0x8b3: even `rand()` -> `337ad560` = CASH_21; bone `Dummy2_Lhand`, hyper = state byte +0x10 | = port |
| CH1h | ReInit: point +-1.0 by the saved state, `GetGameType() == 1` only ( `@32b819b3` ), team kills `+0x4a4` / `+0x4a8`, `32b74930` / `32b74bf0` = End Vigor / Destruction, sync field after both, Shadow Edge loop for every unit; DamageReact: null first, DT_BUFF + tutorial `+0x16a`, upgrade test templet `+0x178`, gain x vslot 0x720 as a double, Madness `2 x gain`; Chivalry tables read from the exe ( `3373daa4` 11 ids, then 61 / 53, 184 / 223 / 161 / 40, singles 99 / 31 / 38 / 87 / 58 / 85: 22 unique ) at `now <= max x 0.15` | = port |
| CH1i / CH1j | 22 blocks: skill ids, script ids ( `0x13e`-`0x151` ) and per-block counts equal `ch0_setskilllevel_blocks.tsv` and the code; IsInJobLine 29 / 120, 30 / 121, 35 / 125; CommonFrameMove order, dash time kept in 28204 / 28226, MP divisor `max( 1, MP )`, charge time `> 0` runs down and `< 0` -> 0; JUNE_BUGS 635 ( `ResetAnimSpeed( speed, 1, 1, 1.0 )` at `32b70281` ) | = port |
| `ReservedSkillStart` chain | `X2GUUser.h` / `.cpp`: one call and one declaration, each under `ELESIS_LIVE` with seven `#ifndef`s ( Arme, Eve, Chung, Elsword, Raven, Rena, Ara ) | ok |
| JUNE_BUGS 635 / 636 / 637 | each row says what the code does ( 636 the null guard on a latent read, 637 the dead memo block ) | ok |

No drift found. No `_DEBUG` define to retire. One lead for CH1m: its kickoff's ReInit question is settled, since June tests game type 1 only ( kickoff amended ).

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --with _CORE _SUMMON _WOS _BLOOD _PASSIVE --range b907661f^..HEAD --files X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `files: 8 PASS, 0 PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR` ( b907661f / 681c0fb5 / 26d3db94 / 3ea67745: `.cpp` +338 / +244 / +151 / +415, `.h` +22 / +22 / +12 / +14 ), `0 deleted, 0 non-ASCII inserted, 0 EOL-mismatched, 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: FAIL` only for `define: NOT FOUND` ( `_LIVE` is `//#define` ). Same on `X2GUUser.cpp` / `.h`: 2 PASS ( +17 / +17 ), same single FAIL reason. `JUNE_ENUM_NAMES` on `X2SkillTree.h` / `X2BuffFactor.h`: 1 PASS, 5 PASS~, `RESULT: PASS`.
- `junediverge.py --check-all`: `allow-listed: 207`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `0 A/B row(s) remain in 2 file(s)`.
- One-file `/c /Zs /EHsc` probes ( `census.cl_cmd`, no PCH ): `X2GUEL.cpp` 0 errors off, with `_LIVE`, and with `_LIVE + _WOS + _BLOOD + _PASSIVE + _SUMMON`; `X2GUUser.cpp` 0 errors off and with `_LIVE`.
- Full build skipped: no play-test.

**Nothing to play-test.** Next: `/x2-phase char-elesis CH1m` ( Opus 5.5 high ).

### CH1m - Done (2026-11-13)

All three class-map rows are `inside`, under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` ( still `//#define` ) with March's bodies in the `#else`: wrapper `32b87a60`
-> `InitState`, part 1 `32b81be0` -> `InitStateCommon`, part 2 `32b864f0` -> `InitStateByUnitClass`. Two more March functions take June's body under the same
flag: `InitStateID` ( June inlines it ) and `ParseCommonRandomState` ( `ch0_classn` row: June's reader is the base `31feaac0` by class name ). No new flag.

**How it was derived.** `june2017_re/char_elesis/ch1m_gen.py` ( Ara's generator, Elesis's names ) writes the C++ from CH0's listing-checked `ch0_reg.tsv`;
`ch1m_spec.py` makes the bytepatch spec. Fresh decompiles of the three functions and `charport_adis.py elesis 32b864f0` read against it. Ids: the `June script
id N` member, else March's registration of the table, else the member of that name: **153 ids, one member each, both ways** ( `ch1m_idmap.tsv` ); one hand
pair, 227 / 228 -> `ESI_DASH_JUMP_COMBO_SPK_XZ` / `_REATTACK` ( March registers that one table for both members too; June's StartFutures `32b50500` /
`32b50520` = March's two ). Every callback resolved ( 0 unresolved ).

**InitState `32b87a60`:** cut-in from the byte table `32b87fec` / jump table `32b87fd8` ( read from the exe ): 120 -> 1, 121 -> 2, 125 -> 3, **every other class
-> 0** ( 8 / 29 / 30 / 35 and the 111 others; unlike Ara's, no class keeps the index ). Then `cmp [esi+0x4088], 0xbb` @32b87af5: `m_CommonState.m_Wait` goes
to `USI_WAIT` unless it is `ESI_SWORD_WAIT` ( written into March's `InitStateID` ). Then part 1, `InitEventStates` ( 3202b110 ), part 2, `InitStateCommonRandom`
( 31fe22d0 ): the C++ keeps March's order ( part 2 before the event states ); part 2's ids ( 208-333 ) do not meet 111-116. START ( Start / EventProcess / End ),
WIN ( `WinStateStart`, `RandomLoseEventProcess` ), LOSE, `InitWaitStateChange( "ESI_WAIT_STATE_CHANGE" )` @32b87ed1 ( 7h ), vslot 0x690, 0x684( 0 ),
`31fd8f00`. `31fc93d0` ( empty stub ) not called.

**InitStateCommon `32b81be0`** ( no class test, 111 names ): hyper id 23 = `GetHyperModeTableName( false )` ( `ESI_HYPER_MODE`, `ESI_EGM_` / `ESI_EBH_` /
`ESI_ECA_HYPER_MODE` for a transcended 120 / 121 / 125 ), FrameMove = Elesis vslot 0x600 `HyperModeFrameMove` ( thunk `32adf320` ), End = base 0x604. No
berserk table ( id 155 is `ESI_STANDUP_ROLLING_FRONT_BACK` here ). Bounding = `InitBoundingStates( "ESI_" )` at June's place ( @1115, 7l ). No skill-chain
lookup ( `32d3ee60` ) in either part. Left out, a comment line each ( as the six earlier ports ): phoenix run 123-126 ( PvP ), dance 127-129 ( Stage 10 ),
riding 93-96 ( Stage 17; March does not register them either ); empty targets `3202a7d0` ( die end ) and `32002f70` ( riding X FrameMove ). 207 registers the
205 table `ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE` with the CH1k pair ( Init / EventProcess of 205, its own `_DETONATION_StateStart` ).

**InitStateByUnitClass `32b864f0`:** one switch, no fall-through: 29 / 120 -> SGM + SSK ( 19 ), 30 / 121 -> SBH + SPK ( 15 ), 35 / 125 -> SCA + SDK ( 23 ); 8 and
every other class register nothing. So a Saber Knight also gets the Grand Master states, a Pyro Knight the Blazing Heart ones and a Dark Knight the Crimson
Avenger ones, as June. The three tail rows ( 321 / 326 / 329 ) jump to the shared `31fd89b0` call @32b87203; listing read for 321.

**`ParseCommonRandomState`:** `ParseCommonRandomStateByUnitClass()` ( June `31feaac0`, Arme's / Ara's ): `GetUnitClassName` gives `ELESIS_KNIGHT` ...
`ELESIS_CRIMSON_AVENGER`, the seven tables of June's `INIT_COMMON_RANDOM_STATE` ( `ELESIS_KNIGHT.src.lua:317-495` ); March's `EL_*` names are gone with
March's script. CHNa kickoff amended ( the `:213` / `:249` / `:2747` switches are done here ).

**JUNE_BUGS 638-639:** 638 `ESI_STANDUP_ATTACK_FRONT_NO_DOWN` / `_BACK_NO_DOWN` / `ESI_WALL_LANDING` have no table in June's or March's script ( March
registers them too; Ara's 600 ); 639 `ESI_DASH_JUMP_COMBO_SSK_ZZX` ( 321 ) takes `SSK_XX`'s EventProcess `32b5a8a0` ( `mov eax, 0x32b8fd40` @32b868e1 ). Row
626 ( `ESI_RIDING_DAMAGE_BACK` ) stands. The 5 `USI_EVENT_*` names with no table are the base `InitEventStates`, every class alike ( rows 157 / 158 ).

**Always.h:** +9, guards only: `_LIVE` needs `ARME_LIVE` ( `ParseCommonRandomStateByUnitClass` ), `HYPER_MODE_FIX` ( hyper StartFutures ), `COMMON_RANDOM_STATE`,
`EVENT_STATES`, `WAIT_STATE_CHANGE`, `BOUNDING_STATE` ( the `_LIVE` probe compiles them ). Checklist L431 / L475 / L512 marked `(Elesis: CH1m)`; funcmap rows
`32b87a60` / `32b81be0` / `32b864f0` PORTED.

**Checks.**
- One-file `/c /Zs /EHsc` probes ( `census.cl_cmd`, no PCH ) of `X2GUEL.cpp`: off, `-D_LIVE`, `_LIVE + _WOS + _BLOOD + _PASSIVE + _SUMMON`: exit 0, 0 errors each.
- `cl /P` off / `_LIVE`: `InitWaitStateChange( "ESI_WAIT_STATE_CHANGE" )` 0 / 1, `InitBoundingStates( "ESI_" )` 0 / 1, `m_iSkillCutInSetIndex = 3` 0 / 1,
  `ESI_SWORD_WAIT != m_CommonState.m_Wait` 0 / 1, `"ESI_DASH_JUMP_COMBO_SSK_ZZX"` 0 / 1, `"EL_KNIGHT"` 1 / 0, `m_StateList[...] = stateData;` 141 / 174.
- `git diff --patience --numstat`: `X2GUEL.cpp` 1237 / 0, `Always.h` 9 / 0; encodings and high-byte counts unchanged ( bytepatch before / after ).
- `flagcheck JUNE_CHAR_ELESIS_LIVE --worktree --files X2Lib/X2GUEL.cpp`: `files: 1 PASS`, `0 deleted, 0 non-ASCII inserted, 424 EOL-mismatched, 0 #else/#endif
  without the flag token, 0 encoding change(s)`; `RESULT: FAIL` only for the `//#define`. The 424 are the `InitStateByUnitClass` block: March's `{` above it
  is the file's one LF-only line there, the block keeps the CRLF of the lines it sits in.
- `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h KTDXLIB/Always.h`: `allow-listed: 0`, `RESULT: PASS` ( no `CX2GUEL::ReInit` allow row needed ).
  `enumscan --check X2Lib/X2GUEL.cpp`: `0 A/B row(s)`.
- Full build skipped: no play-test.
- After the commit ( `65d53f82` ): `flagcheck JUNE_CHAR_ELESIS_LIVE --range 65d53f82^..65d53f82 --files X2Lib/X2GUEL.cpp`: `files: 1 PASS`, same zeros and the
  same 424 EOL lines, `RESULT: FAIL` only for the `//#define`. `Always.h` was committed as its own 9-line hunk ( a peer's june-stat-calc D4a lines stay unstaged ).

**Nothing to play-test** ( `_LIVE` is off ). Next: `/x2-phase char-elesis CH1m review` ( Opus 5.5 high ).

### CH1m - Review (2026-11-13, Opus 5.5)

**Pass, no code change.** Commit `65d53f82` read against fresh `charport_adis.py elesis` listings of `32b87a60` / `32b81be0` / `32b864f0` and the exe bytes.
- **Generator.** `ch1m_gen.py` rerun: both `.inc` files = the `_LIVE` branches line for line ( only the spec's 4 header lines differ ); `idmap.tsv` = `ch1m_idmap.tsv`
  ( 153 ids, no duplicate either way ). It prints one `UNRESOLVED ID MULTI` line for June 205 ( 205 and 207 share the table `..._ACTIVE` ); the idmap settles
  it as 205 `_ACTIVE` / 207 `_DETONATION`. 227 / 228 both `MakeTableReference( "ESI_DASH_JUMP_COMBO_SPK_XZ" )`; 207 = 205's table + the CH1k pair.
- **Independent listing walk.** Every `31fd89b0` call in the listings ( id push, table string, thunk stores since the last inlined `Init` ) against `ch0_reg.tsv`:
  part 1 111 / 111 and part 2 57 / 57, ids, tables and thunks equal ( id 23's table is the `31fdd490` return ). Part 2 jump table `32b8725c` / byte table
  `32b8726c`: 29 / 120 -> SGM + SSK ( 19 ), 30 / 121 -> SBH + SPK ( 15 ), 35 / 125 -> SCA + SDK ( 23 ), the other 91 classes -> `32b8720e` ( epilogue ). 321 and
  326 `jmp 32b87203`, 329 falls into it; after that call the epilogue, so no fall-through = the C++'s `} break;` groups.
- **Wrapper.** Byte table `32b87fec` ( 0x76 bytes, classes 8-125 ): 8 / 29 / 30 / 35 -> 0, 120 -> 1, 121 -> 2, 125 -> 3, the other 111 -> 4; jump table
  `32b87fd8`: 0 and 4 both `32b87aef` ( `mov [esi+0x8ac8], ebx`, `ebx` = 0 ); null unit or class outside 8-125 -> 0. `cmp [esi+0x4088], 0xbb` @32b87af5 ( id 187
  = `ESI_SWORD_WAIT` ), else 0xd. Slots ( struct +0x28 / +0x48 / +0x58 = Start / EventProcess / End ): START `329cb6e0` / `329cb6f0` / `329cb1b0`, WIN `329cb700` /
  `320718d0`, LOSE `329cb710` ( the thunks Ara's review resolved ). `PUSH 0x82` @32b87ed1 with `334fd4dc` "ESI_WAIT_STATE_CHANGE", EventProcess `329cb720`.
  Tail vslot 0x690, 0x684( ebx = 0 ), `31fd8f00`. June calls `31fc93d0` ( bare `RET` ) first in the wrapper and in part 2; the C++ leaves it out ( the Done
  note's "not called" means the C++ ). Order: part 2's ids are 208-333 and the event states sit in the `USI_` range, so `InitEventStates` after part 2 changes
  nothing.
- **Part 1.** Id 23: thunk `32bd5f20` -> `32adf320` ( `mov eax,[ecx]; jmp [eax+0x600]` ) -> Elesis vtable `334fa61c` +0x600 = `32b7f090` `HyperModeFrameMove`;
  `329cc5a0` -> `329cc0e0` ( `jmp [eax+0x604]` ) -> +0x604 = `3201b8e0`, base `HyperModeEnd`; StartFuture / Start / EventProcess = base `31fc8050` / `31ffd490` /
  `31fcc300` = the C++. Bounding 120-122 sit after dance 127-129 and before `ESI_RIDING_START` ( 72 ) = `InitBoundingStates( "ESI_" )`'s place; its three
  callbacks match ( EventProcess; EventProcess; StartFuture / Start / EventProcess ). Left out 123-126, 127-129, 93-96 = the comment lines. No `32d3ee60` in either part.
- **`ParseCommonRandomState`.** `CX2GUUser::ParseCommonRandomStateByUnitClass` ( `X2GUUser.cpp:6030`, `ARME_LIVE` ) reads `INIT_COMMON_RANDOM_STATE[ GetUnitClassName ]`;
  `GetUnitClassName` ( `X2UISkillTreeNew2015.cpp:5614`-`5678`, `JUNE_ENUM_NAMES` branch ) gives the seven names, equal to the seven keys of
  `ELESIS_KNIGHT.src.lua:317`-`495`.
- **JUNE_BUGS 638-639.** `PUSH 0xa0` @32b83d37, `0xa1` @32b83d68, `0xa3` @32b83de3; none of the three names occurs in `ELESIS_KNIGHT.src.lua`. 321: `mov eax,
  0x32b8fd40` @32b868e1, `32b8fd40` = `jmp 32b5a8a0`, the same thunk as 211 ( `SSK_XX` ); the C++ names `ESI_DASH_JUMP_COMBO_SSK_XX_EventProcess` on both. Both stand.

Gates. `flagcheck JUNE_CHAR_ELESIS_LIVE --range 65d53f82^..65d53f82 --files X2Lib/X2GUEL.cpp`: `files: 1 PASS` ( +1237, `eol! 424` ), `0 deleted, 0 non-ASCII
inserted, 424 EOL-mismatched, 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: FAIL` only for the `//#define`. `junediverge.py --check-all`:
`allow-listed: 207`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp`: `0 A/B row(s)`. Probes `/c /Zs /EHsc` of `X2GUEL.cpp` ( `census.cl_cmd`, no PCH ): off,
`_LIVE`, `_LIVE` + `_WOS` + `_BLOOD` + `_PASSIVE` + `_SUMMON`: exit 0, 0 errors each. No `_DEBUG` define to retire. Full build skipped: no play-test ( no code
change ). `CH1n.md` checked. Nothing to play-test. Next: `/x2-phase char-elesis CH1n` ( Opus 5.5 high ).

### CH1n - Done (2026-11-13)

**Slot head (`32b8c470`), inside, `_LIVE`.** June's head sits in March's own `SetEquippedSkillFuncBySkillSlotIndex` up to the id switch; March's head and type
switch are the `#else` (`_LIVE` still `//#define`d). Ara's CH1n is the template; read from a fresh decompile and the exe bytes:
- Bounds `0 > slot || 4 < slot`; PE tables `334f8238` / `334f8260` (normal A / B) and `334f824c` / `334f8274` (hyper A / B) = {50..58} / {60..68} / {51..59} /
  {61..69} = `_CONST_GUUSER_`. `31fd2af0` dropped (7i); the one `+0x11 = 1` store (head) = `IsSkillSlotState`; hyper word `+0x10 = 0x101` = `m_bHyperState`.
  `31fca8e0( id, 1 )` = `GetEquippedActiveSkillTemplet( id, true )`; batch `3207c1a0` / `31fdf970` = anti-tamper, as Ara's.
- **Memo arm `@32b8c5bb` (Elesis differs from Ara).** `31fdbcc0( id )` (`GetTranscendenceMemoSkillID`), `32206450( memo, 1 )` non-null -> a 31-entry switch on the
  memo from 28031: 28031 / 28060 (`RNSI_SA_SGM_JUGGERNAUT_BUSTER_TRANS` / `_TRANS_UPGRADE`) -> `32b8c5c2`, 28032 / 28061 (`RNSI_SA_SBH_BLAZING_DANCE_TRANS` /
  `_TRANS_UPGRADE`) -> `32b8c72f`; the other 27 and any memo outside the table return. Both bodies are the id switch's 28010 / 28048 and 28016 / 28052 case
  bodies (same strings, thunks and ids 257-259 / 278-281; the compiler shares their tails `@32b8d0f4` / `@32b8d355`). Written as a local `eStateSkillID`: the memo
  arm sets it to `RNSI_SA_SGM_JUGGERNAUT_BUSTER` / `RNSI_SA_SBH_BLAZING_DANCE` and skips the type switch; the id switch line takes it under `_LIVE` (`switch (
  eSkillID_ )` in `#else`). So CH3a / CH3b write each body once (kickoffs amended). Four names registered in `X2SkillTree.h` under `JUNE_ENUM_NAMES` (June
  `Enum.src.lua` l.16175 / l.15246 / l.15045 / l.14874).
- **Types** (raw `[templet +4]`, 12 entries from 5): 5 buff = `ESI_COMMON_BUFF` on both (FM `329ccd90` -> vslot 0x5f0 = her `32b6def0` `COMMON_BUFF_FrameMove`, EP
  `329cba40` -> `31fcbd60`); 6 / 7 / 13-16 active; 12 + id 900 = `ESI_THROW_ITEM` (`329ccda0` / `329ccdb0`, vslots 0x5f4 / 0x5f8, not overridden); 8-11 and 17 / 18
  return. Both switches take `param_2` raw (no `GetUpgradeBaseSkillID`), as Ara's.
- **Hyper split = CH3d** (kickoff amended with the shape): `28600 < id` -> switch (d) 28601-28605 (Ancient Fire, Fate Smasher, 28603 -> Sword of Relics), other
  ids above 28600 return; `== 28600` Sword of Relics. The active arm `break`s to the id switch, as Ara's. June's id switch has no default (exit).
- No cool-time / MP / hyper-stone block in `32b8c470` (checklist L266): the dev free-skill gate stays CH1k's (`X2GUEL.cpp` `IsDevUnlimitedSkillUse`). She has no
  second skill switch (0x68c is the base's): nothing for `InitAquiredActiveSkillState`.

**Base callbacks.** `june2017_re/char_elesis/ch1n_triage.py` (Ara's, with her range and Ara's triage added) joins every callback target outside her range in
`ch0_reg.tsv` with the earlier triages -> `ch1n_user_callback_triage.tsv`: `targets 178 rows 178 missing 0` (63 SAME; DIFF 69: 16 ported for every class, 5 kept,
48 Stage 17; 46 `n/a`: 19 shared `InitEventStates`, 14 riding / empty, 3 PvP amusement, 3 dance, 5 bounding, 1 wait-state-change, 1 vslot 0x600 thunk = her own
`HyperModeFrameMove`, CH1i). Nothing new: no `X2GUUser` site needs her `_LIVE`.

**Not ported here, routed:** hyper split (CH3d), the case bodies (their state phases: CH2a-CH3d), the memo bodies (= CH3a's 28010 and CH3b's 28016 cases).

**Checks.**
- One-file probes (`census.cl_cmd`, `/c /Zs /EHsc`, no PCH) of `X2GUEL.cpp`: off, `_LIVE`, `_LIVE` + `_WOS` + `_BLOOD` + `_PASSIVE` + `_SUMMON`: `exit 0 errors 0` each.
- `cl /P` off / `_LIVE`: `switch ( eStateSkillID )` 0 / 1, `RNSI_SA_SGM_JUGGERNAUT_BUSTER_TRANS_UPGRADE:` 0 / 1, `iSlotIndex_ > 4 )` 0 / 1.
- `git diff --patience --numstat`: `X2GUEL.cpp` 87 / 0, `X2SkillTree.h` 9 / 0; bytepatch: high bytes unchanged (13134 / 23310), EOLs kept (CRLF / LF).
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --with SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE --worktree --files X2Lib/X2GUEL.cpp`: `files: 1 PASS, 0 PASS~ (blank/marker only), 0 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR`;
  `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: FAIL` only for the `//#define`.
- `flagcheck SERV_IRUHADEV_JUNE_ENUM_NAMES --worktree --files X2Lib/X2SkillTree.h`: `files: 0 PASS, 1 PASS~ (blank/marker only), 0 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR`, `RESULT: PASS`.
- `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2SkillTree.h`: `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp`: `0 A/B row(s)`.
- Checklist marked `(Elesis: CH1n)`: L266 (no check here), L332 (memo arm), L443 (head), L447 (store dropped), L741 (types 14-16). Funcmap row `Elesis CH1n`.
  No JUNE_BUGS row. Full build skipped: no play-test.
- After the commit ( `2e92189d` ): `flagcheck JUNE_CHAR_ELESIS_LIVE --with JUNE_CHAR_ELESIS_CORE --range 2e92189d^..2e92189d --files X2Lib/X2GUEL.cpp`: `files: 1 PASS, 0 PASS~ (blank/marker only), 0 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR`,
  same zeros, `RESULT: FAIL` only for the `//#define`; `JUNE_ENUM_NAMES --range 2e92189d^..2e92189d --files X2Lib/X2SkillTree.h`: `files: 0 PASS, 1 PASS~ (blank/marker only), 0 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR`, `RESULT: PASS`.

**Nothing to play-test** (`_LIVE` is off). Next: `/x2-phase char-elesis CH1n review` (Opus 5.5 high).

### CH1n - Review (2026-11-13, Opus 5.5): pass, no code change

A fresh Ghidra decompile of `32b8c470`, read against `2e92189d` and the exe bytes (`read_va`):
- **Head.** `(int)slot < 0 || 4 < slot` -> exit (`@32b8eaa0`); PE tables `334f8238` = 50 / 52 / 54 / 56 / 58, `334f824c` = 51 ... 59, `334f8260` = 60 ... 68,
  `334f8274` = 61 ... 69 = `_CONST_GUUSER_` normal A / hyper A / normal B / hyper B, as the port indexes them. `31fd2af0` before the vslot-0 `Init()` dropped (7i);
  `+0x11 = 1` (normal) and the hyper word `+0x10 = 0x101` (`m_bHyperState` + the slot byte) = `IsSkillSlotState`; `31fca8e0( id, 1 )` null -> exit. Nothing else
  runs before the memo test: no cool-time / MP / hyper-stone check (checklist L266), the dev gate stays CH1k's `IsDevUnlimitedSkillUse`.
- **Memo arm `@32b8c5bb`.** `31fdbcc0( id )` then `32206450( memo, 1 )` (= `GetSkillTemplet`, as Ara's review); switch on the memo: `0x6d7f` / `0x6d9c` (28031 / 28060)
  and `0x6d80` / `0x6d9d` (28032 / 28061) register, any other memo falls to `@32b8ea89` (exit), never to the type switch. Each body equals its id-switch case
  line for line: `0x6d6a` / `0x6d90` (28010 / 28048; table `ESI_SA_SGM_JUGGERNAUT_BUSTER_START`, +0x18 `32b8ed10`, +0x28 `32b8ed20`, +0x48 `32b8fff0`, +0x38
  `32b906e0`, states 0x101-0x103 with tail `@32b8d0f4`) and `0x6d70` / `0x6d94` (28016 / 28052; `ESI_SI_SA_SBH_BLAZING_DANCE_START`, `32b8ed50` / `32b90700` /
  `32b8ed60` / `32b8f4e0`, states 0x116-0x119 with tail `@32b8d355`); only the EH state numbers differ. Both bodies end at the exit, as the id switch's case
  does: the `eStateSkillID` route is exact. The four names and values match the CH1n Done note's `Enum.src.lua` lines.
- **Types.** `SKILL_TYPE` 5 `ST_BUFF`; 6 / 7 / 13-16 active (13 = `ST_HYPER_ACTIVE_SKILL`, `FINALITY_SKILL_SYSTEM` defined); 12 `ST_RELATIONSHIP_SKILL` + id 900
  (`SI_ETC_WS_COMMON_LOVE` is 900 under `UPGRADE_SKILL_SYSTEM_2013`, defined) -> `@32b8ea71` copies +0x34 / +0x38 / +0x44 / +0x48 to the hyper slot and exits;
  the rest exit. Thunks: `329ccd90` -> `32a77a20` (vslot 0x5f0), `329cba40` -> `31fcbd60`, `329ccda0` -> `329cc0d0` (0x5f4), `329ccdb0` -> `329cc170` (0x5f8).
  Strings `ESI_COMMON_BUFF` / `ESI_THROW_ITEM` on both slots. Both switches take `param_2` raw.
- **Hyper split.** `0x6fb9` / `0x6fbc` (28601 / 28604) Ancient Fire, `0x6fba` / `0x6fbd` (28602 / 28605) `SI_HA_SCA_FATE_SMASHER`, `0x6fbb` and `== 0x6fb8` Sword
  of Relics `@32b8e95c`, other ids above 28600 exit: the `CH3d.md` amendment matches. `CH3a.md` / `CH3b.md` amendments match the memo bodies above.
- Nit kept: the normal-slot comment names "the one `+0x11` store"; the hyper word writes `+0x11` too. Both are `IsSkillSlotState`; no code consequence.

**Checks.**
- `ch1n_triage.py` rerun: `targets 178 rows 178 missing 0` (DIFF 69, SAME 63, n/a 46); the regenerated tsv is identical to the committed one.
- One-file probes (`census.cl_cmd`, `/c /Zs /EHsc`) of `X2GUEL.cpp`: off, `_LIVE`, `_LIVE` + `_WOS` + `_BLOOD` + `_PASSIVE` + `_SUMMON`: `exit 0 errors 0` each.
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --with SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE --range 2e92189d^..2e92189d --files X2Lib/X2GUEL.cpp`: `files: 1 PASS, 0 PASS~ (blank/marker only), 0 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR`,
  `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: FAIL` only for the `//#define`.
- `flagcheck SERV_IRUHADEV_JUNE_ENUM_NAMES --range 2e92189d^..2e92189d --files X2Lib/X2SkillTree.h`: `files: 0 PASS, 1 PASS~ (blank/marker only), 0 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR`, `RESULT: PASS`.
- `junediverge.py --check-all`: `allow-listed: 207`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp`: `0 A/B row(s)`.
- No `_DEBUG` define to retire. Full build skipped: no play-test. `CH1o.md` checked: current.

**Nothing to play-test.** Next: `/x2-phase char-elesis CH1o` (Opus 5.5 high).

### CH1o - Done (2026-11-13)

**Landed** (flag `_LIVE`, still `//#define`; `SERV_IRUHADEV_JUNE_ENUM_NAMES` for one name; 9 funcmap rows `Char Elesis CH1o`; checklist L289 `(Elesis: CH1o)`).
Files: `X2GUEL.cpp` / `X2GUEl.h`, `X2GUUser.cpp` / `.h`, `X2SkillTree.h`. Every block carries `char-elesis CH1o`. Read from the exe bytes (`charport_adis.py
elesis --phase CH1o`, `disasm_range.py`, `vtslot.py`, `dispsweep.py` with a "`mov r, [r+slot]` then `call r`" filter, an E8 / E9 scan, `enumprobe.py`).
- **0x83c `32b732e0` is March's `ChangeDamageData` (First Strike).** The base (`CX2GUUser`, empty `31f3f070`) and its call at the end of `SetDamageData`
  (`31fcd280`, every path) were already in March / ported, so the kickoff's "add the base and the call site" needs nothing. June's body went inside
  `CX2GUEL::ChangeDamageData`: the same 8 states (switch on id / 100 - 164 = `IsFirstStrikeRelateState`), gated on `GetSkillLevelValue(
  RNSI_P_SSK_FIRST_STRIKE )` > 0 (28401, `31fceab0`) instead of `UC_ELESIS_GRAND_MASTER`; stop time + `+0x9660` (x 0.5 in `ESI_JUMP_ATTACK_X`), MP x
  `+0x9668`, damage x `+0x9664` (both "> 0 else 1" = March's getters); June-only: in `XS_PVP_GAME` (`g_pMain +0x8c == 9`) the damage rate becomes ( rate -
  1 ) x 0.5 + 1. `RNSI_P_SSK_FIRST_STRIKE` joins `X2SkillTree.h`. The writer of `m_cFirstStrikeData` is CH4a's (amended: June `32b88898` reads 28401;
  March's reads 8406, which June's tree no longer has, so the bonus stays at its 1.0 / 0.0 defaults until CH4a).
- **0x534: new user virtual `IsDamageState( bool bFuture_ )`** (base `3208e8c0`, settles CH0's candidate): the now or future state is one of 15 damage
  states, read by June through getters 0x46c-0x4a4 / 0x114 / 0x118 that return June 26-31 and 34-42 = March `USI_DAMAGE_SMALL_FRONT` .. `USI_DAMAGE_REVENGE`
  without the two STANDUP states (`enumprobe`: March = June - 1 there). Only Lu/Ciel overrides those getters, so the base names the `USI_` states
  (char-luciel `CH1o.md` amended). **Elesis's override** `32b90430`: base first, then, for the now state only, her two Endurance Power active states
  (June 205 / 206). Ain (`32a50f40`) also overrides it.
- **Its engine caller, `CommonStateStart @32057d88-32057ddd`** (Ara CH1o precedent: the base comes with its call site): when the starting state is not the
  reserved skill's, a damage state drops the skill reservation (`ClearSkillUseReserve`, slot 514) unless it holds MP (`m_fReservedSkillMp` != 0) for a skill
  whose `m_eActiveSkillUseCondtion` (templet `+0x98`, confirmed through Elesis's `CheckSkillUseCondition` call `@32b84cdb`) is `ASUT_DAMAGE` /
  `_EXCEPT_AIR`. Written in `CountReservedSkillUse`'s no-match returns under `_LIVE`. The buff port's comment there ("not ported: March reserves no MP")
  predates the char ports' reserved MP; left, comments are not rewritten. The other caller, `GetActualCoolTime @32b7ef2c`, is CH4b's (amended).
- **`SetSkillLevelStateData`** = June `3201f850` inside March's class copy (Ara's form): `max( 1, GetSkillLevel( id, false, 0, true ) )`, power rate, id,
  x ( `GetSkillAdditionalPowerRate( id )` + 1 ). No Elesis code calls `3201f850` directly; she reaches it through vslot 0x7a8 `32028190` (not overridden).
- **No code:** `32b526d0` = March's `AppendMajorParticleToDeviceList` (same 7 names, null test); `32b903c0` / `32b903d0` = destructor adjustor thunks
  (`sub ecx, 0x1a0 / 0x190; jmp 32b90520`); 0x558 `GetHpTier` `32b6fb80` has **no caller** in June (no dispatch, no E8; only the vtable holds it), so it
  stays CH1h's plain `_CORE` member and no user virtual is added (corrects CH1h's lead); 0x670 `IsInJobLine` stays a plain member (every June dispatch
  of 0x670 is in class code). No CH1a-CH1n Done note or review routed anything else here, and none said "this needs Opus".
- **No `JUNE_BUGS` row:** the override's future-state gap is unreachable (both callers pass `false`).

**Gates.**
- `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --worktree --files X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h X2Lib/X2GUUser.cpp X2Lib/X2GUUser.h`: `files: 4 PASS, 0
  PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR` (+48 / +3 / +30 / +3), `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0
  #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: FAIL` only for `define: NOT FOUND` (`//#define`). `flagcheck
  SERV_IRUHADEV_JUNE_ENUM_NAMES --worktree --files X2Lib/X2SkillTree.h`: `1 PASS~` (+6), `RESULT: PASS`. `bytes>=0x80` unchanged in all five files.
- `junediverge.py --check` the five files: `allow-listed: 7`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUUser.cpp`: `0 A/B row(s)`.
- Dependency flags ON (`flagprobe.py`): `SERV_ELESIS_SECOND_CLASS_CHANGE` (the call), `_JUNE_SOCKET_PARITY`, `_USER_SKILL_USE_HELPERS`, `_SKILL_TEMPLET`,
  `_CHAR_ARME_CORE` (`ASUT_DAMAGE`), `_DIVERGE_SKILL_LEVEL_VALUE`, `_BUFF_NEW_FINALIZER`, `SERV_ADD_LUNATIC_PSYKER`.
- One-file probes (`census.cl_cmd`, `/c /Zs /EHsc`, no PCH): `X2GUEL.cpp` off / `_LIVE` / `_LIVE` + `_WOS` + `_BLOOD` + `_PASSIVE` + `_SUMMON`, `X2GUUser.cpp`
  off / `_LIVE`: exit 0, 0 errors x5. `cl /P` off / `_LIVE`: `IsDamageState` 0 / 4 in both files, the reservation test 0 / 1, the power-rate term 0 / 1.
- After the commit (`86094940`), the same flagcheck `--range 86094940^..86094940 --files` the four: `files: 4 PASS, 0 PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0
  ERROR`, 0 deleted, 0 non-ASCII, 0 EOL-mismatched, 0 `#else/#endif` without the token, 0 encoding changes; RESULT FAIL only for the `//#define`.
  `SERV_IRUHADEV_JUNE_ENUM_NAMES` on `X2SkillTree.h`: `1 PASS~`, `RESULT: PASS`.
- No `_DEBUG` define. Full build skipped: no play-test.

**Nothing to play-test** (`_LIVE` is off). Next: `/x2-phase char-elesis CH1o review` (Opus 5.5 high).

### CH1o - Review (2026-11-13, Opus 5.5): pass, no code change

Read from the exe bytes (`disasm_range.py`, raw table reads, `vtslot.py`, `dispsweep.py` filtered to "`mov r, [r+slot]` then `call r`", an E8 / E9 scan):
- **0x83c `32b732e0` = `ChangeDamageData`.** id / 100 - 0xa4, `> 0x11` out; jump table `32b734ac` = { `32b7332b`, `32b734a3` }, byte table `32b734b4` = 0 at
  164 / 169 / 173-175 / 177 / 178 / 181 = March `IsFirstStrikeRelateState`'s 8 names (March 118 ... 135 = June - 46 for all 8 and for 205 / 206 -> 159 / 160,
  `enumprobe`). `31fceab0( 0x6ef1 )` `jle` = level > 0 (`GetSkillLevelValue`, one overload under `_DIVERGE_SKILL_LEVEL_VALUE`); `+0x9660` raw, x 0.5
  (`0x3342ebc8` = 0.5) when the state is 0xae = `ESI_JUMP_ATTACK_X`, added to `+0x7c0`; `+0x9668` / `+0x9664` "> 0 else 1.0" (`test ah, 0x41`) = the two March
  getters; `+0x798` is a protected member written only when changed (no semantic difference); `g_pMain +0x8c == 9` -> ( r - 1 ) x 0.5 + 1 (`fsub st(1)`,
  `fxch`, `fmul`, `faddp`) into `+0x3d8`. `SetDamageData` `31fcd280`: four 0x83c dispatches, one before each `ret 4` = March's single call after the
  force-down block. The C++ matches.
- **0x534.** `3208e8c0`: `bFuture` != 0 -> `+0x7cb0`, else `+0x7cbc`; both paths ask the same 15 getters in the same order; user bases return 0x1a-0x1f and
  0x22-0x2a (0x47c / 0x480 = 0x20 / 0x21 skipped) = March 25-30, 33-41 = `USI_DAMAGE_SMALL_FRONT` .. `USI_DAMAGE_REVENGE` minus the two STANDUP; across all
  15 slots only `CX2GULuciel` differs from `CX2GUUser`. `32b90430`: base call, `true` -> 1; `bFuture` set -> 0; now == 0xce / 0xcd (206 / 205) -> 1. `vtslot
  0x534`: only Ain `32a50f40` and Elesis `32b90430` override. Four dispatches: `CommonStateStart @32057d8a` (push 0), Elesis `GetActualCoolTime @32b7ef2c`
  (push 0), and two the Done note did not list, both on `this` in their own class: Ain `32a50a00 @32a50af6` (pushes its argument) and Rose `32a705f0
  @32a709ad` (push 1). The Elesis override is still reached only with `false`, so "no JUNE_BUGS row" holds; funcmap `3208e8c0` row gets the two callers
  (note for the Ain and Rose ports).
- **`CommonStateStart @32057c23-32057ddd`.** Match path: `+0x8eac` == now skill (vslot 0x524), templet `31fca8e0( id, 1 )` non-null, vslot 0x810 false; it
  ends `jmp 32057ddf`, so `32057d88` is the plain else. Else: vslot 0x534( 0 ) false -> out; reserved 0 -> clear; `fld +0x8ea8; fldz; fucompp; test ah,
  0x44; jnp` -> MP == 0 clears, != 0 or NaN goes on (= `0.f != m_fReservedSkillMp`); `31fca8e0( reserved, 1 )` null -> clear; `+0x98` 6 / 7 (`ASUT_DAMAGE`
  / `_EXCEPT_AIR`, `enumprobe` 6 / 7) -> keep; else vslot 0x808 `ClearSkillUseReserve`. The C++ matches. Its guard is the complement of the buff port's
  match test, which uses `GetSkillTemplet` (June: the equipped templet) and leaves out vslot 0x810 (its comment says why); the two differ only when the
  reserved skill is unequipped inside the 1 s window: noted, not routed.
- **`SetSkillLevelStateData`** = `3201f850` (null templet / `+0x867c` return, `32890620( id, 0, 0, 1 )` floored at 1, byte `+4`, `32071720` -> `+8`, id
  `+0xc`, `( 3201ad10 + 1.0 ) x +8`) = the C++. E8 callers of `3201f850`: `3202821e` (the 0x7a8 wrapper) and other class ranges; none in Elesis's.
- **No-code verdicts hold.** `32b526d0`: `+0x84` null return, then the 7 names (`DamageImpact` / `Core` / `Slash` / `Red` / `RingRed` / `CoreRed` /
  `SlashRed`) = March's list. `32b903c0` / `32b903d0` = `sub ecx, 0x1a0 / 0x190; jmp 32b90520`. 0x558: 7 displacement sites, 0 dispatches; `32b6fb80` has no
  E8 / E9 caller and one pointer (the vtable). 0x670: 135 dispatches, all in class code (`329d4073` .. `32cf9347`), none in the engine.

**Hygiene.** `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --range 86094940^..86094940 --files` the four: `files: 4 PASS, 0 PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0
ERROR`, 0 deleted, 0 non-ASCII, 0 EOL-mismatched, 0 `#else/#endif` without the token, 0 encoding changes; RESULT FAIL only for `define: NOT FOUND`
(`//#define`). `flagcheck SERV_IRUHADEV_JUNE_ENUM_NAMES --range 86094940^..86094940 --files X2Lib/X2SkillTree.h`: `1 PASS~`, `RESULT: PASS`.
`junediverge.py --check-all`: `allow-listed: 207`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUUser.cpp`: `0 A/B row(s)`. Probes
(`census.cl_cmd`, `/c /Zs /EHsc`, no PCH): `X2GUEL.cpp` off / `_LIVE` / `_LIVE` + `_WOS` + `_BLOOD` + `_PASSIVE` + `_SUMMON`, `X2GUUser.cpp` off / `_LIVE`:
exit 0, 0 errors x5. No `_DEBUG` define. Full build skipped: no play-test.

**Nothing to play-test.** CH1p kickoff checked (unchanged). Next: `/x2-phase char-elesis CH1p` (Opus 5.5 high).

### CH1p - Done (2026-11-13)

**Her pressed index is June's 0-9 everywhere it is read; one shared reader joined, 18 held-key calls moved, no new flag.** All under
`SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (still `//#define`), March's code in `#else`, every block tagged `// char-elesis CH1p`. Read from the exe bytes
(`dispsweep.py 8ac0 8b58 88bc 8ea0 8ea4 8ea8` filtered to `32b4e770-32b90b6f`, `funcdis.py` at each hit) and the `.i` of `X2GUEL.cpp`, `_LIVE` off / on.

**7ag, the pressed-index space.**
- **Class side: June's already (CH1k).** `CanUseSpecialAttack` `32b84a50` stores the 0-9 slot in `m_iChosenSkillSlotIndex` (`+0x8ea4`, `@32b8545c`);
  `SpecialAttackEventProcess` `32b4ee20` writes `m_iNowSpecialAttack = slot + 1` (`+0x88bc`, `@32b4eec3`). March's `> 3` / `- 4` decode and `CountUsedSkill`
  sit only in the `#else` of `CanUseSpecialAttack` / `StateChangeSpecialAttack`. She has no `GetPressedSkillState` caller; `+0x8ac0` has no hit in her range.
- **Shared reader.** `CommonSpecialAttackEventProcess` (June `31fcadd0`): `UT_ELESIS` joins the June set (`X2GUUser.cpp`, a new outer `_LIVE` level over
  Ara's chain; the stand-in comment now names Add only).
- **`CurrentSkillKeyManager` (March 4 + 4) fed June's 0-9: replaced at every call.** June's `+0x8b58` is `CX2GUUser::m_kSkillSlotKey` (Eve CH1j20). The 23
  `lea ecx, [r+0x8b58]` sites in her range call only `31fce6d0` `Set`, `31fce670` `FollowSlotSwap`, `31fc89a0` `IsKeyReleased`, `31fc8900` `IsKeyPressedOnce`:
  March's `InitCurrentSkillKeyManager` / `CheckSkillSlotSwap` (swap step 4, June 5) / `IsPressedCurrentSkillKey` / `IsUnpressedCurrentSkillKey` one for one.
  Each of March's 18 calls takes June's call at the same spot: Dodge and Slash `32b72740` / `32b727f0`; Rushing Sword `32b72cc0` / `32b85470` (1st start, 1st and
  2nd event process); Extinction `32b5b870` start, `32b5bbe0` charge, `32b5c0c0` degree up (goes to state 0xdd, the down state), `32b5c400` degree down;
  Drastic Charge `32b50be0` start, `32b5fe60` loop. `Set`'s argument is `[+0x88bc] - 1` (`dec`, `@32b72756`). The rest of each body is CH2a / CH2b / CH2b2 /
  CH3a2's; kickoffs amended. `.i`: `m_sCurrentSkillKeyManager.` 18 off / 0 with `_LIVE`, `m_kSkillSlotKey.` 0 / 18.
- **Other `+0x8b58` users** are June-only reads in states March gives no key test: Judgement Drive `32b50430` / `32b50480` (CH2b), Attack Z `32b6eaa0` and
  Sonic Assault memo `32b4f800` / `32b4f840` (CH2a), Ejection Buster `32b51b90` (CH2d), Provoke `32b50b00` / `32b85aa0` (CH3a2), Rushing Sword 3rd (CH2b2);
  `32b4f660` / `32b4f6d0` are dead (CH0). Checklist 7ag gets an Elesis paragraph.
- **Guard** in `Always.h` (+9, pure insertion): `_LIVE` needs `JUNE_SKILL_SLOT_10`, `JUNE_CHAR_EVE_CORE` (`SkillSlotKey`) and `JUNE_CHAR_CHUNG_CORE`
  (`IsKeyPressedOnce`); the `_LIVE` probes prove all three are defined there.

**Hooks 7h / 7l / 7o / 7p / 7x.** 7h `InitWaitStateChange( "ESI_WAIT_STATE_CHANGE" )` (`X2GUEL.cpp:472`) and 7l `InitBoundingStates( "ESI_" )` (`:3183`) are
CH1m's. 7o `SET_LIMIT_MAX_G_SPEED` x1 (reader `X2GUUser.cpp:16745`), 7p `TIME_STOP_IMMUNE_STATE` x3 (`:18385`), 7x `AT_COMMAND` x60 / `AT_SPECIAL` x11 are
script data `ELESIS_KNIGHT.lua` brings in CH1 final; their readers are live and class-generic.

**The script keys CH0 gave this phase** (all "both", March reads them): the four `EFFECT_SET_LIST_*` variants in `LoadEffectSetFromScript`
(`JUNE_EFFECT_SET_LIST_VARIANTS`, `X2GUUser.cpp:50101-50143`, called from `CommonStateStart`; she has no override); `IGNORE_DEFENCE_DECLINE` / `IGNORE_STATE_DEFENCE`
in `SetDamageDataFromLUA` (`X2DamageManager.cpp:755` under `JUNE_BUFF_NEW_STAT`, `:766` under `UPGRADE_RAVEN`, both defined); `RELATIVE_SKILL_LEARN_CHECK` in
`ParsingBuffFactorID` (`X2GUUser.cpp:48935`) and the damage manager (`:5338`, `:5483`). No reader to write, so no `CH1p0`.

**Reserved buffs (7ai).** S3e's `ParsingBuffFactorID` (June `31fe32d0`, whole) takes the skill from `BUFF_FACTOR_RELATIVE_SKILL_ID`, and her states carry
June ids through CH1i's `SetEquippedSkillLevel`: nothing to write. June's script keys Endurance Power `_ACTIVE` (`BFI_BUFF_SUPER_ARMOR`) / `_MEMO`
(`BFI_ELESIS_ENDURANCE_MEMO`) with relative 0, Dodge and Slash / Rushing Sword x3 with 28402 `RNSI_P_SGM_AGILITY_MASTERY` + learn check, Sword Fire fly /
ground with 28210 `RNSI_A_SBH_FLAME_SWORD`; June's Flame Sword state has no table. **The `[JUNE_SKILL] SBUF` lines are gone**: skill-system Z
(`66669d39`) removed `SERV_IRUHADEV_JUNE_SKILL_DEBUG`. The CH1 kickoff is amended: CH1 adds a temporary `[JUNE_CHAR] ELESIS SKILL SBUF` line under
`_DEBUG` and tests these states plus a held B-slot key skill.

**Hygiene.** `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --worktree --files X2Lib/X2GUEL.cpp X2Lib/X2GUUser.cpp`: `X2GUEL.cpp` PASS +72, `X2GUUser.cpp`
PASS +5; `files: 2 PASS, 0 PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`, `0 deleted line(s), 0 non-ASCII inserted, 6 EOL-mismatched, 0 #else/#endif without the
flag token, 0 encoding change(s)`; RESULT FAIL only for `define: NOT FOUND` (`//#define`). The 6 EOL lines are the two trios before Extinction charge /
degree up: they take their anchor's CRLF after a studio LF-only `{`. After the commit (`9884a6be`), `--range 9884a6be^..9884a6be` with the same `--files`:
the same tallies, FAIL only for the `//#define`. `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUUser.cpp`: `allow-listed: 7`,
`RESULT: PASS`. `enumscan.py --check` the two: `0 A/B row(s)`. One-file probes (`census.cl_cmd`, `/c /Zs /EHsc`, no PCH): both files off / `_LIVE` / `_LIVE` +
`_WOS` + `_BLOOD` + `_PASSIVE` + `_SUMMON`: exit 0, 0 errors x6 (38 / 39 warnings, unchanged). No `_DEBUG` define. Full build skipped: no play-test. No
`JUNE_BUGS` row; no funcmap row (the key calls are partial; the owning phases mark the functions PORTED).

**Nothing to play-test** (`_LIVE` is off). Next: `/x2-phase char-elesis CH1p review` (Opus 5.5 high).

### CH1p - Review (2026-11-13, Opus 5.5): pass, no code change

Read from the exe bytes (`dispsweep.py 8b58` and `8ac0 88bc 8ea0 8ea4 8ea8` filtered to `32b4e770-32b90b6f`, `funcdis.py --func` at each hit) and the `.i` of both files:
- **The 18 key calls.** 29 `lea ecx, [r+0x8b58]` sites in her range (the Done note says 23: a miscount, the list is complete). The 16 under March's states call
  exactly what CH1p wrote: `Set` `31fce6d0` @32b7275e / @32b72d5c / @32b5b883 / @32b50bf1, `FollowSlotSwap` `31fce670` @32b727fe / @32b854b9 / @32b5bbeb /
  @32b5c0cb / @32b5c40b / @32b5fe6d, `IsKeyPressedOnce` `31fc8900` @32b72852 / @32b8551f, `IsKeyReleased` `31fc89a0` @32b5bc22 / @32b5c103 / @32b5c443 / @32b5fedb.
  Every `Set` takes `[+0x88bc]` then `dec` (Drastic Charge's before `CommonStateStartFuture`, as written). `32b85470` is the one EventProcess of Rushing Sword
  1st / 2nd / 3rd (`ch0_class_map.tsv:541`), so 16 sites = March's 18 calls. Extinction: charge goes to 0xdb attack / 0xdc on up / 0xdd on down; `32b5c0c0`
  goes to 0xdd (= degree up -> down, March `:11396`) and `32b5c400` to 0xdc (= degree down). `.i`: `m_sCurrentSkillKeyManager.` 18 / 0, `m_kSkillSlotKey.` 0 / 18.
- **The other sites** are June-only reads owned by later phases: `32b4f800` / `32b4f840` / `32b6eaa0` CH2a, `32b50430` / `32b50480` CH2b, `32b50b00` /
  `32b85aa0` CH3a2, `32b51b90` (Ejection Buster, `Set` only) CH2d, `32b4f660` / `32b4f6d0` dead. **Fix (bookkeeping):** CH2d's kickoff was not amended;
  this review adds its held-key note. `+0x88bc` hits = the `Set` argument reads above plus CH1k's write @32b4eec3; `+0x8ea0` / `+0x8ea4` / `+0x8ea8` only in
  `32b4ee20`, `32b84a50`, `32b73e60` (CH1k); `+0x8ac0` none.
- **Shared reader.** `.i` off: `bMarchSlotIndex` without `UT_ELESIS`; `_LIVE`: the same line plus `UT_ELESIS`; the other three uses unchanged. Eve / Arme /
  Chung / Elsword / Raven / Rena / Ara `_LIVE` are all `#define`d (`Always.h:9642`-`12193`), so the new outer level drops no class.
- **No-code verdicts hold.** The `> 3` / `- 4` decode (`X2GUEL.cpp:5265`) and `CountUsedSkill` (`:5377`) sit only in `#else`; no `GetPressedSkillState` or
  `LoadEffectSetFromScript` in her files; `JUNE_EFFECT_SET_LIST_VARIANTS` (`Always.h:10242`), `JUNE_BUFF_NEW_STAT` (`:4769`), `UPGRADE_RAVEN` (`:3242`)
  defined, readers at the cited lines. June's script: `BFI_BUFF_SUPER_ARMOR` / `BFI_ELESIS_ENDURANCE_MEMO` (`ELESIS_KNIGHT.src.lua:10381` / `:10404`),
  `BUFF_FACTOR_RELATIVE_SKILL_ID = 28402` + `RELATIVE_SKILL_LEARN_CHECK` (`:8766`, `:8840`, `:9196`), `RNSI_A_SBH_FLAME_SWORD` (`:14560`) = the CH1 amendment.

**Hygiene.** `flagcheck SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --range 9884a6be^..9884a6be --files X2Lib/X2GUEL.cpp X2Lib/X2GUUser.cpp`: `X2GUEL.cpp` PASS +72
(`eol! 6`, the studio LF-only lines CH1p named), `X2GUUser.cpp` PASS +5; `files: 2 PASS, 0 PASS~, 0 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`, 0 deleted, 0
non-ASCII, 0 `#else/#endif` without the token, 0 encoding changes; RESULT FAIL only for `define: NOT FOUND` (`//#define`). `Always.h`: 0 deleted lines.
`junediverge.py --check-all`: `allow-listed: 207`, `RESULT: PASS`. `enumscan.py --check` the two: `0 A/B row(s)`. Probes (`census.cl_cmd`, `/c /Zs /EHsc`, no
PCH): both files off / `_LIVE` / `_LIVE` + `_WOS` + `_BLOOD` + `_PASSIVE` + `_SUMMON`: exit 0, 0 errors x6 (38 / 39 warnings, unchanged). No `_DEBUG` define.
Full build skipped: no play-test.

**Nothing to play-test.** CH1q kickoff checked (unchanged). Next: `/x2-phase char-elesis CH1q` (Opus 5.5 high).

### CH1q - Done (2026-11-13)

Her Way of the Sword class side and gauge twins are June's under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS` (still `//#define`; CH1 defines it with `_LIVE`). `Always.h` gains a
guard (`_WOS` needs `JUNE_CHAR_ELSWORD_WOS`, `JUNE_ENUM_NAMES`, `JUNE_SKILL_STATE_BUFF`) and `SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS_DEBUG` (defined; its five
`[JUNE_CHAR] ELESIS WOS` lines sit inside `_WOS` code). The snapshot pair is `_CORE`. Every body below was re-read from the listing (`charport_adis.py elesis`,
`disasm_range.py`), not taken from `ch0_wos.tsv`. Funcmap rows `Char Elesis CH1q`: 16. numstat +14 / +5 / +20 / +17 / +127 / +4 / +16 / +3 / +33, 0 deleted.

| June | Site | June differs from March |
|---|---|---|
| `32b74760` / `32b7f0b0` | `Start_SwordmanVigor` / `Start_SwordmanDestruction` | job line 3 (35 / 125) takes BFI 473 / 471 (472 in PvP), else 237 / 246 (247); the five-argument call `( id, 1, NULL, 0, false )` (vslot 0x21c pushes); the PvP test stays game type 1 only, as March |
| `32b74930` / `32b74bf0` | `End_SwordmanVigor` / `End_SwordmanDestruction` | every class erases BTI 435 / 434 and plays `EffectSet_P_SDK_DARK_FORCE_J_End` / `_S_End` first (erase = `3229e160`), then March's lines |
| `32b74ed0` | `GetFinalDamage_WayOfSword` | x `m_fDestructionDamageRate` (`+0x971c`, Dark Force SA 15); active / special enter it at 1.0 x the rate |
| `32b903e0` / `32b903f0` / `32b90400` | user vslots 0x720 / 0x724 / 0x72c | new `CX2GUUser` virtuals `GetWayOfSwordPointMultiplier` / `GetVigorHpIntakeRate` / `GetDestructionBleedingDamageRate` (bases `3208e1b0` 1.0 / `3208e270` 0.0 / 1.0; `vtslot.py`: only `CX2GUEL` overrides); members `+0x9714` / `+0x9718` / `+0x971c` / `+0x9720`, ctor 1 / 0 / 1 / 1 |
| `31f86da5`, `32012f2e` | the two Elesis AttackResult stand-ins | now `fWSPGain *= GetWayOfSwordPointMultiplier()` before `ApplyWayOfSwordPointGainRate` (`X2DamageEffect.cpp`, `X2GUUser.cpp`) |
| `32b6ac80` / `32b6acb0` (new, `_CORE`) | `StoreGageSnapshot` / `RestoreGageSnapshot` | own unit keeps / restores the point (`m_fSnapshotWayOfSwordPoint`, `+0x92b8`), not the state; Crest bases unported (char-add) |
| `32a34a70` | `CX2ElesisGageData::DetonationFrameMove` | outside Destruction no `THREE_CHARGE` 300 cap: the charge runs to the now MP |
| `32a34c60` | `CX2ElesisGageData::InitWhenGameIsOver` | the base only: the state is kept after a game |
| `32a31ee0` | `CX2ElesisGageData::DamageFlushMp` | the detonation gain is stored with no `min( 0, ... )` clamp (`@32a31f30`): JUNE_BUGS 641 |
| `32e6e060` | `CX2ElesisMyGageUI::InitUI` | `InitWakeOrb` (vslot 0x54) before the dialog |
| `32e69e80` | `CX2ElesisMyGageUI::UpdateDetonationGage` | charge reads 0 with no game; outside Destruction the bar is charge / `GetMaxMp()` (gage vslot 0x10 `322b2d30`, arg 0), not / 300 |
| `32e69e70` | `CX2ElesisMyGageUI::RefreshWayOfSwordUI` (vslot 0x5c) | changed flag, then `UpdateUIElswordWSP`; CH1h's `InitComponent` call now reaches it |

**= March or already June, no code:** Get / Set state and point (`32b52790` / `32b527b0` / `32b527e0` / `32b52800` clamp), `WayOfSwordStateChange` `32b7f370`
(CH0's arm-by-arm read; CH1j's lead "CH1q replaces its body" does not hold), `GaleBuffFrameMove`, sword wait Start / StartFuture / EventProcess, `CanSpecialReactProcess`,
`SetCriticalPercent_WayOfSword` `32b4ef90`, the die StartFuture hooks `32b74f80` / `32b74fc0`, the CommonFrameMove(Future) lines, the Wait gate, `CreateGageData`
`32b7ef90` (as Elsword's), the gauge data's UpdateData / packet / Clone / Copy, the gauge UI ctor / dtor / SetShow / OnFrameMove / Play / UIFrameMove / particle /
`UpdateMpAndHyperAndDetonationGage`, `UpdateUIElswordWSP` `32e69a80` (June's `XS_GUILD_AGIT_GAME` skip has no March state, as Elsword's CH1q2). Already June: the
`InitComponent` / `ReInit` / `DamageReact` blocks (CH1h), `DetonationStateChange` + state 207 (CH1h / CH1m), `CommonRender_Prepare` (CH1j), the fatal 0.15
(CH1a, `m_uiChivalryLevel` = `+0x95d0`). `_CONST_EL_::GALE_*` = June's hard floats; no `Const_Elesis_*` key of `WayOfSword.lua` is read. Emulator: `CAC_WSP`
answered already (`SERV_NEW_CHARACTER_EL` ON; the comment at `X2OfflineStatTable.cpp:292-295` is stale, left).

**Routed:** `InitPassiveSkillState`'s Dark Force writes -> CH4a (kickoff amended); `GetActualMPConsume` / `GetActualCoolTime`'s Gale exemption (8023 + memo 608111)
and `SetDamageExceptionProcess` -> CH4b (kickoff has it); the 0x4f8 hook's `GetVigorHpIntakeRate` -> CH4c (has it); `32b89470`'s `+0x9720` -> CH2d (amended);
the 35 / 125 gauge-manager arm -> CHNa (has it); `32b704c0` / `32b705b0` / `32b75760` / `32b7e9a0` as CH0 routed. The BFI 471-473 / BTI 434-435 data rows are only
reached on 35 / 125 (CHNa).

**Bookkeeping.** Divergence: 10 new rows (the four Start / End, `GetFinalDamage_WayOfSword`, three gauge data, two gauge UI), all "ported under `..._ELESIS_WOS`";
rows 417 / 420 / 421 / 424 / 427 settled (417 / 427 no code, 420 / 421 / 424 their Elesis twin ported). `X2BuffFactor.h` (`JUNE_ENUM_NAMES`): BTI 434 / 435,
BFI 471 / 472 / 473. Checklist L261 / L281 / L850 / L958 (CH0's L844 / L952, shifted) marked `(Elesis: CH1q)`. `ch1_offsets.tsv`: `+0x9714`-`+0x9720` named,
`+0x92b8` added. Kickoffs amended: CH1 (the WOS log lines and checks), CH4a, CH2d.

**Gates.**
- flagcheck `--worktree --files` (ten files) `_WOS --with _WOS_DEBUG _LIVE _CORE`: `8 PASS, 1 OTHER, 1 ERROR` (OTHER = `X2BuffFactor.h`, `JUNE_ENUM_NAMES`; ERROR = `Always.h`,
  flagcheck cannot evaluate the neighbouring `_BLOOD && !_LIVE` guard while `_LIVE` is "with"; `Always.h` alone `--with _WOS_DEBUG`: `1 DEFINE`). RESULT FAIL only for
  the commented-out `#define`. `_WOS_DEBUG --with _WOS`: PASS. `_CORE --with _WOS` (`X2GUEl.h`, `X2GUEL.cpp`): PASS. `JUNE_ENUM_NAMES`: `1 PASS~`, PASS. Every run:
  0 deleted, 0 non-ASCII, 0 EOL-mismatched, 0 token, 0 encoding changes.
- `junediverge.py --check` (nine files): `allow-listed: 13`, `RESULT: PASS`; `--open-rows`: 187 done, 5 open (none of this phase). `enumscan.py --check` (three .cpp): `0 A/B row(s)`.
- Probes (`census.cl_cmd`, `/c /Zs /EHsc`): `X2GUEL` / `X2GUUser` / `X2DamageEffect` / `X2GageData` / `X2GageUI`.cpp, off and `_LIVE` + `_WOS`: `PROBES: 0 of 10 failed`;
  `_WOS` alone stops at `Always.h(12651)` C1189 (the probes read `Always.h`).
- Full build skipped: no play-test.

**Nothing to play-test** (`_LIVE` and `_WOS` are off). Next: `/x2-phase char-elesis CH1q review` (Opus 5.5 high).

### CH1q - Review (2026-11-13, Opus 5.5): pass, no code change

Read from the exe bytes (`charport_adis.py elesis`, `disasm_range.py`, `vtslot.py`) and the `.i` of `X2GUEL` / `X2GageData` / `X2GageUI`.cpp, `_WOS` off and `_LIVE` + `_WOS`:
- **Start / End.** `32b74760`: `GetWorld` test + `GetShowCutInAndChangeWorldColor`, colour 0.133 / 0.133 / 0.333 / 1 (= `0xff222255`) faded over 4.0, vslot 0x670( 3 )
  -> `0x1d9` (473) else `0xed` (237), pushes 0 / 0 / 1 / 0 / id = `( id, 1, NULL, 0, false )`, voice, `m_Wait` 0xbb. `32b7f0b0`: job line 3 -> `0x1d8` (472) when game
  type (vslot 0x10c) == 1 else `0x1d7` (471); otherwise `0xf7` (247) / `0xf6` (246). `32b74930`: erase `0x1b3` (435, `3229e160`), `_DARK_FORCE_J_End`, erase `0xf0`
  (240), `EffectSet_VigorEnd`, sound, `m_Wait` 0xd. `32b74bf0`: charge 0 on the gauge, erase `0x1b2` (434), `_S_End`, erase `0xf7` (247), `_DestructionEnd`. Both end
  sets are in data036 `EffectSet.lua`. Values = June's `Enum.src.lua` (BTI 434 / 435, BFI 471 / 472 / 473).
- **`GetFinalDamage_WayOfSword` `32b74ed0`.** State -2 only; type 4: game type 1 -> 1.25 else 1.5, each x `+0x971c`; types 5-6: 1.0 x `+0x971c`; three reads, as written.
- **Getters and stand-ins.** `32b903e0` / `f0` / `32b90400` = `fld [+0x9714 / +0x9718 / +0x9720]`. `vtslot.py` 0x720 / 0x724 / 0x72c: every `CX2GU*` user class has the base
  (`3208e1b0` / `3208e270` / `3208e1b0`), only `CX2GUEL` overrides. `31f86da5` / `32012f2e`: `CalcWayOfSwordPoint` -> `fmul` vslot 0x720 -> the inlined gain rate (the
  `-2` / `2` state test after it), so the multiply goes before `ApplyWayOfSwordPointGainRate`, as ported. Snapshot pair: vslot 0x198 = `IsMyUnit` (`31ff1f30`), 0x678 / 0x67c.
- **Gauge data.** `32a34a70`: outside -2, `SetMax( now MP )`, charge up, `fcomp st(1)` max < now -> max, else `fcom` 0 > now -> 0; no 300 test (Destruction arm = March).
  `32a34c60` = `SetHyperModeRemainTime( 0 )` (`31e8c9d0`) = the base only. `32a31ee0`: `fadd` + `fstp [+0x118]` @32a31f30, no clamp; our gauge setter is a raw store;
  JUNE_BUGS 641 holds.
- **Gauge UI.** `32e6e060`: `InitStatusUI` `32e66e60`, vslot 0x54, `OpenScriptFile( "DLG_Elesis_WayOfSword.lua" )`, then `g_pStatic_EL_BLUE` / `_RED` (= `InitUIElswordWSP`).
  `32e69e80`: `g_pX2Game` test in both arms (ported once before the branch, same result); -2: / 2000.0, picture 2, `EL_RED_2`; else / gauge vslot 0x10( 0 ) (`322b2d30`
  = `GetMaxMp`, funcmap 1582), picture 1, `MP_ACTIVE`. `32e69e70`: `[gage+0x110] = 1`, jmp `32e69a80`.
- **"= March".** `WayOfSwordStateChange` `32b7f370` re-read arm by arm: thresholds `+0x60` (>=) / `+0x64` (<=) / > 0 / < 0 / == 0, remote `+0x8160`, the five transitions
  with March's End / Start calls and guards. `SetWayOfSwordPoint` `32b52800` (clamp to both thresholds) and `SetCriticalPercent_WayOfSword` `32b4ef90` (types 4-6 -> 1.0) = March.
  `IsInJobLine`'s body is under `_CORE` (`X2GUEL.cpp:6647`), so `_WOS` links.
- **`.i`.** Off: March in all three files (`SetWayOfSwordState( 0 )` in `InitWhenGameIsOver`, `min( 0, ... )`, `/ 300`). `_LIVE` + `_WOS`: the end sets, the two `+0x971c`
  multiplies, 4 `ELESIS WOS` lines, `RefreshWayOfSwordUI`, `InitWakeOrb` before the dialog, the unclamped store; Elesis's 300 cap and `/ 300` gone.

**Hygiene.** `flagcheck _WOS --range 6f9608ea^..6f9608ea` (nine files, `--with _WOS_DEBUG _CORE`): `8 PASS, 0 PASS~, 0 DEFINE, 1 OTHER, 0 FAIL, 0 ERROR` (OTHER =
`X2BuffFactor.h`, `JUNE_ENUM_NAMES`), 0 deleted, 0 non-ASCII, 0 EOL-mismatched, 0 token, 0 encoding; RESULT FAIL only for the `//#define`. `Always.h`: +20, 0 deleted.
`_CORE`: PASS (`Always.h:12637`). `_WOS_DEBUG`: PASS (`Always.h:12672`, kept defined for CH1). `JUNE_ENUM_NAMES` (`X2BuffFactor.h`): `PASS~ +14`, PASS.
`junediverge.py --check-all`: `allow-listed: 207`, `RESULT: PASS`. `enumscan.py --check` the three `.cpp`: `0 A/B row(s)` (D 1 / X 1 outside CH1q's lines). Probes
(`census.cl_cmd`, `/c /Zs /EHsc`): the five `.cpp` off / `_LIVE` + `_WOS` / + `_BLOOD` + `_PASSIVE` + `_SUMMON`: 0 errors x15. Full build skipped: no play-test.

**Nothing to play-test.** CH1 kickoff checked (its CH1q amendment stands; deps CH1q review and CH0b both done). Next: `/x2-phase char-elesis CH1` (Opus 5.5 high).

### CH1 - Done (2026-11-13)

Opus 5.5. **The deploy unit is live.** Defined in `Always.h`: `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (:12624), `_WOS` (:12649); `_WOS_DEBUG` (:12672, CH1q's)
and the new `SERV_IRUHADEV_JUNE_CHAR_ELESIS_DEBUG` (:12823) for the play-test. Elesis (8 / 29 / 30 / 120 / 121) now runs June's registration, state bodies,
virtuals, skill head, pressed-slot space and her Way of the Sword class side and gauge twins (CH1a-CH1q). Every `#error` guard holds (probes below).
- **`[JUNE_CHAR] ELESIS DATA`** (`_LIVE`, end of June's `InitState`, `X2GUEL.cpp:493`; include `:6-8`), Ara's line: class, script, motion, state count,
  `wait_state_change`. Not June; stays after the review.
- **`[JUNE_CHAR] ELESIS SKILL SBUF`** (`_ELESIS_DEBUG`, `X2GUUser.cpp`, the CH1p amendment): `parse` (state, `m_iStateBuffSkillID`, level, list size; once a second
  per state), `cleared` (learn check), `at once` (time-0 rows inside the parse), `frame` (`CommonFrameMove_InsertBuffFactor`), `state end`
  (`ApplyAllStateInsertBuffFactor`); `UT_ELESIS` only, helpers in `_CONST_GUUSER_EL_CH1_`. Unported characters cannot cast skills (user, 2026-11-13), so CH1
  cannot show them: the checks move to CH2a / CH2b / CH2c / CH3b (kickoffs amended) and the CH3d review retires the flag.
- **Script:** `luasrc.py adopt-june data036 ELESIS_KNIGHT.lua` (dry run first): June's file replaces March's whole as `ljconv` bytecode, 245,689 B sha1 `4c3b68b4`,
  no core-list row. Record `june2017_re/june_adopt_20261113_083005.tsv`, backup `kom_backup_stage_june\20261113_083005`.
- **Enum gate:** `luabind.py check ELESIS_KNIGHT` first failed on CH0's 4 `ENUM MISSING NAME`; data036 `Enum.lua` gains June's `RNSI_A_SCA_BLOOD_STING` 28224,
  `RNSI_A_SGM_UPPER_SLASH` 28222, `RNSI_P_SBH_BLAZE_ABSOLU` 28418, `RNSI_SA_SCA_CHASING_SWORD` 28043 (`Enum.src.lua:14308 / 16063 / 15734 / 15564`, one
  `-- char-elesis CH1` comment each), `luasrc.py refresh data036 Enum.lua`. Rerun: 0 methods, 0 unbound, `ENUM 0 table(s) missing, 0 member(s) missing, 3 nil in June
  too` (`CT_NORMAL_LOOK_DIRECTE`, `CT_NORMAL_LOOK_TRACKING`, `CT_PARTS_LOOK_TRACKINGT`), `RESULT: PASS`. `core --check` / `ledger --check`: PASS, 0 FAIL, 0 WARN.
  No C++ member added: no C++ reads the four names yet (CH3a2 / CH3b2 / CH3c / CH4 own them).
- **Held motions** (`ch1_held_motions.tsv`): `kom.py stage-june`: June's `MOTION_EL_GM.x` (3,400,722 B) / `Motion_EL_BH.x` (3,083,072 B) replace March's in data033
  (backup `kom_backup_stage_june\20261113_083036`). The five names each that March's script played go with March's script.
- **Packed / copied (no ask):** `PACK OK data036` (~2: the script, `Enum.lua`), `PACK OK data033` (~2: the motions);
  `COPY OK data036 ... 14088772 bytes, 270 entries, md5 5a83c1d0d4d03483090ee888a65a8ec6`, `COPY OK data033 ... 187855678 bytes, 165 entries, md5 883d4aac3ce8c73ea315a51521244185`.
- **Divergence gate:** with `_LIVE` + `_WOS` on, `junediverge` saw four `GT_PVP` tests (rule b): `ReInit` x2 (June body and `_WOS`'s March `#else`),
  `Start_SwordmanDestruction`, `GetFinalDamage_WayOfSword`. June `32b81900` @32b819a8 (`dec eax / jne`), `32b7f0b0` @32b7f28f / @32b7f312 and `32b74ed0` @32b74f14
  (`cmp eax, 1`) call vslot 0x10c and test type 1 only: three allow-list rows with those addresses.
- **Reference log:** `data\ErrorLog_pre_CH1_elesis.txt` (254,291 B, the run of 2026-11-13 08:29).
- **Scans:** `charport_keys.py ELESIS_KNIGHT`: `june-only=9 neither=1 march-only-site=4 both=234` = CH0's `plan_charport_keys.txt` (only the reader label of
  `FORCE_DYNAMIC_CAMERA_INDEX` / `RETAIN_CAMERA_STATE` now reads `CX2GUEL::ReservedSkillStart`).`charport_scan.py`: identical to `plan_charport_scan_20261111.txt` (line endings aside; `ELESIS_KNIGHT` `no NUMERICAL_DATA_INDEX` 69 / `no DAMAGE table` 20). Nothing new.
- **Play-test save** (`elsdb x`, backup taken): slots A 0-3 / B 5-8, 4 / 9 empty. wewe (20): Spiral Blast, Dodge and Slash, Wild Shock, Endurance Power (28201
  learned at 1 now); SaberT (40): + Rushing Sword, B Sonic Assault, Unlimited Blade, Mega Burster, Extinction; eresisu (21): four upgrades, B Sword Fire, Infernal
  Blade, Big Burst, Avatar of the Fire; ElesisGM (29): four upgrades, B Extinction, Drastic Charge, Upper Slash, Victorious Sword; ElesisBH (30): four upgrades,
  B Sword Fire, Flame Sword, Infernal Blade, Flame Protection.
- **Skills** are not castable until their class phase ports them (user, 2026-11-13): no skill check in this play-test; the two-cast, held-key and `SBUF`
  checks are CH2a-CH3b's. The play-test slots stay set for them.
- **Checklist:** `(Elesis: CH1 ...)` at l.16, 116, 123, 150, 212, 426, 427, 428, 536, 600, 625, 682, 883. CH0b's 21 `ErrorEnum(433)` part-mesh lines
  (l.773) are re-checked on this play-test. `MODS.md`: "Stage 7 Elesis, CH1".

Hygiene: `flagcheck.py JUNE_CHAR_ELESIS_DEBUG --worktree --files X2Lib/X2GUUser.cpp KTDXLIB/Always.h`: `1 PASS, 0 PASS~, 0 DEFINE, 1 OTHER, 0 FAIL, 0 ERROR`; 0 deleted,
0 non-ASCII, 0 EOL, 0 token, 0 encoding; `RESULT: PASS`. `JUNE_CHAR_ELESIS_LIVE --with _WOS _WOS_DEBUG --files X2Lib/X2GUEL.cpp KTDXLIB/Always.h`: `X2GUEL.cpp` PASS +7;
`Always.h` ERROR only (guard `:12682` `_BLOOD && !_LIVE` cannot be evaluated with `_LIVE` stripped, as Ara's CH1); 2 deleted = the two `//#define` lines.
`_WOS --files KTDXLIB/Always.h`: `1 OTHER, 0 FAIL`, `RESULT: PASS`. `junediverge.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h X2Lib/X2GUUser.cpp X2Lib/X2GUUser.h
KTDXLIB/Always.h`: `allow-listed: 17`, `RESULT: PASS`; `--check-all`: `allow-listed: 217`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp`: `0 A/B row(s)`.
Probes (`census.cl_cmd`, `/c /Zs /EHsc`, all on): `X2GUEL.cpp`, `X2GUUser.cpp`, `X2GageData.cpp`, `X2GageUI.cpp`, `X2GUElsword_SwordMan.cpp`, `X2DamageManager.cpp`,
`X2Game.cpp` exit 0, 0 errors.
After the commit: `flagcheck --range 9b48ec80^..9b48ec80`: `JUNE_CHAR_ELESIS_DEBUG --files X2Lib/X2GUUser.cpp KTDXLIB/Always.h` `1 PASS, 1 OTHER, 0 FAIL, 0 ERROR`,
`RESULT: PASS`; `JUNE_CHAR_ELESIS_LIVE --files X2Lib/X2GUEL.cpp` `1 PASS, 0 FAIL`, all zeros, `RESULT: PASS`.
Build (`--pch force`): `DEPLOY OK: X2_offline.exe is 26,382,848 B, dated 2026-11-13 08:45:47, md5 6379174fdf3a0f9e33f20624412ae84a, equal to X2/US_SERVICE/x2.exe (2026-11-13 08:45:39); built at main fc391e7e + 3 uncommitted code file(s).`
`EXESTR OK: 3 of 3 found in X2_offline.exe, 26382848 B, dated 2026-11-13 08:45:47, md5 6379174fdf3a0f9e33f20624412ae84a` (`ELESIS DATA`, `ELESIS SKILL SBUF parse`, `ELESIS WOS vigor start`, wide).

**Play-test (user) is the gate:** the five units above, none transcended. Check: `ELESIS DATA` per class, combos, dash, jump, damage / stand-up, riding,
the Way of the Sword both ways across a stage change and a dungeon end, ErrorLog against the reference. No skills (not castable yet); no riding (no Elesis unit owns a
mount: CH6's walk).
The review is `kickoffs/char-elesis/CH1-review.md`.

**Play-test (2026-11-13, run 08:48-08:54 on md5 6379174f), first part.** The user played all five units in battle fields (5 `EGS_JOIN_BATTLE_FIELD_REQ`,
no dungeon), so the stage-change and dungeon-end steps were not reached; a follow-up dungeon run is asked for in the hand-off.
- `offline_server.log`: `ELESIS DATA` for all five classes, each `script ELESIS_KNIGHT.lua motion MOTION_EL.X wait_state_change 1`; states 8: 137, 29: 167,
  30: 157, 120: 173, 121: 163 (the review checks them against the registrations).
- `WOS`: `attack gain` both ways (type 1 negative, type 4 positive, `rate x1.00`: no Dark Force), `vigor start` x2 (30 at 4800, 120 at 4800), `destruction start`
  x1 (121 at -4800), each `dark knight line 0`; in vigor the point drains with `mp rate x1.50`. No `vigor end` / `destruction end`: each mode was still on
  when the user switched character. Point carried across loads (wewe opened at -2924, GM at 4566): June's kept gauge (CH1q).
- `ErrorLog.txt` against `ErrorLog_pre_CH1_elesis.txt` (that run loaded no Elesis): `KEM_ERROR` 0 / 0, `Motion Error` 0 / 0, `lua error` 3 / 3 (TutorSystem,
  `ChainQuestInfo` `AddGroupChainQuest`, both pre-existing), `[LUA_SRC]` 7 / 7, `ELESIS_KNIGHT` / `StateID` / `Parsing Error` 0. New Elesis shapes are first loads:
  one-time `OpenXET` probes (`MESH_EL_SSK / SGM / SPK / SBH` hair and cash parts, `MESH_EL_ED_*`), event-scene `.swf` / `.gfx` probes (`PC_ELESIS_PNG`,
  `QUEST_N_ELESIS_END`). **CH0b's routed `ErrorEnum(433)`** `Anim Frame : 79, Mesh Frame : 80` x20 on the six `Mesh_EL_Basic_*_Sorted.X` parts: those parts
  are June's bytes (`stage-june --dry-run`: 6 identical), so June's own data: `JUNE_BUGS.md` 642 (Ara's 603 twin), checklist l.775 marked.
  `offline_packets.log`: `EXCEPTION` 0, `UNHANDLED` only `E_DISABLE_HB_CHECK_REQ`. No missing animation, no popup.
- **Closed (user, 2026-11-13):** the dungeon run (mode end, stage change, dungeon end) is waived: "i trust your code for this one, it'll be tested anyways
  in future playtest". CH1 is done; the CH1 review retires `_WOS_DEBUG` without asking for that step.

### CH1 - Review (2026-11-13): pass; `_WOS_DEBUG` retired

Opus 5.5. **Verdict: passes.** The only code change retires `SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS_DEBUG` (`Always.h` `//#define`, one `// char-elesis CH1 review: retired`
line); `SERV_IRUHADEV_JUNE_CHAR_ELESIS_DEBUG` stays for CH2a-CH3b (its stale "The CH1 review retires it" comment now names the CH3d review). `ELESIS DATA` stays.
- **Deploy unit:** data036 `ELESIS_KNIGHT.lua` 245,689 B sha1 `4c3b68b4...` header `1B 4B 4C 81` = the `june_adopt_20261113_083005.tsv` row (`bytecode ljconv`); no
  `.src.lua` sidecar. `stage-june --dry-run ch1_held_motions.tsv`: `2 staged (0 new, 0 replaced, 2 identical)`. `kom.py status data036 data033`: both `in sync`, `+0 / -0 / ~0`
  vs live (data036's `[stale .src.lua: compile]` is older non-Elesis sidecars, as in Ara's review). `luabind.py check ELESIS_KNIGHT`: `ENUM 0 table(s) missing, 0 member(s)
  missing, 3 nil in June too`, `RESULT: PASS`. `core --check` / `ledger --check`: PASS, 0 FAIL, 0 WARN. The 4 `-- char-elesis CH1` `Enum.lua` lines (live :12308 / :12315 /
  :12368 / :12814) = `Enum.src.lua:14308 / 16063 / 15734 / 15564`, names and values. `EXESTR OK: 2 of 2` (`ELESIS DATA`, `ELESIS WOS vigor start`), md5 `6379174f`.
- **State counts, exact for all 5 units** (one load each, logged at the end of June's `InitState`, `X2GUEL.cpp:493`): base 134 = 97 `InitStateCommon` (its `_LIVE` body;
  June's riding / PvP amusement / dance left out as Ara's) + 3 `InitBoundingStates( "ESI_" )` + 3 START / WIN / LOSE + 1 `USI_WAIT_STATE_CHANGE` + 30 user states (9
  `USI_EVENT_*` + 20 slot + key 0, Ara's term); class path `InitStateByUnitClass` (29 / 120: 19, 30 / 121: 15, 8: 0); learned-skill sub-states (`SetEquippedSkillLevel`'s
  22 blocks, `m_StateList[ id ]` on each learned base or upgrade; `ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE` is already in the common body). 8 (unit 20, Dodge 3 + Endurance 0)
  134+0+3 = 137; 29 (40: Sonic 3, Dodge 3, Rushing 2, Extinction 4, Judgement 1, Critical Dive 1) 134+19+14 = 167; 30 (21: Sonic 3, Swordfire 2, Dodge 3) 134+15+8 = 157;
  120 (29: + War Prelude 3, Provoke 1, Drastic Charge 2 on 29's 14) 134+19+20 = 173; 121 (30: Sonic 3, Swordfire 2, Dodge 3, Blazing Dance 4, Flame Sword 1, Scarlet Rose 1)
  134+15+14 = 163. All = the log.
- **Way of the Sword:** 43 lines: 40 `attack gain`, 2 `vigor start` (30, 120 at 4800), 1 `destruction start` (121 at -4800). In vigor the opposite type drains the point with
  `mp rate x1.50` (30, type 1 -133) and the same type gains 0.00 at mp x1.00 (120, type 4): `CalcWayOfSwordPoint` / `CalcVigorMpUpOnAttack_Elesis`, as designed. No mode
  end, stage change or dungeon end (waived by the user, 2026-11-13); they ride on later play-tests without the debug lines.
- **Skills:** not cast (unported characters cannot cast skills); the two-cast, held-key and `SBUF` checks stay in `CH2a.md` / `CH2b.md` / `CH2c.md` / `CH3b.md`.
- **ErrorLog** (current / `_pre_CH1_elesis`; last write 08:53:59, no later run): `KEM_ERROR` 0 / 0, `Motion Error` 0 / 0, `lua error` 3 / 3, `[LUA_SRC]` 8 / 8,
  `ELESIS_KNIGHT` / `StateID` / `Parsing Error` 0 / 0. `ErrorEnum(433)` 20 / 0, all `Anim Frame : 79, Mesh Frame : 80` on the six `MESH_EL_BASIC_*_SORTED.X` parts at
  the first load (face 4, hair 4, foot / hand / lowbody / upbody 3 each) = `JUNE_BUGS.md` 642 (CH0b counted 21).
- **Divergence allow-list:** `charport_adis.py elesis 32b81900`: @32b819a8 `mov eax, [edx + 0x10c]` on the `g_pX2Game` handle, `call eax`, `dec eax` / `jne`: game type 1
  only, as the row says. The other two rows (`32b7f0b0`, `32b74ed0`, `cmp eax, 1`) are the same idiom per the CH1 Done note.
- **Scan:** `charport_scan.py` rerun to completion: 57 lines = `plan_charport_scan_20261111.txt`, 0 differences (no Elesis row moved). `charport_keys.py ELESIS_KNIGHT`:
  `june-only=9 neither=1 march-only-site=4 both=234` = CH1's.
- **Kickoffs:** `CH5.md` amended (cut-in table now `X2GUEL.cpp:212-232`, read at `:5083-5102`; June's index switch `:439-453` already gives 125 index 3, so June's fourth
  set must land in CH5 before CHNa makes 125 loadable; March's `#else` `:499-522`; `X2StateCreateUnit.cpp` chart `:3901-3913`, handlers `:790-819`, `SelectUpperClass`
  `:3421`, moved by peers). `CH2a.md` amended (checklist power-rate row now l.259). `X2UnitViewerUI.cpp:630` / `:2414` / `:2530` still the sites.

Hygiene: `flagcheck --range 9b48ec80^..9b48ec80`: `JUNE_CHAR_ELESIS_LIVE --files X2Lib/X2GUEL.cpp` `1 PASS (+7), 0 FAIL`, all zeros, `RESULT: PASS`; `JUNE_CHAR_ELESIS_DEBUG
--files X2Lib/X2GUUser.cpp KTDXLIB/Always.h` `1 PASS, 1 OTHER, 0 FAIL, 0 ERROR`, `RESULT: PASS`; `JUNE_CHAR_ELESIS_WOS --files KTDXLIB/Always.h` `1 OTHER, 0 FAIL`,
`RESULT: PASS`; `JUNE_CHAR_ELESIS_LIVE --files KTDXLIB/Always.h`: `ERROR` only (guard `:12650` `_WOS && !_LIVE` with `_LIVE` stripped, as Ara's CH1), 2 deleted = the two
`//#define` lines. The retire: `flagcheck JUNE_CHAR_ELESIS_WOS_DEBUG --worktree --files KTDXLIB/Always.h`: `1 DEFINE, 0 FAIL`; RESULT FAIL only on "not #defined anywhere"
(the define is commented out, as every retire); 2 deleted = the old `#define` and the `_ELESIS_DEBUG` comment line, 0 non-ASCII, 0 EOL. `junediverge.py --check-all`: `allow-listed: 217`,
`RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`: `0 A/B row(s)`. Probe (`census.cl_cmd`, `/c /Zs /EHsc`, `_WOS_DEBUG` off): `X2GUEL.cpp`,
`X2GUUser.cpp` exit 0, 0 errors (their `X2OfflineLog.h` include stays under `_LIVE` / `_ELESIS_DEBUG`).
Build (started not awaited, finished before the commit): `DEPLOY OK: X2_offline.exe is 26,381,824 B, dated 2026-11-13 09:18:04, md5 f77c835db6e6d99c82053c9c09802c81,
equal to X2/US_SERVICE/x2.exe (2026-11-13 09:17:55); built at main a805e326 + 3 uncommitted code file(s).` `EXESTR OK: 1 of 1 absent` (`ELESIS WOS vigor start`, wide), md5
`f77c835d`. CH5 and CH2a are ready.

## Phase CH5 - UI and data: UnitTemplet rows, cut-ins, the creation screen

**Run with:** Sonnet 5.5 high, Opus review, user play-test. Flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_UI`. Sub-tag `UI`.

**Scope:** her 5 `UnitTemplet.lua` rows become June's and June's rows for 35 and 125 are added (BOM source, `luasrc.py refresh` after; every other
row stays; if CH0 found the templet loader refuses a class >= `UC_END`, the 125 row waits for CHNa): `m_fLobbyRotationY`, `m_HyperCutIn` for
120 / 121 / 125, the names; the cut-in table and the InitState index switch (`X2GUEL.cpp:249-273`) replaced by June's names and layout (7t
cut-in `.gfx` names; March's two missing `.dds` names go); 7ah; the lobby particle hard-codes (`X2UnitViewerUI.cpp:630` / `:2414` / `:2530`)
retired where June's `AnimationNameTable` row plays the same particles (Ara's decision 2 is the precedent: under the unit viewer's flag, March's
code in `#else`). **Creation screen (user, 2026-11-10 and 2026-11-11: a char-port item, not Stage 9).** Adopt June's
`DLG_Unit_Create_ClassChange_Button_Elesis.lua` (six buttons; `adopt-june`, `luabind.py check`) and port every June function the creation state
and unit viewer run for Elesis (class-preview switch, class buttons, `Movie_Create_Elesis_*` previews, the stat chart `X2StateCreateUnit.cpp:3870-3885`),
listed from June first: **Grand Master and Blazing Heart get their entries here**. A click on a third-line button before CHNa must be harmless
(check `SelectUpperClass`; if it is not, the third-line buttons are hidden under `_UI` until CHNa).

**Done gate:** a select-screen pass of all nine March characters with no new error; her skill and hyper cut-ins play June's art; the second-job
names show June's strings; on the creation screen Elesis shows Saber Knight, Grand Master, Pyro Knight and Blazing Heart, each with its preview.

### CH5 - Done (2026-11-13)

Sonnet 5.5. Flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_UI` (`Always.h` end; `#error` without `_ELESIS_LIVE`). Review kickoff `kickoffs/char-elesis/CH5-review.md` written.
- **`UnitTemplet.lua` (data036, core-list BOM source):** her 5 blocks (8 / 29 / 30 / 120 / 121) are June's text; June's Dark Knight 35 (after 30) and Crimson Avenger 125 (after 121) are added (57 -> 59 blocks, the other 52 untouched, BOM + CRLF kept).
  Delta = `ch0_unittemplet_elesis.tsv`: `m_fLobbyRotationY` 30 / 25 / 1 / 15 / 1, `m_HyperCutIn` `HQ_Cutin_EGM / EBH_Transcendence` on 120 / 121, 35 (24 keys) and 125 (26 keys, `HQ_Cutin_ECA_Transcendence`) new. A 35 / 125 unit now has a templet (CH0's NULL `GetType()` crash is closed before CHNa).
  `luasrc.py refresh` REFRESHED, `ledger --check` PASS, `core --check` PASS, `luabind.py check UnitTemplet` PASS.
- **Cut-in table (`X2GUEL.cpp`):** `s_SkillCutInSet` = June `333a8450` (table `337ad5c0`, 4 sets x 3, GFx names): set 0 `HQ_CutIn_Elesis_01`, 1 `_GM_01`, 2 `_BH_01`, 3 `_CA_01`; March's `.dds` table in the `#else`. The InitState switch already gives 125 set 3 (CH1m), so only the table was missing.
  New logs `[JUNE_CHAR] ELESIS UI cut-in set <set> name <gfx>` (reader) and `ELESIS UI hyper cut-in class <c> name <gfx>` (`ShowHyperSkillCutInByScript`). The 7 `.gfx` (4 skill, 3 `HQ_Cutin_E*_Transcendence`) are in data152 / data195.
- **Creation screen:** `DLG_Unit_Create_ClassChange_Button_Elesis.lua` = June's 5284 B bytecode (`adopt-june data034`, record `june2017_re/june_adopt_20261113_093248.tsv`, `luabind.py check` 22 methods, 0 unbound, PASS): six buttons, so Grand Master and Blazing Heart now have entries.
  `character_info_08.tga` = June's (`stage-june`, data057; TET identical), backup `kom_backup_stage_june/20261113_093252`. June's creation functions read in Ghidra = March for 8 / 29 / 30 / 120 / 121, no C++: `SelectElesis` `32554de0` (same call order), stand mesh `32551e30` (type 8 `Dummy_Village_Board8` + Elsword aura),
  `GetUnitStatInfomationChart` `3254ee40` (8 / 29 / 120 = 2,1,2; 30 / 121 = 2,2,2 = March; 35 / 125 = 2,1,1 -> CHNa). Third-line buttons (35 / 125): `GetThirdLineElesisClassTypeByUnitClassType` is assert-only and returns `UC_NONE`, `SelectUpperClass` returns early on it: a click is harmless, no hiding needed (as Ara's).
  The four previews' movies are loose in `data\movie` (`base` / `gm` / `bh` / `ca`).
- **Lobby (7ah n/a):** `X2UnitViewerUI.cpp:644` BH particles read handles created only in the dead `#else` (`:2560`+): inert, not edited. The `...Readyaction` literals (`:2162`, `:5277-5280`) are dead `#else`; the live `AnimationNameTable.lua` has June's `SPK_LobbyReadyAction` (checklist l.372 closed).
- **Packed and copied (no ask):** `COPY OK data036` md5 `4b2273a8463ee645449979985ac8b013` (1 changed = `UnitTemplet.lua`), `COPY OK data034` md5 `5c43ab1e5a668b1bcd682ef2840ad435` (1 changed), `COPY OK data057` md5 `773dd907a1fce4410ebaa68832939832` (1 changed); backups in `kom_backup_tools/*.20261113_0937*`.
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_UI --worktree --files KTDXLIB/Always.h X2Lib/X2GUEL.cpp X2Lib/X2GUUser.cpp`: 2 PASS, 0 PASS~, 0 FAIL, 0 ERROR; 0 deleted, 0 non-ASCII, 1 EOL (the `#endif` before `CreateGUEL`, in an LF region), 0 token-less, RESULT: PASS.
  `junediverge --check X2Lib/X2GUEL.cpp X2Lib/X2GUUser.cpp`: allow-listed 17, RESULT: PASS. `enumscan --check X2Lib/X2GUEL.cpp`: 0 A/B rows.
- **Deploy:** `DEPLOY OK: X2_offline.exe is 26,382,848 B, dated 2026-11-13 09:48:43, md5 c7d1abf8e11546b8726474685ca97a2c`. `EXESTR OK: 6 of 6` (the two log tags, `HQ_CutIn_Elesis_01 / _GM_01 / _BH_01 / _CA_01`, wide), `EXESTR OK: 4 of 4 absent` (March's `HQ_CutIn_Elesis_SGM.dds` / `_SBH.dds` / `_Pyro_Knight.dds` / `_Saber_Knight.dds`).
- **Not testable now (user, 2026-11-13: unported classes cannot cast):** the skill and hyper cut-ins. Owed: CH2a (set 0), CH3a (set 1), CH6 (all four sets, hyper on 120 / 121 / 125); kickoffs amended. Skill-icon gate n/a (no skills registered).
- **Routed:** CHNa: 35 / 125 stat chart (2,1,1), third-line helper, `GetCharacterImageName` portraits, third-line texture pieces and `Movie_Create_Elesis_CA` on the screen (kickoff amended). Stage 12: cut-in item override `31fd8a50`.
- **Checklist:** `(Elesis: CH5 ...)` at l.372, 603, 631, 888. `funcmap.tsv` +4 rows. `MODS.md`: "Stage 7 Elesis, CH5".

**Play-test hand-off:** see the session reply. Log tag `[JUNE_CHAR]` (`ELESIS DATA`; no `ELESIS UI` line is expected, no skill is castable).

**Play-test (user, 2026-11-13 09:51-09:53, exe `c7d1abf8`; logs read).** Select screen: all nine characters loaded (user's word; the log has only the `MESH_PC_ELESIS_STAND` XET probe). Creation screen: six Elesis class clicks logged `MOVIE_CREATE_ELESIS_BASE` / `_BH` / `_GM` / `_BH` / `_GM` / `_BH` (the name probe, `ErrorEnum(136)` + `(72)` as Raven's `RAVEN_OUT.LUA`); no missing control. June's `EffectSet_SBH_LobbyWait` row played 3 times (3 particles each). `KEM_ERROR` 0, `Motion Error` 0, `ErrorEnum(135)` / `(36)` 0, `EXCEPTION` 0, `UNHANDLED` only `E_DISABLE_HB_CHECK_REQ`. Not reached: a village entry (no `ELESIS DATA`), skill cut-ins (owed to CH2a / CH3a / CH6).

### CH5 - Review (2026-11-13, Opus 5.5): PASS, no code change

- **Cut-in table `333a8450` (Ghidra, disassembly):** 12 `SkillCutIn` written at `337ad5c0 + i * 0x30` (`337ad5c0` .. `337ad7d0`), strings pushed in order `334fe8dc` `HQ_CutIn_Elesis_01` x3 (`333a848c` / `8528` / `85c7`), `334fe8b0` `_GM_01` x3, `334fe884` `_BH_01` x3, `334fe858` `_CA_01` x3 (`333a89f9` / `8a96` / `8b35`)
  = the port's 4 x 3 `s_SkillCutInSet` exactly. Size / position floats `334367ac` / `33435598` on every entry = March's one-argument `SkillCutIn( name )` defaults (as Ara's `333a8bf0`). Set index (`X2GUEL.cpp:450-464`, CH1m): 120 -> 1, 121 -> 2, 125 -> 3, else 0; table has 4 rows, no overrun.
  The log line sits after the 0x8b3 override, so it names the drawn cut-in. `CASH_21` / `_22` (`334fe828` / `7f8`) are the 0x8b3 event table, not this one.
- **UnitTemplet (pre = `data036.kom.20261113_093754`, current = kom_contents = the deployed data036's copy):** 57 -> 59 blocks, no duplicate keys; the 7 `UC_ELESIS_*` blocks byte-equal June's `UnitTemplet.src.lua` (7 of 7, order 8 / 29 / 30 / 35 then 120 / 121 / 125);
  the other 52 byte-equal pre-phase, order kept. Semantic key delta = `ch0_unittemplet_elesis.tsv` (173 rows: 116 same, 57 june-only, 0 mismatch). BOM, 1610 CRLF, 0 bare LF. `ledger --check` PASS, `core --check` PASS, `luabind.py check UnitTemplet` PASS (ENUM 0 / 0).
- **Creation screen:** data034 `DLG_Unit_Create_ClassChange_Button_Elesis.lua` = `1B 4B 4C 81`, 5284 B, sha1 `b1f78258..a219` = the adopt record; `luabind.py check` 22 methods, 0 unbound, PASS. `GetUnitStatInfomationChart` `3254ee40`: 8 / 0x1d / 0x78 = 2,1,2; 0x1e / 0x79 = 2,2,2
  = March's `X2StateCreateUnit.cpp:3901-3914` (MIDDLE, LOW, MIDDLE / MIDDLE x3); 0x23 / 0x7d = 2,1,1 (CHNa's). Stand mesh `32551e30`: `Dummy_Village_Board8` + `EffectSet_Elsword_Stand_Aura`. `SelectElesis` `32554de0`: `LobbyDownLanding`, `325514e0( 8 )`, `ElesisVoice_Introduce.ogg` = March.
- **Lobby (`cl /P` on the real `X2UnitViewerUI.cpp`):** `JUNE_UNIT_VIEWER_EFFECT_PLAYER`, `JUNE_ANIMATION_NAME_TABLE`, `_ELESIS_UI`, `_ELESIS_LIVE`, `SERV_ELESIS_SECOND_CLASS_CHANGE` all ON. `m_hSeqHandFireSBH1-3` (read at 664-768) are created only at 2560 / 2579 / 2598, in the dead `#else`
  of `JUNE_UNIT_VIEWER_EFFECT_PLAYER`; the `.i` has no `CreateSequenceHandle` for them, so the BH hand block stays inert. `SPK_LobbyReadyaction` occurs 0 times in the `.i` (2162 / 5280 are dead `#else`); live `AnimationNameTable.lua` (June bytecode) has `SPK_LobbyReadyAction` once.
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_UI --range a693fb15^..a693fb15`: 2 PASS, 0 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR; 0 deleted, 0 non-ASCII, 1 EOL (the noted LF-region `#endif`), 0 token-less, 0 encoding, RESULT: PASS; no later commit touches the flag.
  `junediverge --check-all`: allow-listed 225, RESULT: PASS. `enumscan --check X2Lib/X2GUEL.cpp`: 0 A/B rows. Deployed exe (`2763c264`, a peer's later build that includes CH5): `EXESTR OK: 4 of 4` (the four GFx names, wide), `EXESTR OK: 4 of 4 absent` (March's four `.dds`). No `_UI_DEBUG` define to retire.
- **Logs (CH5 play-test, 09:51-09:53, exe `c7d1abf8`):** `KEM_ERROR`, `Motion Error`, `EXCEPTION`, `CUTIN` 0; `[JUNE_GFX]` 0 (no skill castable). `ErrorEnum(136)` `MOVIE_CREATE_ELESIS_*.LUA` x6 = the subtitle-name probe (movie plays on), `ErrorEnum(431)` `MESH_PC_ELESIS_STAND.XET` = the engine's optional-XET probe: not CH5's.
- **Kickoffs:** `CH6.md` l.48 already walks sets 0-3 by class and the hyper cut-ins on 120 / 121 / 125; l.25 the creation screen. `CHNa.md` already carries CH5's routed items. CHNa is ready (char-ara CHNa review done 2026-11-11).
- Full build skipped: no play-test (no code change).

## Phase CH-N - Elesis's June-only class path: Dark Knight (35) and Crimson Avenger (125)

The template block is `JUNE_CHARACTER_AISHA_PHASES.md` "Phase CH-N" as Ara's Z rewrites it; Ara ran it first. **What Elesis reuses** (Ara's CHNa,
flag `SERV_IRUHADEV_JUNE_CLASS_PATH`): the socket-loader bound, `GetClassLevel`, the optional log sites, the June `FieldQuest.lua`, the quest
reward path that accepts any class. **What is hers** (flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_CLASS`): everything in `ch0_classn.tsv` marked
`Elesis`. Sub-tag `CLASS`.

| Sub-phase | Scope |
|---|---|
| CHNa | **The class loads and plays** (`ch0_classn.tsv`): **the gauge create first** (`X2GageManager.cpp:194-209` gets the 35 / 125 arms: without them her class casts a plain gauge to `CX2ElesisGageData`); `GetX2UnitClassCompatibility` arms (`X2Unit.h:2057-2123`, June `31ed6500`, divergence row 365); the third-line arms of `GetFirstLine...` / `GetSecondLine...ClassTypeByUnitClassType` and their callers (`X2Unit.h:2685-2750`); the class switches inside `X2GUEL.cpp` (`:213`, `:249`, `:411`, `:2747`, `:9581`); `X2Data.cpp:2872-2892`, `X2ComboTree.cpp:302`; `tools/createchar.py` (`CLASSES` / `PARENT`); **stat rows** for 35 / 120 / 121 / 125 per Open decision 1 (server Lua `KncWX2Server/ServerResource/US/StatTable.lua`: the user packs it; until then the loader's type fallback, now logged); `Motion_EL_DK.x` / `MOTION_EL_CA.x` reach the unit; **the creation screen's third line** (Dark Knight, Crimson Avenger: helper arms, stat-chart arms, previews, `Movie_Create_Elesis_CA`); the units: `createchar.py` a 35 (level 20), a 125 (level 40) and a level-80 125. Play-test: both load in the village and a dungeon, combos and the Way of the Sword gauge work, the job name shows, a socketed weapon keeps its socket, the creation screen shows all six entries. |
| CHNb | **The job-change route and the class-change dialogs** (`ch0_classn.tsv`): June's quests 35569-35572 (to 35) and 35573-35577 (to 125) with their rewards through `Handlers_Quest.cpp`; **`LevelUpEvent.lua`'s level-15 row opens `DLG_1ST_Class_Change_Guide_Elesis.lua` (three paths) and its level-35 row adds 35 and opens `DLG_2ND_Class_Change_Guide_Elesis.lua`** (core-list rule if it must be edited); `DLG_Class_Change_Elesis_DK` / `_CA` and June's copies of `_SK` / `_PK` / `_GM` / `_BH` (staged by CH0b); `X2OfflineClassChangeSeed.h` parent rows only if June's cash route needs them (no Elesis row today means "do not refuse"); `luabind.py check` on each dialog. Play-test: a level-15 Knight sees the three-path dialog, takes 35569-35572 (`/qdone`) and becomes Dark Knight; a level-35 Dark Knight sees the second dialog, takes 35573-35577 and becomes Crimson Avenger; a second level-15 Knight still reaches Saber Knight and Pyro Knight from the same dialog; all survive a relog. |

**Done gate:** a 35 and a 125 reached both by job change and by `createchar.py` load, fight and relog; the gauge, socket and stat paths log no
refusal; no other class changed (a select-screen pass of all nine March characters).

### CHNa - Done (2026-11-13)

Opus 5.5. Flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_CLASS` (+ `_CLASS_DEBUG`, the review retires it), `Always.h` end, `#error` without `SERV_IRUHADEV_JUNE_ENUM_NAMES`.
Reused unchanged from Ara's CHNa: `SERV_IRUHADEV_JUNE_CLASS_PATH` (socket-group bound 202, `GetClassLevel` = June: 35 = 1, 125 = 2). Review kickoff `kickoffs/char-elesis/CHNa-review.md`.
- **Gauge (crash fix, first edit):** `X2GageManager.cpp` Elesis arm gains 35 / 125 (June `3248e290` case list 8 / 0x1d / 0x1e / 0x23 / 0x78 / 0x79 / 0x7d, read in Ghidra this
  session), so `CX2GUEL::CreateGageData`'s static_cast to `CX2ElesisGageData` no longer reads a plain `CX2GageData`. Debug line `[JUNE_CHAR] ELESIS CLASS gauge class <c> -> CX2ElesisGageData`.
- **Items:** `GetX2UnitClassCompatibility` 35 -> {8, 35}, 125 -> {8, 35, 125} (June `31ed6500` cases 0x23 / 0x7d, re-read this session; a 125 does not wear 29 / 120 gear).
- **Creation screen:** `GetThirdLineElesisClassTypeByUnitClassType` = June `3234b390` table `337a1660` (8 / 35 / 125; same values as Ara CHNb's `GetUnitClassByTierAndLine`
  Elesis row), March's all-`UC_NONE` body in `#else`; new divergence row. Stat chart 35 / 125 = (MIDDLE, LOW, LOW) = June `3254ee40` 2,1,1 (re-read). CH5 added no hide
  lines; June's six-button dialog, `character_info_08` (`stage-june --dry-run`: identical to June) and `movie_create_elesis_ca.avi` (loose in `data\movie`) were already in.
- **Portraits:** index 57 / 58 appended to all nine March arrays + the switch, from June `31ee0de0` rows 0x3b / 0x3c (`char_image_rows.py 35:0x3b 125:0x3c`; its no-argument run
  reproduces March's Little Devil / Yama Raja rows): `EL_SDK_*` / `EL_SCA_*` (FACE, LARGE, GRAY, MEDIUM, ICON, PARTY) in `DLG_UI_Common_Texture85_NEW`, `CLASS_D` / `CLASS_C`
  (Texture11), `list_ELESIS_D_KNIGHT` / `list_ELESIS_C_AVENGER` (GUI_character_01; `_GRAY` pieces in the `_gray` sheet), `HQ_Dungeon_Clear_Elesis_Dark_Knight / _Crimson_Avenger.tga`.
  Next free index for Add: 59.
- **Portrait assets:** `stage-june --dry-run` on 21 portrait / creation names: 18 identical to June, `Movie_Create_Elesis_CA.avi` not in a June kom (loose file), and
  `DLG_UI_Common_Texture85_NEW` `.tga` + `.TET` new (March had no Texture85). Staged into data057 (`STAGE-JUNE OK`, 2 new); `PACK OK data057`, `COPY OK data057` md5
  `a86401b58beb9afd05ded2219bf11727` (backup `kom_backup_tools/data057.kom.20261113_101652`). The June TET carries all 12 `EL_SDK_*` / `EL_SCA_*` pieces the rows name.
- **Tool:** `createchar.py` CLASSES + PARENT gain 35 (-> 8) / 125 (-> 35). Units (backup `db_backup/els_db_20261113_101653_pre_pre_elesis_chna.sql`): 144 `DKnight20` (35, L20),
  145 `CAvenger40` (125, L40), 146 `CAvenger80` (125, L80); each wears its promotion costume and has class-8 gear at +10 in the equip bag (Void Claymore + Henir set).
- **Moot / checked, not needed:** stat rows (the flat StatTable already has 35 / 120 / 121 / 125: classn row 21; GM / BH numbers unchanged, no `MODS.md` before / after);
  `X2ComboTree.cpp:302` (June loads no combo tree); `RotateYSelectCharacterType` (default 10; the templet reader is Lu/Ciel NU4, as Ara); the `X2GUEL.cpp` class switches
  `:213` / `:249` / `:2747` are CH1m's (35 / 125 arms already in), `:411` is CH4a, `:9581` is CH1o. `Motion_EL_DK.x` / `MOTION_EL_CA.x` are in the March koms and named by
  the CH5 `UnitTemplet.lua` rows. A skill-level socket on a 35 / 125 weapon is not observable without such an item: the bound is proven by code (Ara CHNa).
- **Routed:** transcended 35 / 125 portrait pair (five-argument overload) -> CH3d; job change + class-change dialogs -> CHNb; 35 / 125 skills -> CH2d / CH3c.
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_CLASS --with _CLASS_DEBUG --worktree`: `files: 4 PASS, 0 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`; 0 deleted, 0 non-ASCII,
  0 EOL-mismatched, 0 untokened, 0 encoding changes; `RESULT: PASS`. `junediverge --check` (5 files): allow-listed 9, `RESULT: PASS`. `enumscan --check`: 0 rows on the changed lines
  (the 20 file rows are pre-existing, e.g. `X2Data.cpp:3106-3108`).
- **Deploy:** `DEPLOY OK: X2_offline.exe is 26,412,544 B, dated 2026-11-13 10:27:45, md5 9666b48ab8ebf0ed3c9b5e1997e8c400` (built at main 8800dcdc; it also carries a peer's
  uncommitted `KTDGGFXManager.*`). `EXESTR OK: 6 of 6` (wide: the `ELESIS CLASS gauge class` tag, `EL_SDK_FACE`, `EL_SCA_PARTY`, `list_ELESIS_C_AVENGER`,
  `HQ_Dungeon_Clear_Elesis_Crimson_Avenger.tga`, `CLASS_D`).
- **Play-test hand-off:** see the session reply. Log tag `[JUNE_CHAR]` (`ELESIS CLASS`).
- **Play-test (user, 2026-11-13 10:51-10:57, exe `b4108783` = a peer's redeploy of `4750f9b9`, CHNa code in): all checks passed** (creation screen six entries, DKnight20 /
  Crimson Avenger village + dungeon, gauge, gear, portraits, other classes). Logs: `ELESIS CLASS gauge class 35` x2, `125` x1; `ELESIS DATA class 35 ... states 164`, `class 125 ...
  states 169` (CH1m's SDK / SCA registrations reached; `ESI_SDK_WIN` / `ESI_SCA_START` random-state rolls); packets EXCEPTION 0. Not CHNa's: 20 KEM 433 on
  `MESH_EL_SDK_*` / `MESH_EL_BASIC_FACE` (the CH0b part-mesh frame mismatch, routed to CH1 final), `MESH_EL_SCA_BASIC_FACE.TGA.TGA` (misspelt name in June's data), the usual
  `OpenXET` optional probes, 4 `E_DISABLE_HB_CHECK_REQ`, `AddGroupChainQuest`, `Add_Skill_Page_ED`.

### CHNa - Review (2026-11-13)

Opus 5.5 high. **Verdict: pass, no code change.** Commit `4750f9b9` re-read against June's exe (Ghidra read-only, decompiles this session):
- **Gauge** `X2GageManager.cpp`: June `3248e290` case 8 / 0x1d / 0x1e / 0x23 / 0x78 / 0x79 / 0x7d -> `new` 0x11c (`FUN_32498960`) + 0x80 (`FUN_32494900`) = the Elesis arm
  with the two new labels. Exact.
- **Compatibility** `X2Unit.h`: June `31ed6500` case 0x23 -> item class 8 or 0x23; case 0x7d -> 8, 0x23 or 0x7d. = the two new arms (no 29 / 120 for 125). Exact.
- **Third line** `GetThirdLineElesisClassTypeByUnitClassType`: June `3234b390` reads `337a1660 + (tier + type * 4) * 0x14 + line * 4`; `3234afb0` fills Elesis's block
  (type 8, base `337a18e0`): line 3 = `337a18ec` 8, `337a1900` 0x23, `337a1914` 0x7d. = the function and Ara CHNb's `GetUnitClassByTierAndLine` Elesis row (`X2Unit.h:2974`).
  Divergence row present (`june_march_divergent.tsv`, last row).
- **Chart** `GetUnitStatInfomationChart`: June `3254ee40` case 0x23 / 0x7d -> [0] 2, [1] 1, [2] 1; the Knight arm (case 8 -> 2, 1, 2 = March's MID, LOW, MID) fixes the field
  order, so 35 / 125 = (MIDDLE, LOW, LOW). Exact.
- **Portraits** `X2Data.cpp`: `char_image_rows.py 35:0x3b 125:0x3c` rerun: all 18 base entries (nine tables x two classes) = the rows at index 57 / 58, and the switch maps
  35 -> 57, 125 -> 58. The Elesis blocks sit after Ara's `#endif` in every array, insert-only (flagcheck 0 deleted), so indices 0..56 are what Ara's review proved; the play-test
  showed the right pictures at 57 / 58. Transcended pieces in June's rows (Texture100-102, `TR_EL_SDK_*` / `TR_C_AVENGER_*`; June leaves 35's `160` and `MyGage` TR piece
  empty) stay CH3d's.
- **Tool** `createchar.py`: CLASSES 35 / 125, PARENT 35 -> 8, 125 -> 35. Units 144-146 played.
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_CLASS --with _CLASS_DEBUG --range 4750f9b9^..4750f9b9`: `files: 4 PASS, 0 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`; 0 deleted,
  0 non-ASCII, 0 EOL-mismatched, 0 untokened, 0 encoding changes; `RESULT: PASS`. `junediverge --check-all`: allow-listed 225, `RESULT: PASS`. `enumscan --check` (4 files):
  20 A rows, all pre-existing enum definitions / `X2Data.cpp:3106-3108`, 0 on CHNa's lines.
- **Logs** (`logs.py --grep "ELESIS CLASS"`): `gauge class 35` x2, `125` x1 (10:52-10:55), ErrorLog 0. Matches the play-test.
- **Debug retired:** `//#define SERV_IRUHADEV_JUNE_CHAR_ELESIS_CLASS_DEBUG` (bytepatch; flagcheck `--worktree` on `Always.h`: 1 DEFINE, the one deleted line is the define;
  `RESULT: PASS`). Build not awaited (2026-11-03 rule). CHNb's kickoff amended: it turns the define back on for its own lines and its review retires it.
- Not here (unchanged routing): `RotateYSelectCharacterType` templet reader (Lu/Ciel NU4), transcended 35 / 125 portraits (CH3d), job change + dialogs (CHNb), skills (CH2d / CH3c).

### CHNb - Done (2026-11-13)

Opus 5.5. **Data only: no client source changed, no flag added** (the route and the guide runtime are Ara's class-generic CHNa / CHNb / CHNb2 code).
`_CLASS_DEBUG` stays commented out: CHNb has no lines of its own; the always-on `CLASS GUIDE` and `QUEST complete ... CLASS CHANGE` lines prove the play-test.
- **Quest route (no change):** the shipped data036 `FieldQuest.lua` is June's: 35569-35572 (class 8, level 15, `CJT_JOB_THIRD`, 35572 -> `UC_ELESIS_DARK_KNIGHT` + 109999 + 293720)
  and 35573-35577 (class 35, level 35, 35577 -> `UC_ELESIS_CRIMSON_AVENGER` + 110850 + 293721). `Handlers_Quest.cpp:586-625` grants any class (CH0 row).
  Saber / Pyro routes for the side checks: 35320-35323 (-> 29), 35330-35333 (-> 30).
- **Open check from CH0 settled by code read:** the guide's Accept (`GuideAcceptClassUpQuest`, `GetAvailableQuest( NUI_ECHO )`) finds the Lenpad / Ariel quests,
  because `GetStartQuestList` (June `3232fbd0` @`3232fbee`, `SERV_IRUHADEV_JUNE_QUEST_RUNTIME`) lists a class-up quest with a start NPC at every NPC.
- **Level-up guides (data036 `LevelUpEvent.lua`, core-list source):** level-15 `TYPE6` opens `DLG_1ST_Class_Change_Guide_Elesis.lua` (was March's `Class_Change_Guide_Elesis.lua`);
  level-35 `TYPE6` lists 29 / 30 / 35 and gains `DIALOG_LIST` = `DLG_2ND_Class_Change_Guide_Elesis.lua` (had none) = June `LevelUpEvent.src.lua:103-118, :320-337`.
  BOM + CRLF kept (455 lines); `luasrc.py refresh`; `ledger --check` / `core --check` PASS. The level-70 row stays CH3d's.
- **Dialogs (no new staging):** CH0b's data034 `DLG_1ST_` / `DLG_2ND_Class_Change_Guide_Elesis.lua`, data034 `DLG_Class_Change_Elesis_SK` / `_PK` / `_GM` / `_BH` and data217
  `_DK` / `_CA` are June's files as `ljconv` bytecode (same size as June's, differ only at byte 4, the 0x82 -> 0x81 header). `GetClassChangeDialogName` already names `_DK` / `_CA`.
  `luabind.py check` on the eight: 0 unbound, ENUM 0 / 0, `RESULT: PASS`.
- **Data:** `kom.py pack data036` + `copy`: PACK OK / COPY OK (md5 `07b3744a`, `~1 changed LevelUpEvent.lua`; backup `kom_backup_tools/data036.kom.20261113_110737`).
- **Units** (backup `db_backup/els_db_20261113_110903_pre_char-elesis-CHNb.sql`; account `unit_slots` 130 -> 133): `ElGuideDK` 147, `ElGuideSK` 148, `ElGuidePK` 149
  (class 8, level 14, exp 81600 = 100 below 15) and `ElGuideCA` 150 (class 35, level 34, exp 1212400 = 100 below 35).
- **Hygiene:** no client source changed, so `flagcheck` / `junediverge --check` / `enumscan` have nothing to scan.
- **Deploy:** full build skipped: no code change. The deployed exe (md5 `b4108783`, 2026-11-13 10:43, includes CHNa) already carries the lines:
  `EXESTR OK: 3 of 3 found in X2_offline.exe, 26419200 B, dated 2026-11-13 10:43:12, md5 b4108783ec14ed437b94a3dacbe30d9c`.
- **Play-test (2026-11-13 11:12-11:16, user: all passed):** `CLASS GUIDE 1st: 3 class description(s) wrapped to 180` x3 (147 / 148 / 149);
  `CLASS GUIDE 2nd: class 35 line 3 -> next class 125, buttons 1543 / 1547` + `2nd skills: next class 125, description 36261, 4 skill icon(s), 3 command row(s)` (150);
  Accept took `35569` / `35573` (`QUEST accepted`); `CLASS CHANGE` 147 8 -> 35, 148 8 -> 29, 149 8 -> 30, 150 35 -> 125; every `/qdone` NetError 0; save after relog:
  147 = 35, 148 = 29, 149 = 30, 150 = 125, 140-146 unchanged; 0 EXCEPTION, ErrorLog 0. `AddGroupChainQuest` unbound x1 is pre-existing (backlog line 314).
  Review kickoff `kickoffs/char-elesis/CHNb-review.md` written.

### CHNb - Review (2026-11-13)

Opus 5.5 high. **Verdict: pass, no change to data or code.** Commits `f3befad7` / `e3867379` re-checked against June's data (no Ghidra: no log line disagreed).
- **`LevelUpEvent.lua`** (data036 core-list source, 8734 B, BOM, 455 CRLF / 0 lone LF): level-15 `TYPE6` (:103-119) and level-35 `TYPE6` (:269-286) = June
  `LevelUpEvent.src.lua:103-118` / `:320-337` line for line (class list, dialog, both `ElesisVoice_ClassUpReady0x.ogg`). Diff against the pre-CHNb kom
  (`kom_backup_tools/data036.kom.20261113_110737`, extracted to the scratchpad): exactly three hunks, :112 dialog name, :274-275 `UC_ELESIS_DARK_KNIGHT` added,
  :279-281 `DIALOG_LIST` added; nothing else changed. `luasrc.py ledger --check`: `LEDGER CHECK PASS: 0 FAIL, 0 WARN`; `core --check`: `CORE CHECK PASS: 0 FAIL, 0 WARN`.
- **Dialogs:** all eight are byte-identical to `luasrc.reference_of( June, "june" )` = `ljconv(June bytecode)`: data034 `DLG_1ST_` / `DLG_2ND_Class_Change_Guide_Elesis`
  (8482 / 7025 B, from June data034), data034 `DLG_Class_Change_Elesis_SK` / `_PK` / `_GM` / `_BH` and data217 `_DK` / `_CA` (1392 B, from June data217).
  (Correction to the CHNb note: against June's raw 0x82 file they differ beyond byte 4, at the remapped opcodes; against `ljconv` they are identical.)
  `luabind.py check` on the eight: 21 methods each, 0 unbound, ENUM 0 / 0, `RESULT: PASS`. March's old `Class_Change_Guide_Elesis.lua` stays in data034, now unreferenced.
- **Play-test logs** (run 11:12:39-11:16:31, after the `b4108783` deploy): `CLASS GUIDE 1st: 3 class description(s) wrapped to 180` x3; `CLASS GUIDE 2nd: class 35 line 3 ->
  next class 125, buttons 1543 / 1547` + `2nd skills: ... 4 skill icon(s), 3 command row(s)`; `QUEST accepted 35569 'Dark Knight 1/4' (from NPC 10025)`, `35320`, `35330`,
  `35573 'Crimson Avenger 1/5' (from NPC 10025)` (the guide's Accept); `QUEST complete 35572 CLASS CHANGE unitUID=147 8 -> 35`, `35323 ... 148 8 -> 29`, `35333 ... 149 8 -> 30`,
  `35577 ... 150 35 -> 125`: exactly 4 `CLASS CHANGE` lines, so no other unit changed. Packets 0 EXCEPTION (5 UNHANDLED = `E_DISABLE_HB_CHECK_REQ`, pre-existing), ErrorLog 0 matches.
- **Save** (`elsdb.py chars`): 147 = 35, 148 = 29, 149 = 30, 150 = 125; 140-146 unchanged (119 / 124 / 9 / 4 / 35 / 125 / 125).
- **Deployed kom:** `kom.py status data036`: packed vs live `~0 changed` (CHNb's copy is live). It also reports `PACK NEEDED: Enum.lua newer than packed (11:18:54)`:
  a peer's edit after CHNb's 11:07 pack, not CHNb's; left to its owner.
- **Kickoffs:** CH2d / CH3c name no quest ids (they need a 35 / 125 unit; 144-147 / 145-146 / 150 exist). CH6's walk said "CHNb's quests": amended to name them
  (35569-35572 -> 35, 35320-35323 -> 29, 35330-35333 -> 30, 35573-35577 -> 125, start NPC 10025, Accept from the guide).
- **Hygiene:** no client source changed, so no flagcheck range. `junediverge --check-all`: allow-listed 225, `RESULT: PASS`. No debug define to retire
  (`_CHAR_ELESIS_CLASS_DEBUG` stayed commented out). Full build skipped: no code change.

## Phase CH2 - Skills: Knight and the first jobs (classes 8, 29, 30, 35)

**Run with:** Sonnet 5.5 high (transcription from CH0's map), one Opus review at CH2d (covers CH2a-CH2d), user play-test per phase. Flag
`SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL`. Sub-tags `SKILL`, `NUM`. A skill's line decides its phase: class 8 has it -> CH2a; else the first of 29 / 30 / 35.

| Sub-phase | Scope |
|---|---|
| CH2a | Knight (8) skills (15 rows; Endurance Power `32b80c60` and its memo, Sonic Assault's memo states) |
| CH2b | Saber Knight (29) skills |
| CH2c | Pyro Knight (30) skills |
| CH2d | Dark Knight (35) skills (after CHNb: tested on a 35 unit; the bare `SI_*_SDK_*` script tables); CH2 stage review |

For each id: its case in `SetEquippedSkillFuncBySkillSlotIndex` with June's state functions whole; its block in `SetEquippedSkillLevel` `32b4e790`
if it has one; its attached rows (7u names, 7ai BFI arguments, 7h UPDOWN cancels, the power-rate operator, the numerical-only tables of
`plan_numerical_rows.tsv`); its effect closure (a name found only now is staged and added to `ch0_assets.tsv`); its `WAY_OF_SWORD_TYPE` on CH1q's
code. Upgrade ids reach their base through the templet's `m_iDeleteSkillIDByUpgrade` (June `32071720`): check the templet swap for every term that
reads the id. A skill whose factor names one of the three BFI ids June's `Enum.lua` lacks (`JUNE_BUGS` 279) keeps June's nil, with the row cited.
`NUM use` lines for her skills; March vs June damage in `MODS.md`.

**Done gate:** every class-8 / 29 / 30 / 35 id in the S3 census has a state (except CH0's passive / CH4 ids); each skill in an A and a B slot casts
June's state, motion and effect, deals damage, shows its cool time and spends MP; no missing particle / mesh / texture line.

### CH2a - Done (2026-11-13)

Sonnet 5.5. Flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL` (`Always.h`, `#error` without `_LIVE` and `JUNE_ENUM_NAMES`) + temporary `_SKILL_DEBUG` (`[JUNE_CHAR] ELESIS SKILL equip` lines; the CH2d review retires it, it stays on). No review of its own.
All 28 class-map rows read from `charport_adis.py elesis --phase CH2a` listings: **22 functions written** (10 inside March's function, 12 new), **6 = March** (4 Init lists compared by script, `32b72740` slot StartFuture = CH1p's, `32b72c30` = `SECOND_StartFuture`). `funcmap.tsv` rows `Char Elesis CH2a` (28).
- **Registration:** June base and upgrade ids added as labels to March's cases (Spiral Blast 28000 / 28026, Wild Shock 28001 / 28027, Mega Burster 28002 / 28028, Sonic Assault: Sting 28003 / 28029, Unlimited Blade 28004 / 28030, Dodge and Slash 28200 / 28217; 28201 / 28218 return as June). Wild Shock gains FrameMove. Sonic Assault registers the memo states 318-320 (ready, charge, fire). Dodge and Slash: slot states gain Start + EndFuture; June's states 189 / 311 (back) / 190 (Z-X attack) replace March's `_SECOND` registration under the flag. New names in `X2SkillTree.h` (`JUNE_ENUM_NAMES`): the 8 Knight ids, `RNSI_A_EL_DODGE_AND_SLASH_UPGRADE` 28217, `RNSI_P_SGM_WALTZ_OF_SWORD` 28403.
- **What June differs in:** the 5 anim-end EventProcess (`32b58b70` family, one body at 5 addresses) = March's with walk / back-walk / dash / Z-X cancels needing the foot on a line; Mega Burster 38 preloads (March 21), Sonic 1 more; Dodge and Slash FrameMove = `CommonFrameMove` only (the evasion buff is a script key: `[ELESIS SKILL SBUF]` line); EventProcess: second press re-enters 189, or 311 (back) with the enhanced skill ( templet `m_iDeleteSkillIDByUpgrade` ) and the key opposite to facing, Z / X attack once, End clears both counters; Attack-Z start multiplies physic damage by Waltz of Sword's `ATK_REL` in the destruction state; Wild Shock FrameMove plays its damage effect sets at 0.55 s (memo pair in the destruction state); Sonic memo: charge drains MP 100 / s, charge to 0.5 s (cap plays `EffectSet_SA_EL_SONIC_ASSAULT_CHARGE`), held 2 s or released fires; fire = speed burst (walk speed x 2.75 x (1 + charge)) and damage x (1 + 2 x charge). KProtectedType members ported plain (Raven precedent): attack count `+0x95b4`, `+0x9654`, `+0x9778`, hold time `+0x96f0`.
- **Not here, routed:** Dodge and Slash's max re-use count and Waltz of Sword's ability (`+0x9654`) are written by June's passive init `32b88300` (block @32b884dd): **CH4a**. Until then the second press does not chain (re-use count 0), so the back step (311) is unreachable; first cast and Z / X attack work. `GetActualMPConsume` / cool-time `SI_A_EL_DODGE_AND_SLASH` cases (Victorious Sword) = CH4b. No `JUNE_BUGS` row. `NUM use`: numerical tables are June script data run by the engine; `MODS.md` "Stage 7 Elesis, CH2a".
- **Checklist** `(Elesis: CH2a ...)`: APPLIED_SKILL_ID / NUMERICAL_DATA_INDEX rows, the missing-table-members row (10 RNSI names), the class-switch default row (12 ids bound).
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_SKILL --with _SKILL_DEBUG --worktree --files ...`: `2 PASS ( +382 / +20 ), 2 OTHER ( Always.h = other flags, X2SkillTree.h = JUNE_ENUM_NAMES ), 0 FAIL`, 0 deleted, 0 non-ASCII, 0 EOL-mismatched, 0 untokened, 0 encoding change; `RESULT: PASS`. `junediverge --check` the 3 files: `allow-listed: 10`, `RESULT: PASS`. `enumscan --check X2GUEL.cpp`: 0 A/B rows. `luabind check ELESIS_KNIGHT`: 0 unbound, ENUM 0 missing, `RESULT: PASS`. `skillicons check` 28000-28004 / 28026-28030 / 28200-28201 / 28217-28218: `skills 14, misses 0`, `RESULT: PASS`. `/Zs` probe of `X2GUEL.cpp`: 0 errors.
- **Deploy:** `DEPLOY OK: X2_offline.exe is 26,430,464 B, dated 2026-11-13 11:51:19, md5 62a1a6211612ef8d77162e5087f6fc12, equal to X2/US_SERVICE/x2.exe (2026-11-13 11:51:11); built at main 5b50392e + 34 uncommitted code file(s).` The first run failed in a peer's uncommitted `X2CashShop.cpp` (june-dungeons C3z-4t3, `CKTDGGFXManager` header), waited, then COALESCED onto another session's successful full build that carried my edits. `EXESTR OK: 3 of 3 found in X2_offline.exe, 26430464 B, md5 62a1a6211` (wide `ELESIS SKILL equip skill`, `EffectSet_SA_EL_SONIC_ASSAULT_CHARGE`, `EffectSet_SA_EL_WILD_SHOCK_DAMAGE_Sub_MEMO`).
- **Play-test finding and fix (user, 2026-11-13): after Sonic Assault: Sting ( memo ) the player could not move and could not be hit.** Cause: the memo fire state ( 320 ) was registered with no `OnEventProcess`; its script is `XAP_ONE_WAIT` + `INVINCIBLE`, so it never ended. June `32b8ca71` copies the skill state's EventProcess ( `[esi+0x44/0x48]`, the anim-end -> WAIT chain `32b58b70` ) into it; the CH0 callback table showed constants only. Fix: `stateData.OnEventProcess = normalStateData.OnEventProcess` on state 320 ( `X2GUEL.cpp`, tag `char-elesis CH2a` ). The other registrations ( 189 / 190 / 311, 318 / 319 ) were re-read in the listing: constants only, no copy. **New standing gate `tools/stateends.py check <class file> <June script>`** ( `RESULT: PASS` on `X2GUEL.cpp`; the committed pre-fix file gives `STUCK ESI_SI_A_EL_SONIC_ASSAULT_STING_MEMO_FIRE`; `X2GUAra.cpp` PASS, so Ara does not have the bug ), wired into `x2-phase` Finish, `tools/README.md` and memory. **Redeploy:** `DEPLOY OK: X2_offline.exe is 26,430,464 B, dated 2026-11-13 12:51:22, md5 ba2d00dd457ca39007ee3fbff3605501, equal to X2/US_SERVICE/x2.exe (2026-11-13 12:47:56); built at main 6856c19c + 1 uncommitted code file(s).` `EXESTR OK: 2 of 2` ( wide `ELESIS SKILL equip skill`, `EffectSet_SA_EL_SONIC_ASSAULT_CHARGE` ); first deploy attempt refused while the game ran.
- **Play-test ( user, 2026-11-13 12:53-12:54, exe `ba2d00dd`, first run on `62a1a621` found the stuck fire state ): all passed.** Logs: `ELESIS SKILL equip` 28000 / 28200 / 28001 / 28201 ( bank A slots 0-3, states 49 / 51 / 53 / 55 ), 28002 / 28004 / 28029 ( bank B slots 5-7, states 59 / 61 / 63 ); 7 `ELESIS UI cut-in set 0 name HQ_CutIn_Elesis_01` ( one per special active cast ); `SBUF parse: state 51 ... cleared ( learn check, skill 28402 not learned )` x2 ( Dodge and Slash, wewe has no Agility Mastery, as the CH1 hand-off expected ); ErrorLog 0 lines for `KEM_ERROR` / `Motion Error` / `ErrorEnum`; packets EXCEPTION 0 ( 2 `E_DISABLE_HB_CHECK_REQ` UNHANDLED, `AddGroupChainQuest` x1: not CH2a's ). `NUM use`: no lines ( only `JUNE_SKILL LOAD` carries the tag ). Not played: base 28003, the upgrade ids 28026-28028 / 28030 / 28217 / 28218 ( the CH2d review or CH6 ); Dodge and Slash's re-press and back step wait for CH4a.
- **Play-test hand-off:** see the session reply. Unit 20 `wewe` ( class 8, lv 8 ): slots A 0 Spiral Blast 28000, 1 Dodge and Slash 28200, 2 Wild Shock 28001, 3 Endurance Power 28201; B 5 Mega Burster 28002, 6 Unlimited Blade 28004, 7 Sonic Assault: Sting enhanced 28029 (memo). Not slotted ( 8 slots ): base 28003 and the other upgrade ids 28026-28028 / 28030 / 28217 / 28218. Log tag `[JUNE_CHAR]` (`ELESIS SKILL`).

### CH2b - Done (2026-11-13)

Sonnet 5.5. Same flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL` + the CH2a `_SKILL_DEBUG` ( stays on for the CH2d review ). No review of its own.
All 28 class-map rows read from `charport_adis.py elesis --phase CH2b` listings and the Ghidra decompile of `32b75f90`: **18 functions written** ( 15 inside March's function, 3 new; 6 of the 15 only swap in the line-test cancels, the charge one also the flag chains ), **10 = March**. `funcmap.tsv` rows `Char Elesis CH2b` ( 28, same addresses as the class map ).
- **Registration:** June base and upgrade ids added as labels to March's cases ( Extinction 28005 / 28033, Critical Dive 28006 / 28036, Judgement Drive 28007 / 28045 ). Judgement Drive gains its hold-state FrameMove and registers the final state ( 223 ) with FrameMove + EventProcess ( constants only in the listing at `32b8ce09`, no EventProcess copy; `stateends.py` PASS ). New names: `RNSI_SA_SSK_EXTINCTION_UPGRADE` / `_CRITICAL_DIVE_UPGRADE` / `_JUDGEMENT_DRIVE_UPGRADE` ( `X2SkillTree.h` ), `BTI_BUFF_SGM_VICTORIOUS_SWORD_UPGRADE` 565 ( `X2BuffFactor.h` ), all under `JUNE_ENUM_NAMES`; their values and `SA_PROJECTILE_SIZE` 32, `SA_EFFECTIVE_TIME` 1, `SMI_ELESIS_MEMO6` 608012 proved by a `cl /Zs` probe.
- **What June differs in:** Extinction: the aim angle starts at 0 ( March -30 ); the attack plays the damage effect set with its scale ( 1.2, 1.4 with buff 565 while Victorious Sword is applied = March's `m_bIsApplyVictoriousSword`, +0x9610 ) times the upgrade's projectile size and the upgrade's effective time as life time, `SetEffectScale( scale, true )` + the aim angle as local rotation ( March: power rate only ); preloads 25 ( +10 ); the charge EventProcess' up / down flag chains also fire at animation end; the arrow particle handle is cleared once invalid; every EventProcess' walk / back-walk / dash / Z-X cancels need the foot on a line. FrameMoves, UI effect creation, FrameMoveFuture bodies, StartFuture bodies = March. Judgement Drive is now a hold state ( charge grows after event time 0, 2 s cap, 1 s upgraded; ends at event time + 2.1 s or on release ) and the final state ( damage effect set at 0.01 s, power x ( 1 + charge x 0.25 ), 0.5 upgraded; charge under 0.15 s = 0 ), sharing `m_fDashJumpZXChargeTime` ( +0x96f4 ) with the Dark Knight; March's memo 1.3x animation speed is dropped as June has none. Critical Dive: refund = half of the MP the skill start spent ( new `CX2GUUser::m_fSkillStartMpConsumed`, June +0x9440, written in `CountReservedSkillUse` around `ReservedSkillStart` ), slot found through the upgrade swap, `UpNowMpWithText`; March refunded half of the slot's nominal cost.
- **Not here, routed:** `+0x9610` is set by `SetSpecificValueByBuffTempletID` `32b89260` for BTI 239 / 326 / 565: March sets it for 239 / 326 only ( 565 = the enhanced Victorious Sword: CH3b0 ports `32b89260` whole ). `32b63be0` ( Blazing Dance Finish ) also reads `+0x9440`: CH3b. `GetEqippedSkillMemo` is still March's real function in Extinction's DEGREE states ( June's stub returns 0; `june_march_divergent.tsv` row, replacement not landed ). No `JUNE_BUGS` row. `NUM use`: numerical tables are June script data ( `MODS.md` "Stage 7 Elesis, CH2b" ).
- **Checklist** `(Elesis: CH2b ...)`: the `APPLIED_SKILL_ID` / `NUMERICAL_DATA_INDEX` rows ( Critical Dive's two tables ) and the `+0x9440` bullet.
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_SKILL --with _SKILL_DEBUG --worktree --files ...`: `4 PASS ( +237 / +5 / +9 / +3 ), 2 OTHER ( X2SkillTree.h, X2BuffFactor.h = JUNE_ENUM_NAMES ), 0 FAIL`, 0 deleted, 0 non-ASCII, 5 EOL-mismatched ( inserted next to the file's LF / CRLF boundary, each takes its anchor's EOL ), 0 untokened, 0 encoding change; `RESULT: PASS`. `junediverge --check` the 4 files: `allow-listed: 17`, `RESULT: PASS`. `enumscan --check X2GUEL.cpp`: 0 A/B rows. `stateends.py check X2GUEL.cpp ELESIS_KNIGHT`: 268 tables, 0 STUCK, `RESULT: PASS`. `luabind check ELESIS_KNIGHT`: 0 unbound, ENUM 0 missing, `RESULT: PASS`. `skillicons check` 28005-28007 / 28033-28036 / 28045: `misses 0`, `RESULT: PASS`. `/Zs` probes of `X2GUEL.cpp` and `X2GUUser.cpp`: 0 errors.
- **Deploy:** `DEPLOY OK: X2_offline.exe is 26,437,632 B, dated 2026-11-13 13:48:24, md5 f2899679c89d8a4e066018905a035295, equal to X2/US_SERVICE/x2.exe (2026-11-13 13:48:17); built at main 1fc6edcc + 20 uncommitted code file(s).` The first run failed in a peer's uncommitted `SetLineForce` ( june-dungeons C3y-2b, `X2GUUser.cpp:37771`, 21 errors, none in this phase's lines ); one re-run passed. `EXESTR OK: 2 of 2 found in X2_offline.exe, 26437632 B, md5 f2899679c` ( wide `EffectSet_SA_SSK_JUDGEMENT_DAMAGE`, `SSK_Extinction_Projectile02.Y` ).
- **Play-test ( user, 2026-11-13 14:06-14:08, exe `f2899679` ): all passed.** Logs: `ELESIS SKILL equip` 28005 / 28007 / 28006 ( bank A slots 0-2, states 49 / 51 / 53 ), class script 167 states, 25 `ELESIS UI cut-in` lines, ErrorLog 0 `Motion Error` and no Extinction / Judgement Drive / Critical Dive load line; packets EXCEPTION 0 ( 2 `E_DISABLE_HB_CHECK_REQ` UNHANDLED, `AddGroupChainQuest` x1: not CH2b's ). User confirmed: movable after every skill, Extinction aim / shot, Judgement Drive hold and final, Critical Dive refund and hit. Not played: the enhanced ids 28033 / 28036 / 28045 ( the CH2d review or CH6 ).
- **Play-test hand-off:** see the session reply. Unit 40 `SaberT` ( class 29, lv 70 ), slots set in the save: A 0 Extinction 28005, 1 Judgement Drive 28007, 2 Critical Dive 28006, 3 Dodge and Slash 28200; B 5 Sonic Assault: Sting 28003, 6 Unlimited Blade 28004, 7 Mega Burster 28002, 8 Spiral Blast 28000 ( Wild Shock and Rushing Sword unslotted ). The enhanced ids 28033 / 28036 / 28045 are not learned on this unit: a second round. Log tag `[JUNE_CHAR]` ( `ELESIS SKILL` ).

### CH2b2 - Done (2026-11-13)

Sonnet 5.5. Same flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL` + the CH2a `_SKILL_DEBUG` ( stays on for the CH2d review ). No review of its own.
All 15 class-map rows read from `charport_adis.py elesis --phase CH2b2` and the Ghidra decompile of `32b75210`: **10 written** ( 7 inside March's function, 3 new ), **5 = March / no code** ( `32b4f420` / `32b4f460` Init lists, `32b72e30`, and the two Start / StartFuture halves already ported ). `funcmap.tsv` rows `Char Elesis CH2b2` ( 15 ).
- **Registration:** June base and upgrade ids added as labels to March's cases ( Rushing Sword 28202 / 28219, Power Break 28203 / 28225, Leap Crash 28204 / 28226 ). Rushing Sword: Start ( June `32b705b0` ) and EndFuture on all three hits and the hyper state; Leap Crash: JUMP gains StartFuture, LANDING gains FrameMove. New enum names `RNSI_A_SSK_RUSHING_SWORD_UPGRADE` / `_POWER_BREAK` / `_POWER_BREAK_UPGRADE` ( `X2SkillTree.h`, `JUNE_ENUM_NAMES` ).
- **What June differs in:** Rushing Sword's EventProcess `32b85470` serves the 1st and 2nd hit ( the 2nd gains CommonEventProcess ): next-hit MP = `GetActualMPConsume` of the upgrade-swapped skill ( level >= 1 ), hit limit = Waltz of Sword count + ( 1 if the enhanced skill ) + 1 against a loop count that starts at 1 in the 1st StartFuture and is reset to 0 by the new EndFuture unless the next state is the 2nd / 3rd hit ( March: reset in the 1st StateStart ). All three hits run `32b705b0`: CommonStateStart, then Waltz of Sword ability > 0 and the destruction state: physic damage x ATK_REL ( = CH2a's `32b704c0` ). Leap Crash JUMP adds a speed factor ( x 2200 / 2700 after a dash, y 1000, flag 1 ); LANDING at 0.01 s plays `EffectSet_A_EL_LEAP_CRASH_Landing` with power rate and scale x min( 1 + 0.4 x dash time, 2 ) and a scale modifier ( tag 15 ) from `SA_SIZE_ABS` ( enhanced: numerical table 3 ). Power Break preloads 15 names ( March 2 ). Every EventProcess' walk / back-walk / dash / Z-X cancels need the foot on a line.
- **Dead in June, not ported:** Leap Crash's memo ( 608026 ) branches to states 312 / 313: `GetEqippedSkillMemo` is June's stub ( returns 0 ) and neither state is registered anywhere. No call to the divergent function, so no allow-list row.
- **Not here, routed:** `GetActualMPConsume` ( `32b7ebe0` ) is still March's override ( its Rushing Sword MP cut and Power Break memo cases key on the March ids ): CH4b. `m_iWaltzOfSwordDodgeAbility` and the loop-count writer for the June Waltz of Sword id: CH4a. `JUNE_BUGS` 649 ( landing effect's hyper flag is constant 0 ). `NUM use`: numerical tables are June script data ( `MODS.md` "Stage 7 Elesis, CH2b2" ).
- **Checklist** `(Elesis: CH2b2 ...)`: the `APPLIED_SKILL_ID` / `NUMERICAL_DATA_INDEX` rows ( Rushing Sword's three tables ).
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_SKILL --with _SKILL_DEBUG --worktree --files ...`: `2 PASS ( +207 / +3 ), 1 OTHER ( X2SkillTree.h = JUNE_ENUM_NAMES ), 0 FAIL`, 0 deleted, 0 non-ASCII, 0 EOL-mismatched, 0 untokened, 0 encoding change; `RESULT: PASS`. `junediverge --check` the 3 files: `allow-listed: 10`, `RESULT: PASS`. `enumscan --check X2GUEL.cpp`: 0 A/B rows. `stateends.py check X2GUEL.cpp ELESIS_KNIGHT`: 268 tables, 0 STUCK, `RESULT: PASS`. `luabind check ELESIS_KNIGHT`: `RESULT: PASS`. `skillicons check` 28202-28204 / 28219 / 28225 / 28226: `misses 0`, `RESULT: PASS`.
- **Deploy:** `DEPLOY OK: X2_offline.exe is 26,439,680 B, dated 2026-11-13 14:39:15, md5 13b50b83d7f84bbef603e6341faf4c9c, equal to X2/US_SERVICE/x2.exe (2026-11-13 14:39:08); built at main 84aca201 + 6 uncommitted code file(s).` `EXESTR OK: 2 of 2 found in X2_offline.exe, 26439680 B, md5 13b50b83d7` ( wide `EffectSet_A_EL_LEAP_CRASH_Landing`, `ALS_SHAKE_CHAOS_02.Y` ).
- **Save:** unit 40 `SaberT`: A0 Rushing Sword 28202, A1 Power Break 28203, A3 Dodge and Slash 28200, B5 Leap Crash 28204 ( newly learned, level 1 ), B7 28002, B8 28000; 28003 / 28004 / 28005 / 28006 / 28007 unslotted. Upgrade ids not learned: second round.
- **Play-test ( user, 2026-11-13 14:47-14:48, exe `13b50b83` ): all passed.** Logs: equip lines 28202 / 28203 / 28204 ( bank A slots 0-1, bank B slot 0; states 49 / 51 / 59 ), Rushing Sword chain 49 -> 149 -> 150 and an early stop after the 2nd hit, ErrorLog 0 `Motion Error` and no crash; 4 `OpenXET` lines for Power Break's new mesh preloads ( optional `.XET` missing, as March's own `MEGA_SLASH_TRACE_DUMMY.XET` ); packets EXCEPTION 0. User confirmed: Power Break, Leap Crash jump and landing, movable after every skill, Rushing Sword 2nd / 3rd hits and the low-MP text. Not played: the enhanced ids 28219 / 28225 / 28226. Log tag `[JUNE_CHAR]` ( `ELESIS SKILL` ).

### CH2c - Done (2026-11-13)

Sonnet 5.5. Same flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL` + the CH2a `_SKILL_DEBUG` ( stays on for the CH2d review ). No review of its own.
All 15 class-map rows read from `charport_adis.py elesis --phase CH2c` and the Ghidra decompile of `32b7db80` / `32b8c470`: **12 written** ( 6 inside March's function, 5 new + the Swordfire EventProcess rewrite counted inside ), **3 = March / no code** ( Big Burst, Fireworks, Scaldic Sword Init lists ). `funcmap.tsv` rows `Char Elesis CH2c` ( 15 ).
- **Registration:** June base and upgrade ids added as labels to March's cases ( Swordfire 28012 / 28034, Scaldic Sword 28013 / 28037, Infernal Blade 28014 / 28050, Big Burst 28207 / 28220, Fireworks 28209 / 28230 ); Avatar of the Fire ( 28208 / 28229 ) is a new case ( StateInit, FrameMove, EventProcess, hyper copy ). Swordfire also registers its air / ground states ( 233 / 234, EventProcess only: the registration listing `32b8d1c2`-`32b8d20c` copies nothing else; `stateends.py` PASS ). 11 enum names in `X2SkillTree.h` ( `JUNE_ENUM_NAMES` ) and the state `ESI_SI_A_SPK_AVATAR_OF_THE_FIRE` in `EL_STATE_ID`.
- **What June differs in:** Swordfire's skill state only picks the ground or air state on its first frame; March's landing / air-speed body is now the air state's ( = March's old body ). Swordfire preloads 5 names ( March none ), Infernal Blade 6 more. Every EventProcess' walk / back-walk / dash / Z-X cancels need the foot on a line. Avatar of the Fire ( new ): 0.01 s aura effect set ( `_FireKing` with Blaze Absolu learned, `+0x97a4` ) deleted on damage react + size modifier ( tag 15 ) from SA_SIZE_ABS of 28229; every valid living unit within 1000 loses Fire / Enchant Blaze extra damage and BTI 37 / 293, each loss pulls an absorb particle in ( black-hole position = the caster's Bip01 ) and refunds MP min( 10 x losses, max MP ); 0.1 s damage effect set ( delete NONE ) + the same modifier; its EventProcess has no Z-X cancel and falls into jump-down in the air.
- **Not here, routed:** `m_usBlazeAbsoluLevel` writer: CH4a. Swordfire's `SetEquippedSkillLevel` block was CH1i's. Checklist 7h: the two Avatar effect-set registrations use `{ DCES_DAMAGE_REACT }` / `{ DCES_NONE }`, not `DCES_SKILL_STATE_END` as the row guessed; `32b77b30` stays dead. `JUNE_BUGS` 650 ( the absorb loop has no team or self test ). `NUM use`: Big Burst's numerical-only table is June script data ( `MODS.md` "Stage 7 Elesis, CH2c" ).
- **Checklist** `(Elesis: CH2c ...)`: the `APPLIED_SKILL_ID` / `NUMERICAL_DATA_INDEX` rows ( Big Burst ) and the 7h `AddEffectSetDeleteCondition` row.
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_SKILL --with _SKILL_DEBUG --worktree --files ...`: `2 PASS ( +240 / +8 ), 1 OTHER ( X2SkillTree.h = JUNE_ENUM_NAMES ), 0 FAIL`, 0 deleted, 0 non-ASCII, 6 EOL-mismatched ( each takes its anchor's EOL ), 0 untokened, 0 encoding change; `RESULT: PASS`. `junediverge --check` the 3 files: `allow-listed: 10`, `RESULT: PASS` ( `--check-all` still lists 4 `GT_PVP` hits in `X2GUEL.cpp` that are not this phase's: the CH2d review ). `enumscan --check X2GUEL.cpp`: 0 A/B rows. `stateends.py check X2GUEL.cpp ELESIS_KNIGHT`: 272 tables, 0 STUCK, `RESULT: PASS`. `luabind check ELESIS_KNIGHT`: `RESULT: PASS`. `skillicons check` the 12 ids: `misses 0`, 2 known June gaps ( 28208 / 28229 picture absent from June's own sheet ), `RESULT: PASS`.
- **Deploy:** `DEPLOY OK: X2_offline.exe is 26,441,216 B, dated 2026-11-13 15:21:41, md5 15653d8183d0772d6bbb4c84e01dddce, equal to X2/US_SERVICE/x2.exe (2026-11-13 15:21:34); built at main f9d757f8 + 6 uncommitted code file(s).` `EXESTR OK: 3 of 3 found in X2_offline.exe, 26441216 B, md5 15653d8183` ( wide `EffectSet_A_SPK_Avatar_Of_TheFire_FireKing`, `Avatar_of_the_Fire_Absorb_P01`, `SPK_InfernalBlade_WeaponM01.x` ).
- **Tree fix found on the way:** HEAD's `X2GUUser.cpp` / `.h` and `Always.h` carried CH2b2's `fNowMpBeforeSkillStart` / `m_fSkillStartMpConsumed` hunks in the wrong places ( HEAD did not compile; the CH2b2 play-test built the corrected worktree ). Committed separately as the CH2b2 fix.
- **Save:** unit 21 `eresisu` ( class 30, lv 77 ): A 0 Swordfire 28012, 1 Scaldic Sword 28013, 2 Infernal Blade 28014, 3 Fireworks 28209 ( newly learned, level 1 ); B 5 Big Burst 28207, 6 Avatar of the Fire 28208; the other skills unslotted. Upgrade ids not learned: second round.
- **Play-test ( user, 2026-11-13 15:37-15:38, exe `15653d81` ): all passed, no stuck motion.** Logs: equip lines 28012 / 28013 / 28014 / 28209 ( bank A slots 0-3, states 49 / 51 / 53 / 55 ) and 28207 / 28208 ( bank B slots 0-1, states 59 / 61 ), Infernal Blade's `SBUF frame: state 53 BFI 658`; ErrorLog 0 `Motion Error`, no load line for this phase's new preloads; packets EXCEPTION 0 ( 2 `E_DISABLE_HB_CHECK_REQ` UNHANDLED, `AddGroupChainQuest` x1: not CH2c's ). Known, not this phase's: 3 optional `SCALDIC_SWORD_0n.XET` open failures ( March preloads the same `.Y` meshes ) and 4 `Update3DPosition result : 47` on `ELESIS_FIREWORKS_EXPLOSION.OGG` ( the file is in data069; the same engine line as Chung's phases ). No `NUM use` line is emitted by this build ( as CH2b / CH2b2 ). Not played: the enhanced ids ( 28034 / 28037 / 28050 / 28220 / 28229 / 28230 ): the CH2d review or CH6. Log tag `[JUNE_CHAR]` ( `ELESIS SKILL` ).

### CH2d - Done (2026-11-13)

Sonnet 5.5. Same flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL` + the CH2a `_SKILL_DEBUG` ( stays on for the CH2 stage review, `kickoffs/char-elesis/CH2d-review.md` ).
All 27 class-map rows read from `charport_adis.py elesis --phase CH2d` listings and the Ghidra decompile of `32b89c10` / `32b89470` / `32b8b410` / `32b8b850` / `32b8c470`: **27 written, 0 inside March's functions** ( March has no Dark Knight skill: every state is new ), the 6 Init preload lists generated from the listings, the 5 identical EventProcess compared mechanically. `funcmap.tsv` rows `Char Elesis CH2d` ( 28 with `31fc7ac0` ); `MODS.md` "Stage 7 Elesis, CH2d"; `JUNE_BUGS.md` 651 / 652.
- **Registration:** six new cases in `SetEquippedSkillFuncBySkillSlotIndex` ( June `32b8c470` @596 / @630 / @638 / @1190 / @1198 / @1206 ), base and enhanced ids on one case. Normal state: `StateInit` + the callbacks of the table row; hyper copy: the same without Init. Second states are fresh `UserUnitStateData` ( the listing calls `Init` before each, so no callback is copied ): Ejection Buster attack ( FrameMove, EventProcess ), Burning Phantom START ( EventProcess ) / LOOP ( FrameMove, EventProcess ) / END ( EventProcess ), Shade Scratch ATTACK ( EventProcess ). No `SetEquippedSkillLevel` work: CH1i wrote the 22 blocks. Enum names: 8 `RNSI_*` ids ( `X2SkillTree.h` ), 3 BTI + 3 BFI ( `X2BuffFactor.h` ), `JUNE_ENUM_NAMES`.
- **New helpers:** `ApplyBleedingHpCost` ( `32b89470`, used by Brutal Cutter, Dark Wave, Injection Stigma; CH3c's states call it too ), `IsAnySkillKeyPressed` ( `31fc7ac0` ), member `m_usBurningPhantomLoopCount` ( +0x96fa ); file-local `TryGetAcquiredSkillAbilityValue` ( Ara's copy ) and `RescaleBuffFactorTime` ( June inlines slot 476's type 0xf in Brutal Cutter; March's helper is `static` in `X2GUUser.cpp` ) in `_CONST_EL_CH2D_`.
- **What June does that reads odd ( kept ):** the helper returns true on every path ( JUNE_BUGS 651 ); the Burning Phantom loop FrameMove returns without CommonFrameMove once the count is past 9 ( 652 ); Injection Stigma reads `GetSkillLevel` twice and drops the result; the first `322cc1a0` call in Brutal Cutter is dead. Five EventProcess ( Brutal Cutter, Burning Phantom end, Injection Stigma, Dark Wave, Shade Scratch attack ) are one body, compared byte for byte after normalising addresses; Ejection attack lands only on a line, Shade Scratch's first branch reads Future `unitCondition.bHit` ( +0x118 ) after `m_fEventTime[0].keyInputStart`.
- **Not here, routed:** Ejection Buster's held key ( CH1p review ): `m_kSkillSlotKey.Set` is written in the charge StartFuture and no state in its range reads it ( no `FollowSlotSwap` / `IsKeyReleased`; the charge state lasts one frame ). The blood hooks ( `0x4f8` / `0x4fc` ) stay CH4c; `m_fDestructionBleedingDamageRate` is read inside the `_WOS` line as CH1q said. Enhanced ids not played ( review or CH6 ).
- **Checklist** `(Elesis: CH2d ...)`: the Brutal Cutter `NUMERICAL_DATA_INDEX` rows ( §1 line 116 / 117 ).
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_SKILL --with _SKILL_DEBUG --worktree --files ...`: `2 PASS ( +664 / +29 ), 2 OTHER ( X2SkillTree.h, X2BuffFactor.h = JUNE_ENUM_NAMES ), 0 FAIL`, 0 deleted, 0 non-ASCII, 0 EOL-mismatched, 0 untokened, 0 encoding change; `RESULT: PASS`. `junediverge --check` the 4 files: `allow-listed: 10`, `RESULT: PASS`. `enumscan --check X2GUEL.cpp`: 0 A/B rows. `stateends.py check X2GUEL.cpp ELESIS_KNIGHT`: 289 tables, 0 STUCK, `RESULT: PASS`. `luabind check ELESIS_KNIGHT`: `RESULT: PASS`. `skillicons check` the 12 ids: `misses 0`, `RESULT: PASS`. A `/Zs` probe of `X2GUEL.cpp`: 0 errors, with static asserts on the values the code relies on ( the 8 skill ids, 6 buff ids, `SA_EFFECTIVE_TIME` 1, `SA_ATK_PHYSIC_REL` 7, `MI_CHECK_AND_KILL_1` 15, the state ids, `LINE_RADIUS` 30 ).
- **Deploy:** my `build.py --deploy`: `BUILD OK: X2/US_SERVICE/x2.exe 26,458,624 B, 2026-11-13 16:17:04` ( md5 `30c41b33` ), deploy refused ( game running ), then `--deploy-only` queued behind another session's full build, which compiled this tree ( my edits were on disk ) and deployed it: `X2_offline.exe` 26,462,720 B, 2026-11-13 16:27:38, md5 `4263c1f75e4cbf50eb87d6ed7576dec4`, equal to `X2/US_SERVICE/x2.exe` ( that build also carries a peer's `JUNE_GFX_SCENE_TRACE_DEBUG` ). `EXESTR OK: 5 of 5 found in X2_offline.exe, 26462720 B, md5 4263c1f7` ( wide `EffectSet_SI_SA_SDK_EJECTION_BUSTER_Charge_3`, `EffectSet_B_EL_SDC_Bleeding`, `DamageEffect_A_SDK_DARK_WAVE_DUMMY`, `EffectSet_SA_SDK_BrutalCutterFX2_MEMO`, `SDK_BURNING_PHANTOM_StartDummy.X` ); narrow `SI_SA_SDK_BURNING_PHANTOM_LOOP` / `SI_A_SCA_SHADE_SCRATCH_ATTACK` found in my 30c41b33 build.
- **Save:** unit 144 ( `DKnight20` renamed `DKnight80`, class 35, lv 80, sp 605, not transcended; backups `db_backup/` from `elsdb.py` ): A 0 Dark Wave 28212, 1 Injection Stigma 28213, 2 Shade Scratch 28214, 3 Brutal Cutter 28020; B 5 Ejection Buster 28019, 6 Burning Phantom 28021 ( the last four newly learned at level 1 ); the old 28000-28004 / 28200 / 28410 unslotted. Enhanced ids not learned.
- **Play-test ( user, 2026-11-13 16:50-16:52, exe `4263c1f7` ): Ejection Buster, Dark Wave, Injection Stigma, Shade Scratch, Burning Phantom passed; Brutal Cutter's weapon effect stayed after the buff ended.** Logs: 6 equip lines ( 28212 / 28213 / 28214 / 28020 bank A slots 0-3, 28019 / 28021 bank B slots 0-1; states 49 / 51 / 53 / 55 / 59 / 61 ), ErrorLog 0 lines, packets EXCEPTION 0 ( 2 `E_DISABLE_HB_CHECK_REQ` UNHANDLED and `AddGroupChainQuest` x1: not CH2d's ).
- **Fix ( same day ):** Brutal Cutter's FrameMove pushes the handle of each of its two effect sets on `m_vecEffectSetToDeleteOnStateEnd` ( June `32b89c10` @32b8a110 / @32b8a24a, which I had read as a handle destructor ); the other five Dark Knight listings have no such call. `flagcheck --worktree` PASS ( 2 deleted lines: the two plain `PlayEffectSet` lines inside the flag ), `/Zs` probe 0 errors. `DEPLOY OK: X2_offline.exe is 26,462,720 B, dated 2026-11-13 16:55:34, md5 9043fc8bad96f57677ca5b17b6a04fc4, equal to X2/US_SERVICE/x2.exe (2026-11-13 16:55:27); built at main 1c4a9754 + 1 uncommitted code file(s).` `EXESTR OK: 2 of 2 found in X2_offline.exe, md5 9043fc8b` ( wide `EffectSet_SI_SA_SDK_BRUTAL_CUTTER_FX2`, `EffectSet_B_EL_SDC_Bleeding` ). Re-test ( user, 2026-11-13 17:36-17:38, exe `9043fc8b` ): Brutal Cutter's weapon effect now ends with the buff; logs: 6 equip lines, ErrorLog 0 lines, packets EXCEPTION 0 ( 2 `E_DISABLE_HB_CHECK_REQ` UNHANDLED ), 0 unbound Lua names.

### CH2d - Review (2026-11-13)

Opus 5.5 high. The CH2 stage review: CH2a `454c376c` (+ fix `4a74cb3b`), CH2b `9d42957b`, CH2b2 `699daff1` (+ fix `9adceac8`), CH2c `5326702e`, CH2d `1c4a9754` (+ fix `c3afbbe8`).
**Verdict: pass, no code change; the enhanced-id play-test passed** (below). Only edit: `_SKILL_DEBUG` retired.
- **Spot-checks against the June listings** (`charport_adis.py elesis <va>`), 8 functions, all = June:
  - `32b89470` bleeding helper: effect set first; HP share < 0.2 (double `(double)0.2f`) returns true; copy defender = this; WoS destruction x `+0x9720` for WST 4-6;
    min( GetFinalDamage x count, HP - 0.1 max HP ); BTI 666 x0.2; UpNowHp( -cost, 0 ) only when > 0; true on every path (JUNE_BUGS 651). June calls vslot 0x70 twice
    (compare, then value) and zeroes copy +0x50c-0x50f (EXCEPT_ME_HP_UP / ATTACK_ONLY_ME, unread by GetFinalDamage); one call is equal here: March's GetFinalDamage has
    no random draw (June's variance roll `322788c0` is not ported, funcmap rows 698 / 927).
  - `32b89c10` Brutal Cutter: Oneshot 0.2, helper( m_DamageData, 0.1, 8 ), x1.3 PvP / PvE else x1.4 on both damages, BTI 509 / 429 test, erase 429, BFI 466 / 593,
    memo type 0xf (`MI_CHECK_AND_KILL_1`) rescale of the first time value, `31fcf3e0` ( upgraded ) time x ability 1 ( `SA_EFFECTIVE_TIME` ), `SetGameUnitBuffFactor( this, -1, true )`,
    vslot 0x224; `32a2dec0` at @32b8a110 / @32b8a24a is `vector<UINT>::push_back` on `+0x3ec0` = the CH2d fix is right.
  - `32b8b410` Injection Stigma: 0.01 s ATK_REL (7) x `+0x4424` physic attack (x0.2 with BTI 666), 0.2 s dummy damage effect + helper count 1, power rate x1.3 / x1.4,
    erase BTI 433, BFI 470 = ( 470, no creator, level 1, adjust 0, skip false ).
  - `32b73ae0` / `32b51d10` Burning Phantom loop: shock time cleared; count > 9 returns with no CommonFrameMove (JUNE_BUGS 652); tick = trunc( state time / 0.15 ), 10 MP above
    tick 4; from tick 4: MP < 10, tick >= 9 or no skill key -> END (298).
  - `32b8c470` cases @596 / @630 / @638 / @1190 / @1198 / @1206: every second state (293, 296-298, 308) has `UserUnitStateData::Init` before its stores, constants only.
  - `32b75580` (CH2a) Sonic memo charge: hold += dt, MP 100 / s if affordable and charge < 0.5, charge = min( +dt, 0.5 ), effect set at >= 0.5.
  - `32b60650` (CH2b) Critical Dive: hit after 0.425 s, refund = `+0x9440` x 0.5 through `UpNowMpWithText`, on-line cancels.
- **Held key (CH1p review note): confirmed.** Ejection Buster sets `m_kSkillSlotKey` in the charge StartFuture (`32b51b90`); the charge EventProcess `32b51be0` changes state on
  its first frame and the attack state's two functions (`32b7d110`, `32b67ff0`) read no key. Nothing to port.
- **`GT_PVP` hits (CH2c note): none now.** `junediverge --check-all`: `allow-listed: 232`, `RESULT: PASS`, no hit printed; `X2GUEL.cpp` / `.h`: `allow-listed: 10`, `RESULT: PASS`.
  The allow list has not changed since CH2c, so the four were not in committed `X2GUEL.cpp` code; its June-code PvP tests are CH1's three allow rows (June tests type 1 only).
- **Hygiene** (one `flagcheck JUNE_CHAR_ELESIS_SKILL --with _SKILL_DEBUG --range <c>^..<c>` per commit): all 8 `RESULT: PASS`, 0 FAIL, 0 non-ASCII, 0 untokened, 0 encoding change;
  deleted lines only in `9adceac8` (26: the CH2b2 hunk move) and `c3afbbe8` (2: the plain `PlayEffectSet` lines); `eol!` 5 (`9d42957b`) and 6 (`5326702e`), each the anchor's EOL.
  Retirement: `flagcheck JUNE_CHAR_ELESIS_SKILL --with _SKILL_DEBUG --worktree --files KTDXLIB/Always.h`: `1 OTHER` (a peer's uncommitted `JUNE_PARTICLE_LUA_LAYER` block), `RESULT: PASS`.
  `enumscan --check X2GUEL.cpp`: 0 A/B rows. `stateends.py check X2GUEL.cpp ELESIS_KNIGHT`: 289 tables, 0 STUCK, `RESULT: PASS`. `skillicons check 28000-28999`: 129 skills,
  misses 0, 2 known June gaps (28208 / 28229), `RESULT: PASS`.
- **CH2d play-test logs** (run 17:36:51-17:38:15, exe `a0d1654e` = `9043fc8b` + Q12): 6 equip lines (states 49 / 51 / 53 / 55 / 59 / 61), every state above has a callback
  (stateends), 0 `EXCEPTION`, 0 `KEM_ERROR`, 0 `Motion Error`, 0 unbound Lua names; no `NUM use` line (as every CH2 phase).
- **Debug retire:** `//#define SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL_DEBUG` (`Always.h`; guarded code stays). Build skipped: debug retire only. The deployed exe still has
  the equip lines, which the enhanced-id test below reads. `kickoffs/char-elesis/CH3a.md` amended: CH3a turns the define back on for the CH3 play-tests.
- **Save for the enhanced-id test** (`elsdb.py x`, backup taken): base ids set to level 0, the enhanced id learned at level 1 (the save's own upgrade pattern, as 28000-28004 /
  28026-28030); unit 144 `DKnight80` (35): 0 28221, 1 28233, 2 28234, 3 28038, 5 28035, 6 28055, 7 28028, 8 28030; unit 21 `eresisu` (30): 0 28034, 1 28037, 2 28050,
  3 28230, 5 28220, 6 28229, 7 28026, 8 28027; unit 40 `SaberT` (29): 0 28219, 1 28225, 2 28033, 3 28217, 5 28226, 6 28036, 7 28045, 8 base 28003.
  Not slotted: 28218 (Endurance Power enhanced, a buff return) and 28029 (played in CH2a): CH6.
- **Enhanced-id play-test (user, 2026-11-13 23:32-23:45): all passed.** Logs: 8 equip lines per unit with the ids above (144 at 23:33 / 23:35, 21 at 23:35:57, 40 at
  23:36:56 / 23:41:19; states 49-55 / 59-65), Infernal Blade+ `SBUF frame: state 53 BFI 658`, Saber's `SBUF` 28402 / BFI 656 on states 49 / 149 / 55; ErrorLog 0 `Motion Error`,
  0 `KEM_ERROR`; packets 0 `EXCEPTION` (5 `E_DISABLE_HB_CHECK_REQ` UNHANDLED); 0 unbound Lua names. Known, not CH2's: optional `.XET` misses (`SDK_*` / `SSK_*` / `SPK_*`
  blades, as March's own), the CH0b `Anim Frame 79, Mesh Frame 80` lines on the `MESH_EL_SDK_*` part meshes, a peer's `X2SlideShot` lines.
- **Stale PCH note:** that run's exe (md5 `79b337ec`, a peer's 17:52 build at `1ccad820`) still printed the equip lines, so its PCH kept the old `Always.h`. Harmless here
  (CH3a turns the define back on); the CH3d review's retire must prove the tag gone with `exestr.py --absent` after a `--pch force` build if it builds.

## Phase CH3 - Skills: second jobs (120, 121, 125), summons, hyper actives, transcendence

**Run with:** as CH2 (CH3b0 and CH3d on Opus); one Opus review at CH3d (covers CH3a-CH3d; CH3b0 keeps its own). Sub-tag `SKILL`.

| Sub-phase | Scope |
|---|---|
| CH3a | Grand Master (120) skills (the Dignity aura `32b7e740` with BFI 910 passing `x`, checklist 7ai) |
| CH3b0 | **Opus.** Summons (`ch0_summon.tsv`), flag `_SUMMON`: vslots 0x24c `32b89260` / 0x250 `32b74500` whole (the `m_fCreateEffectSetRate` writers `@32b8930d` / `@32b7471b` and the Fire Blossom spawn `@32b746e8`), `32b79db0` (Fire Blossom), `32b7a0a0` (Wall of Prominence 1635 / Fire King 1971), the spawn call of `32b8b0c0` (Blood Cutter 1641; the state body is CH3c's), `CheckSummonSpecialAttack` `3202daf0`, the `+0x8e74` related-skill-level write, the three NPC enum names, `luabind.py check` on the four NPC scripts and their templet rows; own review |
| CH3b | Blazing Heart (121) skills (Wall of Prominence and Fire Blossoms on CH3b0's spawns; Soul Ignition's effect-set rate) |
| CH3c | Crimson Avenger (125) skills (after CHNb; Blood Cutter on CH3b0's spawn; the bare `SI_*_SCA_*` tables) |
| CH3c2 | **Opus.** Crimson Avenger Shadow Edge 28215 / 28235 (split out of CH3c 2026-11-14: nearest-target search with an exclusion set, a 0x20-byte mark record on `+0x9700`, aimed Link effect, cloned buff factor; the record type is CH4c's) |
| CH3d | **Opus.** Hyper actives (type 13; the 28601.. switch in `32b8c470`), the 3 `SHOW_HYPER_CUT_IN_TIME` states (7t), and the **transcendence rows** (template, Stage 6 close-out): 120 / 121 / 125 on the offline transcend allow-list (`CX2OfflineServer::TranscendCheck`, `Handlers_Quest.cpp`), her `LevelUpEvent.lua` transcendence row + `DLG_Transcendental_Change_Guide_Elesis.lua`; checklist 7ao whole (`HyperModeBuffEffectStart` `32b74300` for a transcended unit; the four base helpers `31fcac90` / `31fcad70` / `31fceab0` `== 1` / `31ff1e80`); her hyper-class skills (`SPRT_HYPER_CLASS`: 28416 / 28417 `RNSI_P_SBH_TRANSCENDENCE` and the SGM / SCA ones) and `TRANSCENDENTAL_SKILL_DATA` rows; the `EFFECT_SET_LIST_EX_NO_TRANS` / `_TRANS_1` keys' transcended side; `GetHyperModeTableName` (`31fdd490`) and `HQ_Cutin_EGM / EBH / ECA_Transcendence`; `list_TR_*` rows in `GetCharacterImageName` (T6c) and her transcended pictures, read from June's row pushes; `POPUP_TR_G_MASTER` / `_B_HEART` / `_C_AVENGER` present; CH3 stage review |

**Done gate:** as CH2 for 120 / 121 / 125 and the hyper actives; each summon appears, acts and leaves as June's script says (the hand-off lists the
`ES_CREATE` and NPC state lines); a transcended Elesis of each second job (`settranscend`) casts her hyper-class skills, slots A-5 / B-5 work, the
transcended picture and cut-in show; `python tools/skillicons.py check <her skill-id range>` = PASS after the transcendence rows.

### CH3a - Done (2026-11-14)

Sonnet 5.5. Same flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL`; `_SKILL_DEBUG` is defined again ( `Always.h`, stays on until the CH3d review retires it ). No review of its own ( the CH3d review covers CH3a-CH3d ).
All 24 class-map rows read from `charport_adis.py elesis --phase CH3a` listings ( 18 listing-only ) and the Ghidra decompile of `32b8c470` / `32b60ce0` / `32b61010` / `32b6dde0` / `32b6de10` / `32b76320` / `32b78170`: **16 written** ( 8 inside March's functions, 8 new ), **7 = March / no code** ( the Inits of Sonic Blade, Juggernaut Buster, War Prelude: every call and string compared, same sequence; Juggernaut's `StateStart`, loop `FrameMove`, loop-last `StateEnd` / `EventProcess` ), Giga Blast's bare-`ret` Init `32b52530` not registered, plus the registration. `funcmap.tsv` rows `Char Elesis CH3a` ( 26 ); `MODS.md` "Stage 7 Elesis, CH3a"; `JUNE_BUGS.md` 654.
- **Registration:** June's labels on March's cases ( Victorious Sword 28008 / 28046, Sonic Blade 28011 / 28049, Juggernaut Buster 28010 / 28048, War Prelude 28009 / 28047 ); Juggernaut Buster's start state gains `FrameMove` ( hyper copy too ) and its end state `FrameMove`; War Prelude gains the air ( 334 ) and ground ( 335 ) states; three new cases: Cross Slash 28039, Divine Blade 28040, Giga Blast 28062 ( charge 336, end 337 = `CX2GUUser::GenericSpecialActiveSkillEventProcess` ), their hyper state takes the normal state's callbacks ( `CopyStateCallbacks`, June `329cc2c0` ). June's hyper copy of the March-case skills carries no `StateInit`; March's does ( kept: only re-preloads ). New names: 7 `RNSI_SA_SGM_*` ( `X2SkillTree.h` ), `BFI_BUFF_SGM_VICTORIOUS_SWORD_UPGRADE` 655 ( `X2BuffFactor.h` ). No `SetEquippedSkillLevel` work: CH1i wrote the blocks.
- **June differs from March:**

| June | Function | Difference |
|---|---|---|
| `32b6f550` / `32b6f620` | Juggernaut start / loop `EventProcess` | speed = dash speed x 0.75 / x 1.05 ( March 0.5 / 0.7 ) |
| `32b77de0` | start `FrameMove` ( new ) | at 0.8 s the BLACKHOLE effect set ( Trans variant for memo skill 28031 / 28060, WithVictoriousSword variant while `m_bIsApplyVictoriousSword` ); the handle goes on three delete lists |
| `32b78170` | end `FrameMove` ( new ) | at 0.29 s the Finish effect set ( WithVictoriousSword variant ); the handle is deleted on death |
| `32b76320` | Victorious Sword `FrameMove` | the buff block: level of 28008, BFI 345 unless BTI 326 / 565 is held, else erase BTI 326 + BFI 655 ( JUNE_BUGS 654 ) |
| `32b61010` | War Prelude `EventProcess` | goes at once to the air state ( not on a line, Dignity level > 0 ) or the ground state |
| `32b5c740` / `32b60d10` / `32b6f1f0` / the 4 new | other `EventProcess` | the walk / back-walk / dash / ZX cancels need the foot on a line; Sonic Blade and the FLY state land only on a line |

- **Not here, routed:** the Dignity aura `32b7e740` ( BFI 909 / 910 ) is CH0's CH4a item ( `ch0_class_map.tsv`: passive code, driven by the +0x9796 level that CH4a writes ); this kickoff's line naming it is stale. `m_bIsApplyVictoriousSword` ( +0x9610 ) is written for BTI 565 only by June's `0x24c` body ( CH3b0 ), so the enhanced buff's WithVictoriousSword effect variants wait for it. `GetActualMPConsume` / cool-time overrides: CH4b. Enhanced ids 28046 / 28048 / 28049 not played here.
- **Checklist** `(Elesis: CH3a ...)`: §1 l.52 / `APPLIED_SKILL_ID` and `NUMERICAL_DATA_INDEX` rows ( Victorious Sword's numerical-only `DAMAGE_DATA_NEXT` table, through the engine ).
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_SKILL --with _SKILL_DEBUG --worktree --files ...`: `2 PASS ( +331 / +8 ), 3 OTHER ( X2SkillTree.h, X2BuffFactor.h = JUNE_ENUM_NAMES; Always.h = the debug define and a peer's lines ), 0 FAIL`, 0 deleted, 0 non-ASCII, 0 EOL-mismatched, 0 untokened, 0 encoding change; `RESULT: PASS`. `junediverge --check` the 4 files: `allow-listed: 10`, `RESULT: PASS`. `enumscan --check X2GUEL.cpp`: 0 A/B rows. `stateends.py check X2GUEL.cpp ELESIS_KNIGHT`: 299 tables, 0 STUCK, `RESULT: PASS`. `luabind check ELESIS_KNIGHT`: 0 unbound, ENUM 0 missing, `RESULT: PASS`. `skillicons check` 28008-28011 / 28039-28040 / 28046-28049 / 28062: `misses 0`, PASS. `/Zs` probe of `X2GUEL.cpp`: 0 errors.
- **Deploy:** `BUILD OK: X2/US_SERVICE/x2.exe 26,474,496 B, 2026-11-14 00:25:06`; `DEPLOY OK: X2_offline.exe is 26,474,496 B, dated 2026-11-14 00:25:13, md5 d7fa0c0e464de1f91722a03fab1431ea, equal to X2/US_SERVICE/x2.exe (2026-11-14 00:25:06); built at main 070b4381 + 6 uncommitted code file(s).` `EXESTR OK` ( md5 d7fa0c0e ): wide `EffectSet_SGM_JuggernautBuster_BLACKHOLE_WithVictoriousSword`, `EffectSet_SGM_JuggernautBuster_Finish_WithVictoriousSword`, `VictoriousSword_UpgradeWeaponM01.X`, `ELESIS SKILL equip`; narrow `ESI_SA_SGM_GIGA_BLAST_CHARGE`, `ESI_SA_SGM_CROSS_SLASH`. The build also carries a peer's uncommitted `SERV_IRUHADEV_GFX_SCENE_CLASS_RELEASE`.
- **Fix ( same day ):** the first deploy `d7fa0c0e` had no June id labels on March's War Prelude case, so slot state 53 registered blank and the press was refused ( `[JUNE_SKILL] MIG no-state skill 28009 ... presses are refused` ); `RNSI_SA_SGM_WAR_PRELUDE` / `_UPGRADE` added ( `flagcheck --worktree` PASS, `/Zs` probe 0 errors ). `DEPLOY OK: X2_offline.exe is 26,475,008 B, dated 2026-11-14 00:44:12, md5 9b71e8e665303db563dfcf20915b228b, equal to X2/US_SERVICE/x2.exe (2026-11-14 00:44:05); built at main ea139503 + 8 uncommitted code file(s).` First play-test ( user, 2026-11-14, exe `d7fa0c0e` ): Sonic Blade, Juggernaut Buster, Victorious Sword, Cross Slash, Divine Blade and Giga Blast passed; logs: 7 equip lines, ErrorLog 0 lines, packets EXCEPTION 0, 0 `Motion Error` / `KEM_ERROR`; War Prelude refused ( above ). **Re-test ( user, 2026-11-14 00:45-00:46, exe `9b71e8e6` ): War Prelude passed.** Logs: equip line 28009 slot 2 bank A state 53, no `MIG no-state` line, 6 cut-in lines, `ES_CREATE EffectSet_SA_SGM_WAR_PRELUDE` ( ground state ), ErrorLog 0 lines, packets EXCEPTION 0 ( 2 `E_DISABLE_HB_CHECK_REQ` UNHANDLED ), 0 unbound Lua names. All 7 skills passed; the enhanced ids and War Prelude's air state ( needs CH4a's Dignity level ) are not played.
- **Save:** unit 29 ( `ElesisGM`, class 120, lv 80, not transcended; backup in `db_backup/` from `elsdb.py` ): A 0 Sonic Blade 28011, 1 Juggernaut Buster 28010, 2 War Prelude 28009, 3 Victorious Sword 28008; B 5 Cross Slash 28039, 6 Divine Blade 28040, 7 Giga Blast 28062 ( 28010 / 28040 / 28062 newly learned at level 1 ); slots 4 / 8 / 9 empty. Enhanced ids not learned.

### CH3a2 - Done (2026-11-14)

Sonnet 5.5. Same flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL`; `_SKILL_DEBUG` stays on until the CH3d review retires it. No review of its own ( the CH3d review covers CH3a-CH3d ).
All 16 class-map rows read from `charport_adis.py elesis --phase CH3a2` and the Ghidra decompile of `32b8c470` ( the registration ): **7 written** ( 5 inside March's functions, 2 new ), **9 = March / no code** ( Drastic Charge's Init, `StateStart`, `StartFuture`, start `EventProcess`, loop `EventProcess` / `FrameMove`, start `FrameMove`, end `StateStart`; Provoke's `StateStart` / `StateEnd` ), plus the registration. `funcmap.tsv` rows `Char Elesis CH3a2` ( 17 ).
- **Registration:** June ids as labels on March's cases ( Provoke 28205 / 28227, Drastic Charge 28206 / 28228 ); Provoke gains `StateStartFuture` ( normal and hyper ); new case Upper Slash 28222 ( `EventProcess` only, hyper = `CopyStateCallbacks` ). Drastic Charge's tick MP ( `m_fDrasticChargeLoopDecreaseMPValue` ) is now read from `GetEquippedActiveSkillTemplet( 28206, true )` with `GetSkillLevel( ..., false, 0, true )` ( June `32b8e07b` / `32b8e0a7` ): March's lookup of its own id 8215 finds nothing on a June save, so the loop would have cost 0 MP. New names: `RNSI_A_SGM_UPPER_SLASH` 28222, `RNSI_A_SGM_PROVOKE_UPGRADE` 28227 ( `X2SkillTree.h` ). No `SetEquippedSkillLevel` work: CH1i wrote the blocks.
- **June differs from March:**

| June | Function | Difference |
|---|---|---|
| `32b85aa0` | Provoke `EventProcess` | a skill-key press inside EVENT_PROCESS_TIME0's window ( 0.35 s on ) with MP >= 10 spends 10 MP ( `UpNowMp( -10 )` ) and arms event flag 0; at 0.35 s or the animation end the flag starts the Revenge state; below 10 MP the line `STR_ID_2549`. The counter-on-hit path stays. |
| `32b50b00` | Provoke `StartFuture` ( new ) | `CommonStateStartFuture`, then the slot key is remembered |
| `32b50a30` | Provoke `Init` | 9 more preloads ( March's 7 distinct come first ) |
| `32b6ced0` | Upper Slash `EventProcess` ( new ) | `USI_JUMP_DOWN` at the end of the animation or once USEFUL_VALUE0 ( `GetUsefulValueFuture( 0, 0 )` ) has passed off a line; then the skill cancel and the on-line walk / back-walk / dash / Z-X cancels |
| `32b5fb30` / `32b85aa0` / `32b5ffc0` | Revenge / Provoke / Drastic End `EventProcess` | the walk / back-walk / dash / Z-X cancels need the foot on a line |

- **Not here, routed:** nothing new. June's change watcher on the tick-MP write ( `32b8e100`-`32b8e16f`, a debug report ) is not ported. `NUM use`: Drastic Charge End and Upper Slash are numerical-only tables read by the engine ( `MODS.md` "Stage 7 Elesis, CH3a2" ). No `JUNE_BUGS` row: the key window opens at the state-change time, so the 10 MP cannot be charged twice.
- **Checklist** `(Elesis: CH3a2 ...)`: §1 l.52 ( the two numerical-only tables ), the `APPLIED_SKILL_ID` / `NUMERICAL_DATA_INDEX` rows, and 7h l.430 ( USEFUL_VALUE0 read ).
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_SKILL --with _SKILL_DEBUG --worktree --files ...`: `2 PASS ( +121 / +2 ), 1 OTHER ( X2SkillTree.h = JUNE_ENUM_NAMES ), 0 FAIL`, 0 deleted, 0 non-ASCII, 0 EOL-mismatched, 0 untokened, 0 encoding change; `RESULT: PASS`. `junediverge --check` the 3 files: `allow-listed: 10`, `RESULT: PASS`. `enumscan --check X2GUEL.cpp`: 0 A/B rows. `stateends.py check X2GUEL.cpp ELESIS_KNIGHT`: 301 tables, 0 STUCK, `RESULT: PASS`. `luabind check ELESIS_KNIGHT`: `RESULT: PASS`. `skillicons check` 28205-28206 / 28222 / 28227-28228: `misses 0`, PASS.
- **Deploy:** run `COALESCED` ( another session's full build, started 01:06:03 after this request, compiled the tree with these edits ): `DEPLOY OK: X2_offline.exe is 26,485,248 B, dated 2026-11-14 01:15:49, md5 917b9e3c95b58f02d35c49878d17141e, equal to X2/US_SERVICE/x2.exe (2026-11-14 01:15:42); built at main 68ac60b4 + 4 uncommitted code file(s).` `EXESTR OK` ( md5 917b9e3c ): narrow `ESI_A_SGM_UPPER_SLASH`; wide `SGM_ProvokeReadyTM01.X`, `LD_Slicer_trace_01.dds`.
- **Save:** unit 29 ( `ElesisGM`, class 120, lv 80, not transcended; backup `db_backup/els_db_20261114_010407_pre_sql.sql` ): A 0 Provoke 28205, A 1 Drastic Charge 28206; B 5 Upper Slash 28222, B 6 Divine Blade 28040, B 7 Giga Blast 28062 ( the earlier specials are unslotted; the three actives already learned at level 1 ). Slots 4 / 8 / 9 empty. Upgrade ids not learned.
- **Play-test ( user, 2026-11-14 01:19-01:20, exe `917b9e3c` ): all passed.** Logs: equip lines 28205 slot 0 bank A state 49, 28206 slot 1 bank A state 51, 28222 slot 0 bank B state 59; `ES_CREATE` `EffectSet_A_SGM_Provoke_Damage` / `EffectSet_A_SGM_PROVOKE` ( ownerState 49 ), `EffectSet_A_SGM_UPPER_SLASH` ( 59 ), `EffectSet_A_SGM_DRASTIC_CHARGE_LOOP` ( 51, hits ), `ES_UPDATE` `EffectSet_A_SGM_DRASTIC_CHARGE_END`; no `MIG no-state` line, packets EXCEPTION 0 ( 2 `E_DISABLE_HB_CHECK_REQ` UNHANDLED ), 0 unbound Lua names, ErrorLog 0 `Motion Error`. One new ErrorLog line: `OpenXET` failed for `SGM_PROVOKEREADYTM01.XET` ( the optional companion of the new Provoke preload; same class as Scaldic Sword's, no visible effect ). The `[JUNE_CHAR] LUCIEL NEWUNIT` lines are another plan's. Log tag `[JUNE_CHAR]` ( `ELESIS SKILL` ).

### CH3b0 - Done (2026-11-14)

Opus 5.5. Flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON` now **defined** (`Always.h`; its `#error` names `_LIVE`, `_CORE`, `JUNE_CHAR_CHUNG_SKILL`, `JUNE_USER_CREATE_EFFECT_SET`,
`JUNE_SKILL_TEMPLET`, `JUNE_ENUM_NAMES`, `JUNE_BUFF_ENUM`, `SERV_ELESIS_SECOND_CLASS_CHANGE`, `BALANCE_PATCH_20131107`, `RIDING_SYSTEM`, all on by `/Zs` probe) and a temporary
`_SUMMON_DEBUG` (`[JUNE_CHAR] ELESIS SUMMON` lines; stays for CH3b / CH3c, CH3d's stage review retires it). Own review: `CH3b0-review.md`. Read from Ghidra (read-only) and
`charport_adis.py elesis 32b8b0c0`; jump tables of `3230c1a0` read from the exe. Funcmap rows `Char Elesis CH3b0` (8).

| June | Port | June differs from March |
|---|---|---|
| `32b89260` vslot 0x24c | whole inside March's (`#else` March) | June's skill id 28008 for Victorious Sword's cool-time rate; BTI 565 (enhanced) joins 239 / 326; **BTI 940 Soul Ignition** writes `m_fCreateEffectSetRate` = `SA_EFFECTIVE_RATE` of 28063 (skipped when the equipped templet is null); the **Blood Runaway saved debuff** (`+0x9738`) beside March's Fire Blossoms one |
| `32b74500` vslot 0x250 | whole inside March's | the Fire Blossom spawn on BTI 294's end only on the owner's client (`IsMyUnit`, June's `31f8b010` gate); 565 resets with 239 / 326; 940 zeroes the rate |
| `32b6fb10` | new `IsBloodRunawayCleanseReady` | riding -> false; interval by HP tier (`32b6fa50` = `GetHpTier`'s body) else 9999; ready when the timer reaches it |
| `32b79db0` | new `ESI_SI_A_SBH_SCALET_ROSE_SUCCESS_End` (unregistered) | June-only: a blossom per hit on the success state, at most 3, 50-500 ahead on the line (`GetFixedDistancePositionForWeaponTaker`), the fire effect at each spot; then the count is zeroed |
| `32b7a0a0` @32b7a3c0-32b7a4f5 | new `CreateWallOfProminenceNPC( vPos )` | June-only NPC wall: 1635, or 1971 Fire King once Blaze Absolu (`m_usBlazeAbsoluLevel`) is learned; level = hyper state |
| `32b8b0c0` @32b8b2e6-32b8b3b7 | new `CreateBloodCutterNPC()` | June-only: 1641 at her position, 0.3 s delay |
| `3230c1a0` | `X2Game.cpp` cases (March's 1567 in `#else`) | 1567 and 1641 take June's default (helper flags 0 with Scalet Rose 28211 / Blood Cutter 28023), 1635 / 1971 flag 1 (fixed HP) with 28018. **1567's related level** (`ELESIS_FIRE_BLOSSOM.lua` reads it) is now `max( 1, Scalet Rose level )` for any class, not March's Blazing-Heart-only Fire Blossoms passive level, and its stats come from her (March set none) |

Every spawn keeps March's `CreateNPCReq` argument list; June's trailing `IsMyUnit` / `0` arguments become the owner gate (as Chung CH3b0 / Rena CH2d0). No count cap: none of
the four ids is in June's summon-buff map `31fc96b0`. `CheckSummonSpecialAttack` (CH1k's call in `CanUseSpecialAttack`) goes live with the flag; its body and the 54-id
`IsSummonSkillBy` list were already June's (D0c-1) with 28018 / 28023 / 28054 / 28057. `+0x8e74` is the base writer `322ece70` (`SetSummonHelperInfo`), no class code.
- **Members** (`X2GUEl.h`, `_SUMMON`, ctor 0): `m_ucScaletRoseHitCount` (+0x9788), `m_usBloodRunawayLevel` (+0x9724), `m_fBloodRunawayCleanseIntervalTier1-3`
  (+0x9728 / +0x972c / +0x9730), `m_fBloodRunawayCleanseElapsedTime` (+0x9734), `m_eBloodRunawaySaveDebuffID` (+0x9738). Their writers are CH3b2's and CH4a's (kickoffs amended).
- **Enum names** (`JUNE_ENUM_NAMES`): `NUI_SI_SA_SBH_WALL_OF_PROMINENCE` 1635, `NUI_SI_SA_SCA_BLOOD_CUTTER` 1641, `NUI_SI_SA_SBH_WALL_OF_PROMINENCE_FIREKING` 1971
  (`X2UnitManager.h`), `BTI_BUFF_SOUL_IGNITION` 940 (`X2BuffFactor.h`), `RNSI_SA_SBH_SOUL_IGNITION` 28063 (`X2SkillTree.h`); values read back from the compiler.
- **NPC data:** core `Enum.lua` (data036) gains the three NUI names; core `NPCTemplet.lua` (data037) June's rows 1635 / 1971 / 1641 verbatim (1567 present); `luasrc
  refresh` both, `ledger --check` / `core --check` PASS. `adopt-june --dry-run` of the four scripts: identical to June. `luabind.py check ELESIS_FIRE_BLOSSOM Elesis_FireWall
  Elesis_FireWall_FireKing Elesis_BloodCutter`: 15 / 9 / 9 / 8 methods, 0 unbound, ENUM 0 missing, `RESULT: PASS`. The 36 assets they and the rows name are all present.
  `PACK OK data036` (identical to live, nothing copied), `PACK OK data037`, `COPY OK data037` md5 `8ea49b1b`.
- **Routed:** the Wall's position (target seek `32008a20`, not ported; fallback `31c68ec0`; land lift `31c7bb00`) is CH3b's FrameMove; the Scalet Rose Start reset and End
  registration CH3b2's; the hit-count delegate `32b524b0` and the Blood Runaway / Fire Blossoms passive writers CH4a's; Blood Cutter's state CH3c's. No JUNE_BUGS row.
- **Checklist:** L258 / L283 `(Elesis: CH3b0 ...)`.

**Hygiene.** `flagcheck JUNE_CHAR_ELESIS_SUMMON --with _SUMMON_DEBUG --worktree --files` the 7 code files: `3 PASS, 0 PASS~, 0 DEFINE, 4 OTHER, 0 FAIL, 0 ERROR`
(`Always.h` = the define blocks plus a peer's uncommitted DU2 block, not committed here; the three enum headers = `JUNE_ENUM_NAMES`); `0 deleted line(s), 0 non-ASCII
inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`, `RESULT: PASS`. `junediverge.py --check` (6 files):
`allow-listed: 19`, `RESULT: PASS`. `enumscan.py --check X2GUEL.cpp X2Game.cpp`: `0 A/B row(s)`. `stateends.py`: n/a (no state registered).
One-file `/Zs` probes (`census.cl_cmd`, no PCH): `X2GUEL.cpp` and `X2Game.cpp` 0 errors, no warning on the new lines. **Full build skipped: no play-test** (Soul Ignition,
Wall, Blood Cutter and Scalet Rose have no skill case until CH3b / CH3b2 / CH3c, and the Fire Blossoms buff needs CH4a's passive on a June-id save; their hand-offs carry
`[JUNE_CHAR] ELESIS SUMMON req npc ...` and `[JUNE_CHAR] SUMMON npc ...`).

### CH3b0 - Review (2026-11-14)

Opus 5.5. **Pass, no code change.** Commit `7473d4bf` read against June in Ghidra (read-only), `charport_adis.py elesis 32b8b0c0`, and the jump table read from `unpacked_x2.exe`.
- **0x24c `32b89260`:** switch = 238 `+0x95d4`, 298 `+0x96d4`, 239 / 326 / 565 Victorious Sword, 940 Soul Ignition; Victorious Sword sets 1.0, then level `31fce5c0( 28008 )`
  (= `GetSkillLevel( id, false, 0, true )`), `> 0` (JLE), then `31fca8e0( 28008, 1 )`, ability 0x6a at that level with bPvp 0 (`SA_DECREASE_REL` = 106, `SA_EFFECTIVE_RATE` = 5,
  counted in `X2SkillTree.h`, no `#if` in the range); Soul Ignition takes the templet first and skips the `@32b8930d` write on null. Both saved-debuff tails = `+0x64 == 1`,
  `+0x66 == 0`, `32369cc0` false (the Fire Blossoms one keeps March's `IsDeBuff` / `IsWorldBuff` / `IsNonControlDebuff`); Blood Runaway's = `word +0x9724 != 0` and `32b6fb10 == 1`.
- **`32b6fb10`:** riding -> false; 9999.0, tier 1 / 2 / 3 -> `+0x9728 / +0x972c / +0x9730`; `FCOMPP` (elapsed vs interval), `TEST AH,5`, `JNP` -> false only when elapsed < interval.
  That is MSVC's x87 code for `elapsed >= interval` (NaN counts as true there), so the port's `>=` compiles to June's test.
- **0x250 `32b74500`:** 298 / 238 cleared, 239 / 326 / 565 -> `+0x9610` 0 and `+0x9614` 1.0, 940 -> `+0x8e50` 0; 294: March's effect set, then `31f8b010( IsMyUnit, 0x61f, 0, 1,
  GetLandPosition (vslot 0x554), GetIsRight, 0.0, 1, -1, team, NAT_ALLY, uid, 0, -1, 0 )`; the three trailing args are March's `CreateNPCReq` defaults (`bElite`, `TN_NONE`, `NCT_NONE`).
- **`32b79db0`:** the bound `min( byte +0x9788, 3 )` is re-read each pass; `GetRandomFloat( i )` (vslot 0x1cc) x 450.0 + 50.0 in double, then stored as float; `31c60890( &GetPos( 0 ), dist,
  GetIsRight, &vPos )`; `PlayEffectSet( name, this, 0, 0, -1, -1, (1,1,1), true, &vPos, 0-vector, 0-vector, 1.0 )` (the custom position is the blossom's spot, by stack offsets);
  the 16-arg `322e67c0` call is behind `IsMyUnit`; then `+0x9788 = 0` before vslot 0x7dc.
- **Spawns:** Wall `@32b7a37d` `CMP word +0x97a4, 0 / JBE` -> 0x663 else 0x7b3; level = the dword the frame move saved from now-state `+0x10` (SETNZ); delay 0, noDrop 1, key -1.
  The helper reads `IsHyperState()` when called, which is the same state and frame as June's read before the seek. Blood Cutter: `GetPos( 0 )`, 0.3 (`3345993c`), level 0, owner gate.
- **`3230c1a0`:** 0x7b3 `JZ 3230cb83` @3230c3b9; 0x663 `JZ 3230cb83` @3230ca4b; 0x669 falls through to the default `3230ca5c`; 0x61f -> index 0x93, byte 5, `3230d060[5]` = `3230ca5c`.
  `3230cb83` = flags 1, the default flags 0, rate 1.0 each. The skills 28211 / 28018 / 28018 / 28023 = `summon_npc_skill_map.tsv` (`31fcf520`).
- `#else` paths: no deleted line in any code file (the 5 in the range are `Always.h`'s CH1k define block becoming the defined one). Widths: `unsigned char` `+0x9788`, `USHORT` `+0x9724`.
- `CH3b.md` "Amended by CH3b0" checked: it is accurate, no change.

**Hygiene.** `flagcheck JUNE_CHAR_ELESIS_SUMMON --range 7473d4bf^..7473d4bf --with _SUMMON_DEBUG`: `files: 3 PASS, 0 PASS~ (blank/marker only), 1 DEFINE, 3 OTHER (other flags), 0 FAIL,
0 ERROR`; `in this flag's files: 5 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`;
`RESULT: PASS`. OTHER = `X2BuffFactor.h` / `X2SkillTree.h` / `X2UnitManager.h` (`JUNE_ENUM_NAMES` names); DEFINE = `Always.h` (+19 -5). `git log -S` the flag after `7473d4bf`: none.
`junediverge.py --check-all`: `allow-listed: 232`, `RESULT: PASS`. `enumscan.py --check X2GUEL.cpp X2Game.cpp`: `0 A/B row(s) remain in 2 file(s) (C 0, D 1, X 3 ignored)`.
**Full build skipped: no play-test** (no code change; the spawns are reached in CH3b / CH3b2 / CH3c / CH4a). `_SUMMON_DEBUG` stays (CH3d retires it).
**Fix after the review (user asked for a build, 2026-11-14):** the full build failed in KTDXLIB on CH3b0's own `Always.h` `#error`. `SERV_ELESIS_SECOND_CLASS_CHANGE` is
defined only in `ServerDefine.h`, which KTDXLIB never sees, and CH3b0's one-file probe compiled only X2Lib files. That check moved to the top of `X2GUEL.cpp` (as Raven's
`SERV_RAVEN_VETERAN_COMMANDER`); the rest of the guard stays in `Always.h`. flagcheck `--worktree --files Always.h X2GUEL.cpp`: `1 PASS, 1 OTHER, 0 FAIL`, `RESULT: PASS`.
`DEPLOY OK: X2_offline.exe is 26,488,832 B, dated 2026-11-14 02:08:40, md5 e9dee3d698e1420fe434100dcb726b2a`.

### CH3b - Done (2026-11-14)

Sonnet 5.5. Same flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL`; `_SKILL_DEBUG` stays on until the CH3d review retires it. No review of its own ( the CH3d review covers CH3a-CH3d ).
All 24 class-map rows read from `charport_adis.py elesis --phase CH3b`, the Ghidra decompiles of `32b7ad80` / `32b63be0` / `32b7a0a0` / `32008a20` and the registration `32b8c470`: **16 written** ( 11 inside March's
functions, 5 new ) + the shared cone seek `32008a20` + the registration, **8 = March / no code** ( the four preload lists of Eternal Fire / Wall of Prominence / Blaze Wing / Blazing Dance START, START `EventProcess`,
FINISH `StateEnd`, FINISH `FrameMoveFuture`; Soul Ignition's Init is a bare `ret` ) . `funcmap.tsv` rows `Char Elesis CH3b` ( 26 ).
- **Registration:** June ids as labels on March's cases ( Eternal Fire 28015 / 28051, Blaze Wing 28017 / 28053, Wall of Prominence 28018 / 28054; Blazing Dance 28016 / 28052 and, through CH1n's memo arm, 28032 / 28061 );
  Blazing Dance's case body is June's ( START: Init, StartFuture, Start, EventProcess; hyper: StartFuture, Start, EventProcess; then the 1ST / ATTACK_BACK / ATTACK_FRONT / FINISH states ), March's is in the `#else`.
  New cases Burst Rising 28041, Ignis Crasher 28042 ( EventProcess only ) and Soul Ignition 28063 ( `GenericSpecialActiveSkillEventProcess`; June's Init is a bare `ret` ). 7 enum names in `X2SkillTree.h` ( `JUNE_ENUM_NAMES` ).
- **What June differs in:**

| June | Function | Difference |
|---|---|---|
| `32b5ee60` / `32b634f0` / `32b637f0` | Eternal Fire / Wall of Prominence / Blaze Wing `EventProcess` | listing-identical to Infernal Blade's `32b5eb60` ( diffed ): the walk / back-walk / dash / Z-X cancels need the foot on a line |
| `32b6c2d0` / `32b6c5d0` | Burst Rising / Ignis Crasher `EventProcess` ( new ) | the same template; Ignis Crasher ends in `USI_JUMP_DOWN` |
| `32b7a0a0` | Wall of Prominence `FrameMove` | the 0.454 s damage effect is gone: at the 0.01 s oneshot the cone seek `32008a20` picks the nearest enemy ( abilities `SA_SCOPE_ABS` / `_Y` / `_Z` / `SA_PROJECTILE_RANGE_REL` of the state skill, defaults 180 / 180 / 180 / 1000 ), else `GetFixedDistancePositionInAllLine` 120 ahead, else `GetPos`; land lift with `bMonster` true ( checklist 7y ); CH3b0's `CreateWallOfProminenceNPC` |
| `32008a20` | cone seek ( new shared `CX2GUUser::GetNearestTargetAimDegree( AimTargetCone, pos )` ) | `32008660`'s sibling: a block of up / down / yaw / reach, an NPC must be class BASIC unless the block's flag byte is set, out = hit position ( 9999 x3 when none ); the caster's other two June callers are `32ab4600` / `32ab4a70` |
| `32b7a520` | Blazing Dance START `StartFuture` ( new ) | hit list cleared here, last target uid -1, line range 99999 then `SA_SCOPE_ABS_Z`, search range from `SA_SCOPE_ABS` ( `_PVP` in PvP / PvE ), strike count 0 |
| `32b517a0` | START `StateStart` | keeps the start position ( `+0x96c8` ) instead of clearing the hit list |
| `32b80e90` / `32b80ee0` | 1ST ( shared with BACK / FRONT ) / FINISH `StartFuture` | strike count +1 ( max 100 ), from the 7th strike `UpNowMp( -20 )`; FINISH counts a strike only after a target was found and sets the last-attack flag |
| `32b63b20` / `32b63b80` | 1ST / FRONT and BACK `EventProcess` | `CommonEventProcess` first; no target -> FINISH; at the animation end `CanBlazingDanceContinue` ( `32b51930` ) picks BACK / FRONT or FINISH. March's 2ND / 3RD / 4TH are no longer registered |
| `32b63be0` | FINISH `EventProcess` | at the animation end the unused share of seven strikes refunds `m_fSkillStartMpConsumed x ( 7 - strikes ) x 0.1` ( checklist 7al ), `USI_JUMP_DOWN` ( y -= 30 ) off a line, else WAIT; cancels need the foot on a line |
| `32b7ad80` / `32b6f8b0` | target search / target test | March's two functions replaced: bosses of a raid and NPCs that escape lock-on skipped ( `+0x8fb6` ); in PvP / PvE the target's distance from her facing line must be within `+0x975c`; a unit on the hit list is skipped, then a listed unit that is not the last target, then the last one; reach = `+0x97b4` squared in double; the target spot is the unit's bone nearest her on the lines ( its own position when that is within 500 ); no line-type or can-down test |

- **Kept as June has it:** the yaw fold `360 - yaw` in the cone seek ( the same as `32008660` ); the target spot falls back to the unit's own position only when the bone point is within 500 ( `32b6f9ac` ); the second-chance search skips the PvP line-distance test. No `JUNE_BUGS` row: none of these is clearly unintended.
  Stand-in: a null line map in the Wall of Prominence fallback ( June dereferences it ) falls back to `GetPos`.
- **Not here, routed:** Flame Sword / Scalet Rose upgrade ids 28231 / 28232 ( CH0 tagged them CH3b ) label CH3b2's own cases ( 28210 / 28231, 28211 / 28232 ): `kickoffs/char-elesis/CH3b2.md` is amended. `m_fWallOfProminenceLifeTime` ( March's damage-effect life time ) is no longer read by June's FrameMove: CH4a's `InitPassiveSkillState` leaves it.
  `NUM use`: all seven skills are numerical-only tables read by the engine ( `MODS.md` "Stage 7 Elesis, CH3b" ). No `SetEquippedSkillLevel` work ( CH1i wrote Blazing Dance's block; the other six have none ).
- **Checklist** `(Elesis: CH3b ...)`: 7y ( `GetLandPosition` bMonster ) and 7al ( `+0x9440` read by Blazing Dance Finish ).
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_SKILL --with _SKILL_DEBUG --worktree --files ...`: `5 PASS ( Always.h +3, X2GUEL.cpp +474, X2GUEl.h +15, X2GUUser.cpp +61, X2GUUser.h +12 ), 1 OTHER ( X2SkillTree.h = JUNE_ENUM_NAMES ), 0 FAIL`, 0 deleted, 0 non-ASCII, 0 EOL-mismatched, 0 untokened, 0 encoding change; `RESULT: PASS`.
  `junediverge --check` the 5 files ( spell `X2Lib/X2GUEL.cpp`, the allow-list is case-sensitive ): `allow-listed: 17`, `RESULT: PASS`. `enumscan --check X2GUEL.cpp X2GUUser.cpp`: `0 A/B row(s) remain`.
  `stateends.py check X2GUEL.cpp ELESIS_KNIGHT`: 309 tables, 0 STUCK, `RESULT: PASS`. `luabind check ELESIS_KNIGHT`: `RESULT: PASS`. `skillicons check` 28015-28018 / 28041-28042 / 28051-28054 / 28063: `misses 0`, PASS.
- **Deploy:** `DEPLOY OK: X2_offline.exe is 26,493,440 B, dated 2026-11-14 02:51:13, md5 ae6ac23005d46b8ad1fefdf5b67d64de, equal to X2/US_SERVICE/x2.exe (2026-11-14 02:51:05); built at main 0343cf6b + 6 uncommitted code file(s).` `EXESTR OK` ( md5 ae6ac230 ): narrow `ESI_SA_SBH_BURST_RISING`, `ESI_SA_SBH_IGNIS_CRASHER`, `ESI_SA_SBH_SOUL_IGNITION`; wide `blazing dance target uid`.
- **Save:** unit 30 ( `ElesisBH`, class 121, lv 80, not transcended; backup `db_backup/els_db_<stamp>_pre_sql.sql` ): A 0 Eternal Fire 28015, A 1 Blazing Dance 28016, A 2 Blaze Wing 28017, A 3 Wall of Prominence 28018; B 5 Burst Rising 28041,
  B 6 Ignis Crasher 28042, B 7 Soul Ignition 28063 ( 28017 / 28042 / 28063 newly learned at level 1; the earlier slots are cleared; slots 4 / 8 / 9 empty ). Upgrade ids not learned.
- **Play-test: passed (2026-11-14, exe `ae6ac230`).** Logs: 7 equip lines ( states 49 / 51 / 53 / 55 bank A, 59 / 61 / 63 bank B ); `blazing dance target uid ... count 1 / 2 / 3`; `ELESIS SUMMON req npc 1635 wall ...` then `SUMMON npc 1635 uid 1000021 skill 28018 lv 1 flags 0x1`; `SBUF` state 63 BFI 995, `soul ignition level 1 effect set rate 1.000`, `ES_CREATE EffectSet_SA_SBH_Soul_Ignition`; ErrorLog 0 lines, 0 Motion Error, 0 MIG no-state, 0 EXCEPTION, 0 unbound Lua names ( 2 UNHANDLED `E_DISABLE_HB_CHECK_REQ` are old ). Log tag `[JUNE_CHAR]`.

### CH3b2 - Done (2026-11-14)

Sonnet 5.5. Same flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL`; `_SKILL_DEBUG` stays on until the CH3d review retires it. No review of its own ( the CH3d review covers CH3a-CH3d ).
All 11 class-map rows read from `charport_adis.py elesis --phase CH3b2` and the registration `32b8c470` ( cases @1138 / @1158 / @1272 ): **7 written** ( 4 inside March's functions, 3 new ), **4 = March / no code**
( Flame Sword Init and Scalet Rose Init preload lists, Scalet Rose `FrameMove` and `End`: the listing equals March's `X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE` branch ). `funcmap.tsv` rows `Char Elesis CH3b2` ( 8 ).
- **Registration:** June ids as labels on March's cases ( Flame Sword 28210 / 28231, Scalet Rose 28211 / 28232 ); Scalet Rose SUCCESS gets `StateStart` ( `32b51550` ) and `StateEnd` ( CH3b0's `32b79db0`, the thunk `32b90830` ) beside March's
  EventProcess; new case Flame Protection 28223 ( EventProcess only, hyper state = `CopyStateCallbacks` ). Flame Sword FINISH has no `StateInit` in June either. 3 enum names in `X2SkillTree.h` ( `JUNE_ENUM_NAMES` ).
- **What June differs in:**

| June | Function | Difference |
|---|---|---|
| `32b625b0` | Flame Sword `EventProcess` | hit test reads the **future** animation time ( March: the now animation ) `> 0.235` with `GetAttackSuccess( true )` -> FINISH; cancels need the foot on a line |
| `32b628f0` / `32b62ef0` | Flame Sword FINISH / Scalet Rose SUCCESS `EventProcess` | listing-identical to the Burst Rising template ( diffed against `32b5eb60`, 0 lines ): the cancels need the foot on a line |
| `32b796e0` | Scalet Rose `EventProcess` | the 0.33-0.465 s damage-effect test is March's; new at the animation end: an upgraded skill ( `IsNowStateSkillUpgraded` ) with 28232 learned and equipped gives `GetActualMPConsume( 28232, level ) x SA_MP_RETAKE_ABS ( 0x42 )` back through `UpNowMpWithText`, then WAIT; cancels need the foot on a line |
| `32b51550` | SUCCESS `StateStart` ( new ) | `CommonStateStart`, then the Fire Blossom hit count `+0x9788` = 0 |
| `32b6d1e0` | Flame Protection `EventProcess` ( new ) | the Burst Rising template ( diffed, 0 lines ) |

- **Kept as June has it:** June reads the user skill tree off `GetUnit()` with no null test before the 28232 level ( `32b797b7` ); the port does the same ( the unit exists in play ). No `JUNE_BUGS` row: nothing is clearly unintended.
- **Routed, not here:** the Scalet Rose hit-count **increment** is the attack-result delegate `32b524b0` that `InitPassiveSkillState` pushes ( CH4a ): until CH4a the count stays 0 and the End spawns no blossom. The CH3b amendment of the
  kickoff said CH3b2 writes the increments; CH3b0's amendment ( later, the same day ) and checklist 7am assign them to CH4a, which this phase follows. March's `ApplyDebuffFlameSword` / `m_uiSkillLevelFlameSword` ( the `SI_A_SBH_FLAME_SWORD` passive-object
  block in `InitPassiveSkillState` ) and June's `m_bIsFrameSword` reader are CH4a / CH3b0's. Upgrade ids 28231 / 28232 are labels only; not learned on the test save.
  `NUM use`: all three are numerical-only tables read by the engine ( `MODS.md` "Stage 7 Elesis, CH3b2" ). No `SetEquippedSkillLevel` work ( CH1i wrote the 28210 / 28211 blocks ).
- **Checklist** `(Elesis: CH3b2 ...)`: row 117 ( `NUMERICAL_DATA_INDEX`, L52 ) and 7ai ( the reserved-buff path, L876 ).
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_SKILL --with _SKILL_DEBUG --worktree --files ...`: `2 PASS ( X2GUEL.cpp +93, X2GUEl.h +2 ), 1 OTHER ( X2SkillTree.h = JUNE_ENUM_NAMES ), 0 FAIL`, 0 deleted, 0 non-ASCII, 0 EOL-mismatched, 0 untokened, 0 encoding change; `RESULT: PASS`.
  `junediverge --check` the 3 files: `allow-listed: 10`, `RESULT: PASS`. `enumscan --check X2GUEL.cpp`: `0 A/B row(s) remain`. `stateends.py check X2GUEL.cpp ELESIS_KNIGHT`: 311 tables, 0 STUCK, `RESULT: PASS`.
  `luabind check ELESIS_KNIGHT`: `RESULT: PASS`. `skillicons check 28210-28232`: `misses 0`, PASS ( 1 known June gap, Avatar of Fire's Enhanced icon ).
- **Deploy:** `DEPLOY OK: X2_offline.exe is 26,494,464 B, dated 2026-11-14 04:56:19, md5 871680f00b62d9471c85810fdf2d2d90, equal to X2/US_SERVICE/x2.exe (2026-11-14 04:56:09); built at main 0d103817 + 3 uncommitted code file(s).`
  `EXESTR OK: 1 of 1 found` ( md5 871680f0 ): narrow `ESI_A_SBH_FLAME_PROTECTION`.
- **Save:** unit 30 ( `ElesisBH`, class 121, lv 80, not transcended; backup `db_backup/els_db_20261114_044735_pre_sql.sql` ): A 0 Flame Sword 28210, A 1 Scalet Rose 28211, A 2 Flame Protection 28223 ( all learned at level 1 already );
  the CH3b skills are unslotted ( learned ), slots 3-9 empty.
- **Play-test: passed (2026-11-14, exe `871680f0`).** Logs: 3 equip lines ( states 49 / 51 / 53 ); `SBUF` state 49 BFI 299 ( 3 casts ) and state 53 BFI 928; `ES_CREATE EffectSet_A_SBH_FLAME_SWORD_Finish` ( state 223 ), `EffectSet_A_SBH_Scalet_Rose_Pulling` ( success state 226, twice ), `EffectSet_BUFF_A_SBH_FLAME_PROTECTION` + `EffectSet_A_SBH_FLAME_PROTECTION` ( state 53 ); ErrorLog 0 Elesis lines, 0 Motion Error, 0 unbound Lua names ( 2 old UNHANDLED `E_DISABLE_HB_CHECK_REQ` ). Log tag `[JUNE_CHAR]`.

### CH3c - Done (2026-11-14)

Sonnet 5.5. Same flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL`; `_SKILL_DEBUG` stays on until the CH3d review retires it. No review of its own ( the CH3d review covers CH3a-CH3d, and CH3c2 ).
Class-map rows read from `charport_adis.py elesis --phase CH3c`, Ghidra `FUN_32b84390` and the registration `32b8c470`: **29 functions written** ( all new ), **Shadow Edge not ported** ( 3 of the 36 rows + 4 vector helpers ): split to **CH3c2 ( Opus )**, `kickoffs/char-elesis/CH3c2.md`.
`funcmap.tsv` rows `Char Elesis CH3c` ( 30 ). New cases in `SetEquippedSkillFuncBySkillSlotIndex`: Brandish Breaker 28022 / 28056, Blood Cutter 28023 / 28057, Shade the Illusion 28024 / 28058, Dread Weapon 28025 / 28059, Chasing Sword 28043, Shade 28044, Blood Falls 28064, Assault Strike 28216 / 28236, Blood Sting 28224.

| June | `CX2GUEL::` | What it does |
|---|---|---|
| `32b737a0` `32b73940` `32b682f0` `32b51c50` `32b8a320` `32b68330` | Brandish Breaker start / loop / finish | start: count +0x96f8 = 0; both starts scale the state power rate by 1 + 0.3 x ( max HP - HP ) / max( max HP, 1 ); start goes to the loop while any skill key is held ( `IsAnySkillKeyPressed` ), else the finish; loop: a tick every 0.15 s makes a LOOP_DUMMY ( memo: MEMO_LOOP_DUMMY + BFI 594 at tick 1 ) effect that only pays `ApplyBleedingHpCost( 1/60, 1 )`, shock debuff time cleared, ends at count 6 or no key held |
| `32b51e60` `32b51e80` `32b68cd0` `32b68d00` `32b68d40` `32b8a9d0` | Dread Weapon start / loop / end | pass count +0x96fc; the loop repeats until 4 passes; the end needs foot-on-line to land ( `USI_JUMP_LANDING` ); start 0.3 s: dummy effect, bleeding cost, effect set `EffectSet_SI_SA_SCA_DREAD_WEAPON_Loop` replaces the one in +0x9710 ( rate x1.3 PvP / PvE, else x1.4 ) |
| `32b8bd70` `32b8c0f0` `32b8ad50` | Assault Strike / Blood Falls / Shade the Illusion FrameMove | the Dread Weapon body at 0.4 s ( diffed: constants and strings only ); Shade the Illusion keeps no handle and then puts the skill's buff factor on herself |
| `32b8b0c0` | Blood Cutter FrameMove | 0.01 s: dummy effect, bleeding cost raises the state power rate ( +0x7cc8 ) x1.3 / x1.4, then CH3b0's `CreateBloodCutterNPC` |
| 5 Inits, 11 EventProcess | preloads; templates | 3 = Cross Slash template ( 0 lines diffed ), 5 = Shade Scratch attack template ( 0 lines ), Dread End / Brandish start / loop / Dread start / loop own shapes |

- **Kept as June has it:** the Brandish loop's missing CommonFrameMove past count 6 ( `JUNE_BUGS` 657 ); a dead `322cc1a0` call after the dummy effect ( dropped ); June's `GetEqippedSkillMemo` reads false ( `DIVERGE_SKILL_MEMO` ), so the memo branch ( 608111, no Enum entry ) is written but unreachable. Blood Falls' Init `32b52550` is a bare ret: not registered.
- **Names:** 10 `RNSI_*` in `X2SkillTree.h`, `BFI_BUFF_BRANDISH_BREAKER_MEMO` 594 in `X2BuffFactor.h` ( `JUNE_ENUM_NAMES` ). Members `m_usBrandishBreakerLoopCount` ( +0x96f8 ), `m_usDreadWeaponLoopCount` ( +0x96fc ), `m_hCrimsonAvengerSkillEffectSet` ( +0x9710 ).
- **Checklist** `(Elesis: CH3c ...)`: row 117 ( L52: the Blood Sting and Chasing Sword tables ). `NUM use`: all numerical-only or script tables ( `MODS.md` "Stage 7 Elesis, CH3c" ). No `SetEquippedSkillLevel` work ( CH1i wrote the Brandish / Dread blocks ).
- **Routed:** Shadow Edge -> CH3c2 ( the 0x20-byte record, `RemoveShadowEdgeRecord`, upkeep `32b813d0` go with it; CH4c reads them ). Blood lines that need CH4c pieces: none here.
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_SKILL --with _SKILL_DEBUG --worktree --files ...`: `2 PASS ( X2GUEL.cpp +588, X2GUEl.h +32 ), 2 OTHER ( X2SkillTree.h, X2BuffFactor.h = JUNE_ENUM_NAMES ), 0 FAIL`, 0 deleted, 0 non-ASCII, 0 EOL-mismatched, 0 untokened, 0 encoding change; `RESULT: PASS`.
  `junediverge --check` the 4 files: `allow-listed: 10`, `RESULT: PASS`. `enumscan --check X2GUEL.cpp`: `0 A/B row(s) remain`. `stateends.py check X2GUEL.cpp ELESIS_KNIGHT`: 333 tables, 0 STUCK, `RESULT: PASS`. `luabind check ELESIS_KNIGHT`: `RESULT: PASS`. `skillicons check 28022-28064` and `28215-28236`: `misses 0`, PASS ( 1 known June gap ).
- **Deploy:** `DEPLOY OK: X2_offline.exe is 26,536,960 B, dated 2026-11-14 05:44:54, md5 af5a6da4666f0f6c343c9560059a11a3, equal to X2/US_SERVICE/x2.exe (2026-11-14 05:44:46); built at main f755b4d2 + 4 uncommitted code file(s).`
  `EXESTR OK: 4 of 4 found` ( wide `DamageEffect_SI_SA_SCA_DREAD_WEAPON_Loop_DUMMY`, `DamageEffect_SA_SDK_BRANDISH_BREAKER_LOOP_DUMMY`, `EffectSet_SA_SCA_Blood_Falls`, `DamageEffect_SI_A_SCA_ASSAULT_STRIKE_DUMMY` ) and `4 of 4` ( narrow `SI_SA_SCA_DREAD_WEAPON_START`, `SI_SA_SDK_BRANDISH_BREAKER_START`, `ESI_A_SCA_BLOOD_STING`, `ESI_SA_SCA_BLOOD_FALLS` ), md5 af5a6da4.
- **Save** ( backup `db_backup/els_db_20261114_053542_pre_sql.sql` ): unit 146 `CAvenger80` ( 125, lv 80, not transcended ): A 0 Brandish Breaker 28022, A 1 Blood Cutter 28023, A 2 Dread Weapon 28025, A 3 Shade the Illusion 28024 ( added ), B 5 Assault Strike 28216, B 6 Blood Falls 28064 ( added ), B 7 Chasing Sword 28043, B 8 Blood Sting 28224; slots 4 / 9 empty. Unit 145 `CAvenger40`: Shade 28044 ( added ) in B 5.
- **Play-test: passed (2026-11-14, exe `af5a6da4`, run 06:08-06:13).** Logs: 8 equip lines on unit 146 ( states 49 / 51 / 53 / 55 / 59 / 61 / 63 / 65 ); Blood Cutter `ELESIS SUMMON req npc 1641 blood cutter mine 1` + `SUMMON npc 1641 ... skill 28023` x4; `ES_CREATE` `EffectSet_SI_SA_SCA_DREAD_WEAPON_Loop` ( state 53 / 54 ), `..._SHADE_THE_ILLUSION_Attack` ( 55 / 56 ), `EffectSet_SI_A_SCA_ASSAULT_STRIKE_Attack` ( 59 / 60 ), `EffectSet_SA_SCA_Blood_Falls` ( 62 ); ErrorLog 0 lines, 0 Motion Error / BLANK / EXCEPTION, 0 unbound Lua names ( 3 old UNHANDLED `E_DISABLE_HB_CHECK_REQ` ).
  Shade 28044 ( unit 145 ) reached the select screen's slot list only: no equip line, so its registration is judged by the user's pass, not by a log line; Brandish Breaker, Chasing Sword and Blood Sting write no effect-set line ( equip lines for all three are present ).

### CH3c2 - Done (2026-11-14)

Opus 5.5. Flags `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL` ( + `_SKILL_DEBUG`, stays on until the CH3d review ) and new `SERV_IRUHADEV_JUNE_GAME_FRONT_NEAREST_ENEMY` ( class-agnostic ). No review of its own ( the CH3d review covers CH3a-CH3d and CH3c2 ).
**Finding:** Shadow Edge is June's copy of Add's March Reverse Reactor ( `CX2GUAdd::ANSI_SA_ALP_REVERSE_REACTOR_FrameMove` / `ProcessReverseReactorTargetInfo` ): same 0.31 s, Bip01 +25 / +100, 1200 link range, and June's 0x20-byte record is Add's `ReverseReactorTargetInfo` layout. June made it a list of up to 3 marks, a UID skip set, a cloned templet buff in place of `BFI_DEBUFF_PLASMA_LINK`, and a heal tick in place of Add's damage transfer.

| June | March | What it does |
|---|---|---|
| `32b84390` | `SI_A_SCA_SHADOW_EDGE_FrameMove` | 0.31 s: `ClearShadowEdgeRecords`; up to 3 x target search ( 700 in PvP / PvE by `322cc1a0`, else 1200 ); explosion `DamageEffect_SCA_SHADOW_EDGE_Projectile_EXPLOSION` at the target, `DAMAGE_EFFECT_SCA_SHADOW_EDGE_Link` from her, aimed; record ( max HP x0.1 in `GT_PVP` only, `SA_EFFECTIVE_TIME` ); buff factor [0] cloned at max( 1, skill level ) onto the target |
| `32b520f0` / `32b6a340` | `_Init` / `_EventProcess` | 3 preloads; the Shade Scratch template ( 0 lines diffed against `32b69040` ) |
| `32b81220` / `32b81300` | `ShadowEdgeRecord::GetTargetUnit` / `ResetShadowEdgeRecord` | find by UID + NPC-ness; reset zeroes time / rate, erases `BTI_DEBUFF_SHADOW_EDGE` 432, destroys the Link |
| `32b813d0` | `ShadowEdgeRecordFrameMove` | countdown; end on time, target lost / dead or > 1200; re-aim; once a second `UpNowHp( SA_ATK_PHYSIC_REL x GetPhysicAttackValue )` ( `+0x4424` = physic attack, `statcalc/gfd_blocks.tsv` ) |
| `32b81900` tail | `ClearShadowEdgeRecords` | reset each, clear ( ReInit and the FrameMove ) |
| `322d7b30` | `CX2Game::GetFrontNearestEnemyUnit` ( new, own flag ) | UID set, `IsValidUnit`, team, `GUSI_DIE`, HP, slot 94 invincible with attack type -1 ( NPC `GetInvincibleByAttackTypeId` ), `IsFront` ( `322ce900` = March ), NPC `NCT_BASIC` + `( GetOrgUnitSize x + y ) / 4` offset ( Add's; June `+0x3e30 / +0x3e34` ), keeps distance <= range |
| `31f75980` | = March `CEffect::SetLinkMainEffectByPos` | same body ( 99999 sentinel, `LINK_EFFECT_RATE` 530 for this Link ); no port |

- **Decision ( kickoff ):** the record, list, reset, clear and upkeep are written here under `_SKILL` ( they are Shadow Edge's own ). CH1h / CH1j's `_BLOOD` declarations and the two `_BLOOD` call sites ( `CommonFrameMove`, `ReInit` ) are kept, insert-only, in the `#else` of new `_SKILL` blocks. CH4c told ( `kickoffs/char-elesis/CH4c.md` amended ); Lu/Ciel CH1l30 told ( `322d7b30` ported, `31f75980` = March ).
- **Registration:** `32b8c470` @`32b8e4c9` cases 28215 / 28235: Init + FrameMove + EventProcess, the hyper state copies FrameMove + EventProcess ( struct slots 0x18 / 0x38 / 0x48 read in the listing ). No `SetEquippedSkillLevel` block.
- **Kept as June has it:** the upkeep returns after the first removal ( `JUNE_BUGS` 658 ); no null test on the templet or its buff vector; the record's max-HP field is written and never read; the FrameMove returns before `CommonFrameMove` when there is no game.
- **Names:** `RNSI_A_SCA_SHADOW_EDGE` 28215 / `_UPGRADE` 28235 ( `X2SkillTree.h` ), `BTI_DEBUFF_SHADOW_EDGE` 432 ( `X2BuffFactor.h` ), `JUNE_ENUM_NAMES`. Attack type -1 has no `ATTACK_TYPE` member ( constant with a comment ). `funcmap.tsv` rows `Char Elesis CH3c2` ( 3 updated, 7 added ).
- **Hygiene:** `flagcheck JUNE_CHAR_ELESIS_SKILL --with _SKILL_DEBUG --worktree --files` ( 7 files ): `2 PASS ( X2GUEL.cpp +235, X2GUEl.h +34 ), 5 OTHER, 0 FAIL`, 0 deleted, `RESULT: PASS`; `JUNE_GAME_FRONT_NEAREST_ENEMY`: `2 PASS ( X2Game.cpp +48, X2Game.h +3 ), 0 FAIL, RESULT: PASS`; `JUNE_ENUM_NAMES`: `2 PASS, RESULT: PASS`. `junediverge --check` 6 files: `allow-listed: 20`, `RESULT: PASS` ( new allow row: the max-HP share is `GT_PVP` only, June `@32b844df` ). `enumscan --check`: 0 A/B rows. `stateends`: 335 tables, 0 STUCK, PASS. `luabind check ELESIS_KNIGHT`: PASS. `skillicons check 28215-28236`: misses 0, PASS. /Zs probes of X2Game.cpp / X2GUEL.cpp: 0 errors.
- **Build note:** a first run failed in KTDXLIB on my own `#error` guard ( a needed flag is not visible from KTDXLIB ); the guard was removed, X2Lib's compile is the dependency proof.
- **Deploy:** `DEPLOY OK: X2_offline.exe is 26,548,736 B, dated 2026-11-14 06:50:30, md5 1cf32172859e1dffd879a1f87b249f64, equal to X2/US_SERVICE/x2.exe (2026-11-14 06:50:23); built at main 633d4817 + 7 uncommitted code file(s).`
  `EXESTR OK: 3 of 3` ( wide `DamageEffect_SCA_SHADOW_EDGE_Projectile_EXPLOSION`, `DAMAGE_EFFECT_SCA_SHADOW_EDGE_Link`, the `shadow edge cast marks` log ) and `1 of 1` ( narrow `SI_A_SCA_SHADOW_EDGE` ), md5 1cf32172.
- **Save** ( backup `db_backup/els_db_20261114_063955_pre_sql.sql` ): unit 146 `CAvenger80`: Shadow Edge 28215 ( level 1 ) into A 0, Brandish Breaker 28022 unslotted; the rest as CH3c.
- **Play-test: passed (2026-11-14, exe `1cf32172`, run 07:08:56-07:10:00).** Logs: equip `skill 28215 slot 0 bank A state 49`; 5 casts: `cast marks 2`, `3`, `3`, `0` ( back turned ), `3`, each `mark N uid ... npc 1 time 8.00 level 1`; `heal 773.3` per mark once a second ( first tick on the first frame, 8 ticks over 8 s: June's floor math ); `mark end ... found 1` on target loss and on every recast ( clear before the new marks ); the last cast ran to `time 0.99` and the client closed 0.7 s before its expiry, so the time-out end is not in this log. ErrorLog 0 lines, 0 Motion Error / BLANK / EXCEPTION, 0 unbound Lua names; `JUNE_FX unsupported APPLIED_SKILL_ID` on the explosion = the known applied-skill-ID engine gap, not this phase ( 2 old UNHANDLED `E_DISABLE_HB_CHECK_REQ` ).

### CH3d - Done (2026-11-14)

Opus 5.5. Flags `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL` ( + `_SKILL_DEBUG`, the CH3d review retires it ) and new `SERV_IRUHADEV_JUNE_CHAR_ELESIS_TRANSCEND` ( `#error` without `_ELESIS_LIVE` or `_ARA_TRANSCEND`, the chains it extends ). `MODS.md` "Stage 7 Elesis, CH3d"; `funcmap.tsv` rows `Char Elesis CH3d` ( 7 added ); checklist rows marked `(Elesis: CH3d ...)` ( 7t l.600 / 604 / 616, 7ae l.733, 7ao l.972 / 976 / 996 ). No JUNE_BUGS row, no divergent function. Review: `CH3d-review.md` ( the CH3 stage review, CH3a-CH3d ).

**Hyper actives ( the 7 class-map functions, `charport_adis.py elesis --phase CH3d`, and the registration `32b8c470` @`32b8c84d` / @`32b8e947`-@`32b8ea20` ).**
- **June's split.** `id > 28600` -> jump table `32b8ecd0` ( 28601 / 28604 -> @`32b8e991` Ancient Fire, 28602 / 28605 -> @`32b8e9ce` Fate Smasher, 28603 -> @`32b8e95c` ), `== 28600` -> @`32b8e95c` Sword of Relics; any other id above 28600 returns ( @`32b8ea89` ). MSVC's tree for one switch: the ids go into March's switch as cases under `_SKILL` ( CH1n's comment at the type `break` kept ). Struct slots read off the tails: +0x14 / +0x18 Init, +0x34 / +0x38 FrameMove, +0x44 / +0x48 EventProcess ( object / function pairs ).
- **Sword of Relics** ( tail @`32b8c8ac` ): normal Init + EventProcess, the hyper state copies **only** the EventProcess ( March also copied the Init: dropped under `_SKILL`, March's in `#else` ). Init `32b50ef0` = March ( 31 preloads, same order ). EventProcess `32b61050`: June's walk / back-walk / dash / Z-X cancels test `IsOnSomethingFuture()` ( +0x7e1c ): the `_ON_LINE_` macros, March's in `#else`.
- **Ancient Fire** ( tail @`32b8c90b` = March's case: Init + FrameMove + EventProcess, hyper copies FrameMove + EventProcess ). Init `32b51800` = March ( 17 ). EventProcess `32b63f90` = the same listing as `32b61050` ( normalised diff empty ), same change. FrameMove `32b7a7a0`: new in June, `GetEquippedActiveSkillTemplet( RNSI_HA_SBH_ANCIENT_FIRE, true )` with `m_iDeleteSkillIDByUpgrade` ( +0x178 ) != 0 picks `DAMAGE_EFFECT_SA_SBH_ANCIENT_FIRE_UPGRADE_Attack01_Pvp / _Dungeon` ( both rows in data036 `DamageEffect.lua` ); the rest ( 0.887 / 2.177 s, PvP by `g_pMain` state 9 = `XS_PVP_GAME`, render block, `+0x96e8` = 2.4 ) = March.
- **Fate Smasher, new ( Crimson Avenger 125 ):** `SI_HA_SCA_FATE_SMASHER_Init` ( `32b52160`, 14 preloads ) and `_EventProcess` ( `32b6a980`, the same listing as `32b61050` ); table `SI_HA_SCA_FATE_SMASHER` ( bare `SI_`, June script l.18510 ); hyper copies the EventProcess ( tail @`32b8e9fe` ). Effect sets / damage effects present in data036 ( `EffectSet.lua` 67 hits, `DamageEffect.lua` 8 ).
- **Names:** `X2SkillTree.h` ( `JUNE_ENUM_NAMES` ) `RNSI_HA_SGM_SWORD_OF_RELICS` .. `RNSI_HA_SCA_FATE_SMASHER_UPGRADE` 28600-28605 from June's `Enum.src.lua` ( 15395 / 14385 / 14958 / 14488 / 15237 / 14331 ).
- **7t:** the 3 `SHOW_HYPER_CUT_IN_TIME` tables are June's adopted script ( `ELESIS_KNIGHT.src.lua`, 3 hits ), read by the engine since CH1; the type-13 cut-in is the engine's. No code.

**Transcendence rows (7ao).**
- Allow-list: `TranscendCheck` takes 120 / 121 / 125 ( a brace-less guard after Ara's, `Handlers_Quest.cpp:1442` ).
- Pictures ( `GetCharacterImageName`, `X2Data.cpp` ), from June's eight per-type initializers ( subagent, Ghidra read-only; spot-checked `@33377c56` `TR_G_MASTER` @`3345cf64` -> `Texture101_NEW` @`3345e208`, `@33372a0b` `TR_G_MASTER` -> `Texture102_NEW` @`3345cf20` ): prefixes `G_MASTER` / `B_HEART` / `C_AVENGER`. **Her keys differ from Ara's:** large = bare `TR_<class>` ( Texture101_NEW ), gray `TR_<class>_GRAY` ( Texture101_GRAY ), gauge = the bare key in **Texture102_NEW** ( June pieces `EL_S*_FACE` ); `_MEDIUM` / `_ICON` / `_PARTY` Texture100_NEW and `list_TR_<class>` `GUI_character_01` ( + `_gray` ) as the other classes. One override block after Rena's. Every piece is in the staged TETs ( data215 Texture101_NEW / _GRAY / 102_NEW / 100_NEW, data057 / datai021 `GUI_character_01` ). Evidence: scratch `ctx.txt` / `dis_*.txt` ( not committed ).
- `LevelUpEvent.lua` ( data036, BOM source ) gets `TYPE7` = June's `TYPE7` ( `LevelUpEvent.src.lua:581-596`: LEVEL 70, `UC_ELESIS_GRAND_MASTER` / `_BLAZING_HEART` / `_CRIMSON_AVENGER`, `DLG_Transcendental_Change_Guide_Elesis.lua`, `ElesisVoice_ClassUpReady01 / 02.ogg` ( data145 ) ). `luasrc.py refresh`, `LEDGER CHECK PASS: 0 FAIL, 0 WARN`; `PACK OK data036` ( `~1 changed: LevelUpEvent.lua` ); `COPY OK data036` md5 e21ff52b.
- Already June's, verified: the guide dialog ( `adopt-june --dry-run data034`: identical ); `POPUP_TR_G_MASTER / _B_HEART / _C_AVENGER` ( data215; no C++ reader in June either ); `HQ_Cutin_EGM / EBH / ECA_Transcendence.gfx` ( data195; June has no `_1_` variants for her ), named by CH5's `UnitTemplet.lua` rows; `GetHyperModeTableName` 120 / 121 / 125 ( `X2GUUser.cpp` `ESI_EGM / EBH / ECA_HYPER_MODE` ); 18 `SPRT_HYPER_CLASS` ids ( 28039-28044, 28062-28064, 28222-28224, 28415-28420 ) and 3 `TRANSCENDENTAL_SKILL_DATA` rows ( `NewSkillTempletVer2.lua` / `SkillCommandDescriptionData.lua` identical to June's ); the hyper effect start's transcended return ( CH1h, `HyperModeBuffEffectStart` = June `32b74300` ). Four base helpers: CX2GUUser's, not copied. Hyper-class passives 28416 / 28418 / 28420 are CH4a's ( not learned on the play-test units ).
- **Skill icon gate:** `skillicons.py check 28000-28999`: `skills 129, misses 0, known June gaps 2, sheets 0`, `RESULT: PASS` ( 28208 / 28229 Avatar of Fire screenshot ).

**Gates.**
- `flagcheck JUNE_CHAR_ELESIS_SKILL --with ..._SKILL_DEBUG --worktree --files X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h X2Lib/X2SkillTree.h`: `2 PASS, 0 PASS~, 0 DEFINE, 1 OTHER ( X2SkillTree.h = JUNE_ENUM_NAMES ), 0 FAIL, 0 ERROR`; `0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`, `RESULT: PASS`.
- `flagcheck JUNE_CHAR_ELESIS_TRANSCEND --worktree --files KTDXLIB/Always.h X2Lib/X2Data.cpp X2Lib/Offline/Handlers_Quest.cpp`: `2 PASS, 0 PASS~, 1 DEFINE, 0 OTHER, 0 FAIL, 0 ERROR`; `0 deleted line(s)`, the rest 0, `RESULT: PASS`.
- `junediverge.py --check` ( the 6 files ): `allow-listed: 11`, `RESULT: PASS`. `enumscan.py --check` the three `.cpp`: 4 A rows, all pre-existing ( `X2Data.cpp` 3209-3211, `Handlers_Quest.cpp:1421` ); `X2GUEL.cpp` 0. `luabind.py check ELESIS_KNIGHT`: `RESULT: PASS`. `stateends.py check X2Lib/X2GUEL.cpp ELESIS_KNIGHT`: `337 registered table(s), 312 with OnEventProcess, 0 STUCK`, `RESULT: PASS`.
- `DEPLOY OK: X2_offline.exe is 26,556,928 B, dated 2026-11-14 07:36:27, md5 864d37cd28a4c775153ba7e6c78ad615, equal to X2/US_SERVICE/x2.exe (2026-11-14 07:36:19); built at main 7b9b4d27 + 6 uncommitted code file(s).`
- `EXESTR OK: 1 of 1` ( narrow `SI_HA_SCA_FATE_SMASHER` ) and `5 of 5` ( wide `DAMAGE_EFFECT_SA_SBH_ANCIENT_FIRE_UPGRADE_Attack01_Pvp`, `DLG_UI_Common_Texture102_NEW.tga`, `G_MASTER`, `C_AVENGER`, `SCA_FATE_SMASHER_DUMMY01.X` ), md5 864d37cd.

**Save** ( backup by `elsdb.py x`, `db_backup/` ): units 29 `ElesisGM` ( 120 ), 30 `ElesisBH` ( 121 ), 146 `CAvenger80` ( 125 ), lv 80, transcendence 1; learned 28603 ( 29 ), 28604 ( 30 ), 28044 / 28602 ( 146 ). Slots ( old slots cleared ): A1-A4 = 28039 / 28040 / 28062 / 28205, 28041 / 28042 / 28063 / 28210, 28043 / 28044 / 28064 / 28215; A-5 ( slot 4 ) = 28603 / 28604 / 28602; B1-B4 = 28206 / 28008 / 28011 / 28010, 28211 / 28012 / 28016 / 28013, 28216 / 28023 / 28025 / 28022; B-5 ( slot 9 ) = 28222 / 28223 / 28224. Not exercised: the base ids 28600 / 28601 and 28605 ( the same cases ), the level-70 guide popup ( CH6 walks the transition on un-transcended units ).
- Post-commit ( `d6612f76` ): `flagcheck JUNE_CHAR_ELESIS_SKILL --with ..._SKILL_DEBUG --range d6612f76^..d6612f76`: `2 PASS, 0 DEFINE, 4 OTHER ( X2SkillTree.h JUNE_ENUM_NAMES; Always.h / X2Data.cpp / Handlers_Quest.cpp = _ELESIS_TRANSCEND ), 0 FAIL`, `0 deleted line(s)`, `RESULT: PASS`; `_ELESIS_TRANSCEND --range` same commit, `--files` the three: `2 PASS, 1 DEFINE, 0 OTHER, 0 FAIL`, `0 deleted line(s)`, `RESULT: PASS`.
- **Play-test: passed (2026-11-14, exe `864d37cd`, run 07:39:30-07:44:02; user: "all test passed").** `[JUNE_TRANSCEND] UNIT 29 / 30 / 146 transcendence 1` ( select and room ). Equip lines for all 30 slotted ids, 28603 / 28604 / 28602 `slot 4 bank A state 57`, 28222 / 28223 / 28224 `slot 4 bank B state 67`. Casts: `EffectSet_HA_SGM_Sword_of_Relics_Attackbox` ownerState 58 ( 28603 via the hyper table ); `HIT DAMAGE_EFFECT_SA_SBH_ANCIENT_FIRE_UPGRADE_Attack01_Dungeon` ( the new enhanced branch ) + Attack02 / 03; `ES_CREATE EffectSet_HA_SCA_FATE_SMASHER_Attack4` ownerState 58 and its finish hit; B-5 Upper Slash state 68, Flame Protection `SBUF ... state 68 BFI 928`. `IMAGE` rows: type 9 `list_TR_G_MASTER / B_HEART / C_AVENGER`, type 2 `Texture101_NEW TR_*`, type 7 `Texture102_NEW TR_*`, type 5 `Texture100_NEW TR_*_ICON`. Awakening on each: `ELESIS UI hyper cut-in ... HQ_Cutin_EGM / EBH / ECA_Transcendence` and `ENGINE awakening: BFI 845`. 0 EXCEPTION, 0 unbound Lua names, no `BLANK`. Not CH3d's: 4 UNHANDLED `E_DISABLE_HB_CHECK_REQ` ( old ), `JUNE_FX unsupported APPLIED_SKILL_ID` ( known engine gap ), and 4 State.log `NPC Motion Error` on `Elesis_BloodCutter.lua` ( `EM_Stone_Wall_Wait / _Broken`, CH3b0 / CH3c's summon: handed to the CH3d review ). Not in this run's lines: the gray / party pictures ( the user's visual pass covers them ).

### CH3d - Review (2026-11-14, Opus 5.5; the CH3 stage review: covers CH3a, CH3a2, CH3b, CH3b2, CH3c, CH3c2, CH3d)

**Verdict: pass, no code fix.** The one code change is retiring `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL_DEBUG` ( `KTDXLIB/Always.h`, `//#define`; the guarded lines stay ). `_SKILL` and `_ELESIS_TRANSCEND` stay on. One June data bug found and kept ( JUNE_BUGS 661 ). Same session as CH3d ( user: "continue" ).

**Spot-checks against the June exe** ( `charport_adis.py elesis <va>`; `charport_consts.read_va` for the table ), all equal to the port:
- CH3a: Juggernaut Buster start EventProcess `32b6f550` ( animation end -> loop 257; on a line speed y 0; x = run-jump rate x dash speed x 0.75 @`32b6f5b4`; right / left key against facing negates; **no key -> x 0** ( the FPU's leftover zero stored @`32b6f600` ) = the port's `else` ).
- CH3b: Blazing Dance finish EventProcess `32b63be0` ( no target -> count - 1; clamp 0..7; refund = MP spent ( +0x9440, then 0 ) x ( 7 - count ) x double 0.1 through `UpNowMpWithText`; off a line -> jump down, y -= 30 = `LINE_RADIUS`, else wait; on-line cancels ).
- CH3c: Brandish Breaker loop FrameMove `32b8a320` ( protected float +0x2480 zeroed = `m_EnchantShock.m_fTime`; count `> 6` skips everything incl. CommonFrameMove ( JUNE_BUGS 657 ); tick = int( state time / 0.15 ); memo 608111 ( 0x9476f ) arm with BFI 594 at count 1, June's ( BFI, creator 0, level 1, 0, 0 ) = March's ( BFI, 1, NULL, 0, false ) order ).
- CH3d: hyper jump table `32b8ecd0` = { @`32b8e991`, @`32b8e9ce`, @`32b8e95c`, @`32b8e991`, @`32b8e9ce` } for 28601-28605 ( read from the exe; CH1n's note held ); Ancient Fire FrameMove `32b7a7a0` ( 0.887 s, `( 28601, true )` + `m_iDeleteSkillIDByUpgrade`, 2.177 s block ); Sword of Relics tail @`32b8c8ac` copies +0x44 / +0x48 only.
- CH3a2 / CH3c2 bodies were reviewed through the stage hygiene and their own play-tests; their Done notes' constants re-read from the digest ( Provoke 0.35 s / 10 MP, Shadow Edge 0.31 s / 700 / 1200 / 3 ) match the constants namespaces `_CONST_EL_CH3A2_` / `_CONST_EL_CH3C2_`.

**Blood Cutter `NPC Motion Error` ( CH3d play-test, State.log ).** June's own data: `Elesis_BloodCutter.lua` ( data037, = June's bytes ) is a copy of `Elesis_FireWall.lua` with `MOTION_FILE_NAME = "BLOOD_CUTTER_Weapon.X"` but the wall's `EM_Stone_Wall_Wait / _Damage / _Broken` animations; `BLOOD_CUTTER_Weapon.X` ( data142, = June's bytes ) has one animation set, `Normal` ( re-read here with `fxdata.xfile_text` ). No staging or code fix; June assets stay byte-exact. JUNE_BUGS 661. Earlier notes' "0 Motion Error" read ErrorLog only; this line is in State.log.

**Debug defines.** `_SKILL_DEBUG` retired now. `SERV_IRUHADEV_JUNE_CHAR_ELESIS_DEBUG` kept: its `parse` / `frame` lines showed ( CH2a, CH2c, CH3b, CH3b2, CH3d ), `at once` and `state end` never did ( asked once in this hand-off ). `_SUMMON_DEBUG` kept: formats `req npc ... scalet rose` and `... fire blossoms end` cannot fire until CH4a's hit-count delegate. Both move to the CH4 stage review ( `CH4a.md` / `CH4c.md` amended ).

**CH3d shared files.** `X2SkillTree.h`: 6 `RNSI_HA_*` 28600-28605, values = June's `Enum.src.lua`. Pass. `X2Data.cpp`: prefixes and the bare-key override after Rena's; the gauge / large / gray rows re-read from the subagent's `dis_3336f3e0.txt` / `dis_33374620.txt` ( `@33372a0b` -> `3345cf20` Texture102_NEW, `@33377c56` -> `3345e208` Texture101_NEW ). Pass. `Handlers_Quest.cpp`: brace-less guard after Ara's, chains into the Aisha / Eve test. Pass. `Always.h`: `_ELESIS_TRANSCEND` block with its `#error`. Pass. `LevelUpEvent.lua` TYPE7 = `LevelUpEvent.src.lua:581-596`. Pass.

**Hygiene.**
- `flagcheck JUNE_CHAR_ELESIS_SKILL --with ..._SKILL_DEBUG --range <c>^..<c>` per commit ( 1df5565c, e36b59dc, 72bbe355, 0747d366, 17129609, bb53f0ee, b24fdc8f, d6612f76 ): all `RESULT: PASS`, 0 FAIL / ERROR; one deleted line ( 1df5565c: CH3a turned the `//#define` back on, a DEFINE row ); OTHER rows = `JUNE_ENUM_NAMES` ( `X2SkillTree.h`, `X2BuffFactor.h` ), `JUNE_GAME_FRONT_NEAREST_ENEMY` ( `X2Game.*` ), `_ELESIS_TRANSCEND` ( d6612f76 ).
- `flagcheck JUNE_CHAR_ELESIS_TRANSCEND --range d6612f76^..d6612f76 --files` the three: `2 PASS, 1 DEFINE, 0 OTHER, 0 FAIL`, `0 deleted line(s)`, `RESULT: PASS`.
- `junediverge.py --check-all`: `allow-listed: 236`, `RESULT: PASS`. `enumscan.py --check X2Lib/X2GUEL.cpp`: `0 A/B row(s) remain ( C 0, D 1, X 0 ignored )`. `stateends.py`: `337 registered table(s), 312 with OnEventProcess, 0 STUCK`, PASS. `skillicons.py check 28000-28999`: `skills 129, misses 0, known June gaps 2`, PASS. `luabind.py check ELESIS_KNIGHT Elesis_BloodCutter`: 0 unbound, PASS.
- This review's change: `flagcheck JUNE_CHAR_ELESIS_SKILL --with ..._SKILL_DEBUG --worktree --files KTDXLIB/Always.h`: `1 DEFINE, 0 FAIL`, `1 deleted line(s)` ( the `#define` commented ), `RESULT: PASS`.

**Play-tests.** CH3a-CH3c2 as their Done notes; CH3d's 07:39-07:44 run ( 30 equip lines, the three hyper actives in state 58, B-5 in 68, `[JUNE_TRANSCEND] IMAGE` type 2 / 5 / 7 / 9, transcended cut-ins and awakenings, 0 EXCEPTION ). Not reached in CH3, handed to CH6 ( `CH6.md` amended ): the enhanced / upgrade ids each note lists, War Prelude's air state, Shadow Edge's time-out end, 28600 / 28601 / 28605, the gray / party pictures, the level-70 guide popup.

**Build:** skipped: debug retire only ( standing rule 2026-11-13 ).

**Kickoffs.** `CH4a.md` ( `_SKILL_DEBUG` retired, `_SUMMON_DEBUG` lines to show ), `CH4c.md` ( the CH4 stage review retires `_DEBUG` and `_SUMMON_DEBUG` ), `CH6.md` ( not-reached list, transcended units, JUNE_BUGS 661 ) amended.

## Phase CH4 - Passives and class mechanics

**Run with:** Opus 5.5 high, one Opus review at CH4c (covers CH4a-CH4c), user play-test (three engine pieces, planned as three from the start).

| Sub-phase | Scope |
|---|---|
| CH4a | `InitPassiveSkillState` `32b88300` (vslot 0x690) whole on June's ids, her passive ids, their objects and readers, its 7 attack-result delegate pushes replacing March's own (checklist l.920 / l.930); the June-only hook 0x83c `32b732e0`; 0x29c `32b6fc40` (the `GetReduceDebuffDurationRate` shape); the class float writer `32b73bf0` / reader `32b7d550` (l.496); the passive-id check of the Rules; flag `_PASSIVE` |
| CH4b | Overrides and memo: `SetDamageExceptionProcess` 0x2b8 `32b80f20` whole (the June-only 28416 / 28417 part `@32b81008-32b811ef`), `GetActualMPConsume` 0x6a0 `32b7ebe0` and `GetActualCoolTime` 0x6a4 `32b7ee10` with the memo reader `31fca9e0`, 0x428 `32b52260` (l.942), 0x558 `32b6fb80`; `MemoConditionElesis` (vtable `334fa4f4`, `32b51980` / `32b519a0`) and the four `MEMO` states' readers; what Elsword's CH4b took for the shared Way of the Sword leftovers (`CureExtraDamage`, `ResetAnimSpeed`'s vigor factor, `SAT_WAY_OF_SWORD_GAUGE_UP`) checked in her class |
| CH4b2 | Split from CH4b (2026-11-14): Advanced Knight Mastery's guard: the roll `32b524f0`, its caller `CX2GUUser::DamageReact @320476a2` ( user byte `+0x9175`, damage x `+0x9784` ), the `DamageReactStateChange` branch to states 332 / 333 |
| CH4c | **Blood hooks** (`ch0_blood.tsv`), flag `_BLOOD`: the hit hooks 0x4f8 `32b7beb0`, 0x4fc `32b898a0`, 0x734 `32b7bad0` and their base call sites (the hooks are inert stubs today, l.454-455), Blood Runaway `32b87880` (BFI 462 registered with it, BTI 426 erased above 4 stacks) and `32b734d0`, `32b7e3b0`, Madness `32b6ff20`, the transcendence berserker chain `32b7e9a0` (BFI 913 passing `x`), the per-frame list 0x730 `32b816a0` / `32b81220`; tested on 35 / 125 units; CH4 stage review |

**Done gate:** each passive's effect shows (CH0 names the stat or log line per passive); blood sucking heals and bleeding ticks on a Dark Knight;
0 Elesis `no_state` rows left except passives with no state in June.

### CH4a - Done (2026-11-14)

Opus 5.5. Flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE` now **on** ( CH1j's `//#define` ), its needs checked by a guard-only block at the end of `Always.h` ( `_CORE`, `_SKILL`,
`_SUMMON`, `_CLASS`, `JUNE_SKILL_STATE_BUFF`, `JUNE_ENUM_NAMES`, `JUNE_DAMAGEDATA`, `ELYSION_SCRIPT_B`, `JUNE_MP_CHANGE_TEXT`, `JUNE_BUFF_BEHAVIOR_PARITY`, `JUNE_CHAR_EVE_PASSIVE`,
`JUNE_DIVERGE_SKILL_LEVEL_VALUE`, `JUNE_CHAR_RENA_TRAP`: all ON by `flagprobe.py` ), + `_PASSIVE_DEBUG` ( on until the CH4c stage review ). Files: `X2GUEL.cpp` / `X2GUEl.h`,
`X2Game.*`, `X2GameUnit.*`, `X2BuffBehaviorTemplet.*`, `X2SkillTree.h` / `X2BuffFactor.h` / `X2DamageManager.h` ( `JUNE_ENUM_NAMES` ), `Always.h`. `funcmap.tsv` rows `Char Elesis CH4a`
( 15 ); checklist rows marked `(Elesis: CH4a ...)` ( `322a7660` row, the `32b88300` delegate row ); `JUNE_BUGS` 664-666; `june_march_divergent.tsv` 3 rows. No review of its own: the
CH4c stage review covers it ( `CH4b.md` / `CH4c.md` amended ).

**Ported ( decompile + listing, `charport_adis.py elesis --phase CH4a` ).**
- **`InitPassiveSkillState` `32b88300` whole** inside March's ( March in `#else` ). March read 8400s ids, which a June save never has: **every Elesis passive was off** before. June's
  head: base, three delegates always, every passive member reset ( March's plus the new ones ), Dodge and Slash `SA_EFFECTIVE_COUNT` -> max re-use ( the CH2a wait ). One switch, no
  fall-through: **29 / 120** Advanced Knight Mastery 28404 ( **both** quick-stand flags, no Get Chance test; force-down melee / range += `CalculateIncreasingRate( SA_FORCE_DOWN_REL )`;
  MP value / damaged rate / effective rate ), `m_fDodgeAndSlashConsumedMp` = 0, Waltz 28403 ( rushing loop, `m_iWaltzOfSwordDodgeAbility` = `SA_MP_CONSUME_REL`: June's one member at
  +0x9654, March's `m_iDecreaseMPRushingSword` is not written ), First Strike 28401, Agility Mastery 28402 ( walk / dash passive x ( 1 + `SA_MOVE_SPEED` ), anim factor ), Chivalry 28400,
  levels of 28415 / 28416 ( memo-aware `GetSkillLevelValue` ); **30 / 121** Scalet Rose push, Flame Sword ( level + push ), Wall of Prominence life, Body of Fire, Fire Blossoms,
  Concentration Mastery ( magic critical, skill level with `true` ), Stigma of Fire ( level, push, damage value ), Unextinguishable, Blazing Heart transcendence 28417 ( level / rate /
  damage ), Blaze Absolu level; **35 / 125** Dark Force ( `_WOS` lines, the CH1q amendment ), Judgment, Blood Revenge, Punish ( `m_fMPConsumeRate` = 1.0, `m_fPunishEffectPowerRate` =
  `SA_ATK_REL` ), Blood Runaway ( tiers 9999 reset, cool -1; learned: cleanse timer 999, level, the `32b87880` push under `_BLOOD`, tiers `SA_COOLTIME_1-3`, cool = counter =
  `SA_EFFECTIVE_TIME` ), Crimson Avenger transcendence level, Madness level ( `_BLOOD` ). Elesis 8 takes the head only. June's `+0x97b0 = -1` has no reader in the class: not ported.
- **Delegates:** new `RefundDodgeAndSlashMpOnAttack` `32b52470`, `CountScaletRoseHitOnAttack` `32b524b0` ( **backSpeed.x < -500**: the CH3b0 note said "> -500"; the listing's
  `test ah,5; jp` skips the increment when NOT below ), `ApplyBurstFireOnAttack` `32b6fd50` ( `DTT_BURST_FIRE`: the target's `EDT_FIRE` / `EDT_ENCHANT_BLAZE` time x damage per second and
  BTI 37 / 293 HP per second x finalizer duration land at once through `UpNowHpWithText`, each cleared; Pyro Knight's Big Burst carries it ), `HealByBurningPhantomOnAttack` `32b7e3b0`;
  June's bodies inside March's `ApplyDebuffStigmaOfFire` `32b872d0` / `ApplyDebuffFlameSword` `32b6f820` ( riding and `AT_SPECIAL` gates, her as creator: divergent rows ).
- **Ticks ( CH1j's declarations ):** `BloodRunawayProc_FrameMove` `32b734d0` ( BFI / BTI 666 by half HP, the cleanse, the cool counter; the debuff walk uses a copy: JUNE_BUGS 666 ),
  `GrandMasterTranscendenceProc_FrameMove` `32b7e580` ( BFI 907 after a dash past `SA_EFFECTIVE_TIME` with no enemy within `SA_SCOPE_ABS`; BFI 908 with exactly one enemy within
  `SA_SCOPE_ABS_PVP`, else BTI 879 erased ), `ApplyGrandMasterAura` `32b7e740` ( BFI 909 on herself; the ally stack: JUNE_BUGS 664 / 665, unreachable offline ).
- **Engine pieces ( new, `_PASSIVE` ):** `CX2Game::GetEnemyUnitCountInRange` `322d4a60` ( the `+0x8fb6` NPC byte stays a stand-in, as `GetLockOnNearstTarget` ),
  `CX2Game::GetAllyUserUnitCountInRange` `322d4ba0`, `CX2GameUnit::GetBuffChangeNowHpPerSecond` `3227c200`, `CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetChangeNowHpPerSecond`
  `32e75fb0`. June's gage-icon erase `32489b20` is called as March's `CX2GageManager::EraseBuff( uid, BTI, false )` ( same lists; its third never holds an ally ).
- **Already done, checked:** 0x83c ( CH1o ), 0x29c `32b6fc40` ( CH1h, reads `m_uiChivalryLevel` which this phase now writes ), the class float writer `32b73bf0` / reader `32b7d550` ( CH1g,
  checklist l.504 ). The CH1b review's `+0x4060` note holds ( no writer here ).
- **Names:** `X2SkillTree.h` 17 `RNSI_P_*` ( 28400 / 28402 / 28404 / 28406-28419, June `Enum.src.lua` ); `X2BuffFactor.h` BTI 879-881, BFI 666 / 907-910; `X2DamageManager.h`
  `DTT_BURST_FIRE` 8 / `DTT_BURNING_PHANTOM` 9 ( the live data036 `Enum.lua` has both ).
- **Passive-id check ( the Rules ):** the 14 `SI_*` ids March's `InitPassiveSkillState` read ( `ch0_passive_ids.tsv`: 8200, 8217, 8016, 104, 8400-8409 ) are off the live path; June's body
  reads only 28200 / 28210 / 28211 / 28018 and 284xx ( Get Chance 104 no longer ).

**Gates.**
- One-file probes ( `/c /Zs /EHsc` ): `X2GUEL.cpp`, `X2Game.cpp`, `X2GameUnit.cpp`, `X2BuffBehaviorTemplet.cpp` exit 0, 0 errors.
- `flagcheck JUNE_CHAR_ELESIS_PASSIVE --with ..._PASSIVE_DEBUG --worktree --files` ( 12 ): `files: 6 PASS, 0 PASS~ (blank/marker only), 0 DEFINE, 6 OTHER (other flags), 0 FAIL, 0 ERROR`;
  `in this flag's files: 0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS`
  ( OTHER = `JUNE_ENUM_NAMES` in the three enum headers, `Always.h`'s comment lines, the `_BLOOD` declaration in `X2GUEl.h`, a peer's DU1c hunks in `X2Game.cpp`, not committed here ).
- `junediverge.py --check` ( 8 files ): `allow-listed: 23`, `RESULT: PASS`. `enumscan.py --check` the four `.cpp`: 1 row, `X2BuffBehaviorTemplet.cpp:5261` ( `fb062a6b`, a buff debug
  line's BBT number ), not this phase's; 0 in this phase's code.
- `DEPLOY OK: X2_offline.exe is 26,570,752 B, dated 2026-11-14 08:40:52, md5 4b5aadce6d5a7f8f98ab309849f31196, equal to X2/US_SERVICE/x2.exe (2026-11-14 08:40:45); built at main 194d520b + 12 uncommitted code file(s).`
- `EXESTR OK: 3 of 3 found in X2_offline.exe, 26570752 B, dated 2026-11-14 08:40:52, md5 4b5aadce6d5a7f8f98ab309849f31196` ( wide: the init and burst-fire lines, `EffectSet_B_EL_Blood_Sucking` ).
- Commit `ce0bcdf4`. `flagcheck --range ce0bcdf4^..ce0bcdf4`: `files: 7 PASS, 0 PASS~ (blank/marker only), 1 DEFINE, 4 OTHER (other flags), 0 FAIL, 0 ERROR`; the 1 deleted line is CH1j's `//#define`; `RESULT: PASS`.
- **Save:** backup `db_backup/els_db_20261114_083128_pre_sql.sql`; learned 29 + 28404 / 28416, 30 + 28409 / 28418, 144 + 28411, 146 + 28414 ( level 1 each ).

**How to see it in game:** one `[JUNE_CHAR] ELESIS PASSIVE init ( ... )` line per load with the values above, then `dodge and slash mp refund`, `scalet rose hit counted`,
`burst fire`, `burning phantom blood sucking`, `stigma of fire` / `stigma of fire judgement`, `flame sword debuff`, `blood runaway tick` / `blood runaway cleanse`,
`grand master transcendence moving buff` / `one on one`, `dignity of grand master buff` as each fires.

**Logs ( 2026-11-14, run 09:36-09:42, md5 4b5aadce; 0 EXCEPTION / Motion Error / KEM_ERROR; known `E_DISABLE_HB_CHECK_REQ` x6 ).** `init` x2 per load on units 29 / 30 / 146 / 21 / 144:
29 dodge reuse 1, chivalry 3 ( 221 / 222 ), quick stand 1 / 1, knight mastery damaged 0.850 rate 0.150, rushing loop 1, waltz dodge 3, walk / dash x1.090, anim 0.040, GM 1, dignity 1;
30 fire ball 1.450, flame sword 1, wall 6.00, blossoms 3 ( 20.00 ), magic critical 0.075 / 0.150, stigma 3 ( 3.800 ), unextinguishable 1, BH 1 ( 0.150 / 3.000 ), blaze absolu 1;
146 CA 1, judgment 3 ( 0.500 ), revenge 3 ( 0.150 / 0.150 ), punish 0.750, blood runaway 1 ( 40 / 40 / 40, cool 1.0 ). Seen: `dignity of grand master buff`, `one on one` x16
( one enemy within 600 ), `moving buff` x16, `stigma of fire` x17, `stigma of fire judgement` x3 ( 5 stacks, x3.800 ), `flame sword debuff` x11, `scalet rose hit counted` x6
( back speed x -1500, so the `< -500` reading holds ) and the summon's `req npc ... scalet rose N of M` x6, `burning phantom blood sucking` x3 ( 28055, hp 515.5 ), `blood runaway
tick` x71 ( `passive buff 1` from hp rate 0.463 down, 0 above half ). **User: everything worked except the Crimson Avenger buff icon** = June's data: `BattleFieldBuffTemplet` gives
BTI 666 `BTI_BUFF_BLOOD_RUNAWAY_PASSIVE` no `BUFF_ICON_TEXTURE_KEY` ( its neighbours have one ), so June shows no icon either; the buff is on ( `passive buff 1` ). Not seen: `burst fire`
( Big Burst 28207 hit twice, its follow-up `DAMAGE_EFFECT_Elesis_Big_Burst` that carries `DTT_BURST_FIRE` was made at 09:41:33 and hit nothing ), `blood runaway cleanse` ( no
debuff on her ), the Dodge and Slash refund ( base id only ): carried to CH4b's play-test ( kickoff amended ). User owes nothing more for CH4a.

### CH4b - Done (2026-11-14)
**Landed.** Commit `4353d257`; `flagcheck --range 4353d257^..4353d257`: `files: 2 PASS, 0 PASS~ (blank/marker only), 0 DEFINE, 1 OTHER (other flags), 0 FAIL, 0 ERROR`, 0 deleted, `RESULT: PASS`. Flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE` (on since CH4a; `_PASSIVE_DEBUG` stays on for the CH4c stage review).
Files: `X2GUEL.cpp`, `X2GUEl.h`, `X2BuffFactor.h` (`JUNE_ENUM_NAMES`). Read from `charport_adis.py elesis` listings (no Ghidra function for 4 of the 6).
- **`SetDamageExceptionProcess` 0x2b8 `32b80f20`:** the part up to @`32b81008` = March's Way of the Sword block (Vigor normal force-down x 0.6 / PvP 0.8, Destruction types
  4-6 ignore state defence), kept; June's two parts appended under the flag. Dignity of Grand Master (`m_usDignityOfGrandMasterLevel` > 0) with BTI 326 or 565 on her, on a
  hit whose `attackerType` != `AT_EFFECT` and `hAttackerEffect` invalid -> `m_bIgnoreStateDefence`. Blazing Heart transcendence (`m_usBlazingHeartTranscendenceLevel` > 0):
  a non-special hit on a defender burning (`m_EnchantBlaze` / `m_Fire` time > 0, or BTI 37 / 47 / 167 / 243 / 290 / 443) rolls `GetRandomFloat( 27 )` <
  `m_fBlazingHeartTranscendenceRate` -> `m_bIgnoreDefence`, `_OnPvp`, `m_bIgnoreStateDefence`, `m_bIgnoreDefenceDecline` (June's word store 0x101 + bytes +0x541 / +0x534);
  then, every hit, `damage.fMagic` x min( 1.1, 1 + `EnchantStat.m_fDefBlaze` / 500 x 0.1 ) (`GetEnchantStat( true, false, false )`; June's 4th argument is null).
  DamageData offsets proven by a probe: head +4 / +8 / +0x14 / +0x3c = June's; the ignore flags keep June's relative order (+0xc / +0xd), June's tail is 0xc4 longer.
- **`GetActualMPConsume` 0x6a0 `32b7ebe0`** whole inside March's: base, then on the base / upgrade pairs 28200 / 28217 free under Victorious Sword, 28201 / 28218 x
  `m_fMPConsumeRate`, 28202 / 28219 max( 0, MP - `m_iRushingSwordNowLoopValue` x MP x 0.1 - `m_iWaltzOfSwordDodgeAbility` ) (June +0x9648 = the loop count CH2b2 named, +0x9654
  = Waltz of Sword's `SA_MP_CONSUME_REL`, which replaces March's `m_iDecreaseMPRushingSword`); Vigor x 0.6 / PvP 0.8 on the upgrade-swapped templet; x `GetSkillMpDecreaseRate`.
  March's memo cases 5 / 16 / 8 stay in `#else`. The kickoff's "x0.7" does not exist in her class: June's Vigor rate is March's 0.6.
- **`GetActualCoolTime` 0x6a4 `32b7ee10`** whole inside March's: Vigor x 0.2 / PvP 0.6 (= March), 28200 / 28217 x the Victorious Sword rate, 28201 / 28218 doubled while
  `IsDamageState( false )` (vslot 0x534, CH1o; `const_cast`: June's override is not const), clamp < 0.
- **Memo:** both overrides' 8023 + memo 608111 exemption reads June's `GetEqippedSkillMemo` stub `31fca9e0` (always false): no code, one comment each.
  `MemoConditionElesis` (vtable `334fa4f4`): **n/a** as Ara / Chung: slot 1 `32b51980` returns the base decide `31fd4450` (always false), ctor `32b642f0` only sets the vtable;
  `m_bApplyMemo = false` already covers it (JUNE_BUGS 332). The four `MEMO` state tables have no reader beyond that.
- **0x428 `GetDefenceRateBySkill` `32b52260`** (new override, base declared by `JUNE_USER_PASSIVE_HOOKS`): in Infernal Blade's state (`GetNowStateSkillID()` ==
  `RNSI_SA_SPK_INFERNAL_BLADE`) max( 0, 1 - 0.002 x `GetEnchantResist( ET_BLAZE )` ), else 1.0. The kickoff's "level term" is vslot 0x30c = `GetEnchantResist`, type 1.
- **0x558 `32b6fb80`** = `GetHpTier`, ported in CH1h with no caller (CH1o): nothing to do.
- **Shared Way of the Sword leftovers in her class: all = March, nothing to port.** `ResetAnimSpeed` `31ffe190` @`31ffe27e`: the Vigor factor tests unit type 1 only (Elsword),
  as March `X2GUUser.cpp` `ResetAnimSpeed`; `UseItemSpecialAbility` case 33 @`322fd29d` = March `X2Game.cpp` (my unit, `SetWayOfSwordPoint` virtual, her class side is CH1q);
  `CureExtraDamage` `3227d390` is generic (funcmap 7320).
- **Names:** BTI 47 `BTI_DEBUFF_SWORD_FIRE`, 167 `BTI_DEBUFF_SWORD_ENCHANT_FIRE`, 243 `BTI_DEBUFF_IGNITION_CROW_INCINERATION_BURN`, 443 `BTI_DEBUFF_PARANOIA_FIRE` (June
  `Enum.src.lua`) under `JUNE_ENUM_NAMES`. Numbers kept, in `_CONST_EL_CH4B_` with June's constant addresses: 0.1f, 0.002, 500.0, 0.1, 1.1, and the random offset 27 (no
  `SPECIAL_RANDOM_OFFSET` member).
- **Divergence:** three `june_march_divergent.tsv` rows (`CX2GUEL::GetActualMPConsume` / `GetActualCoolTime` / `SetDamageExceptionProcess`, replaced in this commit); two allow rows
  (both bodies test game type 1 only, @`32b7ed7c` / @`32b7eedf`). Checklist rows marked `(Elesis: CH4b)`: L331 (memo condition n/a), L333, the vslot 0x428 row.
- **Routed: the Advanced Knight Mastery guard** (CH4a asked for the caller of `32b524f0`): its only caller is `CX2GUUser::DamageReact @320476a2`, a shared condition chain
  that sets user byte `+0x9175`, multiplies the damage by `+0x9784` and sends `DamageReactStateChange` to her states 332 / 333. New phase **CH4b2** (`kickoffs/char-elesis/CH4b2.md`),
  before CH4c; the CH4c stage review covers it.
- **For the CH4c review:** `junediverge.py --list` still shows CH4a's two rows (`ApplyDebuffStigmaOfFire` / `ApplyDebuffFlameSword`) as open, though their source column says
  "ported (char-elesis CH4a)": the lister does not read that wording.

**Gates.** One-file probe (`/Zs /EHsc`) `X2GUEL.cpp`: exit 0, 0 errors.
`flagcheck JUNE_CHAR_ELESIS_PASSIVE --with ..._PASSIVE_DEBUG --worktree --files` (3): `files: 2 PASS, 0 PASS~ (blank/marker only), 0 DEFINE, 1 OTHER (other flags), 0 FAIL, 0 ERROR`;
`in this flag's files: 0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`; `RESULT: PASS`
(OTHER = `X2BuffFactor.h` under `JUNE_ENUM_NAMES`). `junediverge.py --check` (3 files): `allow-listed: 13`, `RESULT: PASS`. `enumscan.py --check X2GUEL.cpp`: `0 A/B row(s)`.
`DEPLOY OK: X2_offline.exe is 26,573,824 B, dated 2026-11-14 10:18:21, md5 c01fa8ce1fef66fe112fb0d4c625775b, equal to X2/US_SERVICE/x2.exe (2026-11-14 10:18:14); built at main 98a35eed + 5 uncommitted code file(s).` (2 of the 5 are a peer's `X2GUUser.*`)
`EXESTR OK: 5 of 5 found in X2_offline.exe, 26573824 B, dated 2026-11-14 10:18:21, md5 c01fa8ce1fef66fe112fb0d4c625775b` (wide: the mp, cool time, dignity, blazing heart ignore-defence and infernal blade lines).
**Save** (backup `db_backup/els_db_20261114_100847_pre_sql.sql`): unit 29 `ElesisGM` Rushing Sword 28202 into slot 5 (B-1), Dodge and Slash 28217 into slot 7 (B-3); Drastic
Charge 28206 and Sonic Blade 28011 out. Units 30 / 21 / 146 unchanged (Infernal Blade upgrade 28050 is on unit 21 slot 2).
**In game:** `[JUNE_CHAR] ELESIS PASSIVE mp: skill ...`, `cool time: skill ...`, `dignity: ignore state defence`, `blazing heart transcendence: ignore defence` /
`magic damage x`, `infernal blade defence rate` (each at most once a second).

**Play-test PASS (user, 2026-11-14; run 10:25-10:30 on md5 `c01fa8ce`; 0 EXCEPTION, 0 ErrorLog lines of this phase; known `E_DISABLE_HB_CHECK_REQ` UNHANDLED x5).** `init` on
units 29 / 30 / 21 / 146. `mp:` 81 lines: Rushing Sword 28202 25.00 -> 22.00 ( loop 0, waltz 3 ) x30, -> 19.50 ( loop 1 ), -> 17.00 ( loop 2 ), -> 13.20 ( loop 0 in Vigor );
Vigor x 0.6 on 28039 100 -> 60 and 28043 80 -> 48. `cool time:` 28217 Dodge and Slash 5.00 -> 0.40 x8 ( Vigor 0.2 x Victorious Sword ), 28023 15.00 -> 3.00, 28224 5.00 -> 1.00.
`dignity: ignore state defence` x11. `blazing heart transcendence: ignore defence` x13 ( rolls 0.133-0.137 < 0.150 ), `magic damage x 1.044 ( blaze resist 220.0 )` x29.
CH4a's carried `burst fire` x9 ( Big Burst into a burning group ): closed. Not seen: `infernal blade defence rate` ( unit 21 has the upgrade 28050 slotted; a state's skill id is
the slotted id in June too, `32024b00` @`32024d2c`, and June names only 28014: **JUNE_BUGS 667**, verbatim; the base-skill check goes to CH4b2's play-test on unit 30, which has
28014 ) and CH4a's `blood runaway cleanse` ( no debuff reached unit 146 ): carried to CH4c, the Crimson Avenger phase. User owes nothing more for CH4b.

### CH4b2 - Done (2026-11-14)
**Landed.** Commit `abe3a805`; `flagcheck --range abe3a805^..abe3a805`: `files: 5 PASS, 0 PASS~ (blank/marker only), 0 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR`,
0 deleted, `RESULT: PASS`. Flag `SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE` (on since CH4a; `_PASSIVE_DEBUG` stays on for the CH4c stage review). No review of its own: the CH4c
stage review covers it, with its own verdict line for the `X2GUUser.*` hunks (shared file). Files: `X2GUEL.cpp`, `X2GUEl.h`, `X2GUUser.cpp`, `X2GUUser.h`, `Always.h`.
Read from Ghidra (`320462a0`, `31feabd0`), `charport_adis.py elesis 32b524f0`, and an operand scan of `.text` for `+0x9175` (6 sites, all ported below).
- **The roll** `32b524f0` = `CX2GUEL::IsAdvancedKnightMasteryGuard()`: `0 < m_fAdvancedKnightMasteryEffectiveRate` (+0x978c), then `GetRandomFloat( 0 ) <= ` it.
  June's compare is `<=` (`fcompp`, `test ah, 1`: only rate < random fails), not the kickoff's `<`. Public getter `GetAdvancedKnightMasteryDamagedRate()` (+0x9784).
- **The byte** +0x9175 = new `CX2GUUser::m_bAdvancedKnightMasteryGuard` beside `m_bQuickenGuardTurning` (+0x9174); cleared in the ctor (@`32042dbf`) and at
  `DamageReact`'s start (@`32046321`, one dword store clears +0x9172-+0x9175 = parrying, quicken blocking / turning, guard), both beside March's quicken resets.
- **The set site** @`320475a5`-`320476b5`: June's guard chain is Raven, Ara, **Elesis**, Eve, else; Elesis's `else if` goes between March's Ara and Eve blocks.
  Tests named from the Raven / Ara blocks June compiles at the same offsets: `GetUnit()->GetType() == UT_ELESIS`, `bCheckExtraDamage` false, DD `m_bNoDamage` false,
  `m_fHpPercentUp <= 0`, `m_fIncreaseMP <= 0`, reActResult `RT_SMALL_DAMAGE` or `RT_BIG_DAMAGE` (Raven / Ara test != dummy / sensor instead), `GetHold()` / `GetGrap()` false
  (+0x8a48 / +0x9108), `GetRidingOn()` false (vslot 0x200), `IsSuperArmor()` false (vslot 0x264), `GetFrameData().unitCondition.bFootOnLine` (+0x80bc),
  `m_FrameDataFuture.unitCondition.bAttackerFront` (+0x7e2d), gage +0xc04's force-down gage (+0x5c: now +0x60, max +0x6c) `GetNowForceDown() < GetMaxForceDown()`, then the roll.
  A passed roll sets the byte and skips `DamageReact_ExtraDamage` (jump @`32047810`); a failed roll goes to it (@`320473ee`). A failed test before the roll falls into
  the Eve test (fails for type 8) and the else (`DamageReact_ExtraDamage`): the `else if` with the roll last is the same flow.
- **The enchant gate** @`32047844` (found by the scan, not in the kickoff): March's `false == m_bParryingState && ... && false == m_bManeuverParryingState` gets
  `&& false == m_bAdvancedKnightMasteryGuard`, June's order: a guarded hit rolls no enchant extra damage.
- **The multiply** @`320492ba`-`320492d5` replaces D4b's stand-in comment (kept in `#else`): `fFinalDamage *=` her damaged rate while the byte is set, after Ara's x0.
  It sits in `JUNE_STATCALC_DAMAGE_REACT`'s block, so `Always.h` gains a guard: `_PASSIVE` needs `JUNE_STATCALC_DAMAGE_REACT` (defined at 12765, before the guard; the probes compile it).
- **The state branch** @`31feacf4`-`31fead30` in `DamageReactStateChange`, after March's `m_bQuickenGuardBlocking` block, before the turning block: byte cleared,
  `RT_SMALL_DAMAGE` -> `ESI_P_SGM_ADVANCED_KNIGHT_MASTERY_SMALL` (June 0x14d), else `ESI_P_SGM_ADVANCED_KNIGHT_MASTERY` (0x14c), `StateChange( id, true )`, return.
- No `june_march_divergent.tsv` row: `DamageReact` / `DamageReactStateChange` are piecewise ports with rows 96 / 97 already; no JUNE_BUGS row (nothing wrong-looking kept).
  Checklist L463 / L482 marked `(Elesis: CH4b2)`; funcmap rows `320462a0 @320492ba` / `32b524f0` marked PORTED.

**Gates.** One-file probes (`/Zs /EHsc`) `X2GUEL.cpp`, `X2GUUser.cpp`: exit 0, 0 errors each.
`flagcheck --worktree` (5 files): my files PASS; `X2GUUser.cpp` FAIL / `X2GUUser.h` OTHER came from a peer's uncommitted `JUNE_CHAR_DUO_BODY` hunks; the range run below
covers my commit only. `junediverge.py --check` (4 files): `allow-listed: 21`, `RESULT: PASS`. `enumscan.py --check X2GUUser.cpp X2GUEL.cpp`: 4 A rows, all
pre-existing (`X2GUUser.cpp:27704-27710` NUI_BEE*, char-luciel `04d2cacf`), 0 in this phase's hunks. `stateends.py check X2GUEL.cpp ELESIS_KNIGHT`: 337 tables, 0 STUCK,
`RESULT: PASS` (both guard states have `OnEventProcess`, lines 5922 / 5929).
`DEPLOY OK: X2_offline.exe is 26,599,424 B, dated 2026-11-14 10:59:12, md5 30b80d107a871237f182caa0fb37bf29, equal to X2/US_SERVICE/x2.exe (2026-11-14 10:59:05); built at main 6c48182a + 5 uncommitted code file(s).` (the 5 = this phase's)
`EXESTR OK: 2 of 2 found in X2_offline.exe, 26599424 B, dated 2026-11-14 10:59:12, md5 30b80d107a871237f182caa0fb37bf29` (wide: the guard roll and guard damage lines).
**Save** (backup `db_backup/els_db_20261114_104831_pre_sql.sql`): unit 30 `ElesisBH` Infernal Blade 28014 into slot 8 (B-4), Scaldic Sword 28013 out. Unit 29 unchanged
(28404 learned, rate 0.150 since CH4a).
**In game:** `[JUNE_CHAR] ELESIS PASSIVE advanced knight mastery guard ( roll ... <= rate 0.150, damage x ... )` and `... guard damage A -> B` (each at most once a second);
CH4b's carried `infernal blade defence rate` on unit 30.

## Phase CH6 - Whole-character walk and gap reruns

**Run with:** Sonnet 5.5 medium, then the user's walk. Back up the save; one Elesis per class (8 / 29 / 30 / 35 / 120 / 121 / 125), one transcended
per second job. Walk: every skill in an A and a B slot (each cast twice: cool time and MP), a hyper, the Way of the Sword (fill both ways, vigor,
destruction, a stage change), each summon, blood sucking and Blood Runaway on 35 / 125, the passives, one dungeon per job, both job changes to
35 / 125 and one to 29 from the level-15 dialog (CHNb's quests on fresh units), the creation screen's six entries, then **leave the game and return
to the village**. Re-run `census.py`, `charport_scan.py`, `charport_keys.py ELESIS_KNIGHT` and `s3_no_state.py`; before / after numbers in the Done
note. **The becoming-transcended transition on un-transcended units (user rule, 2026-10-28):** one unit per second-job class (120, 121, 125) by
`/tc 1`, quest 42030 (`/qdone 42030`) and the book 288493 (`/additem 288493 1`), each showing the guide popup with that class's picture, the new
skill-tree rows, the HUD portrait, and surviving a relog. A defect goes back to its phase's flag.

**Done gate:** the walk passes; 0 unexplained `no_state` rows; no new `KEM_ERROR`.

## Phase Z - Close-out

**Run with:** Sonnet 5.5 medium. `MODS.md` "June character: Elesis" section (flags, files, data, revert order: exe + data036 + motion koms
together; `SERV_IRUHADEV_JUNE_EL_DASHJUMP_XZ_FIREBALL` recorded as superseded; any user-packed `StatTable.lua` rows); the stale "flag off" comments
about `SERV_NEW_CHARACTER_EL` corrected if a phase touched those files; no `_DEBUG` flag left defined; checklist rows marked `(Elesis: CHn)`; the
Aisha plan's "Template for Stage 7" gets an Elesis paragraph (what she took against this plan; what CH-N's second run took); backlog Stage 7 note;
the `stage7-plan` row points at the next character (`char-add`).

## Play-test hand-off (every phase)

Dev console first; copy-ready lines with real values (unit, class, skill id and slot, `/additem`, `/qdone`); per check: where / paste /
what it does / look for / if not. Log tag `[JUNE_CHAR]`, `python tools/logs.py --grep "JUNE_CHAR ELESIS <sub>"`.

## Open decisions (for the user; asked at the end of planning)

1. **Stat rows for Dark Knight (35) and Crimson Avenger (125).** Ara's answer (2026-11-10) followed Ara's own rows, where her second jobs had a
   level 1 of their own. Elesis's evidence is different: `StatTable.lua` has rows for 8 / 29 / 30 only, and **Grand Master and Blazing Heart have
   none**: they play on class 8's rows through the loader's type fallback today. **(a, recommended)** leave it so: 35 and 125 take the same
   fallback as 120 / 121 (no server Lua to write or pack; CHNa only adds the missing log line), and June's whole stat calculation stays Stage 33's.
   **(b)** Ara's rule for all three second jobs: write rows for 120, 121 and 125 with level 1 = 0.8 x June's relative row (Grand Master
   `400, 320, 89.6, 72, 12250`; Blazing Heart `392, 344, 88, 73.6, 11750`; Crimson Avenger `408, 328, 81.6, 77.6, 12000`, in the column order Ara's
   118 uses: CHNa checks it) and levels 81-99 from `cap99_gen.py`, as a labelled stand-in; you pack `StatTable.lua`. It changes Grand Master's and
   Blazing Heart's numbers too. **(c)** rows for 125 only: Crimson Avenger alone would differ from her sister classes.
   **Answered (user, 2026-11-11): (b), Ara's rule for all three second jobs.** CHNa writes the rows as a labelled stand-in: 35 = class 8's 99 rows
   verbatim (as 29 / 30); 120, 121 and 125 = class 8's rows with level 1 = 0.8 x June's relative row and the top levels regenerated by
   `cap99_gen.py` (check the column order and the regenerated level range against Ara's 118 / 119 rows first); the user packs `StatTable.lua`.
   Grand Master's and Blazing Heart's numbers change with it: `MODS.md` records before / after.

Settled by standing rules, not decisions: the motion koms deploy with CH1 final if June drops a name March plays (7ad); Ghidra stays read-only
and listing-only functions are read with `charport_adis.py`; the lobby particle hard-codes are retired where June's table plays them (Ara's
decision 2); June-side icon gaps and the three nil BFI names are `JUNE_BUGS.md` rows; the 54 emotion motions are staged, their behaviour is
Stage 10; third jobs 171-173 are out of scope.
