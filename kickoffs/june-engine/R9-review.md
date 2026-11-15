# June engine remainder - Phase R9 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-engine R9 review` |
| Plan doc | `JUNE_ENGINE_REMAINDER_PHASES.md` (branch `main`) |
| Depends on | Phase R9 done (its Done note and commit) and its play-test read |
| Flags | `SERV_IRUHADEV_JUNE_RENDER_PARTICLE_ADD_TYPES`, `_PARTICLE_EFFECT_ALPHA_PATHS`, `_PARTICLE_SEQ_ALPHA`, `_PARTICLE_OUTLINE_CLAMP`, `_RENDER_EFFECT_STATE_LOST`, `_RENDER_SKINMESH_SMALL_LOADS` (+ `_RENDER_SMALL_DELTAS_DEBUG`, retired by this review) |
| Log tag | `[JUNE_ENGINE] FX small delta` (`python tools/logs.py --grep "FX small delta"`) |

## Read first

```sh
python tools/docsection.py JUNE_ENGINE_REMAINDER_PHASES.md "Phase R9 - Render small deltas" "Phase R9 -- Done" "Phase R0b -- Done"
```

and the funcmap rows `(R9)` (`grep -n "(R9)" june2017_re/funcmap.tsv`).

## Task

Review R9's commit against June `31cf3430`, `31cf62a0` @`31cf65d0`-`31cf7034`, `31cf10c0`, `31cf16b0` @`31cf17e2`-`31cf18d1`, `31cf4ed0` @`31cf562b`,
`31cfe050`, `31d3c090`, `31d184f0`, `31be66b0`, `31d4f2a0`, `31d4e850` and the inlined XET name in `31d4f8b0` @`31d5736e`-`31d587a2`. What this phase could get wrong:
- the system kind `+0xdc`: which `X2Data` Reset function stores 2 / 3 / 0 / 1 (`31ede2d0` / `31ede39c` / `31ede45c` / `31ede51c`; check the ctor id pushed before each
  `31cf88c0` against March's ids 0 / 1 / 2 / 3), and that the clamp is 1.2 for kinds 2 and 3 only;
- the colour order in the RHW and vertex paths: sequence alpha into the float colour, pack to DWORD, then the effect alpha on the packed bytes;
- the skin-mesh draw's effect alpha: the call sits after the whole render-param fill, `31ceefb0`'s blend rule, and `outLineColor.a` compounds (JUNE_BUGS row);
- `CKTDXApp::OnLostDevice`: the state manager reset runs without the renderer's `SUCCEEDED( hr )` condition and the renderer no longer resets it;
- the XET name: `rfind( L'.' )` with no dot gives the whole id; `_UnLoad` and `DestroyMeshContainer` skip the close when the device manager is NULL (the pointer stays).

Hygiene is one command: `python tools/finish.py --review --commits <R9's commit> --with SERV_IRUHADEV_JUNE_RENDER_PARTICLE_ADD_TYPES ...` (all flags
above); explain every OTHER row and deleted line (the mesh-draw comment line is a deliberate replace). Retire `SERV_IRUHADEV_JUNE_RENDER_SMALL_DELTAS_DEBUG`
(comment out the define; then no build, see CLAUDE.md "debug retire only"). Spot-check 2-3 items in Ghidra / `disasm_range.py`. Write "Phase R9 -- Review"
into the plan doc, update the STATUS row, commit.
