#include "paging.h"
#include <stdint.h>
#include <string.h>
#include "kernel.h"
#include "pmm.h"
#include "console.h"

/*
directory entry & table entry
    present  : 1;    0: not present 1: present
    rw       : 1;    0: read-only 1: read/write
    user     : 1;    0: supervisor 1: all
    write    : 1;    0: write-back 1: write-through caching
    cache_d  : 1;    0: cached 1: cache disabled
    accessed : 1;    0: not 1: read or written to
    ####     : 3;    different for pte and pde
    avail    : 3;    available for software use
    frame    : 20;   frame address

page directory entry
    null     : 1;    ignored if size=4Kb, set to 1 if cpu writes to
    size     : 1;    0: 4 KiB 1: 4 MiB
    ignore   : 1;    ignored if size=4Kb, same as pte global

page table entry
    dirty    : 1;    0: not 1: written to
    null     : 1;    usually 0
    global   : 1;    0: not 1: prevent TLB from updating the adress in it's cache on cr3 reset
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

//static inline void __switch_page_directory(vmm_context_t *context)
//{
//    asm volatile("mov %0, %%cr3" : : "r" (context->page_directory));
//}

//static inline void __activate_paging()
//{
//    uint32_t cr0;
//    asm volatile("mov %%cr0, %0" : "=r" (cr0));
//    cr0 |= 0x80000000;
//    asm volatile("mov %0, %%cr0" : : "r" (cr0));
//}

//static void page_fault_callback(cpu_state_t cpu)
//{
//    uint32_t addr;
//    asm volatile("mov %%cr2, %0" : "=r" (addr));
//
//    kprintf("page fault (0x%x) at 0x%x\n", cpu.error, addr);
//    PANIC("Page fault!");
//}

void paging_init()
{
    kernel_context = alloc_frame(1);
    page_directory_t pd = create_page_directory();
    kernel_context->page_directory = pd;


    // TODO: Do the mapping here.


//    register_interrupt_handler(INT_PAGE_FAULT, page_fault_callback);
//
//    __switch_page_directory(kernel_context);
//
//    __activate_paging();
}

























