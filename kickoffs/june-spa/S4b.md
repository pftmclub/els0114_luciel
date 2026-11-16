# June spa villages - Phase S4b - Pporu in the field game

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-spa S4b` |
| Plan doc | `JUNE_SPA_PHASES.md` (branch `main`) |
| Depends on | S4a; not beside another session editing `X2TFieldGame.*` |
| Flag | `SERV_IRUHADEV_JUNE_SPA_PPORU`, `SERV_IRUHADEV_JUNE_SPA_DEBUG` |
| Log tag | `[JUNE_SPA]` (`python tools/logs.py --tag JUNE_SPA`) |
| Review | `kickoffs/june-spa/S4b-review.md` (covers S4a) |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_SPA_PHASES.md "Rules" "Starting context" "Decisions" "Phase S4b - Pporu in the field game" "Phase S0 - Done" "Phase S4a - Done" "Play-test hand-off"
```

Then the map rows marked `S4b` in `june2017_re/spa/spa_map.tsv` (`awk -F'\t' '$5 ~ /^S4b/' june2017_re/spa/spa_map.tsv`).

## Task

Hook S4a's class into the field game, as the map rows marked S4b list:
- created in the TFG ctor for 1500 / 1501 only, the cheat flag `+0x620` cleared; deleted in the dtor;
- its tick in OnFrameMove's spa block, and the field-NPC loop steps (cheat -> ready, appear, click mark);
- the key handler's and `323b3690`'s Pporu skips (the click), `EventProcess` forwarding, the render hook in `323aea40`;
- the hidden start in add-field-NPC (`323b0980`);
- the Lua bind `SetAppearPPoruCheat` on `CX2TFieldGame` (`luabind.py` / the class_def list), so the dev console can make Pporu appear;
- textures (`SPA_PPORU_*.DDS` with their `.TET`), `Alarm_SpaPporu.ogg` and the `PPoru_Click` particle checked staged (S1).

Debug lines under `_DEBUG`, throttled: `pporu create`, `pporu appear`, `pporu click ok=<b>`, `pporu free`.

Run the gates with `tools/finish.py --with SERV_IRUHADEV_JUNE_SPA_PPORU SERV_IRUHADEV_JUNE_SPA_DEBUG`, then `build.py --deploy`.

Play-test in both spas (give the dev-console line for the cheat and the `/additem` / ED setup):
- the cheat makes Pporu appear: alarm, "Start" anim, on-screen notice;
- click it during "Wait": the Hard-boiled Egg (164548) arrives and 20000 ED is taken;
- with less than 20000 ED: June's refusal line, no egg;
- leave and re-enter twice: no crash, no second object.

Expected log lines: `pporu create`, `pporu appear`, `pporu click`, `pporu free`, the emulator's buy line.

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
