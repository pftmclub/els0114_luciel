# June damage and stat calculation - Phase S1b review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc S1b review` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | Phase S1b done (its Done note and commit) and its play-test |
| Flag | `SERV_IRUHADEV_JUNE_STATCALC_UNIT_STAT` (`ResetMaxHP`, the emulator's passive stand-in), `SERV_IRUHADEV_JUNE_STATCALC_HIT_DODGE` (new) + `_STATCALC_DEBUG` (stays on for S2; Z retires it) |
| Log tag | `[JUNE_STATCALC]` (`python tools/logs.py --grep "JUNE_STATCALC] MAXHP"`, `PASSIVE`, `HITRATE`, `DODGEROLL`) |

## Read first

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Phases S1-S4, R1 - Stat sheet and resource gain (after Stages 7 and 8)" "Phase S1b - Done (2026-11-16)"
```

Then the map rows with `phase = S1b` in `june2017_re/statcalc/statcalc_map.tsv` and `grep -n "StatCalc S33 S1b" june2017_re/funcmap.tsv`.

## Task

Review Phase S1b's commit against June's exe:
- `ResetMaxHP` `31feba70` (`X2GUUser.cpp`): the control flow (no instance data -> still the mode-5 test; no room -> return), the 0.7f widened
  to double and 1.5, team `TN_BLUE` = slot `+0x10` == 1. Judge the two stand-ins (slot `+0x58` boss flag, PvP room `+0xf8` code event) and the
  PvP-room-only read: confirm `+0xd8` of `CX2DungeonRoom` / `CX2BattleFieldRoom` in June is the difficulty / field id (never 4 / 5). If you can,
  name June's slot `+0x58` writer (the KRoomSlotInfo setter) to confirm it is the revived `m_bIsBoss`.
- The emulator stand-in `AddPassiveSkillStat` (`Offline/X2OfflineServer.cpp`): it must equal `CX2Unit::GetUnitStat`'s four passive terms (June
  `32358b00`, `X2Unit.cpp` under `SERV_IRUHADEV_JUNE_SKILL_STAT_GETTERS`), applied to the game stat only (never the base stat, or the window
  counts them twice), before the gear, for Lu's and Ciel's stat. The play-test's `SHEET emu` / `SHEET game` / `MAXHP` hp must equal `SHEET
  window` hp for jtLordKnight.
- `CalcHitDodgePercent` `32610a00` (`X2DamageManager.cpp`): June's whole body vs the port line by line (revenge / reflect windows, the
  reflect-by-buff list for both unit types, the NPC-attacker evade, the blind x accuracy order, the Raven / Get Chance / add-accuracy blocks and
  the invincible override). Re-run `python june2017_re/statcalc/emu_hit_dodge.py` (must PASS).
- The dodge offset at the four rolls (@326253e2 / @326285b9 / @3262b2fe / @3262f11a): who the attacker is per path (the effect path's damage
  data `+0xc`), the order against `SetDefenderDamageRateOfPart`, `HasExtraNPCStat` as the stand-in for June's per-ID stat byte `+0xe8` (an NPC
  fetched without `32384d10` shares the byte of its ID's last fetch in June), and `JUNE_BUGS` 775.
- The ability map: the 0x607 identification (`3260e0c0` jump table) and the Stage 30 hand-off; no client code expected.

Hygiene is one command:
`python tools/finish.py --review --commits <S1b's commit(s)> --with SERV_IRUHADEV_JUNE_STATCALC_UNIT_STAT SERV_IRUHADEV_JUNE_STATCALC_HIT_DODGE SERV_IRUHADEV_JUNE_STATCALC_DEBUG -v`
(flagcheck per flag, junediverge `--check` plus `--check-all`, enumscan). Paste its block; explain every OTHER row and deleted line; a FAIL or
ERROR is a finding. `X2PVPRoom.h` is inside `JUNE_ENUM_NAMES`: `flagcheck SERV_IRUHADEV_JUNE_ENUM_NAMES --range <c>^..<c> --files X2Lib/X2PVPRoom.h`.

Spot-check 2-3 items directly in Ghidra (read-only); if one fails, re-check the whole phase. Fix clear transcription errors under the same flag
(then `tools/build.py --deploy`); hand design decisions back to the user. Write "Phase S1b - Review" into the plan doc, update the S1b STATUS row,
check `kickoffs/june-stat-calc/S2.md`, and commit.
