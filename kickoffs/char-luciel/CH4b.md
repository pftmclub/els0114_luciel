# June character Lu/Ciel (Stage 8) - Phase CH4b - Damage exception and overrides

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH4b` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH4a |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_PASSIVE` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL PASSIVE`) (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |
| Review | kickoffs/char-luciel/CH4b-review.md (Opus 5.5 high; the phase writes it when done). **It covers CH4a, CH4b** (user, 2026-11-10): every covered Done note, `flagcheck --range` over all of them, `--check-all`, retires any debug define |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Starting context" "Phase CH4 - Passives and class mechanics"
```
Then the planning digests in `june2017_re/char_luciel/` the task names, and the Done notes of the phases this one depends on
(their routed gaps). Verify a digest line before building on it.

## Task

Do the `CH4b` row of "Phase CH4" from the class map. The Done note names, per passive, the stat or log line that shows it.

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

## Amended by CH0 (2026-11-02)

- **Checklist rows** (`ch0_checklist_rows.tsv` col 5): §7 L256 death prevention: gauge snapshot override vslot 0x740 Luciel 32aeb410 (and the r; §7b L327 SetDamageExceptionProcess per-class block; §7d L379 DamageReact 320483e4 (user) / 327b33cf (NPC): type-10 attacker with USED_SKILL_I; §7j L454 inline user-DamageReact step 320462a0 @3204963d + 32aebaf0: a type-10 unit absor.
- No class function carries CH4b: SetDamageExceptionProcess 0x2b8 `32aea530` is transcribed in CH1k. This phase wires the damage-exception checklist rows below.

## Amended by char-luciel CH4a (2026-11-16)

- **DamageData `+0x5c0`, the hit-from-behind byte ( funcmap row `DD +0x5c0 (StatCalc S33 D4a)` ):** June's `SimpleInit` zeroes it and both DamageReacts set it
  when the defender's `bAttackerFront` is clear ( user `@32048ceb`, NPC `@327b39dc` ). Its only reader is Lu/Ciel's `CreateFearDamageEffect` `32b3ca30` ( third
  argument, from `ApplyFearOnHit` `32b3d380` ): with it set and any crowd-control debuff held, Lord of Fear's damage effect plays. CH4a passes a stand-in `false`
  ( `bHitFromBehind` in `X2GULuciel.cpp`, comment `stand-in: DamageData +0x5c0 ... char-luciel CH4b` ). Add the member to `CX2DamageManager::DamageData`, the two
  writes and the reset, then replace the stand-in with `damageData_.<member>`. Play-test: Dread Lord with Lord of Fear, hit a stunned / slept monster from behind
  with a feared stack on it -> `[JUNE_CHAR] LUCIEL PASSIVE lord of fear effect ( N stack(s), power xP )`.
- **`CX2GUUser` slot-251 post-apply hook `3204dcd0` ( checklist §7 L257 ):** March runs no hook after a buff applies ( `X2GameUnit.cpp:15006` notes it ). June's
  only body is `CX2GUUser`'s: a debuff templet with Tribute ( 30409 ) learned and BTI 462 held may erase one debuff and stack Soul Collector ( BFI 523 via
  `322a8a70` ), and Noble Devotion ( 30419 ) reads its values `0x9a` / `0x4f` / `0xac`. Port it with the virtual slot ( Ain's override `32a3d220` waits for her port ).
  It is the only reader of `RNSI_P_LCN_NOBLE_DEVOTION` besides Shadow Bolt's state ( `32b2fe8c`, CH3b ). CH4a could not take it: a peer phase held `X2GUUser.*`.
