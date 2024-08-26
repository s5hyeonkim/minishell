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
	p->fd.redirect[t->type % 2] = open_redirect(t->type, t->word, p->file.link);
	if (p->fd.redirect[t->type % 2] == -1)
		return (handle_error(NULL, t->word, EXTRA_ERROR));
	return (EXIT_SUCCESS);
}

int	find_redirect(t_process *p, t_token *t)
{
	int	status;

	status = EXIT_SUCCESS;
	if (!t)
		return (status);
	if (is_redirect(t->type))
	{
		if (p->fd.redirect[t->type % 2] > 0)
			close_fd(&p->fd.redirect[t->type % 2]);
		status = open_token(t, p);
	}
	if (!status)
		status = find_redirect(p, t->left);
	if (!status)
		status = find_redirect(p, t->right);
	return (status);
}

int	set_redirect_in(t_token *t, char *name)
{
	free(t->word);
	t->word = ft_strdup(name);
	if (!t->word)
		return (EXTRA_ERROR);
	t->type = T_GREAT;
	return (EXIT_SUCCESS);
}

int	set_heredoc(t_process *p, t_token *t, t_deques *envps)
{
	int	status;

	status = EXIT_SUCCESS;
	if (!t)
		return (status);
	if (t->type == T_DGREAT)
	{
		if (set_filedoc(p) || set_redirect_in(t, p->file.link))
			status = EXTRA_ERROR;
	}
	if (!status)
		status = set_heredoc(p, t->left, envps);
	if (!status)
		status = set_heredoc(p, t->right, envps);
	return (status);
}

int	set_redirect(t_process *p, t_deques *envps)
{
	if (set_heredoc(p, p->t.left, envps))
		return (EXTRA_ERROR);
	return (find_redirect(p, p->t.left));
}
