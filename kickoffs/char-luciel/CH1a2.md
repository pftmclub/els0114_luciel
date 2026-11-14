# June character Lu/Ciel (Stage 8) - Phase CH1a2 - The gauge's play-status pair and June's KGamePlayStatus fields (Opus)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1a2` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1a1 review |
| Flag | a new class-agnostic `SERV_IRUHADEV_JUNE_CHAR_*` flag for the `KGamePlayStatus` layout ( `CommonPacket.h` rule: also `#define`d at the end of `ServerDefine.h`, in `SECOND_HOMES` of `tools/flagcheck.py`, and in `X2ServerProtocol_2010.vcxproj`'s `X2TOOL\|Win32` defines ); the two overrides under `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` |
| Log tag | none (inert until `_LIVE`; CH1 final play-tests it) |
| Review | kickoffs/char-luciel/CH1a2-review.md (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH1a1 - Done (2026-11-14)"
grep "32a32530\|32a350d0" june2017_re/funcmap.tsv
```
CLAUDE.md "Offline-only" ( the `CommonPacket.h` exception ) and memory "CommonPacket.h precedes Always.h".

## Task

CH1a1 routed `CX2LucielGageData` slots 17 / 18 here: `GetMyPlayStatusToPacket` `32a32530` and `SetMyPlayStatusFromPacket` `32a350d0`. They read and
write `KGamePlayStatus` fields March's struct ( `KncWX2Server/Common/CommonPacket.h:2048` ) lacks: an int at `+0x5a` ( the minor gauge's max MP,
`GetMaxMp( 1 )` ), an int at `+0x5e` ( its now MP, `GetNowMp( 1 )` ), a byte at `+0x62` ( the form: my select unit `+0xe40 == 1`, written back with
`3234b8c0` = `CX2Unit::SetMinorUnitForm` ), and `m_cCharAbilType` 6 with the Combination Point in `m_iCharAbilCount` ( `+0x16`; `SetNowCombinationPoint`
then `SetMaxCombinationPoint( 10 )`, changed byte = 1 ).
- **Find June's struct.** The ctor and the Put / Get serializers of June's `KGamePlayStatus` ( locate them from a known packet handler or the
  container's serializer ), so the new members sit in June's order and the wire matches June. Name each member by what it holds ( no June names
  exist ); `CAC 6` gets a `KGamePlayStatus::CHAR_ABIL_TYPE` member under the flag with value 6 ( check June's base `GetMyPlayStatusToPacket`
  `32a32200` and the other classes' getters for the 0-5 values ).
- **Port the two overrides whole** ( base call order as June: `32a32530` calls the base `32a32200` at the end through `FUN_32a32200( status )` ).
- **Emulator consistency** ( one packet = one transaction ): `X2OfflineServer::MakeGamePlayStatus`, `X2OfflineStatTable.cpp`'s CAC map ( add
  `UT_LUCIEL` -> 6 ), `Handlers_Field.cpp`'s `SaveGamePlayStatus` path. Decide with the `X2OfflineDB` schema whether the minor MP needs a column; a
  schema change is a forward-only migration ( back up `els_db.sql` first ) and goes back to the user only if it touches existing rows.

June's numbers and quirks verbatim ( `JUNE_BUGS.md` rows ), a funcmap row per function ( `Char Luciel CH1a2` ). Done gate: `cl /P` on one file per
project shows the layout flag on in both `X2Lib` and `X2ServerProtocol`; `/Zs` probes of `X2GageData.cpp`, `X2OfflineServer.cpp`, a
`X2ServerProtocol` file 0 errors; `flagcheck` PASS ( the new flag with its second home ); `junediverge.py --check` and `enumscan.py --check` PASS;
full build skipped ( no play-test ).
