//apt-get install gcc-multilib

#include "thread.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


// thread metadata
struct thread {
	void *esp;
	void *stack_base;
	int clear;
	struct thread *next;
	struct thread *prev;
};

struct thread *ready_list = NULL;     // ready list
struct thread *cur_thread = NULL;     // current thread
int waiting_threads=0;
struct thread *to_clean;

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
	to_clean=cur_thread;
	cur_thread=next;
	context_switch(prev,next);
	if(to_clean->clear==1){
		free(to_clean->stack_base);
		free(to_clean);
	}
}

// push the cur_thread to the end of the ready list and call schedule
// if cur_thread is null, allocate struct thread for cur_thread
static void schedule1()
{
	// printf("Schedule called");
	
	if(cur_thread==NULL){
		cur_thread=malloc(sizeof(struct thread));
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
	unsigned *esp=stack+1024;

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
	
	new_thread->stack_base=stack;
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
	// free(cur_thread->stack_base);
	// free(cur_thread);

	cur_thread->clear=1;
	schedule();
}

// call schedule1 until ready_list is null
void wait_for_all()
{
	while(ready_list!=NULL||waiting_threads>0){
		schedule1();
	}

}

void sleep(struct lock *lock)
{
    // printf("Sleep called\n");
    struct thread *current_wait_list=(struct thread*)lock->wait_list;
	struct thread *temp=current_wait_list;
	if(current_wait_list==NULL){
		current_wait_list=cur_thread;
		current_wait_list->next=NULL;
		current_wait_list->prev=NULL;
		// printf("Added lock %p\n",cur_thread);
		lock->wait_list=(void*)current_wait_list;
		return;
	}
	while (temp->next!=NULL){
		temp=temp->next;
	}
	temp->next=cur_thread;
	cur_thread->prev=temp;
	cur_thread->next=NULL;
	// printf("Added lock %p\n",cur_thread);
	lock->wait_list=(void*)current_wait_list;
	waiting_threads+=1;
	schedule();
}

void wakeup(struct lock *lock)
{
    // printf("Wakeup called\n");
   struct thread *current_wait_list=(struct thread*)lock->wait_list;
    struct thread *toRet=current_wait_list;
    if(current_wait_list!=NULL)
	{
		current_wait_list=current_wait_list->next;
		if(current_wait_list!=NULL)
			current_wait_list->prev=NULL;	
		lock->wait_list=current_wait_list;
		push_back(toRet);
	    waiting_threads-=1;
	}	
}
