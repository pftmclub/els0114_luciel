# June engine remainder - Phase R14f review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-engine R14f review` |
| Plan doc | `JUNE_ENGINE_REMAINDER_PHASES.md` (branch `main`) |
| Depends on | Phase R14f done (its Done note and commit) and its play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_EQIP_LOAD_ATTACHED`, `SERV_IRUHADEV_JUNE_DIVERGE_EQIP_SET_FIXED`, two DITT lines under `SERV_IRUHADEV_JUNE_ENUM_NAMES` |
| Log tag | `[JUNE_ENGINE]` (no R14f log line: the checks are visual) |

## Read first

```sh
python tools/docsection.py JUNE_ENGINE_REMAINDER_PHASES.md "Phase R14f - June's whole CX2Eqip::LoadAttached" "Phase R14f -- Done"
```

and the funcmap rows `(R14f)` (`grep -n "(R14f)" june2017_re/funcmap.tsv`), JUNE_BUGS 771, the tsv rows `LoadAttached`, `CX2Eqip::SetFixed`,
`CX2SubEquip::CX2SubEquip`, `CX2UnitViewerUI::SetFixed` and the allow row `CX2Eqip::LoadAttached` -> `AttachParticle`.

## Task

Review R14f's commit against June `32961c80`, `32f09170` and `3295a560`. What this phase could get wrong:
- the pass count (8 only for templet unit type 9 and item type 1: `32961cdd-32961d09`), the Rose doubling (`+0xd2 == 0xb`, `+0xd5 == 2`) for both
  the meshes and the particles, the frame choice (`32961dbb-32961e3b`: skin -> `+0x1c0`, else `+0x544[ slot ]`, then `[ i ]` when i > 0 and not the
  8-pass case) and the bound JUNE_BUGS 771 adds;
- the forced-skin test order (custom `+0xa9`, DITT 0x1d, DITT 0x53) and the ctor's 4th argument (`+0x804` = `m_pNormalTexChangeXET`);
- every item id against its render group: re-decode the five jump tables (`32962c18`, `32962cb8`, `32962dc4`, `32962eb4`, `32962ec8`) and walk
  the compare trees (`329620c4-329626fb`, inner `32962788-3296290f`); for each group the fields set (normal param `+0x198` vs `GetRenderParam`
  vslot `0x14`), the word writes at `+0x64` (`bAlphaBlend` + `bZWriteEnable`), `+0xa4` `bAlphaTestEnable`, the `100003637` colour (alpha 0.3f);
- the NULL-param `return` (@`32962782`) and that no other path leaves the function;
- the particle tail's position table (`32962f38`), the Lu / Ciel test (`32962b83-32962beb`) and the two R6c stand-ins;
- `SetFixed` and that the fixed draw still draws an unchained equip's sub-equips and particles once.

Hygiene is one command, not a fresh script:
`python tools/finish.py --review --commits <R14f's commits> --with SERV_IRUHADEV_JUNE_EQIP_LOAD_ATTACHED SERV_IRUHADEV_JUNE_DIVERGE_EQIP_SET_FIXED --skip-flag SERV_IRUHADEV_JUNE_ENUM_NAMES "two DITT enum lines"`
(flagcheck per flag, junediverge `--check` plus `--check-all`, enumscan). Paste its block into the review. Run it with `-v` and explain every OTHER
row and every deleted line; a FAIL or ERROR is a finding.

Spot-check 2-3 items directly in Ghidra (read-only); if one fails, re-check the whole phase.
Fix clear transcription errors under the same flag (then `tools/build.py --deploy`); hand design decisions back to the user. Write
"Phase R14f -- Review" into the plan doc, update the phase's STATUS.md row (and char-luciel DU5d2's: its R14f dependency is met), write or check
the next phase's kickoff file, and commit.
