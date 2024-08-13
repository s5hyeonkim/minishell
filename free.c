#include "minishell.h"

void	free_token(t_token *t)
{
	if (t == NULL)
		return ;
	free_token(t->left);
	free_token(t->right);
	free(t->word);
	free_strs(t->argvs);
	ft_memset(t, 0, sizeof(t_token));
	free(t);
}

void	free_data(t_data d)
{
	free_strs(d.paths);
	free_deques(&d.envps);
	free(d.lcwd);
	// add more?
}

void	free_process(t_process *p, size_t size)
{
	size_t	index;

	index = 0;
	if (!p)
		return ;
	while (index < size)
	{
		free_strs(p[index].args);
		free(p[index].path);
		free(p->link);
		index++;
	}
	free(p);
}

void	close_fd(int *num)
{
	if (*num > 0)
		close(*num);
	*num = 0;
}

void	clean_process(t_process *p, size_t p_size)
{
	size_t		index;
	t_process	*p_id;

	index = 0;
	if (!p)
		return ;
	while (index < p_size)
	{
		p_id = &p[index];
		if (!p_id && ++index)
			continue ;
		// if (p_id->flag) // parent 만
			// unlink(p->link);
		close_fd(&p_id->redirect_fd[0]);
		close_fd(&p_id->redirect_fd[1]);
		close_fd(&p_id->pipe_fd[0]);
		close_fd(&p_id->pipe_fd[1]);
		index++;
	}
	free_process(p, p_size);
}

void	free_shell(t_shell shell)
{
	free_data(shell.data);
	free_token(shell.t);
	clean_process(shell.p, shell.p_size);
	ft_memset(&shell, 0, sizeof(t_shell));
}
