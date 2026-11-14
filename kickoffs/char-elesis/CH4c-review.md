# June character Elesis (Stage 7) - Phase CH4c review (the CH4 stage review: covers CH4a, CH4b, CH4b2, CH4c)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH4c review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH4c done (its Done note, commit and the user's play-test of units 144 / 146) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE` + `..._PASSIVE_DEBUG`, `SERV_IRUHADEV_JUNE_CHAR_ELESIS_BLOOD` + `..._BLOOD_DEBUG`; the review retires `_PASSIVE_DEBUG`, `_BLOOD_DEBUG`, `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON_DEBUG` and `SERV_IRUHADEV_JUNE_CHAR_ELESIS_DEBUG` |
| Log tag | `[JUNE_CHAR]` (`ELESIS PASSIVE`, `ELESIS BLOOD`, `ELESIS SUMMON`, `SBUF`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "CH4a - Done (2026-11-14)" "CH4b - Done (2026-11-14)" "CH4b2 - Done (2026-11-14)" "CH4c - Done (2026-11-14)"
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "CH3d - Review (2026-11-14, Opus 5.5; the CH3 stage review: covers CH3a, CH3a2, CH3b, CH3b2, CH3c, CH3c2, CH3d)"   # the shape of this plan's stage review
```

Have a subagent do the reading and the mechanical runs (flagcheck per commit, junediverge, logs); you keep the Ghidra spot-checks and the verdict.

## Task

Review the whole CH4 stage against June from the exe bytes (Ghidra read-only; listings with `python june2017_re/charport_adis.py elesis <va>`).
Commits: CH4a `ce0bcdf4`, CH4b `4353d257`, CH4b2 `abe3a805`, CH4c (`git log --grep "char-elesis Phase CH4c"`).
- **Spot-check at least 6 functions across the stage**, at least three from CH4c: Blood Revenge `32b7bad0` (the min( damage, her own HP ) cap, JUNE_BUGS 671; the attacker's
  number through `DisplayDamageNumber` = June `322cf1b0`), the Blood Runaway delegate `32b87880` (the `jp` cool-time test, the `KBuffFactor` +0x20 / +0x34 reads = BTI / stack
  count, stride 0x5b), the transcendence tick `32b7e9a0` (the level-1 stack buff, JUNE_BUGS 672) or Madness `32b6ff20`; and one each from CH4a / CH4b / CH4b2.
- **Shared-file verdicts** (one line each): CH4b2's `X2GUUser.*` hunks and CH4c's `X2GUUser.h` base virtuals ( vslots 0x730 / 0x734 ) and `X2GUUser.cpp` call site
  ( June @`320499dc`-`32049a19`, inside `JUNE_STATCALC_DAMAGE_REACT`'s block ); CH4c's `X2BuffFactor.h` names ( 8 BTI, 11 BFI, values = June's `Enum.lua` ).
- **Not reached in CH4** (each Done note names its own): list them and hand them to `CH6.md` (amend it). CH4c's: the Blood Runaway past-4-stacks swap
  (`ELESIS BLOOD runaway damage`) and berserker 2 (Vigor under 0.650 HP) never logged in its play-test.
- **Retire the debug defines**: comment out `SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE_DEBUG`, `..._BLOOD_DEBUG`, `..._SUMMON_DEBUG` and `SERV_IRUHADEV_JUNE_CHAR_ELESIS_DEBUG`
  ( the `SBUF` lines: retire it whether or not `at once` / `state end` showed, CH3d review ) in `KTDXLIB/Always.h`. If that is the only code change, no build runs and the
  Done note says "build skipped: debug retire only" (standing rule 2026-11-13).
- **Hygiene:** `flagcheck.py JUNE_CHAR_ELESIS_PASSIVE --with ..._PASSIVE_DEBUG --range <c>^..<c>` once per CH4a / CH4b / CH4b2 commit, `flagcheck.py JUNE_CHAR_ELESIS_BLOOD
  --with ..._BLOOD_DEBUG --range <CH4c>^..<CH4c>` (a range to HEAD times out), `junediverge.py --check-all`, `enumscan.py --check X2Lib/X2GUEL.cpp`,
  `stateends.py check X2Lib/X2GUEL.cpp ELESIS_KNIGHT`.
- **Play-test reading** (`python tools/logs.py --grep "ELESIS BLOOD"`, `--grep "ELESIS PASSIVE"`): the CH4c Done note's lines by name, 0 `EXCEPTION`, 0 `KEM_ERROR`.

Fix clear errors in place (bytepatch only), hand design decisions back to the user. Write "CH4c - Review" into the plan doc, update the STATUS rows (CH4c done, CH6 ready),
check `kickoffs/char-elesis/CH6.md`, commit with explicit pathspecs.
