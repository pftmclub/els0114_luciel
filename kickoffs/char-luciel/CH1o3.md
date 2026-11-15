# June character Lu/Ciel (Stage 8) - Phase CH1o3 - Effect-set tags, checklist hooks, the skill-state byte

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1o3` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1o2 |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (compiled, unreferenced), `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` (`//#define` until CH1 final), a class-agnostic flag for any engine piece |
| Log tag | none (no play-test) |
| Review | kickoffs/char-luciel/CH1o3-review.md (Opus 5.5 high; the phase writes it when done). It covers CH1o2 and CH1o3 |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH1n - Done (2026-11-15)" "CH1o - Done (2026-11-15)" "CH1l3 - Review (2026-11-15)"
```

and from `kickoffs/char-luciel/CH1o.md` the sections "Amended by CH0 (2026-11-02)" (checklist rows), "Amended by char-luciel CH1l3 review"
(effect-set tags) and "Amended by char-luciel CH1n" (the `+0x11` byte). `june2017_re/char_luciel/ch0_checklist_rows.tsv` rows with phase `CH1o`.

## Task

1. **Effect-set modifier tags 4, 10, 11** ( `X2EffectSet.cpp`, `ApplyDamageEffectModifiers` ): transcribe June `321db1b0`'s three cases ( jump table
   `321dbc60` ) under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE`, as Eve did tags 5 / 12 / 16 / 20; confirm in the listing whether the tag 10 / 11 NEXT-table
   loops write the main effect's own value again ( JUNE_BUGS row if so ). Tag 9 ( force down ) is in only under `SERV_IRUHADEV_JUNE_CHAR_ELSWORD_SKILL`:
   widen it for Lu's Punisher or say why not.
2. **June's state-data byte `+0x11`** ( `3208e430`, user vslot 0x528; CH1n routed it ): every class's setter sets it on its slot states, and Lu's also on
   extra states ( 265 here, 344 in CH2c ). Choose the representation ( a `UserUnitStateData` member set where June stores it and read by the engine's
   `IsSkillSlotState` readers, or a class set Lu's readers consult ), apply it to CH1n's setter, and write Lu's `ReservedSkillStart` override ( standing
   rule: every class port overrides it; June `32057b80` @`32057c4c`-`cb2`: `SpecialAttackCooldown( id, m_fReservedSkillMp )` then
   `SkillMemoMpRefund( id, <+0x11 of the now state>, m_fReservedSkillMp )`, as Chung's `X2GUChung.cpp` `ReservedSkillStart` ). CH1o added Lu's link to the
   base chain ( `X2GUUser.h` / `.cpp`, `_LIVE` ).
3. **Checklist hooks** ( `ch0_checklist_rows.tsv`, phase `CH1o` ): §5 L184-188 `DUNGEON_SPEECH_LUCIEL` / `_LU` / `_CIEL` ( the type-10 branch in the engine
   InitSystem reader `3202b9e0`; March reads `DUNGEON_SPEECH` in `X2GUUser.cpp` ); §7 L260 / L263 ( `CanUseSpecialAttack` `32b3a310`: the quick-slot special
   item wait at the top and the riding-key wait after `CheckSkillUsePVPOfficial`: confirm CH1l2 wrote both ); §7ap L977 ( `SkillIconOverride` 0x56c and
   `ShownSkillCoolTime` 0x6a8 overrides, the skill-use call @`32b3a6ba` ): confirm or wire.

The engine state-id seam is **not** this phase's: the user chose ( 2026-11-15 ) to shift March's `USER_STATE_ID` to June's numbers ( plan `june-state-ids`,
brief `kickoffs/june-backlog/stage41-plan.md` ); CH1 final waits for it. **Closed by june-state-ids S2 (2026-11-15, `857b18cd`):** Lu runs on the shared numbers;
nothing of the seam is left here.

Gates: `finish.py --files <changed> --state X2Lib/X2GULuciel.cpp Luciel_Duo --state X2Lib/X2GULuciel.cpp Luciel_Demonio`; `/Zs` probes plain and with
`/DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`; full build skipped: no play-test. Write the Done note, STATUS, `CH1o3-review.md`, and check `CH1p.md`.
