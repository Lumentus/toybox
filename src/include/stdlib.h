#ifndef _STDLIB_H
#define _STDLIB_H


#include <stddef.h>
/* TODO: complete + comments
   #include <limits.h>
   #include <math.h>
   #include <sys/wait.h> */

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define RAND_MAX 0x7FFFFFFF

#define MB_CUR_MAX 4

typedef struct { int quot, rem; } div_t;
typedef struct { long quot, rem; } ldiv_t;
typedef struct { long long quot, rem; } lldiv_t;

void exit(int status);
void abort(void);
int atexit(void (*func)(void));

void *malloc(size_t size);
void *calloc(size_t size, size_t count);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

int system(const char *command);

int mkstemp(char *_template);

double atof(const char *s);
int atoi(const char *s);
long atol(const char *s); // strtol(str, (char **)NULL, 10)
long long atoll(const char *s); // strtoll(nptr, (char **)NULL, 10)

div_t div(int numer, int denom);
ldiv_t ldiv(long numer, long denom);
lldiv_t lldiv(long long numer, long long denom);

inline int abs(int i) { return (i < 0) ? -i : i; }
inline long labs(long i) { return (i < 0) ? -i : i; }
inline long long llabs(long long i) { return (i < 0) ? -i : i; }

int mblen(const char *s, size_t n);
size_t mbstowcs(wchar_t *pwcs, const char *src, size_t n);
int mbtowc(wchar_t *pwc, const char *src, size_t n);
size_t wcstombs(char *src, const wchar_t *pwcs, size_t n);
int wctomb(char *src, wchar_t wchar);

int rand(void);
int rand_r(unsigned *seed);
long random(void);
void srand(unsigned seed);
void srandom(unsigned seed);
char *initstate(unsigned seed, char *state, size_t size);
char *setstate(const char *state);

void qsort(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *));
void *bsearch(const void *key, const void *base, size_t n, size_t size, int (*cmp)(const void *, const void *));

char *getenv(const char *name);
int putenv(char *s);
int setenv(const char *envname, const char *envval, int overwrite);
int unsetenv(const char *name);

double strtod(const char *ptr, char **endptr);
float strtof(const char *ptr, char **endptr);
long double strtold(const char *ptr, char **endptr);
long strtol(const char *ptr, char **endptr, int base);
long long strtoll(const char *ptr, char **endptr, int base);
unsigned long strtoul(const char *ptr, char **endptr, int base);
unsigned long long strtoull(const char *ptr, char **endptr, int base);


#endif // _STDLIB_H
