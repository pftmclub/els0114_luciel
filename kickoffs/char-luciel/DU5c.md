# June character Lu/Ciel (Stage 8) - Phase DU5c - Both bodies on the create / select screens and the village avatar

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5c` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5b review |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (DU5b's), or a new `SERV_IRUHADEV_JUNE_CHAR_DUO_AVATAR` for the square unit if DU5b's review prefers |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | `kickoffs/char-luciel/DU5c-review.md` (Opus 5.5 high; the phase writes it when done) |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "Phase DU - The two-body engine (DU1-DU5)" "DU5 - Done (2026-11-15)" "DU5b - Done"
```
Then `kickoffs/char-luciel/DU5.md` "Amended by NU0" (both-bodies and Square-unit bullets), `nu0_create.tsv` rows `d1`, `d8`, `s3`,
`nu0_type_switches.tsv` row `X2SquareUnit.cpp:220`, and `grep "(DU5)" june2017_re/funcmap.tsv`.

## Task

- **Create screen:** SelectLuciel `325559f0 @32555fe8` (viewer `+0xd7c`: `31edf260( class, 1 )` rotation of the minor skin, `PlayAnim( .., 2 )`), and the
  basic / promotion body meshes of Ciel (`nu0_create.tsv` `d8`). NU4 shows Lu only.
- **Select screen:** `32564400` (CreateServerSelectUnitViewerUI, `@32564883`) and `3255e570`.
- **Village avatar:** the rest of `CX2SquareUnit::SetMinorUnitForm` `323f3290` (DU5 ported the head): the SD mesh by form `323f1490( form, type )`
  and the viewer `+0x2ec` rebuild `@323f331e-323f3479`; then the NOT's `323fa700` / `323fad80` stand-ins in
  `CX2TFieldGame::Handler_EGS_CHANGE_UNIT_FORM_NOT`; June's 24-name SD mesh table `31ee3470` (array `3372c1e8`, `X2SquareUnit.cpp:220`).
- **Other players' form:** the three CX2Unit-from-user-info writers `32367330` / `32367da0` / `32368300` read a form byte at struct `+0xe8` / `+0x2a` /
  `+0x50`; only if the village or room needs it offline (no other players: likely route to DU5e with the packet fields).

Done gate: the create screen and the select screen show Lu and Ciel where June does; in the village the avatar becomes Ciel / Lu when the window's
switch is pressed (the form NOT), and keeps it after a relog.

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
