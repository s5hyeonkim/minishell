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

int	change_envs(t_deques *deqs, char *str)
{
	t_deque	*node;
	t_pairs	keyval;
	char	*key;

	key = get_key_strs(str);
	node = pop(deqs, search_deq(deqs, key));
	free_deque(node);
	free(key);
	if (set_keyval(str, &keyval) || push_back(deqs, keyval))
		return (MALLOC_FAILED);
	if (keyval.val[0])
		deqs->tail->state = ENV;
	else if (keyval.mid)
		deqs->tail->state = EXPORT;
	return (EXIT_SUCCESS);
}

int ft_export(t_exec *info, t_process p)
{
	char	**envs;
	int		index;
	int		status;

	status = EXIT_SUCCESS;
	if (!p.args[1])
	{
		envs = deqtoenvp(info->data.envps, ENV);
		if (!envs)
			exit_process(info, NULL, MALLOC_FAILED);
		ft_sort(envs);
		print_strs(envs);
		free_strs(envs);
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
	return (status);
}