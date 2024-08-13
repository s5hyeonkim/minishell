#include "execute.h"

char	*read_val_strs(char *strs[], char *key)
{
	char	*ret;
	int		index;
	size_t	size;

	index = 0;
	ret = NULL;
	if (!strs)
		return (NULL);
	while (strs[index])
	{
		size = ft_strchr(strs[index], '=') - strs[index];
		if (!ft_memcmp(strs[index], key, size) && ft_strlen(key) == size)
		{
			ret = strs[index] + size + 1;
			break ;
		}
		index++;
	}
	return (ret);
}

void	set_rwfd(t_process p, int *num, int is_write)
{
	if (p.redirect_fd[is_write] > 0)
		*num = p.redirect_fd[is_write];
	else if (p.pipe_fd[is_write] > 0)
		*num = p.pipe_fd[is_write];
	else
		*num = is_write;
}


void	ft_sort(char **strs, int(guide)(char *, char *))
{
	char	*temp;
	int		index;
	int		index2;

	index = 0;
	while (strs[index])
	{
		index2 = index + 1;
		while (strs[index2])
		{
			if (guide(strs[index], strs[index2]) > 0)
			{
				temp = strs[index];
				strs[index] = strs[index2];
				strs[index2] = temp;
			}
			index2++;
		}
		index++;
	}
}
