# June damage and stat calculation - Phase S1 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc S1 review` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | Phase S1 done (its Done note and commit) and its play-test |
| Flag | `SERV_IRUHADEV_JUNE_STATCALC_UNIT_STAT` (three homes: `Always.h`, `ServerDefine.h`, `X2ServerProtocol_2010.vcxproj` X2TOOL) + `_STATCALC_DEBUG` (stays on for S1b; Z retires it) |
| Log tag | `[JUNE_STATCALC]` (`python tools/logs.py --grep "JUNE_STATCALC] SHEET"`) |

## Read first

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Phases S1-S4, R1 - Stat sheet and resource gain (after Stages 7 and 8)" "Phase S1 - Done"
```

Then the map rows with `phase = S1` in `june2017_re/statcalc/statcalc_map.tsv`.

## Task

Review Phase S1's commit against June's exe:
- `KStat` (`CommonPacket.h` / `.cpp`): 7 fields in June's order `{ float hp; int atkP, atkM, rangeP, rangeM, defP, defM }` (`322814a0` reads
  `[eax]` with `fld`, `+4..+0x18` with `fild`; `323571c0` the same off the stack). Every live `m_iBaseHP` reader moved (`cl /P`, not reading
  `#ifdef`s): `X2Room.cpp`, `X2StateMenu.cpp` (2 sites), `X2ItemManager.cpp` (2), `X2Stat.h`, the emulator. The dead ones (`X2StateMenu.cpp`
  `#else` of `SERV_CHANGE_EQUIPPED_PACKET_REFAC`, `X2StateDungeonGame.cpp` under `SERV_TOONILAND_CHANNELING_EVENT`) are listed in the note.
- `CX2Stat::Stat`: the range pair in every block operation. S1 assumed June treats [6..12] uniformly in `DecStat` / `MultiplyStat` /
  `ConvertAndAddStat` (read: `SetStat` 322a91a0, `AddStat` 32368910 and the window's full add in 3291cfb0, `GetEqipStat` 3234e210). Read one
  June `MultiplyStat` (the `GetFinalDamage` scale-transform copy) and one `DecStat` / `ConvertAndAddStat` site, or route the doubt to S2.
- `SetGameStat` both overloads (`322814a0`, `32281690`): basics order, the KStat overload zeroes [0..5], the Stat overload keeps them; June
  skips storing an unchanged block (S1 always assigns: same values). Re-run `python june2017_re/statcalc/emu_unit_stat.py` (must PASS).
- The emulator's range stand-in (`CX2OfflineInventory::AddEquippedWeaponRange`, `AddBaseWeaponRange`): the user chose cobodex.eu's formula
  (the weapon's own attack x item.kim `damage_range` / 100, in both the base and game stats). Check it is applied once per stat (no double
  count in `MakeGameStat` + `MakeMinorUnitGameStat`), and that the window shows `min ~ max` from the base stat.
- The SHEET lines from the play-test: `emu`, `game`, `window` agree on hp / attack / range / defence, and the window's extra (skill, guild,
  title, socket) is the only difference.

Hygiene is one command: `python tools/finish.py --review --commits <S1's commit(s)> --with SERV_IRUHADEV_JUNE_STATCALC_UNIT_STAT
SERV_IRUHADEV_JUNE_STATCALC_DEBUG`. Paste its block into the review; run it with `-v` and explain every OTHER row and deleted line.

Spot-check 2-3 items directly in Ghidra (read-only); if one fails, re-check the whole phase. Fix clear transcription errors under the same
flag (then `tools/build.py --deploy`); hand design decisions back to the user. `_STATCALC_DEBUG` stays on (S1b uses it). Write "Phase S1 -
Review" into the plan doc, update the S1 row in `STATUS.md`, check `kickoffs/june-stat-calc/S1b.md`, and commit.
