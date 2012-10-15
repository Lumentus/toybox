#ifndef _STDINT_H
#define _STDINT_H


#include <stddef.h>

/* Exact-width integer types */
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long long int64_t;
typedef unsigned long long uint64_t;

/* Minimum-width integer types */
typedef int8_t int_least8_t;
typedef uint8_t uint_least8_t;
typedef int16_t int_least16_t;
typedef uint16_t uint_least16_t;
typedef int32_t int_least32_t;
typedef uint32_t uint_least32_t;
typedef int64_t int_least64_t;
typedef uint64_t uint_least64_t;

/* Fastest minimum-width integer types */
typedef int8_t int_fast8_t;
typedef uint8_t uint_fast8_t;
typedef int32_t int_fast16_t;
typedef uint32_t uint_fast16_t;
typedef int32_t int_fast32_t;
typedef uint32_t uint_fast32_t;
typedef int64_t int_fast64_t;
typedef uint64_t uint_fast64_t;

/* Integer types capable of holding object pointers */
typedef signed int intptr_t;
typedef unsigned int uintptr_t;

/* Greatest-width integer types */
typedef signed long long intmax_t;
typedef unsigned long long uintmax_t;

/* Limits of exact-width integer types */
#define INT8_MIN -128
#define INT8_MAX 127
#define INT16_MIN -32768
#define INT16_MAX 32767
#define INT32_MIN -2147483648
#define INT32_MAX 2147483647
#define INT64_MIN -9223372036854775808LL
#define INT64_MAX 9223372036854775807LL
#define UINT8_MAX 0xFF
#define UINT16_MAX 0xFFFF
#define UINT32_MAX 0xFFFFFFFF
#define UINT64_MAX 0xFFFFFFFFFFFFFFFFULL

/* Limits of minimum-width integer types */
#define INT_LEAST8_MIN INT8_MIN
#define INT_LEAST8_MAX INT8_MAX
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MIN INT64_MIN
#define INT_LEAST64_MAX INT64_MAX
#define UINT_LEAST8_MAX UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

/* Limits of fastest minimum-width integer types */
#define INT_FAST8_MIN INT8_MIN
#define INT_FAST8_MAX INT8_MAX
#define INT_FAST16_MIN INT32_MIN
#define INT_FAST16_MAX INT32_MAX
#define INT_FAST32_MIN INT32_MIN
#define INT_FAST32_MAX INT32_MAX
#define INT_FAST64_MIN INT64_MIN
#define INT_FAST64_MAX INT64_MAX
#define UINT_FAST8_MAX UINT8_MAX
#define UINT_FAST16_MAX UINT32_MAX
#define UINT_FAST32_MAX UINT32_MAX
#define UINT_FAST64_MAX UINT64_MAX

/* Limits of integer types capable of holding object pointers */
#define INTPTR_MIN INT32_MIN
#define INTPTR_MAX INT32_MAX
#define UINTPTR_MAX UINT32_MAX

/* Limits of greatest-width integer types */
#define INTMAX_MIN INT64_MIN
#define INTMAX_MAX INT64_MAX
#define UINTMAX_MAX UINT64_MAX

/* Limits of Other Integer Types */
#define PTRDIFF_MIN INT32_MIN
#define PTRDIFF_MAX INT32_MAX

#define SIG_ATOMIC_MIN 0
#define SIG_ATOMIC_MAX UINT32_MAX

#define SIZE_MAX UINTPTR_MAX

#define WCHAR_MIN 0
#define WCHAR_MAX INT32_MAX

#define WINT_MIN WCHAR_MIN
#define WINT_MAX WCHAR_MAX

/* Macros for minimum-width integer constant expressions */
#define INT8_C(x) ((int_least8_t) x)
#define UINT8_C(x) ((uint_least8_t) x)
#define INT16_C(x) ((int_least16_t) x)
#define UINT16_C(x) ((uint_least16_t) x)
#define INT32_C(x) ((int_least32_t) x)
#define UINT32_C(x) ((uint_least32_t) x)
#define INT64_C(x) ((int_least64_t) x##LL)
#define UINT64_C(x) ((uint_least64_t) x##ULL)

/* Macros for greatest-width integer constant expressions */
#define INTMAX_C(x) INT64_C(x)
#define UINTMAX_C(x) UINT64_C(x)


#endif // _STDINT_H
