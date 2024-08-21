/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:39:43 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/21 13:12:42 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	lexer(t_token **token, t_deques *envps, char *buffer);

int	parselines(t_shell *shell, char *buffer)
{
	char	*validbuffer;
	int		code;

	code = get_validbuffer(buffer, &validbuffer);
	free(buffer);
	if (code == EXIT_SUCCESS)
		code = lexer(&(shell->t), shell->data.envps, validbuffer);
	free(validbuffer);
	if (code == EXIT_SUCCESS)
		code = parser(&shell->t);
	if (code == SYNTAX_ERROR)
		return (EXIT_FAILURE);
	return (code);
}

int	lexer(t_token **token, t_deques *envps, char *buffer)
{
	char	**buffers;

	buffers = split_pipe(buffer);
	if (!buffers)
		return (EXTRA_ERROR);
	if (tokenizer(token, envps, buffers) == EXTRA_ERROR)
		return (EXTRA_ERROR);
	free_strs(buffers);
	return (EXIT_SUCCESS);
}
