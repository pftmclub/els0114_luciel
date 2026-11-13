# June character Elesis (Stage 7) - Phase CH3b0 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH3b0 review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH3b0 done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON` (+ `..._SUMMON_DEBUG`, which stays: CH3b / CH3c read it, CH3d retires it) |
| Log tag | `[JUNE_CHAR]` (`ELESIS SUMMON`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md --no-sub "Rules" "Phase CH3 - Skills: second jobs (120, 121, 125), summons, hyper actives, transcendence"
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "CH3b0 - Done (2026-11-14)"
cat june2017_re/char_elesis/ch0_summon.tsv
grep "CH3b0" june2017_re/funcmap.tsv
```

## Task

Review CH3b0's commit against June (Ghidra read-only; `charport_adis.py elesis 32b8b0c0` for the listing-only state):
- **Vslot 0x24c `32b89260`**: the BTI switch values (238 / 239 / 298 / 326 / 565 / 940), the Victorious Sword order (level `31fce5c0` first, then `31fca8e0( id, 1 )`),
  ability 0x6a = `SA_DECREASE_REL` and 5 = `SA_EFFECTIVE_RATE` with bPvp 0, the Soul Ignition write `@32b8930d` skipped on a null templet, both saved-debuff tails
  (`+0x64 == 1`, `+0x66 == 0`, `32369cc0` false) and `32b6fb10`'s comparison (`FCOMPP` / `TEST AH,5` / `JNP`).
- **Vslot 0x250 `32b74500`**: the reset set, the Fire Blossom call through `31f8b010` with `IsMyUnit` first, `GetLandPosition`, delay 0.
- **`32b79db0`**: the loop bound re-read each pass, `GetRandomFloat( i )` x 450.0 + 50.0, the `GetFixedDistancePositionForWeaponTaker` arguments, the `PlayEffectSet`
  argument list (custom position true at the blossom's spot), the reset of `+0x9788` before `CommonStateEnd` (vslot 0x7dc).
- **`32b7a0a0` / `32b8b0c0` spawns**: id choice on `+0x97a4`, the level argument (now-state `+0x10`), Blood Cutter's 0.3 delay and `GetPos( 0 )`.
- **`3230c1a0`**: re-read the jump tables for 0x61f / 0x669 (default `3230ca5c`) and 0x663 / 0x7b3 (`3230cb83`), and that the default's skill is `31fcf520`'s map.
- The `#else` paths are March's bytes unchanged; member types match June's widths (u8 `+0x9788`, u16 `+0x9724`).

Hygiene: `python tools/flagcheck.py SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON --range <CH3b0 commit>^..<CH3b0 commit> --with SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON_DEBUG`
plus `git log -S SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON <commit>..HEAD`; paste the `files:` / `in this flag's files:` / `RESULT:` lines, explain every OTHER row.
`python tools/junediverge.py --check-all` = `RESULT: PASS`; `python tools/enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2Game.cpp`.
Fix clear transcription errors under the same flag (probe compile; full build only if the review hands off a play-test, which it does not: the spawns are reached in
CH3b / CH3b2 / CH3c / CH4a). Write "CH3b0 - Review" into the plan doc, update the STATUS row (CH3b ready), check `CH3b.md`'s "Amended by CH3b0" section, commit.
