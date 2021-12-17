#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h>	
#include <unistd.h>

#define BUFF_SIZE 128

void getcode(char *charcode, char *statuschar, char *prompt, int statuscode, char *end);

int main (int argc, char **argv) {

	char message[200] = "Welcome to ENSEA Tiny Shell.\nTo quit, tap 'exit'.\n";
	ssize_t ret;
	pid_t pid; 
	char buffer[BUFF_SIZE]; 
	char command[BUFF_SIZE]; 
	char beg[BUFF_SIZE] = "enseash % ";
	char bye[BUFF_SIZE] = "Bye bye...\n";
	char prompt[BUFF_SIZE] = "";
	char strexit[BUFF_SIZE] = "[exit:";
	char sign[BUFF_SIZE] = "[sign:";
	char end[BUFF_SIZE] = "] % ";
	int status;
	char statussign[BUFF_SIZE];
	char statusexit[BUFF_SIZE];

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
				waitpid(pid, &status, WCONTINUED);
				strcat(prompt,"enseash ");
				
				if (WIFEXITED(status)) {
					getcode (strexit, statusexit, prompt, WEXITSTATUS(status), end);
				} else if (WIFEXITED(status)) {
					getcode (sign, statussign, prompt, WTERMSIG(status), end);
				}
				
				write(STDOUT_FILENO, prompt, strlen(prompt));
				prompt[0] ='\0';
			}	
		}	
	}
	
	return 0;

}

void getcode(char *charcode, char *statuschar, char *prompt, int statuscode, char *end) {
	strcat(prompt, charcode);
	sprintf(statuschar, "%d", statuscode);
	strcat(prompt, statuschar);
	strcat(prompt, end); 
}

