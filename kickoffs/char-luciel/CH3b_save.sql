-- char-luciel CH3b play-test save: run with the game closed
--   python tools/elsdb.py x --file kickoffs/char-luciel/CH3b_save.sql
-- LuNoA ( unit 225, Lu / Noblesse, level 70 ): the base skills, 4 per page on keys A S D C ( slots 0-3 ) and the same 4 on Q W E R ( slots 5-8 ).
--   page 1: Swift Smash 30210, Spectral Spear 30024, Shadow Bolt 30016, Twilight Judgement 30022
--   page 2: Soul Lock 30057, Magdonic Cannon 30058, Rain of Chaos 30084, Fantasie Impromptu 2nd Movement 30023
--   page 3: Summon Familiar 30208, Soul Spread 30305
-- LuNoB ( unit 226 ): the enhanced skills ( base level 0 ), the same shape.
--   page 1: Swift Smash+ 30312, Spectral Spear+ 30074, Shadow Bolt+ 30071, Twilight Judgement+ 30072
--   page 2: Fantasie Impromptu 2nd Movement+ 30073, Summon Familiar+ 30311
-- Slots 4 and 9 stay empty ( transcendence ).

DELETE FROM unit_skill_page WHERE unit_uid IN ( 225, 226 );
UPDATE unit_skill SET slot = -1 WHERE unit_uid IN ( 225, 226 );

INSERT OR REPLACE INTO unit_skill ( unit_uid, skill_id, level, slot, csp ) VALUES
 ( 225, 30210, 1, 0, 0 ), ( 225, 30024, 1, 1, 0 ), ( 225, 30016, 1, 2, 0 ), ( 225, 30022, 1, 3, 0 ),
 ( 225, 30057, 1, -1, 0 ), ( 225, 30058, 1, -1, 0 ), ( 225, 30084, 1, -1, 0 ), ( 225, 30023, 1, -1, 0 ),
 ( 225, 30208, 1, -1, 0 ), ( 225, 30305, 1, -1, 0 ),
 ( 226, 30210, 0, -1, 0 ), ( 226, 30024, 0, -1, 0 ), ( 226, 30016, 0, -1, 0 ), ( 226, 30022, 0, -1, 0 ),
 ( 226, 30023, 0, -1, 0 ), ( 226, 30208, 0, -1, 0 ),
 ( 226, 30312, 1, 0, 0 ), ( 226, 30074, 1, 1, 0 ), ( 226, 30071, 1, 2, 0 ), ( 226, 30072, 1, 3, 0 ),
 ( 226, 30073, 1, -1, 0 ), ( 226, 30311, 1, -1, 0 );

INSERT INTO unit_skill_page ( unit_uid, page, slot, skill_id ) VALUES
 ( 225, 1, 0, 30210 ), ( 225, 1, 1, 30024 ), ( 225, 1, 2, 30016 ), ( 225, 1, 3, 30022 ),
 ( 225, 1, 5, 30210 ), ( 225, 1, 6, 30024 ), ( 225, 1, 7, 30016 ), ( 225, 1, 8, 30022 ),
 ( 225, 2, 0, 30057 ), ( 225, 2, 1, 30058 ), ( 225, 2, 2, 30084 ), ( 225, 2, 3, 30023 ),
 ( 225, 2, 5, 30057 ), ( 225, 2, 6, 30058 ), ( 225, 2, 7, 30084 ), ( 225, 2, 8, 30023 ),
 ( 225, 3, 0, 30208 ), ( 225, 3, 1, 30305 ),
 ( 225, 3, 5, 30208 ), ( 225, 3, 6, 30305 ),
 ( 226, 1, 0, 30312 ), ( 226, 1, 1, 30074 ), ( 226, 1, 2, 30071 ), ( 226, 1, 3, 30072 ),
 ( 226, 1, 5, 30312 ), ( 226, 1, 6, 30074 ), ( 226, 1, 7, 30071 ), ( 226, 1, 8, 30072 ),
 ( 226, 2, 0, 30073 ), ( 226, 2, 1, 30311 ),
 ( 226, 2, 5, 30073 ), ( 226, 2, 6, 30311 );

INSERT OR REPLACE INTO unit_skill_page_info ( unit_uid, page_count, active_page ) VALUES ( 225, 3, 1 ), ( 226, 2, 1 );
