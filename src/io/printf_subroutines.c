#include <io/printf_subroutines.h>
#include <math.h>
#include <string.h>


#define INIT_PRINTF_SUBROUTINE \
    bool left_justify = spec.flags & IO_PRINTF_FLAG_MINUS; \
    bool forced_sign = spec.flags & IO_PRINTF_FLAG_PLUS; \
    bool space_for_sign = spec.flags & IO_PRINTF_FLAG_SPACE; \
    bool use_prefix = spec.flags & IO_PRINTF_FLAG_SHARP; \
    bool pad_with_zeroes = spec.flags & IO_PRINTF_FLAG_ZERO; \
    bool precision_specified = spec.flags & IO_PRINTF_FLAG_PRECISION_SPECIFIED;

#define INIT_DIGITS(uppercase) \
    const char *DIGITS = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

#define BUF_SIZE 25

#define PRINTER_HEADER(suffix, type) \
bool io_internal_print_##suffix (ocdev_t ocdev, io_printf_format_specifier_t spec, type d)

#define INTEGER_PRINTER(suffix, type, base, prefix, uppercase) \
PRINTER_HEADER(suffix, type) { \
    INIT_PRINTF_SUBROUTINE; \
    INIT_DIGITS(uppercase); \
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


#define SUBROUTINE_HEADER(suffix) \
bool io_printf_subroutine_##suffix (ocdev_t ocdev, io_printf_format_specifier_t spec, va_list *vlist_ptr)

#define GEN_SUBROUTINE(subroutine_suffix, printer_type) \
SUBROUTINE_HEADER(subroutine_suffix) { \
    switch (spec.length) { \
        case IO_PRINTF_LENGTH_none: \
            return io_internal_print_##printer_type##_int(ocdev, spec, va_arg(*vlist_ptr, int)); \
        case IO_PRINTF_LENGTH_hh: \
            return io_internal_print_##printer_type##_char(ocdev, spec, va_arg(*vlist_ptr, int)); \
        case IO_PRINTF_LENGTH_h: \
            return io_internal_print_##printer_type##_s_int(ocdev, spec, va_arg(*vlist_ptr, int)); \
        case IO_PRINTF_LENGTH_l: \
            return io_internal_print_##printer_type##_l_int(ocdev, spec, va_arg(*vlist_ptr, long int)); \
        case IO_PRINTF_LENGTH_ll: \
            return io_internal_print_##printer_type##_ll_int(ocdev, spec, va_arg(*vlist_ptr, long long int)); \
    } \
    return false; \
} \

GEN_SUBROUTINE(d, s);
SUBROUTINE_HEADER(i) __attribute__ ((alias("io_printf_subroutine_d")));
GEN_SUBROUTINE(u, u);
GEN_SUBROUTINE(o, o);
GEN_SUBROUTINE(x, h);
GEN_SUBROUTINE(X, H);

PRINTER_HEADER(string, char *) {
    INIT_PRINTF_SUBROUTINE;
    uint32_t len = strlen(d);
    uint32_t chars = precision_specified ? min(len, spec.precision) : len;
    if (!left_justify) {
        for (uint32_t i = chars; i < spec.width; ++i) {
            ocdev.putc(' ');
        }
    }
    ocdev.putsl(d, chars);
    if (left_justify) {
        for (uint32_t i = chars; i < spec.width; ++i) {
            ocdev.putc(' ');
        }
    }
    return true;
}

SUBROUTINE_HEADER(s) {
    if (spec.length == IO_PRINTF_LENGTH_none) {
        return io_internal_print_string(ocdev, spec, va_arg(*vlist_ptr, char *));
    }
    return false;
}
