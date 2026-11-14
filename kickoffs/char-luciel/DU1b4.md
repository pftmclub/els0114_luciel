# June character Lu/Ciel (Stage 8) - Phase DU1b4 - Ciel's speed keys and the remaining state-start keys

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU1b4` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU1b3 |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (same flag: one logical change, now in seven parts) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | none of its own: `kickoffs/char-luciel/DU1-review.md` covers DU1-DU1d |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU1b3 - Done"
grep "(DU1b3)" june2017_re/funcmap.tsv
grep -n "31fe3a60\|SPEED_TIME" june2017_re/funcmap.tsv
```
Keys and addresses: `june2017_re/xorstrings.tsv` (`func_va` = `320506f0` / `32057b80`), `june2017_re/char_luciel/ch0_keys.tsv`.

## Task

Split from DU1b3 (2026-11-14): DU1b3 ported the physics (the second PhysicParam `m_MinorUnitPhysicParam`, June unit `+0x36c`, and the whole
`PhysicProcess` second body). Port whole under the flag what feeds and reads that PhysicParam outside `PhysicProcess`, and the keys left over:

- **`CommonStateStartFuture` `320506f0`:** `MINOR_UNIT_SPEED_X / _Y` into `+0x370 / +0x374` (two parse sites: `@320528ce` / `@32052995` and
  `@320536ec` / `@3205387c`; read why there are two, the siblings `@32052a1b` / `@320535e4` in `ch0_keys.tsv`), `MINOR_UNIT_PASSIVE_SPEED_X / _Y`
  into `+0x378 / +0x37c` (`@32052e0d` / `@32052f6e`, `@32053d60` / `@32053f88`), the APPLY_MOVEMENT_SPEED copy into the minor bool (`@3205418e`)
  and its rescale of the minor speeds (`@32054244-32054283`, beside March's `SERV_IRUHADEV_JUNE_APPLY_MOVEMENT_SPEED` block).
  `USE_NOT_SELECT_UNIT_ANIM_TIME_IN_SPEED` (`+0x8f6d`, Future only, `@3205790b`; miss stores 0).
- **`CommonFrameMoveFuture` `32061bf0`:** the minor passive-speed apply `@320631b0-320631e2` (March's `passiveSpeed` -> `nowSpeed` twin), the
  `+0x8f6d` speed-time reader `@3206211d` (before March's `m_vecSpeedFactor` loop, the `SERV_IRUHADEV_JUNE_SPEED_TIME` block: when set, vslot
  0x260 true and `m_bDuoEnabled`, the time is the not-selected body's anim time) and the `+0x8604` twin `@320623ed` / `@32062419` applied to
  `+0x370..+0x37c` (`@32062537-32062577`, after that loop). Both use DU1b2's `fNowAnimTime` (the active body) otherwise.
- **The `MINOR_UNIT_SPEED_TIME(_ULTIMATE)` table `+0x8604`:** June's loader `31fe3a60` swaps the main / minor lists by `+0x8e90`; check what
  March's `SERV_IRUHADEV_JUNE_SPEED_TIME` port (C3l) does with the minor list and add what is missing (checklist §7o L530-532).
- **CH1o's keys still unread:** `FORCE_DYNAMIC_CAMERA_INDEX` `@3205c0e8`, `RETAIN_CAMERA_STATE` `@3205b934`, `SUPER_ARMOR_CUSTOM_COLOR`
  `@3205941f` (CH3c reads it). `MINOR_UNIT_AFTER_IMAGE` goes with DU1d's second after-image; `CP_AMOUNT` is CH1p's.

Every other class has form 0 and none of these keys: prove no change with `/Zs` probes of `X2GUUser.cpp` and one class file. No play-test of
its own: "full build skipped: no play-test".

No hardcoded enum literals (user, 2026-10-26; CLAUDE.md): an id that has a name in an Enum (June's
`Enum.lua`, or a C++ enum) is never written as a number, in any new or touched code: use the member, and
register it under a `SERV_IRUHADEV_` flag with June's value and name if C++ lacks it. A value with no Enum
entry stays a number with a one-line comment. The one exception is a table whose rows are the data itself
(a `.tsv` of ids), not code.

No `June` prefix (user, 2026-10-28; CLAUDE.md): June's body goes inside March's own function under the flag,
March's in `#else`. A function March lacks gets a name in the class's own pattern, never with `June` in it.
Every name (locals and constants too) says what it is in context: no Ghidra address, phase tag or `June` in an identifier.
Call a March function from June code only if June's namesake does the same; otherwise replace March's definition.
Run `python tools/junediverge.py --list` before transcribing; a newly found divergent function gets its `june_march_divergent.tsv` row and the replacement in the same commit.
