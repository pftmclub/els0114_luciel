# June character Add (Stage 7) - Phase CH0c review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-add CH0c review` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | Phase CH0c done (its Done note and commits) and the user's CH0c play-test |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_CREATE` (+ `_DEBUG`, retired here) |
| Log tag | `[JUNE_CHAR]` (`ADD CREATE`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Phase CH0c - Add on the creation screen (March classes)"
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md "CH0c - Done (2026-11-13)"
cat june2017_re/char_add/ch0_create.tsv
grep -P "\tChar Add CH0c|Char Add CH0c:" june2017_re/funcmap.tsv
```

## Task

Review CH0c's commit(s) against `ch0_create.tsv` and June's creation-screen functions:
- The ctor hunk: only the `UT_ADD` slot test is in `#else`; the Raven-Elesis tests and the default-select switch (`X2StateCreateUnit.cpp:175-230`)
  are untouched; June's ctor `3255bd10` really has no limit test for `AddCreateUnitSlot` (spot-check in Ghidra).
- The seven `X2UIPersonalShopBoard.cpp` blocks: each `#else` holds March's `if` verbatim, the `AddItem` stays outside, and combo index 9 maps to
  `UT_ADD` in both index readers.
- The June reads in the Done note: SelectAdd `32555320` floats, `SelectUpperClass` `325519d0` writing `+0x2c0`, `3254e410` as a preload (not
  `EquipPromotionItem`), the exchange-shop routing. Spot-check 2 of them; if one fails, re-check them all.
- The play-test log: `[JUNE_CHAR] ADD CREATE slot shown ( US limit 8, slot found )`, `ADD CREATE select class 9 type 9`, the emulator's
  `CREATE '<name>' class=9 -> unitUID=...`, and no new error against `data\ErrorLog_pre_CH0c_add.txt` (`python tools/logs.py --tag JUNE_CHAR`).
- Retire the debug lines: comment out `#define SERV_IRUHADEV_JUNE_CHAR_ADD_CREATE_DEBUG` (start the build, don't wait: CLAUDE.md 2026-11-03).

Hygiene is one command, not a fresh script:
`python tools/flagcheck.py JUNE_CHAR_ADD_CREATE --range <first CH0c commit>^..<last CH0c commit> --with JUNE_CHAR_ADD_CREATE_DEBUG`, then
`git log -SJUNE_CHAR_ADD_CREATE <last>..HEAD`. Paste its `files:` / `in this flag's files:` / `RESULT:` lines. Explain every OTHER row and
every deleted line; a FAIL or ERROR is a finding. `python tools/junediverge.py --check-all` must print `RESULT: PASS`.

Fix clear errors under the same flag; hand design decisions back to the user. Write "CH0c - Review" into the plan doc, update the CH0c STATUS
row (done; CH6 depends on this review), and commit with explicit pathspecs.

You are on Opus: delegate the reads, greps, log reads and flagcheck runs (x2-phase, *When the phase is Opus*). CP949 sources: `bytepatch.py` only.
Phase tag (user, 2026-11-09): any edited block carries `// char-add CH0c review`.
