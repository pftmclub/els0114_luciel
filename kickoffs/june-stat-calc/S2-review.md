# June damage and stat calculation - Phase S2 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc S2 review` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | Phase S2 done (its Done note and commit) and its play-test |
| Flag | `SERV_IRUHADEV_JUNE_STATCALC_EQUIP_STAT` (new) + `_STATCALC_DEBUG` (stays on for S3; Z retires it) |
| Log tag | `[JUNE_STATCALC]` (`python tools/logs.py --grep "JUNE_STATCALC] ITEMSTAT"`, `RESIST`, `SHEET`) |

## Read first

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Phases S1-S4, R1 - Stat sheet and resource gain (after Stages 7 and 8)" "Phase S2 - Done (2026-11-16)"
```

Then the map rows with `phase = S2` in `june2017_re/statcalc/statcalc_map.tsv` and `grep -n "StatCalc S33 S2" june2017_re/funcmap.tsv`.

## Task

**First, the open play-test finding (Done note, "Open finding").** With testlu1's +13 Henir dual weapon 62119 equipped, the character window's
gear sum (`CX2Unit::GetEqipStat`, `X2Unit.cpp`) skips it although its item stat is right (`ITEMSTAT ... atk=19574/18662`); the Cobo weapon in
the same slot counts. Add a throttled `[JUNE_STATCALC] EQIPSKIP` line under `_STATCALC_DEBUG` naming each skipped item and the test that
dropped it (not in the list / GetItem NULL / IsUsableEquip false and why / group / PvP-equip), build, and hand off: testlu1 re-equips 62119
( `/additem` is not needed, it is in her bag ), opens the character window. Fix the cause under the flag it belongs to (June's window sums
the equipped weapon), then re-check `SHEET window` total = `SHEET emu` game with 62119 equipped.

Then review Phase S2's commit against June's exe:
- `CX2Item::GetStat` `328842f0` and `CX2Item::GetEnchantStat` `32882630` (`X2Item.cpp`): the unit (owner `+0xc4`, else my selected unit), its level
  (`+0xb4`), the calculator choice (PvP-equip bit 7 or the 22 ids at `334c0cb8` -> `328d6d70`, else `328d6f60`), the uncached path's type / class
  (owner templet `+0xaf0`) against the cached path's 0 / 0, the "no unit -> stat stays 0" branches, the range term (`(double)atk * damage_range *
  0.01f` @32882841) and the enchant index (`abs( added + effective level ) < 0x54`, JUNE_BUGS 778). The socket loops: the dungeon-only game types
  (2 / 3, June's 5 has no Enum entry) and `+0x18c` = `GetDungeonType`; PvP-only = type 1 (allow-list row). Re-run
  `python june2017_re/statcalc/emu_equip_stat.py` (must PASS).
- The calculator (`X2ItemStatCalculator.cpp`): the level rule @328d71ac (use level, or the unit level when 0 and not NoEquip; JUNE_BUGS 779) and
  `CalculateItemStatAtUnitLevel` = `328d6d70` (no templet fallback, no cache, the transcendence table as the cached form).
- `CX2Unit::GetEnchantStat` `32358df0` (`X2Unit.cpp`): the equip loop order (item, usable, templet, group), the 45-case defence switch (the
  emulation reads the port's switch: confirm it covers June's whole jump tree), the show tests `32357940` / `323579e0` (June's tail for a flag
  byte other than 0 / 1 is not ported: confirm June's two callers pass 0 / 1 only), title / pet / set order, the `+0x330` source = Dragon Breath,
  the resonance stand-in, the clamp. The CharInfo caller (`X2UICharInfo.cpp`, June 32916160's form group) and Elesis's default group 0.
- `GetEqipStat` / both `GetSetIDNPartsNum`: `IsUsableEquip` replaces the endurance-only test; the appearance filter `328823d0` stays a stand-in.
- The emulator (`Offline/X2OfflineInventory.cpp`, `X2OfflineServer.cpp`, `Handlers_Room.cpp`): `GetEquippedItemStat` must equal the two client
  bodies for this character's class and level; `SumEquippedItemStat` = `GetEqipStat( false, group )` (fashion always, a normal item usable and
  not PvP-equip); the base stat carries no range (S1's `AddBaseWeaponRange` calls are compiled out), the game stat the range of every weapon and
  armour piece. The play-test's `SHEET emu` game range must equal the `SHEET window` total range.

Hygiene is one command:
`python tools/finish.py --review --commits <S2's commit(s)> --with SERV_IRUHADEV_JUNE_STATCALC_EQUIP_STAT SERV_IRUHADEV_JUNE_STATCALC_DEBUG -v`
(flagcheck per flag, junediverge `--check` plus `--check-all`, enumscan). Paste its block; explain every OTHER row and deleted line; a FAIL or
ERROR is a finding.

Spot-check 2-3 items directly in Ghidra (read-only); if one fails, re-check the whole phase. Fix clear transcription errors under the same flag
(then `tools/build.py --deploy`); hand design decisions back to the user. Write "Phase S2 - Review" into the plan doc, update the S2 STATUS row,
check `kickoffs/june-stat-calc/S3.md`, and commit.
