#ifndef _STRING_H
#define _STRING_H


#include <stddef.h>

void* memccpy(void *dst, const void *src, int c, size_t n);
void* memchr(const void *s, int c, size_t n);
void* memcpy(void *dst, const void *src, size_t n);
void* memmove(void *dst, const void *src, size_t n);
void* memset(void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

char* strcat(char *dst, const char *src);
char* strncat(char *dst, const char *src, size_t n);
char* strchr(const char *s, int c);
char* strrchr(const char *s, int c);
char* strcpy(char *dst, const char *src);
char* strncpy(char *dst, const char *src, size_t n);
char* strdup(const char *s);

char* strstr(const char *s, const char *search);
char* strtok(char *s, const char *sep);
char* strtok_r(char *s, const char *sep, char **lasts);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
int strcoll(const char *s1, const char *s2);

size_t strlen(const char *s);

char* strpbrk(const char *s, const char *charset);
size_t strcspn(const char *s, const char *charset);
size_t strspn(const char *s, const char *charset);
size_t strxfrm(char *dst, const char *src, size_t n);

char* strerror(int errnum);
int* strerror_r(int errnum, char *strerrbuf, size_t buflen);


#endif // _STRING_H
