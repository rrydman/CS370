/* ------------------------
CS370 Project 4 - Semaphore
Ross Rydman - (c) March 2014
---------------------------
---------------------------
What is working:

---------------------------
What isn't working:

---------------------------
What I didn't do:

---------------------------
To compile: gcc filename.cpp
To run: ./a.out < input.txt
--------------------------- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFF_SIZE	256

void read_input(FILE *in){
	if (in){
		// Need to handle input differently from C++ - http://www.cs.utah.edu/dept/old/texinfo/glibc-manual-0.02/library_11.html
		// Input is one int per line
			// first line is number of nodes (lines to read)
			// subsequent lines are node uid's
		// Loops for each line in redirected input
		while (in) {
			//string line = "";
			//getline(in, line);
			//if (line.compare("***") == 0){
				break;
			//}
			//else {
				// Handle input
				//int tmp;
				//stringstream ss;
				//ss.str(line);
				// Read first word in line
				//ss >> tmp;
				// do something with temp
				// Read rest of line
			//}
		}
		//in.clear();
	}
	//return in;
}

int read_channel( /* channel */){
	// wait (synchronization_semaphore);
	// wait (race_condition_semaphore);
	int value = 0; // remove value from the queue
	// post (race_condition_semaphore);
	return value;
}

void write_channel( /* channel */){
	// wait(race_condition_semaphore);
	// put value on the queue
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
	char *inbuffer = (char *)calloc(BUFF_SIZE, sizeof(char));
	fgets(inbuffer, BUFF_SIZE, stdin);


	return 0;
}

