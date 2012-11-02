extern interrupt_handler

%macro intr_stub 1
    global intr%1
    intr%1:
        cli
        push byte 0
        push byte %1
        jmp intr_common_handler
%endmacro

%macro intr_stub_error 1
    global intr%1
    intr%1:
        cli
        push byte %1
        jmp intr_common_handler
%endmacro

; Exceptions
intr_stub 0
intr_stub 1
intr_stub 2
intr_stub 3
intr_stub 4
intr_stub 5
intr_stub 6
intr_stub 7
intr_stub_error 8
intr_stub 9
intr_stub_error 10
intr_stub_error 11
intr_stub_error 12
intr_stub_error 13
intr_stub_error 14
; reserved
intr_stub 16

; 17-31 intel reserved

; IRQs
intr_stub 32
intr_stub 33
intr_stub 34
intr_stub 35
intr_stub 36
intr_stub 37
intr_stub 38
intr_stub 39
intr_stub 40
intr_stub 41
intr_stub 42
intr_stub 43
intr_stub 44
intr_stub 45
intr_stub 46
intr_stub 47

; syscall
intr_stub 48

; common handler, saving cpu state
intr_common_handler:
    pusha               ; pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds
    push eax            ; save data segment descriptor

    mov ax, 0x10        ; load kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call interrupt_handler

    pop eax             ; reload original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                ; pops edi,esi,...
    add esp, 8          ; cleans pushed error code and intr number
    sti
    iret                ; pops cs, eip, eflags, ss, esp
