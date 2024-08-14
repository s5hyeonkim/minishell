/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:32:01 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/14 19:19:32 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execute.h"

static void	print_export(char *strs[], int write)
{
	int		index;
	size_t	size;

	index = 0;
	while (strs[index])
	{
		ft_putstr_fd("export ", write);
		if (!ft_strchr(strs[index], '='))
			ft_putendl_fd(strs[index], write);
		else
		{
			size = ft_strchr(strs[index], '=') - strs[index];
			strs[index][size] = 0;
			ft_putstr_fd(strs[index], write);
			ft_putstr_fd("=\"", write);
			ft_putstr_fd(strs[index] + size + 1, write);
			ft_putendl_fd("\"", write);
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

int	export_wo_argv(t_data *d, int fd[2])
{
	char	**envs;

	envs = deqtostrs(d->envps->head);
	if (!envs)
		return (handle_error("export", NULL, EXTRA_ERROR));
	ft_sort(envs, ft_keycmp);
	print_export(envs, fd[1]);
	free_strs(envs);
	return (EXIT_SUCCESS);
}

int	export_wt_argv(t_process p, t_data *d)
{
	t_map	keyval;
	int		index;
	int		status;

	index = 1;
	if (!ft_memcmp(p.args[1], "--", 3))
		index++;
	status = EXIT_SUCCESS;
	ft_memset(&keyval, 0, sizeof(t_map));
	while (p.args[index])
	{
		if (!is_valid_key(p.args[index]))
			status = handle_error(p.args[0], p.args[index], INVALID_IDF);
		else if (set_map(&keyval, p.args[index]) \
		|| replace_back(d->envps, keyval.key, keyval.mid, keyval.val))
		{
			free_map(&keyval);
			return (handle_error(p.args[0], NULL, EXTRA_ERROR));
		}
		free_map(&keyval);
		index++;
	}
	return (status);
}

int	ft_export(t_process p, t_data *d)
{
	int		fd[2];

	set_rwfd(p, &fd[1], 1);
	if (!p.args[1] || (!p.args[2] && !ft_memcmp(p.args[1], "--", 3)))
		return (export_wo_argv(d, fd));
	if (!is_valid_opt(p.args[1]))
	{
		handle_error(p.args[0], p.args[1], INVALID_OPT);
		return (BUILTIN_ERROR);
	}
	return (export_wt_argv(p, d));
}
