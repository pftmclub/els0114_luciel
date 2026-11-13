# June character Elesis (Stage 7) - Phase CH1q review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH1q review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH1q done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS` (`//#define`, only with `_LIVE`), `..._WOS_DEBUG` (defined), `_CORE` (snapshot pair) |
| Log tag | none (nothing runs until `_LIVE` + `_WOS`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "CH1q - Done (2026-11-13)"
python tools/docsection.py JUNE_CHARACTER_ELSWORD_PHASES.md "CH1q - Review (2026-11-04)" "CH1q2 - Review (2026-11-04)"
awk -F'\t' '$NF=="CH1q"' june2017_re/char_elesis/ch0_wos.tsv
```

## Task

Review CH1q against the exe bytes (`charport_adis.py elesis <va>` for the class, `disasm_range.py` for the gauge data / UI) and the `.i` of the changed
files, `_WOS` off / on. Spot-check at least:
- **Start / End:** `32b74760` / `32b7f0b0` vslot 0x670( 3 ) then the BFI choice and the vslot 0x21c pushes ( id, creator 0, level 1, adjust 0, skip 0 ) =
  `( id, 1, NULL, 0, false )` in March's order; `32b74930` / `32b74bf0` erase order (BTI 435 / 434 first, `3229e160` = `EraseBuffTempletFromGameUnit`) and the
  two `_DARK_FORCE_*_End` sets (present in our `EffectSet.lua`?).
- **`GetFinalDamage_WayOfSword` `32b74ed0`:** the three `+0x971c` reads, type 5 / 6 path, game type == 1 only.
- **Getters and stand-ins:** `vtslot.py 0x720 / 0x724 / 0x72c CX2GU` (only `CX2GUEL` overrides); `31f86da5` / `32012f2e` multiply before the specialization.
- **Gauge data:** `32a34a70` (no 300 cap; the `fcomp st(1)` / `fcom` reads), `32a34c60`, `32a31ee0` (no clamp, JUNE_BUGS 641 holds).
- **Gauge UI:** `32e6e060` order, `32e69e80` (`g_pX2Game` test in both arms; vslot 0x10 = `GetMaxMp`), `32e69e70`.
- **The "= March" list:** re-read `WayOfSwordStateChange` `32b7f370` arm by arm (CH1q took CH0's read) and at least two other rows.

Hygiene: `python tools/flagcheck.py SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS --range <CH1q commit>^..<CH1q commit> --files <the nine code files> --with
SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS_DEBUG SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE` (FAIL only for the commented-out `#define`); `_CORE`, `_WOS_DEBUG`,
`JUNE_ENUM_NAMES`; `Always.h` a pure insertion; `python tools/junediverge.py --check-all`; `python tools/enumscan.py --check X2Lib/X2GUEL.cpp
X2Lib/X2GageData.cpp X2Lib/X2GageUI.cpp`; one-file `/Zs` probes of the five `.cpp` off, `_LIVE` + `_WOS`, `_LIVE` + `_WOS` + `_BLOOD` + `_PASSIVE` + `_SUMMON`.
**Keep `_WOS_DEBUG` defined**: its lines are for CH1's play-test, CH1's review retires it. Fix clear transcription errors in place (bytepatch only). Write
"CH1q - Review" into the plan doc, update the STATUS rows (CH1q done; CH1 waits only on its own deps), check `kickoffs/char-elesis/CH1.md`, commit with
explicit pathspecs.

**Nothing to play-test.** Next: `/x2-phase char-elesis CH1` (Opus 5.5 high).
