#ifndef BEAVER_IO_OUTPUT_CHARACTER_DEVICE_H
#define BEAVER_IO_OUTPUT_CHARACTER_DEVICE_H

typedef struct {
    void (* putc)(char);
    void (* puts)(const char *);
} ocdev_t;

#endif // BEAVER_IO_OUTPUT_CHARACTER_DEVICE_H
