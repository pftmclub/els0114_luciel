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
