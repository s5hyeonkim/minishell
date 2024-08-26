/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_value_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:34:43 by yubin             #+#    #+#             */
/*   Updated: 2024/08/26 13:16:07 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	ft_iskeyend(char chr);

char	*get_dollar(char *str, int *len)
{
	char	*dollar;

	dollar = ft_substr(str, 0, 1);
	*len = 1;
	if (!dollar)
		return (NULL);
	return (dollar);
}

char	*get_status(int *len)
{
	char	*status;

	status = ft_itoa(g_status);
	*len = 2;
	if (!status)
		return (NULL);
	return (status);
}

char	*get_env(t_deques *envps, char *word, int *len)
{
	char	*now;
	char	*envp;
	char	*key;

	*len = 0;
	now = word;
	while (*now && ft_iskeyend(*now) == FALSE)
	{
		now++;
		(*len)++;
	}
	key = ft_substr(word, 0, *len);
	*len = ft_strlen(key) + 1;
	if (!key)
		return (NULL);
	envp = get_val_deq(envps, key);
	free(key);
	if (!envp)
		return (NULL);
	return (envp);
}

static int	ft_iskeyend(char chr)
{
	if (chr == DOLLAR || chr == SPACE || chr == '/' \
	|| chr == SGL_QUOTE || chr == DBL_QUOTE)
		return (TRUE);
	return (FALSE);
}
