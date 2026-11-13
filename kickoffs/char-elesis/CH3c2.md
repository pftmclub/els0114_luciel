# June character Elesis (Stage 7) - Phase CH3c2 - Skills: Crimson Avenger Shadow Edge (Opus)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH3c2` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH3c (done 2026-11-14). One at a time with CH1-CH4: they all edit `X2GUEL.*` |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL` and `SERV_IRUHADEV_JUNE_CHAR_ELESIS_BLOOD` for the record type (+ `..._SKILL_DEBUG`) |
| Log tag | `[JUNE_CHAR]` (`ELESIS SKILL`), `[JUNE_SKILL]` (`--grep NUM`) |
| Review | none of its own: the CH3d review covers it (its `_DEBUG` define stays on) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md --no-sub "Rules" "Phase CH3 - Skills: second jobs (120, 121, 125), summons, hyper actives, transcendence"
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "CH3c - Done"
awk -F'\t' '$1 ~ /^32b8(1220|1300|13d0)/' june2017_re/char_elesis/ch0_blood.tsv
grep -n "^32b84390\|^322d7b30\|^31f75980\|^32b813d0\|^32b81300" june2017_re/funcmap.tsv
```

## Task

CH3c ported the other Crimson Avenger actives and stopped on **Shadow Edge** (28215 / 28235, `RNSI_A_SCA_SHADOW_EDGE`, not yet a C++ name: add it and `_UPGRADE` to
`X2SkillTree.h` under `JUNE_ENUM_NAMES`; values in June's `Enum.lua`). Its state functions (`charport_adis.py elesis 32b84390`, Ghidra `FUN_32b84390`):
- `32b520f0` Init (preloads: `BREAKING_FIST_Attack_Box.x`, `WhiteImage.dds`, `SDK_BURNING_PHANTOM_Storm001.Y`), `32b6a340` EventProcess (the Shade Scratch attack template; diffed in CH3c).
- `32b84390` **FrameMove**: at 0.31 s ( `EventTimerOneshot` ) it clears the `+0x9700` record list ( `32b81300` each, then erase ), then up to 3 times asks `CX2Game` `322d7b30`
  ( nearest valid enemy outside a (UID) set, range 700 in PvP / PvE else 1200, candidate row in `funcmap.tsv` ) for a target, makes `DamageEffect_SCA_SHADOW_EDGE_Projectile_EXPLOSION`
  and `DAMAGE_EFFECT_SCA_SHADOW_EDGE_Link` ( aimed with `31f75980` from her hip + 25 up to the target, 99999 ), pushes a 0x20-byte record ( target UID, power rate = max-HP-share x skill ability, user flag, the Link effect's handle )
  on `+0x9700` ( vector helpers `32b90160` / `32b902c0` / `32b904c0` / `32b90540` are compiler `push_back` ) and puts the skill's buff factor ( level of 28215, cloned with `32089f60` ) on the target through `SetBuffFactorToGameUnit`.
- The record type, `RemoveShadowEdgeRecord` `32b81300` ( `BTI_DEBUFF_SHADOW_EDGE` 432 erase, destroy the Link handle ) and the per-frame upkeep `32b813d0` are CH4c's ( `_BLOOD`, declared in `X2GUEl.h` as
  `ClearShadowEdgeRecords` / `ShadowEdgeRecordFrameMove` ). Decide here whether CH3c2 writes the record struct, the list and those two bodies now ( recommended: they are Shadow Edge's own mark ) and tell CH4c.
- Check `322d7b30` and `31f75980` first: char-luciel's CH2d2 / CH1l3 use them ( "Opus helper phase" rows ); reuse the peer's port if it landed, else port them under their own flag.
- Registration: `32b8c470` @1226 cases 28215 / 28235 ( Init + FrameMove + EventProcess; hyper = FrameMove + EventProcess ); the equipped-skill-level block, if any, from `ch0_setskilllevel_blocks.tsv`.
- Gates as CH3c ( flagcheck, junediverge, enumscan, stateends, luabind, skillicons `28215-28236` ). Play-test unit 146 `CAvenger80`: Shadow Edge 28215 in a slot, a dungeon with 3 enemies.

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
Phase tag (user, 2026-11-09): every edited or new code block carries a one-line `// char-elesis CH3c2` comment (at the start of an existing one-line comment, never a second line).
