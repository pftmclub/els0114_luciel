# June user state ids - Phase Z - Close

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, medium** effort, then type `/x2-phase june-state-ids Z` |
| Plan doc | `JUNE_STATE_IDS_PHASES.md` (branch `main`) |
| Depends on | S2 |
| Flag | none (docs only) |
| Log tag | none |
| Review | none |
| Hand-off | none |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_STATE_IDS_PHASES.md "Rules for this plan" "Z - Close" "S1 - Done" "S2 - Done"
```

## Task

The plan doc's Z scope:
- a `MODS.md` row for `SERV_IRUHADEV_JUNE_USER_STATE_IDS`: what it does, that the exe and `data036.kom` ship together, and how to revert (flag off
  plus March's `USER_STATE_ID` rows back in `Enum.lua`);
- a `JUNE2017_CHARACTER_PORT_CHECKLIST.md` row: new characters (Rose, Ain) register at June's ids, use the base `USI_*` names (no shadow copies), and
  run `tools/stateids.py check`;
- backlog Stage 41 marked closed in `JUNE_PORT_BACKLOG_ORDER.md`;
- the stage-end census: `python tools/junebinary.py --check` and `python -I june2017_re/census/v2_exclusions.py`, both `RESULT: PASS`, with the
  unowned count in the Done note;
- the `june-state-ids` STATUS block appended to `STATUS_ARCHIVE.md`, replaced by one *Closed plans* row.
No code change, no build ("full build skipped: no play-test").
