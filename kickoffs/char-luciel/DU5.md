# June character Lu/Ciel (Stage 8) - Phase DU5 - Two bodies in the character window and unit viewer

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU4 review |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEW` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |
| Review | kickoffs/char-luciel/DU5-review.md (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Starting context" "Phase DU - The two-body engine (DU1-DU5)" "Phase NU0 - New-unit and two-body engine map (read-only)"
```
Then the planning digests in `june2017_re/char_luciel/` the task names, and the Done notes of the phases this one depends on
(their routed gaps). Verify a digest line before building on it.

## Task

Do the DU5 row of "Phase DU" from `nu0_duo_engine.tsv` / `charinfo_functions.tsv`. The MsgProc click switch replaces CharInfo's logged
`unit click switching` stub. The form packets get emulator handlers that persist the form. Guild-agit packets only if NU0 found them reachable.
Play-test: switch the shown body in the character window, equip an item on each body, see the conflict message on a wrong drop, relog; both bodies
on the creation-screen preview and the select screen where June shows two.

No hardcoded enum literals (user, 2026-10-26; CLAUDE.md): an id that has a name in an Enum (June's
`Enum.lua`, or a C++ enum) is never written as a number, in any new or touched code: use the member, and
register it under a `SERV_IRUHADEV_` flag with June's value and name if C++ lacks it. A value with no Enum
entry stays a number with a one-line comment. The one exception is a table whose rows are the data itself
(a `.tsv` of ids), not code.

No `June` prefix (user, 2026-10-28; CLAUDE.md): June's body goes inside March's own function under the flag,
March's in `#else`. A function March lacks gets a name in the class's own pattern, never with `June` in it.
Every name (locals and constants too) says what it is in context: no Ghidra address, phase tag or `June` in an identifier.
Call a March function from June code only if June's namesake does the same; otherwise replace March's definition.
Run `python tools/junediverge.py --list` before transcribing; a newly found divergent function gets its `june_march_divergent.tsv` row and the replacement in the same commit.

## Amended by NU0

NU0 (2026-11-02), `nu0_equip_nesi.tsv` rows `DU5`, `nu0_duo_engine.tsv` rows `DU5` (123), `nu0_type_switches.tsv` rows `DU5` (6):
- **Form packets:** `0x4d9` is fire-and-forget, a 1-byte form, sent by `32916e10` / `32916e70` through `32920900`, no ACK. `0x4da` is received in
  `CX2TFieldGame::EventProcess` `323bda90`: `{ UidType unit uid, bool form }` -> `323acfb0`. March has no event, struct or handler. Events go
  before `EGS_CLIENT_EVENT_ID_END` in `EventID_Client.h`, the structs in `ClientPacket.h` (both before `Always.h`: second home in `ServerDefine.h`
  + `SECOND_HOMES`). June has no name table: names such as `EGS_CHANGE_UNIT_FORM_REQ` / `_NOT`. Where the form persists across a relog (June's
  `KUnitInfo` may carry it: read `3235eb20` first) is DU4's one migration, not a second one.
- **Guild agit is not reachable offline** (no agit state, village, house, events or `X2Lib/Offline` handler): `0x4fd` / `0x4fe` are dropped.
- **Unit form byte** `+0xe40`: `3234b8c0` (SetForm) is the only plain writer; the other readers sit in `3234f...`, `3236....`, `323fad80`,
  `3291....`. 50 rows tagged `out-of-scope "other struct?"`: re-check each (viewer / UI members, stack locals).
  Also `32713c10 @32713eaa` (2ND class-change guide, minor flag to `3220bdb0`): `X2LVUpEventMgr.cpp` `InitClassChangeGuide` passes `false` as a stand-in (char-ara CHNb review, 2026-11-11).
- **Viewer** `+0xd80` duo flag, `+0xd7c` minor skin: `X2UnitViewerUI.cpp:960` (`31f93050`), `:6255` (`31f961a0` case 10), `:8681` (also unit
  `+0xe40`); `:7938` is dead (live June body has a stand-in). Square unit `X2SquareUnit.cpp:220`: June's 24-name mesh table `31ee3470` (array
  `3372c1e8`) by form (`Mesh_Lu_Base_SD.X` / `Mesh_Ciel_Base_SD.X`): June's body, not a new case.
- **Both bodies** on the create screen (SelectLuciel `@32555fe8`) and the select screen (`32564400`, `3255e570`), with `31edf260( class, 1 )`
  (NU4 ports it). `321693b0` drops a second ring on EP 28 (`EP_AC_RING2`, DU3 registers the name).

## Amended by CH0 (2026-11-02)

- **Checklist rows** (`ch0_checklist_rows.tsv` col 5): §3 L153 CharCustomData (22 Lu/Ciel rows in June); customize reader reads DUO_TYPE; §3 L153 UnitViewerEffectSetTable (Lu/Ciel duo rows); §7e L368 duo minor model in the unit viewer: +0xd7c skin anim, +0xd80-0xd82 duo type / sh; §7v L649-653 IS_FOR_MINOR_UNIT slot key (X2SlotItem.cpp:53-65 LuaGetValue, June 32e284b0 @32e; §7aj L870-872 unit form change 3234d930 (my unit +0x28 / +0x2c match, 3214f050, 328e8cd0( form; §7as L1001-1012 unit viewer glow: 31f92910, 31f9a900, 31f961a0, frame move 31f93050 (+0xd80), vi.

## Amended by char-luciel DU1c (2026-11-14)

- **The second game stat's writer.** DU1c added `CX2Unit::UnitData::m_MinorUnitGameStat` (June unit data +0x1a0) and `InitStat` reads it while
  Ciel is active, but nothing fills it yet (Ciel falls back to `m_Stat`). June's writer is `UnitData::SetGameStats( KStat first, KStat second )`
  `323571c0` (ret 0x38), called from seven packet sites (`31ed46e0`, `32360090`, `31ed57d0`, `31ed4c80`, `31f1ed70`, `32461e80`, `32473f70`): the
  June packets carry a second `KStat` beside `m_kGameStat`. Port the writer, the packet fields (the `CommonPacket.h` rule applies), and the
  emulator's per-body numbers (the minor unit's equipment, after DU3 / DU4) here.

## Amended by char-luciel DU1c2 (2026-11-14)

- **The type-10 socket aggregate.** `CX2Unit::Init` `32363ca0` makes a 0x488-byte aggregate (`32091730`) for `UT_LUCIEL` instead of March's
  `SocketData` (0x468, `32091080`); its vslots 0xc / 0x10 take a body (0 / 1) and `InitSocketData`'s modes 0 / 1 / 2 and the group filter
  `3286a4b0` (item: 0 = Lu, 1 = Ciel, 2 = both) act only with it. March has no class for it; port it here.
- **The HUD's second gage** (`CX2Unit::ResetHudMaxHpMp`, June `3235a160` @`3235a29e-3235a339`, one-line stand-in comment in `X2Unit.cpp`):
  for `UT_LUCIEL`, MP 2 = `ResetMaxMp( 300, 1 )` x ( 1 + aggregate vslot 0xc( 0 ) + ability x 0.01 ) + vslot 0x10( 0 ), sent as
  `3248a800( uid, mp, 1 )`, the gage manager's second MP (CH1a0's Lu/Ciel gauge class). DU1c2 ported the two form reads of that function.
- **The set-socket form reads.** `GetStat` `328842f0` @`32885558`, `InitSocketData` `32365d30` @`32365fe7` / @`3236616d` and the used-socket
  count `328836c0` @`328837a5` all pass ( my select unit form == 1 ) to `CX2Inventory` `3286a6b0( set id, body )`, which counts the equipped
  parts of a set that body wears (filter `3286a4b0`). All three sit inside the item +0xac "set complete" socket list, which March's
  `ItemData` lacks and nobody has ported (funcmap S7m: "Stage 16"). Whoever ports that list passes the form; the count helper goes here.

## Amended by DU1d3b (2026-11-14)

- `InitSocketData` `3206d770 @3206da0f`: the set-socket count `3286a6b0( set, my unit +0xe40 )` reads the unit form; it is the same count helper
  as above (funcmap row `3206d770 @3206d8f7 / @3206da0f (DU1d3b)`).

## Amended by char-luciel CH1a0 (2026-11-14)

- Three of the packet / unit-viewer sites also read the transform pair `31fc9d90` ( `CX2GUUser::IsTransformed`, CH1a0 ): `31f1ed70` @`31f1f1d8`,
  `32461e80` @`324620b9` and `32195e40` @`3219618d` skip vslots 0x63c / 0x630 while a body is transformed. Port that test with the site.

## Amended by DU3c (2026-11-15)

- **Per-body socket record:** June `InitSocketData 3206d770 @3206dbd3-3206dc6a` (unit type 10 only, `SocketData*` argument non-null) passes each
  equip's four or five socket vectors (random, templet, data, set when `3286a6b0` says so) with its body (`CX2Inventory::GetEquipDuoType`, DU3c) to
  `3271a060`, a `SocketData` method that adds the options' `+0x304` / `+0x23c` values to per-body accumulators (`+0x468..` for body 0 / both, the
  other set for body 1). DU3c ported only the skip that follows (`@3206dc6a`); port the record and its readers here, with the set-socket count.
