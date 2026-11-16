# June character Lu/Ciel (Stage 8) - Phase CH3d review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH3d review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH3d done (its Done note and commit) and the CH3d play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` (+ `_SKILL_DEBUG`, retired here), `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_TRANSCEND`, `SERV_IRUHADEV_JUNE_CHAR_DUO_HYPER_CUT_IN` |
| Log tag | `[JUNE_CHAR] LUCIEL SKILL` / `LUCIEL UI`, `[JUNE_TRANSCEND] IMAGE` |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "CH3d - Done (2026-11-16)"
```
Then `kickoffs/char-luciel/CH3d.md` (the amendments) and the `funcmap.tsv` rows tagged `(CH3d)`.

## Task

Review CH3d's commit against June in Ghidra ( read-only ) and the exe listing ( `charport_adis.py luciel --phase CH3d` ). CH3d is its own stage review ( CH3a-CH3c2 were reviewed ).
- The seven class functions: the three preloads ( names and order ), the two event processes ( `32b02420` / `32b02630`: the x87 tests read as `>= 0` and `<`, the walk branch going to `LSI_LU_WAIT`, the skill branch then the awakening branch ), Diabolic Disaster's frame move `32b07f30` ( June reads Lu's `+0x87d4` anim, not the active body's: judge from the play-test whether Ciel's cast still hides and shows ) and state end `32ae9b20`.
- The three setter cases at `32b4a59b` / `32b4a611` / `32b4a687`: the delegate makers by hand ( Init `+0x14`, FrameMove `+0x34`, EventProcess `+0x44`, StateEnd `+0x54` ), what each hyper state copies ( Descent / Castle: EventProcess only; Diabolic: FrameMove, EventProcess, StateEnd ) and the shared `GenericActiveSkillEventProcess` ( `329cd4a0 > 329cba60 > 31fcc9d0` ).
- `ShowHyperSkillCutInByScript` ( `32014fc0` @320150c1 ): the minor-body test ( vslot 0x260 ), the class 0x81 test, the two state ids ( 0x1c2 / 0x1f6 ) and the templet offsets ( +0x314 / +0x330 / +0x34c ) against `X2Unit.h`'s member order.
- `GetCharacterImageName`: every Lu/Ciel row against June's pushes ( `3336f3e0`, `33378xxx`, `33379860`, `3337eab0`, `33387xxx`, `333976xx`, `333988f0`, `3339ead0` ) and the piece names in the staged `.TET` files.
- `TranscendCheck`, the `LevelUpEvent.lua` TYPE8 row ( June `:622-641` ) and the `Always.h` guards ( `cl /EP` that `_RAVEN_SKILL` is defined at the guard ).

Hygiene is one command: `python tools/finish.py --review --commits <the CH3d commit> --with JUNE_CHAR_LUCIEL_SKILL JUNE_CHAR_LUCIEL_SKILL_DEBUG JUNE_CHAR_LUCIEL_TRANSCEND JUNE_CHAR_DUO_HYPER_CUT_IN -v`;
explain every OTHER row and deleted line. The 4 enumscan rows in `Handlers_Quest.cpp` / `X2Data.cpp` predate CH3d.

Read the play-test's lines ( `diabolic disaster hide / show body`, `LUCIEL UI hyper cut-in class / body / state / name`, `[JUNE_TRANSCEND] IMAGE class 127 / 128 / 129`, the `equip skill 3060x slot 4 / 9` lines ). Retire
`SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL_DEBUG` ( comment out the define: no build ). Fix clear transcription errors under the same flags; hand design decisions back to
the user. Write "CH3d - Review" into the plan doc, update the STATUS rows ( CH3d done; CH6 unblocked when CH4c, CH5 and TU are ), and commit.
