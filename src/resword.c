/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resword.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:36:17 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/14 14:41:35 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_isredirect(char chr);
int ft_ispipe(char chr);

int ft_isresword(char chr)
{
	if (ft_isredirect(chr) || ft_ispipe(chr))
		return (TRUE);
	return (FALSE);
}

int ft_isredirect(char chr)
{
	if (chr == LESS || chr == GREAT)
		return (TRUE);
	return (FALSE);
}

int ft_ispipe(char chr)
{
	if (chr == PIPE)
		return (TRUE);
	return (FALSE);
}
