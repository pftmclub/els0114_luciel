# June character Add (Stage 7) - Phase CH1b review (covers CH1a-CH1b)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-add CH1b review` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | CH1a and CH1b done (their Done notes and commits: `git log --oneline --grep "char-add Phase CH1[ab]"`) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_CORE`, `SERV_IRUHADEV_JUNE_CHAR_ADD_LIVE` (`//#define`, inert) and `_DP` (inert, CH1a) |
| Log tag | none (nothing runs until CH1 defines `_LIVE`; no play-test) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Rules"
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md "CH1a - Done (2026-11-15)" "CH1b - Done (2026-11-15)"
grep -P "\tChar Add CH1[ab]|Char Add CH1[ab]:" june2017_re/funcmap.tsv
awk -F'\t' '$7=="CH1a" || $7=="CH1b"' june2017_re/char_add/ch0_class_map.tsv
```

## Task

Review CH1a's and CH1b's commits against Ghidra (read-only) and the raw listings (`python june2017_re/charport_adis.py add --phase CH1a|CH1b`):
- **Every row is written:** the 32 CH1a and 28 CH1b class-map rows each have a body in `X2GUAdd.cpp` (an `inside` row under `_LIVE` with March's in `#else`,
  a `new` row under `_CORE`); the tag `// char-add CH1a|CH1b: June <va>` names the right address.
- **CH1b's two June-only pieces, in Ghidra:** the Apocalypse tail of the two Die Starts (`32a00010`, `32a000a0`: `HaveThisBuff( 0x159 )` -> `329fb710( 0x644 )`)
  and `FinishSummonedNPC` `329fb710` (loop gates, the `0x644` branch, `+0x8b40` / `+0x98e4` tests, the `0xf4` call = `CX2GameUnit::SetNowHp`, `StateChange_LUA`
  string `APOCALYPSE_FINISH_ATTACK`); `OnApocalypseSummonEnd` `329fb6c0`. Check that `m_bApocalypseFinishBlocked` has no writer of true in the class
  range (JUNE_BUGS 751) and that Eve CH2b4's inline copy in `X2GUUser.cpp` `ResetOnSummonEnd` is left alone (CH2b0 routes it).
- **The 26 March-equal bodies:** spot-check 3 (DamageDownBack EventProcess `329d5730` ids and flag order, a StandUpRolling EventProcess `329d5970` with 45.0,
  `329d5550` EventTimerOneshot vs `X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE`); if one fails, re-check all.
- **CH1a's** `_DP` lines and JumpComboZ / X arms were reviewed by nobody yet: read them against the Done note's list.
- No `_DEBUG` define exists for either phase.

Hygiene is one command, not a fresh script: `python tools/finish.py --review --commits <CH1a and CH1b commits> --with JUNE_CHAR_ADD_CORE JUNE_CHAR_ADD_LIVE
JUNE_CHAR_ADD_DP` (add `-v`; explain every OTHER row and deleted line; a FAIL or ERROR is a finding). Paste its block.

Fix clear transcription errors under the same flag (no full build: no play-test; prove it with the one-file `/Zs` probe, `_LIVE` off and on); hand
design decisions back to the user. Write "CH1b - Review" into the plan doc, update the CH1b STATUS row, check that `kickoffs/char-add/CH1c.md` is
ready (CH1c is unblocked by this review), and commit.
