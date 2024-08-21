/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:42:55 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/21 13:42:57 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*read_val_strs(char *strs[], char *key)
{
	char	*ret;
	int		index;
	size_t	keylen;

	index = 0;
	ret = NULL;
	if (!strs)
		return (NULL);
	while (strs[index])
	{
		keylen = ft_strlen(key);
		if (!ft_memcmp(strs[index], key, keylen) \
		&& *(strs[index] + keylen) == '=')
		{
			ret = strs[index] + keylen + 1;
			break ;
		}
		index++;
	}
	return (ret);
}

char	**get_env_paths(char *envp[])
{
	char	**ret;
	char	*paths;

	paths = read_val_strs(envp, "PATH");
	if (paths)
		ret = ft_split(paths, ':');
	else
		ret = ft_calloc(1, sizeof(char *));
	return (ret);
}

void	ft_sort(char **strs, int (*guide)(char *, char *))
{
	char	*temp;
	int		index;
	int		index2;

	index = 0;
	while (strs[index])
	{
		index2 = index + 1;
		while (strs[index2])
		{
			if (guide(strs[index], strs[index2]) > 0)
			{
				temp = strs[index];
				strs[index] = strs[index2];
				strs[index2] = temp;
			}
			index2++;
		}
		index++;
	}
}

int	get_depth(char *str)
{
	int	num;

	num = 0;
	if (!str)
		return (0);
	while (*str && num < 1000)
	{
		if (!ft_isdigit(*str))
			return (0);
		num = num * 10 + *str - '0';
		str++;
	}
	if (num >= 1000)
		return (0);
	return (num);
}
