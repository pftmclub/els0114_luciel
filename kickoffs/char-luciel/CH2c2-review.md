# June character Lu/Ciel (Stage 8) - Phase CH2c2 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH2c2 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | CH2c and CH2c2 done (their Done notes and commits) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL`, `SERV_IRUHADEV_JUNE_ENUM_NAMES` (the new BFI / BTI names) and the temporary `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL_DEBUG` (this review retires it) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL SKILL`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH2c - Done (2026-11-15)" "CH2c2 - Done (2026-11-15)"
```

## Task

Review the commits of CH2c and CH2c2 (`git log --oneline --grep "char-luciel Phase CH2c"`) against June in Ghidra and the capstone listings
(`june2017_re/charport_adis.py luciel --phase CH2c` / `CH2c2`, the six setter cases with `june2017_re/char_luciel/ch2c_cases.py <entry_va>`). This review covers both
(user, 2026-11-10: one review per CH stage). What CH2c2 could get wrong ( every item below was read from a listing, not decompiled ):
- **Soul Shock** ( `32b2db60` / `32b2e080` ): ability types 6 / 0x20 / 0x51 are named by value in the C++ enum ( `SA_ATK_REL`, `SA_PROJECTILE_SIZE`, `SA_TARGET_MANA_REL` ), check the values against June's table;
  the damage effect's rate is scaled through `CEffect::SetPowerRate` ( June reads a protected float at effect +0x2a4: confirm it is the power rate ); the instance scale is `( size, size, size )` with the +0x7c flag;
- **`AddSoulCollectorStack`** ( `32b3db80`, called by `32b3dd80` and `32b3dea0` ): `31f8df00` = `GetGageData()` +0x24 = the soul gauge ( `UpNowSoul` ); vslot 0xac = `GetHyperModeCount`; the stack limit is `CX2BuffTemplet` +0x3c = `GetAccumulationLimit`;
  BFI 523 / 540 and BTI 462 ( new names in `X2BuffFactor.h` ); the `<= 0` cool time test skips a NaN as June's `test ah, 1`;
- **Demonic Breath**: the press count is the synced short ( `sClassSyncShort`, Future +0x7ecc / Now +0x816c ); the MP of a press is `32007b10( 0x8f, GetFutureStateSkillID() )` with the gauge index = the minor body flag; Fire's three effect sets by count ( 8 / 4 );
- **Fantasie Impromptu**: the six-state future frame move ( speeds x1.5, backwards x0.8 x1.5 ), the beat constants ( 0.148 / 0.083 ), the Accelerando end hand-over to the 2nd movement ( id swap + `TryUseSkillWithCombinationPoint( , ASUT_GROUND )` );
- **Death Crisis of Marbas**: the three event processes' shared aim tail ( +50 / -50 / 0 into `fClassSkillFloat` ), the MP per second ( min charge x 10 ), the Z / X flag logic of Wait / Small fire, `SetLeftHandRotateBone` ( clamp 45, axis ( 0, 0, -1 ) );
- **Soul Strike**: the female shot's random step / half turn, the stack rates shared with the male ( `ApplyStackRates` ), the Enhanced Specter power ( female: `SA_DAMAGE_REL` of the now state, male: templet power rate ), the male loop count;
- **Shadow** ( `32b20670` ): the equipped templet with the upgrade swap, level floor 1, BFI 545 / 546 by body, `SA_MAX_HP_REL_PVP` under the PvP-or-PvE test;
- the six setter cases: hyper copies per case ( Soul Shock FrameMove + EventProcess; Demonic Breath StartFuture + EventProcess; Fantasie Start + EventProcess; Death Crisis StartFuture / Start / EventProcess + `m_eSkillID`; Soul Strike EventProcess / StateEndFuture / StateEnd; Shadow FrameMove + EventProcess ),
  the extra states' callbacks and the male skill-state bytes ( 344 / 345 / 403 );
- June's memo getter is a stub ( `31fca9e0` always 0 ): every memo branch is not written, as CH2c;
- retire `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL_DEBUG` ( `//#define` in `Always.h` ); the lines it guards stay.

Hygiene is one command: `python tools/finish.py --review --commits <the reviewed phases' commits> --with JUNE_CHAR_LUCIEL_SKILL JUNE_ENUM_NAMES JUNE_CHAR_LUCIEL_SKILL_DEBUG`.
Run it with `-v` and explain every OTHER row and every deleted line; a FAIL or ERROR is a finding.

Spot-check 3-4 items directly in Ghidra (read-only); if one fails, re-check the whole phase. Fix clear transcription errors under the same flag; hand design decisions back to the user.
Write "CH2c2 - Review" into the plan doc, update the STATUS.md rows of CH2c2 and CH2d, check `kickoffs/char-luciel/CH2d.md`, and commit. A debug-define-only change needs no build.
