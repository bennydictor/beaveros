#include <vga/vga.h>
#include <io/stddev.h>
#include <io/printf.h>

void kernel_main(void) {
    vga_init();
    vga_set_foreground(COLOR_LIGHT_GREEN);

    io_set_std_ocdev(vga_get_ocdev());

    io_printf("\n\n>>> Hello world! This shit works! %05u\n", 123);
}
