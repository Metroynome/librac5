#ifndef LIBRAC5_H
#define LIBRAC5_H
#include "types.h"
#include "region.h"
#include "symbols.h"
#include "level.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct Rac5Context { Rac5Memory memory; Rac5Module boot; int ready; } Rac5Context;
/* Caller owns context; no heap, libc, system calls, or guessed addresses. */
int rac5Init(Rac5Context *, const volatile void *ram, uint32_t size);
int rac5InitEE(Rac5Context *);
/* Matches table location/count and four symbol addresses; not a whole-image hash.
 * NULL means an unknown or modified build. Initialization remains generic.
 * Returned immutable storage belongs to the library. */
const Rac5BuildInfo *rac5GetBuild(const Rac5Context *);
Rac5Address rac5Resolve(const Rac5Context *, const char *linkageName);
#ifdef __cplusplus
}
#endif
#endif
