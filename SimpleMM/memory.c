#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include "memory.h"

#define PAGE_SIZE 4096
#define META_SIZE 16
typedef struct md{
	size_t allocation_size;
	long long free_bytes_available;

}Metadata;

typedef struct node{
	struct node *next;
	struct node *prev;
}Node;

typedef struct ll{
	Node *current;
}LinkedList;


void *getbase(void *b);
void *add_page(size_t size, int bucket);
void *give_pointer(size_t size, int bucket);
void take_pointer(size_t node_size, int bucket, Metadata *base, void *ptr);




LinkedList buckets[9];
static void *alloc_from_ram(size_t size)
{
	assert((size % PAGE_SIZE) == 0 && "size must be multiples of 4096");
	void* base = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
	if (base == MAP_FAILED)
	{
		printf("Unable to allocate RAM space\n");
		exit(0);
	}
	return base;
}

static void free_ram(void *addr, size_t size)
{
	munmap(addr, size);
}

void myfree(void *ptr)
{
	void *base = getbase(ptr);
	Metadata *meta = (Metadata*)base;
	// size_t node_size =;
	if (meta->allocation_size>4080){
		// printf("\nbig freed %d %p, alloc size: %d", meta->allocation_size, base, meta->allocation_size);
		free_ram(base,meta->allocation_size);
		return;	
		
	}else{

		switch (meta->allocation_size){
			case 16:
				take_pointer(meta->allocation_size,0,base, ptr);
				break;
			case 32:
				take_pointer(meta->allocation_size,1,base, ptr);
				break;
			case 64:
				take_pointer(meta->allocation_size,2,base, ptr);
				break;
			case 128:
				take_pointer(meta->allocation_size,3,base, ptr);
				break;
			case 256:
				take_pointer(meta->allocation_size,4,base, ptr);
				break;
			case 512:
				take_pointer(meta->allocation_size,5,base, ptr);
				break;
			case 1024:
				take_pointer(meta->allocation_size,6,base, ptr);
				break;
			case 2048:
				take_pointer(meta->allocation_size,7,base, ptr);
				break;
			case 4080:
				take_pointer(meta->allocation_size,8,base, ptr);
				break;
		
		}
	}
	
	
}

void *mymalloc(size_t size)
{
	size=roundoff(size);
	void *toRet;

	if (size<=4080)
	{
		switch (size){
			case 16:
				 toRet = NULL;
				toRet=give_pointer(size,0);
				return toRet;		
			case 32:
				 toRet = NULL;
				toRet=give_pointer(size,1);
				return toRet;		
			case 64:
				 toRet = NULL;
				toRet=give_pointer(size,2);
				return toRet;		
			case 128:
				 toRet = NULL;
				toRet=give_pointer(size,3);
				return toRet;		
			case 256:
				 toRet = NULL;
				toRet=give_pointer(size,4);
				return toRet;		
			case 512:
				 toRet = NULL;
				toRet=give_pointer(size,5);
				return toRet;		
			case 1024:
				 toRet = NULL;
				toRet=give_pointer(size,6);
				return toRet;		
			case 2048:
				 toRet = NULL;
				toRet=give_pointer(size,7);
				return toRet;	
			case 4080:
				 toRet = NULL;
				toRet=give_pointer(size,8);
				return toRet;		
				break;
		}
	}
	else{
		void *page = alloc_from_ram(size);
		Metadata *meta = page;
		meta->free_bytes_available=0;
		meta->allocation_size=size;
		return page+16;
	}
	
}

//-------------------Function Definations-------------
size_t roundoff(size_t size){
	if(size<=4080){
		size_t num =16;	
		while(num<4080){
			if(num>=size)
				return num;
			num=num*2;
		}
		return 4080;
	}
	else{
		size=size+16;
		size_t num = 4096*2;
		while (size>num)
		{
			num+=4096;
		}
		return num;	
	}
	
}

void *getbase(void *b){
	long max=PAGE_SIZE-1;
	max=~max;
	b=(void*)((long)b&max);
	return b;

}

void *give_pointer(size_t size, int bucket){
	if(buckets[bucket].current==NULL){
		void* check = add_page(size, bucket);						
		if(check==NULL){
			return check;
		}
	} 

	void *toRet = (void*)buckets[bucket].current;
	buckets[bucket].current=buckets[bucket].current->next;
	if(buckets[bucket].current!=NULL)
		buckets[bucket].current->prev=NULL;
	
	Metadata *base = (Metadata*)getbase(toRet);
	base->free_bytes_available-=base->allocation_size;
	// printf("\nFree bytes in page: %d",  base->free_bytes_available);
	// printf("\nReturned: %p", toRet);

	return toRet;
}


void* add_page(size_t size, int bucket){
	void *toRet ;
	toRet = alloc_from_ram(PAGE_SIZE);
	
	if(toRet==NULL){
		return NULL;
	}
	Metadata *met=toRet;
	met->allocation_size = size;
	met->free_bytes_available = PAGE_SIZE-META_SIZE;
	
	//Making linked list in the allocated memory itself
	void *initial;
	initial = (toRet+META_SIZE);
	size_t bytesLeft=4080;
	Node *temp;
	for (int s = 16; s<=4080-size; s+=size)
	{
		void *next_point;
		next_point=(toRet+s);
		
		temp=(Node*)next_point;
		if (s==16)
		{
			temp->prev=NULL;
		}
		else
		{
			temp->prev=(Node*)(temp-size);
		}
		
		temp->next=(void *)(temp)+size;

	}
	
	temp->next=NULL;	
	buckets[bucket].current = (Node*)initial;
	return toRet;
}

void take_pointer(size_t node_size, int bucket, Metadata *base, void *ptr){
	// printf("\nGiven size  %d",base->allocation_size);
	base->free_bytes_available+= base->allocation_size;
	if(base->free_bytes_available==4080){

		Node *current = buckets[bucket].current;
		while (current!=NULL)
		{
			if (getbase((void*)current)==(void*)base)
			{
				if (current->prev!=NULL && current->next!=NULL){
					current->prev->next=current->next;
					current->next->prev=current->prev;
				}else if(current->prev==NULL && current->next!=NULL){
					current->next->prev=current->prev;

				}else if(current->prev!=NULL && current->next==NULL){
					current->prev->next=current->next;
				}else{
					current=NULL;
				}
			}
			if(current!=NULL){
				current=current->next;
			}
		}
	
			while(getbase((void*)buckets[bucket].current)==(void*)base){
				buckets[bucket].current=buckets[bucket].current->next;
				if (buckets[bucket].current==NULL)
						break;
			}		
		
		// printf("\nfreed page from %d", base->allocation_size);
		free_ram((void*)base,PAGE_SIZE);
		return;
	}else{
		Node *temp = (Node*) ptr;
		// buckets[bucket].end->next=temp;
		temp->next=buckets[bucket].current;
		if(buckets[bucket].current!=NULL)
			buckets[bucket].current->prev = temp;
		buckets[bucket].current=temp;
	}
}
