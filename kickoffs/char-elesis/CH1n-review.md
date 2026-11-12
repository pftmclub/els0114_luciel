# June character Elesis (Stage 7) - Phase CH1n review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH1n review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH1n done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (`//#define`), `_CORE` (defined), `SERV_IRUHADEV_JUNE_ENUM_NAMES` (defined) |
| Log tag | none (nothing runs until `_LIVE`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "CH1n - Done"
python tools/docsection.py JUNE_CHARACTER_ARA_PHASES.md "CH1n - Review"
awk -F'\t' '$7=="CH1n"' june2017_re/char_elesis/ch0_class_map.tsv
```

## Task

Review CH1n's head against a fresh Ghidra decompile of `32b8c470` and the exe bytes (`june2017_re/charport_consts.py` `read_va`). Spot-check at least:
- **Head:** bounds `(int)slot < 0 || 4 < slot`; PE tables `334f8238` / `334f824c` / `334f8260` / `334f8274` = `_CONST_GUUSER_` (normal A, hyper A, normal B,
  hyper B, as the port indexes them); `31fd2af0` dropped (7i); the one `+0x11 = 1` store = `IsSkillSlotState`; hyper word `+0x10 = 0x101`; `31fca8e0( id, 1 )`.
- **Memo arm `@32b8c5bb`:** `31fdbcc0` / `32206450( memo, 1 )`; the switch is on the memo, 31 entries from 28031, only 28031 / 28060 and 28032 / 28061 leave
  the exit; their bodies (`32b8c5c2`, `32b8c72f`) equal the 28010 / 28048 (`32b8cf8c`) and 28016 / 28052 (`32b8d2c6`) case bodies (same thunks, same ids
  257-259 / 278-281, shared tails `@32b8d0f4` / `@32b8d355`), which is what makes the `eStateSkillID` route exact; June's four names = `Enum.src.lua`
  (l.16175, l.15246, l.15045, l.14874).
- **Types:** 5 buff (`329ccd90` -> vslot 0x5f0 = `32b6def0`, `329cba40` -> `31fcbd60`); 6 / 7 / 13-16 active; 12 + id 900 throw (`329ccda0` / `329ccdb0`,
  vslots 0x5f4 / 0x5f8, not overridden by Elesis); 8-11 and 17 / 18 return; table strings `ESI_COMMON_BUFF` / `ESI_THROW_ITEM`. The throw arm's hyper copy
  (`@32b8ea71`) copies the four pointers as the C++ does.
- **Hyper split:** routed to CH3d; check the `CH3d.md` amendment against `@32b8c7e0`-`@32b8e9ce`.
- **No cool-time / MP / hyper-stone check** in `32b8c470` (checklist L266): the dev gate stays CH1k's (`X2GUEL.cpp`, `IsDevUnlimitedSkillUse`).
- **Callbacks:** `python june2017_re/char_elesis/ch1n_triage.py` rerun = the committed `ch1n_user_callback_triage.tsv` (178 / 178, missing 0).

Hygiene: `python tools/flagcheck.py SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --with SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE --range <CH1n commit>^..<CH1n commit> --files
X2Lib/X2GUEL.cpp` (FAIL only for the commented-out `#define`); `flagcheck SERV_IRUHADEV_JUNE_ENUM_NAMES --range ... --files X2Lib/X2SkillTree.h`; `python
tools/junediverge.py --check-all`; `python tools/enumscan.py --check X2Lib/X2GUEL.cpp`; one-file `/Zs` probes of `X2GUEL.cpp` off, `_LIVE`, `_LIVE` + `_WOS` +
`_BLOOD` + `_PASSIVE` + `_SUMMON`. CH1n left no `_DEBUG` define. Fix clear transcription errors in place (bytepatch only). Write "CH1n - Review" into the plan
doc, update the STATUS rows (CH1n done, CH1o ready), check `kickoffs/char-elesis/CH1o.md`, commit with explicit pathspecs.

**Nothing to play-test.** Next: `/x2-phase char-elesis CH1o` (Opus 5.5 high).
