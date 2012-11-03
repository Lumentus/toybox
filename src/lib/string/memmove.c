/*
FUNCTION
    [void* memmove]
    void *dst
    const void *src
    size_t n

INCLUDES
    <string.h>

DESCRIPTION
    Copies n bytes from the object pointed to by src into the object pointed to
    by dst. Copying takes place as if the n bytes from the object pointed to by
    src are first copied into a temporary array of n bytes that does not
    overlap the objects pointed to by dst and src, and then the n bytes from
    the temporary array are copied into the object pointed to by dst.

RETURNS
    Returns dst; no return value is reserved to indicate an error.

ERRORS
    No errors are defined.
*/

#include <string.h>

void* memmove(void *dst0, const void *src0, size_t n)
{
    char *dst = dst0;
    const char *src = src0;

    if (src < dst && dst < src + n)
    {
        // we have to copy backwards
        src += n;
        dst += n;

        while (n--)
        {
            *--dst = *--src;
        }
    }
    else
    {
        while (n--)
        {
            *dst++ = *src++;
        }
    }

    return dst0;
}
