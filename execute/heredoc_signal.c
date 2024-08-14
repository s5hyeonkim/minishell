#include "execute.h"

void	handler_heredoc(int signo)
{
	if (signo == SIGINT)
	{
		g_status = 1;
		rl_replace_line("", 0);
		exit(1);
	}
}

void	handler_heredoc_wait(int signo)
{
	if (signo == SIGINT)
	{
		replace_line(FALSE);
		g_status = 1;
	}
}
