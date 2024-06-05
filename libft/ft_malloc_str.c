/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 19:00:51 by yubshin           #+#    #+#             */
/*   Updated: 2024/03/27 19:01:04 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_malloc_str(char **str, long long malloc_size)
{
	(*str) = (char *)malloc(sizeof(char) * malloc_size + 1);
	if ((*str) == NULL)
		return (0);
	ft_memset(*str, 0, malloc_size + 1);
	return (1);
}
