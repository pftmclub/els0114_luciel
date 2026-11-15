# June character Lu/Ciel (Stage 8) - Phase DU6 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU6 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU6 done and its play-test passed (Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_AWAKEN_EFFECT` (no debug define) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`; DU6 adds no log line, the check is visual) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU6 - Done (2026-11-16)"
```
and `grep "^32291060\|(DU1d2)" june2017_re/funcmap.tsv`, the `HyperModeBuffEffectOnFrameMove` row of `tools/data/june_march_divergent.tsv`.

## Task

Review DU6's commit against June (Ghidra / `june2017_re/disasm_range.py`, read-only):
- `32291060 @32291266-322913ef`: the four positions come from vslot 0x84 ( `GetBonePos( const WCHAR* )` ) with `Bip01_R_Foot` / `_L_Foot` /
  `_R_UpperArm` / `_L_UpperArm`, the right-facing ( vslot 0x170 ) pairing and the left-facing swap as written; the emit-rate thresholds and the
  ( 15, 20 ) / ( 20, 30 ) / ( 30, 40 ) constants ( `3345b368`, `33438a8c`, `33441b9c`, `33438b40` ) equal March's.
- vslot 0x84 for a user is `31fda280` ( no prefix -> `32071d80`, the active body ) = March's `CX2GUUser::GetBonePos` under `JUNE_CHAR_DUO_BODY`; for an
  NPC June's slot 0x84 vs March's `CX2GUNPC::GetBonePos( const WCHAR* ) const` ( `m_pXSkinAnim`, the same frames March cached ).
- No other June per-frame or start path re-binds the hyper-boost handles on a body switch ( `SetActiveBody` `3204e030`, Lu's `32b1f0b0` ).
- Flag-off = March by `cl /P` of `X2GameUnit.cpp` (flag on vs `//#define`).

Hygiene is one command: `python tools/finish.py --review --commits <DU6's commit> --with SERV_IRUHADEV_JUNE_CHAR_DUO_AWAKEN_EFFECT` (`-v`; explain
every OTHER row). Fix clear transcription errors under the same flag (then `tools/build.py --deploy`); hand design decisions back. Write
"DU6 - Review" into the plan doc, update the STATUS row, commit.
