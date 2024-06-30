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
# include <errno.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <term.h>

# include "libft/libft.h"
# include "ft_err.h"
# include "./execute/deque/deque.h"
// external folder 경로
# define external "./execute/"

typedef enum e_type
{
	CMD,
	SIMPLE_CMD,
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
	t_deques	*envps;
}	t_data;

typedef struct s_exec
{
	t_token			*t;
	t_process		*p;
	t_data			data;
}	t_exec;

typedef int (*built_in)(t_exec *info, t_process p);

//libft
char	*get_next_line(int fd);
void	*ft_calloc(size_t count, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t	ft_strlen(const char *str);
void	*ft_memset(void *ptr, int c, size_t len);

//minishell
void	exit_process(t_exec *info, char *obj, int errcode);
void	free_info(t_exec info);

//execute
char		*check_pathenv(char **paths, char *cmd);
built_in	select_builtin(int index);
char		*get_value(char *strs[], char *key);
int			change_envs(t_deques *deqs, char *str);
int     	ft_echo(t_exec *info, t_process p);
int     	ft_cd(t_exec *info, t_process p);
char		*get_pwd(t_exec *info);
int     	ft_pwd(t_exec *info, t_process p);
int     	ft_export(t_exec *info, t_process p);
int     	ft_unset(t_exec *info, t_process p);
int     	ft_env(t_exec *info, t_process p);
int     	ft_exit(t_exec *info, t_process p);
#endif
