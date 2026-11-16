-- char-luciel CH3d play-test save: run with the game closed
--   python tools/elsdb.py x --file kickoffs/char-luciel/CH3d_save.sql
-- Six level-70 units, all transcended ( unit.transcendence = 1 ): A units take the hyper active, B units its enhanced id ( base level 0 ).
--   LuDlA 223 / LuDlB 224 ( Dread Lord 127 ): Descent Darkness 30601 / Enhanced 30603
--   LuNoA 225 / LuNoB 226 ( Noblesse 128 ):   Castle of Abaddon 30602 / Enhanced 30604
--   LuDnA 228 / LuDnB 229 ( Demonio 129 ):    Diabolic Disaster 30605 / Enhanced 30606
-- Page 1: the job's four hyper-class actives on A S D C ( slots 0-3 ) and Q W E R ( 5-8 ), the hyper active on F ( slot 4 ) and T ( slot 9 ).
--   Dread Lord: Fear Tomorrow 30055, Dead Cross 30056, Demonic Cannon 30083, Bluff 30304
--   Noblesse:   Soul Lock 30057, Magdonic Cannon 30058, Rain of Chaos 30084, Soul Spread 30305
--   Demonio:    Abyss Sniping 30059, Demonic Rush 30060, Abyss Extinction 30085, Rapid Fury 30306
-- Page 2 ( Demonio only ): Altar of Evil 30043 on A and Q, Transcendence 30049 on S and W ( the two forms whose awakening shows the _Trans cut-in ).
-- Both forms' linked skills are learned: Death Touch / Mortal Fear / Dark Plasma 30044-30046 ( added after the play-test: without them the Altar window is empty ), Shadow Fury / Shrewd Cutter / Silent Rage 30050-30052.

DELETE FROM unit_skill_page WHERE unit_uid IN ( 223, 224, 225, 226, 228, 229 );
UPDATE unit_skill SET slot = -1 WHERE unit_uid IN ( 223, 224, 225, 226, 228, 229 );
UPDATE unit SET transcendence = 1 WHERE unit_uid IN ( 223, 224, 225, 226, 228, 229 );

INSERT OR REPLACE INTO unit_skill ( unit_uid, skill_id, level, slot, csp ) VALUES
 ( 223, 30055, 1, 0, 0 ), ( 223, 30056, 1, 1, 0 ), ( 223, 30083, 1, 2, 0 ), ( 223, 30304, 1, 3, 0 ), ( 223, 30601, 1, 4, 0 ),
 ( 224, 30055, 1, 0, 0 ), ( 224, 30056, 1, 1, 0 ), ( 224, 30083, 1, 2, 0 ), ( 224, 30304, 1, 3, 0 ), ( 224, 30601, 0, -1, 0 ), ( 224, 30603, 1, 4, 0 ),
 ( 225, 30057, 1, 0, 0 ), ( 225, 30058, 1, 1, 0 ), ( 225, 30084, 1, 2, 0 ), ( 225, 30305, 1, 3, 0 ), ( 225, 30602, 1, 4, 0 ),
 ( 226, 30057, 1, 0, 0 ), ( 226, 30058, 1, 1, 0 ), ( 226, 30084, 1, 2, 0 ), ( 226, 30305, 1, 3, 0 ), ( 226, 30602, 0, -1, 0 ), ( 226, 30604, 1, 4, 0 ),
 ( 228, 30059, 1, 0, 0 ), ( 228, 30060, 1, 1, 0 ), ( 228, 30085, 1, 2, 0 ), ( 228, 30306, 1, 3, 0 ), ( 228, 30605, 1, 4, 0 ),
 ( 229, 30059, 1, 0, 0 ), ( 229, 30060, 1, 1, 0 ), ( 229, 30085, 1, 2, 0 ), ( 229, 30306, 1, 3, 0 ), ( 229, 30605, 0, -1, 0 ), ( 229, 30606, 1, 4, 0 ),
 ( 228, 30043, 1, -1, 0 ), ( 228, 30049, 1, -1, 0 ), ( 229, 30043, 1, -1, 0 ), ( 229, 30049, 1, -1, 0 ),
 ( 228, 30050, 1, -1, 0 ), ( 228, 30051, 1, -1, 0 ), ( 228, 30052, 1, -1, 0 ),
 ( 229, 30050, 1, -1, 0 ), ( 229, 30051, 1, -1, 0 ), ( 229, 30052, 1, -1, 0 ),
 ( 228, 30044, 1, -1, 0 ), ( 228, 30045, 1, -1, 0 ), ( 228, 30046, 1, -1, 0 ),
 ( 229, 30044, 1, -1, 0 ), ( 229, 30045, 1, -1, 0 ), ( 229, 30046, 1, -1, 0 ),
 ( 228, 30421, 1, -1, 0 ), ( 229, 30421, 1, -1, 0 );

INSERT INTO unit_skill_page ( unit_uid, page, slot, skill_id ) VALUES
 ( 223, 1, 0, 30055 ), ( 223, 1, 1, 30056 ), ( 223, 1, 2, 30083 ), ( 223, 1, 3, 30304 ), ( 223, 1, 4, 30601 ),
 ( 223, 1, 5, 30055 ), ( 223, 1, 6, 30056 ), ( 223, 1, 7, 30083 ), ( 223, 1, 8, 30304 ), ( 223, 1, 9, 30601 ),
 ( 224, 1, 0, 30055 ), ( 224, 1, 1, 30056 ), ( 224, 1, 2, 30083 ), ( 224, 1, 3, 30304 ), ( 224, 1, 4, 30603 ),
 ( 224, 1, 5, 30055 ), ( 224, 1, 6, 30056 ), ( 224, 1, 7, 30083 ), ( 224, 1, 8, 30304 ), ( 224, 1, 9, 30603 ),
 ( 225, 1, 0, 30057 ), ( 225, 1, 1, 30058 ), ( 225, 1, 2, 30084 ), ( 225, 1, 3, 30305 ), ( 225, 1, 4, 30602 ),
 ( 225, 1, 5, 30057 ), ( 225, 1, 6, 30058 ), ( 225, 1, 7, 30084 ), ( 225, 1, 8, 30305 ), ( 225, 1, 9, 30602 ),
 ( 226, 1, 0, 30057 ), ( 226, 1, 1, 30058 ), ( 226, 1, 2, 30084 ), ( 226, 1, 3, 30305 ), ( 226, 1, 4, 30604 ),
 ( 226, 1, 5, 30057 ), ( 226, 1, 6, 30058 ), ( 226, 1, 7, 30084 ), ( 226, 1, 8, 30305 ), ( 226, 1, 9, 30604 ),
 ( 228, 1, 0, 30059 ), ( 228, 1, 1, 30060 ), ( 228, 1, 2, 30085 ), ( 228, 1, 3, 30306 ), ( 228, 1, 4, 30605 ),
 ( 228, 1, 5, 30059 ), ( 228, 1, 6, 30060 ), ( 228, 1, 7, 30085 ), ( 228, 1, 8, 30306 ), ( 228, 1, 9, 30605 ),
 ( 229, 1, 0, 30059 ), ( 229, 1, 1, 30060 ), ( 229, 1, 2, 30085 ), ( 229, 1, 3, 30306 ), ( 229, 1, 4, 30606 ),
 ( 229, 1, 5, 30059 ), ( 229, 1, 6, 30060 ), ( 229, 1, 7, 30085 ), ( 229, 1, 8, 30306 ), ( 229, 1, 9, 30606 ),
 ( 228, 2, 0, 30043 ), ( 228, 2, 1, 30049 ), ( 228, 2, 5, 30043 ), ( 228, 2, 6, 30049 ),
 ( 229, 2, 0, 30043 ), ( 229, 2, 1, 30049 ), ( 229, 2, 5, 30043 ), ( 229, 2, 6, 30049 );

INSERT OR REPLACE INTO unit_skill_page_info ( unit_uid, page_count, active_page ) VALUES
 ( 223, 1, 1 ), ( 224, 1, 1 ), ( 225, 1, 1 ), ( 226, 1, 1 ), ( 228, 2, 1 ), ( 229, 2, 1 );
