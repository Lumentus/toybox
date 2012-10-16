#include "pmm.h"
#include <stdint.h>
#include "multiboot.h"
#include "kernel.h"

#define FRAME_SIZE 0x1000
#define ELEMENT_SIZE (8 * sizeof(uint_fast32_t))
#define BITMAP_SIZE ((0x400 * 0x400) / ELEMENT_SIZE)
#define FRAME_MASK ~(FRAME_SIZE - 1)

// TODO: inline functions
#define ALIGN_UP(addr) (((addr) + ~FRAME_MASK) & FRAME_MASK)
#define ALIGN_DOWN(addr) ((addr) & FRAME_MASK)

//static uint_fast32_t *bitmap;
//static uintptr_t last_alloc_frame = 0;















void pmm_init(multiboot_info_t *mb_info)
{
    mb_info->flags = 0;
    return;
}







