#include "../minishell.h"

void	print_strs(char *strs[])
{
	int	index;

	index = 0;
	while (strs[index])
	{
		printf("declare -x: %s\n", strs[index]);
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
				strs[index2] = strs[index];
			}
			index2++;
		}
		index++;
	}
}

int	change_envs(t_deques *deqs, char *str)
{
	t_deque	*node;
	t_pairs	keyval;

	node = pop(deqs, search_deq(deqs, str));
	free_deque(node);
	if (set_keyval(str, &keyval) || push_back(deqs, keyval))
		return (MALLOC_FAILED);
	return (EXIT_SUCCESS);
}

int ft_export(t_exec *info, t_process p)
{
	char	**envs;
	int		index;
	int		status;

	status = EXIT_SUCCESS;
	envs = deqtoenvp(info->data.envps, ENV);
	if (!envs)
		exit_process(info, NULL, MALLOC_FAILED);
	if (!p.args[1])
	{
		ft_sort(envs);
		print_strs(envs);
		return (status);
	}
	index = 0;
	while (p.args[++index])
	{
		if (!ft_isalpha(p.args[index][0]))
			status = 1;
		else if (change_envs(info->data.envps, p.args[index]))
			exit_process(info, NULL, MALLOC_FAILED);
	}
	return (0);
}