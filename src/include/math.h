#ifndef BEAVER_MATH_H
#define BEAVER_MATH_H

#include <stdint.h>

#define GEN(TYPE, PREFIX) \
__attribute__ ((always_inline)) \
static inline TYPE PREFIX##min(TYPE a, TYPE b) { \
    return a < b ? a : b; \
} \
\
static inline TYPE PREFIX##max(TYPE a, TYPE b) { \
    return a > b ? a : b; \
}

GEN(int,)
GEN(unsigned int, u)
GEN(long int, l)
GEN(unsigned long int, ul)
GEN(long long int, ll)
GEN(unsigned long long int, ull)
GEN(intmax_t, imax)

#undef GEN

#endif /* BEAVER_MATH_H */
