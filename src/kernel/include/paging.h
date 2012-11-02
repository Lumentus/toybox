#ifndef PAGING_H
#define PAGING_H


#include <stdint.h>
#include "multiboot.h"

void* alloc_page(size_t pages);
void free_page(void *start, size_t pages);
void paging_register_interrupt();
void paging_init(multiboot_info_t *mb_info);


#endif // PAGING_H
