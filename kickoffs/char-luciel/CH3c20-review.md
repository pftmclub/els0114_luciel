# June character Lu/Ciel (Stage 8) - Phase CH3c20 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH3c20 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH3c20 done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LOCKON_SYSTEM`, `SERV_IRUHADEV_JUNE_CHAR_NPC_LUA_BINDS` |
| Log tag | none (inert until CH3c2) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH3c20 - Done (2026-11-15)"
awk -F'\t' '$1 ~ /\(CH3c20\)/' june2017_re/funcmap.tsv
```

## Task

Review CH3c20's commit against June in Ghidra / the listing (`june2017_re/disasm_range.py`; Ghidra has no function at `327799c0`, `322e7980`,
`322f27e0`). What this phase could get wrong:

- **The hoist:** `CX2GUUser::SLockOnSystem::ProcessLockOn` in `X2GUUser.cpp` is byte-equal to Ara's CH2b0 June branch (`X2GUAra.cpp`, the `#else` of
  `_LOCKON_SYSTEM`); the struct in `X2GUUser.h` keeps every member, default and accessor of Ara's (the `+0x30` range unconditional); Ara still
  compiles and calls the base's struct (`m_sLockOnSystemPtr`, `CreateLockOnSystem`); `X2GULuciel.h` +0x9830 member type.
- **`GetPosCheckDistStandardNPC` (`322f27e0`):** argument order (hidden return, vPos, bool, fDist, vDir, NPC), the weapon-taker call's arguments,
  `GetLandPosition( probe, false, 30.0 )`, the two line-type skips (1 / 7), the segment clamp comparisons (`<= 0`, `length <= projection`), the tie
  rule, and the final `IsPosAlongLastTouchLine != bIsRight` reset.
- **`PushCreateNPCReq_LUA4` (`327799c0`):** the 19 pushed arguments of `322e67c0` mapped to March's `CreateNPCReq` (delay / no-drop / key order;
  team / AI / ally team from arg 8; elite 0; create type 0). **`IsSameOwnerUserID` (`3275b0f0`):** both owner UIDs >= 0 and equal.
- JUNE_BUGS 733 is a June quirk, not a port gap.

Hygiene is one command: `python tools/finish.py --review --commits <the CH3c20 commit> --with SERV_IRUHADEV_JUNE_CHAR_LOCKON_SYSTEM
SERV_IRUHADEV_JUNE_CHAR_NPC_LUA_BINDS --lua SA_LDN_GARDEN_OF_HAURES SA_LDN_GARDEN_OF_HAURES_ADD`. Paste its block into the review; run flagcheck
with `-v` and explain every OTHER row and deleted line; a FAIL or ERROR is a finding.

Spot-check 2-3 items directly in Ghidra (read-only); if one fails, re-check the whole phase. Fix clear transcription errors under the same flag
(probe build, no play-test: CH3c2 plays it). Write "CH3c20 - Review" into the plan doc, update the STATUS.md row (CH3c2 then waits only on
CH3c), check CH3c2's kickoff, and commit.
