/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resword.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:36:17 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/27 12:22:35 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ft_isresword(char chr)
{
	if (ft_isredirect(chr) || ft_ispipe(chr))
		return (TRUE);
	return (FALSE);
}

int	ft_isredirect(char chr)
{
	if (chr == LESS || chr == GREAT)
		return (TRUE);
	return (FALSE);
}

int	ft_ispipe(char chr)
{
	if (chr == PIPE)
		return (TRUE);
	return (FALSE);
}

int	ft_isquote(char chr)
{
	if (chr == SGL_QUOTE)
		return (T_SGL_QUOTE);
	else if (chr == DBL_QUOTE)
		return (T_DBL_QUOTE);
	return (FALSE);
}
