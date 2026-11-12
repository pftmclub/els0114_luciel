# June damage and stat calculation - Phase D3 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc D3 review` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | D3 done (its Done note and commit), D3's play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_STATCALC_HIT_PIPELINE` (+ `_STATCALC_DEBUG`, which stays on until the D6 review) |
| Log tag | `[JUNE_STATCALC]` (`python tools/logs.py --tag JUNE_STATCALC`) |

## Read first

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Starting context" "Phase D3 - The hit pipeline before the formula" "Phase D3 - Done"
```

Then the map rows with `phase` containing `D3`, and the funcmap rows tagged `StatCalc S33` that D3 added (the last eight rows). Read June
with `disasm_range.py` where Ghidra has no function (`32759f30`).

## Task

Review D3's commit against the exe, item by item, for what a transcription gets wrong here:
- **`DamageCheck` prelude** (`3262f5a0` up to the AreaHit test): that `+0x91e4` is `m_bEnterCashShop` (writer `32044a80`, `int*` index
  `0x2479`), the user / NPC branches and that both return false before AreaHit; the owner getter is `32759f00`.
- **The hit-part name**: each of the five copies (`@3262c18d`, `@32611f53`, `@32625ef0`, `@32628ba4`, `@32641c83`) writes the B datum's
  `m_Name` into `m_pCollision` +0 right after the `m_bEnable` skip, and the C8b drivers give B the same role June does (in
  `CollisionDataToUnit` B is the manual datum, so the name is the manual datum's: check that this is June's too).
- **`SetDefenderDamageRateOfPart`** against `32621f40` (reset to 1.0 first, `GetNowStateData` `31e8a990`, the find by name, the first
  match wins) and its four call sites: in the non-heal branch only, before the dodge roll, NPC defenders only.
- **The loader** against `32784c00 @32796718-3279680d` (clear first, `[1]` name, `[2]` rate with the 1.0 default, an entry pushed even
  when a read fails).
- **`IMMUNE_TO_HP_ATTACK`**: key position after `SET_NUM_OF_GAGE`, default `m_bHasBossGage`, ctor 0, the test position in
  `CanApplyHpRateDamage` (`@32272745`).
- **`CEffect::AttackResult` tail** (`31f885bd-31f88669`): call order, the `AT_SPECIAL` test on DD `+8`, the defender being DD `+0x470`
  (`[esp+0x40]` = CEffect `+0x670`), `GetFinalDamage` before the `g_pX2Game` test, truncation; and that `32008280` equals March's summon
  block (the `CreateNPCReq` arguments). The same tally in `CX2GUUser::AttackResult` (`@32014215-32014277`).
- **`AddGivenDamamge`** against `31f8df30` (signed compare, store only when changed) and its divergence row.
- The re-checks: base `322990e0`, user `32045340`, NPC `32759f30` (spot-check two terms each), and that `ADD_RENA_SYSTEM` is on
  (`june2017_re/flagprobe.py`), so `UnitToUnit`'s NPC `AttackResultByType` needed no code.
- The `#else` paths are March's bodies (flagcheck).

Hygiene:
`python tools/flagcheck.py SERV_IRUHADEV_JUNE_STATCALC_HIT_PIPELINE --range <D3 commit>^..<D3 commit> --with SERV_IRUHADEV_JUNE_STATCALC_DEBUG`,
then `git log -S SERV_IRUHADEV_JUNE_STATCALC_HIT_PIPELINE <D3 commit>..HEAD`; `python tools/junediverge.py --check-all`.
Paste the `files:` / `in this flag's files:` / `RESULT:` lines; explain every OTHER row and deleted line.

Numbers: re-run `june2017_re/statcalc/emu_final_damage_b.py` (D3 made its `IMMUNE_TO_HP_ATTACK` sets compare). Read the play-test's
`EFFHIT`, `PART` (only if a raid NPC was reached) and `HIT` lines named in the Done note.

Fix clear transcription errors under the same flag (then `tools/build.py --deploy`); design questions go to the user. The debug define is
not retired here. Write "Phase D3 - Review" into the plan doc, update the D3 / D4 STATUS rows, check the D4 kickoff, and commit with
explicit pathspecs.
