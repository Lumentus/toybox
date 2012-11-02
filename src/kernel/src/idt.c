#include <stdint.h>
#include "ports.h"

#define IDT_ENTRIES 256

#define CAST_ENTRY(x) (*((uint64_t *)&((x))))

// interrupt descripor flags
#define INT_PRES(x) ((x) << 0x07)   // present
#define INT_PRIV(x) ((x) << 0x05)   // privilege level (0 - 3)

#define INT_GATE_TSK_32 0x5     // 32 bit task gate
#define INT_GATE_INT_16 0x6     // 16 bit interrupt gate
#define INT_GATE_TRP_16 0x7     // 16 bit trap gate
#define INT_GATE_INT_32 0xE     // 32 bit interrupt gate
#define INT_GATE_TRP_32 0xF     // 32 bit trap gate

#define INT_KERNEL (INT_GATE_INT_32 | INT_PRES(1) | INT_PRIV(0))

extern void intr0();
extern void intr1();
extern void intr2();
extern void intr3();
extern void intr4();
extern void intr5();
extern void intr6();
extern void intr7();
extern void intr8();
extern void intr9();
extern void intr10();
extern void intr11();
extern void intr12();
extern void intr13();
extern void intr14();
// reserved
extern void intr16();
// 17-31 reserved
extern void intr32();
extern void intr33();
extern void intr34();
extern void intr35();
extern void intr36();
extern void intr37();
extern void intr38();
extern void intr39();
extern void intr40();
extern void intr41();
extern void intr42();
extern void intr43();
extern void intr44();
extern void intr45();
extern void intr46();
extern void intr47();
// syscall
extern void intr48();


struct idt_entry
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t offset_high;
} __attribute__((packed));
typedef struct idt_entry idt_entry_t;

static idt_entry_t idt[IDT_ENTRIES];


static void idt_set(int i, uint32_t offset, uint16_t selector, uint8_t flags)
{
    idt[i].offset_low  = offset & 0xFFFF;
    idt[i].offset_high = (offset >> 16) & 0xFFFF;
    idt[i].selector    = selector;
    idt[i].flags       = flags;
    idt[i].zero        = 0;
}

static inline void idt_load()
{
    struct
    {
        uint16_t size;
        uint32_t ptr;
    } __attribute((packed)) idtp =
    {
        .size = IDT_ENTRIES * 8 - 1,
        .ptr = (uint32_t)idt,
    };

    asm volatile("lidt %0" : : "m" (idtp));
}

void idt_init()
{
    uint32_t i;

    idt_set( 0, (uint32_t)intr0,  0x08, INT_KERNEL);
    idt_set( 1, (uint32_t)intr1,  0x08, INT_KERNEL);
    idt_set( 2, (uint32_t)intr2,  0x08, INT_KERNEL);
    idt_set( 3, (uint32_t)intr3,  0x08, INT_KERNEL);
    idt_set( 4, (uint32_t)intr4,  0x08, INT_KERNEL);
    idt_set( 5, (uint32_t)intr5,  0x08, INT_KERNEL);
    idt_set( 6, (uint32_t)intr6,  0x08, INT_KERNEL);
    idt_set( 7, (uint32_t)intr7,  0x08, INT_KERNEL);
    idt_set( 8, (uint32_t)intr8,  0x08, INT_KERNEL);
    idt_set( 9, (uint32_t)intr9,  0x08, INT_KERNEL);
    idt_set(10, (uint32_t)intr10, 0x08, INT_KERNEL);
    idt_set(11, (uint32_t)intr11, 0x08, INT_KERNEL);
    idt_set(12, (uint32_t)intr12, 0x08, INT_KERNEL);
    idt_set(13, (uint32_t)intr13, 0x08, INT_KERNEL);
    idt_set(14, (uint32_t)intr14, 0x08, INT_KERNEL);

    CAST_ENTRY(idt[15]) = 0; // reserved

    idt_set(16, (uint32_t)intr16, 0x08, INT_KERNEL);

    for (i = 17; i < 32; i++) CAST_ENTRY(idt[i]) = 0; // 17-31 reserved

    idt_set(32, (uint32_t)intr32, 0x08, INT_KERNEL);
    idt_set(33, (uint32_t)intr33, 0x08, INT_KERNEL);
    idt_set(34, (uint32_t)intr34, 0x08, INT_KERNEL);
    idt_set(35, (uint32_t)intr35, 0x08, INT_KERNEL);
    idt_set(36, (uint32_t)intr36, 0x08, INT_KERNEL);
    idt_set(37, (uint32_t)intr37, 0x08, INT_KERNEL);
    idt_set(38, (uint32_t)intr38, 0x08, INT_KERNEL);
    idt_set(39, (uint32_t)intr39, 0x08, INT_KERNEL);
    idt_set(40, (uint32_t)intr40, 0x08, INT_KERNEL);
    idt_set(41, (uint32_t)intr41, 0x08, INT_KERNEL);
    idt_set(42, (uint32_t)intr42, 0x08, INT_KERNEL);
    idt_set(43, (uint32_t)intr43, 0x08, INT_KERNEL);
    idt_set(44, (uint32_t)intr44, 0x08, INT_KERNEL);
    idt_set(45, (uint32_t)intr45, 0x08, INT_KERNEL);
    idt_set(46, (uint32_t)intr46, 0x08, INT_KERNEL);
    idt_set(47, (uint32_t)intr47, 0x08, INT_KERNEL);
    idt_set(48, (uint32_t)intr48, 0x08, INT_KERNEL);

    for (i = 49; i < IDT_ENTRIES; i++) CAST_ENTRY(idt[i]) = 0;

    idt_load();

    // master PIC
    outb(0x20, 0x11); // initialize command
    outb(0x21, 0x20); // interrupt number for IRQ 0
    outb(0x21, 0x04); // IRQ 2 slave
    outb(0x21, 0x01); // ICW 4

    // slave PIC
    outb(0xA0, 0x11);
    outb(0xA1, 0x28);
    outb(0xA1, 0x02);
    outb(0xA1, 0x01);

    // enable IRQs
    outb(0x20, 0x0);
    outb(0xA0, 0x0);
    asm volatile("sti");
}
