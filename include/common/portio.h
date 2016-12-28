#ifndef __BEAVER_COMMON_PORTIO_H__
#define __BEAVER_COMMON_PORTIO_H__

#include <common/types.h>

void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
void outl(uint16_t port, uint32_t val);

uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);

#endif // __BEAVER_COMMON_PORTIO_H__
