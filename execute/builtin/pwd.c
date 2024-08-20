/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:32:07 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/14 19:19:37 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execute.h"

static int	is_valid_option(char *opt)
{
	if (!opt)
		return (TRUE);
	if (!ft_memcmp(opt, "--", 3) || opt[0] != '-' || !ft_memcmp(opt, "-", 2))
		return (TRUE);
	return (FALSE);
}

//PATH_MAX value need to be changed
int	ft_pwd(t_process p, t_data *d)
{
	(void) d;
	if (!is_valid_option(p.args[1]))
		return (handle_error(p.args[0], p.args[1], INVALID_OPT));
	ft_putendl_fd(d->lcwd, p.fd[1]);
	return (EXIT_SUCCESS);
}
