# Offline field and dungeon rewards - Phase R2 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, medium** effort, then type `/x2-phase field-rewards R2 review` |
| Plan doc | `OFFLINE_FIELD_REWARDS_PHASES.md` (branch `main`) |
| Depends on | Phase R2 done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_OFFLINE_DROP_STANDIN` (+ `_DEBUG`) |
| Log tag | `[DROP_STANDIN]` |

## Read first

```sh
python tools/docsection.py OFFLINE_FIELD_REWARDS_PHASES.md "Rules" "Starting context" "Design: the stand-in" "Phase R2 - Loader flag and generator" "Phase R2 - Done (2026-11-16)"
```

## Task

Review Phase R2's commit against the plan's Rules and Design sections. What this phase could get wrong:
- **Flag-off is the studio's table exactly:** with the define commented out nothing is published, no field is read, no line is written (read each
  `#ifdef SERV_IRUHADEV_OFFLINE_DROP_STANDIN` hunk in `X2OfflineDropTable.cpp/.h` and the `_DEBUG` hunks in `Handlers_Room.cpp`).
- **`DROP_STANDIN` is the number 1** (never a boolean), published before `DropTable.lua` and so before `StaticDropTable.lua`.
- **Counters:** `StandIn` is read in both readers; a static row that loses to a studio key is not counted; groups counted only inside 20000-20999.
- **The generator against Rules 1-8:** no studio `(key, MonsterID)` pair or static key repeated, group ids from the reserved range, ASCII block, BOM and
  CRLF kept, no `.lua` created, nothing invented (a role with no `param` line yields no row, loudly). Re-run the fixture proof if the pools / sheet
  contract changed in R1 (`gen --dry-run --out-dir`, then `check` on the output; negatives: edit one row, one group id, one item id).
- **R1's files against `gen`:** R1's `droptable.py pools` / `rates` write a different column set from the one `gen_blocks` documents (see the R2 Done note,
  "open: the two formats differ"). If R1 has committed by now, write the adapter in `gen_inputs` / `gen_blocks` (tier choice by dungeon level, the `per`
  kinds, cards by NPC id, `weight` as the group's inner probability), re-run the fixture proof on R1's real files, and make R3's `gen --only lanox` work.

Hygiene is one command: `python tools/finish.py --review --commits <R2's commit> --with SERV_IRUHADEV_OFFLINE_DROP_STANDIN SERV_IRUHADEV_OFFLINE_DROP_STANDIN_DEBUG`
(paste its block; run with `-v` and explain every OTHER row and deleted line). No build unless you change client code; then one probe build, and the
full build waits for R3. Write "Phase R2 -- Review" into the plan doc, update the R2 row in STATUS.md (move `R2` to the block's `Done:` line), check R3's
kickoff still matches, and commit.
