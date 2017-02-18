#ifndef BEAVER_DEBUG_H
#define BEAVER_DEBUG_H

#include <terminate.h>

#warning using debug.h

#define DEBUG_BREAKPOINT ({ \
    printf("hOI!!!!!! I'm bREAKPOINT!!\n"); \
    terminate(); \
})

#define DEBUG_BREAKPOINT_N(N) ({ \
    printf("hOI!!!!!! I'm bREAKPOINT #%d!!\n", (N)); \
    terminate(); \
})

#endif /* BEAVER_DEBUG_H */
