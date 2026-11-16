# June character Add (Stage 7) - Phase CH1d - Class core: ground combos

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase char-add CH1d` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | CH0 (and its amendments), char-elesis Z, CH1d0 review (the engine helpers it calls). One at a time with CH1-CH4: they all edit `X2GUAdd.*` |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_CORE` (`new` rows, compiled) and `SERV_IRUHADEV_JUNE_CHAR_ADD_LIVE` (`inside` rows; `//#define` until CH1 final) |
| Log tag | none (nothing runs until `_LIVE`; no play-test) |
| Review | none of its own: the CH1d2 review covers it (its `_DEBUG` define stays on) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Rules" "Phase CH0 - Add map (read-only)" "Phase CH1 - Deploy unit: June script, motions and the class core"
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "CH1d - Done"   # the worked example, once written; else Ara's
awk -F'\t' '$7=="CH1d"' june2017_re/char_add/ch0_class_map.tsv   # CH0 confirms the column
```

## Task

Transcribe every function the class map gives phase `CH1d` (ground combos (25 `ANSI_COMBO*`) and the base `AN` lines CH0 assigns here) from Ghidra, **whole**. The plan's "Transcription rules (CH1a-CH1k2)" apply:
- **`inside` rows:** June's body inside March's `CX2GUAdd` function under `SERV_IRUHADEV_JUNE_CHAR_ADD_LIVE`, March's body in `#else`,
  `#endif SERV_IRUHADEV_JUNE_CHAR_ADD_LIVE`. Add `_LIVE` to `Always.h` as `//#define` if no earlier phase did. Prove the June path compiles: one
  `cl /c` of `X2GUAdd.cpp` with `/DSERV_IRUHADEV_JUNE_CHAR_ADD_LIVE` (the build's own flags), result in the Done note.
- **`new` rows:** the plain name CH0 proposed, under `SERV_IRUHADEV_JUNE_CHAR_ADD_CORE`, compiled in every build, referenced by nothing yet. Leave
  `InitState*` alone (CH1m registers).
- DP / formation lines that call a June piece CH1q has not written go under `SERV_IRUHADEV_JUNE_CHAR_ADD_DP`, nasod / barrier / Phase Shift lines
  under `SERV_IRUHADEV_JUNE_CHAR_ADD_NASOD` (an `#ifdef` inside the `_LIVE` body); March's live DP calls stay March's until CH1q.
- A class-range callee (`0x329cf870`-`0x32a316dd`) owned by a later group gets a declaration with `// June <va>: body in <phase>`; an engine callee
  is called through its March counterpart (`funcmap.tsv`, `junediverge.py --list`); an unmapped engine callee is "this needs Opus": stop and say so.
- Listing-only functions are read with `python june2017_re/charport_adis.py add <va>` (Ghidra stays read-only); one whose control flow the listing
  does not make plain is "this needs Opus" for that function: go on with the rest.
- A state that writes the state power rate keeps June's operator (checklist §7 l.256); `m_fNowStatePowerRate` (7i); June's BFI arguments (7ai).
- June's numbers, order and quirks verbatim (a `JUNE_BUGS.md` row each). `X2GUAdd.*` are CP949 (CRLF): `bytepatch.py` only; check each `#ifdef`
  branch with `cl /P`.
- Done gate: every class-map row with this phase is written (the Done note lists them with June addresses, `inside` / `new` and `carved` /
  `listing-only`); the build passes with `_LIVE` undefined (one-file probe build, no full build: no play-test) and the one-file probe passes with it
  defined; `junediverge.py --check` and `enumscan.py --check` on the class files PASS. Nothing to play-test: the hand-off says so and names the next
  phase.

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
Phase tag (user, 2026-11-09): every edited or new code block carries a one-line `// char-add <phase>` comment (at the start of an existing one-line comment, never a second line).

## Amended by CH0 (2026-11-13)

- Class map: **45** functions (1 listing-only; 33 `inside`, 12 `new`), filter `awk -F'\t' '$7=="CH1d"' june2017_re/char_add/ch0_class_map.tsv` (col 7 phase, col 5 inside / new, col 6 proposed name, col 9 carved y / n).
- Script states: 25 (`june2017_re/char_add/ch0_state_skill.tsv` part 1, col 8).
- Checklist rows (`june2017_re/char_add/ch0_checklist_rows.tsv` col 5): L675.
- Calls `32293d70` (line-side test): CH1d0 writes it first. The untagged nasod-armor combos (`ANSI_COMBO_NASOD_ARMOR_*`, 11 tables) are here; their nasod lines go under `_NASOD` inside the `_LIVE` body. March's class tests in `NasodArmorComboZZEventProcess` / `NasodArmorComboXEventProcess` (`X2GUAdd.cpp:5798` / `:5910`) follow June's bodies.
- ( CH1d0, 2026-11-16 ) `32293d70` = `CX2GameUnit::CheckPosAlongLastTouchLine( vPos, OUT bool& bAlong )` ( `_CHAR_ADD_CORE` ): returns `true` with no line map ( `bAlong` left as it was ), `false` with no last-touched line or a dot product of 0 ( `IsSamef` ), else `true` with `bAlong = 0 < dot`. June's caller keeps its own `bool` ( initial value ) for the cases that leave it.

## Added by char-luciel CH1i2 (2026-11-14)

- `GetFixedDistancePositionForWeaponTaker` is June's now ( `SERV_IRUHADEV_JUNE_DIVERGE_FIXED_DISTANCE_WEAPON_TAKER` ). June's `NasodArmorAttackStartFuture` ( `329edde0`, call `@329ee138` ) passes a position March does not: `CX2GameUnit::GetBoneLinePos` ( `32293f80` ) of the target toward this unit, replaced by the target's own `GetPos()` when the two are at most 500.0 apart ( `<=`, NaN keeps the bone position; `33458fa8`, `@329ee0af` ); March passes `&pUnit->GetPos()` ( `X2GUAdd.cpp:5649` ). Distance 100, `!GetIsRight()` and the `== true` test match. `329ef130` has the same pattern ( `@329ef3c8` ).
- ( CH1i2 review ) In `329edde0` the bone position also feeds March's `FIX_AISHA_TELEPORT_POSITON` test: June returns when the line's `startPos.y` is above the **bone position's** y ( `@329ee07c`-`329ee08d` ), where March tests `pUnit->GetPos().y` ( `X2GUAdd.cpp:5643` ).
