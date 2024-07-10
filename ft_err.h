#ifndef FT_ERR_H
# define FT_ERR_H
# ifndef FALSE
# define FALSE 0
# endif
# ifndef TRUE
# define TRUE 1
# endif
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include "./libft/libft.h"

# define MSG_ARGV "invalid arguments\n"
# define MSG_OPT "invalid argument or option\n"
# define MSG_IDF "not a valid identifier\n"
# define MSG_SET "not set\n"
# define MSG_NUM "numeric argument required\n"
# define MSG_CMD "command not found\n"

typedef enum e_err
{
	BUILTIN_ERROR = 2,
	INVALID_ARGV,
	INVALID_OPT,
	INVALID_IDF,
	NOT_SET,
	NOT_NUM,
	EXTRA_ERROR,
	CMD_NOT_FOUND = 127,
}	t_err;

int		handle_error(char *exec, char *obj, int code);
#endif