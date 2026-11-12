# June character Elesis (Stage 7) - Phase CH1k review (covers CH1h-CH1k)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-elesis CH1k review` |
| Plan doc | `JUNE_CHARACTER_ELESIS_PHASES.md` (branch `main`) |
| Depends on | CH1h, CH1i, CH1j and CH1k done (their Done notes and commits) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE` (`//#define`), `_CORE` (defined), `_SUMMON` (`//#define`) |
| Log tag | none (nothing runs until `_LIVE`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_ELESIS_PHASES.md "Rules" "CH1h - Done" "CH1i - Done" "CH1j - Done" "CH1k - Done"
awk -F'\t' '$7=="CH1h" || $7=="CH1i" || $7=="CH1j" || $7=="CH1k"' june2017_re/char_elesis/ch0_class_map.tsv
```

## Task

Review the functions of CH1h-CH1k against June (`python june2017_re/charport_adis.py elesis <va>...`; the listing carries the `StateChange` arguments and the `fcomp` / `test ah` direction; the map's column 9 says which rows are `listing-only`). **Read every `listing-only` function of these phases against the raw listing**, then spot-check at least:
- **CH1k `32b84a50`** (lookup): the order of the checks ( quick-slot wait, unit, `CommonSpecialAttackEventProcess`, slot, templet-mismatch text `0x8bae`, `m_iChosenSkillID` before the use-condition test, War Prelude `+0x9796` ground rule, summon, PVP-official, `3202d140`, `3202d3c0`, riding-key wait, `0x818` ), MP, the dev gate around cool time / MP / stone, and the Injection Stigma HP check ( ability type 7 = `SA_ATK_PHYSIC_REL`, `+0x4424` = `GetPhysicAttackValue()`, buff 666 x 0.2, text `0xa43` = 2627, outside the dev gate ). **`32b4ee20` / `32b52590` / `32b73e60`**: the use-condition 3 / 5 restore, the use and the cooldown ( templet test before `MarkSkillSlotUsed`, `IsTrainingOrPhotozone`, memo block dead ).
- **CH1k `32b80c60` / `32b80840` / `32b75c90` / `32b59aa0`**: BFI 232 / 423 and 479 / 480 ( raw pushes `( BFI, creator, level, adjust, skip )` = C++ `( BFI, level, NULL, 0, false )` ), the `+0x9754` effect-set power-rate argument ( `fPowerRate`, life time -1 ), id 207's plain effect set, the `_ON_LINE_` cancels.
- **CH1k `32b70af0`**: `EventTimerOneshot`, the event 0x8b3 pair order ( `333a82f0` = CASH_21 at `337ad560` ), `GetShowCutInAndChangeWorldColor` / `GetShowActiveSkillShow` order.
- CH1h-CH1j spot checks as their Done notes list; JUNE_BUGS 635-637; the `ReservedSkillStart` hook chain in `X2GUUser.h` / `.cpp` ( seven `#ifndef`s, one call ).

Hygiene: `python tools/flagcheck.py SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE --with SERV_IRUHADEV_JUNE_CHAR_ELESIS_CORE SERV_IRUHADEV_JUNE_CHAR_ELESIS_SUMMON SERV_IRUHADEV_JUNE_CHAR_ELESIS_WOS SERV_IRUHADEV_JUNE_CHAR_ELESIS_BLOOD SERV_IRUHADEV_JUNE_CHAR_ELESIS_PASSIVE --range b907661f^..HEAD --files X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h` (FAIL only for the commented-out `#define`); `python tools/junediverge.py --check-all`; `python tools/enumscan.py --check X2Lib/X2GUEL.cpp X2Lib/X2GUEl.h`; one-file `/Zs` probes of `X2GUEL.cpp` off, with `/D SERV_IRUHADEV_JUNE_CHAR_ELESIS_LIVE`, and with `_LIVE + _WOS + _BLOOD + _PASSIVE + _SUMMON`. Fix clear transcription errors in place (bytepatch only). Write "CH1k - Review" into the plan doc, update the STATUS rows (CH1k review done, CH1m ready), check `kickoffs/char-elesis/CH1m.md`, commit with explicit pathspecs.

**Nothing to play-test.** Next: `/x2-phase char-elesis CH1m` (Opus 5.5 high).
