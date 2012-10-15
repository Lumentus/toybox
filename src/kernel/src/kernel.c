#include "kernel.h"
#include <stdint.h>
#include "multiboot.h"
#include "console.h"

// TODO: list
// - higher half kernel
// - pysical memory manager
// - paging
// - virtual memory manager
// - gdt
// - idt
// - panic function
// - improve Makefile
// - document for C99 standard
// - compiler development? :D

void kmain(uint32_t magic, multiboot_info_t *mb_info)
{
    kclear();

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        kprintf("Invalid magic number: 0x%x\n", magic);
        return;
    }

    kprintf("Welcome to ToyBox!\n\n");

    kprintf("kernel size: %u kB\n", (uint32_t)&kernel_size / 1024);

    while (1);
}
