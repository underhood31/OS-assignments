//apt-get install gcc-multilib

#include "thread.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


// thread metadata
struct thread {
	void *esp;
	struct thread *next;
	struct thread *prev;
};

struct thread *ready_list = NULL;     // ready list
struct thread *cur_thread = NULL;     // current thread

// defined in context.s
void context_switch(struct thread *prev, struct thread *next);

// insert the input thread to the end of the ready list.
static void push_back(struct thread *t)
{
	struct thread *temp=ready_list;
	if(ready_list==NULL){
		ready_list=t;
		ready_list->next=NULL;
		ready_list->prev=NULL;
		return;
	}
	while (temp->next!=NULL){
		temp=temp->next;
	}
	temp->next=t;
	t->prev=temp;
	t->next=NULL;
	
}

// remove the first thread from the ready list and return to caller.
static struct thread *pop_front()
{

	struct thread *toRet=ready_list;
	ready_list=ready_list->next;
	if(ready_list!=NULL)
		ready_list->prev=NULL;	

	return toRet;

}

// the next thread to schedule is the first thread in the ready list.
// obtain the next thread from the ready list and call context_switch.
static void schedule()
{
	// printf("Schedule1 called");

	if(ready_list==NULL)
		return;
	struct thread *prev = cur_thread;
	struct thread *next = pop_front();
	cur_thread=next;
	context_switch(prev,next);
	
}

// push the cur_thread to the end of the ready list and call schedule
// if cur_thread is null, allocate struct thread for cur_thread
static void schedule1()
{
	// printf("Schedule called");
	
	if(cur_thread==NULL){
		cur_thread=malloc(sizeof(struct thread));
		// void *esp = malloc(4096);
		// esp+=4096;
		// cur_thread->esp=esp;
		// cur_thread->next=NULL;
		// cur_thread->prev=NULL;
	}
	push_back(cur_thread);
	schedule();
}

// allocate stack and struct thread for new thread
// save the callee-saved registers and parameters on the stack
// set the return address to the target thread
// save the stack pointer in struct thread
// push the current thread to the end of the ready list
void create_thread(func_t func, void *param)
{
	unsigned *stack = malloc(4096);
	stack+=1024;
	unsigned *esp=stack;

	struct thread *new_thread = malloc(sizeof(struct thread));
	
	//Putting arguements in stack
	*esp=(unsigned)param;
	esp-=sizeof(unsigned)/4;
	//Putting fake return address
	*esp=(unsigned)0;
	esp-=sizeof(unsigned)/4;
	//Pushing function
	*esp=(unsigned)func;
	esp-=sizeof(unsigned)/4;
	//Putting ebx
	*esp=(unsigned)0;
	esp-=sizeof(unsigned)/4;
	//Putting edi
	*esp=(unsigned)0;
	esp-=sizeof(unsigned)/4;
	//Putting edi
	*esp=(unsigned)0;
	esp-=sizeof(unsigned)/4;
	//Putting ebp
	*esp=(unsigned)0;
	

	new_thread->esp=esp;
	new_thread->next=NULL;
	new_thread->prev=NULL;
	// printf("\n	New thread created: base:%p and esp: %p: sizeof(*void)=%d, sizeof(func_t)=%d\n", stack,esp,sizeof(void*), sizeof(func_t));
	// cur_thread=new_thread;
	
	push_back(new_thread);
}

// call schedule1
void thread_yield()
{
	schedule1();
}

// call schedule
void thread_exit()
{
	schedule();
}

// call schedule1 until ready_list is null
void wait_for_all()
{
	while(ready_list!=NULL){
		schedule1();
	}

}
