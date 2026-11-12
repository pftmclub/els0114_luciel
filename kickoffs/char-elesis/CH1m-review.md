# June character Elesis (Stage 7) - Phase CH1m review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH1m review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH1m done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (`//#define`), `_CORE` (defined), `_WOS` / `_BLOOD` / `_PASSIVE` / `_SUMMON` (`//#define`) |
| Log tag | none (nothing runs until `_LIVE`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "CH1m - Done"
python tools/docsection.py JUNE_CHARACTER_ARA_PHASES.md "CH1m - Review"
awk -F'\t' '$7=="CH1m"' june2017_re/char_elesis/ch0_class_map.tsv
```

## Task

Review CH1m's registration against fresh Ghidra decompiles and listings of `32b87a60`, `32b81be0`, `32b864f0` (`python june2017_re/charport_adis.py elesis <va>`)
and the exe bytes. Spot-check at least:
- **Generator:** rerun `python june2017_re/char_elesis/ch1m_gen.py <dir>` and diff its `.inc` output against the `_LIVE` branches of `InitStateCommon` /
  `InitStateByUnitClass`; `ch1m_idmap.tsv` 153 ids, one member each both ways; the hand rows 227 / 228 (`ESI_DASH_JUMP_COMBO_SPK_XZ` / `_REATTACK`, one
  table, June StartFuture `32b50500` / `32b50520`); 207 (`ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_DETONATION`, the 205 table).
- **Wrapper:** the cut-in byte table `32b87fec` (8 / 29 / 30 / 35 -> 0, 120 -> 1, 121 -> 2, 125 -> 3, every other class -> 0; jump table `32b87fd8`, five
  targets, 0 and 4 both `mov [esi+0x8ac8], ebx`); `CMP [esi+0x4088], 0xbb` @32b87af5 (the sword wait kept by `InitStateID`); START / WIN / LOSE thunks
  (`329cb6e0` / `329cb6f0` / `329cb1b0`, `329cb700` / `320718d0`, `329cb710`); `PUSH 0x82` @32b87ed1; tail 0x690 / 0x684( 0 ) / `31fd8f00`. The order change
  (June: part 1, `3202b110`, part 2, `31fe22d0`; C++: part 1, part 2, `InitStateCommonRandom`) is harmless only if no id of part 2 lies in 111-116.
- **Part 1:** id 23's callbacks (vslot thunks `32adf320` -> Elesis `+0x600` = `32b7f090`, `329cc0e0` -> base `+0x604`); the bounding place (after dance
  127-129, before `ESI_RIDING_START`); the left-out ids (123-129, 93-96, the two empty targets); no `32d3ee60` call in either part.
- **Part 2:** one switch, no fall-through: 29 / 120, 30 / 121, 35 / 125, default nothing; the three tail registrations (321 / 326 / 329) jump to the shared
  `31fd89b0` call @32b87203 with their own table and thunks (321 takes `32b8fd40`, the `SSK_XX` EventProcess: JUNE_BUGS 639).
- **`ParseCommonRandomState`:** `GetUnitClassName( 8 / 29 / 30 / 35 / 120 / 121 / 125 )` = the seven `INIT_COMMON_RANDOM_STATE` tables of June's
  `ELESIS_KNIGHT.src.lua` (l.317-495).
- **JUNE_BUGS 638-639:** the exe pushes and script lines they cite.

Hygiene: `python tools/flagcheck.py SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --range <CH1m commit>^..<CH1m commit> --files X2Lib/X2GUEL.cpp` (FAIL only for the
commented-out `#define`; `eol! 424` is the block under March's LF-only `{` of `InitStateByUnitClass`); `python tools/junediverge.py --check-all`; `python
tools/enumscan.py --check X2Lib/X2GUEL.cpp`; one-file `/Zs` probes of `X2GUEL.cpp` off, `_LIVE`, `_LIVE` + `_WOS` + `_BLOOD` + `_PASSIVE` + `_SUMMON`. CH1m
left no `_DEBUG` define. Fix clear transcription errors in place (bytepatch only). Write "CH1m - Review" into the plan doc, update the STATUS rows (CH1m
done, CH1n ready), check `kickoffs/char-elesis/CH1n.md`, commit with explicit pathspecs.

**Nothing to play-test.** Next: `/x2-phase char-elesis CH1n` (Opus 5.5 high).
