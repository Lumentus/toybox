#ifndef PMM_H
#define PMM_H


#include <stdint.h>
#include "multiboot.h"

#define PMM_NO_MEM ((void *)0x13579B00)

void free_frame(uintptr_t addr, size_t frames);
void* alloc_frame(size_t frames);
uintptr_t pmm_get_bitmap();
void pmm_set_bitmap(uintptr_t addr);
size_t pmm_get_bitmap_size();
void pmm_init(multiboot_info_t *mb_info);


#endif // PMM_H
