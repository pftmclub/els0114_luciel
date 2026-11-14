# June character Lu/Ciel (Stage 8) - Phase CH1f2 review (covers CH1b, CH1d, CH1f, CH1f2)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1f2 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH1f2 done (its Done note and commit); CH1b, CH1d, CH1f Done notes |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (compiled, unreferenced) |
| Log tag | none (inert until CH1 final) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH1b - Done (2026-11-14)" "CH1d - Done (2026-11-14)" "CH1f - Done (2026-11-14)" "CH1f2 - Done (2026-11-14)"
```
Then `grep -E "\(CH1(b|d|f|f2)\)" june2017_re/funcmap.tsv` and JUNE_BUGS 692-699.

## Task

Review the four phases' commits against June (`python tools/charport_adis.py` is `june2017_re/charport_adis.py`: `luciel --phase CH1f2 --out DIR`):
- **Spot-check in the listing, then in Ghidra (read-only):** one function per kind from each phase, chosen by you: a combo EventProcess with events
  ( the event order of Ciel's `DASH_JUMP_COMBO_Z`: event 1 before event 0 ), a state with no twin ( `LSI_CIEL_COMBO_XXXZ_LOOP_EventProcess`,
  3 s and the else-if cancels ), the Ciel `XXXZ_END_FrameMove` ( damage effect, position, 1 + charge / 2 life time rate ). If one fails, re-check the whole phase.
- **CH1f2's helper `PlayComboProjectileEffectSet`:** June inlines it ten times; check the argument order of `PlayEffectSet` ( life time = `m_fSoulCollectorDamageRate`,
  power = `GetPowerRate() * m_fPowerOfOriaxDamageRate` ), the two modifier tags ( 2 add = false, 4 add = true ), the MP test on the bone position, and that
  `32b25300` is the only caller without the tag-4 modifier. JUNE_BUGS 699.
- **Constants:** every float / double in the `_CONST_LUCIEL_CH1F2_` block against the listing ( `charport_adis.py` prints `f=` / `d=` ).
- **Provisional members:** `m_fSoulCollectorDamageRate` / `m_fPowerOfOriaxDamageRate` / `m_usSoulCollectorMaxCount` read like a life-time rate, a power rate and a
  count; CH1a named them from other users. Decide whether to rename now or leave to CH4a, and say which in the review.
- **Retire any debug define** the four phases left on ( none expected ).

Hygiene is one command: `python tools/finish.py --review --commits <CH1b, CH1d, CH1f, CH1f2 code commits>` ( add `-v`; explain every OTHER row and deleted line ).
Fix transcription errors under the same flag ( one-file `/Zs` probes, no full build: no play-test ). Write "CH1f2 - Review" into the plan doc,
update the STATUS rows ( CH1g's CH1f2-review dependency ), commit.
