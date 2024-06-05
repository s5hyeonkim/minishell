/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 16:17:23 by yubshin           #+#    #+#             */
/*   Updated: 2023/11/02 11:12:47 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;
	size_t	slen;

	slen = ft_strlen(s);
	if (start > slen)
		substr = malloc(sizeof (char) * 1);
	else if (start + len > slen)
		substr = (char *)malloc(sizeof (char) * (slen - start + 1));
	else
		substr = (char *)malloc(sizeof (char) * (len + 1));
	if (substr == NULL)
		return (0);
	i = 0;
	if (!(start > slen))
	{
		while (s[i + start] && start < slen && i < len)
		{
			substr[i] = s[i + start];
			i++;
		}
	}
	substr[i] = '\0';
	return (substr);
}
