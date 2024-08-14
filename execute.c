/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 07:26:26 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/14 07:28:52 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	exit_subprocess(t_shell *shell, char *obj, int errcode)
{
	// terminal setting 다시 보기
	if (errcode && errcode < CMD_NOT_FOUND)
	{
		errcode = EXIT_FAILURE;
		handle_error(obj, NULL, errcode);
	}
	else if (errcode == CMD_NOT_FOUND)
		handle_error(obj, NULL, errcode);
	if (errcode == SIGEXIT + SIGTERM)
		errcode = EXIT_SUCCESS;
	free_shell(*shell);
	exit(errcode);
}

void	exec_program(t_shell *shell, t_process p)
{
	char	**envp;

	envp = deqtostrs(shell->data.envps);
	if (!envp)
		exit_subprocess(shell, NULL, EXTRA_ERROR);
	if (execve(p.path, p.args, envp) == -1)
		exit_subprocess(shell, p.args[0], CMD_NOT_FOUND);
}

void	handler_heredoc(int signo)
{
	if (signo == SIGINT)
	{
		g_status = 1;
		rl_replace_line("", 0);
		exit(1);
	}
}

void	handler_heredoc_wait(int signo)
{
	if (signo == SIGINT)
	{
		replace_line(FALSE);
		g_status = 1;
	}
}

int	open_redirect(int redirect, char *word, char *link)
{
	int	fd;

	fd = 0;
	if (redirect == T_GREAT)
		fd = open(word, O_RDONLY);
	else if (redirect == T_DGREAT)
		fd = open(link, O_RDONLY);
	else if (redirect == T_LESS)
		fd = open(word, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	else if (redirect == T_DLESS)
		fd = open(word, O_CREAT | O_WRONLY | O_APPEND, 0666);
	return (fd);
}

int	write_files(char *file_name, char *line)
{
	int	fd;

	fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (fd == -1)
		return (EXTRA_ERROR);
	if (line[0])
		write(fd, line, ft_strlen(line));
	close(fd);
	return (EXIT_SUCCESS);
}

int	heredoc_process(char *link, char *limiter)
{
	int		status;
	char	*buffer;
	char	line[ARG_MAX];

	line[0] = 0;
	status = set_signal_init(handler_heredoc);
	write_files(link, line);
	rl_replace_line("", 0);
	buffer = readline("> ");
	while (buffer && ft_memcmp(buffer, limiter, ft_strlen(limiter) + 1))
	{
		ft_strlcat(line, buffer, ARG_MAX);
		ft_strlcat(line, "\n", ARG_MAX);
		free(buffer);
		rl_replace_line("", 0);
		buffer = readline("> ");
	}
	status = write_files(link, line);
	free(buffer);
	return (status);
}

//
int	wait_heredoc(t_process p)
{
	int	status;
	int	s;

	s = set_signal_init(handler_heredoc_wait);
	waitpid(p.pid, &status, 0);
	if (!s)
		s = set_signal_init(handler_sub);
	if (s)
		return (EXTRA_ERROR);
	if (WIFSIGNALED(status))
		return (EXIT_FAILURE);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

int	here_doc(char *link, char *limiter)
{
	t_process	p;

	if (fork_process(&p))
		return (EXIT_FAILURE);
	if (!p.pid)
		exit(heredoc_process(link, limiter));
	else
		return (wait_heredoc(p));
	
}

// 쓰는것
// free 및 close 함수에 unlink 추가 필요
//close_fd print_error 뒤에
int	set_filedoc(t_process *p)
{
	char	*itoa;

	if (!p->link)
	{
		itoa = ft_itoa(p->index);
		if (itoa)
			p->link = ft_strjoin("here_doc", itoa);
		if (!itoa || !p->link)
		{
			free(itoa);
			return (EXTRA_ERROR);
		}
		free(itoa);
	}
	return (EXIT_SUCCESS);
}

int	is_redirect(int type)
{
	if (type == T_GREAT || type == T_LESS || type == T_DGREAT || type == T_DLESS)
		return (TRUE);
	return (FALSE);
}

int	open_token(t_token *t, t_process *p)
{
	int			*fd;

	fd = &p->redirect_fd[t->type % 2];
	if (t->type == T_DGREAT)
	{
		p->flag = 1;
		if (here_doc(p->link, t->word))
			return (EXTRA_ERROR);
	}
	*fd = open_redirect(t->type, t->word, p->link);
	
	if (*fd == -1)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	find_redirect(t_process *p, t_token *t)
{
	int		status;

	if (!t)
		return (EXIT_SUCCESS);
	if (set_filedoc(p))
		return (EXTRA_ERROR);
	if (is_redirect(t->type))
	{
		if (p->redirect_fd[t->type % 2] > 0)
			close_fd(&p->redirect_fd[t->type % 2]);
		if (open_token(t, p))
			return (EXTRA_ERROR);
	}
	status = find_redirect(p, t->left);
	if (!status)
		status = find_redirect(p, t->right);
	return (status);
}

void	dup_fd(int *fd, int std)
{
	if (*fd > 0)
	{
		dup2(*fd, std);
		close_fd(fd);
	}
}

void	set_fd(t_shell *shell, size_t index)
{
	if (shell->p[index].redirect_fd[STDIN_FILENO] > 0)
		dup_fd(&shell->p[index].redirect_fd[STDIN_FILENO], STDIN_FILENO);
	else if (index && shell->p[index - 1].pipe_fd[STDIN_FILENO] > 0)
		dup_fd(&shell->p[index - 1].pipe_fd[STDIN_FILENO], STDIN_FILENO);
	if (shell->p[index].redirect_fd[STDOUT_FILENO] > 0)
		dup_fd(&shell->p[index].redirect_fd[STDOUT_FILENO], STDOUT_FILENO);
	else if (index != shell->p_size - 1 && shell->p[index].pipe_fd[STDOUT_FILENO] > 0)
		dup_fd(&shell->p[index].pipe_fd[STDOUT_FILENO], STDOUT_FILENO);
}

// child free
void	child(t_shell *shell, size_t index)
{
	int	ret;

	if (shell->p[index].pid)
		return ;
	close_pipe(shell, index);
	set_fd(shell, index);
	if (is_builtin(shell->p[index].path))
	{
		ret = exec_builtin(shell->p[index], &shell->data);
		if (!ft_memcmp(shell->p[index].path, "exit", 5) && !ret)
			ret = g_status;
		exit_subprocess(shell, NULL, ret);
	}
	else if (!shell->p[index].args[0][0])
	{
		// if (shell->p_size != 1)
		// 	exit_subprocess(shell, NULL, WAIT_TIMEOUT);
		free_shell(*shell);
		exit(g_status);
	}
	else
	{
		exec_program(shell, shell->p[index]);
	}
}

void	parent(t_shell *shell, size_t index)
{
	if (!shell->p[index].pid)
		return ;
	close_pipe(shell, index);
	waitpid(shell->p[index].pid, 0, WNOHANG);
}
