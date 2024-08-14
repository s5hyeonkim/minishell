/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sublibft.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:30:41 by yubin             #+#    #+#             */
/*   Updated: 2024/08/13 13:32:13 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ismadeofchr(char *str, char chr)
{
	while (str && *str)
	{
		if (*str != chr)
			return (FALSE);
		str++;
	}
	return (TRUE);
}

//str free
char	*substr_free(char *str, unsigned int start, size_t len)
{
	char	*dst;

	dst = ft_substr(str, start, len);
	if (str)
		free(str);
	// if (!dst)
		// return (NULL);
	return (dst);
}

//str1, str2 free
char	*strjoin_free(char *str1, char *str2)
{
	char	*dst;

	dst = ft_strjoin(str1, str2);
	if (str1)
		free(str1);
	if (str2)
		free(str2);
	// if (!dst)
		// return (NULL);
	return (dst);
}

//str, src free
char	*substrjoin (char *str, int start, int len, char *dst)
{
	char	*src;

	src = substr_free(str, start, len);
	if (!src)
		return (NULL);
	dst = strjoin_free(dst, src);
	// if (!dst)
	// 	return (NULL);
	return (dst);
}

//str1 free
char	*strtrim_free(char *str1, char *set)
{
	char	*dst;

	dst = ft_strtrim(str1, set);
	if (str1)
		free(str1);
	// if (!dst)
	// 	return (NULL);
	return (dst);
}