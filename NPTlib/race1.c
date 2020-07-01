#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "thread.h"

#define MAX_THREADS 1024

int counter = 0;

static int acquire(struct lock *lock)
{
	// printf("lock->val=%d\n",lock->val);
	while (lock->val == 0) {
		// printf("sleep called\n");
		sleep(lock);
	}
	lock->val = 0;
}

static int release(struct lock *lock)
{
	wakeup(lock);
	lock->val = 1;
}

void foo(void *ptr)
{
	struct lock *l = (struct lock*)ptr;
	int val;

	acquire((struct lock*) ptr);
	val = counter;
	// printf("aquire called\n");
	val++;
	thread_yield();
	counter = val;	
	// printf("release called\n");
	release((struct lock*) ptr);
	thread_exit();

}

void bar(void *ptr)
{
	struct lock *l = (struct lock*)ptr;
	int val;

	acquire((struct lock*) ptr);
	val = counter;
	val++;
	counter = val;
	release((struct lock*) ptr);

	thread_exit();
}

int main(int argc, char *argv[])
{
	struct lock l;
	l.val = 1;
	l.wait_list = NULL;

	create_thread(foo, &l);
	create_thread(bar, &l);
	wait_for_all();

	assert(counter == 2);
	printf("main thread exiting.\n");

	return 0;
}
