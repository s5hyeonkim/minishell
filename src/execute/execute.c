/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:38:02 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/21 12:58:11 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	fork_process(t_process *p)
{
	p->pid = fork();
	if (p->pid == -1)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	set_cmdargs(t_process *p, char **argvs)
{
	p->exec.args = get_cmdargs(argvs);
	if (!p->exec.args)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	set_cmdpath(t_process *p, char **paths, char *cmd)
{
	p->exec.path = get_cmdpath(paths, cmd);
	if (!p->exec.path)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}
