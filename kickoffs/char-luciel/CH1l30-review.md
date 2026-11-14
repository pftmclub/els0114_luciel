# June character Lu/Ciel (Stage 8) - Phase CH1l30 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1l30 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH1l30 done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ENEMY_SCAN` |
| Log tag | none (inert until Lu/Ciel's callers go live) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH1l30 - Done (2026-11-14)"
```
Then `grep "(CH1l30)" june2017_re/funcmap.tsv`.

## Task

Review CH1l30's commit against June:
- **`CX2Game::GetEnemyUnitListInRange` `322f1a90`** ( `X2Game.cpp` ): argument order ( hidden return, unit, team, range; `RET 0x10`, ECX = the game ),
  the check order ( valid, team == `iMyTeam_` skip, `GUSI_DIE`, HP `<= 0`, same type + same UID skip ), the aim point ( vslot 0x3e4 with `L""` from
  the unit's `GetPos( false )` = `CEffect::GetLockOnAimPos` ), the NPC size `* 0.25` and `NCT_BASIC` skip, `<=` range with NaN refused, the sort
  ( `322acc40` ascending ) and no count cap.
- **`CX2GameUnit::GetNearestEnemyUnitDistance` `32295360`** ( `X2GameUnit.cpp` ): start value 9999999.0 ( `[3344915c]` ), the bool return, the inlined
  HP gage test = March `GetNowHp() <= 0.f` ( gage NULL passes, HP gage NULL skips ), the strict `<` with NaN refused, the unused local vectors.
- **`322d7b30` at `32b318e0`:** June's 4th argument is a `const int&` ( slot 0x178 reads `[esi]` ); Elesis's by-value `iAttackType_` = -1 is the same.
- The `friend class CX2Game;` in `X2DamageEffect.h` and the `#error` guard in `Always.h` ( `JUNE_DAMAGEEFFECT_RUNTIME`, `JUNE_DAMAGEDATA` ).

Hygiene is one command: `python tools/finish.py --review --commits <CH1l30's commit>` ( add `-v`; explain every OTHER row and deleted line ).
Spot-check 2-3 items in Ghidra (read-only); if one fails, re-check the whole phase. Fix transcription errors under the same flag (one-file `/Zs`
probes, no full build: no play-test). Write "CH1l30 - Review" into the plan doc, update the STATUS row ( CH1l3's CH1l30 dependency ), commit.
