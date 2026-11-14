# June character Lu/Ciel (Stage 8) - Phase CH1i2 - Engine helper: June's GetFixedDistancePositionForWeaponTaker (Opus)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1i2` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1i (done); independent of the CH1i review; one at a time in `KTDXLIB/KTDGLineMap.*` |
| Flag | `SERV_IRUHADEV_JUNE_DIVERGE_FIXED_DISTANCE_WEAPON_TAKER` (new; `KTDXLIB/Always.h` house block; needs `RAVEN_WEAPON_TAKER`: confirm with `cl /EP` before saving the `#error`) |
| Log tag | none (inert until a state calls it; CH3c, the Demonio bounding strike, is the first play-test) |
| Review | kickoffs/char-luciel/CH1i2-review.md (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH1i - Done (2026-11-14)"
grep -n "GetFixedDistancePositionForWeaponTaker" tools/data/june_march_divergent.tsv tools/data/june_diverge_allow.tsv june2017_re/funcmap.tsv
python tools/junediverge.py --open-rows
```

## Task

June `31c60890` ( thiscall on the line map: `pos*`, `float dist`, `bool bIsRight`, `OUT D3DXVECTOR3*`, `ret 0x10`; 682 lines in Ghidra, `decompile_function 0x31c60890` ) is the June body of
`CKTDGLineMap::GetFixedDistancePositionForWeaponTaker` ( `KTDGLineMap.cpp:2229`, `RAVEN_WEAPON_TAKER` ). CH1i found it is **not** March's: a negative distance flips `bIsRight` and uses `-dist` ( `@31c608a3`-`31c608ba` ), the start line comes from a scan of
every enabled non-wall line with a point-to-segment test ( 900 = 30 squared ) and a nearest-end fallback over all enabled lines, not from `GetLandPosition`; the walk along the lines was **not compared**.
Read the whole function, compare it with March's, and port June's whole body inside March's function under the flag, March's in `#else` ( the D1 shape of `GetFixedDistancePosition`, divergent row `31c5f090` ).
Keep June's numbers and quirks ( a `JUNE_BUGS.md` row each ). `KTDGLineMap.cpp` is CP949: `bytepatch.py` only.

- Callers to keep in mind: March's `X2GUAdd.cpp:5649`, `X2GUEL.cpp:7767`, `X2GURaven.cpp:29126` and Lu/Ciel's `32b341b0` / `32b343b0` / `32b35fd0` ( `X2GULuciel.cpp` ). The June callers ( ~30 ) pass `( &pos, dist, bIsRight, out )` as March does; a caller whose March code differs from June's is a finding, not a fix.
- When it lands: set the divergent row's `source` to name the flag ( `junediverge.py` then counts it done ), delete the four `GetFixedDistancePositionForWeaponTaker` rows of `tools/data/june_diverge_allow.tsv`, and say so in the Done note. `python tools/junediverge.py --check-all` must print `RESULT: PASS` with no `GetFixedDistancePositionForWeaponTaker` allow row left.
- Done gate: the replacement compiles under the flag on and off ( one-file `/Zs` probe of `KTDGLineMap.cpp` and `X2GULuciel.cpp` ); `finish.py --files KTDXLIB/KTDGLineMap.cpp KTDXLIB/Always.h tools/data/...`; `junediverge --check` / `--check-all`, `enumscan --check` PASS. Full build only when the hand-off is a play-test ( it is not: nothing plays it before CH3c ).

You are on Opus: delegate the reads, greps and known-address dumps ( x2-phase, *When the phase is Opus* ).
No hardcoded enum literals and no `June` prefix ( CLAUDE.md rules ).
