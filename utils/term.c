#include "terminal.h"

void	terminal_printoff(void)
{
	t_termios	term;

	tcgetattr(STDOUT_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDOUT_FILENO, TCSANOW, &term);
	// printf("off: %lx\n", term.c_lflag);
}

void	terminal_printon(void)
{
	t_termios	term;

	tcgetattr(STDOUT_FILENO, &term);
	term.c_lflag |= (ECHOCTL);
	tcsetattr(1, 0, &term);
	// printf("on: %lu\n", shell->term.c_lflag);
}
