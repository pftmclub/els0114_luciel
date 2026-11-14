# June character Lu/Ciel (Stage 8) - Phase DU1d5 - The two-body render

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU1d5` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU1d4 (play-test passed) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (a separate `SERV_IRUHADEV_JUNE_DIVERGE_*` flag if a June body differs beyond the body pick) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | none of its own: `kickoffs/char-luciel/DU1-review.md` covers DU1-DU1d5 |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU1d3 - Done" "DU1d4 - Done"
python tools/docsection.py kickoffs/june-engine/R5.md "R13 amendment (2026-10-17)" "char-luciel DU1d4 amendment (2026-11-14)"
grep "(DU1d4)" june2017_re/funcmap.tsv
```

## Task

Part 12 of DU1 (split by DU1d4, 2026-11-14). Nothing draws Ciel yet: March's user render prepares and draws `m_pXSkinAnim` only. Port June's
three render functions, each whole (June's body inside March's own function, March's in `#else`):

- **`CommonRender_Prepare` = June `31fd07b0`** (user vslot `0x7e8`): the active body NULL -> no render; the hint from the per-body prepare with
  `true`; when `GetNotSelectXSkinAnimPtr()` exists, the per-body prepare again with `false`, its hint dropped (keep June's order and the drop).
- **The per-body prepare = June `31ff03d0`** (user vslot `0x7e4`, none in March; name it in the class pattern, no `June`): the anim is the
  active body (`true`) or the not-selected one (`false`); with `+0x8fa4` and `+0x8fb4` set, `+0x8fa8` 0 takes the second body (`32071d40`), 1 takes
  vslot `0x58`. The `+0x8fa4`-`+0x8fb4` members belong to DU3's hide-equip-by-body pass (`3204e4a0`): declare them here if DU3 has not, zeroed,
  and say so. The rest is March's `CommonRender_Prepare` body: diff it line by line against June's (render-type numbering, `3227baf0( 0x3b1 )`,
  the `+0x160` world-mesh-hidden copy @`31ff0e3a`, the room slot refresh, the equip loop) and port June's version of every line. The `+0x160` copy is
  also june-engine R5's item (amendment): port it here if R5 has not run, and tell R5.
- **`OnFrameRender_Draw` = June `31fd0880`** (user vtable `334781f4` slot 3): only a NULL test on the active body at the top, `m_bHideNoRenderable`
  (`+0x9170`) on both anims, draw the active body then the not-selected one when instant-show, then the view equips with June's tests.
- Check the class overrides of `CommonRender_Prepare` (Eve, Add, Chung, Elesis, Ara ... in March; June's vtables `334cf45c` / `334daaec` and Chung's
  `32cb6eb0` / `32965e70`) still reach the base the way June's do.

Then a full build and a short render play-test on two March classes (village and dungeon: body and equips drawn, the invincible blink, damage
light, frozen / poison colour, super-armor outline, the after-image). Then `DU1-review.md` runs.

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
