/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 07:22:42 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/27 11:41:11 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <curses.h>

# include "libft/libft.h"
# include "ft_err.h"
# include "./src/execute/deque/deque.h"
# include "./src/execute/execute.h"
# include "./src/parsing/parsing.h"
# define PROMPT_MSG "\033[36mminishell ❯\033[0m "

typedef struct sigaction	t_sigaction;
typedef struct s_shell
{
	t_token			*t;
	t_process		*p;
	size_t			p_size;
	t_data			data;
	t_termios		term;
}	t_shell;

/* free.c */
void		free_token(t_token *t);
void		free_shell(t_shell shell);
void		clean_buffer(t_shell *shell);
void		exit_process(t_shell *shell, char *obj, int errcode);
void		exit_wo_error(t_shell *shell, int errcode);

/* execute.c */
void		exit_subprocess(t_shell *shell, char *obj, int errcode);
void		exec_program(t_shell *shell, t_process p);
void		child(t_shell *shell, size_t index);
void		parent(t_shell *shell, size_t index);

/* setting.c */
void		set_shell(t_shell *shell, char *envp[]);
int			set_env_paths(t_data *d);

/* process.c */
void		exec_cmds(t_shell *shell);

/* process_utils.c */
int			set_args(t_process *p, t_data d);
int			open_pipe(t_process *p, size_t size);
size_t		find_pipe(t_token *t);
void		set_fd(t_shell *shell, size_t index);
void		close_pipe(t_shell *shell, size_t index);

/* set_cmd.c */
int			set_cmd(t_shell *shell);

/* terminal.c */
void		get_terminal(t_shell *shell);
void		reset_terminal(t_shell *shell);

/* parsing.c */
int			parselines(t_token **token, t_deques *envps, char *buffer);

/*validation.c*/
void		check_valid(t_shell *shell, int argc);

#endif
