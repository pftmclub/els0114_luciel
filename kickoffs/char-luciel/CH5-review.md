# June character Lu/Ciel (Stage 8) - Phase CH5 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH5 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH5 done and play-tested (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_UI` |
| Log tag | none (visual play-test) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH5 - Done (2026-11-16)"
```

## Task

Review the CH5 commit (`git log --oneline --grep "char-luciel Phase CH5"`) against June in Ghidra. Read-only checks, 4-5 directly:
- `322e07e0` @322e1840-322e1885 and @322e1be1: the Lu arm's unit-type test, the (id, level) arguments of `32b14f70`, the `JL` direction, and that the arm sits after the Rose arm and before the final else;
- `324ce260`: the jump table `324ce82c` / `324ce87c` classes -> file names (37 / 200, 38 / 128, 39, 127, 129) and the `+0xb54 > 0` test;
- `327e0040`: key names `LU` / `CIEL` (XOR-decoded at `337aa760` / `337aa75c`), order of the two strings, the abort on a missing key;
- `328dcba0` / `328db7a0`: the flip for classes 38 / 127 / 129, the transcended pieces, the `+0x8c` store at `328edaca`, and the 328e8f75 call (June skips the generic picture for Lu, the port overwrites it);
- `323cd570` form byte -> voice row; `324d08a0` / `324c99d0` name and emblem.
Also: the three routed items (GetCommonDLG, PlayEmotionSound, manufacture / PvP loading) have a bullet in `JUNE_PORT_BACKLOG_ORDER.md` Stage 9; JUNE_BUGS 754-755 are correct.

Hygiene: `python tools/finish.py --review --commits <the CH5 commit> --with JUNE_CHAR_LUCIEL_UI`. Explain every OTHER row and deleted line.
Fix clear transcription errors under the same flag; hand design decisions back to the user. Write "CH5 - Review" into the plan doc, update STATUS.md, commit.
