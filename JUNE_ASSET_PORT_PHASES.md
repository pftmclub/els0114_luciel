# June asset port: every remaining June non-lua asset in the March client

Plan key `june-asset-port` · branch `main` · log tag `[KOM_MOUNT]` · flag family `SERV_IRUHADEV_KOM_MOUNT_*` ·
kickoffs `kickoffs/june-asset-port/` · backlog Stage 15 · created 2026-10-08 · **brought up to date 2026-11-16** (see "Amended 2026-11-16").

## Amended 2026-11-16 (user: "this plan is kinda old")

The plan was written before the studio's current phase loop and before other plans staged assets of their own. What changed, and where
this doc now says it:

| Since 2026-10-08 | Effect on this plan |
|---|---|
| **Pack and copy need no ask** (user, 2026-11-10; skill `x2-kom-packed`) | Rule 3 and every "ask before copy" step: you copy at once, one `copy --wave` per wave. The tool still refuses while the game runs: then ask the user to close it, never kill it. P11 (a linker setting) is still proposed first. |
| **Other plans stage assets of their own**: char ports (a class's June assets go in whole, CH0b), dungeons and villages (C3x, C3s, C3z-1, W3), june-item-kim K2 (501 files), june-engine, atlas, costume shop | The 2026-10-08 counts are a **baseline**. P0's census is the truth; a file a peer staged is `same bytes` and drops out of every wave. "Inbound routes" below lists what the other plans handed this one. |
| **Merge mods and hold-outs** (W3f 2026-11-12: 7 March village files kept over June's; `Motion_<Class>*` stay with the class ports, 2026-10-18) | New Rule 8. P0's census has a `held` bucket, and no wave list may contain a held name. |
| **`SERV_IRUHADEV_KOM_LONG_NAMES` (X4) is closed and defined** | The 59-character refusal in `stage-june` goes (P0). Names of any length stage. |
| **june-effect is closed** (the `.txt` particle and mesh-player files were its) | The four `_INT.txt` definitions are this plan's (backlog Stage 15, char-eve CH0 2026-10-26): new phase **P8b**. |
| **june-engine R2 copied `XRenderer.fx`** (2026-10-17) | P10 is gates and a play-test only (already noted there). |
| **Phase loop** (`x2-phase`, `finish.py`, `tools/README.md`) | Done note at most ~60 lines; STATUS row one line; commit subject `june-asset-port Phase <id>: <what>` (about 100 characters); `finish.py` for any phase that touches client source; a full build only for a phase that hands off a play-test; a debug define is retired by commenting it out, with no build. Each wave also gets a `MODS.md` entry (Rule 9). |
| **Batched play-tests** (opt-in per plan, x2-phase step 7) | This plan keeps `Hand-off: now`. The waves are data-only and share one check (mounts, KEM errors), so the user may opt in later with `batch at <phase>`. |

## Context

`june-asset-extract` (X0-X5, closed 2026-10-08, `JUNE_ASSET_EXTRACT_PHASES.md`) built the tools:
- `tools/junekom.py`, the byte-exact June extractor;
- `kom.py stage-june`, which enforces the algorithm, placement, duplicate and long-name rules;
- `SERV_IRUHADEV_KOM_LONG_NAMES`.

X5 shipped only the 286 files March already had in a damaged or hand-patched form. The user then asked (2026-10-08) to port **every
June non-lua asset still missing or different**, byte-exact, so the later lua ports find their assets in place.

Decisions (user, 2026-10-08):
- **Every collision is replaced by June's file.** The user's words: "replace existing March assets if they are colliding with the
  June assets". Nothing is held back. When a March consumer breaks because June's file lacks a key it reads, the consumer gets a
  follow-up fix: port the June consumer, or a Ghidra engine phase. The asset is never reverted or patched for it.
- **`.txt` stays with june-effect.** March aborts a `.txt` at its first unknown key, and june-effect owns those keys. This covers the
  34 colliding and 51 June-only particle, mesh-player and other `.txt` files.
- **Scaleform files ship now, inert.** The kom `.gfx`/`.swf`, the event-scene `.xml` and the loose `eventscene/*.swf` go in byte-exact.
  March has no loader, so nothing reads them until Stage 9.

## Rules

1. **June files are byte-exact.** No edit, ever. If March cannot use one, that is a Ghidra-traced engine phase, never a file patch
   (memory `june-assets-byte-exact-no-edits`).
2. **June's `14JUN17\data` and its koms are read-only inputs.**
3. **Staging and packing go through the tools.**
   - Every staging goes through `kom.py stage-june` and every pack through `kom.py pack`.
   - **Copy without asking** (user, 2026-11-10; was "one AskUserQuestion per wave"): `kom.py copy --wave` right after the gates pass.
     Report the `PACK OK` / `COPY OK` lines in the hand-off.
   - The game must be closed. `copy` refuses while it runs: ask the user to close it (never kill it), then rerun.
   - **Shared koms:** other sessions stage into the same koms. Pack a kom only when no peer holds an uncommitted change in its
     `kom_contents` dir (`git status`, STATUS "Touches"), and run gate (c) right before the copy, not at staging time.
4. **Gates, in force from P0** (P0 builds the tools):
   - **(a) No-loss.** A pack diff with `-N removed` refuses the copy.
   - **(b) Unexplained change.** An entry that shows `~changed` but is not on the wave list is decoded from both koms and
     byte-compared. Equal bytes pass. Anything else stops that kom.
   - **(c) Global duplicate.** Checked right before each copy, over the would-be-live set: the live koms with this wave's packed koms
     swapped in. Peer sessions stage too, so a check at staging time is not enough.
   - **(d) Staged bytes.** Every staged file equals June's strict decode.
5. **Pairing and order.**
   - An atlas texture and its `.TET` move together.
   - A skin mesh and its paired motion move together when both differ.
   - June-only files land before the replace waves, so a June mesh that names a June-only texture finds it.
6. **Collisions: June replaces March, always.** A predicted or observed break goes to an owner, not to a question:
   - **class-specific consumers** (a class motion name, a weapon bone) go to `JUNE2017_CHARACTER_PORT_CHECKLIST.md` (memory
     `hand-char-port-material-to-char-ports`);
   - **any other consumer** goes to the plan that owns it: an open `STATUS.md` block or a `JUNE_PORT_BACKLOG_ORDER.md` stage (Stage 9 UI,
     the dungeon / village plans, ...). june-effect and june-engine are closed: a break with no open owner becomes a bullet and brief in
     the backlog stage that fits, or a new stage, in the same session (CLAUDE.md "a plan keeps to its subject");
   - **if March's engine cannot read June's file at all**, a new Ghidra phase (memory `plan-unowned-june-branches-without-asking`).
7. **One flag per logical change.** Use `SERV_IRUHADEV_KOM_MOUNT_DEBUG` (P1, short-lived) and, if P11 runs, its own change. Edit
   `KGCMassFileManager.cpp` (CP949) with `tools/bytepatch.py` only.
   - File-local constants and helpers go in a named `_CONST_KOMMOUNT_P1_` namespace followed by `using namespace`, never `namespace { }`.
   - A comment is one short line. Names say what the thing is (`iRegisteredCount`), with no phase tag or `June` in them.
   - Gate command: `python tools/finish.py --files KTDXLIB/KGCMassFileManager.cpp KTDXLIB/Always.h`; after the commit,
     `--range <c>^..<c>` (never `^..HEAD`).
8. **Hold-outs: a held name is never staged** (new, 2026-11-16). P0's census marks each as `held` with its reason, and
   `verify-staged` and `pack --wave` refuse a wave list that contains one:
   - the 7 March village files W3f kept over June's (memory `village-route-is-a-march-june-merge-mod`; the list is `KEPT_MARCH` in
     `june2017_re/w3b_asset_closure.py`, with `KEPT_MARCH_W3F` in `w3b_village_census.py`);
   - the 83 `Motion_<Class>*` files of the nine March classes in data033 / data010 (june-engine R12b, 2026-10-18): each class port stages
     them with its script;
   - the 306 March-only files C3z-1 removed because June merged them away (`june2017_re/c3z1_june_dropped.tsv`; they are not in any
     kom now, and no wave brings them back);
   - any name a later plan adds to `tools/data/asset_port_held.tsv` (P0 creates it: `name`, `reason`, `owner`, `date`).
9. **Tracking** (new, 2026-11-16). Every wave leaves three records, in the same commit as its Done note:
   - `june2017_re/<phase>_stage_plan.tsv` (from `stage-june --plan-out`) and the `PACK OK` / `COPY OK` lines;
   - a `MODS.md` entry per wave: the koms, counts, the revert recipe (`kom_backup_stage_june\<stamp>` for the staged tree,
     `kom_backup_tools\dataNNN.kom.<stamp>` for the live kom) and the log lines that prove it;
   - the STATUS row (one line) and the census re-run: P0's `p0_census.tsv` is regenerated after every copy, so the next wave plans from
     live state, not from the 2026-10-08 table.

## Starting context

Every repeated step is a tool (`tools/README.md`). Do not hand-type msbuild, unifdef, kom packing or log greps.

| Need | Command |
|---|---|
| One section of this doc | `python tools/docsection.py JUNE_ASSET_PORT_PHASES.md "<heading>"` |
| June kom reader | `python tools/junekom.py list / extract / verify ...` |
| Stage June files | `python tools/kom.py stage-june <list> [--dry-run] [--long-names] [--plan-out F]` |
| Pack / deploy a kom | `/elstools pack dataNNN`, then `/x2-kom-packed copy dataNNN` at once (no ask since 2026-11-10; `tools/kom.py`) |
| P0's new gates | `kom.py verify-staged`, `kom.py pack/copy --wave`, `kom.py dupcheck`, `kom.py copy-loose` |
| Census (P0) | `python june2017_re/asset_port_census.py` -> `p0_census.tsv`, `p0_waves.tsv`; re-run after every copy |
| Done-note gate block | `python tools/finish.py --files <changed client files>` (P1, P11 only; data waves touch no client source) |
| Strings in the exe | `python tools/exestr.py "<string>" ...` (P1's `[KOM_MOUNT]` lines; paste the `EXESTR OK` line beside `DEPLOY OK`) |
| Edit a CP949 source | `python tools/bytepatch.py show / find / apply <spec>` (x2-byte-edit) |
| Flag hygiene | `python tools/flagcheck.py SERV_IRUHADEV_KOM_MOUNT_DEBUG --worktree --files <files>` |
| Build + deploy | `python tools/build.py --deploy` (background; x2-build-deploy) |
| Play-test logs | `python tools/logs.py --tag KOM_MOUNT` (x2-check-logs); KEM_ERROR lines are in `ErrorLog.txt` |

Paths:
- **June data:** `F:\elsword stuff\elsword_2014\els_2014\237311\14JUN17\data\`. It holds 258 koms (`data001-226`, `datai001-032`),
  the exact `*_kom\` extracts (verified by X2), and loose `music\`, `movie\` and `eventscene\`.
- **March staging:** `F:\elsword stuff\elsword_2014\els_2014\237311\24965799\kom_contents\dataNNN_kom\`.
- **March game dir:** `...\24965799\data`.
- **Intermediates** go to `.build\` or `kom_contents`, never the session scratchpad: C: has about 10 GB free, F: about 700 GB.

## Established facts (2026-10-08 baseline: P0's census supersedes every count below)

Since this table was written, C3x, C3s, C3z-1, W3, june-item-kim K2, june-engine, the atlas plan and the char ports have staged or
removed files in the asset koms (`june2017_re/c3z1_stage_plan.tsv`, `c3z1_june_dropped.tsv`, `june_item_kim/k2_assets_staged.tsv`, ...).
Expect P0's June-only and differs counts to be lower and its same-bytes count higher; P0's Done note explains the difference by plan.
The engine facts further down still hold.

**Census.** All 222,811 June entries were compared with the live March koms and `kom_contents` (md5 of June's exact extract against
the staged March file). P0 regenerates this table as `june2017_re/p0_census.tsv`.

| Bucket | Files | Raw size | Action |
|---|---|---|---|
| June-only | 117,562 | 10.5 GB | add: 33,819 into 66 kom numbers March has no archive for, 83,692 into 151 existing March koms |
| Same bytes | 92,457 | 4.7 GB | nothing |
| Differs (March's own version) | 4,397 | 0.9 GB | replace with June's (Rule 6), except the 34 `.txt` |
| March-unstaged | 1,066 (data101 1,065, data118 1) | 28 MB | restore into `kom_contents` first: a repack of data101 today would delete them |
| `.lua` | 7,329 | | lua ports, not here |

By type:
- **June-only:** dds 35,966; tga 26,699; ogg 25,426; x 19,792; y 5,015; xet 3,198; tet 826; gfx 503; txt 51; wav 16; xml 12; ttf 12;
  bmp 10; swf 6.
- **Differs:** tga 1,548; dds 989; x 847; y 622; xet 219; tet 111; txt 34; ogg 25; xml 1 (`AutoSetupGameOptionWeightConfig.xml`);
  fx 1 (`XRenderer.fx`).

**Loose files** (March loads them from disk only):

| Dir | New | Differs | Note |
|---|---|---|---|
| `music\` | 79 | 34 | |
| `movie\` | 32 `.avi` | 0 | |
| `eventscene\` | 2,383 `.swf` | | inert |

**Engine facts:**
- **New kom numbers mount.** `X2Main.cpp:799-830` mounts `data1..226` and `datai1..32` under
  `SERV_IRUHADEV_KOM_FILE_COUNT_226`. June's numbering is the same range.
- **A mount reads headers only.** Payloads are read on demand (`KGCMassFileManager.cpp:1826-1893`, `:2068`). Offsets are DWORDs, and
  the largest June kom is 262 MB.
- **Duplicates are loud.** Under `NOT_ALLOW_DUPLICATE_FILES` (`Always.h:3245`, defined), `AddMassFile` (`:576`) does three things:
  - deletes the whole kom;
  - shows a MessageBox;
  - under `_IN_HOUSE_`, appends the name to `existfileinfo.txt` (`:510`). That is a plain `fopen` append, so it survives the
    mount-time ErrorLog loss.
- **Mount-time ErrorLog lines are lost** (X4, memory `errorlog-loses-mount-time-lines`), so proofs are use-time lines.
- **The March `X2_offline.exe` is not LargeAddressAware.** June's `x2.exe` is, and June ships `increaseva.bat`.

## Inbound routes (what other plans handed this one; checked 2026-11-16)

Each row is a request in another plan's doc. P0's census must find every named file and mark it `route=<phase>`; a wave's Done note
ticks the rows it delivered, and P12 re-checks that none is open.

| From | What | Lands in |
|---|---|---|
| atlas (`ATLAS_PORT_PHASES.md`, E11 / E13) | 27 March-copy size differences (`atlas_a11_sizes.tsv`); 4 TET + texture pairs and 5 non-Elysion names (`DLG_Common_New_Texture75`, 7 March house keys) | P7 (pairs move together) |
| costume shop (`COSTUME_SHOP_PHASES.md`) | `costume_shop/withheld_files.tsv`: files the ~14,800 existing-character costume rows need | P4 waves / P6-P8; its hand-off carries the list |
| june-engine | `XEnvBoxRenderer.fx`, `XHeatShimmerRenderer.fx`, `XRenderer_X2TOOL.fx` (data041), 10 `Cartoon*.bmp` ramps (data042), `PerturbationMap.png`, `13Gang_Protect.tga` (data045) | P4a |
| june-engine | emotion motions (data199-209); the `<movie>.lua` files June's movie open reads | P2-P4; P9 |
| june-engine R2 | `XRenderer.fx` already live (md5 027c8a01) | P10 (gates only) |
| june-effect (closed) | `Village_ADBoard.dds` (data216); `RidingPet_GodBeast_Fire` assets (data140/141: `Motion_Riding_GodBeast_Fire.x`, `.tga`, 13 `.ogg`); 6 BGM tracks the C3y worlds name (`Music072_Stage`, `Music073_Boss`, `Music075_Boss`, `Music079_Boss`, `LI_Boss_Elysion001/002`) | P2 / P3; P9 |
| june-dungeons C3y / C3z-1 (already live) | 408 + 404 world files in data135 / 136 / 146 / 166 / 180 / 182 (two of them new archives) and 17 music files: **same bytes** in the census, so they leave the P2 / P3 / P9 lists | none |
| char-eve CH0 (2026-10-26) | the 4 `_INT.txt` particle and mesh-player definitions (datai015 -> June's) | P8b |
| char-rena | 3 Ruben-village textures (`RUBEN_VILLAGE_HERB01 / 02`, `RUBEN_TECHHOUSE_GANPAN`) | P4 |
| june-engine R12b / char ports | `Motion_<Class>*` (83 files): **not** this plan's | held (Rule 8) |
| june-dungeons W3f | 7 March village files kept over June's | held (Rule 8) |
| june-dungeons C3z-1b | data101 map assets (Velder / Henir), `c3z1_data101_pending.tsv`: waits for P0's restore (done) | its own plan, not a wave here |

## Phases

| Phase | What | Run with | Build | Play-test | Hand-off |
|---|---|---|---|---|---|
| P0 | Tooling, census, restore | Sonnet 5.5 high | no (no client source) | no | - |
| P1 | Mount diagnostic + baseline | Sonnet 5.5 high | yes | boot to village | now |
| P1 review | | Sonnet 5.5 high | | | - |
| P2 | Pilot: 5 new kom numbers | Sonnet 5.5 high | no | boot + walk | now |
| P3 | Remaining new kom numbers (61) | Sonnet 5.5 medium | no | boot + walk | now |
| P4a-c | June-only into existing koms, 3 waves | Sonnet 5.5 medium | no | boot + walk per wave | now |
| P5 | Collision key forecast | Opus 5.5 high | no | no | - |
| P5 review | | Opus 5.5 high | | | - |
| P6 | Replace: textures + sounds (no atlas change) | Sonnet 5.5 high | no | visual tour | now |
| P7 | Replace: TET/XET + paired atlases | Sonnet 5.5 high | no | UI tour | now |
| P8 | Replace: meshes + motions | Sonnet 5.5 high | no | NPC/char tour | now |
| P8b | The 4 `_INT.txt` particle / mesh-player definitions (new 2026-11-16) | Sonnet 5.5 high | no | effects tour | now |
| P9 | Loose: music, movie, eventscene | Sonnet 5.5 high | no | listen | now |
| P10 | `XRenderer.fx` (gates + play-test only) | Sonnet 5.5 high | no | render tour | now |
| P11 | (conditional) LargeAddressAware | Opus 5.5 high | yes | memory | now |
| P12 | Close-out census | Sonnet 5.5 medium | no (debug retire) | no | - |

Review cadence (x2-phase, 2026-11-10): the data waves (P2-P4, P6-P10) have no review of their own; P1 and P5 keep theirs. "Build: no" means
no `build.py` run: only P1 and P11 change the exe. A wave's play-test is the user's boot-and-walk against the deployed P1 exe.

## Phase P0 - Tooling, census, restore

**Run with:** Sonnet 5.5 high. No build (no client source changes; `tools/` and `june2017_re/` only).

1. **Census script** `june2017_re/asset_port_census.py`:
   - one row per June entry: bucket, June kom, March kom, name, size, ext, route;
   - buckets: `june-only`, `same`, `differs`, `march-unstaged`, `lua`, **`held`** (Rule 8: carries its reason and owner), and the
     Inbound-routes tag in a `route` column (`P4a`, `P7`, `P8b`, `held`, ...);
   - excluded types carry their route;
   - writes `june2017_re/p0_census.tsv` and `june2017_re/p0_waves.tsv` (the per-kom wave table for P2-P4 and P6-P8);
   - also writes `tools/data/asset_port_held.tsv` (Rule 8: the 7 W3f files and the 83 `Motion_<Class>*` files to start with);
   - the planning session's script compared June's exact extract with the staged March file by md5, through `kom.py live_names()`
     and `staged_names()`. Re-create it there.
   - **Expected counts have moved.** Reproduce "Established facts" as a check of the script (run it against the live state of
     2026-10-08 if a backup allows, or explain each bucket's delta), then record today's counts. The Done note explains every
     difference by plan (C3x, C3s, C3z-1 and its 306 removals, W3, K2's 501 files, the char ports) using `c3z1_stage_plan.tsv`,
     `c3z1_june_dropped.tsv`, `june_item_kim/k2_assets_staged.tsv` and the staging backups `kom_backup_stage_june\<stamp>`.
2. **Restore the March-unstaged files** (1,066 at planning time) into `kom_contents` byte-exact from the live V.0.3 koms, using
   `junekom.Kom` (it reads V.0.3 algo 0) or elstools extract.
   - **Already done (checked 2026-11-16):** the data118 file (C3s, 2026-10-10) and data101's dir (W3, 2026-11-12). `kom.py pack data101 data118`
     reads `+0 / -0 / ~0` against live (data101 2,181 entries / 37,906,027 B; data118 286 / 3,574,336 B). P0 re-runs that gate and records it.
     The census's `march-unstaged` count must then be 0.
   - These koms are not copied, because nothing changed.
3. **New `kom.py` subcommands and options.** Reuse `stage-june`'s June index and strict decode and the `inspect`/`entry_map`/`diff`
   helpers, and update `tools/README.md`.
   - **Held names:** `verify-staged`, `pack --wave` and `stage-june` read `tools/data/asset_port_held.tsv` and refuse a held name (Rule 8).
   - **Long names:** X4 (`SERV_IRUHADEV_KOM_LONG_NAMES`) is closed and defined, so `stage-june` stops refusing names over 59 characters
     (`--long-names` stays accepted and does nothing). Check the flag is in the deployed exe with `exestr.py` before removing the refusal.
   - `verify-staged <list|kom>`: gate (d). Seed it from `.build/x5_review.py`.
   - `pack ... --wave <list>` / `copy ... --wave <list>`: gates (a) and (b), with decode-and-compare for every unexplained `~changed`.
   - `dupcheck [--with dataNNN ...]`: gate (c) across every live `data*`/`datai*` kom, with packed replacements swapped in.
   - `copy-loose <list> --dest music|movie|eventscene`:
     - backs up to `kom_backup_tools\loose\<stamp>\`;
     - writes `.part`, then replaces;
     - verifies md5;
     - refuses while the game runs.
4. **Settle X5's data048 "~487 changed"** with gate (b) and record the result in the Done note. X5 cleared it on equal decompressed
   size only.

5. **Finish** (no build, no deploy, no kom copy): the Done note says "full build skipped: no play-test"; hygiene is `python tools/finish.py
   --files tools/kom.py june2017_re/asset_port_census.py --no-flag "tooling only: tools/ and june2017_re/, no client source"`, the STATUS row is updated in the same commit,
   and P1's kickoff is checked (it is written).

**Done gate:**
- census TSVs and `tools/data/asset_port_held.tsv` committed, today's counts recorded, every difference from "Established facts"
  explained by plan (the baseline numbers are no longer the target);
- the restore pack diff is `+0/-0/~0` for data101 and data118, and `march-unstaged` is 0;
- each new subcommand has been exercised on a scratch `X2_KOM_CONTENTS` and on data048, and the Done note names each test;
- `tools/README.md` and the `kom.py` docstring describe the new subcommands.

## Phase P0 - Done (2026-11-16, Sonnet 5.5 high; tools and data only, no client source, no build, no kom copy)

**Plan refresh first** (user: "this plan is kinda old"): this doc, `STATUS.md`, backlog Stage 15 and all 18 kickoffs were brought to the current loop
(see "Amended 2026-11-16"): copy needs no ask; Inbound-routes table; hold-out rule 8; tracking rule 9; **P2 rewritten** (243 of 258 June kom numbers are
live, only 15 lack an archive), **P3 retired**, **P8b added**, **P4 waves re-cut**. No flag, so `finish.py` read `SKIPPED no source file`: PASS.

**Census** (`june2017_re/asset_port_census.py`; 222,811 June entries, 258 koms; 10 min cold, 2.5 min cached):

| Bucket | Today | 2026-10-08 | Why it moved |
|---|---|---|---|
| june-only | 41,057 (2.95 GB) | 117,562 | 77,766 staged by other plans since (`p0_delta_summary.tsv`) |
| same bytes | 173,698 | 92,457 | + those 77,766 + 3,475 replaced |
| differs | 719 (P6 377, P7 213, P8 124, P8b 4, `.txt` 1) | 4,397 | 3,475 replaced by C3z-1 (365), W3, atlas, ... |
| held | 8 | - | Rule 8 (6 W3f `.y`, `Motion_ADD_LP.x`, `Motion_ADD_PT.X`) |
| march-unstaged | 0 | 1,066 | restore done (below) |
| lua | 7,329 | 7,329 | equal |

Rebuilt baseline from today's data: june-only ~118.8k (recorded 117.6k), differs ~4.2k (recorded 4.4k): the 1.2k / 0.2k residue is not attributed (the baseline count
is a rebuild from file mtimes, not a replay). Of the 81,241 files staged since 2026-10-09, 5,547 are named by a stage-plan TSV; 75,694 were staged without a `--plan-out` TSV.
Routes: P2 4,421 (15 new koms); P4a 9,413, P4b 12,748, P4c 14,467 (**re-cut**: 2026-10-08 ranges gave 4.2k / 5.2k / 27.2k files; now data001-150 /
151-185 / 186-226 + datai); P6-P8 714; P8b 4 (+ 8 `.txt` rows `txt-review`, routed to P8b; `DialogLog.txt` stays dropped); `june-item-kim` 1 (`item_LA.kim`).
Inbound tags found open: costume shop 48, GodBeast Fire 20, atlas 10, char-eve `_INT.txt` 4, june-engine 4. Files: `p0_census.tsv` (non-same rows),
`p0_waves.tsv`, `p0_summary.tsv`, `p0_delta_summary.tsv`, `tools/data/asset_port_held.tsv` (90 rows: 7 W3f, 83 class motions = the 86 in the R12b backup minus the 4 June's already + data010 `.xet`).

**Restore** (step 2): already done (data118 by C3s 2026-10-10, data101 by W3 2026-11-12). Re-run today: `kom.py pack data101 data118` = `+0 / -0 / ~0`
(data101 2,181 entries 37,906,027 B; data118 286 entries 3,574,336 B); census `march-unstaged` 0. Nothing copied.

**`kom.py`** (docstring and `tools/README.md` updated): `verify-staged`, `pack|copy --wave`, `dupcheck`, `copy-loose`; `stage-june` refuses held names and no longer refuses
names over 59 characters (X4 proven: `EXESTR OK` `[KOM_LONG] found` + `[KOM_LONG]` in the deployed exe md5 45148348). Tests (scratch `X2_GAME_DIR` /
`X2_KOM_CONTENTS` under `.build/p0_scratch`; real tree read-only):
- `stage-june` held name -> REFUSED; 61-character name stages; `verify-staged` list OK (2 names), tampered staged byte -> `STAGED != JUNE`, exit 4;
- `pack --wave`: +2 added on the list -> `WAVE GATE OK`; held name on the list -> exit 4; an added file off the list -> exit 4; a `~changed` entry off the list
  is decoded from the live kom and byte-compared: tampered file -> `1 unexplained`, exit 4; the same file on the list -> OK;
- `copy --wave`: refuses the off-list case (nothing copied), copies the good wave: `COPY OK` md5 3e64f0bc (scratch), backup written;
- `dupcheck`: copy of data118 as data119 -> 286 duplicates, exit 4; real tree `--with data048 data101` -> `DUPCHECK OK`; default mode finds 3 koms whose packed file differs from live (data003, 004, 049: peers' pending work);
- `copy-loose` (scratch music): dry run, 2 new, again = 2 identical, tampered file replaced with a backup under `kom_backup_tools/loose/<stamp>/music`, held name refused.
  A test found and fixed one bug (undefined `LOOSE_ROOT`). Not exercised: the game-running refusal (same `running()` as `copy`; never launch the game).

**data048 "~487 changed"** (X5 cleared it on equal decompressed size only): gate (b) run against the oldest surviving backup `data048.kom.20261015_194144` (`copy` keeps 5 backups, so it is
not proven to be the original live kom): **+106 added, -0 removed, ~3 changed, 0 recompression-only, 3 unexplained**. The 3 are
`ColorBallRed2.dds`, `GuardianStrike01.X`, `HoldCircle.dds`, real byte replacements staged 2026-10-16 20:49 (`stage_plan.tsv`, i.e. June's bytes); the +106 are other plans' staging. So
the "~487" do not occur against any kom this tool can still see (they were comp-size drift of an older pack); live data048 now packs to `+0/-0/~0` (20,913,578 B).
`verify-staged data048` = 692 names, 1 problem: `HQ_CompanyLogo.dds`, March's own copy waiting for P6 (kom mode lists those; a wave list must show 0).

**Open for the user:** none blocking. Notes: C3z-1b's STATUS row still says it waits for P0's data101 restore; that is done (not my row). The 8 `.txt` rows ride P8b with a key pre-check.
Next: `/x2-phase june-asset-port P1` on Sonnet 5.5 high (builds the exe, hands off a boot-to-village play-test). P5 can start alongside on Opus 5.5 high.

## Phase P1 - Mount diagnostic and baseline

**Run with:** Sonnet 5.5 high. Build + deploy.

1. **Flag `SERV_IRUHADEV_KOM_MOUNT_DEBUG`** (short-lived) in `KTDXLIB/Always.h`, code in `KTDXLIB/KGCMassFileManager.cpp` (CP949,
   `bytepatch.py`).
   - **In `AddMassFile`,** record per kom: the kom name, member count, registered count (inserts that succeeded) and mount
     milliseconds. After the last mount, also record the process `PrivateUsage` (`GetProcessMemoryInfo`).
   - **On the first `LoadDataFile` call after mounting** (use-time, so it reaches ErrorLog), log:
     - one `[KOM_MOUNT] <kom> files=<n> registered=<n> ms=<n>` line per kom;
     - one `[KOM_MOUNT] total koms=<n> entries=<n> ms=<n> private=<MB>` line.
   - Inserts only; nothing per frame.
2. **A comparator** (a `logs.py` option or a small script): for every kom, `files == elstools inspect file_count` and
   `registered == files`. A kom that was dropped or never mounted is an absent line.
3. **Baseline play-test:** boot to a village and walk. Record the total mount milliseconds and private MB in the Done note, with the
   number of koms mounted (243 at 2026-11-16, not the 2026-10-08 count). Hand-off: copy-ready, with the `exestr.py` line for the
   `[KOM_MOUNT]` strings beside `DEPLOY OK`. A full build is right here: P1 changes the exe and hands off a play-test.
4. **Finish:** `python tools/finish.py --files KTDXLIB/KGCMassFileManager.cpp KTDXLIB/Always.h` (flag `SERV_IRUHADEV_KOM_MOUNT_DEBUG`), then
   `--range <c>^..<c>` after the commit. The debug define stays on through P2-P10 (every wave's log check reads `[KOM_MOUNT]`); the P1 review
   does not retire it, and P12 does.

**Done gate:** a `[KOM_MOUNT]` line for every live kom; the comparator is clean; the baseline numbers are recorded.

## Phase P1 - Done (2026-11-16, Sonnet 5.5 high; flag `SERV_IRUHADEV_KOM_MOUNT_DEBUG`, on; built, deployed and play-tested; review next)

**What landed.** `KTDXLIB/KGCMassFileManager.cpp` (CP949, `bytepatch.py`, 112 inserted lines, no deletion), `KTDXLIB/KTDXApp.cpp` (11 lines) and `KTDXLIB/Always.h` (8 lines):
- `AddMassFile` starts a QueryPerformanceCounter timer, counts the `m_mapMassFile.insert` calls that succeed (both the long-name and the original branch),
  and, only at the successful end, pushes `{kom base name, GetFileCount(), registered, mount ms}` into a file-local vector. A kom that returns NULL earlier (load
  failure, or the duplicate path that deletes the kom) never gets there, so it is **absent** from the log, not logged as mounted.
- `LockMassFileMap` (called once after the last mount, `X2Main.cpp:1477`) samples `PrivateUsage` (`GetProcessMemoryInfo`; `Psapi.lib` is already linked by `KTDXCommonFunc.h`).
- **Trigger (corrected after the first play-test).** The first build logged from the first `LoadDataFile` after `LockMassFileMap`; the user's run showed 0 lines in a
  1,143-line ErrorLog (ErrorLog.txt only keeps lines from later in startup, as X4 found). The fix moves the trigger to the first frame: `CKTDXApp::OnFrameMove` calls
  `KomMountOnFirstFrame()` once (beside `KTDGFXRuntime_FirstFrameLog`, whose line does survive); `LogKomMountOnce()` returns until then. It writes one
  `[KOM_MOUNT] <kom>.kom files=N registered=N ms=N` line per kom, then `[KOM_MOUNT] total koms=N entries=N ms=N private=<MB>`, through `KLOG("ErrorLog.txt")`.
  The `LoadDataFile` call stays (one volatile read, gated until the first frame). Nothing else per frame: one `static bool` test in `OnFrameMove`.
- Names: helpers and structs sit in a named `_CONST_KOMMOUNT_P1_` namespace plus `using namespace`; no anonymous namespace, no `June`, no phase tag in an identifier.
- **Comparator:** `python tools/logs.py --kom-mount` (new): every live kom (kom.py's index of `elstools inspect`) has a line, `files == inspect`, `registered == files`,
  the total line agrees; prints `KOM_MOUNT CHECK: PASS|FAIL` and the baseline numbers. Tested on a synthetic log: all-good PASS; a missing kom, a short `registered`
  and a wrong total each FAIL.

**Gates.** `finish.py --files KTDXLIB/KGCMassFileManager.cpp KTDXLIB/Always.h --with KOM_MOUNT_DEBUG`: flagcheck PASS (`files: 1 PASS, 0 DEFINE.. 1 OTHER` = the other
flags' lines in Always.h, none mine), junediverge PASS, enumscan PASS, 0 deleted / 0 non-ASCII / 0 EOL-mismatched inserted lines.
First build: `DEPLOY OK: X2_offline.exe is 27,199,488 B, dated 2026-11-16 15:03:03, md5 b525d2f18f70b977cf430339d5627662` (the one with the too-early trigger).
Fix build: my `build.py --deploy` **COALESCED** with another session's full build (started 15:19:21, after my edits, succeeded); that run deployed the exe: X2_offline.exe 27,204,608 B,
dated 2026-11-16 15:20:03, md5 f87b25c6abac0bdbb76342573579294e = X2/US_SERVICE/x2.exe. My own deploy step was refused only because the game was running again.
Proof the fix is in it: `X2.map` lists `KomMountOnFirstFrame`, and the play-test below logged from the first frame.
`EXESTR OK: 3 of 3 found in X2_offline.exe` md5 f87b25c6 (`[KOM_MOUNT] `, `total koms=`, ` registered=`); the first exe had 5 of 5.
After the commit (`5866cb7d`): `finish.py --range 5866cb7d^..5866cb7d --with KOM_MOUNT_DEBUG` = flagcheck PASS (`1 PASS, 1 DEFINE, 0 OTHER, 0 FAIL`), junediverge PASS, enumscan PASS.
Fix commit (`2292741d`): `finish.py --range 2292741d^..2292741d --with KOM_MOUNT_DEBUG` = flagcheck PASS (`2 PASS, 0 DEFINE, 0 OTHER, 0 FAIL`), junediverge PASS, enumscan PASS.

**Play-test and baseline (2026-11-16 15:20 run, exe f87b25c6).** `python tools/logs.py --kom-mount`: **243 live koms, 243 `[KOM_MOUNT]` lines, `KOM_MOUNT CHECK: PASS`** (every kom `files == inspect`,
`registered == files`; no kom absent). Total line: **koms=243 entries=182,446 mount_ms=1,158 private_MB=167** (private memory sampled right after the last mount). 0 UNHANDLED, 0 EXCEPTION,
0 unbound Lua in that run. The first run (exe b525d2f1, 15:04) booted to the village and walked about a minute with no crash or MessageBox (server log clean exit); it only lacked the lines.
These are P2's numbers to compare against; P11 triggers on private memory up about 300 MB or past about 1.4 GB, read from this line after each wave.

**Still open:** nothing (reviewed below). The debug define stays on through P2-P10; P12 retires it.

## Phase P1 - Review (2026-11-16, Sonnet 5.5 high; commits `5866cb7d`, `2292741d`; verdict: PASS, no code fix, no build)

Read `KGCMassFileManager.cpp` `AddMassFile` (`:546-719`), `LockMassFileMap`, the `LoadDataFile` hook (`:1019`) and the `KTDXApp.cpp` hook.
- **Counts at the real insert:** `registered` is bumped right after the `#ifdef SERV_IRUHADEV_KOM_LONG_NAMES` / `#else` pair (`:620-623`), so it reads the one `ib` both
  branches define; a long-name duplicate (`ib.second == false`) is not counted. An `INT_KOM_DUPLICATE` `break` (not defined here) would leave `registered < files`, which the comparator flags.
- **Cost and once-only:** the vector is filled only inside `AddMassFile`, which refuses after `LockMassFileMap` (`:553`), so it is immutable by the first frame. `OnFrameMove` pays one
  `static bool` test; `LogKomMountOnce` is an `InterlockedExchange` latch, so two threads cannot both write. The `LoadDataFile` hook is two volatile reads per lookup (no log, no allocation); since the
  first-frame move it is redundant, harmless, left in (its comment is stale: it now only fires after the first frame).
- **psapi:** `KTDXCommonFunc.h:4` has `#pragma comment(lib,"Psapi.lib")`; the exe linked and logged `private=167`, so `GetProcessMemoryInfo` resolves in `US_SERVICE`.
- **Deleted duplicate kom is absent:** `NOT_ALLOW_DUPLICATE_FILES` is defined (`Always.h:3245`), so the duplicate branch does `delete pMassFile; return NULL` (`:668-691`) before the
  record push (`:700`): no `[KOM_MOUNT]` line, and `logs.py --kom-mount` fails on the missing kom. (March's own bug stays: `m_vecMassFile` keeps the deleted pointer; not P1's.)
- **Namespace:** `_CONST_KOMMOUNT_P1_` plus `using namespace`; no anonymous namespace, no `June` or address in a name.
- **Nit, not fixed:** the `Always.h` define block still says "first LoadDataFile after LockMassFileMap". A comment-only edit there would rebuild the whole PCH for nothing; fix it when P12 retires the define.
- **Gates:** `finish.py --review --commits 5866cb7d 2292741d --with KOM_MOUNT_DEBUG -v`: flagcheck PASS (`3 PASS, 1 DEFINE, 0 OTHER, 0 FAIL`; 0 deleted, 0 non-ASCII, 0 EOL-mismatched
  inserted lines; the one DEFINE row is the +8 `Always.h` block), `junediverge` PASS, `junediverge --check-all` PASS, `enumscan` PASS (7 gates, 0 failed, 3 skipped as not applicable).
  Every OTHER row: none. Every deleted line: none.
- **Comparator re-run on the current log:** `logs.py --kom-mount` = 243 koms, 243 lines, `KOM_MOUNT CHECK: PASS`, `total koms=243 entries=182446 mount_ms=861 private_MB=167`
  (the 15:24 run; the baseline run read 1,158 ms: mount time varies with disk cache, memory does not).
- **Build:** full build skipped: no code change, no play-test. The deployed exe stays f87b25c6 (P1's fix build). `P2.md` checked: its dependency, flag note and gates are current.

## Phase P2 - New kom numbers (rewritten 2026-11-16; was "Pilot: 5 new kom numbers" plus P3's 61)

**Run with:** Sonnet 5.5 high.

**What changed.** At planning time March had no archive for 66 June kom numbers. Other plans have since copied most of them
(data152, 179, 214, 224 and datai027 between 2026-10-10 and 11-07; data135 / 136 / 140 / 141 / 146 / 166 / 180 / 182 / 216, ...): **243 of
June's 258 kom numbers are live.** Fifteen have no archive (checked 2026-11-16, `ls` of the game dir against June's):
data169, 170, 172, 177, 187, 193, 196, 209, 210, 213, 219, 220, 221, datai026, datai032. P0's census gives their file counts and sizes
and is the truth. The "pilot" role is gone: P1's baseline already measures a client with 243 koms, so P2 is simply the new-archive wave.

- **Wave:** every June kom number with no live archive, per `p0_waves.tsv` (split into two waves only if the census shows one is
  lopsided, and say so in the Done note). Their inert Scaleform files go in too.
- **Steps, in order:**
  1. `stage-june` (each kom reads `NEW kom dir`);
  2. `verify-staged`;
  3. `pack`: the diff says "no live kom, new archive", and the entry count equals the wave count;
  4. `dupcheck`;
  5. `copy --wave` (no ask), then check md5.
- **Play-test (boot and walk).** All of these must hold:
  - `[KOM_MOUNT]` lists the new koms with `registered == files`;
  - no MessageBox appeared;
  - `existfileinfo.txt` is unchanged;
  - 0 KEM_ERROR.
- **Measurements:** compare mount milliseconds and private MB with P1's baseline, and record them again after each P4 wave. If private
  memory rose by more than about 300 MB, or the total passes about 1.4 GB, schedule P11 before the next wave.

## Phase P2 - Done (2026-11-16, Sonnet 5.5 high; data only, no client source; play-test owed)

**What landed.** The 15 June kom numbers with no live archive are now live, byte-exact: **4,421 files in 15 new koms** (`june2017_re/P2_stage_plan.tsv`; one wave, the census showed no lopsided split).
Wave list from `p0_census.tsv` (june-only, june kom in the 15): 4,422 names; 1 left out (below). Counts per kom: data169 999, data170 62, data172 550, data177 820, data187 549, data193 293, data196 23 (fonts,
92.7 MB), data209 43 (Ain motions), data210 947 (Ain / Rose voices), data213 84 (Ain tutorial), data219 / 220 / 221 1 each (`dummy2NN.X`), datai026 15, datai032 33 (Rose cash meshes).
- **Left out:** `Npc_All_Ariel_Face_Talk_Dark.txt` (datai026): 0 bytes in June, and `elstools pack` fails its verify on an empty entry ("entry set differs from the dir"). Its census route is
  `txt-review` = **P8b** (kickoff note added): P8b must decide how an empty `.txt` ships (likely: leave it out, nothing reads an empty file). I removed it from the new `datai026_kom` dir, its manifest and the plan TSV.
- **Held names:** none in the wave (`verify-staged` and `stage-june` apply `asset_port_held.tsv`).

**Gates, in order.**
1. `stage-june`: `STAGE-JUNE OK: 15 kom(s), 4422 staged (4422 new, 0 replaced, 0 identical), 0 refused` (dry run first, same numbers). 0 replaced, so no staged-tree backup was needed.
2. `verify-staged` (after the removal): `VERIFY-STAGED OK: 4421 name(s) equal June's strict decode, 0 problems`.
3. `pack --wave`: `PACK OK` + `WAVE GATE OK` for all 15; every kom `+N added (N on the list), -0 removed, ~0 changed, no live kom, new archive`; entry counts equal the wave counts.
4. `dupcheck --with <15>`: `DUPCHECK OK: 0 duplicate names` (186,867 names in the would-be-live set), repeated inside each `copy`.
5. `copy --wave`: `COPY OK` x15 (game not running; md5 / entry count verified by the tool): data169 43,330,403 B 999 entries md5 6d1d49ec; data170 2,866,976 B 62 fa91d621;
   data172 22,467,380 B 550 58c5c456; data177 41,383,770 B 820 44b60b73; data187 15,214,767 B 549 f3f85d65; data193 5,832,107 B 293 c140c42c; data196 92,685,127 B 23 da28a646;
   data209 55,823,478 B 43 32ad5079; data210 50,572,130 B 947 025fb4c8; data213 8,473,660 B 84 97455b85; data219 / 220 / 221 128,248 B 1 each (51cd1f65 / d2e6c58d / f9fe2003);
   datai026 2,114,295 B 15 b0e06bd3; datai032 715,091 B 33 08990fc1.
6. Census re-run (`asset_port_census.py`): `June kom numbers with no live archive: 0`; `live-only March entries with no file in kom_contents: 0`; P4a 9,413 / P4b 12,748 / P4c 14,467 june-only rows left; `p0_*.tsv` regenerated.

**Build:** no build: data only (the exe stays P1's f87b25c6). `finish.py` not run: no client source changed. `MODS.md` entry added (Rule 9).

**Baseline to compare after the play-test** (P1, 243 koms): koms=243 entries=182,446 mount_ms=1,158 (861 on the warm re-run) private_MB=167. Expected now: koms=258 entries=186,867.
**P11 decision:** open until the user's run reports `private_MB`. P11 triggers on private up about 300 MB (> ~467 MB) or past ~1.4 GB. The wave adds 4,421 entries and 0.3 GB of archive on disk, but a mount only
reads each kom's directory (167 MB for 243 koms), so a large rise is not expected; P4 is not blocked on P11 unless the line says otherwise.
**Not exercised by anything yet:** nothing in March names these files (Ain / Rose voices and motions wait for their class ports; `Photo_*.Y` for the photo feature; data196's fonts are June's CN / KR fonts); no visible change is expected.

**Play-test result (2026-11-16 15:48 run, exe f87b25c6, 258 mounted):** `logs.py --kom-mount`: **258 live koms, 258 `[KOM_MOUNT]` lines, `KOM_MOUNT CHECK: PASS`**; total line `koms=258 entries=186867 mount_ms=963 private_MB=169`
(baseline 243 koms, 1,158 ms, 167 MB: **+2 MB private**, mount time within cache noise). 0 KEM_ERROR in `ErrorLog.txt`; `existfileinfo.txt` unchanged (414 B, md5 64e18d8b); clean exit, 0 EXCEPTION, 0 unbound Lua.
The 2 `UNHANDLED E_DISABLE_HB_CHECK_REQ` packets are not P2's (a heartbeat packet the emulator never answers; no effect seen).
**P11 decision: not needed before P4** (+2 MB, far below the +300 MB / 1.4 GB triggers). Re-measure after each P4 wave.

## Phase P3 - Retired (merged into P2, 2026-11-16)

Not a phase any more: its 61 "remaining new kom numbers" were mostly copied by other plans (see P2), and what is left is in P2's wave.
`kickoffs/june-asset-port/P3.md` is a stub that points here. Its one extra, the `RidingPet_GodBeast_Fire` adoption (June's script plus
15 assets in data140 / data141, which are live now), moved to **P4's wave that carries those files**; the census row's `route` column
names it.

## Phase P4 - June-only into existing March koms (P4a / P4b / P4c)

**Run with:** Sonnet 5.5 medium, one session per wave.

- **Scope (2026-10-08 baseline: 151 koms, 83,692 files, about 5 GB raw):** every June-only file whose kom is live, per P0's census.
  Other plans have already staged part of it (the world files C3x / C3s / C3y / C3z-1, K2's 501 item assets, the char ports'
  class assets, the atlas textures), so the real count is lower; and since 243 of 258 koms are live, this phase also carries the
  koms the old P2 / P3 pilot would have created (data152, 179, 214, 224, datai027, ...) as "the rest of June's files".
- **Waves (re-cut by P0, 2026-11-16, by file count; the 2026-10-08 cut at data075 / data150 gave 4.2k / 5.2k / 27.2k files):**
  - P4a `data001-150`: 150 koms, 9,418 files, 0.96 GB;
  - P4b `data151-185`: 31 koms, 12,748 files, 0.66 GB;
  - P4c `data186-226` plus `datai`: 62 koms, 14,469 files, 0.87 GB.
  - `p0_waves.tsv` is the truth (constants `P4A_LAST` / `P4B_LAST` in the census script); a later census can move a cut, and the Done note says so.
- **Routed files ride their wave:** the `route` column of the census names them (Inbound routes: the june-engine `.fx` and `Cartoon*.bmp`
  in P4a, `PerturbationMap.png` / `13Gang_Protect.tga`, the 3 Ruben textures, the costume-shop withheld list, `Village_ADBoard.dds`).
  **`RidingPet_GodBeast_Fire` (moved here from the old P3):** the wave that carries data140 / data141's 15 files is followed by
  `python tools/luasrc.py adopt-june data037 RidingPet_GodBeast_Fire.lua` (`--dry-run` first; June's own bytecode via ljconv, no
  `kom.py compile`), then pack and copy data037 (`/elstools`, `/x2-kom-packed`), and one riding-pet line in the hand-off.
- **All four gates apply,** because every kom is a repack of a live archive. Gate (b) catches staged dirs that drifted from live.
- **data036 is shared** with the lua ports, the item.kim work and the 12 event-scene `.xml`. Pack it only when no peer session holds
  an uncommitted `data036_kom` change (STATUS Touches).
- **Inert Scaleform** files go in the same waves.
- **Play-test per wave:** boot and walk one village, one field and one dungeon whose koms are in the wave. P2's gate applies.

## Phase P4a - Done (2026-11-16, Sonnet 5.5 medium; data only, no client source; play-test owed)

**What landed.** June-only files in data001-150: **9,429 files in 92 existing koms** (`june2017_re/P4a_stage_plan.tsv`, `MODS.md` entry). Wave list = 9,411 census rows (route P4a, size > 0) + 18 earlier-staged June files that were in the staging dirs but never live (data003 x3, data004 x12, data049 x3; each `verify-staged` OK).
- **Left out:** `Velder_Field_001.dds`, `Velder_Field_003.dds` (data101), 0 bytes in June (same pack limit as P2). Not held names.
- **Routes ticked:** june-engine `XEnvBoxRenderer.fx`, `XHeatShimmerRenderer.fx`, `XRenderer_X2TOOL.fx` (data041), `13Gang_Protect.tga` (data045); GodBeast Fire files (data140 / 141); costume-shop rows.

**Gates.** 1 `stage-june`: 92 koms, 9,411 staged (all new, 0 replaced), 0 refused. 2 `verify-staged`: 9,411 then 15 + 3 OK, 0 problems. 3 `pack --wave` x92: `PACK OK` + `WAVE GATE OK`, every kom `-0 removed`, 0 unexplained `~changed` (data049: 76 recompression only). 4 `dupcheck`: `DUPCHECK OK: 0 duplicate names` (196,296 names), repeated after the copy. 5 `copy`: 92 koms live, 55 `COPY OK` this pass + 37 earlier (25 of them were identical already).
**Tool bug (not fixed):** `kom.py copy <many>` stops after the first kom that is "already identical"; copy one kom per call. Peers' stale stagings show up as wave-gate "added entries not on the list": add them to the list after `verify-staged`.
**GodBeast Fire:** `adopt-june data037 RidingPet_GodBeast_Fire.lua` OK (`june_adopt_20261116_162132.tsv`), `pack data037` PACK OK (~1 changed). **data037 COPY OK** (14,435,865 B, 3,001 entries, md5 8c5ecf3a) after the game closed.
**Census re-run:** June kom numbers with no live archive 0; P4a rows left 2 (the zero-byte files); P4b 12,748, P4c 14,467 unchanged.
**Build:** no build: data only (exe is P1's). `finish.py` not run: no client source changed.

## Phase P4b - Done (2026-11-17, Sonnet 5.5 medium; data only, no client source; play-test owed)

**What landed.** June-only files in data151-185: **12,748 files in 21 existing koms** (`june2017_re/P4b_stage_plan.tsv`, `MODS.md` entry). Wave list = the 12,748 census rows with `route=P4b` (none zero-byte, none held); list file `.build/P4b_wave.tsv`.

**Gates.** 1 `stage-june`: 21 koms, 12,748 new, 0 replaced, 0 refused. 2 `verify-staged`: 12,748 equal June's strict decode, 0 problems. 3 `pack --wave` x21: every kom `PACK OK` + wave gate, `-0 removed`, `~0 changed`, 0 unexplained, `+added` equals the list count per kom. 4 `dupcheck --with` the 21: `DUPCHECK OK: 0 duplicate names` (209,053 names). 5 `copy --wave` x21 (one per call): 21 `COPY OK` (e.g. data185 72,214,338 B, 4,314 entries, md5 82aea178; data179 261,866,184 B, 3,952 entries, md5 91526e38).
**Census re-run:** June kom numbers with no live archive 0; live-only entries with no file 0; P4b june-only rows left 0; P4c 14,470 (was 14,467: peers' later stagings moved a few names).
**Build:** no build: data only (exe is P1's). `finish.py` not run: no client source changed.
**Routes:** none of the Inbound routes land in P4b.
**Play-test 2026-11-17 (logs):** 258 / 258 `[KOM_MOUNT]` lines, 209,053 entries, 1,138 ms, 175 MB private, `KOM_MOUNT CHECK: PASS`; 0 KEM_ERROR, 0 Motion Error, `existfileinfo.txt` unchanged (Sep 22). Ain-creation and Gunner checks dropped: those characters are not ported yet. P4c next.

## Phase P4c - Done (2026-11-17, Sonnet 5.5 medium; data only, no client source; play-test owed)

**What landed.** June-only files in data186-226 plus datai: **14,467 files in 33 existing koms** (`june2017_re/P4c_stage_plan.tsv`, `MODS.md` entry). Wave list = the 14,467 census rows with `route=P4c` (none zero-byte, none held); list file `.build/P4c_wave.tsv`. The cut held: 14,467 rows, as planned.

**Gates.** 1 `stage-june`: 33 koms, 14,467 new, 0 replaced, 0 refused. 2 `verify-staged`: 14,467 equal June's strict decode, 0 problems. 3 `pack --wave` x33: 33 `PACK OK` + 33 `WAVE GATE OK`, `-0 removed`, `~0 changed`, 0 unexplained, `+added` equals the list count per kom. 4 `dupcheck --with` the 33: `DUPCHECK OK: 0 duplicate names` (223,520 names). 5 `copy --wave` x33 (one per call): 33 `COPY OK` (e.g. data211 44,454,558 B, 2,503 entries, md5 0c0d78aa; data198 12,096,039 B, 5,356 entries, md5 99002c5d; datai023 21,717,148 B, 10,911 entries, md5 573100cb).
**Census re-run:** June kom numbers with no live archive 0; live-only entries with no file 0; P4c june-only rows left 0 (June-only rows left are routed to later phases: lua-port 7,329, held 8, txt-review 8, june-item-kim 1).
**Build:** no build: data only (exe is P1's). `finish.py` not run: no client source changed.
**Routes:** the 37 costume-shop withheld files rode this wave; `Village_ADBoard_1/2/3.dds` (data216) landed. No GodBeast files here.

**Play-test:** owed (see hand-off). Next: P5 / P6 per STATUS.

## Phase P5 - Collision key forecast

**Run with:** Opus 5.5 high. No build. Opus phase: delegate the mechanical reads and scans (census greps, loader parsers' test runs,
reference-index builds) to Sonnet subagents as `x2-phase` describes; keep the verdicts and the owner decisions.

Nothing is held *by this forecast* (the Rule 8 hold-outs are excluded from the lists before it runs; every other collision is replaced).
P5 predicts which March content will need follow-up work once June's files replace March's, so each owner knows before
P6-P8 land.

1. **`june2017_re/asset_keys.py`**, with parsers written from March's own loaders:

   | Format | Parse | Source |
   |---|---|---|
   | TET | keys | reuse `june2017_re/buff_icon_check.py` `tet_keys` |
   | XET | keys | `KTDXDeviceXET_Preprocessing.h` |
   | KSM/KME | animation-set and frame names | `KTDXDeviceXSkinMesh.*`, `KTDXDeviceXMesh.*` |
   | xof | frame and animation-set names, including MSZIP `bzip` | - |

   Start from `june2017_re/c9g_animation_name_census.py` `motion_names`.
2. **March reference index:**
   - string constants of every live March lua, from the bytecode constant scan (XOR'd where needed, memory `client-lua-xor-key`);
   - March particle and mesh-player `.txt` keys;
   - C++ string literals in `X2Lib`/`KTDXLIB`;
   - texture names embedded in meshes and XETs (`june2017_re/fxdata.py` `embedded_textures`).
3. **Output** `june2017_re/p5_collision_forecast.tsv`, one row per colliding file. Columns:
   - verdict: `clean` or `will-break`;
   - the keys June lacks;
   - the March readers;
   - the owner (Rule 6).
   - Each `will-break` row becomes a row in its owner's doc in the same commit.
   - A pixel redraw at an unchanged key is `clean` (memory `june-asset-wins-over-march-at-same-filename`).
   - `AutoSetupGameOptionWeightConfig.xml` is forecast against its C++ reader, and is replaced anyway.
4. **Inputs the other plans already computed** (read before building new scanners; Inbound routes lists them): the atlas plan's 27
   size differences and 4 TET pairs (`atlas_a11_sizes.tsv`), `world_asset_collide.tsv` / `c3z1_world_asset_collide.tsv` (world meshes),
   `npc_mesh_census.py` output (the NPC "Motion Error" mesh collisions, memory `npc-motion-error-mesh-collisions`), and
   `june2017_re/r12b_anim_loss.tsv` (class motions). A row whose answer is already in one of them cites it instead of re-deriving it.
5. **Each wave list** (`p6_wave.tsv` / `p7_wave.tsv` / `p8_wave.tsv`) is built from the live census, not the 2026-10-08 one: held names
   out, files a peer staged since (`same`) out, and pairs together.

**Done gate:** every colliding non-`.txt` file has a row, and every `will-break` row has been handed to its owner.

## Phase P6 / P7 / P8 - Replace waves

**Run with:** Sonnet 5.5 high. Depends on P4 (June-only files first) and P5.

| Wave | Files |
|---|---|
| P6 | `.dds`/`.tga`/`.bmp` with no `.TET` change, plus the 25 `.ogg` |
| P7 | `.TET`/`.XET` with their paired textures |
| P8 | `.x`/`.y` meshes and motions, paired |

Each wave runs `stage-june <wave list>`, then the four gates, then `copy --wave` (no ask), then play-test, then the census re-run and the
`MODS.md` entry (Rule 9). **Re-run the census first:** since 2026-10-08 other plans replaced files of their own (the dungeon and village
plans' world files, the char ports' class textures, the atlas pairs), so a wave list from P5 that is older than a day is checked against
`p0_census.tsv` before staging. **A held name is never on a list** (Rule 8); `Motion_<Class>*` stays with the class ports.

**Hand-off:**
- Pick real places from the wave list: a village atlas key, a boss portrait, an NPC whose motion changed. Look for June's art and
  motion, and no KEM_ERROR for those names.
- Also visit one or two P5 `will-break` items, to confirm the forecast.

**A break on a replaced file is not reverted.** June's file stays, and the break gets a row with its owner, or a new phase if there
is no owner. The backup in `kom_backup_tools` exists only for a revert the user explicitly asks for.

## Phase P8b - The four `_INT.txt` definitions (new, 2026-11-16)

**Run with:** Sonnet 5.5 high. No build. Depends on P4c (their koms are repacked once by then) and P0; independent of P5-P8.

Added by char-eve CH0 (2026-10-26; backlog Stage 15). june-effect is closed, and the other 75 particle and mesh-player definition files are
June's bytes already. These four are still March's, in datai015, and every character and the UI share them: `UIMajorParticle_INT`,
`GameMajorParticle_INT`, `UIMajorXMeshPlayer_INT`, `GameMajorXMeshPlayer_INT` (June's are bigger). The black dimming layer (backlog l.304)
needs June's `GameMajorParticle_INT`.

1. **Pre-check (Rule 6):** March aborts a `.txt` at its first unknown key. Parse each of June's four with March's own parser (the keys
   june-effect ported are in `MODS.md`), and list any key March's parser does not know. An unknown key is a finding for its owner, not a
   reason to hold the file (Rule 6, user 2026-10-08), but say so before the copy so the play-test knows what to look for.
2. Stage the four byte-exact (`stage-june`), the gates, `copy --wave`. Evidence: `june2017_re/char_eve/ch0_effect_closure.tsv`.
   **Also the 7 other `.txt` rows** the 2026-11-16 census marks `route=txt-review` (june-effect, their old owner, is closed): `GameOptionPreset.txt`
   (differs), `UnitTitle1300.txt`, `UnitTitle1310.txt`, `fontconfig.txt`, `AIN_POWER_OF_GOD_06_1.txt`, `Npc_All_Ariel_Face_Talk_Dark.txt`,
   `NUI_ERIS_BOSS_Map03_A.txt`. Each goes through the same key pre-check (step 1) and rides this wave. `DialogLog.txt` stays dropped (June's own log).
3. **Play-test:** the village, one dungeon with a boss and a skill-heavy character: particles and the UI's particle layers still draw;
   no `[PARTICLE]` / KEM error for the four names.

**Done gate:** the four files are June's bytes in live datai015; the census shows their `.txt` rows `same`.

## Phase P9 - Loose files

**Run with:** Sonnet 5.5 high.

Run `copy-loose`, with backups (no ask; it refuses while the game runs). The counts below are the 2026-10-08 baseline: C3z-1 already
copied 17 music files, so `copy-loose` skips those as identical, and the census's loose rows give today's numbers:
- `music\`: 79 new, and 34 differing (replaced), including the 6 BGM tracks the C3y worlds name (Inbound routes);
- `movie\`: 32 `.avi`, and the `<movie>.lua` June's movie open reads (june-engine) if the census shows them in a kom wave;
- `eventscene\`: 2,383 `.swf` (inert).

**Check:** a field whose BGM differs, such as Hamel `field_hamel001.ogg`, plays June's track, and one of the 6 world tracks (for example
`Music072_Stage`) plays in its world. No error is logged.

## Phase P10 - XRenderer.fx

**Run with:** Sonnet 5.5 high.

- **Why its own phase:** this is the global shader.
- **Pre-check:** March's 38 looked-up names (`KTDGXRenderer.cpp:16-105`) all exist in June's file.
- **Stage and copy** data041, with only this change, no ask (already done by june-engine R2: see the note below).
- **Play-test:** a village, a dungeon and the character select. Look for:
  - no `KEM_ERROR50`;
  - no black or untextured meshes.
- **Revert:** if the screen breaks, revert data041 from its backup (the one revert this plan allows without asking, because a broken
  renderer blocks every other play-test), then open a Ghidra phase.
- **Already shipped:** the June-only `.fx` files and the `Cartoon*.bmp` ramps go in P4, inert.
- **Waits for june-engine R2** *(2026-10-16, Stage 2 planning)*: June compiles `XRenderer.fx` with `D3DXMACRO`s and sets 14 parameters
  March does not, so the pre-check above passes and the result is still not June's render. P10 copies after R2's review
  (`JUNE_ENGINE_REMAINDER_PHASES.md` M1). Also: June ships 3 June-only `.fx` (`XEnvBoxRenderer`, `XHeatShimmerRenderer`,
  `XRenderer_X2TOOL`, all data041), which the 2026-10-08 census did not count; P0's census should show them (P4a, inert).

## Phase P11 - LargeAddressAware (conditional)

**Run with:** Opus 5.5 high. Runs only if the P2/P4 memory numbers call for it.

- **Proposal:** `LargeAddressAware=true` for `X2/X2_2010.vcxproj` `US_SERVICE`, matching June's exe.
- **Why it needs the user:** it is a linker setting, so it cannot sit behind a source flag. **Propose it to the user first** (the one
  step in this plan that still asks). `X2/X2_2010.vcxproj` has no `LargeAddressAware` today (checked 2026-11-16). Runs the full build and
  deploy, then a play-test that records private MB against P1's baseline.

## Phase P12 - Close-out

**Run with:** Sonnet 5.5 medium.

1. **Re-run the census.** Expected:
   - June-only: 0, except excluded types (`.lua`, `.kim`, root binaries) and `held` rows;
   - differs: 0, except `held` rows (the `.txt` files are in P8b now);
   - unstaged: 0;
   - every Inbound-routes row ticked, with the wave that delivered it.
2. **Bookkeeping:** update backlog Stage 15's counts from today's census (its 117,562 / 4,363 are the 2026-10-08 baseline), close the
   block in STATUS (append it to `STATUS_ARCHIVE.md`, add one *Closed plans* row), and write the plan's close-out `MODS.md` summary
   (waves, counts, revert recipes).
3. **Debug flag:** comment out `//#define SERV_IRUHADEV_KOM_MOUNT_DEBUG` (a retire; `finish.py` lists it as `retired`). **No build**
   (CLAUDE.md, 2026-11-13: a debug-define retire runs no build; the next exe build picks it up). Ask the user only if they want it kept.

## Excluded and routed

| Item | Route |
|---|---|
| `.lua` (7,329) | lua ports |
| `.kim` | june-item-kim |
| `.txt` (85 at planning time: 34 colliding, 51 June-only) | june-effect did them (closed): the census shows what is left. The 4 `_INT.txt` are P8b (backlog Stage 15) |
| Held names (Rule 8): 7 W3f village files, 83 `Motion_<Class>*`, `asset_port_held.tsv` rows | never staged; the class ports and W3f own them |
| Files other plans staged (C3x / C3s / C3y / C3z-1 / K2 / char ports) | `same bytes` in the census: nothing to do |
| `DialogLog.txt` | dropped (June's own log) |
| Root-dir binaries and configs (`unpacked_x2.exe`, dlls, `.ini`) | not assets |
| P5 `will-break` rows | not exclusions: the file is replaced and the consumer fix is routed per Rule 6 |

The `.txt` row includes `GameOptionPreset.txt`, `fontconfig.txt`, `UnitTitle*.txt`, `NUI_ERIS_BOSS_Map03_A.txt`,
`Npc_All_Ariel_Face_Talk_Dark.txt` and `AIN_POWER_OF_GOD_06_1.txt`.

## Play-test hand-off (every wave)

The gates run before the copy and the hand-off follows it (Claude copies; no ask since 2026-11-10):

1. `stage-june` reports 0 refused, and `verify-staged` 0 problems.
2. `pack`: `-0 removed`, `+added == wave count` per kom, and every `~changed` is in the wave or decode-equal.
3. `dupcheck` reports 0 (run right before the copy).
4. `copy --wave`: md5 of each live kom equals the packed kom. The hand-off quotes the `PACK OK` / `COPY OK` lines.
5. The census is re-run, and the wave's `MODS.md` entry and `<phase>_stage_plan.tsv` are committed (Rule 9).

**The user's hand-off is copy-ready and detailed** (x2-phase "Finish" step 7; never a one-line "boot and walk"). Per check: where to go (a
real village / field / dungeon / NPC / UI screen chosen from the wave list, with the dev-console line that gets there, such as
`/stage <id>`), what to do, what June's version should look like, what a failure looks like, and which file proves it. End with the log tag
you will read. Never launch the game.

**Log check, scoped to the phase** ("check logs" means this phase's own lines):
- `logs.py --tag KOM_MOUNT` has a line for every kom, with `registered == files == inspect`;
- `existfileinfo.txt` has not grown, and no duplicate MessageBox appeared;
- 0 KEM_ERROR88/89/343/390/95/96/87/136/132/50;
- mount milliseconds and private MB are recorded against the baseline.

**Finish** (each wave): Done note of at most ~60 lines in this doc under the phase, the STATUS row updated in the same commit (one line;
evidence in the note), the next kickoff checked, and a commit with explicit pathspecs: subject `june-asset-port Phase <id>: <what>` of
about 100 characters. The wave touches `kom_contents` (outside the repo) and `june2017_re/`, `MODS.md`, this doc and `STATUS.md` inside it.
