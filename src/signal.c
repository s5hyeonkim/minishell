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

void	child_handler(int signo)
{
	status = signo + SIGEXIT;
	// printf("signal receive %d\n", status);
	if (signo == SIGINT)
	{	
		ft_putstr_fd("aa\n", STDERR_FILENO);
		//exit(1);
	}
}

void	signal_handler(int signo)
{
	status = signo + SIGEXIT;
	// printf("signal receive %d\n", status);
	if (signo == SIGINT)
	{
		replace_line(TRUE);
	}
	if (signo == SIGQUIT)
	{
		// SIG_IGN;
		// ft_putstr_fd("\033[12C", STDERR_FILENO);
		// move_cursor();
	}
	if (signo == SIGTERM)
	{
		printf("sigterm?");

	}
}

void set_signal(t_shell *shell, void(*handler)(int))
{
	t_sigaction action;

	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	if (sigaction(SIGINT, &action, NULL) == (int)SIG_ERR)
		exit_process(shell, NULL, EXTRA_ERROR);
	else if (sigaction(SIGTERM, &action, NULL) == (int)SIG_ERR)
		exit_process(shell, NULL, EXTRA_ERROR);
	action.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &action, NULL) == (int)SIG_ERR)
		exit_process(shell, NULL, EXTRA_ERROR);
}
