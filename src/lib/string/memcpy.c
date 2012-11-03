/*
FUNCTION
    [void* memcpy]
    void *dst
    const void *src
    size_t n

INCLUDES
    <string.h>

DESCRIPTION
    Copies n bytes from the object pointed to by src into the object pointed to
    by dst. If copying takes place between objects that overlap, the behavior
    is undefined.

RETURNS
    Returns dst; no return value is reserved to indicate an error.

ERRORS
    No errors are defined.
*/

#include <string.h>

void* memcpy(void *dst0, const void *src0, size_t n)
{
    char *dst = dst0;
    const char *src = src0;

    while (n--)
    {
        *dst++ = *src++;
    }

    return dst0;
}
