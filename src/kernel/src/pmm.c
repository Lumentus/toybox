#include "pmm.h"
#include <stdint.h>
#include <string.h>
#include "multiboot.h"
#include "kernel.h"
#include "console.h"

#define FRAME_SIZE 0x1000
#define ELEMENT_SIZE (8 * sizeof(uint_fast32_t))
#define FRAME_MASK ~(FRAME_SIZE - 1)

// Bitmap, one bit per frame while 1 is reserved and 0 is free.
static uint_fast32_t *bitmap;
// Last allocated frame, used as a start point to look for free frames.
static uintptr_t last_alloc_frame = 0;
// Size of the bitmap in elements
static size_t bitmap_length;

// Aligns an address on a multiple of FRAME_SIZE, rounding up.
static inline uintptr_t __align_up(uintptr_t addr)
{
    return (addr + ~FRAME_MASK) & FRAME_MASK;
}

// Aligns an address on a multiple of FRAME_SIZE, rounding down.
static inline uintptr_t __align_down(uintptr_t addr)
{
    return addr & FRAME_MASK;
}

// Marks a frame as reserved.
static inline void __frame_mark_used(unsigned bm_idx, uint_fast32_t bit)
{
    bitmap[bm_idx] |= bit;
}

// Marks a frame as free.
static inline void __frame_mark_free(unsigned bm_idx, uint_fast32_t bit)
{
    bitmap[bm_idx] &= ~bit;
}

// Calculates the amount of frames a memory block needs.
static inline size_t __get_frames(uintptr_t addr_start, uintptr_t addr_end)
{
    return (__align_up(addr_end) - __align_down(addr_start)) / FRAME_SIZE;
}

// Marks an amount of frames as used, starting at addr.
static void frame_mark_range_used(uintptr_t addr, size_t frames)
{
    unsigned bm_idx;
    uint_fast32_t bit;
    addr /= FRAME_SIZE;
    bm_idx = addr / ELEMENT_SIZE;
    bit = 1 << (addr % ELEMENT_SIZE);

    while (frames--)
    {
        __frame_mark_used(bm_idx, bit);
        bit <<= 1;

        if (bit == 0)
        {
            bit = 1;
            bm_idx++;
        }
    }
}

// Marks an amount of frames as free, starting at addr.
void free_frame(uintptr_t addr, size_t frames)
{
    unsigned bm_idx;
    uint_fast32_t bit;
    addr /= FRAME_SIZE;
    bm_idx = addr / ELEMENT_SIZE;
    bit = 1 << (addr % ELEMENT_SIZE);

    while (frames--)
    {
        __frame_mark_free(bm_idx, bit);
        bit <<= 1;

        if (bit == 0)
        {
            bit = 1;
            bm_idx++;
        }
    }
}

// Allocates an amount of contiguous frames.
void* alloc_frame(size_t frames)
{
    uint_fast32_t bit;
    size_t size = 0;
    uintptr_t addr = 0;
    unsigned bm_idx = last_alloc_frame / FRAME_SIZE / ELEMENT_SIZE;
    unsigned bit_idx = last_alloc_frame / FRAME_SIZE % ELEMENT_SIZE;

    for (; bm_idx < bitmap_length; bm_idx++)
    {
        if (bitmap[bm_idx] == ~(uint_fast32_t)0)
        {
            size = 0;
            continue;
        }

        if (bitmap[bm_idx] == 0)
        {
            if (size == 0)
            {
                addr = bm_idx * ELEMENT_SIZE * FRAME_SIZE;
            }

            size += ELEMENT_SIZE;
        }
        else
        {
            bit = 1 << bit_idx;

            for (; bit_idx < ELEMENT_SIZE; bit_idx++, bit <<= 1)
            {
                if ((bitmap[bm_idx] & bit) == 0)
                {
                    if (size == 0)
                    {
                        addr = (bm_idx * ELEMENT_SIZE + bit_idx) * FRAME_SIZE;
                    }

                    if (++size >= frames)
                    {
                        break;
                    }
                }
                else
                {
                    size = 0;
                }
            }

            bit_idx = 0;
        }

        if (size >= frames)
        {
            frame_mark_range_used(addr, frames);
            last_alloc_frame = addr + (frames * FRAME_SIZE);

            return (void *)addr;
        }
    }

    return PMM_NO_MEM;
}

// Calculates how much space the bitmap requires to represent the memory.
static size_t required_bitmap_size(multiboot_info_t *mb_info)
{
    uintptr_t upper_end = 0;
    multiboot_memory_map_t *mmap = (void *)mb_info->mmap_addr;
    multiboot_memory_map_t *mmap_end = mmap + mb_info->mmap_length;

    while (mmap < mmap_end)
    {
        if (mmap->type == 1)
        {
            if (mmap->addr + mmap->len > upper_end)
            {
                upper_end = mmap->addr + mmap->len;
            }
        }

        mmap++;
    }

    kprintf("upper_end: 0x%x\n", upper_end);
    // TODO: Does this work with 4GB?
    return (size_t)(upper_end / FRAME_SIZE / 8);
}

// Searches for a free memory block to save the bitmap.
static void* find_free_mem(multiboot_info_t *mb_info, size_t size)
{
    multiboot_memory_map_t *mmap = (void *)mb_info->mmap_addr;
    multiboot_memory_map_t *mmap_end = mmap + mb_info->mmap_length;

    while (mmap < mmap_end)
    {
        if (mmap->type == 1 && mmap->len >= size)
        {
            // TODO: Check for reserved memory blocks.
            return (void *)((uintptr_t)mmap->addr);
        }

        mmap++;
    }

    return (void *)0x1000;
}

// Finds the free memory with the BIOS memory map and marks it as free.
static void process_memory_map(multiboot_info_t *mb_info)
{
    uintptr_t addr_start, addr_end;
    multiboot_memory_map_t *mmap = (void *)mb_info->mmap_addr;
    multiboot_memory_map_t *mmap_end = mmap + mb_info->mmap_length;

    while (mmap < mmap_end)
    {
        if (mmap->type == 1)
        {
            addr_start = mmap->addr;
            addr_end = addr_start + mmap->len;

            free_frame(addr_start, __get_frames(addr_start, addr_end));
        }

        mmap++;
    }
}

// Returns the address of the bitmap.
uintptr_t pmm_get_bitmap()
{
    return (uintptr_t)bitmap;
}

// Sets the address of the bitmap.
void pmm_set_bitmap(uintptr_t addr)
{
    bitmap = (uint_fast32_t *)addr;
}

// Returns the size of the bitmap in bytes.
size_t pmm_get_bitmap_size()
{
    return bitmap_length * sizeof(uint_fast32_t);
}

// Initialises the physical memory manager.
void pmm_init(multiboot_info_t *mb_info)
{
    unsigned i;
    multiboot_module_t *mod = (void *)mb_info->mods_addr;
    size_t bitmap_size = required_bitmap_size(mb_info);
    bitmap = find_free_mem(mb_info, bitmap_size);
    bitmap_length = 8 * bitmap_size / ELEMENT_SIZE;

    kprintf("&bitmap: 0x%x\n", (uintptr_t)bitmap);
    kprintf("bitmap_size: %u\n", bitmap_size);
    kprintf("&multiboot: 0x%x\n", (uintptr_t)mb_info);
    kprintf("mods_count: %u\n", mb_info->mods_count);

    // Set everything as reserved.
    memset(bitmap, 0xFF, bitmap_size);

    // Marks usable space as free.
    process_memory_map(mb_info);

    // Reserve space for the bitmap.
    frame_mark_range_used((uintptr_t)bitmap,
        __get_frames((uintptr_t)bitmap, (uintptr_t)bitmap + bitmap_size));

    // Reserve space for the kernel.
    frame_mark_range_used((uintptr_t)&kernel_start,
        __get_frames((uintptr_t)&kernel_start, (uintptr_t)&kernel_end));

    // Reserve space for the multiboot_info.
    frame_mark_range_used((uintptr_t)mb_info,
        __get_frames((uintptr_t)mb_info, (uintptr_t)(mb_info + 1)));

    // Reserve space for the multiboot modules.
    for (i = 0; i < mb_info->mods_count; i++, mod++)
    {
        // The Multiboot-info of the module.
        frame_mark_range_used((uintptr_t)mod,
            __get_frames((uintptr_t)mod, (uintptr_t)(mod + 1)));

        // The module itself.
        frame_mark_range_used(mod->mod_start,
            __get_frames(mod->mod_start, mod->mod_end));

        // The commandline of the module.
        if (mod->cmdline)
        {
            frame_mark_range_used(
                mod->cmdline,
                __get_frames(
                    mod->cmdline,
                    mod->cmdline + strlen((char *)mod->cmdline)));
        }
    }
}
