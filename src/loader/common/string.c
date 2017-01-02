#include <string.h>
#include <stddef.h>

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
