#ifndef MINISHELL_H
# define MINISHELL_H

//minishell 허용함수

#include "libft.h"

//기타함수
#include <stdio.h> //printf

typedef enum e_type
{
	CMD,
	H_RE,
	I_RE,
	O_RE,
	A_RE,
	PIPE,
	AND,
	OR
}	t_type;

typedef struct s_token
{
	t_type	type;
	char	*cmd; // or filename
	int  resp; // 실행의 책임이 있는 인덱스 번호
}	t_token;

typedef struct s_process
{
  int pipe_fd[2];
  pid_t pid;
}   t_process;

typedef struct s_exec
{
	t_token	*t;
	t_process *p;
	size_t size; //token, process size
}	t_exec;

#endif