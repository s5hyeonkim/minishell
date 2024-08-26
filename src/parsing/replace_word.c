/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:12:31 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/26 13:14:25 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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
