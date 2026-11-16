# June character Lu/Ciel (Stage 8) - Phase DU5c2 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5c2 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5c2 done and its play-test passed (Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY`, `SERV_IRUHADEV_JUNE_DIVERGE_INVENTORY_DEFAULT_SIZE` (no debug define; the two `LUCIEL DUO LOBBY` lines are permanent, one per screen build) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO LOBBY`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU5c2 - Done (2026-11-16)"
```
and `grep "(DU5c2)" june2017_re/funcmap.tsv`, the `CX2Inventory::CX2Inventory` and `CX2Eqip::SetFixed` rows of `tools/data/june_march_divergent.tsv`.

## Task

Review DU5c2's commit against June (Ghidra / `june2017_re/disasm_range.py`, read-only):
- `31fa8ce0` with `param_4` 0: the back body's position ( `+0xd84` is written only by the ctor `@31fab78d`, `+0xd88` only `@31fab47a` ), the
  equip filter `31fac160`, the enchant sync `@31fa9031-31fa9092`, the second-ring skip `@31fa90e7`, the render-param block `@31fa9149`. Judge the
  decision not to repeat the shared tail ( June runs it twice per frame on the shown body while the back body is drawn ): keep, or port the
  doubling as a June quirk ( JUNE_BUGS row ).
- The shown pass's equip filter and `32869e10( second-ring pos, 0, body )` in March's `OnFrameMove`; `31f974b0( false )` / `31fa00b0 @31fa022d`
  in Prepare / Draw; `31f99f70`'s back move and `31f969a0` in `SetPositionOnScr`.
- `325559f0 @32555d38` ( state 5 ), `@32555fe8-3255602a`; `32564400 @32564846-325648ac`.
- `3286d280`: every `m_MaxSize` 0x32; `3257cd60` calls no size function.
- `SetFixed`'s route to june-engine R14f: `32961c80 @32962094` and the viewer's `bAttachedInDGManagerChain` for states 7 / 8 / 9.
- Flag-off = March by `cl /P` of `X2UnitViewerUI.cpp`, `X2StateCreateUnit.cpp`, `X2StateServerSelect.cpp`, `X2Inventory.cpp` (flags on vs `//#define`).

Hygiene is one command: `python tools/finish.py --review --commits <DU5c2's commits> --with SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY
SERV_IRUHADEV_JUNE_DIVERGE_INVENTORY_DEFAULT_SIZE` (`-v`; explain every OTHER row). Fix clear transcription errors under the same flag (then
`tools/build.py --deploy`); hand design decisions back. Write "DU5c2 - Review" into the plan doc, update the STATUS row, commit.
