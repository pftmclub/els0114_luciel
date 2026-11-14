# June character Elesis (Stage 7) - Phase CH3d review (the CH3 stage review: covers CH3a, CH3a2, CH3b, CH3b2, CH3c, CH3c2, CH3d)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH3d review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH3d done (its Done note, commit and the user's play-test of units 29 / 30 / 146, transcended) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL` + `..._SKILL_DEBUG`, `SERV_IRUHADEV_JUNE_CHAR_ELESIS_TRANSCEND`; the review retires `_SKILL_DEBUG`, `SERV_IRUHADEV_JUNE_CHAR_ELESIS_DEBUG` and `_SUMMON_DEBUG` |
| Log tag | `[JUNE_CHAR]` (`ELESIS SKILL`, `ELESIS SUMMON`), `[JUNE_TRANSCEND]` |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "CH3a - Done (2026-11-14)" "CH3a2 - Done (2026-11-14)" "CH3b - Done (2026-11-14)" "CH3b2 - Done (2026-11-14)" "CH3c - Done (2026-11-14)" "CH3c2 - Done (2026-11-14)" "CH3d - Done (2026-11-14)"
python tools/docsection.py JUNE_CHARACTER_ARA_PHASES.md "CH3d - Review"   # the shape of a CH3 stage review
```

Have a subagent do the reading and the mechanical runs (flagcheck per commit, junediverge, logs); you keep the Ghidra spot-checks and the verdict.

## Task

Review the whole CH3 stage (CH3b0 had its own review) against June from the exe bytes (Ghidra read-only; listings with `python june2017_re/charport_adis.py elesis <va>`).
Commits: CH3a `1df5565c` (+ fix `e36b59dc`), CH3a2 `72bbe355`, CH3b `0747d366`, CH3b2 `17129609`, CH3c `bb53f0ee`, CH3c2 `b24fdc8f`, CH3d (`git log --grep "char-elesis Phase CH3d"`).
- **Spot-check at least 6 functions across the stage**, at least two from CH3d: Ancient Fire FrameMove `32b7a7a0` (0.887 s, `GetEquippedActiveSkillTemplet( 28601, true )`
  +0x178 test -> the `_UPGRADE` effects, 2.177 s render block), the registration's hyper block `32b8c470` @`32b8e947` .. @`32b8ea20` (jump table `32b8ecd0` for
  28601-28605; struct slots: the Sword of Relics hyper state copies only +0x44 / +0x48, no Init), and one function each from CH3a-CH3c2 (Shadow Edge upkeep `32b813d0`,
  a CH3b0 spawn reader, a CH3c Blood Cutter state).
- **Pictures:** re-read two `X2Data.cpp` Elesis rows from June's initializers (gauge `33372a0b` -> Texture102 `EL_SGM_FACE`, gray `3337cea1` -> Texture101_GRAY) and judge the
  play-test's `[JUNE_TRANSCEND] IMAGE class 120 / 121 / 125` lines; a type that never logged is asked for once (party, gray unit slot).
- **Not reached anywhere in CH3** (each Done note names its own): list them and hand them to `CH6.md` (amend it), together with 28600 / 28601 / 28605 if CH3d's test played
  only the ids it slotted.
- **Retire the debug defines**: comment out `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL_DEBUG`, `SERV_IRUHADEV_JUNE_CHAR_ELESIS_DEBUG` (the `SBUF` lines, CH1's amendment: confirm
  CH2a / CH2b / CH2c / CH3b showed them, ask once for a line never seen) and `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON_DEBUG` (CH3b0's 5 lines, read in CH3b / CH3c) in
  `KTDXLIB/Always.h`. If that is the only code change, no build runs and the Done note says "build skipped: debug retire only" (standing rule 2026-11-13).
- **Hygiene:** `flagcheck.py JUNE_CHAR_ELESIS_SKILL --with ..._SKILL_DEBUG --range <c>^..<c>` once per commit above (a range to HEAD times out),
  `flagcheck.py JUNE_CHAR_ELESIS_TRANSCEND --range <CH3d>^..<CH3d>`, `junediverge.py --check-all`, `enumscan.py --check X2Lib/X2GUEL.cpp`,
  `stateends.py check X2Lib/X2GUEL.cpp ELESIS_KNIGHT`, `skillicons.py check 28000-28999`.
- **Play-test reading** (`python tools/logs.py --grep "ELESIS SKILL"`, `--grep "JUNE_TRANSCEND"`): every state entered by the CH3 skills has a callback, 0 `EXCEPTION`,
  0 `KEM_ERROR` / `Motion Error`, no `BLANK`.

Fix clear errors in place (bytepatch only), hand design decisions back to the user. Write "CH3d - Review" into the plan doc, update the STATUS rows (CH3d done, CH4a ready),
check `kickoffs/char-elesis/CH4a.md`, commit with explicit pathspecs.
