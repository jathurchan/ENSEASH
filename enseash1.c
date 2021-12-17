
// Display a welcome message, followed by a simple prompt

#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 128

int main (int argc, char *argv[]) {
	
	char message[] = "Welcome to ENSEA Tiny Shell.\nTo quit, write 'exit'.\n";
	char beg[BUFF_SIZE] = "enseash % ";

	write(STDOUT_FILENO, message, strlen(message));
	write(STDOUT_FILENO, beg, strlen(beg));
	
	return 0;
}
