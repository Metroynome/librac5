#include "rac5.h"
typedef struct {
    Rac5BuildInfo info;
    Rac5Address memsetAddress, printfAddress, loadLevelAddress, sendLevelAddress;
} KnownBuild;
#include "region_profiles.inc"

const Rac5BuildInfo *rac5GetBuild(const Rac5Context *ctx)
{
    unsigned i;
    if (!ctx || !ctx->ready || !rac5ModuleValid(&ctx->memory, &ctx->boot)) return 0;
    for (i=0; i<sizeof(knownBuilds)/sizeof(knownBuilds[0]); ++i) {
        const KnownBuild *b = &knownBuilds[i];
        if (ctx->boot.header == b->info.symbolTable && ctx->boot.count == b->info.symbolCount &&
            rac5Resolve(ctx, "memset") == b->memsetAddress &&
            rac5Resolve(ctx, "printf") == b->printfAddress &&
            rac5Resolve(ctx, "MCP_LoadLevelModule__Fi") == b->loadLevelAddress &&
            rac5Resolve(ctx, "MCPI_SendNewLevel__Fi") == b->sendLevelAddress)
            return &b->info;
    }
    return 0;
}
