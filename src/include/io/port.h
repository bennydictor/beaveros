#ifndef BEAVER_IO_PORT_H
#define BEAVER_IO_PORT_H

#include <stdint.h>

#define PORT_PIC8259_MASTER_COMMAND 0x20
#define PORT_PIC8259_MASTER_DATA    0x21
#define PORT_PIC8259_SLAVE_COMMAND  0xA0
#define PORT_PIC8259_SLAVE_DATA     0xA1

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

#endif /* BEAVER_IO_PORT_H */
