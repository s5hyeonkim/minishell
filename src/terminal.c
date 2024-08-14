#include "../minishell.h"

/* terminal */
void	get_terminal(t_shell *shell)
{
	tcgetattr(1, &shell->term);
	// printf("init: %lx\n", shell->term.c_lflag);
}

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

void	reset_terminal(t_shell *shell)
{
	tcsetattr(1, 0, &shell->term);
}