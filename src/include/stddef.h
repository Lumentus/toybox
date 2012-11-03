#ifndef _STDDEF_H
#define _STDDEF_H


#define NULL ((void *)0)

typedef signed int ptrdiff_t;

typedef int wchar_t;

typedef unsigned int size_t;

/* offsetof(type, member-designator)
   Integer constant expression of type size_t,
   the value of which is the offset in bytes to the structure member
   (member-designator), from the beginning of its structure (type). */
#define offsetof(type, member) __builtin_offsetof(type, member)
// define offsetof(type, member) ((size_t)((char *)&((type *)0)->member - (char *)0))


#endif // _STDDEF_H
