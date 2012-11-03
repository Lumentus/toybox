/*
FUNCTION
    [void* malloc]
    size_t size

INCLUDES
    <stdlib.h>

DESCRIPTION
    The malloc() function shall allocate unused space for an object whose size
    in bytes is specified by size and whose value is unspecified.
    The order and contiguity of storage allocated by successive calls to
    malloc() is unspecified. The pointer returned if the allocation succeeds
    shall be suitably aligned so that it may be assigned to a pointer to any
    type of object and then used to access such an object in the space
    allocated (until the space is explicitly freed or reallocated). Each such
    allocation shall yield a pointer to an object disjoint from any other
    object. The pointer returned points to the start (lowest byte address) of
    the allocated space. If the space cannot be allocated, a null pointer shall
    be returned. If the size of the space requested is 0, the behavior is
    implementation-defined: the value returned shall be either a null pointer
    or a unique pointer.

RETURNS
    Upon successful completion with size not equal to 0, malloc() shall return
    a pointer to the allocated space. If size is 0, either a null pointer or a
    unique pointer that can be successfully passed to free() shall be returned.
    Otherwise, it shall return a null pointer.
    POSIX.1-2008: Set errno to indicate the error.

ERRORS
    No errors are defined.
    POSIX.1-2008:
    The malloc() function shall fail if:
    [ENOMEM] Insufficient storage space is available.
*/

#include <stdlib.h>

// extern void* alloc_page(size_t pages);
// extern void free_page(void *start, size_t pages);

void* malloc(size_t size)
{
    size++;
    return NULL;
}
