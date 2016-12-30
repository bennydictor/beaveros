#ifndef BEAVER_IO_PRINTF_SUBROUTINES_H
#define BEAVER_IO_PRINTF_SUBROUTINES_H

#include <io/printf.h>
#include <stdbool.h>

bool io_printf_subroutine_d(const ocdev_t ocdev, const io_printf_format_specifier_t spec, va_list vlist);

#endif // BEAVER_IO_PRINTF_SUBROUTINES_H
