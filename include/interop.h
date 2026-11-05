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

/* REL-relative offsets; MainMenu carries the format tag, Pokitaru is the base. */
#define RAC5_VA_COMPACT_MENU(address) ((u32)(address) | (u32)0x80000000)
#define RAC5_VA_COMPACT_OFFSET(base, address) ((address) == 0 ? 0 : \
	(u16)(((s32)(address) - (s32)(base)) / 4 + 0x8000 + \
	0 * sizeof(char[((address) == 0 || (((s32)(address) - (s32)(base)) % 4 == 0 && \
	((s32)(address) - (s32)(base)) / 4 >= -32767 && \
	((s32)(address) - (s32)(base)) / 4 <= 32767)) ? 1 : -1])))

typedef struct rac5VariableAddressCompact {
	/* 0x00 */ u32 MainMenu;
	/* 0x04 */ u32 Pokitaru;
	/* 0x08 */ u16 Ryllus;
	/* 0x0a */ u16 Kalidon;
	/* 0x0c */ u16 Metalis;
	/* 0x0e */ u16 Dreamtime;
	/* 0x10 */ u16 MedicalOutpostOmega;
	/* 0x12 */ u16 Challax;
	/* 0x14 */ u16 DayniMoon;
	/* 0x16 */ u16 InsideClank;
	/* 0x18 */ u16 Quodrona;
	/* 0x1a */ u16 Roberto;
	/* 0x1c */ u16 JumpLevel;
	/* 0x1e */ u16 MungoArena;
	/* 0x20 */ u16 ClankSegment;
	/* 0x22 */ u16 MetalisGiantClank;
	/* 0x24 */ u16 IslandEscape;
	/* 0x26 */ u16 DangerValley;
	/* 0x28 */ u16 MegaCannons;
	/* 0x2a */ u16 MoonCowDisease;
	/* 0x2c */ u16 MPLobby;
	/* 0x2e */ u16 ChallaxGiantClank;
	/* 0x30 */ u16 KalidonSkyboard;
	/* 0x32 */ u16 MedicalOutpostSkyboard;
	/* 0x34 */ u16 HIGTreehouse;
	/* 0x36: two bytes of EE alignment padding. */
} rac5VariableAddressCompact_t;

#ifdef __cplusplus
extern "C" {
#endif
u32 GetAddress(const void *address);
u32 GetAddressImmediate(const void *address);
#ifdef __cplusplus
}
#endif

#endif
