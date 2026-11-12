# June character Elesis (Stage 7) - Phase CH1o review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH1o review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH1o done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (`//#define`), `SERV_IRUHADEV_JUNE_ENUM_NAMES` (defined) |
| Log tag | none (nothing runs until `_LIVE`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "CH1o - Done (2026-11-13)"
python tools/docsection.py JUNE_CHARACTER_ARA_PHASES.md "CH1o - Review"
awk -F'\t' '$7=="CH1o"' june2017_re/char_elesis/ch0_class_map.tsv
grep "Char Elesis CH1o" june2017_re/funcmap.tsv
```

## Task

Review CH1o against the exe bytes (`charport_adis.py elesis`, `disasm_range.py`, `vtslot.py`, `dispsweep.py`). Spot-check at least:
- **0x83c `32b732e0` = `ChangeDamageData`:** the jump table `32b734ac` / byte table `32b734b4` (cases 164 / 169 / 173-175 / 177 / 178 / 181 = the 8
  `IsFirstStrikeRelateState` names), `31fceab0( 0x6ef1 )` `jle` = level > 0, the 0.5 for 174 (`ESI_JUMP_ATTACK_X`), `+0x7c0` / `+0x798` / `+0x3d8` =
  `fStopTimeDef` / `fHitAddMP` / `damage.fPhysic`, the two "> 0 else 1.0" tests, `g_pMain +0x8c == 9` and `( r - 1 ) x 0.5 + 1`. The call site in
  `SetDamageData` `31fcd280` (every path) against `X2GUUser.cpp`'s `SetDamageData`.
- **0x534:** `3208e8c0`'s 15 getters (0x46c-0x478, 0x114, 0x118, 0x484-0x4a4; 0x47c / 0x480 skipped) and their returns against the `USI_` names (March =
  June - 1); `vtslot` that only Lu/Ciel overrides them; `32b90430`'s 0xce / 0xcd = June 206 / 205 = `ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_MEMO` / `_ACTIVE`.
- **`CommonStateStart @32057c23-32057ddd`:** the branch structure (`jne 32057d88` paths), `push 0` into vslot 0x534, `+0x8eac` / `+0x8ea8` tests (exact
  `fucompp`), `31fca8e0( id, 1 )` `+0x98` 6 / 7 = `m_eActiveSkillUseCondtion`, vslot 0x808 = `ClearSkillUseReserve`; the C++ in `CountReservedSkillUse`.
- **`SetSkillLevelStateData`** = `3201f850` (Ara's review read it); no E8 call from the Elesis range.
- **No-code verdicts:** `32b526d0` = March's list; `32b903c0` / `32b903d0` thunks; 0x558 has no caller (repeat the dispatch / E8 scan); 0x670 dispatches all
  in class ranges.

Hygiene: `python tools/flagcheck.py SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --range <CH1o commit>^..<CH1o commit> --files X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h
X2Lib/X2GUUser.cpp X2Lib/X2GUUser.h` (FAIL only for the commented-out `#define`); `flagcheck SERV_IRUHADEV_JUNE_ENUM_NAMES --range ... --files
X2Lib/X2SkillTree.h`; `python tools/junediverge.py --check-all`; `python tools/enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUUser.cpp`; one-file `/Zs`
probes of `X2GUEL.cpp` off, `_LIVE`, `_LIVE` + `_WOS` + `_BLOOD` + `_PASSIVE` + `_SUMMON`, and `X2GUUser.cpp` off / `_LIVE`. CH1o left no `_DEBUG` define.
Fix clear transcription errors in place (bytepatch only). Write "CH1o - Review" into the plan doc, update the STATUS rows (CH1o done, CH1p ready), check
`kickoffs/char-elesis/CH1p.md`, commit with explicit pathspecs.

**Nothing to play-test.** Next: `/x2-phase char-elesis CH1p` (Opus 5.5 high).
