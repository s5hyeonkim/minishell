/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sublibft.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:30:41 by yubin             #+#    #+#             */
/*   Updated: 2024/08/19 22:44:32 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	strchrlen(char *str, int chr)
{
	int len ;

	if (ft_strchr(str, chr))
		len = ft_strchr(str, chr) - str;
	else 
		len = ft_strlen(str);
	return (len);
}

//str free
char	*substr_free(char *str, unsigned int start, size_t len)
{
	char	*dst;

	dst = ft_substr(str, start, len);
 	free(str);
	return (dst);
}

//str1, str2 free
char	*strjoin_free(char *str1, char *str2)
{
	char	*dst;

	dst = ft_strjoin(str1, str2);
	free(str1);
	free(str2);
	return (dst);
}

//str, src free
// char	*substrjoin(char *str, int start, int len, char *dst)
// {
// 	char	*src;

// 	src = substr_free(str, start, len);
// 	if (!src)
// 		return (NULL);
// 	dst = strjoin_free(dst, src);
// 	return (dst);
// }

char *substrjoin(char *dst, char *str, int len)
{
	char *src;
	char *tmp;

	src = ft_substr(str, 0, len);
	if (!src)
		return (NULL);
	tmp = dst;
	dst = ft_strjoin(tmp, src);
	if (!dst)
		return (NULL);
	free(tmp);
	return (dst);
}

//str1 free
// char	*strtrim_free(char *str1, char *set)
// {
// 	char	*dst;

// 	dst = ft_strtrim(str1, set);
// 	free(str1);
// 	return (dst);
// }