/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:31:36 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/23 19:31:38 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execute.h"

int	ft_env(t_process p, t_data *d)
{
	int		index;
	char	**envs;

	if (p.args[1] && (ft_memcmp(p.args[1], "--", 3) || p.args[2]))
		return (handle_error(p.args[0], NULL, INVALID_OPT));
	envs = deqtostrs(d->envps);
	if (!envs)
		return (handle_error(p.args[0], NULL, EXTRA_ERROR));
	index = 0;
	while (envs[index])
	{
		if (ft_strchr(envs[index], '='))
			ft_putendl_fd(envs[index], STDOUT_FILENO);
		index++;
	}
	free_strs(envs);
	return (EXIT_SUCCESS);
}
