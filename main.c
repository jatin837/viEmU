#include <termios.h>
#include <stdio.h>
#include <unistd.h>

// tcgetattr (  provided by termios.h ) is used to read current attribute int a struct whose address is passed along with the attribute itself
// tcsetattr is used to write the new terminal attribute back out 
void enable_raw_mode() {
	struct termios raw;
	tcgetattr(STDIN_FILENO, &raw);
	raw.c_lflag = raw.c_lflag & ~(ECHO);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(int argc, char **argv){
	enable_raw_mode();

	char c; 
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
	return 0;
}
