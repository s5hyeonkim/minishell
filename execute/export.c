/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:32:01 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/23 19:43:57 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execute.h"

static void	print_export(char *strs[])
{
	int		index;
	size_t	size;

	index = 0;
	while (strs[index])
	{
		ft_putstr_fd("export ", STDOUT_FILENO);
		if (!ft_strchr(strs[index], '='))
			ft_putendl_fd(strs[index], STDOUT_FILENO);
		else
		{
			size = ft_strchr(strs[index], '=') - strs[index];
			strs[index][size] = 0;
			ft_putstr_fd(strs[index], STDOUT_FILENO);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(strs[index] + size + 1, STDOUT_FILENO);
			ft_putendl_fd("\"", STDOUT_FILENO);
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

int	ft_export(t_process p, t_data *d)
{
	char	**envs;
	int		index;
	int		status;
	t_map	keyval;

	status = EXIT_SUCCESS;
	if (!p.args[1] || !ft_memcmp(p.args[1], "--", 3))
	{
		envs = deqtostrs(d->envps);
		if (!envs)
			return (handle_error(p.args[0], NULL, EXTRA_ERROR));
		ft_sort(envs);
		print_export(envs);
		free_strs(envs);
		return (status);
	}
	index = 0;
	ft_memset(&keyval, 0, sizeof(t_map));
	while (p.args[++index])
	{
		if (!is_valid_key(p.args[index]))
			status = handle_error(p.args[0], p.args[1], INVALID_IDF);
		else if (set_map(&keyval, p.args[index]) || replace_back(d->envps, keyval.key, keyval.mid, keyval.val))
		{
			free_map(&keyval);
			return (handle_error(p.args[0], NULL, EXTRA_ERROR));
		}
		free_map(&keyval);
	}
	return (status);
}
