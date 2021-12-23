// Added: Display the return code (or signal) of the previous code
// Improved: Organization with several functions

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h>	
#include <unistd.h>

//Welcome message
#define BUFF_SIZE 128
#define MESSAGE "Welcome to ENSEA Tiny Shell.\nTo quit, tap 'exit'.\n"
#define BEG "enseash % "
#define BYE "Bye bye...\n"
#define BYEBYE "\nBye bye...\n"

//To display return code / signal and execution time of previous code
#define S_EXIT "[exit:"
#define S_SIGN "[sign:"
#define S_END "] % "

void execute(char *command);
void getcode(char *charcode, int statuscode);
void display_return(pid_t pid);


int main (int argc, char **argv) {

	write(STDOUT_FILENO, MESSAGE, strlen(MESSAGE));
	write(STDOUT_FILENO, BEG, strlen(BEG));

	// Fundamentals
	ssize_t ret;

	char buffer[BUFF_SIZE];
	char command[BUFF_SIZE];
	
	while ((ret = read(STDIN_FILENO, buffer, BUFF_SIZE)) > 0) {	// ret bytes > 0 have been read...

		// Get the command without '\n'
		strncpy(command, buffer, ret);
    		command[ret-1] = '\0';

		// Exit with the command exit 
		if ((strncmp(command, "exit", strlen("exit"))==0)) {
			write(STDOUT_FILENO, BYE, strlen(BYE));
			exit(EXIT_SUCCESS);
		}

		pid_t pid;		
		
		if ((pid=fork())==-1) {	// if an error occurs
			write (STDOUT_FILENO, "exit", strlen("exit"));
			perror("Fork impossible");

		} else if (pid == 0) {	// child code
			execute(command);
		} else {	// father code 
			display_return(pid);
		}
	}
	// Exit with <ctrl>+D (ret = 0)
	if (ret == 0) {
		write(STDOUT_FILENO, BYEBYE, strlen(BYEBYE));
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

void getcode(char *charcode, int statuscode) {

	char prompt[BUFF_SIZE] = "";
	
	strcat(prompt,"enseash ");
	strcat(prompt, charcode);
	
	char statuschar[BUFF_SIZE];
	
	sprintf(statuschar, "%d", statuscode);
	strcat(prompt, statuschar);
	
	strcat(prompt, S_END); 

	write(STDOUT_FILENO, prompt, strlen(prompt));
	prompt[0] ='\0';
}

void display_return(pid_t pid){

	int status;
	waitpid(pid, &status, WCONTINUED);
		
	if (WIFEXITED(status)) {
		getcode (S_EXIT, WEXITSTATUS(status));
	} else if (WIFSIGNALED(status)) {
		getcode (S_SIGN, WTERMSIG(status));
	}
}
