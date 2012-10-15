#ifndef PORTS_H
#define PORTS_H


#include <stdint.h>

// Sends a byte on a I/O location.
static inline void outb(uint16_t port, uint8_t data)
{
    asm volatile("outb %1, %0" : : "dN" (port), "a" (data));
}

// Sends a word on a I/O location.
static inline void outw(uint16_t port, uint16_t data)
{
    asm volatile("outw %1, %0" : : "dN" (port), "a" (data));
}

// Sends a dword on a I/O location.
static inline void outl(uint16_t port, uint32_t data)
{
    asm volatile("outl %1, %0" : : "dN" (port), "a" (data));
}

// Receives a byte from an I/O location.
static inline uint8_t inb(uint16_t port)
{
    uint8_t data;
    asm volatile("inb %1, %0" : "=a" (data) : "dN" (port));
    return data;
}

// Receives a word from an I/O location.
static inline uint16_t inw(uint16_t port)
{
   uint16_t data;
   asm volatile("inw %1, %0" : "=a" (data) : "dN" (port));
   return data;
}

// Receives a dword from an I/O location.
static inline uint32_t inl(uint16_t port)
{
    uint32_t data;
    asm volatile("inl %1, %0" : "=a" (data) : "dN" (port));
    return data;
}

// Wait for a moment.
static inline void io_wait()
{
    asm volatile("jmp 1f\n\t"
                 "1:jmp 2f\n\t"
                 "2:");
}

// CPU speed independent, forces the CPU to wait for an I/O operation.
static inline void io_wait_cycle()
{
    // port 0x80 is used for 'checkpoints' during POST
    asm volatile("outb %%al, $0x80" : : "a" (0));
}


#endif // PORTS_H
