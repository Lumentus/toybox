global loader                           ; entry point for linker

extern kmain                            ; kmain, defined in kernel.c

; initial kernel stack space
STACKSIZE equ 0x4000                    ; 16kB

; setting up the Multiboot header
MODULEALIGN equ  1<<0                   ; align loaded modules on page boundaries
MEMINFO     equ  1<<1                   ; provide memory map
FLAGS       equ  MODULEALIGN | MEMINFO  ; multiboot flag field
MAGIC       equ  0x1BADB002             ; magic number, lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)        ; checksum

; needed for boot paging
KERNEL_OFFSET equ 0xC0000000
KERNEL_PD_IDX equ (KERNEL_OFFSET >> 22)


section .text
loader:
    mov esp, stack                      ; set up the stack
    sub esp, KERNEL_OFFSET              ; get the physical address

    call init_boot_paging

    add esp, KERNEL_OFFSET              ; paging enabled, use the virtual address
    mov ebp, esp

    push ebx                            ; multiboot info structure
    push eax                            ; magic number

    call kmain                          ; call kernel main

    cli                                 ; disable interrupts
    hlt                                 ; halt machine
    jmp $                               ; loop

init_boot_paging:
    mov ecx, (BootPageDirectory - KERNEL_OFFSET)
    mov cr3, ecx                        ; load page directory at physical address

    mov ecx, cr4
    or ecx, 0x00000010                  ; enable 4MB pages
    mov cr4, ecx

    mov ecx, cr0
    or ecx, 0x80000000                  ; enable paging
    mov cr0, ecx

    ret


section .multiboot                      ; multiboot-header must be in first 8kB
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM


section .data
align 0x1000
BootPageDirectory:
    dd 0x00000083                       ; map first 4MB
    times (KERNEL_PD_IDX - 1) dd 0      ; empty pages before the kernel
    dd 0x00000083                       ; map 4MB for the kernel
    times (0x1000 - KERNEL_PD_IDX - 1) dd 0 ; empty pages after the kernel


section .bss
align 16
    resb STACKSIZE                      ; reserve stack
stack:
