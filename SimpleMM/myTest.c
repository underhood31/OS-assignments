#include "memory.h"
#include <stdio.h>
void *getbase(void *b);


typedef struct md{
	size_t allocation_size;
	long long free_bytes_available;

}Metadata;
void main(){
    

    Metadata *a = mymalloc(4096);
    size_t b = roundoff(4096);
    a=(Metadata*)getbase(a);
    printf("%ld ; %ld",b,a->allocation_size);



}   

