# June engine remainder - Phase R14c review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-engine R14c review` |
| Plan doc | `JUNE_ENGINE_REMAINDER_PHASES.md` (branch `main`) |
| Depends on | Phase R14c done (its Done note and commits) and its play-test passed (the build was blocked by CH3c20's guard when R14c finished) |
| Flag | `SERV_IRUHADEV_JUNE_EQIP_BASIC_COLLISION_ANIM` (+ `_DEBUG`, retired by this review) |
| Log tag | `[JUNE_ENGINE]` (`python tools/logs.py --grep "EQBASIC"`) |

## Read first

```sh
python tools/docsection.py JUNE_ENGINE_REMAINDER_PHASES.md "Phase R14c - Basic-item collision anim (+0x584 / +0x5e4)" "Phase R14c -- Done" "Phase R14b -- Done"
```

and the funcmap rows `(R14c)` (`grep -n "(R14c)" june2017_re/funcmap.tsv`).

## Task

Review R14c's commit(s) against June `3295dfa0` @`3295e2a9`-`3295e4de`, `3295a570`, `32964ae0` @`32964f22`, `329602f0`, `329657d0`,
`31fd8c00` @`31fd8d40` and `32cc8d30` @`32cc8fb6`. What this phase could get wrong:
- the outer test (owner set; templet `+0xd2` is neither `UT_ROSE` 0xb nor `UT_ADD` 9), and where a NULL basic templet jumps (past the pair) versus a
  NULL minor-unit templet (to the anim build, JUNE_BUGS 734);
- the model index (`i`, since the Add remap cannot apply in this branch) and the dead Rose remap;
- the AddModelXSkinMesh arguments (normal XETs `+0x80c` / `+0x808` / `+0x804`, bCloseDeviceAtDestructor 0, bAddCollisionData 1) and the jiggle
  (the WORN item's DITT 30, tension 1.5, expand 0);
- the frame move order (basic anim before the normal anim, only with `m_pUnitMatrix[i]`) and that nothing draws it;
- the dtor: the unit-anim remove of `+0x5e4[i]` for every attachment, delete after the hyper anim, close after the hyper model;
- both collision sites (Rose / Add -> normal anim, else the basic anim) and that the `#else` is Phase S's stand-in unchanged.

Judge the play-test log: every `EQBASIC` line's `basic_item` must be the class's UnitTemplet `basicWeaponItemID` (Aisha 21, Eve 41, Elsword 7) and
`attack_lines` above 0. `attack_lines 0` means the basic mesh has no hit lines: compare June's same-name mesh with March's before any code change.

Retire `SERV_IRUHADEV_JUNE_EQIP_BASIC_COLLISION_ANIM_DEBUG` (comment out the `#define`; no build, per the debug-retire rule).

Hygiene is one command, not a fresh script:
`python tools/finish.py --review --commits <R14c's commits> --with SERV_IRUHADEV_JUNE_EQIP_BASIC_COLLISION_ANIM SERV_IRUHADEV_JUNE_EQIP_BASIC_COLLISION_ANIM_DEBUG`
(flagcheck per flag, junediverge `--check` plus `--check-all`, enumscan). Paste its block into the review.
Run it with `-v` and explain every OTHER row and every deleted line; a FAIL or ERROR is a finding.

Spot-check 2-3 items directly in Ghidra (read-only); if one fails, re-check the whole phase.
Fix clear transcription errors under the same flag (then `tools/build.py --deploy`); hand
design decisions back to the user. Write "Phase R14c -- Review" into the plan doc, update the
phase's STATUS.md row, check the R14d kickoff, and commit.

## Amended by char-luciel DU3c (2026-11-15)

- Route or port the equip's DG chain byte readers (`CX2Eqip::m_bAttachedInDGManagerChain`, June `+0x15d5`): `32961c80 @32962094` adds a new sub-equip to
  the DG chain only when it is set, `32960ad0 @32960c7a / @32960d33` unchains a new attached particle (and sets its `+0x45a`) only when it is clear. Only the
  viewer callers pass 0 (`31f9c640`, `31f9df40`, `3280a170`, `32810500`, `32812380`); detail in `R14c.md` "Amended by char-luciel DU3c".
