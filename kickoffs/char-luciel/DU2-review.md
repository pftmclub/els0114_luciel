# June character Lu/Ciel (Stage 8) - Phase DU2 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU2 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase DU2 done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_USER_DAMAGE_STATE_GETTERS` (no `_DEBUG`) |
| Log tag | none |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "Phase DU - The two-body engine (DU1-DU5)" "DU2 - Done (2026-11-14)"
```
Then the funcmap rows `grep "Char Luciel DU2" june2017_re/funcmap.tsv` and the DU2 rows of `june2017_re/char_luciel/nu0_duo_engine.tsv`.

## Task

Review DU2's commit against June:
- **The 19 bases.** Each `CX2GUUser` getter returns the March `USI_` member whose June id the base body returns (`mov eax, N` at the vtable
  `334781f4` slot; March = June - 1 from 25). Re-read 3 of them from the exe bytes (not the decompiler: the bodies are uncarved).
- **No accidental override.** No class in `X2Lib` declares a member with any of the 19 names (grep), and `GetJumpDownStateID`'s new `int` return
  breaks no caller.
- **Every reader.** Each NU0 `DU2` reader row has a verdict in the Done note: routed (PhysicProcess_Jumping, ForceDownAndDetonationProcess,
  CommonFrameMove stun test, IsDamageState, IsSocketDownTargetState, Aisha's skill lookup `32cebe80 @32cec2bf`), automatic through the virtual
  (StateChange flush, BlackHole `31f6eea0`, IsNotCommonMoveState), or no March body (CommonEventProcess `+0x8e74` block, `31fd7350`, `32296570`,
  `3208e7d0`). Open two routed sites against June's listing; check the getter order and that the `#else` path is March's untouched.
- **Flag-off = March.** The probe `cl /P` of `X2GUUser.cpp` with the flag on and off differs only inside the flag's blocks.

Hygiene: `python tools/flagcheck.py SERV_IRUHADEV_JUNE_USER_DAMAGE_STATE_GETTERS --range <DU2 commit>^..<DU2 commit>` (then
`git log -SSERV_IRUHADEV_JUNE_USER_DAMAGE_STATE_GETTERS <DU2 commit>..HEAD`), `python tools/junediverge.py --check-all`. Paste the summary lines;
explain every OTHER row (`Always.h`: peers' blocks).

Write "DU2 - Review" into the plan doc, update the DU2 row in STATUS.md (done; CH1p's DU2 dependency met), and commit. No play-test unless a
fix lands (then `build.py --deploy` and a hand-off).
