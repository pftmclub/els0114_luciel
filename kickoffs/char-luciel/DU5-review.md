# June character Lu/Ciel (Stage 8) - Phase DU5 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5 done and its play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEW` (also in `ServerDefine.h`, `SECOND_HOMES`; no `_DEBUG`) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`): `python tools/logs.py --grep "LUCIEL DUO FORM"` |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "Phase DU - The two-body engine (DU1-DU5)" "DU5 - Done (2026-11-15)"
```
Then `grep "(DU5)" june2017_re/funcmap.tsv` and JUNE_BUGS row 749.

## Task

Review the DU5 commit against June (Ghidra read-only; uncarved chunks with `june2017_re/disasm_range.py`):
- **Click switch** `3291d390 @3291d4a2-3291d622` and **buttons** `3291db60 @3291dc9f-3291ddd1`: the gate (`g_pTFieldGame +8` = my square unit, `+0x608`
  emotion, `+0x49c` = `m_bFootOnLine`), the village test (June state `0xe` = `XS_VILLAGE_MAP`; `0xf` agit has no March state), the form sent
  (`!unit +0xe40`), the local branch order (`32912c50`, `3291d2e0`, `31f94190` stand-in, `3291cfb0( 1 )`, `329146d0`, inventory `SetShow( 1 )`).
- **NOT apply** `323acfb0` against `CX2TFieldGame::Handler_EGS_CHANGE_UNIT_FORM_NOT`: the `-1` test, `323f3290`'s return as the window's switch
  value, `321524d0` ids `0x13` / `0x1f` / `0x14` = CharInfo / inventory / pet list, `321969e0` = `SetInventorySort( +0xd8, page )`.
- **Setter** `3234b8c0` (inventory vslot 8 = `SetShow`, `32194ed0`); **square head** `323f3290` (stand-ins for `323f1490` and `@323f331e-323f3479`).
- **Drop** `32918410 @32918837-32918bb9`: the remap before the body, the anyone-accessory body (window switch read for viewer `+0xd80`), the
  conflict return (no drag return), the second test's minor clause. **Right-click** `@321aeb3e` / `@321aebf3`.
- **Conflict** `3234f4d0`: DITT 76 = `DITT_ITEM_ID_HEAD_TYPE_ACCESSORY` (June `Enum.src.lua`), the loop bound (`unit +0x2cc` = equip category size),
  the owner's form. **Ring** `321693b0`. **Highlight** `32913b90 @32913d3e` (JUNE_BUGS 749).
- **Packets:** 0x4d9 = one byte, 0x4da = UidType + Get(char) (`320aa0f0`); the emulator writes `unit.minor_unit_form` and the session cache.
- **Flag-off = March:** `cl /P` of `X2UICharInfo.cpp`, `X2UIInventory.cpp`, `X2TFieldGame.cpp` with the flag on and off differ only inside its blocks
  (shadow `Always.h` / `ServerDefine.h` through `/I`, never edit the shared ones).

Hygiene: `python tools/finish.py --review --commits <DU5> --with SERV_IRUHADEV_JUNE_CHAR_DUO_VIEW`. Paste the block; explain every OTHER row
(enumscan's 190 rows predate DU5: intersect with `git diff -U0`). Write "DU5 - Review" into the plan doc, set DU5 done and DU5b / DU5e ready in
STATUS.md, check `DU5b.md`, commit. No play-test unless a fix lands.
