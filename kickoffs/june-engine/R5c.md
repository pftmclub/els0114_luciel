# June engine remainder - Phase R5c - after-image saved enable byte `+0x1d1`

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase june-engine R5c` |
| Plan doc | `JUNE_ENGINE_REMAINDER_PHASES.md` (branch `main`) |
| Depends on | R5b review (done 2026-11-15) |
| Flag | `SERV_IRUHADEV_JUNE_AFTER_IMAGE_SAVED_ENABLE` (+ `_DEBUG` if the phase logs) |
| Log tag | `[JUNE_ENGINE] AFTERIMG` (`python tools/logs.py --grep "JUNE_ENGINE] AFTERIMG"`) |
| Review | `kickoffs/june-engine/R5c-review.md` (the phase writes it when done) |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_ENGINE_REMAINDER_PHASES.md "Rules" "Starting context" "Phase R5b -- Review" "Phase R5c - after-image saved enable byte"
grep -n "(R5b review)\|3227b220 / 3227a2f0 (R5b)\|^32c85c00\|^32d08d70\|^32cc7b90\|^31fd8230" june2017_re/funcmap.tsv
grep -n "After-image byte\|7al\." JUNE2017_CHARACTER_PORT_CHECKLIST.md
```

## Task

June keeps a saved copy of each after-image's enable byte (`+0x194`) at `CKTDGXSkinAfterImage +0x1d1`. The class `AfterImageFrameMove` overrides (user vslot `0x850`) save and
blank it in their wait states and copy it back after; `CX2GameUnit::EraseAfterImageByBuff` (`3227a2f0 @3227a37f-3227a393`) clears it on both after-images, so a buff's after-image
that ends stays off. March's Elsword, Rena and Aisha ports keep a class-held stand-in (`m_bAfterImageSaved`) that nothing clears: after a buff after-image ends, the next non-wait
frame turns it back on.

- **The member.** Read June's `CKTDGXSkinAfterImage` ctor for `+0x1d1`'s start value, then add it to March's `CKTDGXSkinAfterImage` (`KTDXLIB/KTDGXSkinAfterImage.h/.cpp`) with a getter
  and setter in the class's own naming. `322727c0` (sets it on both after-images) has no caller in June: do not port it.
- **Erase.** `EraseAfterImageByBuff`: clear the byte on `m_pAfterImage` and `m_pMinorUnitAfterImage` (NULL-tested, as June), inside the found branch, after R5b's `m_bAfterImageEnableRequested = false`.
- **The readers.** `CX2GUElsword_SwordMan::AfterImageFrameMove` (`32d08d70`), `CX2GULire_ElvenRanger::AfterImageFrameMove` (`32c85c00`), `CX2GUArme_VioletMage::AfterImageFrameMove`
  (`32cc7b90` / `32cc7c30`): read and write the after-image's byte in place of `m_bAfterImageSaved`, June's body under the new flag, the stand-in in `#else`. Re-read each listing:
  June saves only when `+0x194` is 1, restores only when it is 0, and touches the second after-image `+0x6758` too (the ports say the second one is a stand-in; follow June if the
  member makes it portable). The base `31fd8230` class-13 toggle stays with checklist 7al.
- **Checklist.** Mark the row "After-image byte `+0x1d1`" ported.
- **Play-test.** Find a buff March data reaches that calls `CreateAndInsertAfterImageByBuff` (an after-image buff factor; `/additem` or a skill that grants it), on Elsword, Rena or Aisha
  (whichever class's `_LIVE` flag is on). Steps: buff on, stand still in wait for 2 s, move, wait for the buff to end. Expected: the after-image goes off when the buff ends and stays
  off while moving. Before the fix it comes back. A throttled `[JUNE_ENGINE] AFTERIMG` line on save, restore and erase-clear (under `_DEBUG`) proves the path ran.

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
