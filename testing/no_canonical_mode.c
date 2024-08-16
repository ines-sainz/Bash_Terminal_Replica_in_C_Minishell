#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

int	main(void)
{
	struct	termios old_tty, new_tty;
	tcgetattr(STDIN_FILENO, &old_tty);
	new_tty = old_tty;

	// Desactivar el modo canónico y desactivar la visualización de los caracteres tecleados
	new_tty.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_tty);

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int terminal_width = w.ws_col;

	int cursor_position = 0;
	char ch;
	while (read(STDIN_FILENO, &ch, 1) == 1) {
		if (ch == '\n') {
			printf("\n");
			cursor_position = 0;
		} else {
			putchar(ch);
			cursor_position++;

			if (cursor_position >= terminal_width) {
				printf("\n");
				cursor_position = 0;
			}
		}
	}

	// Restaurar los atributos originales de la terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &old_tty);
	return 0;
}
