#ifndef BEAVER_PANIC_H
#define BEAVER_PANIC_H

#include <stdint.h>

#define PANIC(MSG) do { \
    panic((MSG), __FILE__, __LINE__); \
} while (0)

void panic(const char *, const char *, uint32_t);
void terminate(void);

#endif // BEAVER_PANIC_H

