/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:38:12 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/26 17:27:37 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	free_data(t_data d)
{
	free_strs(d.paths);
	free_deques(&d.envps);
	free(d.lcwd);
}

void	free_process(t_process *p, size_t size)
{
	size_t	index;

	index = 0;
	if (!p)
		return ;
	while (index < size)
	{
		free_strs(p[index].exec.args);
		free(p[index].exec.path);
		free(p[index].file.link);
		index++;
	}
	free(p);
}

void	clean_process(t_process *p, size_t p_size)
{
	size_t		index;
	t_process	*p_id;

	index = 0;
	if (!p)
		return ;
	while (index < p_size)
	{
		p_id = &p[index];
		if (!p_id && ++index)
			continue ;
		close_fd(&p_id->fd.redirect[0]);
		close_fd(&p_id->fd.redirect[1]);
		close_fd(&p_id->fd.pipe[0]);
		close_fd(&p_id->fd.pipe[1]);
		index++;
	}
	free_process(p, p_size);
}

void	clean_files(t_process *p, size_t p_size)
{
	size_t		index;
	t_process	*p_id;

	index = 0;
	if (!p)
		return ;
	while (index < p_size)
	{
		p_id = &p[index];
		if (!p_id && ++index)
			continue ;
		if (p_id->file.flag)
			unlink(p_id->file.link);
		p_id->file.flag = 0;
		index++;
	}
}
