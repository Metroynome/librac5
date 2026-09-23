#ifndef LIBRAC5_INTEROP_H
#define LIBRAC5_INTEROP_H

#include <tamtypes.h>

// REL offsets indexed by the game's level ID; zero means unavailable.
typedef struct VariableAddress {
    u32 MainMenu; /* 0 */
    u32 Pokitaru; /* 1 */
    u32 Ryllus; /* 2 */
    u32 Kalidon; /* 3 */
    u32 Metalis; /* 4 */
    u32 Dreamtime; /* 5 */
    u32 MedicalOutpostOmega; /* 6 */
    u32 Challax; /* 7 */
    u32 DayniMoon; /* 8 */
    u32 InsideClank; /* 9 */
    u32 Quodrona; /* 10 */
    u32 Roberto; /* 11 */
    u32 JumpLevel; /* 12 */
    u32 MungoArena; /* 13 */
    u32 ClankSegment; /* 14 */
    u32 MetalisGiantClank; /* 15 */
    u32 IslandEscape; /* 16 */
    u32 DangerValley; /* 17 */
    u32 MegaCannons; /* 18 */
    u32 MoonCowDisease; /* 19 */
    u32 MPLobby; /* 20 */
    u32 ChallaxGiantClank; /* 21 */
    u32 KalidonSkyboard; /* 22 */
    u32 MedicalOutpostSkyboard; /* 23 */
    u32 HIGTreehouse; /* 24 */
} VariableAddress_t;

#ifdef __cplusplus
extern "C" {
#endif
u32 GetAddress(VariableAddress_t *address);
u32 GetAddressImmediate(VariableAddress_t *address);
#ifdef __cplusplus
}
#endif

#endif
