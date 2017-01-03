#ifndef BEAVER_IO_PRINTF_H
#define BEAVER_IO_PRINTF_H

#include <stdarg.h>
#include <stdint.h>
#include <io/ocdev.h>

int io_vprintf(const char *format, va_list vlist);
int io_vdprintf(const ocdev_t ocdev, const char *format, va_list vlist);
int io_printf(const char *format, ...);
int io_dprintf(const ocdev_t ocdev, const char *format, ...);

#endif // BEAVER_IO_PRINTF_H
