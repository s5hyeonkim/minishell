/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordend.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:37:16 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/20 16:50:05 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_quotend(char *str, int flag);
int		ft_iswordhead(char chr);

char	*find_wordend(char *str)
{
	str = find_notspace(str);
	if (*str == SGL_QUOTE || *str == DBL_QUOTE)
		str = find_quotend(str, ft_isquote(*str));
	else
		while (str && *str && ft_iswordhead(*str) == FALSE)
			str++;
	return (str);
}

int	ft_isquotend(char *str, int flag)
{
	char	*quotend;
	int		sgl_open;
	int		dbl_open;

	sgl_open = 0;
	dbl_open = 0;
	if (*str != SGL_QUOTE && *str != DBL_QUOTE)
		return (EXIT_FAILURE);
	while (*str)
	{
		if (*str == SGL_QUOTE && !sgl_open && dbl_open && dbl_open--)
		{
			quotend = ft_strchr(str, DBL_QUOTE);
			if (quotend)
				return (EXIT_SUCCESS);
			else
				return (EXIT_FAILURE);
		}
		else if (*str == SGL_QUOTE && !sgl_open && !dbl_open)
			sgl_open = TRUE;
		else if (*str == DBL_QUOTE && !dbl_open && sgl_open && sgl_open--)
		{
			quotend = ft_strchr(str, SGL_QUOTE);
			if (quotend)
				return (EXIT_SUCCESS);
			else
				return (EXIT_FAILURE);
		}
		else if (*str == DBL_QUOTE && !dbl_open && !sgl_open)
			dbl_open = TRUE;
		else if ((*str == SGL_QUOTE && sgl_open--) || (*str == DBL_QUOTE && dbl_open--))
			break ;
		str++;
	}
	(void) flag;
	// if ((flag == T_SINGLE_QUOTES && sgl_open) || (flag == T_DOUBLE_QUOTES && dbl_open))
	// 	return (NULL);
	return (EXIT_SUCCESS);
}

char	*find_quotend(char *str, int flag)
{
	char	*quotend;
	int		sgl_open;
	int		dbl_open;

	sgl_open = 0;
	dbl_open = 0;
	while(*str)
	{
		// printf("str:%s\n", str);
		// printf("sgl:%d\n", sgl_open);
		// printf("dbl:%d\n", dbl_open);
		if (*str == SGL_QUOTE && !sgl_open && dbl_open && dbl_open--)
		{
			quotend = ft_strchr(str, DBL_QUOTE);
			if (quotend)
				return (quotend + 1);
			// break ;
			else 
				return (str + ft_strlen(str));
		}
		else if (*str == SGL_QUOTE && !sgl_open && !dbl_open)
			sgl_open = TRUE;
		else if (*str == DBL_QUOTE && !dbl_open && sgl_open && sgl_open--)
		{
			quotend = ft_strchr(str, SGL_QUOTE);
			if (quotend)
				return(quotend + 1);
			else 
				return (str + ft_strlen(str));
			// break ;
		}
		else if (*str == DBL_QUOTE && !dbl_open && !sgl_open)
			dbl_open = TRUE;
		else if	((*str == SGL_QUOTE && sgl_open--) || (*str == DBL_QUOTE  && dbl_open-- ))
			break;
		str++;
	}
	(void) flag;
	// if ((flag == T_SINGLE_QUOTES && sgl_open) || (flag == T_DOUBLE_QUOTES && dbl_open))
		// return (str + ft_strlen(str));
		// return (NULL);
	return (str + 1);
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
