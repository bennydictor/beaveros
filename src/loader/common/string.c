#include <string.h>
#include <stddef.h>

int strcmp(const char *str1, const char *str2) {
    while (*str1 && *str1 == *str2) {
        ++str1;
        ++str2;
    }

    return (int)(*str1) - (int)(*str2);
}

uint32_t strlen(const char *str) {
    uint32_t ans = 0;
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
