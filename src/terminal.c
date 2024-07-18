#include "../minishell.h"

/* terminal */
void	init_terminal(t_shell *shell)
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
	// ft_putstr_fd("here\n", 2);
	// printf("off: %lx\n", term.c_lflag);
}

void	terminal_printon(t_shell *shell)
{
	tcsetattr(1, 0, &shell->term);
	// printf("on: %lu\n", shell->term.c_lflag);
}
