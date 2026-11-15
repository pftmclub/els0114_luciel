# June engine remainder - Phase R5b - CUSTOM_RENDER_STATE per-equip switch (sub-equips, after-image, show / invisibility)

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase june-engine R5b` |
| Plan doc | `JUNE_ENGINE_REMAINDER_PHASES.md` (branch `main`) |
| Depends on | R5 (code landed; build + play-test of R5 first) |
| Flag | `SERV_IRUHADEV_JUNE_CUSTOM_RENDER_STATE_EQUIP` (+ `_DEBUG` if the phase logs) |
| Log tag | `[JUNE_ENGINE] CRS` (`python tools/logs.py --tag JUNE_ENGINE`) |
| Review | `kickoffs/june-engine/R5b-review.md` (the phase writes it when done) |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_ENGINE_REMAINDER_PHASES.md "Rules" "Starting context" "Phase R5 -- Done"
grep -n "(R5)" june2017_re/funcmap.tsv
```

## Task

R5 ported the first half of `CX2GUUser::CommonFrameMove`'s per-equip custom render state block (`32063650 @320659e9-32065bb4`: the state pick and the
CRS 6 render param). R5b ports the second half, `@32065bc1-32065c9f`, inside the same equip loop (`X2GUUser.cpp`, after `SetRenderParam`, before the
equip effect set block), using `CX2GUUser::GetNowCustomRenderState()`:

- **CRS 3 and 9:** `+0x941c` = 1, then `CX2Eqip` `3295a920` ( for each sub-equip, `CX2SubEquip::SetRenderParam` with the body's render param ).
- **CRS 5:** the same, then hide the first after-image list ( `3322a9c0( false )` on `m_pAfterImage`, a show-flag setter over its entry list ), `DisableAfterImage( true, 3 )`,
  `SetShowObject( false, false )` and `SetInvisibility( true )` on the unit.
- **No state while `+0x941c` is 1:** on the last equip of the loop `+0x941c` = 0 and, when `+0x7c80` is 1, `EnableAfterImage( 3 )`; for each equip `3295a990`
  ( the sub-equip's skin render param = its default `+0x268` ), then `SetShowObject( true, false )`, `SetInvisibility( false )` and `UpdateNotRenderingOtherEquipList`
  ( user vslot 0x630 ).
- **New members:** `CX2SubEquip` `+0x268` is a default `RenderParam` made by both ctors, with a setter `32f08a90` called from `32961c80`
  ( read that caller first: it is the item attach path ); `CX2GameUnit` `+0x7c80` is set by `CreateAndInsertAfterImageByBuff` and cleared by
  `EraseAfterImageByBuff`; user `+0x941c` is R5's missing member.
- Funcmap rows `3295a920 / 3295a990 (R5)` and `32063650 @32065bc1-32065c9f (R5)` hold what R5 found; verify the sub-equip layout in Ghidra before coding.
- **Play-test:** a state with `CUSTOM_RENDER_STATE` 5 on a unit with a skin-mesh sub-equip. None is on March data; the dev-console route is Raven's Power Assault
  ( CRS 8 only ) for the exit path, and the Ara / Add class ports for CRS 3 / 5 / 9. If no state reaches it, say so in the Done note and hand off with the
  exit path only.

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
