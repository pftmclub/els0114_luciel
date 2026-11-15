# char-luciel play-test sections

One section per phase, written by that phase at Finish while it knows what it changed. A batch hand-off is assembled from these with
`python tools/playtest.py assemble char-luciel [ID ...]`, never rewritten later. `python tools/playtest.py check char-luciel` lints them.
Standing rules still apply to every section: dev commands first, a table per key (A S D C F Q W E R T), nicknames not ids, skills in slots
0-3 and 5-8 (4 / 9 only for transcendence), skill pages for many skills (max 3 loadouts), and "after each skill you must be able to move;
if you are stuck, name the skill".

## DU3b - June's equip view list on March characters (Elsword, Ara)

### Dev console
`/additem 69865 1` (+7 Sparring Sword, Elsword: a PvP-equip weapon whose stored enchant is +0)
`/additem 81020 1` (Shadow Incubus Nightmare Sword avatar, Elsword)
`/additem 30077 1` (Demonic Horn, a normal face accessory any class can wear)
`/additem 69895 1` (+7 Sparring Spear, Ara)

### Loadout
n/a - no skills are tested; the checks are equipment changes made from the inventory while standing in a field

### Steps
1. Log in an Elsword, enter any battle field (not the village: the view list is the in-game unit's), stand still.
2. Open the inventory (I), equip the +7 Sparring Sword. Look at the sword.
3. Equip the Shadow Incubus avatar sword over it, then take the avatar sword off again. Look at the sword after each change.
4. Equip the Demonic Horn, then take it off.
5. Log in an Ara, enter a battle field, equip the +7 Sparring Spear, awaken (berserk), then take the spear off and put it back on while awakened.

### Expect
- Step 2: the Sparring Sword shows the +7 enchant look at once (June shows a PvP-equip item at its fixed level; March showed it as +0).
- Step 3: the avatar sword replaces it; after taking the avatar off the Sparring Sword comes back, still with the +7 look (not the basic sword).
- Step 4: the horn appears on the head when equipped and disappears when removed.
- Step 5: the spear shows in both the normal and the awakened look; after the swap the awakened look shows it again.
- Nothing new in `ErrorLog.txt`; the game does not crash on any equipment change.

### If not
Name the step and what was shown instead: a basic sword after removing the avatar (usable test or per-body table wrong), the sword at +0
(enchant getter not used by the view list), two swords drawn at once or a sword left floating (the remove scan kept an equip), the horn
staying after removal, or Ara's awakened body missing the spear (berserk view list). A crash on an equip change: send the X2_*.tmp bundle.

### Logs
[JUNE_CHAR] (no DU3b lines; read `ErrorLog.txt` for new errors)
