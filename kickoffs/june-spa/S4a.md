# June spa villages - Phase S4a - The Pporu class

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-spa S4a` |
| Plan doc | `JUNE_SPA_PHASES.md` (branch `main`) |
| Depends on | S3b review; not beside another session editing `X2Lib_2010.vcxproj` |
| Flag | `SERV_IRUHADEV_JUNE_SPA_PPORU` |
| Log tag | `[JUNE_SPA]` (`python tools/logs.py --tag JUNE_SPA`) |
| Review | inside `kickoffs/june-spa/S4b-review.md` |
| Hand-off | none of its own (S4b's play-test runs it); probe build only |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_SPA_PHASES.md "Rules" "Phase S4a - The Pporu class" "Phase S0 - Done"
```

Then the map rows marked `S4a` in `june2017_re/spa/spa_map.tsv` (`awk -F'\t' '$5 ~ /^S4a/' june2017_re/spa/spa_map.tsv`).

## Task

Port the `+0x624` object whole as a new class (`32ec76e0` .. `32ec8350`, the 14 map rows marked S4a). June has no vtable or RTTI for it: name
the class and its methods from what they do, in the field game's pattern (`CX2...`), no `June` in any name. Its own `.h` / `.cpp`, added to
`X2Lib_2010.vcxproj` and its filters.
- Constants as June holds them: interval 480.0 (`@33537ff4`), `rand() % 240 + 480`, success 0.2 s (`@334411a8`), click 0.5 / 1.0 s,
  270 y offset; strings STR 43533, 23588, 3860, 851, 852; item 164548.
- The catch request sends March's `EGS_BUY_ED_ITEM_REQ` (June's 0xe6 has its shape: `m_mapItem { 164548: 1 }`); read June's `32ec7b40` for
  the other fields it sets, and its ACK path `32ec7fa0` against March's `CX2UIShop::Handler_EGS_BUY_ED_ITEM_ACK`.
- The class is inert until S4b creates it: probe build only (full build skipped: no play-test).

Run the gates with `tools/finish.py --with SERV_IRUHADEV_JUNE_SPA_PPORU`.

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
