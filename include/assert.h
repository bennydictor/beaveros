#ifndef BEAVER_ASSERT_H
#define BEAVER_ASSERT_H

#include <panic.h>

#define ASSERT(expr) if (! (expr) ) { PANIC("Assertion failed: (" #expr ")"); }

#endif // BEAVER_ASSERT_H
