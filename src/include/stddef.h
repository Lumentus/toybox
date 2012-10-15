#ifndef _STDDEF_H
#define _STDDEF_H


#define NULL ((void *)0)

// Signed integer type of the result of subtracting two pointers
typedef int ptrdiff_t;

// Integer type whose range of values can represent distinct wide-character codes
typedef unsigned int wchar_t;

// Unsigned integer type of the result of the sizeof operator
typedef unsigned int size_t;

/* offsetof(type, member-designator)
   Integer constant expression of type size_t,
   the value of which is the offset in bytes to the structure member
   (member-designator), from the beginning of its structure (type). */
#define offsetof(type, member) __builtin_offsetof(type, member)
// define offsetof(type, member) ((size_t)((char *)&((type *)0)->member - (char *)0))


#endif // _STDDEF_H
