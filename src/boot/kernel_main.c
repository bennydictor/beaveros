#include <io/vga.h>
#include <io/ocdev.h>
#include <io/printf.h>
#include <registers.h>
#include <terminate.h>

__attribute__ ((force_align_arg_pointer))
__attribute__ ((noreturn))
void kernel_main(void) {
    vga_init();
    vga_set_foreground(COLOR_LIGHT_GREEN);
    std_ocdev = vga_ocdev;

    printf("CR0 = %#.16llx\n", get_cr(0));
    printf("CR3 = %#.16llx\n", get_cr(3));
    printf("CR4 = %#.16llx\n", get_cr(4));

    terminate();
}
