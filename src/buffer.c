/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 19:47:20 by yubin             #+#    #+#             */
/*   Updated: 2024/08/19 19:53:07 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_pipebuffer(char *buffer, char **dstbuffer);

int	get_validbuffer(char *buffer, char **validbuffer)
{
	char 	*tmp;
	int		code;

	*validbuffer = ft_strdup(buffer);
	while(1)
	{
		if (handle_empty_pipe(*validbuffer) == SYNTAX_ERROR \
		|| handle_empty_redirect(*validbuffer) == SYNTAX_ERROR)
		{
			free(*validbuffer);
			return (SYNTAX_ERROR);
		}
		else if (ft_ispipeopen(*validbuffer) == TRUE) //열린 pipe 확인
		{
			tmp = *validbuffer;
			code = get_pipebuffer(tmp, validbuffer);
			free(tmp);
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
	char *srcbuffer;

	if (set_next_cmd(&srcbuffer))
	{
		*dstbuffer = NULL;
		g_status = handle_error(NULL, NULL, SYN_TERM);
		return (g_status);
	}
	else
		*dstbuffer = strjoin_free(buffer, srcbuffer);
	if (!*dstbuffer)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}
