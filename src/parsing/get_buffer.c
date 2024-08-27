/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_buffer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 19:47:20 by yubin             #+#    #+#             */
/*   Updated: 2024/08/27 11:32:34 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	replace_pipebuffer(char **dstbuffer);

int	get_validbuffer(char *buffer, char **validbuffer)
{
	int		code;

	*validbuffer = ft_strdup(buffer);
	if (!*validbuffer)
		return (EXTRA_ERROR);
	while (1)
	{
		if (handle_empty_pipe(*validbuffer) == SYNTAX_ERROR \
		|| handle_empty_redirect(*validbuffer) == SYNTAX_ERROR)
		{
			g_status = SYNTAX_ERROR;
			return (SYNTAX_ERROR);
		}
		else if (ft_ispipeopen(*validbuffer) == TRUE)
		{
			code = replace_pipebuffer(validbuffer);
			if (code == SYNTAX_ERROR)
				g_status = SYNTAX_ERROR;
			if (code != EXIT_SUCCESS)
				return (code);
		}
		else
			break ;
	}
	return (EXIT_SUCCESS);
}

static int	replace_pipebuffer(char **dstbuffer)
{
	char	*srcbuffer;

	if (set_next_cmd(&srcbuffer))
	{
		free(*dstbuffer);
		free(srcbuffer);
		*dstbuffer = NULL;
		return (handle_error(NULL, NULL, SYN_TERM));
	}
	else
		*dstbuffer = strjoin_free(*dstbuffer, srcbuffer);
	if (!*dstbuffer)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}
