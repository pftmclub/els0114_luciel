-- char-luciel CH2d2 play-test save: run with the game closed
--   python tools/elsdb.py x --file kickoffs/char-luciel/CH2d2_save.sql
-- LuDbA ( unit 202, Lu / Diablura ): the four base skills on page 1 ( A S D C ) and page 2 ( Q W E R ).
-- LuDbB ( unit 203 ): the same four as their upgrades ( base level 0 ).
-- Both learn Death Touch, Mortal Fear and Dark Plasma ( the transformed form's three linked skills ).

DELETE FROM unit_skill_page WHERE unit_uid IN ( 202, 203 );
UPDATE unit_skill SET slot = -1 WHERE unit_uid IN ( 202, 203 );

INSERT OR REPLACE INTO unit_skill ( unit_uid, skill_id, level, slot, csp ) VALUES
 ( 202, 30042, 1, 0, 0 ), ( 202, 30043, 1, 1, 0 ), ( 202, 30215, 1, 2, 0 ), ( 202, 30216, 1, 3, 0 ),
 ( 202, 30044, 1, -1, 0 ), ( 202, 30045, 1, -1, 0 ), ( 202, 30046, 1, -1, 0 ),
 ( 203, 30042, 0, -1, 0 ), ( 203, 30043, 0, -1, 0 ), ( 203, 30215, 0, -1, 0 ), ( 203, 30216, 0, -1, 0 ),
 ( 203, 30076, 1, 0, 0 ), ( 203, 30077, 1, 1, 0 ), ( 203, 30219, 1, 2, 0 ), ( 203, 30220, 1, 3, 0 ),
 ( 203, 30044, 1, -1, 0 ), ( 203, 30045, 1, -1, 0 ), ( 203, 30046, 1, -1, 0 );

INSERT INTO unit_skill_page ( unit_uid, page, slot, skill_id ) VALUES
 ( 202, 1, 0, 30042 ), ( 202, 1, 1, 30043 ), ( 202, 1, 2, 30215 ), ( 202, 1, 3, 30216 ),
 ( 202, 2, 5, 30042 ), ( 202, 2, 6, 30043 ), ( 202, 2, 7, 30215 ), ( 202, 2, 8, 30216 ),
 ( 203, 1, 0, 30076 ), ( 203, 1, 1, 30077 ), ( 203, 1, 2, 30219 ), ( 203, 1, 3, 30220 ),
 ( 203, 2, 5, 30076 ), ( 203, 2, 6, 30077 ), ( 203, 2, 7, 30219 ), ( 203, 2, 8, 30220 );

UPDATE unit_skill_page_info SET page_count = 2, active_page = 1 WHERE unit_uid IN ( 202, 203 );
