# June character Add (Stage 7) - Phase CH1f3 - Class core: installation and drone records (the CH1f rows Sonnet routed)

| | |
|---|---|
| Start | new conversation on **Opus 5.5, high** effort, then type `/x2-phase char-add CH1f3` |
| Plan doc | `JUNE_CHARACTER_ADD_PHASES.md` (branch `main`) |
| Depends on | CH1f (its Done note, "Not written"). One at a time with CH1-CH4: they all edit `X2GUAdd.*` |
| Flag | `SERV_IRUHADEV_JUNE_CHAR_ADD_CORE` (`new` rows, compiled) and `SERV_IRUHADEV_JUNE_CHAR_ADD_LIVE` (`//#define` until CH1 final) |
| Log tag | none (nothing runs until `_LIVE`; no play-test) |
| Review | none of its own: the CH1g review covers it (its `_DEBUG` define stays on) |
| Hand-off | now |

## Read first (and nothing else from the plan doc unless you need it)

```sh
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md --no-sub "Rules" "CH1f - Done"
python tools/docsection.py JUNE_CHARACTER_ADD_PHASES.md "CH1e - Done"   # the form of the earlier states
awk -F'\t' '$1=="329f85a0"||$1=="329f8680"||$1=="329fb150"||$1=="329fb270"||$1=="32a247e0"||$1=="32a27900"||$1=="32a288d0"' june2017_re/char_add/ch0_class_map.tsv
python june2017_re/charport_adis.py add 329f85a0 329f8680 329fb150 329fb270 32a247e0 32a27900 32a288d0   # listings; Ghidra stays read-only
```

## Task

CH1f (Sonnet) wrote 34 of its 41 rows and routed these 7, all `new` under `_CORE`: they build and read two record types whose fields are named here once:
- **The installation record** at CX2GUAdd `+0x97f4` ( `0x1c` bytes read here: skill id `+0`, damage-effect handle `+4`, two floats `+0x97fc` / `+0x9800` that `32a27900` uses as the spawn distance in front and the height, state `+0x14`, two flags `+0x18` / `+0x19` ): `329f85a0` ( reset: skill 0, state 2, flags, `UpdateSkillSlotUI( false )`, destroy the effect ), `329f8680` ( state change: `GetMainEffect()->ChangeAnim` `Start` / `Attack` / the empty name; the state and flag in two stack arguments ), `32a27900` ( the installation switch: buffs `0x16e` / `0x16f`, buff-time finalizer `0x156` via `GetBuffTimeFinalizerDuration` / `SetBuffTimeFinalizerDuration`, `UpMutationCount`, `PlayDynamoFactoryChangeEffectSets` ( CH1f ), the `ADD_ATT_DroneStart01.ogg` sound, the new effect from `+0x80c4` land height ), `32a288d0` ( the Dynamo Factory Start: buff `0x16e` levelled clone through `32d41960` ( `none in March`, funcmap "PORT: C6c": **decide how it is ported first** ), `EffectSet_SI_SA_AAT_Installation_Drone_Activator_Generate`, then `32a27900` with the skill's `DAMAGE_EFFECT_*_Generate` ).
- **The drone entry ( `0x38` bytes: float, two ints, a byte, two effect handles `+0x14` / `+0x18`, a wstring `+0x1c` ) and the drone list** ( `329fb270` reaches a vector of them, an owning sound at `[8]` ): `329fb150` ( entry reset ), `329fb270` ( list clear ), `32a247e0` ( `ANSI_AMM_COMBO_ZZXX` FrameMove: three entries, `Weapon0%d` bones, `DamageEffect_ADD_Ultimate_Fury_Attack_Dron`; the `_4th` variant is picked by a counter that never reaches 4: a `JUNE_BUGS` row ).
Find every other reader of both records first ( `grep` the CH0 class map for `+0x97f4`, `+0x96c0`, `+0x982c`, `0x38`-byte vectors; CH2c / CH3b / CH1h-CH1j rows ) and name the fields by what those readers do. The container helpers CH1f2 lists ( `32a2cbc0`, `32a2f970`, `32a2fa60`, `32a315f0`, ... ) are the compiled `std::vector` / copy code of these records: write the record types so that `std::vector` does it, and say in the Done note which CH1f2 rows that makes unnecessary.
The plan's "Transcription rules (CH1a-CH1k2)" apply: June's numbers, order and quirks verbatim ( a `JUNE_BUGS.md` row each ), the `char-add CH1f3` tag in every new block, no `June` / address in a name. You are on Opus: delegate the reads, greps, known-address Ghidra dumps, probe compiles and `finish.py` runs ( x2-phase, *When the phase is Opus* ). CP949 sources: `bytepatch.py` only.

Done gate: every row above is written ( or the Done note says which still waits and why ); `finish.py --files X2Lib/X2GUAdd.cpp X2Lib/X2GUAdd.h <other changed files> --with JUNE_CHAR_ADD_CORE JUNE_CHAR_ADD_LIVE` PASS; one-file `/Zs` probes of `X2GUAdd.cpp` with `_LIVE` off and defined, 0 errors; no full build ( no play-test ). Nothing to play-test: the hand-off says so and names `CH1f2`.
