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

## DU3c - Lu and Ciel each wear their own equipment; Ciel draws a body

### Dev console
`/invin 0` (turns invincibility off so Ciel can die in step 6)
`/additem 30344 1` (Curse of Darkness Dual Weapon, Lu/Ciel: Lu's weapon, with a separate Ciel version of the model)

### Loadout
n/a - no skills are tested; the checks are what each body wears, an equipment change, and one death. Swap bodies with V.

### Steps
1. Log in **testlu1** (Lu/Ciel, Duo; raised to level 15 in the save so the level-10 weapon fits). Enter any battle field (not the village)
   and stand still.
2. Look at both bodies. Press V to swap to Ciel, then V again to swap back to Lu.
3. Open the inventory (I), take off the **Cobo Top Piece for Beginners** (normal top, under Lu's Promotion avatar top), look at both bodies,
   then put it back on.
4. Equip the **Curse of Darkness Dual Weapon** in place of the Cobo Dual Weapon. Swap to Ciel (V) and look at his weapon, then swap back.
5. Leave the field and come back (or relog), so the unit is built fresh with the new weapon. Look at both bodies again.
6. With Ciel active, use one **Ventus' Wings (Elixir)** from the quick slot (a buff icon appears), go into a dungeon (`/stage` any) and let
   Ciel die. Revive (resurrection or back to the village).

### Expect
- Step 2: **Ciel has a body** with clothes and a weapon (the Ciel version of the Cobo beginner set); Lu wears Lu's Promotion avatar. Each
  swap shows the other body with its own clothes; no body is invisible, no clothes float without a body, and neither body wears the other's
  clothes.
- Step 3: Lu still shows Lu's Promotion top (the avatar covers the slot). Ciel's top changes to his basic top while the Cobo top is off and
  comes back when it is put on again.
- Step 4: Lu holds the Curse of Darkness weapon; Ciel holds his own version of it (a different model, not Lu's weapon and not empty hands).
- Step 5: the same look as at the end of step 4 (this is the build-at-load path; steps 2-4 were the equip-change path).
- Step 6: after the revive the Ventus' Wings buff icon is gone (CH1's owed check).
- Nothing new in `ErrorLog.txt`, no crash on load, on swap, on an equipment change or on leaving the field.

### If not
Name the step and what you saw: Ciel invisible or only a floating weapon (his equips were not built: say whether it happens at load (step 2 / 5)
or only after an equipment change (step 3 / 4)), Ciel wearing Lu's Promotion avatar or Lu's model (the body byte is wrong), two weapons in one
hand, a crash when leaving the field (a dummy item freed twice: send the X2_*.tmp bundle from %TEMP%), or the buff still shown after the revive.

### Logs
[JUNE_CHAR] LUCIEL (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`: `LUCIEL DUO switch` per swap; DU3c adds no lines of its own, so read
`ErrorLog.txt` for new errors)
