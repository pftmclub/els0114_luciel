# June character Lu/Ciel (Stage 8) - Phase DU1b3 - Ciel's physics and the remaining state-start keys

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU1b3` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU1b2 |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (same flag: one logical change, now in six parts) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | none of its own: `kickoffs/char-luciel/DU1-review.md` covers DU1-DU1d |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU1b - Done (2026-11-14)" "DU1b2 - Done"
python tools/docsection.py kickoffs/char-luciel/DU1.md "Amended by NU0" "Amended by CH0 (2026-11-02)"
grep "Char Luciel DU1" june2017_re/funcmap.tsv
```
Then the `nu0_duo_engine.tsv` rows (`owner` = `DU1`) of `32015b30` and the `+0x8f5d` / `+0x8f60` / `+0x8f6c` / `+0x8f6d` rows. Keys and
addresses: `june2017_re/xorstrings.tsv` (`func_va` = `320506f0` / `32057b80`).

## Task

Split from DU1b (2026-11-14). Port whole under the flag:

- **`PhysicProcess` `32015b30`:** the second position integrated beside the main one (copied to locals `@32015b81`, the anim-move adds
  `@32017b2f-32017b64` / `@32017e06-32017e3b` / `@32017f7a..`), the four `+0x8f6c && +0x8f5d` tests (`@32018bee`, `@32019159`,
  `@32019460`, `@32019e25`) with the `+0x8f60` leas, the store `@32019e57-32019e7a`, and the not-selected body's offset
  `@32019f9b-3201a1c7` (direction vslot 0x16c * `+0x8f54`, y `+0x8f58`, the rotated delta when `+0x8f5c`; the parse already exists as
  `ParseNotSelectUnitOffset` under `SERV_IRUHADEV_JUNE_NOT_SELECT_UNIT_OFFSET`). `APPLY_ANIM_MOVE_MULTIPLY` (`+0x8f50`, read
  `@3201869d..@320188d1`) is not a duo key: port it here if it sits in the same anim-move lines, otherwise identify and route it.
- **State-start keys** (both `CommonStateStart`s): `USE_NOT_SELECT_MOTION_BEFORE_POS` (`+0x8f5d`, copies the Future second position to
  `+0x8f60`, Future `@32057139`, Now `@320613a6`), `USE_PHYSICPROCESS_NOT_SELECT_UNIT` (`+0x8f6c`), `USE_NOT_SELECT_UNIT_ANIM_TIME_IN_SPEED`
  (`+0x8f6d`, Future only) with its readers in `CommonFrameMoveFuture` (`@3206211d`, `@320623ed`), `MINOR_UNIT_SPEED_X / _Y` and
  `MINOR_UNIT_PASSIVE_SPEED_X / _Y` (the second PhysicParam `+0x36c`, checklist §7o L511-513), the `MINOR_UNIT_SPEED_TIME(_ULTIMATE)` table
  `+0x8604` and its frame-move reader (applied to `+0x370..+0x37c`), and the keys CH1o gives DU1 that are still unread:
  `FORCE_DYNAMIC_CAMERA_INDEX` `@3205c0e8`, `RETAIN_CAMERA_STATE` `@3205b934`, `SUPER_ARMOR_CUSTOM_COLOR` `@3205941f` (CH3c reads it).
  `MINOR_UNIT_AFTER_IMAGE` goes with DU1d's second after-image; `CP_AMOUNT` is CH1p's.

If the physics alone fills the session, split again the same way and say so in the Done note. Every other class has form 0 and none of these
keys: prove no change with `/Zs` probes of `X2GUUser.cpp` and one class file. No play-test of its own: "full build skipped: no play-test".

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

## Amended by DU1b2 (2026-11-14)

- `SyncData::vNotSelectPosition` exists (Future +0x7e48): `PhysicProcess` writes it here. `CommonFrameMove(Future)` already move the
  not-selected body to it.
- **Where the frame-move readers go in March's `CommonFrameMoveFuture`:** the `+0x8f6d` speed-time reader (`@3206211d`: when set,
  `vslot 0x260` true and `m_bDuoEnabled`, the speed-factor time is the not-selected body's anim time) sits just before March's
  `m_vecSpeedFactor` loop (the `SERV_IRUHADEV_JUNE_SPEED_TIME` block); the `+0x8604` twin (`@320623ed`, same `+0x8f6d` test with
  `vslot 0x260` false, applied to `+0x370..+0x37c`) follows that loop. Both use DU1b2's `fNowAnimTime` (the active body) otherwise.
