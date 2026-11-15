# June damage and character stat calculation - phased plan (`june-stat-calc`)

Backlog Stage 33 (`JUNE_PORT_BACKLOG_ORDER.md`, "Stage 33: June damage and character stat calculation"). Planned 2026-11-12 from the brief
`kickoffs/june-backlog/stage33-plan.md`. Plan key `june-stat-calc`, branch `main`, kickoffs `kickoffs/june-stat-calc/`.

## Context

- **The rule behind the plan (user, 2026-10-27).** Stat and damage math is June's whole calculation, never a March / June merge. A June
  seed, curve or data table read by a March formula is a bug, not a port.
- **The trigger for running it now (user, 2026-11-12).** Monsters in some secret dungeons take 0 or almost no damage. The planning session
  found the cause (see "Measurement"): June's monster defence data is already live (`NewNPCStat.lua`, ExtraNPCStat parity), but
  `CX2GameUnit::GetFinalDamage` still turns defence into a reduction with March's straight-line formula. June's defence numbers are built
  for June's curve; in March's formula they pass 100% and the hit is multiplied by 0.
- **Two tracks.**
  - **Damage track (D0-D6): runs now.** It is a playability fault on a reachable path, so the scope rule lets it run ahead of the
    character ports. D1 is the fix for the 0-damage monsters and depends on nothing.
  - **Stat-sheet and resource track (S0-S4, R1): code after the character ports** (Stages 7 and 8; user, 2026-10-27), so every
    character's numbers are tested on June's formulas at once. Its map phase S0 is read-only and can run any time.
- **`GetFinalDamage` is the model of the problem.** March's body (`X2GameUnit.cpp:2705-3999`) carries 16 `SERV_IRUHADEV_` flags from
  several plans (buff, socket parity, enchant parity, effect C2-C10, skill memo, character hooks). Each added one June block. No phase ever
  compared the whole function with June's. NPC `DamageReact` carries 23.

## Rules

1. **June's whole body.** A function this plan touches ends with June's whole body inside March's own function under the phase flag,
   March's body in `#else`. Older flags that already hold a June block inside it stay as they are; the new flag wraps only what was still
   March. A block the map marks `June` is re-read against the exe, not re-ported.
2. **The map decides, not memory.** A phase ports only rows the map marks `March`, `mixed` or `absent`. A row is marked `June` only after
   a read of the exe in this plan (the older Done note is a lead, not proof).
3. **Every reader of a number moves together.** When a phase changes how a value is produced (a curve, a seed, a unit), it lists every
   reader and refresher of that value (per hit, per event, UI, emulator) and ports them in the same phase, or proves they already agree.
4. **No tuning.** No clamp, cap, scale or "safety" value that June does not have. If June's result looks wrong (a negative defence, a
   reduction above 1), port what June does with it and add a `JUNE_BUGS.md` row. A result June never showed is a port gap: find it.
5. **Class-specific material goes to the character ports.** The per-class `SetDamageExceptionProcess` overrides, class blocks inside a
   shared function and class passives are identified here and handed to the class's plan or `JUNE2017_CHARACTER_PORT_CHECKLIST.md`.
   Inside a shared June body, an unported class's block is guarded with a one-line `stand-in: <class> not ported` comment.
6. **Numbers are proven twice.** Each formula phase runs June's own code for the same inputs (Unicorn on `unpacked_x2.exe`, or the x87
   trace of `disasm_range.py --x87` when a block cannot be emulated) against the port's expression, and reads the per-hit log from a
   play-test. The comparison script and its input table are committed under `june2017_re/statcalc/`.
7. **One debug flag for the plan:** `SERV_IRUHADEV_JUNE_STATCALC_DEBUG`. It stays on through the damage track and is retired by the D6
   review; the stat track turns it on again in S1 and Z retires it.
8. Standing rules apply unchanged: one branch `main`, explicit pathspecs, CP949 sources through `bytepatch.py`, no hardcoded enum
   literals, no `June` in names, one-line comments, `junediverge.py` gate, June bugs kept verbatim with a `JUNE_BUGS.md` row.

## Starting context

- **March code.** `X2Lib/X2GameUnit.cpp`: `CalcLevelFactorForFinalDamage` (:1938), `GetFinalDamage` (:2705-3999; defence sum :3172-3317,
  the full-defence constants :3323-3357, the rate :3403-3418, the level-damage path :3421-3531, the damage rate :3535),
  `SetDamageExceptionProcess` (:19185), `GetAdditionalAttackPowerRateByType` (:19912). `X2Lib/X2GUNPC.cpp`: `DamageReact` (:9728-11690),
  `LevelCorrectionExtraDamage` (:11959). `X2Lib/X2GUUser.cpp`: `DamageReact` (:9211), `LevelCorrectionExtraDamage` (:12324),
  `SetDamageExceptionProcess` (~:45863). `X2Lib/X2UnitManager.cpp:1283-1313`: the extra-stat game type (1 Henir, 2 secret, 0 other) passed
  to `CalculateExtraStatValue`. `X2Lib/X2UICharInfo.cpp:1845, :1969`: the character window's June defence curve (CharInfo C3).
  Line numbers drift: find by name.
- **June code.** `GetFinalDamage` `32284aa0` (decompile and listing: `june2017_re/buffre/dec/32284aa0.c` / `.s`), NPC `DamageReact`
  `327b1f30`, user `DamageReact` `320462a0`, `SetDamageExceptionProcess` `322990e0` (user `32045340`, NPC `32759f30`), the damage-result
  hook `3229a530`, the variance roll `322788c0`, `LevelCorrectionExtraDamage` `32760210`. Existing rows:
  `grep -n -E "32284aa0|327b1f30|320462a0|322990e0|StatCalc S33" june2017_re/funcmap.tsv`.
- **June data already live.** `data036` `NewNPCStat.lua` (1,693 rows) and `NewNPCStatFormula.lua` (flags `SERV_IRUHADEV_EXTRA_NPC_STAT`,
  `_PARITY`, both on), June `item.kim` (K2), June `SocketOptionForm.lua`, June skill and buff data. Source form:
  `june_src_fixed/data036_kom/*.src.lua`.
- **Tools.** `python june2017_re/disasm_range.py <va> <va> [--x87]`, `python june2017_re/funcdis.py --func <va>`, Ghidra (skill `ghidra`,
  read-only), Unicorn 2.1.4 is installed, `tools/junediverge.py`, `tools/flagcheck.py`, `tools/bytepatch.py`, `tools/build.py --deploy`,
  `tools/logs.py --tag JUNE_STATCALC`, `tools/exestr.py`, `tools/elsdb.py`, `tools/createchar.py`. Never hand-type msbuild; never read
  this doc in full, pull sections with `docsection.py`.
- **New in this plan.** `june2017_re/statcalc/statcalc_map.tsv` (the map; seeded 2026-11-12), `june2017_re/statcalc/gfd_blocks.tsv` (D0:
  `GetFinalDamage` block by block), `june2017_re/statcalc/emu_*.py` (the comparison scripts), funcmap rows tagged `StatCalc S33`.
- **Flag family.** `SERV_IRUHADEV_JUNE_STATCALC_*`: `_DEFENCE_CURVE` (D1), `_FINAL_DAMAGE` (D2), `_HIT_PIPELINE` (D3), `_DAMAGE_REACT`
  (D4), `_EXTRA_DAMAGE` (D5), `_UNIT_STAT` (S1), `_EQUIP_STAT` (S2), `_SOCKET` (S3), `_BUFF_FACTOR` (S4), `_RESOURCE` (R1), `_DEBUG`.
  **Log tag** `[JUNE_STATCALC]` in `offline_server.log` (`CX2OfflineLog::Server`, throttled to about one line a second per pair).

## Measurement (planning session, 2026-11-12)

Read from the tree, the June exe and June's data. Nothing was run in game.

**1. March's defence formula** (`GetFinalDamage`, dungeon and field):

```
full    = 348 + 62 * attacker level                       (:3323-3357)
factor  = 1 + 0.127 * (defender level - attacker level)   when the defender is higher,
          1 - 0.065 * (attacker level - defender level)   when lower, floor 0   (CalcLevelFactorForFinalDamage, dungeon only)
rate    = defence * factor / full                          (:3409)
damage *= max( 1 - rate * frozen * skill - shock, 0 )      (:3535)
```

Straight line: at `defence >= full` the hit does nothing.

**2. June's defence formula** (`32284aa0` @32285d58-32285dc4 and @32286068-32286090):

```
level   = vslot 0x20c of the unit stored @32284bcf ([esp+0x38]); forced to 1 when g_pX2Game +0x85d is set
rate    = defence / ( level * 39.35 + 258.65 + defence )   (doubles 33487178, 33487170)
damage *= max( 1 - rate * [esp+0x3c] * [esp+0x18], 0 )
```

A curve: it never reaches 1 for a positive defence. No game-type switch inside the block; no shock term in that statement.
The character window already prints this curve (`CX2UICharInfo::GetDamageDecrement`, June `3291eb50` / `322a8b60`), so the window and
combat disagree today.

**3. June's monster defence is built for that curve.** `NewNPCStatFormula.lua`: `DefFactor(level) = 258.65 + 39.35 * level` (the same
two constants), and defence = `k * DF * base / ( DF - (k - 1) * base ) * 1.2 for secret and Henir`, where `k` is the row's
`PHYSIC_DEF_VALUE` / `MAGIC_DEF_VALUE` and `base` is `NormalDef`, `HenirDef`, `SecretNormDef` or `SecretLutoDef` by game type. Through
June's curve that is a reduction of `k * base / ( DF + base )` before the 1.2: `k` scales the reduction, and the defence number itself
grows without limit as `k` rises. March's formula reads that number as if it were linear.

**4. The numbers** (attacker and monster at the same level, so March's level factor is 1; a higher monster makes March worse):

| Curve (game type) | Level | k | Defence | June: damage x | March: damage x |
|---|---|---|---|---|---|
| normal (0) | 70 | 1.0 | 946 | 0.76 | 0.80 |
| normal (0) | 99 | 2.0 | 3,827 | 0.52 | 0.41 |
| secret (2) | 70 | 1.0 | 3,694 | 0.45 | 0.21 |
| secret (2) | 70 | 1.5 | 11,329 | 0.21 | **0.00** |
| secret (2) | 99 | 1.0 | 5,128 | 0.45 | 0.21 |
| secret (2) | 99 | 1.5 | 15,836 | 0.21 | **0.00** |
| secret, type 3 | 70 | 1.0 | 5,702 | 0.35 | **0.00** |

Secret dungeons are the only place the port passes game type 2 (`X2UnitManager.cpp:1288`, `IsSecretDungeon`; type 3 is never passed
today, its row shows what a later fix of that question would meet): a `k = 1.0` monster
takes about a fifth, anything from about `k = 1.2` takes nothing. That is the reported fault. Normal dungeons differ by a few percent.
The 3,386 defence values in `NewNPCStat.lua`: 1.0 x 2,062; 1.1-1.3 x 850; 0.5-0.91 x 274; 1.4-1.6 x 20; 2.0-3.0 x 180.

**5. Left for the phases (not read in this session).**
- Whether June still has a level-gap factor on defence. `CalcLevelFactorForFinalDamage` has no June row; the call @322855b6
  (`3226e530`) is the extra-damage sum, not it. D1 settles it before writing.
- Which unit's level `[esp+0x38]` is (March's counterpart is the attacker's; the damage-free override to 1 matches March's attacker
  branch).
- Where June's shock (-0.1) term went, and the PvP rate.
- `k` of about 1.98 and above on the secret curve makes the Lua's denominator cross 0, so the defence value is negative. Which game
  types the 180 values of 2.0-3.0 meet, and what June's code does with a negative defence, is D1's to read (the port's ExtraNPCStat
  evaluator already falls back to the legacy stat on a non-finite result; a negative one is finite).
- The player as the defender runs the same block: incoming damage changes too. D1's play-test checks both directions.

## Design: the map and the verification

- **Map** `june2017_re/statcalc/statcalc_map.tsv`, one row per formula function, per block of a large function and per stat member.
  Columns: `area` (damage / stat / resource), `item`, `june_va`, `march_site` (`file:function`), `state` (`June` / `March` / `mixed` /
  `absent` / `unknown`), `last_flag`, `readers`, `phase`, `note`. The planning session seeded it with the rows the brief, the divergence
  plan (`tools/data/june_march_divergent.tsv`, verdict `stage33`) and the measurement name. D0 fills the damage rows, S0 the stat and
  resource rows. A phase that ports a row sets it to `June` in its own commit.
- **Block ledger** `june2017_re/statcalc/gfd_blocks.tsv` (D0): `GetFinalDamage` `32284aa0` cut into address ranges in June's order, each
  with what it computes, March's lines, the state and the flag. D2 works down this list; its review checks that no range is left out.
- **Per-hit log** (D1, `_DEBUG`): one line per attacker-defender pair, about once a second:
  `[JUNE_STATCALC] HIT att=<user uid or npc id> lv=<n> def=<npc id or user uid> dt=<damage type> atk=<n> defence=<n> rate=<f> march=<f> dmg=<n>`.
  `march=` is March's rate computed for the log only, so one play-test shows before and after. D2 adds `GFD` lines (each multiplier of
  the main path by name), D4 `REACT` lines (final damage, each reduction, HP change), S1 a `SHEET` block on game start.
- **June's code as the oracle.** `june2017_re/statcalc/emu_defence.py` (D1) maps `unpacked_x2.exe`, runs the straight-line ranges
  @32285d8d-32285dc4 and @32286068-32286090 with the stack slots preloaded, and compares with a Python transcription of the port's
  expression over at least 300 input sets (defence 0, negative, 1e6; level 1, 70, 99; the two multipliers at 0, 0.5, 1). Later phases add
  one script per pure function (precedent: buff Phase 16 review, `320839e0` on 360 sets).

## Phase D0 - Damage map (read-only)

**Scope.** Fill the damage rows of `statcalc_map.tsv` and write `gfd_blocks.tsv`. Read every function in June and its March counterpart:
- `GetFinalDamage` `32284aa0`, range by range, including the level-damage path @32285dc4-32286068 and the tail after @32286090 (the
  damage-type switch `32286c78`, the unit-type-100 block @322860ed, the game-type rates @32286126-3228618a, vslot `0x74`, the multiplier
  sum @3228619f-3228626e, the `AT_COMMAND` terms, boss damage, the enchant piercing term, `32275ba0`, HP-few, HP-rate, `3227c940`);
- its helpers: the variance roll `322788c0` (not ported: say whether June's data now gives it a non-zero range), `3226e530`, `3226e9a0`,
  `322a91a0`, `32273480`, `32274970`, `GetEquippingItemByEquipPos` `32869e10` (3 args in June), `ModifyFinalDamageByGameUnitType`, the
  hook `3229a530` (vslot `0x6c`, multiplies both attacks; the Rena block @3229a79c is char-rena's, the 24222 / 24057 block is Eve's);
- `SetDamageExceptionProcess` `322990e0`, user `32045340`, NPC `32759f30` (March has none), and the list of the eight class overrides
  (funcmap rows 197-204) with the plan that owns each;
- the damage section of NPC `DamageReact` `327b1f30` (from the `GetFinalDamage` call to `UpNowHp`) and of user `DamageReact`
  `320462a0`; `GetBasicReducedDamagePercent` `3201aff0`, `GetSocketReducedDamagePercentInCase` `32000320` (its June body sits in a
  June-named twin today), `322cfca0`, the training clamp;
- `LevelCorrectionExtraDamage` `32760210` and the user's twin, `GetEnchantResist` (`32274b00`, user `31fd8790`, NPC `32757c50`),
  `DamageCheck` `3262f5a0`'s prelude, and the hit dispatch gaps routed from char-raven CH1q (`CEffect::AttackResult` `31f85910` tail
  @31f885bd-31f88669; `UnitToUnit` `32625000` @3262561e).

Then size D2-D5 from the rows: split a phase that holds more than about a dozen `March` / `mixed` blocks, amend its kickoff and STATUS
row, and hand class rows to their plans. No client code.

**Run with.** Opus 5.5 / xhigh; subagents do the greps and the mechanical Ghidra pulls. Review Opus 5.5 / high (`D0-review.md`).

**Read list.** "Rules", "Starting context", "Measurement (planning session, 2026-11-12)", "Design: the map and the verification".

**Done gate.** Every address range of `32284aa0` is in `gfd_blocks.tsv` (ranges contiguous from entry to the last `ret`, checked by a
script line in the Done note); every item in the scope list has a map row with a state and a reader list; funcmap rows tagged
`StatCalc S33` for each new identification; D2-D5 kickoffs amended. Full build skipped: no play-test.

### Phase D0 - Done (2026-11-12)

Read-only. No client code, no flag; full build skipped: no play-test. flagcheck / junediverge: not applicable (no source touched).
D1 ran beside it in another session; its map rows (defence rate, damage rate, `CalcLevelFactorForFinalDamage`, defence sum), funcmap
rows and STATUS row were left alone, and this commit stages only D0's hunks of the shared files.

**Block ledger** `june2017_re/statcalc/gfd_blocks.tsv`: `32284aa0` cut into 55 contiguous ranges in June's order, entry to `32286c48`
(the code ends with `JMP 32284b01` @32286c40 plus padding; the function's only `RET` sits in the shared epilogue @32284b28; the jump
tables `32286c48-32286ca7` after it are data). Check, from the Trunk:
`python -c "import csv;r=list(csv.reader(open('june2017_re/statcalc/gfd_blocks.tsv'),delimiter='\t'))[1:];s=[x[0].split('-') for x in r];print(all(a[1]==b[0] for a,b in zip(s,s[1:])),s[0][0],s[-1][1],len(s))"`
prints `True 32284aa0 32286c48 55`. States: 34 `June` (each re-read against the linear disassembly), 14 `mixed`, 4 `absent`,
3 `March` (D1's three ranges, ported in its uncommitted work; D1 sets them `June`). Ghidra's listing of `32284aa0` omits three switch
bodies (`322851b1-322852d7`, `322854f8`, `32286810-3228681a`): read June with `disasm_range.py`.

**Map** `statcalc_map.tsv`: 16 damage rows updated, 7 added (stat-up `3226e9a0`, stat copy `322a91a0`, critical threshold `32273480`,
`GetIgnoreDefenceRate` `32274970`, the unit hooks `0x3ac` / `0x3b0`, PvP halving `322cfca0`, training clamp). Every scope item has a
state and readers; claimed-but-not-re-read rows stay `unknown` with the phase that re-reads them. funcmap: 11 rows `StatCalc S33`.

**Variance roll `322788c0`: already ported** (Buff Phase 16, `DamageMaximizeRoll`, flags `JUNE_BUFF_NEW_STAT` + `_BEHAVIOR_PARITY`);
the map's "absent" was stale. Re-read: matches June (slot 121 on MAXIMIZE, seed = slot 113 random-table index, floor and cap). Its
range today is the designed 10% basic (`JUNE_ATTACK_RANGE_DESIGN`, users only). June's shipped buff data adds no flat range: BBT 7 / 8
have 284 / 278 PERCENT factors and 4 + 4 FIX ones only in `BattleFieldBuffFactor_INT` (internal build); 80 / 81 are PERCENT only. June's
real basic is the server's second KStat set (`SetKStat` `323571c0`): S1. Seeds: mapped (funcmap 931 / 1295).

**Findings that correct earlier notes.**
- `3229a530` (vslot `0x6c`) holds only class blocks: Raven (24222, 24057), Rena (`0x5a02`, `0x5a13`), Ara (27023 / 27042 + 27420).
  Planning's "Eve" was wrong. All three classes are ported, so D2a ports it whole.
- `3226e530` is `CalcExtraAttackPower` (smash attack), not a D5 item: moved to D2a. Differences: June has no `EDT_EVENT_SMASH_3` case
  and adds a discarded roll per `+0x754` record.
- `GetFinalDamage`'s user-attacker passives key June's ids (24403, 24408, 22410, 23411); March's blocks key March ids (4412 / 2414 /
  3411) with other thresholds (0.2, an HP > 0.9 gate). The world-mission epsilon differs (June 5e-8, March 5e-5). The socket stat
  rates and the atomic shield switch on June states 9 / 11 / 18 / 21 / 23, and the shield formula changed (`1 - r` in game states).
- `GetSocketReducedDamagePercentInCase` is wholly dead under US_SERVICE; the June-named twin no longer exists (live body:
  `GetSocketReducedDamagePercent`, X2GUUser.cpp:27839).
- Base and user `SetDamageExceptionProcess` already match June (map rows; D3 re-checks only).
- Unidentified, left to the porting phase: vslot `0x2ac` (state defence), NPC `+0x8fd4`, vslot `0x784` (Manaflow MP).

**Class hand-offs.** The eight `SetDamageExceptionProcess` overrides: Eve, Chung, Raven, Rena, Aisha, Elsword ported by their char
ports; Add's equals March (char-add); Elesis's is char-elesis's (checklist 7b, already listed). Rose's `GetFinalDamage` branch is in the
checklist (section 7, `@32285d0a`). The class blocks inside `GetFinalDamage` and `3229a530` of ported classes (Raven, Rena, Aisha,
Ara, Chung's shield) are D2a / D2b's: one checklist line says so.

**Sizing.** D2 held 17 `mixed` / `absent` ranges plus three helpers: split by address into **D2a** (`32284aa0-32285ccf`, probe build,
no review of its own) and **D2b** (`32285ccf-32286c48`, full build and the play-test for both; `D2-review.md` covers both).
D3 fits (four items). D4 first cuts both `DamageReact` damage sections into `dr_blocks.tsv` and splits D4a / D4b if they hold more
than about a dozen blocks (unread parts remain in both). D5 fits. Kickoffs `D2a.md` (was `D2.md`), `D2b.md`, `D3.md`, `D4.md`,
`D5.md` amended; STATUS rows updated.

**Review** (`D0-review.md`, Opus 5.5 / high): the ledger, five blocks against the exe, three map rows outside `GetFinalDamage`.

### Phase D0 - Review (2026-11-12)

Verdict: the ledger holds; the map had three stale rows (fixed). Read-only, no flag: flagcheck / junediverge not applicable; full build
skipped: no play-test. March's branch resolved with `cl /P` of `X2GameUnit.cpp` and `june2017_re/flagprobe.py`.

**Coverage.** The Done note's script still prints `True 32284aa0 32286c48 55`. In the exe (capstone, `disasm_range.py`): 2,407
instructions from `32284aa0` to `32286c48`, every `jmp` / `jcc` target inside the range and on an instruction boundary, one `RET`
(`ret 4` @32284b28), the last instruction `jmp 32284b01` @32286c40 plus padding; the four indirect jumps (@322851aa, @32285842,
@3228609b, @32286809) read tables `32286c48` / `32286c60` / `32286c78` / `32286c90` whose entries all land inside; `int3` from
`32286ca8`. All 55 range starts are instruction boundaries. Ranges contiguous, none overlapping.

**Five blocks against the exe and March's live branch** (constants read from `.rdata`):
- `32285481` June: defender (`EDI` = `[esp+0x38]`) `+0x4120` > 0 -> defence x 2.0 (`33437820`); attacker `+0x4124` > 0 -> attack x 1.1
  (`334720b0`) = X2GameUnit.cpp:3013-3022. Holds.
- `3228626e` June: `AT_COMMAND` (DD `+8` == 2) -> melee byte DD `+0x2c` picks `+0x6adc` (87) else `+0x6b90` (96), x the summed damage
  = :3675-3684. Holds.
- `32285055` mixed: June keys 23411 (`0x5b73`), ability `0x11` = `SA_UNFIXED_DEFENSE_REL`; March's live block (`SERV_TRAPPING_RANGER_TEST`
  on) keys `SI_P_LTR_SHARPEN_ARROW`. Holds.
- `3228509f` mixed: June `GT_PVP` (1) skip, user attacker / user this, rate - 1.0 + 5e-8 (`334355b0` / `3346ed68`); March
  `CalculateIncreasingRate` 5e-5. Holds.
- `32284e96` absent (24408): no March counterpart in the live text. Holds; detail added: it tests the attacker's own HP (`EBP`), not
  the defender's like 24403 (ledger, funcmap 10392, D2a kickoff).

**Three map rows outside `GetFinalDamage`**, readers by a `.text` call scan and a grep:
- NPC `DamageReact` `DI_EVENT_ALCHEMY_LAB` (row 21): dungeon test `0xa186` = 41350 @327b45a4; no March code. Holds.
- PvP halving `322cfca0` (row 31): callers `3204968a` / `327b47cd` only, as listed. **Failed:** marked absent, but effect completion
  C10e ported it (`CX2Game::PvpAmusementHalveDamage`, `SERV_IRUHADEV_JUNE_DAMAGE_REACT_HOOKS` on); the body matches June. Row -> June.
- Training clamp (row 32): **failed** the same way: Buff 14c ported it (`X2GUUser.cpp:11582-11596`, `SERV_IRUHADEV_JUNE_TUTORIAL_MODE`
  on; `+0x16a` is the tutorial byte, June @32049b32-32049b6e). Row -> June.

**Whole re-check** (two failures): every non-`June` map row and ledger range was grepped for its June addresses in `X2Lib` / `KTDXLIB`.
One more stale row: NPC `SetDamageExceptionProcess` `32759f30` (row 12, "absent, NPC inherits the base") is ported (effect completion
C8b, `SERV_IRUHADEV_JUNE_DAMAGE_EXCEPTION` + `MODIFY_SET_DAMAGE_DATA` on) and matches June except a documented guard for a non-user owner.
Row -> June. The other hits are citations, not ports. Row 8 notes that Rena's `3229a530` reader already exists (char-rena CH1o).

**Line numbers.** D1 inserted lines into `GetFinalDamage` after D0, so the ledger's `march_lines` from :3041 on were 4-59 lines off.
They were remapped exactly from the D0 file to HEAD by `difflib` (37 rows, no approximate line), with map rows 26 / 30.

**Class hand-offs** landed as the Done note says: checklist rows 333 (eight overrides, Elesis 7b), 953 (Rose @32285d0a), 954 (class
blocks are D2a / D2b's); six overrides are cited in their class files.

**Kickoffs.** D2a fits (24408's HP owner and Rena's reader added; Depends drops D0 review). D3: the NPC layer is re-check only. D4: the
training clamp and the PvP halving are re-check only.

## Phase D1 - Defence curve: the 0-damage fix

**Scope.** In `CX2GameUnit::GetFinalDamage`, under `SERV_IRUHADEV_JUNE_STATCALC_DEFENCE_CURVE`, replace March's defence-rate chain with
June's, whole, from the defence sum to the damage rate (@32285849-32286090), March's in `#else`:
- settle the open reads of "Measurement" item 5 first, in the exe: the level-gap factor (find June's twin of
  `CalcLevelFactorForFinalDamage` or prove there is none; if it differs or is gone, add its `june_march_divergent.tsv` row and the
  replacement in this commit), whose level is used, the shock term, the PvP rate, a negative defence;
- the rate `defence / ( level * 39.35 + 258.65 + defence )` and the damage rate `max( 1 - rate * a * b, 0 )`, with the constants as June
  holds them (doubles) and June's operand order;
- every other reader of the March constants 348 / 62 / 410 (`grep` `X2Lib`, `KTDXLIB`, `X2Lib/Offline`): list each, port or prove equal;
- the level-damage path's use of the rate, if it reads it (@32285dc4 branches on `m_bLvDamage` after the rate is stored);
- `SERV_IRUHADEV_JUNE_STATCALC_DEBUG` and the `HIT` line of "Design";
- `june2017_re/statcalc/emu_defence.py` and its result in the Done note;
- map rows set to `June`; a `MODS.md` entry.

Not in D1: any other block of `GetFinalDamage` (D2), monster stat data, the ExtraNPCStat game type 3 question (owner:
`LANOX_EXTRA_NPC_STAT_PLAN.md` follow-ups).

**Run with.** Opus 5.5 / high. Review Opus 5.5 / high (`D1-review.md`). Not beside another phase that edits `X2GameUnit.cpp`.

**Read list.** "Rules", "Starting context", "Measurement (planning session, 2026-11-12)", "Design: the map and the verification",
"Play-test hand-off".

**Done gate.** `flagcheck` PASS for both flags; `junediverge --check` PASS; the emulation agrees on every input set (or each difference
is explained as June's own behaviour with a `JUNE_BUGS.md` row); `DEPLOY OK`; `EXESTR OK` for `[JUNE_STATCALC] HIT`; play-test: in one
the Elder secret dungeon ("Decisions", item 1) the user hits a `k = 1.2` boss that took 0 before, and the log's `rate=` is below 1
with `march=` at or above 1 for it; a `k = 1.0` monster logs a `rate=` below its `march=`; the monsters' hits on the player also log.

## Phase D1 - Done

**2026-11-12, Opus 5.5 / high. Flags `SERV_IRUHADEV_JUNE_STATCALC_DEFENCE_CURVE`, `SERV_IRUHADEV_JUNE_STATCALC_DEBUG` (`KTDXLIB/Always.h`).**
Commit: this commit (`june-stat-calc Phase D1`). Code: `X2Lib/X2GameUnit.cpp` `GetFinalDamage` only (+59 lines, insert-only).

**The reads (June `32284aa0`; funcmap rows tagged `StatCalc S33`):**
- **Level-gap factor: none in June.** 0.065, 348 and 410 (float, double or float-as-double) are not in the exe; 0.127f has one
  reader, Chung code @32c19ccf; 62.0f two non-damage readers (an effect spawner 32267300, 32e01f71). June feeds the defence sum
  `[esp+0x24]` straight into the rate. `CalcLevelFactorForFinalDamage` gets a `june_march_divergent.tsv` row (`replace`, ported under
  this flag): `GetFinalDamage` no longer calls it; March's body stays for the `#else` and the debug `march=` value.
- **Whose level: the DEFENDER's.** `[esp+0x38]` = DD+0x470 (@32284bcf); vslot 0x20c = `GetUnitLevel` (the level-damage path calls it
  on the attacker @32285dea and on `[esp+0x38]` @32285e17, March's `pAttacker / pDefender->GetUnitLevel()` there). Forced to 1 when
  `g_pX2Game +0x85d` = `IsDamageFreeGame()` (the same path keeps 18.0 on it @32285dde). The planning note's "attacker" was wrong.
  The character window (`GetDamageDecrement`, `322a8b60`) also uses the unit's own level; 33487178 / 33487170 have only these two readers.
- **Constants:** June's doubles are `(double)39.35f` and `(double)258.65f`; the curve is stored as a float before `def / ( curve + def )`.
- **Multipliers:** `[esp+0x3c]` = the frozen rate (written @322855c5-32285682, = March's `fFronzenDebuffDefenceRate`); `[esp+0x18]` =
  `GetDefenceRateBySkill` (vslot 0x428 @32285cf9, already June under `JUNE_USER_PASSIVE_HOOKS`).
- **Shock: gone.** 32284aa0 never reads the shock time (+0x2480). March's `- 0.1` and its local are in `#else`.
- **PvP: no game-type switch** (March's PvP variant is in `#else`; offline has no PvP).
- **Sharpen Arrow** (`[esp+0x130]` = skill 23411's value, March's `fSharpenDamageRate`) scales the defence only on an `AT_COMMAND` hit
  (@32285ccf); March did it on every hit. Ported. Its writer @32285059-32285098 is D2a's.
- **Defence sum** (@32285849-32285cc7): cases 0-2 are the SOCKET_PARITY blocks; weapon cases 3-5 equal March's. Not re-ported.
- **Rose block** @32285d08-32285d54 (UT_ROSE class 346 / 347: attack *= +0x9798): stand-in comment; Rose's port (Stage 8) owns it.
- **Level-damage path** (@32285dc4, `m_bLvDamage`) leaves before the damage rate and never reads the rate: unchanged.
- **Negative defence:** June has no guard (`JUNE_BUGS` 624, latent: the highest secret-dungeon `k` is 1.3; the `k >= 1.98` rows are
  Henir-type NPCs). Between minus the curve and 0 a hit does more than its attack; at exactly minus the curve it is infinite.

**Other readers of 348 / 62 / 410 / the level factor:** `X2UICharInfo.cpp` `GetDamageDecrement` is already June's curve (CharInfo C3;
the 348 + 62 copy is its `#else`). `X2Lib/Offline` has no damage formula. No other reader in `X2Lib` / `KTDXLIB`.

**Emulation:** `june2017_re/statcalc/emu_defence.py` runs @32285d8d-32285dc4 and @32286068-32286090 in Unicorn (FPCW 0x027F) against
the port's expression: `EMU_DEFENCE OK: 889 input sets, 0 mismatch(es) (rate and damage rate compared bit for bit, NaN = NaN)`
(levels 1-99, defence 0 / -0 / negative / 1e6 / inf / minus the curve, both rates 0 / 0.5 / 1).

**Debug line** (`offline_server.log`, about once a second per pair): `[JUNE_STATCALC] HIT att= lv= def= dlv= dt= atk= defence= rate=
march= dmg=`. `att` / `def` = NPC id or user UID; `lv` = attacker level, `dlv` = the defender level June's curve used (one field more
than the plan's shape); `march=` = March's dungeon / field rate with its level-gap factor, for the log only; `atk=` is 0 on a mixed
hit; `dmg` = the final damage `GetFinalDamage` returns. Level-damage hits return early and do not log.

**Hygiene:** `flagcheck SERV_IRUHADEV_JUNE_STATCALC_DEFENCE_CURVE --with SERV_IRUHADEV_JUNE_STATCALC_DEBUG --worktree`: 1 PASS (X2GameUnit.cpp
  +59), 1 DEFINE (Always.h +21), 0 deleted, 0 non-ASCII, 0 EOL, 0 tok -> `RESULT: PASS`; `junediverge --check`: `RESULT: PASS`; `enumscan`: 0 A/B rows.
**Build:** `DEPLOY OK: X2_offline.exe is 26,330,624 B, dated 2026-11-12 12:49:16, md5 f030a272d934783d3108b7bfeb47be3a`;
  `EXESTR OK: 1 of 1 found in X2_offline.exe` (`[JUNE_STATCALC] HIT att=`, wide).

**Bookkeeping:** map rows `GetFinalDamage: defence rate`, `damage rate by defence`, `defence sum by damage type`,
`CalcLevelFactorForFinalDamage` -> `June`; 4 funcmap rows (`StatCalc S33`); `JUNE_BUGS` 624; `MODS.md` entry; divergent row.

**Play-test owed** (hand-off below). 33030 (`DI_ELDER_SECRET_COMMON`) is registered as `UNUSED_DUNGEON_ID` with
`m_bForbidEnterDungeon` in the live `DungeonData.lua`, so the test uses `DI_ELDER_SECRET_HELL` 33031 (stage 0: `NUI_ELDER_SECRET_BUNKER`
1466, k 1.0; stage 2: `_PUNCH_BOSS` 1470 / `_SMALL_KNIFE_BOSS` 1469, k 1.2) and `DI_ALTERA_SECRET_HELL` 33051 (stage 0:
`NUI_ALTERA_SECRET_TYPE2_BOSS` 1551, k 1.2). Both scale monsters to the party level (`m_bRelativeMonsterLevel`).

**Play-test 1 PASSED (2026-11-12, run 12:53-12:57 on the D1 exe md5 f030a272; `logs.py --tag JUNE_STATCALC`, 151 HIT lines).**
jtLordKnight (unit 25, Lord Knight 99) in `DI_ELDER_SECRET_HELL` 33031; monsters scaled to level 104.
- `k = 1.2` boss `NUI_ELDER_SECRET_PUNCH_BOSS` 1470 (stage 2): `defence=8121 rate=0.6511 march=2.0472`, 14 hits, `dmg` up to 138,054
  (March's line read 2.05: 0 damage). Gate met.
- `k = 1.0`: `NUI_ELDER_SECRET_BUNKER` 1466 `rate=0.5526 march=1.3547` (March's level-gap factor 1.635 at a 5-level gap also took it
  to 0), the stage-1/2 `_COMMON` soldiers and Nasods (530-532, 566, 574, 576) the same. `rate` below `march` on every one.
- Incoming: 530 / 532 / 574 / 576 / 1470 -> unit 25 log `dlv=99 defence=3381-4571 rate=0.45-0.52`, `dmg` up to 15,652.
- 16 of 151 lines show `dmg=0` on every kind of monster at the same `rate` (the curve never gives 0 here: `1 - rate` > 0.34), so the 0
  comes after the defence rate (a 0 motion damage or a later multiplier); D2a / D2b read the rest of the body.
- Not run: `_SMALL_KNIFE_BOSS` 1469 and the second dungeon (Altera Secret Hell 33051); the plan's gate is met without them.

### Phase D1 - Review (2026-11-12)

Verdict: **D1 holds; no transcription error, no code change, no build** (the deployed exe md5 f030a272 stays). Opus 5.5 / high.

**Against the exe** (`disasm_range.py`, June `32284aa0`):
- **Units.** `EBP` = DD `+0xc` (attacker, @32284b9e-32284bb1), `[esp+0x38]` = DD `+0x470` (defender, @32284bb5-32284bcf). The rate's
  level is vslot `0x20c` on `[esp+0x38]` (@32285d58-32285d66) = `pDefender->GetUnitLevel()`; the level-damage path calls the same slot
  on `EBP` @32285dea and `[esp+0x38]` @32285e17, as March's `pAttacker` / `pDefender` (:3453-3466). `+0x85d` is `IsDamageFreeGame()`
  (that path's 18.0 test @32285dde = March's `IsDamageFreeGame() == false`). Holds.
- **Rate.** `fild level; fmul qword 33487178; fadd qword 33487170; fstp dword` then `fld def; fld curve; fadd st(1); fdivp` =
  `def / ( curve + def )`, the curve rounded to float in between. **Our compiled port at `X2_offline.exe` 0x32c79f6e-0x32c79f8e is
  the same nine instructions** (its double constants 0x33ce1518 / 0x33ce1510 = `(double)39.35f` / `(double)258.65f`). Holds.
- **Multipliers.** `[esp+0x3c]` frozen, `[esp+0x18]` = 1.0, or vslot `0x428` on the attacker when `+0x264` (`m_GameUnitType`) is 0
  (`GUT_USER`) with `attackType` pushed = :3381-3383. Order `( rate * frozen ) * skill`, as the port. Holds.
- **Floor.** `fldz; fcom t; test ah, 0x41; jne` keeps `t` when `0 < t`, `0 == t` or unordered: `t < 0` -> 0, NaN passes, a rate below 0
  (defence between minus the curve and 0) gives `t > 1` unclamped, a rate above 1 floors to 0. **Our compiled floor 0x32c7a206-0x32c7a227
  is June's 32286068-3228608c instruction for instruction.** Holds.
- **Sharpen Arrow** on `[ebx+8]` (`attackType`) == 2 = `AT_COMMAND` only (@32285ccf-32285ce2). Holds.
- **Level-gap factor: none, re-proved.** Byte scan of the June exe: 0.065 and 410 (float, double, float-as-double) and 1.635 absent;
  348.0f once, unaligned inside a data table, no reference; 0.127f one reader (Chung @32c19cd1); 62.0f readers 3226748a / 32e01f71; 62.0
  double one reader, 32de443d, a UI position offset (`x + 62`). No twin.
- **`#else` = March:** `flagcheck SERV_IRUHADEV_JUNE_STATCALC_DEFENCE_CURVE --range 42183ed2^..42183ed2 --with
  SERV_IRUHADEV_JUNE_STATCALC_DEBUG`: 1 PASS (X2GameUnit.cpp +59), 1 DEFINE, 0 OTHER, 0 FAIL, 0 deleted, 0 non-ASCII, 0 EOL, 0 tok ->
  `RESULT: PASS`. `git log -S` of both flags over `42183ed2..HEAD`: no commits. `junediverge --check-all`: `RESULT: PASS` (206 allow-listed).

**Emulation:** `emu_defence.py` with 50 review sets added (`review_inputs()`: the logged hits, frozen 0.75, skill 0.8, level 0 / -5 /
120, NaN and denormal defence, both sides of minus the curve at level 1, multipliers 2.5 and -1, 35 seeded random sets):
`EMU_DEFENCE OK: 939 input sets, 0 mismatch(es) (rate and damage rate compared bit for bit, NaN = NaN)`.

**Play-test lines** (154 HIT lines, 12:54-12:57): 1470 `dlv=104 defence=8121 rate=0.6511` = 8121 / (104 x 39.35 + 258.65 + 8121);
`march=2.0472` = 8121 x 1.635 / 6486. 1466 `rate=0.5526 march=1.3547`; 532 -> unit 25 `dlv=99 defence=3381 rate=0.4487`; all recompute.
Rates span 0.4487-0.6511, no NaN / inf. 1469 and 1551 have no lines (not run, as the Done note says). 17 lines show `dmg=0` at a normal
rate, two of them incoming (530 / 532 -> 25): routed to D2b's kickoff as a lead (its `GFD` lines must show which multiplier gives the 0).

**Kickoffs.** D2a fits (Depends: D1 review, now met). D2b: the `dmg=0` lead added. The debug define stays on until the D6 review.

## Phase D2 - GetFinalDamage: the rest of June's body

**Scope.** Every `gfd_blocks.tsv` range still `March`, `mixed` or `absent`, under `SERV_IRUHADEV_JUNE_STATCALC_FINAL_DAMAGE`, so the
function is June's from entry to return: block order as June runs it, the variance roll and its seed source if D0 found the range can be
non-zero, the `3229a530` call (base body; class blocks guarded), the level-damage path, the game-type and PvP / PvE rates, the
multiplier sum, `GetEquippingItemByEquipPos`'s third argument, and June-only early returns. Blocks already `June` are re-read and left.
`GFD` debug lines. One emulation script per pure helper ported.

**Run with.** Opus 5.5 / xhigh. Review Opus 5.5 / high. D0 split it (2026-11-12): D2a `32284aa0-32285ccf`, D2b `32285ccf-32286c48`;
one review (`D2-review.md`) covers both.

**Read list.** "Rules", "Starting context", "Design: the map and the verification", "Phase D0 - Done", "Phase D1 - Done".

**Done gate.** `gfd_blocks.tsv` has no row left that is not `June` or `class: <plan>`; flagcheck and junediverge PASS; `DEPLOY OK`;
play-test on three characters (one physical, one magical, one with a ported transcendence): normal attack, a special active and a
level-damage skill against a normal-dungeon monster, a secret-dungeon monster and a boss, each `GFD` line's product equal to the damage
number on screen.

### Phase D2a - Done (2026-11-12)

**Opus 5.5 / xhigh. Flag `SERV_IRUHADEV_JUNE_STATCALC_FINAL_DAMAGE` (`KTDXLIB/Always.h`, three `#error` dependency guards); D2b continues
under it.** Commit: this commit. Insert-only: `X2GameUnit.cpp` +366, `X2GameUnit.h` +3, `X2GUUser.h` +6, `X2GUUser.cpp` +3,
`X2DamageManager.h` +6 (`DTT_ACEINTHEHOLE_FINAL` 36 / `_2ND` 43 under `SERV_IRUHADEV_JUNE_ENUM_NAMES`, June's `Enum.lua` values).
No review of its own: `D2-review.md` (written by D2b) covers D2a. Full build skipped: no play-test (D2b's play-test covers both halves).

**Ported (every `gfd_blocks.tsv` range before `32285ccf` is now `June`):**
- @32284d76: `CX2GameUnit::ModifyAttackRateByAttackerSkill` (June `3229a530`, vslot `0x6c`; `vtslot.py 0x6c`: no class overrides it),
  called on the hit unit with both rates 1.0. Raven: Pointed Stiletto on a unit with its debuff, Devastating Strike on super armour
  (`EffectSet_SA_ENW_Predator_Lucky` on the hit unit, x 1.2f); Rena: Rage Fungus while the lock count is within `SA_EFFECTIVE_COUNT`;
  any user attacker: Ace in the Hole's triggers (36 / 43) on a unit with its debuff; Ara: Specter of Thunder sets `m_bIgnoreStateDefence`
  when awakened with Spiritism Weird Fox learned. Every June block needs a user attacker, so the port returns early for any other.
- @32284db2-3228509f: the passives on June's ids. Confidence Victory (24403) at the hit unit's HP <= 0.3f; Pride Victory (24408) when
  the attacker's own HP rate is **at or above** its `SA_MAX_HP_REL` value (`fld threshold; fcompp; jp skip`: D0's "<=" was wrong);
  Manaflow Overdrive (22410) with no HP gate, `GetSkillLevel( id, false, 0, true )`; Sharpen Arrow (23411) writer.
- Vslot `0x784` = `CX2GUUser` `3208e200`: reads `+0x9494`, which only the ctor writes (0); its setter `3208e230` (vslot `0x844`) has no
  call site and no vcall thunk. Ported as `m_fManaflowMpInSkillState` + getter, so Manaflow gives no bonus in a skill state (`JUNE_BUGS` 630).
- @3228509f: world-mission rates `- 1 + (double)5e-8f` (`CalculateWorldBuffIncreasingRate`, `_CONST_GAMEUNIT_D2A_`, not an edit of
  `KTDXCommonFunc.h`); June tests vslot `0x10c` == 1, so `GT_PVP != GetGameType()` stays (not the `322cc1a0` idiom).
- @32285189: socket stat rates in `XS_PVP_GAME`, `XS_DUNGEON_GAME`, `XS_TRAINING_GAME`, `XS_BATTLE_FIELD` (by name), no pet test.
  **Not registered: `XS_PVE_GAME` 23** (the kickoff asked for it): March's 23 is `XS_ARCADE_ROOM`, and March has no PvE game state, so by
  name there is no case to add; offline has neither PvP nor PvE.
- @322855a6: `CalcExtraAttackPower` as June `3226e530`: no `EDT_EVENT_SMASH_3` case, then one discarded `GetRandomFloat( 4 + j )` per
  record of the attacker's `m_vecSecondExtraDamage` (DD `+0x3b4`; a table lookup, no side effect). Its weapon read keeps March's 2-argument
  `GetEquippingItemByEquipPos`: the third argument is D2b's (its kickoff lists this caller).
- @322857fb: March's 4-argument `AT_NORMAL` call is gone. The base is empty; Eve's and Ara's 4-argument overrides are empty under
  `JUNE_CHAR_EVE_LIVE` / `JUNE_CHAR_ARA_PASSIVE` (their June 3-argument slots hold the bodies); no other class overrides it.

**Stand-in:** June's `IsSuperArmor( 1 )` also needs a user's AnimStop marker `+0x8080` clear (June-only, left unported by Buff Phase 4b-i).
It matters only for a user defender, which only PvP reaches. Divergence row `IsSuperArmor` (verdict `out`) records it.

**Emulation** `june2017_re/statcalc/emu_final_damage_a.py` (Unicorn, FPCW 0x027F, calls stubbed): `EMU_FINAL_DAMAGE_A OK: 861 input
sets, 0 mismatch(es), 0 table mismatch(es); 12 XS_PVE_GAME set(s) not compared (no March state)`. World-mission ranges, the Manaflow
multiplier, the Confidence / Pride / Rage Fungus gates, the socket block on every June state, `3226e530`'s three jump tables.

**Debug line** (`_DEBUG`, printed beside each `HIT` line, same throttle): `[JUNE_STATCALC] GFD att= def= skill= patk=<in>><out>
matk= pdef= mdef= hook=<p>/<m> conf= pride= mana= sharpen= world=<atk>/<def> socket=<patk>/<matk>/<pdef>/<mdef> extra=`. `skill` is
`m_iAppliedSkillID`; `<out>` is the value after this half's multipliers (before the memo / endurance terms). D2b extends it.

**Hygiene.** `flagcheck SERV_IRUHADEV_JUNE_STATCALC_FINAL_DAMAGE --worktree --files <6 files>`: 4 PASS, 1 DEFINE, 1 OTHER
(`X2DamageManager.h`: the two DTT names under `SERV_IRUHADEV_JUNE_ENUM_NAMES`), 0 FAIL, 0 deleted, 0 non-ASCII, 0 EOL, 0 tok ->
`RESULT: PASS`. `junediverge --check <6 files>`: `RESULT: PASS`. `enumscan --check`: 0 rows in this phase's hunks (2 older class-B rows,
`X2GUUser.h:2561-2562`). Probe: `/Zs` of `X2GameUnit.cpp` and `X2GUUser.cpp` with X2Lib's US_SERVICE defines: 0 errors (0 warnings in
`X2GameUnit.cpp`); `/P` shows the June code compiled in and March's 4-argument call gone.

**Bookkeeping.** `gfd_blocks.tsv` 9 rows -> `June`; map rows (hook, extra-damage sum, unit hooks, whole function); funcmap 5 rows
(`StatCalc S33`); divergence rows `CalcExtraAttackPower` (`replace`, ported) and `IsSuperArmor` (`out`); `JUNE_BUGS` 630; `MODS.md`.

**How to see it in game (D2b's play-test):** a Raven's Devastating Strike on a super-armoured boss plays the Predator Lucky effect and its
`GFD hook=1.2000/1.2000`; a Rena's Rage Fungus or Ace in the Hole shows `hook=` above 1; a Dimension Witch's combo hit shows `mana=` above
1 and her skill hit `mana=1.0000`; a Night Watcher with Sharpen Arrow shows its `sharpen=` value (applied to `HIT`'s defence on command hits).

### Phase D2b - Done (2026-11-12)

**Opus 5.5 / xhigh. Flag `SERV_IRUHADEV_JUNE_STATCALC_FINAL_DAMAGE` (D2a's; a fourth `#error` guard for `_BUFF_NEW_STAT`, `_SKILL_MEMO_DAMAGE`,
`_DIVERGE_PVP_OFFICIAL_SKILL`).** Commit: this commit. Insert-only: `X2GameUnit.cpp` +250, `X2Inventory.cpp` +30, `X2GameEdit.cpp` +12,
`X2GameEdit.h` +7, `X2GUNPC.cpp` +6, `X2GUNPC.h` +6, `X2Inventory.h` +4, `X2Main.cpp` +3, `X2GameUnit.h` +2, `Always.h` +6; June names under
`SERV_IRUHADEV_JUNE_ENUM_NAMES`: `DungeonEnum.h` +8 (`DI_SPACE_OF_THOUGHT_1 / _2 / _3 / _DUMMY`), `X2SkillTree.h` +5 (`RNSI_SA_ALP_SUPERSONIC`).
`gfd_blocks.tsv`: 54 ranges `June`, Rose's `class: char-rose` (the D0 check prints `True 32284aa0 32286c48 55`). Funcmap: 13 rows `StatCalc S33`.

**Ported (June `32284aa0` from @32285ccf):**
- @32285dc4 level-damage path (Aisha's Energy Spurt `ABM_ENERGYSPURT` / `_MEMO` is the only `LEVEL_DAMAGE` data): PvP-or-PvE test with June's
  fixed-level channels (`PCC_OFFICIAL` / `_GUILD` / `_EVENT_MODE` / `_1VS1_PVP_CHALLENGE`, class `PCC_PLAY`); DT_PHYSIC fixed part =
  `fAttackerPhysicDamage x fAttackPower` (March: physic stat x 6.16); `( level^2 x 35 x corr x power + fix ) x game` (March: the game rate on the
  level part, the power on the sum); user / NPC rate rounded to float; sum `( U + M ) + F`.
- @32286126 PvP rate behind `GetIsNowPVPCorrectionGame()`. @3228619f re-read (D0 said `June`): June adds `F + D`, then `( U + M )`, with
  `U = f32( u - 1 ) x F`: ported (last bit).
- @322862f6 vslot `0x2ac` = new `virtual float CX2GameUnit::GetReceivedDamageRate() const` (base `3208e1b0` = 1.0); only Add `329d4870` and Ain
  `32a3d540` override it: checklist rows (char-add, Stage 8).
- @32286416 NPC `+0x8fd4` = `CX2GUNPC::m_fDamageRateOfPart` (ctor 1.0), applied above 1.0. Writer `CX2DamageManager` `32621f40` (four collision
  sites) and the `StateData +0x41c` `DAMAGE_RATE_OF_PART` loader: D3 (kickoff amended; data only `RAID_ELTRION_MK2_PHASE1/2/3`, "Head" x 2).
- @32286482 Space of Thought route (41480-41510): the custom event manager lookup (`32696fa0` / `32680cb0`) is a stand-in `false`, backlog
  Stage 39 (brief amended). The boss test stays `IsSocketBossGrade()`.
- @322867e0 atomic shield: `( 1 - r ) x F` in `XS_PVP_GAME` / `_DUNGEON_GAME` / `_TRAINING_GAME` / `_BATTLE_FIELD` (by name; June's 23
  `XS_PVE_GAME` is no March state), `r x F` elsewhere (March: `r - ( 1 - r ) x 0.5` in dungeon / field). Rule 3: the writer is Eve's, already June
  (CH3a key `int( SA_DAMAGED_REL x 100 )`, June `Atomic_Shield_Eve.lua` `SetShieldDamageReduce( key / 100 )`); at June's 0.6 both give 0.4.
- @3228696a `CX2GameUnit::CanApplyHpRateDamage` = June `322726f0` except `IMMUNE_TO_HP_ATTACK` (NPC `+0x8f0c`, vslot `0x430`: stand-in, D3).
  Supersonic is Rena's fourth-line skill (classes 156-158, out of scope): offline the gate is always true.
- @322869b7 dev log: `g_pGameEdit:DamageLog_MI_CRITICAL( true )` (June `32746a10`, auth level 3, `CX2GameEdit +0xde`) prints June's three lines
  (defence-ignore %, defence, ignored defence) to the chat box (`CPT_SYSTEM`, white). The Korean text is `\x` escapes (the build machine is not CP949).
- `GetEquippingItemByEquipPos` `32869e10`: June's 3-argument body (`bMinorUnitSlot = false`; positions 2-14 outside fashion force 0). Two
  stand-ins: March has no minor-unit slots and no second ring slot (its `NESI_ONEPIECE_FASHION` is 20, June's 35, so June's numeric slot table
  cannot read March slots). Callers unchanged: June passes 0 at every constant site (`june2017_re/statcalc/equip_by_pos_callers.tsv`, 34 sites).
- `GetIgnoreDefenceRate` `32274970` re-read: June keeps a NaN sum (the port returned 1.0): fixed; map row `June`.

**D1's `dmg=0` lead:** no D2a / D2b range turns a normal hit into 0 by itself; the candidates are the state defence (100 % reduce or guard),
`GetFinalDamageToBeChangedBySkill` and the HP-rate overwrite. `GFD2` prints the damage after each group, so the play-test names the block.

**Emulation** `june2017_re/statcalc/emu_final_damage_b.py`: `EMU_FINAL_DAMAGE_B OK: 3687 input sets, 0 mismatch(es), 0 static mismatch(es);
12 XS_PVE_GAME set(s) not compared (no March state), 2 IMMUNE_TO_HP_ATTACK set(s) left to D3` (level path, both game-rate branches, sum order,
state defence, part rate, shield on every June state, ignore cap, HP gate on 384 sets; static: channel ids, Space of Thought ids, minor-unit
slots = June's `NESI_MINORUNIT_*`, dev log strings).

**Debug lines** (`_DEBUG`, beside `HIT` / `GFD`, same throttle): `[JUNE_STATCALC] GFD2 att= def= motion= drate= base= game= wos= reduce= state=
recv= stated= part= boss= npc= armor= shield=<r>><after> hp= dmg=` (damage after step 1, the game rates and way of the sword, the final
multipliers and command rate, the state defence, the NPC terms, piercing / super armour / stigma, the shield, the skill change / attacker rate /
my-HP-few, then `dmg`). Level-damage hits: `[JUNE_STATCALC] LVD att= lv= def= dlv= dt= skill= level= corr= power= fix= game= melee= unit= dmg=`.

**Hygiene.** `flagcheck SERV_IRUHADEV_JUNE_STATCALC_FINAL_DAMAGE --worktree --files <12 files>`: 8 PASS, 1 PASS~ (`X2GameEdit.cpp`: one blank
line before the block), 1 DEFINE, 2 OTHER (the two enum headers, `SERV_IRUHADEV_JUNE_ENUM_NAMES`), 0 FAIL, 0 deleted, 0 non-ASCII, 0 EOL ->
`RESULT: PASS`. `junediverge --check <11 files>`: `RESULT: PASS`. `enumscan`: 0 rows in this phase's hunks. Probes (`/Zs`, US_SERVICE):
`X2GameUnit`, `X2Inventory`, `X2GameEdit`, `X2GUNPC`, `X2Main` 0 errors, no new warnings.
**Build.** `DEPLOY OK: X2_offline.exe is 26,350,080 B, dated 2026-11-12 18:23:35, md5 463c23d2abc20e8b984db098b32acfa9`; `EXESTR OK: 2 of 2`
(`[JUNE_STATCALC] GFD2 att=`, `[JUNE_STATCALC] LVD att=`, wide), `EXESTR OK: 1 of 1` (`DamageLog_MI_CRITICAL`).

**Bookkeeping.** Map rows (whole function, `GetEquippingItemByEquipPos`, `GetIgnoreDefenceRate` -> `June`; 4 new rows); divergence row
`GetEquippingItemByEquipPos` -> `replace`, ported; checklist rows; Stage 39 brief; D3 kickoff; `MODS.md`; save: Raven99 set transcended
(`elsdb settranscend Raven99 1`, backup `db_backup/els_db_20261112_182124_pre_settranscend.sql`). No `JUNE_BUGS` row (no June bug kept).

**Play-test owed** (D2a and D2b together): jtLordKnight (physical), ArmeDW (magical, Energy Spurt = the level-damage hit) and Raven99
(transcended) in Wally's Castle (Normal) and Elder Secret Dungeon Hell 33031 (stage 0 `NUI_ELDER_SECRET_BUNKER` 1466, k 1.0; stage 2
`NUI_ELDER_SECRET_PUNCH_BOSS` 1470, k 1.2). Gate: each `GFD2` chain multiplies out to its `dmg`, the `LVD` line for Energy Spurt, `dmg` = the number
on screen for a non-critical hit.

**Play-test PASSED (2026-11-12, run 18:36-18:45 on exe md5 463c23d2; `logs.py --tag JUNE_STATCALC`: 214 `HIT`, 214 `GFD`, 214 `GFD2`, 0 `LVD`).**
Units 25 (jtLordKnight), 49 (Raven99, transcended), 53 (ArmeDW) on 1466 / 530-532 (secret, k 1.0), 1470 (boss, k 1.2), 75-82 / 100 (normal).
- Every `GFD2` chain is consistent: `base` = `motion x drate` and `dmg` = the `HIT` line's `dmg` on all 214 lines (script check, 0 off).
- **D1's `dmg=0` lead, explained:** all 15 zero hits have `motion=0`: the damage data's own motion power is 0 (Lord Knight Grand Cross 21058's
  extra hits, Raven 24013, one boss 1470 magic hit). Not a formula: every multiplier after it is normal (`state=1.0000`, `recv=1.0000`).
- **Energy Spurt (22025) took the main path, as June does:** June's class script plays `EffectSet_Aisha_ABM_EnergySpurt`, whose damage effect
  is `DamageEffect_SA_ADW_Energy_Spurt` (no `LEVEL_DAMAGE`); only `ABM_ENERGYSPURT_MEMO` has it, and nothing in June's exe (plain or xorstring)
  or scripts names `EffectSet_Aisha_ABM_EnergySpurt_MEMO`. Its hits ignore defence (`defence=0`, `drate=1.0000`) for 1.37-1.52M. The
  level-damage path has no player reach in June's data: `emu_final_damage_b.py` is its proof (only `BOT_EffectSet_Aisha_ABM_EnergySpurt`, a
  PvP-hero bot, may still reach `ABM_ENERGYSPURT`).
- Boss 1470 takes `state=0.3000` (its 70 % reduce) and `boss=0` (`IsSocketBossGrade()` false for it, unchanged by D2b: a lead for the review).
- D2a: Pride Victory `pride=1.1500` on 54 Raven99 lines; Manaflow `mana=1.0003-1.1998` on ArmeDW combo hits (her skill-state hits 1.0000,
  except four from damage effects still alive after the state); `hook=1.0000` on every line (Pointed Stiletto was used 4 times; the review
  checks whether its debuff was on the target).
- Incoming: 77 / 530 / 531 / 532 / 1470 -> 25 / 49 / 53 all non-zero except one 1470 hit with `motion=0`.

### Phase D2 - Review (2026-11-12, covers D2a `3973df2a` and D2b `2ec41be0`)

**Opus 5.5 / high. Verdict: pass. No transcription error, no code change.** Every range was read against June's `disasm_range.py`
listing of `32284aa0-32286c48`.

- **D2b level-damage path (@32285dc4-32286063): June.**
  - The level is 18.0 (`33458e68`) in a damage-free game, otherwise the attacker's level. The attacker level is truncated (`32fe25f0`)
    before the PvP channel test can reset the level to 18.
  - `322cc1a0` = `GetIsNowPVPCorrectionGame()`. The channel test checks 0xc / 0xd / 0xe, class 0xb, then 0xf, the port's order.
  - Level gap: `f32( n x 0.1 )`. June's `334395e8` holds the double of 0.1f (0.10000000149011612), so the port's `0.1f` is exact. The
    original emulation levels never hit a gap where 0.1 and 0.1f round apart (n = 9); the review sets add one.
  - Fixed part: DT_PHYSIC `[esp+0x4c]` (DD `+0x38`, `damage.fPhysic`) x `[esp+0x20]` (`fAttackPower`); DT_MAGIC `[esp+0x50]`
    (DD `+0x3c`); otherwise 2000.0 (`33474b4c`).
  - The `pow` result is stored as a float. Then `( sq x 35.0 x corr x power + fix ) x game`. These are rounded to float:
    `f32( ( M - 1 ) x F )`, `f32( U - 1 )` and `f32( ( U - 1 ) x F )`. Sum `( U + M ) + F`; the floor of 1 lets NaN through.
- **D2b main path: June.**
  - Sum (@3228619f-3228626a): `f32( F + D )`, then `+ ( U + M )`.
  - State defence: capped at 100, `( 100 - r ) / 100`, vslot `0x2ac` called, then `R x f32( rate x F )`. `vtslot.py 0x2ac CX2GU`: base
    `3208e1b0` (`fld1`); only Add `329d4870` and Ain `32a3d540` override it.
  - Part rate: `fcom 1.0; test ah, 0x41`. Applied only above 1.0; NaN skips it.
  - Space of Thought: `32863880` on the dungeon id, then `32696fa0` / `32680cb0`. The port's stand-in returns `false`.
  - Shield: the byte table `32286c98` sends states 9 / 11 / 18 / 21 / 23 to `1 - r`. June's `Enum.lua` names them `XS_PVP_GAME` /
    `_DUNGEON_GAME` / `_TRAINING_GAME` / `_BATTLE_FIELD` / `_PVE_GAME`. In March they are 9 / 11 / 17 / 24 (no PvE state), so matching by name is right.
  - `322726f0` tests, in order: `+0x8aeb` (HasBossGage), `+0x8ae9` (ShowBossName), vslot `0x3c8` (`32755a60`, grade == 1), vslot `0x3cc`
    (`32755a70`, grade == 2), `0x430`, `+0x8e80` (IsMainBoss), then `( +0x8f08 - 1 ) > 1`. The port's last line calls March's two grade
    getters (`m_eMonsterGrade` vs `MG_BOSS_NPC` 1 / `MG_MIDDLE_BOSS_NPC` 2; no override), which is the same test.
  - Order after the shield: HP rate on the defender optr after my-HP-few, the 0 floor (NaN kept), `3227c940`, then the dev log.
  - Dev log:
    - Gate: game edit `+0x43c`, byte `+0xde` == 1.
    - Values: `int( 100 - p x 100 )`, `_ftol2` (64-bit) of the defence, and `int( ( 1 - p ) x defence )`. The defence there is already
      reduced by `p`, a June quirk that was kept.
    - Output: `CPT_SYSTEM` = 2, white. June's `AddChatLog` has an 8th argument (0); March's has 7.
    - Toggle `32746a10`: auth level `>= 3` (`XUAL_DEV`). The ctor stores `bl` = 0.
- **`GetEquippingItemByEquipPos` (`32869e10`): June.**
  - The third argument is forced to 0 for positions 2..14 (`EP_WEAPON_HAND`..`EP_DEFENCE_TEMP3`). Accessories skip the fashion test.
  - Slot ids 0x14..0x2a go through the minor-slot table, which must equal the third argument. March has no minor range, so the port returns
    the first match for `false` and NULL for `true`, as June does when no minor slots exist.
  - Census: 21 sites push a constant 0. The 13 register sites are Lu / Ciel UI, June-only positions (0x17 / 0x1c / 0x26), or position 2
    with `bFashion` and the third argument in one register (forced to 0 anyway).
  - All 43 live March callers pass 2 arguments, so their meaning is unchanged.
- **`GetIgnoreDefenceRate`:** `fcom 1.0; test ah, 0x41; jne keep`, so a NaN sum is kept (port: `fSum > 1 ? 1 : fSum`).
- **D2a's two named calls:** `IsSuperArmor()` stands in for vslot 153( 1 ) (divergence row 468, `out`). `XS_PVE_GAME` 23 gets no case
  because March has no such state.

**Leads from D2b's play-test.**
- **`hook=1.0` on Pointed Stiletto: June behaviour.**
  - June's bonus (`3229a5e9`) needs the target to already carry one of the skill's two buff factors, `BFI_DEBUFF_SI_A_RST_CUT_TENDON` or
    `BFI_DEBUFF_BLEEDING` (`NewSkillTempletVer2.src.lua`, 24222). The skill does not apply either of them itself.
  - None of the four hits (1466 x 2, 530, 1470) had Cut Tendon or Bleeding before it. The run's only `EffectSet_DEBUFF_BLEEDING_01`
    (18:44:50, a stage-1 uid) was never followed by a stiletto hit.
- **Boss 1470 `boss=0`: June's test is false too.**
  - `+0xdc` is the spawn's `m_cMonsterGrade` (`323752f0`, written only for an ExtraNPCStat ID). 1470 has that row
    (`NewNPCStat.src.lua:14850`).
  - The stage script places it with grade 3: `[OFFLINE_GRADE] dungeon=33031 stage=2 ... npc=1470 grade=3 bossGauge=0` (three copies),
    and `3 != 1`.
- **Level path reach: re-proved.**
  - June's exe (ASCII and UTF-16, either case) and `xorstrings.tsv` contain no `EffectSet_Aisha_ABM_EnergySpurt_MEMO`,
    `ABM_EnergySpurt_MEMO`, `EnergySpurt_MEMO` or `ABM_ENERGYSPURT_MEMO`.
  - In `june_src_fixed` the MEMO set appears only at its own definition (`EffectSet.src.lua:18193`), and `Arme_VioletMage.src.lua:9968`
    plays the plain set.
  - Both `ABM_ENERGYSPURT` and `_MEMO` carry `LEVEL_DAMAGE` (`DamageEffect.src.lua:14017` / `14070`). The plain one is reached only by
    `BOT_EffectSet_Aisha_ABM_EnergySpurt` (a PvP hero).

**Play-test lines** (run 18:36-18:45, exe md5 463c23d2): 214 `GFD2` lines, matching 214 `HIT` lines.
- An independent script check found 0 off for `base = motion x drate`, `stated = reduce x state x recv`, and `shield after = armor x ( 1 - r )`
  (`armor` when `r >= 1`).
- All 15 `dmg=0` lines have `motion=0`: D1's lead is the damage data, not a D2 range.

**Numbers.**
- `EMU_FINAL_DAMAGE_A OK: 861 input sets, 0 mismatch(es)`.
- `EMU_FINAL_DAMAGE_B OK: 3737 input sets, 0 mismatch(es), 0 static mismatch(es)`, including 50 review sets (`review_inputs()`):
  - level gaps 6 / 7 / 14 / 18 / 23 / 26 and one negative gap; power and memo at 0 and 1e30;
  - NaN and 1.0000001 multipliers;
  - reduce 99 / 100 / 101 / INT_MAX / -1; received rate 0 and 2.5;
  - part rate 1.0 / 1.0000001 / 0.9999999 / NaN / 3.0;
  - shield 0 / 0.9999999 / 1.0 / NaN in June's states and the village.
- `EMU_DEFENCE OK: 939 input sets, 0 mismatch(es)`.

**Hygiene.**
- `flagcheck --range 3973df2a^..3973df2a --with SERV_IRUHADEV_JUNE_STATCALC_DEBUG`: `files: 4 PASS, 0 PASS~, 1 DEFINE, 1 OTHER, 0 FAIL`.
  The OTHER is `X2DamageManager.h`, under `JUNE_ENUM_NAMES`.
- `2ec41be0^..2ec41be0`: `files: 8 PASS, 1 PASS~, 1 DEFINE, 2 OTHER, 0 FAIL`. The PASS~ is one blank line in `X2GameEdit.cpp`; the two OTHER
  are `DungeonEnum.h` and `X2SkillTree.h`, under `JUNE_ENUM_NAMES`.
- Both runs: `in this flag's files: 0 deleted line(s), 0 non-ASCII inserted line(s), 0 EOL-mismatched inserted line(s),
  0 #else/#endif without the flag token, 0 encoding change(s)` and `RESULT: PASS`.
- `git log -S SERV_IRUHADEV_JUNE_STATCALC_FINAL_DAMAGE 2ec41be0..HEAD`: empty.
- `junediverge --check-all`: `allow-listed: 207`, `RESULT: PASS`.
- Ledger: 55 rows, 54 `June` and 1 `class: char-rose`.

**Build.** Full build skipped: no code change, no play-test. The debug define stays on until the D6 review. The D3 kickoff already carries
both D2b writer items (`32621f40` / `DAMAGE_RATE_OF_PART` and `IMMUNE_TO_HP_ATTACK`). After D3, the 2 `IMMUNE_TO_HP_ATTACK` emulation sets
must compare.

## Phase D3 - The hit pipeline before the formula

**Scope.** Under `SERV_IRUHADEV_JUNE_STATCALC_HIT_PIPELINE`: `SetDamageExceptionProcess` base `322990e0` (damage terms only; its MP
terms are R1's), the user's `32045340` June-only gate, the NPC override `32759f30` (new in March: master-unit forward), `DamageCheck`'s
prelude, and the two dispatch gaps (`CEffect::AttackResult` tail: the owner's on-hit sockets for a damage effect's hit; `UnitToUnit`:
`AttackResultByType` for every non-user attacker). The eight class overrides are listed with their owners, not ported.

**Run with.** Opus 5.5 / high. Review Opus 5.5 / high.

**Read list.** "Rules", "Starting context", "Phase D0 - Done", "Phase D2b - Done".

**Done gate.** Map rows `June`; gates PASS; `DEPLOY OK`; play-test: a damage effect's hit (an arrow or a fireball) fires the attacker's
on-hit socket effect, and a summoned NPC's hit on a monster logs `HIT` with the summon as attacker.

### Phase D3 - Done (2026-11-12)

**Opus 5.5 / high. Flag `SERV_IRUHADEV_JUNE_STATCALC_HIT_PIPELINE`** (guard: D2's flag, `JUNE_COLLISION`, `JUNE_SOCKET_PARITY`,
`UPGRADE_RAVEN`, `DUNGEON_RANK_NEW`, `MODIFY_SET_DAMAGE_DATA`). Map: the 8 D3 rows `June`, one row routed to S0 (below).

**Re-checked, no code.** Base `322990e0`, user `32045340` and NPC `32759f30` read term by term against the flagged bodies: all June
(the user layer's magic-only conversion needs `FIX_EXPLOSIVE_NASOD_HAND` and `RAVEN_SECOND_CLASS_CHANGE`, both ON by `flagprobe.py`; the NPC
layer's user-owner guard is no deviation, since March owners are `CX2GUUser` by type). `UnitToUnit`'s NPC `AttackResultByType`: `ADD_RENA_SYSTEM`
is **ON**, so March already calls it (char-raven CH1q's "off" was wrong); the user branch's `31fcd620` is the `ProcessVPP` delegate (`+0x8db8`).

**Ported.**
- **`DamageCheck` prelude** (`3262f5a0`): attacker `+0x91e4` = `CX2GUUser::m_bEnterCashShop` (writer `SetEnterCashShop` `32044a80`). A user
  attacker in the cash shop, or an NPC attacker whose owner (`32759f00`) is, gets no hit; before AreaHit. New `GetEnterCashShop()`.
- **Hit part.** June's `CKTDXCollision` keeps a wstring at `+0`: every collision helper copies the B datum's `m_Name` into it right after the
  `m_bEnable` skip (`@3262c18d`, `@32611f53`, `@32625ef0`, `@32628ba4`, `@32641c83`; the C8a note "nothing reads it" is answered by
  `32621f40`). Ported as `SetCollisionDataName` in `EffectCollisionDataCheck`, which all five C8b drivers share. In `CollisionDataToUnit` B
  is the attack's manual datum, so that path keys on the attacker's datum name: `JUNE_BUGS.md` row 634 (latent).
- **`SetDefenderDamageRateOfPart`** (`32621f40`): NPC defender, rate reset to 1.0, then the first `{ part, rate }` of the now-state list
  whose part equals the hit name. Called in the non-heal branch before each of the four dodge rolls (`@32625439`, `@3262f184`,
  `@3262861c`, `@3262b358`). Loader: `StateData::m_vecDamageRateOfPart` after `FURY_DATA` (`32784c00 @32796718-3279680d`: clear,
  `[1]` name, `[2]` rate default 1.0, pushed even when a read fails). Data: `RAID_ELTRION_MK2_PHASE1 / 2 / 3` only.
- **`IMMUNE_TO_HP_ATTACK`**: `CX2GUNPC::m_bImmuneToHpAttack` (ctor false `@3276337c`), read after `SET_NUM_OF_GAGE` with
  `m_bHasBossGage` as the default (`327a0fa0 @327a454c`), tested after `GetIsMiddleBosRaidNPC()` in `CanApplyHpRateDamage`
  (`@32272745`; vslot `0x430` has no override, `vtslot.py`). The D2b stand-in comment stays in the `#else`.
- **`CEffect::AttackResult` tail** (`31f885bd-31f88669`), user owner, after `AttackResultByType`: `ProcessSocketEffectByAttackSuccess`
  (vslot `0x3f0`, made public), then unless `AT_SPECIAL` the summon socket (`32008280` = March's inline block of `CX2GUUser::AttackResult`,
  moved whole into `ProcessSummonNpcSocket()`, which both call), then for a user defender `GetFinalDamage` (vslot `0x70`) truncated into
  the owner's `AddGivenDamamge` when `g_pX2Game` exists. June's `CX2GUUser::AttackResult` has the same tally (`@32014215-32014277`):
  ported there too. A user defender means PvP, so the tally has no offline reach. March's dead `PVP_ZOOM_CAMERA` (OFF) is untouched.
- **`AddGivenDamamge`** = June `31f8df30`: add only when the tally is below `INT_MAX - x`. Divergence row `AddGivenDamamge` (`replace`,
  ported); its other callers are the NPC `DamageReact` tallies (`327b1f30 @327b4d13 / @327b4ef6`).

**Routed (not this plan's damage track).** The four dodge rolls also offset the random index by the attacker's (or NPC attacker's
owner's) `GetRandomInt( 0 )` when the NPC defender's June-only templet byte `+0xe8` is set (`@326253e2`, `@326285b9`, `@3262b2fe`,
`@3262f11a`). March's `NPCUnitTemplet` has no such field: map row (phase S0), S0 kickoff amended, with `CalcHitDodgePercent` `32610a00`.
The NPC attacker's `AdjustDamageDataBeforeDamageReact` forward (`32778fe0`) stays routed to char-luciel CH1k.

**Numbers.** `emu_final_damage_b.py`: its port model gained the immunity test; `OK: 3739 input sets, 0 mismatch(es), 0 static mismatch(es)`,
192 HP-gate sets with `IMMUNE_TO_HP_ATTACK` now compared.

**Hygiene.** `flagcheck SERV_IRUHADEV_JUNE_STATCALC_HIT_PIPELINE --worktree --files <10 files>`: 9 PASS, 1 DEFINE, 0 OTHER, 0 FAIL, 0 deleted,
0 non-ASCII, 12 EOL-mismatched inserted lines (the dodge sites sit in `X2DamageManager.cpp`'s mixed CRLF / LF region) -> `RESULT: PASS`.
`junediverge --check <10 files>`: `RESULT: PASS`. `enumscan`: 0 rows in this phase's hunks (3 old rows elsewhere in the files).
`/Zs` probes (`X2DamageManager`, `X2DamageEffect`, `X2GUUser`, `X2GUNPC`, `X2GameUnit`): 0 errors.
**Build.** `DEPLOY OK: X2_offline.exe is 26,362,368 B, dated 2026-11-12 23:08:43, md5 a75da7a4a3404e19c4b69a91c5a8860d` (built at `681c0fb5` plus
peers' uncommitted char-elesis files); `EXESTR OK: 2 of 2` (`[JUNE_STATCALC] EFFHIT owner=`, `[JUNE_STATCALC] PART npc=`, wide), `EXESTR OK: 2 of 2`
(`IMMUNE_TO_HP_ATTACK`, `DAMAGE_RATE_OF_PART`, narrow).
**Debug** (`_STATCALC_DEBUG`): `[JUNE_STATCALC] EFFHIT owner= effect= at= react=` (the tail ran for a damage effect, about once a second),
`[JUNE_STATCALC] PART npc= part= rate=` (a listed part was hit).

**Play-test owed.** ArmeDW (unit 53, Dimension Witch 99) with `Perkisas: Staff of Greed` 30391 (socket option 35335: SIEE 13, 20 % per hit,
1 s interval, `BFI_BUFF_SOCKET_PERKISAS_PERSONA`), any normal dungeon entered with the auto-party button. Gate: ranged X hits (damage effects)
give `EFFHIT owner=<ArmeDW UID>` lines and the Perkisas buff icon; the party bots' hits log `HIT att=<hero NPC id>`.

**Play-test PASSED (2026-11-12, run 23:14-23:17 on exe md5 a75da7a4; `logs.py --grep "[JUNE_STATCALC] (EFFHIT|PART)"`, `HIT` by attacker).**
- 42 `EFFHIT owner=53` lines (ArmeDW): `DamageEffect_EnergyBall_Short` / `_Big1` / `_Big2` and `ADW_MAGICBALL_SMALL_SLOW` (`at=2` AT_COMMAND), so
  the tail runs on her projectile hits. From 23:16:19 on, `DamageEffect_RedD_P1_sohan_AttackBox` (`at=1` AT_SPECIAL, `react=7`) also logs under
  owner 53: the summoned Perkisas's attack, which only appears after five stacks of option 35335's buff. The socket fired from her hits.
- Summon hits: `HIT att=1105` x131, `att=1104` x99, `att=1106` x71 (the three auto-party hero NPCs, `lv=99`) on monsters 2223 / 2224 / 2225 / 2294,
  each with a non-zero `dmg`; ArmeDW's own `att=53` x77.
- No `PART` line (no raid NPC in the run; expected). ErrorLog: 0 matching lines. The one unbound Lua name (`AddGroupChainQuest`) is not this phase's.

### Phase D3 - Review (2026-11-12, covers `617950eb`)

**Opus 5.5 / high. Verdict: pass. No transcription error, no code change.** Each item was read against June's `disasm_range.py` listing.

- **`DamageCheck` prelude (`3262f5a0-3262f658`): June.** Attacker optr DD `+0xc`. Type `+0x264` == 0 (user) and byte `+0x91e4` set, or
  type == 1 (NPC) and `32759f00`'s owner (`+0x8d5c` observer, unwrapped, no type test) has `+0x91e4` set -> `ret 0x10` with false, before
  the `hitType == 0x23` (AreaHit) test. March's live `CX2GUUser* GetOwnerGameUnit()` (`X2OPTIMIZE_UNITTYPE_BUG_FIX`) is the same read.
- **Hit name: June.** All five copies are `31f54910( B, 0, -1 )` (wstring assign of B's `m_Name` at datum `+0`) right after B's
  `m_bEnable` (`+0x1c`) skip: `@32611f53` UnitToUnit test `32611970`, `@32625ef0` MeshToUnit, `@32628ba4` CollisionDataToUnit,
  `@3262c18d` EffectToUnit, `@32641c83` UnitToEffect. In CollisionDataToUnit B is DD `+0x1c` (`pCollisionData`, the manual datum), so
  June keys on the attacker's datum there too (`JUNE_BUGS` 634 stands). June writes that one outside the defender loop and the port inside
  it; the value is the same and is only read after a hit, so nothing differs.
- **`SetDefenderDamageRateOfPart` = `32621f40`: June.** `+0x264 == 1`, dynamic cast to `CX2GUNPC`, `fld1` to `+0x8fd4` before
  `31e8a990` (now state) `+0x41c`; `32640af0` is a first-match `wstring::compare == 0` over 0x20-byte entries, rate at `+0`. The four
  sites (`@32625439`, `@3262f184`, `@3262861c`, `@3262b358`) each follow the `+0x504` / `+0x508` / `+0x588` <= 0 heal test and precede
  vslot `0x1cc` (the dodge roll), as ported. The `+0xe8` offset between them stays S0's.
- **Loader (`@32796718-3279680d`): June.** `3238bd20` clear before `BeginTable( "DAMAGE_RATE_OF_PART" )`. Each row starts fresh
  (`fld1`, name = `3344f1f4` = L""), then `[1]` and `[2]`. `32fea820` leaves the float alone on a failed read (third argument unused), and
  the row is pushed (`327c7340`) anyway. One `LoadStateData` in March, string in the exe.
- **`IMMUNE_TO_HP_ATTACK`: June.** Init reads `+0x8aeb` (HAS_BOSS_GAGE), `+0x8aec` (NUM_OF_GAGE), then `+0x8f0c` (`@327a454c`) with
  `+0x8aeb` as the failure default, then the `GetHasBossGage()` test (`@327a4586`). Ctor `@3276337c` stores 0. Getter vslot `0x430` =
  `32755a80` (`+0x8f0c`). In `322726f0` the order is `+0x8aeb`, `+0x8ae9`, `0x3c8`, `0x3cc`, `0x430` (`@32272745`), `+0x8e80`, grade, as ported.
- **`CEffect::AttackResult` tail (`31f885bd-31f88669`): June.** Owner type 0, then vslot `0x4e0` (AttackResultByType), `0x3f0`
  (CEffect `+0x200` = DD), and `32008280` unless DD `+8` == 1 (`AT_SPECIAL`). The defender `[esp+0x40]` is stored `@31f859ac` from
  CEffect `+0x670` = DD `+0x470` (funcmap row 1261). Then type 0, vslot `0x70` before the g_pX2Game holder test (`3372c0fc`), `_ftol2`,
  `31f8df30`. The tail sits after the force-die particle loops, March's place for the `UPGRADE_RAVEN` block.
- **`32008280` = March's summon block.** Checks the game holder, IsHost (`+0x2c0/4` == `+0x2b8/c`), cool time `+0x8d7c` <= 0, then the
  12-byte entries, with `GetRandomFloat( i ) <= rate`. On a pass it sets the cool time and calls `CreateNPCReq( id, level (vslot 0x20c),
  true, pos, right, 0.f, true, -1, team, NAT_ALLY, uid )`; June's extra trailing `-1, 0, 0, 0` are its defaults. User `32012b50`:
  `0x4dc`, `32008280` (`@320141ef`), `32275cf0`, `0x3f0`, then the tally on `+0x810` (DD `+0x3a0` + `0x470`), as ported.
- **`AddGivenDamamge` = `31f8df30`: June.** `ebx = 0x7fffffff - x; cmp ebx, tally; jle skip`, then a store only when the sum changed.
  Divergence row correct. (A negative `x` wraps in June and skips; the port's `INT_MAX - x` compiles to the same subtraction.)
- **Re-checks.** NPC `32759f30`: base first, then the owner's vslot `0x758` behind the `+0x867c` / `+0xaf0` tests. Base `322990e0`:
  `+0x6574 != +0x6578 -> DD +0x2f`; melee (DD `+0x2c`) reads the `+0x594c` protected multiplier, range `+0x5a00`. User `32045340`: base
  first, `+0x8b62 = 0` before the `+0x4108` (-1 or attack type) gate. All match. `flagprobe`: `ADD_RENA_SYSTEM`, `FIX_EXPLOSIVE_NASOD_HAND`,
  `RAVEN_SECOND_CLASS_CHANGE` ON, `PVP_ZOOM_CAMERA` OFF.

**Hygiene.** `flagcheck SERV_IRUHADEV_JUNE_STATCALC_HIT_PIPELINE --range 617950eb^..617950eb --with SERV_IRUHADEV_JUNE_STATCALC_DEBUG`:
`files: 9 PASS, 0 PASS~ (blank/marker only), 1 DEFINE, 0 OTHER (other flags), 0 FAIL, 0 ERROR`;
`in this flag's files: 0 deleted line(s), 0 non-ASCII inserted line(s), 12 EOL-mismatched inserted line(s), 0 #else/#endif without the flag token, 0 encoding change(s)`;
`RESULT: PASS` (the 12 EOL lines are the dodge sites in `X2DamageManager.cpp`'s mixed region; the `#else` paths are March's bodies).
`git log -S SERV_IRUHADEV_JUNE_STATCALC_HIT_PIPELINE 617950eb..HEAD`: none. `junediverge --check-all`: `allow-listed: 207`, `RESULT: PASS`.
**Numbers.** `emu_final_damage_b.py`: `OK: 3739 input sets, 0 mismatch(es), 0 static mismatch(es); 12 XS_PVE_GAME set(s) not compared
(no March state), 192 HP-gate set(s) with IMMUNE_TO_HP_ATTACK compared (D3)`.
**Play-test log re-read** (one run, 23:14-23:17, exe md5 a75da7a4): 53 `EFFHIT owner=53` lines (44 `at=2`, 9 `at=1` Perkisas
`RedD_P1_sohan_AttackBox`; the Done note's 42 was an early count), 0 `PART`, `HIT att=1105` x133 / `1104` x101 / `1106` x72 / `53` x79.
Conclusion unchanged.
**Build.** Full build skipped: no code change, no play-test. The debug define stays on until the D6 review. D4 kickoff: one bullet added
(the NPC `DamageReact` tallies already reach June's `AddGivenDamamge`).

## Phase D4 - DamageReact: from the final damage to the HP change

**Scope.** Under `SERV_IRUHADEV_JUNE_STATCALC_DAMAGE_REACT`, the damage arithmetic of NPC `DamageReact` `327b1f30` and user
`DamageReact` `320462a0` between the `GetFinalDamage` call and `UpNowHp`: the reduction sums, `GetSocketReducedDamagePercentInCase`'s
June body moved into March's own function (the June-named twin goes), `322cfca0`, the order of the HP-rate overwrite, the training
clamp's June-only term, the damage number shown. Reaction, hit-stop, sound and buff code in the same functions is not this plan's.
`REACT` debug lines. The `DI_EVENT_ALCHEMY_LAB` block is ported only if that dungeon is registered by then; otherwise its row says who
owns it.

**Run with.** Opus 5.5 / high. Review Opus 5.5 / high.

**Read list.** "Rules", "Starting context", "Phase D0 - Done", "Phase D2b - Done".

**Done gate.** Map rows `June`; gates PASS; `DEPLOY OK`; play-test: for ten logged hits each way, `REACT`'s HP change equals the `GFD`
damage after the listed reductions, on a unit with damage-reduction sockets and one without.

### Phase D4 - Cut (2026-11-12)

Read-only, as the kickoff orders: cut both sections, count, split, stop. No client code, no flag; full build skipped: no play-test.
flagcheck / junediverge: not applicable (no source touched).

**The sections were longer than D0 said.** D0's end points (`@327b3da5`, `@32048fab`) are the attacker's HP-drain `UpNowHp`, not the
defender's. The defender's are `@327b4c11` (NPC) and `@32049b30` (user, then the tutorial clamp to `32049b70`). Map rows 17 / 18 corrected.

**Ledger** `june2017_re/statcalc/dr_blocks.tsv` (columns `side range computes march_lines state flag phase note`): NPC 22 contiguous
ranges `327b33f0-327b4c13` (2 `June`, 6 `absent`, 4 `mixed`, 9 `unknown` = claimed by an older flag, not re-read, 1 `n/a` = number
position); user 29 ranges `32048435-32049b70` (3 `June`, 5 `absent`, 5 `mixed`, 16 `unknown`). Check, from the Trunk:
`python -c "import csv;r=list(csv.reader(open('june2017_re/statcalc/dr_blocks.tsv'),delimiter='\t'))[1:];[print(k,all(a[1]==b[0] for a,b in zip(s,s[1:])),s[0][0],s[-1][1],len(s)) for k in('npc','user') for s in[[x[1].split('-') for x in r if x[0]==k]]]"`
prints `npc True 327b33f0 327b4c13 22` and `user True 32048435 32049b70 29`. User `DamageReact` has no decompile (Ghidra's fails):
the cut reads the listing `june2017_re/buffre/dec/320462a0.s`.

**Findings for the porting phases.**
- Both sides call vslot `0x378( DD, &damage )` when DD `+0x4fc` is set (`@327b35fe`, `@320489f3`), and both have a state-10 skill-templet
  multiply plus skill 30205 after the critical block (`327b39cf`, `32048cdb`) where March has its back-attack `x1.1`.
- NPC-only: ids `0x772` / `0x791` damage scale, the element pair's two extra id pairs, a fixed damage `+0x8f84`, a protected flag `+0x75bc`
  that zeroes damage in game type 2, a `+0x38 == 4` tally case, a `+0x8488` call before `UpNowHp`.
- User-only: an attack-type-1 skill-templet multiply right after `GetFinalDamage`, a `+0x9175` branch beside Ara's quicken guard, June's own
  Elesis provoke gate, a state-10 / `32aebaf0` multiply, a max-HP block (`3204968f`) before the HP-rate overwrite.
- `DI_EVENT_ALCHEMY_LAB` (41350) is not in the live `DungeonData.lua`: owner backlog Stage 19 (already listed there); map row 21 says so.

**Sizing.** 20 `absent` / `mixed` blocks (10 a side, Stage 19's not counted) plus 25 `unknown`: split by side into **D4a** (NPC, probe
build, no review of its own) and **D4b** (user, full build and the play-test for both); `D4-review.md` covers both. Kickoffs `D4a.md`
(was `D4.md`), `D4b.md`, `D4-review.md`; STATUS rows updated.

### Phase D4a - Done (2026-11-13)

**Opus 5.5 / high. Flag `SERV_IRUHADEV_JUNE_STATCALC_DAMAGE_REACT` (new; D4b continues under it).** Commit: this commit. Insert-only:
`X2GUNPC.cpp` +183, `X2Game.cpp` +16, `Always.h` +15, `X2GUNPC.h` +13, `X2Game.h` +7, `X2DamageManager.h` +6, `X2GameUnit.h` +4, `X2Main.cpp` +3,
`X2DamageManager.cpp` +3; June names under `SERV_IRUHADEV_JUNE_ENUM_NAMES`: `X2UnitManager.h` +7 (`NUI_BARRIERMACHINE_FIRE_HENIR` 1767, `_ICE_HENIR` 1768,
`NUI_NASOD_ELEMENT_FIRE_HENIR` 1805, `_ICE_HENIR` 1806; free values, checked on the preprocessed enum). `dr_blocks.tsv`: every `npc` row `June` except
the number position (`n/a`) and Stage 19's alchemy lab (`absent`). Map rows 17 (`June`), 28, 31; funcmap 16 rows `StatCalc S33 D4a`.

**Ported (June `327b1f30` from @327b33f6, in June's order):**
- @327b33fc Lu/Ciel's summoned darkness (`NUI_SI_SA_LLD_SUMMON_DARKNESS` / `_PVP`): r = `m_KeyCode` x 0.001; it keeps damage x ( 1 - r ) and its AI
  owner (new `CX2Game::GetUnitByUIDAndType`, June slot `0xc8` `322d80a0`) loses r x the kept damage via `UpNowHpWithText` (JUNE_BUGS 640). Crash guard:
  only an ally AI (type 1 or 4) is read. Unreachable until Lu/Ciel (Stage 8).
- @327b3491 element NPCs: June adds the Henir ids (ice 1806 -> barriers 235 / 1768, fire 1805 -> 234 / 1767); `+0x8754` = `m_FlagList[3]`. March in `#else`.
- @327b35db `SetDamageValueByForce` (Lua-bound `31e4d1c0`, `+0x8f84`, ctor -1): above -1 it replaces the damage, before the enchant resist. Bound in
  X2Main.cpp; June data: `PVE_PATTRN_AI_Nasod_Element_Fire` / `_Ice`.
- @327b35fe slot `0x378` = `ApplyEnchantResist( m_ResistType )` (`3226dd20`, = March, no override): already in place (DUNGEON_ITEM on).
- @327b41c9 NPC Init `NOT_APPLY_DAMAGE_IN_DUNGEON` (`+0x75bc`): damage 0 in `GT_DUNGEON` (Eve's Nasod Watch / Electronic Field, Add's Surface Cutting,
  Ferdinand). Its other reader, June's NPC `UpNowHp` override `32784200`, is routed to D5 (kickoff amended); `32778e40` has no caller.
- @327b4b3f sub-stage ally NPC (AI type 4, no enum name): game `+0xb24` += int( damage ) (`AddSubStageAllyNpcDamage`); its readers `322dbef0` /
  `322ef1b0` (a monster-HP damage share) are not in March: the member has no reader yet.
- @327b4bad the state's `DAMAGED_FUNC` (InitState key, `+0x8488`) runs before `UpNowHp` when damage > 0, as March's `ATTACKED_FUNC` call. June data:
  `FOOLSDAY_FUNMECHANIC`, `FOOLSDAY_WASH`, `Treasure_Mimic`. Its sibling `DIED_FUNC` went to backlog Stage 19.
- `GetDrainHpBuffValue` (`32071c80`, @327b3b50) differs: June adds the buff rate to the script's `DRAIN_HP_RATE` (DD `+0x4e4`, copied at load
  `3263afdf`); March overwrote it. New `DamageData::m_fDrainHpRateFromScript`; `june_march_divergent.tsv` row, replaced (both DamageReacts call it).

**Re-read, = June (no code):** GetFinalDamage call; critical (rate + DD `+0x5d4`, WayOfSword, anti-critical clamp, `32273480` makes no roll, the
`GetRandomFloat( 1 )` test, `GetCriticalDamageRate` by jump table `327b5390`); `ApplyHeadShotDamage` `322978e0`; double attack; HP and MP drain;
additional defence, k, Add stasis, slots `0x4f8` / `0x4fc`; socket cool time; `322cfca0` + slot `0x304` and slot 6 in place; tally; hitted count.
`ADD_BACKATTACK` is undefined (`cl /EP`): March's x1.5 / x1.1 are already dead code. The number shown = March's value; June's number queue
`32299d60` (skins, delay, critical / double particles) and `SHOW_DAMAGE_AT_IMPACT_POINT` are display: backlog Stage 20 bullet.

**Class hand-off.** The cut's "state 10" is `GetUnitType() == UT_LUCIEL`: @327b39cf a hit from behind sets DD `+0x5c0`, then Lu/Ciel's members
`+0x95f8` / `+0x9600` / `32ae5120` (ORIAX buffs -> `+0x9604`) scale it: `stand-in: Lu/Ciel not ported` comment, checklist 7aw.

**Debug line** (`_DEBUG`, same throttle as `GFD2`): `[JUNE_STATCALC] REACT att= def= gfd= share= pre= crit=<0/1>><after> double= dungeon= add= hook=
dmg= hp=<before>><after>` (after GetFinalDamage, the owner share, before the critical, after it, after x2, after the dungeon guard, after the
additional damage, after the hooks; then the HP change).

**Hygiene.** `flagcheck SERV_IRUHADEV_JUNE_STATCALC_DAMAGE_REACT --worktree --files <10 files>`: 8 PASS, 0 PASS~, 0 DEFINE, 2 OTHER (Always.h, the
enum header under `JUNE_ENUM_NAMES`), 0 FAIL, 0 deleted, 0 non-ASCII, 0 EOL -> `RESULT: PASS`. `junediverge --check <9 files>`: `RESULT: PASS`.
`enumscan --reuse --check <8 files>`: 0 A/B rows. Probes (`/Zs`, US_SERVICE): `X2GUNPC`, `X2Game`, `X2DamageManager`, `X2Main`, `X2GameUnit` 0 errors,
no new warnings. The `#error` guard first named `X2OPTIMIZE_NPC_NONHOST_SIMULATION` (KTDX.h defines it after Always.h): dropped.
After commit `3812c4cd`: `flagcheck --range 3812c4cd^..3812c4cd`: 8 PASS, 1 DEFINE, 1 OTHER, 0 FAIL, 0 deleted -> `RESULT: PASS`.
**Build.** Full build skipped: no play-test (D4b builds and hands off the play-test for both sides; its kickoff lists D4a's checks).

**Bookkeeping.** `dr_blocks.tsv`, map, funcmap, divergence row, JUNE_BUGS 640, checklist 7aw, backlog Stages 19 / 20, D4b and D5 kickoffs, `MODS.md`.

### Phase D4b - Done (2026-11-13)

**Opus 5.5 / high. Flag `SERV_IRUHADEV_JUNE_STATCALC_DAMAGE_REACT` (D4a's; two description lines added).** Commit: `c0a28ccc`. Insert-only:
`X2GUUser.cpp` +133, `X2GUUser.h` +3, `Always.h` +2. `dr_blocks.tsv`: user 29 rows, 22 `June`, 6 `absent` (each a one-line stand-in or a routed
write), 1 `n/a` (number display). Map rows 18 / 19 / 20 / 28 `June`, 31 re-checked; funcmap 10 rows `StatCalc S33 D4b`; divergence rows
`GetBasicReducedDamagePercent` and `GetSocketReducedDamagePercentInCase` closed (`replace`, ported under the flag).

**Ported (June `320462a0`, in June's order):**
- @32048446 an AT_SPECIAL hit whose skill (`GetSkillTemplet( m_eUsedSkillID )`) is missing or not `ST_HYPER_ACTIVE_SKILL` (`32070510` != 5) is
  x the new `m_fDamagedRateBySpecialActive` (June `+0x8e94`, ctor 1.0 @32040e56). Its only writer is Lu/Ciel's Nobility (`RNSI_P_LCN_NOBILITY`
  30410, `SA_DAMAGED_REL`, @32b45ec0): x 1.0 for every March class (checklist 7j).
- @32049359 Elesis Grand Master's provoke: `31f8ae70` is "attack type == AT_SPECIAL", so June takes every attack type but AT_SPECIAL
  (March: AT_NORMAL only). The rest of the gate (unit / class, `+0x967c`, DD tests, hold / grab / riding / attacker front) = March.
- `GetBasicReducedDamagePercent` (vslot `0x348` = `3201aff0`, user vtable `334781f4`) and `GetSocketReducedDamagePercent` (`32000320`) both start
  with GT_PVE (`322cc1c0`) -> 0. The 16-step3b5 port left that out when March had no GT_PVE. The rest of both = June (re-read: set sum, interval
  store, title return, random `+0x60`, DUNGEON_ONLY 2 / 3 + type, PVP_ONLY `322cc180`). No offline reach (no GT_PVE game).
  March's `GetSocketReducedDamagePercentInCase` and its callers stay as they are: they are compiled out on US (`ADJUST_SECRET_ITEM_OPTION`
  undefined) and have no live caller, and deleting them would break insert-only. The live function is the one with June's body.

**Re-read, = June (no code):** GetFinalDamage call; Raven and Eve parries (0.7, 0.25, 70, 100, effect sets, Raven's sound); the basic reduction
(monster term, boss, `52fc`, cap 1.0); penetrate; bone shield / SIEE 5; hitted count; slot `0x378` = `ApplyEnchantResist` (= March, in place);
critical (head shot, rate + `+0x5d4`, WayOfSword, anti-critical clamp, threshold + `GetRandomFloat( 1 )`, stigma, jump table `3204a834` =
`GetCriticalDamageRate`, >= 1.0); double attack; `GetDrainHpBuffValue` in place (after x2, before the drain); HP and MP drain; Ara quicken guard
x 0; additional defence; additional attack k; provoke zero; Add stasis; hooks `0x4f8` / `0x4fc`; `322cfca0`, then `GetHpRateDamage`, then slot
`0x304` (June's order, JUNE_BUGS 93); Add `0x6d8` / Lunatic Psyker `0x6f8`; damage count; random mission; Add DP; `UpNowHp( -damage )`; the
tutorial clamp. June runs the random mission before the Add DP, March after it. The two are independent, so the order was left.

**Stand-ins and routes (one-line comments in place):** @32048cdb Lu/Ciel back-hit scale (checklist 7aw); @320492ba Elesis `+0x9175` guard,
whose owner is now identified: set @320476af on unit type 8 by her roll `32b524f0` on `+0x978c`, damage x `+0x9784` (checklist 7j / L477);
@320492d9 Rose `32a7ff70`; @3204963d Lu/Ciel shadow shield (7j); @3204968f the 2017 treasure event (Stage 19); @320499dc Elesis `0x730` /
`0x734` (char-elesis CH4c); @32049aeb Ain `32622e80` (Stage 8). Not class code, readers unknown, so the writer and its readers go together
(rule 3): `320832a0` (the hit stored on the attacker / NPC owner, `+0x90fc`) and `31fdcb70` (hits per attacking NPC id, `+0x8854`), backlog Stage 40.
The number display (`@32049752-3204998e`, skin vslots `0x230` / `0x22c`, queue `32299d60`) extends D4a's Stage 20 bullet.
**Kept March, not in June here:** the `NEXON_QA_CHEAT_REQ` zero (in-house dev cheat) and `ProcessSkillAtDie`. June's death prevention
(`320072d0` in `UpNowHp`) is not whole yet (checklist L261; Rena's Soft Body still lives in `ProcessSkillAtDie`).

**Numbers.** The only new arithmetic is x `m_fDamagedRateBySpecialActive` (1.0) and a gate, so there is no emulation script (D4a likewise).
**Debug line** (`_DEBUG`, same throttle): `[JUNE_STATCALC] REACT att= def= gfd= special= pre= crit=<0/1>><after> double= guard= add= hprate= dmg=
hp=<before>><after>`: after GetFinalDamage, the special-active rate, before the critical, after it, after x2, after the guards + provoke +
additional defence, after the additional attack + provoke zero, after the PvP halving + HP rate + slot `0x304`; then the HP change (my unit only).

**Hygiene.** `flagcheck SERV_IRUHADEV_JUNE_STATCALC_DAMAGE_REACT --worktree --files X2GUUser.cpp X2GUUser.h Always.h`: 2 PASS, 1 DEFINE, 0 OTHER,
0 FAIL, 0 deleted, 0 non-ASCII, 0 EOL -> `RESULT: PASS`. `junediverge --check X2GUUser.cpp X2GUUser.h`: `RESULT: PASS`. `enumscan --reuse --check`:
the 2 rows left are older lines (`X2GUUser.h:2561-2562`, NPC ids), not this phase's.
After commit `c0a28ccc`: `flagcheck --range c0a28ccc^..c0a28ccc`: 2 PASS, 1 DEFINE, 0 OTHER, 0 FAIL, 0 deleted -> `RESULT: PASS`.
**Build.** `DEPLOY OK: X2_offline.exe is 26,366,976 B, dated 2026-11-13 00:59:45, md5 798e1d603edc6201a12db70400e4528d, equal to X2/US_SERVICE/x2.exe (2026-11-13 00:59:38); built at main 79ef54ec + 3 uncommitted code file(s).`
`EXESTR OK: 2 of 2 found in X2_offline.exe, 26366976 B, dated 2026-11-13 00:59:45, md5 798e1d603edc6201a12db70400e4528d` (the user `REACT ... special=` and D4a's NPC `REACT ... share=` lines).

**Play-test** (both sides, D4a and D4b): owed. Save set up (backups in `db_backup/`): Raven99 (unit 49) wears four Grendized Mechanized MK-2 (Raven) pieces 111816-111819 = set 1365
4-piece option 41149 (`m_fDamageReduce` 0.5 at 30 %, dungeon only; no option in June's data always fires); ArmeDW (unit 53) has no reduction set or title.
jtCodeEmpress (unit 27): Electronic Field 25020 in slot 5, Dimension Link Sentry 25000 (level 1) in slot 6. Henir element NPCs 1805 / 1806 spawn in no
dungeon: the element check uses Altera Plain Normal 30270 (232 / 233 + barriers 234 / 235, last stage). `DAMAGED_FUNC` NPCs (3112 / 3113 / 3134) appear
only in the unregistered 2017 event dungeons 41460 / 41470: not reachable offline.
**Play-test run 1 (2026-11-13 05:02-05:09, exe md5 798e1d60; `logs.py --grep "\[JUNE_STATCALC\] REACT"`): 170 REACT lines, 3 of 4 checks pass.**
- HP change = `dmg` on every line: 38 user (Raven99 49 x 11, ArmeDW 53 x 23, jtCodeEmpress 27 x 4), 132 NPC (15 ids incl. 1466, 530, 232 / 233 / 234).
- Nasod elements 232 / 233 with barriers up: `pre=1` (forced 1), then `add=` lifts it by the attacker's additional damage (e.g. `pre=1 ... add=266
  dmg=266`), June's order (the barrier test sits before the additional-damage step). The hand-off's "takes 1" was too strict.
- Eve summons: Electronic Field 1622 and Nasod Watch 384 `dmg=0`, HP unchanged (`NOT_APPLY_DAMAGE_IN_DUNGEON`).
- ArmeDW (no reduction set): `pre = gfd` on all 23; `guard < double` on all 23 (her additional defence).
- **Open:** Raven99's set option 41149 (0.5 at 30 %) shows on none of 11 logged hits (`pre = gfd`; about 2 % if it works). Static checks hold: kim
  set 1365 grants 41149 at 4 parts, pieces carry set 1365, live `OptionItemData` has 41149 (`DUNGEON_ONLY` read by `X2SocketItem.cpp:559`).
  Retest with 30+ logged hits before the review; none -> trace `InitSetItemOptions` / `GetBasicReducedDamagePercent` at runtime.
**Play-test PASSED (retest 2026-11-13 05:12-05:14, same exe):** Raven99 (49) 12 logged hits, 5 with `pre` = exactly 0.500 x `gfd` (option 41149;
e.g. `gfd=20820 pre=10410 ... dmg=10410 hp=179286>168876`), HP change = `dmg` on all 15 lines. Run 1's 0 / 11 was chance. D4 review next.

### Phase D4 review - Done (2026-11-13)

**Opus 5.5 / high. Covers D4a (`3812c4cd`) and D4b (`c0a28ccc`). Verdict: pass, no code change.** No transcription error found.

**Ledger.** The cut note's check prints `npc True 327b33f0 327b4c13 22` and `user True 32048435 32049b70 29`. States: npc 20 `June`, 1 `absent`
(Stage 19's alchemy lab), 1 `n/a`; user 22 `June`, 6 `absent` (each a one-line stand-in or a routed write, as D4b lists), 1 `n/a`.

**Re-read against `unpacked_x2.exe`** (`disasm_range.py`, the `.s` listings):
- NPC @327b33fc: ids 0x772 / 0x791 = 1906 / 1937 (June `Enum.src.lua`); owner via game slot `0xc8` with ( `+0x78` / `+0x7c` UID, `+0x80` type );
  r = float( `+0x874c` x 0.001 ), damage = ( 1 - r ) x damage, then the owner gets -damage x r (`322780b0`). Same as the port (JUNE_BUGS 640).
- NPC @327b3491: ice 233 / 1806 checks barriers 235 / 1768, fire 232 / 1805 checks 234 / 1767, each with `+0x8754` == 1 -> 1.0. All six values
  match June's `Enum.src.lua`.
- NPC @327b35db: `+0x8f84` replaces the damage only when > -1 (`test ah,0x41`); its writers are the ctor @327633a7 and the Lua setter `31e4d1c0`.
  The other `+0x8f84` hit (`31fde67f`) belongs to a user-class ref-counted member, not this one. Slot `0x378` = `3226dd20` = `ApplyEnchantResist` (funcmap).
- NPC @327b41c9: the protected byte `+0x75bc` == 1 and game type (slot `0x10c`) == 2 -> 0. Its place (after the MP drain, before the
  additional defence) is the port's.
- NPC @327b4b3a: AI `+0x38` == 4 -> game `+0xb24` += `_ftol2`( damage ) (truncation = the port's cast). @327b4bad: `+0x8488` != -1 and damage > 0
  (`test ah,5; jp`) -> the call with ( app, game, this ), after the hitted count and before `UpNowHp` (slot `0xec`).
- User @32048446: DD `+0x8` == 1 (`AT_SPECIAL`); `GetSkillTemplet( +0x590, 1 )`; null, or `32070510` != 5 -> x `+0x8e94`. `32070510`'s jump
  table (`3207054c`) maps only `m_eType` 13 to 5, and `ST_HYPER_ACTIVE_SKILL` = 13, so the port's direct type test is exact. June's
  `GetSkillTemplet` second argument is the level gate (`IsEnableCode` 0x910 / 0x911, server code events never set offline) = March's one-argument
  call here. `+0x8e94` scan of the exe: user writes only at ctor @32040e56 and Lu/Ciel `InitPassiveSkillState` @32b45e7b (1.0) / @32b45ec0
  (Nobility). The `3276` / `3277` / `327a` hits are an NPC member at the same offset.
- User @32049359: `31f8ae70` returns ( type == 1 ), so provoke runs on every type but AT_SPECIAL. The rest of the gate (`+0x967c` cleared first,
  `+0x4dc`, HP / MP up <= 0, RT 7 / 10, hit type 26, hold / grab / riding / attacker front, set back to 1 on success) = March's block.
- User @320475c1: unit `+0xaf0` type 8 (UT_ELESIS), reaction 1 / 2, then `32b524f0` -> `+0x9175` = 1 @320476af. Matches the stand-in comment.
- User @3204968f: game `+0xbbc` = the 2017 treasure event manager (funcmap rows 2062 / 2063; `32d5c800` = "attacker in its unit list") ->
  max HP x 0.1 and slots `0x11c` / `0x120`. Stand-in correct, Stage 19.
- Order: `322cfca0` @32049683, then the treasure block, then `32273e80` @32049727, then slot `0x304`. That is the port's order. The tutorial clamp stays after `UpNowHp`.
- `GetSocketReducedDamagePercent` `32000320` @3200034b and `GetBasicReducedDamagePercent` `3201aff0` @3201b026: `322cc1c0` (game type == 4 =
  `GT_PVE`) is the first test, -> 0. Both heads = the port. Both divergence rows are closed.
- **`GetSocketReducedDamagePercentInCase` left in place: enough.** The US_SERVICE preprocessor output (`.build/enumscan/X2GuEve.i`, 2026-11-13,
  includes `X2GUUser.h`) has no declaration of it: `Always_US.h:4` undefines `ADJUST_SECRET_ITEM_OPTION` after `Always.h:703` defines it.
  So it has no live caller. Deleting it would only break insert-only.

**Gates.** `flagcheck --range 3812c4cd^..3812c4cd`: 8 PASS, 1 DEFINE, 1 OTHER (`X2UnitManager.h`, `JUNE_ENUM_NAMES`), 0 FAIL, 0 deleted -> `RESULT: PASS`.
`flagcheck --range c0a28ccc^..c0a28ccc`: 2 PASS, 1 DEFINE, 0 FAIL, 0 deleted -> `RESULT: PASS`. No later commit touches the flag
(`git log -S`). `junediverge --check-all`: allow-listed 207, `RESULT: PASS`. `_STATCALC_DEBUG` stays on (D6 review retires it).
**Build.** Full build skipped: no play-test (no code change).

**Play-test log** (still on disk; the run is after the deploy, md5 798e1d60): 15 `REACT` lines. HP change = `dmg` on all 15 (script check). Raven99 (49),
hit by NPC 530: 12 lines, 5 with `pre` / `special` = 0.5000 (option 41149), 7 at 1.0000, no critical. Run 1's 170 lines were overwritten by the
retest. D4b's note recorded them: user ArmeDW `pre = gfd` x 23, NPC 232 / 233 forced 1, Eve summons 0.

## Phase D5 - Extra damage, enchant resist and level correction

**Scope.** Under `SERV_IRUHADEV_JUNE_STATCALC_EXTRA_DAMAGE`: `LevelCorrectionExtraDamage` NPC `32760210` and the user's twin (June's
dungeon list: adds 41280, drops the Henir branch), the extra-damage sum `3226e530`, the per-tick damage of the extra-damage types as
the map lists them, `GetEnchantResist`'s user override `31fd8790` with its six per-type floats and their writer (`31fe19d0`
@31fe1b84-31fe1d0d) and the NPC override `32757c50`. Missing EDT cases (black-out, mark list) stay with backlog Stage 36.

**Run with.** Opus 5.5 / high. Review Opus 5.5 / high.

**Read list.** "Rules", "Starting context", "Phase D0 - Done".

**Done gate.** Map rows `June`; gates PASS; `DEPLOY OK`; play-test: poison, fire and frozen ticks on a monster and on the player match
the log's computed tick; a resist-socketed unit shows the lower tick.

### Phase D5 - Done (2026-11-15)

**Opus 5.5 / high. Flag `SERV_IRUHADEV_JUNE_STATCALC_EXTRA_DAMAGE` (new).** Commit: this commit. Insert-only: `X2GUNPC.cpp` +114, `X2GUUser.cpp` +80
(debug only), `Always.h` +15, `X2DamageManager.cpp` +5, `X2GUNPC.h` +3, `DungeonEnum.h` +5 (`DI_EVENT_WALLY_LAND` 41280 under `JUNE_ENUM_NAMES`,
value proved by `enumprobe`). Map rows 22 / 23 / 24 `June`, 4 rows added; divergence rows `LevelCorrectionExtraDamage`, `GetEnchantResist` updated,
`GetCalcPerDamage` added (replaced); funcmap 10 rows `StatCalc S33 D5`. Flags read by `cl /Zs` (not by `#ifdef`): `ADDITIONAL_MEMO`, `CHILDRENS_DAY_EVENT_DUNGEON`,
`EXTRADAMAGE_RATE_BUG_FIX`, `SUMMON_MONSTER_CARD_SYSTEM`, `DAMAGEDATA_RATE_MODIFIER`, `CHUNG_MEMO_01`, `EXTRA_DAMAGE_CUT_NEGATIVE_RATE` on; `NEW_HENIR_DUNGEON`,
`SERV_HALLOWEEN_EVENT_2013` off.

**Ported:**
- NPC `LevelCorrectionExtraDamage` `32760210`: June's level-1 list @327602ed-3276044e (Halloween 32500-32502, defence 38300-38302, Valentine 38400,
  tree day 39500-39550, `DI_EVENT_WALLY_LAND` 41280); March's Henir challenge case (38100) is gone. The rest re-read = March (constants 0.025 / 0.5 / 500
  read from the exe; cap 0.5, floor 0, rate modifier with no `< 1` test).
- NPC `GetEnchantResist` `32757c50`: after the 150-per-enchant and NPC-stat terms June adds the base `32274b00` (buff change attribute + item resist map,
  `ecx` = this @32757cdc). D0 had called it `= March`. Every reader goes through the virtual (buff resist, frozen roll, extra damage): no double count.
- NPC `UpNowHp` override `32784200` (new, slot `0xec`): a `NOT_APPLY_DAMAGE_IN_DUNGEON` NPC refuses a change <= 0 in `GT_DUNGEON` (NaN passes, as June's
  `fcomp`), then the base. Its tail (@327842cc-3278435e) is a June dev diagnostic: the HP goes into game map `+0x334` by NPC UID (`322e28e0`, existing
  entries only), and when game `+0xba4` == 1 June writes to address 0. `+0xba4` is set only by a chat-box command (`31ec2921`, string 39279). The only reader
  is the NPC's `CommonEventProcess` trace (`322d8b80`). March has none of it. **Not ported, routed to backlog Stage 40** (kickoff said keep the null write with
  a JUNE_BUGS row; it is a debug trap with no setter here, and porting it would mean porting the command and the map: drift). NPC `SetNowHp` `32778c60` = base +
  the same tail: nothing to port.
- `GetCalcPerDamage` `32622300` (the per-tick value both LevelCorrections write): bases 80 / 145 / 60 / 82 (EDT 1 / 3 / 8 / 18), 151 / 156 scripted,
  `x (1 + 0.2 (lv - 1)) x (1 - resist / 500)` = March. Only the PvP-channel curve's test differs: June PvP **or** PvE (`322cc1a0` = `GetIsNowPVPCorrectionGame`),
  channel classes 9 / 11 / 10 = March's names. No offline reach.

**Re-read, = June (no code):** user `LevelCorrectionExtraDamage` `31fe6db0` (no dungeon list; rate from `m_fPureRate`; null tests; floor 0); user
`GetEnchantResist` `31fd8790` = char-chung CH4a2's port; its writer `31fe19d0` @31fe1b80-31fe1da6 (26401 SA `0x58` x6, 28405 SA `0x58` blaze, 30409 SA `0x54` x6) =
`InitPassiveSkillState`; base `32274b00` = March. Frozen (EDT 17) has no tick base in June either (0 per second for a levelled frozen).

**Lead for the review (map row `extra-damage tick application`, `unknown`):** the per-frame tick sites were not D5 rows and were not read. March's user
`CommonFrameMoveFuture_ExtraDamage` frozen block subtracts `m_Fire.m_DamagePerSec`; compare NPC `3276b400` and the user's June twin.

**Debug** (`_DEBUG`): `[JUNE_STATCALC] EDT def=npc <uid> id=<npc> | def=user <uid> type= lv= resist= perSec= rate=` after each LevelCorrection (once a second per
unit and type; written for every hit's extra damage, proc or not), and `EDTTICK def=... poison= fire= cold= lost= in=` once per second of HP lost while a
poison / fire / cold tick runs (hits in that second are counted too).

**Hygiene.** `finish.py --files <6 files> --with SERV_IRUHADEV_JUNE_STATCALC_EXTRA_DAMAGE`: flagcheck 4 PASS, 0 DEFINE, 2 OTHER (`Always.h`: peers' uncommitted
defines; `DungeonEnum.h`: under `JUNE_ENUM_NAMES`), 0 FAIL, 0 deleted, 0 non-ASCII, 0 EOL, 0 bare `#else` -> `RESULT: PASS`; junediverge `RESULT: PASS`;
enumscan exit 0. Probes (`/Zs`, US_SERVICE): `X2GUNPC`, `X2GUUser`, `X2DamageManager` 0 errors.
After commit `a93c5420`: `finish.py --range a93c5420^..a93c5420 --with ..._EXTRA_DAMAGE`: flagcheck 4 PASS, 1 DEFINE, 1 OTHER (`DungeonEnum.h`, `JUNE_ENUM_NAMES`),
0 FAIL -> `RESULT: PASS`; junediverge PASS; enumscan exit 0; stateends skipped (no skill state registered). `D5-review.md`, dropped by a racing peer commit
(`833d1d53`), restored in its own commit.
**Build.** `DEPLOY OK: X2_offline.exe is 27,020,800 B, dated 2026-11-15 22:24:18, md5 49fa44dc1524480474d0ac42dc78c1d3, equal to X2/US_SERVICE/x2.exe (2026-11-15 22:24:11); built at main fac2180d + 38 uncommitted code file(s).`
(coalesced: a peer's full build started after the last D5 edit.) `EXESTR OK: 2 of 2 found in X2_offline.exe, 27020800 B, dated 2026-11-15 22:24:18, md5 49fa44dc1524480474d0ac42dc78c1d3` (the NPC `EDT` and user `EDTTICK` lines).

**Play-test save** (backup `db_backup/els_db_20261115_221853_pre_d5-before.sql`): jtLordKnight (25) Apocalypse sword (item 2435) socket 25001 (poison);
jtRuneSlayer (26) and jtInfinitySword (71) each got a new equipped Apocalypse sword (4379 / 4380) with 25000 (fire) / 25004 (cold). The three options proc
at rate 1.0 for 100 s (`OptionItemData`, `LEVEL = 0`), so the rate is capped at 0.5 and the tick is the attacker-level curve. Target: Altera Plain Normal
stage 2, Nasod Element Fire 232 / Ice 233 behind their barriers (1 damage a hit, so they live). No reliable monster puts these ticks on the player
(the player-side check is best effort: Hamel secret dungeon's Dark Priest, a March fire attack); the user body is unchanged code.
**Play-test run 1 (2026-11-15 22:30-22:48, exe md5 49fa44dc, after the deploy): poison and fire pass, cold not run.** The user fought Elysion field
monsters (Plingchu G / R 2129 / 2130, Pyurita 2131, Pimpim 2134), not the Altera elements. Script over every `EDT` / `EDTTICK` line:
- jtLordKnight (25), poison: `EDT ... type=1 lv=99 resist=350.0 perSec=494.40 rate=0.292` (= 80 x 20.6 x 0.3; rate (19 x 0.025 + 0.5) x 0.3); 2134's 43 clean
  seconds: median `lost` 493.9 / s against `perSec` 494.40.
- jtRuneSlayer (26), fire: `type=3 lv=99 resist=50.0 perSec=2688.30 rate=0.500` (= 145 x 20.6 x 0.9; cap 0.5) on four monsters; 116 seconds, median 2688.8 / s
  (2651 / 2726 = one frame more or less). Seconds with hits in them read higher, as the note says.
- Resisted tick: the same monsters resist poison 350 and fire 50, so poison ticks at 30 % of its 1,648 base and fire at 90 %. 0 formula mismatches over 32 `EDT`
  lines; the other 13 are EDT 16 at level -1 (fixed, no tick). ErrorLog: no `JUNE_STATCALC` line, no crash.
- Cold: jtInfinitySword (71) hit 5154-5157 with no `EDT type=8` line, because a cold socket never loads. March's loader (`X2SocketItem.cpp`, the
  `m_SocketExtraDamage` switch) keeps only EDT 1 / 2 / 3 / 4 / 5 / 6 / 16 / 18, and June's (`32729267`, table `32737230` -> `3273721c` group 0, `+0x324`) keeps the same
  eight; 8 falls to the default in both. Option 25004 is dead data in June too, so this is not a defect (funcmap row). The cold tick base (60) sits in the same
  `GetCalcPerDamage` table as poison and fire. Ticks on the player still have no reliable source: review / D6.
**Play-test: passed** (poison, fire and the resisted tick by log; cold is unreachable through a socket in June as well).

### Phase D5 review - Done (2026-11-15)

**Opus 5.5 / high. Covers D5 (`a93c5420`; doc-only follow-ups `ddaf8568`, `29896225`, `3584941e`, `d8a8f35a`). Verdict: pass, no code change.**
No transcription error found.

**Re-read against `unpacked_x2.exe`** (`disasm_range.py`):
- NPC `LevelCorrectionExtraDamage` `32760210`: the tree @327602ed-3276044e decodes to 14 ids: 32500-32502 (`cmp 0x7ef4` / `0x7ef6`), 38300-38302
  (`0x959c` / `0x959e`), 38400 (`0x9600`), 39500 / 39510 (`sub 0x9a4c`, `sub 0xa`), 39520 (`0x9a60`), 39530 / 39540 (`sub 0x9a6a`, `sub 0xa`), 39550
  (`0x9a7e`), 41280 (`0xa140`). No 38100, no 38401. The port's 14 case labels resolve to exactly these values (`CASE_DEFENCE_DUNGEON` = the three
  elder defence ids). The kickoff's "15" was a miscount. Guard kept: `g_pData` `33775860`, room `+0x70`, party manager `+0x5c`, party data (`+8`).
- NPC `GetEnchantResist` `32757c50`: 150.0 is the double at `334748c8` (read: 150.0), one per `+0x8bd8` vector entry. The jump table `32757cf4` maps types
  1-6 in order to NPC stat `+0xb0`..`+0xc4` (Red / Blue / Green / Wind / Light / Dark ints = March's switch). `ecx` is never rewritten before the base call
  @32757cdc (= this), and the base is added after the NPC terms = the port.
- NPC `UpNowHp` `32784200`: `+0x75bc` == 1, then game slot `0x10c` == 2 (`GT_DUNGEON`), then `fldz; fcomp [delta]; test ah,1; je return`: delta <= 0
  returns, NaN sets C0 and passes. Then the base (gage `+0xc04` -> `+0x20`). The port's order and NaN behaviour match. It adds a `g_pX2Game` null
  test June lacks (June would crash there; harmless).
- **Debug tail: routing to Stage 40 is right.** The map write (`322e28e0`) changes no gameplay value. The address-0 write runs only after the chat command
  (`31ec2921`) sets `+0xba4`, and the command is not ported. Porting the trap alone would be dead code. That is not a June bug the play can reach.
  The backlog bullet (D5 commit) keeps the command, the map and the trap together.
- `GetCalcPerDamage` `32622300`: byte table `326224a8` maps EDT 1 / 3 / 8 / 18 to cases 0-3 (floats 80 / 145 / 60 / 82 at `33457df4` / `3346bf18` /
  `33458fa0` / `334af19c`) and EDT 151 / 156 to case 4 (return `+0x18`). Every other type falls to default (0). The level rule (own level, else
  unit + add level, minus 1, < 0 returns `+0x18`) = March. Constants are 0.2 / 0.1 (doubles of the float literals) and 500.0. The test is
  `322cc1a0` (PvP or PvE). Channel classes 9 / 11 / 10 = `PCC_FREE` / `PCC_PLAY` / `PCC_TOURNAMENT` (ClientPacket.h), March's three.
- **Tick application (map row 60, was `unknown`, now `June`):** NPC `CommonFrameMoveHost_ExtraDamage` `3276b400` (caller `3277eb13`) = March's body.
  The immune byte `+0x4104` returns. Enchant poison / enchant blaze tick once a second. Poison / fire / lightning / cold / frozen each call
  `UpNowHp( -own DPS x elapsed )` then `SetNowHp( 1 )` if HP fell below 1, and curse calls `FlushMp`. Frozen reads its own DPS (`+0x173c`).
  The user twin is `31fd1020` (one caller, `CommonFrameMoveFuture` @32062fa8), = March's `CommonFrameMoveFuture_ExtraDamage`. It has six blocks, each
  gated on HP > 0, with the gage crash UI after fire / lightning / cold / frozen. **Its frozen block subtracts `m_Fire`'s DPS (`+0xf78`) in June too.**
  So March's quirk is June's behaviour: kept, no code. It is March's own, so it gets no `JUNE_BUGS` row (that file's rules).
  `REFACTORING_BY_TOOL_TEAM` is off (`KTDX.h:185` comments out its parent), so March's NPC poison block is the same `UpNowHp` + `SetNowHp( 1 )` form.
- `X2GUUser.cpp` +80: debug lines only (`EDT def=user`, `EDTTICK`), all inside `_STATCALC_DEBUG && SERV_IRUHADEV_OFFLINE`.

**Play-test log** (on disk: a 22:53-22:56 retest after the deploy, md5 49fa44dc; run 1's lines were overwritten, and D5's note recorded them): 3 `EDT`
lines (NPC 2130 / 2132, poison, lv 99, resist 350). Each `perSec` = 80 x (1 + 0.2 x 98) x (1 - 350 / 500) = 494.40 (script check, 0 mismatches). 11 `EDTTICK`
lines on NPC 1000033: the first second includes the hit (130310.4); the other 10 read 493.9 x 8, 487.0 and 500.7 (one frame either side of 494.40 / s).
No fire or cold lines in this run. D5's run 1 has fire 2688.30 / s against median 2688.8.

**Gates.** `finish.py --review --commits a93c5420 --with SERV_IRUHADEV_JUNE_STATCALC_EXTRA_DAMAGE -v`: flagcheck 4 PASS, 1 DEFINE, 1 OTHER (`DungeonEnum.h`,
`JUNE_ENUM_NAMES`), 0 FAIL, 0 deleted, 0 non-ASCII, 0 EOL -> `RESULT: PASS`; junediverge `RESULT: PASS`; `--check-all` `RESULT: PASS`; enumscan exit 0;
stateends / skillicons / luabind skipped (no skill state, skill or script). `FINISH RESULT: PASS (7 gates, 0 failed, 3 skipped)`. No later commit touches the flag (`git log -S`).
**Build.** Full build skipped: no play-test (no code change). `_STATCALC_DEBUG` stays on (the D6 review retires it).

**D6.** Ready. The kickoff gains one line: the tick sites are proved = June, so a player-side tick check is optional.

## Phase D6 - Damage smoke on every ported character, track review

**Scope.** No new code. One play-test sheet per ported character (a geared level-99 unit with sockets and one without): normal, command,
special active, hyper active and level-damage hits against a normal monster, a secret monster and a boss, and hits taken. The review
reads every `HIT` / `GFD` / `REACT` line against the map, runs `flagcheck --range` over D1-D5 and `junediverge --check-all`, and
retires `SERV_IRUHADEV_JUNE_STATCALC_DEBUG` (build not awaited).

**Run with.** Sonnet 5.5 / high for the sheets; the review Opus 5.5 / high.

**Read list.** "Rules", "Play-test hand-off", the Done notes of D1-D5.

**Done gate.** Every character's sheet passes or its failure is routed (a class block to its port, a formula row back to a D phase);
no damage row in the map is `March` or `mixed`.

## Phase S0 - Stat-sheet and resource map (read-only)

**Scope.** The stat and resource rows of the map: base stats by level, `SetKStat` `323571c0` (two 7-value sets), `SetGameStat`
`322814a0`, `ResetMaxHP` / `ResetMaxMP` `31fd2810`, June `Stat` [9] / [10], `GetEqipStat` `3234e210`, `CX2Item::GetStat` `328842f0`,
`GetSetIDNPartsNum` `32350cb0`, `GetEnchantStat` `32358df0`'s group argument, the option-data template `320839e0`, `SumSocketOption`
`3204f190`, `ResetIncreaseSkillLevelBySocket` `3235a080`, the `IsSocketOptionFormLoaded()` paths, titles, pets, passives, the buff stat
members (`32270dd0`, `32271540`, `322718e0`, `32290950`), the three `SetFactor` finalizers, `32358970`; MP on hit and on hit taken,
`ResetMPChangeRate` `31fd9e00`, `AbsorbEffectAttackProcess` `32274660`, the awakening gauge, regen, `GetActualMPConsume`, the six
`GetSkillMpDecreaseRate` sites. Size S1-S4 and R1, amend their kickoffs.

**Run with.** Opus 5.5 / xhigh. Review Opus 5.5 / high. Any time (read-only).

**Read list.** "Rules", "Starting context", "Design: the map and the verification"; the backlog's Stage 33 section.

**Done gate.** Every item above has a row with a state and readers; the S and R kickoffs are amended. Full build skipped.

## Phases S1-S4, R1 - Stat sheet and resource gain (after Stages 7 and 8)

Each phase: the map's rows for its area, June's whole bodies under its flag, a `SHEET` or per-event debug line, one emulation script per
pure function, an Opus review, and a play-test on every character (level 99, with and without sockets).

| Phase | Area | Flag | Main files |
|---|---|---|---|
| S1 | unit stat core: `SetKStat`, `SetGameStat`, `ResetMaxHP` / `ResetMaxMP`, `Stat` [9] / [10] | `_UNIT_STAT` | `X2Stat.*`, `X2Unit.*`, `X2GameUnit.*`, `X2GUUser.*` |
| S2 | equipment, set items, enchant stat | `_EQUIP_STAT` | `X2Unit.*`, `X2Item.*`, `X2ItemStatCalculator.*` |
| S3 | sockets and options: `320839e0` (June's rewrite, approved 2026-11-12), `SumSocketOption`, socket skill levels | `_SOCKET` | `X2SocketItem.*`, `X2GameUnit.*`, `X2GUUser.*`, `X2Unit.*` |
| S4 | buff finalizers `SetFactor` x3, buff stat members re-read | `_BUFF_FACTOR` | `X2BuffFinalizer*`, `X2GameUnit.*` |
| R1 | MP / awakening gain, regen, MP cost, MP change rate, absorb | `_RESOURCE` | `X2GameUnit.*`, `X2GUUser.*`, `X2GageData.*`, class MP blocks |

**Run with.** Opus 5.5 / high each, Opus review each. **Depends.** S0 review, Stage 7 and Stage 8 closed, D6 review. One at a time
(shared files). S0 may split or merge them.

## Phase Z - Close

**Scope.** `MODS.md` complete, the map has no `March` / `mixed` / `unknown` row, `tools/data/june_diverge_allow.tsv` holds no `stage33`
reason, the backlog's Stage 33 marked closed, memory updated, the STATUS block archived.

**Run with.** Sonnet 5.5 / medium. **Done gate.** `junediverge --check-all` PASS; full build skipped unless Z changed code.

## Play-test hand-off

Each code phase hands over, copy-ready (dev-console lines in full):
1. **Unit.** Nickname, class and level of the test unit, made or set with `/x2-create-char` / `/x2-db`; skills slotted by the phase in
   slots 0-3 and 5-8.
2. **Where.** The dungeon id and difficulty, the console line to enter it, the stage that spawns the monster, the monster's NPC id and
   name. A secret-dungeon check names one monster with `k = 1.0` and one with `k >= 1.2` from `NewNPCStat.lua`.
3. **Do.** Which attack, how many hits, and for incoming damage which monster to stand in front of (`/invin 0`).
4. **Look for.** The damage numbers on screen (a range), and that the monster's HP bar moves.
5. **If not.** Which log line is missing or which field is off, and what that means.
6. **Log check.** `python tools/logs.py --tag JUNE_STATCALC`: the lines the phase's gate names, by monster id.

## Decisions (user, 2026-11-12)

1. **Where the 0 damage showed: the Elder secret dungeon** (the user does not remember the others). `DI_ELDER_SECRET_COMMON` 33030 and
   `DI_ELDER_SECRET_HELL` 33031 are in `IsSecretDungeon`'s list, so they get game type 2. `NewNPCStat.lua` has six
   `NUI_ELDER_SECRET_*` rows: `CANNON_BIG`, `PUNCH`, `SMALL_KNIFE` and `BUNKER` at `k = 1.0` (March: about a fifth of the damage),
   `PUNCH_BOSS` and `SMALL_KNIFE_BOSS` at `k = 1.2` (March: 0). That matches "0 to little". D1's play-test is this dungeon.
2. **`320839e0` (the option-data rewrite) is ported.** The buff Phase 5c decision to leave it out is withdrawn: S3 ports June's
   rewrite whole.

Not a question: normal dungeons shift by a few percent once D1 lands (table in "Measurement": June is slightly harder at `k = 1.0`,
easier at `k = 2.0`), and so does damage taken. That is June's balance; D1 ports it as is.
