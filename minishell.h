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
# include "./src/parsing.h"
# define PATH_MAX 1024

// # define PROMPT_MSG "minishell$ "
# define PROMPT_MSG "\033[36mminishell ❯\033[0m "

volatile int	status;
typedef struct sigaction t_sigaction;
typedef	struct termios	t_termios;


typedef struct s_shell
{
	t_token			*t; // tree
	t_process		*p;
	size_t			p_size;
	t_data			data;
	t_termios		term;
}	t_shell;


/* main.c */
void		exit_process(t_shell *shell, char *obj, int errcode);
// void		child_handler(int signo);
char		**get_env_paths(char *envp[]);
void	terminal_printon(t_shell *shell);
void	terminal_printoff(void);

/* execute dir */
void		set_cmds(t_shell *shell);
void		set_process(t_shell *shell);
t_builtin	find_builtin(int index);
char		*read_val_strs(char *strs[], char *key);
int			set_cwd(char **cwd);

/* execute */
int		is_builtin(char *cmd);
int		exec_builtin(t_process p, t_data *d);
void	wait_process(t_shell *shell);
void	exec_program(t_shell *shell, t_process p);
void	set_process(t_shell *shell);
void	close_pipe(t_shell *shell, int index);
void	subprocess(t_shell *shell);
void	inprocess(t_shell *shell);
void   	exec_cmds(t_shell *shell);
void	dup_fd(int *fd, int std);

/* free.c */
void		free_token(t_token *t);
void		free_data(t_data d);
void		free_tprocess(t_process *p, size_t size);
void		free_shell(t_shell shell);
void 		free_cmds(t_token **t, t_process **p, size_t *psize);
void		clean_cmds(t_shell *shell);

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
void	terminal_printoff(void);
void	terminal_printon(t_shell *shell);

/* setting.c */
void	set_shell(t_shell *shell, char *envp[]);
int		set_token(t_token **t);
void	tokenization(t_shell *shell, t_token *t);
void	parent(t_shell *shell, size_t index);
void	child(t_shell *shell, size_t index);
char	*get_cmdpath(char **paths, char *cmd);
char	**get_cmdargs(char **cmd);
int		open_redirect(t_process *p, t_token *t);
void	open_pipe(t_shell *shell, size_t index);
size_t	find_pipe(t_token *t);

/* parsing.c */
void	parselines(t_shell *shell, char *buffer);

#endif
