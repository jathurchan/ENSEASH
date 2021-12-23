// Display a welcome message, followed by a simple prompt

#include <string.h>
#include <unistd.h>

//Welcome message
#define BUFF_SIZE 128
#define MESSAGE "Welcome to ENSEA Tiny Shell.\nTo quit, tap 'exit'.\n"
#define BEG "enseash % "

int main (int argc, char **argv[]) {

	write(STDOUT_FILENO, MESSAGE, strlen(MESSAGE));
	write(STDOUT_FILENO, BEG, strlen(BEG));
	
	return 0;
}
