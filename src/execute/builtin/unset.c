/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:32:13 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/21 17:16:46 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execute.h"

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

	index = 1;
	status = EXIT_SUCCESS;
	if (!ft_memcmp(p.exec.args[1], "--", 3))
		index++;
	while (p.exec.args[index])
	{
		if (!is_valid_name(p.exec.args[index]))
			status = handle_error(p.exec.args[0], p.exec.args[index], INVALID_IDF);
		else
			remove_targetdeq(d->envps, find_deq(d->envps, p.exec.args[index]));
		index++;
	}
	return (status);
}

int	ft_unset(t_process p, t_data *d)
{
	if (!p.exec.args[1] || (!p.exec.args[2] && !ft_memcmp(p.exec.args[1], "--", 3)))
		return (EXIT_SUCCESS);
	if (!is_valid_opt(p.exec.args[1]))
	{
		handle_error(p.exec.args[0], p.exec.args[1], INVALID_OPT);
		return (BUILTIN_ERROR);
	}
	return (unset_wt_argv(p, d));
}
