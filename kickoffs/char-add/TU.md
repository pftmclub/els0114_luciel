# June character Add (Stage 7) - Phase TU - Add's June tutorial (DI_DUNGEON_TUTORIAL_ADD 41230)

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase char-add TU` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | CH1 review (Add on his June kit), CH0c review |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_TUTORIAL` (+ `_DEBUG`) |
| Log tag | `[JUNE_CHAR]` (`ADD TUTORIAL`) |
| Review | `kickoffs/char-add/TU-review.md` (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Rules" "Starting context"
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md "CH0c - Done (2026-11-13)"
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Phase TU - Tutorial"   # the same work for Lu/Ciel (41240), parked
grep -P "^325622e0\t" june2017_re/funcmap.tsv
```

## Task

Why this phase exists (user, 2026-11-13): CH0c's play-test froze after creating Add. March sends a new unit to its battle-field tutorial,
Add's `DI_BATTLE_FIELD_TUTORIAL_ADD` 39608 has no `DungeonData` row (March never had one; June dropped every battle-field tutorial), and the
client quit on the empty dungeon. CH0c's stand-in skips the tutorial for Add (create reply `X2StateCreateUnit.cpp`, first select
`X2StateServerSelect.cpp`, flag `JUNE_CHAR_ADD_CREATE`). This phase gives him June's tutorial and retires that skip.

- June's `Handler_EGS_CREATE_TUTORIAL_ROOM_REQ` `325622e0` sends unit type 9 to `0xa10e` = `DI_DUNGEON_TUTORIAL_ADD` 41230. Port **Add's case
  only** into March's `CX2StateServerSelect::Handler_EGS_CREATE_TUTORIAL_ROOM_REQ` (`:7501`) under the flag, March's `DI_BATTLE_FIELD_TUTORIAL_ADD`
  in `#else`. The other classes' cases stay March's (Lu/Ciel TU and its follow-ups own them).
- Register `DI_DUNGEON_TUTORIAL_ADD = 41230` in `DungeonEnum.h` under `JUNE_ENUM_NAMES` and in the core-list `DungeonEnum.lua` (data036), and June's
  `DungeonData` row (`june_src_fixed/data035_kom/DungeonData.src.lua`, `Dungeon_Tutorial_Add.lua`, loading screens `HQ_Loading_TUTORIAL_ADD_01/02.tga`,
  `STR_ID_35436`, its `DUNGEON_CHECK_CONDITION`) in the core-list `DungeonData.lua` (data035).
- `Dungeon_Tutorial_Add.lua` is already in data035: `luabind.py check Dungeon_Tutorial_Add` = PASS; stage its world, NPCs, loading screens and
  strings the census finds missing (byte-exact, pack, copy).
- Emulator: `CX2OfflineQuest::IsQuestTutorialDungeon` and any other tutorial list (`Handlers_Room.cpp`, `Handlers_Skill.cpp`) learn 41230; the room
  handler loads its stages ("stage 0 out of range" must not appear).
- Retire CH0c's tutorial skip (both sites) under this flag.
- Play-test: create a new Add, he enters June's tutorial, plays it to its end and lands in the village; relog.

**Amended 2026-11-13 (june-dungeons C3y-2 re-scoped):** C3y-2 ports June's whole tutorial switch for every March class, Add's case, enum,
DungeonData row, world, NPCs and emulator lists included. If C3y-2 has landed, skip those bullets: this phase only retires CH0c's skip (both
sites) and play-tests. If this phase lands first, do the bullets as written; C3y-2 keeps Add's case.

No hardcoded enum literals (user, 2026-10-26; CLAUDE.md): an id that has a name in an Enum (June's
`Enum.lua`, or a C++ enum) is never written as a number, in any new or touched code: use the member, and
register it under a `SERV_IRUHADEV_` flag with June's value and name if C++ lacks it. A value with no Enum
entry stays a number with a one-line comment. The one exception is a table whose rows are the data itself
(a `.tsv` of ids), not code.

No `June` prefix (user, 2026-10-28; CLAUDE.md): June's body goes inside March's own function under the flag,
March's in `#else`. A function March lacks gets a name in the class's own pattern, never with `June` in it.
Every name (locals and constants too) says what it is in context: no Ghidra address, phase tag or `June` in an identifier.
Call a March function from June code only if June's namesake does the same; otherwise replace March's definition.
Run `python tools/junediverge.py --list` before transcribing; a newly found divergent function gets its `june_march_divergent.tsv` row and the replacement in the same commit.
Phase tag (user, 2026-11-09): every edited or new code block carries a one-line `// char-add TU` comment.
CP949 sources: `bytepatch.py` only.
