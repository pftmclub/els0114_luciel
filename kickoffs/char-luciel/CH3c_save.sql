-- char-luciel CH3c play-test save: run with the game closed
--   python tools/elsdb.py x --file kickoffs/char-luciel/CH3c_save.sql
-- LuDnA ( unit 228, Lu / Demonio, level 70 ): the base skills, 4 per page on keys A S D C ( slots 0-3 ) and the same 4 on Q W E R ( slots 5-8 ).
--   page 1: Desperado 30047, Sinister Mind 30048, Garden of Haures 30053, Abyss Extinction 30085
--   page 2: Rapid Impact 30217, Punisher 30218, Rapid Fury 30306
-- LuDnB ( unit 229 ): the enhanced skills ( base level 0 ), the same shape.
--   page 1: Desperado+ 30078, Sinister Mind+ 30079, Garden of Haures+ 30081, Abyss Extinction 30085
--   page 2: Rapid Impact+ 30314, Punisher+ 30315, Rapid Fury 30306
-- Stable Dark Energy 30421 ( passive, level 1 ) on both: a Sinister Mind cast in the air starts its fly state.
-- Transcendence 30049, Abyss Sniping 30059 and Demonic Rush 30060 are held for CH3c2 ( not on any page ). Slots 4 and 9 stay empty ( transcendence ).

DELETE FROM unit_skill_page WHERE unit_uid IN ( 228, 229 );
UPDATE unit_skill SET slot = -1 WHERE unit_uid IN ( 228, 229 );

INSERT OR REPLACE INTO unit_skill ( unit_uid, skill_id, level, slot, csp ) VALUES
 ( 228, 30047, 1, 0, 0 ), ( 228, 30048, 1, 1, 0 ), ( 228, 30053, 1, 2, 0 ), ( 228, 30085, 1, 3, 0 ),
 ( 228, 30217, 1, -1, 0 ), ( 228, 30218, 1, -1, 0 ), ( 228, 30306, 1, -1, 0 ),
 ( 229, 30047, 0, -1, 0 ), ( 229, 30048, 0, -1, 0 ), ( 229, 30053, 0, -1, 0 ), ( 229, 30217, 0, -1, 0 ), ( 229, 30218, 0, -1, 0 ),
 ( 229, 30078, 1, 0, 0 ), ( 229, 30079, 1, 1, 0 ), ( 229, 30081, 1, 2, 0 ), ( 229, 30085, 1, 3, 0 ),
 ( 229, 30314, 1, -1, 0 ), ( 229, 30315, 1, -1, 0 ), ( 229, 30306, 1, -1, 0 ),
 ( 228, 30421, 1, -1, 0 ), ( 229, 30421, 1, -1, 0 );

INSERT INTO unit_skill_page ( unit_uid, page, slot, skill_id ) VALUES
 ( 228, 1, 0, 30047 ), ( 228, 1, 1, 30048 ), ( 228, 1, 2, 30053 ), ( 228, 1, 3, 30085 ),
 ( 228, 1, 5, 30047 ), ( 228, 1, 6, 30048 ), ( 228, 1, 7, 30053 ), ( 228, 1, 8, 30085 ),
 ( 228, 2, 0, 30217 ), ( 228, 2, 1, 30218 ), ( 228, 2, 2, 30306 ),
 ( 228, 2, 5, 30217 ), ( 228, 2, 6, 30218 ), ( 228, 2, 7, 30306 ),
 ( 229, 1, 0, 30078 ), ( 229, 1, 1, 30079 ), ( 229, 1, 2, 30081 ), ( 229, 1, 3, 30085 ),
 ( 229, 1, 5, 30078 ), ( 229, 1, 6, 30079 ), ( 229, 1, 7, 30081 ), ( 229, 1, 8, 30085 ),
 ( 229, 2, 0, 30314 ), ( 229, 2, 1, 30315 ), ( 229, 2, 2, 30306 ),
 ( 229, 2, 5, 30314 ), ( 229, 2, 6, 30315 ), ( 229, 2, 7, 30306 );

INSERT OR REPLACE INTO unit_skill_page_info ( unit_uid, page_count, active_page ) VALUES ( 228, 2, 1 ), ( 229, 2, 1 );
