# June dungeons - C3y-2 review - the March classes' June tutorial dungeons (covers C3y-2b)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-dungeons C3y-2 review` |
| Plan doc | `JUNE_DUNGEONS_PHASES.md` (branch `main`) |
| Depends on | C3y-2, C3y-2b, C3y-2c and C3y-2d done, and their play-tests passed (amended 2026-11-14: this review covers C3y-2b / 2c / 2d; Flags row gains `SERV_IRUHADEV_JUNE_PARTICLE_LUA_LAYER`, `SERV_IRUHADEV_GFX_SCENE_CLASS_RELEASE` and any C3y-2d flag) |
| Flags | `SERV_IRUHADEV_JUNE_CLASS_TUTORIAL_DUNGEON` (+ `_DEBUG`), `SERV_IRUHADEV_JUNE_TUTORIAL_LUA_BIND`, `SERV_IRUHADEV_JUNE_TUTORIAL_PLAY_GUIDE`; C3y-2b: `SERV_IRUHADEV_JUNE_SCENE_END_HOOK`, `SERV_IRUHADEV_JUNE_TUTORIAL_ROOM_LOADING` |
| Log tag | `JUNE_FX C3y-2` (offline_server.log; `python tools/logs.py --grep "JUNE_FX C3y-2"`) |

## Read first

```sh
python tools/docsection.py JUNE_DUNGEONS_PHASES.md "C3y-2 - Stopped (2026-11-13, Opus 5.5): blocked on C3z-4t" "C3y-2 - Done" "C3y-2b - Done"
grep "C3y-2" june2017_re/funcmap.tsv
grep -n "C3y-2" tools/data/june_march_divergent.tsv tools/data/june_diverge_allow.tsv
python tools/junediverge.py --list
```

Code: the three flags' blocks in `X2StateServerSelect.cpp`, `X2StateBeginning.cpp`, `DungeonEnum.h`, `X2Main.cpp`, `X2Game.*`, `X2GUUser.h`,
`X2DungeonGame.*`, `X2StateDungeonGame.cpp`, `X2PlayGuide.*`, `X2Lib/Offline/{X2OfflineQuest,Handlers_Room,Handlers_Skill}.cpp`.

## Task

1. **Against the disassembly:** the room switch `325622e0` (unit types 2-9 and default); `PopTalkBox` `32305f00` (the TalkBox fields,
   `( p4 == 0 )` -> unit type, JUNE_BUGS 644); `IsOnSomethingNow` `31e4d0a0` (+0x80bc = Now's foot-on-line); `ShowJumpGuide` `327ca2c0`;
   the jump guide `3289e030` / `3289e270` / `3289cfe0` / `3289e370` / `3289d1d0` (gates, layer, sound, 80 px, the 2.0 s hide); June's
   `OnFrameMove_Tutorial` `3289e180` and `SetPostionInputGuide` `3289df90` (no create); the Release hide `327d2160` @`327d21c7` and the two
   delete sites (`327d2240` @`327d25f3`, `324ce714` @`324cea74`). Spot-check 3 directly in Ghidra.
   **C3y-2b:** the two scene methods (binder @`31de31c0` / @`31de344f`, bodies `327ccad0` / `327ccb20`), `CX2SlideShot::MsgProc` `325c74b0` and its
   call @`31d8e5ce`, Lua `ScenEnd` `325c74a0`, `CX2Main::OnSceneEnded` `31d87420` (the `323ca2a0` stand-in) and its GFx call @`3226be30`, the
   room's tutorial branch `324e6130` @`324e79a3`-`324e7a71`; the `CX2SlideShot::ScenEnd` divergence row routed to Stage 9.
2. **Data:** spot-check 3 of the 9 DungeonData rows, 5 of the 41 NPCTemplet rows and the 9 WorldData rows (keyed by `worldID`; Eve's
   trailing-space name kept) against `june_src_fixed`; `python tools/luabind.py check` over the 9 scripts = PASS; `luasrc.py ledger --check`.
3. **Hygiene:** `python tools/flagcheck.py <FLAG> --range <first C3y-2 commit>^..HEAD` for each flag (paste the RESULT lines; explain OTHER
   rows), `python tools/junediverge.py --check-all`, `python tools/enumscan.py --check` on the changed files.
   **C3y-2c / 2d:** read "C3y-2c - trace run 1 ... fix 1", "C3y-2c - run 2 ... fix 2" and "C3y-2d - fix 3" in the plan doc; against the
   Scaleform source (`Libs/ExternalLib/Scaleform/Src/GFx/AS3/AS3_VM.cpp` `RegisterUserDefinedClassTraits`, `FindVMAbcFileWeak`,
   `GetUserDefinedTraits`; `AS3_MovieRoot.cpp` `ExecuteAbc`) check `CKTDGGFXUI::PrepareSceneClasses`: owner lookup, the `_fla` exclusion, release /
   restore, the clear before the movie goes. `flagcheck --range` for `SERV_IRUHADEV_GFX_SCENE_CLASS_RELEASE` shows fix 2's 19 lines deleted inside the flag (expected).
4. Retire `SERV_IRUHADEV_JUNE_CLASS_TUTORIAL_DUNGEON_DEBUG` and C3y-2c's `SERV_IRUHADEV_JUNE_GFX_SCENE_TRACE_DEBUG` (comment out the defines; no build: debug retire only).
5. Write "C3y-2 - Review" into the plan doc, update STATUS.md (C3y-2 to Done; char-add TU / char-luciel TU now only wait on their own
   phases), check the next kickoffs, commit.
