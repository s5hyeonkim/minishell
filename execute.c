#include "minishell.h"

/* execute parsing*/

/* program */
	/* builtin */

	/* external */
void	exec_program(t_shell *shell, t_process p)
{
	char	**envp;

	//printf("external %s %s\n", p.path, p.args[0]);
	envp = deqtostrs(shell->data.envps);
	if (!envp)
		exit_process(shell, NULL, EXTRA_ERROR);
	// ft_putendl_fd(p.path, 2);
	// for (int i = 0; p.args[i] != 0; i++)
	// 	ft_putendl_fd(p.args[i], 2);
	if (execve(p.path, p.args, envp) == -1)
		exit_process(shell, p.args[0], CMD_NOT_FOUND);
}

void	handler_heredoc(int signo)
{
	if (signo == SIGINT)
		replace_line(false);
}

void	handler_heredoc_wait(int signo)
{
	if (signo == SIGINT)
		replace_line(TRUE);
}

int	set_handler_heredoc(void(*handler)(int), int signo)
{
	t_sigaction	action;

	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	if (sigaction(signo, &action, NULL) == (int) SIG_ERR)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	set_signal_heredoc(void(*handler)(int))
{
	int	status;

	status = 0;
	terminal_printoff();
	if (!status)
		status = set_handler_heredoc(handler, SIGINT);
	if (!status)
		status = set_handler_heredoc(handler, SIGTERM);
	if (!status)
		status = set_handler_heredoc(SIG_IGN, SIGQUIT);
	return (status);
}

int	open_redirect(int redirect, char *word, char *link)
{
	int fd;

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

	if (!line[0])
		return (EXIT_SUCCESS);
	fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (fd == -1)
		return (EXTRA_ERROR);
	write(fd, line, ft_strlen(line));
	close(fd);
	return (EXIT_SUCCESS);	
}

int	here_doc(char *link, char *limiter)
{
	t_process 	h;
	char		a[1024];
	char		*buffer;
	int			s;

	ft_memset(a, 0, sizeof(char) * 1024);
	if (fork_process(&h))
		return (EXIT_FAILURE);
	if (!h.pid)
	{
		s = set_signal_heredoc(handler_heredoc);
		buffer = readline("> ");
		while (buffer && ft_memcmp(buffer, limiter, ft_strlen(limiter) + 1))
		{
			ft_strlcat(a, buffer, 1024);
			ft_strlcat(a, "\n", 1024);
			free(buffer);
			buffer = readline("> ");
		}
		s = write_files(link, a);
		free(buffer);
		// printf("status: %d\n", s);
		exit(s);
	}
	else
	{
		set_signal_heredoc(handler_heredoc_wait);
		waitpid(h.pid, &s, 0);
		if (WIFSIGNALED(s))
			return (EXIT_SUCCESS);
		// printf("estatus: %d\n", WEXITSTATUS(s));
		if (WIFEXITED(s))
			return (WEXITSTATUS(s));
		set_signal_heredoc(handler_init);
	}
	return (EXIT_SUCCESS);
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
	// printf("dup %d %d\n", *fd, std);
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

void	child(t_shell *shell, size_t index)
{
	int	ret;

	if (shell->p[index].pid)
		return ;
	// printf("%s#\n%c#\n", shell->p[index].args[0], shell->p[index].args[0][0]);
	close_pipe(shell, index);
	set_fd(shell, index);
	if (is_builtin(shell->p[index].path))
	{
		ret = exec_builtin(shell->p[index], &shell->data);
		if (!ft_memcmp(shell->p[index].path, "exit", 5) && !ret)
			ret = status;
		exit_process(shell, NULL, ret);
	}
	else if (!shell->p[index].args[0][0])
	{
		if (shell->p_size != 1)
			exit_process(shell, NULL, WAIT_TIMEOUT);
		exit(0);
	}
	else 
		exec_program(shell, shell->p[index]);
}

void	parent(t_shell *shell, size_t index)
{
	if (!shell->p[index].pid)
		return ;
	close_pipe(shell, index);
	waitpid(shell->p[index].pid, 0, WNOHANG);
}

