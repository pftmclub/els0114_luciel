# June spa villages - Phase S3b review (covers S3a and S3c)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-spa S3b review` |
| Plan doc | `JUNE_SPA_PHASES.md` (branch `main`) |
| Depends on | S3b done and its play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_SPA_BUFF`, `SERV_IRUHADEV_OFFLINE_SPA_BUFF`, `SERV_IRUHADEV_OFFLINE_SPA_SOAK_TIMER`, `SERV_IRUHADEV_JUNE_SPA_DEBUG` |
| Log tag | `[JUNE_SPA]` (`python tools/logs.py --tag JUNE_SPA`) |
| Review | - |
| Hand-off | - |

## Read first

```sh
python tools/docsection.py JUNE_SPA_PHASES.md "Rules" "Starting context" "Design" "Decisions" "Phase S3a - Spa packets and the emulator's buff answer" "Phase S3b - Soaking, the spa notice and effect" "Phase S3a - Done" "Phase S3b - Done" "Phase S3c - Done"
```

Then the map rows marked `S3a` and `S3b`.

## Task

Review S3a's and S3b's commits against the map and the exe:
- every June function is whole: branch polarity, NaN tests (the 3.0 s test is `<=` through `test ah, 0x41; jp`), constants from `.rdata`,
  member order;
- struct fields and `Put` / `Get` order against `320acab0`, `320acb70`, `320acbe0`;
- the emulator stand-in matches "Decisions" 1 as amended 2026-11-17 (S3c: 60 s soak, pause / resume, 0x639 per minute; S3a's grant in `#else`) and logs `stand-in:`;
- the `#else` path is March's.

Spot-check three items in Ghidra. If one fails, re-check the whole phase.

Run `python tools/finish.py --review --commits <S3a, S3b and S3c commits>` with `-v`, and explain every OTHER row and deleted line. Fix
transcription errors under the same flag (then `build.py --deploy`); design questions go to the user. Write "Phase S3b - Review", update the
STATUS rows, check `S4a.md`, and commit.
