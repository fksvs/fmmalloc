#ifndef FMMALLOC_H
#define FMMALLOC_H

#include <sys/types.h>

void *fmmalloc(size_t size);
void fmfree(void *ptr);

#endif
