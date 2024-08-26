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
	if (p->fd.redirect[0] > 0)
		p->fd.in[0] = p->fd.redirect[0];
	if (p->fd.redirect[1] > 0)
		p->fd.in[1] = p->fd.redirect[1];
	else
		p->fd.in[1] = 1;
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

int	is_folder(char *to_dir)
{
	DIR	*dir;

	dir = opendir(to_dir);
	if (!dir)
		return (FALSE);
	closedir(dir);
	return (TRUE);
}

int	is_file(char *to_dir)
{
	int	fd;

	fd = open(to_dir, O_RDONLY);
	if (fd == -1)
		return (FALSE);
	close(fd);
	return (TRUE);
}