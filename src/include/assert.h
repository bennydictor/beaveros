#ifndef BEAVER_ASSERT_H
#define BEAVER_ASSERT_H

#include <panic.h>

#define ASSERT(EXPR) ({ \
    if (!(EXPR)) { \
        PANIC("Assertion failed: (" #EXPR ")"); \
    } \
})

#endif /* BEAVER_ASSERT_H */
