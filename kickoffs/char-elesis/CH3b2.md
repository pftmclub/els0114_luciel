# June character Elesis (Stage 7) - Phase CH3b2 - Skills: Blazing Heart (121), actives 28210 / 28211 / 28223

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase char-elesis CH3b2` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH3b (CH0 split, 2026-11-12). One at a time with CH1-CH4: they all edit `X2GUEL.*` |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL` (+ `..._SKILL_DEBUG`) |
| Log tag | `[JUNE_CHAR]` (`ELESIS SKILL`), `[JUNE_SKILL]` (`--grep NUM`) |
| Review | none of its own: the CH3d review covers it (its `_DEBUG` define stays on) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md --no-sub "Rules" "Starting context" "Phase CH2 - Skills: Knight and the first jobs (classes 8, 29, 30, 35)" "Phase CH3 - Skills: second jobs (120, 121, 125), summons, hyper actives, transcendence"
python tools/docsection.py JUNE2017_CHARACTER_PORT_CHECKLIST.md "1. Blocker: June damage numbers come from a table this build does not have"
python tools/docsection.py JUNE_CHARACTER_ARA_PHASES.md "CH3b - Done"   # the worked example
awk -F'\t' '$7=="CH3b2"' june2017_re/char_elesis/ch0_class_map.tsv   # CH0 confirms the column
```

## Task

Scope: the ids class 121 has and its first-job line does not (a skill's line decides its phase). For each id: its case in `SetEquippedSkillFuncBySkillSlotIndex` (`32b8c470`; CH0's
switch map names the case) with June's state functions whole (Ghidra and, for `listing-only` rows, `charport_adis.py elesis <va>`), inside March's
case under `SERV_IRUHADEV_JUNE_CHAR_ELESIS_SKILL` (March's in `#else`) or a new case; its block in `SetEquippedSkillLevel` `32b4e790` if it has one; its attached rows (7u names,
7ai BFI arguments, 7h UPDOWN cancels, §7 l.256 power-rate operator, the numerical-only tables of `plan_numerical_rows.tsv`); its effect closure
(a name found only now is staged byte-exact and added to `ch0_assets.tsv`); its `WAY_OF_SWORD_TYPE` on CH1q's code. Upgrade ids reach their base
through the templet's `m_iDeleteSkillIDByUpgrade` (June `32071720`): check the templet swap for every term that reads the id. A skill whose factor
names a BFI id June's `Enum.lua` lacks (`JUNE_BUGS` 279) keeps June's nil. A function the map does not settle (new mechanism, unmapped callee, a
listing the control flow of which is not plain) is "this needs Opus": stop. Wall of Prominence and Fire Blossoms spawn through CH3b0's code; Soul Ignition's `CREATE_EFFECT_SET` rows play at the rate CH3b0's writers set: the hand-off lists the `ES_CREATE` and NPC state lines the test must produce.
- `NUM use` lines for these skills; March vs June damage per skill in `MODS.md`.
- CP949: `bytepatch.py` only. Mark the ported checklist rows `(Elesis: <phase>)`.
- Hand-off: the unit and class, each skill id and its A / B slot (never slot 4 / 9 before transcendence; learn only the bank under test), each
  cast twice (cool time, MP), what June's motion and effect look like.

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
Phase tag (user, 2026-11-09): every edited or new code block carries a one-line `// char-elesis <phase>` comment (at the start of an existing one-line comment, never a second line).

## Amended by CH0 (2026-11-12)

- **Functions:** `awk -F'\t' '$7=="CH3b2"' june2017_re/char_elesis/ch0_class_map.tsv` = 11 (8 listing-only: col 9 `n`; 8 `inside`, 3 `new`: col 5; proposed names col 6, `?` = name by the class pattern from the body).
- **Script states:** `awk -F'\t' '$8=="CH3b2"' june2017_re/char_elesis/ch0_state_skill.tsv` = 5.
- **Skill ids** (3): 28210, 28211, 28223.
- **Checklist rows** (`june2017_re/char_elesis/ch0_checklist_rows.tsv` col 5): L52, L876.
- CH0 split off CH3b: actives 28210 Flame Sword (reserved-buff path, `[JUNE_SKILL] SBUF` lines), 28211 Scalet Rose (Fire Blossom spawn `32b79db0`, written by CH3b0), 28223 Flame Protection.

## Amended by CH3b0 (2026-11-14)

- **Scalet Rose success End** is written: `ESI_SI_A_SBH_SCALET_ROSE_SUCCESS_End()` (June `32b79db0`, under `_SUMMON`). Register it on the success state's End slot (0x58,
  thunk `32b90830`, `@32b8e372`). The same state's Start `32b51550` zeroes `m_ucScaletRoseHitCount` (+0x9788): write it here. The per-hit increment is the attack-result
  delegate `32b524b0` that `InitPassiveSkillState` pushes when 28211 is learned (CH4a); until CH4a the count stays 0 and the End spawns nothing, so this phase's play-test
  checks the End runs (no stuck state) and CH4a's checks the blossoms (`[JUNE_CHAR] ELESIS SUMMON req npc 1567 scalet rose <i> of <n> ...`, `[JUNE_CHAR] SUMMON npc 1567 ... skill 28211`).

## Amended by CH3b (2026-11-14)

- **Upgrade ids:** 28231 / 28232 (Flame Sword / Scalet Rose upgrades, tagged CH3b by CH0) label this phase's own cases (28210 / 28231, 28211 / 28232): add them as labels on March's cases together with the bodies. CH3b did not label them.
- Scalet Rose's success blossoms (June `32b79db0`, `m_ucScaletRoseHitCount` +0x9788) are CH3b0's; this phase writes the hit-count increments.
