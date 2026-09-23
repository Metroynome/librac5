#include "level.h"
typedef struct { int id; const char *name; const char *source; } Level;
static const Level levels[] = {
    {0, "Main Menu / Frontend", "../LEVEL_BIN/levelstartup_disc.elf"},
    {1, "Pokitaru - Jowai Resort", "../LEVEL_BIN/rcp1_level01_disc.elf"},
    {2, "Ryllus - Vetega Jungle", "../LEVEL_BIN/rcp1_level02_disc.elf"},
    {3, "Kalidon - Mechanoid Factory", "../LEVEL_BIN/rcp1_level03_disc.elf"},
    {4, "Metalis - Junkyard LXIV", "../LEVEL_BIN/rcp1_level04_disc.elf"},
    {5, "Dreamtime", "../LEVEL_BIN/rcp1_level05_disc.elf"},
    {6, "Medical Outpost Omega - Surgical Facility", "../LEVEL_BIN/rcp1_level06_disc.elf"},
    {7, "Challax - Technomite City", "../LEVEL_BIN/rcp1_level07_disc.elf"},
    {8, "Dayni Moon - Farming Cooperative", "../LEVEL_BIN/rcp1_level08_disc.elf"},
    {9, "Inside Clank", "../LEVEL_BIN/rcp1_level09_disc.elf"},
    {10, "Quodrona - Clone Factory", "../LEVEL_BIN/rcp1_level10_disc.elf"},
    {15, "Metalis - Giant Clank", "../LEVEL_BIN/rcp1_level04_Giantclank_disc.elf"},
    {16, "Island Escape", "../LEVEL_BIN/rcp1_levelMP01_disc.elf"},
    {17, "Danger Valley", "../LEVEL_BIN/rcp1_levelMP02_disc.elf"},
    {18, "Mega Cannons", "../LEVEL_BIN/rcp1_levelMP03_disc.elf"},
    {19, "Moon Cow Disease", "../LEVEL_BIN/rcp1_levelMP04_disc.elf"},
    {20, "Multiplayer Lobby", "../LEVEL_BIN/rcp1_lobby_disc.elf"},
    {21, "Challax - Giant Clank", "../LEVEL_BIN/rcp1_level07_Giantclank_disc.elf"},
    {22, "Kalidon - Skyboard Race", "../LEVEL_BIN/rcp1_level03_airboard_race_disc.elf"},
    {23, "Medical Outpost Omega - Skyboard Race", "../LEVEL_BIN/rcp1_level06_airboard_race_disc.elf"},
    {24, "HIG Treehouse - California", "../LEVEL_BIN/rcp1_HIGTreehouse_disc.elf"},
};
const char *levelGetNameById(int id) { unsigned i; if(id==-1) id=0; for(i=0;i<sizeof(levels)/sizeof(levels[0]);++i) if(levels[i].id==id) return levels[i].name; return 0; }
const char *levelGetSourceById(int id) { unsigned i; if(id==-1) id=0; for(i=0;i<sizeof(levels)/sizeof(levels[0]);++i) if(levels[i].id==id) return levels[i].source; return 0; }
