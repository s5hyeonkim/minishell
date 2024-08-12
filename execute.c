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
	if (execve(p.path, p.args, envp) == -1)
		exit_process(shell, p.args[0], CMD_NOT_FOUND);
}

int	here_doc(char **buffer, char *limiter, int fd)
{
	while (true)
	{
		//개행까지 들어가는지 확인 필요
		*buffer = readline("heredoc>");
		if (*buffer == NULL)
			return (EXTRA_ERROR);
		if (!ft_memcmp(*buffer, limiter, ft_strlen(limiter) + 1))
			break ;
		write(fd, *buffer, ft_strlen(*buffer));
	}
	return (EXIT_SUCCESS);
}

int	open_token(t_token *t, char *link)
{
	int			fd;
	char		*buffer;

	fd = 0;
	if (t->type == T_GREAT)
		fd = open(t->word, O_RDONLY);
	if (t->type == T_LESS)
		fd = open(t->word, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (t->type == T_DGREAT)
		fd = open(link, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (t->type == T_DLESS)
		fd = open(t->word, O_CREAT | O_APPEND | O_WRONLY, 0666);
	if (fd != -1 && t->type == T_DGREAT)
		here_doc(&buffer, t->word, fd);
	printf("open %d\n", t->type);
	return (fd);
}
// 쓰는것
// free 및 close 함수에 unlink 추가 필요
//close_fd print_error 뒤에
int	open_redirect(t_process *p, t_token *t)
{
	char	*num;
	char	*link;

	if (!t)
		return (EXIT_SUCCESS);
	num = ft_itoa(p->index);
	if (num)
		link = ft_strjoin("here_doc", num);
	if (!num || !link)
		return (EXIT_FAILURE);
	if (t->type == T_GREAT || t->type == T_DLESS ||	t->type == T_LESS || t->type == T_DGREAT)
	{
		if (p->redirect_fd[t->type % 2] > 2)
			close(p->redirect_fd[t->type % 2]);
		if (t->type % 2 == 0)
		{
			unlink(link);
			p->flag = 0;
		}
		p->redirect_fd[t->type % 2] = open_token(t, link);
		if (t->type == T_DGREAT)
			p->flag = 1;
		return (p->redirect_fd[t->type % 2]);
	}
	free(link);
	free(num);
	if (t->left && open_redirect(p, t->left))
		return (EXIT_FAILURE);
	if (t->right && open_redirect(p, t->right))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	dup_fd(int *fd, int std)
{
	if (*fd <= 2)
		return ;
	dup2(*fd, std);
	close(*fd);
	*fd = 0;
}

void	set_fd(t_shell *shell, size_t index)
{
	// printf("red: input %d output %d\n", shell->p[index].redirect_fd[0], shell->p[index].redirect_fd[1]);
	if (shell->p[index].redirect_fd[STDIN_FILENO] > STDERR_FILENO)
		dup_fd(&shell->p[index].redirect_fd[STDIN_FILENO], STDIN_FILENO);
	else if (index && shell->p[index - 1].pipe_fd[STDIN_FILENO])
		dup_fd(&shell->p[index - 1].pipe_fd[STDIN_FILENO], STDIN_FILENO);
	if (shell->p[index].redirect_fd[STDOUT_FILENO] > STDERR_FILENO)
		dup_fd(&shell->p[index].redirect_fd[STDOUT_FILENO], STDOUT_FILENO);
	else if (index != shell->p_size - 1 && shell->p[index].pipe_fd[STDOUT_FILENO])
		dup_fd(&shell->p[index].pipe_fd[STDOUT_FILENO], STDOUT_FILENO);
}

void	child(t_shell *shell, size_t index)
{
	int	ret;

	if (shell->p[index].pid)
		return ;
	set_fd(shell, index);
	if (is_builtin(shell->p[index].path))
	{
		ret = exec_builtin(shell->p[index], &shell->data);
		if (!ft_memcmp(shell->p[index].path, "exit", 5) && !ret)
			ret = shell->data.status;
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
	if (index && index != shell->p_size - 1)
	{
		close(shell->p[index].pipe_fd[1]);
		shell->p[index].pipe_fd[1] = 0;
	}
	waitpid(shell->p[index].pid, 0, WNOHANG);
}
