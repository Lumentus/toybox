#ifndef _STRING_H
#define _STRING_H


#include <stddef.h>

void* memccpy(void *dst, const void *src, int c, size_t n);
void* memchr(const void *s, int c, size_t n);
void* memcpy(void *dst, const void *src, size_t n);
void* memmove(void *dst, const void *src, size_t n);
void* memset(void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);


#endif // _STRING_H
