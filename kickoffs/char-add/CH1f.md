# June character Add (Stage 7) - Phase CH1f - Class core: Arc Tracer / Mastermind lines

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase char-add CH1f` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | CH0 (and its amendments), char-elesis Z, the previous CH1 sub-phase. One at a time with CH1-CH4: they all edit `X2GUAdd.*` |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_CORE` (`new` rows, compiled) and `SERV_IRUHADEV_JUNE_CHAR_ADD_LIVE` (`inside` rows; `//#define` until CH1 final) |
| Log tag | none (nothing runs until `_LIVE`; no play-test) |
| Review | none of its own: the CH1g review covers it (its `_DEBUG` define stays on) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Rules" "Phase CH0 - Add map (read-only)" "Phase CH1 - Deploy unit: June script, motions and the class core"
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "CH1f - Done"   # the worked example, once written; else Ara's
awk -F'\t' '$7=="CH1f"' june2017_re/char_add/ch0_class_map.tsv   # CH0 confirms the column
```

## Task

Transcribe every function the class map gives phase `CH1f` (the `AAT` and `AMM` states and their `A` / `P` states (June-only classes 33 / 123: compiled now, reached from CHNa)) from Ghidra, **whole**. The plan's "Transcription rules (CH1a-CH1k2)" apply:
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

- Class map: **41** functions (0 listing-only; 0 `inside`, 41 `new`), filter `awk -F'\t' '$7=="CH1f"' june2017_re/char_add/ch0_class_map.tsv` (col 7 phase, col 5 inside / new, col 6 proposed name, col 9 carved y / n).
- Script states: 19 (`june2017_re/char_add/ch0_state_skill.tsv` part 1, col 8).
- CH0 split: CH1f = the `AAT` / `AMM` states and their non-container helpers (41); CH1f2 = the container helpers at `0x32a2c000`+ they reach (21). `ANSI_AAT_CREATE_DYNAMO_FACTORY` is registered five times (ids 291-294, 299; Start `32a288d0` via thunk `32a31730`, EventProcess `32a18ec0`): key by id. `ANSI_AMM_COMBO_XXDownX_END` (id 237) is a `~` row. June registers `ANSI_AMM_DASH_COMBO_XX` (279) with no table.

## Added by CH1d (2026-11-16)

- The enum already holds `ANSI_AMM_COMBO_ZZX` ( 277 ), `ANSI_AMM_COMBO_XXDownX_START` ( 235 ), `ANSI_AAT_COMBO_NASOD_ARMOR_X` ( 222 ) and `_Z` ( 225 ), added by CH1d under `_CORE`: do not add them again.
- `ANSI_AAT_DASH_COMBO_ZBackX` ( `329dc020` @`329dc09b` ) sets `CX2GUAdd::m_bComboXXAfterDashComboZBackX` ( June `+0x9698`, member added by CH1d ) to true before `StateChange( ANSI_COMBO_XX )`; ComboXX's Z and down + X arms read it.

## Added by the CH1d2 review (2026-11-16)

- `ANSI_AAT_DASH_COMBO_ZX` Start / End / FrameMove ( `329d2190` / `329d2200` / `329dbce0` ) and `ZBackX` Start / End / FrameMove ( `329d2230` / `329d2260` / `329dbfc0` ) are the dash X render pattern: write them exactly as the reviewed `DashComboXStateStart` / `End` / `FrameMove` ( `X2GUAdd.cpp`, tag `char-add CH1d2 review` ): `+0x8e64` is the user's `m_iBuffCustomRenderState`, `+0xc44` is `m_RenderParam.color.a` ( not Add's `m_fRenderParamColorA` ), state 5 is `CX2GUUser::CRS_INVISIBLE`, the members are `m_iCustomRenderStateOnStateStart` / `m_fRenderParamColorAOnStateStart`. Never Add's own `m_eCustomRenderState` / `CRS_COMBO_DASH_X`.

## Added by CH1e (2026-11-17)

- `CX2GUAdd::m_ComboChainTarget` ( June `+0x9910`, `ComboChainTargetData`, ctor -1 / user ) and `GetComboChainTargetUnit()` ( June `32294460` ) exist under `_CORE`: `ANSI_AMM_COMBO_ZZX.EventProcess` ( `329df090` ) reads it exactly as `329de850` does ( the X key arm needs `NULL != GetComboChainTargetUnit()` ); do not add a second target member.
- The enum holds `ANSI_APT_DASH_COMBO_ZXX`, `ANSI_APT_DASH_JUMP_COMBO_ZX_LANDING`, `ANSI_ALP_COMBO_ZZXX` and `ANSI_ALP_COMBO_ZZXX_LANDING` ( added by CH1e ). `ZX_CANCEL_AFTER_ON_LINE_` ( `m_fAttackCancelAfter`, pureZ / pureX on a line ) is a macro in `X2GUAdd.cpp` under `_CORE`: June's tails use it after the dash cancel. `329de260`-style states test the animation end before the foot ( JUNE_BUGS 790 ): read each function's first test in the listing.
