#include <common/panic.h>
#include <vga/vga.h>
#include <io/printf.h>

void panic(const char *msg, const char *filename, uint32_t line) {
    vga_set_foreground(COLOR_LIGHT_RED);
    vga_set_background(COLOR_BLACK);
    io_dprintf(vga_get_ocdev(), "PANIC at %s:%d: %s\n", filename, line, msg);
    terminate();
}
