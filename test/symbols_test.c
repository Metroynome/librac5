#include "rac5.h"
#include "interop.h"
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static unsigned char ram[RAC5_EE_RAM_SIZE];
static void put(uint32_t a, uint32_t v) { ram[a]=v; ram[a+1]=v>>8; ram[a+2]=v>>16; ram[a+3]=v>>24; }
static void fixture(uint32_t base, uint32_t delta)
{
    static const char *names[]={"memset","MCP_LoadLevelModule__Fi","MCPI_SendNewLevel__Fi"};
    unsigned i;
    put(base,0x32524e53);put(base+12,base+0x80);put(base+16,3);
    for(i=0;i<3;++i) {
        uint32_t r=base+0x80+i*12,n=base+0x100+i*64;
        strcpy((char *)ram+n,names[i]);put(r,n);put(r+4,0x1800000+delta+i*16);ram[r+10]=2;
    }
}
int main(int argc, char **argv)
{
    VariableAddress_t addresses = {0};
    Rac5Context ctx;
    assert(sizeof(VariableAddress_t)==25*sizeof(Rac5Address));
    assert(offsetof(VariableAddress_t,MainMenu)==0);
    assert(offsetof(VariableAddress_t,IslandEscape)==16*sizeof(Rac5Address));
    assert(offsetof(VariableAddress_t,HIGTreehouse)==24*sizeof(Rac5Address));
    assert(!GetAddress(&addresses));
    Rac5Module module;
    Rac5Symbol symbol;
    uint32_t base;
    char text[128];
    /* Synthetic relocation checks, not claims of real PAL/Japanese validation. */
    for(base=0x100000; base<=0x130000; base+=0x10000) {
        memset(ram,0,sizeof(ram));fixture(base,base-0x100000);
        assert(rac5Init(&ctx,ram,sizeof(ram)));
        assert(rac5Resolve(&ctx,"memset")==0x1800000+base-0x100000);
        assert(!rac5Resolve(&ctx,"missing"));
        assert(rac5SymbolAt(&ctx.memory,&ctx.boot,0,&symbol));
        assert(rac5ReadString(&ctx.memory,symbol.name,text,sizeof(text)));
        assert(!strcmp(text,"memset"));
        assert(!rac5ReadString(&ctx.memory,symbol.name,text,3));
        assert(!rac5SymbolAt(&ctx.memory,&ctx.boot,3,&symbol));
        fixture(0x140000,0);assert(!rac5FindBoot(&ctx.memory,&module));
        put(0x140000,0);
        put(base+12,0xfffffff0);assert(!rac5Resolve(&ctx,"memset"));
    }
    memset(ram,0,sizeof(ram));fixture(0x100000,0);
    put(0x10000c,RAC5_EE_RAM_SIZE-4);
    assert(!rac5Init(&ctx,ram,sizeof(ram)));
    fixture(0x100000,0);put(0x100080,RAC5_EE_RAM_SIZE-2);
    ram[RAC5_EE_RAM_SIZE-2]='x';ram[RAC5_EE_RAM_SIZE-1]='y';
    assert(!rac5Init(&ctx,ram,sizeof(ram)));
    assert(!rac5Init(&ctx,ram,128));
    assert(!rac5InitEE(&ctx));
    assert(!rac5Init(NULL,ram,sizeof(ram)));
    assert(!strcmp(rac5LevelName(16),"Island Escape"));
    assert(!rac5LevelName(11));
    if(argc>1) {
        FILE *f=fopen(argv[1],"rb");assert(f);
        assert(fread(ram,1,sizeof(ram),f)==sizeof(ram));assert(fgetc(f)==EOF);fclose(f);
        assert(rac5Init(&ctx,ram,sizeof(ram)));
        assert(ctx.boot.header==0x01f0ad00 && ctx.boot.count==1804);
        assert(rac5Resolve(&ctx,"memset")==0x01ec62f4);
        assert(rac5FindModule(&ctx.memory,rac5LevelSource(16),&module));
        assert(module.header==0x00e8b380 && module.count==262);
        assert(rac5GetCurrentLevel(&ctx)==16);
        assert(rac5SetModuleSymbolAddress(&ctx,&module,&addresses,16,"lib_main__FiPPc"));
        assert(addresses.IslandEscape==0x00ea2da8);
        assert(!rac5SetModuleSymbolAddress(&ctx,&module,&addresses,17,"lib_main__FiPPc"));
        rac5SetAddressContext(&ctx);
        assert(GetAddress(&addresses)==0x00ea2da8);
        assert(GetAddressImmediate(&addresses)==0x00ea2da8);
        assert(!rac5SetModuleSymbolAddress(&ctx,&module,&addresses,16,"missing"));
        assert(!GetAddress(&addresses));
        rac5SetAddressContext(NULL);
        assert(rac5FindSymbol(&ctx.memory,&module,"lib_main__FiPPc",&symbol));
        assert(symbol.address==0x00ea2da8);
        assert(rac5FindSymbol(&ctx.memory,&module,"G_wadFilename",&symbol));
        assert(symbol.address==0x0105b828);
        assert(rac5ReadString(&ctx.memory,symbol.address,text,sizeof(text)));
        assert(!strcmp(text,"ART/LEVELS/levelMP01/DATA/levelMP01.mb.wad"));
        /* Verify every resolved level import against boot, not just one example. */
        {
            unsigned i,imports=0;Rac5Symbol imported,boot;
            for(i=0;i<module.count;++i) {
                if(!rac5SymbolAt(&ctx.memory,&module,i,&imported) || imported.linkage!=1) continue;
                assert(rac5ReadString(&ctx.memory,imported.name,text,sizeof(text)));
                assert(rac5FindSymbol(&ctx.memory,&ctx.boot,text,&boot));
                assert(imported.address==boot.address);++imports;
            }
            assert(imports==259);
        }
        ram[module.source]='X';assert(!rac5ModuleValid(&ctx.memory,&module));
        puts("Island Escape dump: boot + level + all 259 imports verified");
    }
    puts("Synthetic relocated tables, malformed bounds, ambiguous boot, stale module: passed");
    return 0;
}
