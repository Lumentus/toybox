#include "timer.h"
#include <stdint.h>
#include "ports.h"
#include "interrupt.h"
#include "console.h"

static uint32_t tick = 0;

static void timer_callback(cpu_state_t cpu)
{
    tick++;
    kprintf("Timer (0x%x): %u\n", cpu.int_no, tick);
}

void timer_init(uint32_t frequency)
{
    uint32_t divisor = 1193182 / frequency;

    register_interrupt_handler(IRQ0, timer_callback);

    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}
