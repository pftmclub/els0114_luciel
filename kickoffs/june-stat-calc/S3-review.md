# June damage and stat calculation - Phase S3 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc S3 review` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | Phase S3 done (its Done note and commit) and its play-test |
| Flag | `SERV_IRUHADEV_JUNE_STATCALC_SOCKET` (new; S3b adds to it) + `_STATCALC_DEBUG` (stays on; Z retires it) |
| Log tag | `[JUNE_STATCALC]` (`python tools/logs.py --grep "JUNE_STATCALC] MPUP"`) |

## Read first

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Phases S1-S4, R1 - Stat sheet and resource gain (after Stages 7 and 8)" "Phase S3 - Done (2026-11-16)"
```

Then the map rows with `phase = S3` in `june2017_re/statcalc/statcalc_map.tsv` and `grep -n "StatCalc S33 S3" june2017_re/funcmap.tsv`.

## Task

Review Phase S3's commit against June's exe (Ghidra, read-only):
- The loader (`X2SocketItem.cpp`, June `32726850` @32732152-3273256a): `m_fIntervalTime` read first, the push only when the chance is above 0
  (`test ah, 0x41`), the entry ( chance, MP, interval ); that June has no other read of `m_fPercentMPUpByAttacked` (xorstrings `327321fe` only),
  so the early March pair is rightly compiled out.
- `CalculateMpUpAttackedBySocketOption` = `31fd9100` and `CheckSocketMpUpAttackedCoolTime` = `31fda180` (`X2GUUser.cpp`): the skip test, the seed
  that advances only on a roll, the timer set only on a hit, the tick's `<= 0` clamp; re-run `python june2017_re/statcalc/emu_socket_mpup.py` (must
  PASS) and check its resize cases are covered by reading (the emulation keeps the sizes equal).
- The timer vector's life: both resizes in `SumSocketOption` (June resizes after all three `ConvertAndAdd` calls, @3204f324 / @3204f3f6 /
  @3204f44e: confirm the rank-linked one at @3204f329 really calls `32077f50`; xrefs list only @3204f428 / @3204f480), the clear in
  `UpdateSocketDataAndEnchantData` (@3204b4d3), the tick's place in `OnFrameMove` (@3206f728, after `CheckSocketCoolTime`).
- The DamageReact guard (@32047b00-32047b4d): DD `+0x500` = March's `m_bAttackOurTeam` (offsetprobe: March 0x43c, right before `m_fHpPercentUp`
  0x440, as June's +0x500 / +0x504), `damageType != DT_BUFF`; the HP part unchanged (31fd5620 = March, no negative branch in either).
- The tooltip (`X2SocketItem.cpp`, June 3271f8c1): `(int)(float)( x * 100.0 + 0.5 )` for both strings, sign and abs as June.
- The two routed findings: `+0x94a0` (Lanox Secret Ammo only) -> Stage 16, the customize socket source -> Stage 12 (`JUNE_PORT_BACKLOG_ORDER.md`).
Gates: `python tools/finish.py --review --commits <S3's commit> --with SERV_IRUHADEV_JUNE_STATCALC_SOCKET SERV_IRUHADEV_JUNE_STATCALC_DEBUG -v`.
Build only if the review changes code and hands off a play-test. Then check S3b's kickoff.

## Done gate

Verdict line in the plan doc ("Phase S3 - Review"), map rows unchanged or corrected, STATUS row updated, S3b's Depends satisfied.
