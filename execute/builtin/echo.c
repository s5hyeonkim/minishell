/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:31:30 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/14 19:18:46 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execute.h"

static int	is_no_nl(char *str)
{
	if (str && !ft_memcmp(str, "-n", 3))
		return (TRUE);
	return (FALSE);
}

int	ft_echo(t_process p, t_data *d)
{
	int	index;

	(void) d;
	index = 1;
	if (is_no_nl(p.args[1]))
		index++;
	while (p.args[index])
	{
		ft_putstr_fd(p.args[index++], p.fd[1]);
		if (p.args[index])
			ft_putchar_fd(' ', p.fd[1]);
	}
	if (!is_no_nl(p.args[1]))
		ft_putchar_fd('\n', p.fd[1]);
	return (EXIT_SUCCESS);
}
