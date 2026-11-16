# June character Lu/Ciel (Stage 8) - Phase CH3a0 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH3a0 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH3a0 done (its Done note and commit) and its play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SUMMON`, `SERV_IRUHADEV_JUNE_SUMMON_HELPER_PASSIVE_RATE`, `SERV_IRUHADEV_JUNE_SUMMON_DAMAGE_ADJUST_BY_OWNER` (+ `_LUCIEL_SUMMON_DEBUG`) |
| Log tag | `[JUNE_CHAR] LUCIEL SUMMON` (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL SUMMON"`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "CH3a0 - Done (2026-11-16)"
```
Then `kickoffs/char-luciel/CH3a0.md` (the amendments) and the 4 `funcmap.tsv` rows tagged `(CH3a0)`.

## Task

Review CH3a0's commit against June in Ghidra (read-only) and the exe listing:
- `3230c1a0`: the five Lu cases (0x772 / 0x791 / 0x774 / 0x8dd / 0x8de) and their flags (0x40, 1, 0x50); the owner-type-10 store of the NPC UID into `+0x9690`;
  that every other path (the shared tail at `3230ca6d`) is already March's; the skills fed from `31fcf520` (30013 / 30016 / 30053).
- `322ece70` flag 0x10 (`@322ed0a4`): the order of the stat-scale and attack-rate products against `SetSummonHelperInfo`'s new block, `+0x4074` / `+0x4068` /
  `+0x4064` read as `m_fSummonedStatRateByPassive` / `m_fSummonedAtkRateByPassive` / `m_fStatAtkRateOfSummoned`, the ctor value of `+0x4074` (`@322a4351`).
- `32778fe0` and the three `X2DamageManager.cpp` sites: every attacker unit, `NAT_ALLY`, the `dynamic_cast`, `GUT_USER`, `GetUnitByUIDAndType`; that no
  `AdjustDamageDataBeforeDamageReact` override changed signature and the flag-off path is March's `GUT_USER` test.
- Data: the five `NPCTemplet.lua` rows byte-equal to `june_src_fixed/data037_kom/NPCTemplet.src.lua`, the three `Enum.lua` names and values, the live koms
  equal to the packed ones.

Hygiene is one command: `python tools/finish.py --review --commits <CH3a0's commit> --with JUNE_SUMMON_DAMAGE_ADJUST_BY_OWNER JUNE_SUMMON_HELPER_PASSIVE_RATE
JUNE_CHAR_LUCIEL_SUMMON JUNE_CHAR_LUCIEL_SUMMON_DEBUG --lua SI_SA_LLD_SUMMON_DARKNESS SI_SA_LLD_SUMMON_DARKNESS_PVP SI_SA_LCN_SHADOW_BOLT SA_LDN_GARDEN_OF_HAURES
SA_LDN_GARDEN_OF_HAURES_ADD -v`; explain every OTHER row and deleted line.

Read the play-test's `[JUNE_CHAR] LUCIEL SUMMON` lines (set lines per NPC, hit lines on Lu and on Eve). Retire `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SUMMON_DEBUG`
(comment out the define: no build). Fix clear transcription errors under the same flag; hand design decisions back to the user. Write "CH3a0 - Review" into
the plan doc, update the STATUS rows (CH3a0 done; CH3a ready), check `kickoffs/char-luciel/CH3a.md` (its spawn is `CreateNPCReq( NUI_SI_SA_LLD_SUMMON_DARKNESS
... NAT_ALLY, GetUnitUID() )` from the state FrameMove, as Elesis CH3b0), and commit.
