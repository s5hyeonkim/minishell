/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:38:20 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/19 16:39:54 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	open_redirect(int redirect, char *word, char *link)
{
	int	fd;

	fd = 0;
	if (redirect == T_GREAT)
		fd = open(word, O_RDONLY);
	else if (redirect == T_DGREAT)
		fd = open(link, O_RDONLY);
	else if (redirect == T_LESS)
		fd = open(word, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	else if (redirect == T_DLESS)
		fd = open(word, O_CREAT | O_WRONLY | O_APPEND, 0666);
	return (fd);
}

int	open_token(t_token *t, t_process *p)
{
	int			*fd;
	int			status;

	fd = &p->redirect_fd[t->type % 2];
	if (t->type == T_DGREAT)
	{
		p->flag = 1;
		status = here_doc(p->link, t->word);
		if (status)
			return (status);
	}
	*fd = open_redirect(t->type, t->word, p->link);
	if (*fd == -1)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	find_redirect(t_process *p, t_token *t)
{
	int		status;

	status = 0;
	if (!t)
		return (EXIT_SUCCESS);
	if (set_filedoc(p))
		return (EXTRA_ERROR);
	if (is_redirect(t->type))
	{
		if (p->redirect_fd[t->type % 2] > 0)
			close_fd(&p->redirect_fd[t->type % 2]);
		status = open_token(t, p);
	}
	if (!status)
		status = find_redirect(p, t->left);
	if (!status)
		status = find_redirect(p, t->right);
	return (status);
}
