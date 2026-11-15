# June character Lu/Ciel (Stage 8) - Phase DU5e - Each body's own numbers (second game stat, sockets, set count)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5e` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5 review (independent of DU5b-DU5d: no viewer work) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_STAT` (new; a `CommonPacket.h` field needs the `ServerDefine.h` second home, `SECOND_HOMES` and the `X2ServerProtocol_2010.vcxproj` X2TOOL define, as NU2) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | `kickoffs/char-luciel/DU5e-review.md` (Opus 5.5 high; the phase writes it when done) |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Starting context" "Phase DU - The two-body engine (DU1-DU5)" "DU1c - Done (2026-11-14)" "DU1c2 - Done (2026-11-14)" "DU3c - Done (2026-11-15)" "DU5 - Done (2026-11-15)"
```
Then `kickoffs/char-luciel/DU5.md` sections "Amended by char-luciel DU1c", "Amended by char-luciel DU1c2", "Amended by DU1d3b", "Amended by
char-luciel CH1a0" and "Amended by DU3c" (this phase's whole scope), and `grep "(DU5)\|(DU1c)\|(DU1c2)\|(DU3c)" june2017_re/funcmap.tsv`.

## Task

These were amended into DU5 and split out by DU5 (2026-11-15); the amendments name every address:
- **Second game stat:** `UnitData::SetGameStats( KStat first, KStat second )` `323571c0`, the seven packet sites, the packet fields (the
  `CommonPacket.h` rule), the emulator's per-body numbers (the minor unit's equipment, DU3 / DU4); `CX2Unit::UnitData::m_MinorUnitGameStat` is unfilled.
  Three of those sites skip vslots 0x63c / 0x630 while transformed (`31fc9d90`, CH1a0).
- **Type-10 socket aggregate** (0x488, `32091730`, vslots 0xc / 0x10 by body, `InitSocketData` modes, group filter `3286a4b0`) and the **HUD's second
  gage** (`CX2Unit::ResetHudMaxHpMp` `3235a160 @3235a29e-3235a339`, stand-in comment in `X2Unit.cpp`).
- **Set-socket count** `CX2Inventory 3286a6b0( set, body )` and its four readers (`328842f0 @32885558`, `32365d30 @32365fe7 / @3236616d`, `328836c0
  @328837a5`, `3206d770 @3206da0f`), plus the character window's other-form set sum (`3291ba40 @3291bf2d`, stand-in comment in `X2UICharInfo.cpp`);
  the item `+0xac` set-socket list itself is Stage 16 (funcmap S7m): port only the count and pass the form.
- **Per-body socket record** `3206d770 @3206dbd3-3206dc6a` -> `SocketData 3271a060` and its readers.
- **Other players' form** (from DU5): the user-info form bytes (`32367330` / `32367da0` / `32368300`) if DU5c routed them here.

Done gate: Lu and Ciel each show their own HP / attack / defence in the character window (switch with the button) and in a dungeon (tag), matching
each body's equipment; the HUD shows Ciel's MP gauge sized by her own sockets.

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

## Split by DU5e (2026-11-15)

DU5e ported the second game stat (the first task bullet: `SetGameStats`, the seven sites, the packet fields, the emulator's per-body numbers).
The rest moved to **DU5f** (`kickoffs/char-luciel/DU5f.md`): the type-10 socket aggregate, the HUD's second gage, the set-socket count and its
window reader `@3291bf2d`, the per-body socket record `3271a060`, and the other players' form bytes if DU5c routes them. The character window
already shows each body's HP / attack / defence (DU5's `GetShownEquipStat` by form), so DU5e's gate is the dungeon side.
