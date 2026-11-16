# June character Add (Stage 7) - Phase CH1i - Class core: overrides 0x600-0x7bc

| | |
|---|---|
| Start | new conversation on **Sonnet 5.5, high** effort, then type `/x2-phase char-add CH1i` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | CH0 (and its amendments), char-elesis Z, the previous CH1 sub-phase. One at a time with CH1-CH4: they all edit `X2GUAdd.*` |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_CORE` (`new` rows, compiled) and `SERV_IRUHADEV_JUNE_CHAR_ADD_LIVE` (`inside` rows; `//#define` until CH1 final) |
| Log tag | none (nothing runs until `_LIVE`; no play-test) |
| Review | none of its own: the CH1j2 review covers it (its `_DEBUG` define stays on) |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Rules" "Phase CH0 - Add map (read-only)" "Phase CH1 - Deploy unit: June script, motions and the class core"
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "CH1i - Done"   # the worked example, once written; else Ara's
awk -F'\t' '$7=="CH1i"' june2017_re/char_add/ch0_class_map.tsv   # CH0 confirms the column
```

## Task

Transcribe every function the class map gives phase `CH1i` (overrides 0x600-0x7bc (0x600 HyperModeFrameMove `329ffe90`, 0x658, 0x670, 0x684, 0x6a4 `329e6610`, 0x6bc, **the training-game values 0x6c4 / 0x6f4** (vtable `329e21d0` / `329e2130`; the backlog names `329e2170` / `329e20e0`: CH0 settles which), 0x71c, 0x738, 0x7b8 `329d1730`; 0x6c8 / 0x6cc / 0x6d4-0x6e8 / 0x740 as declarations for CH1q / CH2a0, 0x73c for CH4b, InitPassiveSkillState and the passive hook for CH4a)) from Ghidra, **whole**. The plan's "Transcription rules (CH1a-CH1k2)" apply:
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

- Class map: **28** functions (15 listing-only; 5 `inside`, 23 `new`), filter `awk -F'\t' '$7=="CH1i"' june2017_re/char_add/ch0_class_map.tsv` (col 7 phase, col 5 inside / new, col 6 proposed name, col 9 carved y / n).
- 0x600 HyperModeFrameMove `329ffe90`; 0x7b8 SetEquippedSkillLevel `329d1730` (18 blocks, 32 vslot-0x7a8 state calls, no member write: `june2017_re/char_add/ch0_setskilllevel_blocks.tsv`); 0x6d4 / 0x6d8 SetStasisfieldHitValue / DamageValue `329e25b0` / `329e25e0`; 0x6dc-0x6e8 the UpDPBy*ThisFrame accessors (June keeps March's bug: both getters read the attack flag `+0x95f5`). **Training keys settled:** slot 0x6c4 = `329e21d0` SetDPValue and 0x6f4 = `329e2130` SetMutationCount (clamp 0..6); the backlog's `329e2170` / `329e20e0` are GetMaxDPValue / GetMutationCount. All four are CH1q's (DP), declared here. 0x690 (CH4a), 0x73c (CH4b), 0x6c8 / 0x6cc / 0x6d0 / 0x740 / 0x744 (CH1q) are declarations.

## Added by CH1d (2026-11-16)

- `IsInJobLine` ( `329e3190`, vslot 0x670 ) is declared by CH1d ( `X2GUAdd.h`, `_CORE` ): write its body. CH1d's `ANSI_COMBO_XXZ_StateStart` and the ComboXX / ComboXXX bodies call it under `_LIVE` ( job lines 1 / 2 / 3 ).
