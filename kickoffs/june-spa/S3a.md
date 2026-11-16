# June spa villages - Phase S3a - Spa packets and the emulator's buff answer

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase june-spa S3a` |
| Plan doc | `JUNE_SPA_PHASES.md` (branch `main`) |
| Depends on | S2b review; not beside another session editing `KncWX2Server/Common/` packet files or `Offline/Handlers_*` |
| Flag | `SERV_IRUHADEV_JUNE_SPA_BUFF`, `SERV_IRUHADEV_OFFLINE_SPA_BUFF` (emulator stand-in) |
| Log tag | `[JUNE_SPA]` (`python tools/logs.py --tag JUNE_SPA`) |
| Review | inside `kickoffs/june-spa/S3b-review.md` |
| Hand-off | none of its own (S3b's play-test runs it); probe build only |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_SPA_PHASES.md "Rules" "Design" "Decisions" "Phase S3a - Spa packets and the emulator's buff answer" "Phase S0 - Done"
```

Then the map rows marked `S3a` in `june2017_re/spa/spa_map.tsv` (`awk -F'\t' '$5 ~ /^S3a/' june2017_re/spa/spa_map.tsv`).

## Task

1. **Packets.** Append `EGS_SPA_SOAK_REQ`, `EGS_SPA_SOAK_ACK`, `EGS_SPA_BUFF_END_TIME_NOT` to March's event-id list under the flag, and put
   their structs and `Put` / `Get` in `KncWX2Server/Common`, field by field as S0's Done note writes them (June's tags: 0x637 bool, char;
   0x638 int, bool, `__int64`; 0x639 `__int64`). A change in `CommonPacket.h` also needs the `ServerDefine.h` second home and a `SECOND_HOMES`
   entry.
2. **Enums.** Register `BTI_SPA_GROW_BUFF` 918, `BTI_SPA_STRONG_BUFF` 919 and `BFI_SPA_GROW_BUFF` 948 where C++ lacks them; check the two
   `BuffTemplet` rows and both factor rows are live in data036.
3. **Emulator** (`SERV_IRUHADEV_OFFLINE_SPA_BUFF`), "Decisions" 1 = refresh on every entry: on `{ 1, SBT_GROW / SBT_STRONG }` grant (or re-grant)
   the type's buff for its templet period from now through `CX2OfflineBuff`, reply `EGS_SPA_SOAK_ACK { NET_OK, true, end time }`, send the buff
   list; one `[JUNE_SPA] emu grant buff=<BTI> sec=<n> stand-in:` line per grant. `{ 0, -1 }` and other types: logged, no reply. Never send
   `EGS_SPA_BUFF_END_TIME_NOT`.

The client sends nothing until S3b: prove it compiles with a probe build (full build skipped: no play-test). Run the gates with
`tools/finish.py --with SERV_IRUHADEV_JUNE_SPA_BUFF SERV_IRUHADEV_OFFLINE_SPA_BUFF`.

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
