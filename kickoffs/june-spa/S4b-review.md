# June spa villages - Phase S4b review (covers S4a)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-spa S4b review` |
| Plan doc | `JUNE_SPA_PHASES.md` (branch `main`) |
| Depends on | S4b done and its play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_SPA_PPORU`, `SERV_IRUHADEV_JUNE_SPA_DEBUG` |
| Log tag | `[JUNE_SPA]` (`python tools/logs.py --tag JUNE_SPA`) |
| Review | - |
| Hand-off | - |

## Read first

```sh
python tools/docsection.py JUNE_SPA_PHASES.md "Rules" "Starting context" "Decisions" "Phase S4a - The Pporu class" "Phase S4b - Pporu in the field game" "Phase S4a - Done" "Phase S4b - Done"
```

Then the map rows marked `S4a` and `S4b`.

## Task

Review S4a's and S4b's commits against the map and the exe:
- every June function is whole: branch polarity, NaN tests, constants from `.rdata`, member order (`+0` .. `+0x1c`);
- the buy request's fields against `32ec7b40`, the ACK path against `32ec7fa0`;
- the create / delete pairing (ctor, dtor) and the hooks' places in OnFrameMove;
- the `#else` path is March's.

Spot-check three items in Ghidra. If one fails, re-check the whole phase.

Run `python tools/finish.py --review --commits <S4a and S4b commits>` with `-v`, and explain every OTHER row and deleted line. Fix
transcription errors under the same flag (then `build.py --deploy`); design questions go to the user. Write "Phase S4b - Review", update the
STATUS rows, check `S5.md`, and commit.
