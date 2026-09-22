#include "interop.h"
#include <stddef.h>
static const Rac5Context *addressContext;
/* Explicit fields avoid undefined pointer arithmetic across struct members. */
Rac5Address rac5GetAddressForLevel(const VariableAddress_t *table, int id)
{
    if (!table) return 0;
    if (id == -1) id = 0;
    switch (id) {
    case 0: return table->MainMenu;
    case 1: return table->Pokitaru;
    case 2: return table->Ryllus;
    case 3: return table->Kalidon;
    case 4: return table->Metalis;
    case 5: return table->Dreamtime;
    case 6: return table->MedicalOutpostOmega;
    case 7: return table->Challax;
    case 8: return table->DayniMoon;
    case 9: return table->InsideClank;
    case 10: return table->Quodrona;
    case 11: return table->Roberto;
    case 12: return table->JumpLevel;
    case 13: return table->MungoArena;
    case 14: return table->ClankSegment;
    case 15: return table->MetalisGiantClank;
    case 16: return table->IslandEscape;
    case 17: return table->DangerValley;
    case 18: return table->MegaCannons;
    case 19: return table->MoonCowDisease;
    case 20: return table->MPLobby;
    case 21: return table->ChallaxGiantClank;
    case 22: return table->KalidonSkyboard;
    case 23: return table->MedicalOutpostSkyboard;
    case 24: return table->HIGTreehouse;
    default: return 0;
    }
}
int rac5SetAddressForLevel(VariableAddress_t *table, int id, Rac5Address address)
{
    if (!table) return 0;
    if (id == -1) id = 0;
    switch (id) {
    case 0: table->MainMenu=address; return 1;
    case 1: table->Pokitaru=address; return 1;
    case 2: table->Ryllus=address; return 1;
    case 3: table->Kalidon=address; return 1;
    case 4: table->Metalis=address; return 1;
    case 5: table->Dreamtime=address; return 1;
    case 6: table->MedicalOutpostOmega=address; return 1;
    case 7: table->Challax=address; return 1;
    case 8: table->DayniMoon=address; return 1;
    case 9: table->InsideClank=address; return 1;
    case 10: table->Quodrona=address; return 1;
    case 11: table->Roberto=address; return 1;
    case 12: table->JumpLevel=address; return 1;
    case 13: table->MungoArena=address; return 1;
    case 14: table->ClankSegment=address; return 1;
    case 15: table->MetalisGiantClank=address; return 1;
    case 16: table->IslandEscape=address; return 1;
    case 17: table->DangerValley=address; return 1;
    case 18: table->MegaCannons=address; return 1;
    case 19: table->MoonCowDisease=address; return 1;
    case 20: table->MPLobby=address; return 1;
    case 21: table->ChallaxGiantClank=address; return 1;
    case 22: table->KalidonSkyboard=address; return 1;
    case 23: table->MedicalOutpostSkyboard=address; return 1;
    case 24: table->HIGTreehouse=address; return 1;
    default: return 0;
    }
}
static uint32_t readWord(const Rac5Memory *memory, uint32_t address)
{
    const volatile unsigned char *p = (const volatile unsigned char *)((uintptr_t)memory->base + address);
    return (uint32_t)p[0] | ((uint32_t)p[1]<<8) | ((uint32_t)p[2]<<16) | ((uint32_t)p[3]<<24);
}
int rac5GetCurrentLevel(const Rac5Context *ctx)
{
    uint32_t getter, hi, low, address, level;
    if (!ctx || !ctx->ready || ctx->memory.size!=RAC5_EE_RAM_SIZE) return RAC5_LEVEL_UNKNOWN;
    getter=rac5Resolve(ctx,"MCPTRANS_GetLevel__Fv");
    if (!getter || (getter&3) || getter>ctx->memory.size-16) return RAC5_LEVEL_UNKNOWN;
    hi=readWord(&ctx->memory,getter); low=readWord(&ctx->memory,getter+8);
    /* lui v1,hi; jr ra; lw v0,lo(v1); nop, verified in all three retail ELFs. */
    if ((hi&0xffff0000u)!=0x3c030000u || readWord(&ctx->memory,getter+4)!=0x03e00008u ||
        (low&0xffff0000u)!=0x8c620000u || readWord(&ctx->memory,getter+12)!=0)
        return RAC5_LEVEL_UNKNOWN;
    address=((hi&0xffffu)<<16)+(int32_t)(int16_t)(low&0xffffu);
    if ((address&3) || address<0x100000u || address>ctx->memory.size-4) return RAC5_LEVEL_UNKNOWN;
    level=readWord(&ctx->memory,address);
    return level<RAC5_LEVEL_SLOT_COUNT ? (int)level : RAC5_LEVEL_UNKNOWN;
}
Rac5Address rac5GetAddress(const Rac5Context *ctx, const VariableAddress_t *table)
{
    return rac5GetAddressForLevel(table,rac5GetCurrentLevel(ctx));
}
void rac5SetAddressContext(const Rac5Context *ctx) { addressContext=ctx; }
Rac5Address GetAddress(VariableAddress_t *table) { return rac5GetAddress(addressContext,table); }
Rac5Address GetAddressImmediate(VariableAddress_t *table) { return GetAddress(table); }
int rac5SetModuleSymbolAddress(const Rac5Context *ctx, const Rac5Module *module,
                              VariableAddress_t *table, int id, const char *name)
{
    Rac5Symbol symbol;
    char source[128];
    const char *expected=rac5LevelSource(id);
    unsigned i;
    if (!rac5SetAddressForLevel(table,id,0) || !expected || !ctx || !ctx->ready || !module) return 0;
    if (!rac5ModuleValid(&ctx->memory,module) ||
        !rac5ReadString(&ctx->memory,module->source,source,sizeof(source))) return 0;
    for (i=0; source[i] && expected[i] && source[i]==expected[i]; ++i) {}
    if (source[i]!=expected[i]) return 0;
    if (!rac5FindSymbol(&ctx->memory,module,name,&symbol)) return 0;
    /* VariableAddress stores usable EE addresses, not unmapped linker constants. */
    if (symbol.address>=ctx->memory.size) return 0;
    return rac5SetAddressForLevel(table,id,symbol.address);
}
