#ifndef BEAVER_DEBUG_H
#define BEAVER_DEBUG_H

#define DEBUG_HLT ({ \
    printf("hOI!"); \
    terminate(); \
})

#define DEBUG_HLT_N(X) ({ \
    printf("hOI! #%d", (X)); \
    terminate(); \
})

#endif /* BEAVER_DEBUG_H */
