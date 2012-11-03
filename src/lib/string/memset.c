/*
FUNCTION
    [void* memset]
    void *s
    int c
    size_t n

INCLUDES
    <string.h>

DESCRIPTION
    Copies c (unsigned char) into each of the first n bytes of the object
    pointed to by s.

RETURNS
    Returns s; no return value is reserved to indicate an error.

ERRORS
    No errors are defined.
*/

#include <string.h>

void* memset(void *s0, int c, size_t n)
{
    char *s = s0;

    while (n--)
    {
        *s++ = (char)c;
    }

    return s0;
}
