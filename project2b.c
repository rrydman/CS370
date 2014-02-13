/* CS370 - Ross Rydman - Project 2 */

/*
	Full submission
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
	struct termios origConfig;
	
	// Get original config
	tcgetattr(0, &origConfig);
	// Copy config
	struct termios newConfig = origConfig;
	// Adjust config
	newConfig.c_lflag &= ~(ICANON|ECHO);
	newConfig.c_cc[VERASE] = 0;
	tcflush(0, TCIFLUSH);
	// Set the new config
	tcsetattr(0, TCSANOW, &newConfig);

	while (exited != 1) {
		broken = 0;
		// print cwd shell prompt
		char *cwdbuf = (char *)malloc(100);
		getcwd(cwdbuf,100);
		printf("%s -> ", cwdbuf);
		fflush(stdout);
		free(cwdbuf);
		char inbuffer[BUFF_SIZE];
		int bytesRead = 0;
		char *c = malloc(sizeof(char));
		char *space = malloc(sizeof(char));
		*space = ' ';
		while (broken == 0){
			read(0, c, 1); // Read one character
			if (*c == 8 | *c == 127) { // Handle bs and del keys correctly
				if (bytesRead == 0) continue; // Dont touch prompt
				write(2, c, 1);
				write(2, space, 1);
				write(2, c, 1);
			} else if (*c == 27){ // Handle updownleftright correctly
				read(0, c, 1);
				if (*c == 91){
					read(0,c,1);
					if (*c == 65) broken = 3; // up arrow
					if (*c == 66) broken = 4; // down arrow
					if (*c == 67) continue; // left arrow
					if (*c == 68) continue; // right arrow
					continue;
				}
			} else {
				write(2, c, 1);
			}	
			// Break on line feed
			if (*c == '\n'){
				broken = 2;
			}
			// Write or delete char to line buffer
			if (*c == 8 | *c == 127){
				--bytesRead;
				inbuffer[bytesRead] = '\0';
			} else {
				inbuffer[bytesRead] = *c;
				bytesRead++;
				inbuffer[bytesRead] = '\0';
			}
		}
		// store buffer to history 
			// todo
		if (broken > 2){
			// clear input
			printf("Up/down arrow detected. History not implemnted fully yet.\n");
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
				int error = chdir(command[1]);
				if (error == -1)
					perror("Error changing directory.\n");
			}
			// if exit command, break
			else if (strcmp(command[0], "exit") == 0) {
				exited = 1;
				continue;
			}
			// if not cd or exit, execute command
			else {
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
	tcsetattr(0, TCSANOW, &origConfig);

	return 0;
}
