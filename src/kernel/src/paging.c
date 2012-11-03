#include "paging.h"
#include <stdint.h>
#include <string.h>
#include "kernel.h"
#include "multiboot.h"
#include "pmm.h"
#include "interrupt.h"
#include "console.h"

/*

directory entry & table entry
    31              12|11        9|                                 0
    +---+---/~/---+---+---+---+---+---+---+---+---+---+---+---+---+---+
    |  Frame Address  |   AVAIL   |  PD / PT  | A | C | W | U | R | P |
    +---+---/~/---+---+---+---+---+---+---+---+---+---+---+---+---+---+

P   If the bit is set, the page is actually in physical memory at the moment.
R   Read/Write permissions flag.
    If the bit is set, the page is read/write. Otherwise the page is read-only.
U   User\Supervisor bit, controls access to the page based on privilege level.
    If the bit is set, then the page may be accessed by all.
W   Write-Through abilities of the page.
    If the bit is set, write-through caching is enabled.
    If not, then write-back is enabled instead.
C   Cache Disable bit. If the bit is set, the page will not be cached.
A   Accessed bit, used to discover whether a page has been read or written to.
AVAIL
    Available for software use.

PD
      2       0
    +---+---+---+
    | 0 | S | G |
    +---+---+---+

G   Global, ignored if size 4 KiB. Otherwise the same as PT Global.
S   Page Size, stores the page size for that specific entry.
    If the bit is set, then pages are 4 MiB in size. Otherwise, they are 4 KiB.
0   Ignored if size 4 KiB. Otherwise set to 1 if the CPU writes to.

PT
      2       0
    +---+---+---+
    | D | 0 | G |
    +---+---+---+

G   Global, prevents the TLB from updating the address in it's cache if CR3 is
    reset. The global enable bit in CR4 must be set to enable this feature.
0   Usually 0.
D   If the Dirty flag is set, then the page has been written to. This flag is
    not updated by the CPU, and once set will not unset itself.
*/

#define PE_PRESENT  0x01
#define PE_RW       0x02
#define PE_USER     0x04
#define PE_WRITE    0x08
#define PE_CACHE_D  0x10
#define PE_ACCESSED 0x20
#define PE_FRAME    0xFFFFF000

// page directory entry only
#define PDE_SIZE    0x40

// page table entry only
#define PTE_DIRTY   0x40
#define PTE_GLOBAL  0x80

// other definitions
#define PAGE_SIZE 0x1000
#define PD_SIZE 0x400
#define PT_SIZE 0x400
#define PD_RSHIFT 22
#define PT_RSHIFT 12

typedef uint32_t* page_table_t;
typedef uint32_t* page_directory_t;

typedef struct vmm_context
{
    page_directory_t page_directory;
} vmm_context_t;

static vmm_context_t *kernel_context;

// Aligns an address on a multiple of PAGE_SIZE, rounding up.
static inline uint32_t __align_up(uint32_t addr)
{
    return (addr + ~PE_FRAME) & PE_FRAME;
}

// Aligns an address on a multiple of PAGE_SIZE, rounding down.
static inline uint32_t __align_down(uint32_t addr)
{
    return addr & PE_FRAME;
}

static inline unsigned __get_pd_idx(uint32_t addr)
{
    return addr >> PD_RSHIFT;
}

static inline unsigned __get_pt_idx(uint32_t addr)
{
    return (addr >> PT_RSHIFT) & ~PE_FRAME;
}

static page_directory_t create_page_directory()
{
    page_directory_t pd = alloc_frame(1);

    memset(pd, 0, PAGE_SIZE);

    return pd;
}

static page_table_t create_page_table()
{
    page_table_t pt = alloc_frame(1);

    memset(pt, 0, PAGE_SIZE);

    return pt;
}

static void map_page(vmm_context_t *context, uint32_t v_addr, uint32_t p_addr)
{
    // TODO: flags
    unsigned pd_idx = __get_pd_idx(v_addr);
    unsigned pt_idx = __get_pt_idx(v_addr);
    page_directory_t pd = context->page_directory;
    page_table_t pt = (uint32_t *)(pd[pd_idx] & PE_FRAME);

    if ((pd[pd_idx] & PE_PRESENT) == 0)
    {
        pt = create_page_table();
        pd[pd_idx] = (uint32_t)pt | PE_RW | PE_PRESENT;
    }

    if (pt[pt_idx] & PE_PRESENT)
    {
        PANIC("Already mapped!");
    }

    pt[pt_idx] = ((uint32_t)p_addr) | PE_PRESENT; // (flags & ~PE_FRAME)

    // TODO: flush the entry in the TLB, if paging is already activated

    // i486 and later only!
    // asm volatile("invlpg %0" : : "memory" (*(uint8_t *)v_addr));
}

static void map_memory(vmm_context_t *context, uint32_t v_addr_start, uint32_t p_addr_start, uint32_t p_addr_end)
{
    p_addr_start = __align_down(p_addr_start);
    v_addr_start = __align_down(v_addr_start);
    p_addr_end = __align_up(p_addr_end);

    while (p_addr_start < p_addr_end)
    {
        map_page(context, v_addr_start, p_addr_start);
        v_addr_start += PAGE_SIZE;
        p_addr_start += PAGE_SIZE;
    }
}

static void identity_map(vmm_context_t *context, uint32_t p_addr_start, uint32_t p_addr_end)
{
    p_addr_start = __align_down(p_addr_start);
    p_addr_end = __align_up(p_addr_end);
    unsigned pd_idx = __get_pd_idx(p_addr_start);
    unsigned pt_idx = __get_pt_idx(p_addr_start);
    page_directory_t pd = context->page_directory;
    page_table_t pt = (uint32_t *)(pd[pd_idx] & PE_FRAME);

    while (p_addr_start < p_addr_end)
    {
        if ((pd[pd_idx] & PE_PRESENT) == 0)
        {
            pt = create_page_table();
            pd[pd_idx] = (uint32_t)pt | PE_RW | PE_PRESENT;
        }

        pt[pt_idx] = p_addr_start | PE_RW | PE_PRESENT;
        p_addr_start += PAGE_SIZE;

        if (++pt_idx >= PT_SIZE)
        {
            pt_idx = 0;
            pd_idx++;
        }
    }
}

//void* alloc_page(uint32_t pages)
//{
//    // TODO: implement
//}

//void free_page(void *start, uint32_t pages)
//{
//    // TODO: implement
//}

static inline void __switch_page_directory(vmm_context_t *context)
{
    asm volatile("mov %0, %%cr3" : : "r" (context->page_directory));
}

static void activate_paging()
{
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" : : "r" (cr0));
}

static void page_fault_callback(cpu_state_t cpu)
{
    uint32_t addr;
    asm volatile("mov %%cr2, %0" : "=r" (addr));

    kprintf("\n\npage fault (0x%x) at 0x%x", cpu.error, addr);
    PANIC("Page fault!");
}

void paging_register_interrupt()
{
    register_interrupt_handler(INT_PAGE_FAULT, page_fault_callback);
}

void paging_init(multiboot_info_t *mb_info)
{
    kernel_context = alloc_frame(1);
    page_directory_t pd = create_page_directory();
    kernel_context->page_directory = pd;

    // TODO: Map the page directory.

    map_memory(kernel_context, (uintptr_t)&kernel_v_start,
        (uintptr_t)&kernel_start, (uintptr_t)&kernel_end);

    identity_map(kernel_context, pmm_get_bitmap(),
        pmm_get_bitmap() + pmm_get_bitmap_size());

    map_memory(kernel_context, (uintptr_t)mb_info,
        (uintptr_t)mb_info - (uintptr_t)&kernel_offset,
        (uintptr_t)mb_info - (uintptr_t)&kernel_offset +
            sizeof(multiboot_info_t));

    identity_map(kernel_context, 0x0, 0x00400000);
//    identity_map(kernel_context, 0xB8000, 0xBFFFF);

    __switch_page_directory(kernel_context);

    activate_paging();
}
