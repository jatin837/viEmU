#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// tcgetattr (  provided by termios.h ) is used to read current attribute int a struct whose address is passed along with the attribute itself
// tcsetattr is used to write the new terminal attribute back out 

struct termios orig_termios;

void disable_raw_mode(void);

void die(const char *s) {
	perror(s);
	exit(1);
}

void enable_raw_mode() {
	int status = tcgetattr(STDIN_FILENO, &orig_termios);
	if (status == -1)
		die("tcgetattr failed");
	atexit(disable_raw_mode);

	struct termios raw = orig_termios;
	raw.c_lflag = raw.c_lflag & ~(ECHO | ICANON | ISIG | IEXTEN);
	raw.c_iflag = raw.c_iflag & ~(BRKINT | ICRNL | INPCK |IXON | ISTRIP);
	raw.c_oflag = raw.c_oflag & ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw))
		die("tcsetattr");
}

void disable_raw_mode() {
	int status = tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
	if (status  == -1)
		die("tcsetatttr failed");
}

int main(int argc, char **argv){
	enable_raw_mode();
	
	while (1) {
		char c = '\0';
		if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
			die("READ");
		if (iscntrl(c)) 
			printf("%d\r\n", c);
		else 
			printf("%d => %c \r\n", c, c);
		if (c == 'q') break;
	}
	return 0;
}
