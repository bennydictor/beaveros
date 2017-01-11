#include <vga.h>
#include <io/ocdev.h>
#include <io/printf.h>
#include <registers.h>


void kernel_main(void) {
    vga_init();
    vga_set_foreground(COLOR_LIGHT_GREEN);
    set_std_ocdev(vga_get_ocdev());

    printf("CR0 = %#.16llx\n", get_cr(0));
    printf("CR3 = %#.16llx\n", get_cr(3));
    printf("CR4 = %#.16llx\n", get_cr(4));
}
