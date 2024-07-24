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

int	ft_unset(t_process p, t_data *d)
{
	int		index;
	int		status;
	char	*key;

	index = 0;
	status = EXIT_SUCCESS;
	if (!p.args[1] || !ft_memcmp(p.args[1], "--", 3))
		return (status);
	while (p.args[++index])
	{
		if (!is_valid_option(p.args[index]))
		{
			status = BUILTIN_ERROR;
			handle_error(p.args[0], p.args[index], INVALID_OPT);
		}
		else if (!is_valid_name(p.args[index]))
			status = handle_error(p.args[0], p.args[index], INVALID_IDF);
		else
		{
			key = get_key(p.args[index]);
			if (!key)
				return (handle_error(p.args[0], NULL, EXTRA_ERROR));
			remove_targetdeq(d->envps, find_deq(d->envps, key));
			free(key);
		}
	}
	return (status);
}
