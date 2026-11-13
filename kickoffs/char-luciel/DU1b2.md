# June character Lu/Ciel (Stage 8) - Phase DU1b2 - Ciel's body every frame

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU1b2` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU1b (done 2026-11-14) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (same flag: one logical change, now in six parts) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | none of its own: `kickoffs/char-luciel/DU1-review.md` covers DU1-DU1d |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU1 - Done (2026-11-14)" "DU1b - Done (2026-11-14)"
grep "Char Luciel DU1" june2017_re/funcmap.tsv
```
Then the `nu0_duo_engine.tsv` rows (`owner` = `DU1`) of `32061bf0` and `32063650`. June's `CommonFrameMove` (`32063650`) does not decompile in
Ghidra (timeout): read it with `june2017_re/disasm_range.py 32063650 3206d600`; `CommonFrameMoveFuture` (`32061bf0`) decompiles.

## Task

Split from DU1b (2026-11-14). Port whole, inside March's `CommonFrameMoveFuture` / `CommonFrameMove` under the flag:

- **The second position in `SyncData`:** Future `+0x7e48..+0x7e50`, Now `+0x80e8..+0x80f0` (a `D3DXVECTOR3` after `position`; the
  `FrameData` pair is 0x2a0 apart). Add the field and its copy / init; DU1c adds the wire bits and converters, DU1b3 writes it in physics.
- **Now frame move (`32063650`):** the active body and the not-selected body moved each frame (`@3206538c-32065532`: the riding saddle
  branch moves both to the saddle; otherwise the active body (`32071d80`) to the Now position and the other (`32071db0`) to the Now second
  position `+0x80e8`), both rotated (`31cfdcd0`, `@32065547-320656cf`), the second body's clock (anim vfunc 4 with the elapsed time,
  `@32065712-3206573e`), `RotateBone` on both (`@3206573e-320657e2`), the equip loop picking `+0x8e88` for an equip whose minor-unit byte
  `+0x15d4` is 1 (`@3206585b`; the byte is DU3's: same stand-in as DU1b), and the `+0x8e90` reads at `@3206722c`, `@32067289`, `@32067900`,
  `@320691ff`, `@3206c239`, `@3206d44b`.
- **Future frame move (`32061bf0`):** the active-body anim-time reads (decompile lines with `param_1[0x23a4]` picking `+0x87d0` /
  `+0x8e84`: the speed-factor time, the `+0x84f4` window), the Future twins of the moves / clock above (`@32062419`, `@32062601`,
  `@32062961-32062da8`). `USE_NOT_SELECT_UNIT_ANIM_TIME_IN_SPEED` (`+0x8f6d`) and the `+0x8604` speed-time twin are DU1b3's: leave their
  branches to it (say where in the Done note).

Every other class has form 0 and no duo: prove no change with `/Zs` probes of `X2GUUser.cpp` and one class file. No play-test of its own:
the Done note says "full build skipped: no play-test".

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
