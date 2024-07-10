#ifndef MINISHELL_H
# define MINISHELL_H

//허용함수 헤더
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <term.h>

# include "libft/libft.h"
# include "ft_err.h"
# include "./execute/deque/deque.h"
# define NAME_MAX 256
# define PATH_MAX 1024

typedef enum e_type
{
	CMD = 1,
	SIMPLE_CMD,
	REDIRECT,
	REDIRECTS,
	PIPE,
	AND,
	OR
}	t_type;

typedef struct s_token
{
	t_type			type;
	char			*cmd; // or filename
	struct s_token	*left;
	struct s_token	*right;
}	t_token;


typedef struct s_process
{
    pid_t   pid;
    char    *path;
    char    **args;
    int     flag;
	int		fd[2];
	t_token	t;
}   t_process;

typedef struct s_data
{
	char		**paths;	//path environment variables
	char		*home;
	t_deques	*envps;
}	t_data;

typedef struct s_exec
{
	t_token			*t;
	t_process		*p;
	size_t			size;
	int				status;
	t_data			data;
}	t_exec;

typedef int (*built_in)(t_exec *info, t_process p);

//minishell
void		exit_process(t_exec *info, char *obj, int errcode);
void		replace_lines(void);
void    	child_handler(int signo);
void		set_signal(t_exec *info, void(*func)(int));

//execute
void		exec_cmds(t_exec *info);
void		set_cmds(t_exec *info);
void		set_process(t_exec *info);
built_in	find_builtin(int index);
char		*read_val_strs(char *strs[], char *key);
int			set_pwd(char **cwd);
int			ft_pwd(t_exec *info, t_process p);
int			ft_env(t_exec *info, t_process p);
int			ft_exit(t_exec *info, t_process p);
int 		ft_export(t_exec *info, t_process p);
int 		ft_cd(t_exec *info, t_process p);
int			ft_echo(t_exec *info, t_process p);
int			ft_unset(t_exec *info, t_process p);

// free
void		free_token(t_token *t);
void		free_data(t_data d);
void		free_tprocess(t_process *p, size_t size);
void		free_info(t_exec info);

#endif
