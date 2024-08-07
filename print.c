/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:51:46 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/24 20:34:55 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_err.h"

int	ft_putstr_err(t_err code)
{
	const char	*msg[] = {"", "", "", MSG_ARGV, MSG_OPT, MSG_IDF, MSG_SET, MSG_NUM}; 

	if (code == CMD_NOT_FOUND)
	{
		ft_putstr_fd(MSG_CMD, STDERR_FILENO);
		return (code);
	}
	else if (code == SYN_TOK)
	{
		ft_putstr_fd(MSG_SYN_TOK, STDERR_FILENO);
		return (SYNTAX_ERROR);
	}
	else if (code == SYN_TERM)
	{
		ft_putstr_fd(MSG_SYN_TERM, STDERR_FILENO);
		return (SYNTAX_ERROR);
	}
	if (code == EXTRA_ERROR)
		perror("");
	else
		write(STDERR_FILENO, msg[code], ft_strlen(msg[code]));
	return (EXIT_FAILURE);
}

int	handle_error(char *exec, char *obj, int code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (exec)
	{
		ft_putstr_fd(exec, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (obj)
	{
		ft_putstr_fd(obj, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	return (ft_putstr_err(code));
}
