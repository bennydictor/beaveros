#ifndef BEAVER_IO_PRINTF_H
#define BEAVER_IO_PRINTF_H

#include <stdarg.h>
#include <stdint.h>
#include <io/output_character_device.h>

typedef struct {
    uint8_t flags;
    char specifier;
    uint16_t width, precision;
    uint16_t length;
} io_printf_format_specifier_t;

#define IO_PRINTF_FLAG_PLUS (1)
#define IO_PRINTF_FLAG_MINUS (2)
#define IO_PRINTF_FLAG_SPACE (4)
#define IO_PRINTF_FLAG_SHARP (8)
#define IO_PRINTF_FLAG_ZERO (16)
#define IO_PRINTF_FLAG_PRECISION_SPECIFIED (32)
#define IO_PRINTF_FLAG_PARSE_ERROR (INT8_MIN)

#define IO_PRINTF_LENGTH_none (0)
#define IO_PRINTF_LENGTH_hh (1)
#define IO_PRINTF_LENGTH_h (2)
#define IO_PRINTF_LENGTH_l (3)
#define IO_PRINTF_LENGTH_ll (4)
// j, z, t and L are just fucked up

io_printf_format_specifier_t io_parse_format_specifier(const char **format_ptr, va_list vlist);

int io_vprintf(const char *format, va_list vlist);
int io_vdprintf(const ocdev_t ocdev, const char *format, va_list vlist);
int io_printf(const char *format, ...);
int io_dprintf(const ocdev_t ocdev, const char *format, ...);

#endif // BEAVER_IO_PRINTF_H
