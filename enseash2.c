// Execution of the entered command and return to the prompt

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h>	
#include <unistd.h>

//Welcome message
#define BUFF_SIZE 128
#define MESSAGE "Welcome to ENSEA Tiny Shell.\nTo quit, tap 'exit'.\n"
#define BEG "enseash % "

int main (int argc, char **argv) {
	
	write(STDOUT_FILENO, MESSAGE, strlen(MESSAGE));
	write(STDOUT_FILENO, BEG, strlen(BEG));

	// Fundamentals
	ssize_t ret;
	int status;

	char buffer[BUFF_SIZE]; 
	char command[BUFF_SIZE]; 
	
	while ((ret = read(STDIN_FILENO, buffer, BUFF_SIZE)) > 0) {		// ret bytes have been read...

		pid_t pid;
		
		if ((pid=fork())==-1) {	// if an error occurs
			perror("Fork impossible");

		} else if (pid == 0) {	// child code

    		strncpy(command, buffer, ret);
    		command[ret-1] = '\0';	// eliminates '\n'
    		
			if (execlp(command, command, (char*)NULL) == -1) {
				perror("Unknown command");
				exit(EXIT_FAILURE);
			}
			

		} else {	// father code 

			wait(&status);
			write(STDOUT_FILENO, BEG, strlen(BEG));	
	
		}		
	}
	
	return 0;
}
