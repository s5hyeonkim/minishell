/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_value_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:34:43 by yubin             #+#    #+#             */
/*   Updated: 2024/08/20 16:40:47 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_iskeyend(char chr);

char	*get_novalue(char *str, int *len)
{
	char	*novalue;

	novalue = NULL;
	*len = strchrlen(str, DOLLAR);
	if (*len)
		novalue = ft_substr(str, 0, *len);
	return (novalue);
}

char	*get_dollar(char *str, int *len)
{
	char	*res;

	res = NULL;
	if (*(str + 1) == DOLLAR || ft_isspace(*(str + 1)) || !*(str + 1))
	{
		res = ft_substr(str, 0, 1);
		*len = 1;
	}
	return (res);
}

char	*get_status(char *str, int *len)
{
	char	*envp;

	envp = NULL;
	str++;
	if (*str == '?')
	{
		envp = ft_itoa(g_status);
		if (!envp)
			return (NULL);
	}
	*len = 2;
	return (envp);
}

char	*get_env(t_deques *envps, char *keyhead, int *len)
{
	char	*now;
	char	*envp;
	char	*key;

	envp = NULL;
	*len = 0;
	now = keyhead;
	while (*now && ft_iskeyend(*now) == FALSE)
	{
		now++;
		(*len)++;
	}
	key = ft_substr(keyhead, 0, *len);
	if (!key)
		return (NULL);
	envp = get_val_deq(envps, key);
	*len = ft_strlen(key) + 1;
	free(key);
	return (envp);
}

int	ft_iskeyend(char chr)
{
	if (chr == DOLLAR || chr == SPACE || chr == '/' \
	|| chr == SGL_QUOTE || chr == DBL_QUOTE)
		return (TRUE);
	return (FALSE);
}
