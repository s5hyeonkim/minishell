/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pair.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:50:35 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/23 19:37:54 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "deque.h"

char	*ft_pairjoin(t_map *keyval)
{
	size_t	len;
	char	*str;

	len = ft_strlen(keyval->key);
	if (keyval->mid)
		len++;
	len += ft_strlen(keyval->val);
	str = ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	ft_strlcat(str, keyval->key, len + 1);
	if (keyval->mid)
		ft_strlcat(str, "=", len + 1);
	ft_strlcat(str, keyval->val, len + 1);
	return (str);
}

char	*get_key(char *str)
{
	char	*key;
	char	*target;

	target = ft_strchr(str, '=');
	if (target)
	{
		if (str < target && str[target - str - 1] == '+')
			key = ft_substr(str, 0, ft_strchr(str, '=') - str - 1);
		else
			key = ft_substr(str, 0, ft_strchr(str, '=') - str);
	}
	else
		key = ft_strdup(str);
	return (key);
}

char	*get_value(char *str)
{
	char	*val;

	if (ft_strchr(str, '='))
		val = ft_substr(str, ft_strchr(str, '=') - str + 1, ft_strlen(str));
	else
		val = ft_strdup("");
	return (val);
}

int	set_map(t_map *keyval, char *str)
{
	ft_memset(keyval, 0, sizeof(keyval));
	keyval->state = EXPORT;
	if (ft_strchr(str, '='))
	{
		keyval->mid = '=';
		keyval->state = ENV;
	}
	keyval->key = get_key(str);
	keyval->val = get_value(str);
	if (!keyval->key || !keyval->val)
	{
		free_map(keyval);
		return (EXTRA_ERROR);
	}
	return (EXIT_SUCCESS);
}

int	set_keyval(t_map *keyval, char *key, char *val, t_state state)
{
	ft_memset(keyval, 0, sizeof(t_map));
	keyval->mid = 0;
	keyval->key = ft_strdup(key);
	if (keyval->key)
		keyval->val = ft_strdup(val);
	if (!keyval->key || !keyval->val)
	{
		free_map(keyval);
		return (EXTRA_ERROR);
	}
	if (state == ENV)
		keyval->mid = '=';
	return (EXIT_SUCCESS);
}
