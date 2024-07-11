#include "minishell.h"

void	free_token(t_token *t)
{
	if (t)
	{
		// printf("here\n");
		free_token(t->left);
		t->left = NULL;
		free_token(t->right);
		t->right = NULL;
		free(t->cmd);
		t->cmd = NULL;
	}
	// free(t);
}

void	free_data(t_data d)
{
	free_strs(d.paths);
	free_deques(&d.envps);
	// add more?
}

void	free_tprocess(t_process *p, size_t size)
{
	size_t	index;

	index = 0;
	if (!p)
		return ;
	while (index < size)
	{
		free_strs(p[index].args);
		free(p[index].path);
		index++;
	}
	free(p);
}

void	free_shell(t_shell shell)
{
	free_token(shell.t);
	free_data(shell.data);
	free_tprocess(shell.p, shell.p_size);
	ft_memset(&shell, 0, sizeof(t_shell));
}
