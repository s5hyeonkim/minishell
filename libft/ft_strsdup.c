/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsdup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:41:13 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/19 16:41:28 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	**ft_strsdup(char **envp)
{
	size_t	len;
	char	**new;

	len = 0;
	while (envp[len])
		len++;
	new = ft_calloc(len + 1, sizeof(char *));
	len = 0;
	if (new)
	{
		while (envp[len])
		{
			new[len] = ft_strdup(envp[len]);
			len++;
		}
	}
	return (new);
}
