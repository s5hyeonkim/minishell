#include "../minishell.h"

/* utils */
void	move_cursor(void)
{
	ft_putstr_fd("\033[1A", STDERR_FILENO);
	ft_putstr_fd("\033[12C", STDERR_FILENO);
}

/* signal*/
void	replace_line(int redisplayon)
{
	ft_putchar_fd('\n', STDERR_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	if (redisplayon == TRUE)
		rl_redisplay();
}
