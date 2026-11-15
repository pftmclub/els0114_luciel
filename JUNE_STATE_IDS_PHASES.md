# June user state ids - March's `USER_STATE_ID` takes June's numbers

Plan key `june-state-ids` · backlog Stage 41 · branch `main` · created 2026-11-15 (planning session, brief `kickoffs/june-backlog/stage41-plan.md`).
Flag `SERV_IRUHADEV_JUNE_USER_STATE_IDS` (+ `_DEBUG` in S1 only) · log tag `[STATE_IDS]` (`python tools/logs.py --tag STATE_IDS`) · kickoffs
`kickoffs/june-state-ids/`.

## Context

March's `CX2GUUser::USER_STATE_ID` (`X2Lib/X2GUUser.h:578-721`) and June's agree up to 23 and then fall behind. Lu/Ciel registers every state at
June's id, so every engine `StateChange( USI_* )` at 24 or above sends Lu to the wrong state, `InitEventStates` overwrites `LSI_RIDING_SPECIAL_MOVE`
( 92 ), and `m_bBoundingStatesReady` stays false (char-luciel CH1m / CH1n / CH1o Done notes). **User, 2026-11-15 (char-luciel CH1o):** shift March's
enum to June's numbers, with June's members inserted at June's values; every class enum and the core-list `Enum.lua` follow. Getters at every engine site
were rejected. Precedent: `SERV_IRUHADEV_JUNE_RESERVED_STATE_IDS` (effect C9h-4g, 2026-10-10) shifted the class ids by 11 with `Enum.lua` shipped
alongside, the same way.

## Census (done in planning, 2026-11-15)

`tools/data/june_user_state_ids.tsv` (147 rows: name, June value, March value, live `Enum.lua` value, note). Sources: June
`june_src_fixed/data036_kom/Enum.src.lua:11254-11404` (`GUSI_END` = 5); March from `june2017_re/enumprobe.py` (all 101 names resolved, every flag
inside the enum on: `JUNE_SKILL_SLOT_10`, `RIDING_SYSTEM`, `JUNE_USER_STATE_ID16`, `JUNE_RESERVED_STATE_IDS`); live
`24965799\kom_contents\data036_kom\Enum.lua:9150-9253` (BOM source, equals March C++ row for row).
- **June 147 members (`USI_END` 152), March 101 (`USI_END` 106).** March has no member June lacks. The shared names are in the same order, and June's
  values run 6..152 without a gap. **Inserting June's 46 missing names at June's values makes every shared name equal.** No March member is renumbered
  or renamed by hand.
- **The 46 inserted members:** 24 `USI_TRANSCENDENTAL_HYPER_MODE`; 93-109 `USI_RIDING_SPECIFIC` .. `USI_RIDING_ATTACK_X_END`, the 12
  `USI_QUICK_SLOT_SKILL_[HYPER_]ITEM_1-6`, `USI_CHEAT_CUSTOM_MOTION`; 123-129 the phoenix run (4) and dance (3); 131-151 the 20
  `USI_SPECIAL_ATTACK_CORE_*` and `TSI_BUFF_COMMON_CLOCK_OF_ELIAN_HALF` (June's own name and prefix, `Enum.src.lua:11402`, kept verbatim).
- **Every class enum moves by +46:** the nine `*_BASE = USI_END + 1` (`X2GUAdd.h:372`, `X2GUAra.h:60`, `X2GUArme_VioletMage.h:8`, `X2GUChung.h:16`,
  `X2GUEl.h:12`, `X2GUElsword_SwordMan.h:8`, `X2GULire_ElvenRanger.h:8`, `X2GURaven.h:8`, `X2GuEve.h:8`) go from 107 to 153. `X2GULuciel.h`'s
  `LUCIEL_STATE_ID` is absolute June numbers and does not move.

## Readers of the numbers (survey, 2026-11-15)

| Reader | Verdict |
|---|---|
| Class enums at `USI_END + 1`, `_CONST_GUUSER_::aNormal/aHyperSkillStateID[SlotB]` (`X2GUUser.h:5974-5993`), `UserSkillTree` switch, `aiState[]` tables | by name: shift with the enum |
| Range tests `USI_DAMAGE_GROGGY..USI_DAMAGE_GRAPPLED_FRONT` (`X2GURaven.cpp:21161`, `X2GuEve.cpp:49900`), `USI_RIDING_WAIT_HABIT..USI_RIDING_DASH_JUMP_LANDING` (`X2GUUser.cpp:47548`) | safe: no inserted id falls inside either block (24 sits before GROGGY; riding ends at 92) |
| Sync data, `UserUnitStateData::stateID`, the state map, `UnitCondition::nextState`, P2P `m_ucNowState` | USHORT / short under `JUNE_USER_STATE_ID16`: safe up to 65535 |
| Offline emulator, `X2OfflineDB`, `els_db.sql`, packets in `KncWX2Server/Common` | no user state id stored or sent: safe |
| Buff / finalizer / effect-set state ids (`X2BuffFinalizerTemplet.cpp:765-811`, `X2BuffTemplet.cpp:2474-2536`, `X2EffectSet.cpp:3523`) | runtime values from names: safe |
| **`X2TrainingGameUI.cpp:98, 277, 343`** `GetNowStateID() == (const char) iStateID` | **edit (S1):** a signed char never matches above 127; after the shift every class state is 153 or above. Compare as int |
| `X2StateDungeonGame.cpp:4121, 4451` `(char)GetNowStateID()` (TextOutSkillList, no caller: `c3d1_state_id_sites.tsv` row 12) | dead: left, noted |
| **`Enum.lua` `USER_STATE_ID`** (`GUSI_END + n` rows) | **edit (S1):** June's table rows, same pack as the exe. `CHUNG_STATE_ID` / `ARA_STATE_ID` (`USI_END + n`) follow by themselves |
| `Enum.lua` absolute class tables `ELSWORD_SWORDMAN` / `ARME_VIOLEMAGE` / `LIRE_ELVENRANGER` / `RAVEN` / `EVE` (`*_BASE = 6`) | already out of step with C++ (107) before this plan; the shift changes nothing. Routed (below) |
| Class scripts, `CommonSkillState.lua`, NPC scripts | read names through `Enum.lua` at load, or use string state names: follow |
| `tools/luabind.py` STATE gate, `tools/stateends.py` | parse the enum or names, no hard-coded numbers: the gate S1 runs |
| `X2GULuciel.*` (absolute June ids, `StateChange( 263 / 268 )`, shadow `USI_SPECIAL_ATTACK_*` = 50-69, member-scope tables `X2GULuciel.cpp:14224-14227`) | right after the shift; tidy in S2 |
| No `static_assert` on state values, no `[USI_END]` array, no `USI_X + i` arithmetic, no numeric state id in any March class | - |

**Routed, not this plan's subject:** the five absolute class tables in `Enum.lua` feed the 23 `Dungeon_Training_*` scripts (data035) through
`X2TrainingGameUI` and were already wrong before this plan (Lua `ESSI_COMBO_X1` 60, C++ about 122). Owner: backlog Stage 2's training-game bullet (added
2026-11-15). The plan fixes only the char cast, which the shift would otherwise make worse.

## Rules for this plan

- Standing rules: `CLAUDE.md`, `JUNE_PORT_BACKLOG_ORDER.md` "Standing rules". Enum rule: June's names go inside March's enum under the flag; never
  renumber or rename a March member; the inserted members do the shifting.
- **The exe and `data036.kom` ship together.** With the flag on, the staged `Enum.lua` must carry June's `USER_STATE_ID` rows; with the flag off it
  needs March's rows back (the `luasrc.py` backup / `kom_contents_bytecode` snapshot). The flag's `Always.h` description says so, as `RESERVED_STATE_IDS`' does.
- **`X2GUUser.h` and `X2GULuciel.*` are shared with char-luciel.** S1 runs alone in `X2GUUser.*`; S2 runs alone in `X2GULuciel.*`. Neither runs beside a
  char-luciel phase that has uncommitted edits in those files.
- **Gate: `tools/stateids.py check` (S1 writes it).** It resolves every `USER_STATE_ID` name with `enumprobe.py`, then compares the result with June's
  `Enum.src.lua` table and the staged `Enum.lua`. `RESULT: PASS` means all three agree on every name. Rose and Ain reuse it.

## Starting context

- Tools: `tools/bytepatch.py` (`X2GUUser.h` is CP949), `tools/finish.py`, `tools/build.py --deploy`, `tools/luasrc.py` (Enum.lua is core-list BOM
  source: edit, `luasrc.py refresh`, `ledger --check`), `tools/kom.py pack data036` / `copy data036`, `tools/luabind.py check`, `tools/logs.py`,
  `june2017_re/enumprobe.py` (a file of expressions; makes and deletes its own probe in `X2Lib`).
- Census: `tools/data/june_user_state_ids.tsv`. June table: `june_src_fixed/data036_kom/Enum.src.lua:11254-11404`.
- Existing flags inside the enum: `SERV_IRUHADEV_JUNE_USER_STATE_ID16` (`Always.h:6188`), `SERV_IRUHADEV_JUNE_RESERVED_STATE_IDS` (`Always.h:6493`).

## S1 - The shift: June's 46 members, `Enum.lua`, the training-UI casts, regression play-test

**Scope.** In `X2GUUser.h`'s `USER_STATE_ID`, under `SERV_IRUHADEV_JUNE_USER_STATE_IDS`, insert the 46 census members at their June positions, each with
an explicit `= <June value>`. The flag `#error`s without `JUNE_USER_STATE_ID16` and `JUNE_RESERVED_STATE_IDS`. Write `tools/stateids.py check` (rules).
Change the three `X2TrainingGameUI.cpp` casts to int compares under the same flag. Replace the staged `Enum.lua` `USER_STATE_ID` rows with June's table
(core list; `luasrc.py refresh`; ledger), then pack and copy data036. A `_DEBUG` line logs every `StateChange` to a user state id that the unit has not
registered (`[STATE_IDS] UNREGISTERED <class> <id>`, throttled), unless March already logs that case, in which case the phase uses March's line.
Full build + deploy. One regression play-test over every live class (see the kickoff).
**Run with.** Sonnet 5.5 / high (the census fixes the list; the work is transcription and data). Opus 5.5 / high review.
**Read list.** This doc: "Census", "Readers of the numbers", "Rules for this plan", "S1"; `Always.h:6480-6512`.
**Done gate.** `stateids.py check` PASS; `luabind.py check Ara_Martial_Artist Chung_Iron_Cannon` STATE gate PASS; `finish.py` PASS; `DEPLOY OK` +
`PACK OK` / `COPY OK`; play-test passed; the log shows no `UNREGISTERED` line from a March class.

### S1 - Done (2026-11-15)

- **Landed:** flag `SERV_IRUHADEV_JUNE_USER_STATE_IDS` (+ `_DEBUG`). `X2GUUser.h`: 46 members at June's values, 4 runs (24; 93-109 after `USI_RIDING_SPECIAL_MOVE` inside `RIDING_SYSTEM`; 123-129 after `USI_BOUNDING_UP`; 131-151 after `USI_WAIT_STATE_CHANGE`). `USI_END` 106 -> 152. `X2TrainingGameUI.cpp`: 3 int compares, March's cast in `#else`.
- **`tools/stateids.py check`:** `147 / 147 C++ = June; 147 June names, 147 staged Enum.lua names, 147 C++ names`, `RESULT: PASS`; `--write-tsv` regenerated the census (column `march_value` is now `cpp_value`). README row added. (cl stops at 100 errors, so it probes in chunks of 60.)
- **Enum.lua:** `USER_STATE_ID` block (was 104 lines incl. the char-ara CH1 comment) replaced by June's 148-line block, BOM + CRLF kept; `luasrc.py refresh` + `ledger --check` PASS (0 FAIL), `core --check` PASS. `luabind.py check Ara_Martial_Artist Chung_Iron_Cannon`: `STATE: 64 state-id reference(s) compared with the C++ enum, 0 wrong`, `RESULT: PASS`. `PACK OK data036`, `COPY OK data036` (md5 7f53674276abdd04581275d626e10285, backup `kom_backup_tools/data036.kom.20261115_093540`).
- **Debug line:** `StateChange` logs nothing for an unregistered target (it only sets `nextState`). The lookup that fills the gap is `GetStateDataIterator` (it inserts a default entry), so the line sits there: `[STATE_IDS] UNREGISTERED unit class <class> state <id> from <now id>`, once a second per id, id 0 skipped. `EXESTR OK: 1 of 1 found` (wide, `[STATE_IDS] UNREGISTERED unit class`).
- **Tool change (enumscan):** the explicit `= <June value>` on an inserted member is the enum's definition, so `tools/enumscan.py` now skips enumerator initializers. Six old rows in `X2GUUser.h` / `.cpp` (two `NUI_*` constants of C9h-4h, four relative-position `case 1..4` of DU1b3) are not this phase's code: `stray` verdicts appended to `june2017_re/enum_literals.tsv`. The review confirms.
- **Gates:** `finish.py --files ... --skip-flag JUNE_RESERVED_STATE_IDS / JUNE_USER_STATE_ID16 / JUNE_SKILL_STATE_PTR (outer blocks the inserts sit in)`: flagcheck JUNE_USER_STATE_IDS PASS (3 PASS, 1 DEFINE, 0 OTHER, 0 FAIL, 0 deleted lines, 6 bare `#else` / `#endif` = the 3 `#else` in the training UI and 3 `#error` guards `#endif`); junediverge PASS; enumscan PASS; stateends / skillicons / luabind SKIPPED here (no skill states, no June script staged; luabind run by hand above). The post-commit `--range` block is below.
- **Build:** `DEPLOY OK: X2_offline.exe is 26,625,536 B, dated 2026-11-15 09:32:42, md5 8627a24f1fdeec48048c7f99a3245f35` (`--pch force`; 0 errors, 638 warning lines).
- **Not tested, no reachable trigger:** bounding (only some `DamageEffect` rows use `RT_BOUNDING`; no monster identified) and the event kit states (pickup of items 100005740-43 is a field drop). The unregistered-state log covers them if they ever fire.
- **Readers:** nothing new beyond the survey table; the review re-greps the char casts and `255` tests.
- **Save:** play-test units use existing characters; slot 5 filled for Raven, Ara and Add (Deadly Blow, Wind Wedge, Particle Prism) by `elsdb x` (backup `db_backup/els_db_20261115_092304_pre_sql.sql`).
- **Logs after the play-test (2026-11-15, run after the deploy, 09:39-09:59):** 274 `[STATE_IDS] UNREGISTERED` lines, all expected default-entry lookups, none from a failed state change: classes 100 / 103 / 105 / 106 / 109 / 117 / 119 / 120 / 122, ids 50, 52 .. 68 (the ten normal `USI_SPECIAL_ATTACK_*` slot states, `from 0`, ten per class load) come from `SnapshotBeforeSlotRebind` calling `GetStateDataIterator` just before the slot's Init registers the state. Two outliers: class 103 state 4 from 8 (a game-unit state below `USI_BASE`, the shift does not touch it) and class 109 state 242 from 0 (`CX2GUEve::SetSkillLevelStateDataByID`, "none when not registered"; Eve-port code, 242 - 46 = 196 before). No `UNREGISTERED` for any inserted id (24, 93-109, 123-129, 131-151). `ErrorLog.txt` has only the known JUNE_FX / JUNE_ENGINE / PET_PREVIEW lines; 10 `E_DISABLE_HB_CHECK_REQ` UNHANDLED (pre-existing). The debug line is noisy by design, so the review retires it.
- **Play-test (user, 2026-11-15): passed** for Elsword, Aisha, Rena, Raven, Eve, Chung, Ara, Elesis. Add (LunaticPsykerT) stays in the win pose and does not return to the normal state on the result screen: ignored by the user, Add's port has not happened ( `USI_WAIT_STATE_CHANGE` is not registered for her; char-add owns it ).
- **Review:** `kickoffs/june-state-ids/S1-review.md` written. Play-test: owed by the user (hand-off below in chat).

### S1 - Review (2026-11-15, Opus 5.5)

**Verdict: S1 stands; no code fix.** The only edit is the debug retire (`//#define SERV_IRUHADEV_JUNE_USER_STATE_IDS_DEBUG`).
- **Numbers.** `stateids.py check`: `147 / 147 C++ = June`, `RESULT: PASS`. `enumprobe.py`: `USI_TRANSCENDENTAL_HYPER_MODE` 24, `USI_WAIT_STATE_CHANGE`
  130, `USI_END` 152. Ghidra: June's Eve InitState `329ba9c0` registers `ENSI_WAIT_STATE_CHANGE` with `push 0x82` (@329bae3d), and its first class states
  equal C++ after the shift (`ENSI_COMBO_ZZZdownZZ` 0xab = 171, `ENSI_COMBO_XXfrontXX` 0xb0 = 176), so the class base 153 (`USI_END` + 1) is June's.
- **Readers (re-grepped).** Every `char` / `UCHAR` / `BYTE` hold of a state id is safe: `GetGameUnitState[Future]` `(UCHAR)` casts, `UnitCondition::nextState`,
  `UserUnitStateData::stateID`, `m_OldStateId`, P2P `m_ucNowState` are in the dead `#else` of `JUNE_USER_STATE_ID16` (guarded by S1's `#error`);
  `KXPT_UNIT_NPC_SYNC::m_cNowState`, the pet sync struct, `X2GUNPC.cpp:4989` (`> 127`) and `X2GUUser.cpp:25947` (`BYTE`) are NPC states;
  `m_TutorialBeforeUserStateID` is set in the ctor and never read; `X2StateDungeonGame.cpp:4121 / 4451` stay dead (survey). No `[USI_END]` array,
  no `USI_END -` / `*` arithmetic, no `static_assert`.
- **Training UI.** The three compares are `(int)`, March's `(const char)` in `#else` (`X2TrainingGameUI.cpp:99 / 282 / 352`). `GetNowStateID()` is `int`.
- **Enum.lua.** `luasrc.py ledger --check` PASS (0 FAIL, 0 WARN); `luabind.py check Ara_Martial_Artist Chung_Iron_Cannon` STATE 64 refs, 0 wrong, PASS.
  Deployed `data036.kom` md5 `7f53674276abdd04581275d626e10285` = S1's `COPY OK`; the staged `Enum.lua` carries the inserted rows.
- **Logs.** `logs.py --tag STATE_IDS` now reads 250 lines (S1 counted 274), same picture: states 50-68 `from 0` for classes 100-122 (the
  `SnapshotBeforeSlotRebind` lookup at `X2GUUser.cpp:7611` before the slot's Init registers it, checked), class 103 state 4 from 8, class 109 state 242.
  No line for an inserted id, none from a failed state change. Not defects.
- **Enumscan.** Both `stray` verdicts are right: `NUI_NEIL_THE_SNIPER_EVENT` / `NUI_PROTON_BOMB_EVENT` are the definitions of June's names (C9h-4h), and
  `GetUserUnitRelativePos` returns a plain `int` with no enum (`X2GUNPC.h:3394`), so `case 1..4` is not an NPC id. The initializer skip in
  `enumscan.py` only marks a `{` opened after `enum` / `enum X` / `enum class X`.
- **Routed, not this plan (found here).** Eve's class enum is out of step with June's exe beyond state 176, before and after S1: `ENSI_COMBO_XXZ4` June
  0xbe (190) / C++ 189, `ENSI_ECN_COMBO_ZZX_LOOP` 232 / 229, `ENSI_DASH_COMBO_ZX` 180 / 241 (one member missing, one moved; other classes not checked).
  No numeric Eve id is sent or compared in offline play, so the play-test passed. Appended to the Stage 2 training-dungeon bullet in
  `JUNE_PORT_BACKLOG_ORDER.md` (the training scripts are the only numeric readers).
- **Gates.** `finish.py --review --commits 61d8b2a2 --skip-flag JUNE_RESERVED_STATE_IDS / JUNE_USER_STATE_ID16 / JUNE_SKILL_STATE_PTR (outer blocks) -v`:
  flagcheck JUNE_USER_STATE_IDS PASS (3 PASS, 1 DEFINE, 0 OTHER, 0 FAIL, 0 deleted lines; 6 bare `#else` / `#endif` = the training UI's 3 `#else` and the
  3 `#error` guards' `#endif`), junediverge PASS, junediverge `--check-all` PASS, enumscan PASS: `FINISH RESULT: PASS (10 gates, 0 failed, 6 skipped)`.
  Retire: `finish.py --files KTDXLIB/Always.h`: `JUNE_USER_STATE_IDS_DEBUG retired`, `FINISH RESULT: PASS`.
- **Build skipped: debug retire only.** The next built phase drops the `[STATE_IDS]` line from the exe.
- **Next:** S2 ready (`kickoffs/june-state-ids/S2.md` checked).

## S2 - Lu/Ciel on the shared numbers

**Scope.** In `X2GULuciel.*` (inert `_CORE`), after S1:
1. Drop the shadow copies June's names now share with the base. CLAUDE.md: when March's and June's names share a value, March's is used. These are
   `USI_SPECIAL_ATTACK_1` .. `_SLOTB_HYPER_5` (50-69; CH1n) and any other `LUCIEL_STATE_ID` member that only re-declares a `CX2GUUser` name.
   Prove each removed value equals the base value with `enumprobe.py`.
2. List the `LSI_*` members that now equal base members: riding 72-92 (`LSI_RIDING_*`), event kit 110-119, bounding 120-122, `LSI_WAIT_STATE_CHANGE`
   130, the slot states. Keep June's `LSI_` names, since they are June's Lu table. The one-line comment says they equal `USI_*`.
3. **`InitBoundingStates` / `InitWaitStateChange`.** CH1m registered Lu's bounding rows (120-122) and the wait change (130) inline because March's ids
   were 102-104 / 105. Compare June's namesakes (the bounding vslots 0x5d4-0x5e4 Lu calls, and June's wait-change helper) with March's bodies in Ghidra.
   If they match (junediverge rule), Lu calls them: `m_bBoundingStatesReady` is set and RT_BOUNDING works. Ciel's bounding (417-419) stays inline,
   and the flag `m_bBoundingStatesReady` is set the way June sets it. If they differ, add a divergent-table row and the replacement in the same commit,
   or route it.
4. Re-check `StateChange( 263 )` / `( 268 )` and the member-scope slot tables (`X2GULuciel.cpp:14224-14227`) against the base tables: after S1 the two
   must agree.
5. In `Always.h`, add the guard `#error SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE needs SERV_IRUHADEV_JUNE_USER_STATE_IDS`, confirmed with `cl /EP`.
6. Close the CH1m / CH1o seam notes: amend char-luciel `CH1.md` (nothing left for the engine id seam) and `CH1o2.md` / `CH1o3.md` if they name the seam.

**Run with.** Opus 5.5 / high (Ghidra comparison). No review of its own: char-luciel CH1's review covers it (inert class code; `CH1.md` amended).
**Read list.** This doc: "Census", "S2"; char-luciel `JUNE_CHARACTER_LUCIEL_PHASES.md` "CH1m - Done (2026-11-15)", "CH1n - Done (2026-11-15)".
**Done gate.** `finish.py --files X2GULuciel.cpp X2GULuciel.h --state ...` PASS; `/Zs` probe of `X2GULuciel.cpp` plain and with
`/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`, 0 errors. Full build skipped: no play-test (Lu goes live in CH1).

### S2 - Done (2026-11-15)

Opus 5.5 high. Code commit `857b18cd` (inert `_CORE`; `_LIVE` stays `//#define`). No debug define. No review of its own: char-luciel CH1's review covers it.
- **Item 1, shadows:** the 20 `USI_SPECIAL_ATTACK_*` members CH1n put in `LUCIEL_STATE_ID` are dropped (`X2GULuciel.h`, one comment line in their place).
  `enumprobe.py`: `CX2GUUser::USI_SPECIAL_ATTACK_1` .. `_SLOTB_HYPER_5` = 50 .. 69, the dropped values. No other `LUCIEL_STATE_ID` member re-declared a base name.
  In-class readers (the setter's four slot tables, `X2GULuciel.cpp:14200`) now resolve to `CX2GUUser::`; the `/Zs` probes prove it.
- **Item 2, LSI_ members equal to base:** every Lu member from 25 to 130 now equals a `USI_*`: damage 25-42, `LSI_RIDING_HYPER_MODE` 44, pepper run 45-49,
  `LSI_LU_THROW_ITEM` 70, riding 72-96, bounding 120-122, phoenix 123 / 126, dance 127-129, `LSI_WAIT_STATE_CHANGE` 130 (census tsv by value; probed:
  `USI_THROW_ITEM` 70, `USI_RIDING_START` 72, `_ATTACK_X_END` 96, `USI_BOUNDING_*` 120-122, `USI_DANCE_WAIT` 129, `USI_WAIT_STATE_CHANGE` 130). June's `LSI_`
  names kept; the enum's one-line head comment says so. Lu has no members for 24, 43, 71, 97-119, 124-125, 131-151 (June's Lu table has none).
- **Item 3, helpers (Ghidra listings `june2017_re/char_luciel/ghidra/`):** June's Lu registers bounding at `32b3e8b0` @`32b3f302` / @`32b3f381` / @`32b3f41b`
  (ids 0x78-0x7a, tables `LSI_LU_DAMAGE_BOUNDING_*`) through thunks `329cc0b0` / `329cc150` / `329cc1e0` + `329cc050` + `329cc0c0` = vslots 0x5d4-0x5e4, the
  same thunks every class uses (funcmap: Eve CH1b rows), which March's `InitBoundingStates` registers by name. Wait change: `32b43d10` @`32b44174`, id 0x82,
  thunk `329cb720` -> `31fc8620` = `WaitStateChangeEventProcess`, as `InitWaitStateChange`. **Same: no divergent row.** Lu now calls
  `InitBoundingStates( "LSI_LU_" )` and `InitWaitStateChange( "LSI_WAIT_STATE_CHANGE" )`; the first sets `m_bBoundingStatesReady` once Lu's three tables load,
  so `RT_BOUNDING` works for Lu (June has no such flag; it is March's guard, as for the nine classes). Ciel's 417-419 stay inline (`GetStateIDByForm` maps them).
  Kept on purpose (all classes): the helpers register nothing and log once when a table is missing (June registers anyway).
  `ch1m_regcheck.py` now expands the two calls into the rows the helpers register: `matched 337 registrations, 0 problems` (unchanged).
- **Item 4:** the setter's slot tables now name the same `CX2GUUser::USI_SPECIAL_ATTACK_*` members as the engine's (`X2GUUser.h:6034-6044`); 50-69 on both sides.
  `StateChange( 263 )` / `( 268 )` are class-range ids above `USI_END`, untouched by S1. `InitEventStates` registers 110-118 (probed), so it no longer
  overwrites `LSI_RIDING_SPECIAL_MOVE` 92 (CH1m's finding closed).
- **Item 5:** `Always.h` guard `#error SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE needs SERV_IRUHADEV_JUNE_USER_STATE_IDS`, placed after S1's define (13530). Proved by the
  `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` probe passing it.
- **Item 6:** char-luciel `CH1.md` and `CH1o3.md` amended (seam closed); `CH1o2.md` does not name it. STATUS CH1 row: the `june-state-ids S2` dependency is done.
- **Gates (worktree):** `finish.py --files X2GULuciel.cpp X2GULuciel.h --state ... Luciel_Duo --state ... Luciel_Demonio`: flagcheck JUNE_CHAR_LUCIEL_CORE PASS
  (2 PASS, 0 OTHER, 0 FAIL, 48 deleted lines = the 20 shadows + 2 comment lines in `.h`, the 6-line wait-change and 20-line bounding rows in `.cpp`, all inside the
  `_CORE`-only file), flagcheck JUNE_ENUM_NAMES PASS (2 OTHER = `_CORE`'s), junediverge PASS, enumscan PASS, stateends PASS both: `FINISH RESULT: PASS (8 gates,
  0 failed, 2 skipped)`. `finish.py --files KTDXLIB/Always.h --skip-flag JUNE_USER_STATE_IDS "guard target"`: JUNE_CHAR_LUCIEL_LIVE INERT (1 PASS), junediverge /
  enumscan PASS: `FINISH RESULT: PASS (7 gates, 0 failed, 4 skipped)`.
  Why two runs: flagcheck cannot evaluate an existing `Always.h` guard `#if defined( A ) && !defined( F )` when F is the flag checked (A unknown), so with
  `Always.h` in the set `_CORE` (guard at 13430) and `_ENUM_NAMES` (10175) read ERROR on lines this phase did not touch.
- **Gates (range `857b18cd^..857b18cd`, same split):** flagcheck `_CORE` PASS (2 PASS, 48 deleted), `_ENUM_NAMES` PASS (2 OTHER), `_LIVE` 1 PASS, not defined (INERT);
  `finish.py --range` junediverge / enumscan / stateends PASS.
- **Compile:** `/Zs` probes (census `cl_cmd`, no PCH) of `X2GULuciel.cpp` plain and with `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`: exit 0, 0 errors, 0 class warnings.
  **Full build skipped: no play-test.**
- **How to see it in game:** nothing yet (inert until char-luciel CH1). CH1's play-test can show it: Lu returns from the win pose after a dungeon result
  (`USI_WAIT_STATE_CHANGE`; Add stays in it until char-add), and `offline_server.log` has `JUNE_FX C3d-3 bounding states registered for LSI_LU_`.
- **Next:** `/x2-phase june-state-ids Z` (Sonnet 5.5 medium).

## Z - Close

**Scope.** `MODS.md` row (flag, the `Enum.lua` coupling, how to revert); backlog Stage 41 closed; `STATUS.md` block to `STATUS_ARCHIVE.md` plus a
*Closed plans* row; `tools/README.md` row for `stateids.py`; checklist row in `JUNE2017_CHARACTER_PORT_CHECKLIST.md` (new characters register at June's
ids with no shadow names; run `stateids.py check`). Stage-end census (`junebinary.py --check`, `v2_exclusions.py`).
**Run with.** Sonnet 5.5 / medium. **Done gate.** Both census tools `RESULT: PASS`; STATUS archived.
