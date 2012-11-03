#include "kernel.h"
#include <stdint.h>
#include "multiboot.h"
#include "console.h"
#include "pmm.h"
#include "paging.h"
#include "gdt.h"
#include "interrupt.h"
#include "idt.h"

// TODO: list
// - reserve first 4MB?
// - paging
// - virtual memory manager
// - panic function, add special format functionality
// - improve Makefile
// - seperate section for ro kernel data
// - multiboot header for asm files

void kmain(uint32_t magic, multiboot_info_t *mb_info)
{
    kclear();

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        kprintf("Invalid magic number: 0x%x\n", magic);
        return;
    }

    kprintf("Welcome to ToyBox!\n\n");

    kprintf("kernel size    : %ukB\n", (uint32_t)&kernel_size / 1024);
    kprintf("kernel vstart  : 0x%x\n", (uint32_t)&kernel_v_start);
    kprintf("kernel vend    : 0x%x\n", (uint32_t)&kernel_v_end);
    kprintf("kernel offset  : 0x%x\n\n", (uint32_t)&kernel_offset);

    pmm_init(mb_info);
    paging_init(mb_info);
    gdt_init();
    init_interrupt_handler();
    idt_init();
    paging_register_interrupt();

    kprintf("\n");
    // test alloc

    uint32_t *ptr = (uint32_t *)0x00300000;
    uint32_t fault = *ptr;
    fault++;

    kprintf("\nend");

    while (1);
}

__attribute__((noreturn)) void panic(char *file, int line, char *msg)
{
    kprintf("\n\n");

    kprintf("file: %s\nline: %u\nmsg: %s\n\n", file, line, msg);

    while (1);
}



















