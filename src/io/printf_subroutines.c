#include <io/printf_subroutines.h>
#include <common/math.h>

#define INIT_PRINTF_SUBROUTINE \
    bool left_justify = spec.flags & IO_PRINTF_FLAG_MINUS; \
    bool forced_sign = spec.flags & IO_PRINTF_FLAG_PLUS; \
    bool space_for_sign = spec.flags & IO_PRINTF_FLAG_SPACE; \
/*    bool use_prefix = spec.flags & IO_PRINTF_FLAG_SHARP; */ \
    bool pad_with_zeroes = spec.flags & IO_PRINTF_FLAG_ZERO; \
    bool precision_specified = spec.flags & IO_PRINTF_FLAG_PRECISION_SPECIFIED;

#define BUF_SIZE 25

#define DECIMAL_PRINTER(suffix, type) \
bool io_internal_print_##suffix (ocdev_t ocdev, io_printf_format_specifier_t spec, type d) { \
    INIT_PRINTF_SUBROUTINE \
    char buf[BUF_SIZE]; \
    buf[BUF_SIZE - 1] = 0; \
    char sign_c = 0; \
    if (d < 0) { \
        sign_c = '-'; \
        d = -d; \
    } else if (forced_sign) { \
        sign_c = '+'; \
    } else if (space_for_sign) { \
        sign_c = ' '; \
    } \
    char *buf_writer = buf + BUF_SIZE - 1; \
    while (d > 0) { \
        *(--buf_writer) = (d % 10) + '0'; \
        d /= 10; \
    } \
    if (!precision_specified) { \
        spec.precision = 1; \
    } \
    int digits = (buf + BUF_SIZE - 1) - buf_writer; \
    int non_space_characters = (sign_c ? 1 : 0) + max(spec.precision, digits); \
    char width_padder = pad_with_zeroes ? '0' : ' '; \
    if (!left_justify) {\
        for (int i = non_space_characters; i < spec.width; ++i) { \
            ocdev.putc(width_padder); \
        } \
    } \
    if (sign_c) { \
        ocdev.putc(sign_c); \
    } \
    for (int i = digits; i < spec.precision; ++i) { \
        ocdev.putc('0'); \
    } \
    ocdev.puts(buf_writer); \
    if (left_justify) {\
        for (int i = non_space_characters; i < spec.width; ++i) { \
            ocdev.putc(width_padder); \
        } \
    } \
    return true; \
}

DECIMAL_PRINTER(int, int);
DECIMAL_PRINTER(s_char, signed char);
DECIMAL_PRINTER(s_int, short int);
DECIMAL_PRINTER(l_int, long int);
DECIMAL_PRINTER(ll_int, long long int);

DECIMAL_PRINTER(u_int, unsigned int);
DECIMAL_PRINTER(char, char);
DECIMAL_PRINTER(us_int, unsigned short int);
DECIMAL_PRINTER(ul_int, unsigned long int);
DECIMAL_PRINTER(ull_int, unsigned long long int);

bool io_printf_subroutine_d(ocdev_t ocdev, io_printf_format_specifier_t spec, va_list vlist) {
    switch (spec.length) {
        case IO_PRINTF_LENGTH_none:
            return io_internal_print_int(ocdev, spec, va_arg(vlist, int));
        case IO_PRINTF_LENGTH_hh:
            return io_internal_print_s_char(ocdev, spec, va_arg(vlist, int));
        case IO_PRINTF_LENGTH_h:
            return io_internal_print_s_int(ocdev, spec, va_arg(vlist, int));
        case IO_PRINTF_LENGTH_l:
            return io_internal_print_l_int(ocdev, spec, va_arg(vlist, long int));
        case IO_PRINTF_LENGTH_ll:
            return io_internal_print_ll_int(ocdev, spec, va_arg(vlist, long long int));
    }
    return false;
}

bool io_printf_subroutine_u(ocdev_t ocdev, io_printf_format_specifier_t spec, va_list vlist) {
    switch (spec.length) {
        case IO_PRINTF_LENGTH_none:
            return io_internal_print_u_int(ocdev, spec, va_arg(vlist, unsigned int));
        case IO_PRINTF_LENGTH_hh:
            return io_internal_print_char(ocdev, spec, va_arg(vlist, unsigned int));
        case IO_PRINTF_LENGTH_h:
            return io_internal_print_us_int(ocdev, spec, va_arg(vlist, unsigned int));
        case IO_PRINTF_LENGTH_l:
            return io_internal_print_ul_int(ocdev, spec, va_arg(vlist, unsigned long int));
        case IO_PRINTF_LENGTH_ll:
            return io_internal_print_ull_int(ocdev, spec, va_arg(vlist, unsigned long long int));
    }
    return false;
}
