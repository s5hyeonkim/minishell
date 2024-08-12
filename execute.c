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
	pid_t	pid;
	char	a[1024];
	// status = set_signal_heredoc(handler_heredoc);
	ft_memset(a, 0, sizeof(char) * 1024);
	pid = fork();
	if (pid == -1)
		return (-1);
	else if (!pid)
	{
		set_signal_heredoc(handler_heredoc);
		while (true)
		{
			//개행까지 들어가는지 확인 필요
			*buffer = readline("> ");
			if (*buffer == NULL)
				return (-1);
			if (!ft_memcmp(*buffer, limiter, ft_strlen(limiter) + 1))
			{
				free(*buffer);
				break ;
			}
			ft_strlcat(a, *buffer, 1024);
			ft_strlcat(a, "\n", 1024);
			free(*buffer);
		}
		write(fd, a, ft_strlen(a) - 1);
		exit(0);
	}
	else
	{
		waitpid(pid, 0, 0);
	}
	return (EXIT_SUCCESS);
}

int	open_token(t_token *t, char *link)
{
	int			fd;

	fd = 0;
	if (t->type == T_GREAT)
		fd = open(t->word, O_RDONLY);
	if (t->type == T_LESS)
		fd = open(t->word, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (t->type == T_DGREAT)
		fd = open(link, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (t->type == T_DLESS)
		fd = open(t->word, O_CREAT | O_APPEND | O_WRONLY, 0666);
	// printf("fd :%d\n", fd);
	return (fd);
}
// 쓰는것
// free 및 close 함수에 unlink 추가 필요
//close_fd print_error 뒤에
int	open_redirect(t_process *p, t_token *t)
{
	char	*num;
	char	*link;
	char 	*buffer;
	int		status;

	if (!t)
		return (EXIT_SUCCESS);
	// printf("type: %d\n", t->type);
	// printf("%p\n", t);
	num = ft_itoa(p->index);
	if (num)
		link = ft_strjoin("here_doc", num);
	if (!num || !link)
		return (EXIT_FAILURE);
	if (t->type == T_GREAT || t->type == T_DLESS ||	t->type == T_LESS || t->type == T_DGREAT)
	{
		if (p->redirect_fd[t->type % 2] > 2)
		{
			close(p->redirect_fd[t->type % 2]);
			p->redirect_fd[t->type % 2] = 0;
		}
		if (t->type % 2 == 0 && p->flag)
		{
			unlink(link);
			p->flag = 0;
		}
		p->redirect_fd[t->type % 2] = open_token(t, link);
		// printf("open %d\n", p->redirect_fd[t->type % 2]);
		if (p->redirect_fd[0] != -1 && t->type == T_DGREAT)
			here_doc(&buffer, t->word, p->redirect_fd[0]);
		if (t->type == T_DGREAT)
			p->flag = 1;
		// printf("%d %d\n", t->type, p->redirect_fd[t->type % 2]);
		return (p->redirect_fd[t->type % 2]);
	}
	free(link);
	free(num);
	status = open_redirect(p, t->left);
	if (status != -1)
		status = open_redirect(p, t->right);
	return (status);
}

void	dup_fd(int *fd, int std)
{
	if (*fd <= 2)
		return ;
	// printf("dup %d %d\n", *fd, std);
	dup2(*fd, std);
}

void	close_fd(int *fd)
{
	if (*fd <= 2)
		return ;
	close(*fd);
	*fd = 0;
}

void	set_fd(t_shell *shell, size_t index)
{
	// printf("red: input %d output %d\n", shell->p[index].redirect_fd[0], shell->p[index].redirect_fd[1]);
	if (shell->p[index].redirect_fd[STDIN_FILENO] > STDERR_FILENO)
	{
		// printf("redirect0 %d\n", shell->p[index].redirect_fd[STDIN_FILENO]);
		dup_fd(&shell->p[index].redirect_fd[STDIN_FILENO], STDIN_FILENO);
	}
	else if (index && shell->p_size > 1 && shell->p[index - 1].pipe_fd[STDIN_FILENO])
	{
		// printf("pipe0%zu %d\n", index - 1, shell->p[index - 1].pipe_fd[STDIN_FILENO]);
		dup2(shell->p[index - 1].pipe_fd[STDIN_FILENO], STDIN_FILENO);
	}
	if (shell->p[index].redirect_fd[STDOUT_FILENO] > STDERR_FILENO)
	{
		// printf("redirect1 %d\n", shell->p[index].redirect_fd[STDOUT_FILENO]);
		dup_fd(&shell->p[index].redirect_fd[STDOUT_FILENO], STDOUT_FILENO);
	}
	else if (index != shell->p_size - 1 && shell->p[index].pipe_fd[STDOUT_FILENO])
	{
		// printf("pipe1%zu %d\n", index, shell->p[index].pipe_fd[STDOUT_FILENO]);
		dup2(shell->p[index].pipe_fd[STDOUT_FILENO], STDOUT_FILENO);
	}
}

void	child(t_shell *shell, size_t index)
{
	int	ret;

	if (shell->p[index].pid)
		return ;
	// printf("try %zu %s# %s#\n", index, shell->p[index].path, shell->p[index].args[1]);
	set_fd(shell, index);
	if (index != shell->p_size - 1)
	{
		close(shell->p[index].pipe_fd[0]);
		shell->p[index].pipe_fd[0] = 0;
	}
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
		exit_process(shell, NULL, EXIT_SUCCESS);
	}
	else 
		exec_program(shell, shell->p[index]);
}

void	parent(t_shell *shell, size_t index)
{
	if (!shell->p[index].pid)
		return ;
	if (index != shell->p_size - 1)
	{
		close(shell->p[index].pipe_fd[1]);
		shell->p[index].pipe_fd[1] = 0;
	}
	if (index > 1)
	{
		close(shell->p[index - 1].pipe_fd[0]);
		shell->p[index - 1].pipe_fd[0] = 0;
	}
	waitpid(shell->p[index].pid, 0, WNOHANG);
}

