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
		free(t->word);
		t->word = NULL;
	}
	free(t);
}

void	free_data(t_data d)
{
	free_strs(d.paths);
	free_deques(&d.envps);
	free(d.lcwd);
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
// loop 안 추가 필요
void free_cmds(t_token **t, t_process **p, size_t *psize)
{
	free_token(*t);
	*t = NULL;
	free_tprocess(*p, *psize);
	*p = NULL;
	*psize = 0;
}

void	free_shell(t_shell shell)
{
	free_data(shell.data);
	clean_cmds(&shell);
	ft_memset(&shell, 0, sizeof(t_shell));
}
