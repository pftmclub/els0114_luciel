# June character Lu/Ciel (Stage 8) - Phase DU6 - The awakening effect follows the active body

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU6` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | none (in-game `CX2GUUser` / `CX2GULuciel` only; can run beside DU5c / DU5d) |
| Flag | new `SERV_IRUHADEV_JUNE_CHAR_DUO_AWAKEN_EFFECT` (or the existing flag of the code June's fix lands in) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | `kickoffs/char-luciel/DU6-review.md` (Opus 5.5 high; the phase writes it when done) |
| Hand-off | now |

## Found by

User report during the DU5b2 play-test (2026-11-16, unit 202 LuDbA, battle field): **awakening while Ciel is the active body leaves the
awakening fire behind**: it stays at a fixed spot (where Lu was standing) and does not follow Ciel. Screenshot in the DU5b2 session.

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "CH1k2 - Done (2026-11-14)" "CH1o2 - Done (2026-11-15)"
```
Then `grep "(DU1d2)\|(DU1d3)\|(CH1o2)" june2017_re/funcmap.tsv` (the `MAJOR:` / `MINOR:` bone lookups `31fd9200` / `31fda280`, the effect-set
tags 4 / 10 / 11, the hand sparks `31ff8510` case 10).

## Task

1. Identify the effect: which awakening particles / effect set the user unit starts (March `CX2GameUnit` hyper-mode buff effect, the
   `CX2GUUser::HyperModeBuffEffectStart` override chain, `CX2GULuciel::HyperModeBuffEffectStart` `32b1f0b0`, the buff templet's effect set), and
   which skin or bone it traces (March: `m_pXSkinAnim` = Lu's body).
2. Read June's version in Ghidra: where June picks the active body for it (`IsMinorUnitActive` vslot 0x260, `m_pMinorUnitXSkinAnim`, the
   `MINOR:` bone prefix, effect-set tag 10 / 11), and whether June re-binds the effect on a body switch while awake.
3. Port June's body whole under the flag (March's in `#else`); a June function March lacks gets a class-pattern name. Divergent rows as usual.

Done gate: awaken (Ciel active) -> the fire stays on Ciel while she moves and dashes; switch to Lu while awake -> it is on Lu; other classes'
awakening unchanged (Elsword, Aisha).

No hardcoded enum literals, no `June` prefix, one-line comments, `junediverge.py --list` first (CLAUDE.md).
