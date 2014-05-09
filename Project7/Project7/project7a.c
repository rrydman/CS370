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

// Helper function to tokenize a string
void splitString(char *str, char **splitstr)
{
	char *p;
	int i = 0;
	p = strtok(str, " \n");
	while (p != NULL)
	{
		splitstr[i] = malloc(strlen(p) + 1);
		if (splitstr[i]){
			strcpy(splitstr[i], p);
		}
		i++;
		p = strtok(NULL, " \n");
	}
}

int main() {
	/* ------------ Start ------------ */
	/*  Handle input from redirection  */
	/* ------------------------------- */
	char *linebuffer = (char *)calloc(BUFF_SIZE, sizeof(char));
	char *line[BUFF_SIZE] = { NULL };
	int i,j;
	fgets(linebuffer, BUFF_SIZE, stdin); // numProcesses
	numProcesses = atoi(linebuffer);
	fgets(linebuffer, BUFF_SIZE, stdin); // numResources
	numResources = atoi(linebuffer);
	fgets(linebuffer, BUFF_SIZE, stdin); // blank line
	// Create and fill existing[] and available[] arrays
	int** existing = (int**)malloc(numResources * sizeof(int*));
	int** available = (int**)malloc(numResources * sizeof(int*));
	fgets(linebuffer, BUFF_SIZE, stdin);
	splitString(linebuffer, line);
	for (i = 0; i < numResources; i++) existing[i] = atoi(line[i]);
	for (i = 0; i < numResources; i++) available[i] = atoi(line[i]);
	fgets(linebuffer, BUFF_SIZE, stdin); // blank line
	// Create and fill allocation[][]
	int** allocation = make_int_array(numProcesses, numResources);
	for (i = 0; i < numProcesses; i++) { // Rows
		fgets(linebuffer, BUFF_SIZE, stdin);
		splitString(linebuffer, line);
		for (j = 0; j < numResources; j++){ // Cols
			allocation[i][j] = atoi(line[j]);
		}
	}
	fgets(linebuffer, BUFF_SIZE, stdin); // blank line
	// Create and fill max[][]
	int** max = make_int_array(numProcesses, numResources);
	for (i = 0; i < numProcesses; i++) { // Rows
		fgets(linebuffer, BUFF_SIZE, stdin);
		splitString(linebuffer, line);
		for (j = 0; j < numResources; j++){ // Cols
			max[i][j] = atoi(line[j]);
		}
	}
	/* ------------- End ------------- */
	/*            Input done           */
	/* ------------------------------- */


	// create and fill need[][]
	int** need = make_int_array(numProcesses, numResources);

	// Compute need

	// Output
	printf("Bankers Algorithm: \n\n");
	printf("Total Existing Resources: \n");
	for (i = 0; i < numResources; i++) printf("%d ", existing[i]);
	printf("\n\n");

	printf("Allocation Table: \n");
	for (i = 0; i < numProcesses; i++) { // Rows
		for (j = 0; j < numResources; j++){ // Cols
			printf("%d ", allocation[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("Max Table: \n");
	for (i = 0; i < numProcesses; i++) { // Rows
		for (j = 0; j < numResources; j++){ // Cols
			printf("%d ", max[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("Available Resources: \n");
	for (i = 0; i < numResources; i++) printf("%d ", available[i]);
	printf("\n\n");

	printf("Need Table: \n");
	for (i = 0; i < numProcesses; i++) { // Rows
		for (j = 0; j < numResources; j++){ // Cols
			printf("%d ", need[i][j]);
		}
		printf("\n");
	}
	printf("\n");

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