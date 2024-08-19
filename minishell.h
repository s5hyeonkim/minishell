/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 07:22:42 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/14 11:22:27 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <curses.h>

# include "libft/libft.h"
# include "ft_err.h"
# include "./execute/deque/deque.h"
# include "./execute/execute.h"
# include "./src/parsing.h"
# define WAIT_TIMEOUT 258
// # define PROMPT_MSG "minishell$ "
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

/* main.c */
void		exit_process(t_shell *shell, char *obj, int errcode);
// void		child_handler(int signo);
char		**get_env_paths(char *envp[]);

/* execute dir */
void		set_cmds(t_shell *shell);
void		get_terminal(t_shell *shell);
void		reset_terminal(t_shell *shell);

void		set_process(t_shell *shell);
t_builtin	find_builtin(int index);
char		*read_val_strs(char *strs[], char *key);
/* execute */
void		wait_process(t_shell *shell);
void		exec_program(t_shell *shell, t_process p);
void		set_process(t_shell *shell);
void		close_pipe(t_shell *shell, size_t index);
void		subprocess(t_shell *shell);
void		inprocess(t_shell *shell);
void		exec_cmds(t_shell *shell);
void		clean_files(t_process *p, size_t p_size);
/* free.c */
void		free_token(t_token *t);
void		free_data(t_data d);
void		free_process(t_process *p, size_t size);
void		free_shell(t_shell shell);
void		clean_process(t_process *p, size_t size);
void		exit_wo_error(t_shell *shell, int errcode);



/* signal_utils.c  */
void		move_cursor(void);


/* setting.c */
void		set_shell(t_shell *shell, char *envp[]);
// int			set_token(t_token **t);
// void		tokenization(t_shell *shell, t_token *t);
void		parent(t_shell *shell, size_t index);
void		child(t_shell *shell, size_t index);
int			fork_process(t_process *p);
int			find_redirect(t_process *p, t_token *t);
int			open_pipe(t_process *p, size_t size);
size_t		find_pipe(t_token *t);
int			wait_heredoc(t_process p);
void		set_fd(t_shell *shell, size_t index);
void		dup_fd(int *fd, int std);
int			here_doc(char *link, char *limiter);
int			open_token(t_token *t, t_process *p);

/* parsing.c */
int			parselines(t_shell *shell, char *buffer);

#endif
