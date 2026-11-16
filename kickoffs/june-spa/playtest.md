# june-spa play-test sections

One section per phase, written by that phase at Finish while it knows what it changed. A batch hand-off is assembled from these with
`python tools/playtest.py assemble june-spa [ID ...]`, never rewritten later. `python tools/playtest.py check june-spa` lints them.
Standing rules still apply to every section: dev commands first, copy-ready lines, per check where / what / expect / if not, and the log
lines to read. Batch: S1 and S2a are assembled with S2b at the S2b seam.

## S1 - Spa data and assets (regression: villages still load)

Data only: no client code changed, so there is nothing new to see yet. The spa doors and signs come with S2a / S2b. This section proves the new
data (Ruben's spa door, two spa village rows, five spa houses, spa NPC rows, spa map rows, two spa worlds) did not break the villages that
parse it. Use any character of level 10 or more. `data034`, `data036`, `data037`, `data039` and `data040` were packed and copied.

### Dev console
`/warp 0`
`/warp 1`

### Loadout
| key | skill (nickname) | page / slot |
|---|---|---|
| any | no skill is used in this check; any character works | none |

### Steps
1. Start the game from `start_offline.bat` and log in with any character of level 10 or more. Note the time.
2. Open the dev console (END key) and paste `/warp 0`. You arrive in Ruben village. Look at the village for 10 seconds: the NPCs stand, the
   billboard (the notice board) stands, nothing flickers. Walk once from the left edge to the right edge. Nothing new may appear anywhere (the spa door
   comes with S2b).
3. Open the world map (the map button in the bottom bar), click Ruben, then close it. The Ruben local map must open and close as before.
4. Paste `/warp 1`. You arrive in Elder village. Same check as step 2 (NPCs and billboard stand).
5. Go to Ranox village (Silent Night Shelter's village) the way you did for earlier Lanox tests (world map or the Sander gate). Same check.
6. Quit the game by the normal exit (not the window cross during loading).

### Expect
- Ruben, Elder and Ranox load with their usual NPCs; no spa prompt, no spa sign and no spa particle anywhere yet (the spa door and signs need
  code event 31, which S2a adds). This is correct for S1.
- The world map and the Ruben local map open and close as before.
- `ErrorLog.txt` has no new line from this session. In particular no `[LUA_SRC]` line (a script failing to load), no `Village` / `House`
  parse line, and no "table index is nil".
- `offline_server.log` shows no new `EXCEPTION` and `offline_packets.log` no new `UNHANDLED` line for these steps.

### If not
- A village that does not load, or loads with missing NPCs: say which village. The likely cause is `DLG_Map_Village.lua` (data034) or
  `DLG_Map_House.lua` (data034); the packed old copies are in `kom_backup_tools\` (the newest `data034.kom` backup).
- The world map or the Ruben local map failing: `DLG_Map_Local_Ruben.lua` (data034; `SpaData.ID` now set) or `MapData.lua` (data036).
- A character that fails to log in or a shop / NPC dialog that is missing: `Enum.lua` (data036) or `NPCTemplet.lua` (data037); name the NPC.
- Send the first new `ErrorLog.txt` line word for word.

### Logs
JUNE_SPA (no line is expected in S1; read `ErrorLog.txt` as a whole for lines newer than the session start)

## S2a - Code event 31 from the save, the spa villages load, warp rows

New: the save gets a `code_events` table on the first start of this exe (schema v24 -> v25, row 31 = spa on), and the emulator sends it after the
character list. Both spa villages now get their templets, so you can warp into them, and the warp list shows the two spa rows while code event 31
is on. The spa doors and their prompts are S2b's (in the S2b section), so do not expect a door prompt here. Use **reyaa** (Grand Archer, level 99)
for every step. The game must be closed before the two `elsdb.py` lines; I run them if you ask.

### Dev console
`/warp 19`
`/warp 20`
`/warp 0`
`python tools/elsdb.py code-event off 31` (terminal, game closed)
`python tools/elsdb.py code-event on 31` (terminal, game closed)

### Loadout
| key | skill (nickname) | page / slot |
|---|---|---|
| any | no skill is used in this check | none |

### Steps
1. Start the game from `start_offline.bat` and log in with **reyaa**. (First start of this exe migrates the save to v25.)
2. In Ruben, open the dev console (END key) and paste `/warp 19`. You go to the **Ruben spa**. Look at: the loading screen pictures, the field
   title that appears at the top, the camera (a wide side view, like a village), and press the minimap / map key: the map opens and the channel
   drop-down in the minimap window can be clicked.
3. Paste `/warp 20`. You go to the **Ranox spa**: same four checks.
4. Paste `/warp 0` (back to Ruben). Look at the village for the **spa sign** (the new sign NPC near the spa door, "Ruben spa").
5. Click the warp (Cobo Express) button that opens the list of villages to teleport to. Scroll to the top: the first two rows must be the Ruben
   spa and the Ranox spa, then Ruben, Elder and the rest. Close the list (do not warp).
6. Paste `/warp 19` again and, while standing in the Ruben spa, quit by the normal exit. Start the game again, log in with reyaa: you must arrive
   in the Ruben spa, not Ruben.
7. Quit. Run `python tools/elsdb.py code-event off 31` (or ask me to). Start, log in with reyaa (you arrive in the Ruben spa: leave with `/warp 0`).
   In Ruben: the spa sign is gone. Open the warp list: no spa rows; Ruben is the first row.
8. Quit. Run `python tools/elsdb.py code-event on 31` (or ask me to) so the event is on again for S2b.

### Expect
- Step 1: `offline_server.log` has `[JUNE_SPA] DB MIG v25 done: code_events added, 31 = CEI_SPA_ON_OFF on` (first start only),
  `[JUNE_SPA] spadata local=10000 village=1500` and `[JUNE_SPA] spadata local=10008 village=1501` (each time the world data loads), and
  `[JUNE_SPA] code-event send ids=31` after the character list.
- Steps 2 and 3: the loading pictures are the spa ones (`HQ_LoadingSpa_Ruben_01/02`, `HQ_LoadingSpa_Ranox_01/02`), the title is the spa's own
  banner (picture 29 Ruben spa, 30 Ranox spa), the game camera is the spa's own (`CAMERA { 1800, 160, 300 }`: a village-like side view), the
  map window opens at the full-screen village size with Ranox field 3's map view (June gives both spas that one), and the minimap's channel
  drop-down is enabled. The `FIELD` line in `offline_server.log` shows `mapID=1500` / `mapID=1501`.
- Step 4: the spa sign stands in Ruben (and in Ranox, if you pass there).
- Step 5: two spa rows at the top of the list (both need code event 31; the Ranox one also Ranox's level, 99 is enough).
- Step 6: login lands in the Ruben spa (`FIELD ... entering mapID=1500`).
- Step 7: `[JUNE_SPA] code-event send ids=(none)`, no spa sign, no spa rows. The warp item list (inventory) shows no Ruben spa row either way:
  June's `DLG_Destination.lua` has no `Village_1500` control (June's data, correct).
- No new `ErrorLog.txt` line, no `EXCEPTION` / `UNHANDLED` for these steps.

### If not
- `/warp 19` or `/warp 20` does nothing or sends you to Ruben: the spa templet did not load; send the `[JUNE_SPA] spadata` lines (missing = the
  local-map `SpaData` was not read; `DLG_Map_Local_Ruben.lua` / `DLG_Map_Local_Ranox.lua`).
- A black or village loading picture, a missing title, a wrong camera or a map that opens at the wrong size: name the spa and which of the four.
- The game refuses the save at login (`DB ERROR save is schema v25`): an older exe was started; the deployed one must be this build.
- No spa rows with the event on, or rows still there after `off`: send the `code-event send ids=` line and the output of
  `python tools/elsdb.py code-event list`.
- Login after step 6 lands in Ruben: send the `FIELD` lines of that login.

### Logs
[JUNE_SPA] and the emulator's `FIELD` lines (`python tools/logs.py --tag JUNE_SPA`, then `--grep FIELD`)
