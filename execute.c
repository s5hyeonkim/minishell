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
	status = signo + SIGEXIT;
	if (signo == SIGINT)
		exit(1);
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

int	here_doc(char **buffer, char *limiter, int fd)
{
	t_process	p;
	char		a[1024];
	// status = set_signal_heredoc(handler_heredoc);
	if (fd <= 0)
		return (EXTRA_ERROR);
	ft_memset(a, 0, sizeof(char) * 1024);
	if (fork_process(&p))
		return (EXTRA_ERROR);
	if (!p.pid)
	{
		set_signal_heredoc(handler_heredoc);
		while (true)
		{
			//개행까지 들어가는지 확인 필요
			*buffer = readline("> ");
			if (*buffer == NULL)
				exit(1);
			if (!ft_memcmp(*buffer, limiter, ft_strlen(limiter) + 1))
			{
				free(*buffer);
				break ;
			}
			ft_strlcat(a, *buffer, 1024);
			// ft_strlcat(a, "\n", 1024);
			free(*buffer);
		}
		write(fd, a, ft_strlen(a) - 1);
		exit(0);
	}
	else
		waitpid(p.pid, 0, 0);
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

int	is_heredoc(int type)
{
	if (type == T_GREAT || type == T_LESS || type == T_DGREAT || type == T_DLESS)
		return (TRUE);
	return (FALSE);
}

int	open_token(t_token *t, t_process *p)
{
	int			*fd;
	char		*buffer;

	fd = &p->redirect_fd[t->type % 2];
	if (t->type == T_GREAT)
		*fd = open(t->word, O_RDONLY);
	if (t->type == T_LESS)
		*fd = open(t->word, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (t->type == T_DGREAT)
	{
		p->flag = 1;
		*fd = open(p->link, O_CREAT | O_TRUNC | O_RDWR, 0666);
		if (here_doc(&buffer, t->word, *fd))
			return (EXTRA_ERROR);
	}
	if (t->type == T_DLESS)
		*fd = open(t->word, O_CREAT | O_APPEND | O_WRONLY, 0666);
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
	if (is_heredoc(t->type))
	{
		if (p->redirect_fd[t->type % 2] > 2)
			close_fd(&p->redirect_fd[t->type % 2]);
		if (t->type % 2 == 0 && p->flag)
		{
			unlink(p->link);
			p->flag = 0;
		}
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

