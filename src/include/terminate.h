#ifndef BEAVER_TERMINATE_H
#define BEAVER_TERMINATE_H

#include <io/vga.h>
#include <io/printf.h>

#define PANIC(MSG, ...) ({ \
    vga_set_foreground(COLOR_LIGHT_RED); \
    vga_set_background(COLOR_BLACK); \
    if (vga_get_x() != 0) { \
        vga_putc('\n'); \
    } \
    dprintf(vga_ocdev, "hOI!!!!!! Something haz gone... tERRIBLY WRONG!!\n" \
            "PANIC at " __FILE__ ":%d: " MSG "\n", __LINE__, ##__VA_ARGS__); \
    terminate(); \
})

#define WARNING(MSG, ...) ({ \
    uint8_t _old_vga_color = vga_get_color(); \
    vga_set_foreground(COLOR_LIGHT_MAGENTA); \
    vga_set_background(COLOR_BLACK); \
    if (vga_get_x() != 0) { \
        vga_putc('\n'); \
    } \
    dprintf(vga_ocdev, "hOI!!!!!! Something haz gone... wRONG!!\n" \
            "WARNING at " __FILE__ ":%d: " MSG "\n", __LINE__, \
            ##__VA_ARGS__); \
    vga_set_color(_old_vga_color); \
})

#define ASSERT(EXPR) ({ \
    if (!(EXPR)) { \
        vga_set_foreground(COLOR_LIGHT_RED); \
        vga_set_background(COLOR_BLACK); \
        if (vga_get_x() != 0) { \
            vga_putc('\n'); \
        } \
        dprintf(vga_ocdev, "hOI!!!!!! Something haz gone tERRIBLY WRONG!!\n" \
                "ASSERTION FAILED at " __FILE__ ":%d: " #EXPR "\n", \
                __LINE__); \
        terminate(); \
    } \
})

void intloop(void) __attribute__ ((noreturn));
void terminate(void) __attribute__ ((noreturn));

#endif /* BEAVER_TERMINATE_H */
