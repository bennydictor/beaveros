#include <io/portio.h>

void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

void outw(uint16_t port, uint16_t val) {
    __asm__ __volatile__ ( "outw %0, %1" : : "a"(val), "Nd"(port) );
}

void outl(uint16_t port, uint32_t val) {
    __asm__ __volatile__ ( "outl %0, %1" : : "a"(val), "Nd"(port) );
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ __volatile__ ( "inw %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

uint32_t inl(uint16_t port) {
    uint32_t ret;
    __asm__ __volatile__ ( "inl %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}
