# June character Lu/Ciel (Stage 8) - Phase CH1i review (covers CH1g, CH1g2, CH1h, CH1i)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1i review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH1i done (its Done note and commits); the CH1g, CH1g2, CH1h Done notes |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (compiled, unreferenced) |
| Log tag | none (inert until CH1 final) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH1g - Done (2026-11-14)" "CH1g2 - Done (2026-11-14)" "CH1h - Done (2026-11-14)" "CH1i - Done (2026-11-14)"
```
Then `grep -E "\(CH1(g|g2|h|i)\)" june2017_re/funcmap.tsv` and JUNE_BUGS 700-705.

## Task

Review the four phases' commits against June (`june2017_re/charport_adis.py`: `luciel --phase CH1i --out DIR`; note it does not stop at a `ret` without padding, cut at the next class-map address):
- **Spot-check in the listing, then in Ghidra (read-only):** one function per kind from each phase, chosen by you. For CH1i: the three not-selected-body functions
  ( `32b08940`, `32b09bb0`, `32b09fa0`: the NOT_SELECT macro, the order of the duo / null / flag tests, the Demonio class test and the `if( false )` opening ), the two inverted-state ones ( `32b0a380`, `32b0a3e0`, JUNE_BUGS 704 ),
  one gated template ( `32b08d30`: `0 < USEFUL_VALUE0 Now`, key, next id +20 ) and one of the six end states ( `32b0aec0`, the `oneSit -> vslot 0x568` branch ). If one fails, re-check the whole phase.
- **CH1i's StartFutures:** `32b341b0` / `32b343b0` / `32b35fd0`: argument order of `GetFixedDistancePositionForWeaponTaker`, the `!GetIsRight()`, the half width + add ( 0.5 x width + USEFUL_VALUE0 Future / 30 / 20 ), `GetPos( true )` versus `GetPos( false )` per call, the line type 0 + enable test, the position and `vNotSelectPosition` stores, the line-group test.
- **CH1i's hyper mode pair:** `32b37d80` ( two oneshot timers, awakening stand-ins, sound only with the flag ) and `32b1f2b0` ( bone name, the three sequences, TOKANG as a major particle ).
- **Constants:** every float / double of `_CONST_LUCIEL_CH1G_` .. `_CH1I_` against the listing ( `charport_adis.py` prints `f=` / `d=` ).
- **Retire any debug define** the four phases left on ( none expected ).

Hygiene is one command: `python tools/finish.py --review --commits 69234352 2e5864b0 4946d4f1 <the CH1i code commit>` ( add `-v`; explain every OTHER row and deleted line ).
Fix transcription errors under the same flag ( one-file `/Zs` probes, no full build: no play-test ). Write "CH1i - Review" into the plan doc,
update the STATUS rows ( CH1k's CH1i-review dependency ), commit.
