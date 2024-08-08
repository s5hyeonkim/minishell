#include "minishell.h"

size_t	find_pipe(t_token *t)
{
	size_t	pipe_num;

	if (!t)
		return (0);
	pipe_num = 0;
	if (t->type == T_PIPE && t->right)
		pipe_num++;
	pipe_num += find_pipe(t->left);
	pipe_num += find_pipe(t->right);
	return (pipe_num);
}

void	open_pipe(t_shell *shell, size_t index)
{
	shell->p[index].pid = fork();
	if (shell->p[index].pid == -1)
		exit_process(shell, NULL, EXTRA_ERROR);
	if (index == shell->p_size - 1)
		return ;
	if (pipe(shell->p[index].pipe_fd) == -1)
		exit_process(shell, NULL, EXTRA_ERROR);
}