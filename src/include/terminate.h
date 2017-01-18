#ifndef BEAVER_TERMINATE_H
#define BEAVER_TERMINATE_H

#include <io/vga.h>
#include <io/printf.h>

#define PANIC(MSG, ...) ({ \
    vga_set_foreground(COLOR_LIGHT_RED); \
    vga_set_background(COLOR_BLACK); \
    dprintf(vga_ocdev, "PANIC at " __FILE__ ":%d: " MSG, \
            __LINE__, ##__VA_ARGS__); \
    terminate(); \
})

#define WARNING(MSG, ...) ({ \
    vga_set_foreground(COLOR_LIGHT_MAGENTA); \
    vga_set_background(COLOR_BLACK); \
    dprintf(vga_ocdev, "WARNING at " __FILE__ ":%d: " MSG, \
            __LINE__, ##__VA_ARGS__); \
})

#define ASSERT(EXPR) ({ \
    if (!(EXPR)) { \
        PANIC("assertion failed: (" #EXPR ")"); \
    } \
})

void terminate(void) __attribute__ ((noreturn));

#endif /* BEAVER_TERMINATE_H */
