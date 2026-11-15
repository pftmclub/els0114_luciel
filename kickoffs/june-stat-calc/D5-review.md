# June damage and stat calculation - Phase D5 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc D5 review` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | D5 done and its play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_STATCALC_EXTRA_DAMAGE` (+ `_STATCALC_DEBUG`, which stays on until the D6 review) |
| Log tag | `[JUNE_STATCALC]` (`python tools/logs.py --tag JUNE_STATCALC`) |

## Read first

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Starting context" "Phase D5 - Extra damage, enchant resist and level correction" "Phase D5 - Done"
```

Then the map rows whose `phase` names `D5` or `D5-review`, and `grep -n "StatCalc S33 D5" june2017_re/funcmap.tsv`.

## Task

Review D5's commit against the exe (`disasm_range.py`; Ghidra has no function at `32757c50` / `31fd8790`):
- NPC `LevelCorrectionExtraDamage` `32760210`: the dungeon compare tree @327602ed-3276044e gives exactly the port's 15 ids (decode each `cmp` / `sub` step;
  0x9a4c / 0x9a56 / 0x9a60 / 0x9a6a / 0x9a74 / 0x9a7e, 0xa140), no 38100 and no 38401; the guard (room, party manager, party data) kept.
- NPC `GetEnchantResist` `32757c50`: the base call @32757cdc keeps `ecx` = this; 150.0 is a double at `334748c8`; the port adds the base after the NPC terms.
- NPC `UpNowHp` `32784200`: the gate order (`+0x75bc`, game slot `0x10c` == 2, `fcomp` / `test ah,1`: a delta <= 0 returns, NaN passes); then the base.
  Decide whether routing the debug tail (`322e28e0` map, `+0xba4` null write, chat command `31ec2921`) to Stage 40 is right, or the kickoff's
  "keep verbatim" should stand.
- `GetCalcPerDamage` `32622300`: jump table `32622490` / `326224a8`, constants, the PvP-or-PvE test, channel classes 9 / 11 / 10.
- **The map's `extra-damage tick application` row (`unknown`):** read NPC `CommonFrameMoveHost_ExtraDamage` `3276b400` (poison / fire / cold / frozen
  blocks after the blaze section) and find the user's June `CommonFrameMoveFuture_ExtraDamage` (March's frozen block subtracts `m_Fire.m_DamagePerSec`).
  A difference is a finding: fix it under the flag if small, else a new D phase.
- The play-test log: each `EDTTICK` line's `lost` per second against the `perSec` of the types it names, and every `EDT` line's `perSec` against
  `base x (1 + 0.2 (lv - 1)) x (1 - resist / 500)`.

Hygiene is one command: `python tools/finish.py --review --commits <D5's commits> --with SERV_IRUHADEV_JUNE_STATCALC_EXTRA_DAMAGE` (run with `-v`, explain
every OTHER row and deleted line). `_STATCALC_DEBUG` stays on.

Fix clear transcription errors under the same flag (then `tools/build.py --deploy`); hand design decisions back to the user. Write "Phase D5 review - Done"
into the plan doc, update the STATUS rows (D5 done, D6 ready), check `D6.md`, and commit.
