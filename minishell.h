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
# include <sys/signal.h>
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
# include "./execute/execute.h"
# define NAME_MAX 256
# define PATH_MAX 1024
# define SIGEXIT 128
// # define PROMPT_MSG "minishell$ "
# define PROMPT_MSG "\033[36mminishell ❯\033[0m "

volatile int	status;
typedef struct sigaction t_sigaction;
typedef	struct termios	t_termios;

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
	char		*user;
	t_deques	*envps;
}	t_data;

typedef struct s_shell
{
	t_token			*t;
	t_process		*p;
	size_t			p_size;
	int				status;
	t_data			data;
	t_termios		term;
}	t_shell;

typedef int (*built_in)(t_shell *shell, t_process p);


/* main.c */
void		exit_process(t_shell *shell, char *obj, int errcode);
// void		child_handler(int signo);
char		**get_env_paths(char *envp[]);
void		child(t_shell *shell, int index);
void		parent(t_shell *shell, int index);
void		exec_cmds(t_shell *shell);
void	set_terminal_printon(t_shell *shell);
void	set_terminal_printoff(void);

/* execute dir */
void		exec_cmds(t_shell *shell);
void		set_cmds(t_shell *shell);
void		set_process(t_shell *shell);
built_in	find_builtin(int index);
char		*read_val_strs(char *strs[], char *key);
int			set_cwd(char **cwd);
int			ft_pwd(t_shell *shell, t_process p);
int			ft_env(t_shell *shell, t_process p);
int			ft_exit(t_shell *shell, t_process p);
int 		ft_export(t_shell *shell, t_process p);
int 		ft_cd(t_shell *shell, t_process p);
int			ft_echo(t_shell *shell, t_process p);
int			ft_unset(t_shell *shell, t_process p);

/* execute */
int	is_builtin(char *cmd);
int	exec_builtin(t_shell *shell, t_process p);
void	wait_process(t_shell *shell);
void	exec_program(t_shell *shell, t_process p);
void	set_process(t_shell *shell);
void	close_pipe(t_shell *shell, int index);
void	subprocess(t_shell *shell);
void	inprocess(t_shell *shell);

/* free.c */
void		free_token(t_token *t);
void		free_data(t_data d);
void		free_tprocess(t_process *p, size_t size);
void		free_shell(t_shell shell);

/* signal.c */
void		set_signal(t_shell *shell, void(*handler)(int), int signo);
void		set_signal_init(t_shell *shell, void(*handler)(int));
void		set_signal_sub(t_shell *shell, void(*handler)(int));
void		handler_init(int signo);
void		handler_sub(int signo);

/* signal_utils.c  */
void	move_cursor(void);
void	replace_line(int redisplayon);

/* terminal.c */
void	init_terminal(t_shell *shell);
void	set_terminal_printoff(void);
void	set_terminal_printon(t_shell *shell);

/* setting.c */
void	set_shell(t_shell *shell, char *envp[]);

#endif
