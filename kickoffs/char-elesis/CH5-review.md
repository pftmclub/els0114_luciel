# June character Elesis (Stage 7) - Phase CH5 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH5 review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH5 done (its Done note, commit and the user's play-test) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_UI` (defined in `KTDXLIB/Always.h`; `#error` without `_ELESIS_LIVE`) |
| Log tag | `[JUNE_CHAR]` (`ELESIS UI cut-in ...`, `ELESIS UI hyper cut-in ...`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md --no-sub "Rules" "CH5 - Done (2026-11-13)"
python tools/docsection.py JUNE_CHARACTER_ARA_PHASES.md "CH5 - Review (2026-11-11, Opus 5.5): PASS, no code change"
grep -n "Elesis: CH5" JUNE2017_CHARACTER_PORT_CHECKLIST.md
```

## Task

Check CH5 against June from the exe bytes (Ghidra read-only) and the kom diff. Spot-check at least:
- **Table `333a8450`.** 12 `SkillCutIn` at `337ad5c0 + i * 0x30`; strings pushed three times each (xrefs `333a848c` ... `333a8b35`): `334fe8dc` `HQ_CutIn_Elesis_01`, `334fe8b0` `_GM_01`, `334fe884` `_BH_01`, `334fe858` `_CA_01`.
  Sizes / positions are March's one-argument `SkillCutIn( name )` (`334367ac` / `33435598`). The port's `s_SkillCutInSet` is that table: set 0 = 8 / 29 / 30 / 35, 1 = 120, 2 = 121, 3 = 125 (CH1m's switch, `X2GUEL.cpp` `InitState`).
  The two `.dds` names of March's table (`HQ_CutIn_Elesis_Pyro_Knight.dds`, `_Saber_Knight.dds`) are in the `#else` only; `exestr.py --absent` them.
- **UnitTemplet.** Blocks split on `AddUnitTemplet`, keyed on `unitClass`: current vs the pre-phase copy (`kom_backup_tools/data036.kom.20261113_093754`) vs June's `UnitTemplet.src.lua`. The 7 `UC_ELESIS_*` blocks equal June's
  text 7 of 7; the other 52 equal the pre-phase 52 of 52 (57 -> 59 blocks); key delta = `june2017_re/char_elesis/ch0_unittemplet_elesis.tsv`. `luasrc.py ledger --check`, `core --check`, `luabind.py check UnitTemplet`.
- **Creation screen.** `DLG_Unit_Create_ClassChange_Button_Elesis.lua` = June's bytecode (data034, `adopt-june`, 5284 B). The C++ is March's for 8 / 29 / 30 / 120 / 121: `SelectElesis` (June `32554de0`: same controls,
  `DLG_Unit_Background_elesis.tga`, `LobbyDownLanding`, `325514e0( 8 )`, `ElesisVoice_Introduce.ogg`), `ChangeCreateUnitStandMeshInstance` (`32551e30` type 8 = `Dummy_Village_Board8` + `EffectSet_Elsword_Stand_Aura`),
  `GetUnitStatInfomationChart` (`3254ee40`: 8 / 29 / 120 = 2,1,2; 30 / 121 = 2,2,2; 35 / 125 = 2,1,1 are CHNa's). `character_info_08.tga` = June's (data057, TET identical). The third-line buttons (35 / 125) show and are inert until CHNa.
- **Lobby.** `X2UnitViewerUI.cpp:644` (BH hand-made particles) reads handles created only in the dead `#else` of `JUNE_UNIT_VIEWER_EFFECT_PLAYER` (`:2560`+): inert, left as is; the `SPK_LobbyReadyaction` literals (`:2162`, `:5277-5280`) are in the dead `#else` of `JUNE_UNIT_VIEWER_EFFECT_PLAYER` / `JUNE_ANIMATION_NAME_TABLE`
  (check with `cl /P`); the live `AnimationNameTable.lua` has June's `SPK_LobbyReadyAction`.
- **Not here.** Skills are not castable until CH2a / CH3a (user, 2026-11-13), so the cut-in and hyper cut-in lines are owed to those phases and CH6; the 35 / 125 stat chart, portraits, `Movie_Create_Elesis_CA` and the DK / CA name strings on the creation screen are CHNa's.

Hygiene: `flagcheck.py SERV_IRUHADEV_JUNE_CHAR_ELESIS_UI --range <CH5 commit>^..<CH5 commit>`, `junediverge.py --check-all`, `enumscan.py --check X2Lib/X2GUEL.cpp`.
Read the user's play-test logs with `python tools/logs.py --grep "ELESIS"` and `--tag JUNE_GFX`. Fix clear errors in place (bytepatch only). Write "CH5 - Review" into the plan doc, update the STATUS row,
check `kickoffs/char-elesis/CH6.md` (it must walk cut-in sets 0-3, the hyper cut-ins on 120 / 121 / 125 and the creation screen), commit with explicit pathspecs.
