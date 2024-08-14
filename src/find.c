/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:39:11 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/14 14:41:51 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char    *find_redirect_start(char *str)
{
	while (*str && !ft_isredirect(*str))
		str++;
	return (str);
}

char    *find_filename_start(char *str)
{
	while (ft_isspace(*str))
		str++;
	if (*str == LESS && *(str + 1) == LESS)
		str += 2;
	else if (*str == GREAT && *(str + 1) == GREAT)
		str += 2;
	else if (*str == LESS || *str == GREAT)
		str++;
	while (ft_isspace(*str))
		str++;
	return (str);
}

char    *find_pipeend(char *buffer)
{
	while (buffer && *buffer)
	{	
		if (*buffer == SGL_QUOTE || *buffer == DBL_QUOTE)
		{
			buffer = find_wordend(buffer, 0);
			if (!buffer)
				return (buffer);
		}
		else if (*buffer == PIPE)
			return (buffer);
		buffer++;
	}
	return (NULL);
}
