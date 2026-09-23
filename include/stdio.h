#ifndef LIBRAC5_STDIO_H
#define LIBRAC5_STDIO_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
/* Game-owned libc. Link librac5; call only after the game runtime is ready. */
int printf(const char *format, ...);
int sprintf(char *buffer, const char *format, ...);
int snprintf(char *buffer, size_t capacity, const char *format, ...);
#ifdef DEBUG
#define DPRINTF(...) do { printf(__VA_ARGS__); } while (0)
#else
#define DPRINTF(...) do { } while (0)
#endif
#ifdef __cplusplus
}
#endif
#endif
