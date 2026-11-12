# June character Add (Stage 7) - Phase CH1d0 - Engine helpers March lacks (CH0 item 6)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-add CH1d0` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | CH1c (the CH1 run, after char-elesis Z); before CH1d, the first Sonnet phase that calls one of these. Touches engine files, not `X2GUAdd.*` |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_CORE` (new helpers compiled, referenced by nothing yet; a new engine member gets its own `SERV_IRUHADEV_` flag if it changes a class layout) |
| Log tag | `[JUNE_CHAR]` (`ADD CORE`) where the phase logs; otherwise none |
| Review | `kickoffs/char-add/CH1d0-review.md` (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Rules" "Phase CH0 - Add map (read-only)" "Phase CH1 - Deploy unit: June script, motions and the class core"
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "CH0 - Done (2026-11-12)"   # Elesis's item 6, for the shape
cat june2017_re/char_add/ch0_unmapped_identified.tsv
```

## Task

CH0 item 6 found 12 June engine callees (outside Add's range, no funcmap row) that Sonnet class phases call and March lacks
(`june2017_re/char_add/ch0_unmapped_identified.tsv`, the rows that are not `same`). Port each whole under its plain name in its own class, in
the class's pattern, so the Sonnet phases call a March-side name; add a `funcmap.tsv` row per function (status `confirmed` once ported):
- **`32294530`** (free function, ~14 KB decompile; first caller CH2a): snaps a position to the nearest valid point on the line map (`CanDown`
  first, then a nearest-segment scan over all lines). The phase's main work.
- `32293d70` (`CX2GameUnit`, CH1d): which side of the unit's last-touched line a position is on (line vector `+0x58`, bool out).
- `31f63f20` (`CX2DamageEffect::CEffect`, CH2c, **unknown**): stores three floats at `+0xa68..+0xa70` right after an effect-size scale; name it
  by its reader (find who reads `+0xa68` in June) or report it unreadable.
- `31f63d30` (`CEffect`, CH1j / CH3c): stores the per-frame accumulator step at `+0xb20` (read by funcmap row `31f75da0`): March lacks the
  member; check whether an earlier engine port added it (grep `X2DamageEffect.h`) before adding one.
- `31f63d40` (`CEffect`, CH2a): setter for the ONLY_FRONT fields (`m_bIsOnlyFront`, `m_bSaveIsRight`, saved position; members exist,
  `X2DamageEffect.h:629-632`).
- `31fcefa0` (`CX2GUUser` vslot 0x84c, CH1k2): custom-render-state expiry check (switch on `+0x8e64`, reset via `31fc9570`, which Ara / Raven
  ported); the base virtual Add's `329d49b0` overrides.
- `322d58c0` (`CX2Game`, CH1j): `GetNearestUnitInSpecificRange` with an explicit origin (June `322d55b0` = March `X2Game.cpp:7019` with the origin
  parameterised).
- Small accessors: `31c82d50` (`CXMeshInstance` set `m_State` = `IS_DYING_BY_CRASH`; `m_State` is private: a one-line member, KTDXLIB),
  `31c834e0` (`CXMeshInstance` anim-end test), `31fc9430` (`CX2GUUser` position store, future / now), `31edf5a0` (`IsUpgradedSkill` with a null
  guard; `X2SkillTree.cpp:6077`), `3226e310` (cool-time tick: `t -= elapsed; t <= 0 -> 0, true`).
For each, say which Add phase calls it (the tsv's first-phase column) and write the name into that kickoff's "Amended by CH0" list if it differs
from the proposal. `32d27770` (riding-pet wait-state getter, `differs`) is CH1q's, not this phase's. One-file probe builds of every touched
`.cpp` (0 errors); no full build (no play-test).

You are on Opus: delegate the reads, greps, known-address Ghidra dumps, probe compiles and flagcheck runs (x2-phase, *When the phase is Opus*). CP949 sources: `bytepatch.py` only.

No hardcoded enum literals (user, 2026-10-26; CLAUDE.md): an id that has a name in an Enum (June's
`Enum.lua`, or a C++ enum) is never written as a number, in any new or touched code: use the member, and
register it under a `SERV_IRUHADEV_` flag with June's value and name if C++ lacks it. A value with no Enum
entry stays a number with a one-line comment. The one exception is a table whose rows are the data itself
(a `.tsv` of ids), not code.

No `June` prefix (user, 2026-10-28; CLAUDE.md): June's body goes inside March's own function under the flag,
March's in `#else`. A function March lacks gets a name in the class's own pattern, never with `June` in it.
Every name (locals and constants too) says what it is in context: no Ghidra address, phase tag or `June` in an identifier.
Call a March function from June code only if June's namesake does the same; otherwise replace March's definition.
Run `python tools/junediverge.py --list` before transcribing; a newly found divergent function gets its `june_march_divergent.tsv` row and the replacement in the same commit.
Phase tag (user, 2026-11-09): every edited or new code block carries a one-line `// char-add <phase>` comment (at the start of an existing one-line comment, never a second line).

## Amended by CH0 (2026-11-13)

- `june2017_re/char_add/ch0_unmapped_identified.tsv` (58 rows: 45 same, 11 june-only, 1 differs, 1 unknown): the 12 non-`same` rows whose first caller is a Sonnet phase are this phase's; `32294530` is flagged needs-Opus (~14 KB). `32d27770` (differs) is CH1q's. No other `CH<n>0` phase: every other unmapped callee is `same` (std / compiler helpers or March functions).
