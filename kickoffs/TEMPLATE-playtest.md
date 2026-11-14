# <plan> play-test sections

One section per phase, written by that phase at Finish while it knows what it changed. A batch hand-off is assembled from these with
`python tools/playtest.py assemble <plan> [ID ...]`, never rewritten later. `python tools/playtest.py check <plan>` lints them.
Standing rules still apply to every section: dev commands first, a table per key (A S D C F Q W E R T), nicknames not ids, skills in slots
0-3 and 5-8 (4 / 9 only for transcendence), skill pages for many skills (max 3 loadouts), and "after each skill you must be able to move;
if you are stuck, name the skill".

## <phase id> - <short title>

### Dev console
`/additem <itemID> [qty]`
`<the whole Lua line, copy-ready>`

### Loadout
| key | skill (nickname) | page / slot |
|---|---|---|
| A | <nickname> | page 1, slot 0 |

### Steps
1. <where to go: village, dungeon, `/stage N`>
2. <what to press and cast, in order>

### Expect
<what must be seen: names, numbers, the exact log lines>

### If not
<what a failure looks like; which skill, state or file to name when reporting>

### Logs
<LOG_TAG>
