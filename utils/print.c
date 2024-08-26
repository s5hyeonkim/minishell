/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:51:46 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/26 17:15:53 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_err.h"

const char	*err_to_msg(int *code)
{
	int	err;

	err = *code;
	if (err == CMD_NOT_FOUND)
		return (MSG_CMD);
	*code = SYNTAX_ERROR;
	if (err == SYN_TOK)
		return (MSG_SYN_TOK);
	if (err == SYN_TERM)
		return (MSG_SYN_TERM);
	*code = EXIT_FAILURE;
	if (err == INVALID_ARGV)
		return (MSG_ARGV);
	if (err == INVALID_OPT)
		return (MSG_OPT);
	if (err == INVALID_IDF)
		return (MSG_IDF);
	if (err == NOT_SET)
		return (MSG_SET);
	if (err == NOT_NUM)
		return (MSG_NUM);
	if (err == EXTRA_ERROR)
		return (strerror(errno));
	return ("");
}

int	print_error(int code)
{
	size_t		size;
	const char	*msg = err_to_msg(&code);

	size = ft_strlen(msg);
	if (size)
		write(STDERR_FILENO, msg, size);
	if (size && msg[size - 1] != '\n')
		write(STDERR_FILENO, "\n", 1);
	return (code);
}

int	handle_error(char *exec, char *obj, int code)
{
	if (code == EXIT_FAILURE)
		return (code);
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

int	handle_error_withstr(char *exec, char *str, int len, int code)
{
	char	*obj;
	int		status;

	obj = ft_substr(str, 0, len);
	status = handle_error(exec, obj, code);
	free(obj);
	return (status);
}
