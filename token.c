#include "minishell.h"

void	exec_cmds(t_shell *shell)
{
	if (shell->p_size > 1 || !is_builtin(shell->p[0].path))
	{
		set_signal_sub(shell, handler_sub); 
		subprocess(shell);
		wait_process(shell);
	}
	else
	{
		inprocess(shell);
		//print_deques(shell->data.envps);
	}
}

/* signal_execute */
void	child(t_shell *shell, int index)
{
	// printf("child process start\n");
	int	ret;
	if (is_builtin(shell->p[index].path))
	{
		ret = exec_builtin(shell, shell->p[index]);
		exit_process(shell, NULL, ret);
	}
	else
		exec_program(shell, shell->p[index]);
}

void	parent(t_shell *shell, int index)
{
	int child_status;

	close_pipe(shell, index);
	waitpid(shell->p[index].pid, &child_status, WNOHANG);
}

