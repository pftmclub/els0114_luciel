# June damage and stat calculation - Phase S0 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc S0 review` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | Phase S0 done (its Done note and commit) |
| Flag | none (read-only) |
| Log tag | `[JUNE_STATCALC]` |

## Read first

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Design: the map and the verification" "Phase S0 - Stat-sheet and resource map (read-only)" "Phase S0 - Done" "Phases S1-S4, R1 - Stat sheet and resource gain (after Stages 7 and 8)"
```

Then the map's `stat` / `resource` rows and the dodge row (`june2017_re/statcalc/statcalc_map.tsv`), and the funcmap rows tagged
`StatCalc S33 (S0)`.

## Task

Review S0's map rows and the S / R split. No flag, so no flagcheck; no build.

- **Coverage:** every item of the S0 section's Scope has a row with a state, a March site and readers (base stats, `SetKStat` /
  `SetGameStats`, `SetGameStat`, `ResetMaxHP` / `ResetMaxMP`, Stat [9] / [10], `GetEqipStat`, `CX2Item::GetStat`, `GetSetIDNPartsNum`,
  `GetEnchantStat`, `320839e0`, `SumSocketOption`, `ResetIncreaseSkillLevelBySocket`, the `IsSocketOptionFormLoaded` paths, titles /
  pets / passives, the four buff member functions, the three `SetFactor`s, `32358970`, MP on hit and on hit taken, `ResetMPChangeRate`,
  absorb, the awakening gauge, regen, `GetActualMPConsume`, the six `GetSkillMpDecreaseRate` sites, the dodge row).
- **Spot-check in the exe** (read-only; resolve March's live branch with `cl /P`, not by reading `#ifdef`s): two rows S0 marked `June`
  (suggested: `320839e0` against `X2BuffTemplet.h:1206-1244`, and `GetDamageDecrement` `3291eb50`), two marked `mixed` or `March`
  (suggested: `ResetMaxHP` `31feba70`, the MP-up-when-hit check `31fd9100` with its resize @3204f428), and the absent `+0xe8` byte
  (`323752f0` @32375304 is its only writer of 1).
- **The corrections:** `SERV_BALANCE_FINALITY_SKILL_EVENT` on in an X2Lib TU (`june2017_re/flagprobe.py`), and the six June
  `GetSkillMpDecreaseRate` calls live in their class bodies.
- **Each row's reader list** for three rows, with a grep.
- **The split:** S1 / S1b / S2 / S3 / S4 / R1 / R1b kickoffs name exactly the map's rows for their phase, STATUS has a row for each, and
  no `March` / `mixed` / `absent` / `unknown` row is left without a phase.

If one spot-check fails, re-check that area's rows whole. Fix rows in place; write "Phase S0 - Review", update the S0 STATUS row, check the
S1 kickoff still fits, commit with explicit pathspecs.
