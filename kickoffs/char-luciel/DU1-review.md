# June character Lu/Ciel (Stage 8) - Phase DU1 review (covers DU1, DU1b, DU1b2, DU1b3, DU1b4, DU1c, DU1c2, DU1d, DU1d2, DU1d3, DU1d4)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU1 review` (after DU1d2 is done) |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU1, DU1b, DU1b2, DU1b3, DU1b4, DU1c, DU1c2, DU1d, DU1d2 done (their Done notes and commits) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (+ `SERV_IRUHADEV_JUNE_CHAR_CLASS_GRADE`, DU1) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Phase DU - The two-body engine (DU1-DU5)" "DU1 - Done (2026-11-14)" "DU1b - Done (2026-11-14)" "DU1b2 - Done" "DU1b3 - Done" "DU1b4 - Done" "DU1c - Done" "DU1d - Done"
```

## Task

Review the seven parts (DU1b: the switching `SetActiveBody` / `UpdateDuo` / the Now apply `31fc9510`, the latch `+0x8e93`, event flags 8 / 9) against `nu0_duo_engine.tsv` (every `owner` = `DU1` row has a verdict in one of the Done notes) and June's listings:
- **Members and offsets:** the second body mirrors the first (`+0x6b4`): mesh / anim / frame members, `+0x8e90` / `+0x8e91` / `+0x8e92` /
  `+0x8e93`, the not-select block `+0x8f50..+0x8f68`; ctor values (`3203e850 @32042429-32042479`, `+0x8f5c` word, `+0x8f6e`).
- **Accessors:** each one against `32070cf0` / `32070d10` / `32071d60` / `32071d80` / `32071db0` / `32071d40`; `GetSkinAnimPtr` stays the
  first body (vslot 22 = `32071d20`).
- **InitMotion:** the mix order (Motion1 before Motion2 for the second body; the first body's order is March's), the grade gates, the
  `GetX2UnitClassGrade` replacement (`@31fe5c5c`) and its three callers.
- **DU1b-DU1d:** `DUO_TYPE` values and miss defaults, the not-selected-body math, the sync bit layout and encodings, and that every
  accessor site edited in `X2GUUser.cpp` is one June reads with the ternary (no extra sites).
- **No change for other classes:** a `cl /P` diff of `X2GUUser.cpp` with the flag on and off for a non-Lu/Ciel path is not possible in the
  shared tree (it needs an `Always.h` edit): use flagcheck's unifdef strip (flag-off = `<first DU1 commit>^` byte for byte) and argue form 0.

Hygiene: `python tools/flagcheck.py SERV_IRUHADEV_JUNE_CHAR_DUO_BODY --range <first DU1 commit>^..HEAD --with SERV_IRUHADEV_JUNE_CHAR_CLASS_GRADE`
and `python tools/junediverge.py --check-all`. Paste the summary lines. Explain every OTHER row and deleted line; a FAIL or ERROR is a finding.

Spot-check 2-3 items in Ghidra (read-only); if one fails, re-check that part whole. Fix clear transcription errors under the same flag;
design decisions go to the user. Write "DU1 - Review" into the plan doc, update STATUS.md (CH1a0, CH1p and CH3c wait on this review),
and commit.

## Amended by DU1d (2026-11-14)

- DU1d split again: the review also covers **DU1d2** (add `"DU1d2 - Done"` to the docsection line; it now runs after DU1d2). Check DU1d's 22
  macro twins in `X2GUUser.h` (each reads `GetXSkinAnimFuture()` where the original read `m_pXSkinAnimFuture`, nothing else changed) against
  June's expansions in `31fcc350` / `31fcc9d0` / `31fe7610`, and that every DU1d / DU1d2 site is a `du1d_body_reads.tsv` row (no extra sites).
  Spot-check two "none" owners (`31fcce20`, `3200c550` unreferenced; `31fcbd10` = SUMMON_MAGIC_SPEAR by Elsword's registration `@32d22329`).

## Amended by DU1b3 (2026-11-14)

- DU1b3 split again: DU1b3 = the physics (`m_MinorUnitPhysicParam` June unit `+0x36c`, the whole `PhysicProcess` second body, the helpers
  `PhysicProcess_PushByUnits` = June `32010060` and `PhysicProcess_LandOnLine` = June `32010440`, the not-selected offset); DU1b4 = the
  speed keys, the frame-move speed readers and the camera / armor keys. Check that both helpers are March's inline blocks verbatim (live
  branches) on the main body, and the two JUNE_BUGS rows 659 / 660 against `@3201917c` / `@3201a009`.

## Amended by DU1b4 (2026-11-14)

- DU1b4 added a second flag, `SERV_IRUHADEV_JUNE_DIVERGE_COMMON_STATE_END_FUTURE` (June's `CommonStateEndFuture` prefix, every class; its
  `june_march_divergent.tsv` row). Review it with the DU1 series: `flagcheck JUNE_CHAR_DUO_BODY --with JUNE_DIVERGE_COMMON_STATE_END_FUTURE`
  over the range, and read `31fdd8e0` against the prefix. The camera keys DU1b4 routed to Stage 2 are not this review's.

## Amended by DU1c (2026-11-14)

- The review also covers **DU1c2** (read its Done note; add `"DU1c2 - Done"` to the docsection line) and DU1c's divergence flag
  `SERV_IRUHADEV_JUNE_DIVERGE_UP_MP_ITEM_PVE` (add it to the flagcheck `--with` list): June `322e8690` and @`322fd2fe` skip item 200680 in
  `GT_PVE`. Check DU1c's sync bit order against `32d5be30` (E, B x / y / z, F before 0x200000; I after 0x800000) and the `+0x88c8` / `+0x88d4`
  identification (`CashItemAbility`, three readers, three writers).

## Amended by DU1c2 (2026-11-14)

- Check DU1c2's `ProcessUpgradeWeaponParticle` `UT_LUCIEL` case against the listing of `32292610` (@`32292f71-3229328d`, tail `32293254` ->
  `32292ae0`): which sequence gets which position (stack slots +0x10 ... +0x28 = seq 5 ... 10), and the new `GetWeaponIndexHavingModel` base.

## Amended by DU1d2 (2026-11-14)

- DU1d2 split again: the review also covers **DU1d3** and **DU1d4** (add `"DU1d2 - Done"`, `"DU1d3 - Done"`, `"DU1d4 - Done"` to the docsection
  line; it now runs after DU1d4, which builds and hands off the regression play-test). Add DU1d2's flag `SERV_IRUHADEV_JUNE_DIVERGE_PLAY_SOUND_REVIVE`
  to the flagcheck `--with` list and read `31fdb390` against it. Check `GetCloneFrameByBodyPrefix` against `31fd9200` / `31fda280` / `31fda680`
  (strings `33473188` "MAJOR:" / `33473178` "MINOR:"; the D-phase `"MAJOR_"` was the error DU1d2 fixed), `RotateBone`'s second call
  (`@31fd954e`, JUNE_BUGS 668), the `UT_LUCIEL` finger loops (`@31fde94f` / `@31fdeffa`: start 0, end 4, unit `+0xe40 != 1`) and that the
  census rows DU1d3 re-attributed name the right function.

## Amended by DU1d3 (2026-11-14)

- DU1d3 split again: the review also covers **DU1d3b** (add `"DU1d3b - Done"` to the docsection line). Check DU1d3's second after-image against
  June (`32275a30` / `322720f0` mode pick, InitComponent `@31feec8a`, TimedEvent `@31ff2882`, CommonStateStart `@3205b699`, NotifyShowObjectChanged
  `@31ff1dac` / `@31ff1db7` = JUNE_BUGS 669), that `IsMinorUnitActive` is virtual in `CX2GameUnit` (base false = June `31e4c550`), and that
  every March caller of `EnableAfterImage` / `DisableAfterImage` keeps mode 3 (June's NPC callers pass 3 too).

## Amended by DU1d3b (2026-11-14)

- Check DU1d3b's 17 hunks (`X2GUUser.cpp`, `grep -n "char-luciel DU1d3b"`) against the funcmap rows `(DU1d3b)`: `PlayEmotion` (riding Play, MixAnim,
  ChangeAnim / Play, unit-viewer duo row), `EmotionFrameMove`, `OnFrameMove` scale pair, `CommonFrameMoveFuture_TimedEvent` (4), `CommonFrameMove_TimedEvent`
  (attack window + 4 stop lists), `SoundEventTimer`, `CommonFrameMove_Particle`; and the routes (Stage 2 / 10 / 16, DU3, DU5).
- **Finding for the review:** DU1b's `_CONST_GUUSER_DU1B_` says DUO_TYPE has "no Enum entry in June", but June's `Enum.lua` has `DUO_TYPE`
  (`DT_MAJOR` 0, `DT_MINOR` 1, `DT_BOTH` 2, `DT_KEEP` 3; `june_src_fixed/data036_kom/Enum.src.lua:17515`). DU1b's `DUO_TYPE_NONE = 3` ("the duo
  ends") shares `DT_KEEP`'s value: decide whether the constants take June's names (enum rule) and whether value 3 keeps or ends the duo.

## Amended by DU1d4 (2026-11-14)

- DU1d4 split again: the review also covers **DU1d5** (the two-body render) and runs after it. Add `"DU1d4 - Done"` and `"DU1d5 - Done"` to the
  docsection line. Check DU1d4's hunks (`X2GUUser.cpp`, `grep -n "char-luciel DU1d4"`, 21 blocks) against the funcmap rows `(DU1d4)` and the
  census's corrected function starts (`du1d_body_reads.tsv` rows whose owner begins `DU1d4`).
- The DU1 regression play-test ran in DU1d4 (its Done note has the result); DU1d5 hands off its own render check.

## Amended by DU1d5 (2026-11-14)

- DU1d5 added `SERV_IRUHADEV_JUNE_DIVERGE_USER_RENDER` (June-only render lines, every class; two `june_march_divergent.tsv` rows). Hygiene over the
  range: `flagcheck JUNE_CHAR_DUO_BODY --with JUNE_DIVERGE_USER_RENDER` on the code files, and `Always.h` per flag without `--with` (the DU1d2
  compound guard is an ERROR under `--with`). Read `CommonRender_PrepareBody` against `31ff03d0` line by line (the five DIVERGE lines and the
  hide-equip override), `CommonRender_Prepare` against `31fd07b0` (hint order) and `OnFrameRender_Draw` against `31fd0880`; JUNE_BUGS 673.
