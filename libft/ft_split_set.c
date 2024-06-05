/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_set.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 20:25:30 by yubshin           #+#    #+#             */
/*   Updated: 2024/05/28 21:46:27 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strin(char str, char *charset)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (str == charset[i])
			return (-1);
		i++;
	}
	return (1);
}

int	ft_wrdcnt(char *str, char *charset)
{
	int	i;
	int	wrdcnt;
	int	idx_0;
	int	idx_1;

	i = 0;
	wrdcnt = 0;
	while (str[i])
	{
		idx_0 = ft_strin(str[i], charset);
		idx_1 = ft_strin(str[i + 1], charset);
		if (idx_0 == 1 && (idx_1 == -1 || str[i + 1] == '\0'))
			wrdcnt++;
		i++;
	}
	return (wrdcnt);
}

char	*ft_strcpy(char *str, char **split, int spl_idx, int strlen)
{
	int	idx;

	idx = 0;
	while (idx < strlen)
	{
		split[spl_idx][idx] = *str;
		idx++;
		str++;
	}
	split[spl_idx][strlen] = '\0';
	return (str);
}

int	ft_strmal(char *str, char *charset, char **split, int wrdcnt)
{
	int	spl_idx;
	int	strlen;

	spl_idx = 0;
	while (spl_idx < wrdcnt)
	{
		while (ft_strin(*str, charset) == -1)
			str++;
		strlen = 0;
		while (ft_strin(str[strlen], charset) == 1 && str[strlen])
			strlen++;
		split[spl_idx] = (char *)malloc(sizeof (char) * (strlen + 1));
		if (split[spl_idx] == NULL)
			return (0);
		ft_strcpy(str, split, spl_idx, strlen);
		str = str + strlen;
		spl_idx++;
	}
	split[spl_idx] = 0;
	return (1);
}

char	**ft_split_set(char *str, char *charset)
{
	int		wrdcnt;
	char	**split;

	wrdcnt = ft_wrdcnt(str, charset);
	split = (char **) malloc (sizeof (char *) * (wrdcnt + 1));
	if (split == NULL)
		return (0);
	ft_strmal(str, charset, split, wrdcnt);
	return (split);
}