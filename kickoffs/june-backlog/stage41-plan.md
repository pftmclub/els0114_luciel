# June port backlog - Stage 41 planning - March's user state ids take June's numbers

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase new-plan june-state-ids` |
| Output | plan key `june-state-ids` (`kickoffs/june-state-ids/`, a `june-state-ids` STATUS block), plan doc `JUNE_STATE_IDS_PHASES.md` (new) |
| Plan doc | `JUNE_PORT_BACKLOG_ORDER.md` (Stage 41) |
| Depends on | nothing; **char-luciel CH1 final waits for this plan's last code phase** (user, 2026-11-15) |
| Flag | the plan names one (`SERV_IRUHADEV_JUNE_USER_STATE_IDS` suggested) |
| Log tag | the plan names one if a phase logs |
| Review | none (a planning session) |

## Read first (and nothing else unless you need it)

```sh
python tools/docsection.py JUNE_PORT_BACKLOG_ORDER.md "Stage 41: March's user state ids take June's numbers (char-luciel CH1o, 2026-11-15)" "Standing rules (every stage, every kickoff)"
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH1m - Done (2026-11-15)" "CH1n - Done (2026-11-15)" "CH1o - Done (2026-11-15)"
```

and `kickoffs/char-luciel/CH1o.md` ("Amended by char-luciel CH1m", "Amended by char-luciel CH1n").

## Task

The user chose (2026-11-15, char-luciel CH1o) to renumber March's `CX2GUUser::USER_STATE_ID` to June's numbers instead of routing every engine
`StateChange( USI_* )` through getters. March and June agree below 24; from there March is short of June's members (probed: `USI_DAMAGE_AIR_FLY_FRONT`
39 vs June 40, `USI_SPECIAL_ATTACK_1` 49 vs 50, `USI_RIDING_START` 71 vs 72, `USI_RIDING_DIE` 86 vs 87, `USI_EVENT_SNIPER_CONTROL_IDLE` 93 vs 111,
`USI_BOUNDING_DOWN` 102 vs 120, `USI_WAIT_STATE_CHANGE` 105 vs 130). Lu/Ciel registers every state at June's id, so until the numbers agree the
engine sends Lu to the wrong state (riding, slots, events, bounding, wait change), the event kit overwrites `LSI_RIDING_SPECIAL_MOVE` ( 92 ) and
`m_bBoundingStatesReady` stays false.
1. **Census.** June's `USER_STATE_ID` from `june_src_fixed/data036_kom/Enum.src.lua` ( `GUSI_END + n` rows ) and the exe; March's from `cl /P`
   ( `june2017_re/enumprobe.py` ). List every June member March lacks, with its June value and name ( `tools/data/` tsv ).
2. **Readers of the numbers.** Every place a state id is a number, not a name: hard-coded literals, arrays or tables indexed by state id
   ( `_CONST_GUUSER_::aNormalSkillStateID` ... ), every class enum that starts at `USI_END + 1`, `Enum.lua` ( core list: its `USER_STATE_ID` and
   class-state values must equal C++, `luabind.py check` STATE gate ), class scripts, `tools/stateends.py`, packets and the offline emulator, saves.
   Say which shift with the enum and which need an edit.
3. **Shape.** June's names inserted into March's enum under the flag at June's values ( CLAUDE.md enum rule: never renumber or rename a March
   member by hand; the inserted members do the shifting ), `Enum.lua` following in the same phase, one regression play-test that walks every
   live class through riding, a skill slot, an event state and the wait change after a win.
4. **Lu/Ciel.** After the shift, `CX2GULuciel`'s June ids shadow nothing; say which of its `LUCIEL_STATE_ID` names then equal the base members
   ( the slot names CH1n added, `LSI_WAIT_STATE_CHANGE`, bounding ) and whether `InitBoundingStates` / `InitWaitStateChange` can replace its inline rows.

Write the plan in the x2-phase `new-plan` shape (one `##` section per phase, kickoffs, STATUS block, Seam if it batches), add `june-state-ids`'s
last code phase to char-luciel CH1's Depends, mark this brief's STATUS row done, and commit with explicit pathspecs.
