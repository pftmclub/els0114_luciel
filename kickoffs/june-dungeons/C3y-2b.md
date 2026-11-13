# June dungeons - C3y-2b - fix the class tutorial dungeons' first play-test (input lock, slide-shot skip, loading flash, assets)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-dungeons C3y-2b` |
| Plan doc | `JUNE_DUNGEONS_PHASES.md` (branch `main`) |
| Depends on | C3y-2 (built, deployed; first play-test 2026-11-13 failed, see below). Shares `X2Main.cpp` / the tutorial guide with C3z-4t3 (play-test owed): check `git status` first |
| Flags | C3y-2's (`SERV_IRUHADEV_JUNE_CLASS_TUTORIAL_DUNGEON` + `_DEBUG`, `_TUTORIAL_LUA_BIND`, `_TUTORIAL_PLAY_GUIDE`); a new `SERV_IRUHADEV_` flag per logical fix |
| Log tag | `JUNE_FX C3y-2` (`python tools/logs.py --grep "JUNE_FX C3y-2"`) |
| Review | folded into `C3y-2-review.md` (amend it to cover C3y-2b) |

## What the user saw (2026-11-13, new Elsword then new Elesis)

1. After creation, the **old** tutorial loading screen shows for about a second, then June's loading screen.
2. In the dungeon the character **cannot move**.
3. The opening story (slide shot) **cannot be skipped** with ESC / right click.

## What the logs already show (run 12:39:50, exe md5 62a1a621)

- **Input lock (cause found):** ErrorLog `[LUA_SRC] can't find 'IsRecentlyPlayedEventScene'` x1114 and `'PlayEventScene'` x1. June's scripts call
  `slot1:PlayEventScene( "Tutorial_Elsworld_01" )` after `slot1:CannotInput( 9999 )` and wait on `slot1:IsRecentlyPlayedEventScene(...)`
  (`Dungeon_Tutorial_Elsword.src.lua:341-353`, same in every class script). June binds both as **CX2DungeonGame methods** (binder `31d95080`,
  names @`31de30c8` / @`31de3297`, the same class block as `ShowJumpGuide` @`31de34c7`) -> `327ccad0` / `327ccb20`. March binds them only as
  globals (`X2Main.cpp:5683-5684`, `GfxPlayEventScene_LUA` / `GfxIsRecentlyPlayedEventScene_LUA`, `X2GFXEventScene.cpp:637` / `:646`), so the
  scene never plays, the condition never passes and input stays locked for 9999 s. Fix: `class_def<CX2DungeonGame>` for both, members that
  call the existing globals (confirm in Ghidra that `327ccad0` / `327ccb20` ignore `this`). Then check every other `slot1:` call in the 9
  scripts against the class June binds it on.
- **Gate gap behind it:** `tools/luabind.py` matches method names only, so a global with the same name hid both misses (its own docstring
  says so). Make `check` report a `slot1:X` call whose only March binding is `lua_tinker::def` (global) or another class; re-run it on the 9.
- **Also seen, check the cause:** offline_server.log `JUNE_FX C3z-4t gate input mask: guide 0, value -1` once a second for the whole
  tutorial while no guide ran (C3z-4t3's gate). Rule it in or out for the lock (owner: C3z-4t3; report, don't edit its code).
- **Slide-shot skip:** the story is the slide shot `PF_STAGE0_SUB0_SHOT0..3` (`X2SlideShot.cpp:131` logs `m_SceneName = PF_STAGE0_SUB0_SHOT0`);
  each shot advances on `slot1:IsSkipKeyPressed() == true` (`Dungeon_Tutorial_Elsword.src.lua:594-688`). Find why it never reads true:
  March's `CX2Game::IsSkipKeyPressed` key set vs June's, the tutorial default key map (`[JUNE_BUFF] tutorial keys: default key map applied`,
  June `31f5e290`), the guide manager's allowed-key list (`SetUseAllowedKeyList`), C3z-4t3's ESC gate.
- **Loading flash:** find what shows the old battle-field tutorial loading screen before the room ACK (the ServerSelect -> dungeon-room
  path, a cached `m_LoadingScreenFileName`, or a hard-coded tutorial loading image) and what June shows there.
- **Assets:** this run logged `OpenXET` failures for the tutorial worlds (`TUTORIAL_ELSWORD_STAGE0_*.XET`, ...) and missing textures
  (`FIELD_RUBEN_LOTUS04.DDS`, `RUBEN_FIELD_EL_TREE_TOP_STONE02/03/04.DDS`, ...). C3y-2's census only followed quoted names. List the
  names this run's ErrorLog reports for the 9 tutorial worlds and their NPCs, stage what June has (`kom.py stage-june`), list what June
  lacks too, pack + copy.

## Read first

```sh
python tools/docsection.py JUNE_DUNGEONS_PHASES.md "C3y-2 - Done (2026-11-13, Opus 5.5 high): June's class tutorial dungeons for the March classes"
python tools/logs.py --grep "C3y-2|C3z-4t|EventScene|SlideShot|tutorial keys" --max 60
LC_ALL=C.UTF-8 grep -a "327ccad0\|327ccb20" june2017_re/funcmap.tsv
```

## Task

Fix the four items above in that order, each under its own flag (or inside C3y-2's when it is a missed piece of the same port), June
addresses cited, June bugs kept with a `JUNE_BUGS.md` row. Full build + deploy, then the same play-test as C3y-2 (new Elsword + new
Elesis: loading screen, opening slide shot skippable, character moves, tutorial to the village, relog).
