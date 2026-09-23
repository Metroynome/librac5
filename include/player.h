#ifndef LIBRAC5_PLAYER_H
#define LIBRAC5_PLAYER_H

#include <tamtypes.h>
#include "types.h"
#include "math.h"
#include "math3d.h"

typedef struct Player { // 0x39f0
/* 0x0000 */ char unk_0000[0x96c];
/* 0x096c */ float hitpoints;
/* 0x0970 */ float maxHp;
} Player;

typedef Hero Player;

#endif
