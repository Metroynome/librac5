#ifndef LIBRAC5_MOBY_H
#define LIBRAC5_MOBY_H
#include "interop.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct Moby Moby;
typedef struct MobyClass MobyClass;
extern VariableAddress_t vaMobyCreate, vaMobyDestroy;
/* Uses a free slot of a loaded class; transform is 16 aligned floats or NULL; group 0xff uses the class default. */
Moby *mobyCreate(MobyClass *, unsigned int modelSignature, const float *transform, unsigned int group);
/* effects is used by single-player; multiplayer ignores it. */
void mobyDestroy(Moby *, int effects);
MobyClass *mobyGetClass(const Moby *);
void *mobyGetPVar(const Moby *);
#ifdef __cplusplus
}
#endif
#endif
