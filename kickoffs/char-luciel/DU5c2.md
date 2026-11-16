# June character Lu/Ciel (Stage 8) - Phase DU5c2 - Both bodies on the create / select screens, the viewer's back body

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5c2` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5c (split from it 2026-11-16); its review may run in parallel (different files except `X2UnitViewerUI.h`) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (DU5b's) for the viewer; a new `SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY` for the two states if they need one; `CX2Eqip::SetFixed` under its own divergence flag |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | `kickoffs/char-luciel/DU5c2-review.md` (Opus 5.5 high; the phase writes it when done) |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "DU5b - Done (2026-11-16)" "DU5b2 - Done (2026-11-16)" "DU5c - Done (2026-11-16)"
```
Then `kickoffs/char-luciel/DU5c.md` (the task list and the three amendments: everything not marked done by DU5c's Done note is yours),
`nu0_create.tsv` rows `a5`, `d1`, `d3`, `d8`, `s3` (print with python, the file has non-ASCII), and `grep "(DU5b)\|(DU5b2)\|(DU5c)" june2017_re/funcmap.tsv`.

## Task (split from DU5c by DU5c, 2026-11-16)

- **Create screen:** SelectLuciel `325559f0 @32555fe8` (viewer `+0xd7c`: `31edf260( class, 1 )` rotation of the minor skin = `31f90fb0`, `PlayAnim( .., 2 )`),
  Ciel's basic / promotion meshes (`nu0_create.tsv` `d8`). NU4 shows Lu only.
- **Select screen:** `32564400` (CreateServerSelectUnitViewerUI, `@32564883`) and `3255e570`.
- **The viewer's back body** (DU5c.md "Amended by char-luciel DU5b"): its frame move ( `31faa550`'s second `31fa8ce0( t, dt, 0 )` ), its draw
  ( `31f974b0` / `31fa00b0`, `+0xd81` / `+0xd82` ), the `+0xd82` writers ( `PlayEmotion 31fa52b0`, `31fa5c90` ), `31f99f70`'s back-body move, the equip
  position / scale / colour copies ( `31f93710` / `31f93820` / `31f93920`, and DU5b2's `3295b260` stand-ins ), the frame move's enchant lookup by body
  ( `31fa8ce0 @31fa9040`, `32869e10( pos, 0, body )` ).
- **`CX2Eqip::SetFixed`** (`3295a560`, DU5c.md "Amended by char-luciel DU5b2"): June sets `m_bFixed` only; read June's three `+0x8ec` readers first
  (`3295bd54`, `3295caa7`, `3295d5aa`: outside carved Ghidra functions, use `disasm_range.py`), then replace March's under a divergence flag, drop the
  two `june_diverge_allow.tsv` rows and turn the `CX2Eqip::SetFixed` row to ported.
- **R14e's select-screen onepiece** (DU5c.md "Amended by june-engine R14e"): the select-screen unit gets June's equip size the way June's character-list
  handler does; play-test `jtLordKnight` with 163569.
- **Other players' form:** `32367330` / `32367da0` / `32368300` read a form byte from the user infos; offline there are no other players: route or
  close with a line in the Done note.

Done gate: the create screen and the select screen show Lu and Ciel where June does (side by side, Ciel rotated by her templet value); the select
screen shows an onepiece costume; other classes unchanged.

No hardcoded enum literals, no `June` prefix, `junediverge.py --list` before transcribing (CLAUDE.md).
