# June character Elesis (Stage 7) - Phase CH1 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH1 review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH1 done and its play-test logs read ("check logs" answered in the CH1 Done note) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`, `_WOS` (defined); `_WOS_DEBUG` (defined: this review retires it); `SERV_IRUHADEV_JUNE_CHAR_ELESIS_DEBUG` (defined, stays: unported characters cannot cast skills, so its lines move to CH2a / CH2b / CH2c / CH3b and the CH3d review retires it) |
| Log tag | `[JUNE_CHAR]` (`ELESIS DATA`, `ELESIS WOS`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "CH1 - Done (2026-11-13)"
python tools/docsection.py JUNE_CHARACTER_ARA_PHASES.md "CH1 - Review (2026-11-11): pass, no code change"   # the shape of this review
```

## Task

Review CH1's deploy unit and the play-test evidence. Check at least:
- **Deploy unit:** data036 `ELESIS_KNIGHT.lua` header `1B 4B 4C 81` = the `june2017_re/june_adopt_20261113_083005.tsv` row, no `.src.lua` sidecar;
  `kom.py stage-june --dry-run june2017_re/char_elesis/ch1_held_motions.tsv` = 2 identical; `kom.py status data036 data033` in sync with live;
  `luabind.py check ELESIS_KNIGHT` `RESULT: PASS`; `luasrc.py core --check` / `ledger --check` PASS; the 4 `-- char-elesis CH1` `Enum.lua` lines carry
  June's values (`Enum.src.lua:14308 / 15564 / 15734 / 16063`).
- **State counts:** each class's `ELESIS DATA ... states N` against the registrations (`InitStateCommon` + START / WIN / LOSE + bounding + wait-state-change
  + event + slot states + `InitStateByUnitClass` + learned-skill sub-states), as Ara's review did; every count the same on every load.
- **Way of the Sword:** `ELESIS WOS vigor start / end`, `destruction start / end`, `attack gain` per the CH1q amendment; a mode across a stage change
  (one point back, mode ends) and after a dungeon end (kept). The mode end, stage change and dungeon end were not reached and the user waived them (2026-11-13): retire `_WOS_DEBUG` without asking.
- **Skills:** not cast in CH1 (unported characters cannot cast skills, user 2026-11-13); the two-cast, held-key and `SBUF` checks are the skill phases'
  (`CH2a.md` / `CH2b.md` / `CH2c.md` / `CH3b.md` amended). Nothing to check here.
- **ErrorLog** against `data\ErrorLog_pre_CH1_elesis.txt`: `KEM_ERROR`, `Motion Error`, `lua error`, `ELESIS_KNIGHT`, `StateID`, `Parsing Error`;
  CH0b's 21 `ErrorEnum(433)` part-mesh lines (checklist "Elesis frame count") either gone or a `JUNE_BUGS.md` row as Ara's 603.
- **Divergence allow-list:** the three `CX2GUEL` `GT_PVP` rows CH1 added (`32b819a8`, `32b7f28f` / `32b7f312`, `32b74f14`): open one in
  `charport_adis.py elesis` and confirm type 1 only.
- **Retire** `_WOS_DEBUG` (comment out the `#define`; leave `SERV_IRUHADEV_JUNE_CHAR_ELESIS_DEBUG` on for the skill phases). If that is the
  only code change, start the build and do not wait (CLAUDE.md, 2026-11-03). `ELESIS DATA` stays, as Rena's and Ara's did.
- **Kickoffs:** CH5 and CH2a are ready after the review; check their line references against the post-CH1 files.

Hygiene: `flagcheck --range <CH1 commit>^..<CH1 commit>` per flag (Always.h ERROR from the `--with` guard evaluation is expected, as Ara's CH1),
`junediverge.py --check-all` `RESULT: PASS`, `enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h` 0 A/B.
