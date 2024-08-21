/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:31:42 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/14 19:19:22 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execute.h"

t_builtin	find_builtin(int index)
{
	t_builtin	fp[7];

	fp[_ECHO] = ft_echo;
	fp[_CD] = ft_cd;
	fp[_PWD] = ft_pwd;
	fp[_EXPORT] = ft_export;
	fp[_UNSET] = ft_unset;
	fp[_ENV] = ft_env;
	fp[_EXIT] = ft_exit;
	return (fp[index]);
}
