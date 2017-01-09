#ifndef BEAVER_VGA_H
#define BEAVER_VGA_H

#include <stdint.h>
#include <io/ocdev.h>

typedef enum {
    COLOR_BLACK,
    COLOR_BLUE,
    COLOR_GREEN,
    COLOR_CYAN,
    COLOR_RED,
    COLOR_MAGENTA,
    COLOR_BROWN,
    COLOR_LIGHT_GREY,
    COLOR_DARK_GREY,
    COLOR_LIGHT_BLUE,
    COLOR_LIGHT_GREEN,
    COLOR_LIGHT_CYAN,
    COLOR_LIGHT_RED,
    COLOR_LIGHT_MAGENTA,
    COLOR_YELLOW,
    COLOR_WHITE,
} vga_color_t;

void vga_init(void);

void vga_putc(char);
void vga_clear(void);
void vga_puts(const char *);
void vga_putsl(const char *, uint32_t);

void vga_move_cursor(uint16_t, uint16_t);
uint16_t vga_get_x(void);
uint16_t vga_get_y(void);
uint16_t vga_get_width(void);
uint16_t vga_get_height(void);

void vga_set_color(uint8_t);
void vga_set_foreground(vga_color_t);
void vga_set_background(vga_color_t);

ocdev_t vga_get_ocdev(void);

#endif // BEAVER_VGA_H
