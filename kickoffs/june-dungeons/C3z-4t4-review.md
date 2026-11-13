# June dungeons - C3z-4t4 review - the tutorial guide's game-side gates ( t3 ) and walkthrough scenes ( t4 )

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-dungeons C3z-4t4 review` |
| Plan doc | `JUNE_DUNGEONS_PHASES.md` (branch `main`) |
| Depends on | C3z-4t4 (play-test passed 2026-11-13) and C3z-4t4b done with its play-test |
| Flags | `SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_GATES` (+ `_DEBUG`), `SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_WALK` (+ `_WALK_DEBUG`), t1's `_GUIDE_DEBUG` (on again for t4) |
| Log tag | `JUNE_FX C3z-4t` (offline_server.log) |

## Read first

```sh
python tools/docsection.py JUNE_DUNGEONS_PHASES.md "C3z-4t3 - Done" "C3z-4t4 - Done"
grep "C3z-4t3\|C3z-4t4" june2017_re/funcmap.tsv
python tools/junediverge.py --list
```

Map: `june2017_re/c3z4t_scope.tsv`. Code: the `_GATES` and `_WALK` blocks (grep the flags over `X2Lib`, `KTDXLIB`).

## Task

1. **t3, against the disassembly:** ESC `31f1e1a0` @`31f1e1e8`-`31f1e212`; the dialog manager `+0x111` writers (`31fc42ba` / `31fc4d70`) and both
   readers (`323d2cb1`, `31d7d9cc`); StateChange `@31d88e27` and the village block; `31fc63f0` / `31fc63d0` and their call sites; SetShowUI `32154820`;
   the input mask `31fcea10` / `320708f0`; the zoom lock reader `3245a8f0`; party manager `+0x8a` (`3243d4e0` / `3243c260`).
2. **t4, against the disassembly:** the four begin-by-condition functions `31fc5920` / `31fc59f0` / `31fc5ae0` / `31fc5c20` with `31fbe080` (which
   scene field each matches, the `31fc4530` arguments, the `+0xcc` writes), `31fc4c30`, `31f31990`; each hook site: field ctor `32509d30`
   @`3250a047`-`3250a289` (the break, then 31fc5c20 still runs), battle-field ctor `324bbe50` (is `g_pData +0x134` the battle-field id where I am?
   the port uses `GetBattleFieldIdWhereIam`, marked likely), `324b90b0`, `32f17780` @`32f17917`, `31f09b50` @`31f09ce5`, `323e4cb0` @`323e5294`,
   `323e3cab`, `322e5b40` (the `+0xaf8` pre-update level; JUNE_BUGS 647), `322c2930` @`322c2cbe`.
3. **C3z-4t4b:** the quick-quest base y against June `323d0ca0` @`323d18c5`-`323d190c` (its flag in the hygiene step too). Scene 201 was walked in
   neither play-test (user 2026-11-13: left to this review): hand off the walk (new level-1 character, accept 11005, leave Ruben and come back
   before finishing it; look for `begin 201 ( by going quest )`). The deployed exe keeps `_WALK_DEBUG` until the next build after item 6.
4. **Routed, check the bullets exist:** Stage 4 (the informer rework `32f17780`, `323e05f0`), Stage 28 (`OpenPartyDLG` rework, `322c3fa0`).
5. **Hygiene:** `python tools/flagcheck.py SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_GATES --range 52951ffe^..52951ffe --with SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_GATES_DEBUG`
   and `python tools/flagcheck.py SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_WALK --range d393113b^..d393113b --files <the C3z-4t4 code files> --with SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_WALK_DEBUG`;
   paste the summary lines (`_DEBUG` `#error` guard ERRORs are house precedent). `python tools/junediverge.py --check-all` = `RESULT: PASS`;
   `enumscan.py --check` on the changed files = 0 A/B on added lines.
6. **Retire the debug lines:** comment out `#define SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_GATES_DEBUG`, `#define SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG`
   and `#define SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_WALK_DEBUG` (the last one is the play-test switch that skips June's account level-10 refusal:
   with it off the walkthrough again shows only on an account with no character at level 10+, as in June).

Fix clear transcription errors under the same flag (then a full build and a play-test); hand design decisions back to the user. If the
only code change is retiring the debug defines: no build (CLAUDE.md shared-builds rule), "build skipped: debug retire only" in the note.

## Done gate

A `C3z-4t4 - Review` note in `JUNE_DUNGEONS_PHASES.md` (pass / findings, flagcheck and junediverge lines), STATUS row updated, commit with
explicit pathspecs.

No hardcoded enum literals (user, 2026-10-26; CLAUDE.md). No `June` prefix (user, 2026-10-28; CLAUDE.md).
