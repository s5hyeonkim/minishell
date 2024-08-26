/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:30:41 by yubin             #+#    #+#             */
/*   Updated: 2024/08/23 13:46:19 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	strchrlen(char *str, int chr)
{
	int	len ;

	if (ft_strchr(str, chr))
		len = ft_strchr(str, chr) - str;
	else
		len = ft_strlen(str);
	return (len);
}

char	*strjoin_free(char *dst, char *src)
{
	char	*tmp;

	tmp = dst;
	dst = ft_strjoin(tmp, src);
	free(tmp);
	free(src);
	return (dst);
}

char	*substrjoin_free(char *dst, char *str, int len)
{
	char	*src;
	char	*tmp;

	tmp = dst;
	src = ft_substr(str, 0, len);
	if (!src)
	{	
		free(tmp);
		return (NULL);
	}
	dst = ft_strjoin(tmp, src);
	free(tmp);
	free(src);
	if (!dst)
		return (NULL);
	return (dst);
}

char	*strchr_end(char *str, int chr)
{
	char	*res;

	res = ft_strchr(str, chr);
	if (!res)
		res = str + ft_strlen(str);
	else
		res++;
	return (res);
}

char	*find_notspace(char *str)
{
	while (str && *str && ft_isspace(*str))
		str++;
	return (str);
}
