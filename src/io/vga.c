#include <io/vga.h>
#include <io/port.h>

uint16_t *vga_buffer;
static uint16_t vga_width;
static uint16_t vga_height;
static uint16_t vga_x;
static uint16_t vga_y;
static uint8_t vga_color;
const ocdev_t vga_ocdev = { vga_putc, vga_puts, vga_putns };

static void vga_update_cursor(void) {
    uint16_t cursor = vga_x + vga_width * vga_y;
    /* TODO: defines for IO ports */
    outb(0x3D4, 14);
    outb(0x3D5, cursor >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursor);
}

static void vga_scroll(void) {
    uint16_t blank = vga_color;
    blank <<= 8;
    blank |= ' ';
    if (vga_y >= vga_height) {
        uint16_t y_to = 0;
        for (uint16_t y_from = vga_y - vga_height + 1; y_from < vga_height;
                ++y_to, ++y_from) {
            for (uint16_t x = 0; x < vga_width; ++x) {
                vga_buffer[vga_width * y_to + x] =
                        vga_buffer[vga_width * y_from + x];
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
    vga_update_cursor();
}

void vga_init(void *vbuffer) {
    vga_buffer = vbuffer;
    vga_width = 80;
    vga_height = 25;
    vga_set_color(COLOR_BLACK << 4 | COLOR_WHITE);
    vga_clear();
    vga_move_cursor(0, 0);
}

void vga_putc(char c) {
    if (c == '\b') {
        if (vga_x > 0) {
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
        vga_buffer[vga_x + vga_width * vga_y] =
                (uint16_t) c | ((uint16_t) vga_color << 8);
        ++vga_x;
    }

    if (vga_x >= vga_width) {
        vga_x = 0;
        ++vga_y;
    }

    vga_scroll();
    vga_update_cursor();
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
    vga_update_cursor();
}

int vga_puts(const char *str) {
    int ret = 0;
    while (*str) {
        vga_putc(*str++);
        ++ret;
    }
    return ret;
}

int vga_putns(const char *str, size_t length) {
    int ret = 0;
    while (*str && length--) {
        vga_putc(*str++);
        ++ret;
    }
    return ret;
}

void vga_move_cursor(uint16_t x, uint16_t y) {
    vga_x = x;
    vga_y = y;
    vga_update_cursor();
}

uint16_t vga_get_x(void) {
    return vga_x;
}

uint16_t vga_get_y(void) {
    return vga_y;
}

uint16_t vga_get_width(void) {
    return vga_width;
}

uint16_t vga_get_height(void) {
    return vga_height;
}

void vga_set_color(uint8_t color) {
    vga_color = color;
}

uint8_t vga_get_color(void) {
    return vga_color;
}

void vga_set_foreground(uint8_t color) {
    vga_color &= 0xf0;
    vga_color |= color;
}

void vga_set_background(uint8_t color) {
    vga_color &= 0x0f;
    vga_color |= color << 4;
}

ocdev_t vga_get_ocdev(void) {
    return vga_ocdev;
}
