#include "../minishell.h"

static void	print_export(char *strs[])
{
	int		index;
	size_t	size;

	index = 0;
	while (strs[index])
	{
		if (!ft_strchr(strs[index], '='))
			printf("export %s\n", strs[index]);
		else
		{
			size = ft_strchr(strs[index], '=') - strs[index];
			strs[index][size] = 0;
			printf("export %s=\"%s\"\n", strs[index] ,strs[index] + size + 1);
			strs[index][size] = '=';
		}
		index++;
	}
}

static int	ft_keycmp(char *a, char *b)
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

static void	ft_sort(char **strs)
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

int ft_export(t_shell *shell, t_process p)
{
	char	**envs;
	int		index;
	int		status;
	t_map	keyval;

	status = EXIT_SUCCESS;
	if (!p.args[1] || !ft_memcmp(p.args[1], "--", 3))
	{
		envs = deqtostrs(shell->data.envps);
		if (!envs)
			return (handle_error(p.args[0], NULL, EXTRA_ERROR));
		ft_sort(envs);
		print_export(envs);
		free_strs(envs);
		return (status);
	}
	index = 0;
	while (p.args[++index])
	{
		if (!is_valid_key(p.args[index]))
			status = handle_error(p.args[0], p.args[1], INVALID_IDF);
		else if (set_map(&keyval, p.args[index]) || replace_back(shell->data.envps, keyval.key, keyval.mid, keyval.val))
		{
			free_map(&keyval);
			return (handle_error(p.args[0], NULL, EXTRA_ERROR));
		}
	}
	return (status);
}