# June character Lu/Ciel (Stage 8) - Phase CH1a1 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1a1 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH1a1 done (its Done note and commit) |
| Flag | class-agnostic `SERV_IRUHADEV_JUNE_CHAR_FORM_MP_GAGE` (live, every class) and `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (no `_DEBUG`) |
| Log tag | none (no play-test: regen amounts are unchanged; Lu/Ciel is inert) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "CH1a1 - Done (2026-11-14)"
```
Then `grep "Char Luciel CH1a1" june2017_re/funcmap.tsv` (38 rows), `JUNE_BUGS.md` rows 675-678 and the two `june_march_divergent.tsv` rows
`CX2GageData::OnFrameMove` / `CX2GageData::UpdateDamageFlushMP`.

## Task

Review CH1a1's commit against June (Ghidra read-only; the uncarved bodies through `june2017_re/funcdis.py --func <va>`):
- **Slot map.** Dump vt `3349b2a4` against the base `33487acc` (and Chung `3349b064`): slots 1-11 take the gauge index; 0 / 13 / 16 need no code
  (compiler dtor, a bare base call, the base body); 17 / 18 are CH1a2's.
- **Base bodies ( `FORM_MP_GAGE`, live for every class ).** `322b2d00` / `322b3010` / `322b33d0` / `322b2d30` / `322b34d0` / `322b35a0` / `322b2cb0` /
  `322b2d60` / `322b3650` / `322b36f0` / `32a31910` (index 1 -> nothing; `FlushMp` ignores the index), the `+0xc` gauge in ctor `322b3a70`, copy ctor
  `322b24f0`, dtor `322b2da0`, `Init` `322b37a0`, op= `32a36530`. `OnFrameMove` `32a33c40` + `MpFrameMove` `32a344a0`: tracked regen, no `Veryfy`,
  the `+0x18` gate left out ( confirm again that nothing writes `[gage + 0x18]` ). `UpdateDamageFlushMP` `32a33d50` ( `-1.0` at `33436c90` ).
- **Lu/Ciel bodies ( `_CORE` ).** `32a31a90`, `32a328d0`, `32a35430` ( no `+0x10c` store ), `32a31ac0`, `32a32bf0`, `32a32cc0`, `32a31800` ( the `+0x14`
  copy for my Ciel ), `32a31af0`, `32a32d70`, `32a32e10`, `32a31b30`, `32a35020` ( point before max ), `32a360d0`, `32a35150`, `32a36f70`, `32a36ef0`,
  and `32a35170` ( form in play at the main rate, the other form `+0x12c` x time: the name `m_fRestFormMpPerSecond` ).
- **Callers.** No March caller passes a third argument to `SetNowMp` / `UpNowMp`; nothing binds the MP accessors by member pointer.
- **Routing.** CH1a2 ( play-status pair + `KGamePlayStatus` ) and CH1a3 ( `CX2LucielMyGageUI` + manager cases ) kickoffs and STATUS rows exist;
  CH1 final depends on both reviews.

Spot-check 3 items directly in Ghidra; if one fails, re-check the whole phase. Hygiene: `python tools/flagcheck.py
SERV_IRUHADEV_JUNE_CHAR_FORM_MP_GAGE --range <CH1a1 commit>^..<CH1a1 commit> --with SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE` (both flags share the
gauge files), `git log -S<flag> <commit>..HEAD`, `python tools/junediverge.py --check-all`, `python tools/enumscan.py --check X2Lib/X2GageData.h
X2Lib/X2GageData.cpp`. Paste the summary lines; explain every OTHER row and deleted line.

Write "CH1a1 - Review" into the plan doc, update the CH1a1 row in STATUS.md (done; CH1a2, CH1a3 unblocked; CH1k's CH1a1 dependency met), and
commit. No build unless a fix lands (then `/Zs` probes of `X2GageData.cpp` and `X2GUUser.cpp`; no play-test).
