#include "minishell.h"

void	free_token(t_token *t)
{
	if (t)
	{
		free_token(t->left);
		t->left = NULL;
		free_token(t->right);
		t->right = NULL;
		free(t->cmd);
		free(t);
	}
}

void	free_data(t_data d)
{
	free_strs(d.paths);
	free_deques(&d.envps);
	// add more?
}

void	free_tprocess(t_process *p)
{
	int	index;

	index = 0;
	if (!p)
		return ;
	while (p[index].args)
	{
		free_strs(p[index].args);
		free(p[index].path);
		index++;
	}
}

void	free_info(t_exec info)
{
	free_token(info.t);
	free_data(info.data);
	free_tprocess(info.p);
}
