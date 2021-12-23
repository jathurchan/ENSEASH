// Added: Display the execution time of the previous code
// Improved: Organization with several functions

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h>	
#include <unistd.h>
#include <errno.h>       
#include <time.h>
#include <math.h>

//Welcome message
#define BUFF_SIZE 128
#define MESSAGE "Welcome to ENSEA Tiny Shell.\nTo quit, tap 'exit'.\n"
#define BEG "enseash % "
#define BYE "Bye bye...\n"
#define BYEBYE "\nBye bye...\n"
#define EXIT "exit"

//To display return code / signal and execution time of previous code
#define S_EXIT "[exit:"
#define S_SIGN "[sign:"
#define BARRE "|"
#define S_END "] % "

void execute(char *command);
void display_prompt(char *charcode, int statuscode, double exec_time);
void display_return(pid_t pid);
double get_time(struct timespec *start, struct timespec *end);
char** splitstr(char* a_str, const char a_delim);	

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
		if ((strncmp(command, EXIT, strlen(EXIT))==0)) {
			write(STDOUT_FILENO, BYE, strlen(BYE));
			exit(EXIT_SUCCESS);
		}
		
		pid_t pid;

		if ((pid=fork())==-1) {	// if an error occurs
			write (STDOUT_FILENO, EXIT, strlen(EXIT));
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
	
	char** tokens = splitstr(command, ' ');

	if (execvp(tokens[0], &tokens[0]) == -1) {
		perror("Unknown command");
		exit(EXIT_FAILURE);
	}

	if (tokens) {
	        int i;
	        for (i = 0; *(tokens + i); i++)
	        {
	            free(*(tokens + i));
	        }

	        free(tokens);
	}
	
}

char** splitstr(char* command, const char separator) {
    char** splitted = 0;
    size_t count = 0;
    char* temp = command;
    char* last = 0;
    char delim[2] = {separator, 0};

    while (*temp) {		//counts the number of elements to be split
        if (separator == *temp) {
            count++;
            last = temp;
        }
        temp++;
    }
    
    count += last < (command + strlen(command) - 1);
    count += 2;
    
    splitted = malloc(sizeof(char*) * count);

    if (splitted) {
        size_t index  = 0;
        char* token = strtok(command, delim);

        while (token) {
            *(splitted + index++) = strdup(token);
            token = strtok(NULL, delim);
        }
        
        *(splitted + index++) = (char *) NULL;
        *(splitted + index) = 0;
    }
    return splitted;
}

void display_prompt(char *charcode, int statuscode, double exec_time) {
	
	char prompt[BUFF_SIZE] = "";
	
	strcat(prompt,"enseash ");
	strcat(prompt, charcode);
	
	char statuschar[BUFF_SIZE];
	
	sprintf(statuschar, "%d", statuscode);
	strcat(prompt, statuschar);
	strcat(prompt, BARRE);
	
	char exec[BUFF_SIZE];
	
	sprintf(exec, "%.fms", exec_time); 
	strcat(prompt, exec);
	strcat(prompt, S_END); 

	write(STDOUT_FILENO, prompt, strlen(prompt));
	prompt[0] ='\0';
}

void display_return(pid_t pid){
	
	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);
	
	int status;
	waitpid(pid, &status, WCONTINUED);
	
	clock_gettime(CLOCK_REALTIME, &end);
	double exec_time = get_time(&start, &end);
	
	if (WIFEXITED(status)) {
		display_prompt (S_EXIT, WEXITSTATUS(status), exec_time);
	} else if (WIFEXITED(status)) {
		display_prompt (S_SIGN, WTERMSIG(status), exec_time);
	}

}

double get_time(struct timespec *start, struct timespec *end) {
    	return floor((end->tv_sec - start->tv_sec) * 1e3 + (end->tv_nsec - start->tv_nsec)* 1e-6);  	
}
