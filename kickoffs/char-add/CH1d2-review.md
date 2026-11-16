# June character Add (Stage 7) - Phase CH1d2 review (covers CH1c-CH1d2)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-add CH1d2 review` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | CH1c, CH1d and CH1d2 done (their Done notes and commits: `git log --oneline --grep "char-add Phase CH1[cd]"`; CH1d0 has its own review) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_CORE`, `SERV_IRUHADEV_JUNE_CHAR_ADD_LIVE` (`//#define`, inert) and `_DP` (inert) |
| Log tag | none (nothing runs until CH1 defines `_LIVE`; no play-test) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Rules"
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md "CH1c - Done (2026-11-15)" "CH1d - Done (2026-11-16)" "CH1d2 - Done (2026-11-16)"
grep -P "Char Add CH1(c|d|d2):" june2017_re/funcmap.tsv
awk -F'\t' '$7=="CH1c" || $7=="CH1d" || $7=="CH1d2"' june2017_re/char_add/ch0_class_map.tsv
```

## Task

Review the CH1c, CH1d and CH1d2 commits against Ghidra (read-only) and the raw listings (`python june2017_re/charport_adis.py add --phase CH1c|CH1d|CH1d2`):
- **Every row is written:** the 2 CH1c, 45 CH1d and 27 CH1d2 class-map rows each have a body in `X2GUAdd.cpp` ( an `inside` row under `_LIVE` with March's in
  `#else`, a `new` row under `_CORE`, or the Done note's "= March" list ); the tag `// char-add CH1c|CH1d|CH1d2: June <va>` names the right address.
- **CH1d2's 9 "= March" rows:** spot-check 3 ( `329d9520` the loop's foot / 3.0 s test and both state ids, `329d2100` the end-future turn, `329e48d0`'s inlined
  `PlayLoopSound` = JUNE_BUGS 125 ); if one fails, re-check all 9.
- **CH1d2's key flags in Ghidra:** one ground dash ( `329d7720` ZBackX direction test and flag order 0, 2, 1, 3, 4 ), one jump listing-only ( `329d9020` the three
  class tests 0x7b / 0x7a / 0x7e ), the XX finish MP gate `329edb00` ( `FlushMp( 2.0 )` = vslot 0xe4, the else path also taken without a game ) and the three
  new Z / ZZ / ZZZ FrameMoves ( byte-identical, JUNE_BUGS 783 ).
- **The render-state stand-in:** `DashComboXStateStart` / `End` / `FrameMove` keep and restore `m_eCustomRenderState` and the alpha; check that nothing else reads
  `+0x95b4` / `+0x95b8` in the class range ( `charport_adis.py add` greps ), that `CRS_COMBO_DASH_X` is a fair stand-in for June's 5 ( `31fd4460` case 5 ) and hand the
  `ApplyRenderParam` question to CH1h.
- **CH1c and CH1d** were not reviewed by anyone yet: spot-check as their kickoffs say ( CH1c `329f5490` shock block, CH1d the nasod arm `32a088c0` and
  `329edde0` ), and confirm the 10 + 13 June state names appended by CH1d / CH1d2 carry the ids of `ch0_state_skill.tsv`.
- No `_DEBUG` define exists for these phases.

Hygiene is one command, not a fresh script: `python tools/finish.py --review --commits <CH1c, CH1d and CH1d2 commits> --with JUNE_CHAR_ADD_CORE JUNE_CHAR_ADD_LIVE
JUNE_CHAR_ADD_DP` ( add `-v`; explain every OTHER row and deleted line; a FAIL or ERROR is a finding ). Paste its block.

Fix clear transcription errors under the same flag ( no full build: no play-test; prove it with the one-file `/Zs` probe, `_LIVE` off and on ); hand design
decisions back to the user. Write "CH1d2 - Review" into the plan doc, update the CH1d2 review STATUS row, check that `kickoffs/char-add/CH1e.md` is ready
( CH1e is unblocked by this review ), and commit.
