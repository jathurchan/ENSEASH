#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h>	
#include <unistd.h>

#define BUFF_SIZE 128

int main (int argc, char **argv) {

	char message[200] = "Welcome to ENSEA Tiny Shell.\nTo quit, tap 'exit'.\n";
	ssize_t ret;
	pid_t pid; 
	char buffer[BUFF_SIZE]; 
	char command[BUFF_SIZE]; 
	char beg[BUFF_SIZE] = "enseash % ";
	char bye[BUFF_SIZE] = "Bye bye...\n";

	write(STDOUT_FILENO, message, strlen(message));
	write(STDOUT_FILENO, beg, strlen(beg));
	
	while ((ret = read(STDIN_FILENO, buffer, BUFF_SIZE)) > 0) {
	
		strncpy(command, buffer, ret);
    		command[ret-1] = '\0';
    		
		if (strncmp(command, "exit", strlen("exit"))==0) {
			write(STDOUT_FILENO, bye, strlen(bye));
			return 0;
		} else {
			if ((pid=fork())==-1) {
				write (STDOUT_FILENO, "exit", strlen("exit"));
				exit(EXIT_FAILURE);
			} else if (pid == 0) {		
				execlp(command, command, NULL);
			} else {
				wait(NULL);
				write(STDOUT_FILENO, beg, strlen(beg));
			}	
		}	
	}
	
	return 0;

}
