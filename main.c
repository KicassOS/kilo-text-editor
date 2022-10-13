#include <ctype.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

struct termios orig_termios;

void	disableRawMode()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void	enableRawMode()
{
	struct termios raw;

	tcgetattr(STDERR_FILENO, &orig_termios);
	atexit(disableRawMode);
	raw = orig_termios;
	raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | IXON | ICRNL);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  char c;

  enableRawMode();

  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
  {
	if (iscntrl(c))
		printf("%d\r\n", c);
	else
		printf("%d ('%c')\r\n", c, c);
  }
  return 0;
}