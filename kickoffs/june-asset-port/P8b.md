# June asset port - Phase P8b - the four _INT.txt definitions

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase june-asset-port P8b` |
| Plan doc | `JUNE_ASSET_PORT_PHASES.md` (branch `main`) |
| Depends on | P0, P4c |
| Flag | none (P1's `SERV_IRUHADEV_KOM_MOUNT_DEBUG` must be on in the deployed exe) |
| Log tag | `[KOM_MOUNT]` (`python tools/logs.py --tag KOM_MOUNT`; KEM_ERROR lines in `ErrorLog.txt`, duplicate names in `existfileinfo.txt`) |
| Review | none |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_ASSET_PORT_PHASES.md "Amended 2026-11-16" "Rules" "Starting context" "Inbound routes" "Phase P8b - The four `_INT.txt` definitions" "Play-test hand-off (every wave)"
```

## Task

Do "Phase P8b" in the plan doc (new 2026-11-16; backlog Stage 15, added by char-eve CH0 2026-10-26): stage June's `UIMajorParticle_INT`,
`GameMajorParticle_INT`, `UIMajorXMeshPlayer_INT` and `GameMajorXMeshPlayer_INT.txt` over March's (datai015). Evidence:
`june2017_re/char_eve/ch0_effect_closure.tsv`.
- **Pre-check first:** parse June's four with March's own `.txt` parser and list any key it does not know (March aborts a `.txt` at its first
  unknown key). Hand an unknown key to its owner (Rule 6); it does not hold the file, but the play-test must know what to look for.
- **Also the 7 other `.txt` rows** the census marks `route=txt-review` (june-effect, their old owner, is closed): `GameOptionPreset`, `UnitTitle1300`,
  `UnitTitle1310`, `fontconfig`, `AIN_POWER_OF_GOD_06_1`, `Npc_All_Ariel_Face_Talk_Dark`, `NUI_ERIS_BOSS_Map03_A`; same pre-check, same wave. `DialogLog.txt` stays dropped.
  **`Npc_All_Ariel_Face_Talk_Dark.txt` is 0 bytes in June** (P2 found it, 2026-11-16): `elstools pack` cannot pack an empty entry (verify fails), so decide before staging: leave it out and record why, or find a tool path that keeps it.
- **Hand-off:** the village, a dungeon boss and a skill-heavy character: particles and the UI particle layers still draw.

Pack and copy yourself, no ask (user, 2026-11-10; skill `x2-kom-packed`). Run the gates in order (Rule 4):
1. `verify-staged`;
2. `pack --wave`: `-0 removed`, and every `~changed` is in the wave or decode-equal;
3. `dupcheck` over the would-be-live set, right before the copy (peers stage into the same koms);
4. `copy --wave`, then check md5. It refuses while the game runs: ask the user to close it, never kill it.

Then (Rule 9): re-run `june2017_re/asset_port_census.py`, commit `june2017_re/P8b_stage_plan.tsv` (from `stage-june --plan-out`), and
write the wave's `MODS.md` entry (koms, counts, revert recipe, proving log lines). A held name (`tools/data/asset_port_held.tsv`,
Rule 8) is never on a list. Report the `PACK OK` / `COPY OK` lines in the hand-off.

The hand-off is copy-ready and detailed (plan doc "Play-test hand-off (every wave)"): real places from the wave list, the dev-console line
that gets there, what June's version looks like, what a failure looks like, the log tag you will read. Never launch the game.

Finish as `x2-phase` says: no build (data only: the exe is P1's, so the Done note says "no build: data only"), a Done note of at most ~60
lines under the phase, the STATUS row (one line), the next kickoff checked, one commit with explicit pathspecs, subject
`june-asset-port Phase P8b: <what>` (about 100 characters). `finish.py` is for client source and none changed.
