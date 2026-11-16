# June spa villages - Phase S2 review (covers S1)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-spa S2 review` |
| Plan doc | `JUNE_SPA_PHASES.md` (branch `main`) |
| Depends on | S2 done and its play-test passed |
| Flag | `SERV_IRUHADEV_OFFLINE_CODE_EVENTS`, `SERV_IRUHADEV_JUNE_SPA_ENTRY`, `SERV_IRUHADEV_JUNE_SPA_DEBUG` |
| Log tag | `[JUNE_SPA]` |

## Read first

```sh
python tools/docsection.py JUNE_SPA_PHASES.md "Rules" "Starting context" "Design" "Decisions" "Phase S1 - Spa data and assets" "Phase S2 - Code-event switch, entering and leaving the spa" "Phase S1 - Done" "Phase S2 - Done"
```

## Task

Review S1's data and S2's code against the map and the exe.

**S1:**
- the merged rows equal June's: both village rows, the Ruben door, houses 40141-40147, NPCTemplet, MapData, WorldData, the Enum values;
- every other file is byte-equal to June's (`luasrc.py` / `kom.py` evidence);
- re-run the closure, luabind, core and ledger gates.

**S2:**
- the ctor's code-event test and list builders;
- the prompt distances and particles;
- `323b2e80`'s stored fields and their readers;
- the exit path and the Ruben cases;
- the code-event migration (forward-only, seeded row), the login send's shape against `31f00970`, and `elsdb.py`.

Spot-check three June reads in Ghidra. If one fails, re-check the whole phase.

Run `python tools/finish.py --review --commits <S1 and S2 commits>` with `-v`, and explain every OTHER row and deleted line. Fix
transcription errors under the same flag (then `build.py --deploy`); design questions go to the user. Write "Phase S2 - Review", update the
STATUS row, check `S3.md`, and commit.
