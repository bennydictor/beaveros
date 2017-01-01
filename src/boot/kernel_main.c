#include <vga/vga.h>
#include <io/stddev.h>
#include <io/printf.h>
#include <common/panic.h>

void kernel_internal_printf_testing(void) {
    io_printf("Printf testing (specifier is %%+-#08.3{d,o,x} ): \n");
    io_printf("%+-#08.3d%+-#08.3o%+-#08.3x\n", 100, 100, 100);
    io_printf("%*s%*s%*s\n", 8, "Dec", 8, "Oct", 8, "Hex");
    io_printf("I can even print symbols: %c\n", 'q'); 
}

void kernel_main(uint32_t eax, uint32_t ebx) {
    vga_init();
    vga_set_foreground(COLOR_LIGHT_GREEN);

    io_set_std_ocdev(vga_get_ocdev());

    io_printf("\n\n>>> Hello world!\n");
    io_printf("Registers values: EAX = %#.8x, EBX = %#.8x\n", eax, ebx);
    PANIC("TEST_PANIC");
    io_printf("You'll never see me!\n");
}
