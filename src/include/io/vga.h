#ifndef BEAVER_VGA_H
#define BEAVER_VGA_H

#include <stdint.h>
#include <stddef.h>
#include <io/ocdev.h>

#define COLOR_BLACK         0
#define COLOR_BLUE          1
#define COLOR_GREEN         2
#define COLOR_CYAN          3
#define COLOR_RED           4
#define COLOR_MAGENTA       5
#define COLOR_BROWN         6
#define COLOR_LIGHT_GREY    7
#define COLOR_DARK_GREY     8
#define COLOR_LIGHT_BLUE    9
#define COLOR_LIGHT_GREEN   10
#define COLOR_LIGHT_CYAN    11
#define COLOR_LIGHT_RED     12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_YELLOW        14
#define COLOR_WHITE         15

extern uint16_t *vga_buffer;

void vga_init(void *vga_buffer);

void vga_putc(char);
void vga_clear(void);
int vga_puts(const char *);
int vga_putns(const char *, size_t);

void vga_move_cursor(uint16_t x, uint16_t y);
uint16_t vga_get_x(void);
uint16_t vga_get_y(void);
uint16_t vga_get_width(void);
uint16_t vga_get_height(void);

uint8_t vga_get_color(void);
void vga_set_color(uint8_t);
void vga_set_foreground(uint8_t);
void vga_set_background(uint8_t);

extern const ocdev_t vga_ocdev;

#endif /* BEAVER_VGA_H */
