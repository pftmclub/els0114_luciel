# June character Lu/Ciel (Stage 8) - Phase DU5d - Viewer effects and checklist rows for two bodies

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5d` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5b2 review (covers DU5b + DU5b2) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (DU5b's) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | `kickoffs/char-luciel/DU5d-review.md` (Opus 5.5 high; the phase writes it when done) |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "Phase DU - The two-body engine (DU1-DU5)" "DU5b - Done"
```
Then `kickoffs/char-luciel/DU5.md` "Amended by NU0" (viewer bullet) and "Amended by CH0" (all five checklist rows), `nu0_type_switches.tsv` rows
`X2UnitViewerUI.cpp:960`, `:6255`, `:8559`, `:8681`, and `ch0_checklist_rows.tsv` col 5 rows tagged DU5.

## Task

- **Weapon effects in the viewer, case 10:** `X2UnitViewerUI.cpp:960` (`31fa8ce0` case 10 -> `31f93050`, reads `+0xd80`), `:6255` (`31f961a0` case 10:
  `Dummy1_Rhand` vs `Bone_RFinger2` by `+0xd80`, rates 15 / 30 / 60 / 25 / 15), `:8559` (Lu/Ciel in the first upgrade-particle group), `:8681`
  (`31f9a900` own case 10 `@31f9b488`); `:7938` is dead.
- **Checklist (CH0):** §3 L153 CharCustomData (22 Lu/Ciel rows; the customize reader reads DUO_TYPE), §3 L153 UnitViewerEffectSetTable (duo rows),
  §7v L649-653 `IS_FOR_MINOR_UNIT` slot key (`X2SlotItem.cpp:53-65` `LuaGetValue`, June `32e284b0`), §7aj L870-872 unit form change `3234d930`
  (already `CX2Unit::UpdateSkillTreeCommandGroup`; port its portrait refresh `328dcba0` if reachable), §7as L1001-1012 viewer glow (`31f92910`, `31f9a900`).

Done gate: in the character window Lu's and Ciel's weapon glow / enchant / upgrade particles sit on the right hand of the shown body; every other
class's viewer effects are unchanged.

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

## Amended by char-luciel DU5b2 (2026-11-16): per-body weapon trace

- `InitWeaponTrace` (`31f9a2a0`) loops bodies 0-2 (`+0x5e0 + body x 8` = the fashion weapon slot of each body, then `+0x238 + body x 8` the normal
  one) and keeps trace frames per body; DU5b2 reads Lu's weapon only (`[ EP_WEAPON_HAND ][0]`, stand-in comment). Port the per-body frames with
  the viewer weapon effects (case 10).
