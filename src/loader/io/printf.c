#include <io/printf.h>
#include <io/ocdev.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <math.h>

typedef struct {
    uint8_t flags;
    char specifier;
    uint16_t width, precision;
    uint16_t length;
} printf_format_specifier_t;

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
// No, they are not. Please, implement them.
// TODO j, z, t, L

static uint16_t atoi(const char **str) {
    uint16_t ans = 0;
    while (**str >= '0' && **str <= '9') {
        ans *= 10;
        ans += **str - '0';
        ++*str;
    }
    return ans;
}

// Format specifier structure: %[flags][width][.precision][length]specifier
static printf_format_specifier_t parse_format_specifier(const char **format_ptr,
                                                                va_list *vlist_ptr) {
    const char *format = *format_ptr;
    printf_format_specifier_t ans;
    ans.flags = 0;

    // Flags
    bool are_there_flags = true;

    while (are_there_flags) {
        ++format;
        switch (*format) {
            case '-':
                ans.flags |= IO_PRINTF_FLAG_MINUS;
                break;
            case '+':
                ans.flags |= IO_PRINTF_FLAG_PLUS;
                break;
            case ' ':
                ans.flags |= IO_PRINTF_FLAG_SPACE;
                break;
            case '#':
                ans.flags |= IO_PRINTF_FLAG_SHARP;
                break;
            case '0':
                ans.flags |= IO_PRINTF_FLAG_ZERO;
                break;
            default:
                are_there_flags = false;
                break;
        }
    }

    // Width
    ans.width = 0;
    if (*format == '*') {
        ans.width = va_arg(*vlist_ptr, int);
        ++format;
    } else {
        ans.width = atoi(&format);
    }

    // Precision
    ans.precision = 0;
    if (*format == '.') {
        ++format;
        ans.flags |= IO_PRINTF_FLAG_PRECISION_SPECIFIED;
        if (*format == '*') {
            ans.precision = va_arg(*vlist_ptr, int);
            ++format;
        } else {
            ans.precision = atoi(&format);
        }
    }

    // Length
    ans.length = IO_PRINTF_LENGTH_none;
    if (*format == 'h') {
        ++format;
        ans.length = IO_PRINTF_LENGTH_h;
        if (*format == 'h') {
            ++format;
            ans.length = IO_PRINTF_LENGTH_hh;
        }
    } else if (*format == 'l') {
        ++format;
        ans.length = IO_PRINTF_LENGTH_l;
        if (*format == 'l') {
            ++format;
            ans.length = IO_PRINTF_LENGTH_ll;
        }
    }

    if (strchr("diuoxXfFeEgGaAcspn%", *format) != NULL) {
        ans.specifier = *format++;
    } else {
        ans.specifier = '!';
        ans.flags |= IO_PRINTF_FLAG_PARSE_ERROR;
    }

    *format_ptr = format;
    return ans;
}

#define INIT_DIGITS(UPPERCASE) \
    const char *DIGITS = UPPERCASE ? "0123456789ABCDEF" : "0123456789abcdef";

#define BUF_SIZE 25

#define PRINTER_HEADER(SUFFIX, TYPE) \
bool print_##SUFFIX(ocdev_t ocdev, printf_format_specifier_t spec, TYPE d)

#define SIGN_CHECKER \
    if (d < 0) { \
        sign_c = '-'; \
        d = -d; \
    } else

#define INTEGER_PRINTER(SUFFIX, TYPE, BASE, PREFIX, UPPERCASE, SIGN_CHECK) \
PRINTER_HEADER(SUFFIX, TYPE) { \
    INIT_DIGITS(UPPERCASE); \
    char buf[BUF_SIZE]; \
    buf[BUF_SIZE - 1] = 0; \
    char sign_c = 0; \
    SIGN_CHECK if (spec.flags & IO_PRINTF_FLAG_PLUS) { \
        sign_c = '+'; \
    } else if (spec.flags & IO_PRINTF_FLAG_SPACE) { \
        sign_c = ' '; \
    } \
    char *buf_writer = buf + BUF_SIZE - 1; \
    int prefix_len = (spec.flags & IO_PRINTF_FLAG_SHARP) ? strlen(PREFIX) : 0; /* I want to print 0x0 instead of 0 */ \
    while (d > 0) { \
        *(--buf_writer) = DIGITS[d % (BASE)]; \
        d /= (BASE); \
    } \
    if (!(spec.flags & IO_PRINTF_FLAG_PRECISION_SPECIFIED)) { \
        spec.precision = 1; \
    } \
    int digits = (buf + BUF_SIZE - 1) - buf_writer; \
    int non_space_characters = (sign_c ? 1 : 0) + max(spec.precision, digits) + prefix_len; \
    char width_padder = (spec.flags & IO_PRINTF_FLAG_ZERO) ? '0' : ' '; \
    if (!(spec.flags & IO_PRINTF_FLAG_MINUS)) {\
        for (int i = non_space_characters; i < spec.width; ++i) { \
            ocdev.putc(width_padder); \
        } \
    } \
    if (sign_c) { \
        ocdev.putc(sign_c); \
    } \
    ocdev.putsl(PREFIX, prefix_len); \
    for (int i = digits; i < spec.precision; ++i) { \
        ocdev.putc('0'); \
    } \
    ocdev.puts(buf_writer); \
    if (spec.flags & IO_PRINTF_FLAG_MINUS) {\
        for (int i = non_space_characters; i < spec.width; ++i) { \
            ocdev.putc(' ' /*width_padder*/); \
        } \
    } \
    return true; \
}

#define GEN_PRINTERS(COMMON_SUFFIX, BASE, SIGN, PREFIX, UPPERCASE, SIGN_CHECK) \
INTEGER_PRINTER(COMMON_SUFFIX##_int, SIGN int, BASE, PREFIX, UPPERCASE, SIGN_CHECK); \
INTEGER_PRINTER(COMMON_SUFFIX##_char, SIGN char, BASE, PREFIX, UPPERCASE, SIGN_CHECK); \
INTEGER_PRINTER(COMMON_SUFFIX##_s_int, SIGN short int, BASE, PREFIX, UPPERCASE, SIGN_CHECK); \
INTEGER_PRINTER(COMMON_SUFFIX##_l_int, SIGN long int, BASE, PREFIX, UPPERCASE, SIGN_CHECK); \
INTEGER_PRINTER(COMMON_SUFFIX##_ll_int, SIGN long long int, BASE, PREFIX, UPPERCASE, SIGN_CHECK);

GEN_PRINTERS(s, 10, signed, "", false, SIGN_CHECKER);
GEN_PRINTERS(u, 10, unsigned, "", false, );
GEN_PRINTERS(o, 8, unsigned, "0", false, );
GEN_PRINTERS(h, 16, unsigned, "0x", false, );
GEN_PRINTERS(H, 16, unsigned, "0X", true, );

#define SUBROUTINE_HEADER(SUFFIX) \
bool printf_subroutine_##SUFFIX(ocdev_t ocdev, \
                                    printf_format_specifier_t spec, \
                                    va_list *vlist_ptr)

#define GEN_SUBROUTINE(SUBROUTINE_SUFFIX, PRINTER_TYPE) \
SUBROUTINE_HEADER(SUBROUTINE_SUFFIX) { \
    switch (spec.length) { \
        case IO_PRINTF_LENGTH_none: \
            return print_##PRINTER_TYPE##_int(ocdev, spec, va_arg(*vlist_ptr, int)); \
        case IO_PRINTF_LENGTH_hh: \
            return print_##PRINTER_TYPE##_char(ocdev, spec, va_arg(*vlist_ptr, int)); \
        case IO_PRINTF_LENGTH_h: \
            return print_##PRINTER_TYPE##_s_int(ocdev, spec, va_arg(*vlist_ptr, int)); \
        case IO_PRINTF_LENGTH_l: \
            return print_##PRINTER_TYPE##_l_int(ocdev, \
                                                            spec, \
                                                            va_arg(*vlist_ptr, long int)); \
        case IO_PRINTF_LENGTH_ll: \
            return print_##PRINTER_TYPE##_ll_int(ocdev, \
                                                                spec, \
                                                                va_arg(*vlist_ptr, long long int)); \
    } \
    return false; \
} \

GEN_SUBROUTINE(d, s);
SUBROUTINE_HEADER(i) __attribute__ ((alias("printf_subroutine_d")));
GEN_SUBROUTINE(u, u);
GEN_SUBROUTINE(o, o);
GEN_SUBROUTINE(x, h);
GEN_SUBROUTINE(X, H);

PRINTER_HEADER(string, char *) {
    uint32_t len = strlen(d);
    uint32_t chars = (spec.flags & IO_PRINTF_FLAG_PRECISION_SPECIFIED) ? umin(len, spec.precision) : len;
    if (!(spec.flags & IO_PRINTF_FLAG_MINUS)) {
        for (uint32_t i = chars; i < spec.width; ++i) {
            ocdev.putc(' ');
        }
    }
    ocdev.putsl(d, chars);
    if (spec.flags & IO_PRINTF_FLAG_MINUS) {
        for (uint32_t i = chars; i < spec.width; ++i) {
            ocdev.putc(' ');
        }
    }
    return true;
}

SUBROUTINE_HEADER(s) {
    if (spec.length == IO_PRINTF_LENGTH_none) {
        return print_string(ocdev, spec, va_arg(*vlist_ptr, char *));
    }
    return false;
}

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ans = vdprintf(get_std_ocdev(), format, args);
    va_end(args);
    return ans;
}

int dprintf(const ocdev_t ocdev, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ans = vdprintf(ocdev, format, args);
    va_end(args);
    return ans;
}

int vprintf(const char *format, va_list vlist) {
    return vdprintf(get_std_ocdev(), format, vlist);
}

#define CASE_SUBROUTINE(symbol, suffix) \
    case symbol : \
        printf_subroutine_##suffix (ocdev, spec, &vlist); \
        break;

int vdprintf(const ocdev_t ocdev, const char *format, va_list vlist) {
    while (*format) {
        if (*format == '%') {
            printf_format_specifier_t spec = parse_format_specifier(&format, &vlist);
            switch (spec.specifier) {
                CASE_SUBROUTINE('d', d);
                CASE_SUBROUTINE('i', i);
                CASE_SUBROUTINE('u', u);
                CASE_SUBROUTINE('o', o);
                CASE_SUBROUTINE('x', x);
                CASE_SUBROUTINE('X', X);
                CASE_SUBROUTINE('s', s);
                case 'c':
                    ocdev.putc(va_arg(vlist, int));
                    break;
                case '%': // The simpliest subroutine
                    ocdev.putc('%');
                    break;
                default:
                    ocdev.puts("<Not-Implemented-Yet>");
                    break;
            }
        } else {
            ocdev.putc(*format++);
        }
    }
    return 0;
}
