/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resword.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:36:17 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/20 16:41:40 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		return (T_SINGLE_QUOTES);
	else if (chr == DBL_QUOTE)
		return (T_DOUBLE_QUOTES);
	return (FALSE);
}
