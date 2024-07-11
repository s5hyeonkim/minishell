#include "../minishell.h"

/* signal*/
void	replace_line(void)
{
	ft_putchar_fd('\n', STDERR_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	child_handler(int signo)
{
	(void) signo;
	status = signo + 128;
	//printf("signal receive %d\n", status);
	if (signo == SIGINT)
		exit(status);
}

void	signal_handler(int signo)
{
	status = signo + 128;
	//printf("signal receive %d\n", status);
	if (signo == SIGINT)
		replace_line();
		
}

// void	set_signal(t_shell *shell, void(*func)(int))
// {
// 	if (signal(SIGINT, func) == SIG_ERR)
// 		exit_process(shell, NULL, EXTRA_ERROR);
// 	if (signal(SIGTERM, func) == SIG_ERR)
// 		exit_process(shell, NULL, EXTRA_ERROR);
// 	if (signal(SIGQUIT, func) == SIG_ERR)
// 		exit_process(shell, NULL, EXTRA_ERROR);
// }

void set_signal(t_shell *shell, void(*signal_handler)(int))
{
	t_sigaction action;

	action.sa_handler = signal_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	if (sigaction(SIGINT, &action, NULL) == (int)SIG_ERR)
		exit_process(shell, NULL, EXTRA_ERROR);
	if (sigaction(SIGQUIT, &action, NULL) == (int)SIG_ERR)
		exit_process(shell, NULL, EXTRA_ERROR);
	if (sigaction(SIGTERM, &action, NULL) == (int)SIG_ERR)
		exit_process(shell, NULL, EXTRA_ERROR);
}