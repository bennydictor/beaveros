#ifndef BEAVER_COMMON_ASSERT_H
#define BEAVER_COMMON_ASSERT_H

#include <common/panic.h>

#define ASSERT(expr) if (! (expr) ) { PANIC("Assertion failed: (" #expr ")"); }

#endif // BEAVER_COMMON_ASSERT_H
