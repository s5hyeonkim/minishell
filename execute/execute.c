/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:31:42 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/23 19:31:44 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execute.h"
// stdout으로 없는 내용 쓰는 것 구현하면 마지막 exit code가 훼손될 수 있음.

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
