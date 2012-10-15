#ifndef KERNEL_H
#define KERNEL_H


#define PANIC(msg) \
    panic("PANIC: %s:%d:%s", __FILE__, __LINE__, msg); \
    while (1);

extern const void kernel_start;
extern const void kernel_v_start;

extern const void kernel_end;
extern const void kernel_v_end;

extern const void kernel_size;


#endif // KERNEL_H
