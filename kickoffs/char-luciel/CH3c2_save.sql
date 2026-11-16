-- char-luciel CH3c2 play-test save: run with the game closed
--   python tools/elsdb.py x --file kickoffs/char-luciel/CH3c2_save.sql
-- LuDnA ( unit 228, Lu / Demonio, level 70 ): the base skills, 4 per page on keys A S D C ( slots 0-3 ) and the same 4 on Q W E R ( slots 5-8 ).
--   page 1: Desperado 30047, Transcendence 30049, Abyss Sniping 30059, Demonic Rush 30060
--   page 2: Rapid Impact 30217 ( its move attack is CH3c2's )
-- LuDnB ( unit 229 ): the enhanced skills ( base level 0 ), the same shape.
--   page 1: Desperado+ 30078, Transcendence+ 30080, Abyss Sniping 30059, Demonic Rush 30060
--   page 2: Rapid Impact+ 30314
-- Both learn Shadow Fury 30050, Shrewd Cutter 30051 and Silent Rage 30052 ( the Transcendence form's three linked skills ) and Stable Dark Energy 30421.
-- Slots 4 and 9 stay empty ( transcendence ).

DELETE FROM unit_skill_page WHERE unit_uid IN ( 228, 229 );
UPDATE unit_skill SET slot = -1 WHERE unit_uid IN ( 228, 229 );

INSERT OR REPLACE INTO unit_skill ( unit_uid, skill_id, level, slot, csp ) VALUES
 ( 228, 30047, 1, 0, 0 ), ( 228, 30049, 1, 1, 0 ), ( 228, 30059, 1, 2, 0 ), ( 228, 30060, 1, 3, 0 ),
 ( 228, 30217, 1, -1, 0 ),
 ( 229, 30047, 0, -1, 0 ), ( 229, 30049, 0, -1, 0 ), ( 229, 30217, 0, -1, 0 ),
 ( 229, 30078, 1, 0, 0 ), ( 229, 30080, 1, 1, 0 ), ( 229, 30059, 1, 2, 0 ), ( 229, 30060, 1, 3, 0 ),
 ( 229, 30314, 1, -1, 0 ),
 ( 228, 30050, 1, -1, 0 ), ( 228, 30051, 1, -1, 0 ), ( 228, 30052, 1, -1, 0 ),
 ( 229, 30050, 1, -1, 0 ), ( 229, 30051, 1, -1, 0 ), ( 229, 30052, 1, -1, 0 ),
 ( 228, 30421, 1, -1, 0 ), ( 229, 30421, 1, -1, 0 );

INSERT INTO unit_skill_page ( unit_uid, page, slot, skill_id ) VALUES
 ( 228, 1, 0, 30047 ), ( 228, 1, 1, 30049 ), ( 228, 1, 2, 30059 ), ( 228, 1, 3, 30060 ),
 ( 228, 1, 5, 30047 ), ( 228, 1, 6, 30049 ), ( 228, 1, 7, 30059 ), ( 228, 1, 8, 30060 ),
 ( 228, 2, 0, 30217 ), ( 228, 2, 5, 30217 ),
 ( 229, 1, 0, 30078 ), ( 229, 1, 1, 30080 ), ( 229, 1, 2, 30059 ), ( 229, 1, 3, 30060 ),
 ( 229, 1, 5, 30078 ), ( 229, 1, 6, 30080 ), ( 229, 1, 7, 30059 ), ( 229, 1, 8, 30060 ),
 ( 229, 2, 0, 30314 ), ( 229, 2, 5, 30314 );

INSERT OR REPLACE INTO unit_skill_page_info ( unit_uid, page_count, active_page ) VALUES ( 228, 2, 1 ), ( 229, 2, 1 );
