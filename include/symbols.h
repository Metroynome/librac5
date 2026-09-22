#ifndef LIBRAC5_SYMBOLS_H
#define LIBRAC5_SYMBOLS_H
#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif
#define RAC5_EE_RAM_SIZE 0x02000000u
#define RAC5_SYMBOL_NAME_MAX 1024u
/* base points to a full EE image on a host; zero denotes physical EE RAM on PS2. */
typedef struct Rac5Memory { const volatile unsigned char *base; uint32_t size; } Rac5Memory;
typedef struct Rac5Module {
    Rac5Address header, table, source;
    uint32_t count, sourceHash;
} Rac5Module;
typedef struct Rac5Symbol {
    Rac5Address name, address;
    unsigned char linkage, processed;
} Rac5Symbol;
enum { RAC5_SYMBOL_EXTERNAL=1, RAC5_SYMBOL_RELATIVE=2,
       RAC5_SYMBOL_WEAK=3, RAC5_SYMBOL_ABSOLUTE=4 };
/* 1=success, 0=missing/invalid. All pointers in loaded tables are absolute. */
int rac5ReadString(const Rac5Memory *, Rac5Address, char *, uint32_t);
int rac5ModuleAt(const Rac5Memory *, Rac5Address, Rac5Module *);
int rac5ModuleValid(const Rac5Memory *, const Rac5Module *);
int rac5SymbolAt(const Rac5Memory *, const Rac5Module *, uint32_t, Rac5Symbol *);
int rac5FindSymbol(const Rac5Memory *, const Rac5Module *, const char *, Rac5Symbol *);
/* Explicit O(RAM size) scans: use during initialization, never every frame. */
int rac5FindBoot(const Rac5Memory *, Rac5Module *);
/* Finds exactly one matching resident REL; does not assert it is actively executing. */
int rac5FindModule(const Rac5Memory *, const char *sourcePath, Rac5Module *);
#ifdef __cplusplus
}
#endif
#endif
