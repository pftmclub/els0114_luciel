# June character Lu/Ciel (Stage 8) - Phase CH1k0 review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH1k0 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phase CH1k0 done (its Done note and commit `88c14ed5`) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_BUFF_TIME_PASS` (class-agnostic, `X2GameUnit.*`) |
| Log tag | none (inert until CH1k / CH1m's callers go live) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "CH1k0 - Done (2026-11-14)"
grep "(CH1k0)" june2017_re/funcmap.tsv
grep -E "^\| (690|691) \|" JUNE_BUGS.md
```

## Task

Review CH1k0's commit against June ( Ghidra `322a74a0`, `322a7000`, `3227bcb0`, read-only ):
- **The debuff filter, both ways:** `322a74a0` skips a buff whose templet exists and has `+0x64` ( `IS_DEBUFF` ) clear ( `jnz @322a7538` ), so it
  scales debuffs; `322a7000` skips one whose templet has it set ( `jz @322a70b8` ). CH0 had `322a74a0` backwards: confirm the port's
  `IsDeBuff()` tests, and that a missing templet passes in both.
- **The rate guard** of `322a74a0` ( `fldz / fcomp [rate]`, `test ah,0x41`, `jz` out ): rate >= 0 and NaN run, rate < 0 returns.
- **The awakening clock** in `322a7000` ( `@322a714f`-`@322a71b8` ): BTI 5 / 0x336, unit `+0xc04` = `GetGageData()`, protected `+0xe8` = the hyper-mode
  remain time, `> 0` only ( `test ah,5 / jp` ), `SetHyperModeRemainTime` `31e8c9d0`, inside the finalizer loop ( JUNE_BUGS 691 ).
- **`3227bcb0`:** `Set( Get + add )`, `ChangeBuffFinalizerTempletPtrList` called unconditionally, argument order ( BTI, float ).
- The `#error` guard's three targets are defined where the guard sits ( `cl /P` ).

Hygiene is one command: `python tools/finish.py --review --commits 88c14ed5 --skip-flag SERV_IRUHADEV_JUNE_BUFF_BINDING "target of the #error guard"
--skip-flag SERV_IRUHADEV_JUNE_CHAR_EVE_PASSIVE "target of the #error guard"`; run it with `-v` and explain every OTHER row and deleted line.
A clear transcription error is fixed under the same flag ( probe build: the code stays inert, no play-test ). Write "CH1k0 - Review" into the plan doc,
update the STATUS row ( CH1k then waits only on the CH1i review ), and commit.
