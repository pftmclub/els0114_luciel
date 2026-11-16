# June asset port - Phase P1 review

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase june-asset-port P1 review` |
| Plan doc | `JUNE_ASSET_PORT_PHASES.md` (branch `main`) |
| Depends on | Phase P1 done (its Done note and commits) |
| Flag | `SERV_IRUHADEV_KOM_MOUNT_DEBUG` |
| Log tag | `[KOM_MOUNT]` (`python tools/logs.py --tag KOM_MOUNT`; KEM_ERROR lines in `ErrorLog.txt`, duplicate names in `existfileinfo.txt`) |
| Review | - |
| Hand-off | - |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_ASSET_PORT_PHASES.md "Amended 2026-11-16" "Rules" "Phase P1 - Mount diagnostic and baseline" "Phase P1 - Done"
```

## Task

Review P1's commit(s). Check that:
- the counts are taken where `m_mapMassFile.insert` really succeeds, including the `SERV_IRUHADEV_KOM_LONG_NAMES` branch;
- the per-kom store has no per-frame cost and logs exactly once;
- `GetProcessMemoryInfo` links (psapi) in `US_SERVICE`;
- a kom that `AddMassFile` deletes (the duplicate path, `:576`) is visibly absent rather than logged as mounted;
- file-local names sit in a named `_CONST_KOMMOUNT_P1_` namespace, not an anonymous one.

**Hygiene** is one command: `python tools/finish.py --review --commits <the P1 commits>` (not a `--range` to HEAD). Paste its block, run it
with `-v`, and explain every OTHER row and every deleted line.

**Then:**
- fix clear defects under the same flag, then `tools/build.py --deploy` (a fix changes the exe);
- the debug define stays on: P2-P10 read `[KOM_MOUNT]`, and P12 retires it;
- write "Phase P1 - Review" into the plan doc, update the STATUS row, check `P2.md`, commit.
