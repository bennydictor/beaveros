#ifndef __BEAVER_COMMON_PORTIO_H__
#define __BEAVER_COMMON_PORTIO_H__

#include <common/types.h>

inline void outb(uint16_t port, uint8_t val);
inline void outw(uint16_t port, uint16_t val);
inline void outl(uint16_t port, uint32_t val);

inline uint8_t inb(uint16_t port);
inline uint16_t inw(uint16_t port);
inline uint32_t inl(uint16_t port);

#endif // __BEAVER_COMMON_PORTIO_H__
