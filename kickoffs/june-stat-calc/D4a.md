# June damage and stat calculation - Phase D4a - NPC DamageReact: from the final damage to the HP change

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-stat-calc D4a` |
| Plan doc | `JUNE_STAT_CALC_PHASES.md` (branch `main`) |
| Depends on | D4 cut (2026-11-12, `june2017_re/statcalc/dr_blocks.tsv`); not beside another phase that edits `X2GUNPC.cpp` |
| Flag | `SERV_IRUHADEV_JUNE_STATCALC_DAMAGE_REACT` (+ `_STATCALC_DEBUG`) |
| Log tag | `[JUNE_STATCALC]` (`python tools/logs.py --tag JUNE_STATCALC`) |
| Review | `kickoffs/june-stat-calc/D4-review.md` (covers D4a and D4b; D4a has no review of its own) |

Amended 2026-11-12 by the D4 cut (was `D4.md`): D4 found 20 `absent` / `mixed` blocks across both functions and split by side, as D2 was.

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_STAT_CALC_PHASES.md "Rules" "Starting context" "Phase D4 - DamageReact: from the final damage to the HP change" "Phase D4 - Cut (2026-11-12)" "Phase D0 - Done" "Phase D2b - Done" "Play-test hand-off"
```

Then the `npc` rows of `june2017_re/statcalc/dr_blocks.tsv` and the map rows whose `phase` names `D4a`. June's decompile:
`june2017_re/buffre/dec/327b1f30.c` (lines ~799-1550) with the listing `327b1f30.s` for addresses.

## Task

Port the damage arithmetic of NPC `DamageReact` `327b1f30` from the `GetFinalDamage` call (`@327b33f6`) to the defender's `UpNowHp`
(`@327b4c11`; D0's `@327b3da5` is the attacker's HP-drain call), one `dr_blocks.tsv` row at a time, in June's order:

- Every `absent` and `mixed` row (`327b33fc`, `327b3491`, `327b35db`, `327b35fe`, `327b39cf`, `327b41c9`, `327b480f`, `327b4b12`,
  `327b4b82`): identify each member, vslot and NPC id first (June's `Enum.lua` / NPC templet names, no number where a name exists).
  Slot `0x378` is called with the same arguments by user `DamageReact` (`@320489f3`): D4a identifies it for both sides.
- Every `unknown` row: re-read against the exe; it becomes `June` or is ported.
- March's back-attack `x1.1` (and the critical rate's `x1.5`) have no place in June's order: resolve `ADD_BACKATTACK` with `cl /P` first.
- `322cfca0`, the given-damage tallies (`@327b4d13 / @327b4ef6`, D3) and `m_fDamageRateOfPart` (D3): check only their place in June's order.
- The `DI_EVENT_ALCHEMY_LAB` row is backlog Stage 19's (41350 not registered): leave it.
- Damage-number position and reaction / hit-stop / sound code in the same function are not this plan's (`n/a` rows).
- `REACT` debug lines (`_DEBUG`): final damage, each reduction by name, the HP change. Same throttle as `GFD2`.

Gate: map rows and `dr_blocks.tsv` `npc` rows `June`; flagcheck and junediverge PASS; probe build (`/Zs`, 0 errors); full build skipped:
D4b builds and hands off the play-test for both.

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
