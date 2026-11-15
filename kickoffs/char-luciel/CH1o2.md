# June character Lu/Ciel (Stage 8) - Phase CH1o2 - Engine readers of the state getters

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1o2` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1o review |
| Flag | `SERV_IRUHADEV_JUNE_USER_VIRTUAL_HOOKS` (defined, class-agnostic; CH1o's) |
| Log tag | none (no play-test; the regression play-test is CH1 final's) |
| Review | no review of its own: the CH1o3 review covers CH1o2 and CH1o3 |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH1o - Done (2026-11-15)" "DU2 - Done (2026-11-14)"
```

and `june2017_re/char_luciel/ch1o_vcall_sites.tsv` (the rows whose verdict starts `CH1o2`: 84 sites of user vslots 0x438-0x464).
`june2017_re/char_luciel/ch1o_vcall_scan.py <june2017_re dir> <vslot hex ...>` re-runs the scan.

## Task

CH1o made the wait / walk / dash / jump / die state getters virtual ( `GetWaitStateID` 0x438 ... `GetDieBackStateID` 0x464 ). Lu/Ciel answers
Ciel's ids from them while Ciel is in play, so every engine reader June routes through a getter must ask it, as DU2 did for the damage band.
For each site in the table: name the June function (funcmap), find March's counterpart, and give a verdict:
- **automatic:** March already calls the getter (e.g. `IsWaitState`, `GetSpecialItemUsableExtraStateID`, the ones CH1o made virtual).
- **routed:** March compares or changes to a `USI_*` constant / `m_CommonState.m_Wait` where June asks the getter: replace it under the flag
  (`#ifdef` getter `#else` March's constant). Expect `StateChange`, `PlayEmotion`, `CommonFrameMove`, `PhysicProcess_Jumping`'s line helper
  (`31fd7350`), `CanApplyBuffToGameUnit`, `ProcessExpListByNpcUnitDie`, the dungeon / battle-field `ShortCutKeyProcess`, `IsDashState`.
- **no March body:** June-only code (say which stage owns it) or a class's own code (Elsword `32b59aa0`: the class port's).
- NPC rows (`CX2GUNPC::Init`, `CommonFrameMove`, `3277cf10`) call the getters on an NPC object's user owner or are NPC vtables at the same
  offset: check the object type before routing.
Record the verdicts in the table (new column), one line per site. No hardcoded ids (CLAUDE.md enum rule).

Gates: `finish.py --files <changed>`; `/Zs` probes (plain and `/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`); full build skipped: no play-test.
Write the CH1o2 Done note, STATUS, and check `CH1o3.md`.
