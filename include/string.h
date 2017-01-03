#ifndef BEAVER_STRING_H
#define BEAVER_STRING_H

#include <stdint.h>
#include <stddef.h>

int strcmp(const char *, const char *);
size_t strlen(const char *);
const char *strchr(const char *, int);
void *memcpy(void *, const void *, uint32_t);
void *memmove(void *, const void *, uint32_t);

#endif // BEAVER_STRING_H
