# June character Elesis (Stage 7) - Phase CH1 - Deploy unit live: _LIVE, the June script, the play-test

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH1` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH1q review, CH0b |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`, `_WOS` defined (+ `..._DEBUG` for the load lines) |
| Log tag | `[JUNE_CHAR]` (`ELESIS DATA`, `ELESIS CORE`, `ELESIS WOS`), `ELESIS SKILL` (`--grep SBUF`) |
| Review | `kickoffs/char-elesis/CH1-review.md` (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md --no-sub "Rules" "Phase CH1 - Deploy unit: June script, motions and the class core"
python tools/docsection.py JUNE_CHARACTER_ARA_PHASES.md "CH1 - Done" "CH1 - Review"
python tools/docsection.py JUNE_CHARACTER_ELSWORD_PHASES.md "CH1 - Done (2026-11-04)"   # the Way of the Sword checks
```

## Task

Define `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (+ `_WOS`); adopt `ELESIS_KNIGHT.lua` (`/x2-adopt-june`, `--dry-run` first; March's copy is bytecode outside the ledger:
adoption replaces it) and pass `python tools/luabind.py check ELESIS_KNIGHT` (every unbound name bound whole or in `lua_unbound_known.tsv`); copy
CH0b's held motion koms (if any); pack, copy; full build and deploy; the play-test on units 20 (Knight), 40 (Saber Knight), 21 (Pyro Knight), 29
(Grand Master), 30 (Blazing Heart). Save a pre-CH1 ErrorLog first.

Done gate (plan, CH1 final): June's script and motions load (`ELESIS DATA` line); no new `KEM_ERROR` / `Motion Error` / missing-animation line
against the pre-CH1 run; combos, dash, jump, damage and riding play June's animations; the Way of the Sword gauge fills both ways, vigor and
destruction start and end as June and the state survives a stage change; Endurance Power, Dodge and Slash and Sword Fire produce the
`[JUNE_CHAR] ELESIS SKILL SBUF` lines (the first live reserved-buff path, 7ai; amended by CH1p below); each skill cast twice shows its cool time and spends MP; `charport_scan.py` /
`charport_keys.py ELESIS_KNIGHT` show nothing new. Hand-off: dev console first, per check where / paste / look for / if not.

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
Phase tag (user, 2026-11-09): every edited or new code block carries a one-line `// char-elesis <phase>` comment (at the start of an existing one-line comment, never a second line).

## Amended by char-ara CH1 (2026-11-11): the enum gate

`luabind.py check <script>` now also runs the ENUM and STATE gates (tool docstring). Ara's CH1 shipped with her June script reading
names the merged data036 `Enum.lua` lacked: the file died at load (`can't find 'CRS_INVISIBLE' class variable`) and her `CHAIN_SYSTEM`
popped `StateID` on every field entry. The check must print `RESULT: PASS` **before** the pack. Fix every `ENUM MISSING TABLE / NAME` by adding
June's member to `Enum.lua` (core-list source, `-- char-elesis CH1` one-line comment, then `luasrc.py refresh data036 Enum.lua`), and every
`STATE MISMATCH / NO C++ MEMBER` so the Lua value equals the C++ enum (add the C++ member first when it is missing). `enum nil in June` lines
are June's own nils: leave them.
CH0's run (2026-11-11, pre-port): 4 `SKILL_ID` names missing (`RNSI_A_SCA_BLOOD_STING`, `RNSI_A_SGM_UPPER_SLASH`, `RNSI_P_SBH_BLAZE_ABSOLU`,
`RNSI_SA_SCA_CHASING_SWORD`).

## Amended by CH0 (2026-11-12)

- **Checklist rows** (`june2017_re/char_elesis/ch0_checklist_rows.tsv` col 5): L14, L116, L123, L150, L427, L428, L530, L597, L619, L670, L762, L876.
- Stage the held `MOTION_EL_GM.x` / `Motion_EL_BH.x` (`ch1_held_motions.tsv`) with the script. Live data033 already has June's `Motion_EL.x`, `Motion_EL_SK.x`, `Motion_EL_Lobby.x`: the six March names they lost (`SI_A_EL_LEAP_CRASH_*`, `SI_A_EL_RUSHING_SWORD`, `SI_SA_SSK_SPIRAL_BLAST`, `_VICTORIOUS_SWORD`) are renamed in June and come right with June's script. Enum gate (`luabind.py check ELESIS_KNIGHT`, CH0): 4 `ENUM MISSING NAME` (`RNSI_A_SCA_BLOOD_STING`, `RNSI_A_SGM_UPPER_SLASH`, `RNSI_P_SBH_BLAZE_ABSOLU`, `RNSI_SA_SCA_CHASING_SWORD`): add June's members to data036 `Enum.lua`; 0 STATE lines, 0 unbound methods.

## Amended by CH1p (2026-11-13): the reserved-buff lines

- The `[JUNE_SKILL] SBUF parse / frame / state end` lines no longer exist: skill-system Z (`66669d39`) removed `SERV_IRUHADEV_JUNE_SKILL_DEBUG` and its
  blocks (Ara's CH2d / CH3b play-tests saw none). CH1 adds one temporary line under a new `SERV_IRUHADEV_JUNE_CHAR_ELESIS_DEBUG` (house block, `#define`d),
  for `UT_ELESIS` only, in `CX2GUUser::ParsingBuffFactorID` (state id, `m_iStateBuffSkillID`, `m_uiNowStateSkillLevel`, list size, "cleared" on the
  learn-check return) and in `CommonFrameMove_InsertBuffFactor` / `ApplyAllStateInsertBuffFactor` (BFI, level): `[JUNE_CHAR] ELESIS SKILL SBUF parse /
  frame / state end ...`, throttled per state. The CH1 review retires it.
- What June's script keys (`june_src_fixed/data036_kom/ELESIS_KNIGHT.src.lua`), so what the play-test checks: Endurance Power `ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE`
  (`BFI_BUFF_SUPER_ARMOR` 143, relative skill 0: level 1 at once) and `_MEMO` (`BFI_ELESIS_ENDURANCE_MEMO` 335); Dodge and Slash x3 / Rushing Sword x3
  (`BFI_BUFF_SGM_AGILITY_MASTERY` at 0.1 s, relative 28402 `RNSI_P_SGM_AGILITY_MASTERY`, learn check: no line past "cleared" unless the passive is learned);
  Sword Fire fly / ground (`BFI_BUFF_FLAME_SWORD` at 1.4 s, relative 28210 `RNSI_A_SBH_FLAME_SWORD`) and Infernal Blade (`BFI_BUFF_INFERNAL_BLADE` 1.5 s).
  June's Flame Sword state itself has no table (March's `{ BFI 299, 0.1 }` is gone). The C++ (S3e `ParsingBuffFactorID`, whole June) needs no change: her
  states carry June ids through CH1i's `SetEquippedSkillLevel`.
- The play-test also casts a B-slot skill that holds a key (Extinction or Drastic Charge from slot B, then swap slots while holding): CH1p moved her 11
  key-holding states to June's 0-9 `m_kSkillSlotKey`; with `_LIVE` on, March's 4 + 4 read would watch the wrong key (B 5-9 read as W..R or none).

## Amended by CH1q (2026-11-13): the Way of the Sword lines

- `_WOS_DEBUG` is defined: with `_WOS` on, the play-test must show `[JUNE_CHAR] ELESIS WOS vigor start` / `vigor end` / `destruction start` / `destruction end` (point, class, `dark knight line 0` on 8 / 29 / 30 / 120 / 121) and `attack gain` (once a second, `rate x1.00` without Dark Force). Read with `python tools/logs.py --grep "ELESIS WOS"`.
- What to check in game: the gauge moves both ways on hits given and taken ( `attack gain` lines ); entering each mode plays its colour and voice; the mode ends with `EffectSet_VigorEnd` / `_DestructionEnd` (the Dark Knight end sets play too but draw nothing without their buff); the MP bar's detonation part now fills up to the whole MP (no 300 cap) outside Destruction; after a stage change the mode steps one point back toward the centre and ends (June's `ReInit`); after a dungeon ends the mode is kept (June's `InitWhenGameIsOver`). The CH1 review retires `_WOS_DEBUG`.
