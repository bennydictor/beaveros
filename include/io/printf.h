#ifndef BEAVER_IO_PRINTF_H
#define BEAVER_IO_PRINTF_H

#include <stdarg.h>
#include <stdint.h>
#include <io/ocdev.h>

int vprintf(const char *format, va_list vlist);
int vdprintf(const ocdev_t ocdev, const char *format, va_list vlist);
int printf(const char *format, ...);
int dprintf(const ocdev_t ocdev, const char *format, ...);

#endif // BEAVER_IO_PRINTF_H
