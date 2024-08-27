/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:31:36 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/26 17:26:38 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execute.h"

static int	is_envopt(char *str)
{
	if (!str || !ft_memcmp(str, "--", 3) || str[0] != '-')
		return (TRUE);
	return (FALSE);
}

int	ft_env(t_process p, t_data *d)
{
	int		index;
	char	**envs;

	if (!is_envopt(p.exec.args[1]))
		return (handle_error(p.exec.args[0], p.exec.args[1], INVALID_OPT));
	envs = deqtostrs(d->envps->head, ENV);
	if (!envs)
		return (handle_error(p.exec.args[0], NULL, EXTRA_ERROR));
	index = 0;
	while (envs[index])
	{
		if (ft_strchr(envs[index], '='))
			ft_putendl_fd(envs[index], p.fd.in[1]);
		index++;
	}
	free_strs(envs);
	return (EXIT_SUCCESS);
}
