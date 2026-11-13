# June dungeons - C3z-4t4b - the quick-quest list sits where June's tutorial highlight expects it

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-dungeons C3z-4t4b` |
| Plan doc | `JUNE_DUNGEONS_PHASES.md` (branch `main`) |
| Depends on | C3z-4t4 (play-test passed 2026-11-13 apart from this) |
| Flag | `SERV_IRUHADEV_JUNE_QUICK_QUEST_POS` (new; one flag for the position change) |
| Log tag | `JUNE_FX C3z-4t` |
| Review | none of its own: `C3z-4t4-review.md` covers it (add one line there) |

## Read first

```sh
python tools/docsection.py JUNE_DUNGEONS_PHASES.md "C3z-4t4 - Done" "C3z-4t4 - Play-test"
grep "323d0ca0\|323dd640\|323c96c0\|QuickQuest" june2017_re/funcmap.tsv
python tools/junediverge.py --list
```

## Task

The C3z-4t4 play-test (user, 2026-11-13) passed, but the tutorial GFx highlight for the quest scenes (211 / 202 / 411, `tutorial_guide.gfx`
timelines 6 / 7 / 11) sits off the quick-quest list. The C3z-4t4 session found the cause: June's `CX2UIQuestNew` ctor `323d0ca0` places each quick-quest
dialog (`DLG_UI_NEW_Quest_Quick.lua`, @`323d1884`-`323d190c`) at x `810.0f` (`33493cd8`), y `173.0 + 22.0 * i` (doubles `33493cd0` / `33474d68`);
March's ctor (`X2UIQuestNew.cpp:359`) uses y `143.f + g_fQuickQuestMinimizeSizeY * i`. June's list is 30 px lower, and the highlight is drawn for it.
The dialog file's own `SetPos( 817, 143 )` is the same in both.

1. Confirm `g_fQuickQuestMinimizeSizeY` is 22 and that no other March site sets an absolute quick-quest position (Expand / Minimize at :7948 / :7976 move
   relative to the current one). Check June's `UpdateQuickQuestDLG` `323dd640` (ported under `JUNE_DIVERGE_QUICK_QUEST`) and `MinimizeAllUIQuickQuest` /
   `ExpandAllUIQuickQuest` `323c96c0` / `323c98c0` for another absolute y.
2. Port June's base y (173) under the new flag, March's 143 in `#else`. Scope rule: only the position; the rest of June's ctor stays as it is unless it
   also moves something the guide's highlight points at (say so in the Done note and route the rest).
3. Check whether any other right-side dialog the tutorial highlights (the quick-quest show toggle, the party bottom bar for 503 / 506) has the same
   offset; report, do not chase beyond the tutorial's scenes.

## Play-test gate

A new level-1 character (the `_WALK_DEBUG` switch is still on): scene 211's highlight lands on the quick-quest entry, 411's on the instant-complete
button. Also walk out of Ruben and back with quest 11005 going and look for `begin 201 ( by going quest )` (not seen in the C3z-4t4 play-test).

No hardcoded enum literals (user, 2026-10-26; CLAUDE.md). No `June` prefix (user, 2026-10-28; CLAUDE.md).
