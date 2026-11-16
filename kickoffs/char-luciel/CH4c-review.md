# June character Lu/Ciel (Stage 8) - Phase CH4c review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH4c review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH4c done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_PASSIVE` (untouched by CH4c; `_PASSIVE_DEBUG` stays retired) |
| Log tag | `[JUNE_CHAR] LUCIEL PASSIVE` (no new line) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "CH4c - Done (2026-11-16)"
```
Then `kickoffs/char-luciel/CH4c.md` (the amendments), the `funcmap.tsv` rows tagged `(CH4c)`, and the Stage 12 bullet CH4c added to
`JUNE_PORT_BACKLOG_ORDER.md` ("customize entries' body filter").

## Task

CH4c changed no client code: it found every item of its row ported already or never firing in June. Review the verdicts, not a diff.
- Re-read in Ghidra / the exe bytes (read-only): `32ae5150` -> `31fd4450` (always false), and 2 of the 15 stub calls to `31fca9e0` in the class
  against their ports' comments (pick one from CH2c2 and `32b2d2a0`, which the CH3b peer ported).
- Re-run the unit-test scan the Done note describes (`call 31e4c370 / 31e4d0e0 / 31e4d0c0` then `cmp` with 10 or a Lu/Ciel class id, outside
  `32ae4080-32b4e76f`): 14 sites, each one ported, routed earlier, or in the Stage 12 bullet. Any other site is a finding.
- `3229e1a0`: confirm no Lu/Ciel BTI case. `322fbc60`: confirm the SAT 12 case is its only Lu/Ciel piece.
- No code changed, so `finish.py` has nothing to check; if the review fixes code, run `python tools/finish.py --files <changed files>` and build.

Write "CH4c - Review" into the plan doc, update the STATUS.md row, check CH6's kickoff names what it needs from CH4, and commit. No play-test
unless the review changes code.
