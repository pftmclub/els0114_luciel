# June character Lu/Ciel (Stage 8) - Phase DU5c review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5c review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5c done and its play-test passed (Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_AVATAR` (no debug define; the `LUCIEL DUO AVATAR` log line is permanent, one per SD mesh load) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO AVATAR`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU5c - Done (2026-11-16)"
```
and `grep "(DU5c)" june2017_re/funcmap.tsv`, the `CX2SquareUnit::Init` row of `tools/data/june_march_divergent.tsv`.

## Task

Review DU5c's commit against June (Ghidra / `june2017_re/disasm_range.py`, read-only):
- The SD table: `333a3d20`'s 26 names ( lengths and case ) = `_CONST_SQUAREUNIT_DU5C_::SD_MESH_FILE_NAME`; `31ee3470`'s index ( type x 2 + spa ) and the
  ctor call `323f8080 @323f8bb2-323f8c8a` ( only with `g_pData +0x268`, the spa test on its `+0x64` ); March's switch stays in the `#else`.
- `323f1490`: the close / delete / open / skin-anim order, `+0x2b4` = `SetApplyMotionOffset`, the show-off tail; type 10 only.
- `323fea90` head ( before InitSystem ) and `323f3290`'s viewer part: the name and play type read from the shown skin ( `+0x228`, `+0x2a8` ), the raw
  `+0xd80` write, vslot 0x28 = `NotifyShowObjectChanged` ( `31f96ad0` ), `ChangeAnim( name, 0, 0.0 )` + `Play`.
- `323acfb0`'s replays ( `323fa700` = `InitWaitMotion`, `323fad80` = `StateStart` ); `StateStart`'s access change has no other effect.
- `323f7380` ( `+0xd81` = 0, `+0x118c` = 1 before SetUnit ) and `31ed46e0 @31ed4815` / `@31ed489b` ( the slot viewer; the watch window's viewer ).
- Flag-off = March by `cl /P` of `X2SquareUnit.cpp`, `X2TFieldGame.cpp`, `X2Room.cpp` (flag on vs `//#define`).

Hygiene is one command: `python tools/finish.py --review --commits <DU5c's commits> --with SERV_IRUHADEV_JUNE_CHAR_DUO_AVATAR` (`-v`; explain
every OTHER row). Fix clear transcription errors under the same flag (then `tools/build.py --deploy`); hand design decisions back. Write
"DU5c - Review" into the plan doc, update the STATUS row, commit.
