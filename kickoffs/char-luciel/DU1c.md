# June character Lu/Ciel (Stage 8) - Phase DU1c - The two-body user sync and the per-body readers outside the class

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU1c` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU1b4 |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | none of its own: `kickoffs/char-luciel/DU1-review.md` covers DU1, DU1b, DU1c, DU1d |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Phase DU - The two-body engine (DU1-DU5)" "DU1 - Done (2026-11-14)" "DU1b - Done"
python tools/docsection.py kickoffs/char-luciel/DU1.md "Amended by NU0" "Amended by CH0 (2026-11-02)"
```
Then the `nu0_duo_engine.tsv` rows of the functions named below, and funcmap's Effect C3e rows (`grep "C3e" june2017_re/funcmap.tsv`).

## Task

Part 3 of DU1 (split by DU1, 2026-11-14). Port whole under the flag:

- **User sync** (c3e rows B, E, F, G, I; H is Ain's): `KDYNAMIC_UNIT_USER_SYNC::Serialize` `32d5c2e0` / `Deserialize` `32d5be30` (bits 0x10000
  float, 0x20000-0x80000 second position x half / y float / z half, 0x100000 class short, 0x200000 class-skill float, 0x1000000 active
  body word), `SendFrameData` `3203d4d0` (second position x / z encoded against the line index), `SyncData::SetData` `3201f5f0`, the
  protected <-> plain converters `32071ea0` / `320829b0`, the active-body writer `31fe9e40`. The packet struct is shared code: the
  CommonPacket.h exception in CLAUDE.md applies if the struct lives there (second `#define` in `ServerDefine.h`, `SECOND_HOMES`); keep the
  emulator consistent if it touches the struct.
- **Per-body protected float pairs** `+0x88c8` / `+0x88d4` picked by the active body: `AttackResult` `32012b50`, `CanApplyBuffToGameUnit`
  `32045dc0`, `CX2DamageEffect::CEffect::AttackResult` `31f85910`.
- **Readers outside `CX2GUUser`:** `CX2Game::ProcessExpListByNpcUnitDie` `322f8920`, the game loop `3228c540`, the form-byte weapon-effect
  bones `X2GameUnit.cpp:11909` (`3228c540`) / `:19465` (`32292610`), `CX2GameUnit::GetStat` `328842f0` and the socket / item readers of
  the unit form byte (`32365d30`, `3235a160`, `328836c0`) where they are not DU3 / DU5 rows.

Every other class has form 0: prove no change with `/Zs` probes. No play-test of its own: "full build skipped: no play-test".

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

## Amended by DU1b (2026-11-14)

- Order is now DU1b -> DU1b2 -> DU1b3 -> DU1c (DU1b split again). DU1b2 adds the second position to `SyncData`; this phase adds its wire bits.
- **`InitStat` per form:** June's `CX2GUUser::InitStat` (user vslot 0x330, `31fe5480`, uncarved) takes the game stat from the unit by the
  active body (`3234af90( +0x8e90 == 1 )` on unit `+0x28`), March's from `m_GameStat`. `UpdateDuo` already calls `InitStat()` with a stand-in
  comment: replace March's body with June's here (a `june_march_divergent.tsv` row in the same commit) together with the other unit form readers.

## Amended by DU1b2 (2026-11-14)

- `SyncData::vNotSelectPosition` exists (Future +0x7e48 / Now +0x80e8, zeroed in `Init`, copied with the struct): wire it in `Serialize` /
  `Deserialize` / `SetData` / `SendFrameData` (bits 0x20000-0x80000).
- **The synced active-body word** (bit 0x1000000, Future +0x7f5c / Now +0x81fc, protected short): its Future writer is also in
  `CommonFrameMoveFuture` `@32063316` (`= ( +0x8e90 == 1 )`, beside the funcmap's `31fe9e40`), and its Now reader is the sync pop after
  `CommonFrameMove` (starts ~`3206cfe0`, `ret 8` `@3206d473`): for others' units of type 10, `@3206d42d-3206d45a` calls `SetActiveBody( word == 1 )`
  when it differs. Both come with the field here (funcmap rows `32061bf0 @32063316 (DU1b2)`, `3206d42d (DU1b2)`).

## Amended by DU1b3 (2026-11-14)

- Order is now DU1b3 -> DU1b4 -> DU1c (DU1b3 split again). `PhysicProcess` now writes `m_FrameDataFuture.syncData.vNotSelectPosition` every
  frame (the first body's position, or the kept `m_vNotSelectMotionBeforePos`, plus the offset): the wire bits carry that value.
