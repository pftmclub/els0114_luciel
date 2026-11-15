# June user state ids - Phase S1 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-state-ids S1 review` |
| Plan doc | `JUNE_STATE_IDS_PHASES.md` (branch `main`) |
| Depends on | S1 done (its Done note, the commit, the user's regression play-test result and "check logs") |
| Flag | `SERV_IRUHADEV_JUNE_USER_STATE_IDS` (+ `_DEBUG`, retired by this review) |
| Log tag | `[STATE_IDS]` |

## Read first

```sh
python tools/docsection.py JUNE_STATE_IDS_PHASES.md "Census (done in planning, 2026-11-15)" "Readers of the numbers (survey, 2026-11-15)" "Rules for this plan" "S1 - Done"
```

(`S1 - Done` is the heading S1's Done note uses; `--list` the doc if it differs.)

## Task

1. **Numbers.** `python tools/stateids.py check` must print `147 / 147 C++ = June` and `RESULT: PASS`. Spot-check 3 June values in Ghidra
   (read-only): `USI_TRANSCENDENTAL_HYPER_MODE` 24, `USI_WAIT_STATE_CHANGE` 130, `USI_END` 152.
2. **Readers.** Re-check the survey's "safe" verdicts with a grep, not from memory: any `(char)` / `(UCHAR)` / `(BYTE)` cast or `char` / `UCHAR` field that holds a
   user state id (`GetNowStateID`, `nowState`, `m_ucNowState`, `stateID`, `nextState`), any `255` / `0xff` / `127` test on a state id, any array or
   bitset sized by `USI_END`. Report each hit with its verdict; fix a real one under the flag.
3. **Training UI.** The three `X2TrainingGameUI.cpp` int compares; `#else` keeps March's cast.
4. **Enum.lua.** `luasrc.py ledger --check` PASS; `luabind.py check Ara_Martial_Artist Chung_Iron_Cannon` STATE gate PASS; the staged and deployed
   data036 carry June's `USER_STATE_ID` rows (`kom.py` pack / copy lines in the Done note).
5. **Play-test result and logs.** `python tools/logs.py --tag STATE_IDS`: no `UNREGISTERED` line naming a March class (a line for an id the class
   never registered in March too, such as an event or amusement state, is not a defect: list it). Retire the debug: comment out
   `SERV_IRUHADEV_JUNE_USER_STATE_IDS_DEBUG` in `Always.h` (a debug retire only needs no build).
6. **Enumscan tool change.** `tools/enumscan.py` now skips an enumerator's own `NAME = value`; six old rows (`X2GUUser.h` NUI_* constants, the
   `X2GUUser.cpp` relative-position `case 1..4`) carry `stray` verdicts in `june2017_re/enum_literals.tsv`. Confirm both are right.

Hygiene is one command: `python tools/finish.py --review --commits <S1 commit> --skip-flag JUNE_RESERVED_STATE_IDS "outer block" --skip-flag
JUNE_USER_STATE_ID16 "outer block" --skip-flag JUNE_SKILL_STATE_PTR "outer block of GetStateDataIterator"`. Run with `-v` and explain every OTHER row.

Fix clear errors under the same flag. Write "S1 - Review" into the plan doc, update the STATUS.md rows (S2 becomes ready), check `S2.md`, and commit.
