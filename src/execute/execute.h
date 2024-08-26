/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:49:06 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/21 17:17:32 by yubshin          ###   ########.fr       */
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
# include "../../utils/ft_signal.h"
# include "../parsing/parsing.h"
# include "../../utils/terminal.h"
# include "deque/deque.h"
# include "../../ft_err.h"
# include "../../utils/utils.h"

// getconf ARG_MAX
# define ARG_MAX 262144
# define PATH_MAX 1024

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

typedef struct s_fd
{
	int		in[2];
	int		pipe[2];
	int		redirect[2];
}	t_fd;

typedef struct s_exec
{
	char	*path;
	char	**args;
}	t_exec;

typedef struct s_sys
{
	char	*link;
	int		flag;
}	t_sys;

typedef struct s_process
{
	size_t	index;
	pid_t	pid;
	t_exec	exec;
	t_fd	fd;
	t_sys	file;
	t_token	t;
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
int	        set_env_pwd(t_deques *deqs, char **lcwd, char **lnwd);
void		parsing_dir(char *wd, char *now, size_t len);
int			set_cwd(char **cwd);

/*cd.c*/
int			ft_cd(t_process p, t_data *d);
/*cd_utils.c*/
char		*get_nextdir(char *path, char *cwd);
int			is_valid_folder(char *wd, char *now, char *next);

/*echo.c*/
int			ft_echo(t_process p, t_data *d);
/*env.c*/
int			ft_env(t_process p, t_data *d);
/*exit.c*/
int			ft_exit(t_process p, t_data *d);
/*export.c*/
int			ft_export(t_process p, t_data *d);

/* export_utils.c*/
int 		add_val(t_deques *envps, t_map *keyval, char *str);

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
int			is_folder(char	*to_dir);
int	        is_file(char *to_dir);

/*cmdline_utils.c*/
int			is_valid_quotation(size_t *start, int *open1, int open2);
int			set_parsing_deques(t_deques *deqs, char *cmd);

/*cmdline.c*/
char		*get_pathcmd(char **paths, char *cmd);
char		*get_cmdpath(char **paths, char *cmd);
char		**get_cmdargs(char **cmds);

/*execute.c*/
int			fork_process(t_process *p);
t_builtin	find_builtin(int index);
int			set_cmdargs(t_process *p, char **argvs);
int			set_cmdpath(t_process *p, char **paths, char *cmd);

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
int			here_doc(char *link, char *limiter, t_deques *envps);

/*redirect_utils.c*/
void		set_fd_builtin(t_process *p);
int			is_redirect(int type);
void		dup_fd(int *fd, int std);
void		close_fd(int *num);

/*redirect.c*/
int			open_redirect(int redirect, char *word, char *link);
int			find_redirect(t_process *p, t_token *t);
int			open_token(t_token *t, t_process *p);
int			set_redirect(t_process *p, t_deques *deqs);

#endif
