#ifndef PAGING_H
#define PAGING_H


#include <stdint.h>

void* alloc_page(size_t pages);
void free_page(void *start, size_t pages);
void paging_init();


#endif // PAGING_H
