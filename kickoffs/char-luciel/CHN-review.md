# June character Lu/Ciel (Stage 8) - Phase CHN review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CHN review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CHN and CHN2 done and play-tested (their Done notes and commits) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CLASS_CHANGE` |
| Log tag | `QUEST complete <id> CLASS CHANGE`, `CLASS GUIDE`, `[JUNE_CHAR] LUCIEL DATA` (`python tools/logs.py --grep "CLASS CHANGE\|CLASS GUIDE\|LUCIEL DATA"`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Phase CHN - Job change (Aisha's CH-N template, every Lu/Ciel job class)" "CHN - Done (2026-11-15)"
```

## Task

Review CHN's commit(s):
- `GetX2UnitClassCompatibility` (`X2Unit.h`): the seven Lu/Ciel cases against June `31ed6500` (case 200, 0x25-0x27, 0x7f-0x81) in Ghidra.
- `X2OfflineClassChangeSeed.h`: each of the 14 item rows against `item.kim` (`/x2-kim`: job-advance items carry the parent class, tickets the
  class itself) and the `Handlers_Shop.cpp` gates that read them (a re-pick ticket refused on 200; a job-advance item needs the parent);
  `PARENT_ROWS` against June `3234b390` row 10; the `SIDE_STEP_CLASSES` 39 stand-in comment.
- `X2OfflineLuaEnumSeed.h`: 7 names, values = June `Enum.src.lua`.
- data036 `LevelUpEvent.lua`: the two `TYPE9` blocks byte-equal to June `LevelUpEvent.src.lua:154-170` / `:377-395` (BOM, CRLF), ledger / core PASS.
- The play-test logs: `CLASS CHANGE` 200 -> 37 / 38 / 39 and 37 -> 127, 38 -> 128, 39 -> 129, kept after relog; 0 EXCEPTION.
- CHN2 (promotion cube stand-in rows in `KncWX2Server/ServerResource/US/RandomItemTable.lua`): every row against `item.kim` and March's row shape; the 5-piece decision in its Done note.

Hygiene is one command: `python tools/finish.py --review --commits <CHN's commits> --with SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CLASS_CHANGE`.
Paste its block; explain every OTHER row and deleted line (`-v`). No `_DEBUG` flag to retire. A review with no code change runs no build.
Write "CHN - Review" into the plan doc, update the STATUS row (CH2b then waits only on CH2a), and commit.
