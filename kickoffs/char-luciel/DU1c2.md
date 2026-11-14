# June character Lu/Ciel (Stage 8) - Phase DU1c2 - The second-body readers outside CX2GUUser

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU1c2` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU1c |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | none of its own: `kickoffs/char-luciel/DU1-review.md` covers DU1-DU1d |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU1c - Done"
```
Then the rows of the functions below in `june2017_re/char_luciel/nu0_duo_engine.tsv` and `nu0_type_switches.tsv` (rows `X2GameUnit.cpp:11909`,
`:19465`, `:19774`), `ch0_checklist_rows.tsv` row `§7as`, and `grep "(DU1c)" june2017_re/funcmap.tsv`.

## Task

Part 7 of DU1 (split by DU1c, 2026-11-14). Port whole under the flag, or give each a verdict:

- **Weapon effects by unit type 10:** `CX2GameUnit::Weapon::CommonFrameMoveForUser` (`X2GameUnit.cpp:11909`, June `32297d50` case 10 ->
  `3228c540`: the enchant particle follows the minor unit's bones when the owner user's `+0x8e90 == 1`, the game-loop reader `@3228c602`) and
  `ProcessUpgradeWeaponParticle` (`X2GameUnit.cpp:19465`, June `32292610` @`32292f91`: `Effect_LWeapon` / `RWeapon` of the skin anim when the
  unit form `+0xe40 == 1`; NU0 marked it "base not proven CX2Unit": prove it). `InitUpgradeWeaponParticle` `32299730` and the hand sparks
  `31ff8510` (checklist `§7as`, "PLAN GAP") are read for the same case; port them here if they are type-10 cases of March functions.
- **Unit form readers:** `CX2GameUnit::GetStat` `328842f0` @`32885558`, `CX2Unit` socket init `32365d30` @`32365fe7` / @`3236616d`,
  `CX2Item` used-socket count `328836c0` @`328837a5`: prove the base is `CX2Unit` (`+0xe40` = `m_bMinorUnitForm`), then port the form branch
  into March's counterpart, or route it (DU3 / DU5) when it belongs to the per-body equipment.
- **No March body:** `3235a160` (HUD stat refresh into a June-only singleton, Skill S7d: not ported) and `31ed3340` (the second caller of
  `3234af90`, reads the first body's game stat): identify and record.

Every other class has form 0 and is not type 10: prove no change with `/Zs` probes. No play-test of its own: "full build skipped: no play-test".

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
