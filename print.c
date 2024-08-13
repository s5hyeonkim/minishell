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

const char	*err_to_msg(int *code)
{
	if (*code == CMD_NOT_FOUND)
		return (MSG_CMD);
	if (*code == SYN_TOK)
	{
		*code = SYNTAX_ERROR;
		return (MSG_SYN_TOK);
	}
	if (*code == SYN_TERM)
	{
		*code = SYNTAX_ERROR;
		return (MSG_SYN_TERM);
	}
	*code = EXIT_FAILURE;
	if (*code == INVALID_ARGV)
		return (MSG_ARGV);
	if (*code == INVALID_OPT)
		return (MSG_OPT);
	if (*code == INVALID_IDF)
		return (MSG_IDF);
	if (*code == NOT_SET)
		return (MSG_SET);
	if (*code == NOT_NUM)
		return (MSG_NUM);
	return (strerror(errno));
}

int	print_error(int code)
{
	size_t		size;
	const char	*msg = err_to_msg(&code);

	size = ft_strlen(msg);
	write(STDERR_FILENO, msg, size);
	return (code);
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
	return (print_error(code));
}
