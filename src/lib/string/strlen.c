/*
FUNCTION
    <size_t strlen>
    const char *s

INCLUDES
    <string.h>

DESCRIPTION
    Computes the number of bytes in the string to which s points, not including
    the terminating null byte.
RETURNS
    Returns the length of s; no return value is reserved to indicate an error.
*/

#include <string.h>

size_t strlen(const char *s)
{
    const char *start = s;

    while (*s)
    {
        s++;
    }

    return s - start;
}
