#ifndef BEAVER_PANIC_H
#define BEAVER_PANIC_H

#include <io/vga.h>
#include <io/printf.h>
#include <terminate.h>

#define PANIC(MSG, ...) _PANIC_HELPER(MSG, __FILE__, __LINE__, ##__VA_ARGS__)

#define _PANIC_HELPER(MSG, F, L, ...) ({ \
    vga_set_foreground(COLOR_LIGHT_RED); \
    vga_set_background(COLOR_BLACK); \
    dprintf(vga_ocdev, "PANIC at " F ":%d: " MSG, __LINE__, ##__VA_ARGS__); \
    terminate(); \
})

#define WARNING(MSG, ...) _PANIC_HELPER(MSG, __FILE__, __LINE__, ##__VA_ARGS__)
#define _WARNING_HELPER(MSG, F, L, ...) ({ \
    vga_set_foreground(COLOR_LIGHT_MAGENTA); \
    vga_set_background(COLOR_BLACK); \
    dprintf(vga_ocdev, "WARNING at " F ":%d: " MSG, __LINE__, ##__VA_ARGS__); \
    terminate(); \
})

#endif /* BEAVER_PANIC_H */
