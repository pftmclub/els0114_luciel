# Elesis CH6 walk sheet (2026-11-14)

Save backup: `data\db_backup\els_db_20261114_123230_pre_pre_ch6_walk_elesis.sql` (plus one per save edit). Log tag `[JUNE_CHAR]`; say "check logs" when done.

## 0. Setup, once

- Open the dev console with **END** (auth level 3). Default keys: A S D C = first row, F = A-5 (transcended only), Q W E R = second row, T = B-5 (transcended only).
- Options: skill cut-in show = on (you or party), so cut-in pictures appear.
- Console lines (all copy-ready; `g_pX2Game` lines work only inside a dungeon / field):

| Need | Paste | Effect |
|---|---|---|
| refill HP | `/hpf` | HP full |
| refill MP | `/mpf` | MP full (use between skills so the MP check reads clean) |
| low HP | `/mhp 1500` | your HP becomes 1500 (for Vigor heals, Madness, berserker) |
| awakening orbs | `g_pX2Game:GetMyUnit():SetHyperModeCount( 3 )` | 3 orbs; then press the awakening key yourself |
| next stage | `/stage 2` (then `/stage 3` ...) | jump to that stage of the dungeon you are in |
| monsters nearly dead | `/monhp 1` | every living monster to 1 HP, any hit kills |
| invincible OFF (normal) | `/invin 1` | NOTE `/invin 0` = invincible AND free MP: never use 0 during the MP / cool-time checks |

Units (all in the save; pick them by nickname at character select):

| Nickname | Class | State |
|---|---|---|
| wewe | Knight (8) | not transcended, 1 skill page(s) |
| SaberT | Saber Knight (29) | not transcended, 2 skill page(s) |
| eresisu | Pyro Knight (30) | not transcended, 2 skill page(s) |
| DKnight80 | Dark Knight (35) | not transcended, 2 skill page(s) |
| ElesisGM | Grand Master (120, transcended) | transcended, 3 skill page(s) |
| ElesisBH | Blazing Heart (121, transcended) | transcended, 3 skill page(s) |
| CAvenger80 | Crimson Avenger (125, transcended) | transcended, 3 skill page(s) |
| ElTrGM75 / ElTrBH75 / ElTrCA75 | Grand Master / Blazing Heart / Crimson Avenger | NOT transcended, level 75: for section 10 only |
| ElJobDK / ElJobSK / ElJobPK | Knight, level 14, 100 exp short of 15 | for section 8 |
| ElJobCA | Dark Knight, level 34, 100 exp short of 35 | for section 8 |

## 1. Every skill, unit by unit

How to run every row: go into a dungeon with the unit (any normal dungeon; stage 1 is enough). For each key in the table, top to bottom: press it near a monster. Cast 1: the skill plays, the class cut-in picture shows, the MP bar drops (the same amount each time). Cast 2 at once: nothing happens (the icon shows the cool-time clock). Wait for the clock, Cast 3: plays again, MP drops again. Type `/mpf` every 2-3 skills. After every skill you must be able to move; if you are stuck, name the skill and stop.
Cut-in picture set: Knight / Saber / Pyro / Dark Knight = Elesis; Grand Master = GM picture; Blazing Heart = BH picture; Crimson Avenger = CA picture. Mark `[x]` when it passed, write what you saw when it did not.

### 1.1 wewe (Knight (8))

**wewe - page 1**

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Spiral Blast | [ ] | [ ] | [ ] | [ ] |  |
| S | Dodge and Slash | [ ] | [ ] | [ ] | [ ] |  |
| D | Wild Shock | [ ] | [ ] | [ ] | [ ] |  |
| C | Sonic Assault | [ ] | [ ] | [ ] | [ ] |  |
| Q | Mega Buster | [ ] | [ ] | [ ] | [ ] |  |
| W | Unlimited Blade | [ ] | [ ] | [ ] | [ ] | long special: you must be able to move afterwards |
| E | Iron Body - Strong | [ ] | [ ] | [ ] | [ ] | buff icon / stance shows |
| R | (empty) | | | | | |

### 1.2 SaberT (Saber Knight (29))

**SaberT - page 1** (click page 1 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Rushing Sword | [ ] | [ ] | [ ] | [ ] |  |
| S | Saber - Extinction | [ ] | [ ] | [ ] | [ ] |  |
| D | Critical Dive | [ ] | [ ] | [ ] | [ ] |  |
| C | Power Break | [ ] | [ ] | [ ] | [ ] |  |
| Q | Leap | [ ] | [ ] | [ ] | [ ] |  |
| W | Sword of Judgement | [ ] | [ ] | [ ] | [ ] |  |
| E | Spiral Blast | [ ] | [ ] | [ ] | [ ] |  |
| R | Dodge and Slash | [ ] | [ ] | [ ] | [ ] |  |

**SaberT - page 2** (click page 2 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Wild Shock | [ ] | [ ] | [ ] | [ ] |  |
| S | Sonic Assault | [ ] | [ ] | [ ] | [ ] |  |
| D | Mega Buster | [ ] | [ ] | [ ] | [ ] |  |
| C | Unlimited Blade | [ ] | [ ] | [ ] | [ ] | long special: you must be able to move afterwards |
| Q | Iron Body - Strong | [ ] | [ ] | [ ] | [ ] | buff icon / stance shows |
| W | (empty) | | | | | |
| E | (empty) | | | | | |
| R | (empty) | | | | | |

### 1.3 eresisu (Pyro Knight (30))

**eresisu - page 1** (click page 1 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Explosive Fist | [ ] | [ ] | [ ] | [ ] |  |
| S | Sword Fire | [ ] | [ ] | [ ] | [ ] |  |
| D | Scaldic Sword | [ ] | [ ] | [ ] | [ ] |  |
| C | Avatar of Fire | [ ] | [ ] | [ ] | [ ] |  |
| Q | Fireplay | [ ] | [ ] | [ ] | [ ] |  |
| W | Infernal Blade | [ ] | [ ] | [ ] | [ ] |  |
| E | Spiral Blast | [ ] | [ ] | [ ] | [ ] |  |
| R | Dodge and Slash | [ ] | [ ] | [ ] | [ ] |  |

**eresisu - page 2** (click page 2 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Wild Shock | [ ] | [ ] | [ ] | [ ] |  |
| S | Sonic Assault | [ ] | [ ] | [ ] | [ ] |  |
| D | Mega Buster | [ ] | [ ] | [ ] | [ ] |  |
| C | Unlimited Blade | [ ] | [ ] | [ ] | [ ] | long special: you must be able to move afterwards |
| Q | Iron Body - Strong | [ ] | [ ] | [ ] | [ ] | buff icon / stance shows |
| W | (empty) | | | | | |
| E | (empty) | | | | | |
| R | (empty) | | | | | |

### 1.4 DKnight80 (Dark Knight (35))

**DKnight80 - page 1** (click page 1 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Dark Wave | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| S | Ejection Buster | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| D | Brutal Cutter | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| C | Injection Stigma | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| Q | Shade Scratch | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| W | Burning Phantom | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| E | Spiral Blast | [ ] | [ ] | [ ] | [ ] |  |
| R | Dodge and Slash | [ ] | [ ] | [ ] | [ ] |  |

**DKnight80 - page 2** (click page 2 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Wild Shock | [ ] | [ ] | [ ] | [ ] |  |
| S | Sonic Assault | [ ] | [ ] | [ ] | [ ] |  |
| D | Mega Buster | [ ] | [ ] | [ ] | [ ] |  |
| C | Unlimited Blade | [ ] | [ ] | [ ] | [ ] | long special: you must be able to move afterwards |
| Q | Iron Body - Strong | [ ] | [ ] | [ ] | [ ] | buff icon / stance shows |
| W | (empty) | | | | | |
| E | (empty) | | | | | |
| R | (empty) | | | | | |

### 1.5 ElesisGM (Grand Master (120, transcended))

**ElesisGM - page 1** (click page 1 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | [Enhanced] Sword of Victory | [ ] | [ ] | [ ] | [ ] | enhanced |
| S | [Enhanced] Provoke | [ ] | [ ] | [ ] | [ ] | enhanced |
| D | War Prelude | [ ] | [ ] | [ ] | [ ] | also once in the air (jump, then press): the air version plays |
| C | [Enhanced] Juggernaut Buster | [ ] | [ ] | [ ] | [ ] | enhanced |
| F | Sword of Relics (hyper) | [ ] | [ ] | [ ] | [ ] | awakening first (section 0 line), then this key; hyper cut-in picture; base hyper |
| Q | [Enhanced] Drastic Charge | [ ] | [ ] | [ ] | [ ] | enhanced |
| W | [Enhanced] Sonic Blade | [ ] | [ ] | [ ] | [ ] | enhanced |
| E | Rushing Sword | [ ] | [ ] | [ ] | [ ] |  |
| R | Saber - Extinction | [ ] | [ ] | [ ] | [ ] |  |
| T | Upper Slash | [ ] | [ ] | [ ] | [ ] | transcended skill (B-5 slot) |

**ElesisGM - page 2** (click page 2 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Critical Dive | [ ] | [ ] | [ ] | [ ] |  |
| S | Power Break | [ ] | [ ] | [ ] | [ ] |  |
| D | Leap | [ ] | [ ] | [ ] | [ ] |  |
| C | Sword of Judgement | [ ] | [ ] | [ ] | [ ] |  |
| Q | Spiral Blast | [ ] | [ ] | [ ] | [ ] |  |
| W | Dodge and Slash | [ ] | [ ] | [ ] | [ ] | base: a hit gives back the MP it cost, blue MP number (section 6c) |
| E | Wild Shock | [ ] | [ ] | [ ] | [ ] |  |
| R | Sonic Assault | [ ] | [ ] | [ ] | [ ] |  |

**ElesisGM - page 3** (click page 3 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Mega Buster | [ ] | [ ] | [ ] | [ ] |  |
| S | Unlimited Blade | [ ] | [ ] | [ ] | [ ] | long special: you must be able to move afterwards |
| D | Iron Body - Strong | [ ] | [ ] | [ ] | [ ] | buff icon / stance shows |
| C | Cross Slash | [ ] | [ ] | [ ] | [ ] | transcended skill |
| Q | Divine Blade | [ ] | [ ] | [ ] | [ ] | transcended skill |
| W | Giga Blast | [ ] | [ ] | [ ] | [ ] | transcended skill |
| E | (empty) | | | | | |
| R | (empty) | | | | | |

### 1.6 ElesisBH (Blazing Heart (121, transcended))

**ElesisBH - page 1** (click page 1 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Eternal Fire | [ ] | [ ] | [ ] | [ ] |  |
| S | [Enhanced] Red Lotus Sword | [ ] | [ ] | [ ] | [ ] | enhanced |
| D | Blazing Dance | [ ] | [ ] | [ ] | [ ] |  |
| C | Blazing Wing | [ ] | [ ] | [ ] | [ ] |  |
| F | Ancient Fire (hyper) | [ ] | [ ] | [ ] | [ ] | awakening first (section 0 line), then this key; hyper cut-in picture; base hyper |
| Q | [Enhanced] Flame Rose | [ ] | [ ] | [ ] | [ ] | enhanced; Flame Rose summon (blossoms) appears |
| W | Fire Wall | [ ] | [ ] | [ ] | [ ] | Fire Wall summon appears |
| E | Explosive Fist | [ ] | [ ] | [ ] | [ ] |  |
| R | Sword Fire | [ ] | [ ] | [ ] | [ ] |  |
| T | Flame Protection | [ ] | [ ] | [ ] | [ ] | transcended skill (B-5 slot) |

**ElesisBH - page 2** (click page 2 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Scaldic Sword | [ ] | [ ] | [ ] | [ ] |  |
| S | Avatar of Fire | [ ] | [ ] | [ ] | [ ] |  |
| D | Fireplay | [ ] | [ ] | [ ] | [ ] |  |
| C | Infernal Blade | [ ] | [ ] | [ ] | [ ] |  |
| Q | Spiral Blast | [ ] | [ ] | [ ] | [ ] |  |
| W | Dodge and Slash | [ ] | [ ] | [ ] | [ ] |  |
| E | Wild Shock | [ ] | [ ] | [ ] | [ ] |  |
| R | Sonic Assault | [ ] | [ ] | [ ] | [ ] |  |

**ElesisBH - page 3** (click page 3 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Mega Buster | [ ] | [ ] | [ ] | [ ] |  |
| S | Unlimited Blade | [ ] | [ ] | [ ] | [ ] | long special: you must be able to move afterwards |
| D | Iron Body - Strong | [ ] | [ ] | [ ] | [ ] | buff icon / stance shows |
| C | Rising Burst | [ ] | [ ] | [ ] | [ ] | transcended skill |
| Q | Ignis Crusher | [ ] | [ ] | [ ] | [ ] | transcended skill |
| W | Soul Ignition | [ ] | [ ] | [ ] | [ ] | transcended skill |
| E | (empty) | | | | | |
| R | (empty) | | | | | |

### 1.7 CAvenger80 (Crimson Avenger (125, transcended))

**CAvenger80 - page 1** (click page 1 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Brandish Breaker | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| S | Shadow Edge | [ ] | [ ] | [ ] | [ ] | also once with nothing in range: it must end by itself (time-out); HP drops a little (HP cost) |
| D | Blood Cutter | [ ] | [ ] | [ ] | [ ] | blade summon appears (ignore State.log NPC Motion Error: June data); HP drops a little (HP cost) |
| C | Shade Illusion | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| F | [Enhanced] Fate Smash (hyper) | [ ] | [ ] | [ ] | [ ] | awakening first (section 0 line), then this key; hyper cut-in picture; enhanced hyper; HP drops a little (HP cost) |
| Q | Assault Strike | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| W | Dread Weapon | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| E | Chaser Sword | [ ] | [ ] | [ ] | [ ] | transcended skill; HP drops a little (HP cost) |
| R | Shade | [ ] | [ ] | [ ] | [ ] | its Shade appears next to you; HP drops a little (HP cost) |
| T | Blood Sting | [ ] | [ ] | [ ] | [ ] | transcended skill (B-5 slot); HP drops a little (HP cost) |

**CAvenger80 - page 2** (click page 2 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Blood Falls | [ ] | [ ] | [ ] | [ ] | transcended skill; HP drops a little (HP cost) |
| S | Dark Wave | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| D | Ejection Buster | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| C | Brutal Cutter | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| Q | Injection Stigma | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| W | Shade Scratch | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| E | Burning Phantom | [ ] | [ ] | [ ] | [ ] | HP drops a little (HP cost) |
| R | Spiral Blast | [ ] | [ ] | [ ] | [ ] |  |

**CAvenger80 - page 3** (click page 3 beside the skill bar)

| Key | Skill | Cast 1 (MP drops) | Cast 2 at once (refused: cool time) | Cast 3 after the clock (works) | Can move after | Watch for |
|---|---|---|---|---|---|---|
| A | Dodge and Slash | [ ] | [ ] | [ ] | [ ] |  |
| S | Wild Shock | [ ] | [ ] | [ ] | [ ] |  |
| D | Sonic Assault | [ ] | [ ] | [ ] | [ ] |  |
| C | Mega Buster | [ ] | [ ] | [ ] | [ ] |  |
| Q | Unlimited Blade | [ ] | [ ] | [ ] | [ ] | long special: you must be able to move afterwards |
| W | Iron Body - Strong | [ ] | [ ] | [ ] | [ ] | buff icon / stance shows |
| E | (empty) | | | | | |
| R | (empty) | | | | | |


## 2. Way of the Sword (ElesisGM, ElesisBH, DKnight80)

Run on each of the three. In a dungeon, look at the gauge under your HP.

| Step | Do | Expect |
|---|---|---|
| a | hit a monster with Z-only combos (Z, ZZ, ZZZ, ZZZZ, dash then Z) | gauge moves one way on every hit |
| b | hit with X-only combos (X, XX, XXX, dash then X) | gauge moves the other way |
| c | keep hitting one way until the bar is full at that end | the mode starts (screen effect, bar changes); note which end and what it did |
| d | in that mode hit with the same type, then the opposite type | the opposite type drains the point faster |
| e | repeat c for the OTHER end | the other mode starts (you now have seen Vigor and Destruction) |
| f | while in a mode type `/stage 2` | the mode and the point survive the stage change |
| g | let the point run out (stop hitting) | the mode ends with its end effect, you can move |
| h | finish or leave the dungeon, back in the village | point kept, no stuck glow or effect in the village |

DKnight80 only: type `/mhp 1500` first; in Vigor your hits heal you (green number plus a blood effect); in Destruction your damage is higher.

## 3. The four summons

| Nickname | Skill (key) | Expect |
|---|---|---|
| ElesisBH | Flame Rose (page 1, Q): hit the monster with it | rose / blossom summons appear and fire (up to 3 blossoms) |
| ElesisBH | Fire Wall (page 1, W) | a wall of flame stays and burns monsters |
| ElesisBH | Fire Wall again (Mistress of Inferno is learned) | the wall also brings the Fire King, a bigger flame figure |
| CAvenger80 | Blood Cutter (page 1, D) | the blade summon appears and slashes (ignore State.log NPC Motion Error) |
| ElesisBH | Blooming Flame passive: keep a fire buff running until it ends | a blossom spawns at the end |

## 4. Blood sucking and Blood Runaway (DKnight80, CAvenger80)

| Step | Nickname | Do | Expect |
|---|---|---|---|
| a | DKnight80 | `/mhp 1500`, enter Vigor (section 2), hit monsters | HP goes up on hits with a green number and a blood effect |
| b | DKnight80 | Judgement passive: hit monsters with normal attacks outside special skills | now and then the target bleeds (red effect) and you heal a little |
| c | CAvenger80 | `/mhp 1500`, hit monsters with normal attacks (not specials), about one hit per second | Overflowing Blood stacks up to 4 (buff icon counts) |
| d | CAvenger80 | keep hitting past 4 stacks, one hit per cool time | the stacking buff gives way to the damage buff (NOT SEEN in CH4c: watch this one) |
| e | CAvenger80 | get hit by a monster | Revenge of Blood: you steal HP from the attacker, its number in white, yours in green |

## 5. Passives (skill tree on each unit: every learned passive must show its icon and level)

| Nickname | Passive | How to see it |
|---|---|---|
| ElesisGM | Advanced Knight Mastery | get knocked down: you stand up at once |
| ElesisGM | Enhanced Agility | dash is a little faster than on SaberT |
| ElesisGM | Awakened Will: Grand Master | dash far from monsters then fight exactly one: a buff icon each time |
| ElesisGM | The Knight-Captain (Dignity) | buff icon on yourself after a skill; needed by War Prelude's air version |
| ElesisBH | Fire Stigma | monsters you hit get stacking stigma (up to 5), damage grows |
| ElesisBH | Concentration / Body of Fire | skill tree shows them; magic crit now and then |
| ElesisBH | Mistress of Inferno | see section 3, Fire Wall |
| DKnight80 | Dark Force / Judgement / Revenge of Blood | section 4 |
| CAvenger80 | Condemnation | skill tree shows it; no popup |
| CAvenger80 | Madness | `/mhp 800` (under 30% HP): berserk buff icon appears; `/hpf`: it goes |
| CAvenger80 | Awakened Will: Crimson Avenger | `/mhp 2000` (under 65%), fight 2-3 monsters at once: berserker buff, stacks with the enemy count |

## 6. Three checks CH4 did not reach

| Id | Nickname | Do | Expect |
|---|---|---|---|
| 6a | CAvenger80 | same as 4d | past 4 Overflowing Blood stacks the buff swaps to the damage buff |
| 6b | CAvenger80 | enter Vigor (section 2), then `/mhp 2000` (under 65%) | the second berserker buff appears and the first is gone |
| 6c | ElesisGM | page 2, key W is base Dodge and Slash; Waltz of the Sword is learned; cast it into a monster | a hit gives back the MP the skill spent: a blue MP number |

## 7. One dungeon per job

| Nickname | Do | Expect |
|---|---|---|
| wewe | clear one dungeon (`/stage N` and `/monhp 1` to shorten); once with 3 awakening orbs on the transcended ones | no crash, no stuck state; the end-of-dungeon picture shows this class (blank or another class = tell me) |
| SaberT | clear one dungeon (`/stage N` and `/monhp 1` to shorten); once with 3 awakening orbs on the transcended ones | no crash, no stuck state; the end-of-dungeon picture shows this class (blank or another class = tell me) |
| eresisu | clear one dungeon (`/stage N` and `/monhp 1` to shorten); once with 3 awakening orbs on the transcended ones | no crash, no stuck state; the end-of-dungeon picture shows this class (blank or another class = tell me) |
| DKnight80 | clear one dungeon (`/stage N` and `/monhp 1` to shorten); once with 3 awakening orbs on the transcended ones | no crash, no stuck state; the end-of-dungeon picture shows this class (blank or another class = tell me) |
| ElesisGM | clear one dungeon (`/stage N` and `/monhp 1` to shorten); once with 3 awakening orbs on the transcended ones | no crash, no stuck state; the end-of-dungeon picture shows this class (blank or another class = tell me) |
| ElesisBH | clear one dungeon (`/stage N` and `/monhp 1` to shorten); once with 3 awakening orbs on the transcended ones | no crash, no stuck state; the end-of-dungeon picture shows this class (blank or another class = tell me) |
| CAvenger80 | clear one dungeon (`/stage N` and `/monhp 1` to shorten); once with 3 awakening orbs on the transcended ones | no crash, no stuck state; the end-of-dungeon picture shows this class (blank or another class = tell me) |

## 8. Job-change dialogs on fresh units

| Nickname | Do | Expect |
|---|---|---|
| ElJobDK | in a dungeon / field kill one monster (level 15) | the 1st class-change guide opens with three classes |
| ElJobDK | pick Dark Knight, press Accept, then paste `/qdone 35569` `/qdone 35570` `/qdone 35571` `/qdone 35572` | class becomes Dark Knight |
| ElJobSK | same guide, pick Saber Knight, Accept, then `/qdone 35320` `/qdone 35321` `/qdone 35322` `/qdone 35323` | class becomes Saber Knight |
| ElJobPK | same guide, pick Pyro Knight, Accept, then `/qdone 35330` `/qdone 35331` `/qdone 35332` `/qdone 35333` | class becomes Pyro Knight |
| ElJobCA | kill one monster (level 35) | the 2nd guide opens: Dark Knight line leads to Crimson Avenger |
| ElJobCA | pick Crimson Avenger, Accept (start NPC 10025), then `/qdone 35573` `/qdone 35574` `/qdone 35575` `/qdone 35576` `/qdone 35577` | class becomes Crimson Avenger |
| all four | leave to character select and back in | the new class stays |

## 9. Creation screen

Character select -> create new -> Elesis. Click all six class buttons in turn, back out without creating. Expect: each shows its preview movie (Knight, Saber, Pyro, Grand Master, Blazing Heart, third line), no black or empty preview, no popup. A third-line click may do nothing.

## 10. Becoming transcended (ElTrGM75, ElTrBH75, ElTrCA75)

One unit at a time, in the village:

| Step | Paste / Do | Expect |
|---|---|---|
| a | `/tc 1` | accepted, no popup of refusal |
| b | `/qdone 42030` | quest done |
| c | `/additem 288493 1` and use the book from the bag | the guide popup opens with THIS class's picture |
| d | open the skill tree | new transcendence rows are there (no empty icons) |
| e | look at the HUD | the transcended portrait shows |
| f | go to character select and log back in | still transcended: portrait, rows, F / T slots open |

## 11. Leave and return

Last: from a dungeon go back to the village on each unit once; no stuck glow, bars normal.
