# June character Lu/Ciel (Stage 8) - Phase CH1k2 review (covers CH1k, CH1k2)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1k2 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1k and CH1k2 done (their Done notes and commits `da5999ca` and the CH1k2 code commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (`_LIVE` stays `//#define`), `SERV_IRUHADEV_JUNE_CHAR_UNIT_MP_VIRTUAL`, `SERV_IRUHADEV_JUNE_ENUM_NAMES` (`PGT_AMUSEMENT_ITEM`) |
| Log tag | none (nothing runs until `_LIVE`; no play-test) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH1k - Done (2026-11-14)" "CH1k2 - Done (2026-11-14)"
python june2017_re/charport_adis.py luciel --phase CH1k2 > <scratchpad>/ch1k2_adis.txt
```
Then `awk -F'\t' '$6=="CH1k2" || $6=="CH1k"' june2017_re/char_luciel/ch0_class_map.tsv` and JUNE_BUGS 707-708.

## Task

Review CH1k and CH1k2 against June's listings. Spot-check at least these, in Ghidra or the capstone dump (read-only):
- **CH1k's one inference:** `GetWeaponIndexHavingModel` `32b151b0`: Lu's array `CX2Eqip +0x5a4` taken as the hyper model array ( `+0x544` normal anim, `+0x564` hyper anim, `+0x584` normal model ).
- **`ReInit` `32b373e0`:** the PvP-room test ( `g_pData +0x6c`, `+0xd8 == 8` = `PGT_AMUSEMENT_ITEM` ), team 0 / 1 only, kill lead from `CX2Game` `+0x4a4` / `+0x4a8` ( taken as Red / Blue team kill: confirm
  the member order ), lead above 1 / exactly 1 / amusement branches, the second form index, and the `+0x8f70` window delete ( stand-in, same as the destructor ).
- **`IsInvasionOfDarkEnergyActive` `32b4b580`:** the listing is `0 <= time` ( `fcomp` + `test ah, 0x41` + `jp` ), CH0's hint said `0 < time`.
- **State-id getters:** every constant of the 18 `0x1a8`-based rows against the `LSI_*` names ( decoded in CH1k2's Done note ); the Die pair returns Lu's id while transformed; `GetWaitStateID` is the only one that calls `GetStateIDByForm`.
- **`32b4cfb0` and `32b4dd50`** are STL / March's `SetHyperModeCountByBuff`: confirm no class code is owed.
- **The `+` declarations:** the eleven getters whose base is not virtual in March ( `GetWaitStateID` .. `GetDieBackStateID` ) are declared `virtual` in `X2GULuciel.h` only; CH1o ( kickoff amended ) makes the bases virtual.

Hygiene is one command: `python tools/finish.py --review --commits da5999ca <the CH1k2 code commit> --skip-flag GAGE_TALLY ... ` ( the `#error` guard targets of CH1k's `Always.h` block, as CH1k's note ); run with `-v`
and explain every OTHER row and deleted line. `--check-all` runs inside `--review`. No debug define in either phase to retire.
Fix transcription errors under the same flags ( one-file `/Zs` probes of `X2GULuciel.cpp`, no full build: no play-test ). Write "CH1k2 - Review" into the plan doc, update the STATUS rows ( CH1l is
blocked on this review ), commit.

You are on Opus: delegate the reads, greps and known-address dumps ( x2-phase, *When the phase is Opus* ).
