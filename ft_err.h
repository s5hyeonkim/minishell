#ifndef FT_ERR_H
# define FT_ERR_H
# ifndef FALSE
# define FALSE 0
# endif
# ifndef TRUE
# define TRUE 1
# endif
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>

typedef enum e_err
{
	MALLOC_FAILED = 1,
	INVALID_ARGV,
	SIG_ERROR,
	FORK_FAILED,
	PIPE_FAILED,
	FILE_NOT_EXIST,
	PWD_ERROR,
	EXTRA_ERROR,
	CMD_NOT_FOUND = 127,
}	t_err;

void	print_objerr(char *obj, int code);
void	print_err(int code);
void	print_msg(char *obj, int code);

#endif