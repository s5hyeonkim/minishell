/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:38:35 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/19 16:40:24 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	close_fd(int *num)
{
	if (*num > 0)
		close(*num);
	*num = 0;
}

void	dup_fd(int *fd, int std)
{
	if (*fd > 0)
	{
		dup2(*fd, std);
		close_fd(fd);
	}
}

int	is_redirect(int type)
{
	if (type == T_GREAT || type == T_LESS)
		return (TRUE);
	if (type == T_DGREAT || type == T_DLESS)
		return (TRUE);
	return (FALSE);
}

void	set_fd_builtin(t_process *p)
{
	int	fd_in;
	int	fd_out;

	fd_in = dup(STDIN_FILENO);
	fd_out = dup(STDOUT_FILENO);
	if (p->redirect_fd[0] > 0)
		dup_fd(&p->redirect_fd[0], fd_in);
	if (p->redirect_fd[1] > 0)
		dup_fd(&p->redirect_fd[1], fd_out);
	p->redirect_fd[0] = fd_in;
	p->redirect_fd[1] = fd_out;
}
