/* ------------------------
CS370 Project 4 - Semaphore
Ross Rydman - (c) March 2014
---------------------------
---------------------------
What is working:
	Read input correctly
	Queue implementation
---------------------------
What isn't working:

---------------------------
What I didn't do yet:
	Thread creation
	Semaphore implementation
---------------------------
To compile: gcc filename.c -lrt
To run: ./a.out < input.txt
--------------------------- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFF_SIZE	10
#define QUEUE_SIZE  5

// Queue
typedef struct _queue{
	int count;
	int head, tail;
	int values[QUEUE_SIZE];
} queue;
void push(queue *q, int value){
	q->tail = (q->tail) % QUEUE_SIZE;
	q->values[q->tail++] = value;
	q->count++;
}
int pop(queue *q){
	q->head = (q->head) % QUEUE_SIZE;
	q->count--;
	return q->values[(q->head)++];
}
void init(queue *q){
	q->head = q->tail = q->count = 0;
}
// End Queue

typedef struct _channel{
	queue q;
	sem_t race_sem;
	sem_t sync_sem;
} channel;

typedef struct _node{
	int uid;
	int tempuid;
	int onehop;
	int phase;
	int status;  // 0 = relay ; 1 = active
} node;

// Globals
int leaderflag;


int read_channel(channel *chan){
	// wait (synchronization_semaphore);
	// wait (race_condition_semaphore);
	int value = pop(&chan->q); // int value = chan.queue.pop() 
	// post (race_condition_semaphore);
	return value;
}

void write_channel(channel *chan, int value){
	// wait(race_condition_semaphore);
	push(&chan->q, value);
	// post(race_condition_semaphore);
	// post(synchronization_semaphore);
}

void active_node_phase(){
	// print [phase number][node uid][node temp uid]
	// write temp uid
	// read one hop temp uid
	// write one hop temp uid

	// read two hop temp uid
	// if one hope temp uid == temp uid
		// this node is the leader
		// print leader: node uid
	// else if one hope temp uid > two hop temp uid && one hop temp uid > temp uid
		// this node continues to be an active node
		// temp uid = one hop temp uid
	// else
		// this node becomes a relay node
}

void relay_node_phase(){
	// read temp uid
	// write temp uid
	// read temp uid
	// write temp uid
}
int main()
{
	// Handle input
	int nodecount,len,i;
	char *linebuffer = (char *)calloc(BUFF_SIZE, sizeof(char));
	fgets(linebuffer, BUFF_SIZE, stdin);
	nodecount = atoi(linebuffer);
	node *nodeptr = malloc(nodecount * sizeof(node));
	channel *channelptr = malloc(nodecount * sizeof(channel));
	// Create nodes
	for (i = 0; i < nodecount; i++){
		fgets(linebuffer, BUFF_SIZE, stdin);
		nodeptr[i].uid = atoi(linebuffer);
		nodeptr[i].status = 1;
		nodeptr[i].phase= 1;
		sem_init(&channelptr[i].race_sem, 0, 1); // Initialize Race Semaphore for channel
		sem_init(&channelptr[i].sync_sem, 0, 0); // Initialize Sync Semaphore for channel
		init(&channelptr[i].q); // Initialize Channel's Queue
		printf("[%d][%d][%d]\n", nodeptr[i].phase, nodeptr[i].uid, nodeptr[i].uid); // Debugging to display input
	}

	free(nodeptr);
	free(channelptr);
	return 0;
}

