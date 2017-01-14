#ifndef BEAVER_IO_PORTIO_H
#define BEAVER_IO_PORTIO_H

#include <stdint.h>

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

#endif /* BEAVER_IO_PORTIO_H */
