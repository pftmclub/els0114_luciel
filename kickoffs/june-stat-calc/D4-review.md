# June damage and stat calculation - Phase D4 review (covers D4a and D4b)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc D4 review` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | D4a and D4b done, D4b's play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_STATCALC_DAMAGE_REACT` (+ `_STATCALC_DEBUG`, which stays on until the D6 review) |
| Log tag | `[JUNE_STATCALC]` (`python tools/logs.py --tag JUNE_STATCALC`) |

## Read first

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Starting context" "Phase D4 - DamageReact: from the final damage to the HP change" "Phase D4 - Cut (2026-11-12)" "Phase D4a - Done" "Phase D4b - Done"
```

Then `june2017_re/statcalc/dr_blocks.tsv` (both sides) and the map rows whose `phase` names `D4a` or `D4b`.

## Task

Review D4a's and D4b's commits against the exe, block by block, for what a transcription gets wrong here:
- The ledger: each side contiguous (`npc 327b33f0-327b4c13`, `user 32048435-32049b70`), every row `June` except the `n/a` rows and the
  `absent` rows that are stand-ins (NPC: Stage 19's alchemy lab; user, amended by D4b: six rows, Lu/Ciel / Elesis / Rose / Ain / the 2017
  treasure event / the two Stage 40 writes); the check line in the cut note prints `True` for both.
- Every block that was `absent` or `mixed`: the identified members, vslots and ids against the exe (spot-check the `0x378` slot, the
  `+0x8f84` fixed damage, the user `+0x8e94` AT_SPECIAL multiply and its only writer `32b45ec0`, the Elesis `+0x9175` owner (`@320475c1`
  type 8, `32b524f0`), the provoke gate `31f8ae70`, the user's max-HP block `3204968f` = treasure event).
- Order: the reductions in June's order, the HP-rate overwrite where June has it, `322cfca0` and the tutorial clamp in place.
- `GetSocketReducedDamagePercent` against `32000320` and `GetBasicReducedDamagePercent` against `3201aff0` (D4b added both GT_PVE heads);
  both divergence rows closed. Amended by D4b: March's `GetSocketReducedDamagePercentInCase` and its callers were left in place, compiled
  out on US by `ADJUST_SECRET_ITEM_OPTION` (deleting them would break insert-only); judge whether that is enough.
- The `#else` paths are March's bodies (`flagcheck --range` over both commits); `junediverge --check-all` PASS.
- The play-test log: ten hits each way, `REACT`'s HP change = `GFD` damage after the listed reductions, with and without reduction sockets.
