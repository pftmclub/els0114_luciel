-- char-luciel CH3a play-test save: run with the game closed
--   python tools/elsdb.py x --file kickoffs/char-luciel/CH3a_save.sql
-- LuDlA ( unit 223, Lu / Dread Lord, level 70 ): the base skills, 4 per page on keys A S D C ( slots 0-3 ) and the same 4 on Q W E R ( slots 5-8 ).
--   page 1: Supreme Punishment 30012, Summon Darkness 30013, Phantom Blade 30015, Oblivion 30014
--   page 2: Fear Tomorrow 30055, Dead Cross 30056, Demonic Cannon 30083, Dancing Blade 10018
--   page 3: Terror Absorption 30204, Shackling Chain 30206, Bluff 30304
-- LuDlB ( unit 224 ): the enhanced skills ( base level 0 ), the same shape.
--   page 1: Supreme Punishment+ 30064, Summon Darkness+ 30065, Phantom Blade+ 30067, Oblivion+ 30066
--   page 2: Terror Absorption+ 30308, Shackling Chain+ 30309
-- Slots 4 and 9 stay empty ( transcendence ).

DELETE FROM unit_skill_page WHERE unit_uid IN ( 223, 224 );
UPDATE unit_skill SET slot = -1 WHERE unit_uid IN ( 223, 224 );

INSERT OR REPLACE INTO unit_skill ( unit_uid, skill_id, level, slot, csp ) VALUES
 ( 223, 30012, 1, 0, 0 ), ( 223, 30013, 1, 1, 0 ), ( 223, 30015, 1, 2, 0 ), ( 223, 30014, 1, 3, 0 ),
 ( 223, 30055, 1, -1, 0 ), ( 223, 30056, 1, -1, 0 ), ( 223, 30083, 1, -1, 0 ), ( 223, 10018, 1, -1, 0 ),
 ( 223, 30204, 1, -1, 0 ), ( 223, 30206, 1, -1, 0 ), ( 223, 30304, 1, -1, 0 ),
 ( 224, 30012, 0, -1, 0 ), ( 224, 30013, 0, -1, 0 ), ( 224, 30015, 0, -1, 0 ), ( 224, 30014, 0, -1, 0 ),
 ( 224, 30204, 0, -1, 0 ), ( 224, 30206, 0, -1, 0 ),
 ( 224, 30064, 1, 0, 0 ), ( 224, 30065, 1, 1, 0 ), ( 224, 30067, 1, 2, 0 ), ( 224, 30066, 1, 3, 0 ),
 ( 224, 30308, 1, -1, 0 ), ( 224, 30309, 1, -1, 0 );

INSERT INTO unit_skill_page ( unit_uid, page, slot, skill_id ) VALUES
 ( 223, 1, 0, 30012 ), ( 223, 1, 1, 30013 ), ( 223, 1, 2, 30015 ), ( 223, 1, 3, 30014 ),
 ( 223, 1, 5, 30012 ), ( 223, 1, 6, 30013 ), ( 223, 1, 7, 30015 ), ( 223, 1, 8, 30014 ),
 ( 223, 2, 0, 30055 ), ( 223, 2, 1, 30056 ), ( 223, 2, 2, 30083 ), ( 223, 2, 3, 10018 ),
 ( 223, 2, 5, 30055 ), ( 223, 2, 6, 30056 ), ( 223, 2, 7, 30083 ), ( 223, 2, 8, 10018 ),
 ( 223, 3, 0, 30204 ), ( 223, 3, 1, 30206 ), ( 223, 3, 2, 30304 ),
 ( 223, 3, 5, 30204 ), ( 223, 3, 6, 30206 ), ( 223, 3, 7, 30304 ),
 ( 224, 1, 0, 30064 ), ( 224, 1, 1, 30065 ), ( 224, 1, 2, 30067 ), ( 224, 1, 3, 30066 ),
 ( 224, 1, 5, 30064 ), ( 224, 1, 6, 30065 ), ( 224, 1, 7, 30067 ), ( 224, 1, 8, 30066 ),
 ( 224, 2, 0, 30308 ), ( 224, 2, 1, 30309 ),
 ( 224, 2, 5, 30308 ), ( 224, 2, 6, 30309 );

INSERT OR REPLACE INTO unit_skill_page_info ( unit_uid, page_count, active_page ) VALUES ( 223, 3, 1 ), ( 224, 2, 1 );
