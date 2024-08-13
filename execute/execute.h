/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:49:06 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/23 19:51:58 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXECUTE_H
# define EXECUTE_H
# include <sys/types.h>
# include <dirent.h>
# include "deque/deque.h"
# include "../ft_err.h"
# define PATH_MAX 1024
volatile long	status;

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

typedef int	(*t_builtin)(t_process p, t_data *d);

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
int			is_valid_option(char *opt);
void	set_rwfd(t_process p, int *num, int write);

#endif