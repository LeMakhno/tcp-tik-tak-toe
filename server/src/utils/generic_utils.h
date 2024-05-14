#ifndef GENERIC_UTILS
#define GENERIC_UTILS 1

#include "../include/common.h"

void *malloc_w(size_t size);
void *calloc_w(size_t n, size_t size);
void mclear(void *ptr, size_t size);
void close_w(int fd, bool silent);
char *new_string(char *str);
char *new_fstring(int maxlen, char *format, ...);
bool random_bool();

#endif
