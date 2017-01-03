#ifndef BEAVER_STRING_H
#define BEAVER_STRING_H

#include <stdint.h>

int strcmp(const char *str1, const char *str2);
uint32_t strlen(const char *str);
const char *strchr(const char *str, int character);
void *memcpy(void *dest, const void *src, uint32_t num);

#endif // BEAVER_STRING_H
