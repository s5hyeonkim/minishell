/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_err.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:55:15 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/21 13:02:45 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ERR_H
# define FT_ERR_H
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include "./libft/libft.h"

# define MSG_ARGV "invalid arguments\n"
# define MSG_OPT "invalid options\n"
# define MSG_IDF "not a valid identifier\n"
# define MSG_SET "not set\n"
# define MSG_NUM "numeric argument required\n"
# define MSG_CMD "command not found\n"
# define MSG_SYN_TOK "syntax error near unexpected token\n"
# define MSG_SYN_TERM "syntax error: unexpected end of file\n"
# ifndef FALSE
#  define FALSE 0
# endif
# ifndef TRUE
#  define TRUE 1
# endif

typedef enum e_err
{
	BUILTIN_ERROR = 2,
	INVALID_ARGV,
	INVALID_OPT,
	INVALID_IDF,
	NOT_SET,
	NOT_NUM,
	EXTRA_ERROR,
	E_ISDIR = 21,
	CMD_NOT_FOUND = 127,
	SIGEXIT = 128,
	SYNTAX_ERROR = 258,
	SYN_TOK,
	SYN_TERM,
	SIGNALED
}	t_err;

int		handle_error(char *exec, char *obj, int code);
int		handle_error_withstr(char *exec, char *str, int len, int code);

#endif
