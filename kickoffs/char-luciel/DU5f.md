# June character Lu/Ciel (Stage 8) - Phase DU5f - Each body's own sockets (the type-10 aggregate, HUD second gage, set count)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5f` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5e review (split from DU5e, 2026-11-15; the second game stat is DU5e's) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_SOCKET` (new, `Always.h` only unless a packet field turns up: then the `CommonPacket.h` rule, as DU5e) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | `kickoffs/char-luciel/DU5f-review.md` (Opus 5.5 high; the phase writes it when done) |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Phase DU - The two-body engine (DU1-DU5)" "DU1c2 - Done (2026-11-14)" "DU3c - Done (2026-11-15)" "DU5e - Done (2026-11-15)"
```
Then `kickoffs/char-luciel/DU5.md` sections "Amended by char-luciel DU1c2", "Amended by DU1d3b" and "Amended by DU3c" (this phase's addresses),
and `grep "(DU1c2)\|(DU1d3b)\|(DU3c)\|(DU5e)\|(S7k)\|(S7m)" june2017_re/funcmap.tsv`.

## Task

Split from DU5e (2026-11-15); the amendments name every address:
- **Type-10 socket aggregate:** `CX2Unit::Init` `32363ca0` builds `CX2SocketItem::SocketData_DuoUnit` (`32091730`, 0x488, vtable `33478ab0`, two
  `KProtectedType<CX2Stat::MPStat>` pairs at +0x468 / +0x478 read by vslots 3 / 4 `32091b10` / `32091b60`; funcmap 941) for `UT_LUCIEL` instead of
  `SocketData` (0x468, `32091080`). Its vslots 0xc / 0x10 take a body; `InitSocketData`'s modes 0 / 1 / 2 and the group filter `3286a4b0`
  (`CX2Inventory::GetEquipGroup`) act only with it. March has no class for it.
- **HUD second gage:** `CX2Unit::ResetHudMaxHpMp` `3235a160 @3235a29e-3235a339` (one-line stand-in comment in `X2Unit.cpp`): for `UT_LUCIEL`, MP 2 =
  `ResetMaxMp( 300, 1 )` x ( 1 + aggregate vslot 0xc( 0 ) + ability x 0.01 ) + vslot 0x10( 0 ), sent as `3248a800( uid, mp, 1 )`
  (`CX2GageManager::SetMaxMp( uid, mp, idx )`, S7g). The emulator's `KGamePlayStatus` minor-unit max MP is a CH1a2 stand-in (`ONE_CHARGE * 3`).
- **Set-socket count** `CX2Inventory 3286a6b0( set, body )` (equipped parts of a set that body wears) and its readers: `InitSocketData` `3206d770
  @3206da0f` (the user's), and the character window's other-form set sum `3291ba40 @3291bf2d` (stand-in comment in `X2UICharInfo.cpp`). The three
  inside the item `+0xac` set-socket list (`328842f0 @32885558`, `32365d30 @32365fe7 / @3236616d`, `328836c0 @328837a5`) stay with that list
  (Stage 16, funcmap S7m): port only the count and pass the form where March has a site.
- **Per-body socket record:** `InitSocketData` `3206d770 @3206dbd3-3206dc6a` (unit type 10, `SocketData*` argument non-null) passes each equip's
  socket vectors with its body (`GetEquipGroup`) to `3271a060`, a `SocketData` method summing the options' `+0x304` / `+0x23c` into per-body
  accumulators (`+0x468..` body 0 / both, the other set body 1). DU3c ported only the skip that follows (`@3206dc6a`). Port the record and its readers.
- **Other players' form** (from DU5): the user-info form bytes (`32367330` / `32367da0` / `32368300`) only if DU5c routed them here.

Done gate: the HUD shows Ciel's MP gauge sized by her own sockets / accessories (an MP-boosting item on Ciel only raises Ciel's gauge), and the
character window's set count follows the shown body.

No hardcoded enum literals, no `June` prefix, `junediverge.py --list` before transcribing (CLAUDE.md rules, as in every DU kickoff).
