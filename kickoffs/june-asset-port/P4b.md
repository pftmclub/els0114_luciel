# June asset port - Phase P4b - June-only files, kom data151-185

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, medium** effort, then type `/x2-phase june-asset-port P4b` |
| Plan doc | `JUNE_ASSET_PORT_PHASES.md` (branch `main`) |
| Depends on | P4a |
| Flag | none (P1's `SERV_IRUHADEV_KOM_MOUNT_DEBUG` must be on in the deployed exe) |
| Log tag | `[KOM_MOUNT]` (`python tools/logs.py --tag KOM_MOUNT`; KEM_ERROR lines in `ErrorLog.txt`, duplicate names in `existfileinfo.txt`) |
| Review | none |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_ASSET_PORT_PHASES.md "Amended 2026-11-16" "Rules" "Starting context" "Inbound routes" "Phase P4 - June-only into existing March koms (P4a / P4b / P4c)" "Play-test hand-off (every wave)"
```

## Task

Do wave P4b of "Phase P4": the June-only files whose kom is data151-185 (`june2017_re/p0_waves.tsv`; P0 re-cut the ranges on 2026-11-16 (about 9.4k / 12.7k / 14.5k files; the 2026-10-08 cut was 4.2k / 5.2k / 27.2k):
the table is the truth). Files other plans already staged are `same bytes` and are not on the list.
- **Gate (b) matters here:** every kom is a repack of a live archive. A `~changed` entry outside the wave that is not decode-equal
  stops that kom.
- **data036 and other shared koms:** check STATUS and `git status` for a peer holding an uncommitted change in that kom's
  `kom_contents` dir before packing it.
- **Routed files** ride their wave (the census `route` column): see "Inbound routes" and the P4 section.
- **`RidingPet_GodBeast_Fire`:** the wave that carries data140 / data141's 15 files (see the P4 section) is followed by
  `python tools/luasrc.py adopt-june data037 RidingPet_GodBeast_Fire.lua` (`--dry-run` first), then pack and copy data037.

Pack and copy yourself, no ask (user, 2026-11-10; skill `x2-kom-packed`). Run the gates in order (Rule 4):
1. `verify-staged`;
2. `pack --wave`: `-0 removed`, and every `~changed` is in the wave or decode-equal;
3. `dupcheck` over the would-be-live set, right before the copy (peers stage into the same koms);
4. `copy --wave`, then check md5. It refuses while the game runs: ask the user to close it, never kill it.

Then (Rule 9): re-run `june2017_re/asset_port_census.py`, commit `june2017_re/P4b_stage_plan.tsv` (from `stage-june --plan-out`), and
write the wave's `MODS.md` entry (koms, counts, revert recipe, proving log lines). A held name (`tools/data/asset_port_held.tsv`,
Rule 8) is never on a list. Report the `PACK OK` / `COPY OK` lines in the hand-off.

The hand-off is copy-ready and detailed (plan doc "Play-test hand-off (every wave)"): real places from the wave list, the dev-console line
that gets there, what June's version looks like, what a failure looks like, the log tag you will read. Never launch the game.

Finish as `x2-phase` says: no build (data only: the exe is P1's, so the Done note says "no build: data only"), a Done note of at most ~60
lines under the phase, the STATUS row (one line), the next kickoff checked, one commit with explicit pathspecs, subject
`june-asset-port Phase P4b: <what>` (about 100 characters). `finish.py` is for client source and none changed.
