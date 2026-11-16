# June character Lu/Ciel (Stage 8) - Phase CH3c2 review ( covers CH3c and CH3c2 )

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel CH3c2 review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | Phases CH3c and CH3c2 done (their Done notes and commits) and the CH3c2 play-test passed |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL` (+ `_SKILL_DEBUG`, retired here), `SERV_IRUHADEV_JUNE_ENUM_NAMES` |
| Log tag | `[JUNE_CHAR] LUCIEL SKILL` (`python tools/logs.py --grep "JUNE_CHAR\] LUCIEL SKILL"`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Starting context" "CH3c - Done (2026-11-16)" "CH3c2 - Done (2026-11-16)"
```
Then `kickoffs/char-luciel/CH3c2.md` (the amendments), the `funcmap.tsv` rows tagged `(CH3c)` / `(CH3c2)` for Luciel, and `JUNE_BUGS.md` rows 776-777 and 781-782.

## Task

Review CH3c's commits ( `cff1bbd0`, the fix `a6edaf9d` ) and CH3c2's against June in Ghidra ( read-only ) and the exe listing. One stage review: CH3c has none of its own.
- CH3c2, the 35 functions ( `ch0_class_map.tsv` rows with phase `CH3c2` ), four of them line by line:
  Desperado's shot frame move `32b32090` ( the power `GetPowerRate() x count x SA 2`, the lock-shot effect set with the power, the pitch clamp 30 / -20 with the middle band stored as 0, the 100 offset, `m_vInstancePositionOffset` at +0x1a4 ),
  Abyss Sniping's aim `32b172d0` ( the three ability reads and where each is stored, the cone call's argument order, the two aim passes ) and its loop effect `32b36d90` ( the clamp only when the pitch is not 0, the stored yaw = the x87 zero ),
  Shrewd Cutter's end start `32b33bf0` ( `GetLandPos( true )`, the nine `CanDown` arguments, the lock-on safe position + 45 and state 499 ) and Rapid Impact's move attack `32b32550` ( the two `GetFixedDistancePositionForWeaponTaker` tries and which facing each keeps ).
- The Transcendence form's WAIT / WALK / JUMP_READY / JUMP_LANDING are copies of the Altar of Evil's ( `32b31260`, `32b31480`, `32b133d0`, `32b316a0` ) with the Transcendence ids: diff the listings again ( the double step before the walk in the wait; the double step and hyper mode before the stop test in the walk ).
- The three setter cases pasted from `ch3c_held_cases.txt` ( Transcendence 30049 / 30080, Abyss Sniping 30059, Demonic Rush 30060 ) and the nine registrations that replaced `body in CH3c2` comments: the member each thunk fills ( Start / StartFuture / FrameMove / EventProcess / StateEnd ), the hyper states copying only what June copies ( Desperado's hyper has no start: JUNE_BUGS 781 ).
- `m_vecAbyssSnipingTarget` is `std::vector<CX2GameUnit*>` now; the two enum names in `X2BuffFactor.h` ( 790, 792 ).

Hygiene is one command: `python tools/finish.py --review --commits cff1bbd0 a6edaf9d <the CH3c2 commit> --with JUNE_CHAR_LUCIEL_SKILL JUNE_CHAR_LUCIEL_SKILL_DEBUG JUNE_ENUM_NAMES -v`;
explain every OTHER row and deleted line ( the nine deleted lines of CH3c2 are the `body in CH3c2` comments, the tenth is the vector's old type ).

Read the play-test's `[JUNE_CHAR] LUCIEL SKILL` lines ( CH3c's and CH3c2's: `desperado aim value / targets / shot`, `abyss sniping aim`, `shrewd cutter start / attack`, `transcendence form start`, `garden of haures spawn`, `rapid impact move attack` ). Retire
`SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL_DEBUG` ( comment out the define: no build ). Fix clear transcription errors under the same flag; hand design decisions back to
the user. Write "CH3c2 - Review" into the plan doc, update the STATUS rows ( CH3c / CH3c2 done; CH3d ready ), check `kickoffs/char-luciel/CH3d.md`, and commit.
