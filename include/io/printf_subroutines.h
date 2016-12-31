#ifndef BEAVER_IO_PRINTF_SUBROUTINES_H
#define BEAVER_IO_PRINTF_SUBROUTINES_H

#include <io/printf.h>
#include <stdbool.h>

#define SUBROUTINE(suffix) bool io_printf_subroutine_##suffix (const ocdev_t ocdev, const io_printf_format_specifier_t spec, va_list *vlist_ptr)

SUBROUTINE(d);
SUBROUTINE(i);
SUBROUTINE(u);
SUBROUTINE(o);
SUBROUTINE(x);
SUBROUTINE(X);
SUBROUTINE(s);

#undef SUBROUTINE

#endif // BEAVER_IO_PRINTF_SUBROUTINES_H
