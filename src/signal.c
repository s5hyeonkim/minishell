#include "../minishell.h"

typedef struct sigaction t_sigaction;

void set_signal(t_exec *info, void(*func)(int))
{
	t_sigaction sig;

	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGQUIT, &sig, NULL);

}