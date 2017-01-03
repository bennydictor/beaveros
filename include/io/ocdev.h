#ifndef BEAVER_IO_OCDEV_H
#define BEAVER_IO_OCDEV_H

#include <stdint.h>

typedef struct ocdev {
    void (*putc)(char);
    void (*puts)(const char *);
    void (*putsl)(const char *, uint32_t);
} ocdev_t;

void set_std_ocdev(const ocdev_t);
ocdev_t get_std_ocdev(void);

#endif // BEAVER_IO_OCDEV_H
