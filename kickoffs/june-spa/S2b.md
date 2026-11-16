# June spa villages - Phase S2b - Spa doors: entering and leaving

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-spa S2b` |
| Plan doc | `JUNE_SPA_PHASES.md` (branch `main`) |
| Depends on | S2a; not beside another session editing `X2TFieldGame.*` or `X2State.cpp` |
| Flag | `SERV_IRUHADEV_JUNE_SPA_ENTRY`, `SERV_IRUHADEV_JUNE_SPA_DEBUG` |
| Log tag | `[JUNE_SPA]` (`python tools/logs.py --tag JUNE_SPA`) |
| Review | `kickoffs/june-spa/S2b-review.md` (covers S1 and S2a) |
| Hand-off | now; **seam** for S1 and S2a: `build.py --deploy`, then hand off `python tools/playtest.py assemble june-spa S1 S2a S2b` in full |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_SPA_PHASES.md "Rules" "Starting context" "Design" "Decisions" "Phase S2b - Spa doors: entering and leaving" "Phase S0 - Done" "Phase S1 - Done" "Phase S2a - Done" "Play-test hand-off"
```

Then the map rows marked `S2b` in `june2017_re/spa/spa_map.tsv` (`awk -F'\t' '$5 ~ /^S2b/' june2017_re/spa/spa_map.tsv`).

## Task

Port June's door code whole, as the map rows marked S2b list:
- TFG ctor: the `+0x324` / `+0x334` lists under June's inline code-event 31 test (`324f2ca0`, `324f2d30` as `CX2LocationManager` functions);
  the dtor frees them;
- OnFrameMove: the per-frame prompt reset, the entrance loop (150.0), the spa-village block's exit loop and exit key, and the spa-sign talk flag
  in the field-NPC loop (`X2TFieldGame.cpp:1937`'s comment goes);
- the prompts `323ab220` / `323ab390` and the reset `323ab4f0` with its two callers (`CX2CashShop::SetOpen`, the `CX2StateField` slot);
- the key handler's door branch (`323b2e80`: return position and line, the OK / Cancel box with `USCM_ENTER`), `USCM_ENTER` in
  `323b4040` (level-10 refusal, 1500 / 1501), the sign cases of `323b3690`, and the exit `323a8a60`;
- `Handler_EGS_STATE_CHANGE_FIELD_REQ`'s June body (the spa return: Ruben line 5 / Ranox line 31 spots); its June-only states (0xf guild
  agit, 0x16) get labelled stand-ins. Check `junediverge.py` for its row first.
- Register `USCM_ENTER` (75000, `UI_SPA_CUSTOM_MSG`) where C++ lacks it.
- Use S2a's `CEI_SPA_ON_OFF` (`ScriptID_Code.h`, flag `_OFFLINE_CODE_EVENTS`) for every code-event 31 test, including the sign test's literal 31 at
  `X2TFieldGame.cpp:3388` (Atlas E1d's line: `#ifdef` new `#else` old). The doors' destination is in `m_uiBattleFieldId` / `m_usBattleFieldStartLineIndex`
  (S2a reads June's `OtherPlace*` keys into them).
Leave the Pporu lines (S4b) and the soak (S3b) out.

Debug lines under `_DEBUG`, throttled: `door near=<0/1> pos=<id>`, `enter village=<id> return=<village>/<line>`, `exit to=<village>/<line>`.

Run the gates with `tools/finish.py --with SERV_IRUHADEV_JUNE_SPA_ENTRY SERV_IRUHADEV_JUNE_SPA_DEBUG`, then `build.py --deploy`, and paste
`exestr.py "[JUNE_SPA]"` beside `DEPLOY OK`.

Write section `S2b` of `playtest.md`, then hand off the assembled S1 + S2a + S2b sections. The checks:
- the Ruben door and the Ranox door: prompt, June's box, OK enters (loading image, BGM, world, NPCs); Cancel stays;
- a level-9 unit gets June's refusal string;
- talking to each spa sign opens the same box;
- both exits of each spa lead to June's spot by the door;
- `elsdb.py code-event off 31` and a relog: no door prompt, no signs; then switch it back on.

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
