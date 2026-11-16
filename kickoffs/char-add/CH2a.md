# June character Add (Stage 7) - Phase CH2a - Skills: Nasod Ruler (9)

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase char-add CH2a` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | CH1 review, CH2b0 review (Particle Prism 29003 / 29029 summons NPC 1576 / 1577). One at a time with CH1-CH4: they all edit `X2GUAdd.*` |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_SKILL` (+ `..._SKILL_DEBUG`) |
| Log tag | `[JUNE_CHAR]` (`ADD SKILL`), `[JUNE_SKILL]` (`--grep NUM`) |
| Review | none of its own: the CH2d review covers it (its `_DEBUG` define stays on) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Rules" "Starting context" "Phase CH2 - Skills: Nasod Ruler and the first jobs (classes 9, 32, 33, 36)" "Phase CH3 - Skills: second jobs (122, 123, 126), hyper actives, transcendence"
python tools/docsection.py JUNE2017_CHARACTER_PORT_CHECKLIST.md "1. Blocker: June damage numbers come from a table this build does not have"
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "CH2a - Done"   # once written; else Ara's
```

## Task

Scope: the skill ids class 9 has (a skill's line decides its phase). For each id: its case in `SetEquippedSkillFuncBySkillSlotIndex` with June's state functions whole (Ghidra, the map's addresses), inside
March's case under `SERV_IRUHADEV_JUNE_CHAR_ADD_SKILL` (March's in `#else`) or a new case; its attached rows (7u names, 7ai BFI arguments, 7h UPDOWN
cancels, §7 l.256 power-rate operator, the numerical-only rows of `plan_numerical_rows.tsv`, the effect-modifier tags 7a l.306 / l.311 and alpha tags
7c l.356, `USEFUL_VALUE` readers l.427, `GetLandPosition( ..., true )` l.680, the `CanDown( NULL, &idx )` shape l.781); its effect closure (a name
found only now is staged byte-exact and added to `ch0_assets.tsv`); its DP gain / cost on CH1q's code. Upgrade ids reach their base through the
templet's `m_iDeleteSkillIDByUpgrade` (June `32071720`): check the templet swap for every term that reads the id. A function the map does not settle
(new mechanism, unmapped callee) is "this needs Opus": stop.
- `NUM use` lines for these skills; March vs June damage per skill in `MODS.md`.
- CP949: `bytepatch.py` only. Mark the ported checklist rows `(Add: <phase>)`.
- Hand-off: the unit and class, each skill id and its A / B slot (slots 0-3 / 5-8, never 4 / 9 unless the check is about transcendence; slot every
  tested skill yourself, never ask the user to swap), each cast twice (cool time, MP or DP), what June's motion and effect look like.

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

- Class map: **34** functions (3 listing-only; 30 `inside`, 4 `new`), filter `awk -F'\t' '$7=="CH2a"' june2017_re/char_add/ch0_class_map.tsv` (col 7 phase, col 5 inside / new, col 6 proposed name, col 9 carved y / n).
- Script states: 11 (`june2017_re/char_add/ch0_state_skill.tsv` part 1, col 8).
- Skill ids: 14: 29000 RNSI_SA_AN_ENERGY_BOOM, 29001 RNSI_SA_AN_VOID_BREAKER, 29002 RNSI_SA_AN_PARTICLE_ACCELERATOR, 29003 RNSI_SA_AN_PARTICLE_PRISM, 29004 RNSI_SA_AN_NEUTRON_BOMB, 29026 RNSI_SA_AN_ENERGY_BOOM_UPGRADE, 29027 RNSI_SA_AN_VOID_BREAKER_UPGRADE, 29028 RNSI_SA_AN_PARTICLE_ACCELERATOR_UPGRADE, 29029 RNSI_SA_AN_PARTICLE_PRISM_UPGRADE, 29030 RNSI_SA_AN_NEUTRON_BOMB_UPGRADE, 29201 RNSI_A_AN_PHASE_SHIFT, 29202 RNSI_A_AN_PULSE_BULLET, 29216 RNSI_A_AN_PHASE_SHIFT_UPGRADE, 29217 RNSI_A_AN_PULSE_BULLET_UPGRADE.
- Checklist rows (`june2017_re/char_add/ch0_checklist_rows.tsv` col 5): L309, L314, L359, L689, L733, L789, L117, L178, L727, L787, L865.
- Skill ids and their states: `awk -F'\t' '$6=="CH2a"' june2017_re/char_add/ch0_state_skill.tsv` (part 2); the state functions: `awk -F'\t' '$7=="CH2a"' june2017_re/char_add/ch0_class_map.tsv`.
- ( CH1d0, 2026-11-16 ) The engine helpers CH2a calls, written by CH1d0: `32294530` = `CX2GameUnit::GetNearestPosOnLineMap( vPos, bool )` ( static; Phase Shift Blink passes `false`: on a line it returns `vPos` unchanged in practice, JUNE_BUGS 780; off any line it walks 10 from `vPos` along the nearest line ); `31f63d40` = `CX2DamageEffect::CEffect::SetLockOnOnlyFront( bIsRight, iLastTouchLineIndex, vOwnerPos )` ( sets `m_bIsOnlyFront` too ); `31edf5a0` = `g_pData->IsUpgradedSkill( skill )`. The Blink start then stores the result with `31fc9430` = `CX2GUUser::SetSyncDataPos( vPos, bFuture )`.
