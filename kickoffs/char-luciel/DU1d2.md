# June character Lu/Ciel (Stage 8) - Phase DU1d2 - The bone / hand / emotion second-body readers, then the regression play-test

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU1d2` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU1d |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | none of its own: `kickoffs/char-luciel/DU1-review.md` covers DU1-DU1d2 |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU1b - Done" "DU1c2 - Done" "DU1d - Done"
python tools/docsection.py kickoffs/char-luciel/DU1d.md "Amended by DU1b (2026-11-14)" "Amended by DU1b4 (2026-11-14)" "Amended by DU1c (2026-11-14)" "Amended by DU1c2 (2026-11-14)"
```
Then `june2017_re/char_luciel/du1d_body_reads.tsv`: every row with `owner` = `DU1d2` (63 June functions; the `sites` column lists each inline
row and each out-of-line getter call: `32071d80` = `GetXSkinAnimPtr()`, `32070cf0` = `GetXSkinAnimFuture()`, `32071db0` =
`GetNotSelectXSkinAnimPtr()`, `32071d40` = `GetMinorUnitXSkinAnimPtr()`). DU1d's scratch script for the edits was a generator over
(line range, June address, Future / Now) that wraps each March line in `#ifdef` / `#else`; write your own the same way.

## Task

Part 9 of DU1 (split by DU1d, 2026-11-14), the rest of the base-function readers in `X2GUUser.cpp`, inside March's `CX2GUUser` functions
under the flag, March's line in `#else`:

- **Bones and hands (the DU1d kickoff's second bullet):** `ScaleHead` `31fd64a0` (`iBody`), `RotateBone` `31fd93e0`, `GetBonePos` `31fda680`
  (form) and **`31fda280`** (`GetBonePos( const WCHAR* )`, vslot 0x84: strips `MAJOR:` / `MINOR:` and picks Lu's `+0x87d0` or Ciel's `+0x8e88`;
  DU1c2's type-10 particles call it with `L"MAJOR:Bone_RFinger2"`), the feet `31fdc0e0` / `31fdc310`, `31fde420` / `31fde6d0` / `31fded80`,
  `GetHeadBonePos` `31fcac20` (`+0x4218`), `320200b0`, `31fd9200` (the `MAJOR_` / `MINOR_` bone stand-in at `X2GUUser.cpp:16076`), the marker /
  hold particles `31fee682`, `ProcessEnchantedWeaponEffectAtHand` `31ff8510`, `SetUnitReForm`'s second skin `31fda0d0` (the live `UT_LUCIEL:
  break` stand-in), `PlaySoundRevive` `31fdb390` (`LuVoice_` / `CielVoice_Revival.ogg` by the active body), `InitSocketData` slot 484
  `3206d770`, `HasSocketAuraFor` `3204e380`, `UpdateEquipCollisionData` `31fd8c00`, `SetSkillLevelStateData` `3201f850`, `PlayEmotion`
  `32043380`, `EmotionFrameMove` `3204a860`. The stand-in comments DU1-DU1c2 and older phases left name most sites
  (`grep -n "stand-in: Lu/Ciel not ported\|second body" X2Lib/X2GUUser.cpp`).
- **The second after-image (DU1b amendment):** unit `+0x6758` (created at `31feed3d` in `31fee682`, on `+0x8e80`), `EnableAfterImage( mode )` /
  `DisableAfterImage( bForce, mode )` (`32275a30` / `322720f0`: 0 first, 1 second, 3 = unit vslot 152 `3208e160` = the active body), the
  selector `3208e530`, `MINOR_UNIT_AFTER_IMAGE` (`@3205b69b`, table `+0x852c`); widen `SetActiveBody`'s after-image toggle (DU1b stand-in).
- **The out-of-line getter callers:** the census's other `DU1d2` rows (render prepare `31fd07b0`, DIE_FRONT / DIE_BACK starts, active-skill
  show, `CommonRender` / custom render, `StartIntrudeFrameMove`, `DamageAirDownLandingFrameMove`, `DoAimingTarget`, `DamageDataChangeProcess`,
  `CanApplyBuffToGameUnit`, `COMMON_RELATIONSHIP_SKILL_FrameMove`, `ShowHyperSkillCutInByScript`, `InitEqip`, `Init` `3204ea20`, and the
  `CommonStateStart` `@32060bb3` / `CommonFrameMove` calls DU1b2's Done note does not list). For each: a March function -> June's getter at
  exactly June's call; "none in March" -> say whose phase it is.
- **The minor passive resets (DU1b4 amendment):** June's twins of March's `m_PhysicParam.passiveSpeed = D3DXVECTOR2( -1, -1 )` sites
  (`grep -n "passiveSpeed\s*= D3DXVECTOR2\|INIT_VECTOR2( m_PhysicParam.passiveSpeed" X2Lib/X2GUUser.cpp`) for a `+0x378 / +0x37c` reset.

Split again by the kickoff's own rule if it does not fit one session; the **last** part closes the DU1 series: a full build and the regression
play-test the DU1 kickoff asked for (an existing character's login / village / dungeon round-trip with no new log line), covering DU1b4's
untimed SPEED_TIME entries (walk, dash, jump and a few skills on one March class), DU1c's MP-on-hit cash ability (a SAT 12 item such as the
Magic Necklace 200680, equip / unequip in the village) and `InitStat` (HP / MP / attack unchanged at dungeon start), DU1c2's weapon glow and
attribute sparks (a +11 / +13 weapon, an attribute-enchanted weapon) and the village HUD's max HP / MP, and DU1d's cancel macros (combo and
skill cancels by arrow / walk / dash / skill key on two March classes). Then `DU1-review.md` runs.

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
