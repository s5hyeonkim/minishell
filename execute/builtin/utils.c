/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:11:02 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/14 19:19:47 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execute.h"

void	set_rwfd(t_process *p)
{
	if (p->redirect_fd[0] > 0)
		p->fd[0] = p->redirect_fd[0];
	if (p->redirect_fd[1] > 0)
		p->fd[1] = p->redirect_fd[1];
	else
		p->fd[1] = 1;
}

int	is_valid_opt(char	*opt)
{
	if (!opt)
		return (TRUE);
	if (opt[0] != '-')
		return (TRUE);
	if (!ft_memcmp(opt, "-", 2))
		return (TRUE);
	if (!ft_memcmp(opt, "--", 3))
		return (TRUE);
	return (FALSE);
}
