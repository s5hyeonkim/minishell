/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 19:54:15 by yubin             #+#    #+#             */
/*   Updated: 2024/08/19 22:53:36 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_ispipeopen(char *buffer)
{
	int		len;
	char	*nextbuffer;

	while(*buffer)
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

int	count_pipe(char *buffer)
{
	int	n;
	int	len;

	n = 1;
	while (buffer && *buffer)
	{	 
		len = wordlen(buffer);
		buffer += len;
		buffer = find_notspace(buffer);
	 	if (ft_ispipe(*buffer))
			n++;
	}
	return (n);
}

int handle_empty_pipe(char *buffer)
{
	int		len;
	int 	pipeopen;

	pipeopen =ft_ispipeopen(buffer);
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

char **split_pipe(char *buffer)
{
	char	**strs; //buffers
	int		pipenum;
	int		len;
	int		index;

	pipenum = count_pipe(buffer);
	// printf("pipenum:%d\n", pipenum);
	strs = ft_calloc(pipenum + 1, sizeof(char *));
	if (!strs)
		return (NULL);
	index = 0;
	len = 0;
	while (index < pipenum)
	{
		// printf("buffer:%s\n", buffer);
		// printf("buffer:%zu\n", ft_strlen(buffer));
		buffer = wordlen_pipe(buffer, &len);
		// printf("len:%d\n", len);
		strs[index] = ft_substr(buffer, 0, len);
		if (!strs[index])
		{
			free_strs(strs);
			return (NULL);
		}
		// printf("strs[%d]:%s\n", index, strs[index]);
		index++;
		buffer += len;
	}
	return (strs);
}
