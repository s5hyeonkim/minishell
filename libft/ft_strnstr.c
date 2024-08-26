/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 11:20:44 by sohykim           #+#    #+#             */
/*   Updated: 2023/10/24 11:57:38 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *h, const char *needle, size_t len)
{
	size_t	index;
	size_t	id;

	index = 0;
	id = 0;
	if (!ft_strlen(needle))
		return ((char *) h);
	if (len > ft_strlen(h))
		len = ft_strlen(h);
	while (index + id < len + 1)
	{
		if (h[index + id] && needle[id] && h[index + id] == needle[id])
			id++;
		else if (id == ft_strlen(needle))
			return ((char *)&h[index]);
		else
		{
			index++;
			id = 0;
		}
	}
	return (0);
}
