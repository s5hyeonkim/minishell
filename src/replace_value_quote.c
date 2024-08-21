/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_value_quote.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:24:38 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/21 11:40:48 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*replace_value(t_deques *envps, char *str);
char	*replace_quote(char *str);

char	*replace_word(t_deques *envps, char *str)
{
	str = replace_value(envps, str);
	if (!str)
		return (NULL);
	str = replace_quote(str);
	if (!str)
		return (NULL);
	return (str);
}

char	*replace_value(t_deques *envps, char *str)
{
	char	*headstr;
	char	*envp;
	char	*dst;
	int		len;

	headstr = str;
	dst = ft_calloc(1, sizeof(char));
	while (*str)
	{
		envp = NULL;
		envp = get_novalue(str, &len);
		if (!envp && *str == DOLLAR)
			envp = get_dollar(str, &len);
		if (!envp && *str == DOLLAR)
			envp = get_status(str, &len);
		if (!envp && *str == DOLLAR)
			envp = get_env(envps, str + 1, &len);
		if (envp)
			dst = strjoin_free(dst, envp);
		str += len;
	}
	free(headstr);
	return (dst);
}

char	*replace_quote(char *str)
{
	char	*headstr;
	char	*dst;
	char	*src;
	int		len;

	headstr = str;
	dst = ft_calloc(1, sizeof(char));
	while (*str)
	{
		len = wordlen(str);
		if (ft_isquote(*str) && is_closed_quotation(str))
			src = ft_substr(str, 1, len - 2);
		else
			src = ft_substr(str, 0, len);
		dst = strjoin_free(dst, src);
		str += len;
	}
	free (headstr);
	return (dst);
}
