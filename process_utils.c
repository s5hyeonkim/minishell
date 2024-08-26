/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:44:42 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/21 12:59:27 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_fd(t_shell *shell, size_t index)
{
	if (shell->p[index].fd.redirect[STDIN_FILENO] > STDIN_FILENO)
		dup_fd(&shell->p[index].fd.redirect[STDIN_FILENO], STDIN_FILENO);
	else if (index && shell->p[index - 1].fd.pipe[STDIN_FILENO] > 0)
		dup_fd(&shell->p[index - 1].fd.pipe[STDIN_FILENO], STDIN_FILENO);
	if (shell->p[index].fd.redirect[STDOUT_FILENO] > STDIN_FILENO)
		dup_fd(&shell->p[index].fd.redirect[STDOUT_FILENO], STDOUT_FILENO);
	else if (index != shell->p_size - 1 && shell->p[index].fd.pipe[1] > 0)
		dup_fd(&shell->p[index].fd.pipe[STDOUT_FILENO], STDOUT_FILENO);
	shell->p[index].fd.in[1] = 1;
}

int	set_args(t_process *p, t_data d)
{
	t_token	*t;

	t = p->t.right;
	if (set_cmdargs(p, t->argvs) || set_cmdpath(p, d.paths, t->word))
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

size_t	find_pipe(t_token *t)
{
	size_t	pipe_num;

	if (!t)
		return (0);
	pipe_num = 0;
	if (t->type == T_PIPE)
		pipe_num++;
	pipe_num += find_pipe(t->left);
	pipe_num += find_pipe(t->right);
	return (pipe_num);
}

int	open_pipe(t_process *p, size_t size)
{
	if (p->index != size - 1 && pipe(p->fd.pipe) == -1)
		return (handle_error(NULL, NULL, EXTRA_ERROR));
	return (EXIT_SUCCESS);
}

void	close_pipe(t_shell *shell, size_t index)
{
	int	is_child;

	is_child = TRUE;
	if (shell->p[index].pid)
		is_child = FALSE;
	if (index != shell->p_size - 1)
	{
		close(shell->p[index].fd.pipe[!is_child]);
		shell->p[index].fd.pipe[!is_child] = 0;
	}
	if (index)
	{
		close(shell->p[index - 1].fd.pipe[is_child]);
		shell->p[index - 1].fd.pipe[is_child] = 0;
	}
}
