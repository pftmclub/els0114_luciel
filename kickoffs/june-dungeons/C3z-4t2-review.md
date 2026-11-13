# June dungeons - C3z-4t2 review - the tutorial guide manager ( t1 ) and its GFx movie ( t2 )

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-dungeons C3z-4t2 review` |
| Plan doc | `JUNE_DUNGEONS_PHASES.md` (branch `main`) |
| Depends on | C3z-4t2 done and its play-test passed (C3z-4t1 play-test passed 2026-11-13) |
| Flags | `SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE` (+ `_DEBUG`), `SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_GFX` |
| Log tag | `JUNE_FX C3z-4t` (offline_server.log) |

## Read first

```sh
python tools/docsection.py JUNE_DUNGEONS_PHASES.md "C3z-4t - Done (scope) (2026-11-13, Opus 5.5 high): 4 port phases, T1 first" "C3z-4t1 - Done" "C3z-4t2 - Done"
grep "C3z-4t" june2017_re/funcmap.tsv
python tools/junediverge.py --list
```

Map: `june2017_re/c3z4t_scope.tsv`. Code: `X2Lib/X2TutorialGuideManager.*`, `X2Lib/X2TutorialGuideAdapter.*`, the `_GFX` blocks of
`KTDXLIB/KTDGGFXManager.*`, and t1's hooks in `X2Main.*`, `X2Game.h`, `X2StateStartUp.cpp`, `X2DungeonGame.cpp`, `KTDXLIB/KTDIDevice.h`.

## Task

1. **t1, against the disassembly:** the 52 table keys of `31fbe4a0` (spot-check 6: types, defaults, the scene offset each lands in);
   `CanBeginGuide` `31fc4530` (the order of the blocks, the dungeon exception); the step `31fc4060` and end `31fc4c40` (what each writes);
   the key wait `31fc6130` (allowed keys ESC + F2 through `DIVERGE_GET_KEY_STATE`); the 9 Lua methods (binder `31d95080`, names and order).
2. **t2, against the disassembly:** ctor `32965bd0` (listener name, the two binding names, the Int value object `334c909c` = VT_Int),
   show `32965a60` / hide `329659b0` (the binding test before the set, the notify either way), dtor `329659e0`, the callback `32965a90`
   (answered true, function object released at once), `31fc3c20` (adapter made even without the root movie), vslots `31fbdb30` / `31fbdb60`
   (no March caller, no delete in `31fbdb60`), and JUNE_BUGS row 643 (the dtor leak kept verbatim).
3. **The moved load line:** `JUNE_FX C3z-4t load 24 scene(s) ... (logged at the first begin)` is in the play-test log once, before the
   first `begin`.
4. **Hygiene:** `python tools/flagcheck.py SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE --range 7f27edc3^..HEAD --with SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG`
   and `python tools/flagcheck.py SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_GFX --range <C3z-4t2 commit>^..HEAD`; paste the summary lines. The
   new files' first line (`#include "StdAfx.h"` / `#pragma once`) FAILs and the `_DEBUG` `#error` guard ERROR are house precedent;
   anything else is a finding. `python tools/junediverge.py --check-all` = `RESULT: PASS`; `enumscan.py --check` on the changed files = 0 A/B.
5. **Retire the debug lines:** comment out `#define SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG` (it also carries t2's show / hide / callback
   lines). The always-on failure lines (`load failed`, `gfx adapter unbound`, `gfx show %d not seen`, the first-begin count) stay.

Fix clear transcription errors under the same flag (then a full build and a play-test); hand design decisions back to the user. If the
only code change is commenting out the `_DEBUG` define, start the build and do not wait for it (Done note: "not awaited"). Write
"C3z-4t2 - Review" into the plan doc, update both STATUS rows (C3z-4t2 to Done, C3z-4t3 to ready), check `kickoffs/june-dungeons/C3z-4t3.md`,
and commit.
