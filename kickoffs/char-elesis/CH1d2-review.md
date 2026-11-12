# June character Elesis (Stage 7) - Phase CH1d2 review (covers CH1c-CH1d2)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH1d2 review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH1c, CH1d and CH1d2 done (their Done notes and commits) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (`//#define`), `_CORE` (defined) |
| Log tag | none (nothing runs until `_LIVE`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "CH1c - Done" "CH1d - Done" "CH1d2 - Done"
awk -F'\t' '$7=="CH1c" || $7=="CH1d" || $7=="CH1d2"' june2017_re/char_elesis/ch0_class_map.tsv
```

## Task

Review the functions of CH1c (2), CH1d (9) and CH1d2 (9) against June (`python june2017_re/charport_adis.py elesis <va>...`; the listing carries the `StateChange` arguments and the `fcomp` / `test ah` direction; `je` after `cmp byte, 1` jumps when the byte IS 1; the map's column 9 says which rows are `listing-only`). Spot-check at least:
- **`32b559e0`** (DashComboZ EP): flag order 4 / 5 / 1 / 2 / 0 / 3, the `IsInJobLine` argument per flag (3 / 3 / 1 / 2 / none / 2), the Z / X key per flag, the six target ids, flag slots `+0x7d85`-`+0x7d8a` and times `m_fEventTime[0..5]` (`+0x8c10` + 12 per index).
- **`32b562f0`** (DashComboX EP): flag 0 = X + class `0x78` + `GetNowMp() >= 2.0` (`fcomp` / `test ah, 1` / `jne` skips when MP < 2.0), flag 1 = Z + class `0x7d`.
- **`32b56cc0`** (DashJumpComboX EP): order 4 / 1 / 2 / 0 / 3 and the compare per flag (`0x7d`, `0x78`, `0x79`, none, `0x78`) with the Z / X key; JUNE_BUGS 628 beside CH1d's 627. **`32b56710` / `32b56aa0` / `32b57100`:** the `IsInJobLine` arguments and the target ids; `32b57100`'s `y -= 45`.
- **CH1d:** the nine combo functions against their listings, the file-local flag / cancel macros (`ELSE_IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_`, `_ON_LINE_`), ComboZZZ's flag order 2 / 0 / 3 / 4 / 1, ComboX / ComboXX job lines. **CH1c:** `RidingHyperModeFrameMove`, `CommonHyperModeFrameMove` (the effect-set stand-ins in `_CONST_EL_CH1C_`).
- The eight `EL_STATE_ID` names CH1d2 added under `_CORE` (June script ids 299 / 304 / 323 / 330 / 321 / 325 / 285 / 328) against `june2017_re/char_elesis/ch0_state_skill.tsv`.
- `june2017_re/char_elesis/ch1_offsets.tsv` rows for the dash combos against the listings.

Hygiene: `python tools/flagcheck.py SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --with SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE --range <CH1c commit>^..<CH1d2 commit>` (FAIL only for the commented-out `#define`); `python tools/junediverge.py --check-all`; one-file `/Zs` probe of `X2GUEL.cpp` off and with `/D SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`. Fix clear transcription errors in place (bytepatch only). Write "CH1d2 - Review" into the plan doc, update the STATUS rows (CH1d2 review done, CH1e ready), check `kickoffs/char-elesis/CH1e.md`, commit with explicit pathspecs.

**Nothing to play-test.** Next: `/x2-phase char-elesis CH1e` (Sonnet 5.5 high).
