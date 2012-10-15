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


section .multiboot                      ; multiboot-header must be in first 8kB
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM


section .text
loader:
    mov esp, stack + STACKSIZE          ; set up the stack
    push ebx                            ; multiboot info structure
    push eax                            ; magic number

    call kmain                          ; call kernel main

    cli                                 ; disable interrupts
    hlt                                 ; halt machine
    jmp $                               ; loop


section .bss
align 16
    stack: resb STACKSIZE               ; reserve stack
