#ifndef FMMALLOC_H
#define FMMALLOC_H

#include <sys/types.h>

void *fmmalloc(size_t size);
void fmfree(void *ptr);
void *fmcalloc(size_t nmemb, size_t size);
void *fmrealloc(void *ptr, size_t size);
void *fmreallocarray(void *ptr, size_t nmemb, size_t size);

#endif
