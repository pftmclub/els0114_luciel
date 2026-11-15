# June character Lu/Ciel (Stage 8) - Phase DU5f review

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-luciel DU5f review` |
| Plan doc | `JUNE_CHARACTER_LUCIEL_PHASES.md` (branch `main`) |
| Depends on | DU5f done and its play-test passed (Done note and commit) |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_DUO_SOCKET` (+ `_DEBUG`, which this review retires) |
| Log tag | `[JUNE_CHAR]` (`LUCIEL DUO SOCKET`) |

## Read first

```sh
python tools/docsection.py JUNE_CHARACTER_LUCIEL_PHASES.md "Rules" "DU5f - Done (2026-11-16)"
```
and `grep "(DU5f)" june2017_re/funcmap.tsv` (13 rows), `JUNE_BUGS.md` rows 762-765.

## Task

Review DU5f's commit against June (Ghidra / `june2017_re/disasm_range.py`, read-only):
- `SocketData_DuoUnit` (`32091730`, vtable `33478ab0`): pair offsets +0x468 / +0x478, vslots 3 / 4 (`32091b10` / `32091b60`: argument true = +0x468),
  ReInit `32092450`; `3271a060` reads the option's +0x2f4 / +0x22c through `ADD EAX, 0x10` (the map value), adds to body 0 for groups 0 / 2 and body 1
  for 1 / 2, and has no PvP / dungeon-only test.
- Every `3271a060` caller is ported with the right group: user `3206d770` (5 equip vectors before the skip, pet aura with 2), `32050230` (group 0 / 1
  sets, mode = `+0x8e90 == 1`), unit `32365d30` (equips, then the mode skip; pet aura with 2), `323652b0` (sets). Items +0x8c / +0x9c / +0xac stay Stage 16.
- `3235a160 @3235a22f-3235a339` and `32b0fa90`: gauge 0 reads vslot ( true ), gauge 1 vslot ( false ), the operand order of `max + max x rate + add`,
  and `3248a800`'s gauge index reaching the gage data's slot 0x14.
- `32350b80` / `3286a3e0` / `3234d8d0` / `32912cc0`: the form test (group 2 always, 1 with the form, else without), the window override only while
  `+0x1e5` ( m_bShow ) is set and the viewer exists; `32350cb0`'s group test; whether March's other `GetSetIDNPartsNum` callers (`GetEnchantStat`,
  `GetRepairDiscountRate`, the party pass, the window) match June's callers of `32350b80`.
- `@3291bf2d`: the local is never read (JUNE_BUGS 764).
- Flag-off = March by `cl /P` of `X2Unit.cpp` and `X2GUUser.cpp` (flag on vs `//#define`).

Hygiene is one command: `python tools/finish.py --review --commits <DU5f's commit> --with SERV_IRUHADEV_JUNE_CHAR_DUO_SOCKET SERV_IRUHADEV_JUNE_CHAR_DUO_SOCKET_DEBUG`
(`-v`; explain every OTHER row). Retire `SERV_IRUHADEV_JUNE_CHAR_DUO_SOCKET_DEBUG` (comment out its `#define`: no build). Fix clear transcription errors
under the same flag (then `tools/build.py --deploy`); hand design decisions back. Write "DU5f - Review" into the plan doc, update the STATUS row, check
the next DU kickoff (DU5c / DU5d once DU5b2's review lands), commit.
