#ifndef _STDARG_H
#define _STDARG_H


typedef __builtin_va_list va_list;
#define va_start(va, type)  __builtin_va_start(va, type)
#define va_arg(va, type)    __builtin_va_arg(va, type)
#define va_end(va)          __builtin_va_end(va)
#define va_copy(dst, src)   __builtin_va_copy(dst, src)


#endif // _STDARG_H
