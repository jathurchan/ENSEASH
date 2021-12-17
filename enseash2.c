
// Execution of the entered command and return to the prompt

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h>	
#include <unistd.h>

#define BUFF_SIZE 128

int main (int argc, char **argv) {

	char message[] = "Welcome to ENSEA Tiny Shell.\nTo quit, tap 'exit'.\n";
	char beg[BUFF_SIZE] = "enseash % ";

	ssize_t ret;
	pid_t pid;
	int status;

	char buffer[BUFF_SIZE]; 
	char command[BUFF_SIZE]; 
	
	write(STDOUT_FILENO, message, strlen(message));
	write(STDOUT_FILENO, beg, strlen(beg));
	
	while ((ret = read(STDIN_FILENO, buffer, BUFF_SIZE)) > 0) {		// ret bytes have been read...

		if ((pid=fork())==-1) {	// if an error occurs
			perror("Fork impossible");

		} else if (pid == 0) {	// child code

    		strncpy(command, buffer, ret);
    		command[ret-1] = '\0';	// eliminate '\n'
			if (execlp(command, command, (char*)NULL) == -1) {
				perror("Unknown command");
				exit(EXIT_FAILURE);
			}
			

		} else {	// father code 

			wait(&status);
			write(STDOUT_FILENO, beg, strlen(beg));

		}		
	}
	
	return 0;
}
