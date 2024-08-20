/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 19:54:15 by yubin             #+#    #+#             */
/*   Updated: 2024/08/20 14:02:29 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_ispipeopen(char *buffer);
int	count_pipe(char *buffer);

int	handle_empty_pipe(char *buffer)
{
	int	len;
	int	pipeopen;

	pipeopen = ft_ispipeopen(buffer);
	while (ft_isspace(*buffer) || ft_isredirect(*buffer))
		buffer++;
	if (ft_ispipe(*buffer))
		return (handle_error_withstr(NULL, buffer, 1, SYN_TOK));
	while (*buffer)
	{
		buffer = wordlen_pipe(buffer, &len);
		buffer += len;
		buffer = find_notspace(buffer);
		if (pipeopen == FALSE)
		{	
			buffer++;
			buffer = find_notspace(buffer);
			if (ft_ispipe(*buffer))
				return (handle_error_withstr(NULL, buffer, 1, SYN_TOK));
		}
	}
	return (EXIT_SUCCESS);
}

int	ft_ispipeopen(char *buffer)
{
	int		len;
	char	*nextbuffer;

	while (*buffer)
	{
		buffer = wordlen_pipe(buffer, &len);
		buffer += len;
		buffer = find_notspace(buffer);
		if (ft_ispipe(*buffer))
		{
			nextbuffer = buffer + 1;
			nextbuffer = find_notspace(nextbuffer);
			if (!*nextbuffer)
				return (TRUE);
		}
		buffer++;
	}
	return (FALSE);
}

char	**split_pipe(char *buffer)
{
	char	**strs;
	int		pipenum;
	int		len;
	int		index;

	pipenum = count_pipe(buffer);
	strs = ft_calloc(pipenum + 1, sizeof(char *));
	if (!strs)
		return (NULL);
	index = 0;
	len = 0;
	while (index < pipenum)
	{
		buffer = wordlen_pipe(buffer, &len);
		strs[index] = ft_substr(buffer, 0, len);
		if (!strs[index])
		{
			free_strs(strs);
			return (NULL);
		}
		index++;
		buffer += len;
	}
	return (strs);
}

int	count_pipe(char *buffer)
{
	int	pipenum;
	int	len;

	pipenum = 1;
	while (buffer && *buffer)
	{	
		len = wordlen(buffer);
		buffer += len;
		buffer = find_notspace(buffer);
		if (ft_ispipe(*buffer))
			pipenum++;
	}
	return (pipenum);
}
