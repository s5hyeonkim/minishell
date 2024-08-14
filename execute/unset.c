/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:32:13 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/23 19:44:37 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execute.h"

int	is_valid_name(char *s)
{
	if (ft_isdigit(*s) || *s == '=')
		return (FALSE);
	while (*s)
	{
		if (!ft_isdigit(*s) && !ft_isalpha(*s) && *s != '_')
			return (FALSE);
		s++;
	}
	return (TRUE);
}

int	unset_wt_argv(t_process p, t_data *d)
{
	int		index;
	int		status;
	char	*key;

	index = 1;
	status = EXIT_SUCCESS;
	if (!ft_memcmp(p.args[1], "--", 3))
		index++;
	while (p.args[index])
	{
		if (!is_valid_name(p.args[index]))
			status = handle_error(p.args[0], p.args[index], INVALID_IDF);
		else
		{
			key = get_key(p.args[index]);
			if (!key)
				return (handle_error(p.args[0], NULL, EXTRA_ERROR));
			remove_targetdeq(d->envps, find_deq(d->envps, key));
			free(key);
		}
		index++;
	}
	return (status);
}

int	ft_unset(t_process p, t_data *d)
{
	if (!p.args[1] || (!p.args[2] && !ft_memcmp(p.args[1], "--", 3)))
		return (EXIT_SUCCESS);
	if (p.args[1][0] == '-' && ft_memcmp(p.args[1], "--", 3) && ft_memcmp(p.args[1], "-", 2))
	{
		handle_error(p.args[0], p.args[1], INVALID_OPT);
		return (BUILTIN_ERROR);
	}
	return (unset_wt_argv(p, d));
}
