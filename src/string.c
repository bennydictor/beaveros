#include <string.h>
#include <stddef.h>
#include <stdint.h>

/* TODO: optimize string.c */

int strcmp(const char *str1, const char *str2) {
    while (*str1 && *str1 == *str2) {
        ++str1;
        ++str2;
    }

    return (int) *str1 - (int) *str2;
}

int strncmp(const char *str1, const char *str2, size_t n) {
    if (n == 0) {
        return 0;
    }
    while (--n && *str1 && *str1 == *str2) {
        ++str1;
        ++str2;
    }

    return (int) *str1 - (int) *str2;
}

size_t strlen(const char *str) {
    size_t ans = 0;
    while (*str++) {
        ++ans;
    }
    return ans;
}

const char *strchr(const char *str, int character) {
    while (*str && *str != character) {
        ++str;
    }
    return *str ? str : NULL;
}

void *memcpy(void *dest, const void *src, size_t num) {
    uint8_t *dest_copy = dest;
    const uint8_t *src_copy = src;
    while (num--) {
        *dest_copy++ = *src_copy++;
    }
    return dest;
}

void *memmove(void *dest, const void *src, size_t num) {
    uint8_t *dest_copy = dest;
    const uint8_t *src_copy = src;

    if (dest_copy < src_copy) {
        while (num--) {
            dest_copy[num] = src_copy[num];
        }
    } else if (dest_copy > src_copy) {
        while (num--) {
            *dest_copy++ = *src_copy++;
        }
    }

    return dest;
}

void *memset(void *ptr, int val, size_t num) {
    uint8_t *addr = ptr;
    while (num--) {
        addr[num] = val;
    }
    return ptr;
}
