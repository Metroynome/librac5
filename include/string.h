#ifndef LIBRAC5_STRING_H
#define LIBRAC5_STRING_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
/* Game-owned libc. Link librac5; call only after the game runtime is ready. */
void *memset(void *dest, int value, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
void *memmove(void *dest, const void *src, size_t count);
int memcmp(const void *a, const void *b, size_t count);
void *memchr(const void *data, int value, size_t count);
size_t strlen(const char *s);
int strcmp(const char *a, const char *b);
int strncmp(const char *a, const char *b, size_t count);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t count);
char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t count);
char *strchr(const char *s, int value);
#ifdef __cplusplus
}
#endif
#endif
