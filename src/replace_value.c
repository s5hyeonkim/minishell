/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:33:11 by yubin             #+#    #+#             */
/*   Updated: 2024/08/20 10:52:35 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*replace_value(t_deques *envps, char *str)
{
	char	*headstr;
	char	*envp;
	char	*dst;
	int		len;

	headstr = str;
	dst = ft_calloc(1, sizeof(char));
	// ft_memset(&dst, 1, sizeof(char));
	while (*str)
	{
		// printf("str:%s\n", str);
		envp = NULL;
		envp = get_novalue(str, &len);
		if (!envp && *str == DOLLAR)
			envp = get_dollar(str, &len);
		if (!envp && *str == DOLLAR)
			envp = get_status(str, &len);
		if (!envp && *str == DOLLAR)
			envp = get_env(envps, str + 1, &len);
		// printf("envp:%s\n", envp);
		if (envp)
			dst = strjoin_free(dst, envp);
		str += len;
	}
	// printf("dst:%s\n", dst);
	free(headstr);
	return (dst);
}
