#ifndef BEAVER_COMMON_PANIC_H
#define BEAVER_COMMON_PANIC_H

#include <stdint.h>

#define PANIC(msg) panic(( msg ), __FILE__, __LINE__);
void panic(const char *msg, const char *filename, uint32_t line);
extern void terminate(void);

#endif // BEAVER_COMMON_PANIC_H

