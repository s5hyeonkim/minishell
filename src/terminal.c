
#include "src.h"

void	set_terminal_printoff(void)
{
	t_termios	term;

	tcgetattr(1, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(1, 0, &term);
}


void	set_terminal_printon(void)
{
	t_termios	term;

	tcgetattr(1, &term);
	term.c_lflag &= (ECHOCTL);
	tcsetattr(1, 0, &term);
}
