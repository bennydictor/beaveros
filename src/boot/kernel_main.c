#include <vga/vga.h>
#include <io/stddev.h>

void kernel_main(void) {
    vga_init();
    vga_set_foreground(COLOR_LIGHT_GREEN);

    io_set_std_ocdev(vga_get_ocdev());

    vga_puts("\n\n>>> Hello world!");
}
