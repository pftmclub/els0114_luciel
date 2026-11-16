# June spa villages - Phase S3b - Soaking, the spa notice and effect

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-spa S3b` |
| Plan doc | `JUNE_SPA_PHASES.md` (branch `main`) |
| Depends on | S3a; not beside another session editing `X2TFieldGame.*` |
| Flag | `SERV_IRUHADEV_JUNE_SPA_BUFF`, `SERV_IRUHADEV_JUNE_SPA_DEBUG` |
| Log tag | `[JUNE_SPA]` (`python tools/logs.py --tag JUNE_SPA`) |
| Review | `kickoffs/june-spa/S3b-review.md` (covers S3a) |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_SPA_PHASES.md "Rules" "Starting context" "Design" "Decisions" "Phase S3b - Soaking, the spa notice and effect" "Phase S0 - Done" "Phase S2b - Done" "Phase S3a - Done" "Play-test hand-off"
```

Then the map rows marked `S3b` in `june2017_re/spa/spa_map.tsv` (`awk -F'\t' '$5 ~ /^S3b/' june2017_re/spa/spa_map.tsv`).

## Task

Port June's soak loop whole, as the map rows marked S3b list:
- the TFG ctor's spa state init and the notice font `+0x480` (find March's matching font member, or add June's);
- `323ac9d0`: the spa type from `SPA_INFO`, by the line my unit stands on;
- the soak timers and both 0x637 sends in OnFrameMove (1.0 s and 3.0 s are doubles in June's `.rdata`; keep them as June holds them);
- 0x638 / 0x639 in `EventProcess`;
- the spa block of `323aea40`: the notice by type (STR 43074 / 43476), `323ac3e0`'s M:SS, `EffectSet_SPA_Buff` into `+0x484`, and its stop;
- the dtor's stop and leave send.
The dead twins (`323ac910`, `323ac990`, `323ad4c0`) stay out unless a caller turns up.

Debug lines under `_DEBUG`, throttled: `type <old> -> <new> line=<n>`, `send 0x637 flag=<b> type=<n>`, `recv 0x638 ok=<b> end=<t>`,
`effect play/stop handle=<h>`.

Run the gates with `tools/finish.py --with SERV_IRUHADEV_JUNE_SPA_BUFF SERV_IRUHADEV_OFFLINE_SPA_BUFF SERV_IRUHADEV_JUNE_SPA_DEBUG`, then
`build.py --deploy`.

Play-test in both spas:
- stand in a pool line: after 1 s the notice with M:SS left and the effect show; the buff icon shows 20:00;
- step out and back in: the buff restarts at 20:00 ("Decisions" 1);
- step out of the line: the effect stops;
- enter a dungeon: the buff applies, following June's PvE rule.

Expected log lines: `type`, `send 0x637`, `recv 0x638`, `effect`, `emu grant`.

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
