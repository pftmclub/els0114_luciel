# June dungeons - C3y-2c - the tutorial's GFx talk scenes: a click replays the line, no Banthus portrait

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-dungeons C3y-2c` |
| Plan doc | `JUNE_DUNGEONS_PHASES.md` (branch `main`) |
| Depends on | C3y-2b (play-test with its fix 3 build). Touches the GFx event-scene runtime (`X2GFXEventScene.*`, `KTDXLIB/KTDGGFXManager.*`, june-gfx G-phase code): check `git status` for peers first |
| Flags | a new `SERV_IRUHADEV_JUNE_*` flag per logical fix; a temporary `_DEBUG` flag for the scene trace (the C3y-2 review retires it) |
| Log tag | `[JUNE_GFX] SCENE` (ErrorLog.txt; `python tools/logs.py --grep "JUNE_GFX\] SCENE"`) |
| Review | folded into `C3y-2-review.md` (amend it to cover C3y-2c) |

## What the user saw (2026-11-13, new Elsword, tutorial "Ruben Forest", sub-stage 2)

1. The dialog mentioning Banthus **replays when you left click** instead of closing at its last line.
2. The dialog names Banthus but **no Banthus portrait** shows.

3. (user, 2026-11-13, this phase) One left click on a line skips several lines.
4. (user, 2026-11-13, this phase) After the boss dies a scene shows only black until ESC / right click; then the quest dialog shows.

## What C3y-2b found (logs of that run are overwritten; re-run first)

- Sub-stage 2 plays the GFx event scenes `Tutorial_Elsworld_03` (after the boss: slide shot `PF_STAGE0_SUB2_SHOT`, then `PlayEventScene`)
  and `Tutorial_Elsworld_04` (after the `NUI_TUTORIAL_ELSWORLD_DP2` direct NPC dies). Both are SWF-only scenes: no `GFXEventSceneInfo.xml` or
  region-XML row in March or June (`getEventSceneData: no row for ...`), so the SWF's own timeline and ActionScript drive the lines.
- `data\eventscene\` is byte-for-byte June's (same file set). `tutorial_elsworld_03.swf` (CWS) imports `T_NPC_Peita_PNG.swf`,
  `PC_Aisha_PNG.swf`, `PC_Rena_PNG.swf`, `PC_Elsword_PNG.swf`, `Emotion_PNG.swf`, `T_Balloonword_PNG.swf`, `T_BG_Enter_PNG.swf`; its symbol
  strings name `Peita_Berthe_Body01-05.png`, `stand_aisha_body_png`, `stand_elsword_body_L/R_png`, and no Banthus symbol. It also carries
  `EventScene.nextScene`, `nextEventScene`, `endEventScene`, `endEventSceneAni`, `notifyClickClose`, `appendText`.
- March's click path: `CX2GFXEventScene::MsgProc` (June `32269e00`) WM_LBUTTONUP -> `FireNextScene` (`32e59800`, binding
  `EventScene.nextScene`); Enter does the same in `OnFrameMove` (`3226c120`); RMB-up / ESC held 0.6 s -> `End( true )` ("End ( forced )").
  The bridge handles four AS calls: `endEventScene`, `playEventSceneSound`, `getEventSceneData`, `playedEventSceneAndGetMent`.
  `notifyClickClose` in June's binary belongs to another widget (`32ec6d40`, `gameDataFunctionInit` table), not the scene.

## Task

1. Re-run the sub-stage 2 scenes with a `_DEBUG` trace on every bridge call, binding fire and frame change (throttled), and find what the
   SWF expects at its last line (an AS call the bridge ignores, a binding value, `endEventSceneAni`, or a double click delivery: MsgProc plus
   Scaleform's own mouse input). Compare with June's handlers in Ghidra (`3226b6c0` End, `32e59*` bridge, `32269e00` MsgProc).
2. Find where June gets Banthus's portrait for these scenes (another imported library, a frame label, or none: then June shows none too and
   it is a `JUNE_BUGS` row, not a fix).
3. Fix under its own flag, June addresses cited; build, deploy, hand off the sub-stage 2 play-test (new Elsword, then new Elesis).
