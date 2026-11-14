# June character Lu/Ciel (Stage 8) - Phase DU1d3 - The rest of the second-body readers and the second after-image

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU1d3` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU1d2 |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | none of its own: `kickoffs/char-luciel/DU1-review.md` covers DU1-DU1d4 |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU1b - Done" "DU1d - Done" "DU1d2 - Done"
python tools/docsection.py kickoffs/char-luciel/DU1d.md "Amended by DU1b (2026-11-14)"
```
Then `june2017_re/char_luciel/du1d_body_reads.tsv`: every row with `owner` = `DU1d3` (24 June functions), and `grep "(DU1d2)" june2017_re/funcmap.tsv`.

## Task

Part 10 of DU1 (split by DU1d2, 2026-11-14), inside March's `CX2GUUser` functions under the flag, March's line in `#else`:

- **First, re-check each row's function (DU1d2's census defect).** The census files a read under the previous function when June has no padding
  between two functions. Before transcribing a row, find the real start of each site (`python june2017_re/disasm_range.py <a-0x60> <a>`: a
  `ret` followed by a prologue with no `int3`). Known: `3201fb8d` is in uncarved `3201f8d0` (an equip pass on `+0x15d4` / `+0x172` /
  `+0x1b0 == 7`, called from Init `3204ea20 @3204eb06`: DU3's equip byte, route it), `3204e4f3` is in `3204e4a0` (`+0x8fa4` / `+0x8fa8`, called
  from CommonFrameMove `@320661f9`). Correct the census rows you find wrong (function and owner).
- **The rest of bullet 1:** the marker / hold particles `31fee682` (`HoldCenter`, `HeadMarker*`, `TRANSCENDE*`), `ProcessEnchantedWeaponEffectAtHand`
  `31ff8510`, `InitSocketData` slot 484 `3206d770` (second anim + unit form), `UpdateEquipCollisionData` `31fd8c00` (the `+0x8e91` test; the
  `+0x15d4` part is DU3's), `PlayEmotion` `32043380`, `EmotionFrameMove` `3204a860`, `320200b0` (Bip01 lookups on the second anim),
  `GetWeaponEnchantData` `31fcb1b0`, `ApplyRotateBoneMap` `31fdb4e0` (none in March: say whose), `InitEqip` `32024510`, Init `3204ea20`
  (`+0xe40` read), the uncharacterized rows (`31fceeb0`, `31fdf4c0`, `31fe0bb0`, `31fe2fb0`, `31fe4210`, `31ff1ff0`, `31ff3ce0`, `3204e110`,
  `322726f0`). For each: a March function -> June's read at exactly June's place; "none in March" -> say whose phase it is.
- **The second after-image (DU1b amendment):** unit `+0x6758` (created at `31feed3d` in `31fee682`, on `+0x8e80`), `EnableAfterImage( mode )` /
  `DisableAfterImage( bForce, mode )` (`32275a30` / `322720f0`: 0 first, 1 second, 3 = unit vslot 152 `3208e160` = the active body), the
  selector `3208e530` (vslot 0x6b0: `+0x6754` on `+0x87d4` for 0, `+0x6758` on `+0x8e88` for 1, tail `3322aa80`), `MINOR_UNIT_AFTER_IMAGE`
  (`@3205b69b`, table `+0x852c`); widen `SetActiveBody`'s after-image toggle (DU1b stand-in).

Split again by the kickoff's own rule if it does not fit one session (DU1d2 managed ~13 functions). No build: probe builds only. DU1d4 then
does the out-of-line getter callers, the full build and the regression play-test.

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
