# June character Add (Stage 7) - Phase CH2b0 - Summons

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-add CH2b0` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | CH1 review; before CH2a (the first summoning skill: Particle Prism 29003 / 29029, class 9) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_SUMMON` (+ `..._SUMMON_DEBUG`) |
| Log tag | `[JUNE_CHAR]` (`ADD SUMMON`) |
| Review | `kickoffs/char-add/CH2b0-review.md` (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Rules" "Phase CH2 - Skills: Nasod Ruler and the first jobs (classes 9, 32, 33, 36)"
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "CH3b0 - Done" "CH3b0 - Review"   # once written; else Rena's "CH2d0 - Done"
cat june2017_re/char_add/ch0_summon.tsv
```

## Task

Port his summons from `ch0_summon.tsv`: the six June spawn sites (`329f350a`, `329feafb`, `32a05ace`, `329f5430`, `32a0c997`, `32a0ca9d`; NPC
`0x1444` 5188 x2, Apocalypse 1604 at `329f5430`, the rest named by CH0) with June's spawn rules, inside March's two `CreateNPCReq` sites
(`X2GUAdd.cpp:6103` / `:6120`) where they pair and new under `_SUMMON` where they do not; NPC ids by name (`X2UnitManager.h` under
`SERV_IRUHADEV_JUNE_ENUM_NAMES` if C++ lacks them); the summon NPC scripts adopted with `luabind.py check` (bind `GetEffectSetInstance` `321d97c0` and
`SetLocalRotateDegree` `31e4c520` whole, and drop their "add port" rows from `lua_unbound_known.tsv`); the summon follow-ups of `3230c1a0` (checklist
l.280) and `CheckSummonSpecialAttack` `3202daf0` for his summon skills; June NPC scripts that play June-only meshes need those meshes staged
(`npc_mesh_census.py`). The casts themselves are the skill phases'. Hand-off: the `ES_CREATE` / summon state lines the test must produce.

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

- Checklist rows (`june2017_re/char_add/ch0_checklist_rows.tsv` col 5): L283, L290, L291, L309, L314, L359, L733, L789, L117, L178, L258, L727, L787, L865.
- `june2017_re/char_add/ch0_summon.tsv`: five live sites (one more, `329f350a` in `329f3070`, is dead code: no caller in the exe). Particle Prism `32a0c740` -> 1576 / 1577 (pairs with March's two `CreateNPCReq`, `X2GUAdd.cpp:6103` / `:6120`; class 9, **first in CH2a: CH2b0 runs before CH2a**), ADE Void Impact `329fe650` -> 5188, AMM Charged Impulsar `32a05470` -> 5189 or 5190 (formation mode and mutation > 0), Apocalypse `329f5200` -> 1604 (the plan's `329f5190` is Doom's Day's End). 5189 / 5190 lack `X2UnitManager.h` members. `luabind.py check`: `PlayEffectSet_CustomSet_LUA` unbound in both Charged Impulsar scripts (bind whole from June or row it); Apocalypse's two known rows (`GetEffectSetInstance`, `SetLocalRotateDegree`) are this phase's. Dying Apocalypse `329fb6c0` (L291) is here.

## Amended by the CH1b review (2026-11-15)

- `CX2GUUser::ResetOnSummonEnd` (`X2GUUser.cpp`, Eve CH2b4's inline erase for `UC_ADD_MASTERMIND`) is June `31fc8a90` @`31fc8ab0` calling Add's `329fb6c0`: switch that branch to `CX2GUAdd::OnApocalypseSummonEnd()` (CH1b wrote it under `_CORE`) and widen its `protected` declaration so the user class can call it. CH1b left the inline copy alone.
