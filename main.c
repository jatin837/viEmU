#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){
	char c;
	read(STDIN_FILENO, &c, 2);
	return 0;
}
