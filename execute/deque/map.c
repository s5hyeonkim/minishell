/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pair.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:50:35 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/11 12:50:37 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "deque.h"

char	*ft_pairjoin(char *key, char mid, char *val)
{
	char	*temp;
	char	*ret;
    char    m[2];

	ret = NULL;
    m[0] = mid;
    m[1] = 0;
	temp = ft_strjoin(key, m);
	if (!temp)
		return (NULL);
	ret = ft_strjoin(temp, val);
	free(temp);
	return (ret);
}

char    *get_key(char *str)
{
    char    *key;

    if (ft_strchr(str, '='))
        key = ft_substr(str, 0, ft_strchr(str, '=') - str);
    else
        key = ft_strdup(str);
    return (key);
}

char    *get_value(char *str)
{
    char    *val;

    if (ft_strchr(str, '='))
        val = ft_substr(str, ft_strchr(str, '=') - str + 1, ft_strlen(str));
    else
        val = ft_strdup("");
    return (val);
}

int	set_map(char *str, t_map *keyval)
{
    ft_memset(keyval, 0, sizeof(keyval));
	if (ft_strchr(str, '='))
		keyval->mid = '=';
    keyval->key = get_key(str);
    keyval->val = get_value(str);
	if (!keyval->key || !keyval->val)
	{
		free_map(*keyval);
		keyval->key = NULL;
		keyval->val = NULL;
		return (EXTRA_ERROR);
	}
	return (EXIT_SUCCESS);
}
