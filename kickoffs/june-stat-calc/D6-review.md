# June damage and stat calculation - Phase D6 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc D6 review` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | D6: all eight sheets in `kickoffs/june-stat-calc/playtest.md` passed or routed (the D6 Done note lists each) |
| Flag | `SERV_IRUHADEV_JUNE_STATCALC_*` (D1-D5 flags) and `SERV_IRUHADEV_JUNE_STATCALC_DEBUG`, retired by this review |
| Log tag | `[JUNE_STATCALC]` (`python tools/logs.py --tag JUNE_STATCALC`) |

## Read first

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Phase D6 - Damage smoke on every ported character, track review" "Phase D6 - Done"
```

Then the Done notes of D1-D5 and their reviews (only the flag names, commits and map rows), and `june2017_re/statcalc/statcalc_map.tsv` rows with area `damage`.

## Task

- Read the `HIT` / `GFD` / `GFD2` / `REACT` lines of every D6 sheet against the map (the D6 Done note names each character's run and what was routed).
- Run `python tools/finish.py --review --commits <the D1-D5 commits>` (flagcheck per flag with `--with` the D1-D5 flags, `junediverge --check-all`, enumscan).
  Explain every OTHER row and deleted line.
- Map gate: no `damage` row is `March` or `mixed`. The one `March` row, `dodge roll index offset (NPC stat byte +0xe8)`, is routed to S1b (S0 review); confirm it names S1b.
- Retire `SERV_IRUHADEV_JUNE_STATCALC_DEBUG` (`//#define` in `KTDXLIB/Always.h`). Build skipped: debug retire only.
- Write "Phase D6 review - Done" into the plan doc, update the D6 row in STATUS.md, check the S1 kickoff, commit with explicit pathspecs.
