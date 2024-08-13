#include "minishell.h"

size_t	find_pipe(t_token *t)
{
	size_t	pipe_num;

	if (!t)
		return (0);
	pipe_num = 0;
	if (t->type == T_PIPE)
		pipe_num++;
	pipe_num += find_pipe(t->left);
	pipe_num += find_pipe(t->right);
	return (pipe_num);
}

int	fork_process(t_process *p)
{
	p->pid = fork();
	if (p->pid == -1)
	{
		handle_error(NULL, NULL, EXTRA_ERROR);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	open_pipe(t_process *p, size_t size)
{
	if (p->index != size - 1 && pipe(p->pipe_fd) == -1)
	{
		handle_error(NULL, NULL, EXTRA_ERROR);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	close_pipe(t_shell* shell, size_t index)
{
	int	is_child;

	is_child = TRUE;
	if (shell->p[index].pid)
		is_child = FALSE;
	if (index != shell->p_size - 1)
	{
		close(shell->p[index].pipe_fd[!is_child]);
		shell->p[index].pipe_fd[!is_child] = 0;
	}
	if (index)
	{
		close(shell->p[index - 1].pipe_fd[is_child]);
		shell->p[index - 1].pipe_fd[is_child] = 0;
	}
}
