# June engine remainder - Phase R14e review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-engine R14e review` |
| Plan doc | `JUNE_ENGINE_REMAINDER_PHASES.md` (branch `main`) |
| Depends on | Phase R14e done (its Done note and commits) |
| Flag | `SERV_IRUHADEV_JUNE_INIT_EQIP`, `SERV_IRUHADEV_JUNE_VIEWER_EQUIP_VISIBILITY` |
| Log tag | `[JUNE_ENGINE]` (R14e added no log line) |

## Read first

```sh
python tools/docsection.py JUNE_ENGINE_REMAINDER_PHASES.md "Rules" "Phase R14e - InitEqip and the character viewer's slot visibility (Raven left arm)" "Phase R14e -- Done"
```

and the funcmap rows `(R14e)`, `JUNE_BUGS.md` row 743, `june_march_divergent.tsv` rows `CX2GUUser::InitEqip` and
`CX2UnitViewerUI::UpdateEquipSlotVisibility`.

## Task

Review R14e's commit(s) against June `32024510` (InitEqip) and `31f94360` (the viewer's UpdateEquipSlotVisibility):
- **The viewer's division-group run:** each group's DITT value and the slots it clears, read from June's byte offsets (`+0x958 + pos * 3 + body`;
  pos 27 = `EP_AC_TEMP3` / June `EP_AC_EARRING`, 28 = `EP_AC_TEMP4` / `EP_AC_RING2`), the order of the tests, which ones count toward the per-body
  limit and which `break` the walk (1001 breaks without counting), and the `else` chain of the hair / face run (43 also needs no SET2 hide; with a
  SET2 hide it falls through to 65).
- **Bodies:** body = equip `+0x15d4` (`IsMinorUnitEquip`) everywhere except the nasod-hand pass, which reads the inventory slot (`3286a350`);
  the per-body passes read body 0's then body 1's fashion slot. The stand-ins: bodies 1-2 have no fashion / normal UID table in March's viewer
  (char-luciel DU5), so the apply loop runs body 0 only.
- **InitEqip:** the item-ability pass (accessory or fashion -> inventory body == `m_bMinorUnitActive`; other items -> not a minor equip), the
  direct (non-virtual) `UpdateNotRenderingOtherEquipList` call, and the main-weapon particle stand-in (June `32961c80` case 2, routed to R14d).
- The `#else` paths are March's bodies unchanged.

Hygiene is one command, not a fresh script:
`python tools/finish.py --review --commits <R14e's commits> --with SERV_IRUHADEV_JUNE_INIT_EQIP SERV_IRUHADEV_JUNE_VIEWER_EQUIP_VISIBILITY`
(flagcheck per flag, junediverge `--check` plus `--check-all`, enumscan). Paste its block into the review. Run it with `-v` and explain every OTHER
row and every deleted line; a FAIL or ERROR is a finding.

Spot-check 2-3 items directly in Ghidra (read-only), e.g. DITT 41's sixteen slots (@`31f94d4x`), the nasod-hand pass @`31f952c3`, and the
item-ability test @`32024a17`; if one fails, re-check the whole phase. Fix clear transcription errors under the same flag (then
`tools/build.py --deploy`); hand design decisions back to the user. Write "Phase R14e -- Review" into the plan doc, update the phase's STATUS.md
row, check R14d's kickoff (amended by R14e), and commit.
