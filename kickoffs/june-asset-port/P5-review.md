# June asset port - Phase P5 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-asset-port P5 review` |
| Plan doc | `JUNE_ASSET_PORT_PHASES.md` (branch `main`) |
| Depends on | Phase P5 done |
| Flag | none |
| Log tag | `[KOM_MOUNT]` (`python tools/logs.py --tag KOM_MOUNT`; KEM_ERROR lines in `ErrorLog.txt`, duplicate names in `existfileinfo.txt`) |
| Review | - |
| Hand-off | - (no play-test) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_ASSET_PORT_PHASES.md "Amended 2026-11-16" "Rules" "Phase P5 - Collision key forecast" "Phase P5 - Done"
```

## Task

Review P5.
- **Parsers:** check each one in `june2017_re/asset_keys.py` against the March loader it cites: TET, XET, KSM/KME, and xof including
  MSZIP.
- **Forecast rows:** hand-check 10 `clean` and 10 `will-break` rows of `p5_collision_forecast.tsv`. Open both files, find the key and
  find the March reader.
- **Routing:** confirm every `will-break` row reached its owner's doc.
- **Wave lists:** confirm P6, P7 and P8 together cover every colliding non-`.txt` file exactly once, keep atlas/TET and mesh/motion pairs
  together, and contain no held name (Rule 8) and no file a peer has since made `same` (re-run the census first).

**Then:**
- fix errors in the tool or the TSVs;
- write "Phase P5 - Review", update STATUS, check `P6.md`, commit (hygiene: `finish.py --files <the .py files> --no-flag "analysis only"`).
