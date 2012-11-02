#include <stdint.h>

#define GDT_ENTRIES 5

// granularity
#define SEG_SIZE(x) ((x) << 0x06)   // size (0: 16 bit, 1: 32 bit)
#define SEG_GRAN(x) ((x) << 0x07)   // granularity (0: 1B - 1MB, 1: 4KB - 4GB)

// access byte
#define SEG_TYPE(x) ((x) << 0x04)   // descriptor (0: system, 1: code/data)
#define SEG_PRIV(x) ((x) << 0x05)   // privilege level (0 - 3)
#define SEG_PRES(x) ((x) << 0x07)   // present (1: valid selector)

// Read/Write/Execute
#define SEG_DATA_RD        0x00     // Read-Only
#define SEG_DATA_RDWR      0x02     // Read/Write
#define SEG_DATA_RDEXPD    0x04     // Read-Only, expand-down
#define SEG_DATA_RDWREXPD  0x06     // Read/Write, expand-down
#define SEG_CODE_EX        0x08     // Execute-Only
#define SEG_CODE_EXRD      0x0A     // Execute/Read
#define SEG_CODE_EXC       0x0C     // Execute-Only, conforming
#define SEG_CODE_EXRDC     0x0E     // Execute/Read, conforming


// gdt segments
#define GDT_CODE_PL0 SEG_TYPE(1) | SEG_PRIV(0) | \
                     SEG_PRES(1) | SEG_CODE_EXRD

#define GDT_DATA_PL0 SEG_TYPE(1) | SEG_PRIV(0) | \
                     SEG_PRES(1) | SEG_DATA_RDWR

#define GDT_GRAN_PL0 SEG_SIZE(1) | SEG_GRAN(1)

#define GDT_CODE_PL3 SEG_TYPE(1) | SEG_PRIV(3) | \
                     SEG_PRES(1) | SEG_CODE_EXRD

#define GDT_DATA_PL3 SEG_TYPE(1) | SEG_PRIV(3) | \
                     SEG_PRES(1) | SEG_DATA_RDWR

#define GDT_GRAN_PL3 SEG_SIZE(1) | SEG_GRAN(1)


struct gdt_entry
{
    uint16_t limit_low;     // limit 0-15
    uint16_t base_low;      // base  0-15
    uint8_t base_middle;    // base 16-23
    uint8_t access;         // access 0-7
    uint8_t granularity;    // limit 0-3, flags 4-7
    uint8_t base_high;      // base 24-31
} __attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

extern void gdt_reload();

static gdt_entry_t gdt[GDT_ENTRIES];


static void gdt_set(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    gdt[i].base_low     = base & 0xFFFF;
    gdt[i].base_middle  = (base >> 16) & 0xFF;
    gdt[i].base_high    = (base >> 24) & 0xFF;
    gdt[i].limit_low    = limit & 0xFFFF;
    gdt[i].granularity  = (limit >> 16) & 0x0F;
    gdt[i].granularity |= granularity & 0xF0;
    gdt[i].access       = access;
}

static void gdt_load()
{
    struct
    {
        uint16_t size;
        uint32_t ptr;
    } __attribute__((packed)) gdtp =
    {
        .size = GDT_ENTRIES * 8 - 1,
        .ptr = (uint32_t)gdt,
    };

    asm volatile("lgdt %0" : : "m" (gdtp));
    gdt_reload();
}

void gdt_init()
{
    gdt_set(0, 0, 0, 0, 0);
    gdt_set(1, 0, 0x000FFFFF, (GDT_CODE_PL0), (GDT_GRAN_PL0));
    gdt_set(2, 0, 0x000FFFFF, (GDT_DATA_PL0), (GDT_GRAN_PL0));
    gdt_set(3, 0, 0x000FFFFF, (GDT_CODE_PL3), (GDT_GRAN_PL3));
    gdt_set(4, 0, 0x000FFFFF, (GDT_DATA_PL3), (GDT_GRAN_PL3));

    gdt_load();
}
