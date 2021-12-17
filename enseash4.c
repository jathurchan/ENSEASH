
// Added: Display the return code (or signal) of the previous code
// Improved: Organization with several functions

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h>	
#include <unistd.h>

#define BUFF_SIZE 128

void execute(char *command);
void getcode(char *charcode, char *statuschar, char *prompt, int statuscode, char *end);
void display_return(pid_t pid, int status, char *s_exit, char *s_sign, char *prompt, char *status_exit, char *status_sign, char *s_end);


int main (int argc, char **argv) {

	// Welcome Message
	char message[] = "Welcome to ENSEA Tiny Shell.\nTo quit, tap 'exit'.\n";
	char beg[BUFF_SIZE] = "enseash % ";
	char bye[BUFF_SIZE] = "Bye bye...\n";
	char bye2[BUFF_SIZE] = "\nBye bye...\n";

	write(STDOUT_FILENO, message, strlen(message));
	write(STDOUT_FILENO, beg, strlen(beg));

	// Fundamentals

	ssize_t ret;
	pid_t pid;
	int status;

	char buffer[BUFF_SIZE];
	char command[BUFF_SIZE];

	// To display return code / signal

	char prompt[BUFF_SIZE] = "";
	char s_exit[BUFF_SIZE] = "[exit:";
	char s_sign[BUFF_SIZE] = "[sign:";
	char s_end[BUFF_SIZE] = "] % ";
	
	char status_sign[BUFF_SIZE];
	char status_exit[BUFF_SIZE];

	
	while ((ret = read(STDIN_FILENO, buffer, BUFF_SIZE)) > 0) {	// ret bytes > 0 have been read...

		// Get the command without '\n'
		strncpy(command, buffer, ret);
    	command[ret-1] = '\0';

		// Exit with the command exit 
		if ((strncmp(command, "exit", strlen("exit"))==0)) {
			write(STDOUT_FILENO, bye, strlen(bye));
			exit(EXIT_SUCCESS);
		}

		if ((pid=fork())==-1) {	// if an error occurs
			write (STDOUT_FILENO, "exit", strlen("exit"));
			perror("Fork impossible");

		} else if (pid == 0) {	// child code
			execute(command);
		} else {	// father code 
			display_return(pid, status, s_exit, s_sign, prompt, status_exit, status_sign, s_end);
		}
	}
	// Exit with <ctrl>+D (ret = 0)
	if (ret == 0) {
		write(STDOUT_FILENO, bye2, strlen(bye2));
		exit(EXIT_SUCCESS);
	}
	return 0;

}

void execute(char *command) {
	if (execlp(command, command, (char*)NULL) == -1) {
		perror("Unknown command");
		exit(EXIT_FAILURE);
	}
}

void getcode(char *charcode, char *statuschar, char *prompt, int statuscode, char *end) {
	strcat(prompt, charcode);
	sprintf(statuschar, "%d", statuscode);
	strcat(prompt, statuschar);
	strcat(prompt, end); 
}

void display_return(pid_t pid, int status, char *s_exit, char *s_sign, char *prompt, char *status_exit, char *status_sign, char *s_end){
	waitpid(pid, &status, WCONTINUED);
	strcat(prompt,"enseash ");
		
	if (WIFEXITED(status)) {
		getcode (s_exit, status_exit, prompt, WEXITSTATUS(status), s_end);
	} else if (WIFEXITED(status)) {
		getcode (s_sign, status_sign, prompt, WTERMSIG(status), s_end);
	}
	
	write(STDOUT_FILENO, prompt, strlen(prompt));
	prompt[0] ='\0';
}