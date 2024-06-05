/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 19:21:49 by yubshin           #+#    #+#             */
/*   Updated: 2023/11/09 13:15:11 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dstlen;
	size_t	srclen;
	size_t	dstidx;
	size_t	srcidx;

	srclen = ft_strlen(src);
	if (dst == NULL && dstsize == 0)
		return (srclen);
	dstlen = ft_strlen(dst);
	dstidx = dstlen;
	srcidx = 0;
	while (src[srcidx] && dstlen + srcidx + 1 < dstsize)
	{
			dst[dstidx] = src[srcidx];
			dstidx++;
			srcidx++;
	}
	dst[dstidx] = '\0';
	if (dstlen < dstsize)
		return (srclen + dstlen);
	else if (dstlen >= dstsize)
		return (srclen + dstsize);
	else
		return (0);
}
