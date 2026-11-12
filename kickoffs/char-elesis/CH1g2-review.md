# June character Elesis (Stage 7) - Phase CH1g2 review (covers CH1e-CH1g2)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH1g2 review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH1e, CH1f, CH1f2, CH1g and CH1g2 done (their Done notes and commits) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (`//#define`), `_CORE` (defined) |
| Log tag | none (nothing runs until `_LIVE`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "CH1e - Done" "CH1f - Done" "CH1f2 - Done" "CH1g - Done" "CH1g2 - Done"
awk -F'\t' '$7=="CH1e" || $7=="CH1f" || $7=="CH1f2" || $7=="CH1g" || $7=="CH1g2"' june2017_re/char_elesis/ch0_class_map.tsv
```

## Task

Review the functions of CH1e-CH1g2 against June (`python june2017_re/charport_adis.py elesis <va>...`; the listing carries the `StateChange` arguments and the `fcomp` / `test ah` direction; `je` after `cmp byte, 1` jumps when the byte IS 1; the map's column 9 says which rows are `listing-only`). **Read every `listing-only` function of these phases against the raw listing**, then spot-check at least:
- **CH1g2 `32b66ac0` / `32b66df0`** (Z-Z-Z loops): the word at `+0x9758` ( `m_usDashComboZZZLoopCount`: `ZZ` start writes 0, `32b51a70` increments, the Z test is `jae` at 4 ), flag 0 targets `0x12e` / `0x12d`, animation end -> `0x12f`. **`32b677d0`** ( ZX loop ): `fcomp` 2.0 against the *present* frame's state time, `pureX` test, target `0x133`. **`32b523d0`** ( XZ charge ): the two-branch fire condition ( event time 0 with Z released, or 3.1 ), target `0x14b`.
- **CH1g2 `32b7d920`** ( XZ fire FrameMove ): the three `x ( 1 + k c )` factors ( 0.5 power, 0.333 life time, 0.133 scale ), the `CreateInstance` trailing arguments ( bReserve false, reserve time -1, force-down rate 1, life time rate, hit count -1, hit gap -1, mesh index 0, hit-add MP -1, two false ) and `GetPos()` as the spawn point; the 0.01 s tick ordering ( `CommonFrameMove` first ). **`32b7c510` / `32b7c940`:** the four effect-set names and times ( 0.01 / 0.25 and 0.08 / 0.31 ).
- The eight `EL_STATE_ID` names CH1g2 added ( June ids 300-303 / 305-307 / 331 ) and CH1e-CH1g's against `june2017_re/char_elesis/ch0_state_skill.tsv`; `ch1_offsets.tsv` rows `0x9758` and `0x96f4`.
- CH1e-CH1g: the file-local macros ( `IF_STATE_CHANGE_ON_EX_OR_ANIMATION_END_`, `ZX_CANCEL_AFTER_EX_ON_LINE_` in both blocks ), the `_CONST_EL_CH1E_` .. `_CH1G2_` namespaces, JUNE_BUGS 631-633, and the `inside` bodies under `_LIVE` ( each one's `#else` is March's ).

Hygiene: `python tools/flagcheck.py SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --with SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE --range ae70cf92^..HEAD` (FAIL only for the commented-out `#define`); `python tools/junediverge.py --check-all`; `python tools/enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`; one-file `/Zs` probe of `X2GUEL.cpp` off and with `/D SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`. Fix clear transcription errors in place (bytepatch only). Write "CH1g2 - Review" into the plan doc, update the STATUS rows (CH1g2 review done, CH1h ready), check `kickoffs/char-elesis/CH1h.md`, commit with explicit pathspecs.

**Nothing to play-test.** Next: `/x2-phase char-elesis CH1h` (Sonnet 5.5 high).
