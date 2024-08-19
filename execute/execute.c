/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:38:02 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/19 16:38:04 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	fork_process(t_process *p)
{
	p->pid = fork();
	if (p->pid == -1)
	{
		handle_error(NULL, NULL, EXTRA_ERROR);
		return (EXTRA_ERROR);
	}
	return (EXIT_SUCCESS);
}

int	token_to_word(t_process *p, t_data d, t_token *t)
{
	if (!t || t->type != T_CMD_WORD)
		return (EXIT_SUCCESS);
	p->args = get_cmdargs(t->argvs);
	p->path = get_cmdpath(d.paths, t->word);
	if (!p->args || !p->path)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}
