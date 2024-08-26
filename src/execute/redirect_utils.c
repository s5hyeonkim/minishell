/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:38:35 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/26 17:29:18 by sohykim          ###   ########.fr       */
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
	if (p->fd.redirect[0] > 0)
		dup_fd(&p->fd.redirect[0], fd_in);
	if (p->fd.redirect[1] > 0)
		dup_fd(&p->fd.redirect[1], fd_out);
	p->fd.redirect[0] = fd_in;
	p->fd.redirect[1] = fd_out;
}

int	open_redirect(t_token t)
{
	int	fd;

	fd = 0;
	if (t.type == T_GREAT)
		fd = open(t.word, O_RDONLY);
	else if (t.type == T_LESS)
		fd = open(t.word, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	else if (t.type == T_DLESS)
		fd = open(t.word, O_CREAT | O_WRONLY | O_APPEND, 0666);
	return (fd);
}
