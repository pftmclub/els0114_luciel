# June damage and stat calculation - Phase S3b - socket flat stat in the emulator's game stat

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc S3b` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | S3 review; one S / R phase at a time |
| Flag | `SERV_IRUHADEV_JUNE_STATCALC_SOCKET` (S3's; + `_STATCALC_DEBUG`) |
| Log tag | `[JUNE_STATCALC]` (`python tools/logs.py --grep "JUNE_STATCALC] SHEET"`) |
| Review | `kickoffs/june-stat-calc/S3b-review.md` (the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Phase S1 - Done (2026-11-16)" "Phase S1b - Done (2026-11-16)" "Phase S2 - Done (2026-11-16)" "Phase S3 - Done (2026-11-16)"
```

Then the map row `socket flat stat in the emulator's game stat` (`phase = S3b`).

## Task

Split from S3 (2026-11-16). The D6 lead: unit 204 (D6ElswordS, weapon sockets 20 x 3 = +400 physical attack each, armour 220 x 3 per piece =
+100 physical defence each) reads the same in-game `patk` as the plain 205. No client path adds a socket's flat stat to the game stat
(`CX2Item::GetStat( true )` has no caller); June's game stat came from the server, and the emulator never added sockets.
- **The stand-in (no June server; S1b's precedent):** copy March's server, read in S3 (KncWX2Server, verify each line before porting):
  `KInventory::GetEquippedStat` (`GameServer/Inventory.cpp:2198`: per counted item the user sockets :2385-2398, random sockets :2409-2418,
  templet sockets :2436-2444, level-linked templet sockets :2347-2356 into the enchant-scaled base :2371-2375; set options :2450-2507 at the
  set's max level), `CXSLSocketItem::GetSocketOptionStat` (`Common/X2Data/XSLSocketItem.cpp:1362-1481`: raw x 1, x ( item use level, or unit
  level below 1, + `m_iAddLevel` ) when `m_bIsLevelLinkedStat`; dungeon-only only in a dungeon / field room, PvP-only only outside one;
  `m_IncreaseHPRate` x the HP one-point, total HP rate <= 0.5), title options (`UserTitleManager.cpp:1498-1536`, unit level), pet aura
  (`UserPet.cpp:129-166`, not hungry; dungeon / field / training only). Riding-pet options are not in the server's game stat.
- **Rule 3:** the character window's socket part (`X2UICharInfo.cpp` socket twin of `SumSocketOption`, `SHEET window ... socket=`) and the
  emulator's game stat must agree on a socketed unit, or the difference is explained (the window's ConvertAndAdd curve against the server's
  raw add). The socket option's range ([9] / [10], S1's routed item, `+0x314` / `+0x318` in `328842f0`'s sum) goes with it.
- **Proof:** a `SHEET emu` socket part, then `python tools/statcalc_check.py --units 204 205 --socket 204 205` must PASS on a play-test (Elysion:
  Soless Expert 33892 as in D6).

Per row: list every reader and refresher (rule 3); set the row to `June default` (a stand-in for server data) with the server lines cited.
Play-test on unit 204 / 205 at level 99, with and without sockets: the hand-off names the window and `SHEET` numbers to expect.

No hardcoded enum literals, no `June` in names, one-line comments, `junediverge.py` gate, as every phase (CLAUDE.md).
