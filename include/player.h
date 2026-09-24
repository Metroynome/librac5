#ifndef LIBRAC5_PLAYER_H
#define LIBRAC5_PLAYER_H

#include <tamtypes.h>
#include "types.h"
#include "math.h"
#include "math3d.h"

// Island Escape (Co-op) player 1 struct: 0106f410
typedef struct Hero { // 0x39f0
/* 0x0000 */ char unk_0000[0x96c];
/* 0x096c */ float hitpoints;
/* 0x0970 */ float maxHp;
} Hero;

typedef Hero Player;

#endif
