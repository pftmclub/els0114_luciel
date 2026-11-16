# June character Lu/Ciel (Stage 8) - Phase DU5d2 - The back body's emotions, the window's equip tint, the remaining scale callers

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5d2` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5d review; june-engine R14f (`CX2Eqip::LoadAttached`: the sub-equip list the tint walks) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_VIEWER` (DU5b's) or `SERV_IRUHADEV_JUNE_CHAR_DUO_LOBBY` (DU5c2's) for the create / select screen parts |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO`) |
| Review | `kickoffs/char-luciel/DU5d2-review.md` (Opus 5.5 high; the phase writes it when done) |
| Hand-off | now |

Split from DU5d (2026-11-16): these three items reached DU5d through the DU5c2 amendment; DU5d ported the weapon effects and the checklist rows.

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "DU5c2 - Done (2026-11-16)" "DU5d - Done (2026-11-16)"
```
and the june-engine R14f Done note (`STATUS.md` names its plan doc).

## Task

- **`+0xd82` writers** (`m_bShowDuoBackUnit`, the back body's emotion gate): `PlayEmotion` `31fa52b0` (8 sites) and `31fa5c90` (3 sites, a jump table:
  case 7 sets it, cases other than 6 / 7 / 8 set it after the customize reset); the emotion frame move `31fa64f0` reads the back skin's anim end
  (`+0x2ac`) when `+0xe60` is set. DU5c2 drew the back body ( create / select screens ) but kept its emotions out.
- **`31f93920`** (equip colour by body): for each view equip, the equip's body byte (`+0x15d4`) equal to the argument -> `3295a9e0` (each sub-equip of
  the `+0x8dc` list gets the colour in its render param) and equip `+0x190` = 1, else `+0x190` = 0. Called only by `UpdateSwitchingView` (the
  character window). DU5b's stand-in comment in `UpdateSwitchingView` stays until this lands. Name `+0x190` from its readers first.
- **`31f969a0` = `CX2UnitViewerUI::SetUnitScale`**: June's 28 callers scale both bodies where March calls `GetMatrix().Scale`. DU5c2 changed
  `SetPositionOnScr`, `SelectLuciel` and the select screen; the rest ( cash shop `3250e6c0`, the other create-screen `Select*` `32551bb7`..`32556c39`,
  dungeon result `32516a70`, `324886b0`, `3223xxxx` / `3224xxxx` UI viewers, `323f23c0` square unit ) only where a two-body viewer reaches them
  (a non-Lu viewer has no minor skin, so its scale equals March's).

Done gate: on the select / create screen the back body plays its emotions as June; in the character window the hidden body's equipment takes the
back body's tint; every other class unchanged.

No hardcoded enum literals, no `June` prefix, one-line comments with the phase tag (`// char-luciel DU5d2`): CLAUDE.md. Run
`python tools/junediverge.py --list` before transcribing.

## Amended by june-engine R14f (2026-11-16)

June's `CX2UnitViewerUI::SetFixed` `31f933c0` differs from March's: with `true` it fixes a UT_ATTACH_ANIM / UT_ATTACH_NORMAL equip and also a
UT_SKIN equip that has sub-equips or attached particles (the `31fac390` test, March's `HasSubEquipOrAttachedParticle`), then calls `31f91f90( 0 )`;
with `false` it calls `31f91f90` with the viewer's shown state. March fixes the two attach types only. Port it here under this phase's flag (tsv row
`CX2UnitViewerUI::SetFixed`, verdict `stage2`, routed here; funcmap `31f933c0 (R14f)`). R14f made `CX2Eqip::SetFixed` June's (`m_bFixed` only), so
a fixed skin equip with a wing or tail draws through the equip's fixed draw once this lands.
