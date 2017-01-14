#include <math.h>

inline int min(int a, int b) {
    return a < b ? a : b;
}

inline int max(int a, int b) {
    return a < b ? b : a;
}

inline unsigned int umin(unsigned int a, unsigned int b) {
    return a < b ? a : b;
}

inline unsigned int umax(unsigned int a, unsigned int b) {
    return a < b ? b : a;
}

inline uint64_t min_ull(uint64_t a, uint64_t b) {
    return a < b ? a : b;
}

inline uint64_t max_ull(uint64_t a, uint64_t b) {
    return a < b ? b : a;
}
