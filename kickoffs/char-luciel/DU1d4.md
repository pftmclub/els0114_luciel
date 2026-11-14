# June character Lu/Ciel (Stage 8) - Phase DU1d4 - The out-of-line getter callers, then the DU1 regression play-test

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU1d4` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU1d3 |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | none of its own: `kickoffs/char-luciel/DU1-review.md` covers DU1-DU1d4 |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU1b2 - Done" "DU1d - Done" "DU1d2 - Done" "DU1d3 - Done"
python tools/docsection.py kickoffs/char-luciel/DU1d.md "Amended by DU1b4 (2026-11-14)" "Amended by DU1c (2026-11-14)" "Amended by DU1c2 (2026-11-14)"
```
Then `june2017_re/char_luciel/du1d_body_reads.tsv`: every row with `owner` = `DU1d4` (26 June functions whose sites are only out-of-line
getter calls: `32071d80` = `GetXSkinAnimPtr()`, `32070cf0` = `GetXSkinAnimFuture()`, `32071db0` = `GetNotSelectXSkinAnimPtr()`, `32071d40` =
`GetMinorUnitXSkinAnimPtr()`).

## Task

Part 11 of DU1 (split by DU1d2, 2026-11-14), the last one:

- **The out-of-line getter callers:** render prepare `31fd07b0`, the shared DIE_FRONT / DIE_BACK starts `31fd2d60` / `31fd2e30`, active-skill show
  `31fd3180` / `31fde0f0`, CUSTOM_RENDER setup `31fd4460`, PEPPER_RUN_END_FrameMove `31fd98b0`, `SetLeftHandScale` `31fd9f40`, the sound player
  `31fea120`, `31ff03d0` (vslot 0x7e4), `DoAimingTarget` `31ff5440`, `StartIntrudeFrameMove` `31ffc5c0`, `DamageAirDownLandingFrameMove`
  `31ffd160`, `COMMON_RELATIONSHIP_SKILL_FrameMove` `320018c0`, the PlagHeadMark refresh `3200bc20`, `ShowHyperSkillCutInByScript` `32014fc0`,
  `DamageDataChangeProcess` `320454a0`, `CanApplyBuffToGameUnit` `32045dc0`, `CommonStateStart` `@32060bb3`, `CommonFrameMove` (the calls DU1b2's
  Done note does not list), and the rest of the `DU1d4` rows. Re-check each row's function first (DU1d2's census defect: a read after a
  `ret` with no padding belongs to the next function). For each: a March function -> June's getter at exactly June's call (March's line in
  `#else`); "none in March" -> say whose phase it is.
- **Close the DU1 series:** a full build (`python tools/build.py --deploy`, `DEPLOY OK` in the Done note) and the regression play-test the DU1
  kickoff asked for: an existing character's login / village / dungeon round-trip with no new log line, covering DU1b4's untimed SPEED_TIME
  entries (walk, dash, jump and a few skills on one March class), DU1c's MP-on-hit cash ability (a SAT 12 item such as the Magic Necklace
  200680, equip / unequip in the village) and `InitStat` (HP / MP / attack unchanged at dungeon start), DU1c2's weapon glow and attribute
  sparks (a +11 / +13 weapon, an attribute-enchanted weapon) and the village HUD's max HP / MP, DU1d's cancel macros (combo and skill
  cancels by arrow / walk / dash / skill key on two March classes), and **DU1d2's revive voice** (die in a dungeon and revive with a
  resurrection stone on Rena: `RenaVoice_Revive01` / `02` plays; on another March class its usual voice). Then `DU1-review.md` runs.

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
