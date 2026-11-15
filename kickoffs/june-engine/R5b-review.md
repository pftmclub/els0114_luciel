# June engine remainder - Phase R5b review - CUSTOM_RENDER_STATE per-equip switch

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-engine R5b review` |
| Plan doc | `JUNE_ENGINE_REMAINDER_PHASES.md` (branch `main`) |
| Depends on | R5b done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CUSTOM_RENDER_STATE_EQUIP` |
| Log tag | none (the phase has no `_DEBUG`; nothing reaches the block in March data) |

## Read first

```sh
python tools/docsection.py JUNE_ENGINE_REMAINDER_PHASES.md "Starting context" "Phase R5 -- Done" "Phase R5b -- Done"
grep -n "(R5b)" june2017_re/funcmap.tsv
```

## Task

Review R5b's commit against June `32063650 @32065bc1-32065c9f`, `3295a920`, `3295a990`, `32f08a00`, `32f08a50`, `32f08a90`, `32961c80 @32962937-32962958`,
`3227b220` and `3227a2f0`. What this phase could get wrong:
- the pick values: CRS 3 and 9 take the short path, 5 the long one (`SUB 3` / `SUB 2` / `SUB 4` chain at `@32065bc5-32065bd6`); 1, 2, 4, 6, 7, 8 and 0 all fall to the exit path;
- the long path's order and arguments: sub-equip pass, `3322a9c0( false )` on `+0x6754`, `322720f0( true, 3 )`, `31cfe4a0( false, false )`, `3200a590( true )`;
- the exit path: gated on `+0x941c` = 1, the last-index test against the view list size, `EnableAfterImage( 3 )` only when `+0x7c80` is 1, then restore, `SetShowObject( true, false )`,
  `SetInvisibility( false )`, user vslot `0x630`;
- `+0x7c80`: set after the enable in Create, cleared in Erase only when an element was found; the `+0x1d1` clears left unported (check that a quick Ghidra / `disasm_range.py` look still finds no reader);
- `CX2SubEquip` default param captured at the end of the per-item switch in `LoadAttached`, never before it, and `m_vecpSubEquip` pass skipping non-skin sub-equips in `3295a920` but not in `3295a990`;
- every `#else` is March's original line (the edits are insert-only).

Hygiene is one command, not a fresh script:
`python tools/finish.py --review --commits <R5b commit> --with SERV_IRUHADEV_JUNE_CUSTOM_RENDER_STATE_EQUIP` (flagcheck, junediverge `--check` plus `--check-all`, enumscan).
Paste its block into the review. Run it with `-v` and explain every OTHER row and every deleted line; a FAIL or ERROR is a finding.

Spot-check 2-3 items directly in Ghidra (read-only); if one fails, re-check the whole phase. Fix clear transcription errors under the same flag; hand design decisions back to the user.
No build in this review unless a code fix lands (then `/Zs` probe the file; a full build waits for the next play-test phase). Write "Phase R5b -- Review" into the plan doc, update the
phase's STATUS.md row, check the next phase's kickoff file, and commit.
