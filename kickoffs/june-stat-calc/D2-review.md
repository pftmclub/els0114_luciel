# June damage and stat calculation - Phase D2 review (covers D2a and D2b)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc D2 review` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | D2a and D2b done (their Done notes and commits), D2b's play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_STATCALC_FINAL_DAMAGE` (+ `_STATCALC_DEBUG`, which stays on until the D6 review) |
| Log tag | `[JUNE_STATCALC]` (`python tools/logs.py --tag JUNE_STATCALC`) |

## Read first

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Starting context" "Design: the map and the verification" "Phase D2 - GetFinalDamage: the rest of June's body" "Phase D2a - Done" "Phase D2b - Done"
```

Then `june2017_re/statcalc/gfd_blocks.tsv` (all 55 rows), the map rows with `phase` D2a / D2b, and the funcmap rows tagged
`StatCalc S33` that D2a and D2b added. Read June with `disasm_range.py` (Ghidra's listing skips `322851b1-322852d7`, `322854f8`,
`32286810-3228681a`).

## Task

Review D2a's and D2b's commits against the exe, range by range, for what a transcription gets wrong here:
- **D2b level-damage path** (@32285dc4-32286063): the PvP-or-PvE test and the channel immediates, the fixed part (`[esp+0x4c]` x
  `[esp+0x20]` for DT_PHYSIC, `[esp+0x50]` alone for DT_MAGIC), the order `( pow x 35 x corr x power + fix ) x game`, the float stores
  between (corr, `u - 1`), and the sum `( U + M ) + F`.
- **D2b main path:** the sum order @3228624e-3228626a and `f32( u - 1 )`; the received-damage rate (vslot `0x2ac`: re-run `vtslot.py
  0x2ac CX2GU`, base and the two overrides); the part rate's `> 1.0` test; the Space of Thought ids and the stand-in; the shield's state
  table (byte table `32286c98`); `CanApplyHpRateDamage` against `322726f0` (the order of its tests, the vslot `0x3c8` / `0x3cc` bodies);
  the dev log (strings, ints truncated, `CPT_SYSTEM`) and its toggle (auth level, ctor value).
- **`GetEquippingItemByEquipPos`**: June `32869e10` against the port; check the census `equip_by_pos_callers.tsv` (every constant third
  argument is 0; the register ones are a shared zero on position 2 or June-only positions) and that no March caller changed meaning.
- **`GetIgnoreDefenceRate`**: the NaN keep.
- **D2a's two named calls:** `IsSuperArmor()` for June's vslot 153( 1 ) (stand-in, divergence row `out`) and no case for June's
  `XS_PVE_GAME` 23 (no March state).
- The `#else` path is March's body byte for byte (flagcheck), and no ledger row is left that is not `June` or `class: <plan>`
  (`python -c` check in "Phase D0 - Done").

Hygiene:
`python tools/flagcheck.py SERV_IRUHADEV_JUNE_STATCALC_FINAL_DAMAGE --range 3973df2a^..<last D2b commit> --with SERV_IRUHADEV_JUNE_STATCALC_DEBUG`
(D2a is `3973df2a`; a long range times out, so check each commit with `--range <c>^..<c>` if needed), then
`git log -S SERV_IRUHADEV_JUNE_STATCALC_FINAL_DAMAGE <last D2b commit>..HEAD`; `python tools/junediverge.py --check-all`.
Paste the `files:` / `in this flag's files:` / `RESULT:` lines; explain every OTHER row and deleted line.

Numbers: re-run `june2017_re/statcalc/emu_final_damage_a.py`, `emu_final_damage_b.py` and `emu_defence.py`, and add 50 input sets of
your own to `emu_final_damage_b.py` (`review_inputs()`, as D1's review did). Read the play-test's `HIT` / `GFD` / `GFD2` / `LVD` lines
for the named monsters only: each `GFD2` chain must multiply out to its `dmg`, and the `dmg=0` lead from D1 must be explained by a
named block (or the note must say no D2 range gives it).

Leads from D2b's play-test ("Phase D2b - Done", play-test paragraph): `hook=1.0000` on every Raven99 line although Pointed Stiletto was
used (was its debuff on the target? read 3229a530's Raven block against the logged hits); boss 1470 logs `boss=0` (`IsSocketBossGrade()`;
not D2b's code, but say whether June's `templet +0xdc == 1` is true for it); the level-damage path had no reach (only the memo effect set has
`LEVEL_DAMAGE` and June never plays it): re-prove that June names `EffectSet_Aisha_ABM_EnergySpurt_MEMO` nowhere (xorstrings too).

Fix clear transcription errors under the same flag (then `tools/build.py --deploy`); design questions go to the user. The debug define is
not retired here. Write "Phase D2 - Review" into the plan doc, update the D2a / D2b / D3 STATUS rows, check the D3 kickoff (D2b added two
writer items to it), and commit with explicit pathspecs.
