# librac5

## Compact address tables

`GetAddress` and `GetAddressImmediate` accept both the original `VariableAddress_t` and `rac5VariableAddressCompact_t`. Existing full tables keep their layout. A compact table occupies 56 bytes instead of 100, saving 44 bytes per table before linker alignment and the shared decoder cost.

```c
rac5VariableAddressCompact_t vaExample = {
	.MainMenu = RAC5_VA_COMPACT_MENU(0),
	.Pokitaru = 0x00400000,
	.Ryllus = RAC5_VA_COMPACT_OFFSET(0x00400000, 0x00400100),
};
```

These are illustrative values, not game addresses. Always tag MainMenu, even when it has no address. Pokitaru remains a 32-bit base; later fields encode a signed word delta biased by 0x8000. Zero means unavailable; a field equal to the base encodes 0x8000. Nonzero entries must differ from the base by a multiple of four within +/-32767 words. The macro rejects constant offsets outside that range at compile time. Use a full table when it cannot fit.

The website's Address Table (Compact) formatter emits this layout, and falls back to a full table when its base is missing/ambiguous or a delta cannot fit. Recompile consumers against the updated headers and library. Do not index compact fields as an array of 32-bit addresses.

RAC5 entries are REL-relative offsets, not absolute EE addresses. The resolver adds the currently loaded module base after the original level/loading/module checks. `vaMobyCreate` and `vaMobyDestroy` now use compact tables; `vaMsgStr` remains full because its deltas do not all fit. NTSC-U, PAL, and NTSC-J builds pass. Their converted offsets match the originals in all 25 slots, and compiled MIPS full/compact resolver tests pass. With the existing compiler, the two table data sections shrink by 96 bytes including alignment while decoder text grows by 96 bytes: no aggregate object-section reduction yet. Final linked savings depend on which tables/functions are retained. This update has not been gameplay-tested.
