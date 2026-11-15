# June character Lu/Ciel (Stage 8) - Phase TU review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel TU review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | TU done and its play-test passed (its Done note names the commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_TUTORIAL` (+ `SERV_IRUHADEV_JUNE_ENUM_NAMES` for the one `DungeonEnum.h` line) |
| Log tag | `[JUNE_CHAR]` (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL TUTORIAL"`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "TU - Done"
python tools/docsection.py JUNE_DUNGEONS_PHASES.md "C3y-2 - Done (2026-11-13, Opus 5.5 high): June's class tutorial dungeons for the March classes"
```

## Task

Review TU's commit against June in Ghidra (read-only) and against the live data:
- The room switch `325622e0`: unit type 10 -> `0xa118` = 41240 (`DI_DUNGEON_TUTORIAL_LUCIEL`); the case sits beside C3y-2's, March's
  `CX2StateServerSelect::Handler_EGS_CREATE_TUTORIAL_ROOM_REQ` body is otherwise unchanged, the `X2StateBeginning.cpp` copy is compiled out.
- Data = June's rows verbatim (`DungeonEnum.src.lua:101`, `Enum.src.lua:2357` / `4006` / `4890` / `4918` / `5825`, `DungeonData.src.lua:29877`,
  `WorldData.src.lua:3153`, the four `NPCTemplet.src.lua` rows), no value collision in live `Enum.lua`; `luabind.py check` of the five scripts = PASS.
- Assets: 214 names staged (data081 / 160 / 161 / 164); the 7 Lu/Ciel tutorial `.lua` were already June bytes; data160 / data161 are new koms.
- The play-test: every check has a verdict; the end of the tutorial lands where June lands (the village), Elsword's tutorial is unchanged.
  Open items routed: the Rose / Ain stand-in comment stays (their ports), `DI_BATTLE_FIELD_TUTORIAL_LUCIEL` 39609 is not needed (June has no row).

Hygiene: `python tools/finish.py --review --commits <TU's code commit> --with SERV_IRUHADEV_JUNE_CHAR_LUCIEL_TUTORIAL SERV_IRUHADEV_JUNE_ENUM_NAMES --skip-flag SERV_IRUHADEV_JUNE_CLASS_TUTORIAL_DUNGEON "C3y-2's room switch, the target of one #error guard"`.
Explain every OTHER row (`-v`) and the Always.h `#error` ERROR row (the guard, precedent). Spot-check 2-3 items in Ghidra; fix clear transcription
errors under the flag (then `tools/build.py --deploy`), hand design decisions back to the user. Write "TU -- Review" into the plan doc, update the
`STATUS.md` row, and commit.
