#ifndef BEAVER_IO_PRINTF_H
#define BEAVER_IO_PRINTF_H

#include <stdarg.h>
#include <io/ocdev.h>

int vprintf(const char *, va_list);
int vdprintf(const ocdev_t, const char *, va_list);
int printf(const char *, ...) __attribute__ ((format (printf, 1, 2)));
int dprintf(const ocdev_t, const char *, ...)
        __attribute__ ((format (printf, 2, 3)));

#endif /* BEAVER_IO_PRINTF_H */
