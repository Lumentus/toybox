#ifndef KERNEL_H
#define KERNEL_H


#define PANIC(msg) panic(__FILE__, __LINE__, msg);

__attribute__((noreturn)) void panic(char *file, int line, char *msg);

// Defined in kernel.ld.
extern const void kernel_start;
extern const void kernel_v_start;

extern const void kernel_end;
extern const void kernel_v_end;

extern const void kernel_offset;

extern const void kernel_size;


#endif // KERNEL_H
