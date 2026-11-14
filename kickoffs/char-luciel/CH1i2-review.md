# June character Lu/Ciel (Stage 8) - Phase CH1i2 review (June's GetFixedDistancePositionForWeaponTaker)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1i2 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH1i2 done (its Done note and commit); independent of the CH1i review |
| Flag | `SERV_IRUHADEV_JUNE_DIVERGE_FIXED_DISTANCE_WEAPON_TAKER` (`KTDXLIB/Always.h`, `#error` guard on `RAVEN_WEAPON_TAKER` + `SERV_IRUHADEV_JUNE_LINEMAP_LAND_MONSTER_ROAD`) |
| Log tag | none (no play-test before CH3c) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH1i2 - Done (2026-11-14)"
grep -n "^31c60890\|^31c7bb00" june2017_re/funcmap.tsv
grep -n "GetFixedDistancePositionForWeaponTaker" tools/data/june_march_divergent.tsv tools/data/june_diverge_allow.tsv
```
Then JUNE_BUGS 706 and the `#ifdef SERV_IRUHADEV_JUNE_DIVERGE_FIXED_DISTANCE_WEAPON_TAKER` block of `KTDXLIB/KTDGLineMap.cpp` ( `python tools/bytepatch.py find KTDXLIB/KTDGLineMap.cpp FIXED_DISTANCE_WEAPON_TAKER` ).

## Task

Review CH1i2's commit against June `31c60890` ( `decompile_function 0x31c60890`; raw listing with `june2017_re/disasm_range.py`; constant widths with `june2017_re/charport_consts.py --fn 31c60890` ):
- **The start line:** CH1i2 reads the 600-line block as June's `GetLandPosition` ( `31c7bb00` ) inlined with `bMonster` true and calls `GetLandPosition( *pvPos, true, 30.f, &iLineIndex )`. Confirm in the listing: only `LT_WALL` skipped ( no `cmp ..., 7` ), the y prefilter and span tests,
  the Manhattan < 30 hit keeping the highest closest point, no index fallback ( index starts -1 ), the 999999 nearest-end pass and `return false` with no line; and that this tree's `GetLandPosition( pos, bool, float, int* )` matches ( `KTDGLineMap.cpp`, W1c ).
- **The walk:** the first stretch from `( pos.x, line y, pos.z )` ( left: start's y to start, right: end's y to end, `@31c61d4e` and the right twin after `31c61f51` ), the loop compare polarity, `walk < dist + 15` ( `@31c61e3a`, NaN path ),
  the 15-in branch ( `lerp = 15 / length`, which end gets `1 - lerp` per side ) and the long branch, and the return values.
- **The flip:** `fDist < 0` strict, NaN does not flip ( `@31c608a3`-`31c608ba` ).
- **The `#else`** is March's body byte for byte ( `flagcheck -v`: no deleted line ).
- **The Add caller finding:** June `329edde0` `@329ee138` ( the bone-line position / within 500 test ) is routed to char-add CH1d ( note in `kickoffs/char-add/CH1d.md` ): confirm the note is right; no code change here.
- **Divergence table:** the row's `source` names the flag, no `GetFixedDistancePositionForWeaponTaker` row is left in `june_diverge_allow.tsv`.

Hygiene is one command: `python tools/finish.py --review --commits <the CH1i2 code commit> --skip-flag SERV_IRUHADEV_JUNE_LINEMAP_LAND_MONSTER_ROAD "guard target"` ( add `-v`; explain every OTHER row and deleted line ).
Fix transcription errors under the same flag ( one-file `/Zs` probes of `KTDGLineMap.cpp`, no full build: no play-test ). Write "CH1i2 - Review" into the plan doc, update the STATUS row ( CH3c's CH1i2 dependency ), commit.
No debug define to retire ( CH1i2 added none ).

You are on Opus: delegate the reads, greps and known-address dumps ( x2-phase, *When the phase is Opus* ).
