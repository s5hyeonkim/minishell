/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:42:42 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/19 16:42:44 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *t)
{
	if (t == NULL)
		return ;
	free_token(t->left);
	free_token(t->right);
	free(t->word);
	free_strs(t->argvs);
	ft_memset(t, 0, sizeof(t_token));
	free(t);
}

void	free_shell(t_shell shell)
{
	free_data(shell.data);
	free_token(shell.t);
	clean_process(shell.p, shell.p_size);
	ft_memset(&shell, 0, sizeof(t_shell));
}

void	exit_wo_error(t_shell *shell, int errcode)
{
	reset_terminal(shell);
	free_shell(*shell);
	exit(errcode);
}

/* exit */
void	exit_process(t_shell *shell, char *obj, int errcode)
{
	terminal_printon();
	if (errcode && errcode < CMD_NOT_FOUND)
	{
		handle_error(obj, NULL, errcode);
		errcode = EXIT_FAILURE;
	}
	else if (errcode == CMD_NOT_FOUND)
		handle_error(obj, NULL, errcode);
	if (errcode == SIGEXIT + SIGTERM)
		errcode = EXIT_SUCCESS;
	clean_files(shell->p, shell->p_size);
	reset_terminal(shell);
	free_shell(*shell);
	exit(errcode);
}

void	clean_buffer(t_shell *shell)
{
	clean_files(shell->p, shell->p_size);
	free_token(shell->t);
	clean_process(shell->p, shell->p_size);
	shell->p = 0;
	shell->p_size = 0;
	shell->t = 0;
}
