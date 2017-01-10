#ifndef BEAVER_IO_PRINTF_H
#define BEAVER_IO_PRINTF_H

#include <stdarg.h>
#include <stdint.h>
#include <io/ocdev.h>

int vprintf(const char *, va_list);
int vdprintf(const ocdev_t, const char *, va_list);
int printf(const char *, ...);
int dprintf(const ocdev_t, const char *, ...);

#endif // BEAVER_IO_PRINTF_H
