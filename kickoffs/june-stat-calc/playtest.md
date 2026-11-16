# june-stat-calc play-test sections (D6, one per ported character)

One section per character, handed over one at a time. Each character has two level-99 units made by the phase: `D6<Name>S` (every worn piece
socketed: weapon attack sockets, armour defence sockets) and `D6<Name>N` (the same top gear, no sockets). Both are transcended (the hyper active
skill needs it) and carry the same loadout. Standing rules apply: dev commands first, a table per key (A S D C F Q W E R T), nicknames not ids,
skills in slots 0-3 and 5-8 (slot 4 only for the hyper active, which is a transcendence skill), and "after each skill you must be able to move;
if you are stuck, name the skill".

## D6-Elsword - Elsword damage smoke (Lord Knight (class 100))

### Dev console
`/stage 2` (inside a dungeon: jumps to stage 2, used for the boss)
`/invin 0` (turns invincibility off; type it for the incoming-damage steps 8 and 9 only)
No `/additem` is needed: `D6ElswordS` and `D6ElswordN` already wear the same top gear (+10), `D6ElswordS` has sockets in every worn piece, and the skills are slotted.

### Loadout
Page 1 on both `D6ElswordS` and `D6ElswordN` (the same table). Keys not listed are empty (T / slot 9 stay empty).
| key | skill (nickname) | page / slot |
|---|---|---|
| A | Impact Smash (active) | slot 0 |
| S | Sonic Blade (special active, grade 1) | slot 1 |
| D | Windmill (special active, grade 2) | slot 2 |
| C | Grand Cross (special active, grade 3) | slot 3 |
| F | Gigantic Slash (hyper active) | slot 4 |
| Q | Imperial Crusher (special active, grade 2) | slot 5 |
| W | Sonic Blade: Crescent (special active, grade 1) | slot 6 |

### Steps
Pause **2 seconds between every hit kind below**: the debug line is written once a second per attacker / monster pair, so a second hit inside
that second logs nothing. Hit each kind on the **same** monster, in this order, and write the order down (the log does not name the kind):
(1) normal: the plain Z combo, 3 Z presses; (2) command: one direction attack from the in-game command list (dash Z, jump Z or jump X; the one
your Impact Smash has); (3) special active: the skill on D; (4) special active: the skill on C; (5) special active: the skill on S;
(6) hyper active: F (fill the hyper gauge first by hitting monsters until the F skill is lit); (7) one more special active: Q.
1. Log in `D6ElswordS` (the socketed unit). Open the character window (C) once and look at the attack numbers: the physical attack is the one the sockets raise.
2. Level-89 monster: enter **Elysion: Soless (Expert)** (dungeon 33892, monsters level 89) from the world map, stage 0. On the first monster you reach, do the 7 hit kinds above.
3. Leave. Enter **Elder Secret Dungeon (Hell)** (dungeon 33031), stage 0. On `Bunker` (the first monster, NPC 1466) do the 7 hit kinds.
4. Type `/stage 2`. On the boss (NPC 1470, `Punch Boss`) do the 7 hit kinds again.
5. Leave the dungeon. Log in `D6ElswordN` (no sockets). Look at the character window: the physical attack is lower than D6ElswordS's.
6. Repeat step 2 with `D6ElswordN`: hit kinds (1) normal, (2) command, (3) D, (6) F only.
7. Repeat steps 3 and 4 with `D6ElswordN`: hit kinds (1) normal, (3) D, (6) F only, on `Bunker` and on the boss.
8. Incoming damage, `D6ElswordN`: type `/invin 0`, enter **Elder Secret Dungeon (Hell)** stage 0, stand in front of `Bunker` and take 3 hits (do not attack back), then `/stage 2` and take 3 hits from the boss.
9. Leave, enter **Elysion: Soless (Expert)** stage 0 with `/invin 0` still on, stand in front of a normal monster and take 3 hits. Then close the game.

### Expect
- On screen: every hit shows a number and the monster's HP bar moves. Normal monster (level 89): five to six digits per skill hit. `Bunker` (secret, k 1.0) takes less per point of attack than the normal monster (D1's run read `rate=0.5526` on it at level 104); the boss shows `state=0.3000` on `GFD2` (its 70 % reduce, D2b).
- Log lines, one `HIT` + `GFD` + `GFD2` per hit kind per monster (`python tools/logs.py --tag JUNE_STATCALC`): `HIT att=204 lv=99 def=NPCID dlv=LEVEL` (`att` is `D6ElswordS`'s unit uid, and the plain unit's is `205`; `NPCID` is the monster's id, `LEVEL` its level).
- `rate` = defence / ( dlv x 39.35 + 258.65 + defence ), to 4 decimals, on every line; `march=` is March's old rate and is only for comparison.
- `GFD2`: `base` = `motion x drate`, and its last `dmg` equals the `HIT` line's `dmg` on every line.
- `D6ElswordS` vs `D6ElswordN`: on `GFD patk=IN>OUT` (or `matk=` for magic classes) the socketed unit's `out` is higher than the plain unit's `out` by the sockets' attack total (the weapon carries three +400 attack sockets).
- Incoming (`REACT def=` the unit's uid): `hp=BEFORE>AFTER` falls by exactly `dmg` unless a guard or shield is named on the line; `dlv=99` on the matching `HIT def=` line with the unit's uid and `rate` from the unit's own defence (the socketed unit's is higher).
- No `HIT` line with `dmg=0` unless its `GFD2` shows `motion=0`.
- Grand Cross's extra hits have a damage-data motion power of 0, so a `dmg=0` line with `motion=0` on its GFD2 is expected (D2b).

### If not
- No `[JUNE_STATCALC]` lines at all: the debug define is off or the build is not the deployed one; say so and stop.
- `dmg=0` with `motion` above 0: send the `HIT` / `GFD` / `GFD2` triple (routes to D2b); a `rate` that does not match the formula: send the line (routes to D1); `REACT hp` not falling by `dmg`: send the REACT line (routes to D4a for a monster, D4b for the unit); a hit with no `GFD2` line is only the once-a-second throttle unless every hit kind is missing.
- A skill that does nothing, plays the wrong effect or leaves you unable to move: name the skill (a class block, routed to this character's port, not a formula fault).
- A crash: send the X2_*.tmp bundle from the temp folder.

### Logs
[JUNE_STATCALC] (`python tools/logs.py --tag JUNE_STATCALC`; `HIT`, `GFD`, `GFD2`, `REACT` for the units D6ElswordS and D6ElswordN)

## D6-Aisha - Aisha damage smoke (Dimension Witch (class 113))

### Dev console
`/stage 2` (inside a dungeon: jumps to stage 2, used for the boss)
`/invin 0` (turns invincibility off; type it for the incoming-damage steps 8 and 9 only)
No `/additem` is needed: `D6AishaS` and `D6AishaN` already wear the same top gear (+10), `D6AishaS` has sockets in every worn piece, and the skills are slotted.

### Loadout
Page 1 on both `D6AishaS` and `D6AishaN` (the same table). Keys not listed are empty (T / slot 9 stay empty).
| key | skill (nickname) | page / slot |
|---|---|---|
| A | Heavy Press (special active, grade 1) | slot 0 |
| S | Impact Hammer (special active, grade 1) | slot 1 |
| D | Screw Driver (special active, grade 2) | slot 2 |
| C | Impact Zone (special active, grade 3) | slot 3 |
| F | Fate Space (hyper active) | slot 4 |
| Q | Crazy Worm (special active, grade 2) | slot 5 |
| W | Air Stunner (special active, grade 1) | slot 6 |
| E | Energy Spurt (special active, grade 3) | slot 7 |

### Steps
Pause **2 seconds between every hit kind below**: the debug line is written once a second per attacker / monster pair, so a second hit inside
that second logs nothing. Hit each kind on the **same** monster, in this order, and write the order down (the log does not name the kind):
(1) normal: the plain Z combo, 3 Z presses; (2) command: one direction attack from the in-game command list (dash Z, jump Z or jump X; the one
your Heavy Press has); (3) special active: the skill on D; (4) special active: the skill on C; (5) special active: the skill on S;
(6) hyper active: F (fill the hyper gauge first by hitting monsters until the F skill is lit); (7) one more special active: Q.
1. Log in `D6AishaS` (the socketed unit). Open the character window (C) once and look at the attack numbers: the magical attack is the one the sockets raise.
2. Level-89 monster: enter **Elysion: Soless (Expert)** (dungeon 33892, monsters level 89) from the world map, stage 0. On the first monster you reach, do the 7 hit kinds above.
3. Leave. Enter **Elder Secret Dungeon (Hell)** (dungeon 33031), stage 0. On `Bunker` (the first monster, NPC 1466) do the 7 hit kinds.
4. Type `/stage 2`. On the boss (NPC 1470, `Punch Boss`) do the 7 hit kinds again.
5. Leave the dungeon. Log in `D6AishaN` (no sockets). Look at the character window: the magical attack is lower than D6AishaS's.
6. Repeat step 2 with `D6AishaN`: hit kinds (1) normal, (2) command, (3) D, (6) F only.
7. Repeat steps 3 and 4 with `D6AishaN`: hit kinds (1) normal, (3) D, (6) F only, on `Bunker` and on the boss.
8. Incoming damage, `D6AishaN`: type `/invin 0`, enter **Elder Secret Dungeon (Hell)** stage 0, stand in front of `Bunker` and take 3 hits (do not attack back), then `/stage 2` and take 3 hits from the boss.
9. Leave, enter **Elysion: Soless (Expert)** stage 0 with `/invin 0` still on, stand in front of a normal monster and take 3 hits. Then close the game.

### Expect
- On screen: every hit shows a number and the monster's HP bar moves. Normal monster (level 89): five to six digits per skill hit. `Bunker` (secret, k 1.0) takes less per point of attack than the normal monster (D1's run read `rate=0.5526` on it at level 104); the boss shows `state=0.3000` on `GFD2` (its 70 % reduce, D2b).
- Log lines, one `HIT` + `GFD` + `GFD2` per hit kind per monster (`python tools/logs.py --tag JUNE_STATCALC`): `HIT att=206 lv=99 def=NPCID dlv=LEVEL` (`att` is `D6AishaS`'s unit uid, and the plain unit's is `207`; `NPCID` is the monster's id, `LEVEL` its level).
- `rate` = defence / ( dlv x 39.35 + 258.65 + defence ), to 4 decimals, on every line; `march=` is March's old rate and is only for comparison.
- `GFD2`: `base` = `motion x drate`, and its last `dmg` equals the `HIT` line's `dmg` on every line.
- `D6AishaS` vs `D6AishaN`: on `GFD patk=IN>OUT` (or `matk=` for magic classes) the socketed unit's `out` is higher than the plain unit's `out` by the sockets' attack total (the weapon carries three +400 attack sockets).
- Incoming (`REACT def=` the unit's uid): `hp=BEFORE>AFTER` falls by exactly `dmg` unless a guard or shield is named on the line; `dlv=99` on the matching `HIT def=` line with the unit's uid and `rate` from the unit's own defence (the socketed unit's is higher).
- No `HIT` line with `dmg=0` unless its `GFD2` shows `motion=0`.
- Energy Spurt ignores defence by June's data (`defence=0`, `drate=1.0000`, a seven-digit `dmg`); that is the D2b result, not a failure. No normal hit takes the level-damage path in June's data (D2b), so no `LVD` line is expected.

### If not
- No `[JUNE_STATCALC]` lines at all: the debug define is off or the build is not the deployed one; say so and stop.
- `dmg=0` with `motion` above 0: send the `HIT` / `GFD` / `GFD2` triple (routes to D2b); a `rate` that does not match the formula: send the line (routes to D1); `REACT hp` not falling by `dmg`: send the REACT line (routes to D4a for a monster, D4b for the unit); a hit with no `GFD2` line is only the once-a-second throttle unless every hit kind is missing.
- A skill that does nothing, plays the wrong effect or leaves you unable to move: name the skill (a class block, routed to this character's port, not a formula fault).
- A crash: send the X2_*.tmp bundle from the temp folder.

### Logs
[JUNE_STATCALC] (`python tools/logs.py --tag JUNE_STATCALC`; `HIT`, `GFD`, `GFD2`, `REACT` for the units D6AishaS and D6AishaN)

## D6-Rena - Rena damage smoke (Grand Archer (class 103))

### Dev console
`/stage 2` (inside a dungeon: jumps to stage 2, used for the boss)
`/invin 0` (turns invincibility off; type it for the incoming-damage steps 8 and 9 only)
No `/additem` is needed: `D6RenaS` and `D6RenaN` already wear the same top gear (+10), `D6RenaS` has sockets in every worn piece, and the skills are slotted.

### Loadout
Page 1 on both `D6RenaS` and `D6RenaN` (the same table). Keys not listed are empty (T / slot 9 stay empty).
| key | skill (nickname) | page / slot |
|---|---|---|
| A | Arc Shot (active) | slot 0 |
| S | Freezing Arrow - Sting (special active, grade 1) | slot 1 |
| D | Wind Blast (special active, grade 2) | slot 2 |
| C | Aero Strafe (special active, grade 2) | slot 3 |
| F | Shooter's Destiny (hyper active) | slot 4 |
| Q | Ace in the Hole (special active, grade 3) | slot 5 |
| W | Stigma Shot (active) | slot 6 |

### Steps
Pause **2 seconds between every hit kind below**: the debug line is written once a second per attacker / monster pair, so a second hit inside
that second logs nothing. Hit each kind on the **same** monster, in this order, and write the order down (the log does not name the kind):
(1) normal: the plain Z combo, 3 Z presses; (2) command: one direction attack from the in-game command list (dash Z, jump Z or jump X; the one
your Arc Shot has); (3) special active: the skill on D; (4) special active: the skill on C; (5) special active: the skill on S;
(6) hyper active: F (fill the hyper gauge first by hitting monsters until the F skill is lit); (7) one more special active: Q.
1. Log in `D6RenaS` (the socketed unit). Open the character window (C) once and look at the attack numbers: the physical attack is the one the sockets raise.
2. Level-89 monster: enter **Elysion: Soless (Expert)** (dungeon 33892, monsters level 89) from the world map, stage 0. On the first monster you reach, do the 7 hit kinds above.
3. Leave. Enter **Elder Secret Dungeon (Hell)** (dungeon 33031), stage 0. On `Bunker` (the first monster, NPC 1466) do the 7 hit kinds.
4. Type `/stage 2`. On the boss (NPC 1470, `Punch Boss`) do the 7 hit kinds again.
5. Leave the dungeon. Log in `D6RenaN` (no sockets). Look at the character window: the physical attack is lower than D6RenaS's.
6. Repeat step 2 with `D6RenaN`: hit kinds (1) normal, (2) command, (3) D, (6) F only.
7. Repeat steps 3 and 4 with `D6RenaN`: hit kinds (1) normal, (3) D, (6) F only, on `Bunker` and on the boss.
8. Incoming damage, `D6RenaN`: type `/invin 0`, enter **Elder Secret Dungeon (Hell)** stage 0, stand in front of `Bunker` and take 3 hits (do not attack back), then `/stage 2` and take 3 hits from the boss.
9. Leave, enter **Elysion: Soless (Expert)** stage 0 with `/invin 0` still on, stand in front of a normal monster and take 3 hits. Then close the game.

### Expect
- On screen: every hit shows a number and the monster's HP bar moves. Normal monster (level 89): five to six digits per skill hit. `Bunker` (secret, k 1.0) takes less per point of attack than the normal monster (D1's run read `rate=0.5526` on it at level 104); the boss shows `state=0.3000` on `GFD2` (its 70 % reduce, D2b).
- Log lines, one `HIT` + `GFD` + `GFD2` per hit kind per monster (`python tools/logs.py --tag JUNE_STATCALC`): `HIT att=208 lv=99 def=NPCID dlv=LEVEL` (`att` is `D6RenaS`'s unit uid, and the plain unit's is `209`; `NPCID` is the monster's id, `LEVEL` its level).
- `rate` = defence / ( dlv x 39.35 + 258.65 + defence ), to 4 decimals, on every line; `march=` is March's old rate and is only for comparison.
- `GFD2`: `base` = `motion x drate`, and its last `dmg` equals the `HIT` line's `dmg` on every line.
- `D6RenaS` vs `D6RenaN`: on `GFD patk=IN>OUT` (or `matk=` for magic classes) the socketed unit's `out` is higher than the plain unit's `out` by the sockets' attack total (the weapon carries three +400 attack sockets).
- Incoming (`REACT def=` the unit's uid): `hp=BEFORE>AFTER` falls by exactly `dmg` unless a guard or shield is named on the line; `dlv=99` on the matching `HIT def=` line with the unit's uid and `rate` from the unit's own defence (the socketed unit's is higher).
- No `HIT` line with `dmg=0` unless its `GFD2` shows `motion=0`.
- Arrows are projectile (damage effect) hits, each arrow its own hit.

### If not
- No `[JUNE_STATCALC]` lines at all: the debug define is off or the build is not the deployed one; say so and stop.
- `dmg=0` with `motion` above 0: send the `HIT` / `GFD` / `GFD2` triple (routes to D2b); a `rate` that does not match the formula: send the line (routes to D1); `REACT hp` not falling by `dmg`: send the REACT line (routes to D4a for a monster, D4b for the unit); a hit with no `GFD2` line is only the once-a-second throttle unless every hit kind is missing.
- A skill that does nothing, plays the wrong effect or leaves you unable to move: name the skill (a class block, routed to this character's port, not a formula fault).
- A crash: send the X2_*.tmp bundle from the temp folder.

### Logs
[JUNE_STATCALC] (`python tools/logs.py --tag JUNE_STATCALC`; `HIT`, `GFD`, `GFD2`, `REACT` for the units D6RenaS and D6RenaN)

## D6-Raven - Raven damage smoke (Blade Master (class 106))

### Dev console
`/stage 2` (inside a dungeon: jumps to stage 2, used for the boss)
`/invin 0` (turns invincibility off; type it for the incoming-damage steps 8 and 9 only)
No `/additem` is needed: `D6RavenS` and `D6RavenN` already wear the same top gear (+10), `D6RavenS` has sockets in every worn piece, and the skills are slotted.

### Loadout
Page 1 on both `D6RavenS` and `D6RavenN` (the same table). Keys not listed are empty (T / slot 9 stay empty).
| key | skill (nickname) | page / slot |
|---|---|---|
| A | Finishing Slash (active) | slot 0 |
| S | Wolf Fang (special active, grade 1) | slot 1 |
| D | Giga Drive - Limiter (special active, grade 2) | slot 2 |
| C | Sharp Charge (special active, grade 3) | slot 3 |
| F | Extreme Blade (hyper active) | slot 4 |
| Q | Shoot Up Genocide (special active, grade 2) | slot 5 |
| W | Flash (special active, grade 1) | slot 6 |

### Steps
Pause **2 seconds between every hit kind below**: the debug line is written once a second per attacker / monster pair, so a second hit inside
that second logs nothing. Hit each kind on the **same** monster, in this order, and write the order down (the log does not name the kind):
(1) normal: the plain Z combo, 3 Z presses; (2) command: one direction attack from the in-game command list (dash Z, jump Z or jump X; the one
your Finishing Slash has); (3) special active: the skill on D; (4) special active: the skill on C; (5) special active: the skill on S;
(6) hyper active: F (fill the hyper gauge first by hitting monsters until the F skill is lit); (7) one more special active: Q.
1. Log in `D6RavenS` (the socketed unit). Open the character window (C) once and look at the attack numbers: the physical attack is the one the sockets raise.
2. Level-89 monster: enter **Elysion: Soless (Expert)** (dungeon 33892, monsters level 89) from the world map, stage 0. On the first monster you reach, do the 7 hit kinds above.
3. Leave. Enter **Elder Secret Dungeon (Hell)** (dungeon 33031), stage 0. On `Bunker` (the first monster, NPC 1466) do the 7 hit kinds.
4. Type `/stage 2`. On the boss (NPC 1470, `Punch Boss`) do the 7 hit kinds again.
5. Leave the dungeon. Log in `D6RavenN` (no sockets). Look at the character window: the physical attack is lower than D6RavenS's.
6. Repeat step 2 with `D6RavenN`: hit kinds (1) normal, (2) command, (3) D, (6) F only.
7. Repeat steps 3 and 4 with `D6RavenN`: hit kinds (1) normal, (3) D, (6) F only, on `Bunker` and on the boss.
8. Incoming damage, `D6RavenN`: type `/invin 0`, enter **Elder Secret Dungeon (Hell)** stage 0, stand in front of `Bunker` and take 3 hits (do not attack back), then `/stage 2` and take 3 hits from the boss.
9. Leave, enter **Elysion: Soless (Expert)** stage 0 with `/invin 0` still on, stand in front of a normal monster and take 3 hits. Then close the game.

### Expect
- On screen: every hit shows a number and the monster's HP bar moves. Normal monster (level 89): five to six digits per skill hit. `Bunker` (secret, k 1.0) takes less per point of attack than the normal monster (D1's run read `rate=0.5526` on it at level 104); the boss shows `state=0.3000` on `GFD2` (its 70 % reduce, D2b).
- Log lines, one `HIT` + `GFD` + `GFD2` per hit kind per monster (`python tools/logs.py --tag JUNE_STATCALC`): `HIT att=210 lv=99 def=NPCID dlv=LEVEL` (`att` is `D6RavenS`'s unit uid, and the plain unit's is `211`; `NPCID` is the monster's id, `LEVEL` its level).
- `rate` = defence / ( dlv x 39.35 + 258.65 + defence ), to 4 decimals, on every line; `march=` is March's old rate and is only for comparison.
- `GFD2`: `base` = `motion x drate`, and its last `dmg` equals the `HIT` line's `dmg` on every line.
- `D6RavenS` vs `D6RavenN`: on `GFD patk=IN>OUT` (or `matk=` for magic classes) the socketed unit's `out` is higher than the plain unit's `out` by the sockets' attack total (the weapon carries three +400 attack sockets).
- Incoming (`REACT def=` the unit's uid): `hp=BEFORE>AFTER` falls by exactly `dmg` unless a guard or shield is named on the line; `dlv=99` on the matching `HIT def=` line with the unit's uid and `rate` from the unit's own defence (the socketed unit's is higher).
- No `HIT` line with `dmg=0` unless its `GFD2` shows `motion=0`.
- Raven's Pride Victory raises `pride=` above 1.0000 on GFD lines (D2a); that is expected.

### If not
- No `[JUNE_STATCALC]` lines at all: the debug define is off or the build is not the deployed one; say so and stop.
- `dmg=0` with `motion` above 0: send the `HIT` / `GFD` / `GFD2` triple (routes to D2b); a `rate` that does not match the formula: send the line (routes to D1); `REACT hp` not falling by `dmg`: send the REACT line (routes to D4a for a monster, D4b for the unit); a hit with no `GFD2` line is only the once-a-second throttle unless every hit kind is missing.
- A skill that does nothing, plays the wrong effect or leaves you unable to move: name the skill (a class block, routed to this character's port, not a formula fault).
- A crash: send the X2_*.tmp bundle from the temp folder.

### Logs
[JUNE_STATCALC] (`python tools/logs.py --tag JUNE_STATCALC`; `HIT`, `GFD`, `GFD2`, `REACT` for the units D6RavenS and D6RavenN)

## D6-Eve - Eve damage smoke (Code Empress (class 109))

### Dev console
`/stage 2` (inside a dungeon: jumps to stage 2, used for the boss)
`/invin 0` (turns invincibility off; type it for the incoming-damage steps 8 and 9 only)
No `/additem` is needed: `D6EveS` and `D6EveN` already wear the same top gear (+10), `D6EveS` has sockets in every worn piece, and the skills are slotted.

### Loadout
Page 1 on both `D6EveS` and `D6EveN` (the same table). Keys not listed are empty (T / slot 9 stay empty).
| key | skill (nickname) | page / slot |
|---|---|---|
| A | Spitfire (active) | slot 0 |
| S | Electronic Field (special active, grade 1) | slot 1 |
| D | Assault Spear - Buster (special active, grade 2) | slot 2 |
| C | Dimension Link - Guardian (special active, grade 3) | slot 3 |
| F | Code: Thunderbolt (hyper active) | slot 4 |
| Q | Electronic Thunder (special active, grade 2) | slot 5 |
| W | Tesla Flare (special active, grade 1) | slot 6 |

### Steps
Pause **2 seconds between every hit kind below**: the debug line is written once a second per attacker / monster pair, so a second hit inside
that second logs nothing. Hit each kind on the **same** monster, in this order, and write the order down (the log does not name the kind):
(1) normal: the plain Z combo, 3 Z presses; (2) command: one direction attack from the in-game command list (dash Z, jump Z or jump X; the one
your Spitfire has); (3) special active: the skill on D; (4) special active: the skill on C; (5) special active: the skill on S;
(6) hyper active: F (fill the hyper gauge first by hitting monsters until the F skill is lit); (7) one more special active: Q.
1. Log in `D6EveS` (the socketed unit). Open the character window (C) once and look at the attack numbers: the magical attack is the one the sockets raise.
2. Level-89 monster: enter **Elysion: Soless (Expert)** (dungeon 33892, monsters level 89) from the world map, stage 0. On the first monster you reach, do the 7 hit kinds above.
3. Leave. Enter **Elder Secret Dungeon (Hell)** (dungeon 33031), stage 0. On `Bunker` (the first monster, NPC 1466) do the 7 hit kinds.
4. Type `/stage 2`. On the boss (NPC 1470, `Punch Boss`) do the 7 hit kinds again.
5. Leave the dungeon. Log in `D6EveN` (no sockets). Look at the character window: the magical attack is lower than D6EveS's.
6. Repeat step 2 with `D6EveN`: hit kinds (1) normal, (2) command, (3) D, (6) F only.
7. Repeat steps 3 and 4 with `D6EveN`: hit kinds (1) normal, (3) D, (6) F only, on `Bunker` and on the boss.
8. Incoming damage, `D6EveN`: type `/invin 0`, enter **Elder Secret Dungeon (Hell)** stage 0, stand in front of `Bunker` and take 3 hits (do not attack back), then `/stage 2` and take 3 hits from the boss.
9. Leave, enter **Elysion: Soless (Expert)** stage 0 with `/invin 0` still on, stand in front of a normal monster and take 3 hits. Then close the game.

### Expect
- On screen: every hit shows a number and the monster's HP bar moves. Normal monster (level 89): five to six digits per skill hit. `Bunker` (secret, k 1.0) takes less per point of attack than the normal monster (D1's run read `rate=0.5526` on it at level 104); the boss shows `state=0.3000` on `GFD2` (its 70 % reduce, D2b).
- Log lines, one `HIT` + `GFD` + `GFD2` per hit kind per monster (`python tools/logs.py --tag JUNE_STATCALC`): `HIT att=212 lv=99 def=NPCID dlv=LEVEL` (`att` is `D6EveS`'s unit uid, and the plain unit's is `213`; `NPCID` is the monster's id, `LEVEL` its level).
- `rate` = defence / ( dlv x 39.35 + 258.65 + defence ), to 4 decimals, on every line; `march=` is March's old rate and is only for comparison.
- `GFD2`: `base` = `motion x drate`, and its last `dmg` equals the `HIT` line's `dmg` on every line.
- `D6EveS` vs `D6EveN`: on `GFD patk=IN>OUT` (or `matk=` for magic classes) the socketed unit's `out` is higher than the plain unit's `out` by the sockets' attack total (the weapon carries three +400 attack sockets).
- Incoming (`REACT def=` the unit's uid): `hp=BEFORE>AFTER` falls by exactly `dmg` unless a guard or shield is named on the line; `dlv=99` on the matching `HIT def=` line with the unit's uid and `rate` from the unit's own defence (the socketed unit's is higher).
- No `HIT` line with `dmg=0` unless its `GFD2` shows `motion=0`.
- Electronic Field and Dimension Link - Guardian hit through damage effects; if one of them logs nothing, say which, it is a class block.

### If not
- No `[JUNE_STATCALC]` lines at all: the debug define is off or the build is not the deployed one; say so and stop.
- `dmg=0` with `motion` above 0: send the `HIT` / `GFD` / `GFD2` triple (routes to D2b); a `rate` that does not match the formula: send the line (routes to D1); `REACT hp` not falling by `dmg`: send the REACT line (routes to D4a for a monster, D4b for the unit); a hit with no `GFD2` line is only the once-a-second throttle unless every hit kind is missing.
- A skill that does nothing, plays the wrong effect or leaves you unable to move: name the skill (a class block, routed to this character's port, not a formula fault).
- A crash: send the X2_*.tmp bundle from the temp folder.

### Logs
[JUNE_STATCALC] (`python tools/logs.py --tag JUNE_STATCALC`; `HIT`, `GFD`, `GFD2`, `REACT` for the units D6EveS and D6EveN)

## D6-Chung - Chung damage smoke (Tactical Trooper (class 117))

### Dev console
`/stage 2` (inside a dungeon: jumps to stage 2, used for the boss)
`/invin 0` (turns invincibility off; type it for the incoming-damage steps 8 and 9 only)
No `/additem` is needed: `D6ChungS` and `D6ChungN` already wear the same top gear (+10), `D6ChungS` has sockets in every worn piece, and the skills are slotted.

### Loadout
Page 1 on both `D6ChungS` and `D6ChungN` (the same table). Keys not listed are empty (T / slot 9 stay empty).
| key | skill (nickname) | page / slot |
|---|---|---|
| A | Fatal Cannon (active) | slot 0 |
| S | Cannon Prison (special active, grade 1) | slot 1 |
| D | Bombing Artillery (special active, grade 2) | slot 2 |
| C | Heavy Field (special active, grade 3) | slot 3 |
| F | Satellite Rain (hyper active) | slot 4 |
| Q | Artillery Strike - Missile Shower (special active, grade 2) | slot 5 |
| W | Mortar (active) | slot 6 |

### Steps
Pause **2 seconds between every hit kind below**: the debug line is written once a second per attacker / monster pair, so a second hit inside
that second logs nothing. Hit each kind on the **same** monster, in this order, and write the order down (the log does not name the kind):
(1) normal: the plain Z combo, 3 Z presses; (2) command: one direction attack from the in-game command list (dash Z, jump Z or jump X; the one
your Fatal Cannon has); (3) special active: the skill on D; (4) special active: the skill on C; (5) special active: the skill on S;
(6) hyper active: F (fill the hyper gauge first by hitting monsters until the F skill is lit); (7) one more special active: Q.
1. Log in `D6ChungS` (the socketed unit). Open the character window (C) once and look at the attack numbers: the physical attack is the one the sockets raise.
2. Level-89 monster: enter **Elysion: Soless (Expert)** (dungeon 33892, monsters level 89) from the world map, stage 0. On the first monster you reach, do the 7 hit kinds above.
3. Leave. Enter **Elder Secret Dungeon (Hell)** (dungeon 33031), stage 0. On `Bunker` (the first monster, NPC 1466) do the 7 hit kinds.
4. Type `/stage 2`. On the boss (NPC 1470, `Punch Boss`) do the 7 hit kinds again.
5. Leave the dungeon. Log in `D6ChungN` (no sockets). Look at the character window: the physical attack is lower than D6ChungS's.
6. Repeat step 2 with `D6ChungN`: hit kinds (1) normal, (2) command, (3) D, (6) F only.
7. Repeat steps 3 and 4 with `D6ChungN`: hit kinds (1) normal, (3) D, (6) F only, on `Bunker` and on the boss.
8. Incoming damage, `D6ChungN`: type `/invin 0`, enter **Elder Secret Dungeon (Hell)** stage 0, stand in front of `Bunker` and take 3 hits (do not attack back), then `/stage 2` and take 3 hits from the boss.
9. Leave, enter **Elysion: Soless (Expert)** stage 0 with `/invin 0` still on, stand in front of a normal monster and take 3 hits. Then close the game.

### Expect
- On screen: every hit shows a number and the monster's HP bar moves. Normal monster (level 89): five to six digits per skill hit. `Bunker` (secret, k 1.0) takes less per point of attack than the normal monster (D1's run read `rate=0.5526` on it at level 104); the boss shows `state=0.3000` on `GFD2` (its 70 % reduce, D2b).
- Log lines, one `HIT` + `GFD` + `GFD2` per hit kind per monster (`python tools/logs.py --tag JUNE_STATCALC`): `HIT att=214 lv=99 def=NPCID dlv=LEVEL` (`att` is `D6ChungS`'s unit uid, and the plain unit's is `215`; `NPCID` is the monster's id, `LEVEL` its level).
- `rate` = defence / ( dlv x 39.35 + 258.65 + defence ), to 4 decimals, on every line; `march=` is March's old rate and is only for comparison.
- `GFD2`: `base` = `motion x drate`, and its last `dmg` equals the `HIT` line's `dmg` on every line.
- `D6ChungS` vs `D6ChungN`: on `GFD patk=IN>OUT` (or `matk=` for magic classes) the socketed unit's `out` is higher than the plain unit's `out` by the sockets' attack total (the weapon carries three +400 attack sockets).
- Incoming (`REACT def=` the unit's uid): `hp=BEFORE>AFTER` falls by exactly `dmg` unless a guard or shield is named on the line; `dlv=99` on the matching `HIT def=` line with the unit's uid and `rate` from the unit's own defence (the socketed unit's is higher).
- No `HIT` line with `dmg=0` unless its `GFD2` shows `motion=0`.
- Chung's cannon shells are projectile hits; Heavy Field and Mortar hit over time, so wait for the field to finish before the next cast.

### If not
- No `[JUNE_STATCALC]` lines at all: the debug define is off or the build is not the deployed one; say so and stop.
- `dmg=0` with `motion` above 0: send the `HIT` / `GFD` / `GFD2` triple (routes to D2b); a `rate` that does not match the formula: send the line (routes to D1); `REACT hp` not falling by `dmg`: send the REACT line (routes to D4a for a monster, D4b for the unit); a hit with no `GFD2` line is only the once-a-second throttle unless every hit kind is missing.
- A skill that does nothing, plays the wrong effect or leaves you unable to move: name the skill (a class block, routed to this character's port, not a formula fault).
- A crash: send the X2_*.tmp bundle from the temp folder.

### Logs
[JUNE_STATCALC] (`python tools/logs.py --tag JUNE_STATCALC`; `HIT`, `GFD`, `GFD2`, `REACT` for the units D6ChungS and D6ChungN)

## D6-Ara - Ara damage smoke (Yama Raja (class 119))

### Dev console
`/stage 2` (inside a dungeon: jumps to stage 2, used for the boss)
`/invin 0` (turns invincibility off; type it for the incoming-damage steps 8 and 9 only)
No `/additem` is needed: `D6AraS` and `D6AraN` already wear the same top gear (+10), `D6AraS` has sockets in every worn piece, and the skills are slotted.

### Loadout
Page 1 on both `D6AraS` and `D6AraN` (the same table). Keys not listed are empty (T / slot 9 stay empty).
| key | skill (nickname) | page / slot |
|---|---|---|
| A | Rakshasa Inferno (special active, grade 3) | slot 0 |
| S | Rakshasa Stance 2: Energy Burst (special active, grade 1) | slot 1 |
| D | Rakshasa Stance 4: Soul Escape (special active, grade 2) | slot 2 |
| C | Suppression Energy (special active, grade 3) | slot 3 |
| F | Hell Blast (hyper active) | slot 4 |
| Q | Vortex Energy (special active, grade 2) | slot 5 |
| W | Bind: Energy (special active, grade 1) | slot 6 |
| E | Energy Void (special active, grade 3) | slot 7 |

### Steps
Pause **2 seconds between every hit kind below**: the debug line is written once a second per attacker / monster pair, so a second hit inside
that second logs nothing. Hit each kind on the **same** monster, in this order, and write the order down (the log does not name the kind):
(1) normal: the plain Z combo, 3 Z presses; (2) command: one direction attack from the in-game command list (dash Z, jump Z or jump X; the one
your Rakshasa Inferno has); (3) special active: the skill on D; (4) special active: the skill on C; (5) special active: the skill on S;
(6) hyper active: F (fill the hyper gauge first by hitting monsters until the F skill is lit); (7) one more special active: Q.
1. Log in `D6AraS` (the socketed unit). Open the character window (C) once and look at the attack numbers: the physical attack is the one the sockets raise.
2. Level-89 monster: enter **Elysion: Soless (Expert)** (dungeon 33892, monsters level 89) from the world map, stage 0. On the first monster you reach, do the 7 hit kinds above.
3. Leave. Enter **Elder Secret Dungeon (Hell)** (dungeon 33031), stage 0. On `Bunker` (the first monster, NPC 1466) do the 7 hit kinds.
4. Type `/stage 2`. On the boss (NPC 1470, `Punch Boss`) do the 7 hit kinds again.
5. Leave the dungeon. Log in `D6AraN` (no sockets). Look at the character window: the physical attack is lower than D6AraS's.
6. Repeat step 2 with `D6AraN`: hit kinds (1) normal, (2) command, (3) D, (6) F only.
7. Repeat steps 3 and 4 with `D6AraN`: hit kinds (1) normal, (3) D, (6) F only, on `Bunker` and on the boss.
8. Incoming damage, `D6AraN`: type `/invin 0`, enter **Elder Secret Dungeon (Hell)** stage 0, stand in front of `Bunker` and take 3 hits (do not attack back), then `/stage 2` and take 3 hits from the boss.
9. Leave, enter **Elysion: Soless (Expert)** stage 0 with `/invin 0` still on, stand in front of a normal monster and take 3 hits. Then close the game.

### Expect
- On screen: every hit shows a number and the monster's HP bar moves. Normal monster (level 89): five to six digits per skill hit. `Bunker` (secret, k 1.0) takes less per point of attack than the normal monster (D1's run read `rate=0.5526` on it at level 104); the boss shows `state=0.3000` on `GFD2` (its 70 % reduce, D2b).
- Log lines, one `HIT` + `GFD` + `GFD2` per hit kind per monster (`python tools/logs.py --tag JUNE_STATCALC`): `HIT att=216 lv=99 def=NPCID dlv=LEVEL` (`att` is `D6AraS`'s unit uid, and the plain unit's is `217`; `NPCID` is the monster's id, `LEVEL` its level).
- `rate` = defence / ( dlv x 39.35 + 258.65 + defence ), to 4 decimals, on every line; `march=` is March's old rate and is only for comparison.
- `GFD2`: `base` = `motion x drate`, and its last `dmg` equals the `HIT` line's `dmg` on every line.
- `D6AraS` vs `D6AraN`: on `GFD patk=IN>OUT` (or `matk=` for magic classes) the socketed unit's `out` is higher than the plain unit's `out` by the sockets' attack total (the weapon carries three +400 attack sockets).
- Incoming (`REACT def=` the unit's uid): `hp=BEFORE>AFTER` falls by exactly `dmg` unless a guard or shield is named on the line; `dlv=99` on the matching `HIT def=` line with the unit's uid and `rate` from the unit's own defence (the socketed unit's is higher).
- No `HIT` line with `dmg=0` unless its `GFD2` shows `motion=0`.
- Note in the report whether Ara's berserk (awakening) was on for a hit; it changes her attack.

### If not
- No `[JUNE_STATCALC]` lines at all: the debug define is off or the build is not the deployed one; say so and stop.
- `dmg=0` with `motion` above 0: send the `HIT` / `GFD` / `GFD2` triple (routes to D2b); a `rate` that does not match the formula: send the line (routes to D1); `REACT hp` not falling by `dmg`: send the REACT line (routes to D4a for a monster, D4b for the unit); a hit with no `GFD2` line is only the once-a-second throttle unless every hit kind is missing.
- A skill that does nothing, plays the wrong effect or leaves you unable to move: name the skill (a class block, routed to this character's port, not a formula fault).
- A crash: send the X2_*.tmp bundle from the temp folder.

### Logs
[JUNE_STATCALC] (`python tools/logs.py --tag JUNE_STATCALC`; `HIT`, `GFD`, `GFD2`, `REACT` for the units D6AraS and D6AraN)

## D6-Elesis - Elesis damage smoke (Grand Master (class 120))

### Dev console
`/stage 2` (inside a dungeon: jumps to stage 2, used for the boss)
`/invin 0` (turns invincibility off; type it for the incoming-damage steps 8 and 9 only)
No `/additem` is needed: `D6ElesisS` and `D6ElesisN` already wear the same top gear (+10), `D6ElesisS` has sockets in every worn piece, and the skills are slotted.

### Loadout
Page 1 on both `D6ElesisS` and `D6ElesisN` (the same table). Keys not listed are empty (T / slot 9 stay empty).
| key | skill (nickname) | page / slot |
|---|---|---|
| A | Upper Slash (active) | slot 0 |
| S | Sonic Blade (special active, grade 1) | slot 1 |
| D | Juggernaut Buster (special active, grade 2) | slot 2 |
| C | Giga Blast (special active, grade 3) | slot 3 |
| F | Sword of Relics (hyper active) | slot 4 |
| Q | Divine Blade (special active, grade 2) | slot 5 |
| W | Cross Slash (special active, grade 1) | slot 6 |

### Steps
Pause **2 seconds between every hit kind below**: the debug line is written once a second per attacker / monster pair, so a second hit inside
that second logs nothing. Hit each kind on the **same** monster, in this order, and write the order down (the log does not name the kind):
(1) normal: the plain Z combo, 3 Z presses; (2) command: one direction attack from the in-game command list (dash Z, jump Z or jump X; the one
your Upper Slash has); (3) special active: the skill on D; (4) special active: the skill on C; (5) special active: the skill on S;
(6) hyper active: F (fill the hyper gauge first by hitting monsters until the F skill is lit); (7) one more special active: Q.
1. Log in `D6ElesisS` (the socketed unit). Open the character window (C) once and look at the attack numbers: the physical attack is the one the sockets raise.
2. Level-89 monster: enter **Elysion: Soless (Expert)** (dungeon 33892, monsters level 89) from the world map, stage 0. On the first monster you reach, do the 7 hit kinds above.
3. Leave. Enter **Elder Secret Dungeon (Hell)** (dungeon 33031), stage 0. On `Bunker` (the first monster, NPC 1466) do the 7 hit kinds.
4. Type `/stage 2`. On the boss (NPC 1470, `Punch Boss`) do the 7 hit kinds again.
5. Leave the dungeon. Log in `D6ElesisN` (no sockets). Look at the character window: the physical attack is lower than D6ElesisS's.
6. Repeat step 2 with `D6ElesisN`: hit kinds (1) normal, (2) command, (3) D, (6) F only.
7. Repeat steps 3 and 4 with `D6ElesisN`: hit kinds (1) normal, (3) D, (6) F only, on `Bunker` and on the boss.
8. Incoming damage, `D6ElesisN`: type `/invin 0`, enter **Elder Secret Dungeon (Hell)** stage 0, stand in front of `Bunker` and take 3 hits (do not attack back), then `/stage 2` and take 3 hits from the boss.
9. Leave, enter **Elysion: Soless (Expert)** stage 0 with `/invin 0` still on, stand in front of a normal monster and take 3 hits. Then close the game.

### Expect
- On screen: every hit shows a number and the monster's HP bar moves. Normal monster (level 89): five to six digits per skill hit. `Bunker` (secret, k 1.0) takes less per point of attack than the normal monster (D1's run read `rate=0.5526` on it at level 104); the boss shows `state=0.3000` on `GFD2` (its 70 % reduce, D2b).
- Log lines, one `HIT` + `GFD` + `GFD2` per hit kind per monster (`python tools/logs.py --tag JUNE_STATCALC`): `HIT att=218 lv=99 def=NPCID dlv=LEVEL` (`att` is `D6ElesisS`'s unit uid, and the plain unit's is `219`; `NPCID` is the monster's id, `LEVEL` its level).
- `rate` = defence / ( dlv x 39.35 + 258.65 + defence ), to 4 decimals, on every line; `march=` is March's old rate and is only for comparison.
- `GFD2`: `base` = `motion x drate`, and its last `dmg` equals the `HIT` line's `dmg` on every line.
- `D6ElesisS` vs `D6ElesisN`: on `GFD patk=IN>OUT` (or `matk=` for magic classes) the socketed unit's `out` is higher than the plain unit's `out` by the sockets' attack total (the weapon carries three +400 attack sockets).
- Incoming (`REACT def=` the unit's uid): `hp=BEFORE>AFTER` falls by exactly `dmg` unless a guard or shield is named on the line; `dlv=99` on the matching `HIT def=` line with the unit's uid and `rate` from the unit's own defence (the socketed unit's is higher).
- No `HIT` line with `dmg=0` unless its `GFD2` shows `motion=0`.
- Elesis's Way of the Sword gauge changes her damage rate (`wos=` on GFD2); note which stage it was at.

### If not
- No `[JUNE_STATCALC]` lines at all: the debug define is off or the build is not the deployed one; say so and stop.
- `dmg=0` with `motion` above 0: send the `HIT` / `GFD` / `GFD2` triple (routes to D2b); a `rate` that does not match the formula: send the line (routes to D1); `REACT hp` not falling by `dmg`: send the REACT line (routes to D4a for a monster, D4b for the unit); a hit with no `GFD2` line is only the once-a-second throttle unless every hit kind is missing.
- A skill that does nothing, plays the wrong effect or leaves you unable to move: name the skill (a class block, routed to this character's port, not a formula fault).
- A crash: send the X2_*.tmp bundle from the temp folder.

### Logs
[JUNE_STATCALC] (`python tools/logs.py --tag JUNE_STATCALC`; `HIT`, `GFD`, `GFD2`, `REACT` for the units D6ElesisS and D6ElesisN)
