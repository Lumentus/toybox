#ifndef _STDLIB_H
#define _STDLIB_H


#include <stddef.h>
/* TODO: complete
#include <limits.h>
#include <math.h>
#include <sys/wait.h>
*/

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define RAND_MAX 0x7FFFFFFF

#define MB_CUR_MAX 4

void *malloc(size_t size);
void *calloc(size_t size, size_t count);
void *realloc(void *ptr, size_t size);
void free(void *ptr);


#endif // _STDLIB_H
