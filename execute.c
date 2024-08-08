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

int	open_token(t_token *t)
{
	int	fd;

	fd = 0;
	if (t->type == T_GREAT || t->type == T_DGREAT)
		fd = open(t->word, O_RDONLY);
	if (t->type == T_LESS)
		fd = open(t->word, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (t->type == T_DLESS)
		fd = open(t->word, O_CREAT | O_APPEND | O_WRONLY, 0666);
	printf("t->word: %s %d\n", t->word, fd);
	return (fd);
}

//close_fd print_error 뒤에
int	open_redirect(t_process *p, t_token *t)
{
	int	status;

	status = EXIT_SUCCESS;
	if (!t)
		return (EXIT_SUCCESS);
	if (t->type == T_GREAT || t->type == T_DGREAT || \
	t->type == T_DLESS ||	t->type == T_LESS)
	{
		if (p->redirect_fd[t->type % 2] > 0)
			close(p->redirect_fd[t->type % 2]);
		p->redirect_fd[t->type % 2] = open_token(t);
		return (p->redirect_fd[t->type % 2]);
	}
	if (t->left)
		status = open_redirect(p, t->left);
	if (status >= 0 && t->right)
		status = open_redirect(p, t->right);
	printf("word %s, in %d out %d\n", p->path, p->redirect_fd[0], p->redirect_fd[1]);
	return (status);
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
	printf("red: input %d output %d\n", shell->p[index].redirect_fd[0], shell->p[index].redirect_fd[1]);
	if (shell->p[index].redirect_fd[0] > 2)
		dup_fd(&shell->p[index].redirect_fd[0], 0);
	else if (index && shell->p[index - 1].pipe_fd[0])
		dup_fd(&shell->p[index - 1].pipe_fd[0], 0);
	if (shell->p[index].redirect_fd[1] > 2)
		dup_fd(&shell->p[index].redirect_fd[1], 1);
	else if (index != shell->p_size - 1 && shell->p[index].pipe_fd[1])
		dup_fd(&shell->p[index].pipe_fd[1], 1);
}

void	child(t_shell *shell, size_t index)
{
	int	ret;

	if (shell->p[index].pid)
		return ;
	if (index != shell->p_size - 1)
	{
		close(shell->p[index].pipe_fd[0]);
		shell->p[index].pipe_fd[0] = 0;
	}
	set_fd(shell, index);
	if (is_builtin(shell->p[index].path))
	{
		ret = exec_builtin(shell->p[index], &shell->data);
		if (!ft_memcmp(shell->p[index].path, "exit", 5) && !ret)
			ret = shell->data.status;
		exit_process(shell, NULL, ret);
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
