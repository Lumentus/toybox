#include <stdint.h>

/*
gdt descriptor
    15      0
    +---+---+
    | size  |
    +---+---+---+---+
    |    offset     |
    +---+---+---+---+

size
    Size of the table subtracted by 1. Can be up to 65536 bytes
    (a maximum of 8192 entries).
offset
    Linear address of the table itself.

gdt entry
    15                                              0
    +-----------------------+-----------------------+
    |                  limit 0:15                   |
    +-----------------------+-----------------------+
    |                   base 0:15                   |
    +-----------------------+-----------------------+
    |        access         |      base 16:23       |
    +-----------------------+-----------------------+
    |      base 24:31       |   flags   |limit 16:19|
    +-----------------------+-----------------------+

base
    32 bit value containing the linear address where the segment begins.
limit
    20 bit value, tells the maximum addressable unit (either in 1 byte units,
    or in pages).

access
      7                                   0
    +----+----+----+----+----+----+----+----+
    | PR |  PRIVL  | SE | EX | DC | RW | AC |
    +----+----+----+----+----+----+----+----+

AC  Accessed bit. The CPU sets this to 1 when the segment is accessed.
RW  Readable bit/Writable bit.
    Readable bit for code selectors: Whether read access for this segment is
    allowed. Write access is never allowed for code segments.
    Writable bit for data selectors: Whether write access for this segment is
    allowed. Read access is always allowed for data segments.
DC  Direction bit/Conforming bit.
    Direction bit for data selectors: Tells the direction. 0 the segment grows
    up, 1 segment grows down, ie. the offset has to be greater than the base.
    If 1 code in this segment can be executed from an equal or lower privilege
    level. The privl-bits represent the highest privilege level that is allowed
    to execute the segment.
    If 0 code in this segment can only be executed from the ring set in privl.
EX  Executable bit.
    If 1 code in this segment can be executed, ie. a code selector.
    If 0 it is a data selector.
SE  1 for code-/datasegments.
    0 for Gates and TSS.
    Bits 0 to 3 are only valid for code-/datasegments, otherwise they specify
    the segmenttype.
PRIVL
    Privilege, 2 bits. Contains the ring level, 0 = highest (kernel),
    3 = lowest (user applications).
PR  This must be 1 for all valid selectors.

flags
      3               0
    +----+----+----+----+
    | GR | SZ | LM | AV |
    +----+----+----+----+

AV  Available for software use.
LM  0 means protected mode, 1 means Long Mode.
    If 1 the size bit must be set to 0.
    If 0 the size bit specifies the bit size of the segment.
    Should only be set to 1 for code segments, otherwise 0.
SZ  If 0 the selector defines 16 bit protected mode. If 1 it defines 32 bit
    protected mode. You can have both 16 bit and 32 bit selectors at once.
GR  Granularity bit.
    If 0 the limit is in 1 B blocks (byte granularity),
    if 1 the limit is in 4 KiB blocks (page granularity).
*/

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
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity; // limit 0-3, flags 4-7
    uint8_t base_high;
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
