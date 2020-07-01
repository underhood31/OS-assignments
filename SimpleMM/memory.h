#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stddef.h>

void *mymalloc(size_t Size);
void myfree(void *ptr);
size_t roundoff(size_t size);
void *getbase(void *b);

#endif
