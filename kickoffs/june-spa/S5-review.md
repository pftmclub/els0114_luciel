# June spa villages - Phase S5 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-spa S5 review` |
| Plan doc | `JUNE_SPA_PHASES.md` (branch `main`) |
| Depends on | S5 done and its play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_SPA_RPS`, `SERV_IRUHADEV_JUNE_SPA_DEBUG` |
| Log tag | `[JUNE_SPA]` |

## Read first

```sh
python tools/docsection.py JUNE_SPA_PHASES.md "Rules" "Starting context" "Design" "Decisions" "Phase S5 - Done"
```

Then the phase's own section (`docsection.py JUNE_SPA_PHASES.md --list` gives its exact heading) and the map rows marked `S5`.

## Task

Review S5's commits against the map rows marked `S5` and the exe:
- every June function is whole: branch polarity, NaN tests, constants from `.rdata`, member order;
- struct fields and `Put` / `Get` order, where the phase touched packets;
- the emulator stand-in matches "Decisions" and logs `stand-in:`;
- the `#else` path is March's.

Spot-check three items in Ghidra. If one fails, re-check the whole phase.

Run `python tools/finish.py --review --commits <S5 commits>` with `-v`, and explain every OTHER row and deleted line. Fix transcription
errors under the same flag (then `build.py --deploy`); design questions go to the user. Write "Phase S5 - Review", update the STATUS row,
check `Z.md`, and commit.
