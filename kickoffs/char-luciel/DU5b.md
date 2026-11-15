# June character Lu/Ciel (Stage 8) - Phase DU5b - The unit viewer's second body (character window)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5b` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5 review |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (new; needs `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEW`) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |
| Review | `kickoffs/char-luciel/DU5b-review.md` (Opus 5.5 high; the phase writes it when done) |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "Phase DU - The two-body engine (DU1-DU5)" "DU5 - Done (2026-11-15)"
```
Then `kickoffs/char-luciel/DU5.md` "Amended by NU0" (viewer bullet) and "Amended by CH0" (`§7e`), `grep "(DU5)" june2017_re/funcmap.tsv`,
`june2017_re/charinfo_functions.tsv` rows `31f93a10`, `31faa150`, `31f94190`, `3291d360`, `329148d0`, and `nu0_create.tsv` row `d1`.

## Task

`CX2UnitViewerUI` has no second body; June's viewer (0x11f0 bytes) keeps one at `+0xd7c` (minor skin anim), `+0xd80` (shown body / switching),
`+0xd81` (form filter on), `+0xd82`. First map it (a Sonnet census of every June viewer read / write of `+0xd7c..+0xd82` in `31f8f000-31fb2000`,
with callers; the NU0 `+0xe40` rows in that range marked "other struct?" are viewer members: settle each), then port:
- the minor body's creation, equips and skin from NU3's templet members (the viewer's `SetUnit` / `UpdateEqip` paths: `31faa6f0`, `31fa9e60`,
  `31f933c0`, `31fac3c0`), June's `PlayAnim` `31fa3010` with `which` = 2 (both skins), render and frame move of the second skin;
- `31f93a10` (two-body view: scale, position, rotation), `31f94190` (form filter by equip `+0x15d4` against `+0xd80`, position list),
  `31faa150` (minor-unit item filter + reset), `3291d360` (viewer reset + `UVUMT_WAIT` + `ResetStat( 0 )`);
- the member `+0xd80` with June's writer `32912c50` (`CX2UICharInfo::SetSwitchingView`), and retire DU5's stand-ins that read the window's
  `m_bSwitchingView` for it (`ResetStat`, `OnDropAnyItem`), plus the viewer calls DU5 left as stand-ins: `SwitchViewOnlyThisDialog` (`31f94190`),
  `CX2TFieldGame::Handler_EGS_CHANGE_UNIT_FORM_NOT` (`31fac190`, `31f94190`, `31fa8230`, `31fa5c90( 0 )`), `ResetUnitViewerUI` (`+0xd81`).

Done gate: the character window shows Lu and Ciel side by side, the shown one in front; the switch button and a click swap them; each wears its
own equipment; every other class's window is unchanged (relog test on two other characters). Create / select screens are DU5c's.

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
