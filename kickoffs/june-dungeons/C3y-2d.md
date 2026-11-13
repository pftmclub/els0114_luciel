# June dungeons - C3y-2d - GFx scenes run the first scene's code (wrong voices, fast typing, end scene loops)

| | |
|---|---|
| Start | new conversation on **Fable 5.2, high** effort (user, 2026-11-14), then type `/x2-phase june-dungeons C3y-2d` |
| Plan doc | `JUNE_DUNGEONS_PHASES.md` (branch `main`) |
| Depends on | C3y-2c (fix 1 kept; fix 2 deployed but its lookup never matched). Touches `KTDXLIB/KTDGGFXManager.cpp` and maybe the Scaleform runtime glue: check `git status` for peers first |
| Flag | `SERV_IRUHADEV_GFX_SCENE_CLASS_RELEASE` (C3y-2c fix 2, not June): fix it in place, or retire it for a better flag; `SERV_IRUHADEV_JUNE_GFX_SCENE_TRACE_DEBUG` stays on |
| Log tag | `[JUNE_GFX] SCENE` (ErrorLog.txt; `python tools/logs.py --grep "JUNE_GFX\] SCENE"`) |
| Review | folded into `C3y-2-review.md` (it covers 2b / 2c / 2d) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_DUNGEONS_PHASES.md "C3y-2c - trace run 1 (2026-11-13) and fix 1" "C3y-2c - run 2 (2026-11-13) and fix 2" "C3y-2c - run 3 (2026-11-14)"
```

## What the user sees (all GFx talk scenes, any quest; clearest in the tutorial)

1. A scene plays **another scene's voices** (an Ignia line in the Ruben tutorial).
2. Text **types too fast** and lines jump on before the white box has finished appearing.
3. The tutorial's end scene (`Tutorial_Elsworld_03`, 15 lines) **does not end**: it wraps to line 1 until right click / ESC.

## What C3y-2c proved (runs 2 and 3)

- GFx advance speed is right (1.00x movie time per real second). Clicks are clean (one `FireNextScene`, one observer per scene).
- **Every scene runs the class code of the first scene played that session.** Each scene swf's document class is `EventScene` (plus
  `SoundPlayer` and its symbol classes). Run 2: the first scene was `Quest_22160_Start` (7 voice labels `GFX_ID_2371..2377`, 8 frames), and every
  tutorial scene called exactly those voices; `_03` (15 frames) got 22160's 8 frame scripts and none for frames 9-15, so its timeline ran on and
  wrapped. Run 3: the first scene was `Quest_11000_ELESIS_End`, and every later scene played its voices (`GFX_ID_3854`, `2876`, `3855`, `301`, `3856`).
- Why: `Libs/ExternalLib/Scaleform` carries two local 2026-08-27 patches (ElswordPS): `LoaderInfo::SetAppDomain` puts every loaded widget in the
  frame's app domain (shared; `VMAbcFile::RegisterUserDefinedClassTraits` skips a name that exists), and `MovieRoot::UnloadMovie` skips the
  teardown for content reparented out of its loader (main.gfx's `SubWidgetLoader` always reparents), so stage listeners, timers and
  `RemoveLoadedMovieDef` never run and a scene is never freed (its `EventScene.config` adds a stage `KEY_DOWN` listener). `libgfx_as3.lib` is
  prebuilt and untracked with no project files here, so the lib cannot be rebuilt in this tree.
- **Fix 2 does nothing:** before each 0x2711 load it resolves `EventScene` with `VM::Resolve2ClassTraits( name, GetPublicNamespace(),
  GetCurrentAppDomain() )` and unregisters the owning file's classes, but every run logs `SCENE CLASSES none registered ( no EventScene )`. So the
  scenes' classes are not where that lookup searches (another domain: the domain of main.gfx's ABC file, a child of `CurrentDomain`? a different
  namespace object? `GetRegisteredClassTraits` semantics?).

## Task

1. Find where a scene swf's `EventScene` class is really registered (domain object, namespace) and why the fix-2 lookup misses it. Read
   `Src/GFx/AS3/AS3_VM.cpp` (`VMAppDomain::GetClassTrait` / `AddClassTrait`, `VM::GetRegisteredClassTraits`, `GetFrameAppDomain`), `AS3_MovieRoot.cpp`
   (the load path ~3590-3620: `info->applicationDomainGet()`), and `Obj/Display/AS3_Obj_Display_LoaderInfo.cpp` (`SetAppDomain`). A cheap way to be
   sure: log the domain pointers (`CurrentDomain`, the root file's domain, main.gfx's `GameDataFunction` class file's domain) under the trace flag.
2. Make each scene use its own classes. Preferred: keep fix 2's shape (release the previous scene's classes before the next scene loads) with the
   right domain, for example resolving through the domain of the file that owns a main.gfx class. Alternatives if that is unsafe: free the scene
   properly (the teardown the `UnloadMovie` patch skips) or give scene swfs their own child domain. Any Scaleform-internal access stays inside
   `KTDGGFXManager.cpp` (DXUT's `V` macro is already pushed around the `AS3_MovieRoot.h` include). Keep the leftover-instance guard
   (`SERV_IRUHADEV_JUNE_GFX_SCENE_STALE_INSTANCE`) as it is.
3. Done gate (logs): one `SCENE CLASSES released ...` line naming the previous scene before each scene after the first; tutorial scenes call
   `Tuto_*.ogg` voices (not `GFX_ID_*`); `Tutorial_Elsworld_03` ends `by the scene` after its 15th line. Play-test: existing character in the village
   first (a quest scene plays), then a new Elsword through the tutorial, then a new Elesis.
