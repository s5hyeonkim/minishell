/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:25:03 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/26 13:15:27 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static char	*get_value_oneword(t_deques *envps, char *word);
static char	*get_nodollar_value(char *word, int *len);
static char	*get_dollar_value(t_deques *envps, char *word, int *len);

char	*replace_value(t_deques *envps, char *str)
{
	char	*headstr;
	char	*word;
	char	*dst;
	char	*value;
	int		len;

	headstr = str;
	dst = ft_calloc(1, sizeof(char));
	while (*str && dst)
	{
		len = wordlen(str);
		word = ft_substr(str, 0, len);
		if (word)
			value = get_value_oneword(envps, word);
		if (value)
			dst = strjoin_free(dst, value);
		if (!word || !value)
			break ;
		free(word);
		str += len;
	}
	free(headstr);
	return (dst);
}

static char	*get_value_oneword(t_deques *envps, char *word)
{
	char	*dst;
	char	*value;
	int		len;

	len = 0;
	dst = NULL;
	while (*word)
	{
		value = get_nodollar_value(word, &len);
		if (!value && len)
			return (NULL);
		if (!len)
			value = get_dollar_value(envps, word, &len);
		dst = strjoin_free(dst, value);
		if (!dst)
			return (NULL);
		word += len;
	}
	return (dst);
}

static char	*get_nodollar_value(char *word, int *len)
{
	char	*dst;

	dst = NULL;
	*len = 0;
	if (*word == SPACE)
	{
		dst = ft_strdup(" ");
		*len = find_notspace(word) - word;
	}
	else if ((*find_notspace(word) == SGL_QUOTE \
		&& is_closed_quotation(find_notspace(word))) \
		|| !ft_strchr(word, DOLLAR))
	{
		dst = ft_strdup(word);
		*len = ft_strlen(word);
	}
	return (dst);
}

static char	*get_dollar_value(t_deques *envps, char *word, int *len)
{
	char	*dollarhead;
	char	*dst;
	char	*value;

	*len = 0;
	dollarhead = ft_strchr(word, DOLLAR);
	dst = ft_substr(word, 0, dollarhead - word);
	if (*(dollarhead + 1) == DOLLAR || ft_isspace(*(dollarhead + 1)) \
	|| !*(dollarhead + 1) || ft_isquote(*(dollarhead + 1)))
		value = get_dollar(dollarhead, len);
	else if (*(dollarhead + 1) == '?')
		value = get_status(len);
	else
		value = get_env(envps, dollarhead + 1, len);
	if (!value)
		return (value);
	*len += dollarhead - word;
	dst = strjoin_free(dst, value);
	return (dst);
}
