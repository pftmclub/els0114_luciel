# June character Elesis (Stage 7) - Phase CH4b2 - Advanced Knight Mastery guard (Opus)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH4b2` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH4b. One at a time with CH1-CH4: they all edit `X2GUEL.*` / `X2GUUser.*` |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE` (+ `..._PASSIVE_DEBUG`, on) |
| Log tag | `[JUNE_CHAR]` (`ELESIS PASSIVE`) |
| Review | none of its own: the CH4c stage review covers CH4a-CH4c and this phase |

Split from CH4b on 2026-11-14: the CH4a kickoff asked CH4b to find the caller of June's guard roll `32b524f0`. It is outside her class range
(`CX2GUUser::DamageReact @320476a2`), and the port is a shared-code mechanic with a state branch, so it got its own phase (scope rule: a passive of
this plan's subject).

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md --no-sub "Rules" "Phase CH4 - Passives and class mechanics"
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "CH4b - Done (2026-11-14)"
grep -n "9175" JUNE2017_CHARACTER_PORT_CHECKLIST.md
grep -n "^32b524f0\|320462a0 @320492ba" june2017_re/funcmap.tsv
```

## Task

Port Advanced Knight Mastery's guard (28404, Grand Master 29 / 120), whole, as June has it:
- **The roll** `32b524f0` (in her range, called non-virtually): `0 < m_fAdvancedKnightMasteryEffectiveRate` ( +0x978c ) and `GetRandomFloat( 0 ) < ` it -> true. A
  `CX2GUEL` function named by the class pattern ( e.g. `IsAdvancedKnightMasteryGuard` ), never with an address.
- **The set site** `CX2GUUser::DamageReact` `320462a0` @`320475c1`-`320476b5`: the condition chain in front of the call ( unit templet type 8 = `UT_ELESIS`,
  the DD and state tests at @`320475cf`-`3204769e`: read each one and name it from March's members ), then the user byte `+0x9175` = 1 and the jump to
  @`32047810`. Replace the stand-in comment at `X2GUUser.cpp` ( `// stand-in: Elesis's +0x9175 guard not ported` ) with the multiply @`320492ba`-`320492d5`:
  damage x her `m_fAdvancedKnightMasteryDamagedRate` ( +0x9784 ) while the byte is set.
- **The state branch** in `CX2GUUser::DamageReactStateChange` `31feabd0` @`31feacf4`-`31fead30` ( checklist §7l row "Pre-switch class byte `+0x9175`" ): after
  March's `m_bQuickenGuardBlocking` block, before `+0x9174`: byte cleared, then reActResult SMALL -> `ESI_P_SGM_ADVANCED_KNIGHT_MASTERY_SMALL` ( 333 ), else
  `ESI_P_SGM_ADVANCED_KNIGHT_MASTERY` ( 332 ). Both states are registered already ( CH1e, `ch0_state_skill.tsv` rows 209 / 210 ).
- The byte is a new `CX2GUUser` member beside `m_bQuickenGuardBlocking` ( March's `+0x9173` ), reset where June resets it. The `X2GUUser.*` hunks stay under
  `_PASSIVE`; if `_PASSIVE` is not visible there, widen the `#if` with `JUNE_STATCALC_DAMAGE_REACT` as CH4a did, proven with `cl /EP`.
- Run `stateends.py check` ( the two states are entered here ) and the usual gates. Play-test on unit 29 `ElesisGM` ( 28404 learned since CH4a, rate 0.150 ):
  get hit by small monsters about 20 times; expect a few guard motions and `[JUNE_CHAR] ELESIS PASSIVE advanced knight mastery guard ( ... )` lines, and the
  damage on those hits x 0.85. After every guard she must be able to move.

You are on Opus: delegate the reads, probes and builds. Rules as CH4b's kickoff ( no enum literals, no `June` in names, one-line phase-tagged comments,
`junediverge.py --list` first ).
