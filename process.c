#include "minishell.h"

void	wait_process(t_shell *shell)
{
	int	index;
	int	status;

	index = 0;
	while (shell->p[index].path)
	{
		waitpid(shell->p[index].pid, &status, 0);
		if (WIFEXITED(status))
			shell->data.status = WEXITSTATUS(status);
		index++;
	}
}

void	subprocess(t_shell *shell)
{
	size_t	index;

	index = 0;
	printf("execute in child process\n");
	set_signal_sub(shell, handler_sub); 
	while (index < shell->p_size)
	{
		open_pipe(shell, index);
		child(shell, index);
		parent(shell, index);
		index++;
	}
	wait_process(shell);
	set_signal_init(shell, handler_init);
}

void	set_fd_builtin(t_process *p)
{
	int	fd_in;
	int	fd_out;

	fd_in = dup(STDIN_FILENO);
	fd_out = dup(STDOUT_FILENO);
	if (p->redirect_fd[0] > 0)
		dup_fd(&p->redirect_fd[0], fd_in);
	if (p->redirect_fd[1] > 0)
		dup_fd(&p->redirect_fd[1], fd_out);
	p->redirect_fd[0] = fd_in;
	p->redirect_fd[1] = fd_out;
}

void	inprocess(t_shell *shell)
{
	long	status;

	printf("execute in current process\n");
	set_fd_builtin(shell->p);
	status = exec_builtin(shell->p[0], &shell->data);
	if (!ft_memcmp(shell->p[0].args[0], "exit", 5) && !status)
	{
		status = shell->data.status;
        clean_cmds(shell);
		exit(status);
	}
	shell->data.status = status;
}

char	**debug(char *path, char **args)
{
	size_t	index;
	size_t	size;
	size_t	add;
	char	**debug;

	index = 0;
	size = 0;
	add = 0;
	while (args[size])
		size++;
	debug = ft_calloc(size + 2 , sizeof(char *));
	if (ft_memcmp(path, args[0], ft_strlen(path) + 1))
		debug[add++] = ft_strdup(path);
	while (index < size)
	{
		debug[index + add] = args[index];
		index++;
	}
	debug[++index] = 0;
	return debug;
}

int	token_to_word(t_shell *shell, t_token *t, size_t index)
{
	t_process	*p;

	if (!t || t->type != T_CMD_WORD)
		return (EXIT_SUCCESS);
	p = &shell->p[index];
	p->args = get_cmdargs(t->argvs);
    p->path = get_cmdpath(shell->data.paths, t->word);
    if (!p->args || !p->path)
        return (EXTRA_ERROR);
    return (EXIT_SUCCESS);
}

int   token_to_process(t_shell *shell, t_token *t, size_t *index)
{
    int         status;

    status = EXIT_SUCCESS;
    if (t->type == T_SIMPLE_CMD)
    {
		if (token_to_word(shell, t->right, *index) || open_redirect(&shell->p[*index], t->left) == -1)
            return (EXTRA_ERROR);
		printf("%s\n", shell->p[*index].path);
		printf("%s\n", shell->p[*index].args[0]);
		printf("%s\n", shell->p[*index].args[1]);
		shell->p[*index].index = *index;
		*index += 1;
        return (EXIT_SUCCESS);
    }
    if (t->left)
        status = token_to_process(shell, t->left, index);
    else if (!status && t->right)
        status = token_to_process(shell, t->right, index);
    return (status);
}

void    exec_cmds(t_shell *shell)
{
    size_t  index;

    index = 0;
    shell->p_size = find_pipe(shell->t);
    shell->p = ft_calloc(shell->p_size + 1, sizeof(t_process));
    if (!shell->p || token_to_process(shell, shell->t, &index))
    {
        shell->data.status = EXIT_FAILURE;
        handle_error("", "", EXTRA_ERROR);
    }
    else if (shell->p_size == 1 && is_builtin(shell->p[0].path))
		inprocess(shell);
	else
		subprocess(shell);
}


// void print_token(t_token *t)
// {
// 	const char *d[] = {"", "|", "simple cmd", "cmd word", "redirects", ">>", "<<", ">", "<"};
// 	if (!t)
// 		return;
// 	printf("type is %s\n", d[t->type]);
// 	if (t->word)
// 		printf("token word: %s\n", t->word);
// 	print_token(t->left);
// 	if (t->right && t->right->type == T_SIMPLE_CMD)
// 		printf("\n\n\n");
// 	print_token(t->right);	
// }