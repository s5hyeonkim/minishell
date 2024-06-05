/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:20:25 by yubshin           #+#    #+#             */
/*   Updated: 2023/11/03 17:15:16 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	*str_tmp;
	unsigned char	c_tmp;

	i = 0;
	str_tmp = (unsigned char *) str;
	c_tmp = (unsigned char) c;
	while (i < n)
	{
		if (str_tmp[i] == c_tmp)
			return (str_tmp + i);
		i++;
	}
	return (0);
}
