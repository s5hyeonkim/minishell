#include "minishell.h"

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

int	is_redirect(int type)
{
	if (type == T_GREAT || type == T_LESS || type == T_DGREAT || type == T_DLESS)
		return (TRUE);
	return (FALSE);
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

void	dup_fd(int *fd, int std)
{
	if (*fd > 0)
	{
		dup2(*fd, std);
		close_fd(fd);
	}
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