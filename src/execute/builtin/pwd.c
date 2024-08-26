/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:32:07 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/21 17:15:15 by yubshin          ###   ########.fr       */
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

int	ft_pwd(t_process p, t_data *d)
{
	if (!is_valid_option(p.exec.args[1]))
		return (handle_error(p.exec.args[0], p.exec.args[1], INVALID_OPT));
	ft_putendl_fd(d->lcwd, p.fd.in[1]);
	return (EXIT_SUCCESS);
}
