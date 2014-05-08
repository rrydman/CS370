/*
----------------------------------------------------------------------
CS370 Project 7 - Banker's Algorithm
Ross Rydman - (c) May 2014
----------------------------------------------------------------------
To compile: gcc project7a.c
To run: ./a.out < input.txt
----------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE	100

// Globals
typedef enum { false, true } bool;
int numProcesses; // n = rows
int numResources; // m = cols

// Helper function to create 2d arrays
int** make_int_array(int n, int m) {
	int** theArray;
	theArray = (int**)malloc(n * sizeof(int*));
	int i;
	for (i = 0; i < n; i++)
		theArray[i] = (int*)malloc(m * sizeof(int));
	return theArray;
}

// Helper function to free 2d array memory
void destroy_int_array(int** theArray, int n) {
	int i;
	for (i = 0; i < n; i++) {
		free(theArray[i]);
	}
	free(theArray);
}

int main() {
	// Handle input from redirection
	char *linebuffer = (char *)calloc(BUFF_SIZE, sizeof(char));
	fgets(linebuffer, BUFF_SIZE, stdin);
	numProcesses = atoi(linebuffer);
	fgets(linebuffer, BUFF_SIZE, stdin);
	numResources = atoi(linebuffer);
	// existing[numResources]
	int** existing = (int**)malloc(numResources * sizeof(int*));
	// fill existing[] array from input
		// todo

	// Available[numResources] (initially same as Available)
	int** available = (int**)malloc(numResources * sizeof(int*));
	// copy existing[] to available[]
		// todo

	// allocation[][]
	int** allocation = make_int_array(numProcesses, numResources);
	// fill allocation[] array from input
		// todo

	// max[][]
	int** max = make_int_array(numProcesses, numResources);
	// fill max[] array from input
		// todo

	// Need[][]
	int** need = make_int_array(numProcesses, numResources);

	// Compute need

	// Output
	printf("Bankers Algorithm: \n\n");
	printf("Total Existing Resources: \n");
	// for numResources in Existing[]
		// printf("%d", Existing[i]);

	printf("Allocation Table: \n");
	// for rows in allocation[i][]
		// for cols in allocation[][j]
			// printf("%d", allocation[i][j]);
		// printf("\n");

	printf("Max Table: \n");
	// for rows in Max[i][]
		// for cols in Max[][j]
			// printf("%d", Max[i][j]);
		// printf("\n");

	printf("Total Existing Resources: \n");
	// for numResources in available[]
		// printf("%d", available[i]);

	printf("Need Table: \n");
	// for rows in Need[i][]
		// for cols in Need[][j]
			// printf("%d", Need[i][j]);
		// printf("\n");

	// satisfy processes
	// print state


	// Cleanup memory
	free(existing);
	free(available);
	destroy_int_array(allocation, numProcesses);
	destroy_int_array(max, numProcesses);
	destroy_int_array(need, numProcesses);
	return 0;
}