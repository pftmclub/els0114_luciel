# June character Lu/Ciel (Stage 8) - Phase CH2a2 - Duo skills: wrong skill cast, cool time not kept

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH2a2` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH2a (`fe073ac8`, built, play-tested once) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` (+ `_SKILL_DEBUG`, still on) |
| Log tag | `[JUNE_CHAR]` (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL SKILL"`) |
| Review | none of its own; the CH2b review covers it (CH2b is blocked on this fix passing its play-test) |
| Hand-off | now |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH2a - Done (2026-11-15)"
```
Then, in `X2Lib/X2GULuciel.cpp`: `SpecialAttackEventProcess` (~10318), `SpecialAttackUse` (~10487), `CanUseSpecialAttack` (~11061, the lookup that sets `m_iChosenSkillID` / `m_iChosenSkillSlotIndex`), `StateChangeBySkillID` (~657), `ReservedSkillStart` (~10842), and CH2a's setter cases in `SetEquippedSkillFuncBySkillSlotIndex`. In `X2GUUser.cpp`: `StateChangeBySkillSlot` (~48364), `CountReservedSkillUse` (~48040), `SetEquippedSkillLevelCommon` (~7125).

## What the user saw (play-test 1, 2026-11-15, both `LuSkillA` and `LuSkillB`)

- **Wrong skill cast, and it changes every time:** Nether Shredder plays Soul Infest, Soul Infest plays Evil Claw, Hunting Prey (Haunting Beat) plays Nether Shredder's fists and bind. The user's reading: the state carries another skill's id, which differs per cast.
- **Cool time not kept properly**, "most of the time" (not tracked per skill). Seen in the log: `LuSkillB` Evil Claw upgrade (30029) cast at 17:21:45 and again at 17:21:48.
- Saves: `LuSkillA` (unit 191: bases at level 1, upgrades level 0, page 1 A Triple Shot / S Stomp / D Flick Shot / C Heavy Hit, Q Evil Claw / W Soul Infest / E Nether Shredder / R Hunting Prey; page 2 the banks swapped; page 3 auras 208 / 30301) and `LuSkillB` (unit 192, the upgrade ids). Class 200, level 70.

## What CH2a's logs already show (do not redo)

- The setter cases are right: June's jump table (`32b4a850` / `32b4a920`) maps 30001 -> `32b46bb3`, 30002 -> `c21`, 30003 -> `cce`, 30004 -> `df4` (Nether Shredder), 30005 -> `d3c` (Evil Claw), 30006 -> `d79` (Soul Infest), as written. `[JUNE_CHAR] LUCIEL SKILL equip` lines give the right slot / state pairs (A 50 52 54 56, B 60 62 64 66).
- `ES_CREATE ... ownerState=N` lines in `offline_server.log` match the key pressed in every cast the logs let me pair (Q/W/E -> 60/62/64). So the state *entered* looked right for those; the wrong cast is what the player sees for other presses, or the state changes after entry.
- `SpecialAttackUse` -> `ReserveSkillUse` -> `StateChangeBySkillID` (Ciel's states by skill id) -> else `StateChangeBySkillSlot( m_iChosenSkillSlotIndex )`; `ReservedSkillStart` pays the cool time with `GetNowStateSkillID()`. Both read per-cast values (`m_iChosenSkillID`, `m_iChosenSkillSlotIndex`, the Now state's `m_eSkillID`) that a stale or wrong value would break the same way the user describes.
- Skill types: June marks Triple Shot / Stomp / Flick Shot / Evil Claw as switching skills (types 14 / 15, each starts from one body and switches to the other), Nether Shredder as type 16 (needs a full combination gauge). `DUO_TYPE` in `Luciel_Duo.lua`: Triple Shot and Flick Shot `DT_MINOR`, Stomp and Evil Claw `DT_MAJOR`, Soul Infest and Nether Shredder `DT_BOTH`. A wrong body, or a refused press, may be what "wrong skill" is.

## Task

Find why a key press runs another skill's state and why the cool time is wrong, and fix it. Start by adding a log line at every skill use (key slot, `m_iChosenSkillID`, `m_iChosenSkillSlotIndex`, state entered, the Now state's `m_eSkillID`, body, cool time set) under `_SKILL_DEBUG`, since the first play-test had none. Candidates, in the order the code reaches them: the lookup in `CanUseSpecialAttack` (does it pick the pressed slot's skill, with the slot index in June's 0-9 numbering for bank B), `CX2GUUser` stale `m_iChosenSkillID` / slot kept across a use (`SpecialAttackEventProcess` restores them for use condition 3 / 5), `StateChangeBySkillSlot` with the hyper tables, the switching-skill / body rules (`IsMinorUnitActive`), and whether `SpecialAttackCooldown` is reached for every state (extra states 191 / 266 / 267 take their id from `m_iChosenSkillID` / the setter). Check every call against June's listing, not the digest.

Done gate: the user's three wrong casts (Nether Shredder, Soul Infest, Hunting Prey, on both characters, both banks) play their own skill, and a second press inside the cool time is refused on every CH2a skill; the new log lines show skill id, state and cool time per use. Hand off the same loadouts as CH2a (the saves are ready) with a table per key, plus the CH1 owed point-on-hit check.
