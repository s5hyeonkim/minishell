#include "../minishell.h"

/* terminal */
void	get_terminal(t_shell *shell)
{
	tcgetattr(1, &shell->term);
	// printf("init: %lx\n", shell->term.c_lflag);
}

void	reset_terminal(t_shell *shell)
{
	tcsetattr(1, 0, &shell->term);
}