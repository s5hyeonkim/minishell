/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:39:11 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/20 12:21:59 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_notspace(char *str)
{
	while(str && *str && ft_isspace(*str))
		str++;
	return (str);
}

int	handle_error_withstr(char *exec, char *str, int len, int code)
{
	char	*obj;

	obj = ft_substr(str, 0, len);
	g_status = handle_error(exec, obj, code);
	free(obj);
	return (g_status);
}