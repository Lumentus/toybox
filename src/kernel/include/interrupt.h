#ifndef INTERRUPT_H
#define INTERRUPT_H


#include <stdint.h>

#define INT_DIVIDE_ERROR            0
#define INT_DEBUG_EXCEPTIONS        1
#define INT_NONMASKABLE_INT         2
#define INT_BREAKPOINT              3
#define INT_OVERFLOW                4
#define INT_BOUNDS_CHECK            5
#define INT_INVALID_OPCODE          6
#define INT_COPROCESSOR_NOT_AVL     7
#define INT_DOUBLE_FAULT            8
#define INT_COPROC_SEG_OVERRUN      9
#define INT_INVALID_TSS             10
#define INT_SEGMENT_NOT_PRESENT     11
#define INT_STACK_EXCEPTION         12
#define INT_GENERAL_PROTECTION      13
#define INT_PAGE_FAULT              14
#define INT_COPROCESSOR_ERROR       16

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef struct cpu_state
{
    uint32_t ds;        // data segment selector
    uint32_t edi;       // pushed by pusha
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t int_no;    // interrupt number
    uint32_t error;     // error code

    uint32_t eip;       // pushed by the processor
    uint32_t cs;
    uint32_t eflags;
    uint32_t useresp;
    uint32_t ss;
} cpu_state_t;

typedef void (*interrupt_t)(cpu_state_t);

void init_interrupt_handler();
void register_interrupt_handler(uint8_t int_no, interrupt_t handler);


#endif // INTERRUPT_H
