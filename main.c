#include <ctype.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

#define CTRL_KEY(k) ((k) & 0x1f)

struct termios orig_termios;
struct editorConfig
{
	int screenrows;
	int screencols;
	struct termios orig_termios;
};

struct editorConfig E;

void	editorDrawRows()
{
	int	y;
	for (y = 0; y < 24; y++)
	{
		write(STDOUT_FILENO, "~\r\n", 3);
	}
}

void editorRefreshScreen() {
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	editorDrawRows();
	write(STDOUT_FILENO, "\x1b[H", 3);
}

void	die(const char *s)
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 4);
	perror(s);
	exit(1);
}

void	disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
		die("tcsetattr");
}

void	enableRawMode()
{
	struct termios raw;

	if (tcgetattr(STDERR_FILENO, &E.orig_termios) == -1)
		die("tcgetattr");
	atexit(disableRawMode);
	raw = E.orig_termios;
	raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | IXON | ICRNL);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

char	editorReadKey()
{
	int		nread;
	char	c;

	while ((nread = read(STDIN_FILENO, &c, 1) != 1))
	{
		if (nread == -1 && errno != EAGAIN)
			die("read");
	}
	return c;
}

void	editorProcessKeypress()
{
	char	c = editorReadKey();

	switch (c)
	{
		case CTRL_KEY('q'):
		write(STDOUT_FILENO, "\x1b[2J", 4);
		write(STDOUT_FILENO, "\x1b[H", 4);
			exit(0);
			break;
	}
}

int	getWindowSize(int *rows, int *cols)
{
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
		return -1;
	else
	{
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
}

void	initEditor()
{
	if (getWindowSize(&E.screenrows, &E.screencols) == -1)
		die("getWindowSize");
}

int main() {
  char c;

  enableRawMode();
  initEditor();

  while (1)
  {
	editorRefreshScreen();
	editorProcessKeypress();	
  }
  return 0;
}
