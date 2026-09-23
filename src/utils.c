#include <tamtypes.h>
#include "utils.h"

//--------------------------------------------------------------------------------
void POKE_LUI_ORI(u32 luiAddr, u32 oriAddr, u32 value, int reg)
{
    u32 hi = (value >> 16) & 0xffff;
    u32 lo = value & 0xffff;

    POKE_U32(luiAddr, 0x3c000000 | (reg << 16) | hi);
    POKE_U32(oriAddr, 0x34000000 | (reg << 21) | (reg << 16) | lo);
}

//--------------------------------------------------------------------------------
int isInEEMemory(void* p)
{
    u32 upper = (u32)p >> 16;
    return upper >= 0x0008 && upper < 0x0200;
}

//--------------------------------------------------------------------------------
int isInScratchpadMemory(void* p)
{
    u32 upper = (u32)p >> 16;
    u32 lower = (u32)p & 0xFFFF;
    return upper == 0x7000 && lower < 0x4000;
}

//--------------------------------------------------------------------------------
int isInIopMemory(void* p)
{
    u32 upper = (u32)p >> 16;
    return upper >= 0x1C00 && upper < 0x1C20;
}
