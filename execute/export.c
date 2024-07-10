#include "../minishell.h"

void	print_strs(char *strs[])
{
	int		index;
	size_t	size;

	index = 0;
	while (strs[index])
	{
		if (!ft_strchr(strs[index], '='))
			printf("declare -x %s\n", strs[index]);
		else
		{
			size = ft_strchr(strs[index], '=') - strs[index];
			strs[index][size] = 0;
			printf("declare -x: %s=\"%s\"\n", strs[index] ,strs[index] + size + 1);
			strs[index][size] = '=';
		}
		index++;
	}
}

int	ft_keycmp(char *a, char *b)
{
	int	index;

	index = 0;
	while (a[index] && b[index])
	{
		if (a[index] == '=')
			return (-1);
		if (b[index] == '=')
			return (1);
		if (a[index] != b[index])
			break ;
		index++;
	}
	return (a[index] - b[index]);
}

void	ft_sort(char **strs)
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
			if (ft_keycmp(strs[index], strs[index2]) > 0)
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


int ft_export(t_exec *info, t_process p)
{
	char	**envs;
	int		index;
	int		status;

	status = EXIT_SUCCESS;
	if (!p.args[1] || !ft_memcmp(p.args[1], "--", 3))
	{
		envs = deqtoenvp(info->data.envps, ENV);
		if (!envs)
			return (handle_error(p.args[0], NULL, EXTRA_ERROR));
		ft_sort(envs);
		print_strs(envs);
		free_strs(envs);
		return (status);
	}
	index = 0;
	while (p.args[++index])
	{
		if (!ft_isalpha(p.args[index][0]))
			status = EXIT_FAILURE;
		else if (push_keyval(info->data.envps, p.args[index]))
		{
			handle_error(p.args[0], NULL, EXTRA_ERROR);
			return (BUILTIN_ERROR);
		}
	}
	return (status);
}