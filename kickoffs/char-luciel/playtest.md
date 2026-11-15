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

## DU5b - The unit viewer's second body: the character window shows the switched body

### Dev console
none - no item and no command: DU5b only changes which body the character window draws and animates

### Loadout
n/a - no skills are tested. The checks are the character window (U) and its two switch buttons; no V swap needed.

### Steps
1. Log in **testlu1** (Lu/Ciel, Duo; the save's form is 1, Ciel). Stay in the village.
2. Open the character window (U). Look at the body in the window, then wait 5 seconds and watch it idle.
3. Press the window's switch button, then click the body in the window, then press the button "this window only".
4. Close the window, enter any battle field, open the window again and press "this window only" twice.
5. Relog onto **jtLordKnight** and then **jtCodeEmpress**, opening the character window (U) on each.

### Expect
- Step 2: the window shows **one** body (June's character window never shows Lu and Ciel side by side): Ciel, because the saved form is 1.
  It plays its lobby wait loop (it moves), it is not frozen in a T-pose.
- Step 3: each press or click swaps the window to the other body, which also idles. In the village the button and the click also swap the
  village avatar (DU5's form packet); "this window only" swaps the window alone.
- Step 4: in the field "this window only" swaps the window body each time; the field character does not change.
- With DU5b alone Ciel stands **without gear** in the window (all equipment still sits on Lu's body); with DU5b2 in the same build each body
  wears its own equipment (DU5b2's section).
- Step 5: both windows look exactly as before (one body, own equipment, idle loop).
- Nothing new in `ErrorLog.txt`, no crash when the window opens, swaps or closes.

### If not
Name the step and what you saw: two bodies in the window (the two-body view was not turned off), an empty window or a body that never moves
after a swap (the shown body is not created or not animated: say which body, Lu or Ciel), the swap changing nothing, or a crash on open / swap
(send the X2_*.tmp bundle from %TEMP%).

### Logs
[JUNE_CHAR] LUCIEL (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`: one `LUCIEL DUO FORM unit 166: form a -> b saved, NOT sent` line per
village swap in step 3; DU5b adds no lines of its own, so read `ErrorLog.txt` for new errors)
## DU5b2 - Each body's equipment in the character window ( June's per-body viewer equipment )

### Dev console
none - testlu1 already wears both bodies' gear: Lu's Promotion set over a Cobo beginner set, Ciel's Promotion set, Ciel's Spa Wear one-piece

### Loadout
n/a - no skills are tested. The checks are the character window (U), its switch button, a click on the body and drag-unequipping one item.

### Steps
1. Log in **testlu1** (Lu/Ciel, Duo). Open the character window (U); note which body shows and what it wears.
2. Press the window's switch button so the other body shows; note what it wears. Click the body to swap back.
3. Ciel shown: drag **Ciel's Promotion Gloves** from her glove slot to the bag. Look at her hands. Drag them back onto the slot.
4. Lu shown: drag **Lu's Promotion Gloves** to the bag. Look at Lu's hands. Drag them back.
5. Close and reopen the window twice, swapping body each time.
6. Relog onto **jtLordKnight**, then **jtCodeEmpress**: open the character window (U) and look at each; then open the character select screen.

### Expect
- Lu shown: Lu wears **Lu's Promotion** top / bottom / gloves / shoes and the Promotion dual weapon (the fashion covers the Cobo set); nothing of
  Ciel's is on Lu.
- Ciel shown: Ciel wears her own gear, the **Spa Wear - Charming Geyser** one-piece and her Promotion pieces the one-piece leaves visible
  (gloves, shoes, weapon); she is **not bare** and wears nothing of Lu's.
- Step 3: without her Promotion gloves Ciel shows the **Ciel copy of the Cobo beginner gloves** (item 200113303, built from Lu's normal Cobo
  gloves), never bare hands; re-equipping puts the Promotion gloves back.
- Step 4: Lu shows the Cobo beginner gloves; re-equipping restores the Promotion gloves.
- Step 5: every reopen and swap shows the same gear per body as steps 1-2.
- Step 6: both classes' windows and the select screen look exactly as before (one body, own fashion, weapon in hand).
- No crash, nothing new in `ErrorLog.txt`.

### If not
Name the step, the body (Lu or Ciel) and the slot: a bare body (the item was built on the other skin or not at all), Lu's piece on Ciel or Ciel's
on Lu (wrong body slot), both a fashion and the normal piece visible at once (the slot visibility pass missed a body), the gloves not coming back
after step 3 / 4, a weapon missing, or a crash (send the X2_*.tmp bundle from %TEMP%).

### Logs
[JUNE_CHAR] LUCIEL (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`: the `LUCIEL DUO FORM` lines of each village swap; DU5b2 adds no lines,
so `ErrorLog.txt` is read for new errors)
