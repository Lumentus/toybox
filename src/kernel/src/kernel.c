#include "kernel.h"
#include <stdint.h>
#include "multiboot.h"
#include "console.h"
#include "pmm.h"
#include "paging.h"

// TODO: list
// - paging
// - virtual memory manager
// - gdt
// - idt
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
    kprintf("kernel start   : 0x00%x\n", (uint32_t)&kernel_start);
    kprintf("kernel end     : 0x00%x\n", (uint32_t)&kernel_end);
    kprintf("kernel vstart  : 0x%x\n", (uint32_t)&kernel_v_start);
    kprintf("kernel vend    : 0x%x\n", (uint32_t)&kernel_v_end);
    kprintf("kernel offset  : 0x%x\n\n", (uint32_t)&kernel_offset);

    pmm_init(mb_info);

    paging_init();

    kprintf("\n");
    kprintf("alloc: 0x%x\n", alloc_frame(1));
    kprintf("alloc: 0x%x\n", alloc_frame(3));
    kprintf("alloc: 0x%x\n", alloc_frame(1));
    kprintf("alloc: 0x%x\n", alloc_frame(140));
    kprintf("alloc: 0x%x\n", alloc_frame(1));

//    uint32_t *ptr = (uint32_t *)0xFFFFFFFF;
//    uint32_t fault = *ptr;
//    fault++;

    while (1);
}

__attribute__((noreturn)) void panic(char *file, int line, char *msg)
{
    kprintf("\n\n");

    kprintf("file: %s\nline: %u\nmsg: %s\n\n", file, line, msg);

    while (1);
}



















