#include "interrupt.h"
#include <stdint.h>
#include "ports.h"

static interrupt_t interrupt_handlers[256];

void interrupt_handler(cpu_state_t cpu)
{
    // cpu interrupts
    if (cpu.int_no <= 0x1F)
    {
    }
    // hardware interrupts
    else if (cpu.int_no >= 0x20 && cpu.int_no <= 0x2F)
    {
        if (cpu.int_no >= 0x28)
        {
            outb(0xA0, 0x20);
        }

        outb(0x20, 0x20);
    }

    // registered interrupt handlers
    if (interrupt_handlers[cpu.int_no] != 0)
    {
        interrupt_handlers[cpu.int_no](cpu);
    }
}

void register_interrupt_handler(uint8_t int_no, interrupt_t handler)
{
    interrupt_handlers[int_no] = handler;
}

void init_interrupt_handler()
{
    uint32_t i;
    for (i = 1; i < 256; i++)
    {
        interrupt_handlers[i] = 0;
    }
}
