# June character Lu/Ciel (Stage 8) - Phase DU1b - The duo state machine and the not-selected body

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU1b` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU1 (part 1, done 2026-11-14) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (same flag: one logical change split in four parts) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | none of its own: `kickoffs/char-luciel/DU1-review.md` covers DU1, DU1b, DU1c, DU1d |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Phase DU - The two-body engine (DU1-DU5)" "DU1 - Done (2026-11-14)"
python tools/docsection.py kickoffs/char-luciel/DU1.md "Amended by NU0" "Amended by CH0 (2026-11-02)"
```
Then the `nu0_duo_engine.tsv` rows (`owner` = `DU1`) of the functions named below. Verify a digest line in Ghidra before building on it.

## Task

Part 2 of DU1 (split by DU1, 2026-11-14). DU1 landed the members, accessors, ctor / dtor, `InitMotion`'s second body and the anim
play / stop / one-shot resets. Port whole, inside March's `CX2GUUser` functions under the flag:

- **Switching:** `SetActiveBody` `3204e030` (after-image off / on, `UpdateDuo(false)`, the unit's form setter `3234b8c0`: DU5 owns its
  inventory refresh, so add only the store here or a one-line stand-in, the hand scale `31fde6d0` when buff 0xc6 is on, `+0x84dc` /
  `+0x84de`), `SetDuoEnabled` `32092a00`, `UpdateDuo` `3204df80` (type-10 gate; the `+0x8f6e` / `+0x8f6f` pair is CH1a0's `31fc9d50` /
  `31fc9d90`: declare the two bytes here if CH1a0 has not; `31fd6260` the equip-view show / hide loop; vslots 0x638 / 0x2c0 / 0x330:
  resolve them with `june2017_re/vtslot.py`; `UpdateSocketDataAndEnchantData`; the skill-tree command group `3234d930` ->
  `CX2UISkillTreeNew::SetCommandGroup`). Unit creation `32d51e80` calls `SetActiveBody` from the form byte.
- **State start:** `CommonStateStart` Future `32050230` / Now `32057b80`: `DUO_TYPE` (miss = 3; 3 off -> duo word 0 + `UpdateDuo(true)`,
  0 / 1 body, 2 both; `+0x8acc`), `+0x8e92` end-duo-at-anim-end, `+0x8e93` latch, `CUSTOM_OFFSET_NOT_SELECT_UNIT` (`+0x8f54` / `+0x8f58`),
  `USE_NOT_SELECT_MOTION_DIFFERENCE` (`+0x8f5c`, miss = 1), `USE_NOT_SELECT_MOTION_BEFORE_POS` (`+0x8f5d`, copies `+0x7e48` to `+0x8f60`),
  and settle `USE_PHYSICPROCESS_NOT_SELECT_UNIT` / `USE_NOT_SELECT_UNIT_ANIM_TIME_IN_SPEED` (`+0x8f50` candidate) from the listing.
- **Frame move / end:** `CommonFrameMoveFuture` `32061bf0` and `CommonFrameMove` `32063650` (second body clock, `SetPosition` of the
  not-selected body from `GetNotSelectXSkinAnimPtr()` at the Now second position, the duo end at anim end), `CommonStateEnd` `32015250`.
- **Physics:** `PhysicProcess` `32015b30 @32019f9b-3201a1c7` (the not-selected body's position: direction * `+0x8f54`, y `+0x8f58`, the
  rotated delta when `+0x8f5c`) and the four `+0x8f5d` tests (`@32019166`, `@3201946d`, `@32019e25`, the `+0x8f60` leas). The second
  position members (`+0x7e48..`, `+0x80e8..`) are SyncData fields: add them to `SyncData` here (DU1c adds the wire bits and converters).

Every other class has form 0 and `DUO_TYPE` absent from its scripts: prove no change with a `/Zs` probe of `X2GUUser.cpp` and one class
file. No play-test of its own (nothing reaches it before CH1): Done note says "full build skipped: no play-test".

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
