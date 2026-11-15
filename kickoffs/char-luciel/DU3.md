# June character Lu/Ciel (Stage 8) - Phase DU3 - June's equip-view rewrite

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU3` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH1 review |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_EQUIP_VIEW` |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL"`) |
| Review | kickoffs/char-luciel/DU3-review.md (Opus 5.5 high; the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "Starting context" "Phase DU - The two-body engine (DU1-DU5)" "Phase NU0 - New-unit and two-body engine map (read-only)"
```
Then the planning digests in `june2017_re/char_luciel/` the task names, and the Done notes of the phases this one depends on
(their routed gaps). Verify a digest line before building on it.

## Task

Do the DU3 row of "Phase DU": the whole set, for every class (Chung / Ara / Arme overrides included), with Lu/Ciel's override from the class
map. The funcmap rows `Char Chung CH1l0` and the divergence rows marked `stage8` are the starting list; mark those rows done. Play-test: Lu and Ciel
each show their own equipment; three March characters' fashion over a broken item and a PvP item look as June's.

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

NU0 (2026-11-02), `nu0_equip_nesi.tsv` rows `DU3` and `nu0_duo_engine.tsv` rows `DU3`: `3201b930`, `31fd2f00`, `31ffe990` (form 1 builds Ciel's
look), `31fd48a0` (Now anim for a form, vslot 0x570), `31fd63c0`, `31fd9200` / `31fda280` (the `MAJOR:` / `MINOR:` equip dump; labels rest on
strings), `328825c0`, `32882540`. `31fd48a0`, `31fd63c0`, `32882540` have no March symbol. EP 28: June's second ring goes to `EP_AC_RING2` (28), March's
`EP_AC_TEMP4` has the value: register the name (`321693b0` is DU5's, it uses it).

## Amended by CH0 (2026-11-02)

- **Checklist rows** (`ch0_checklist_rows.tsv` col 5): §7g L407 item record +48 m_ItemIDForMinorUnit (429 Lu items -> Ciel dummy, 30344 -> 20003; §7g L408 item flags2 bit 2 m_bIsForMinorUnit (2,582 Ciel dummies); §7g L409 Extra accessors 32867660..32867960: unit type 10 -> key 10 (Lu) or 11 (Ciel) by ; §7z L683-688 CX2Eqip::_UpdateSkin 3295ae20 attaches every model for owner unit type 10 when t; §7aa L690-697 minor-unit attach-anim models: 3295dfa0 @3295e2cf-3295e4f6 builds the second ani; §7aa L698-701 UpdateEquipCollisionData pair (329657d0 reads basic items) is every-class, not L; §7as L994-1003 upgrade-weapon glow type 10 cases: InitUpgradeWeaponParticle 32299730, ProcessUp.

## Amended by char-luciel DU1b (2026-11-14)

- **The equip's minor-unit byte** (`CX2Eqip +0x15d4`, set when the equip is Ciel's): `CX2GUUser::UpdateShowViewListByActiveBody` (`31fd6260`,
  DU1b) reads it as a stand-in `false` (every equip Lu's). Replace the stand-in with the member when it lands here; DU1b2's equip loop in
  `CommonFrameMove` (`@3206585b`) has the same stand-in.

## Amended by char-luciel DU1b2 (2026-11-14)

- The `CommonFrameMove` equip loop's stand-in is `bEquipForMinorUnit` (`X2GUUser.cpp`, `@3206585b`): it picks the body whose render param
  the equip takes. June's loop then runs a block March lacks (`@320659e9-32065ae1`: `+0x8e64` / `+0x8e68` / `+0x8e70`, the active body's anim
  time against `+0x8e68`): identify it with the equip-view rewrite (funcmap row `32063650 @3206584a-320659e4 (DU1b2)`).

## Amended by DU1d3b (2026-11-14)

- **Second body equips:** `31fe4210` (called from `InitEqip` `32024510 @32024551` on the view list) -> `31fe3e30` -> `31fe2fb0` x8 build Lu's
  equip on `+0x87d4` and, for an item with a minor-unit id (item templet `+0x30`), Ciel's equip on `+0x8e88` (`31fac2a0`). Census rows
  `31fe2fb0` / `31fe4210`, funcmap row `31fe2fb0 / 31fe3e30 / 31fe4210 (DU1d3b)`.
- **Socket options by body:** `InitSocketData` `3206d770 @3206d8f7`: per equip, `3286a4b0` (the item's minor-unit field, 2 = both) not both and
  not the active body (`+0x8e90`) -> that equip's socket options are skipped. Port it with the field (funcmap row `3206d770 @3206d8f7 / @3206da0f (DU1d3b)`).
- The emotion setter's equip show by `+0x15d4` (`31fe0bb0`) goes with Stage 10's emotion rewrite, not here.

## Amended by DU1d5 (2026-11-14)

- `3204e4a0`'s members are declared (zeroed in the ctor, nothing writes them yet): `m_bHideEquipByBody` (+0x8fa4), `m_iHideEquipBody` (+0x8fa8),
  `m_bHideEquipByBodyApplied` (+0x8fb4), under `SERV_IRUHADEV_JUNE_CHAR_DUO_BODY` in `X2GUUser.h`. DU3 adds the window floats (+0x8fac / +0x8fb0),
  the script keys that set them, the pass itself (`CommonFrameMove` @`320661f9`) and its restore `3204df80`. `CommonRender_PrepareBody` already reads
  the three (June `31ff03d0`'s override: body 0 hidden draws the second body, 1 draws the first); JUNE_BUGS 673 is what to watch in the play-test.

## Amended by char-luciel CH1p (2026-11-15)

- `NOT_SHOW_UNIT_BY_DUOTYPE` ( the script key that sets the three members ) is read in `CommonStateStart` `32057b80` @`320618a8`-`3206194c` ( key `33478e98`,
  plain ): before the read `+0x8fac` = -9999.0 ( `3347284c` ), `+0x8fb0` = 0, `+0x8fa8` = 0, `+0x8fa4` = 0; table present -> `[1]` float `+0x8fac` ( miss -9999.0 ),
  `[2]` float `+0x8fb0` ( miss 0 ), `[3]` int `+0x8fa8` ( miss 0 ), `+0x8fa4` = 1. funcmap row `320618a8-3206194c (CH1p)`. Two Duo states use it ( `LSI_LDB_WIN`
  `{ 1, 5, 0 }`, Luciel_Duo L3920 / L4598 ). CH1p left it here because its consumer `3204e4a0` reads the equip's minor-unit byte ( `+0x15d4` ).

## Amended by char-luciel CH1q (2026-11-15)

- CH1's play-test: **Ciel draws no body** in a field ( his animation, effects and sound run ), because every equip is Lu's under the `+0x15d4` stand-in and
  Ciel's look ( `31fe4210` -> `31fe2fb0`, form 1 `31ffe990` ) is not built. This phase's play-test must show Ciel's body and equipment after a swap, then
  CH1's owed check: Ciel dies with a buff on ( `/invin 0` ), the buff is gone after the revive.
