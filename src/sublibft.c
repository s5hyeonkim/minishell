/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sublibft.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:30:41 by yubin             #+#    #+#             */
/*   Updated: 2024/08/20 16:31:27 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
