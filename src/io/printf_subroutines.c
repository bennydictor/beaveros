#include <io/printf_subroutines.h>
#include <common/math.h>
#include <common/string.h>

#define INIT_PRINTF_SUBROUTINE(uppercase) \
    bool left_justify = spec.flags & IO_PRINTF_FLAG_MINUS; \
    bool forced_sign = spec.flags & IO_PRINTF_FLAG_PLUS; \
    bool space_for_sign = spec.flags & IO_PRINTF_FLAG_SPACE; \
    bool use_prefix = spec.flags & IO_PRINTF_FLAG_SHARP; \
    bool pad_with_zeroes = spec.flags & IO_PRINTF_FLAG_ZERO; \
    bool precision_specified = spec.flags & IO_PRINTF_FLAG_PRECISION_SPECIFIED; \
    const char *DIGITS = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

#define BUF_SIZE 25

#define INTEGER_PRINTER(suffix, type, base, prefix, uppercase) \
bool io_internal_print_##suffix (ocdev_t ocdev, io_printf_format_specifier_t spec, type d) { \
    INIT_PRINTF_SUBROUTINE(uppercase); \
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
    int prefix_len = (use_prefix && d) ? strlen(prefix) : 0; \
    while (d > 0) { \
        *(--buf_writer) = DIGITS[d % (base)]; \
        d /= (base); \
    } \
    if (!precision_specified) { \
        spec.precision = 1; \
    } \
    int digits = (buf + BUF_SIZE - 1) - buf_writer; \
    int non_space_characters = (sign_c ? 1 : 0) + max(spec.precision, digits) + prefix_len; \
    char width_padder = pad_with_zeroes ? '0' : ' '; \
    if (!left_justify) {\
        for (int i = non_space_characters; i < spec.width; ++i) { \
            ocdev.putc(width_padder); \
        } \
    } \
    if (sign_c) { \
        ocdev.putc(sign_c); \
    } \
    ocdev.putsl(prefix, prefix_len); \
    for (int i = digits; i < spec.precision; ++i) { \
        ocdev.putc('0'); \
    } \
    ocdev.puts(buf_writer); \
    if (left_justify) {\
        for (int i = non_space_characters; i < spec.width; ++i) { \
            ocdev.putc(' ' /*width_padder*/); \
        } \
    } \
    return true; \
}

#define GEN_PRINTERS(common_suffix, base, sign, prefix, uppercase) \
INTEGER_PRINTER( common_suffix##_int, sign int, base, prefix, uppercase); \
INTEGER_PRINTER( common_suffix##_char, sign char, base, prefix, uppercase); \
INTEGER_PRINTER( common_suffix##_s_int, sign short int, base, prefix, uppercase); \
INTEGER_PRINTER( common_suffix##_l_int, sign long int, base, prefix, uppercase); \
INTEGER_PRINTER( common_suffix##_ll_int, sign long long int, base, prefix, uppercase); 

GEN_PRINTERS(s, 10, signed, "", false);
GEN_PRINTERS(u, 10, unsigned, "", false);
GEN_PRINTERS(o, 8, unsigned, "0", false);
GEN_PRINTERS(h, 16, unsigned, "0x", false);
GEN_PRINTERS(H, 16, unsigned, "0X", true);


#define GEN_SUBROUTINE(subroutine_suffix, printer_type) \
bool io_printf_subroutine_##subroutine_suffix (ocdev_t ocdev, io_printf_format_specifier_t spec, va_list vlist) { \
    switch (spec.length) { \
        case IO_PRINTF_LENGTH_none: \
            return io_internal_print_##printer_type##_int(ocdev, spec, va_arg(vlist, int)); \
        case IO_PRINTF_LENGTH_hh: \
            return io_internal_print_##printer_type##_char(ocdev, spec, va_arg(vlist, int)); \
        case IO_PRINTF_LENGTH_h: \
            return io_internal_print_##printer_type##_s_int(ocdev, spec, va_arg(vlist, int)); \
        case IO_PRINTF_LENGTH_l: \
            return io_internal_print_##printer_type##_l_int(ocdev, spec, va_arg(vlist, long int)); \
        case IO_PRINTF_LENGTH_ll: \
            return io_internal_print_##printer_type##_ll_int(ocdev, spec, va_arg(vlist, long long int)); \
    } \
    return false; \
} \

GEN_SUBROUTINE(d, s);
bool io_printf_subroutine_i(ocdev_t, io_printf_format_specifier_t, va_list) __attribute__ ((alias("io_printf_subroutine_d")));
GEN_SUBROUTINE(u, u);
GEN_SUBROUTINE(o, o);
GEN_SUBROUTINE(x, h);
GEN_SUBROUTINE(X, H);
