#ifndef LIBRAC5_STDLIB_H
#define LIBRAC5_STDLIB_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
/* Game-owned libc. Link librac5; call only after the game runtime is ready. */
void *malloc(size_t size);
void free(void *ptr);
int atoi(const char *s);
int rand(void);
#ifdef __cplusplus
}
#endif
#endif
