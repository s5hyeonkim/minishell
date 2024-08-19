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
	terminal_printon();
	free_shell(*shell);
	exit(errcode);
}
