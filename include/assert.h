#ifndef BEAVER_ASSERT_H
#define BEAVER_ASSERT_H

#include <panic.h>

#define ASSERT(EXPR) do { \
    if (!(EXPR)) { \
        PANIC("Assertion failed: (" #EXPR ")"); \
    } \
} while (0)

#endif // BEAVER_ASSERT_H
