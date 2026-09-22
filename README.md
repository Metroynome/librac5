# librac5

PS2 Ratchet & Clank: Size Matters library foundation, following the sibling
`librac1`/`librac2`/`librac3`/`librac4` include/src/lib layout and regional archives.
Resolves the game's surviving SNR2/SNDLL symbols at runtime, without a fixed
NTSC-U address table. No player, pad, health, rendering or object-layout APIs
are invented from the other Ratchet engines.

## Regional support

| Target | Archive | Validation |
| --- | --- | --- |
| auto | librac5auto.a | All three real boot ELFs and NTSC-U Island Escape dump tested |
| ntscu | librac5ntscu.a | SCUS_976.15 retail v1.00 |
| pal | librac5pal.a | SCES_550.19 retail v1.00; 1,803 named boot symbols checked |
| ntscj | librac5ntscj.a | SCPS_151.20 retail v1.00; 1,799 named boot symbols checked |

All archives use the same resolver. Compile-time region macros select the build
label returned by `rac5BuildRegion()`, not a region restriction.
`rac5GetBuild(&ctx)` identifies a known regional image using the boot symbol-table
address/count and four anchor symbol addresses. It returns an immutable
`Rac5BuildInfo` with region, serial, version, entrypoint and symbol-table metadata,
or NULL for an unknown/modified profile. A successful generic initialization
can still return NULL from this stricter identification. This is profile matching,
not a cryptographic check of the running executable. Check the returned region
before applying any region-specific gameplay patch.

PAL/Japanese validation loads each real ELF's PT_LOAD segment into a host test
image and runs the actual C resolver. It is not an emulator capture and does not
verify live overlay loading, game-function ABI, hooks, or cheat execution.

## Build

Use a PS2DEV shell with GNU make and `mips64r5900el-ps2-elf-*` on PATH:

```sh
make -j4                         # all four archives under lib/
make -f Makefile.ntscu            # one target (pal/ntscj/auto also available)
make install PS2SDK=/path/to/ps2sdk
```

Override `EE_PREFIX=ee-` for a legacy toolchain. `make install` copies headers
into `ports/include/librac5` and archives into `ports/lib`, like the sibling
libraries. No SDK install is needed for the sibling sm-cheats local build.
On this Windows setup, add `C:/ps2dev/ee/bin` and `C:/msys64/usr/bin` to PATH.

## Use

```c
#include <librac5/rac5.h>
Rac5Context ctx;
if (rac5InitEE(&ctx)) {
    const Rac5BuildInfo *build = rac5GetBuild(&ctx);
    Rac5Address address = rac5Resolve(&ctx, "memset");
    /* build->region identifies the actual known build, independently of the archive. */
    /* Use only after checking the symbol's meaning, prototype and game ABI. */
}
```

`rac5Init(ctx, ramImage, 0x02000000)` also works on a host using a full dump.
All calls return failure/zero for a missing or malformed table/name.
`rac5Resolve` returns an address, not a callable wrapper; the record's linkage
kind is not a C type. A game call can require a particular gp and execution
context. No guessed function signatures are exposed.

Initialization scans physical RAM from 0x00100000 to 0x02000000 on 4-byte
boundaries once; do not run it every frame. Boot recognition requires a unique
source-less SNR2 table with `memset`, `MCP_LoadLevelModule__Fi` and
`MCPI_SendNewLevel__Fi`. These anchors were checked in all three retail
boot ELFs; other revisions may require additional profiles. Symbol searches are linear; cache validated
boot addresses in your own context when appropriate.

For a level, call `rac5FindModule(&ctx.memory, rac5LevelSource(16), &module)`
when needed, then `rac5FindSymbol`. Loaded table values are already absolute:
never add the module base again. All 21 REL source paths were checked against each regional disc and match
the library mapping. Other revisions still need verification. `16C` uses level 16's
REL, so it has no separate numeric code-module ID. Frontend is runtime index 0; -1 remains accepted as a backwards-compatible
tooling alias. A resident table is not proof of an actively executing
module. Reacquire module handles after loading transitions; header/source
changes invalidate a handle, but unloaded memory can remain intact. Resolve
only while the game is not concurrently unloading/loading that module.

## Evidence and tests

`docs/symbols-{ntscu,pal,ntscj}.tsv` contains each region's named boot records
and decoded names. These are searchable references; struct members/sizes and
general return types remain unknown. `docs/region-profiles.json` records the
ELF SHA256 hashes, entrypoints, table locations/counts, and anchor addresses.
`docs/regional-verification.json` records the offline verification results.

Regenerate profiles from the extracted discs with
`python tools/update_region_profiles.py` inside librac5. By default this reads
`../test/sm`; override `--sm-root` for another extraction location. The generated
`src/region_profiles.inc` and docs are checked in; building needs no game files.

One imported name, `PAUSEMODE_GetCurrentPauseScreen__Fv`, is absent from every
region's boot table. It is recorded per module in the verification report.
A boot lookup therefore returns zero; the library does not fabricate an address.
That symbol's runtime provider remains to be established.

```sh
make -C test test HOST_CC=gcc
make -C test regions HOST_CC=gcc PYTHON=python
make -C test test HOST_CC=gcc DUMP=/path/to/island_escape_eeMemory.bin
```

The optional dump test specifically expects the inspected NTSC-U Island Escape
capture: boot at 0x01F0AD00, level at 0x00E8B380, all 259 imports matching boot.
Other tests exercise different table bases, missing names, invalid bounds,
unterminated strings, duplicate boot tables and changed source metadata.
The regional test checks every named boot symbol, all 21 level source paths,
and boot-resolvable REL imports for each region. It exercises all four build
labels against all three real ELF images, including unknown/changed profiles.
Cross-compilation and fully resolved payload links were checked with PS2DEV GCC 15.2.0. These are offline tests;
no payload has been run in PCSX2 or on a console yet.

## VariableAddress tables

Include `librac5/interop.h` for `VariableAddress_t`, `GetAddress`, and
`GetAddressImmediate`. The struct follows the real `levelExecs` array in all
three boot ELFs: MainMenu=0, campaign levels=1..10, legacy slots=11..14,
MetalisGiantClank=15, IslandEscape=16, DangerValley=17, MegaCannons=18,
MoonCowDisease=19, MPLobby=20, ChallaxGiantClank=21, KalidonSkyboard=22,
MedicalOutpostSkyboard=23, HIGTreehouse=24. Named struct fields preserve every
slot so the indices do not shift across gaps.

There are no extra executable slots for 16C..19C: these asset variants reuse
REL indices 16..19. No fabricated 25..28 runtime IDs or duplicate fields are
added. The 25-entry executable array ends with a null pointer at index 25.
Slot 11 (Roberto) and 12 (JumpLevel) point to LEVEL_10.REL on retail discs;
13 (MungoArena) and 14 (ClankSegment) name REL files absent from those discs.
Their field names come from localLevelExecs development paths and do not
establish playable retail levels. `rac5LevelSource` still reports unavailable
entries as NULL. Existing rac-defs-sm.json frontend -1 remains a tooling ID;
this change does not rewrite its schema or IDs.

A VariableAddress entry is an **absolute EE address**. It must not contain an
on-disc REL offset. Use zero for unavailable/unresolved entries. For example:

```c
#include <librac5/interop.h>
VariableAddress_t vaLevelMain = {0};
Rac5Module levelModule;
/* ctx was initialized; acquire a stable loaded level module outside transitions. */
if (rac5FindModule(&ctx.memory, rac5LevelSource(16), &levelModule)) {
    rac5SetModuleSymbolAddress(&ctx, &levelModule, &vaLevelMain,
                              16, "lib_main__FiPPc");
}
Rac5Address address = rac5GetAddress(&ctx, &vaLevelMain);
```

The helper checks the supplied module source path and resolves the actual
loaded address; it does not assume a fixed base or rebase an already relocated
value. It clears the requested slot if resolution fails. A resident table alone
does not prove the module is active. Clear your table when modules unload and
refresh it after transitions; a previously stored address is not auto-invalidated.
No speculative per-level camera/player/function address tables are provided.

`rac5GetAddressForLevel` and `rac5SetAddressForLevel` allow explicit index access.
For the sibling-library call style, bind a long-lived initialized context with
`rac5SetAddressContext(&ctx)`, then call `GetAddress(&vaLevelMain)` or
`GetAddressImmediate(&vaLevelMain)`. Pass NULL to unbind. Both selectors use the
same index behavior: no unverified pause/loading/game-mode checks are assumed.
Unbound/invalid contexts and out-of-range indices return zero.

`rac5GetCurrentLevel` reads the global referenced by the verified instruction
sequence in MCPTRANS_GetLevel__Fv (without calling game code). Its failure value
is RAC5_LEVEL_UNKNOWN (-2), which cannot collide with the -1 tooling alias.
The globals are NTSC-U/PAL 0x01EDE348 and NTSC-J 0x01EE0068. Their locations are
decoded from each game's getter, not selected from a US-only constant.
All 25 selections and invalid indices were checked in each regional ELF test;
the actual Island Escape capture reports 16 and resolves lib_main to 0x00EA2DA8.
