# June character Lu/Ciel (Stage 8) - Phase CH1a0 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1a0 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH1a0 done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_TRANSFORM_LINK` (class-agnostic) and `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (no `_DEBUG`) |
| Log tag | none (inert) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH1a0 - Done (2026-11-14)"
```
Then `grep "Char Luciel CH1a0" june2017_re/funcmap.tsv` (23 rows) and `JUNE_BUGS.md` row 674.

## Task

Review CH1a0's commit against June (Ghidra, read-only):
- **Transform pair.** `31fc9d50` / `31fc9d90`: the case values ( 0 / 1 / 2 / 3 ) and their `CX2Unit::DT_*` names; case 2 = either, case 3 = both.
- **The window object** ( user `+0x8f70`, 0x18 bytes ): `SetCoolTime` `31fc9e80` (the `&&` range test kept, negative -> 0, the show flag is
  value > 0), `SetCoolTimeShow` `31fc9df0`, `GetSlotSkillID` / `GetSlotSkillLevel` `31fdc680` / `31fdc810` (return 0 on every miss, the `SlotB` +
  index name through `STR_ID_3738`), `SelectSlot` `31fdc9a0`, `CanUseSlot` `3200f680` (consume <= now MP; unordered -> false).
- **The user helpers.** `31fc9f00` (add vs multiply on `bAdd`), `31fd0490` (order: cool time on the selected slot, then MP, then selection -1),
  `32030160` (key rows: last pressed wins; the early return when a slot is already selected; message ids 2549 / 226), `32039dc0`, `31fd0440`
  (`oneC` / `oneR` from the `+0x86da` input offsets 0x19 / 0x39).
- **Slot data type 6** `328d9b00` / `328da050` and `SDT_TRANSFORM_SKILL_SLOT = 6` (`KTDGUISlot.h`); `SDT_END` is unused.
- **Gauge** `32498e40` / `32a38250` / `32a327a0`: members, max 10, float 1.0, store-then-clamp. Its 20 virtual overrides are CH1a1's, not a gap here.
- **Switching guide** `322e6300` against Eve's `322e65c0` port: the three differences named in the funcmap row.
- **Routing.** Every branch the Done note routes ( CH1a1, CH2d20, CH1k, CH1l, CH1 final, DU5, the dungeon reader `321faa90` ) is in that kickoff.

Spot-check 3 items directly in Ghidra; if one fails, re-check the whole phase. Hygiene: `python tools/flagcheck.py
SERV_IRUHADEV_JUNE_CHAR_TRANSFORM_LINK --range <CH1a0 commit>^..<CH1a0 commit>` and the same for `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (its own
files: `Always.h`, `X2Game.h`, `X2Game.cpp`, `X2GageData.h`; the tool cannot evaluate one `#if` in `X2GUUser.cpp`), then `git log -S<flag>
<commit>..HEAD`, `python tools/junediverge.py --check-all`. Paste the summary lines; explain every OTHER row.

Write "CH1a0 - Review" into the plan doc, update the CH1a0 row in STATUS.md (done; CH1a, CH1a1, CH1k0 and CH1l30 unblocked), and commit. No
build unless a fix lands (then a one-file `/Zs` probe; no play-test: the code is inert).
