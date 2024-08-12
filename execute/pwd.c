/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:32:07 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/23 19:32:09 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execute.h"

int	is_valid_option(char *opt)
{
	if (!opt)
		return (TRUE);
	if (!ft_memcmp(opt, "--", 3) || !ft_memcmp(opt, "-", 2) || opt[0] != '-')
		return (TRUE);
	return (FALSE);
}

//PATH_MAX value need to be changed
int	ft_pwd(t_process p, t_data *d)
{
	int	fd_out;

	(void) d;
	if (!is_valid_option(p.args[1]))
		return (handle_error(p.args[0], p.args[1], INVALID_OPT));
	set_rwfd(p, &fd_out, 1);
	ft_putendl_fd(d->lcwd, fd_out);
	return (EXIT_SUCCESS);
}
