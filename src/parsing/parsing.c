/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:39:43 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/27 11:27:02 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	lexer(t_token **token, t_deques *envps, char *buffer);

int	parselines(t_token **token, t_deques *envps, char *buffer)
{
	char	*validbuffer;
	int		code;

	code = get_validbuffer(buffer, &validbuffer);
	if (code == EXIT_SUCCESS)
		code = lexer(token, envps, validbuffer);
	free(validbuffer);
	if (code == EXIT_SUCCESS)
		code = parser(token);
	if (code == EXTRA_ERROR)
	{
		g_status = handle_error(NULL, NULL, EXTRA_ERROR);
		return (EXIT_FAILURE);
	}
	return (code);
}

static int	lexer(t_token **token, t_deques *envps, char *buffer)
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
