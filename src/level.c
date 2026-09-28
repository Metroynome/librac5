#include "level.h"

typedef struct Level {
    int id;
    const char *name;
} Level_t;

static const Level_t levels[] = {
    {0, "Main Menu / Frontend"},
    {1, "Pokitaru - Jowai Resort"},
    {2, "Ryllus - Vetega Jungle"},
    {3, "Kalidon - Mechanoid Factory"},
    {4, "Metalis - Junkyard LXIV"},
    {5, "Dreamtime"},
    {6, "Medical Outpost Omega - Surgical Facility"},
    {7, "Challax - Technomite City"},
    {8, "Dayni Moon - Farming Cooperative"},
    {9, "Inside Clank"},
    {10, "Quodrona - Clone Factory"},
    {15, "Metalis - Giant Clank"},
    {16, "Island Escape"},
    {17, "Danger Valley"},
    {18, "Mega Cannons"},
    {19, "Moon Cow Disease"},
    {20, "Multiplayer Lobby"},
    {21, "Challax - Giant Clank"},
    {22, "Kalidon - Skyboard Race"},
    {23, "Medical Outpost Omega - Skyboard Race"},
    {24, "HIG Treehouse - California"},
};

const char *levelGetNameById(int id) {
    unsigned i;
    if(id == -1)
        id = 0;
        
    for(i = 0; i < sizeof(levels)/sizeof(levels[0]); ++i) {
        if(levels[i].id == id) {
            return levels[i].name;
        }
    }    
    return 0;
}
