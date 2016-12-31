#ifndef BEAVER_IO_OUTPUT_CHARACTER_DEVICE_H
#define BEAVER_IO_OUTPUT_CHARACTER_DEVICE_H

#include <stdint.h>

typedef struct {
    void (* putc)(char);
    void (* puts)(const char *);
    void (* putsl)(const char *, uint32_t);
} ocdev_t;

#endif // BEAVER_IO_OUTPUT_CHARACTER_DEVICE_H
