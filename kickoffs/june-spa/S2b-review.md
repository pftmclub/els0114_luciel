# June spa villages - Phase S2b review (covers S1 and S2a)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-spa S2b review` |
| Plan doc | `JUNE_SPA_PHASES.md` (branch `main`) |
| Depends on | S2b done and the S1 + S2a + S2b play-test passed |
| Flag | `SERV_IRUHADEV_OFFLINE_CODE_EVENTS`, `SERV_IRUHADEV_JUNE_SPA_ENTRY`, `SERV_IRUHADEV_JUNE_SPA_DEBUG` |
| Log tag | `[JUNE_SPA]` (`python tools/logs.py --tag JUNE_SPA`) |
| Review | - |
| Hand-off | - |

## Read first

```sh
python tools/docsection.py JUNE_SPA_PHASES.md "Rules" "Starting context" "Design" "Decisions" "Phase S1 - Spa data and assets" "Phase S2a - Code events, spa village plumbing, Ruben omissions" "Phase S2b - Spa doors: entering and leaving" "Phase S1 - Done" "Phase S2a - Done" "Phase S2b - Done"
```

Then the map rows marked `S2a` and `S2b`.

## Task

Review S1's data and S2a's and S2b's code against the map and the exe.

**S1:**
- the merged rows equal June's: both village rows, the Ruben door, houses 40141-40147, NPCTemplet, MapData, WorldData, the Enum values;
- every other file is byte-equal to June's (`luasrc.py` / `kom.py` evidence);
- re-run the closure, luabind, core and ledger gates.

**S2a:**
- the code-event migration (forward-only, seeded row), the login send's shape against `31f00970`, and `elsdb.py`;
- `SpaData` and `VillageParsing` against `32501bb0`;
- each spa case outside the field game against its map row.

**S2b:**
- the ctor's code-event test and list builders, the dtor's frees;
- the prompt distances, textures and flags;
- `323b2e80`'s stored fields, the box and `USCM_ENTER`;
- the exit path and `Handler_EGS_STATE_CHANGE_FIELD_REQ`'s spa return (positions from `.rdata`).
- S2b's calls (Done note): the exit key is action 0 (`GA_ATTACK_FAST`), `HideJoinSpa` resets the entrance prompt only, the REQ's two stand-ins
  (the `g_pData +0x5b8` game-start flag, guild agit state 15) and the `stage38` rows for June's `TalkNpc` / `UICustomEventProc` / `SetShowStateDLG`.

Spot-check three June reads in Ghidra. If one fails, re-check the whole phase.

Run `python tools/finish.py --review --commits <S1, S2a and S2b commits>` with `-v`, and explain every OTHER row and deleted line. Fix
transcription errors under the same flag (then `build.py --deploy`); design questions go to the user. Write "Phase S2b - Review", update the
STATUS rows, check `S3a.md`, and commit.
