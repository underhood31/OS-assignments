#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>

// you can change the prototype of existing
// functions, add new routines and global variables
// cheatmode, car1, car2, report are different threads
// they communicate with each other via shared memory

typedef struct instructions
{
	struct instructions *next;
	int position;
}instruction_set;

//-----------------Variables for interaction-----------//
int car1_pos;
int car2_pos;
instruction_set *car1_instructions;
instruction_set *car2_instructions;
int cheat;
pthread_t cheat_thread;
pthread_mutex_t lock_car1;
pthread_mutex_t lock_car2;
pthread_barrier_t print_barrier;
//----------------------------------------------------//

// the cheatmode thread
void *cheatmode(void *arg)
{	
	// printf("<*>Cheatmode Started\n");
	long lastcheat=0;
	while(car1_pos<=100 && car2_pos<=100){
			// printf("Do you want to cheat(Y/n): \n");
			char c;
			int s1 = scanf("%c",&c);
			if (c=='Y' || c=='y')
			{
				cheat=1;
				printf("Enter 1 to relocate car 1 and 2 to relocate car 2: ");
				int n;
				int s2 = scanf("%d",&n);
				if(n==1){
					printf("Enter the new position: ");
					int pos;
					int s3 = scanf("%d",&pos);
					instruction_set *temp;
					temp = (instruction_set*)malloc(sizeof(instruction_set));
					temp->next = car1_instructions;
					temp->position = pos;
					pthread_mutex_lock(&lock_car1);
					car1_instructions=temp;
					pthread_mutex_unlock(&lock_car1);
				}else if(n==2){
					printf("Enter the new position: ");
					int pos;
					int s3 = scanf("%d",&pos);
					instruction_set *temp;
					temp = (instruction_set*)malloc(sizeof(instruction_set));
					temp->next = car2_instructions;
					temp->position = pos;
					pthread_mutex_lock(&lock_car2);
					car2_instructions=temp;
					pthread_mutex_unlock(&lock_car2);
				}
			}
			cheat=0;

	}


	return NULL;
}

// the car1 thread
void *car1(void *arg)
{
	// printf("<*>car1 Started\n");

	car1_pos=1;
	while(car1_pos<=100 && car2_pos<=100){
		if(!cheat){
			int r;
			r=rand()%10;
			car1_pos+=r;
			pthread_mutex_lock(&lock_car1);
			//Reading cheatmode stack
			if(car1_instructions!=NULL){
				car1_pos=car1_instructions->position;
				car1_instructions=car1_instructions->next;
			}
			pthread_mutex_unlock(&lock_car1);
		}
		pthread_barrier_wait(&print_barrier);
		sleep(1);

	}
	return NULL;
}

// the car2 thread
void *car2(void *arg)
{
	// printf("<*>car2 Started\n");
	car2_pos=1;

	while(car1_pos<=100 && car2_pos<=100){
		if(!cheat){
			int r;
			r=rand()%10;
			car2_pos+=r;
			
			pthread_mutex_lock(&lock_car2);
			//Reading cheatmode stack
			if(car2_instructions!=NULL){
				car2_pos=car2_instructions->position;
				car2_instructions=car2_instructions->next;
			}
			pthread_mutex_unlock(&lock_car2);
		}
		pthread_barrier_wait(&print_barrier);
		sleep(1);
	}

	
	return NULL;
	
}

// the report thread
void *report(void *arg)
{

	int last_report=0;
	while(car1_pos<=100 && car2_pos<=100 ){
		pthread_barrier_wait(&print_barrier);
		if(!cheat){
			printf("Do you want to cheat(Y/n): \n");
			printf("Car1 : %d\n", car1_pos);
			printf("Car2 : %d\n", car2_pos);
			last_report=time(0);
		}
		
	}

	if(car1_pos>=100 && car2_pos<100){
		printf("WINNER is Car 1!\n");
	}
	else if(car2_pos>=100 && car1_pos<100){
		printf("WINNER is Car 2!\n");
	}else{
		printf("This is a TIE!!\n");
	}
	// exit(0);
	pthread_cancel(cheat_thread);

	return NULL;

}



// create all threads
int main()
{
	pthread_t cheatmode_id, car1_id, car2_id, report_id;	
	pthread_barrier_init(&print_barrier, NULL, 3);
	//printf("~~~~~Creating threads~~~~~\n");
	pthread_create(&cheatmode_id,NULL, cheatmode, NULL);
	cheat_thread=cheatmode_id;
	pthread_create(&car1_id,NULL, car1, NULL);
	pthread_create(&car2_id,NULL, car2, NULL);
	pthread_create(&report_id,NULL, report, NULL);

	pthread_join(cheatmode_id,NULL);
	pthread_join(car1_id,NULL);
	pthread_join(car2_id,NULL);
	pthread_join(report_id,NULL);
	return 0;
}
