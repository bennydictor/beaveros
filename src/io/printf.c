#include <io/printf.h>
#include <stdarg.h>
#include <stdbool.h>
#include <io/stddev.h>

uint16_t io_internal_atoi(const char **str) {
    uint16_t ans = 0;
    while (**str >= '0' && **str <= '9') {
        ans *= 10;
        ans += **str - '0';
        ++*str;
    }
    return ans;
}

// Format specifier structure: %[flags][width][.precision][length]specifier
io_printf_format_specifier_t io_parse_format_specifier(const char *format, va_list vlist) {
    io_printf_format_specifier_t ans;
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
        ans.width = va_arg(vlist, int);
        ++format;
    } else {
        ans.width = io_internal_atoi(&format);
    }

    // Precision
    ans.precision = 0;
    if (*format == '.') {
        ++format;
        if (*format == '*') {
            ans.precision = va_arg(vlist, int);
            ++format;
        } else {
            ans.precision = io_internal_atoi(&format);
        }
    }

    // Length
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
    
    if 
}

int io_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ans = io_vdprintf(io_get_std_ocdev(), format, args);
    va_end(args);
    return ans;
}

int io_dprintf(const ocdev_t ocdev, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ans = io_vdprintf(ocdev, format, args);
    va_end(args);
    return ans;
}

int io_vprintf(const char *format, va_list vlist) {
    return io_vdprintf(io_get_std_ocdev(), format, vlist);
}

int io_vdprintf(const ocdev_t ocdev, const char *format, va_list vlist) {
    return 0;
}
