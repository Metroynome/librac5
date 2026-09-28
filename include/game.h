#ifndef LIBRAC5_GAME_H
#define LIBRAC5_GAME_H

#include "level.h"

#define GAME_MODE_ISLAND_ESCAPE_COOP (*(int*)0x01069C68)

int gameGetCurrentLevel(void);

#endif
