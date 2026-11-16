# June character Add (Stage 7) - Phase CH1d0 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-add CH1d0 review` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | CH1d0 done (its Done note and commit: `git log --oneline --grep "char-add Phase CH1d0"`) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_CORE` (the helpers) and `SERV_IRUHADEV_JUNE_DAMAGEEFFECT_ALPHA_FADE` (CEffect's fade, live: a no-op until a step is set) |
| Log tag | none (nothing calls the helpers until CH1d / CH1j / CH2a; no play-test) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Rules"
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md "CH1d0 - Done (2026-11-16)"
grep -P "^[0-9a-f]{8} \(CH1d0\)" june2017_re/funcmap.tsv
grep "^| 780 " JUNE_BUGS.md
```

## Task

Review CH1d0's commit against Ghidra (read-only), the raw listings (`python june2017_re/disasm_range.py`, Ghidra `disassemble_function`) and
`charport_consts.py --fn 32294530`:
- **`32294530` -> `CX2GameUnit::GetNearestPosOnLineMap`** (`X2GameUnit.cpp`), the phase's main work, read from the listing (the decompile mixes stack
  slots): the `CanDown` arguments ( `@322946c1`: in index NULL, out index, bOnLine 1, bDroppable 0 ), the false path ( `@32294934`-`322949f5`: the
  start-vs-end pick, the walk on `&vPos` with 10.0, the y restore, the address test kept as JUNE_BUGS 780 ), the true path ( the 0 <= t < fLength
  projection, the 999999 / 30.0 thresholds, the beforeLine / nextLine pick, `LT_MONSTER_ROAD` only on the side line ), the scan ( the skip set, `<=`
  keeps the last, start / end / interior with NaN going interior ) and the final walk from `vPos` ( not the scan's point ). Say whether the March
  code's `vBest = vPos_` start ( June leaves it unset ) can ever reach the output.
- **The fade ( `_DAMAGEEFFECT_ALPHA_FADE` )**: `31f75da0` clamp order and the `IsSamef` gate; `31f75be0` mesh `+0x4b8` = `m_Color.a`
  ( `SetMeshAlphaColor` ), the three lists and the `+0x94` handle; `31cfe3f0`'s lambda ( rgb kept, alpha set ); the ctor values ( @31f7adc6-31f7ade0:
  `+0xb1c` 1.0, `+0xb20` 0 ); the hook at the end of March's `CEffect::OnFrameMove` against June's exits ( no-game @31f88864, head die @31f8aab9,
  normal end; the mid die @31f8aa77 skips it ). Confirm no other June writer of `+0xb1c` / `+0xb20` ( `june2017_re/dispsweep.py b1c b20` ).
- **The small helpers**, one look each: `32293d70` ( true without a line map, `IsSamef` on the dot ), `322d58c0` vs `322d55b0` ( only the origin
  differs ), `31f63d40` ( store order, `m_iSaveLastTouchLineIndex` is the int at `+0x990` ), `31fc9430`, `31edf5a0`, `3226e310` ( NaN and `<= 0` ),
  `31c82d50`, `31c834e0`; `31f63f20` = March's `SetDieDamageEffectScale( float, float, float )` and `31fcefa0` = june-engine R5's
  `UpdateCustomRenderState` ( nothing written for either ).
- The names written into the first-caller kickoffs ( CH1d, CH1j, CH1k2, CH2a, CH2b, CH2c, CH3c ) match the code.
- No `_DEBUG` define exists for this phase.

Hygiene is one command: `python tools/finish.py --review --commits <the CH1d0 commit> --with JUNE_CHAR_ADD_CORE JUNE_DAMAGEEFFECT_ALPHA_FADE
JUNE_CHAR_ADD_LIVE` (add `-v`; `_LIVE` is a partner only, so `Always.h`'s CH1a guard evaluates; explain every OTHER row and deleted line; a FAIL or
ERROR is a finding). The enumscan hit at `X2DamageEffect.cpp` ( a 1000 ms log throttle from 2026-09-26 ) is not CH1d0's.

Fix clear transcription errors under the same flag (no full build: no play-test; prove it with one-file `/Zs` probes of the touched `.cpp` files);
hand design decisions back to the user. Write "CH1d0 - Review" into the plan doc, update the CH1d0 STATUS row, check that `kickoffs/char-add/CH1d.md`
is ready (CH1d is unblocked by this review), and commit.
