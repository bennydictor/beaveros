#include <vga/vga.h>

void kernel_main(void) {
    vga_init();
    vga_set_foreground(COLOR_LIGHT_GREEN);
    vga_puts("\n\n>>> Hello world!");
}
