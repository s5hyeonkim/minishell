/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordend.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:37:16 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/21 14:01:37 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*find_quotend(char *str);
int		ft_iswordhead(char chr);

char	*find_wordend(char *str)
{
	str = find_notspace(str);
	if (*str == SGL_QUOTE || *str == DBL_QUOTE)
		str = find_quotend(str);
	else
		while (str && *str && ft_iswordhead(*str) == FALSE)
			str++;
	return (str);
}

char	*find_quotend(char *str)
{
	int		sgl_open;
	int		dbl_open;

	sgl_open = 0;
	dbl_open = 0;
	while (*str)
	{
		if (*str == SGL_QUOTE && !sgl_open && dbl_open && dbl_open--)
			return (strchr_end(str, DBL_QUOTE));
		else if (*str == SGL_QUOTE && !sgl_open && !dbl_open)
			sgl_open = TRUE;
		else if (*str == DBL_QUOTE && !dbl_open && sgl_open && sgl_open--)
			return (strchr_end(str, SGL_QUOTE));
		else if (*str == DBL_QUOTE && !dbl_open && !sgl_open)
			dbl_open = TRUE;
		else if ((*str == SGL_QUOTE && sgl_open--) \
			|| (*str == DBL_QUOTE && dbl_open--))
			return (str + 1);
		str++;
	}
	return (str);
}

char	*is_closed_quotation(char *str)
{
	int		sgl_open;
	int		dbl_open;

	sgl_open = 0;
	dbl_open = 0;
	while (*str)
	{
		if (*str == SGL_QUOTE && !sgl_open && dbl_open && dbl_open--)
			return (ft_strchr(str, DBL_QUOTE));
		else if (*str == SGL_QUOTE && !sgl_open && !dbl_open)
			sgl_open = TRUE;
		else if (*str == DBL_QUOTE && !dbl_open && sgl_open && sgl_open--)
			return (ft_strchr(str, SGL_QUOTE));
		else if (*str == DBL_QUOTE && !dbl_open && !sgl_open)
			dbl_open = TRUE;
		else if ((*str == SGL_QUOTE && sgl_open--) \
			|| (*str == DBL_QUOTE && dbl_open--))
			return (str + 1);
		str++;
	}
	if (sgl_open || dbl_open)
		return (NULL);
	return (str);
}

int	ft_iswordhead(char chr)
{
	if (ft_isspace(chr) == TRUE \
	|| ft_ispipe(chr) == TRUE \
	|| ft_isquote(chr) != FALSE \
	|| ft_isredirect(chr) == TRUE)
		return (TRUE);
	return (FALSE);
}
