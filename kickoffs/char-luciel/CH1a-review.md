# June character Lu/Ciel (Stage 8) - Phase CH1a review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1a review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH1a done (its Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE`, `SERV_IRUHADEV_JUNE_ENUM_NAMES` (the registrations), `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` (`//#define`) |
| Log tag | none (inert) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH1a - Done (2026-11-14)"
```
Then `grep "(CH1a)" june2017_re/funcmap.tsv` (19 rows), `JUNE_BUGS.md` rows 679-681, `june2017_re/char_luciel/ch1_offsets.tsv` (the 123 class rows).

## Task

Review CH1a's commit against June (Ghidra, read-only):
- **Members.** Every ctor store `32b1bd90` ( both halves: the member-initializer run and the body's re-stores ) against the header's initialisers,
  with the x87 stack and its `fxch`es followed ( `0x334380f0` is -1.0, not 0 ); the nine members June leaves unset ( JUNE_BUGS 681 ); each type
  against its users ( the element sizes of the eleven vectors, the map, the two protected floats, `sLockOnSystemPtr` ). Spot-check five provisional
  names in `ch1_offsets.tsv` against a user and rename any that a body contradicts.
- **Dtor** `32b39380`: order of the calls, the four NPC ids, BTI 474 / 481, the guide call, the window delete stand-in ( CH2d20 ).
- **Bodies:** `32aeaa10` ( the jump tables `32aeafa8` / `32aeaf54` and `32aeb07c`: every case -> state, the Ciel test placement, the two branches with
  their own tests ), `32b14860`, `32b14f70`, `32b3acf0`, `32aeb1e0`, `32b0fa00`, `32b14db0` ( a hit counter, not a reference ), `32b20100`, `32b34920`,
  `32b35540`, `32ae6130`, and InitState's `+0x8ac8` switch. Check the spec-A `+ 1.0` reading in `UpCombinationPoint` against Eve's port and June.
- **State enum.** 531 names over 493 ids from `ch0_reg.tsv` + `ch0_state_skill.tsv` + the setter's extra states; the five multi-id names and the 22
  unnamed ids ( Done note ).
- **Registrations.** 188 Lu/Ciel `SKILL_ID` names ( `X2SkillTree.h` ), BTI 474 / 481 / 482 ( `X2BuffFactor.h` ), `NUI_SA_LDN_GARDEN_OF_HAURES` ( `X2UnitManager.h` ):
  values against June's `Enum.src.lua`.
- **Routing.** CH1a4 ( the transform-skin trio ), CH2d20, CH1m, CH4c, CH1 final amendments are in their kickoffs.

Spot-check 3 items directly in Ghidra; if one fails, re-check the whole phase. Hygiene: `python tools/flagcheck.py <flag> --range <CH1a commit>^..<CH1a
commit>` for the three flags ( the new `.cpp` is the precedented new-file FAIL; `Always.h`'s compound `#if` guards are the tool's Unevaluable ERROR ),
`git log -S<flag> <commit>..HEAD`, `python tools/junediverge.py --check-all`, `python tools/enumscan.py --check X2Lib/X2GULuciel.h X2Lib/X2GULuciel.cpp`.

Write "CH1a - Review" into the plan doc, update the CH1a row in STATUS.md (done; CH1b and CH1a4 unblocked), and commit. No build unless a fix lands
(then a one-file `/Zs` probe; no play-test: the code is inert).
