#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// tcgetattr (  provided by termios.h ) is used to read current attribute int a struct whose address is passed along with the attribute itself
// tcsetattr is used to write the new terminal attribute back out 

struct termios orig_termios;

void disable_raw_mode(void);

void enable_raw_mode() {
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disable_raw_mode);
	struct termios raw = orig_termios;
	raw.c_lflag = raw.c_lflag & ~(ECHO | ICANON | ISIG | IEXTEN);
	raw.c_iflag = raw.c_iflag & ~(IXON | ICRNL);
	raw.c_oflag = raw.c_oflag & ~(OPOST);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

int main(int argc, char **argv){
	enable_raw_mode();

	char c; 
	while (read(STDIN_FILENO, &c, 1) >= 1 && c != 'q') {
		if (iscntrl(c)) {
			printf("%c\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
	}
	return 0;
}
