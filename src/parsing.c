/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:39:43 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/20 17:00:06 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		lexer(t_token **token, t_deques *envps, char *buffer);

int	parselines(t_shell *shell, char *buffer)
{
	char	*validbuffer;
	int		code;

	validbuffer = NULL;
	code = get_validbuffer(buffer, &validbuffer);
	free(buffer);
	if (code == EXTRA_ERROR || code == SYNTAX_ERROR)
		return (code);
	code = lexer(&(shell->t), shell->data.envps, validbuffer);
	free(validbuffer);
	if (code == EXTRA_ERROR)
		return (code);
	parser(&shell->t);
	return (EXIT_SUCCESS);
}

int	lexer(t_token **token, t_deques *envps, char *buffer)
{
	char	**buffers;

	buffers = split_pipe(buffer);
	if (!buffers)
		return (EXTRA_ERROR);
	/*buffers 출력*/
	// debug_buffers(buffers); 
	if (tokenizer(token, envps, buffers) == EXTRA_ERROR)
		return (EXTRA_ERROR); //leaks
	free_strs(buffers);
	return (EXIT_SUCCESS);
}
