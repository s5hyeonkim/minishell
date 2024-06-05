/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:04:30 by yubshin           #+#    #+#             */
/*   Updated: 2023/11/03 17:16:14 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t		i;
	char		*dst_tmp;
	const char	*src_tmp;

	i = 0;
	dst_tmp = dst;
	src_tmp = src;
	if (dst == NULL && src == NULL)
		return (0);
	while (i < len)
	{
		if (dst > src)
			dst_tmp[len - 1 - i] = src_tmp[len - 1 - i];
		else
			dst_tmp[i] = src_tmp[i];
		i++;
	}
	return (dst);
}
