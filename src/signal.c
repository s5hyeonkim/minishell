#include "../minishell.h"

void set_signal(t_shell *shell, void(*handler)(int), int signo)
{
	t_sigaction action;

	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	if (sigaction(signo, &action, NULL) == (int)SIG_ERR)
		exit_process(shell, NULL, EXTRA_ERROR);
}

void set_signal_init(t_shell *shell, void(*handler)(int))
{
	terminal_printoff();
	set_signal(shell, handler, SIGINT);
	set_signal(shell, handler, SIGTERM);
	set_signal(shell, SIG_IGN, SIGQUIT);
}

//SIG_INT, SIG_QUIT에 handler_sub 적용 
void set_signal_sub(t_shell *shell, void(*handler)(int))
{
	terminal_printon(shell);
	set_signal(shell, handler, SIGINT);
	set_signal(shell, handler, SIGQUIT);
}

void	handler_init(int signo)
{
	status = signo + SIGEXIT;
	// printf("signal receive %d\n", status);
	if (signo == SIGINT)
		replace_line(TRUE);
	if (signo == SIGTERM)
	{
		move_cursor();
		ft_putstr_fd("exit\n", STDERR_FILENO);
	}
}

void	handler_sub(int signo)
{
	status = signo + SIGEXIT;
	// printf("signal receive %d\n", status);
	if (signo == SIGINT)
		replace_line(FALSE);
	if (signo == SIGQUIT)
	{
		ft_putstr_fd("Quit: ", STDERR_FILENO);
		ft_putstr_fd(ft_itoa(signo), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
}