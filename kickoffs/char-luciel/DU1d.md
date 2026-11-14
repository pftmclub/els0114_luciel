# June character Lu/Ciel (Stage 8) - Phase DU1d - The base-function second-body readers, then the regression play-test

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU1d` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU1c2 |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | `kickoffs/char-luciel/DU1-review.md` (covers DU1, DU1b, DU1c, DU1d) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Phase DU - The two-body engine (DU1-DU5)" "DU1 - Done (2026-11-14)" "DU1b - Done" "DU1c - Done"
python tools/docsection.py kickoffs/char-luciel/DU1.md "Amended by NU0" "Amended by CH0 (2026-11-02)"
```
Then every `nu0_duo_engine.tsv` row with `owner` = `DU1` that DU1, DU1b and DU1c have not covered (their Done notes list what they took).

## Task

Part 4 of DU1 (split by DU1, 2026-11-14), the rest of the DU1 rows, inside March's `CX2GUUser` functions under the flag:

- **The inline-accessor sites in `X2GUUser.cpp`:** where June reads `+0x8e90 ? +0x8e84 : +0x87d0` (or the Now pair) inside a base function
  (`THROW_ITEM_EventProcess`, `COMMON_BUFF_EventProcess`, `StartIntrudeEventProcess`, the damage / stand-up / revenge / hyper event processes,
  `GenericGroundComboEndEventProcess`, the generic active / special-active event processes, `COMMON_RELATIONSHIP_SKILL_EventProcess`,
  `ResetAnimSpeed`, `Send_SKILL_USE_REQ`, `ReInit`, `CommonEventProcess`, the SNIPER / PROTON handlers `3200d1d0` / `3200e040`,
  `THROW_ITEM_FrameMove` `31ff92d0`, ...), March reads `m_pXSkinAnimFuture` / `m_pXSkinAnim` directly: write June's accessor
  (`GetXSkinAnimFuture()` / `GetXSkinAnimPtr()`) at exactly June's sites, March's line in `#else`. Other class files are not touched:
  every other class has form 0 (NU0: the 2,248 accessor rows need no per-site edit).
- **Bones and hands:** `ScaleHead` `31fd64a0` (`iBody`), `RotateBone` `31fd93e0`, `GetBonePos` `31fda680` (form), the feet `31fdc0e0` /
  `31fdc310`, `31fde420` / `31fde6d0` / `31fded80`, `GetHeadBonePos` `31fcac20` (`+0x4218`), `320200b0`, the marker / hold particles
  `31fee682`, `ProcessEnchantedWeaponEffectAtHand` `31ff8510`, `SetUnitReForm`'s second skin `31fda0d0` (the live `UT_LUCIEL: break`
  stand-in), `PlaySoundRevive` `31fdb390` (`LuVoice_` / `CielVoice_Revival.ogg` by the active body), `InitSocketData` slot 484 `3206d770`,
  `HasSocketAuraFor` `3204e380`, `UpdateEquipCollisionData` `31fd8c00`, `SetSkillLevelStateData` `3201f850`, `PlayEmotion` `32043380`.

Close-out of the DU1 series: a full build and the regression play-test the DU1 kickoff asked for (an existing character's
login / village / dungeon round-trip with no new log line). Then `DU1-review.md` runs.

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

- **The second after-image** (unit `+0x6758`, created at `31feed3d` in `31fee682`, on skin anim `+0x8e80`) and the after-image mode June's
  `EnableAfterImage( mode )` / `DisableAfterImage( bForce, mode )` take (`32275a30` / `322720f0`: 0 first, 1 second, 3 = unit vslot 152 =
  the active body). `SetActiveBody` (DU1b) tests and toggles only `m_pAfterImage` with a stand-in comment: widen it here. `MINOR_UNIT_AFTER_IMAGE`
  (`@3205b69b`, table `+0x852c`) comes with it.

## Amended by DU1b4 (2026-11-14)

- The minor PhysicParam's speeds are now fed (SPEED / PASSIVE_SPEED keys by active body, `m_vecMinorUnitSpeedFactor`, the frame-move minor pass).
  Among the base-function readers, check June's twins of March's `m_PhysicParam.passiveSpeed = D3DXVECTOR2( -1, -1 )` sites in `X2GUUser.cpp`
  (`grep -n "passiveSpeed\s*= D3DXVECTOR2\|INIT_VECTOR2( m_PhysicParam.passiveSpeed" X2Lib/X2GUUser.cpp`) for a `+0x378 / +0x37c` reset.
- The regression play-test now also covers `SERV_IRUHADEV_JUNE_DIVERGE_COMMON_STATE_END_FUTURE` (every class: untimed SPEED_TIME entries
  apply at state end): walk, dash, jump and a few skills on one March class, then "check logs" for nothing new.

## Amended by DU1c (2026-11-14)

- DU1c split again: **DU1c2** (the readers outside `CX2GUUser`: weapon-effect type-10 cases, `GetStat`, the socket form readers) now sits
  between DU1c and DU1d. DU1d depends on DU1c2.
- The regression play-test also covers DU1c's every-class lines: the MP-on-hit cash ability (a SAT 12 item such as the Magic Necklace 200680
  still gives MP on hit and on damage), equip / unequip of that item in the village, and `InitStat` (HP / MP / attack unchanged at dungeon start).

## Amended by DU1c2 (2026-11-14)

- **Add `31fda280` to the bones:** `CX2GUUser::GetBonePos( const WCHAR* )` (user vslot 0x84, the one returning the vector; `31fda680` is vslot
  0x80). June strips a `MAJOR:` / `MINOR:` prefix and takes the bone from Lu's (+0x87d0) or Ciel's (+0x8e88) anim. DU1c2's type-10 weapon
  particles call `GetBonePos( L"MAJOR:Bone_RFinger2" )` / `L"MAJOR:Bone_LFinger2"`, which March's GetBonePos does not parse yet. DU3's kickoff
  lists `31fda280` as "the equip dump" (wrong label); it belongs here with the other bones.
- `ProcessEnchantedWeaponEffectAtHand` `31ff8510` (the type-10 hand sparks) stays here: DU1c2 left it, being an `X2GUUser.cpp` function.
- The regression play-test also covers DU1c2's every-class lines: a +11 / +13 weapon still glows and an attribute-enchanted weapon still
  sparks on a March class; the village HUD's max HP / MP are unchanged.
