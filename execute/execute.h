/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:49:06 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/14 18:47:15 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXECUTE_H
# define EXECUTE_H
# include <sys/types.h>
# include <dirent.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <curses.h>
# include "../src/ft_signal.h"
# include "../src/parsing.h"
# include "../utils/terminal.h"
# include "deque/deque.h"
# include "../ft_err.h"

// getconf ARG_MAX
# define ARG_MAX 262144
# define PATH_MAX 1024

volatile long	g_status;

typedef enum e_builtno
{
	_ECHO,
	_CD,
	_PWD,
	_EXPORT,
	_UNSET,
	_ENV,
	_EXIT
}	t_builtno;

typedef struct s_process
{
	pid_t	pid;
	char	*path;
	char	**args;
	char	*link;
	size_t	index;
	int		flag;
	int		fd[2];
	int		pipe_fd[2];
	int		redirect_fd[2];
}	t_process;

typedef struct s_data
{
	char		**paths;
	char		*lcwd;
	t_deques	*envps;
	long		status;
}	t_data;

typedef int		(*t_builtin)(t_process p, t_data *d);

/*builtin.c*/
int			is_builtin(char *cmd);
int			exec_builtin(t_process p, t_data *d);

/*cd_utils.c*/
int			check_folder(char *to_dir);
int			set_env_pwd(t_deques *deqs, char *key, char *val);
void		parsing_dir(char *wd, char *now, size_t len);
int			set_cwd(char **cwd);

// builtin function
/*cd.c*/
int			ft_cd(t_process p, t_data *d);
/*echo.c*/
int			ft_echo(t_process p, t_data *d);
/*env.c*/
int			ft_env(t_process p, t_data *d);
/*exit.c*/
int			ft_exit(t_process p, t_data *d);
/*export.c*/
int			ft_export(t_process p, t_data *d);
/*pwd.c*/
int			ft_pwd(t_process p, t_data *d);
/*unset.c*/
int			ft_unset(t_process p, t_data *d);
/*utils.c*/
char		*read_val_strs(char *strs[], char *key);
void		set_fd_builtin(t_process *p);
void		ft_sort(char **strs, int (*guide)(char *, char *));
int			is_valid_opt(char	*opt);
void		set_rwfd(t_process *p);

/*cmdline_utils.c*/
int			is_valid_quotation(size_t *start, int *open1, int open2);
int			set_parsing_deques(t_deques *deqs, char *cmd);

/*cmdline.c*/
char		*get_pathcmd(char **paths, char *cmd);
char		*get_cmdpath(char **paths, char *cmd);
char		**get_cmdargs(char **cmds);

/*execute.c*/
int			fork_process(t_process *p);
int			token_to_word(t_process *p, t_data d, t_token *t);
t_builtin	find_builtin(int index);

/*free.c*/
void		free_data(t_data d);
void		free_process(t_process *p, size_t size);
void		clean_process(t_process *p, size_t p_size);
void		clean_files(t_process *p, size_t p_size);

/*herdoc_signal.c*/
void		handler_heredoc(int signo);
void		handler_heredoc_wait(int signo);

/*heredoc.c*/
int			set_filedoc(t_process *p);
int			here_doc(char *link, char *limiter);

/*redirect_utils.c*/
void		set_fd_builtin(t_process *p);
int			is_redirect(int type);
void		dup_fd(int *fd, int std);
void		close_fd(int *num);

// redirect.c
int			open_redirect(int redirect, char *word, char *link);
int			find_redirect(t_process *p, t_token *t);
int			open_token(t_token *t, t_process *p);

#endif
