# June spa villages - Phase Z - Close

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, medium** effort, then type `/x2-phase june-spa Z` |
| Plan doc | `JUNE_SPA_PHASES.md` (branch `main`) |
| Depends on | every S review |
| Flag | none new (retires `SERV_IRUHADEV_JUNE_SPA_DEBUG`) |
| Log tag | `[JUNE_SPA]` (`python tools/logs.py --tag JUNE_SPA`) |
| Review | none |
| Hand-off | none |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_SPA_PHASES.md "Rules" "Phase Z - Close"
```

## Task

Close the plan as the phase section says:
- `SERV_IRUHADEV_JUNE_SPA_DEBUG` commented out (no build: debug retire only);
- `MODS.md` complete for every flag of this plan;
- `spa_map.tsv` has no `absent` / `mixed` row without a routed owner;
- `python tools/junediverge.py --check-all` prints `RESULT: PASS`;
- backlog Stage 24 marked closed with its routed items (spa avatars -> Stage 11, menu icon -> Stage 9, achievements -> Stage 31);
- a memory note for the plan;
- the STATUS block moved to `STATUS_ARCHIVE.md`, with one *Closed plans* row.
