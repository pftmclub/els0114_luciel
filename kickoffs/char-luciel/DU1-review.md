# June character Lu/Ciel (Stage 8) - Phase DU1 review (covers DU1, DU1b, DU1b2, DU1b3, DU1b4, DU1c, DU1d)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU1 review` (after DU1d is done) |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU1, DU1b, DU1b2, DU1b3, DU1b4, DU1c, DU1d done (their Done notes and commits) |
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

## Amended by DU1b3 (2026-11-14)

- DU1b3 split again: DU1b3 = the physics (`m_MinorUnitPhysicParam` June unit `+0x36c`, the whole `PhysicProcess` second body, the helpers
  `PhysicProcess_PushByUnits` = June `32010060` and `PhysicProcess_LandOnLine` = June `32010440`, the not-selected offset); DU1b4 = the
  speed keys, the frame-move speed readers and the camera / armor keys. Check that both helpers are March's inline blocks verbatim (live
  branches) on the main body, and the two JUNE_BUGS rows 659 / 660 against `@3201917c` / `@3201a009`.
