# June character Elesis (Stage 7) - Phase CH1p review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH1p review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH1p done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (`//#define`) |
| Log tag | none (nothing runs until `_LIVE`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "CH1p - Done (2026-11-13)"
python tools/docsection.py JUNE_CHARACTER_ARA_PHASES.md "CH1p - Review"
awk -F'\t' '$7=="CH1p"' june2017_re/char_elesis/ch0_checklist_rows.tsv
```

## Task

Review CH1p against the exe bytes (`funcdis.py`, `dispsweep.py 8ac0 8b58 88bc 8ea0 8ea4 8ea8` filtered to `32b4e770-32b90b6f`) and the `.i` of both files,
`_LIVE` off / on. Spot-check at least:
- **The 18 key calls:** each `lea ecx, [r + 0x8b58]` site's call target (`31fce6d0` Set, `31fce670` FollowSlotSwap, `31fc89a0` IsKeyReleased, `31fc8900`
  IsKeyPressedOnce) against the line CH1p put under `_LIVE` in the same March state (Dodge and Slash, Rushing Sword 1st / 2nd, Extinction start / charge /
  degree up / down, Drastic Charge start / loop); `Set`'s argument `[+0x88bc] - 1` (`dec`); `32b5c0c0` = degree up (goes to the down state) and `32b5c400`
  = degree down. The `_LIVE` `.i` keeps no `m_sCurrentSkillKeyManager.` use in her file.
- **The sweep's other users** are new or June-only reads owned by CH2a / CH2b / CH2b2 / CH2d / CH3a2 (kickoffs amended), `32b4f660` / `32b4f6d0` dead
  (CH0), and the `+0x88bc` / `+0x8ea0` / `+0x8ea4` / `+0x8ea8` hits are CH1k's (`32b4ee20`, `32b84a50`, `32b73e60`).
- **Shared reader:** `UT_ELESIS` in `bMarchSlotIndex` with `_LIVE`, absent off; every earlier class `_LIVE` is `#define`d, so the new outer level drops none.
- **No-code verdicts:** her decode and `CountUsedSkill` only in `#else`; no `GetPressedSkillState` caller; the four `EFFECT_SET_LIST_*` variants
  (`LoadEffectSetFromScript`, `JUNE_EFFECT_SET_LIST_VARIANTS`), `IGNORE_STATE_DEFENCE` (`UPGRADE_RAVEN`) / `IGNORE_DEFENCE_DECLINE`
  (`JUNE_BUFF_NEW_STAT`) and `RELATIVE_SKILL_LEARN_CHECK` readers live with no Elesis override; the reserved-buff ids as June's script keys them
  (CH1 kickoff amendment).

Hygiene: `python tools/flagcheck.py SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --range <CH1p commit>^..<CH1p commit> --files X2Lib/X2GUEL.cpp X2Lib/X2GUUser.cpp`
(FAIL only for the commented-out `#define`); `Always.h` a pure insertion (guard only); `python tools/junediverge.py --check-all`; `python tools/enumscan.py
--check X2Lib/X2GUEL.cpp X2Lib/X2GUUser.cpp`; one-file `/Zs` probes of `X2GUEL.cpp` and `X2GUUser.cpp` off, `_LIVE`, `_LIVE` + `_WOS` + `_BLOOD` + `_PASSIVE`
+ `_SUMMON`. CH1p left no `_DEBUG` define. Fix clear transcription errors in place (bytepatch only). Write "CH1p - Review" into the plan doc, update the
STATUS rows (CH1p done, CH1q ready), check `kickoffs/char-elesis/CH1q.md`, commit with explicit pathspecs.

**Nothing to play-test.** Next: `/x2-phase char-elesis CH1q` (Opus 5.5 high).
