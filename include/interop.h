#ifndef LIBRAC5_INTEROP_H
#define LIBRAC5_INTEROP_H
#include "rac5.h"
#ifdef __cplusplus
extern "C" {
#endif
#define RAC5_LEVEL_SLOT_COUNT 25
#define RAC5_LEVEL_UNKNOWN (-2)
/* Absolute addresses, in the actual levelExecs index order (0..24).
 * Slots 11/12 alias LEVEL_10 on disc; 13/14 name absent RELs. No extra C slots.
 * Zero means unavailable. Clear overlay entries when their module unloads. */
typedef struct VariableAddress {
    Rac5Address MainMenu; /* 0 */
    Rac5Address Pokitaru; /* 1 */
    Rac5Address Ryllus; /* 2 */
    Rac5Address Kalidon; /* 3 */
    Rac5Address Metalis; /* 4 */
    Rac5Address Dreamtime; /* 5 */
    Rac5Address MedicalOutpostOmega; /* 6 */
    Rac5Address Challax; /* 7 */
    Rac5Address DayniMoon; /* 8 */
    Rac5Address InsideClank; /* 9 */
    Rac5Address Quodrona; /* 10 */
    Rac5Address Roberto; /* 11 */
    Rac5Address JumpLevel; /* 12 */
    Rac5Address MungoArena; /* 13 */
    Rac5Address ClankSegment; /* 14 */
    Rac5Address MetalisGiantClank; /* 15 */
    Rac5Address IslandEscape; /* 16 */
    Rac5Address DangerValley; /* 17 */
    Rac5Address MegaCannons; /* 18 */
    Rac5Address MoonCowDisease; /* 19 */
    Rac5Address MPLobby; /* 20 */
    Rac5Address ChallaxGiantClank; /* 21 */
    Rac5Address KalidonSkyboard; /* 22 */
    Rac5Address MedicalOutpostSkyboard; /* 23 */
    Rac5Address HIGTreehouse; /* 24 */
} VariableAddress_t;
Rac5Address rac5GetAddressForLevel(const VariableAddress_t *, int levelIndex);
int rac5SetAddressForLevel(VariableAddress_t *, int levelIndex, Rac5Address);
/* Reads the global referenced by the verified getter; never calls game code.
 * -2 means unknown/invalid; -1 is accepted only as a tooling alias in table APIs. */
int rac5GetCurrentLevel(const Rac5Context *);
Rac5Address rac5GetAddress(const Rac5Context *, const VariableAddress_t *);
/* Compatibility with the sibling libraries. Bind after rac5Init/InitEE;
 * context must remain alive. NULL unbinds. No pause/loading-state gating is
 * inferred: both selectors only select by current index and return zero on failure. */
void rac5SetAddressContext(const Rac5Context *);
Rac5Address GetAddress(VariableAddress_t *);
Rac5Address GetAddressImmediate(VariableAddress_t *);
/* Store a resolved absolute address from a caller-supplied loaded module.
 * Source path must match the level; missing symbols clear that slot.
 * No module scan, rebasing guess, or automatic active-module assertion. */
int rac5SetModuleSymbolAddress(const Rac5Context *, const Rac5Module *,
                              VariableAddress_t *, int levelIndex, const char *name);
#ifdef __cplusplus
}
#endif
#endif
