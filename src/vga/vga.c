#include <vga/vga.h>
#include <io/portio.h>

uint16_t *vga_buffer;
uint16_t vga_width;
uint16_t vga_height;
uint16_t vga_x;
uint16_t vga_y;
uint8_t vga_color;
const ocdev_t vga_ocdev = {vga_putc, vga_puts, vga_putsl};

void vga_internal_update_cursor(void) {
    uint16_t cursor = vga_x + vga_width * vga_y;
    outb(0x3D4, 14);
    outb(0x3D5, cursor >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursor);
}

void vga_internal_scroll(void) {
    uint16_t blank = vga_color;
    blank <<= 8;
    blank |= ' ';
    if (vga_y >= vga_height) {
        uint16_t y_to = 0;
        for (uint16_t y_from = vga_y - vga_height + 1; y_from < vga_height; ++y_to, ++y_from) {
            for (uint16_t x = 0; x < vga_width; ++x) {
                vga_buffer[vga_width * y_to + x] = vga_buffer[vga_width * y_from + x];
            }
        }
        vga_x = 0;
        vga_y = y_to;
        while (y_to < vga_height) {
            for (uint16_t x = 0; x < vga_width; ++x) {
                vga_buffer[vga_width * y_to + x] = blank;
            }
            ++y_to;
        }
    }
    vga_internal_update_cursor();
}

void vga_init(void) {
    vga_buffer = (uint16_t *)0xb8000;
    vga_width = 80;
    vga_height = 25;
    vga_set_color(COLOR_BLACK << 4 | COLOR_WHITE);
    vga_clear();
    vga_move_cursor(0, 0);
}

void vga_putc(char c) {
    if (c == '\b') {
        if (vga_x /* > 0*/) { 
            --vga_x;
        }
    } else if (c == '\t') {
        do {
            vga_putc(' ');
        } while (vga_x & 7);
    } else if (c == '\r') {
        vga_x = 0;
    } else if (c == '\n') {
        vga_x = 0;
        ++vga_y;
    } else if (c >= ' ') {
        vga_buffer[vga_x + vga_width * vga_y] = (uint16_t)(c) | ((uint16_t)(vga_color) << 8);
        ++vga_x;
    }

    if (vga_x >= vga_width) {
        vga_x = 0;
        ++vga_y;
    }

    vga_internal_scroll();
    vga_internal_update_cursor();
}

void vga_clear() {
    uint16_t blank = vga_color;
    blank <<= 8;
    blank |= ' ';
    for (uint16_t i = 0; i < vga_width * vga_height; ++i) {
        vga_buffer[i] = blank;
    }
    vga_x = 0;
    vga_y = 0;
    vga_internal_update_cursor();
}

void vga_puts(const char *str) {
    while (*str) {
        vga_putc(*str++);
    }
}

void vga_putsl(const char *str, uint32_t length) {
    while (length--) {
        vga_putc(*str++);
    }
}

void vga_move_cursor(uint16_t x, uint16_t y) {
    vga_x = x;
    vga_y = y;
    vga_internal_update_cursor();
}

uint16_t vga_get_x() {
    return vga_x;
}

uint16_t vga_get_y() {
    return vga_y;
}

uint16_t vga_get_width() {
    return vga_width;
}

uint16_t vga_get_height() {
    return vga_height;
}

void vga_set_color(uint8_t color) {
    vga_color = color;
}

void vga_set_foreground(vga_color_t color) {
    vga_color &= -16;
    vga_color |= color;
}

void vga_set_background(vga_color_t color) {
    vga_color &= 15;
    vga_color |= color << 4;
}

ocdev_t vga_get_ocdev(void) {
    return vga_ocdev;
}
