# June dungeons - C3y-2 review - the March classes' June tutorial dungeons

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-dungeons C3y-2 review` |
| Plan doc | `JUNE_DUNGEONS_PHASES.md` (branch `main`) |
| Depends on | C3y-2 done and its play-test passed |
| Flags | `SERV_IRUHADEV_JUNE_CLASS_TUTORIAL_DUNGEON` (+ `_DEBUG`), `SERV_IRUHADEV_JUNE_TUTORIAL_LUA_BIND`, `SERV_IRUHADEV_JUNE_TUTORIAL_PLAY_GUIDE` |
| Log tag | `JUNE_FX C3y-2` (offline_server.log; `python tools/logs.py --grep "JUNE_FX C3y-2"`) |

## Read first

```sh
python tools/docsection.py JUNE_DUNGEONS_PHASES.md "C3y-2 - Stopped (2026-11-13, Opus 5.5): blocked on C3z-4t" "C3y-2 - Done"
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
2. **Data:** spot-check 3 of the 9 DungeonData rows, 5 of the 41 NPCTemplet rows and the 9 WorldData rows (keyed by `worldID`; Eve's
   trailing-space name kept) against `june_src_fixed`; `python tools/luabind.py check` over the 9 scripts = PASS; `luasrc.py ledger --check`.
3. **Hygiene:** `python tools/flagcheck.py <FLAG> --range <first C3y-2 commit>^..HEAD` for each flag (paste the RESULT lines; explain OTHER
   rows), `python tools/junediverge.py --check-all`, `python tools/enumscan.py --check` on the changed files.
4. Retire `SERV_IRUHADEV_JUNE_CLASS_TUTORIAL_DUNGEON_DEBUG` (comment out the define; no build: debug retire only).
5. Write "C3y-2 - Review" into the plan doc, update STATUS.md (C3y-2 to Done; char-add TU / char-luciel TU now only wait on their own
   phases), check the next kickoffs, commit.
