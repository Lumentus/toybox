/*
FUNCTION
	<void* memset>
    void *dst
    int c
    size_t n

INCLUDES
    <string.h>

DESCRIPTION
    Copies c (unsigned char) into each of the first n bytes of the object
    pointed to by dst.

RETURNS
    Returns dst; no return value is reserved to indicate an error.
*/

#include <string.h>

void* memset(void *dst0, int c, size_t n)
{
    char *dst = dst0;

    while (n--)
    {
        *dst++ = (char)c;
    }

    return dst0;
}
