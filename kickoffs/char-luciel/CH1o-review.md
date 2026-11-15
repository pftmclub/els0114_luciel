# June character Lu/Ciel (Stage 8) - Phase CH1o review (covers CH1m, CH1n, CH1o)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1o review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1o done (its Done note and commits); CH1m `a2312de2`, CH1n `cfe12590` |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE`, `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` (`//#define`), `SERV_IRUHADEV_JUNE_USER_VIRTUAL_HOOKS` (defined, class-agnostic), `SERV_IRUHADEV_JUNE_ENUM_NAMES` |
| Log tag | none (no play-test) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Phase CH1 - Deploy unit: the class core" "CH1m - Done (2026-11-15)" "CH1n - Done (2026-11-15)" "CH1o - Done (2026-11-15)"
```

and `june2017_re/char_luciel/ch1o_vcall_sites.tsv`, `ch1n_skill_cases.tsv`, `ch1m_regcheck.py`.

## Task

Review the three phases' commits:
- **CH1m:** re-run `python june2017_re/char_luciel/ch1m_regcheck.py` (337 rows, 0 problems); spot-check 3 registrations in `disasm_32b3e8b0.txt`
  (a riding row, a Demonio bounding-target row, a hyper row); the 18 left-out and 4 deferred rows each have their one-line reason.
- **CH1n:** the four written cases (208, 30301 / 30302, 30303, 900) against the capstone listing of `32b46580`; the 20 slot names = Enum.lua values.
- **CH1o (engine, defined flag, affects every class):** every new or now-virtual base equals June's base body (`ch1o` bases in `X2GUUser.h` /
  `X2GameUnit.h`; June bodies `3208ddd0 .. 33069eb0`, `32ec5990`, `31fca010`, `31fceed0`, `31fe9da0`, `31fd48a0`) and equals March's old behaviour, so
  no live class changes; no March class declares any of the 23 names (grep); each routed site sits where June calls the slot
  (`ch1o_vcall_sites.tsv` rows marked `CH1o:`): InitComponent / ReInit / CommonFrameMoveFuture MP sync, CommonFrameMove `SetNowMpFromStoredValue`,
  StateChange `SetHandEquipShow( false )` before the local-unit test, PlayEmotion gate and `SetHandEquipShow( true )`, CommonStateStartFuture
  `CP_AMOUNT` + 0x648, CommonStateStart 0x848, `DamageData::SimpleInit` reset hook. Spot-check 2 of them in Ghidra. Check the `_LIVE` widenings
  (Ara 0x2fc, Raven 0x300, `IsHyperModeState`, the `ReservedSkillStart` chain) compile alone: a probe with `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`.
- JUNE_BUGS 721, 722, 723 rows match the listings.

Hygiene is one command: `python tools/finish.py --review --commits a2312de2 cfe12590 <CH1o commits>` (plus `--state X2Lib/X2GULuciel.cpp Luciel_Duo
--state X2Lib/X2GULuciel.cpp Luciel_Demonio`). Paste its block; explain every OTHER row and deleted line with `-v`.

No play-test, no full build (`/Zs` probes). Fix clear transcription errors under the same flag; hand design decisions to the user. Write
"CH1o - Review" into the plan doc, update STATUS, check `CH1o2.md`, commit.
