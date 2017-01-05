#ifndef BEAVER_STRING_H
#define BEAVER_STRING_H

#include <stdint.h>
#include <stddef.h>

int strcmp(const char *, const char *);
size_t strlen(const char *);
const char *strchr(const char *, int);
void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);

#endif // BEAVER_STRING_H
