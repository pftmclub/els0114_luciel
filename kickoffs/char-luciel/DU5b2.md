# June character Lu/Ciel (Stage 8) - Phase DU5b2 - Each body's equipment in the unit viewer

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5b2` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5b done (no review in between: `DU5b2-review.md` covers both) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (DU5b's; a divergence flag per replaced function if it reaches every class) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | `kickoffs/char-luciel/DU5b2-review.md` (Opus 5.5 high; covers DU5b + DU5b2) |
| Hand-off | now: the seam of the DU5b batch (`python tools/playtest.py assemble char-luciel DU5b DU5b2`) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "DU5b - Done (2026-11-16)" "DU3c - Done (2026-11-15)"
```
Then `grep "(DU5b)" june2017_re/funcmap.tsv`, the four `stage8` rows `CX2UnitViewerUI::UpdateEqip / AddEqip / AddEquipByTID /
ReplaceEmptyToBasic` in `tools/data/june_march_divergent.tsv`, `kickoffs/char-luciel/DU5.md` "Amended by june-engine R14e" (both bullets), and
`X2GUUser.cpp` DU3's `UpdateEquipViewListInfo` / `ReplaceEmptyToBasic` (the same per-body rules on the in-game unit). The DU5b census
(`viewer_census.tsv`) was a scratch file: re-run a census only if you need it.

## Task

Port June's viewer equipment functions whole into March's own functions under the flag (March's bodies in `#else`):
- `UpdateEqip` `31fa9e60` (no economical path; category 9 re-added with June's body filter: a fashion / accessory item only for the shown body
  outside viewer states 4 / 5 / 6 / 10; `31ed6500` class and `328823d0` appearance tests; `ApplyCustomize( 2 )` is Stage 12: stand-in);
- `AddEqip` `31f9df40` (Ciel's copy of Lu's normal item: templet minor id `+0x30` -> `AddEquipByTID( id, false, true, enchant )`, or Ciel's
  common model `32867880( 10, 1 )`; the skin by `IsMinorUnitEquipItem`; the try-on byte `+0x118c` (find its March owner or stand it in);
  the field-state filter; state 7-9 `bAttachedInDGManagerChain` false; `m_bFixed` -> `SetFixed` by equip type; `AdjustEquipPositionForSecondRing`);
- `AddEquipByTID` `31f9c640` with June's `( itemTID, bBasic, bMinorUnit, enchant )`; `ReplaceEmptyToBasic` `31f9e930` -> `31f9cfd0` per position
  (Ciel's basic from the templet minor id when Ciel is shown); `RemoveEqip` / `RemoveAllEquips` by body (find June's);
- widen `m_ViewEquipNormal` / `m_ViewEquipFashion` to `[ CX2Unit::EP_END ][ 3 ]` (June `+0x208` / `+0x5b0`) at every live site (frame move,
  render prepare, avatar capture, weapon trace, `IsFashionItemEquippedAt` / `IsItemEquippedAt`: find June's body index for each) and drop
  R14e's two stand-ins in `UpdateEquipSlotVisibility` (the per-body fashion table and the apply loop over bodies 0-2);
- R14e's other bullet: June's viewer basic-equip fill vs March's `EP_RAVEN_LEFT_ARM` fill.
Each replaced function that is not Lu/Ciel-only reaches every class: its `june_march_divergent.tsv` row turns from `stage8` to `replace`
with the flag in `source`. Then build, and hand off the DU5b batch.

Done gate: in the character window Lu and Ciel each wear their own equipment and fashion; the switch button and a click swap the shown body;
two other classes' windows and select screens are unchanged (relog test).

No hardcoded enum literals (user, 2026-10-26; CLAUDE.md): an id that has a name in an Enum (June's
`Enum.lua`, or a C++ enum) is never written as a number, in any new or touched code: use the member, and
register it under a `SERV_IRUHADEV_` flag with June's value and name if C++ lacks it. A value with no Enum
entry stays a number with a one-line comment. The one exception is a table whose rows are the data itself
(a `.tsv` of ids), not code.

No `June` prefix (user, 2026-10-28; CLAUDE.md): June's body goes inside March's own function under the flag,
March's in `#else`. A function March lacks gets a name in the class's own pattern, never with `June` in it.
Every name (locals and constants too) says what it is in context: no Ghidra address, phase tag or `June` in an identifier.
Call a March function from June code only if June's namesake does the same; otherwise replace March's definition.
Run `python tools/junediverge.py --list` before transcribing; a newly found divergent function gets its `june_march_divergent.tsv` row and the replacement in the same commit.
