# June character Elesis (Stage 7) - Phase CH2d review (the CH2 stage review: covers CH2a-CH2d, incl. CH2b2)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH2d review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH2d done (its Done note, commit and the user's play-test of unit 144 `DKnight80`) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL` + `..._SKILL_DEBUG` (the review retires the debug define) |
| Log tag | `[JUNE_CHAR]` (`ELESIS SKILL`), `[JUNE_SKILL]` (`--grep NUM`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "CH2a - Done" "CH2b - Done" "CH2b2 - Done" "CH2c - Done" "CH2d - Done"
```

Have a subagent do the reading and the mechanical runs (flagcheck per commit, junediverge, logs); you keep the Ghidra spot-checks and the verdict.

## Task

Review the whole CH2 stage against June from the exe bytes (Ghidra read-only; listings with `charport_adis.py elesis <va>`). Commits: CH2a `454c376c`
(+ fix `4a74cb3b`), CH2b `9d42957b`, CH2b2 `699daff1` (+ fix `9adceac8`), CH2c `5326702e`, CH2d (`git log --grep "char-elesis Phase CH2d"`).
- **Spot-check at least 6 functions across the stage**, at least two from CH2d: Brutal Cutter FrameMove `32b89c10` (Oneshot 0.2, helper `32b89470( m_DamageData, 0.1, 8 )`, damage
  x1.3 / x1.4, the BTI 509 / 429 branch and BFI 466 / 593, memo type 0xf then the `31fcf3e0` time x ability 1, `SetGameUnitBuffFactor( this, -1, true )`, vslot 0x224),
  the bleeding helper `32b89470` (the copy's defender = this, 0.2 / 0.1 constants, min( damage x count, HP - 0.1 max HP ), BTI 666 x0.2, always true), Injection Stigma
  `32b8b410` (0.01 s HP cost, 0.2 s dummy damage effect + helper count 1, BTI 433 erase, BFI 470 with level 1 / skip 0), Burning Phantom loop `32b73ae0` / `32b51d10`
  (0.15 s tick, count > 9 returns without CommonFrameMove, 10 MP above tick 4, end conditions), and the registration cases `32b8c470` @596 / @630 / @638 / @1190 / @1198 / @1206
  (a fresh `UserUnitStateData` per second state: Ejection attack = FrameMove + EventProcess, Burning Phantom START / LOOP / END, Shade Scratch ATTACK).
- **The enhanced ids were never played** (CH2b2 28219 / 28225 / 28226, CH2c 28034 / 28037 / 28050 / 28220 / 28229 / 28230, CH2d 28035 / 28038 / 28055 / 28221 / 28233 / 28234;
  CH2a / CH2b: say which). Have the user learn and cast them on the existing units ( `python tools/elsdb.py` slots; never slot 4 / 9 ): the templet swap
  ( `m_iDeleteSkillIDByUpgrade` ) must keep every state / FrameMove on the same functions. A failure is a finding fixed here.
- **Held key (CH1p review note):** Ejection Buster sets the key in its StartFuture and no state reads it ( June has no read in that range ); confirm and say so.
- **Retire `_SKILL_DEBUG`**: comment out its `#define` block in `KTDXLIB/Always.h` (guarded code stays); per the standing debug-retire rule (user, 2026-11-13) that is the only
  code change, so no build is run and the Done note says "build skipped: debug retire only" (a real fix in the same review builds instead).
- **Hygiene:** `flagcheck.py JUNE_CHAR_ELESIS_SKILL --with ..._SKILL_DEBUG --range <c>^..<c>` once per commit above (a range to HEAD times out), `junediverge.py --check-all`
  (the four `GT_PVP` hits in `X2GUEL.cpp` that CH2c noted are this review's to settle), `enumscan.py --check X2Lib/X2GUEL.cpp`, `stateends.py check X2Lib/X2GUEL.cpp ELESIS_KNIGHT`,
  `skillicons.py check` over the class-8 / 29 / 30 / 35 ids.
- **Play-test reading** (`python tools/logs.py --grep "ELESIS SKILL"`): every state entered by the CH2 skills has a callback, 0 `EXCEPTION`, 0 `KEM_ERROR` / `Motion Error`.

Fix clear errors in place (bytepatch only), hand design decisions back to the user. Write "CH2d - Review" into the plan doc, update the STATUS rows (CH2d done, CH3a ready),
check `kickoffs/char-elesis/CH3a.md`, commit with explicit pathspecs.
