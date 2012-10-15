LOAD_ADDR = 0x00100000;
LINK_ADDR = 0x00100000;
OFFSET = LINK_ADDR - LOAD_ADDR;

ENTRY(loader)

SECTIONS
{
    . = LINK_ADDR;

    kernel_start = . - OFFSET;
    kernel_v_start = .;

    .text ALIGN(0x1000) : AT(ADDR(.text) - OFFSET)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - OFFSET)
    {
        *(.rodata)
    }

    .data ALIGN(0x1000) : AT(ADDR(.data) - OFFSET)
    {
        *(.data)
    }

    .bss ALIGN(0x1000) : AT(ADDR(.bss) - OFFSET)
    {
        _sbss = .;
        *(COMMON)
        *(.bss)
        _ebss = .;
    }

    . = ALIGN(0x1000);

    kernel_end = . - OFFSET;
    kernel_v_end = .;

    kernel_size = kernel_end - kernel_start;
}
