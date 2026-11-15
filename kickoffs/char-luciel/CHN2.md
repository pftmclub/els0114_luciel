# June character Lu/Ciel (Stage 8) - Phase CHN2 - Promotion cube stand-in rows for every June job

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase char-luciel CHN2` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CHN (play-test 1 done 2026-11-15) |
| Flag | none expected (server-Lua data only); a client edit, if one turns out to be needed, goes under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CLASS_CHANGE` |
| Log tag | `CUBE` lines in `offline_server.log` (`python tools/logs.py --grep "CUBE"`) |
| Review | covered by `kickoffs/char-luciel/CHN-review.md` (CHN + CHN2) |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CHN - Done (2026-11-15)"
python tools/docsection.py CLAUDE_REFERENCE.md "Background: the server-Lua rule"
```

## Task

**User decision (2026-11-15, CHN play-test):** the job-change Promotion Cubes, 109999 (1st job) and 110850 (2nd job), get **labelled stand-in
rows for every June job class**, because June's server `RandomItemTable.lua` is not in the repo. Today they refuse with NetError 285
(`CUBE refused - item 110850 lists no item group for class 128`).

- **Shape = March's own rows** in `KncWX2Server/ServerResource/US/RandomItemTable.lua` (BOM + CRLF, keep both exact): cube 109999 at `:2404`, 110850 at
  `:2657`, one `{ m_cUnitClass = UNIT_CLASS["..."], m_iItemGroupID = N }` per class; each group is that job's promotion set, one
  `g_pRandomItemManager:AddRandomItemGroup( group, item, 20, 15, 1 )` per piece (e.g. Lord Knight group 12576 = 131136-131140, sword / top /
  bottom / gloves / shoes). Read the existing rows' columns (rate, period, quantity) and copy them; never invent a column value.
- **Classes and items, from `item.kim` by name** (`/x2-kim find --re "(?i)<job>.*promotion"`; check `unit_class` matches):
  1st-job cube: Lu/Ciel 37 / 38 / 39, Ara 34, Elesis 35, Add 33 / 36 (and any other June 1st job March's list lacks: diff the class list against
  `X2Unit.h`'s `UNIT_CLASS` under `JUNE_ENUM_NAMES`, minus third jobs 150-186). 2nd-job cube: Lu/Ciel 127 / 128 / 129, Ara 124, Elesis 125, Add 123 / 126,
  and Elesis 120 / 121 if missing. Lu/Ciel found in CHN: 135623-135627 (37), 135629-135633 (38), 42103-42107 (39), 135637-135641 (127),
  135643-135647 (128), 42111-42115 (129).
- **Decide and write down, do not guess silently:** Diabla / Demonio also have a `Promotional Hair` (42108 / 42116) and Lu/Ciel promotion
  pieces may have a Ciel copy (`item_id_for_minor_unit`). March's sets are the 5 equipment pieces; keep to the 5 unless `item.kim` shows the
  5-piece reading is wrong, and say which in the Done note. A class with no promotion set in `item.kim` gets no row (say so).
- **New group ids:** take the next free ids after the file's highest `AddRandomItemGroup` group, one per class per cube; list them in the Done note.
- Every added block carries a one-line Lua comment `-- stand-in ( char-luciel CHN2 ): June's server table not in the repo; item.kim promotion set`.
- The emulator resolves `UNIT_CLASS["UC_LUCIEL_..."]` through `X2OfflineLuaEnum` (CHN added the Lu/Ciel names; other June names come from
  data036 `Enum.lua`, which wins). Confirm each name you use resolves (the loader logs refused rows) before handing off.
- **Packing:** `RandomItemTable.lua` is server Lua: **the user packs it** (XOR + data036, CLAUDE.md "server-side Lua the client needs").
  Hand off the file path and ask for the pack; then the play-test: open each cube on the CHN units 180-185 (`/additem 109999 1` on a 1st job,
  `/additem 110850 1` on a 2nd job), plus one Ara / Elesis unit if the save has one. Expect the set in the bag and a `CUBE` opened line.
