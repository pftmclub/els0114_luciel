# June character Lu/Ciel (Stage 8) - Phase DU5e review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5e review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5e done and its play-test passed (Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_STAT` (second homes `ServerDefine.h` + `X2ServerProtocol_2010.vcxproj` X2TOOL) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO STAT`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU1c - Done (2026-11-14)" "DU5e - Done (2026-11-15)"
```
and `grep "(DU5e)" june2017_re/funcmap.tsv` (9 rows), `JUNE_BUGS.md` row 753.

## Task

Review DU5e's commit against June (Ghidra / `june2017_re/disasm_range.py`, read-only):
- `SetGameStats` `323571c0`: both writes, and that March's `SetKStat` per stat is the right reading of June's inline conversion (June's KStat has
  7 fields, field 0 a float base HP).
- The seven sites: which KStat goes where (the push order of the two `rep movsd` blocks), and which one each site gives the user (vslot 0x1e0);
  `31f1ed70 @31f1ee8c` picks by the unit's form, the other four take the first (JUNE_BUGS 753).
- The packet offsets: the second KStat right after `m_kGameStat` in `KRoomUserInfo` (`32360090`: +0xb0 / +0xcc) and the five NOTs; Put / Get pairs.
- The transform test `@31f1f1d8` / `@324620b9` covers exactly March's NEW_CHARACTER_CHUNG block plus the weapon refresh, and `UpdateDuo( 0 )`
  `@31f1f27a` sits inside it (equip change only).
- The emulator stand-in: Lu = base + groups 0 / 2, Ciel = base + groups 1 / 2 (the slot table = `CX2Inventory::GetEquipGroup`), other classes unchanged
  and a zero second stat; every room / level-up sender fills both.
- Flag-off = March by `cl /P` of `X2StateMenu.cpp` and `X2Unit.cpp` (flag on vs `//#define` in `Always.h` and `ServerDefine.h`).

Hygiene is one command: `python tools/finish.py --review --commits <DU5e's commit> --with SERV_IRUHADEV_JUNE_CHAR_DUO_STAT` (`-v`; explain every OTHER
row). Fix clear transcription errors under the same flag (then `tools/build.py --deploy`); hand design decisions back. Write "DU5e - Review" into
the plan doc, update the STATUS row, check `kickoffs/char-luciel/DU5f.md`, commit.
