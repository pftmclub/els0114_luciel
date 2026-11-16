# Offline field and dungeon rewards - Phase R6 review (covers R3-R6)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase field-rewards R6 review` |
| Plan doc | `OFFLINE_FIELD_REWARDS_PHASES.md` (branch `main`) |
| Depends on | R6 done (its Done note and commit; the smoke is deferred to the plan's end, user 2026-11-16) |
| Flag | `SERV_IRUHADEV_OFFLINE_DROP_STANDIN` (+ `_DEBUG`) |
| Log tag | `[DROP_STANDIN]` |

## Read first

```sh
python tools/docsection.py OFFLINE_FIELD_REWARDS_PHASES.md "Rules" "Design: the stand-in" "Phase R3 - Done (2026-11-16)" "Phase R4 - Done (2026-11-16)" "Phase R5 - Done (2026-11-16)" "Phase R6 - Done (2026-11-16)"
```

## Task

Review R3-R6 together (the generated block in `DropTable.lua` / `StaticDropTable.lua` and `tools/droptable.py`) against the Rules. What this could get wrong:
- **Studio rows untouched:** the diff of both server files against the commit before R3 is insertions only inside the one marked block (the one deleted line per
  file, if any, is the block's own `-- scope:` comment); no studio `(key, MonsterID)` pair or static key repeated; group ids 20000-20999.
- **R6's copies:** every `-- source:` row equals its source studio row (Exp / ED / EDProperty / items / groups), the source is the same role and the same dungeon,
  or its other difficulty (user decision 2026-11-16); a copied row never carries a monster card of another NPC. Re-run `droptable.py check` and `simulate` for the
  R3-R6 keys; re-confirm the 30 unresolved pairs have no same-role studio row (`gen --dry-run` lists them) and decide with the user whether any gets a rule.
- **Smoke results:** R3 passed; R4 (Elysion), R5 (secret) and R6 (Garpai Rock Normal 33210, spec in the R6 Done note) smokes were deferred to the plan's end (user
  2026-11-16): list what is still unplayed in the review and ask the user once whether to hand them off now.
- **`_DEBUG`:** read every `#ifdef SERV_IRUHADEV_OFFLINE_DROP_STANDIN_DEBUG` hunk, then comment out the define in `KTDXLIB/Always.h` (retire only; build skipped:
  debug retire only).

Hygiene is one command: `python tools/finish.py --review --commits ad28b6e7 c3b7e545 ee33a061 6295a5e9 59c30487 <R6's commit> --with SERV_IRUHADEV_OFFLINE_DROP_STANDIN SERV_IRUHADEV_OFFLINE_DROP_STANDIN_DEBUG`
(run with `-v`, explain every OTHER row and deleted line; it includes `junediverge --check-all`). Write "Phase R6 -- Review" into the plan doc, update the R6 row in
STATUS.md (move `R6` to the block's `Done:` line), check Z's kickoff, and commit.
