/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 16:48:30 by sohykim           #+#    #+#             */
/*   Updated: 2024/04/11 16:49:22 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_realloc(void *ptr, size_t size)
{
	void	*dst;

	dst = NULL;
	if (size)
	{
		dst = ft_calloc(size, 1);
		if (dst && ptr)
			ft_memcpy(dst, ptr, size);
	}
	free(ptr);
	return (dst);
}
