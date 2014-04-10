/* 
----------------------------------------------------
CS370 Project 5 - Dining Philosophers
Ross Rydman - (c) April 2014
----------------------------------------------------
Experimenting with Yoda conditions so I apologize 
for the confusion if you have not seen them before.
More info: wikipedia.org/wiki/Yoda_Conditions
----------------------------------------------------
To compile: gcc filename.c -pthread
To run: ./a.out <0|1>
----------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>

// Constants
#define PHIL_CNT 5				// How many Philosophers to simulate
#define ROOM_SIZE 4				// Max room size when simulating non-deadlock solution
#define EATROUND_MAX 5			// Max rounds of eating per philosopher

// Globals
sem_t fork_sem[PHIL_CNT];		// Semaphore for fork mutex
sem_t room_sem;					// Semaphore to implement PHIL-1 roomsize restriction
sem_t cnt_sem;					// Semaphore for providing mutex to global counters
int done_executing = 0;
int execute_mode = 0;			// 0 is solution simulation, 1 is deadlock simulation
pthread_t threads[PHIL_CNT];	// Threads array
int satisfied_phil_cnt = 0;		// Counter of how many threads (philosophers) have completed eating
int heldforks_cnt = 0;			// Counter of how many left forks are being held

// Main logic function. Handles both simulation types.
void *philosophize(void *threadparam){
	int thread = (intptr_t) threadparam;			// Cast void pointer
	int round_cnt = EATROUND_MAX;

	while (0 != round_cnt){
		printf("Philosopher %d is done thinking and is now ready to eat. \n", thread);
		// Use room sem if in solution simulation
		if (0 == execute_mode) sem_wait(&room_sem);
		// Pickup a left fork
		sem_wait(&fork_sem[thread]);
		printf("Philosopher %d: taking left fork %d \n", thread, thread);
		if (1 == execute_mode) {
			sem_wait(&cnt_sem);
			heldforks_cnt++;
			sem_post(&cnt_sem);
		}
		// Pickup a right fork
		if (PHIL_CNT -1 == thread){
			sem_wait(&fork_sem[0]);
			printf("Philosopher %d: taking right fork 0 \n", thread);
		}
		else{
			sem_wait(&fork_sem[thread + 1]);
			printf("Philosopher %d: taking right fork %d \n", thread, thread + 1);
		}
		printf("Philosopher %d: eating. \n", thread);
		// Release left fork
		sem_post(&fork_sem[thread]);
		printf("Philosopher %d: putting down left fork %d \n", thread, thread);
		if (1 == execute_mode) {
			sem_wait(&cnt_sem);
			heldforks_cnt--;
			sem_post(&cnt_sem);
		}
		// Release right fork
		if (PHIL_CNT - 1 == thread){
			sem_post(&fork_sem[0]);
			printf("Philosopher %d: putting down right fork 0 \n", thread);
		}
		else{
			sem_post(&fork_sem[thread + 1]);
			printf("Philosopher %d: putting down right fork %d \n", thread, thread + 1);
		}
		// Post room sem if in solution simulation
		if (0 == execute_mode) sem_post(&room_sem);
		round_cnt--;
	}
	printf("Philosopher %d is done eating \n", thread);
	sem_wait(&cnt_sem);
	satisfied_phil_cnt++;
	sem_post(&cnt_sem);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	// Init semaphores
	sem_init(&room_sem, 0, PHIL_CNT -2); 
	sem_init(&cnt_sem, 0, 1);
	int i;
	for (i = 0; i< PHIL_CNT; i++) sem_init(&fork_sem[i], 0, 1);

	// Examine arguments and fire off threads
	// Incorrect arg count
	if (2 != argc){
		printf("Usage: %s 0 for working example \n or \nUsage: %s 1 to observe deadlock example\n", argv[0], argv[0]);
		return 0;
	} 
	// Solution Simulation
	else if (0 == strcmp(argv[1], "0")){
		execute_mode = 0;
		for (i = 0; i < PHIL_CNT; i++) pthread_create(&threads[i], NULL, philosophize, (void *)(intptr_t)i);
	}
	// Deadlock Simulation
	else if (0 == strcmp(argv[1], "1")) {
		execute_mode = 1;
		for (i = 0; i < PHIL_CNT; i++) pthread_create(&threads[i], NULL, philosophize, (void *)(intptr_t)i);
	}
	// Incorrect argument passed
	else {
		printf("Usage: %s 0 for working example \n or \nUsage: %s 1 to observe deadlock example\n", argv[0], argv[0]);
		return 0;
	}

	while (0 == done_executing){
		if (satisfied_phil_cnt == PHIL_CNT && 0 == execute_mode){
			done_executing = 1;
		}	
	}
	printf("All are done eating!\n");
	return 0;
}

