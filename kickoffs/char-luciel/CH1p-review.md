# June character Lu/Ciel (Stage 8) - Phase CH1p review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1p review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1p done (its Done note names the commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_DUO` (`//#define` until CH1 final), `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (class code), `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` (one enum-name line in `X2UnitManager.cpp`) |
| Log tag | none (no play-test; the `[JUNE_CHAR] LUCIEL DUO` lines run from CH1 final) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH1p - Done"
grep "(CH1p)" june2017_re/funcmap.tsv
```

## Task

Review CH1p's commit against June in Ghidra (read-only):
- `AddCombinationPointOnHit` `32b14bc0`: the `+0x594` test, the FCOMP / `TEST AH,5` / `JNP` cool-time test ( a NaN passes ), 1.0 into `+0x96a8`.
- `SetBoundingTarget` `32b38cb0`: the order of the four exits ( `+0x973c >= 0` with a NaN failing, uid sign, vslot 0x200 = `GetRidingOn`, the `+0x96f0` / `+0x96ec`
  test ), the cost roll ( vslot 0x1cc = `GetRandomFloat( 0 )` against `+0x9730`, `TEST AH,0x41` / `JP` -> 1 ), the gage `+0x114` read, game vslots 0xa0 / 0xa8, the
  stores ( `+0x9700` 0.5, the key at `+0x96f0` / `+0x96f8`, `+0x96e8` / `+0x96e0` cleared ), the static name ( `334f34a0` ) and `GetKeyString( 0x27 )` = `GAMEACTION_SIT`.
- `UpdateBoundingKeyDialogPosition` `32b35cf0`: vslots 0x138 ( `GetPos( true )` ), 0x16c / 0x170, constants 100 / 208 / 1500, app `+0x1b8` / `+0x1bc` =
  resolution scale, camera `+0x38` / `+0x44` = eye, and the dialog `+0x120` / `+0x124` = June's `m_vSize` ( June's dialog ctor `31cd9230` puts the drag range at
  `+0x12c` / `+0x134`, March's `m_vSize` is at `+0x110` ): JUNE_BUGS 726.
- The hooks: `31fd02d0` ( user, after CommonStateStart ) and `3275a070` @`3275a0ac` ( NPC, `nowState == +0x8708` = `mBoundingLandCrash`, placed after March's
  bounding-up block ); both pass the victim's `+0x264` type, vslot 0x54 uid and `+0x40dc` serial.
- The four member renames ( `m_iPendingCombinationPoint`, `m_iBoundingStrikeCombinationPointCost`, `m_iBoundingTargetLastHitAttackSerial`,
  `m_fFreeBoundingStrikeRate` ) against their June users ( `ch1_offsets.tsv` ), JUNE_BUGS 723 withdrawn, 726 / 727 right.
- The data: data036 `Enum.lua` `DUO_TYPE` = June's `Enum.src.lua:17515`; `DLG_LUCIEL_BOUNDING_KEY.lua` in data034 = June's ( `luabind.py check DLG_LUCIEL_BOUNDING_KEY` ).
- The two routes: the delegate push to CH4a, `NOT_SHOW_UNIT_BY_DUOTYPE` to DU3 ( their kickoffs' CH1p amendments ).

Hygiene is one command, not a fresh script:
`python tools/finish.py --review --commits <CH1p's code commit> --with JUNE_CHAR_LUCIEL_DUO` (Always.h: `_DUO`'s only complaint is "not #defined", the
deferred flag; `--with JUNE_CHAR_DUO_BODY` trips on a peer's `#if defined && !defined` guard in `Always.h`, so run `flagcheck JUNE_CHAR_DUO_BODY --commits <c>
--files X2Lib/X2UnitManager.cpp` and `flagcheck JUNE_CHAR_LUCIEL_CORE --with JUNE_CHAR_LUCIEL_DUO --commits <c> --files X2Lib/X2GULuciel.cpp X2Lib/X2GULuciel.h`
on their own). Run with `-v` and explain every OTHER row and deleted line; a FAIL or ERROR is a finding.

Compile: `/Zs` probes ( census `cl_cmd` ) of `X2GULuciel.cpp`, `X2GUUser.cpp`, `X2GUNPC.cpp`, `X2DamageManager.cpp`, `X2UnitManager.cpp`, plain and with
`-DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_DUO -DSERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE`. No full build ( no play-test ).

Fix clear transcription errors under the same flag; hand design decisions back to the user. Write "CH1p - Review" into the plan doc, update the
STATUS.md row, check `CH1.md`, and commit.
