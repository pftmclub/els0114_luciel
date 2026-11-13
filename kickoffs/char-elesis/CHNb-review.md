# June character Elesis (Stage 7) - Phase CHNb review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CHNb review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CHNb done and its play-test passed (Done note, commits) |
| Flag | none of its own: CHNb is data only. Class-generic code it relies on: `SERV_IRUHADEV_JUNE_CLASS_CHANGE_GUIDE`, `SERV_IRUHADEV_JUNE_QUEST_RUNTIME`, `SERV_IRUHADEV_JUNE_CLASS_PATH` (Ara's, already reviewed). `_CHAR_ELESIS_CLASS_DEBUG` stays commented out. |
| Log tag | `[JUNE_CHAR]` (`CLASS GUIDE 1st` / `2nd` / `2nd skills`), `QUEST complete ... CLASS CHANGE`, `[ADMIN_CHEAT]` |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "Phase CH-N - Elesis's June-only class path: Dark Knight (35) and Crimson Avenger (125)" "CHNb - Done (2026-11-13)"
python tools/docsection.py JUNE_CHARACTER_ARA_PHASES.md "CHNb - Review"   # what the class-generic guide code was checked against
awk -F'\t' '$7 ~ /CHNb/' june2017_re/char_elesis/ch0_classn.tsv
```

## Task

Check CHNb's data against June (no Ghidra needed unless a log line disagrees):
- **`LevelUpEvent.lua`** (live data036 kom_contents, core-list source): the level-15 Elesis `TYPE6` and the level-35 Elesis `TYPE6` equal June's
  `june_src_fixed/data036_kom/LevelUpEvent.src.lua:103-118` and `:320-337` (class list, dialog, sounds); nothing else in the file changed
  (`git`-less file: diff against the ledger snapshot or the pre-CHNb backup `kom_backup_tools/data036.kom.20261113_110737`). BOM + CRLF intact;
  `luasrc.py ledger --check` / `core --check` PASS.
- **Dialogs:** the eight `DLG_*Elesis*` class-change files are June's (`ljconv` of `14JUN17/data/data034_kom` / `data217_kom`); `luabind.py check` on them PASS.
- **Play-test logs:** `CLASS GUIDE 1st: N class description(s) wrapped` for the level-15 units, `CLASS GUIDE 2nd: class 35 line 3 -> next class 125` for ElGuideCA,
  `QUEST complete 35572 CLASS CHANGE ... 8 -> 35`, `35577 ... 35 -> 125`, `35323 ... 8 -> 29`, `35333 ... 8 -> 30`; the save keeps 35 / 125 / 29 / 30 after the relog
  (`elsdb.py chars`); 0 EXCEPTION; no other unit's class changed.
- **Kickoffs:** CH2d and CH3c (Dark Knight / Crimson Avenger skills) depend on this review; CH6's walk covers both job changes on fresh units: check they still
  name the right quest ids.

No code change expected, so no flagcheck range; `junediverge --check-all` once (RESULT: PASS). Full build skipped unless a fix lands.
Write "CHNb - Review" into the plan doc, update the STATUS row (move CHNb to the block's `Done:` line), commit with explicit pathspecs.
