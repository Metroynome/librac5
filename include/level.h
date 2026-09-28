#ifndef LIBRAC5_LEVEL_H
#define LIBRAC5_LEVEL_H

#include <tamtypes.h>
#include "types.h"

/* Runtime levelExecs indices. -1 remains a backwards-compatible tooling alias. */
typedef enum Levels {
    LEVEL_MAIN_MENU = 0,
    LEVEL_MAIN_MENU_TOOLING = -1,
    LEVEL_POKITARU = 1,
    LEVEL_RYLLUS = 2,
    LEVEL_KALIDON = 3,
    LEVEL_METALIS = 4,
    LEVEL_DREAMTIME = 5,
    LEVEL_MEDICAL_OUTPOST_OMEGA = 6,
    LEVEL_CHALLAX = 7,
    LEVEL_DAYNI_MOON = 8,
    LEVEL_INSIDE_CLANK = 9,
    LEVEL_QUODRONA = 10,
    LEVEL_METALIS_GIANT_CLANK = 15,
    LEVEL_ISLAND_ESCAPE = 16,
    LEVEL_DANGER_VALLEY = 17,
    LEVEL_MEGA_CANNONS = 18,
    LEVEL_MOON_COW_DISEASE = 19,
    LEVEL_M_P_LOBBY = 20,
    LEVEL_CHALLAX_GIANT_CLANK = 21,
    LEVEL_KALIDON_SKYBOARD = 22,
    LEVEL_MEDICAL_OUTPOST_SKYBOARD = 23,
    LEVEL_H_I_G_TREEHOUSE = 24,
} eLevels;


const char *levelGetNameById(eLevels id);

#endif
