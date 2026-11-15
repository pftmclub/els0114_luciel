# June engine remainder - Phase R14d review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-engine R14d review` |
| Plan doc | `JUNE_ENGINE_REMAINDER_PHASES.md` (branch `main`) |
| Depends on | Phase R14d done (its Done note and commit) and its play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_EQIP_FRAME_MOVE` (+ `_DEBUG`, retired by this review), `SERV_IRUHADEV_JUNE_EQIP_DTOR`, the `RPUI_RAKSHASA` line under `SERV_IRUHADEV_JUNE_ENUM_NAMES` |
| Log tag | `[JUNE_ENGINE]` (`python tools/logs.py --grep "EQTRACE"`) |

## Read first

```sh
python tools/docsection.py JUNE_ENGINE_REMAINDER_PHASES.md "Phase R14d - June's whole CX2Eqip::OnFrameMove and the DG-chain byte readers" "Phase R14d -- Done"
```

and the funcmap rows `(R14d)` (`grep -n "(R14d)" june2017_re/funcmap.tsv`), JUNE_BUGS 745-746, the three tsv rows R14d added
(`LoadAttached`, `AttachParticle`, `CX2Eqip::AttachedParticleGroup::OnFrameMove`) and the `LoadAttached` allow row.

## Task

Review R14d's commit against June `32964ae0`, `329645a0`, `3295d8a0` and `329602f0`. What this phase could get wrong:
- the hidden gate: GetShowObject (`+0xc` manager through `31fae370`, own `+0x172`), then only UT_ATTACH_ANIM with my unit's riding pet id
  (`+0x683c` -> pet `+0x350`, is it `RidingPetInfo::m_Id`?) == 63 `RPUI_RAKSHASA`; E_FAIL elsewhere;
- the UT_SKIN switch against the two jump tables (`32965660` / `32965668`, `3296568c` / `32965694`): face ids -> `SetNoRenderable` only, the 13
  alpha ids, default alpha 0, hyper 202560 / 202610 only; mesh `+0x8d0` / `+0x864` are what `SetNoRenderable` / `SetAlphaRender` write;
- TraceLandPos: frame 0 for every slot (JUNE_BUGS 745), the offset re-read with the body, `329645a0` (village state only by name: June's 15 is
  `XS_GUILD_AGIT_GAME`; square unit `+0x4a0` = `m_LandPosition`; user vslot `0x554` = `GetLandPosition`), the root frame's combine translation;
- the attach-anim order (basic, normal, hyper with `+0x1c4` / `+0x1c5`), the matrix (scale x yaw-pitch-roll( y, x, z ) x translation x unit
  matrix), the body argument in UT_ATTACH_NORMAL, LoadAttached after types 1 and 2 only;
- the three stand-ins (`3295ce00` and `329646c0` -> R6c, the `+0x15f0` switch -> Stage 11) and that nothing else June does is missing;
- the dtor: `ReleaseEquipEffectSet( true )` after the sub-equips matches `329602f0`'s `321dafb0( &+0x15f8, 0 )`.

Judge the play-test log: every `EQTRACE` line has `found 1` in field / dungeon / village and `offset` = frame offset + land - root (the item sits
at the unit's feet). A line from the character window (JUNE_BUGS 746) is expected to show a world-sized offset.

Retire `SERV_IRUHADEV_JUNE_EQIP_FRAME_MOVE_DEBUG` (comment out the `#define`; no build, per the debug-retire rule).

Hygiene is one command, not a fresh script:
`python tools/finish.py --review --commits <R14d's commits> --with SERV_IRUHADEV_JUNE_EQIP_FRAME_MOVE SERV_IRUHADEV_JUNE_EQIP_FRAME_MOVE_DEBUG SERV_IRUHADEV_JUNE_EQIP_DTOR --skip-flag SERV_IRUHADEV_JUNE_ENUM_NAMES "one RPUI_RAKSHASA enum line"`
(flagcheck per flag, junediverge `--check` plus `--check-all`, enumscan). Paste its block into the review.
Run it with `-v` and explain every OTHER row and every deleted line; a FAIL or ERROR is a finding.

Spot-check 2-3 items directly in Ghidra (read-only); if one fails, re-check the whole phase.
Fix clear transcription errors under the same flag (then `tools/build.py --deploy`); hand
design decisions back to the user. Write "Phase R14d -- Review" into the plan doc, update the
phase's STATUS.md row, check the R14f kickoff, and commit.
