/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 16:41:16 by yubshin           #+#    #+#             */
/*   Updated: 2023/11/02 11:14:18 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*jstr;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	jstr = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (jstr == NULL)
		return (0);
	ft_strlcpy(jstr, s1, s1_len + 1);
	ft_strlcat(jstr, s2, s1_len + s2_len + 1);
	return (jstr);
}
