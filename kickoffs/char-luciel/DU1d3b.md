# June character Lu/Ciel (Stage 8) - Phase DU1d3b - Emotion, socket / frame-move and per-frame second-body readers

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU1d3b` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU1d3 |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | none of its own: `kickoffs/char-luciel/DU1-review.md` covers DU1-DU1d4 (DU1d3b included) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU1d2 - Done" "DU1d3 - Done"
```
Then `june2017_re/char_luciel/du1d_body_reads.tsv`: every row whose `owner` starts with `DU1d3b` (DU1d3 rewrote these owners with the
real function of each site), and `grep "(DU1d3)" june2017_re/funcmap.tsv`.

## Task

Part 10b of DU1 (split by DU1d3, 2026-11-14), inside March's `CX2GUUser` functions under the flag, March's line in `#else`. Re-check each
site's function start first (DU1d3's method: a `ret` / `int3` followed by an address that is a call target or a pointer; jump-table targets
are not starts).

- **Emotion:** `PlayEmotion` `32043380` (26 sites, the unit form `+0xe40` branches) with its two helpers `31fdb800` (@`31fdb864` / @`31fdb8d3`)
  and `31fdf5d0` (@`31fdf700`), `EmotionFrameMove` `3204a860`, and `31fe0bb0` (the emotion mode `+0x89d8`, called from `StateChange` `320348c0`
  @`3203499f` / @`32034c08` and @`31fe1e0d`): say first whether March has each.
- **Socket / frame move:** slot 484 `3206d770` (@`3206d8f7` / @`3206da0f`, second anim + unit form) and `OnFrameMove` `3206eaf0` (@`3206ecb7`,
  @`3206f1bb`, @`3206f263`); `320200b0` (Bip01 lookups on the second Now anim, called from `OnFrameMove` @`3207000f`).
- **Per-frame callees:** `31ff1ff0` (called from `CommonFrameMove` @`3206322f`; its own sites `31ff2043`-`31ff2704`), the rest of
  `CommonFrameMove_TimedEvent` `31ff27c0` (@`31ff2952`-@`31ff39b1`: STATE_SOUND_PLAY and the timed events after it; DU1d3 ported only the
  after-image pairs), `31ff3ce0` (@`31ff3df5`, called @`32067387`).
- **The rest:** `31fe2fb0` (@`31fe31b3`, called thrice from `31fe3e8d`-`31fe3ed3`), `31fe4210` (three `+0x8e88` reads, called from `InitEqip`
  @`32024551` on the view list: DU3's if the reads are the equip byte).
- **Lead (compare, port only if it is a body read):** June's `Init` state switch (`3204ea20` @`3204ed8f`): game type 1 / 5 -> state 9, 3 ->
  state 13, any other type no state change there (March's default `USI_START` / riding start may sit in the later riding block). DU1d3 ported
  only the `SetActiveBody` call. A difference that is not a body read goes to `june_march_divergent.tsv` with its replacement or its own phase.

For each: a March function -> June's read at exactly June's place; "none in March" -> say whose phase it is. Split again by the kickoff's own
rule if it does not fit one session. No build: probe builds only. DU1d4 then does the out-of-line getter callers, the full build and the
regression play-test.

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
