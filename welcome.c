#include <string.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
	char message[200] = "Welcome to ENSEA Tiny Shell.\nTo quit, tap 'exit'.\nenseash % \n";
	#include <unistd.h>
	write(STDOUT_FILENO, message, strlen(message));
	
	return 0;
}
