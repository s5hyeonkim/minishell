/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 19:47:20 by yubin             #+#    #+#             */
/*   Updated: 2024/08/20 13:06:12 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_pipebuffer(char *buffer, char **dstbuffer);

int	get_validbuffer(char *buffer, char **validbuffer)
{
	int		code;

	*validbuffer = ft_strdup(buffer);
	while (1)
	{
		if (handle_empty_pipe(*validbuffer) == SYNTAX_ERROR \
		|| handle_empty_redirect(*validbuffer) == SYNTAX_ERROR)
		{
			free(*validbuffer);
			return (SYNTAX_ERROR);
		}
		else if (ft_ispipeopen(*validbuffer) == TRUE)
		{
			buffer = *validbuffer;
			code = get_pipebuffer(buffer, validbuffer);
			free(buffer);
			if (code == EXTRA_ERROR || code == SYNTAX_ERROR)
				return (code);
		}
		else
			break ;
	}
	return (EXIT_SUCCESS);
}

int	get_pipebuffer(char *buffer, char **dstbuffer)
{
	char	*srcbuffer;

	if (set_next_cmd(&srcbuffer))
	{
		*dstbuffer = NULL;
		g_status = handle_error(NULL, NULL, SYN_TERM);
		return (g_status);
	}
	else
		*dstbuffer = ft_strjoin(buffer, srcbuffer);
	if (!*dstbuffer)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}
