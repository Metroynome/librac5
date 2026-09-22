#include "rac5.h"
const char *rac5BuildRegion(void)
{
#if defined(RAC5_NTSCU)
    return "ntscu";
#elif defined(RAC5_PAL)
    return "pal";
#elif defined(RAC5_NTSCJ)
    return "ntscj";
#else
    return "auto";
#endif
}
int rac5Init(Rac5Context *ctx, const volatile void *ram, uint32_t size)
{
    if (!ctx) return 0;
    ctx->ready = 0;
    ctx->memory.base = (const volatile unsigned char *)ram;
    ctx->memory.size = size;
    if (size != RAC5_EE_RAM_SIZE) return 0;
    ctx->ready = rac5FindBoot(&ctx->memory, &ctx->boot);
    return ctx->ready;
}
int rac5InitEE(Rac5Context *ctx)
{
#if defined(_EE)
    return rac5Init(ctx, (const volatile void *)0, RAC5_EE_RAM_SIZE);
#else
    (void)ctx;
    return 0;
#endif
}
Rac5Address rac5Resolve(const Rac5Context *ctx, const char *name)
{
    Rac5Symbol symbol;
    if (!ctx || !ctx->ready || !rac5FindSymbol(&ctx->memory, &ctx->boot, name, &symbol)) return 0;
    return symbol.address;
}
