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

/* parsing and set tokens 수정 필요 */
void	tokenization(t_shell *shell, t_token *t)
{
	printf("origin cmd: %s\n", t->cmd);
	t->type = SIMPLE_CMD;
	(void) shell;
	//tokenization(shell, t->left);
	//tokenization(shell, t->right);
}

int	set_token(t_token **t)
{
	*t = ft_calloc(1, sizeof(t_token));
	if (*t == NULL)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}