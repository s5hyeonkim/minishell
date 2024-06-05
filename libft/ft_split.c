/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 17:59:40 by yubshin           #+#    #+#             */
/*   Updated: 2024/05/28 20:35:49 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_wrdcnt(char const *str, char c)
{
	size_t	wrdcnt;

	wrdcnt = 0;
	while (*str)
	{
		if (*str != c && (*(str + 1) == c || !*(str + 1)))
			wrdcnt++;
		str++;
	}
	return (wrdcnt);
}

static size_t	ft_lenmal(char const *str, char c, char **res, size_t i)
{
	size_t	wrdlen;

	wrdlen = 0;
	while (*str && *str != c)
	{
		wrdlen++;
		str++;
	}
	res[i] = (char *)malloc(sizeof(char) * (wrdlen + 1));
	if (res[i] == NULL)
		return (0);
	return (wrdlen);
}

static void	ft_lencpy(char **res, const char *str, size_t i, size_t wrdlen)
{
	size_t	j;

	j = 0;
	while (j < wrdlen)
	{
		res[i][j] = str[j];
		j++;
	}
	res[i][j] = '\0';
}

static size_t	ft_malchk(char **res, size_t res_i, size_t wrdlen)
{
	size_t	i;

	i = 0;
	if (wrdlen == 0)
	{	
		while (i < res_i)
		{
			free(res[i]);
			i++;
		}
		free(res);
		return (0);
	}
	return (1);
}

char	**ft_split(char const *str, char c)
{
	char	**res;
	size_t	wrdcnt;
	size_t	wrdlen;
	size_t	i;

	res = 0;
	wrdcnt = ft_wrdcnt(str, c);
	res = (char **)malloc(sizeof (char *) * (wrdcnt + 1));
	if (res == NULL)
		return (0);
	i = 0;
	while (i < wrdcnt)
	{
		while (*str == c)
			str++;
		wrdlen = ft_lenmal(str, c, res, i);
		if (ft_malchk(res, i, wrdlen) == 0)
			return (NULL);
		ft_lencpy(res, str, i, wrdlen);
		str = str + wrdlen;
		i++;
	}
	res[i] = 0;
	return (res);
}
