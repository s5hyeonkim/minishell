#ifndef FT_ERR_H
# define FT_ERR_H

typedef enum e_err
{
	MALLOC_FAILED = 1,
	INVALID_ARGV,
	SIG_ERROR,
	FORK_FAILED,
	FILE_NOT_EXIST,
	EXTRA_ERROR,
	CMD_NOT_FOUND = 127,
}	t_err;

#endif