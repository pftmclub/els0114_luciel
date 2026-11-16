# June character Add (Stage 7) - Phase CH4a - Passives, the delegate lists and the anim-rate float

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-add CH4a` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | CH3d review. One at a time with CH1-CH4: they all edit `X2GUAdd.*` |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_PASSIVE` (+ `..._PASSIVE_DEBUG`) |
| Log tag | `[JUNE_CHAR]` (`ADD PASSIVE`) |
| Review | none of its own: the CH4b review covers it (its `_DEBUG` define stays on) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Rules" "Phase CH4 - Passives and class mechanics"
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "CH4a - Done"   # once written; else Ara's
awk -F'\t' '$7=="CH4a"' june2017_re/char_add/ch0_class_map.tsv
```

## Task

Do "CH4a": `InitPassiveSkillState` (March `X2GUAdd.cpp:385`) and his passive ids (June `RNSI_P_*`), their objects and readers; the delegate
registrations `32a1ccb0` (A `32a04ce0`, `329d4190`, `32a01d20`; D `32a01dd0`) replacing March's own pushes (checklist l.924 / l.932); BBT 92
`CHANGE_MAXIMIZE` with `+0x9680` (l.251; if the buff port's Phase 5c has not landed, route it); the user float `+0x92a0` his passives add (l.1053,
vslot 0x824, divergent row 401 `ResetAnimSpeed`); the passive-id check of the Rules. Play-test: each passive's effect shows (CH0 names the stat or log
line per passive).

You are on Opus: delegate the reads, greps, known-address Ghidra dumps, probe compiles and flagcheck runs (x2-phase, *When the phase is Opus*). CP949 sources: `bytepatch.py` only.

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
Phase tag (user, 2026-11-09): every edited or new code block carries a one-line `// char-add <phase>` comment (at the start of an existing one-line comment, never a second line).

## Amended by CH0 (2026-11-13)

- Class map: **14** functions (4 listing-only; 1 `inside`, 13 `new`), filter `awk -F'\t' '$7=="CH4a"' june2017_re/char_add/ch0_class_map.tsv` (col 7 phase, col 5 inside / new, col 6 proposed name, col 9 carved y / n).
- Skill ids: 22: 106 RNSI_P_COMMON_HYPER_AURA, 29400 RNSI_P_APT_NASOD_ARMOR_MODE, 29401 RNSI_P_APT_DUST_STORM, 29402 RNSI_P_ALP_BODY_OF_TRANSCENDENCE, 29403 RNSI_P_ALP_POWER_EXCHANGER, 29404 RNSI_P_ALP_FANTASY_TRACER, 29405 RNSI_P_AAT_NASOD_ARMOR_MODE, 29406 RNSI_P_AAT_HIDDEN_MEMORY, 29407 RNSI_P_AMM_ENHANCED_DYNAMO_FACTORY_TRANSFORM, 29408 RNSI_P_AMM_NEW_UPDATE, 29409 RNSI_P_AMM_TRANSFORM_OFENCE_MODE, 29410 RNSI_P_ATT_NASOD_ARMOR_MODE, 29411 RNSI_P_ADE_CONTROL_TIME, 29412 RNSI_P_ADE_MIND_CONTROL, 29413 RNSI_P_ADE_MIND_CIRCLE, 29414 RNSI_P_ADE_ESP, 29415 RNSI_P_ALP_TRANSCENDENCE, 29416 RNSI_P_ALP_ENERGY_AMPLIFY, 29417 RNSI_P_AMM_TRANSCENDENCE, 29418 RNSI_P_AMM_ENHANCED_ARTIFICIAL_INTELLIGENCE, 29419 RNSI_P_ADE_TRANSCENDENCE, 29420 RNSI_P_ADE_TIME_TROUBLE.
- Checklist rows (`june2017_re/char_add/ch0_checklist_rows.tsv` col 5): L254, L936, L1068.
- 14 functions incl. InitPassiveSkillState `32a1ccb0` (0x690) and the 4 delegate adapters `32a2b340`, `32a309a0`, `32a309b0`, `32a309c0` (delegates `329d4190`, `32a04ce0`, `32a01d20`, `32a01dd0`); 22 passive skill ids with no state. `june2017_re/char_add/ch0_passive_ids.tsv`: 36 `SI_*` reads, 25 differ (9xxx -> 29xxx); spelling `SI_SA_APT_CONQUEROR` vs June `RNSI_SA_APT_CONQUERER`; `SI_SA_AHM_CHAIN_LIGHTNING` (an Aisha id) is read; `SI_A_AN_PULSE_BULLET`, `SI_HA_ALP_DOOMS_DAY`, `SI_P_AN_CHARGED_IMPULSAR` are not live in the `.i`. `+0x92a0` (L1068).

## Added by CH1d (2026-11-16)

- CH1d added `m_bEnableNasodArmorModeOverlimit` ( June `+0x98f8` ) and `m_mapTranscendedPassiveAbilityValue` ( `+0x9668`, skill id -> `SA_*` -> float, read by `GetTranscendedPassiveAbilityValue` `329e6f80` only when the unit is transcended ): InitPassiveSkillState `32a1ccb0` sets the flag for `RNSI_P_ATT_NASOD_ARMOR_MODE` ( @`32a1e18b` ) and `32a30680` fills the map ( first reader: the nasod arms' mutation roll for `RNSI_P_ADE_TRANSCENDENCE`, `SA_EFFECTIVE_RATE` ).
