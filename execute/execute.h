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
# include <sys/wait.h>
# include <signal.h>
# include <sys/signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
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

t_builtin	find_builtin(int index);
char		*read_val_strs(char *strs[], char *key);
int			set_cwd(char **cwd);
int			ft_pwd(t_process p, t_data *d);
int			ft_env(t_process p, t_data *d);
int			ft_exit(t_process p, t_data *d);
int			ft_export(t_process p, t_data *d);
int			ft_cd(t_process p, t_data *d);
int			ft_echo(t_process p, t_data *d);
int			ft_unset(t_process p, t_data *d);
void		set_rwfd(t_process p, int *num, int write);
int			check_folder(char *to_dir);
int			set_env_pwd(t_deques *deqs, char *key, char *val);
void		parsing_dir(char *wd, char *now, size_t len);
void		ft_sort(char **strs, int (*guide)(char *, char *));
int			is_valid_opt(char	*opt);
int			exec_builtin(t_process p, t_data *d);
int			is_builtin(char *cmd);
void		replace_line(int redisplayon);

// cmdline_utils.c
int			set_parsing_deques(t_deques *deqs, char *cmd);
void		handler_heredoc(int signo);
void		handler_heredoc_wait(int signo);
char		*get_pathcmd(char **paths, char *cmd);
char		*get_cmdpath(char **paths, char *cmd);
char		**get_cmdargs(char **cmds);

#endif
