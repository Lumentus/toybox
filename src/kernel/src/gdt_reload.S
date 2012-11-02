global gdt_reload

gdt_reload:              ; Reload CS register containing code selector:
    jmp 0x08:reload_cs   ; 0x08 points at the new code selector

reload_cs:               ; Reload data segment registers:
    mov ax, 0x10         ; 0x10 points at the new data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret
