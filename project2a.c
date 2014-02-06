/* CS370 - Ross Rydman - Project 2 */

/* 
Intermediate requirements:
   Print prompt while waiting for input
   Print a prompt until user exits
   Accept commands from user and execute until exit
   	If command not found - print error
*/

#include<termios.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define BUFF_SIZE	256
#define ARG_COUNT	20

void splitString(char *str, char **splitstr) 
{      
	char *p;      
	int i = 0;      
	p = strtok(str," \n");      
	while(p != NULL)      
	{                
		splitstr[i] = malloc(strlen(p) + 1);
		if (splitstr[i]){
			strcpy(splitstr[i], p);
		}
		i++;
		p = strtok (NULL, " \n");       
	} 
}

int main(void)
{
	int exited = 0;
	int broken = 0;
	//struct termios origConfig;
	
	// Get original config
	//tcgetattr(0, &origConfig);
	// Copy config
	//struct termios newConfig = origConfig;
	// Adjust config
	//newConfig.c_lflag &= ~(ICANON|ECHO);
	//newConfig.c_cc[VMIN] = 10;
	//newConfig.c_cc[VTIME] = 2;
	// Set the new config
	//tcsetattr(0, TCSANOW, &newConfig);

	while (exited != 1) {
		// print prompt
		char *cwdbuf = (char *)malloc(100);
		getcwd(cwdbuf,100);
		printf("%s -> ", cwdbuf);
		free(cwdbuf);
		// read input (break on up down and \n)
		char *inbuffer = (char *)calloc(BUFF_SIZE, sizeof(char));
		fgets(inbuffer, BUFF_SIZE, stdin);
		// if broken on up or down
		if (broken == 1){
			// clear input
				// execute appropriate command to generate corresponding msg
			// print message
		}
		else {
			// parse input into an array
			char *command[ARG_COUNT] = {NULL};
			splitString(inbuffer, command);

			// Loop again if no input provided
			if (command[0] == NULL){
				continue;
			}
			// if cd command then change working directory
			else if (strcmp(command[0], "cd") == 0) {
				printf("CD entered - need to implement!\n");
			}
			// if exit command, break
			else if (strcmp(command[0], "exit") == 0) {
				exited = 1;
				continue;
			}
			// if not cd or exit, execute command
			else {
				// execute command
				unsigned int pid = fork();
				int status;
				int errno;
				if (pid == -1){
					printf("Could not fork!");
				} else if (pid == 0){
					close(2);
					dup(1);
					int success = execvp(command[0], command);
					if (success == -1)
						printf("Command not found!\n");
					exit(errno);
				} else {
				waitpid(pid, &status, WUNTRACED);
				}		
			}

		}
	}
	// restore input config
	//tcsetattr(0, TCSANOW, &origConfig);

	return 0;
}
