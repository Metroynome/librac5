#include "symbols.h"
#define SNR2 0x32524e53u
static int range(const Rac5Memory *m, uint32_t a, uint32_t n)
{
    return m && m->size <= RAC5_EE_RAM_SIZE && a <= m->size && n <= m->size-a;
}
static unsigned char byte(const Rac5Memory *m, uint32_t a)
{
    return *(const volatile unsigned char *)((uintptr_t)m->base + a);
}
static uint32_t word(const Rac5Memory *m, uint32_t a)
{
    return (uint32_t)byte(m,a) | ((uint32_t)byte(m,a+1)<<8) |
           ((uint32_t)byte(m,a+2)<<16) | ((uint32_t)byte(m,a+3)<<24);
}
static int stringHash(const Rac5Memory *m, uint32_t a, uint32_t *hash)
{
    uint32_t i, h=2166136261u;
    if (!a) return 0;
    for (i=0; i<RAC5_SYMBOL_NAME_MAX && range(m,a,i+1); ++i) {
        unsigned char c=byte(m,a+i);
        if (!c) { *hash=h; return i != 0; }
        if (c<32 || c>126) return 0;
        h=(h^c)*16777619u;
    }
    return 0;
}
int rac5ReadString(const Rac5Memory *m, Rac5Address a, char *out, uint32_t capacity)
{
    uint32_t i;
    if (!out || !capacity || !a) return 0;
    out[0]=0;
    for (i=0; i+1<capacity && i<RAC5_SYMBOL_NAME_MAX && range(m,a,i+1); ++i) {
        out[i]=(char)byte(m,a+i);
        if (!out[i]) return 1;
    }
    out[0]=0;
    return 0;
}
static int sameString(const Rac5Memory *m, uint32_t a, const char *s)
{
    uint32_t i;
    if (!s || !a) return 0;
    for (i=0; i<RAC5_SYMBOL_NAME_MAX && range(m,a,i+1); ++i) {
        unsigned char c=byte(m,a+i);
        if (c!=(unsigned char)s[i]) return 0;
        if (!c) return 1;
    }
    return 0;
}
int rac5ModuleAt(const Rac5Memory *m, Rac5Address a, Rac5Module *out)
{
    Rac5Module v;
    uint32_t hash;
    if (!out || (a&3) || !range(m,a,0x3c) || word(m,a)!=SNR2) return 0;
    v.header=a; v.table=word(m,a+12); v.count=word(m,a+16); v.source=word(m,a+20); v.sourceHash=0;
    if (!v.table || (v.table&3) || !v.count || v.count>65536 || !range(m,v.table,v.count*12)) return 0;
    if (v.source) {
        if (!stringHash(m,v.source,&hash)) return 0;
        v.sourceHash=hash;
    }
    out->header=v.header; out->table=v.table; out->source=v.source;
    out->count=v.count; out->sourceHash=v.sourceHash;
    return 1;
}
int rac5ModuleValid(const Rac5Memory *m, const Rac5Module *module)
{
    Rac5Module now;
    return module && rac5ModuleAt(m,module->header,&now) && now.table==module->table &&
           now.count==module->count && now.source==module->source && now.sourceHash==module->sourceHash;
}
static int symbolAt(const Rac5Memory *m, const Rac5Module *module, uint32_t index, Rac5Symbol *out)
{
    uint32_t a, hash;
    if (!out || index>=module->count) return 0;
    a=module->table+index*12;
    out->name=word(m,a); out->address=word(m,a+4);
    out->linkage=byte(m,a+10); out->processed=byte(m,a+11);
    return out->address && out->linkage>=1 && out->linkage<=4 && stringHash(m,out->name,&hash);
}
int rac5SymbolAt(const Rac5Memory *m, const Rac5Module *module, uint32_t index, Rac5Symbol *out)
{
    return rac5ModuleValid(m,module) && symbolAt(m,module,index,out);
}
int rac5FindSymbol(const Rac5Memory *m, const Rac5Module *module, const char *name, Rac5Symbol *out)
{
    uint32_t i;
    Rac5Symbol symbol;
    if (!out || !name || !rac5ModuleValid(m,module)) return 0;
    for (i=0; i<module->count; ++i) {
        if (symbolAt(m,module,i,&symbol) && sameString(m,symbol.name,name)) { out->name=symbol.name; out->address=symbol.address;
            out->linkage=symbol.linkage; out->processed=symbol.processed; return 1; }
    }
    return 0;
}
static int scan(const Rac5Memory *m, const char *source, Rac5Module *out)
{
    uint32_t a;
    int found=0;
    Rac5Module candidate;
    Rac5Symbol symbol;
    if (!out || !m || m->size!=RAC5_EE_RAM_SIZE) return 0;
    for (a=0x00100000; a<=m->size-0x3c; a+=4) {
        if (word(m,a)!=SNR2 || !rac5ModuleAt(m,a,&candidate)) continue;
        if (source) {
            if (!candidate.source || !sameString(m,candidate.source,source)) continue;
        } else {
            if (candidate.source ||
                !rac5FindSymbol(m,&candidate,"memset",&symbol) ||
                !rac5FindSymbol(m,&candidate,"MCP_LoadLevelModule__Fi",&symbol) ||
                !rac5FindSymbol(m,&candidate,"MCPI_SendNewLevel__Fi",&symbol)) continue;
        }
        if (found) return 0; /* Ambiguous resident tables: never pick an arbitrary one. */
        out->header=candidate.header; out->table=candidate.table; out->source=candidate.source;
        out->count=candidate.count; out->sourceHash=candidate.sourceHash; found=1;
    }
    return found;
}
int rac5FindBoot(const Rac5Memory *m, Rac5Module *out) { return scan(m,0,out); }
int rac5FindModule(const Rac5Memory *m, const char *source, Rac5Module *out)
{
    return source && *source && scan(m,source,out);
}
