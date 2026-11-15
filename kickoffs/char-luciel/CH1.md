# June character Lu/Ciel (Stage 8) - Phase CH1 - Class core: Final: Lu/Ciel goes live

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1p review, CH1a2 review, CH1a3 review, NU4 review, CH0b, **`june-state-ids` S2** ( user, 2026-11-15; S2 = that plan's last code phase ) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`, `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_DUO` defined |
| Log tag | `[JUNE_CHAR]` (`LUCIEL CORE` / `DATA`) (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |
| Review | kickoffs/char-luciel/CH1-review.md (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Starting context" "Phase CH1 - Deploy unit: the class core" "Phase CH0 - Lu/Ciel class map (read-only)"
```
Then the planning digests in `june2017_re/char_luciel/` the task names, and the Done notes of the phases this one depends on
(their routed gaps). Verify a digest line before building on it.

## Task

Do the CH1 row: define `_LIVE` and `_DUO`, add the `UT_LUCIEL` cases to `CX2Game::CreateGUUser` (`X2Game.cpp:21276`) and the intruder
copy (`:2141-2201`), adopt `Luciel_Duo.lua` and `Luciel_Demonio.lua` (`adopt-june`, bytecode; `luabind.py check` both = PASS), copy the motion
koms CH0b held, and add the `[JUNE_CHAR] LUCIEL DATA` line at the end of InitState. Play-test (the plan's CH1 row): create, enter the village,
walk, jump, dash, combo, swap bodies, enter a dungeon, relog. Write `CH1-review.md` and amend CH2a / DU3 / CHN / TU from what the play-test shows.

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

## Amended by CH0 (2026-11-02)

- **Checklist rows** (`ch0_checklist_rows.tsv` col 5): §5 L184-188 per-character parser census; §7h L421 EFFECT_SET_LIST variants; §7h L423 SOUND_PLAY_EX x5, CUSTOM_STATE_DELETE_SOUND x5, STATE_SOUND_PLAY x0; §7h L424 USEFUL_VALUE0..4 rows and getters; §7o L506-507 SET_LIMIT_MAX_G_SPEED -4000 in <prefix>DAMAGE_BOUNDING_DOWN (13 tables; Lu and C; §7p L545 TIME_STOP_IMMUNE_STATE = TRUE in the class script; §7t L586-589 SHOW_HYPER_CUT_IN_TIME in the class script; §7x L664 combo hits carry June AT_COMMAND tag; adopt the script with its tags.
- Adopt `DLG_LUCIEL_BOUNDING_KEY.lua` (data034) with the scripts: Init 0x40 `32b1e8e0` loads it at `32b1ea23`. `luabind.py check` all three.

## Amended by char-ara CH1 (2026-11-11): the enum gate

`luabind.py check <script>` now also runs the ENUM and STATE gates (tool docstring). Ara's CH1 shipped with her June script reading
names the merged data036 `Enum.lua` lacked: the file died at load (`can't find 'CRS_INVISIBLE' class variable`) and her `CHAIN_SYSTEM`
popped `StateID` on every field entry. The check must print `RESULT: PASS` **before** the pack. Fix every `ENUM MISSING TABLE / NAME` by adding
June's member to `Enum.lua` (core-list source, `-- char-luciel CH1` one-line comment, then `luasrc.py refresh data036 Enum.lua`), and every
`STATE MISMATCH / NO C++ MEMBER` so the Lua value equals the C++ enum (add the C++ member first when it is missing). `enum nil in June` lines
are June's own nils: leave them.
Run of 2026-11-11 (pre-port): `DUO_TYPE.DT_BOTH` / `DT_KEEP` and `SKILL_ID.RNSI_A_LLD_BLUFF` missing.

## Amended by char-luciel CH1a0 (2026-11-14)

- `_LIVE` adds the two `CX2Game` calls to `UpdateLucielSwitchingGuideUI( &false, 0.f )` ( June ctor @`32307bca`, dtor @`32308957`, each just
  before Eve's `UpdateEveRecoveryUI` call; `X2Game.cpp` ~468 / ~852 ).

## Amended by char-luciel CH1a (2026-11-14)

- `CX2GULuciel::CreateGULuciel` ( Chung's `CreateGUChung` pattern ) was left out of CH1a: the class stays abstract until CH1o wires the pure virtuals. Add it with the factory cases.
- Depends on CH1a4's review as well ( the dtor calls `EndTransformedForm` / `ReleaseTransformedSkinAnim`, whose bodies are CH1a4's ).

## Amended by char-luciel CH1a3 (2026-11-14)

- `_LIVE` turns on Lu/Ciel's own gauge ( `CreateMyGageUI` case, `CreateGageDataByUnitClass` in the party / PvP member inserts ). Its `InitUI` opens
  `DLG_Luciel_CombinationPoint.lua` ( data034, `ch0_assets.tsv` row 750: missing, owner CH5, `adopt-june` ). Adopt it here with the scripts
  ( `luabind.py check` ) or the HUD shows no Combination Point row until CH5; also confirm the particle `LD_Arousal_UI_01` ( full points ) loads.
  The play-test then checks: the point row fills one picture per point, the full mark and effect at 10, the portrait swaps to Ciel's face on a body
  swap, and the resting body's MP shows on the unselected info bar.

## Amended by char-luciel CH1o (2026-11-15)

- **Waits for plan `june-state-ids`** ( user, 2026-11-15 ): March's `USER_STATE_ID` shifts to June's numbers, so the engine's riding / slot / event / bounding /
  wait-change state changes land on Lu's June-numbered states. After it, set `m_bBoundingStatesReady` for Lu / Ciel ( CH1m registers bounding inline ) and check
  that `InitEventStates` no longer overwrites `LSI_RIDING_SPECIAL_MOVE`.
  **Amended by june-state-ids planning (2026-11-15):** that plan's S1 shifts the enum ( 46 June members, `USI_END` 106 -> 152, every class +46,
  `Enum.lua` with it ) and S2 does the Lu/Ciel part ( shadow `USI_SPECIAL_ATTACK_*` dropped, `InitBoundingStates` / `InitWaitStateChange` or a
  replacement, `m_bBoundingStatesReady` ); S2 has no review of its own, so this phase's review covers S2's diff. Read S2's Done note
  ( `python tools/docsection.py JUNE_STATE_IDS_PHASES.md "S2 - Done"` ) before the task.
  **Amended by june-state-ids S2 (2026-11-15): the seam is closed; nothing is left for the engine state-id seam.** Lu calls `InitBoundingStates( "LSI_LU_" )`
  ( sets `m_bBoundingStatesReady` ) and `InitWaitStateChange( "LSI_WAIT_STATE_CHANGE" )`; the shadow `USI_SPECIAL_ATTACK_*` are gone; `InitEventStates` (110-118)
  no longer overwrites 92; `Always.h` guards `_LIVE` on `SERV_IRUHADEV_JUNE_USER_STATE_IDS`. The review covers S2's commit `857b18cd`. The play-test adds: Lu
  returns from the win pose after a dungeon result ( wait change ), and `offline_server.log` shows `JUNE_FX C3d-3 bounding states registered for LSI_LU_`.
- `SERV_IRUHADEV_JUNE_USER_VIRTUAL_HOOKS` ( CH1o, defined ) is live for every class from CH1o on: the regression part of this play-test walks one other class
  ( emotion, a skill, a dash, a jump, a death ) to show nothing changed.

## Amended by char-luciel CH1o2 (2026-11-15)

- **Ciel's death states ( lead ):** the state-end stand-ins in `CX2GUUser` ( `X2GUUser.cpp` ~7731 / ~7741: `SOCKET_PARITY`'s SIEE 18 and `BUFF_DEATH_CLEAR`'s
  clear-all ) test `USI_DIE_FRONT` / `USI_DIE_BACK`, standing in for June's die StateEnd `3202a7d0`, which June registers on every class's die states. Lu's die
  ids are 7 / 8 ( match ); Ciel's `LSI_CIEL_DIE_FRONT` / `_BACK` ( 192 / 193 ) do not, so a dying Ciel would keep her buffs. Check whether Lu's InitState rows
  give Ciel's die states `3202a7d0`; if so, make the stand-in ask `GetDieFrontStateID()` / `GetDieBackStateID()` ( const since CH1o2 ) or register the StateEnd,
  and add a Ciel death to the play-test ( buffs gone after the revive ).
