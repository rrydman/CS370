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

void forkPro(char **command){
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
	int printedHistory = 0;
	char* history[10]; // command history array
	int histCounter = 0;
	// fill history array
	int i;
	for (i=0; i<10; i++){
		history[i] = (char*)malloc(BUFF_SIZE);
	}
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
		if (printedHistory == 0){
			char *cwdbuf = (char *)malloc(100);
			getcwd(cwdbuf,100);
			printf("%s -> ", cwdbuf);
			fflush(stdout);
			free(cwdbuf);
		}
		char inbuffer[BUFF_SIZE];
		int bytesRead = 0;
		char *c = malloc(sizeof(char));
		char *space = malloc(sizeof(char));
		char *bs = malloc(sizeof(char));
		*bs = '\b';
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
					if (*c == 65){
						broken = 3; // up arrow
						while (bytesRead > 0){
							write(2, bs, 1);
							write(2, space, 1);
							write(2, bs, 1);
							bytesRead--;
						}
					}
					if (*c == 66) {
                                                broken = 4; // down arrow
                                                while (bytesRead > 0){
                                                        write(2, bs, 1);
                                                        write(2, space, 1);
                                                        write(2, bs, 1);
                                                        bytesRead--;
                                                }
                                        }
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
		// Create tokenized command
		char *command[ARG_COUNT] = {NULL};
		splitString(inbuffer, command);
		if (command[0] == NULL) continue; // Nothing typed - loop again
		// store buffer to history
		if (histCounter == 9){
			for(i=1;i<10;i++){
				history[i-1] = history[i];
				strcpy(history[histCounter], *command);
			}
		} else {
			strcpy(history[histCounter], *command);
			histCounter++;
		}
		if (broken > 2){
			// Up arrow
			if (broken == 3){
				printf("%s", history[0]);
				fflush(stdout);
				//forkPro(history[0]);
			}
			// Down arrow
			if (broken == 4){
			
			}
			printf("\nCould not get history to work right in time. So close, yet so far.\n");
			fflush(stdout);
			//printedHistory = 1;
		}
		else {
			// if cd command then change working directory
			if (strcmp(command[0], "cd") == 0) {
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
				forkPro(command);
			}
		printedHistory = 0;
		}
	}
	// restore input config
	tcsetattr(0, TCSANOW, &origConfig);

	return 0;
}
