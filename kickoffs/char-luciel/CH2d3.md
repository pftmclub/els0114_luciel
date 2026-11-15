# June character Lu/Ciel (Stage 8) - Phase CH2d3 - Altar of Evil form: model swap and skill slot UI (Opus)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH2d3` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH2d2 (done, play-tested) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` (class code), `SERV_IRUHADEV_JUNE_CHAR_TRANSFORM_LINK` (the window, `CX2GUUser`), `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE` (hooks inside March functions) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL SKILL`) |
| Review | covered by `CH2d2-review.md` (amended: it now covers CH2d, CH2d2, CH2d3) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH2d20 - Done (2026-11-15)" "CH2d2 - Done (2026-11-16)"
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH1a4 - Done (2026-11-14)"
```

## Task

**User report from the CH2d2 play-test (2026-11-16, `LuDbA`, Altar of Evil, key S):** in the transformed form (1) the skill slot window / HUD slots look misplaced, (2) the model does not change,
(3) after the form ends ( key C ) the skill slots do not go back to the regular ones. The log shows `altar transform start body L buff 1` on every cast and no line for the end, so the
buff and the states run; the visible parts do not.

Find in June, in Ghidra, **who** makes each visible change and port it whole (no stand-in):
- **Model swap.** `LoadTransformedSkinAnim` ( `32b162e0` ), `SetFormSkinAnim` ( `32b13e20` ), `ReleaseTransformedSkinAnim` ( `32b13d30` ), `EndTransformedForm` ( `32b38f30` ) are ported ( CH1a4 ) but may have no caller:
  xref them and the transformed-skin members ( +0x8f74 / +0x8f8c ) in the listing: the start state ( `32b43910` / `32b43ae0` ), the buff's behavior ( BTI 763 / 764 start and finish ), `CommonStateStart` ( `32b37910` ), `OnFrameMove` / `ParseCommonRandomState`.
  Check the transformed mesh / motion files exist in the kom ( `Mesh_*Luciel*` transform names the loader reads ) before blaming code.
- **Slot UI in the form.** The transform window ( `CX2TransformSkillSlotUI`, CH2d20 ): its position and A / B type ( `SetSkillUIType` `31fe2b10`, `UpdateSlotGuideDescPos` `31fe12f0` ), who hides the regular slot bar
  while it shows ( `CX2UIManager::UI_TRANSFORM_SKILL_SLOT` 44, `SetShowTransformSkillSlotUI`, the hooks CH2d20 added ) and who shows it again at the form's end ( `EndTransformedForm` ends in `SetShow( false )`? ).
  The end path: key C -> state 452 -> `StateEnd` = `LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_END_StateEnd` -> `EndTransformedForm( 0, false )`; prove it runs ( a debug line under `_SKILL_DEBUG` ) and what it should restore.
- Check the Ciel side only for the shared pieces; Ciel's form states are CH3c2's.

Stay inside the plan's subject: a June branch that is neither in the transform's start, end, model or slot UI goes to its own row ( scope rule ). Keep June's bugs verbatim ( `JUNE_BUGS.md` row each ).

You are on Opus: delegate reads, greps and known-address Ghidra dumps. CP949 sources: `bytepatch.py` only. Done gate: `finish.py --files ... --with JUNE_CHAR_LUCIEL_SKILL JUNE_CHAR_TRANSFORM_LINK JUNE_CHAR_LUCIEL_LIVE`
PASS, `build.py --deploy` with `DEPLOY OK` (a play-test phase), `exestr` line, funcmap rows ( `Char Luciel CH2d3` ).

Play-test hand-off ( copy-ready, per key, nicknames ): `LuDbA`, key S ( Altar of Evil ): the model changes to the form's, the transform window sits where June puts it, the regular slots come back at key C and at the buff's end;
repeat from page 2 ( key W ) and on `LuDbB`. Also name whatever else the user should see ( the window's end texture, cool times counting on the three linked skills ).
