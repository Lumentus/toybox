#ifndef PMM_H
#define PMM_H


#include <stdint.h>
#include "multiboot.h"

#define PMM_NO_MEM (void *)0xDEADC0DE

void free_frame(uintptr_t addr, uint32_t frames);
void* alloc_frame(uint32_t frames);
void pmm_init(multiboot_info_t *mb_info);


#endif // PMM_H
