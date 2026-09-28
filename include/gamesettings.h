#ifndef LIBRAC5_GAMESETTINGS_H
#define LIBRAC5_GAMESETTINGS_H

#include <tamtypes.h>
#include "types.h"

#define GAME_SETTINGS_PTR ((GameSettings_t**)0x010aa5a8)

typedef enum GameMode {
    GAME_MODE_ANY = -1,
    GAME_MODE_IRON_LOMBAX = 0,
    GAME_MODE_DEATHMATCH = 1,
    GAME_MODE_CTF = 2,
} eGameMode;

typedef enum GameType {
    GAME_TYPE_COOP,
    GAME_TYPE_VERSUS
} eGameType;

typedef struct GameSettings {
/* 0x000 */ char unk_000[0x180];
/* 0x180 */ int level;
/* 0x184 */ int mode;
/* 0x188 */ int type;
} GameSettings_t;

#endif
