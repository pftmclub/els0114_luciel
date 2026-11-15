# June character Lu/Ciel (Stage 8) - Phase CH1 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1 done and its play-test passed (its Done note names the commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`, `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_DUO` (both defined by CH1), `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (one override), `SERV_IRUHADEV_JUNE_USER_VIRTUAL_HOOKS` (the two die-state ends) |
| Log tag | `[JUNE_CHAR]` (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH1 - Done"
python tools/docsection.py JUNE_STATE_IDS_PHASES.md "S2 - Done"
grep "(CH1)" june2017_re/funcmap.tsv
```

## Task

Review CH1's commits ( `5e880f3c`, fix `975e8b87` ), CH1q's commit `16997120`, and **june-state-ids S2's commit `857b18cd`** ( S2 had no review of its own; `CH1.md`'s CH1o amendment hands it here ), against
June in Ghidra (read-only):
- The factory `3230f470` and its two callers `322ced00` @`322ceeb4` / `322d5bf0` @`322d5d8f`: case 10 does nothing but `new` + ctor `32b1bd90` ( no
  extra call after it that March's `CreateGUUser` / `AddUserUnitIntruder` lacks ). Say which caller is which.
- The `CX2Game` ctor / dtor guide calls ( @`32307bca` / @`32308957` ): order memorize -> Lu/Ciel -> Eve, `&false` and `0.f`.
- `ParseCommonRandomState`: June has no class override ( base `Init` `3204ea20` calls `31feaac0` ); every class name `GetUnitClassName` gives
  ( 200 / 37 / 38 / 39 / 127 / 128 / 129 ) is a key in `Luciel_Duo.lua`'s `INIT_COMMON_RANDOM_STATE`.
- The die-state ends ( `X2GUUser.cpp` `DoStateEndStart` ): `ch0_reg.tsv` rows 185 / 186 give Ciel's 192 / 193 `End=329cada0>3202a7d0`; the getter test
  matches both bodies and a transformed unit's Lu die state. The play-test's Ciel death is the proof.
- S2 ( `857b18cd` ): `InitBoundingStates( "LSI_LU_" )`, `InitWaitStateChange( "LSI_WAIT_STATE_CHANGE" )`, the shadow `USI_SPECIAL_ATTACK_*` removal,
  `InitEventStates` no longer writing 92, the `Always.h` guard. Re-read June's InitState `32b43d10` at @`32b44174` and the bounding registrations.
- The data: data036 `Luciel_Duo.lua` / `Luciel_Demonio.lua` and data034 `DLG_Luciel_CombinationPoint.lua` = June's bytecode ( `luabind.py check` the three ),
  data036 `Enum.lua` `RNSI_A_LLD_BLUFF = 30304` = June's `Enum.src.lua:15069`.
- CH1q ( `JUNE_DIVERGE_WEAPON_LOAD_DATA_FOR_USER` ): `CreateSkinAfterImage`'s NULL test = June `31f8ace0`; March's `Weapon::LoadDataForUser` `#ifdef` body =
  June `3228d660` line by line ( head guard, the inventory `GetItem( uid, true )` @`3228ea5a`, the 15 switch ids @`3228eb09`-`3228eb85`, the 4 traces ).
  Add `JUNE_DIVERGE_WEAPON_LOAD_DATA_FOR_USER` to the `--with` list below and CH1q's commit to `--commits`.
- The play-test result: every check in the hand-off has a verdict; any failure is routed ( a fix phase in this plan if it is playability, else its owner ).

Hygiene is one command, not a fresh script:
`python tools/finish.py --review --commits <CH1's code commit> 857b18cd --with JUNE_CHAR_LUCIEL_CORE JUNE_CHAR_LUCIEL_LIVE JUNE_CHAR_LUCIEL_DUO JUNE_USER_VIRTUAL_HOOKS`
( add S2's own flag if the run names it ). Run with `-v` and explain every OTHER row and deleted line; a FAIL or ERROR is a finding.

No `_DEBUG` define to retire ( CH1 added none ). Fix clear transcription errors under the same flag ( then `tools/build.py --deploy` and a play-test of the
fix ); hand design decisions back to the user. Write "CH1 - Review" into the plan doc, update the STATUS.md row ( CH2a, DU3, CHN, TU unblock on it ),
check those four kickoffs carry CH1's amendments, and commit.
