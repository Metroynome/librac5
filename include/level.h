#ifndef LIBRAC5_LEVEL_H
#define LIBRAC5_LEVEL_H
/* Runtime levelExecs indices. -1 remains a backwards-compatible tooling alias. */
enum Rac5Level {
    RAC5_LEVEL_MAIN_MENU = 0,
    RAC5_LEVEL_MAIN_MENU_TOOLING = -1,
    RAC5_LEVEL_POKITARU = 1,
    RAC5_LEVEL_RYLLUS = 2,
    RAC5_LEVEL_KALIDON = 3,
    RAC5_LEVEL_METALIS = 4,
    RAC5_LEVEL_DREAMTIME = 5,
    RAC5_LEVEL_MEDICAL_OUTPOST_OMEGA = 6,
    RAC5_LEVEL_CHALLAX = 7,
    RAC5_LEVEL_DAYNI_MOON = 8,
    RAC5_LEVEL_INSIDE_CLANK = 9,
    RAC5_LEVEL_QUODRONA = 10,
    RAC5_LEVEL_METALIS_GIANT_CLANK = 15,
    RAC5_LEVEL_ISLAND_ESCAPE = 16,
    RAC5_LEVEL_DANGER_VALLEY = 17,
    RAC5_LEVEL_MEGA_CANNONS = 18,
    RAC5_LEVEL_MOON_COW_DISEASE = 19,
    RAC5_LEVEL_M_P_LOBBY = 20,
    RAC5_LEVEL_CHALLAX_GIANT_CLANK = 21,
    RAC5_LEVEL_KALIDON_SKYBOARD = 22,
    RAC5_LEVEL_MEDICAL_OUTPOST_SKYBOARD = 23,
    RAC5_LEVEL_H_I_G_TREEHOUSE = 24,
};
#ifdef __cplusplus
extern "C" {
#endif
const char *rac5LevelName(int discLevelId);
const char *rac5LevelSource(int discLevelId);
#ifdef __cplusplus
}
#endif
#endif
