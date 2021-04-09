#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// tcgetattr (  provided by termios.h ) is used to read current attribute int a struct whose address is passed along with the attribute itself
// tcsetattr is used to write the new terminal attribute back out 

/*
	Defining a bitmask CTRL_KEY which returns the bitwise AND of a key with ctrl(31 decimal)
*/
#define CTRL_KEY(k) ((k) & 0x1f)
#define REFRESH_SCR "\x1b[2J"
#define REPOSITION_CURSOR "\x1b[H"

// 0x1f in binary --> 00011111


struct termios orig_termios;

void editor_refresh_screen() {
	write(STDOUT_FILENO, REFRESH_SCR, 4);
	write(STDOUT_FILENO, REPOSITION_CURSOR, 3);
}
void disable_raw_mode(void);

void die(const char *s) {
	editor_refresh_screen();
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
char editor_read_key() {
	int nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1){
		if (nread == -1 && errno != EAGAIN)
			die("read");
	}
	return c;

}

void editor_process_keypress() {
	char c = editor_read_key();

	switch (c) {
		case CTRL_KEY('q'):
			editor_refresh_screen();	
			exit(0);
			break;
	}
}

int main(int argc, char **argv){
	enable_raw_mode();
	editor_refresh_screen();
	while (1)
		editor_process_keypress();
	return 0;
}
