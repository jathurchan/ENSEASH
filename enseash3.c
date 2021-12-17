
// Manage exit command and <ctrl>+d

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h>	
#include <unistd.h>

#define BUFF_SIZE 128

int main (int argc, char **argv) {

	char message[BUFF_SIZE] = "Welcome to ENSEA Tiny Shell.\nTo quit, tap 'exit'.\n";
	char beg[BUFF_SIZE] = "enseash % ";
	char bye[BUFF_SIZE] = "Bye bye...\n";
	char bye2[BUFF_SIZE] = "\nBye bye...\n";

	ssize_t ret;
	pid_t pid;
	int status;

	char buffer[BUFF_SIZE]; 
	char command[BUFF_SIZE];

	// Welcome message
	
	write(STDOUT_FILENO, message, strlen(message));
	write(STDOUT_FILENO, beg, strlen(beg));
	
	while ((ret = read(STDIN_FILENO, buffer, BUFF_SIZE)) > 0) {		// ret bytes have been read...

		// Get the command without '\n'
		strncpy(command, buffer, ret);
    	command[ret-1] = '\0';

		// Exit with the command exit 
		if ((strncmp(command, "exit", strlen("exit"))==0)) {
			write(STDOUT_FILENO, bye, strlen(bye));
			exit(EXIT_SUCCESS);
		}

		if ((pid=fork())==-1) {	// if an error occurs
			perror("Fork impossible");

		} else if (pid == 0) {	// child code
			if (execlp(command, command, (char*)NULL) == -1) {
				perror("Unknown command");
				exit(EXIT_FAILURE);
			}
		} else {	// father code 

			wait(&status);
			write(STDOUT_FILENO, beg, strlen(beg));

		}
	
	}
	// Exit with <ctrl>+D (ret = 0)
	if (ret == 0) {
		write(STDOUT_FILENO, bye2, strlen(bye2));
		exit(EXIT_SUCCESS);
	}
	return 0;

}
