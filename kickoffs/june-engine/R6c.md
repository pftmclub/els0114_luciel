# June engine remainder - Phase R6c - The equip's enchant-particle list (SetEnchantParticleShow)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort (map, then port), then type `/x2-phase june-engine R6c` |
| Plan doc | `JUNE_ENGINE_REMAINDER_PHASES.md` (branch `main`) |
| Depends on | R6b (same file, `X2Eqip.*`); routed from june-march-divergence D1-3e (tsv row 263) |
| Flag | `SERV_IRUHADEV_JUNE_EQUIP_ENCHANT_PARTICLE` |
| Log tag | `[JUNE_ENGINE]` (add a sub-tag only if a check needs a log line) |
| Review | `kickoffs/june-engine/R6c-review.md` (the phase writes it when done) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_MARCH_DIVERGENCE_PHASES.md "D1-3e Done (2026-10-29, Opus 5.5; one site under D1-3c's \`WEAPON_EFFECT_SHOW\`; viewer family -> D1-3g, 263 -> june-engine R6c)"
python tools/docsection.py JUNE_ENGINE_REMAINDER_PHASES.md "Rules" "Starting context"
```

and row 263 of `tools/data/june_march_divergent.tsv` (`sed -n '263p'`).

## Task

March keeps each weapon's attribute-enchant particles as three fixed handles on `CX2GameUnit::Weapon` (`m_ahEnchantWeapon[0..2]`), shown and
hidden by `Weapon::SetEnchantParticleShow` (`X2GameUnit.cpp:10686`; callers `X2GameUnit.cpp:9651` / `:11499`, `X2GUUser.cpp:2266` / `:21033`,
`X2GuEve.cpp:6911`). June keeps them in a list of entries on the owning equip object (`+0x8f0`): `3295cd30` is `ADD ECX,0x8f0; JMP 3295bf90`, and
`3295bf90( bShow, index, -1 = all )` sets each entry's show byte `+0x34`, finds its sequence (`+0x2c`) in the major or minor particle system by the
type byte `+0x1c`, and flips the sequence's show flag `+0x172` (calling vslot `0x28` when the visible state changes, with `31fae370`).

1. **Map** the list: its owner class (the `3295xxxx` range is `CX2Eqip`; confirm from the ctor), the entry struct, its writers (`3295d960`,
   `3295ddf0`, `32961c80`) and their callers, and which March functions create / destroy / move the three weapon handles today.
2. **Port** the container and every reader and writer whole under the flag (no partial updates), March's in `#else`, so
   `Weapon::SetEnchantParticleShow`'s callers (June `329c5bc0` CX2GUEve::CommonFrameMove, `3200a590` the CX2GUUser invisibility setter,
   `329afaa0`) reach June's list. If the map shows the list is the whole attribute-enchant create path, port that path in this phase or split.
3. Close tsv row 263 (`june_march_divergent.tsv`) with this phase's commit; funcmap rows `(R6c)`; `MODS.md`; `JUNE_BUGS.md` rows for June quirks.

No hardcoded enum literals (user, 2026-10-26; CLAUDE.md). No `June` prefix (user, 2026-10-28).

## Amended by june-engine R14d (2026-11-15)

The `+0x8f0` list is `CX2Eqip::m_AttachedParticleGroup` in June (ctor `3295d8a0`, dtor `3295dad0`), a reworked attached-particle group. Port it
whole here with the enchant entries:
- the group ctor's remaining members (`+0x24..+0x2c` scale 1.0, `+0x34` unit skin anim ptr, eqip `+0x928` = a ctor byte); R14d ported the owner
  UID `+0x10`, fixed position `+0x18`, source `+0x30` and `329645a0` (`AttachedParticleGroup::GetOwnerLandPosition`);
- June's group frame move `329646c0` (empty bone -> the owner's land position, `TraceMatrixLandPos`, `TRACE_MATRIX_RT_ATTACH_FRAME`, the group
  scale into each sequence) in place of March's `AttachedParticleGroup::OnFrameMove` (tsv row verdict `stage2`; R14d calls March's as a stand-in);
- `AttachParticle` `32960ad0` whole (June's entry struct; `RemoveObjectChain` + seq `+0x45a` when `+0x15d5` is clear, @`32960c7a` / @`32960d33`);
  close its tsv replace row;
- the anim-time sync `3295ce00( bRebuild, fTime )` that R14d's `OnFrameMove` leaves as two stand-in comments (after the normal and the hyper attach
  anim; rebuild only for item 280322 when anim `+0x2ac` is set): identify anim `+0x2ac` / `+0x218` and sequence `+0x464` first;
- compare the whole eqip ctor `32961280` while there: June stores `+0x190` = 0 always and its dtor deletes no dummy item (March's `m_bDummyBasic` /
  `m_pDummyItem`); say whether June dropped the dummy-item path and route it.
- **Proof item (R14d play-test, 2026-11-15):** Elsword's Miracle Alchemist Light 85811 shows nothing in March: its six particles sit on bone
  `TraceLandPos`, which March's `AttachParticle` cannot find in the item mesh and deletes. June keeps the entry frameless (`32960d65`) and the group
  frame move draws it at the owner's land position (`3296479f` -> `329645a0`, already ported by R14d). The R6c play-test must show this light at the feet.
